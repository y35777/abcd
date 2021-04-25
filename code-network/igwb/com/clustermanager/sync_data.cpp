#include "../include/base_type.h"
#include "sync_data.h"



/********************************************************
* 名称: CSyncData::CSyncData
* 功能: 构造函数
* 输入: nAccessPoint : 接入点号
* 输出:
* 返回:
********************************************************/
CSyncData::CSyncData(int nAccessPoint)
{
    m_nBufferLen        = 0;
    m_nAccessPoint      = nAccessPoint;
    m_tLastActiveTime   = 0;
}

/********************************************************
* 名称: CSyncData::~CSyncData
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CSyncData::~CSyncData()
{
    while(m_SyncInfoFileList.size() > 0)
    {
        delete m_SyncInfoFileList.back();
        m_SyncInfoFileList.pop_back();
    }
}

/********************************************************
* 名称: CSyncData::Open
* 功能: 状态文件打开操作，只是在被同步方准备接收同步数据时调用
* 输入:
* 输出:
* 返回: int 打开操作是否成功，0表示成功、非零值表示失败
********************************************************/
int CSyncData::Open()
{
    LIST<CSyncInfoFile*>::iterator i;

    for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
    {
        if((*i)->Open() != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CSyncData::Close
* 功能: 状态文件关闭操作，只是在被同步方完成接收同步数据后调用
* 输入:
* 输出:
* 返回: void
********************************************************/
void CSyncData::Close()
{
    LIST<CSyncInfoFile*>::iterator i;

    for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
    {
        (*i)->Close();
    }
}

/********************************************************
* 名称: CSyncData::GetSyncDataLen
* 功能: 返回已封装成SMscAPSyncFrame结构的同步数据的长度
* 输入:
* 输出:
* 返回: int 返回同步数据的长度
********************************************************/
int CSyncData::GetSyncDataLen()
{
    return 0;
}

