#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "../frontsave/front_save.h"
#include "sync_struct.h"
#include "sync_manager.h"
#include "sync_data_creator.h"


/********************************************************
* 名称: CSyncManager::CSyncManager
* 功能: 构造函数
* 输入:   
* 输出: 
* 返回: 
********************************************************/
CSyncManager::CSyncManager()
{
    m_pSyncDataMap = NULL;
}

/********************************************************
* 名称: CSyncManager::~CSyncManager
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CSyncManager::~CSyncManager()
{
    ExitInstance();
}

/********************************************************
* 名称: CSyncManager::InitInstance
* 功能: 初始化操作
* 输入: 
* 输出: 
* 返回: int 初始化操作是否成功，0表示成功、非零值表示失败
********************************************************/
int CSyncManager::InitInstance()
{
    m_pSyncDataMap = CSyncDataCreator::CreateSyncDataList();
    
    if((m_pSyncDataMap == NULL) && (ERR_INIT_SYNC_MGR == errno))
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CSyncManager::ExitInstance
* 功能: 退出前的清理操作
* 输入: 
* 输出: 
* 返回: void 
********************************************************/
void CSyncManager::ExitInstance()
{
    Close();

    if(NULL != m_pSyncDataMap)
    {
        MAP<BYTE, CSyncData*>::iterator i;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            delete (*i).second;
        }
        m_pSyncDataMap->clear();
        delete m_pSyncDataMap;
        m_pSyncDataMap = NULL;
    }
}

/********************************************************
* 名称: CSyncManager::Open
* 功能: 状态文件打开操作，只是在被同步方准备接收同步数据时调用
* 输入: 
* 输出: 
* 返回: int 打开操作是否成功，0表示成功，非零值表示失败
********************************************************/
int CSyncManager::Open()
{
    if(NULL != m_pSyncDataMap)
    {
        MAP<BYTE, CSyncData*>::iterator i;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            if((*i).second->Open() != ERR_SUCCESS)
            {
                Close();
                return ERR_FAIL;
            }
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CSyncManager::Close
* 功能: 状态文件关闭操作，只是在被同步方完成接收同步数据后调用
* 输入: 
* 输出: 
* 返回: void 
********************************************************/
void CSyncManager::Close()
{
    if(NULL != m_pSyncDataMap)
    {
        MAP<BYTE, CSyncData*>::iterator i;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            (*i).second->Close();
        }
    }
}

/********************************************************
* 名称: CSyncManager::GetSyncDataLen
* 功能: 返回已封装成SSyncFrame结构的同步数据的长度
* 输入: 
* 输出: 
* 返回: int 返回同步数据的长度
********************************************************/
int CSyncManager::GetSyncDataLen()
{
    int nLen = 0;
    if(NULL != m_pSyncDataMap)
    {
        MAP<BYTE, CSyncData*>::iterator i;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            if((*i).second->GetSyncDataLen() > 0)
            {
                nLen += (*i).second->GetSyncDataLen();
            }
        }
    }

    if(nLen > 0)
    {
        nLen += SYNC_FRAME_HEAD_LENGTH;
    }
    return nLen;
}

/********************************************************
* 名称: CSyncManager::GetSyncData
* 功能: 获取同步数据操作
* 输入: 
* 输出: pData     : 输出缓冲区的指针,封装成SSyncFrame
*                   结构的同步数据
*       nLen      : 输出缓冲区的长度
*       bReadFile : 是否从文件中读出
* 返回: int 获取同步数据操作是否成功,0表示成功,非零值表示失败
********************************************************/
int CSyncManager::GetSyncData(void*& pData, int& nLen, BOOL bReadFile)
{
    int nErrNo      = ERR_BUFFER_NOT_ENOUGH;
    char* pBuffer   = NULL;
    int nBufferLen  = 0;
    int nBufferRead = 0;
    MAP<BYTE, CSyncData*>::iterator i;
    int nTimes      = 0;

    pData = 0;
    nLen  = 0;
    if((NULL == m_pSyncDataMap) || (m_pSyncDataMap->size() == 0))
    {
        return ERR_SUCCESS;
    }

    //读两次是为了避免网络模块在倒换前最后一次同步后，又创建了新的
    //MSC主机协议栈，使同步数据长度改变，当执行Open函数，从文件中
    //重新读出时，GetSyncDataLen返回的数据将得到更新
    while((ERR_BUFFER_NOT_ENOUGH == nErrNo) && (nTimes++ < 2))
    {
        nErrNo      = 0;
        nBufferRead = SYNC_FRAME_HEAD_LENGTH;
        nBufferLen  = GetSyncDataLen();
        if (pBuffer != NULL)
        {
            delete [] pBuffer;
        }
        if(0 >= nBufferLen)
        {
            return ERR_SUCCESS;
        }
        pBuffer = new char[nBufferLen];
        int nRetLen = 0;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            nRetLen = (*i).second->GetSyncData(pBuffer + nBufferRead, 
                                               nBufferLen - nBufferRead, 
                                               bReadFile);
            if ((0 == nRetLen) && 
                (ERR_BUFFER_NOT_ENOUGH == errno))
            {
                nErrNo = ERR_BUFFER_NOT_ENOUGH;
                break;
            }
            nBufferRead += nRetLen;
        }
    }
    
    if(nTimes > 2)
    {
        if (pBuffer != NULL)
        {
            delete [] pBuffer;
        }
        return ERR_FAIL;
    }

    ((SSyncFrame*)pBuffer)->uLen = nBufferRead - SYNC_FRAME_HEAD_LENGTH;
    pData = pBuffer;
    nLen = nBufferRead;
    
    return ERR_SUCCESS;
}

