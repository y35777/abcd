      /*******************************************************************
**
**  产品名：    SoftX3000
**  模块名：    接收TRAP及处理发送缓冲队列模块
**  文件名：    sun_alarm.cpp
**  描述信息：  
**
**  作者：      郑利锋（34756）
**  版本说明：  V100R001
**  版权信息：  华为技术有限公司
**  创建时间：  2002.11.15
**  修改记录：  
**  1、作者：   
**        日期：
**        修改说明：
**                  
**  2、……
**
********************************************************************/
#include "sun_alarm.h"


//*****************************************************
//  函 数 名:   CSunAlarm::CSunAlarm
//  功    能:   构造函数
//  输入参数:   无
//  输出参数:   无
//  调用函数:   无
//*****************************************************
CSunAlarm::CSunAlarm()
{
    m_hReadSysLogThread = 0;
    m_nReadSysLogRunning = false;
    m_szKeyWordFile[0]  = '\0';
    m_szAlarmIDFile[0]  = '\0';
}

//*****************************************************
//  函 数 名:   CSunAlarm::~CSunAlarm
//  功    能:   析构函数
//  输入参数:   无
//  输出参数:   无
//  调用函数:   无
//*****************************************************
CSunAlarm::~CSunAlarm()
{
    ExitRcvThread();
    m_pGlobalFunc = NULL;
}

//*****************************************************
//  函 数 名:   CSunAlarm::StartRcvThread(...)
//  功    能:   启动读系统日志线程和处理系统告警帧线程
//  输入参数:   const char *szCfgPath  告警项配置文件路径
//              int nPort = 162                 
//              const char *szErrFile  运行错误日志记录文件名
//  输出参数:   无
//  调用函数:   LogExecutionThread()
//              ScheduleExecutionThread()
//  返 回 值:   bool
//              false 表明线程启动不成功
//              true  表明线程启动成功
//*****************************************************
bool CSunAlarm::StartRcvThread(const char *szCfgPath, int nPort)
{   
    // 初始化告警项配置文件和关键字配置文件路径
    if ((szCfgPath == NULL) || (szCfgPath[0] == '\0'))
    {
        m_strError = "HDMonitor config path can not be empty!";
        return false;
    }

    char szPath[256] = ""; 
    strcpy(szPath, szCfgPath);

    int nLen = strlen(szPath);
    if((szPath[nLen - 1] != '\\') 
       && (szPath[nLen - 1] != '/'))
    {
        szPath[nLen] = '/';
        szPath[nLen + 1] = '\0';
    }

    //初始化关键字配置文件(全路径)      
    sprintf(m_szKeyWordFile, "%s%s", 
            szPath, S_SUN_HD_KEYWORD_FILE);

    //初始化告警项配置文件(全路径)      
    sprintf(m_szAlarmIDFile, "%s%s", 
            szPath, S_SUN_HD_EVENT_FILE);

    if(!ReadAlarmKeyword())
    {
        m_strError = "HDMonitor read sunhdkeyword.ini file failed!";
        return false;
    }

    //创建读系统日志线程
    int nRet = m_ThreadManager.spawn((ACE_THR_FUNC)LogExecutionThread,
                                      (void*)this,
                                      THR_NEW_LWP | THR_JOINABLE,
                                      &m_hReadSysLogThread);
    
    //如果创建读系统日志线程失败，则退出
	if(nRet < 0)
	{
        m_strError = "create LogExecutionThread failed!";
        m_strError += ACE_OS::strerror(ACE_OS::last_error());
        
		m_IsExit = true;
        ExitRcvThread();
		return false;
	}

	m_nReadSysLogRunning = true;
    
    return CHardwareAlarm::StartRcvThread(szCfgPath, nPort);
}


