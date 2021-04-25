#include "log_bin_util.h"
#include "../include/resource.h"
#include "../include/toolbox.h"


CLogBinUtil::CLogBinUtil()
{
    m_fpLogFile  = NULL;
    m_nReadIndex = 0;
}

CLogBinUtil::~CLogBinUtil()
{
    CloseFile();
}

//����־�ĵ����ļ�������������ָ���ļ�
int CLogBinUtil::OpenFile(const char * const szFilePath)
{
    CloseFile();
    
    if(szFilePath != NULL)
    {
        m_fpLogFile = fopen(szFilePath, "rb+");
        if ( m_fpLogFile != NULL)
        {
            // ��ȡ��ǰ��־�ļ��ĳ���
            ACE_stat astat;
            int nRet = ACE_OS::stat(szFilePath, &astat);
            if ( nRet != 0)
            {
                TRACE(MTS_LOG, S_LOG_GET_LOG_STAT_FAIL, szFilePath);
                fclose(m_fpLogFile);
                m_fpLogFile = 0;
                return -1;
            }
            long length = astat.st_size;
            
            // ���ļ��Ķ�дָ���Ƶ����һ����������¼���ļ��Ŀ�ʼ��
            long offset = length % sizeof(SLog);
            nRet = fseek(m_fpLogFile, -offset, SEEK_END);
            if ( nRet != 0) 
            {
                TRACE(MTS_LOG, S_LOG_SEEK_FAIL);
                fclose(m_fpLogFile);
                m_fpLogFile = 0;
                return -1;
            }
        }
        else 
        {
            if((m_fpLogFile = fopen(szFilePath,"wb+")) == NULL)
            {
                TRACE(MTS_LOG, S_LOG_OPEN_LOG_FAIL, szFilePath);
                return -1;
            }
        }

        return 0;
    }

    return -1;
}

//�ر��ļ�
int CLogBinUtil::CloseFile(void)
{
    if(NULL != m_fpLogFile)
    {
        fclose(m_fpLogFile);
        m_fpLogFile = NULL;
    }

    return 0;
}

//���ʣ�����־��
int CLogBinUtil::GetRemainCount(void)
{
    if(NULL != m_fpLogFile)
    {
        int nFileSize = FileSize(m_fpLogFile);
        int nCurPos   = ftell(m_fpLogFile);

        if(nFileSize >=0 && nCurPos >= 0)
        {
            return (nFileSize - nCurPos) / sizeof(SLog);
        }
    }

    return 0;
}

//дһ����־��Ϣ
int  CLogBinUtil::AppendLog(UINT4       uSerialNo,
                            BYTE        LogType,
                            const char* szUserName,
                            UINT4       uIPAddr,
                            time_t      tLogTime,
                            BYTE        OperResult,
                            const char* szOperCmd,
                            const char* szCmdDesc)
{
    SLog log;
	log.uSerialNo   = uSerialNo;
    log.LogType     = LogType;
	strncpy(log.szUserName,szUserName,sizeof(log.szUserName));
	log.szUserName[sizeof(log.szUserName) - 1] = '\0';
    log.uIPAddr     = uIPAddr;
    log.tLogTime    = tLogTime; 
    log.OperResult  = OperResult;
	strncpy(log.szOperCmd,szOperCmd,sizeof(log.szOperCmd));
	log.szOperCmd[sizeof(log.szOperCmd) - 1] = '\0';
	strncpy(log.szCmdDesc,szCmdDesc,sizeof(log.szCmdDesc));
	log.szCmdDesc[sizeof(log.szCmdDesc) - 1] = '\0';
                                     
    int nRet = fwrite(&log, sizeof(SLog), 1, m_fpLogFile); 
    if ( nRet != 1
        || fflush(m_fpLogFile) != 0)
    {
        return -1;
    }        
    
    return 0;
}

//��һ����־��Ϣ
int  CLogBinUtil::ReadLog(int     whence,
                          UINT4&  uSerialNo,
                          BYTE&   LogType,
                          char*   szUserName,
                          int     nUserNameSize,
                          UINT4&  uIPAddr,
                          time_t& tLogTime,
                          BYTE&   OperResult,
                          char*   szOperCmd,
                          int     nOperCmdSize,
                          char*   szCmdDesc,
                          int     nCmdDescSize)
{
	//������־��¼�����ҷ���Ҫ��ļ�¼
    int nRet      = -1;
    int nCount    = FileSize(m_fpLogFile)/sizeof(SLog);
    
    switch(whence) 
    {
    case EL_FIRST:
        m_nReadIndex = 0;
        nRet = ReadOneLog(m_nReadIndex, 
                          uSerialNo,
                          LogType,
                          szUserName,
                          nUserNameSize,
                          uIPAddr,
                          tLogTime,
                          OperResult,
                          szOperCmd,
                          nOperCmdSize,
                          szCmdDesc,
                          nCmdDescSize);
    	break;

    case EL_NEXT:
        if(m_nReadIndex >= nCount)
        {
            return 1;
        }
        m_nReadIndex++;
        nRet = ReadOneLog(m_nReadIndex, 
                          uSerialNo,
                          LogType,
                          szUserName,
                          nUserNameSize,
                          uIPAddr,
                          tLogTime,
                          OperResult,
                          szOperCmd,
                          nOperCmdSize,
                          szCmdDesc,
                          nCmdDescSize);
    	break;

    case EL_FINAL:
        nRet = ReadOneLog(nCount - 1, 
                          uSerialNo,
                          LogType,
                          szUserName,
                          nUserNameSize,
                          uIPAddr,
                          tLogTime,
                          OperResult,
                          szOperCmd,
                          nOperCmdSize,
                          szCmdDesc,
                          nCmdDescSize);
    	break;

    default:
        break;

    }

    return nRet;
}

//��һ����־��Ϣ
//��һ����־��Ϣ
int  CLogBinUtil::ReadOneLog(int     nIndex,
                             UINT4&  uSerialNo,
                             BYTE&   LogType,
                             char*   szUserName,
                             int     nUserNameSize,
                             UINT4&  uIPAddr,
                             time_t& tLogTime,
                             BYTE&   OperResult,
                             char*   szOperCmd,
                             int     nOperCmdSize,
                             char*   szCmdDesc,
                             int     nCmdDescSize)
{
    SLog log;
    fseek(m_fpLogFile, nIndex*sizeof(log), SEEK_SET);
    int nRead = fread(&log, sizeof(log), 1, m_fpLogFile);
    if(1 != nRead)
    {
        return -1;
    }
	uSerialNo   = log.uSerialNo;
    LogType     = log.LogType;
	strncpy(szUserName,log.szUserName,nUserNameSize);
	szUserName[nUserNameSize - 1] = '\0';
    uIPAddr     = log.uIPAddr;
    tLogTime    = log.tLogTime; 
    OperResult  = log.OperResult;
	strncpy(szOperCmd,log.szOperCmd,nOperCmdSize);
	szOperCmd[nOperCmdSize - 1] = '\0';
	strncpy(szCmdDesc,log.szCmdDesc,nCmdDescSize);
	szCmdDesc[nCmdDescSize - 1] = '\0';

    return 0;
}
