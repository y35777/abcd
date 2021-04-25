#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "meteracc_task.h"
#include "meter_index.h"
#include "meter_file.h"
#include "meter_acc.h"
#include "resource.h"
#include <time.h>

CMeterAccTask::CMeterAccTask()
{
    m_szMeterAccSrcDir[0]   = '\0';
    m_szMeterAccLocalDir[0] = '\0';
    m_szMeterAccBakDir[0]   = '\0';
    m_szMeterAccBSDir[0]    = '\0';
    m_szPrefix[0]           = '\0';
    m_szSuffix[0]           = '\0';
    m_nMeterTables          = 0;
    m_nSubmitTimeInd        = 0;
    m_pSrcFmt               = NULL;  
    m_pDestFmt              = NULL;
    m_pIndexFile            = NULL;
    
    m_pIndexFile            = NULL;
    m_pReadEntry            = NULL;
    m_bForTTAT              = 1;

	m_bRunAcc               = TRUE; 
    m_ThreadExitEvt.reset();
}

CMeterAccTask::~CMeterAccTask()
{
    if (m_pSrcFmt)
    {
        delete m_pSrcFmt;
        m_pSrcFmt = NULL;
    }

    if (m_pDestFmt)
    {
        delete m_pDestFmt;
        m_pDestFmt = NULL;
    }

    if (m_pIndexFile)
    {
        delete m_pIndexFile;
        m_pIndexFile = NULL;
    }

    METER_FILE_MAP::iterator it = m_mapAccFile.begin();
    while (it != m_mapAccFile.end())
    {
        if (NULL != (*it).second)
        {
            delete (*it).second;
        }        

        it++;
    }

    if (m_pReadEntry)
    {
        delete []m_pReadEntry;
        m_pReadEntry = NULL;
    }
}

