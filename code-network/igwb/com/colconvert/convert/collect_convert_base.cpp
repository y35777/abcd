#include "collect_convert_base.h"
#include <assert.h>

#include "../../include/toolbox.h"
#include "../include/collect_convert_include.h"
#include "include/collect_convert_define.h"
#include "../../include/format_out.h"

//以下是采集操作的基类CCollectConvertBase的定义
//构造函数
CCollectConvertBase::CCollectConvertBase(int nSwitchIndex) //构造函数传入协议对象指针
{
    m_nSwitchIndex = nSwitchIndex;
    m_pIGWB = NULL;

    m_szSwitchName[0] = '\0';
    m_szCollectId[0] = '\0';
    m_szCfgPath[0] = '\0';

    m_pIBillProcessor = NULL;
}

//析构函数
CCollectConvertBase::~CCollectConvertBase()
{
    //
    if (m_pIGWB != NULL)
    {
        delete m_pIGWB;
        m_pIGWB = NULL;
    }
}

int CCollectConvertBase::Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("采集任务(%s)转换模块打开配置文件(%s)失败", 
              m_szSwitchName, 
              m_szCfgPath);
        return ERR_FAIL;
    }


    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName, 
             sizeof(szSecName), 
             "%s%d", 
             CFG_CO_SEC_SWITCH, 
             m_nSwitchIndex);
    szSecName[CFG_BUF_SIZE - 1] = '\0';

    //获取交换机名
    INIFile.GetString(szSecName,
                      CFG_CO_NAME, 
                      CFG_CO_NAME_DEFAULT, 
                      m_szSwitchName, 
                      sizeof(m_szSwitchName));

    //获取交换机类型
    INIFile.GetString(szSecName,
                      CFG_CO_TYPE, 
                      CFG_CO_TYPE_DEFAULT, 
                      m_szCollectId, 
                      sizeof(m_szCollectId));

    //接入点号
    m_nAPID = CCollectorAdapter::GetProcessType() - PT_CO_BASE + 1;

    //初始化m_pIBillProcessor
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/config/format", GETAPPPATH);

    m_pIGWB = new CIGWB(szIGWBPath);

    if (m_pIGWB == NULL)
    {
        COTRACE("采集对象(%s)创建IGWB对象失败,配置文件路径(%s)",
            m_szSwitchName,
            szIGWBPath);

        return ERR_FAIL;
    }

    m_pIBillProcessor = (IBillProcessor* )m_pIGWB->QueryInterface(IID_FP_IBILLPROCESSOR);

	if (NULL == m_pIBillProcessor)
    {
        COTRACE("采集对象(%s)创建IBillProcessor对象失败,配置文件路径(%s)",
            m_szSwitchName,
            szIGWBPath);

        return ERR_FAIL;
    }

    int nResult = m_pIBillProcessor->Init(m_nAPID);     
    if (0 != nResult)
    {
        COTRACE("采集对象(%s)初始化IBillProcessor对象(AP=%d)失败,配置文件路径(%s)",
            m_szSwitchName,
            m_nAPID,
            szIGWBPath);

        return ERR_FAIL;
    }

    return ERR_SUCCESS;

}

int CCollectConvertBase::Process(const char* szBillFile, const int& nConvertInfo)
{
    int nRet = ERR_FAIL;

    //创建最终目录
    nRet = CreateLocSubDir(); 

    //转换话单文件
    SConvertFileReq stConvertReq;
    memset(&stConvertReq, 0, sizeof(stConvertReq));

    strncpy(stConvertReq.szSrcName, szBillFile, sizeof(stConvertReq.szSrcName));
    stConvertReq.nConvertInfo = nConvertInfo;

    nRet = Convert(stConvertReq);

    if (ERR_SUCCESS != nRet)
    {
        SInnerAlarm Alarm;
        Alarm.yAlarmType    = AT_EVENT;
        Alarm.uAlarmID      = ALARM_ID_CONVERT_FILE_FAULT;
        Alarm.yAlarmPara[0] = ALARM_PARA_NONE;
        SENDALARM(&Alarm);
    }


    //清除过期文件
    if (ERR_SUCCESS != ClearOldFile())
    {
        WRITELOG_F("采集对象(%s)删除过期的最终话单文件失败",
                   m_szSwitchName);
        COMSGOUT(MSGOUT_LEVEL_URGENT, 
               "采集对象(%s)删除过期的最终话单文件失败",
               m_szSwitchName);
    }

    return nRet;

}

int CCollectConvertBase::Convert(SConvertFileReq& stReq)
{
    int nSubObjIndex = stReq.nConvertInfo;

    if (m_SubObjMap.find(nSubObjIndex) != m_SubObjMap.end())
    {
        return m_SubObjMap[nSubObjIndex]->Convert(stReq);
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
            "采集对象(%s)转换话单文件时找不到的转换子对象(%d)",
            m_szSwitchName,
            nSubObjIndex);

        COTRACE("采集对象(%s)转换话单文件时找不到转换子对象(%d)",
            m_szSwitchName,
            nSubObjIndex);

    }

    return ERR_FAIL;

}

int CCollectConvertBase::CreateLocSubDir()
{
    int nRet = ERR_SUCCESS;
    MAP<int, CCollectConvertSubObj*>::iterator i = m_SubObjMap.begin();
    for(; i != m_SubObjMap.end(); i++)
    {
        if(ERR_SUCCESS != i->second->CreateLocSubDir())
        {
            nRet = ERR_FAIL;
        }
    }

    return nRet;

}

