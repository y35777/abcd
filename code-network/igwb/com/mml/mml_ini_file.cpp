// mml_ini_file.h.cpp: implementation of the CMmlIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "mml_ini_file.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMmlIniFile::CMmlIniFile(const char *szFileName):CINIFile(szFileName) , m_pFileBuffer(NULL)
{
}
CMmlIniFile::CMmlIniFile():CINIFile("")
{
    m_pFileBuffer = NULL;
}

CMmlIniFile::~CMmlIniFile()
{
    if(m_pFileBuffer)
    {
        delete[] m_pFileBuffer;
        m_pFileBuffer = NULL;
    }
}

int CMmlIniFile::Open(const char *pszFileName)
{
    if(m_pFile)
    {
        Close();
    }
    strncpy(m_szFileName, pszFileName, MAX_PATH);
    m_szFileName[MAX_PATH - 1] = '\0';
    StringLRTrim(m_szFileName);
    //文件长度不能超过512K
    if(m_pFileBuffer == NULL)
        m_pFileBuffer = new BYTE[512 * 1024];
    return Open();
}

int CMmlIniFile::DelSection(const char *szSection)
{
    return ReplaceSecWithStr(szSection,NULL);
}

int CMmlIniFile::DeleteKey(const char *szSection,const char *szKey)
{
    if(!IsOpen())
    {
        if(!Open())
            return FALSE;
    }
    if(!ReadFileToBuffer())
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

int CMmlIniFile::ReplaceSecWithStr(const char *szSection, const char *szSubstitution)
{
    if(!IsOpen())
    {
        if(!Open())
            return FALSE;
    }
    if(!ReadFileToBuffer())
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

int CMmlIniFile::TruncateFile()
{
    Close();
    ACE_OS::truncate(m_szFileName,m_lFileSize);
    return TRUE;
}

int CMmlIniFile::ReadFileToBuffer()
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
int CMmlIniFile::WriteInt(const char *szSectionName,
                          const char *szKeyName,
                          UINT4 uKeyValue)
{
    if(!IsOpen())
    {
        if(!Open())
            return FALSE;
    }

    return CINIFile::WriteInt(szSectionName,szKeyName,uKeyValue);
}
int CMmlIniFile::WriteString(const char *szSectionName,
                             const char *szKeyName,
                             const char *szKeyValue)
{
    if(!IsOpen())
    {
        if(!Open())
            return FALSE;
    }
    return CINIFile::WriteString(szSectionName,szKeyName,szKeyValue);
}