/****************************************************************************
函数原型	protected:int CMeterAccTask::Init(const char* pszRootDir)
功能描述	计次表累加启动前的初始化工作
输入参数	pszRootDir  ---  指向程序绝对路径指针
输出参数	无 
返回值	    0     ---成功
            非零值---失败
抛出异常	无
*****************************************************************************/
int CMeterAccTask::Init(const char* pszRootDir)
{
    if (NULL == pszRootDir)
    {
        return -1;
    }

    char szCfgFile[MAX_PATH];
    sprintf(szCfgFile, "%s/config/ini/igwb.ini", pszRootDir);

    CINIFile CfgFile(szCfgFile);
    if (TRUE != CfgFile.Open())
    {
        TRACE(MTS_METERACC, S_BS_FAIL_OPEN_FILE, szCfgFile);
        return -1;
    }

    m_nBackupDays   = CfgFile.GetInt(CFG_SEC_METER_ACC,
                                     METER_ACC_DAYS_FOR_BACKUP, 
                                     METER_ACC_DAYS_FOR_BACKUP_DEFAULT);
    //默认本地保存90天，至少90天，最多1年
    if ((m_nBackupDays < METER_ACC_DAYS_FOR_BACKUP_DEFAULT) 
     || (m_nBackupDays > 365))
    {
        m_nBackupDays = METER_ACC_DAYS_FOR_BACKUP_DEFAULT;
    }

    m_bDebug   = CfgFile.GetInt(CFG_SEC_METER_ACC,
                                METER_ACC_DEBUG, 
                                METER_ACC_DEBUG_DEFAULT);

    //如果是调试版本，则m_nBackupDays的单位按分钟对待
    if (m_bDebug)
    {   
        m_nBackupDays *= 1;    //换算成秒
    }
    //如果是正式版本，则m_nBackupDays的单位按天对待
    else
    {
        //1天 =  86400秒
        m_nBackupDays *= 86400; //换算成秒 
    }

    m_bForTTAT = CfgFile.GetInt(CFG_SEC_METER_ACC,
                                METER_ACC_FOR_TTAT, 
                                METER_ACC_FOR_TTAT_DEFAULT);

    CfgFile.GetString(CFG_SEC_METER_ACC, 
                      CFG_METER_ACC_SRC_DIR, 
                      CFG_METER_ACC_SRC_DIR_DEFAULT, 
                      m_szMeterAccSrcDir, 
                      MAX_PATH);

     CfgFile.GetString(CFG_SEC_METER_ACC,
                      CFG_METER_ACC_LOCAL_DIR,
                      CFG_METER_ACC_LOCAL_DIR_DEFAULT,
                      m_szMeterAccLocalDir,
                      MAX_PATH);

    if (TRUE != CreateDeepDir(m_szMeterAccLocalDir))
    {
        TRACE(MTS_METERACC, S_CREATE_DIR_FAILED, m_szMeterAccLocalDir);
        return -1;
    }

    CfgFile.GetString(CFG_SEC_METER_ACC,
                      CFG_METER_ACC_BAK_DIR,
                      CFG_METER_ACC_BAK_DIR_DEFAULT,
                      m_szMeterAccBakDir,
                      MAX_PATH);

    if (TRUE != CreateDeepDir(m_szMeterAccBakDir))
    {
        TRACE(MTS_METERACC, S_CREATE_DIR_FAILED, m_szMeterAccBakDir);
        return -1;
    }
    
    CfgFile.GetString(CFG_SEC_METER_ACC,
                      CFG_METER_ACC_BS_DIR,
                      CFG_METER_ACC_BS_DIR_DEFAULT,
                      m_szMeterAccBSDir,
                      MAX_PATH);

    if (TRUE != CreateDeepDir(m_szMeterAccBSDir))
    {
        TRACE(MTS_METERACC, S_CREATE_DIR_FAILED, m_szMeterAccBSDir);
        return -1;
    }

    CfgFile.GetString(CFG_SEC_METER_ACC,
                      CFG_METER_ACC_PREFIX,
                      CFG_METER_ACC_PREFIX_DEFAULT,
                      m_szPrefix,
                      32);

    CfgFile.GetString(CFG_SEC_METER_ACC,
                      CFG_METER_ACC_SUFFIX,
                      CFG_METER_ACC_SUFFIX_DEFAULT,
                      m_szSuffix,
                      32);
    
    m_nMeterTables   = CfgFile.GetInt(CFG_SEC_METER_ACC,
                                      CFG_METER_ACC_TABLES, 
                                      CFG_METER_ACC_TABLES_DEFAULT);

    m_nSubmitTimeInd = CfgFile.GetInt(CFG_SEC_METER_ACC,
                                      CFG_METER_ACC_TIME_CATEGORY, 
                                      CFG_METER_ACC_TIME_CATEGORY_DEFAULT);

    if (m_nMeterTables <= 0)
    {
        return -1;
    }

    if (m_nSubmitTimeInd <= 0)
    {
        return -1;
    }

    //读入计次表表底提交时间参数
    for (int i = 0; i < m_nSubmitTimeInd; i++)
    {
        char szParameter[64];
        char szParaValue[128];

        sprintf(szParameter, METER_ACC_SUBMIT_TIME, i);
        CfgFile.GetString(CFG_SEC_METER_ACC, 
                          szParameter, 
                          METER_ACC_SUBMIT_TIME_DEFAULT, 
                          szParaValue, 
                          128);
        SSubmitTime st;
        char* pszTmp = strchr(szParaValue, ':');
        if (NULL == pszTmp)
        {
            st.nSubmitDay  = (BYTE)atoi(szParaValue);
            st.nSubmitHour = (BYTE)23;
        }
        else if (pszTmp > szParaValue)
        {
            char szHour[128];
            strncpy(szHour, pszTmp, 128);
            szHour[127] = '\0';

            szParaValue[pszTmp - szParaValue] = '\0';    
            st.nSubmitDay  = (BYTE)atoi(szParaValue);

            if (strlen(szHour) <= 1)
            {
                st.nSubmitHour = (BYTE)23;
            }
            else
            {
                pszTmp = szHour;
                pszTmp++;
                st.nSubmitHour  = (BYTE)atoi(pszTmp);
            }
        }
        else
        {
            st.nSubmitDay = 15;
        }

        if (st.nSubmitDay > 30) 
        {
            st.nSubmitDay = 30;
        }
            
        if (st.nSubmitHour > 23)
        {
            st.nSubmitHour = 23;
        }

        m_mapSubmitTime.insert(SUBMIT_TIMES_MAP::value_type(i, st));
    }

    //格式库初始化
    char szFmtRootDir[MAX_PATH];
    sprintf(szFmtRootDir, "%s/config/format", GetAppPath());

    m_pSrcFmt  = new CFormat(szFmtRootDir, METER_ACC_SRC_FORMAT_NAME);
    if (0 != m_pSrcFmt->Init())
    {
        delete m_pSrcFmt;
        m_pSrcFmt = NULL;
        
        TRACE(MTS_METERACC, S_FMT_INIT_FAILED, METER_ACC_SRC_FORMAT_NAME);

        return -1;
    }
    
    m_pDestFmt = new CFormat(szFmtRootDir, METER_ACC_DEST_FORMAT_NAME);
    if (0 != m_pDestFmt->Init())
    {
        delete m_pSrcFmt;
        m_pSrcFmt = NULL;
        
        delete m_pDestFmt;
        m_pDestFmt = NULL;
        TRACE(MTS_METERACC, S_FMT_INIT_FAILED, METER_ACC_DEST_FORMAT_NAME);

        return -1;
    }

    m_pDestFmt->SetSrcFmt(m_pSrcFmt);
    
    unsigned long nSrcBillLen   = m_pSrcFmt->GetFmtLen();
    
    if (nSrcBillLen < (sizeof(unsigned long) * m_nMeterTables + SUBSCRIBER_NUM_LEN + 1))
    {
        delete m_pSrcFmt;
        m_pSrcFmt = NULL;

        delete m_pDestFmt;
        m_pDestFmt = NULL;

        TRACE(MTS_METERACC, S_INVALID_FMT_LEN, METER_ACC_DEST_FORMAT_NAME, nSrcBillLen);

        return -1;
    }
    
    m_pReadEntry  = new unsigned char[nSrcBillLen];

    CfgFile.GetString(CFG_SEC_METER_ACC,
                      CFG_METER_ACC_INDEX_DIR,
                      CFG_METER_ACC_INDEX_DIR_DEFAULT,
                      m_szIndexFile,
                      MAX_PATH);
    
    if (TRUE != CreateDeepDir(m_szIndexFile))
    {
        delete m_pReadEntry;
        m_pReadEntry = NULL;

        delete m_pSrcFmt;
        m_pSrcFmt = NULL;

        delete m_pDestFmt;
        m_pDestFmt = NULL;

        TRACE(MTS_METERACC, S_CREATE_DIR_FAILED, m_szIndexFile);
        return -1;
    }

    strcat(m_szIndexFile, "/meteracc.idx");
    m_pIndexFile = new CMeterAccIndex;
    if (0 != m_pIndexFile->Init(m_szIndexFile))
    {
        delete m_pIndexFile;
        m_pIndexFile = NULL;

        delete m_pReadEntry;
        m_pReadEntry = NULL;

        delete m_pSrcFmt;
        m_pSrcFmt = NULL;

        delete m_pDestFmt;
        m_pDestFmt = NULL;

        return -1;
    }

    return 0;
}

