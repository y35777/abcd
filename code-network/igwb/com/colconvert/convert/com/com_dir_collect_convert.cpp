#include "com_dir_collect_convert.h"
#include "../comm_convert_fun.h"
#include "../5ess/5ess_convert_fun.h"
#include "zte_convert_fun.h"
#include "cc08_convert_fun.h"
#include "tellin_convert_fun.h"
#include "../../../include/format_out.h"
#include <math.h>

#define ORG_METER_NAME   "meter.sum"
#define CUR_METER_NAME   "meter_%s.sum"
#define DATE_METER_NAME  "meter_%s_%s.sum"
#define CONFIG_METERSUM  "metersum_config.ini"
#define CFG_SECTION_NAME "metersum_task"
#define TASK_COUNT       "task_count"


//以下是采集操作的CComDirCollectConvert的定义
//构造函数
CComDirCollectConvert::CComDirCollectConvert(int nSwitchIndex,
                                             int nDirIndex,
                                             IBillProcessor* pIBillProcessor)
                                            : CCollectConvertSubObj(nSwitchIndex,
                                                                    nDirIndex,
                                                                    pIBillProcessor)
{
    sprintf(m_szSubObjName, "%s%d", CFG_CO_SEC_COL_DIR, nDirIndex); 
}

//析构函数
CComDirCollectConvert::~CComDirCollectConvert()
{
    //
}


int CComDirCollectConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //根据根据不同的话单类型指定不同转换函数
    if (StringiCmp(m_szCollectId, CO_TYPE_CC08))
    {
        m_pConvertFun = ProcCC08File;
    }
    else if (StringiCmp(m_szCollectId, CO_TYPE_ZXJ))
    {
        if (StringiCmp(m_szBillType, CO_BILL_TYPE_DETAIL))
        {
            m_pConvertFun = ProcessZTEFile;
        }
        else if (StringiCmp(m_szBillType, CO_BILL_TYPE_METER))
        {
            m_pConvertFun = ProcessZTEMeterFile;
        }
        else
        {
            m_pConvertFun = ProcessOtherFile;
        }
    }
    else if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "采集对象(%s, DIR=%d)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        COTRACE("采集对象(%s, DIR=%d)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        return ERR_FAIL;

    }

    return ERR_SUCCESS;
}

//以下是采集操作的基类CTellinDirConvert的定义
//构造函数
CSJZTellinDirConvert::CSJZTellinDirConvert(int nSwitchIndex,
                                     int nDirIndex,
                                     IBillProcessor* pIBillProcessor)
									 :CTellinDirConvert(nSwitchIndex,
									 nDirIndex,
									 pIBillProcessor)
{
}

//析构函数
CSJZTellinDirConvert::~CSJZTellinDirConvert()
{
    //
}


int CSJZTellinDirConvert::Init()
{
    int nRet = CTellinDirConvert::Init();
    
    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }
    char szAPSection[40];
	char szChlSection[40];

	CINIFile cfgIniFile(GetCfgFilePath());
    cfgIniFile.Open();
    sprintf(szAPSection, "%s%d", CFG_SEC_AP, CCollectorAdapter::GetProcessType() - PT_CO_BASE + 1);
	
    char szFinalRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR,
                         CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                         szFinalRootDir, MAX_PATH);

	//获得状态文件的路径
	char szPathName[MAX_PATH];
#ifdef _PLATFORM_WIN32
	SNPRINTF(szPathName,
		sizeof(szPathName),
		"d:/other/collect/%s",
		m_szCollectId);
#else
	SNPRINTF(szPathName,
		sizeof(szPathName),
		"/var/other/collect/%s",
		m_szCollectId);
