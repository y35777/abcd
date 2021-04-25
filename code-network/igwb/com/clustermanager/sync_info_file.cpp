#include "../include/base_type.h"
#include "sync_info_file.h"


/********************************************************
* 名称: CSyncInfoFile::CSyncInfoFile
* 功能: 构造函数
* 输入: const char* const szFilePath : 文件所在路径
*       const char* const szFileName : 文件名
*       int nAPNum                   : 接入点号
*       int nModule                  : 模块号
*       int nChl                     : 通道号
* 输出:
* 返回:
********************************************************/
CSyncInfoFile::CSyncInfoFile(const char* const szFilePath,
                             const char* const szFileName,
                             int nAPNum,
                             int nModule,
                             int nChl)
{
    m_nBufferLen    = 0;
    m_pBuffer       = NULL;
    m_nSyncDataLen  = 0;
    m_nAccessPoint  = nAPNum;
    m_nModule       = nModule;
    m_nChannel      = nChl;

    strncpy(m_szFilePath, szFilePath, MAX_PATH);
    m_szFilePath[MAX_PATH - 1] = '\0';

    strncpy(m_szFileName, szFileName, MAX_PATH);
    m_szFileName[MAX_PATH - 1] = '\0';

    strncpy(m_szFullFileName, m_szFilePath, MAX_PATH);
    m_szFullFileName[MAX_PATH - 1] = '\0';

    if((strlen(m_szFullFileName) > 0) &&
       (m_szFullFileName[strlen(m_szFullFileName) - 1] != '\\') &&
       (m_szFullFileName[strlen(m_szFullFileName) - 1] != '/'))
    {
        strncat(m_szFullFileName, "/", MAX_PATH);
        m_szFullFileName[MAX_PATH - 1] = '\0';
    }

    strncat(m_szFullFileName, m_szFileName, MAX_PATH);
    m_szFullFileName[MAX_PATH - 1] = '\0';
}

/********************************************************
* 名称: CSyncInfoFile::~CSyncInfoFile
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CSyncInfoFile::~CSyncInfoFile()
{
    if (NULL != m_pBuffer)
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
    }
}

/********************************************************
* 名称: CSyncInfoFile::GetAccessPoint
* 功能: 获得接入点号
* 输入:
* 输出:
* 返回: int 接入点号
********************************************************/
int CSyncInfoFile::GetAccessPoint()
{
    return m_nAccessPoint;
}

/********************************************************
* 名称: CSyncInfoFile::GetModule
* 功能: 获得模块号
* 输入:
* 输出:
* 返回: int 模块号
********************************************************/
int CSyncInfoFile::GetModule()
{
    return m_nModule;
}

/********************************************************
* 名称: CSyncInfoFile::GetChannel
* 功能: 获得通道号
* 输入:
* 输出:
* 返回: int 通道号
********************************************************/
int CSyncInfoFile::GetChannel()
{
    return m_nChannel;
}

/********************************************************
* 名称: CSyncInfoFile::GetSyncDataLen
* 功能: 获得同步数据长度
* 输入:
* 输出:
* 返回: int 同步数据长度
********************************************************/
int CSyncInfoFile::GetSyncDataLen()
{
    return m_nSyncDataLen;
}

/********************************************************
* 名称: CSyncInfoFile::GetLastWriteTime
* 功能: 取状态文件的最新修改时间
* 输入:
* 输出:
* 返回: time_t
********************************************************/
time_t CSyncInfoFile::GetLastWriteTime()
{
    //获取m_szFileName指向文件的最新更新时间
    ACE_stat FStat;
    memset((char*)&FStat, 0, sizeof(FStat));

    ACE_OS::stat(m_szFullFileName, &FStat);

    return FStat.st_mtime;
}