int CMeterAccTask::open(void* args)
{
    if (0 != Init((char *)args))
    {
        return -1;
    }

    if (this->activate() != 0)
    {
        return -1;
    }

    return 0;
}

int CMeterAccTask::svc()
{
    return ProcessThread();
}

/* 计次表累加处理主线程 */
int CMeterAccTask::ProcessThread()
{
    while (m_bRunAcc)
    {
        ACE_DIR *pDir       = NULL;
        dirent *pDirent     = NULL;
        
        //打开目录失败，有可能mtr_proc在iGWB激活之前就已经开始运行
        if (NULL == (pDir = ACE_OS::opendir(m_szMeterAccSrcDir)))
        {
            Sleep(60);      //休眠一分钟

            continue;
        }
        
        while (NULL != (pDirent = ACE_OS::readdir(pDir)))
        {                    
            if ((strcmp(pDirent->d_name, ".") == 0) 
             || (strcmp(pDirent->d_name, "..") == 0))
            {
                continue;
            }

            char szSrcFile[MAX_PATH];
            sprintf(szSrcFile, "%s/%s", m_szMeterAccSrcDir, pDirent->d_name);

            ACE_stat st;
            ACE_OS::stat(szSrcFile, &st);
            if ((st.st_mode & S_IFMT) == S_IFDIR)
            {
                continue;
            }
            
		    //处理一个计次表文件
            if (0 != ProcessAFile(szSrcFile))
            {
                ACE_OS::closedir(pDir);
                pDir = NULL;
                return -1;
            }            
            else
            {
			    //成功处理完一个文件后，立即删除这个文件
                if (!FileDel(szSrcFile))
                {
                    TRACE(MTS_METERACC, S_BACKUP_DEL_FILE_FAILURE, szSrcFile);
                    ACE_OS::closedir(pDir);
                    pDir = NULL;
                    return -1;
                }
            }
			
		}	
		//看是否要提交
		if (0 != SubmitTimeCheck())
		{
			return -1;
		}

        if (pDir)
        {
            ACE_OS::closedir(pDir);
            pDir = NULL;
        }    
        
        DelOutdatedMeterFile();//删除过期的本地备份表底文件
		 
		Sleep(1*29);           //休眠29秒 //3分钟
    }

    return 0;
}

