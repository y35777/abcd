#ifndef __SYNC_OBJECT_H__
#define __SYNC_OBJECT_H__

#include "sync_info_file.h"

class CSyncData
{
public:
    CSyncData(int nAccessPoint);
    virtual ~CSyncData();

    //初始化操作
    virtual int InitInstance() = 0;

    //状态文件打开操作，只是在被同步方准备接收同步数据时调用
    int Open();
    //状态文件关闭操作，只是在被同步方完成接收同步数据后调用
    void Close();

    //返回已封装成SMscAPSyncFrame结构的同步数据的长度
    virtual int GetSyncDataLen();
    //获取同步数据操作
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE) = 0;
    //写入状态数据操作
    virtual int PutSyncData(void* pData, int nLen) = 0;
    //获取同步数据操作，只被同步方会调用
    virtual void DoSyncData(void* pData, int nLen) = 0;

protected:

    //同步状态文件读写对象指针的队列
    LIST<CSyncInfoFile*>    m_SyncInfoFileList;

    //本接入点同步数据结构的长度
    int                     m_nBufferLen;

    //接入点号
    int                     m_nAccessPoint;

    //上次激活的时间
    time_t                  m_tLastActiveTime;
};

#endif //__SYNC_OBJECT_H__
