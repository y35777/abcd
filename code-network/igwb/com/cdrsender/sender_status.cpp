/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : sender_status.cpp
 *������Ϣ  �����ļ�����CSenderStatus��ʵ��
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/
#include "sender_status.h"
#include "../include/toolbox.h"

//////////////////////////////////////////////////////////////////////////
/**************************************************************************
 *����ԭ��  : CSenderStatusFile::CSenderStatusFile(const char* szFileName)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
CSenderStatusFile::CSenderStatusFile()
{
    m_pFile = NULL;
}
/**************************************************************************
 *����ԭ��  : CSenderStatusFile:: ~CSenderStatusFile()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
CSenderStatusFile:: ~CSenderStatusFile()
{
    Close();
}
/**************************************************************************
 *����ԭ��  : BOOL CSenderStatusFile::Open()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
BOOL CSenderStatusFile::Open(const char* szFileName)
{
    ASSERT(szFileName != NULL);
    STRNCPY(m_szFilePath, szFileName, MAX_PATH);

    if (m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
    
    m_pFile = fopen(m_szFilePath, "rb+");
    if (m_pFile == NULL)
    {
        m_pFile = fopen(m_szFilePath, "wb+");
    }

    if(m_pFile != NULL) 
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/**************************************************************************
 *����ԭ��  : void CSenderStatusFile::Close()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
void CSenderStatusFile::Close()
{
    if (m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}
/**************************************************************************
 *����ԭ��  : BOOL CSenderStatusFile::Write(void* pData, UINT4 uLen)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
BOOL CSenderStatusFile::Read(void* pData, UINT4 uLen)
{
    if(NULL == m_pFile)
    {
        return FALSE;
    }

    UINT4 uReadLen = fread(pData, sizeof(BYTE), uLen, m_pFile);
    if(uReadLen == uLen)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/**************************************************************************
 *����ԭ��  : BOOL CSenderStatusFile::Read(void* pData, UINT4 uLen)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
BOOL CSenderStatusFile::Write(void* pData, UINT4 uLen)
{
    if(m_pFile == NULL)
    {
        return FALSE;
    }

    UINT4 uReadLen = fwrite(pData, sizeof(BYTE), uLen, m_pFile);
    int nRet = fflush(m_pFile);
    if(uReadLen == uLen && nRet == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/**************************************************************************
 *����ԭ��  : UINT4 CSenderStatusFile::GetOffset(UINT4 nTaskID)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
INT4 CSenderStatusFile::GetOffset(UINT4 nTaskID)
{
    ASSERT(m_pFile != NULL);
    
    int nRet = fseek(m_pFile, 0, SEEK_SET);
    if (nRet != 0)
    {
        return -1;
    }

    TStatusRecord Record;

    int nOffset = 0;
    BOOL bFind = FALSE;

    while(fread(&Record, sizeof(TStatusRecord), 1 , m_pFile) == 1)
    {
        if (nTaskID == Record.m_nTaskID)
        {
            nOffset = ftell(m_pFile) - sizeof(TStatusRecord);
            bFind = TRUE;
			break;
        }
    }
    
    if(!bFind)
    {
        nOffset = ftell(m_pFile);
    }
    return nOffset; 
}
/**************************************************************************
 *����ԭ��  : BOOL GetRecord(UINT4 nID, TStatusRecord* pInfo)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
BOOL CSenderStatusFile::GetRecord(UINT4 nTaskID, TStatusRecord* pRecInfo)
{
    ASSERT(pRecInfo != NULL);
    
    memset(pRecInfo, 0, STATUSINFOLENGTH);
    
    INT4 nOffset = GetOffset(nTaskID);

    if (nOffset < 0)
    {
        return FALSE;
    }

    if (!SetOffset(nOffset))
    {
        return FALSE;
    }

    if (fread(pRecInfo, STATUSINFOLENGTH, 1, m_pFile) != 1)
    {
        return FALSE;
    }
    
    return TRUE;
}

/**************************************************************************
 *����ԭ��  : BOOL  CSenderStatusFile::SetOffset(UINT4 nOffset, INT nPos)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
BOOL  CSenderStatusFile::SetOffset(UINT4 nOffset, INT POS)
{
    if (m_pFile == NULL)
    {
        return FALSE;
    }

    int nRet =0;
    nRet = fseek(m_pFile, nOffset, POS); 
    if ( nRet == 0)
    {
        return TRUE;
    }
    return FALSE;
}
