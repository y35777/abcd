      /*******************************************************************
**
**  ��Ʒ����    SoftX3000
**  ģ������    ����TRAP�������ͻ������ģ��
**  �ļ�����    sun_alarm.cpp
**  ������Ϣ��  
**
**  ���ߣ�      ֣���棨34756��
**  �汾˵����  V100R001
**  ��Ȩ��Ϣ��  ��Ϊ�������޹�˾
**  ����ʱ�䣺  2002.11.15
**  �޸ļ�¼��  
**  1�����ߣ�   
**        ���ڣ�
**        �޸�˵����
**                  
**  2������
**
********************************************************************/
#include "sun_alarm.h"


//*****************************************************
//  �� �� ��:   CSunAlarm::CSunAlarm
//  ��    ��:   ���캯��
//  �������:   ��
//  �������:   ��
//  ���ú���:   ��
//*****************************************************
CSunAlarm::CSunAlarm()
{
    m_hReadSysLogThread = 0;
    m_nReadSysLogRunning = false;
    m_szKeyWordFile[0]  = '\0';
    m_szAlarmIDFile[0]  = '\0';
}

//*****************************************************
//  �� �� ��:   CSunAlarm::~CSunAlarm
//  ��    ��:   ��������
//  �������:   ��
//  �������:   ��
//  ���ú���:   ��
//*****************************************************
CSunAlarm::~CSunAlarm()
{
    ExitRcvThread();
    m_pGlobalFunc = NULL;
}

