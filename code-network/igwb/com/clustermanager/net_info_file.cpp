#include "../include/base_type.h"
#include "net_info_file.h"


/********************************************************
* 名称: CNetInfoFile::CNetInfoFile
* 功能: 构造函数
* 输入: const char* const szFilePath : 文件所在路径
*       const char* const szFileName : 文件名
*       int nAPNum                   : 接入点号
*       int nModule                  : 模块号
*       int nChl                     : 通道号
* 输出:
* 返回:
********************************************************/
CNetInfoFile::CNetInfoFile(const char* const szFilePath,
                             const char* const szFileName,
                             int nAPNum,
                             int nModule,
                             int nChl)
                             :CSyncInfoFile(szFilePath,
                                            szFileName,
                                            nAPNum,
                                            nModule,
                                            nChl)
{
    m_pFile = NULL;
}

/********************************************************
* 名称: CNetInfoFile::~CNetInfoFile
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CNetInfoFile::~CNetInfoFile()
{
    if(NULL != m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

/********************************************************
* 名称: CNetInfoFile::Open
* 功能: 状态文件打开操作，只有被同步方会调用
* 输入:
* 输出:
* 返回: int 打开操作是否成功
********************************************************/
int CNetInfoFile::Open()
{
    m_nBufferLen = 0;
    m_nSyncDataLen = 0;
    if (NULL != m_pBuffer)
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
    }

    if(NULL == m_pFile)
    {
        //用fopen函数以二进制读写方式打开m_szFileName指向的文件
        //Opens for both reading and writing. (The file must exist.)
        m_pFile = fopen(m_szFullFileName, "rb+");
        if(NULL == m_pFile)
        {
            return ERR_SUCCESS;
        }
    }

    //取文件长度
    if(fseek(m_pFile, 0, SEEK_END) != 0)
    {
        return errno;
    }
    int nFileLen = ftell(m_pFile);
    if(fseek(m_pFile, 0, SEEK_SET) != 0)
    {
        return errno;
    }

    //读出数据
    if (nFileLen > 0)
    {
        //创建一块大小为m_nBufferLen的内存，其指针赋给m_pBuffer
        m_nBufferLen   = nFileLen;
        m_nSyncDataLen = m_nBufferLen;
        m_pBuffer      = new char[m_nBufferLen];

        //从文件中读出所有内容，置于m_pBuffer缓冲区内
        if(fread(m_pBuffer, m_nBufferLen, 1, m_pFile) != 1)
        {
            return errno;
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CNetInfoFile::Close
* 功能: 状态文件关闭操作，只有被同步方会调用
* 输入:
* 输出:
* 返回: void
********************************************************/
void CNetInfoFile::Close()
{
    if(NULL != m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

/********************************************************
* 名称: CNetInfoFile::GetSyncData
* 功能: 获取同步数据操作
* 输入: nData     : 输出缓冲区的指针
*       nLen      : 输出缓冲区的长度
*       bReadFile : 是否从文件中读出新的数据，TRUE表示要
*                   从文件中读出，FALSE表示从缓冲区中读出
* 输出:
* 返回: int 获得的实际读出的数据长度
********************************************************/
int CNetInfoFile::GetSyncData(void* pData, int nLen, BOOL bReadFile)
{
    errno = 0;

    if(bReadFile)
    {
        //打开状态文件;
        if (Open() != ERR_SUCCESS)
        {
            return 0;
        }
    }

    if (nLen < m_nBufferLen)
    {
        errno = ERR_BUFFER_NOT_ENOUGH;
        return 0;
    }
    else if((NULL != m_pBuffer) && (m_nBufferLen > 0))
    {
        //将m_pBuffer指向的缓冲区内容拷贝到pData指向的缓冲区中
        memcpy((BYTE*)pData, m_pBuffer, m_nBufferLen);
        return m_nBufferLen;
    }

    return 0;
}

/********************************************************
* 名称: CNetInfoFile::PutSyncData
* 功能: 写入状态数据操作
* 输入: pData : 输入缓冲区的指针
*       nLen  : 输入缓冲区的长度
* 输出:
* 返回: int 获得的实际写入的数据长度
********************************************************/
int CNetInfoFile::PutSyncData(void* pData, int nLen)
{
    errno = 0;

    //如果没有数据则直接退出
    if(nLen <= 0)
    {
        return 0;
    }

    //如果nLen>m_nBufferLen，则重新分配一个长度为nLen的缓冲区，
    //将其指针赋给m_pBuffer
    if(nLen > m_nBufferLen)
    {
        delete [] m_pBuffer;
        m_pBuffer = new char[nLen];
    }

    m_nBufferLen   = nLen;
    m_nSyncDataLen = nLen;

    //用pData指向的缓冲区内容来覆盖m_pBuffer指向的缓冲区
    memcpy(m_pBuffer, (BYTE*)pData, m_nBufferLen);

    return m_nBufferLen;
}

/********************************************************
* 名称: CNetInfoFile::Flush
* 功能: 将缓冲区中的数据写入状态文件，只有被同步方会调用
* 输入:
* 输出:
* 返回: int 写状态文件是否成功，0表示成功，非零值表示失败
********************************************************/
int CNetInfoFile::Flush()
{
    if(NULL == m_pFile)
    {
        //用fopen函数以二进制读写方式打开m_szFileName指向的文件
        //Opens for both reading and writing. (The file must exist.)
        m_pFile = fopen(m_szFullFileName, "rb+");
        if(NULL == m_pFile)
        {
            //创建并打开新的文件，将文件指针赋给m_pFile;
            //Opens an empty file for writing. If the given file exists,
            //its contents are destroyed.
            m_pFile = fopen(m_szFullFileName, "wb+");
            if(NULL == m_pFile)
            {
                return errno;
            }
        }
    }

    //将m_pBuffer缓冲区中的内容写入m_pFile指向的文件
    if(fseek(m_pFile, 0, SEEK_SET) != 0)
    {
        return ERR_FAIL;
    }

    //从文件中读出所有内容，置于m_pBuffer缓冲区内
    if((fwrite(m_pBuffer, m_nBufferLen, 1, m_pFile) != 1) ||
       (fflush(m_pFile) != 0))
    {
        return ERR_FAIL;
    }

    ACE_OS::truncate(m_szFullFileName, m_nBufferLen);

    return ERR_SUCCESS;
}