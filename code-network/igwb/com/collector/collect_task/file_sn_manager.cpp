#include "file_sn_manager.h"
#include "collect_include.h"
#include "../include/adapter_include.h"
#include "../../include/toolbox.h"

#include <algorithm>
#include <math.h>

CFileSNMgr::CFileSNMgr(const char* szStatusFile, 
                       int nCheckSNDepth, 
                       int nSNLen,
                       int nPostfixLen,
                       GETFILENAMEBYSNFUN pGetNameFun)
{
    SetStatusFileName(szStatusFile);
    SetCheckSNDepth(nCheckSNDepth);

    SetSNLen(nSNLen);
    SetPostfixLen(nPostfixLen);

    if (pGetNameFun == NULL)
    {
        SetGetNameFun(GetFileNameBySN);
    }
    else
    {
        SetGetNameFun(pGetNameFun);
    }
    m_nSNDownLimit = 0;
    m_pStatusFile  = NULL;
}

CFileSNMgr::~CFileSNMgr()
{
    if(m_pStatusFile != NULL)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
    }

}

int CFileSNMgr::Init()
{
    int nRet = 0;

    nRet = LoadStatus();
    if (ERR_SUCCESS != nRet)
    {
        COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
               "警告:%s打开状态文件失败",
               m_szMgrInfo);

        COTRACE("警告:%s打开状态文件失败",
               m_szMgrInfo);

    }
    else
    {
        m_nFileSN = GetFileSNByName(m_szFileName);
    }

    return nRet;
}

BOOL CFileSNMgr::CheckFileSN(const char* szFile, char* szPreviousFile)
{
    int nFileSN = GetFileSNByName(szFile);
    if((nFileSN >= m_nSNUpLimit) || (nFileSN < m_nSNDownLimit))
    {
        return TRUE;
    }

    //得到AMA话单文件的序列号
    BOOL bResult = FALSE;

    //如果是新的状态文件，直接保存序列号
    if (m_bNewStatusFile)
    {
        SetFileName(szFile);
        m_nFileSN = GetFileSNByName(m_szFileName);

        SaveStatus();

        m_bNewStatusFile = FALSE;

        return TRUE;
    }

    //查找此文件是否是漏采的文件
    VECTOR<UINT4>::iterator i = m_LostFileSNlist.begin();
    for (; i != m_LostFileSNlist.end(); i++)
    {
        if(*i == nFileSN)
        {
            m_LostFileSNlist.erase(i);
            SaveStatus();
            return TRUE;
        }
    }

    //检查是否有到达检查深度的漏采话单文件
    //如果有，则发送漏采话单告警
    if (m_LostFileSNlist.size() > 0)
    {
        i = m_LostFileSNlist.begin();

        if(((nFileSN < *i + (m_nSNUpLimit - m_nSNDownLimit) / 2) 
            && (nFileSN >= *i + m_nCheckSNDepth))
           || ((nFileSN + (m_nSNUpLimit - m_nSNDownLimit) / 2 < *i) 
            && (nFileSN + (m_nSNUpLimit - m_nSNDownLimit) >= *i + m_nCheckSNDepth)))
        {
            STRING strLostFile = m_pGetNameFun(*i, 
                                        m_szFileName,
                                        m_nSNLen,
                                        m_nPostfixLen);

            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "文件漏采:在深度为%d的文件序列号检查中没有发现文件(%s)",
                   m_nCheckSNDepth,
                   strLostFile.c_str());

            WRITELOG_F("文件漏采:在深度为%d的文件序列号检查中没有发现文件(%s)",
                      m_nCheckSNDepth,
                      strLostFile.c_str());

            SInnerAlarm Alarm;
            Alarm.yAlarmType    = AT_EVENT;
            Alarm.uAlarmID      = ALARM_ID_COLLECT_FILE_LOST;
            Alarm.yAlarmPara[0] = ALARM_PARA_NONE;
            SENDALARM(&Alarm);

            m_LostFileSNlist.erase(i);

            SaveStatus();
        }
    }

    //判断序列号是否连续
    if ((nFileSN == m_nFileSN + 1)
        || (nFileSN == m_nFileSN + 1 - (m_nSNUpLimit - m_nSNDownLimit)))
    {
        bResult = TRUE;
    }
    else
    {
        bResult = TRUE;
        strcpy(szPreviousFile, m_szFileName);

        int j = 0;
        if ((nFileSN < m_nFileSN + (m_nSNUpLimit - m_nSNDownLimit) / 2) 
             && (nFileSN > m_nFileSN))
        {
            for (j = m_nFileSN + 1; j < nFileSN; j++)
            {
                m_LostFileSNlist.push_back(j);
            }
        }
        else if((nFileSN + (m_nSNUpLimit - m_nSNDownLimit) / 2 < m_nFileSN) 
                && (nFileSN + (m_nSNUpLimit - m_nSNDownLimit) >= m_nFileSN))
        {
            for (j = m_nFileSN + 1; j < m_nSNUpLimit; j++)
            {
                m_LostFileSNlist.push_back(j);
            }

            for (j = m_nSNDownLimit; j < nFileSN; j++)
            {
                m_LostFileSNlist.push_back(j);
            }
        }
    }

    SetFileName(szFile);
    m_nFileSN = GetFileSNByName(m_szFileName);
    SaveStatus();

    return bResult;
}


