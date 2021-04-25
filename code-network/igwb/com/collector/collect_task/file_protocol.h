#ifndef __FILE_PROTOCOL_H__
#define __FILE_PROTOCOL_H__

#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"

//常量定义
static  const int  ATTR_FILE = 0;  //文件属性：代表文件
static  const int  ATTR_DIR = 10;  //文件属性：代表目录

//以下是文件传输基类的定义
class  CFileProtocol
{

public:

    CFileProtocol();
    virtual  ~CFileProtocol();

    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual  int  InitProtocol();

    //测试能否正常连接对端文件服务器, 虚函数, 由派生类具体实现
    virtual  int  TryConnect();

    //实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual  int  GetFile(const SGetFileReq &pReq);

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual  int  GetFileList( const  char* szSrcDir, SListOut &pOut );

    //删除文件服务端的指定文件
    virtual  int  DelFile( const char* szFileName );

    //取文件服务端的指定文件的长度
    virtual  int  GetFileLen(const char* szFileName,
                             unsigned long& uFileLen);

    //重命名指定文件
    virtual  int  RenameFile( const char *szOld, const char *szNew );

    //连接文件服务端
    virtual  int  Connect();

    //断开连接
    virtual  int  Disconnect();

    //设置文件传输模式
    virtual  int  SetMode(int nMode);

    //得到最后一次文件传输操作的错误信息
    const char*   GetLastErr();

    //设置远程主机信息
    void SetRemoteHostMsg(const char* const szHost,
                          const char* const szUser,
                          const char* const szPwd);

    //设置本地主机信息
    void SetLocalHostMsg(const char* const szHost,
                         const char* const szUser,
                         const char* const szPwd);

public:
    char  m_szErrInfo[MAX_ERROR_LEN]; //保存最后一次文件传输操作的错误信息


protected:
    char  m_szSvrName[50];      //保存远端文件服务器的名字
    char  m_szSvrUsr[50];       //远端文件服务器的用户名
    char  m_szSvrPwd[50];       //远端文件服务器的用户密码

    char  m_szLocalName[50];    //保存本地文件服务器的名字
    char  m_szLocalUsr[50];     //本地文件服务器的用户名
    char  m_szLocalPwd[50];     //本地文件服务器的用户密码

    int   m_nStatus;            //状态，0表示正常
};

#endif //__FILE_PROTOCOL_H__
