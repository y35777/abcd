#ifndef __SAVE_SYNC_INFO_FILE_H__
#define __SAVE_SYNC_INFO_FILE_H__

#include "sync_info_file.h"
#include "double_info_file.h"

#define FRONTSAVE_CHANNEL_ID  255

class CSaveInfoFile : public CSyncInfoFile
{
public:
    CSaveInfoFile(const char* const szFilePath,
                  const char* const szFileName,
                  const char* const szBakFilePath,
                  const char* const szBakFileName,
                  int nAPNum,
                  int nModule,
                  int nChl);
    virtual ~CSaveInfoFile();

    //设置文件头长度，记录单元长度和记录单元个数及缓冲区
    void SetFileStruct(void* pData,
                       const UINT4 uHeadLen, 
                       const UINT4 uUnitLen, 
                       const UINT4 uUnitNum);
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

    //备状态文件所在的目录路径名
    char                m_szBakFilePath[MAX_PATH];

    //备状态文件名
    char                m_szBakFileName[MAX_PATH];

    //文件头长度
    UINT4               m_uHeadLen;

    //记录单元长度
    UINT4               m_uUnitLen;

    //记录单元个数
    UINT4               m_uUnitNum;

    //前后存盘状态文件读写对象
    CDoubleInfoFile     m_InfoFile;
};

#endif //__SAVE_SYNC_INFO_FILE_H__