#endif
	CreateDeepDir(szPathName);
    
    //跳过default通道
    LIST<STRING>::iterator iter = m_ChlNames.begin();
	iter++ ; 
    SChlInfo stDefChlInfo;
	m_ChlInfoList.push_back(stDefChlInfo);

    //从Channel%d-1开始读prefix区段
    for (int i = 1; iter != m_ChlNames.end(); iter++ , ++i)
	{
		SNPRINTF(szChlSection, 
                 sizeof(szChlSection), 
                 CFG_SEC_CHANNEL, 
                 CCollectorAdapter::GetProcessType() - PT_CO_BASE + 1, 
                 i);
        szChlSection[sizeof(szChlSection) - 1] = '\0';

        SChlInfo stChlInfo;

        //从配置文件中读取文件序列号长度，配置参数的读取原则见函数头中的备注
        stChlInfo.nCsnLen = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                      CFG_CSN_LEN, CFG_CSN_LEN_DEFAULT);
        stChlInfo.nCsnLen = cfgIniFile.GetInt(szAPSection, CFG_CSN_LEN, stChlInfo.nCsnLen);
        stChlInfo.nCsnLen = cfgIniFile.GetInt(szChlSection, CFG_CSN_LEN, stChlInfo.nCsnLen);

        //从配置文件中读取起始序列号，配置参数的读取原则见函数头中的备注
        stChlInfo.uFileCsnStart = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                         CFG_START_CSN, CFG_START_CSN_DEFAULT);
        stChlInfo.uFileCsnStart = cfgIniFile.GetInt(szAPSection,
                                         CFG_START_CSN, stChlInfo.uFileCsnStart);
        stChlInfo.uFileCsnStart = cfgIniFile.GetInt(szChlSection,
                                         CFG_START_CSN, stChlInfo.uFileCsnStart);

        //从配置文件中读取结束序列号，配置参数的读取原则见函数头中的备注
        UINT4 uDefaultEndCsn;
        if (stChlInfo.nCsnLen >= 10 || stChlInfo.nCsnLen < 0)
        //由于UINT4的大小限制，所以最大的序列号只支持到9个9
        {
            stChlInfo.nCsnLen = 9;
            uDefaultEndCsn = 999999999;
        }
        else if (stChlInfo.nCsnLen == 0)
        {
            uDefaultEndCsn = 999999999;
        }
        else
        {
            uDefaultEndCsn = pow((double)10, stChlInfo.nCsnLen) - 1;
        }
        stChlInfo.uFileCsnEnd = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                          CFG_END_CSN, uDefaultEndCsn);
        stChlInfo.uFileCsnEnd = cfgIniFile.GetInt(szAPSection, CFG_END_CSN, stChlInfo.uFileCsnEnd);
        stChlInfo.uFileCsnEnd = cfgIniFile.GetInt(szChlSection, CFG_END_CSN, stChlInfo.uFileCsnEnd);

        //从配置文件中读取序列号步长，配置参数的读取原则见函数头中的备注
        stChlInfo.uFileCsnStep = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                                   CFG_CSN_STEP, CFG_CSN_STEP_DEFAULT);
        stChlInfo.uFileCsnStep = cfgIniFile.GetInt(szAPSection,
                                           CFG_CSN_STEP, stChlInfo.uFileCsnStep);
        stChlInfo.uFileCsnStep = cfgIniFile.GetInt(szChlSection,
                                           CFG_CSN_STEP, stChlInfo.uFileCsnStep);

        //从配置文件中读取正式文件的前缀
        cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_PREFIX, CFG_PREFIX_DEFAULT,
						     stChlInfo.szFormalPrefix, sizeof(stChlInfo.szFormalPrefix));
        cfgIniFile.GetString(szAPSection, CFG_PREFIX, stChlInfo.szFormalPrefix,
						     stChlInfo.szFormalPrefix, sizeof(stChlInfo.szFormalPrefix));
		cfgIniFile.GetString(szChlSection, CFG_PREFIX, stChlInfo.szFormalPrefix, 
                             stChlInfo.szFormalPrefix, sizeof(stChlInfo.szFormalPrefix));	

        //从配置文件中读取正式文件的中缀
        cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_MIDFIX, CFG_MIDFIX_DEFAULT,
						     stChlInfo.szFormalMidfix, sizeof(stChlInfo.szFormalMidfix));
        cfgIniFile.GetString(szAPSection, CFG_MIDFIX, stChlInfo.szFormalPrefix,
						     stChlInfo.szFormalMidfix, sizeof(stChlInfo.szFormalMidfix));
		cfgIniFile.GetString(szChlSection, CFG_MIDFIX, stChlInfo.szFormalMidfix, 
                             stChlInfo.szFormalMidfix, sizeof(stChlInfo.szFormalMidfix));	

        //从配置文件中读取正式文件的后缀
        cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_POSTFIX, CFG_POSTFIX_DEFAULT,
						     stChlInfo.szFormalPostfix, sizeof(stChlInfo.szFormalPostfix));
        cfgIniFile.GetString(szAPSection, CFG_POSTFIX, stChlInfo.szFormalPostfix,
						     stChlInfo.szFormalPostfix, sizeof(stChlInfo.szFormalPostfix));
		cfgIniFile.GetString(szChlSection, CFG_POSTFIX, stChlInfo.szFormalPostfix, 
                             stChlInfo.szFormalPostfix, sizeof(stChlInfo.szFormalPostfix));	

        //构造状态文件路径
		SNPRINTF(stChlInfo.szStatusFile,
				 sizeof(stChlInfo.szStatusFile),
				 "%s/cdr_sequence_status_%s.dat",
				 szPathName, (*iter).c_str());
		

        //获取最终话单长度
        stChlInfo.nBillLen = m_pIBillProcessor->GetFinalLen(i);

        //是否启动日志特性
        stChlInfo.bCheckLog = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                        CFG_CHECK_LOG_ENABLE,
                                        CFG_CHECK_LOG_ENABLE_DEFAULT);
        stChlInfo.bCheckLog = cfgIniFile.GetInt(szAPSection,
                                        CFG_CHECK_LOG_ENABLE,
                                        stChlInfo.bCheckLog);
        stChlInfo.bCheckLog = cfgIniFile.GetInt(szChlSection,
                                        CFG_CHECK_LOG_ENABLE,
                                        stChlInfo.bCheckLog);

        if (stChlInfo.bCheckLog)
        {
            if (stChlInfo.nBillLen == 0)
            {
                COTRACE("当配置为写稽核日志时,通道(%d)的话单长度为0", i);
                return FALSE;
            }

            //稽核日志文件的前缀
            cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_CHECK_LOG_PREFIX, 
                                 CFG_CHECK_LOG_PREFIX_DEFAULT,
                                 stChlInfo.szCheckLogPrefix, sizeof(stChlInfo.szCheckLogPrefix));
            cfgIniFile.GetString(szAPSection, CFG_CHECK_LOG_PREFIX, 
                                 stChlInfo.szCheckLogPrefix,
                                 stChlInfo.szCheckLogPrefix, sizeof(stChlInfo.szCheckLogPrefix));
            cfgIniFile.GetString(szChlSection, CFG_CHECK_LOG_PREFIX, 
                                 stChlInfo.szCheckLogPrefix,
                                 stChlInfo.szCheckLogPrefix, sizeof(stChlInfo.szCheckLogPrefix));

            //稽核日志文件的后缀
            cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_CHECK_LOG_POSTFIX, 
                                 CFG_CHECK_LOG_POSTFIX_DEFAULT,
                                 stChlInfo.szCheckLogPostfix, sizeof(stChlInfo.szCheckLogPostfix));
            cfgIniFile.GetString(szAPSection, CFG_CHECK_LOG_POSTFIX, 
                                 stChlInfo.szCheckLogPostfix,
                                 stChlInfo.szCheckLogPostfix, sizeof(stChlInfo.szCheckLogPostfix));
            cfgIniFile.GetString(szChlSection, CFG_CHECK_LOG_POSTFIX, 
                                 stChlInfo.szCheckLogPostfix,
                                 stChlInfo.szCheckLogPostfix, sizeof(stChlInfo.szCheckLogPostfix));
        
        
            //计算日志的通道目录
            SNPRINTF(stChlInfo.szCheckedLogPath, 
                     sizeof(stChlInfo.szCheckedLogPath),
                     "%s/%s/%s/%s", 
                     szFinalRootDir, "CheckLog", 
                     CCollectorAdapter::GetAPName(), (*iter).c_str());
            stChlInfo.szCheckedLogPath[sizeof(stChlInfo.szCheckedLogPath) - 1] = '\0';
            CreateDeepDir(stChlInfo.szCheckedLogPath);

            //开放可读写权限
    #ifndef _PLATFORM_WIN32
            if (0 != chmod(stChlInfo.szCheckedLogPath,
                           S_IRUSR | S_IWUSR | S_IXUSR
                           | S_IRGRP | S_IWGRP | S_IXGRP
                           | S_IROTH | S_IWOTH | S_IXOTH))
            {
                return FALSE;
            }
    #endif
        }

		m_ChlInfoList.push_back(stChlInfo);

		nRet = loadstatus(i);
        if(ERR_SUCCESS == nRet)
        {
            return nRet;
        }
	}
	
    return ERR_SUCCESS;
}

int CSJZTellinDirConvert::loadstatus(int nChannel)
{
    if(nChannel >= m_ChlInfoList.size())
    {
        return ERR_FAIL;
    }

    FILE* pStatusFile = NULL;
    pStatusFile = fopen(m_ChlInfoList[nChannel].szStatusFile, "rb+");
    if(NULL == pStatusFile)
    {
        //创建新文件
        pStatusFile = fopen(m_ChlInfoList[nChannel].szStatusFile, "wb+");
		
        if (NULL == pStatusFile)
        {	
            return ERR_FAIL;
        }
        else
        {	
            fclose(pStatusFile);
            pStatusFile = NULL;
			m_ChlInfoList[nChannel].uCurCsn = 
                m_ChlInfoList[nChannel].uFileCsnStart - m_ChlInfoList[nChannel].uFileCsnStep;
			SaveStatus(nChannel);
            return ERR_SUCCESS;
        }
    }
	
    int nFileLength = FileSize(pStatusFile);
    if(nFileLength < sizeof(UINT4))
    {
        fclose(pStatusFile);
        pStatusFile = NULL;
        return ERR_FAIL;
    }
	
	UINT4 nTmp;
    int nRet = fread(&nTmp,
					 sizeof(UINT4),
					 1,
					 pStatusFile);
	
    if(1 != nRet)
    {
        fclose(pStatusFile);
        pStatusFile = NULL;
        return ERR_FAIL;
    }

	m_ChlInfoList[nChannel].uCurCsn = nTmp;
    fclose(pStatusFile);
    pStatusFile = NULL;
	return ERR_SUCCESS;
}

int CSJZTellinDirConvert::SaveStatus(int nChannel)
{
    if(nChannel >= m_ChlInfoList.size())
    {
        return ERR_FAIL;
    }
    
    FILE* pStatusFile = NULL;
    pStatusFile = fopen(m_ChlInfoList[nChannel].szStatusFile, "wb");
    if(NULL == pStatusFile)
    {
        return ERR_FAIL;
    }
	
    int nRet = 0;
    UINT4 nTmp = m_ChlInfoList[nChannel].uCurCsn;
	
    //写状态文件
    nRet = fwrite(&nTmp,
				 sizeof(UINT4),
				 1,
				 pStatusFile);
	
    if(1 != nRet)
    {
        fclose(pStatusFile);
        pStatusFile = NULL;		
        return ERR_FAIL;
    }
	
    nRet = fflush(pStatusFile);
    fclose(pStatusFile);
    pStatusFile = NULL;

    ACE_OS::truncate(m_ChlInfoList[nChannel].szStatusFile, sizeof(UINT4));
	
    return nRet;	
}

