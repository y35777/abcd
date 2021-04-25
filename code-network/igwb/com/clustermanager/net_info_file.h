#ifndef __NET_SYNC_INFO_FILE_H__
#define __NET_SYNC_INFO_FILE_H__

#include "sync_info_file.h"

#define NETWORK_CHANNEL_ID  254

class CNetInfoFile : public CSyncInfoFile
{
public:
    CNetInfoFile(const char* const szFilePath,
                 const char* const szFileName,
                 int nAPNum,
                 int nModule,
                 int nChl);
    virtual ~CNetInfoFile();

    //状态文件打开操作，只有被同步方会调用
    virtual int Open();
    //状态文件关闭操作，只有被同步方会调用
    virtual void Close();
    //获取同步数据操作
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE);
    //写入状态数据操作
    virtual int PutSyncData(void* pData, int nLen);
    //将缓冲区中的数据写入状态文件，只有被同步方会调用
    virtual int Flush();

protected:

    //状态文件的FILE指针
    FILE*    m_pFile;
};

#endif //__NET_SYNC_INFO_FILE_H__
