#ifndef __LINK_BASE_H__
#define __LINK_BASE_H__

#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "file_protocol.h"
#include <assert.h>

class CLinkBase
{
public:
    CLinkBase(CFileProtocol*    fp,
              const char* const szHost,
              const char* const szUser,
              const char* const szPwd,
              int nAlarmPara)
    {
        assert((fp != NULL) && (szHost != NULL) && (szUser != NULL) && (szPwd != NULL));

        m_fp = fp;

        SetRemoteHostMsg(szHost, szUser, szPwd);

        m_nAlarmPara = nAlarmPara;

        m_szLocalUser[0] = '\0';           
        m_szLocalPwd[0] = '\0';            
        m_szLocalHostName[0] = '\0';
    }

    virtual ~CLinkBase()
    {
        if(m_fp != NULL)
        {
            delete m_fp;
            m_fp = NULL;
        }
    }

    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual int  Init()
    {
        return m_fp->InitProtocol();
    }

    //连接文件服务端
    virtual int  Connect()
    {
        return m_fp->Connect();
    }

    //断开连接
    virtual int  Disconnect()
    {
        return m_fp->Disconnect();
    }

    //设置文件传输模式
    virtual int  SetMode(int nMode)
    {
        return m_fp->SetMode(nMode);
    }

    //测试能否正常连接对端文件服务器, 虚函数, 由派生类具体实现
    virtual int  TestLink()
    {
        int nRet = ERR_FAIL;
        if(m_fp != NULL)
        {
            nRet = m_fp->TryConnect();
        }

        return nRet;
    }

    //实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual int  GetFile(const SGetFileReq &pReq)
    {
        return m_fp->GetFile(pReq);
    }

    //取文件服务端的指定文件的长度
    virtual int  GetFileLen(const char* szFileName,
                            unsigned long& uFileLen)
    {
        return m_fp->GetFileLen(szFileName, uFileLen);
    }

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual int  GetFileList(const char* const szSrcDir, SListOut &pOut)
    {
        return m_fp->GetFileList(szSrcDir, pOut);
    }

    //删除文件服务端的指定文件
    virtual int  DelFile(const char* const szFileName)
    {
        return m_fp->DelFile(szFileName);
    }

    //重命名指定文件
    virtual int  RenameFile(const char *szOld, const char *szNew)
    {
        return m_fp->RenameFile(szOld, szNew);
    }

    //得到最后一次文件传输操作的错误信息
    const char*  GetLastErr()
    {
        return m_szErrInfo;
    }

    BOOL GetbFailure()
    {
        return m_bFailure;
    }

    int GetAlarmPara()
    {
        return m_nAlarmPara;
    }

    const char* GetLocalHost()
    {
        return m_szLocalHostName;
    }

    const char* GetRemoteHost()
    {
        return m_szHostName;
    }
    

    //设置远程主机信息
    void SetRemoteHostMsg(const char* const szHost,
                          const char* const szUser,
                          const char* const szPwd)
    {
        strncpy(m_szHostName, szHost, sizeof(m_szHostName));
        m_szHostName[sizeof(m_szHostName) - 1] = '\0';

        strncpy(m_szUser, szUser, sizeof(m_szUser));
        m_szUser[sizeof(m_szUser) - 1] = '\0';

        strncpy(m_szPwd, szPwd, sizeof(m_szPwd));
        m_szPwd[sizeof(m_szPwd) - 1] = '\0';

        m_fp->SetRemoteHostMsg(szHost,szUser,szPwd);
    }

    //设置本地主机信息
    void SetLocalHostMsg(const char* const szHost,
                         const char* const szUser,
                         const char* const szPwd)
    {
        strncpy(m_szLocalHostName, szHost, sizeof(m_szLocalHostName));
        m_szHostName[sizeof(m_szLocalHostName) - 1] = '\0';

        strncpy(m_szLocalUser, szUser, sizeof(m_szLocalUser));
        m_szUser[sizeof(m_szLocalUser) - 1] = '\0';

        strncpy(m_szLocalPwd, szPwd, sizeof(m_szLocalPwd));
        m_szPwd[sizeof(m_szLocalPwd) - 1] = '\0';

        m_fp->SetLocalHostMsg(szHost,szUser,szPwd);
    }

protected:

    BOOL  m_bFailure;                   //是否故障
    CFileProtocol*  m_fp;
    char  m_szErrInfo[MAX_ERROR_LEN];   //保存最后一次文件传输操作的错误信息

    char  m_szLocalHostName[50];        //保存本地名字
    char  m_szLocalUser[50];            //本地用户名
    char  m_szLocalPwd[50];             //本地用户密码

    char  m_szHostName[50];             //保存远端文件服务器的名字
    char  m_szUser[50];                 //远端文件服务器的用户名
    char  m_szPwd[50];                  //远端文件服务器的用户密码

    int   m_nAlarmPara;                 //告警参数码
};

#endif //__LINK_BASE_H__