void CSJZTellinDirConvert::ConstuctDestFileLst(SConvertFileReq& stReq)
{
    STRING strSrcFileName = "";
    STRING strSrcPath = "";   
    m_FinalFileList.clear();
    m_CvtResultList.clear();
    
    divide_file_path_name(stReq.szSrcName,
						  strSrcPath,
						  strSrcFileName);
	
	char szFinalFile[200];

    LIST<STRING>::iterator iter = m_ChlNames.begin();

    for (int nChannel = 0; iter != m_ChlNames.end(); iter++, nChannel++)
    {	
		if (nChannel > 0)
		{
            //构造序列号
			loadstatus(nChannel);
			m_ChlInfoList[nChannel].uCurCsn += m_ChlInfoList[nChannel].uFileCsnStep;
            if ((m_ChlInfoList[nChannel].uCurCsn > m_ChlInfoList[nChannel].uFileCsnEnd) 
                || (m_ChlInfoList[nChannel].uCurCsn < m_ChlInfoList[nChannel].uFileCsnStart))
            {
                m_ChlInfoList[nChannel].uCurCsn = m_ChlInfoList[nChannel].uFileCsnStart;
            }
			STRING szFileCsn = GetFileCsnStr(m_ChlInfoList[nChannel].uCurCsn, nChannel);
            

            //构造文件名
			m_ChlInfoList[nChannel].tFileCreateTime = time(0);
			tm tmNow = *localtime(&m_ChlInfoList[nChannel].tFileCreateTime);
            STRING szPrefix = FormatCfgStr(m_ChlInfoList[nChannel].szFormalPrefix, 
                                           tmNow, 
                                           nChannel);
            STRING szMidfix = FormatCfgStr(m_ChlInfoList[nChannel].szFormalMidfix, 
                                           tmNow, 
                                           nChannel);
            STRING strFileName;
            if (0 == strlen(m_ChlInfoList[nChannel].szFormalPostfix))
            {
                strFileName = szPrefix + szFileCsn + szMidfix;
            }
            else
            {
                strFileName
                     = szPrefix + szFileCsn + szMidfix + "." + m_ChlInfoList[nChannel].szFormalPostfix;
            }

			SNPRINTF(szFinalFile,
					sizeof(szFinalFile),
					"%s/%s/temp/%s",
					m_szFinalRootPath,
					(*iter).c_str(),
					strFileName.c_str());
		}
		else
        {
			SNPRINTF(szFinalFile,
					sizeof(szFinalFile),
					"%s/%s/temp/%s",
					m_szFinalRootPath,
					(*iter).c_str(),
							strSrcFileName.c_str());
        }
        szFinalFile[sizeof(szFinalFile) - 1] = '\0';
		
        m_FinalFileList.push_back(szFinalFile);
        m_CvtResultList.push_back(0);
    }	
}

void CSJZTellinDirConvert::OnFileConvertSucc(int nChannel)
{
    SaveStatus(nChannel);

    if(m_ChlInfoList[nChannel].bCheckLog)
    {
        STRING szFileInfo;
        szFileInfo = GetBillFileInfo(m_FinalFileList[nChannel].c_str(), nChannel);
        SaveCheckLog(szFileInfo.c_str(), nChannel);        
    }
}

STRING CSJZTellinDirConvert::GetBillFileInfo(const char* szFilePath, int nChannel)
{
    if ((NULL == szFilePath) || ('\0' == szFilePath[0]))
    {
        return "";
    }

    STRING strSrcFileName = "";
    STRING strSrcPath = "";
    divide_file_path_name(szFilePath,
                          strSrcPath,
                          strSrcFileName);

    char szFileInfo[63];
    SNPRINTF(szFileInfo, 51, "%-50s", strSrcFileName.c_str());
    szFileInfo[50] = '\0';

    int nFileLen = FileSize(szFilePath);
    int nRecordCnt = nFileLen / m_ChlInfoList[nChannel].nBillLen;
    SNPRINTF(&szFileInfo[50], 11, "%010d", nRecordCnt);
    szFileInfo[60] = '\r';
    szFileInfo[61] = '\n';
    szFileInfo[62] = '\0';

    return szFileInfo;
}

BOOL CSJZTellinDirConvert::IsRepeatLog(const char* szLogMsg, FILE* pFile)
{
    if ((NULL == pFile) || (FileSize(pFile) < 62))
    {
        return FALSE;
    }

    char szLastLine[63] = "";
    int  nLineLen = 62;

    if (0 != fseek(pFile, -(nLineLen), SEEK_END))
    {
        return FALSE;
    }

    if (1 != fread(szLastLine, nLineLen, 1, pFile))
    {
        return FALSE;
    }
    szLastLine[62] = '\0';
   
    if (strncmp(szLastLine, szLogMsg, 50) == 0)
    {
        return TRUE;
    }

    return FALSE;

}

BOOL CSJZTellinDirConvert::SaveCheckLog(const char* szLogMsg, int nChannel)
{
    if (szLogMsg == NULL)
    {
        return FALSE;
    }

	tm tmFileCreate = *localtime(&m_ChlInfoList[nChannel].tFileCreateTime);

    //计算稽核日志前缀
    STRING szLogPrefix = FormatCfgStr(m_ChlInfoList[nChannel].szCheckLogPrefix, 
                                      tmFileCreate, 
                                      nChannel);

    //计算稽核文件路径
    STRING szCheckLogFile = szLogPrefix + m_ChlInfoList[nChannel].szCheckLogPostfix;
    szCheckLogFile = STRING(m_ChlInfoList[nChannel].szCheckedLogPath) + "/" + szCheckLogFile;

    FILE* pFile = fopen(szCheckLogFile.c_str(), "rb+");
    if (NULL == pFile)
    {
        pFile = fopen(szCheckLogFile.c_str(), "wb+");
        if (NULL == pFile)
        {
            COTRACE(S_BS_FAIL_OPEN_FILE, 
                 szCheckLogFile.c_str());
            return FALSE;
        }
    }

    //没有重复的日志，则写入
    if (!IsRepeatLog(szLogMsg, pFile))
    {
        if ((0 != fseek(pFile, 0, SEEK_END))
            || (1 != fwrite(szLogMsg, strlen(szLogMsg), 1, pFile))) 
        {
            COTRACE(S_BS_FAIL_WRITE_FILE, 
                  szCheckLogFile.c_str());
            fclose(pFile);
            pFile = NULL;
            return FALSE;
        }
        fflush(pFile);
    }

    fclose(pFile);
    pFile = NULL;

    return TRUE;

}


STRING CSJZTellinDirConvert::GetFileCsnStr(UINT4 uCsn, int nChannel)
{
    if(m_ChlInfoList[nChannel].nCsnLen == 0)
    {
        return "";
    }

    char szFormat[100], szFileCsn[100];

    SNPRINTF(szFormat, 100, "%%0%dd", m_ChlInfoList[nChannel].nCsnLen);
    szFormat[99] = 0;
    SNPRINTF(szFileCsn, 100, szFormat, uCsn);
    szFileCsn[99] = 0;

    return szFileCsn;
}

