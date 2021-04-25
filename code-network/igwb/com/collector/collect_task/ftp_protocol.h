#ifndef __FTP_PROTOCOL_H__
#define __FTP_PROTOCOL_H__

#include "file_protocol.h"
#include "../../toolbase/ftp_client.h"

//以下是文件传输基类的定义
class  CFtpProtocol : public CFileProtocol
{

public:

    CFtpProtocol();
    virtual  ~CFtpProtocol();

    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual  int  InitProtocol();

    //实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual  int  GetFile(const SGetFileReq &pReq);

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual  int  GetFileList( const  char* szSrcDir, SListOut &pOut );

    //删除文件服务端的指定文件
    virtual  int  DelFile( const char* szFileName );

    //重命名指定文件
    virtual  int  RenameFile( const char *szOld, const char *szNew );

    //连接文件服务端
    virtual  int  Connect();

    //断开连接
    virtual  int  Disconnect();

    //设置文件传输模式
    virtual  int  SetMode(int nMode);

    //测试能否正常连接对端文件服务器, 虚函数, 由派生类具体实现
    virtual  int  TryConnect();

protected:
    CFtpClient    m_FtpClient;
    char          m_szRemotePath[MAX_PATH];
    int           m_nMode;
    ACE_Mutex     m_Mutex;
    BOOL          m_bConnected;
};

#endif //__FTP_PROTOCOL_H__