int CCollectConvertBase::ClearOldFile()
{
    int nRet = ERR_SUCCESS;
    MAP<int, CCollectConvertSubObj*>::iterator i = m_SubObjMap.begin();
    for(; i != m_SubObjMap.end(); i++)
    {
        if(ERR_SUCCESS != i->second->ClearOldFile())
        {
            nRet = ERR_FAIL;
        }
    }

    return nRet;

}

//added by chenliangwei
//清除指定目录下的过期子目录
int CCollectConvertBase::ClearOldDateSubDir(const char* szRootDir, 
                                      const int nSaveDays)
{
	if(nSaveDays <= 0)
    {
		return ERR_SUCCESS;
    }

	
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    //打开szRootDir目录
    pDir = ACE_OS::opendir(szRootDir);
    if(pDir == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, 
            "打开目录(%s)失败(Errno: %d)", 
            szRootDir, 
            errno);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTmpBuf);
        COTRACE(szTmpBuf);

        return ERR_FAIL;
    }

    //遍历指定根目录下的子目录，并删除过期的目录
    int nRes = ERR_SUCCESS;
	char szPath[MAX_PATH];

	while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
		SNPRINTF(szPath,MAX_PATH,"%s/%s",szRootDir, pDirent->d_name);
		szPath[MAX_PATH - 1] = 0;
	
		if(strcmp(pDirent->d_name,".") == 0
		   || strcmp(pDirent->d_name,"..") == 0)
        {
            continue;
        }

		ACE_OS::stat(szPath, &sDirStat);
		int nElapsed = 0;
		if(S_IFDIR == (sDirStat.st_mode & S_IFDIR) && IsDigitValue(pDirent->d_name))
		{
            //将目录转换为时间
			time_t curTime,DirTime;
			int nDateNum = ACE_OS::atoi(pDirent->d_name);
			tm tTmp;
			memset(&tTmp,0,sizeof(tm));
		    tTmp.tm_year = (nDateNum / 10000) - 1900;
            tTmp.tm_mon = (nDateNum % 10000)/100 - 1;
			tTmp.tm_mday = nDateNum %100;
			DirTime = mktime(&tTmp); //得到目录时间的基准值

			//取当前时间
			time(&curTime);
			tTmp = *localtime(&curTime);
			tTmp.tm_hour = 0;
			tTmp.tm_min = 0;
			tTmp.tm_sec = 0;
			curTime = mktime(&tTmp); //得到当前的日期，时分秒都为0

            //判断目录时间是否已过期
			nElapsed = difftime(curTime,DirTime);
			if(nElapsed >= (nSaveDays * 24 * 3600))
			{
                SListOut listout;
                nRes = GetLocalFileList(szPath, listout);

                if (nRes != ERR_SUCCESS)
                {
                    WRITELOG_F("删除过期的最终话单目录%s失败", 
                               szPath);
                    COMSGOUT(MSGOUT_LEVEL_URGENT, 
                           "删除过期的最终话单目录%s失败", 
                           szPath);

              		nRes = ERR_FAIL;
                }

                while(listout.FileList.size() > 0)
                {
                    char szFilePath[MAX_PATH];
                    SNPRINTF(szFilePath, 
                             MAX_PATH, 
                             "%s/%s", 
                             szPath, 
                             listout.FileList.front().szFileName);

                    if (FileDel(szFilePath))
                    {
                        WRITELOG_C("删除过期的最终话单文件%s", 
                                   szFilePath);
                        COMSGOUT(MSGOUT_LEVEL_SIDLY, 
                               "删除过期的最终话单文件%s", 
                               szFilePath);
                    }
                    else
                    {
                        WRITELOG_F("删除过期的最终话单文件%s失败", 
                                   szFilePath);
                        COMSGOUT(MSGOUT_LEVEL_URGENT, 
                               "删除过期的最终话单文件%s失败", 
                               szFilePath);
              		    nRes = ERR_FAIL;
                    }

                    listout.FileList.erase(listout.FileList.begin());
                }
 
				rmdir(szPath);
			}
		}
	}

	//关闭目录
	ACE_OS::closedir(pDir);
	return nRes;
}
//added end

//added by chenliangwei
//得到本地指定目录下的文件列表
int  CCollectConvertBase::GetLocalFileList(const char* szSrcDir, SListOut &pOut)
{
    //得到指定目录下所有文件
    ACE_DIR *pdir = ACE_OS::opendir(szSrcDir);
    dirent *pdirent;
    
    //李杜芳 修改 12.17
    if (pdir==NULL)
    {
        return ERR_FAIL;
    }
    //修改完毕
    
    while (pdirent = ACE_OS::readdir(pdir))
    {
		if(strcmp(pdirent->d_name,".") == 0
		   || strcmp(pdirent->d_name,"..") == 0)
        {
            continue;
        }

        SFileInfo fi;
        strncpy(fi.szFileName, pdirent->d_name, sizeof(fi.szFileName));
        fi.szFileName[sizeof(fi.szFileName) - 1] = '\0';
        fi.nFileSize = 0;
        fi.nAttr     = 0;
        pOut.FileList.push_back(fi);
    }

    ACE_OS::closedir(pdir);

    return ERR_SUCCESS;
}
//added end