//格式化前缀字符串
STRING CSJZTellinDirConvert::FormatCfgStr(const char* szFormat, 
                                          const tm&   tmTime, 
                                          const UINT4 uChannelID)
{
    //替换前缀中的特殊字符
    //by ldf 2004-02-17 阿联酋
    char szYear[10], szYear2[10], szMonth[10], szDay[10], szChannel[10];
    char szHour[10], szHour2[10], szMinute[10], szSecond[10];
    char szAmPm[10],szWeek[10];
    //end
    sprintf(szYear,  "%04d", tmTime.tm_year + 1900);
    sprintf(szYear2, "%02d", tmTime.tm_year%100);
    sprintf(szMonth, "%02d", tmTime.tm_mon + 1);
    sprintf(szDay,   "%02d", tmTime.tm_mday);

    sprintf(szHour,  "%02d", tmTime.tm_hour);
    if(tmTime.tm_hour%12 != 0)
    {
        sprintf(szHour2, "%02d", tmTime.tm_hour%12);
    }
    else
    {
        sprintf(szHour2, "%02d", 12);
    }

    if(tmTime.tm_hour > 11 )
    {
        sprintf(szAmPm, "%s", "p.m.");
    }
    else
    {
        sprintf(szAmPm, "%s", "a.m.");
    }

    sprintf(szMinute, "%02d", tmTime.tm_min);
    sprintf(szSecond, "%02d", tmTime.tm_sec);

    sprintf(szWeek,   "%d",   tmTime.tm_wday);

    sprintf(szChannel, "%02d", uChannelID);

    char szPrefix[512], szPrefix1[512], szPrefix2[512], szPrefix3[512],
        szPrefix4[512], szPrefix5[512], szPrefix6[512], szPrefix7[512],
        szPrefix8[512], szPrefix9[512], szPrefix10[512], szPrefix11[512],
        szPrefix12[512];
    ReplaceSubStr(szFormat,  szPrefix1, "%y", szYear2);
    ReplaceSubStr(szPrefix1, szPrefix2, "%Y", szYear);
    ReplaceSubStr(szPrefix2, szPrefix3, "%m", szMonth);
    ReplaceSubStr(szPrefix3, szPrefix4, "%d", szDay);

    ReplaceSubStr(szPrefix4, szPrefix5, "%H", szHour);
    ReplaceSubStr(szPrefix5, szPrefix6, "%I", szHour2);
    ReplaceSubStr(szPrefix6, szPrefix7, "%M", szMinute);
    ReplaceSubStr(szPrefix7, szPrefix8, "%S", szSecond);

    ReplaceSubStr(szPrefix8, szPrefix9, "%p", szAmPm);

    ReplaceSubStr(szPrefix9, szPrefix10, "%w", szWeek);
    ReplaceSubStr(szPrefix10, szPrefix11, "%%", "%");
    ReplaceSubStr(szPrefix11, szPrefix12, "%c", szChannel);
    ReplaceSubStr(szPrefix12, szPrefix, "%C", szChannel);
    //end
    
    return STRING(szPrefix);
}


//以下是采集操作的基类CTellinDirConvert的定义
//构造函数
CTellinDirConvert::CTellinDirConvert(int nSwitchIndex,
                                     int nDirIndex,
                                     IBillProcessor* pIBillProcessor)
                                    :CComDirCollectConvert(nSwitchIndex,
                                                           nDirIndex,
                                                           pIBillProcessor)
{
}

//析构函数
CTellinDirConvert::~CTellinDirConvert()
{
    //
}


int CTellinDirConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();
    
    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    m_pConvertFun = ProcTELLINFile;

    return ERR_SUCCESS;
}

void CTellinDirConvert::ConstuctDestFileLst(SConvertFileReq& stReq)
{
    //因为传入的源文件名是全路径，需要把文件名和路径分开
    STRING strSrcFileName = "";
    STRING strSrcPath = "";
    
    m_FinalFileList.clear();
    m_CvtResultList.clear();
    
    divide_file_path_name(stReq.szSrcName,
        strSrcPath,
        strSrcFileName);
    
    //获得最终话单文件
    STRING strDstName = "";
    strDstName = GetDstFileName(strSrcFileName.c_str());
    
    //转换
    char szFinalFile[MAX_PATH] = "";
    
    
    char szDstName[40];
    strncpy(szDstName, strDstName.c_str(), sizeof(szDstName));
    szDstName[sizeof(szDstName) - 1] = '\0';
    
    char* pszTime = NULL;
    char* pszSvrCode = szDstName;
    
    pszTime = strchr(szDstName, '_');
    if (pszTime != NULL)
    {
        pszTime[0] = '\0';
        pszTime++;
    }

    LIST<STRING>::iterator iter = m_ChlNames.begin();
    for (; iter != m_ChlNames.end(); iter++)
    {
        //如果是“业务键_YYYYMMDDNNNN”格式，变为“业务键_省代码_YYYYMMDDNNNN”
        if (pszTime != NULL)
        {
            SNPRINTF(szFinalFile,
                sizeof(szFinalFile),
                "%s/%s/temp/%s_%s_%s",
                m_szFinalRootPath,
                (*iter).c_str(),
                pszSvrCode,
                (*iter).c_str(),
                pszTime);
        }
        else    //否则保持原样
        {
            SNPRINTF(szFinalFile,
                sizeof(szFinalFile),
                "%s/%s/temp/%s",
                m_szFinalRootPath,
                (*iter).c_str(),
                strDstName.c_str());
        }
        
        szFinalFile[sizeof(szFinalFile) - 1] = '\0';
        
        m_FinalFileList.push_back(szFinalFile);
        m_CvtResultList.push_back(0);
    }
}

//拷贝第二份文件
int CTellinDirConvert::CopySecondFile()
{
    int nRet = ERR_SUCCESS;
    STRING strSecondFile = "";
    STRING strSrcFileName = "";
    STRING strSrcPath = "";
    
    int nChlCount = m_ChlNames.size();
    for (int i = 0; i < nChlCount; i++)
    {
        if (m_CvtResultList[i] > 0)
        {
            divide_file_path_name(m_FinalFileList[i].c_str(),
                strSrcPath,
                strSrcFileName);
            
            strSecondFile = m_SecondDirList[i];
            strSecondFile += "/" + strSrcFileName;
            
            BOOL bSuccess = TRUE;
            if (m_bNeedSecondAdapter)
            {
                bSuccess = (SaveSecondTELLIN(m_FinalFileList[i].c_str(), 
                    strSecondFile.c_str()) == ERR_SUCCESS);
            }
            else
            {
                bSuccess = FileCopy(m_FinalFileList[i].c_str(), 
                    strSecondFile.c_str());
            }
            
            
            if (!bSuccess)
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                    "采集对象(%s)拷贝最终话单文件(%s->%s)失败",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                
                WRITELOG_F("采集对象(%s)拷贝最终话单文件(%s->%s)失败",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                nRet = ERR_FAIL;
                break;
            }
            else
            {
                COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                    "采集对象(%s)拷贝最终话单文件(%s->%s)成功",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                
                WRITELOG_C("采集对象(%s)拷贝最终话单文件(%s->%s)成功",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
            }
        }
    }
    
    return nRet;
}

//以下是采集操作的基类CMeterSumDirConvert的定义
//构造函数
CMeterSumDirConvert::CMeterSumDirConvert(int nSwitchIndex,
                                         int nDirIndex,
                                         IBillProcessor* pIBillProcessor)
                                        : CComDirCollectConvert(nSwitchIndex,
                                                                nDirIndex,
                                                                pIBillProcessor)
{
}

//析构函数
CMeterSumDirConvert::~CMeterSumDirConvert()
{
    //
}


int CMeterSumDirConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();
    
    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        return ERR_FAIL;
    }
    
    /*
        在相关交换机字段获取信息
    */
    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d", //"switch*"
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[CFG_BUF_SIZE - 1] = '\0';

    m_IsMonthBill   = INIFile.GetInt(szSecName,CFG_CREATE_MONTH_FILE,1);
    m_IsDayBill     = INIFile.GetInt(szSecName,CFG_CREATE_DAY_FILE,1);
    m_BillDate      = INIFile.GetInt(szSecName,CFG_METER_BILL_DATE,21); 
    m_nMeterUpLimit = INIFile.GetInt(szSecName,
                                     CFG_METER_UPLIMIT,
                                     CFG_METER_UPLIMIT_DEFAULT); 

    /*
        在相关SubObj字段获取信息
    */
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d_%s",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex,
             m_szSubObjName);
    szSecName[CFG_BUF_SIZE - 1] = '\0';

    m_IsMonthBill   = INIFile.GetInt(szSecName,CFG_CREATE_MONTH_FILE,m_IsMonthBill);
    m_IsDayBill     = INIFile.GetInt(szSecName,CFG_CREATE_DAY_FILE,m_IsDayBill);
    m_BillDate      = INIFile.GetInt(szSecName,CFG_METER_BILL_DATE,m_BillDate);  
    m_nMeterUpLimit = INIFile.GetInt(szSecName,CFG_METER_UPLIMIT,m_nMeterUpLimit);

    INIFile.Close();

    return ERR_SUCCESS;    
}