//*****************************************************
//  �� �� ��:   CSunAlarm::StartRcvThread(...)
//  ��    ��:   ������ϵͳ��־�̺߳ʹ���ϵͳ�澯֡�߳�
//  �������:   const char *szCfgPath  �澯�������ļ�·��
//              int nPort = 162                 
//              const char *szErrFile  ���д�����־��¼�ļ���
//  �������:   ��
//  ���ú���:   LogExecutionThread()
//              ScheduleExecutionThread()
//  �� �� ֵ:   bool
//              false �����߳��������ɹ�
//              true  �����߳������ɹ�
//*****************************************************
bool CSunAlarm::StartRcvThread(const char *szCfgPath, int nPort)
{   
    // ��ʼ���澯�������ļ��͹ؼ��������ļ�·��
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

    //��ʼ���ؼ��������ļ�(ȫ·��)      
    sprintf(m_szKeyWordFile, "%s%s", 
            szPath, S_SUN_HD_KEYWORD_FILE);

    //��ʼ���澯�������ļ�(ȫ·��)      
    sprintf(m_szAlarmIDFile, "%s%s", 
            szPath, S_SUN_HD_EVENT_FILE);

    if(!ReadAlarmKeyword())
    {
        m_strError = "HDMonitor read sunhdkeyword.ini file failed!";
        return false;
    }

    //������ϵͳ��־�߳�
    int nRet = m_ThreadManager.spawn((ACE_THR_FUNC)LogExecutionThread,
                                      (void*)this,
                                      THR_NEW_LWP | THR_JOINABLE,
                                      &m_hReadSysLogThread);
    
    //���������ϵͳ��־�߳�ʧ�ܣ����˳�
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
//  �� �� ��:   CSunAlarm::ExitRcvThread()
//  ��    ��:   �˳������̣߳�����LogExecutionThread�̺߳�Schedule�̣߳�
//  �������:   ��
//  �������:   ��
//  ���ú���:   ��
//  �� �� ֵ:   bool
//*****************************************************
bool CSunAlarm::ExitRcvThread()
{
    if(!CHardwareAlarm::ExitRcvThread())
    {
        return false;
    }
    
	if (m_nReadSysLogRunning)
	{
		m_IsExit = true; //�����˳��̱߳�־

	    //�ر�m_hTrapThread���
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
//  �� �� ��:   CSunAlarm::LogExecutionThread()
//  ��    ��:   ������ϵͳ��־�߳�
//  �������:   void* param
//  �������:   ��
//  ���ú���:   LogInspect()
//  �� �� ֵ:   void
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
//  �� �� ��:   CSunAlarm::LogInspect(void)
//  ��    ��:   ������־�ļ�����
//  �������:   ��
//  �������:   ��
//  ���ú�����  ��
//  �� �� ֵ:   bool
//*****************************************************
bool CSunAlarm::LogInspect(void)
{
    char szLogFileLine[1024];       //�洢��־�ļ����ж�ȡ����
    long int nOldFileSize = 0;      //��¼�ϴζ�ȡ��ϵͳ��־�ļ���С
    long int nNewFileSize = 0;      //��¼���ζ�ȡ��ϵͳ��־�ļ���С
    char szFullString[1024] = "";   //�洢��ϵͳ��־��ȡ����־����
    char szErrorType[50]    = "";   //��������
    int nSeekRnt = 0;               //�����ļ�����ֵ
    FILE* pLogFp = NULL;            //��־�ļ����
     
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
                nOldFileSize = 0;   //��־�ļ��Ѿ���棬��ͷ��ʼ����
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
            
            //����ϵͳ��־�ļ������һ����־��¼
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

        ACE_OS::sleep(INSPECT_TIME);  //�ȴ�ʱ����
    }

    return true;
}

bool CSunAlarm::ReadAlarmKeyword(void)
{
    FILE* fp = NULL;                            //�ļ����
    char szKeyWord[KEYWORD_MAX_LEN] = "";       //�ַ�����ʾ��Ҫ���ҵ�����
    char szFileLine[512];                       //�ļ�������
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
            pszTemp++;      //������ߵ�����
            if (pszFindStr > (pszTemp + 1))
            {
                //�������ؼ������ҵ�����
                strncpy(szKeyword, pszTemp, pszFindStr - pszTemp - 1);
                if (strstr(pszLocation, szKeyword) == NULL)
                {
                    bMatched = false;
                    break;      //�������ؼ��֣�����
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
            //���澯֡���뷢�ͻ��������
            SetAlarm(m_WarnData, m_WarnData.Len);
        }
    }

    return;
}

bool CSunAlarm::ReadAlarmCfg(char* szErrorType)
{
    FILE* fpAlarmCfg;                //ALARMID�����ļ����
    char szALine[1024];              //�ļ�������
    std::string strAlarmFileLine;    //�ļ�������

    bool bRnt = false;      //����ֵ
    int nEqualPos = 0;      //�Ⱥ�λ��
    int nSemiPos = 0;       //�ֺ�λ��
    std::string strTmpStr;  //��ʱ�����ַ���
    int nStrLen;            //�����ַ�����

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
            memset(szALine, 0, 1024); //��ʼ���ַ�������
            nStrLen = strAlarmFileLine.length();
            strAlarmFileLine.erase(0, nStrLen);
            nStrLen = strTmpStr.length();
            strTmpStr.erase(0, nStrLen);

            if (fgets(szALine, 1023, fpAlarmCfg) != NULL)
            {
                strAlarmFileLine.append(szALine);
                
                if (strAlarmFileLine.find(szErrorType) != -1)
                {
                    /*��ֵAlarmId*/
                    nEqualPos = strAlarmFileLine.find("=");
                    nSemiPos = strAlarmFileLine.find(";");
                    strTmpStr = strAlarmFileLine.substr(nEqualPos+1, nSemiPos-nEqualPos);
                    m_WarnData.AlarmId = (unsigned short)atoi(strTmpStr.c_str());

                    /*��ֵAlarmType*/
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
 
    /*��ֵlength, Location[]*/
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
 *�������ܣ�ȥ���ַ�����ߵĿո�
 *���룺
 *     szStr����������ַ���
 *�����
 *     szStr��������ɵ��ַ���
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
    //�޸Ľ���by lx
}

/************************************************************************
 *�������ܣ�ȥ���ַ����ұߵĿո�
 *���룺
 *     szStr����������ַ���
 *�����
 *     szStr��������ɵ��ַ���
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
 *�������ܣ�ȥ���ַ������ҵĿո�
 *���룺
 *     szStr����������ַ���
 *�����
 *     szStr��������ɵ��ַ���
 *************************************************************************/
void CSunAlarm::StringLRTrim(char *szStr)
{
    StringLTrim(szStr);
    StringRTrim(szStr);
}