/********************************************************
* 名称: CSyncManager::PutSyncData
* 功能: 写入状态数据操作
* 输入: pData : 输入缓冲区的指针,封装成SSyncFrame
*               结构的同步数据
*       nLen  : 输入缓冲区的长度
* 输出: 
* 返回: int 获得的实际写入的数据长度
********************************************************/
int CSyncManager::PutSyncData(void* pData, int nLen)
{
    SSyncFrame* pFrame          = (SSyncFrame*)pData;
    int nBufferWritten          = SYNC_FRAME_HEAD_LENGTH;
    SMscAPSyncFrame* pSyncFrame = NULL;

    if((NULL == m_pSyncDataMap) || (m_pSyncDataMap->size() == 0))
    {
        return 0;
    }

    while(nBufferWritten < int(nLen - MSC_AP_SYNC_FRAME_HEAD_LENGTH))
    {
        pSyncFrame = (SMscAPSyncFrame*) ((char*)pFrame + nBufferWritten);
        if((m_pSyncDataMap->find(pSyncFrame->uAPointID) 
            != m_pSyncDataMap->end()) && (pSyncFrame->uLen > 0))
        {
            (*m_pSyncDataMap)[pSyncFrame->uAPointID]->PutSyncData(
                                                      (void*)pSyncFrame, 
                                                      pSyncFrame->uLen +
                                                      MSC_AP_SYNC_FRAME_HEAD_LENGTH);
        }
        nBufferWritten += MSC_AP_SYNC_FRAME_HEAD_LENGTH + pSyncFrame->uLen;
    }

    if(SYNC_FRAME_HEAD_LENGTH == nBufferWritten)
    {
        nBufferWritten = 0;
    }
    return nBufferWritten;
}

/********************************************************
* 名称: CSyncManager::DoSyncData
* 功能: 实施数据同步操作，只被同步方会调用
* 输入: pData     : 输入缓冲区的指针 ，封装成SMscAPSyncFrame
*                   结构的同步数据
*       nLen      : 输入缓冲区的长度
* 输出: 
* 返回: void 
********************************************************/
void CSyncManager::DoSyncData(void* pData, int nLen)
{
    if((NULL == m_pSyncDataMap) || (m_pSyncDataMap->size() == 0))
    {
        return;
    }
 
    SSyncFrame* pFrame          = (SSyncFrame*)pData;
    int nBufferWritten          = SYNC_FRAME_HEAD_LENGTH;
    SMscAPSyncFrame* pSyncFrame = NULL;
    while(nBufferWritten <= int(nLen - MSC_AP_SYNC_FRAME_HEAD_LENGTH))
    {
        pSyncFrame = (SMscAPSyncFrame*) ((char*)pFrame + nBufferWritten);
        if((m_pSyncDataMap->find(pSyncFrame->uAPointID) 
            != m_pSyncDataMap->end()) && (pSyncFrame->uLen > 0))
        {
            (*m_pSyncDataMap)[pSyncFrame->uAPointID]->DoSyncData(
                                                      (void*)pSyncFrame, 
                                                      pSyncFrame->uLen +
                                                      MSC_AP_SYNC_FRAME_HEAD_LENGTH);
        }
        nBufferWritten += MSC_AP_SYNC_FRAME_HEAD_LENGTH + pSyncFrame->uLen;
    }   
}