//把临时目录下的文件移动到正式目录下
int CMeterSumDirConvert::RenameDestFile()
{
    char szFinalFile[MAX_PATH] = "";
    STRING strSrcFileName = "";
    STRING strSrcPath = "";
    
    int nChlCount = m_ChlNames.size();
    for (int i = 0; i < nChlCount; i++)
    {
            divide_file_path_name(m_FinalFileList[i].c_str(),
                                  strSrcPath,
                                  strSrcFileName);
            
            SNPRINTF(szFinalFile,
                     sizeof(szFinalFile),
                     "%s/%s",
                     m_FinalDirList[i].c_str(),
                     strSrcFileName.c_str());
            szFinalFile[sizeof(szFinalFile) - 1] = '\0';
            
            FileMove(m_FinalFileList[i].c_str(), szFinalFile);
            m_FinalFileList[i] = szFinalFile;
     }

    for (int j=1; j<m_FinalFileList.size()/nChlCount; ++j)
        for(int i=0;i<nChlCount;++i)
        {
            divide_file_path_name(m_FinalFileList[j*nChlCount + i].c_str(),
                                  strSrcPath,
                                  strSrcFileName);
        
            SNPRINTF(szFinalFile,
                     sizeof(szFinalFile),
                     "%s/%s",
                     m_FinalDirList[ i].c_str(),
                     strSrcFileName.c_str());
            szFinalFile[sizeof(szFinalFile) - 1] = '\0';
        
            FileMove(m_FinalFileList[j*nChlCount + i].c_str(), szFinalFile);
            m_FinalFileList[j*nChlCount + i] = szFinalFile;
        }
    
    return ERR_SUCCESS;
}

//拷贝第二份文件
int CMeterSumDirConvert::CopySecondFile()
{
    int nRet = ERR_SUCCESS;
    STRING strSecondFile = "";
    STRING strSrcFileName = "";
    STRING strSrcPath = "";
    
    int nChlCount = m_ChlNames.size();
    BOOL bSuccess = TRUE;

    for (int i = 0; i < nChlCount; i++)
    {
        if (m_CvtResultList[i] > 0)
        {
            divide_file_path_name(m_FinalFileList[i].c_str(),
                                  strSrcPath,
                                  strSrcFileName);
                
            strSecondFile = m_SecondDirList[i];
            strSecondFile += "/" + strSrcFileName;
                
            bSuccess = FileCopy(m_FinalFileList[i].c_str(), 
                                strSecondFile.c_str());

            
            if (!bSuccess)
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                        "采集对象(%s)拷贝最终话单文件(%s->%s)失败",
                        m_szSwitchName,
                        m_FinalFileList[i].c_str(),
                        strSecondFile.c_str());
                
                WRITELOG_F("采集对象(%s)拷贝最终话单文件(%s->%s)失败",
                            m_szSwitchName,
                            m_FinalFileList[i].c_str(),
                            strSecondFile.c_str());
                nRet = ERR_FAIL;
                break;
            }
            else
            {
                COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                        "采集对象(%s)拷贝最终话单文件(%s->%s)成功",
                        m_szSwitchName,
                        m_FinalFileList[i].c_str(),
                        strSecondFile.c_str());
                
                WRITELOG_C("采集对象(%s)拷贝最终话单文件(%s->%s)成功",
                            m_szSwitchName,
                            m_FinalFileList[i].c_str(),
                            strSecondFile.c_str());
            }
        }
    }
    for (int j=1; j<m_FinalFileList.size()/nChlCount; ++j)
    {       
        for (int i =0 ;i<nChlCount;++i)
        {
            if (m_CvtResultList[j*nChlCount + i] > 0)
            {
                divide_file_path_name(m_FinalFileList[j*nChlCount +i ].c_str(),
                                      strSrcPath,
                                      strSrcFileName);
                
                strSecondFile = m_SecondDirList[ i];
                strSecondFile += "/" + strSrcFileName;
                
                bSuccess = FileCopy(m_FinalFileList[j*nChlCount + i].c_str(), 
                                    strSecondFile.c_str());
                
                if (!bSuccess)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                            "采集对象(%s)拷贝最终话单文件(%s->%s)失败",
                            m_szSwitchName,
                            m_FinalFileList[j*nChlCount +i].c_str(),
                            strSecondFile.c_str());
                    
                    WRITELOG_F("采集对象(%s)拷贝最终话单文件(%s->%s)失败",
                                m_szSwitchName,
                                m_FinalFileList[j*nChlCount +i].c_str(),
                                strSecondFile.c_str());
                    nRet = ERR_FAIL;
                    break;
                }
                else
                {
                    COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                            "采集对象(%s)拷贝最终话单文件(%s->%s)成功",
                            m_szSwitchName,
                            m_FinalFileList[j*nChlCount +i].c_str(),
                            strSecondFile.c_str());
                    
                    WRITELOG_C("采集对象(%s)拷贝最终话单文件(%s->%s)成功",
                                m_szSwitchName,
                                m_FinalFileList[j*nChlCount +i].c_str(),
                                strSecondFile.c_str());
                }
            }

        }

    }    
    return nRet;
}

BOOL CMeterSumDirConvert::isLeapYear(int nYear)
{
    if( nYear % 400 == 0)
        return TRUE;
    if( nYear % 100 == 0)
        return FALSE;
    if( nYear % 4 == 0)
        return TRUE;
    return FALSE;
}

//输入年月，返回该月的天数。
int CMeterSumDirConvert::GetDaysOfMonth(int nYear, int nMonth)
{
    switch( nMonth )
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
        break;
    case 2:
        if(isLeapYear(nYear))
            return 29;
        else
            return 28;
        break;
    default:
        return -1;
        break;
    }
}

//昨天
int CMeterSumDirConvert::GetLastDate(int& nYear, int& nMonth, int& nDay)
{
    tm tm1 = {0, 0, 0, nDay, nMonth - 1, nYear - 1900};
    time_t nNow = mktime(&tm1);
    nNow -= 24 * 60 * 60;
    tm tmTmp = *localtime(&nNow);
    nYear = tmTmp.tm_year + 1900;
    nMonth = tmTmp.tm_mon + 1;
    nDay = tmTmp.tm_mday;
    return 0;
}

