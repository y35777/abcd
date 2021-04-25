/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : sender_status.cpp
 *描述信息  ：本文件是类CSenderStatus的实现
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#include "sender_status.h"
#include "../include/toolbox.h"

//////////////////////////////////////////////////////////////////////////
/**************************************************************************
 *函数原型  : CSenderStatusFile::CSenderStatusFile(const char* szFileName)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
CSenderStatusFile::CSenderStatusFile()
{
    m_pFile = NULL;
}
/**************************************************************************
 *函数原型  : CSenderStatusFile:: ~CSenderStatusFile()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
CSenderStatusFile:: ~CSenderStatusFile()
{
    Close();
}
/**************************************************************************
 *函数原型  : BOOL CSenderStatusFile::Open()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : void CSenderStatusFile::Close()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : BOOL CSenderStatusFile::Write(void* pData, UINT4 uLen)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : BOOL CSenderStatusFile::Read(void* pData, UINT4 uLen)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : UINT4 CSenderStatusFile::GetOffset(UINT4 nTaskID)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : BOOL GetRecord(UINT4 nID, TStatusRecord* pInfo)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : BOOL  CSenderStatusFile::SetOffset(UINT4 nOffset, INT nPos)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
