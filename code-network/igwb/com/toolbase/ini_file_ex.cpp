// ini_file_ex.cpp: implementation of the CINIFileEx class.
//
//////////////////////////////////////////////////////////////////////
#include "../include/export_dll.h"
#include "ini_file_ex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CINIFileEx::CINIFileEx(const char *szFileName):CINIFile(szFileName)
{
    m_pFileBuffer = NULL;
}

CINIFileEx::~CINIFileEx()
{
    if(m_pFileBuffer != NULL)
    {
        delete[] m_pFileBuffer;
        m_pFileBuffer = NULL;
    }
}

BOOL CINIFileEx::Open()
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }

    if (strlen(m_szFileName) == 0)
    {
        return FALSE;
    }

    m_pFile = fopen(m_szFileName, "rb+");
    if(m_pFile == NULL)
    {
        m_pFile = fopen(m_szFileName, "wb+");
    }

    if(m_pFile != NULL)
    {
        fseek(m_pFile,0,SEEK_END);
        m_lFileSize = ftell(m_pFile);
    }

    //文件长度不能超过512K
    if(m_pFileBuffer == NULL)
        m_pFileBuffer = new BYTE[512 * 1024];
    
    return (m_pFile != NULL);
}

int CINIFileEx::DelSection(const char *szSection)
{
    return ReplaceSecWithStr(szSection,NULL);
}

int CINIFileEx::DeleteKey(const char *szSection,const char *szKey)
{
    if(!IsOpen())
    {
        if(!Open())
            return FALSE;
    }
    if(!LoadFile())
        return FALSE;
    int nLen = 0;
    char *pPos1 = strstr((char *)m_pFileBuffer,szSection);
    if(pPos1 == NULL)
        return FALSE;
    char *pPos2 = strstr(pPos1,szKey);
    if(pPos2 == NULL)
        return FALSE;
    char *pPos3 = strstr(pPos2,"\n");
    if(pPos3 == NULL)
    {
        //表明是最后一个"Key=Value"
        *pPos1 = '\0';
        nLen = strlen((char *)m_pFileBuffer);
    }
    else
    {
        pPos3 ++;
        STRING szRemainder = pPos3;
        *pPos2 = '\0';
        strcat((char *)m_pFileBuffer,szRemainder.c_str());
        nLen = strlen((char *)m_pFileBuffer);
    }
    rewind(m_pFile);
    fwrite(m_pFileBuffer,1,nLen,m_pFile);
    m_lFileSize = nLen;
    TruncateFile();
    return TRUE;
}

int CINIFileEx::ReplaceSecWithStr(const char *szSection, const char *szSubstitution)
{
    if(!IsOpen())
    {
        if(!Open())
            return FALSE;
    }
    if(!LoadFile())
        return FALSE;
    int nLen = 0;
    char *pPos1 = strstr((char *)m_pFileBuffer,szSection);
    if(pPos1 == NULL)
        return FALSE;
    char *pPos2 = strstr((pPos1+1),"[");
    if(pPos2 == NULL)
    {
        //表明是最后一节
        *pPos1 = '\0';
        if(szSubstitution)
            strcat((char *)m_pFileBuffer,szSubstitution);
        nLen = strlen((char *)m_pFileBuffer);
    }
    else
    {
        STRING szRemainder = pPos2;
        *pPos1 = '\0';
        if(szSubstitution)
            strcat((char *)m_pFileBuffer,szSubstitution);
        strcat((char *)m_pFileBuffer,szRemainder.c_str());
        nLen = strlen((char *)m_pFileBuffer);
    }
    rewind(m_pFile);
    fwrite(m_pFileBuffer,1,nLen,m_pFile);
    m_lFileSize = nLen;
    TruncateFile();
    return TRUE;
}

int CINIFileEx::TruncateFile()
{
    Close();
    ACE_OS::truncate(m_szFileName,m_lFileSize);
    return TRUE;
}

int CINIFileEx::LoadFile()
{
    if(!IsOpen())
        return FALSE;
    fseek(m_pFile,0,SEEK_END);
    int nLen = ftell(m_pFile);
    rewind(m_pFile);
    //文件长度不能超过1M
    if(m_pFileBuffer == NULL)
        m_pFileBuffer = new BYTE[1024 * 1024];
    fread(m_pFileBuffer,1,nLen,m_pFile);
    m_pFileBuffer[nLen] = '\0';
    return TRUE;
}