//今天的上月
int CMeterSumDirConvert::GetLastMonth(int& nYear, int& nMonth, int& nDay)
{
    if ((nMonth>12) || (nDay >31))
    {
        return -1;
    }
    
    if (1 == nMonth)
    {
        nYear -= 1;
        nMonth =12;
    }
    else
    {
        nMonth -= 1;
    }
    
    if (nDay > GetDaysOfMonth(nYear,nMonth))
    {
        nDay = GetDaysOfMonth(nYear,nMonth);;
    }
    return 0;
}

void CMeterSumDirConvert::initialize(STRING& sTodayMeterFile,
                                     STRING& sLastMeterFile,
                                     STRING& sLastMonthMeterFile,
                                     const char* pszSrcFile)
{
    STRING sTodayTime = STRING(pszSrcFile);
    STRING::size_type pos = sTodayTime.find(ORG_METER_NAME);
    if ((pos < sTodayTime.size()) && (pos >=0))
    {
        sTodayTime = sTodayTime.substr(pos-9,8); 
    }
    else
    {
        time_t nNow = time(0);
        tm tmTmp = *localtime(&nNow);
        char szCurDate[24];
        strftime(szCurDate, sizeof(szCurDate), "%Y%m%d", &tmTmp);
        sTodayTime = STRING(szCurDate);
    }

    int nYear,nMonth,nDay;
    nYear = atoi(sTodayTime.substr(0,4).c_str());
    nMonth = atoi(sTodayTime.substr(4,2).c_str());
    nDay = atoi(sTodayTime.substr(6,2).c_str());
    int nSaveDay,nSaveMonth,nSaveYear;
    nSaveDay = nDay;
    nSaveMonth = nMonth;
    nSaveYear = nYear;
    
    //今天
    char szTodayDate[20];
    memset(szTodayDate,0,20);
    sprintf(szTodayDate,"%04d%02d%02d",nYear,nMonth,nDay);
    
    //昨天
    GetLastDate(nYear,nMonth,nDay);
    char szLastDate[20];
    sprintf(szLastDate,"%04d%02d%02d",nYear,nMonth,nDay);
    
    //上个月今天
    nDay = nSaveDay;
    nMonth = nSaveMonth;
    nYear = nSaveYear;    
    GetLastMonth(nYear,nMonth,nDay);
    char szLastMonthDate[20];
    sprintf(szLastMonthDate,"%d%02d%02d",nYear,nMonth,nDay);
    
    sTodayMeterFile = STRING(szTodayDate);
    sLastMeterFile = STRING(szLastDate);
    sLastMonthMeterFile = STRING(szLastMonthDate);
}


int CMeterSumDirConvert::Process_cc08_special_meter_file(const char*     pszSrcFile,
                                                         VECTOR<STRING>& FinalFileNameList,
                                                         VECTOR<int>&    CvtResultList,
                                                         const int bFileLevelVerify,
                                                         const int bBillLevelVerify,
                                                         void* pIBillProcessor,
                                                         const int nOrigBillLen)
{
    STRING sToday;
    STRING sLastday;
    STRING sLastMonth;
    initialize(sToday,sLastday,sLastMonth,pszSrcFile);  //日期

    int nBillDate,nIsMonthBill,nIsDayBill;

    //配置文件
    nBillDate = m_BillDate;
    nIsMonthBill = m_IsMonthBill;
    nIsDayBill = m_IsDayBill;

    //今天文件
    STRING sSrcFile = STRING(pszSrcFile);

    VECTOR<STRING> month_FinalFileList(FinalFileNameList);
    VECTOR<STRING> day_FinalFileList(FinalFileNameList);
    VECTOR<STRING> other_FinalFileList(FinalFileNameList);
    VECTOR<int> month_CvtResult(CvtResultList);
    VECTOR<int> day_CvtResult(CvtResultList);
    VECTOR<int> other_CvtResult(CvtResultList);
    FinalFileNameList.clear();
    CvtResultList.clear();

    //月净值
    int nRet = ERR_SUCCESS;
    if ((nIsMonthBill) && (nBillDate == atoi(sToday.substr(6,2).c_str())))
    {
        //上月记次文件包含目录c:\frontsave\20030303\meter.sum
        STRING sLastMonthFile(sSrcFile);
        STRING::size_type pos = sLastMonthFile.find(sToday);
        if ((pos < sLastMonthFile.size()) && (pos >=0))
        {
            sLastMonthFile.replace(pos,8,sLastMonth);
        }
        else
        {
            return ERR_FAIL;
        }

        //月新文件名meter_200401.sum
        char szMonthNewFile[40];
        sprintf(szMonthNewFile,CUR_METER_NAME,sToday.substr(0,6).c_str());

        //每个文件改名成月文件明
        for (int i=0;i < month_FinalFileList.size();++i)
        {
            pos = month_FinalFileList[i].find(ORG_METER_NAME);
            if ((pos < month_FinalFileList[i].size()) && (pos >=0))
            {
                month_FinalFileList[i].replace(pos,strlen(ORG_METER_NAME),STRING(szMonthNewFile));
            }
        }
        
        nRet = process_and_build_finalfile( pszSrcFile,
                                            sLastMonthFile.c_str(),
                                            month_FinalFileList,
                                            month_CvtResult,
                                            pIBillProcessor,
                                            nOrigBillLen);
    }
    else
    {
        month_FinalFileList.clear();
        month_CvtResult.clear();
    }

//日净值
    if(nIsDayBill)
    {
        //昨天记次文件包含目录
        STRING sLastFile(sSrcFile);
        STRING::size_type pos = sLastFile.find(sToday);
        if ((pos < sLastFile.size()) && (pos >=0))
        {
            sLastFile.replace(pos,8,sLastday);
        }
        else
        {
            return ERR_FAIL;
        }

        //日新文件名meter_20040101.sum
        char szNewFile[40];
        sprintf(szNewFile,CUR_METER_NAME,sToday.c_str());

        for (int i=0;i < day_FinalFileList.size();++i)
        {
            pos = day_FinalFileList[i].find(ORG_METER_NAME);
            if ((pos < day_FinalFileList[i].size()) && (pos >=0))
            {
                  day_FinalFileList[i].replace(pos,strlen(ORG_METER_NAME),STRING(szNewFile));
            }
         }

        nRet = process_and_build_finalfile( pszSrcFile,
                                            sLastFile.c_str(),
                                            day_FinalFileList,
                                            day_CvtResult,
                                            pIBillProcessor,
                                            nOrigBillLen
                                           );
    }
    else
    {
        day_FinalFileList.clear();
        day_CvtResult.clear();
    }

//任意日期段的净值    
    Proce_Other_File(sSrcFile,
    				 other_FinalFileList, 
    				 other_CvtResult,
    				 pIBillProcessor,
    				 nOrigBillLen);

 
//保存月产生的文件.

    if ( day_FinalFileList.size() > 0)
    {
        for (int i=0;i<day_FinalFileList.size();i++)
            FinalFileNameList.push_back(day_FinalFileList[i]);
        std::copy(day_CvtResult.begin(), day_CvtResult.end(), std::back_inserter(CvtResultList));
    }
    
    if ( month_FinalFileList.size() > 0)
    {
        for (int i=0;i<month_FinalFileList.size();i++)
            FinalFileNameList.push_back(month_FinalFileList[i]);
        std::copy(month_CvtResult.begin(), month_CvtResult.end(), std::back_inserter(CvtResultList));
    }

    if ( other_FinalFileList.size() > 0)
    {
        for (int i=0;i<other_FinalFileList.size();i++)
            FinalFileNameList.push_back(other_FinalFileList[i]);
        std::copy(other_CvtResult.begin(), other_CvtResult.end(), std::back_inserter(CvtResultList));
    }

   
    return nRet;
}