//*****************************************************
//  函 数 名:   CSunAlarm::ExitRcvThread()
//  功    能:   退出接收线程（包括LogExecutionThread线程和Schedule线程）
//  输入参数:   无
//  输出参数:   无
//  调用函数:   无
//  返 回 值:   bool
//*****************************************************
bool CSunAlarm::ExitRcvThread()
{
    if(!CHardwareAlarm::ExitRcvThread())
    {
        return false;
    }
    
	if (m_nReadSysLogRunning)
	{
		m_IsExit = true; //设置退出线程标志

	    //关闭m_hTrapThread句柄
	    if (m_hReadSysLogThread != 0)
	    {
		    m_ThreadManager.wait();
            m_hReadSysLogThread = 0;
	    }

        m_nReadSysLogRunning = false;
    }
    
	return true;    
}
//*****************************************************
//  函 数 名:   CSunAlarm::LogExecutionThread()
//  功    能:   启动读系统日志线程
//  输入参数:   void* param
//  输出参数:   无
//  调用函数:   LogInspect()
//  返 回 值:   void
//*****************************************************
void* CSunAlarm::LogExecutionThread(void* param)
{
    CSunAlarm* This = (CSunAlarm*) param;
    if (This != NULL)
    {
        This->LogInspect();
        This->m_nReadSysLogRunning = false;
    }
 
    return 0;
}

//*****************************************************
//  函 数 名:   CSunAlarm::LogInspect(void)
//  功    能:   处理日志文件内容
//  输入参数:   无
//  输出参数:   无
//  调用函数：  无
//  返 回 值:   bool
//*****************************************************
bool CSunAlarm::LogInspect(void)
{
    char szLogFileLine[1024];       //存储日志文件按行读取内容
    long int nOldFileSize = 0;      //记录上次读取的系统日志文件大小
    long int nNewFileSize = 0;      //记录本次读取的系统日志文件大小
    char szFullString[1024] = "";   //存储从系统日志读取的日志内容
    char szErrorType[50]    = "";   //故障类型
    int nSeekRnt = 0;               //查找文件返回值
    FILE* pLogFp = NULL;            //日志文件句柄
     
    while (!m_IsExit)
    {
        pLogFp = fopen("/var/adm/messages", "rt");
        if (pLogFp == NULL)
        {
            return false;
        }
        else
        {
            nOldFileSize = nNewFileSize;
            fseek(pLogFp, 0, SEEK_END);
            nNewFileSize = ftell(pLogFp);
        }       
        
        if (nNewFileSize !=  nOldFileSize)
        {   
            if (pLogFp == NULL) 
            {
                return false;
            }
            
            if (nNewFileSize < nOldFileSize)
            { 
                nOldFileSize = 0;   //日志文件已经另存，从头开始检索
            }
            
            nSeekRnt = fseek(pLogFp, nOldFileSize, SEEK_SET);
            
            if (nSeekRnt == 0)
            {               
                while (!feof(pLogFp) )
                { 
                    if ((fgets(szLogFileLine, 1024, pLogFp) != NULL))
                    {
                        if ((strstr(szLogFileLine,"[ID ") != NULL))
                        {
                            if (strlen(szFullString) != 0)
                            {
                                szErrorType[0] = '\0';
                                
                                if (QueryKeyword(szFullString, szErrorType))
                                {                           
                                    ConstructAlarmFrame(szErrorType, szFullString);   
                                } 

                                szFullString[0] = '\0';
                            }
                        }
                        
                        strcat(szFullString, szLogFileLine);     
                    }
                }
            }
            
            //处理系统日志文件中最后一条日志记录
            if (strlen(szFullString) != 0)
            {
                szErrorType[0] = '\0';
                if (QueryKeyword(szFullString, szErrorType))
                {
                    ConstructAlarmFrame(szErrorType, szFullString);                 
                }
                szFullString[0] = '\0';
            }
        }
        
        fclose(pLogFp);
        pLogFp = NULL;

        ACE_OS::sleep(INSPECT_TIME);  //等待时间间隔
    }

    return true;
}

