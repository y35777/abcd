#include "../../include/base_type.h"
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "file_protocol.h"
#include <assert.h>

//以下是文件传输基类CFileProtocol的实现

// 构造函数
CFileProtocol::CFileProtocol()
{
    //成员变量初始化
    m_szSvrName[0]   = '\0';
    m_szSvrUsr[0]    = '\0';
    m_szSvrPwd[0]    = '\0';

    m_szLocalName[0] = '\0';
    m_szLocalUsr[0]  = '\0';
    m_szLocalPwd[0]  = '\0';

    m_szErrInfo[0]   = '\0';
    m_nStatus        = -1;
}

//析构函数
CFileProtocol::~CFileProtocol()
{
    //NULL
}


//初始化本地文件协议服务, 是虚函数, 由派生类具体实现该方法
int CFileProtocol::InitProtocol()
{
    return  ERR_SUCCESS;
}

//测试能否正常连接对端文件服务器, 虚函数, 由派生类具体实现
int CFileProtocol::TryConnect()
{
    return  m_nStatus;
}

//实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
int CFileProtocol::GetFile(const SGetFileReq &pReq)
{
    return  ERR_SUCCESS;
}

//获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
int CFileProtocol::GetFileList(const  char* szSrcDir, SListOut &pOut)
{
    return  ERR_SUCCESS;
}

//删除文件服务端的指定文件
int CFileProtocol::DelFile(const char* szFileName)
{
    return  ERR_SUCCESS;
}

//取文件服务端的指定文件的长度
int CFileProtocol::GetFileLen(const char* szFileName,
                              unsigned long& uFileLen)
{
    return  0;
}

//重命名指定文件
int CFileProtocol::RenameFile( const char *szOld, const char *szNew )
{
    return  ERR_SUCCESS;
}

//连接文件服务端
int CFileProtocol::Connect()
{
    return  ERR_SUCCESS;
}

//断开连接
int CFileProtocol::Disconnect()
{
    return  ERR_SUCCESS;
}

//设置文件传输模式
int CFileProtocol::SetMode(int nMode)
{
    return  ERR_SUCCESS;
}


/********************************************************************
Function:        char  *  GetLastErr();
Description:     得到最后一次文件传输操作的错误信息
Calls:           无
Called By:       类的使用者
Table Accessed:  无
Table Updated:   无
Input:           无
Output:          无
Return:          返回字符串指针，包含最后一次文件传输操作的错误信息
Others:          无
*********************************************************************/
const char* CFileProtocol::GetLastErr()
{
    //返回最后一次文件传输操作的错误信息
    return  m_szErrInfo;
}


//设置远程主机信息
void CFileProtocol::SetRemoteHostMsg(const char* const szHost,
                                     const char* const szUser,
                                     const char* const szPwd)
{
    if((NULL != szHost) && (szHost[0] != '\0'))
    {
        strncpy(m_szSvrName, szHost, sizeof(m_szSvrName));
        m_szSvrName[sizeof(m_szSvrName) - 1] = '\0';
    }

    if((NULL != szUser) && (szUser[0] != '\0'))
    {
        strncpy(m_szSvrUsr, szUser, sizeof(m_szSvrUsr));
        m_szSvrUsr[sizeof(m_szSvrUsr) - 1] = '\0';
    }

    if((NULL != szPwd) && (szPwd[0] != '\0'))
    {
        strncpy(m_szSvrPwd, szPwd, sizeof(m_szSvrPwd));
        m_szSvrPwd[sizeof(m_szSvrPwd) - 1] = '\0';
    }

}


//设置本地主机信息
void CFileProtocol::SetLocalHostMsg(const char* const szHost,
                                    const char* const szUser,
                                    const char* const szPwd)
{
    if((NULL != szHost) && (szHost[0] != '\0'))
    {
        strncpy(m_szLocalName, szHost, sizeof(m_szLocalName));
        m_szLocalName[sizeof(m_szLocalName) - 1] = '\0';
    }

    if((NULL != szUser) && (szUser[0] != '\0'))
    {
        strncpy(m_szLocalUsr, szUser, sizeof(m_szLocalUsr));
        m_szLocalUsr[sizeof(m_szLocalUsr) - 1] = '\0';
    }

    if((NULL != szPwd) && (szPwd[0] != '\0'))
    {
        strncpy(m_szLocalPwd, szPwd, sizeof(m_szLocalPwd));
        m_szLocalPwd[sizeof(m_szLocalPwd) - 1] = '\0';
    }

}