int CMeterSumDirConvert::Proce_Other_File(STRING& sSrcFile,
    				 					  VECTOR<STRING>& FinalFileNameList, 
    				 					  VECTOR<int>& CvtResultList,
    				 					  void* pIBillProcessor,
    				 					  const int	nOrigBillLen)
{
	// open file c:/igwbccs/config/ini/metersum_config.ini
	STRING sCfgPath = STRING(GETAPPPATH) + STRING("/config/ini/") +STRING(CONFIG_METERSUM);
	STRING::size_type pos;
    int nRet = ERR_SUCCESS;
    VECTOR<STRING> save_FinalFileList(FinalFileNameList);
    VECTOR<int>  save_CvtResultList(CvtResultList);
    FinalFileNameList.clear();
    CvtResultList.clear();

	
    CINIFile INIFile(sCfgPath.c_str());
    if (INIFile.Open())
    {
        STRING sSecName(CFG_SECTION_NAME);
        int nTaskCount = INIFile.GetInt(sSecName.c_str(), TASK_COUNT, 0);

        for(int i=1; i<=nTaskCount; ++i)
        {
            char szID[20];
            SNPRINTF(szID, sizeof(szID), "%d", i);
            STRING sTmpSecName = sSecName + szID;
            char szBillDateList[CFG_BUF_SIZE];
            szBillDateList[0] = '\0';
            INIFile.GetString(sSecName.c_str(),
                              sTmpSecName.c_str(),
                              "",
                              szBillDateList,
                              sizeof(szBillDateList));
            
            if (strlen(szBillDateList) < 17)
            {
                continue;
            }
            char* pstr = strstr(szBillDateList, ",");
            if (NULL != pstr)
            {
                pstr[0] = '\0';
                ++pstr;
                VECTOR<STRING> temp_FinalFileList(save_FinalFileList);
                VECTOR<int>  temp_CvtResultList(save_CvtResultList);
                
                StringLRTrim(szBillDateList);
                StringLRTrim(pstr);

                if (!(IsDigitValue(szBillDateList) && (strlen(szBillDateList) == 8)))
                {
                    continue;
                }
                    

                if (!(IsDigitValue(pstr)  && (strlen(szBillDateList) == 8)))
                {
                       continue;
                }

                STRING sTodayTime = sSrcFile;
                pos = sTodayTime.find(ORG_METER_NAME);
                if ((pos < sTodayTime.size()) && (pos >=0))
                {
                    sTodayTime = sTodayTime.substr(pos-9,8); 
                }
                else
                {
                    time_t nNow = time(0);
                    tm tmTmp = *localtime(&nNow);
                    char szCurDate[24];
                    strftime(szCurDate, sizeof(szCurDate), "%Y%m%d", &tmTmp);
                    sTodayTime = szCurDate;
                }
                
                if (strncmp(szBillDateList, pstr, 8) > 0)
                {
                	char szSwap[8];
                	memcpy(szSwap, szBillDateList, 8);
                	memcpy(szBillDateList, pstr, 8);
                	memcpy(pstr, szSwap, 8);
                }

                STRING sLastMonthFile = sSrcFile;
                STRING sThisMonthFile = sSrcFile;
                pos = sLastMonthFile.find(sTodayTime);
                if ((pos < sLastMonthFile.size()) && (pos >=0))
                {
                    sLastMonthFile.replace(pos,8,szBillDateList);
                }
                else
                {
                    continue;
                }

                pos = sThisMonthFile.find(sTodayTime);
                if ((pos < sThisMonthFile.size()) && (pos >=0))
                {
                    sThisMonthFile.replace(pos,8,pstr);
                }
                else
                {
                    continue;
                }

                if ((ACE_OS::access(sThisMonthFile.c_str(), F_OK) != 0) || (ACE_OS::access(sLastMonthFile.c_str(), F_OK) != 0))
                {
                    continue;
                }
                    
               //文件名meter_20010101_20040202.sum  
               char szMonthNewFile[40];
               sprintf(szMonthNewFile, DATE_METER_NAME, szBillDateList, pstr);

               for (int i=0;i < temp_FinalFileList.size();++i)
               {
                   pos = temp_FinalFileList[i].find(ORG_METER_NAME);
                   if ((pos < temp_FinalFileList[i].size()) && (pos >=0))
                   {
                       temp_FinalFileList[i].replace(pos,strlen(ORG_METER_NAME),STRING(szMonthNewFile));
                   }
               }

               nRet = process_and_build_finalfile(sThisMonthFile.c_str(),
                                                  sLastMonthFile.c_str(),
                                                  temp_FinalFileList,
                                                  temp_CvtResultList,
                                                  pIBillProcessor,
                                                  nOrigBillLen); 

                if (nRet == ERR_SUCCESS)
                {
                    INIFile.WriteString(sSecName.c_str(),sTmpSecName.c_str(),"");
                    std::copy(temp_FinalFileList.begin(), temp_FinalFileList.end(), std::back_inserter(FinalFileNameList));
                    std::copy(temp_CvtResultList.begin(), temp_CvtResultList.end(), std::back_inserter(CvtResultList));
                }
            }	
        }
    }

   	return nRet;       
}	    				 					  