//加载状态文件
int CFileSNMgr::LoadStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "rb+");
    if(NULL == m_pStatusFile)
    {
        //创建新文件
        m_pStatusFile = fopen(m_szStatusFile, "wb+");

        if (NULL == m_pStatusFile)
        {
            COTRACE("%s打开/创建状态文件(%s)失败",
              m_szMgrInfo,
              m_szStatusFile);
            return ERR_FAIL;
        }
        else
        {
            m_bNewStatusFile = TRUE;
            fclose(m_pStatusFile);
            m_pStatusFile = NULL;

            return ERR_SUCCESS;
        }
    }

    int nFileLength = FileSize(m_pStatusFile);
    if(nFileLength < LEN_OF_FILE_NAME + sizeof(UINT4))
    {
        m_bNewStatusFile = TRUE;
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_SUCCESS;
    }

    //读入文件名
    int nRet = fread(m_szFileName,
                 LEN_OF_FILE_NAME,
                 1,
                 m_pStatusFile);
    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_FAIL;
    }

    //读入漏采的话单文件序列号
    nFileLength = nFileLength - LEN_OF_FILE_NAME;
    int nItemCount = nFileLength / sizeof(UINT4);
    UINT4 *pReadBuffer = new UINT4[nItemCount];
    nRet = fread(pReadBuffer,
                 nFileLength,
                 1,
                 m_pStatusFile);

    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
        delete []pReadBuffer;

        return ERR_FAIL;
    }

    if (nItemCount > 0)
    {
        m_LostFileSNlist.clear();

        int nLostFileCount = pReadBuffer[0];
        for(int i = 0; i < nLostFileCount; i++)
        {
            m_LostFileSNlist.push_back(pReadBuffer[i + 2]);
        }
    }

    fclose(m_pStatusFile);
    m_pStatusFile = NULL;
    delete []pReadBuffer;

    return ERR_SUCCESS;
}

//保存状态文件
int CFileSNMgr::SaveStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "wb");
    if(NULL == m_pStatusFile)
    {
        COTRACE("%s打开/创建状态文件(%s)失败",
              m_szMgrInfo,
              m_szStatusFile);
        return ERR_FAIL;

        return ERR_FAIL;
    }

    //写入文件名
    int nRet = 0;
    nRet = fwrite(m_szFileName,
                  LEN_OF_FILE_NAME,
                  1,
                  m_pStatusFile);

    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_FAIL;
    }

    int nItemCount = m_LostFileSNlist.size() + 1;
    UINT4 *pWriteBuffer = new UINT4[nItemCount];

    pWriteBuffer[0] = m_LostFileSNlist.size();

    for(int i = 0; i < m_LostFileSNlist.size(); i++)
    {
        pWriteBuffer[i + 1] = m_LostFileSNlist[i];
    }


    int nFileLength = nItemCount * sizeof(UINT4);

    nRet = fwrite(pWriteBuffer,
                  nFileLength,
                  1,
                  m_pStatusFile);

    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
        delete []pWriteBuffer;

        return ERR_FAIL;
    }

    nFileLength += LEN_OF_FILE_NAME;

    nRet = fflush(m_pStatusFile);
    fclose(m_pStatusFile);
    m_pStatusFile = NULL;

    delete []pWriteBuffer;

    ACE_OS::truncate(m_szStatusFile, nFileLength);

    return nRet;
}

//设置序列号的长度
void CFileSNMgr::SetSNLen(const int& nSNLen)
{
    m_nSNLen = nSNLen;
    m_nSNUpLimit = pow((float)10, nSNLen);
}

//设置当前文件名
void CFileSNMgr::SetFileName(const char* szFileName)
{
    strncpy(m_szFileName, szFileName, sizeof(m_szFileName));
}

//得到文件序列号
int CFileSNMgr::GetFileSNByName(const char* szFileName)
{
    if ((szFileName == NULL)||(szFileName[0] == '\0')
        ||strlen(szFileName) < m_nSNLen + m_nPostfixLen)
    {
        return 0;
    }

    int nPrefixLen = strlen(szFileName) - m_nSNLen - m_nPostfixLen;
    char* szFileSN = new char[m_nSNLen + 1];

    memcpy(szFileSN, szFileName + nPrefixLen, m_nSNLen);
    szFileSN[m_nSNLen] = '\0';

    if (!IsDigitValue(szFileSN))
    {
        return 0;
        delete[] szFileSN;
    }

    int nFileSN = atoi(szFileSN);
    delete[] szFileSN;

    return nFileSN;
}

//通过文件的序列号计算漏采文件名
STRING CFileSNMgr::GetFileNameBySN(const int   nFileSN,
                                   const char* szReferName,
                                   const int   nSNLen,
                                   const int   nPostfixLen)
{
    //计算前缀
    int nPrefixLen = strlen(szReferName) - nSNLen - nPostfixLen;
    char* szPrefix = new char[nPrefixLen + 1];

    memcpy(szPrefix, szReferName, nPrefixLen);
    szPrefix[nPrefixLen] = '\0';

    //计算序列号
    char szFormat[20];
    sprintf(szFormat, "%%0%dd", nSNLen);
    char* szFileSN = new char[nSNLen + 1];
    SNPRINTF(szFileSN, nSNLen + 1, szFormat, nFileSN);
    szFileSN[nSNLen] = '\0';

    //计算后缀
    char* szPostfix = new char[nPostfixLen + 1];

    memcpy(szPostfix, 
           szReferName + strlen(szReferName) - nPostfixLen, 
           nPostfixLen);
    szPostfix[nPostfixLen] = '\0';

    STRING strFileName = "";
    strFileName =strFileName + szPrefix + szFileSN + szPostfix;

    delete[] szPrefix;
    delete[] szFileSN;
    delete[] szPostfix;

    return strFileName;

}
//added end