bool CSunAlarm::ReadAlarmKeyword(void)
{
    FILE* fp = NULL;                            //文件句柄
    char szKeyWord[KEYWORD_MAX_LEN] = "";       //字符串表示需要查找的内容
    char szFileLine[512];                       //文件行内容
    SAlarmKeyword ak;
    
    if ((fp = fopen(m_szKeyWordFile, "rt")) == NULL)
    {
        return false;
    }

    while (!feof(fp))
    {
        if (fgets(szFileLine, 512, fp) != NULL)
        {
            StringLRTrim(szFileLine);
            if (strlen(szFileLine) == 0)
            {
                continue;
            }
            
            char *pszFindStr = strchr(szFileLine, '=');
            if ((pszFindStr == NULL) || (pszFindStr == szFileLine)) 
            {
                continue;
            }

            pszFindStr++;
            if (strlen(pszFindStr) == 0)
            {
                continue;
            }

            if ((pszFindStr - szFileLine - 1) < ALARM_SRC_LEN)
            {
                strncpy(ak.szAlarmSrc, szFileLine, pszFindStr - szFileLine - 1);
                ak.szAlarmSrc[pszFindStr - szFileLine - 1] = '\0';
            }
            else
            {
                strncpy(ak.szAlarmSrc, szFileLine, ALARM_SRC_LEN);
                ak.szAlarmSrc[ALARM_SRC_LEN - 1] = '\0';
            }

            if (strlen(pszFindStr) < ALARM_KEYWORD_LEN)
            {
                strcpy(ak.szKeyword, pszFindStr);
            }
            else
            {
                strncpy(ak.szKeyword, pszFindStr, ALARM_KEYWORD_LEN);
                ak.szKeyword[ALARM_KEYWORD_LEN - 1] = '\0';
            }

            m_KeywordList.push_back(ak);
        }
    }

    return true;
}

bool CSunAlarm::QueryKeyword(char* pszLocation, char* pszErrorType)                                             
{
    KEYWORD_LIST::iterator it = m_KeywordList.begin();
    char szKeyword[ALARM_KEYWORD_LEN] = "";
    char* pszFindStr = NULL;
    char* pszTemp    = NULL;
    bool bMatched    = false;

    while (it != m_KeywordList.end())
    {
        pszTemp = (*it).szKeyword;
        pszFindStr = strchr(pszTemp, ';');
        while (pszFindStr)
        {
            pszTemp++;      //跳过左边的引号
            if (pszFindStr > (pszTemp + 1))
            {
                //不拷贝关键字左右的引号
                strncpy(szKeyword, pszTemp, pszFindStr - pszTemp - 1);
                if (strstr(pszLocation, szKeyword) == NULL)
                {
                    bMatched = false;
                    break;      //不包含关键字，跳过
                }
                else
                {
                    bMatched = true;
                }
            }
            
            pszFindStr++;
            pszTemp = pszFindStr;
            pszFindStr = strchr(pszTemp, ';');           
        }  

        if (bMatched)
        {
            strcpy(pszErrorType, (*it).szAlarmSrc);
            return true;
        }

        it++;
    }

    return false;
}

void CSunAlarm::ConstructAlarmFrame(char *pszErrType,char *pszFullStr)
{   
    if (ReadAlarmCfg(pszErrType))
    {
        if (ConstructAD(pszFullStr))
        {
            //将告警帧加入发送缓冲队列中
            SetAlarm(m_WarnData, m_WarnData.Len);
        }
    }

    return;
}

