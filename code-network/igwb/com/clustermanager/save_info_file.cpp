#include "../include/base_type.h"
#include "../include/msg_def.h"
#include "../frontsave/front_save.h"
#include "sync_struct.h"
#include "save_info_file.h"


/********************************************************
* 名称: CSaveInfoFile::CSaveInfoFile
* 功能: 构造函数
* 输入: const char* const szFilePath    : 主状态文件所在路径
*       const char* const szFileName    : 主状态文件名
*       const char* const szBakFilePath : 备状态文件所在路径
*       const char* const szBakFileName : 备状态文件名
*       int nAPNum                      : 接入点号
*       int nModule                     : 模块号
*       int nChl                        : 通道号
* 输出:
* 返回:
********************************************************/
CSaveInfoFile::CSaveInfoFile(const char* const szFilePath,
                             const char* const szFileName,
                             const char* const szBakFilePath,
                             const char* const szBakFileName,
                             int nAPNum,
                             int nModule,
                             int nChl)
                             :CSyncInfoFile(szFilePath,
                                            szFileName,
                                            nAPNum,
                                            nModule,
                                            nChl)
{
    strncpy(m_szBakFilePath, szBakFilePath, MAX_PATH);
    m_szBakFilePath[MAX_PATH - 1] = '\0';

    strncpy(m_szBakFileName, szBakFileName, MAX_PATH);
    m_szBakFileName[MAX_PATH - 1] = '\0';

    m_uHeadLen     = -1;
    m_uUnitLen     = -1;
    m_uUnitNum     = -1;
    m_nSyncDataLen = sizeof(SSaveSyncData);

    //设置主/备状态文件名和存放路径
    m_InfoFile.Init(m_szFilePath, m_szFileName, 
                    m_szBakFilePath, m_szBakFileName);
}

/********************************************************
* 名称: CSaveInfoFile::~CSaveInfoFile
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CSaveInfoFile::~CSaveInfoFile()
{
    //
}

/********************************************************
* 名称: CSaveInfoFile::SetFileStruct
* 功能: 设置状态文件头长
* 输入: pData    : 缓冲区指针
*       uHeadLen : 状态文件头长
*       uUnitLen : 单元长度
*       uUnitNum : 单元个数
* 输出:
* 返回: void
********************************************************/
void CSaveInfoFile::SetFileStruct(void* pData,
                                  const UINT4 uHeadLen,
                                  const UINT4 uUnitLen, 
                                  const UINT4 uUnitNum)
{
    assert(0    != uHeadLen);
    m_uHeadLen   = uHeadLen;
    m_uUnitLen   = uUnitLen;
    m_uUnitNum   = uUnitNum;

    assert(NULL == m_pBuffer);
    m_nBufferLen = uHeadLen;
    m_pBuffer    = (char*)pData;

    //调用函数，设置文件头长度，记录单元长度和记录单元个数
    m_InfoFile.SetFileStruct(m_uHeadLen, m_uUnitLen, m_uUnitNum);
}

/********************************************************
* 名称: CSaveInfoFile::Open
* 功能: 状态文件打开操作，只有被同步方会调用
* 输入:
* 输出:
* 返回: int 打开操作是否成功
********************************************************/
int CSaveInfoFile::Open()
{
    //打开状态文件，如果文件存在，则读取状态信息
    if(m_InfoFile.OpenInfoFile(FALSE))
    {
        //读出状态文件的头部信息
        assert(m_pBuffer != NULL);
        if(!m_InfoFile.ReadHead((BYTE*)m_pBuffer))
        {
            return ERR_FAIL;
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CSaveInfoFile::Close
* 功能: 状态文件关闭操作，只有被同步方会调用
* 输入:
* 输出:
* 返回: void
********************************************************/
void CSaveInfoFile::Close()
{
    //关闭状态文件
    m_InfoFile.Close();
}

/********************************************************
* 名称: CSaveInfoFile::GetSyncData
* 功能: 获取同步数据操作
* 输入: nData     : 输出缓冲区的指针
*       nLen      : 输出缓冲区的长度
*       bReadFile : 是否从文件中读出新的数据，TRUE表示要
*                   从文件中读出，FALSE表示从缓冲区中读出
* 输出:
* 返回: int 获得的实际读出的数据长度
********************************************************/
int CSaveInfoFile::GetSyncData(void* pData, int nLen, BOOL bReadFile)
{
    errno = 0;

    if (nLen < m_nSyncDataLen)
    {
        errno = ERR_BUFFER_NOT_ENOUGH;
        return 0;
    }

    if(bReadFile)
    {
        //打开状态文件;
        if (Open() != ERR_SUCCESS)
        {
            return 0;
        }
    }

    //将m_pBuffer强制转换为FS_STATUS_FILE_HEAD结构，将pData
    //强制转换为SSaveSyncData结构，并从m_pBuffer指向的缓冲区
    //中获取包号及文件序列号，填写到pData指向的缓冲区中
    ((SSaveSyncData*)pData)->uFileCsn = ((FS_STATUS_FILE_HEAD*)m_pBuffer)->uFileCsn;
    ((SSaveSyncData*)pData)->uPID     = ((FS_STATUS_FILE_HEAD*)m_pBuffer)->uLastPID;
    ((SSaveSyncData*)pData)->uBillCsn = 0;

    return m_nSyncDataLen;
}

/********************************************************
* 名称: CSaveInfoFile::PutSyncData
* 功能: 写入状态数据操作
* 输入: pData : 输入缓冲区的指针
*       nLen  : 输入缓冲区的长度
* 输出:
* 返回: int 获得的实际写入的数据长度
********************************************************/
int CSaveInfoFile::PutSyncData(void* pData, int nLen)
{
    errno = 0;

    //如果没有数据则直接退出
    if(nLen <= 0)
    {
        return 0;
    }

    //如果输入的数据长度小于需要的长度,则返回失败
    if(nLen < m_nSyncDataLen)
    {
        errno = ERR_BUFFER_NOT_ENOUGH;
        return 0;
    }

    ((FS_STATUS_FILE_HEAD*)m_pBuffer)->uFileCsn = 
        ((SSaveSyncData*)pData)->uFileCsn;

    return m_nSyncDataLen;
}

/********************************************************
* 名称: CSaveInfoFile::Flush
* 功能: 将缓冲区中的数据写入状态文件，只有被同步方会调用
* 输入:
* 输出:
* 返回: int 写状态文件是否成功，0表示成功，非零值表示失败
********************************************************/
int CSaveInfoFile::Flush()
{
    if(m_InfoFile.OpenInfoFile(TRUE, (BYTE*)m_pBuffer))
    {
        //将m_pBuffer的内容写入状态文件
        if((!m_InfoFile.WriteHead((BYTE*)m_pBuffer, m_nBufferLen)) ||
           (!m_InfoFile.Flush()))
        {
            return ERR_FAIL;
        }
    }

    return ERR_SUCCESS;
}