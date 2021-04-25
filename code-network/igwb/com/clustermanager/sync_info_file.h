#ifndef __SYNC_INFO_FILE_H__
#define __SYNC_INFO_FILE_H__

class CSyncInfoFile
{
public:
    CSyncInfoFile(const char* const szFilePath,
                  const char* const szFileName,
                  int nAPNum,
                  int nModule,
                  int nChl);
    virtual ~CSyncInfoFile();

    int GetAccessPoint();           //获得接入点号
    int GetModule();                //获得模块号
    int GetChannel();               //获得通道号
    int GetSyncDataLen();           //获得同步数据长度
    time_t GetLastWriteTime();      //取状态文件的最新修改时间

    virtual int Open() = 0;         //状态文件打开操作，只有被同步方会调用
    virtual void Close() = 0;       //状态文件关闭操作，只有被同步方会调用
    //获取同步数据操作
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE) = 0;
    //写入状态数据操作
    virtual int PutSyncData(void* pData, int nLen) = 0;
    //将缓冲区中的数据写入状态文件，只有被同步方会调用
    virtual int Flush() = 0;

protected:

    //缓冲区长度
    int      m_nBufferLen;

    //缓冲区指针
    char*    m_pBuffer;

    //状态文件所在的目录路径名
    char     m_szFilePath[MAX_PATH];

    //状态文件名
    char     m_szFileName[MAX_PATH];

    //状态文件全路径名
    char     m_szFullFileName[MAX_PATH];

    //同步数据长度
    int      m_nSyncDataLen;

    //接入点号
    int      m_nAccessPoint;

    //模块号
    int      m_nModule;

    //通道号
    int      m_nChannel;

};

#endif //__SYNC_INFO_FILE_H__