bool CSunAlarm::ReadAlarmCfg(char* szErrorType)
{
    FILE* fpAlarmCfg;                //ALARMID配置文件句柄
    char szALine[1024];              //文件行内容
    std::string strAlarmFileLine;    //文件行内容

    bool bRnt = false;      //返回值
    int nEqualPos = 0;      //等号位置
    int nSemiPos = 0;       //分号位置
    std::string strTmpStr;  //临时保存字符串
    int nStrLen;            //保存字符长度

    fpAlarmCfg = fopen(m_szAlarmIDFile, "rt");
    if (fpAlarmCfg == NULL)
    {
        m_strError = "HDMonitor read sunhdevent.ini file failed!";        
        bRnt = false;
    }
    else
    {
        while (!feof(fpAlarmCfg))
        {
            memset(szALine, 0, 1024); //初始化字符串内容
            nStrLen = strAlarmFileLine.length();
            strAlarmFileLine.erase(0, nStrLen);
            nStrLen = strTmpStr.length();
            strTmpStr.erase(0, nStrLen);

            if (fgets(szALine, 1023, fpAlarmCfg) != NULL)
            {
                strAlarmFileLine.append(szALine);
                
                if (strAlarmFileLine.find(szErrorType) != -1)
                {
                    /*赋值AlarmId*/
                    nEqualPos = strAlarmFileLine.find("=");
                    nSemiPos = strAlarmFileLine.find(";");
                    strTmpStr = strAlarmFileLine.substr(nEqualPos+1, nSemiPos-nEqualPos);
                    m_WarnData.AlarmId = (unsigned short)atoi(strTmpStr.c_str());

                    /*赋值AlarmType*/
                    nEqualPos = strAlarmFileLine.find("=", nSemiPos);
                    nSemiPos = strAlarmFileLine.find(";", nEqualPos);
                    nStrLen = strTmpStr.length();
                    strTmpStr.erase(0, nStrLen);
                    strTmpStr = strAlarmFileLine.substr(nEqualPos+1, nSemiPos-nEqualPos);
                    m_WarnData.AlarmType = (unsigned char)atoi(strTmpStr.c_str());

                    bRnt = true;
                    break;
                }
            }
            else
            {
                bRnt = false;
                break;
            }
        }
    }

    fclose(fpAlarmCfg);
    fpAlarmCfg = NULL;
    
    return bRnt;
}

bool CSunAlarm::ConstructAD(char* pLocation)
{
    char cTmpBuff[10];  
 
    /*赋值length, Location[]*/
    if ((int) strlen(pLocation) < PARAM_MAX_LEN )
    {   
        m_WarnData.Len = strlen(pLocation);
        memset(cTmpBuff, 0, 10);
        ACE_OS_String::itoa(strlen(pLocation), cTmpBuff, 10);
        memcpy( &m_WarnData.Paras, pLocation, strlen(pLocation));
    }
    else
    {
        m_WarnData.Len = PARAM_MAX_LEN;
        memset(cTmpBuff, 0, 10);
        ACE_OS_String::itoa(PARAM_MAX_LEN, cTmpBuff, 10);
        memcpy(&m_WarnData.Paras, pLocation, PARAM_MAX_LEN);
    }
    
    return true;
}

/***************************************************************************
 *函数功能：去掉字符串左边的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 **************************************************************************/
void CSunAlarm::StringLTrim(char *szStr)
{
    char* szTemp = NULL;

    szTemp = szStr;

    while ((*szTemp == ' ')  || (*szTemp == '\t') 
        || (*szTemp == '\r') || (*szTemp == '\n'))
    {
        szTemp++;
    }
    
    //by lx SWPD01788
    if(szTemp != szStr)
    {
    	strcpy(szStr, szTemp);
    }
    //修改结束by lx
}

/************************************************************************
 *函数功能：去掉字符串右边的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 ************************************************************************/
void CSunAlarm::StringRTrim(char *szStr)
{
    char *szTemp = NULL;
    char ch;
    int  nLen;

    nLen = strlen(szStr);

    szTemp = szStr + nLen;

    while (szTemp != szStr)
    {
        szTemp--;
        ch = *szTemp;
        if ((ch == ' ') || (ch == '\n') 
         || (ch == '\t') || (ch == '\r'))
        {
            *szTemp = '\0';
        }
        else
        {
            break;
        }
    }
}

/*************************************************************************
 *函数功能：去掉字符串左右的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 *************************************************************************/
void CSunAlarm::StringLRTrim(char *szStr)
{
    StringLTrim(szStr);
    StringRTrim(szStr);
}
