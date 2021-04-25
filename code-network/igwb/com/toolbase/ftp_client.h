/******************************************************************************
 *产品名    ：iGateway Bill V200
 *模块名    ：
 *文件名    ：ftp_client.h
 *描述信息  ：类CFtpClient的定义文件，该类封装了FTP客户端的常用功能，可以跨平台
              使用。可参考RFC 959。
 *版本说明  ：
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：Qiu Yiping
 *创建时间  ：2002-05-25
 *修改记录  ：
 ******************************************************************************/
#ifndef _FTP_CLIENT_H_
#define _FTP_CLIENT_H_

#include "ftp_command.h"
#include "../include/base_type.h"

class IGWB_Svc_Export CFtpClient
{
public:
    CFtpClient(void);
    ~CFtpClient(void);

public:
    int  Connect(const char* pszHost,
                 const char* pszUser,
                 const char* pszPass,
                 const char* pszLocalAddr = "127.0.0.1",
                 int nTransMode  = FTP_TRANS_BINARY,
                 short nCtrlPort = FTP_CTRL_PORT);

    void Disconnect();

    int  GetDirectory(char* pszDirectory);
    int  SetDirectory(const char* pszDirectory);
    int  SetTransMode(const int nMode);

    int  PutFile(const char* pszLocalFile,
                 const char* pszRemoteFile = NULL);

    int  GetFile(const char* pszLocalFile,
                 const char* pszRemoteFile = NULL);

    int  DeleteFile(const char* pszFile);
    int  RenameFile(const char* pszOrigName, const char* pszNewName);

    int  MakeDirectory(const char* pszDirectory);
    int  RemoveDirectory(const char* pszDirectory);
    int  ListDirectory(const char* pszDirectory, FTP_FILELIST& FileList);

    void SetPassive(int nMode) {m_nPassive = nMode;};


private:
    int  Command(const char *fmt, ...);
    int  GetLastResp(char *pTmp = NULL);
    int  Login();
    int  NotifyDataPort();
    int  Accept(void);

    //创建对应的数据套接字
    int PrepareDataPort();

    int PrepareDataPortWithPort();
    int PrepareDataPortWithPasv();
    int GetAddrInPasvANS (sockaddr_in& SvrAddr);

    int AnalyzeAFileLine(char* pszLine, SFtpFile& AFile);
    int GetMonFromStr(const char* szMonth);

private:
    char  m_szHostAddr[FTP_HOST_NAME_LEN];  //FTP SERVER主机名或IP地址
    char  m_szUserName[FTP_USER_NAME_LEN];  //登陆FTP SERVER用户名
    char  m_szPassword[FTP_PASSWORD_LEN];   //登陆FTP SERVER的IP地址
    char  m_szLocalAddr[FTP_HOST_NAME_LEN]; //本地FTP数据侦听IP地址
    short m_nCtrlPort;                      //FTP控制端口
    int   m_nTransMode;                     //文件传输模式(文本或二进制)
    char  m_szLastResp[FTP_RESPONSE_BUF];   //FTP控制命令响应缓冲区
    char* m_pRecvBuf;
    int   m_nRecvBufLen;

    struct sockaddr_in m_LstnAddr;          //FTP数据侦听套接字地址

    int   m_hCtrlSocket;    //FTP命令SOCKET
    int   m_hDataSocket;    //FTP数据SOCKET
    int   m_hLstnSocket;    //FTP侦听SOCKET
	int   m_nPassive;
};

#endif   //_FTP_CLIENT_H_