int CMeterAccTask::Sleep(const int nSeconds)
{
    ACE_Time_Value nSleepTime(nSeconds);
    nSleepTime += ACE_OS::gettimeofday();

    return m_ThreadExitEvt.wait(&nSleepTime);
}

/*
   处理一个用户计次表文件
*/
int CMeterAccTask::ProcessAFile(const char* pszSrcFile)
{
	unsigned long nZero=0;       //归零变量                 
    char szFileName[MAX_PATH];
    unsigned long nMeterShift = SUBSCRIBER_NUM_LEN + 1;	

    //以读写方式打开原文件
    FILE* pSrcFile = fopen(pszSrcFile, "rb+");
    if (NULL == pSrcFile)
    {
        TRACE(MTS_METERACC, S_BS_FAIL_OPEN_FILE, pszSrcFile);
        return -1;
    }
    
	//把文件内容一次读到一个大缓冲区中
	unsigned long nSrcPacketLen = 0;
	if (0 != fseek(pSrcFile, 0, SEEK_END))
    {
        fclose(pSrcFile);
        return -1;
    }

    nSrcPacketLen = ftell(pSrcFile);

    if (0 != fseek(pSrcFile, 0, SEEK_SET))
    {
        fclose(pSrcFile);
        return -1;
    }
	unsigned char *szSrcFileBuf = new unsigned char[nSrcPacketLen];
	unsigned char *szSrcFileBufTmp = szSrcFileBuf;
    if (1 != fread(szSrcFileBuf, nSrcPacketLen, 1, pSrcFile))
	{
		fclose(pSrcFile);
		delete []szSrcFileBuf;
		return -1;
	          
	}
	if (0 != fseek(pSrcFile, 0, SEEK_SET))
    {
        fclose(pSrcFile);
        return -1;
    }
    //以后的操作就对这个大缓冲区进行


    //获取原始话单的长度
    unsigned long nSrcBillLen = m_pSrcFmt->GetFmtLen();
    unsigned long nSrcBillSize = nSrcPacketLen/nSrcBillLen; 
	
    while (nSrcBillSize)
    {
		//在大缓冲区中取一张话单
		memcpy(m_pReadEntry,szSrcFileBufTmp,nSrcBillLen);
		szSrcFileBufTmp += nSrcBillLen;
		nSrcBillSize--;		
        

        //找到对应的计次表累加表底文件(临时文件)
        CMeterFile* pMeterFile = NULL;
        unsigned char nCatetory = m_pReadEntry[SUBSCRIBER_NUM_LEN];
		//处理0xFF 后的乱码 
		for(int k = 0; k< SUBSCRIBER_NUM_LEN; k++)
		{
			//处理高四位发现为F的情况
			if((m_pReadEntry[k] & 0xF0) == 0xF0)
			{
				m_pReadEntry[k] = 0xFF; 
				k++;
				while(k < SUBSCRIBER_NUM_LEN )
				{
					m_pReadEntry[k++] = 0xFF;//结束符后都赋0xFF
				}                 
				break;
			}
			//处理低四位发现为F的情况
			if((m_pReadEntry[k] & 0x0F) == 0x0F)
			{
				k++;
				while(k < SUBSCRIBER_NUM_LEN )
				{
					m_pReadEntry[k++] = 0xFF;//结束符后都赋0xFF
				}                 
				break;
			}
		}
		//结束	
        METER_FILE_MAP::iterator itfile = m_mapAccFile.find(nCatetory);
		
        if ((itfile == m_mapAccFile.end()) || (NULL == (*itfile).second))
        {
            sprintf(szFileName, "%s/meter%d.acc", m_szMeterAccLocalDir, nCatetory);
            
            pMeterFile = new CMeterFile(szFileName,
                                        m_pIndexFile,
                                        m_pSrcFmt,
                                        m_pDestFmt,
                                        m_bForTTAT,
                                        m_nMeterTables);

            if (0 != pMeterFile->Init())
            {
                fclose(pSrcFile);
    
                return -1;
            }

            m_mapAccFile.insert(METER_FILE_MAP::value_type(nCatetory, pMeterFile));

            SUBMIT_TIMES_MAP::iterator ittime = m_mapSubmitTime.find(nCatetory);
            if (ittime == m_mapSubmitTime.end())
            {
                SSubmitTime st;
                st.nSubmitDay  = 15;
                st.nSubmitHour = 23;
                m_mapSubmitTime.insert(SUBMIT_TIMES_MAP::value_type(nCatetory, st));
            }
        }
        else
        {
            pMeterFile = (*itfile).second;   
        }
		
        if (0 != pMeterFile->ProcessARecord(m_pReadEntry, nSrcBillLen))
        {
            fclose(pSrcFile);
            return -1;
        }
		
		/* 直接修改计次表中相应值的记录 
		* 清除操作记录计次表值字段
		*/ 
        for (int i = m_nMeterTables - 1; i >= 0; i--)
        {   			
            memcpy(&m_pReadEntry[nMeterShift + i * sizeof(unsigned long)], 
				   &nZero, sizeof(unsigned long));
        }
		
        //写入pSrcFile后,文件指针向后偏移 nSrcBillLen         
        if (1 != fwrite(m_pReadEntry, nSrcBillLen, 1, pSrcFile))
        {
            fclose(pSrcFile);
            return -1;
        }

	
		if(0 == (nSrcBillSize % METER_ACC_FFLUSH_TIME))
		{	
			fflush(pSrcFile);     //刷新文件(pSrcFile)
			pMeterFile->Flush();				
		}
	}
	
	if(szSrcFileBuf)
	{
		delete []szSrcFileBuf;
		szSrcFileBuf = NULL;
		szSrcFileBufTmp= NULL;
	}
	
	fclose(pSrcFile);   //一张表单处理完关闭文件
	
	return 0;
}

