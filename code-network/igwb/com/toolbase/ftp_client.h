/******************************************************************************
 *��Ʒ��    ��iGateway Bill V200
 *ģ����    ��
 *�ļ���    ��ftp_client.h
 *������Ϣ  ����CFtpClient�Ķ����ļ��������װ��FTP�ͻ��˵ĳ��ù��ܣ����Կ�ƽ̨
              ʹ�á��ɲο�RFC 959��
 *�汾˵��  ��
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ��Qiu Yiping
 *����ʱ��  ��2002-05-25
 *�޸ļ�¼  ��
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

    //������Ӧ�������׽���
    int PrepareDataPort();

    int PrepareDataPortWithPort();
    int PrepareDataPortWithPasv();
    int GetAddrInPasvANS (sockaddr_in& SvrAddr);

    int AnalyzeAFileLine(char* pszLine, SFtpFile& AFile);
    int GetMonFromStr(const char* szMonth);

private:
    char  m_szHostAddr[FTP_HOST_NAME_LEN];  //FTP SERVER��������IP��ַ
    char  m_szUserName[FTP_USER_NAME_LEN];  //��½FTP SERVER�û���
    char  m_szPassword[FTP_PASSWORD_LEN];   //��½FTP SERVER��IP��ַ
    char  m_szLocalAddr[FTP_HOST_NAME_LEN]; //����FTP��������IP��ַ
    short m_nCtrlPort;                      //FTP���ƶ˿�
    int   m_nTransMode;                     //�ļ�����ģʽ(�ı��������)
    char  m_szLastResp[FTP_RESPONSE_BUF];   //FTP����������Ӧ������
    char* m_pRecvBuf;
    int   m_nRecvBufLen;

    struct sockaddr_in m_LstnAddr;          //FTP���������׽��ֵ�ַ

    int   m_hCtrlSocket;    //FTP����SOCKET
    int   m_hDataSocket;    //FTP����SOCKET
    int   m_hLstnSocket;    //FTP����SOCKET
	int   m_nPassive;
};

#endif   //_FTP_CLIENT_H_
