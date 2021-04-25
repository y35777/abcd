#ifndef __VERTEL_FTAM_FILE_PROTOCOL_H__
#define __VERTEL_FTAM_FILE_PROTOCOL_H__

#include "file_protocol.h"
#include <assert.h>

//以下是文件传输基类的定义
class CVertelFtam : public CFileProtocol
{
public:

    CVertelFtam(int nTransSpeed, const char* szDevName);
    virtual  ~CVertelFtam();

    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual  int  InitProtocol();

    //测试能否正常连接对端文件服务器, 虚函数, 由派生类具体实现
    virtual  int  TryConnect();

    //实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual int   GetFile(const SGetFileReq &pReq);

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual int   GetFileList(const  char* szSrcDir, SListOut &pOut);

    //删除文件服务端的指定文件
    virtual  int  DelFile(const char* szFileName );

    //取文件服务端的指定文件的长度
    virtual  int  GetFileLen(const char* szFileName,
                             unsigned long& uFileLen);

    //重命名指定文件
    virtual  int  RenameFile(const char *szOld, const char *szNew);

    //连接文件服务端
    virtual  int  Connect()
    {
        return ERR_SUCCESS;
    }

    //断开连接
    virtual  int  Disconnect()
    {
        return ERR_SUCCESS;
    }

protected:
    static ACE_Recursive_Thread_Mutex g_VertelMutex;
    int                               m_nTransSpeed;
    int                               m_bOperate;
    LIST<int>                         m_LastStatusList;
    char                              m_szDevName[50];
};

#endif //__VERTEL_FTAM_FILE_PROTOCOL_H__