int CMeterSumDirConvert::process_and_build_finalfile(const char*   pszSrcFile, //今天
                                                     const char*   pszDestFile,//昨天
                                                     VECTOR<STRING>& FinalFileNameList,
                                                     VECTOR<int>&    CvtResultList,
                                                     void* pIBillProcessor,
                                                     const int nOrigBillLen)
{
    int nBillLen = nOrigBillLen;
    IBillProcessor* pBillProcessor = (IBillProcessor*)pIBillProcessor;
    int nResult;
    int nRet = ERR_SUCCESS;
//---------------------------------------------------------------------
    MAP<SMetSumKey,INT> mLastFileMap;
    
    char OldBuff[200];      //每次一条话单
    char* pOldBuff = OldBuff;
    struct SMetSumKey MetSumKey;//号码
    memset(&MetSumKey,'\0',sizeof(SMetSumKey));
    memset(OldBuff,'\0',sizeof(OldBuff));
    int i = 0;
    int nRead = 0;
    FILE* pDesFile = fopen(pszDestFile, "rb");
    if (NULL == pDesFile)
    {
    
        COMSGOUT(MSGOUT_LEVEL_NORMAL,
                "打开话单文件(%s)失败，文件不存在",
                pszDestFile);

        char tmpStr[200];
        sprintf(tmpStr,"打开话单文件(%s)失败,文件不存在",pszDestFile);
        WRITELOG_F(tmpStr);
    }
    else
    {
        fseek(pDesFile, 0, SEEK_SET);

        nRead = fread(pOldBuff, nBillLen, 1, pDesFile);
        while(nRead > 0)            // 多少条话单
        {
            memcpy(&MetSumKey.byChgObj,pOldBuff+5,1);
            if(MetSumKey.byChgObj != 0)
            {
                memset(MetSumKey.szCallNo, 0xFF, 12);
                memcpy(&MetSumKey.uTrunkGrp, pOldBuff+26, 2);
                MetSumKey.uTrunkGrp = ITOHS(MetSumKey.uTrunkGrp);
            }
            else
            {
                memcpy(MetSumKey.szCallNo, pOldBuff+16, 10);
                memset(&MetSumKey.uTrunkGrp, 0xFF, 2);
            }
            
            mLastFileMap[MetSumKey] = (i++)*nBillLen; //建立map         
            nRead = fread(pOldBuff, nBillLen, 1, pDesFile);
        }
    }
//--------------------------------------------------------------------
    FILE* pSrcFile = fopen(pszSrcFile, "rb");
    if (NULL == pSrcFile)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "打开话单文件(%s)失败",
               pszSrcFile);
        
        char tmpStr[200];
        sprintf(tmpStr,"打开话单文件(%s)失败",pszSrcFile);
        WRITELOG_F(tmpStr);
        if (NULL != pDesFile)
            fclose(pDesFile);        
        return ERR_FAIL;
    }

    VECTOR<FILE*> ChlFiles;
    VECTOR<STRING>::iterator item = FinalFileNameList.begin();

    for (; item != FinalFileNameList.end(); item++)
    {
        ChlFiles.push_back(NULL);
    }

    char NewBuff[200];
    char* pNewBuff = NewBuff;
    SMetSumKey NewMetSumKey;
    memset(&NewMetSumKey,'\0',sizeof(NewMetSumKey));
    memset(NewBuff,'\0',sizeof(NewBuff));
    
    fseek(pSrcFile, 0, SEEK_SET);
    nRead = fread(pNewBuff, nBillLen, 1, pSrcFile);
    if (nRead != 1)
    {
        if (ChlFiles[0] == NULL)
        {
            ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
            if (ChlFiles[0] == NULL)
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                    "打开话单文件(%s)失败",
                    FinalFileNameList[0].c_str());
                
                char tmpStr[200];
                sprintf(tmpStr,"打开话单文件(%s)失败",FinalFileNameList[0].c_str());
                WRITELOG_F(tmpStr);
                nRet = ERR_FAIL;                  
            }
        }
        
        fwrite(NewBuff, nRead, 1, ChlFiles[0]);
        CvtResultList[0] = 1;
    }        

    i = 1;//读一张话单
    while( nRead == 1 )
    {       
        memcpy(&NewMetSumKey.byChgObj,pNewBuff+5,1);
        if(NewMetSumKey.byChgObj != 0)
        {
            memset(NewMetSumKey.szCallNo, 0xFF, 12);
            memcpy(&NewMetSumKey.uTrunkGrp, pNewBuff+26, 2);
            NewMetSumKey.uTrunkGrp = ITOHS(NewMetSumKey.uTrunkGrp);
        }
        else
        {
            memcpy(NewMetSumKey.szCallNo, pNewBuff+16, 10);
            memset(&NewMetSumKey.uTrunkGrp, 0xFF, 2);
        }

        int nNewPara,nOldPara;
        MAP<SMetSumKey,int>::iterator pMap = mLastFileMap.find(NewMetSumKey);
        if (pMap != mLastFileMap.end())
        {
            pNewBuff += 28;//28个字节偏移量是记次参数。//16个偏移字节是10个字节的电话号码
            fseek(pDesFile,pMap->second,SEEK_SET);
            
            nRead = fread(OldBuff, nBillLen, 1, pDesFile);
            pOldBuff = OldBuff;
            pOldBuff += 28;
            
            for (int k=0;k<40;++k) //40个记次参数
            {
                memcpy(&nNewPara,pNewBuff,4);
                memcpy(&nOldPara,pOldBuff,4);

                if (nOldPara > nNewPara) //表值翻转
                {
                    nNewPara = nNewPara + m_nMeterUpLimit - nOldPara;
                }
                else  //一般情况
                {
                    nNewPara -= nOldPara;                 
                }

                memcpy(pNewBuff,&nNewPara,4);                                   
                pNewBuff += 4;
                pOldBuff += 4;
            }
        }

        UINT4 uChlID;
        CFormat* pDestFmt = NULL;
        void* pDestBill = NULL;
        UINT4 uLen;

        //获取分拣通道号和转换格式对象，并进行话单的处理
        pBillProcessor->Distribute(NewBuff, nBillLen,
                                    uChlID, pDestFmt);

        if ((pDestFmt != NULL) && (uChlID != 0))
        {
            nResult = pDestFmt->ConvertABill(NewBuff,
                                             nBillLen,
                                             pDestBill, uLen);
            if (ERR_SUCCESS == nResult)
            {
                if (ChlFiles[uChlID] == NULL)
                {
                    FILE* pFile = fopen(FinalFileNameList[uChlID].c_str(), "wb");
                    if (pFile == NULL)
                    {
                          COMSGOUT(MSGOUT_LEVEL_URGENT,
                                 "打开话单文件(%s)失败",
                                 FinalFileNameList[uChlID].c_str());
                     
                          char tmpStr[200];
                          sprintf(tmpStr,"打开话单文件(%s)失败",FinalFileNameList[uChlID].c_str());
                          WRITELOG_F(tmpStr);
                          delete pDestBill;
                          nRet = ERR_FAIL;
                          break;
                    }

                    ChlFiles[uChlID] = pFile;
                }

                fwrite(pDestBill, uLen, 1, ChlFiles[uChlID]);
                CvtResultList[uChlID] = 1;

                delete pDestBill;
            }
        }
        else
        {
            nResult = ERR_FAIL;
        }

        //如果转换不成功，将错误话单写入错误文件
        if (ERR_SUCCESS != nResult)
        {
            if (ChlFiles[0] == NULL)
            {
                ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                if (ChlFiles[0] == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "打开话单文件(%s)失败",
                           FinalFileNameList[0].c_str());
                    char tmpStr[200];
                    sprintf(tmpStr,"打开话单文件(%s)失败",FinalFileNameList[0].c_str());
                    WRITELOG_F(tmpStr);
                    nRet = ERR_FAIL;
                    break;
                }
            }

            fwrite(NewBuff, nBillLen, 1, ChlFiles[0]);
            CvtResultList[0] = 1;
        }

        //检查文件是否已读完
        if(i*nBillLen >= FileSize(pSrcFile))
        {
            break;
        }

        fseek(pSrcFile,i*nBillLen,SEEK_SET);
        ++i;
        nRead = fread(NewBuff, nBillLen, 1, pSrcFile);
        if (nRead != 1)
        {
            if (ChlFiles[0] == NULL)
            {
                ChlFiles[0] = fopen(FinalFileNameList[0].c_str(), "wb");
                if (ChlFiles[0] == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                        "打开话单文件(%s)失败",
                        FinalFileNameList[0].c_str());
                    
                    char tmpStr[200];
                    sprintf(tmpStr,"打开话单文件(%s)失败",FinalFileNameList[0].c_str());
                    WRITELOG_F(tmpStr);
                    nRet = ERR_FAIL;                  
                    break;
                }
            }
            
            fwrite(NewBuff, nRead, 1, ChlFiles[0]);
            CvtResultList[0] = 1;
            break;
        }
        pNewBuff = NewBuff;
    }//end while
   
    if (NULL != pSrcFile)
        fclose(pSrcFile);
    if (NULL != pDesFile)
        fclose(pDesFile);

    for(int j = 0; j < ChlFiles.size(); j++)
    {
        if (ChlFiles[j] != NULL)
        {
            fclose(ChlFiles[j]);
        }
    }


    return nRet;
}

//转换话单文件
int CMeterSumDirConvert::ConvertFile(SConvertFileReq& stReq)
{
    int nRet = Process_cc08_special_meter_file( stReq.szSrcName,
                                                m_FinalFileList,
                                                m_CvtResultList,
                                                m_bFileLevelVerify,
                                                m_bBillLevelVerify,
                                                m_pIBillProcessor,
                                                m_nOrigBillLen);
    return nRet;
}
