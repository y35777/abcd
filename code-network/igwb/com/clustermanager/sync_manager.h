#ifndef __SYNC_OBJECT_MANAGER_H__
#define __SYNC_OBJECT_MANAGER_H__

#include "sync_data.h"

class CSyncManager
{
public:
    CSyncManager();
    virtual ~CSyncManager();

    int InitInstance(); //初始化操作

    //状态文件打开操作，只是在被同步方准备接收同步数据时调用
    int Open();
    //状态文件关闭操作，只是在被同步方完成接收同步数据后调用
    void Close();

    //返回已封装成SMscAPSyncFrame结构的同步数据的长度
    int GetSyncDataLen();
    //获取同步数据操作
    int GetSyncData(void*& pData, int& nLen, BOOL bReadFile = TRUE);
    //写入状态数据操作
    int PutSyncData(void* pData, int nLen);
    //获取同步数据操作，只被同步方会调用
    void DoSyncData(void* pData, int nLen);            

protected:

    void ExitInstance();    //退出前的清理操作

    //接入点号到同步数据对象的MAP
    MAP<BYTE, CSyncData*>*     m_pSyncDataMap;
};

#endif //__SYNC_OBJECT_MANAGER_H__