/*
  向计费中心提交表底文件。
*/
int CMeterAccTask::SubmitAFile(const int nIndex)
{
    METER_FILE_MAP::iterator it = m_mapAccFile.find(nIndex);
    
    if ((it == m_mapAccFile.end()) || (NULL == (*it).second))
    {
        return -1;
    }
    
    if ((NULL == m_pSrcFmt) || (NULL == m_pDestFmt))
    {
        return -1;
    }

    CMeterFile* pMeterFile = (*it).second;

    time_t lTime;
    struct tm *pTime;

    time(&lTime);
    pTime = localtime(&lTime);

    char szTmpFile[MAX_PATH];
    char szBSFile[MAX_PATH];
    char szBackupFile[MAX_PATH];

    if (0 == strlen(m_szSuffix))
    {
        sprintf(szTmpFile, "%s/%s%d.tmp", m_szMeterAccLocalDir, m_szPrefix, nIndex);
        sprintf(szBSFile, "%s/%s%d", m_szMeterAccBSDir, m_szPrefix, nIndex);
        sprintf(szBackupFile, "%s/%s%d_%04d%02d%02d%02d%02d%02d", 
                m_szMeterAccBakDir, 
                m_szPrefix, 
                nIndex,
                pTime->tm_year + 1900,
                pTime->tm_mon + 1,
                pTime->tm_mday,
                pTime->tm_hour,
                pTime->tm_min,
                pTime->tm_sec);

    }
    else
    {
        sprintf(szTmpFile, "%s/%s%d.%s.tmp", m_szMeterAccLocalDir, m_szPrefix, nIndex, m_szSuffix);
        sprintf(szBSFile, "%s/%s%d.%s", m_szMeterAccBSDir, m_szPrefix, nIndex, m_szSuffix);
        sprintf(szBackupFile, "%s/%s%d_%04d%02d%02d%02d%02d%02d.%s", 
                m_szMeterAccBakDir, 
                m_szPrefix, 
                nIndex,
                pTime->tm_year + 1900,
                pTime->tm_mon + 1,
                pTime->tm_mday,
                pTime->tm_hour,
                pTime->tm_min,
                pTime->tm_sec,
                m_szSuffix);
    }

    if (0 != pMeterFile->Submit(szTmpFile, szBSFile, szBackupFile))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/* 
   计次表底提交时间检查。
*/
int CMeterAccTask::SubmitTimeCheck()
{
    METER_FILE_MAP::iterator itfile = m_mapAccFile.begin();

    while (itfile != m_mapAccFile.end())
    {
        int nIndex = (*itfile).first;

        SUBMIT_TIMES_MAP::iterator ittime = m_mapSubmitTime.find(nIndex);
        if (ittime == m_mapSubmitTime.end())
        {
            return -1;
        }

        SSubmitTime st = (SSubmitTime)((*ittime).second);

        if (m_bDebug)
        {
            if (0 != SubmitAFile(nIndex))
            {
                return -1;
            }
        }
        else
        {
            time_t lTime;
            struct tm *pTime;
            time(&lTime);
            pTime = localtime(&lTime);
            if ((pTime->tm_mday == st.nSubmitDay) 
             && (pTime->tm_hour == st.nSubmitHour)
             && (pTime->tm_min <= 5)) //考虑秒的影响休眠要大于5			 		 
            {
                if (0 != SubmitAFile(nIndex))
                {
                    return -1;
                }
                Sleep(6.5*60);    //休眠6.5分钟，确保一天不重复提交
            }
        }

        itfile++;
    }

    return 0;
}

void CMeterAccTask::StopTask(void)
{
	m_bRunAcc               = FALSE;  //退出计次表累加处理主线程循环
    m_ThreadExitEvt.signal();

    wait();
}

/*  删除过期的本地备份表底文件 */
int CMeterAccTask::DelOutdatedMeterFile(void)
{
    ACE_DIR *pDir       = NULL;
    dirent *pDirent     = NULL;
    
    //打开目录失败
    if (NULL == (pDir = ACE_OS::opendir(m_szMeterAccBakDir)))
    {
        return 0;
    }
    
    while ((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {                    
        if ((strcmp(pDirent->d_name, ".") == 0) 
         || (strcmp(pDirent->d_name, "..") == 0))
        {
            continue;
        }        

        char szBackupFile[MAX_PATH];
        sprintf(szBackupFile, "%s/%s", m_szMeterAccBakDir, pDirent->d_name);

        ACE_stat st;
        ACE_OS::stat(szBackupFile, &st);
        if ((st.st_mode & S_IFMT) == S_IFDIR)
        {
            continue;
        }        
    
        time_t lTime;
       
        time(&lTime);
        localtime(&lTime);
        
        if ((m_bDebug) && ((lTime - st.st_ctime) > m_nBackupDays))
        {
            if (!FileDel(szBackupFile))
            {
                TRACE(MTS_METERACC, S_BACKUP_DEL_FILE_FAILURE, szBackupFile);
                return -1;
            }
        }
        else if ((lTime - st.st_ctime) > m_nBackupDays )//modify by zkq 2004-07-26
        {
            if (!FileDel(szBackupFile))
            {
                TRACE(MTS_METERACC, S_BACKUP_DEL_FILE_FAILURE, szBackupFile);
                return -1;
            }
        }

    }

    return 0;
}

