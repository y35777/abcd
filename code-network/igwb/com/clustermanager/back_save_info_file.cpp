#include "../include/base_type.h"
#include "../include/msg_def.h"
#include "../backsave/save_chanl.h"
#include "sync_struct.h"
#include "back_save_info_file.h"


/********************************************************
* 名称: CBackSaveInfoFile::CBackSaveInfoFile
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
CBackSaveInfoFile::CBackSaveInfoFile(const char* const szFilePath,
                                     const char* const szFileName,
                                     const char* const szBakFilePath,
                                     const char* const szBakFileName,
                                     int nAPNum,
                                     int nModule,
                                     int nChl)
                                     :CSaveInfoFile(szFilePath,
                                                    szFileName,
                                                    szBakFilePath,
                                                    szBakFileName,
                                                    nAPNum,
                                                    nModule,
                                                    nChl)
{
    //
}

/********************************************************
* 名称: CBackSaveInfoFile::~CBackSaveInfoFile
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CBackSaveInfoFile::~CBackSaveInfoFile()
{
    //
}

/********************************************************
* 名称: CBackSaveInfoFile::GetSyncData
* 功能: 获取同步数据操作
* 输入: nData     : 输出缓冲区的指针
*       nLen      : 输出缓冲区的长度
*       bReadFile : 是否从文件中读出新的数据，TRUE表示要
*                   从文件中读出，FALSE表示从缓冲区中读出
* 输出:
* 返回: int 获得的实际读出的数据长度
********************************************************/
int CBackSaveInfoFile::GetSyncData(void* pData, int nLen, BOOL bReadFile)
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

    //将m_pBuffer强制转换为BS_STATUS_FILE_HEAD结构，将pData
    //强制转换为SSaveSyncData结构，并从m_pBuffer指向的缓冲区
    //中获取包号、文件序列号及话单序列号，填写到pData指向的缓冲区中
    ((SSaveSyncData*)pData)->uFileCsn = ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uFileCsn;
    ((SSaveSyncData*)pData)->uPID     = ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uLastPID;
    ((SSaveSyncData*)pData)->uBillCsn = ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uBillCsn;

    return m_nSyncDataLen;
}

/********************************************************
* 名称: CBackSaveInfoFile::PutSyncData
* 功能: 写入状态数据操作
* 输入: pData : 输入缓冲区的指针
*       nLen  : 输入缓冲区的长度
* 输出:
* 返回: int 获得的实际写入的数据长度
********************************************************/
int CBackSaveInfoFile::PutSyncData(void* pData, int nLen)
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

    ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uFileCsn = 
        ((SSaveSyncData*)pData)->uFileCsn;
    ((BS_STATUS_FILE_HEAD*)m_pBuffer)->uBillCsn = 
        ((SSaveSyncData*)pData)->uBillCsn;

    return m_nSyncDataLen;
}
