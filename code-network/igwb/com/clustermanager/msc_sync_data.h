#ifndef __MSC_SYNC_OBJECT_H__
#define __MSC_SYNC_OBJECT_H__

#include "sync_struct.h"
#include "sync_data.h"

class CMscSyncData : public CSyncData
{
public:
    CMscSyncData(int nAccessPoint);
    virtual ~CMscSyncData();

    //初始化操作
    virtual int InitInstance();

    //返回已封装成SMscAPSyncFrame结构的同步数据的长度
    virtual int GetSyncDataLen();
    //获取同步数据操作
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE);
    //写入状态数据操作
    virtual int PutSyncData(void* pData, int nLen);
    //实施数据同步操作，只被同步方会调用
    virtual void DoSyncData(void* pData, int nLen);
    //获取上次写状态文件的时间
    time_t GetLastWriteTime();

protected:

    //查找指定模块号及通道号的同步数据块指针
    SMscSyncData* FindSyncData(SMscAPSyncFrame* pAPFrame, 
                               UINT4 uMID, UINT4 uChannel);

    //前存盘的包号
    UINT4   m_uFrontSavePID;

    //前存盘的文件序列号
    UINT4   m_uFrontFileCsn;

    //网络模块的包号
    UINT4   m_uNetworkPID;
};

#endif //__MSC_SYNC_OBJECT_H__
