/******************************************************************************
 *��Ʒ��    ��iGateway Bill V200
 *ģ����    ��
 *�ļ���    ��ftp_client.cpp
 *������Ϣ  ����CFtpClient��ʵ���ļ��������װ��FTP�ͻ��˵ĳ��ù��ܡ�
              ������FTP SERVER��ͨ�ţ�ͳһʹ��ԭʼBerkeley�׽ӿڱ�̣�
              ����ģʽ��
 *�汾˵��  ��
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ��Qiu Yiping
 *����ʱ��  ��2002-05-25
 *�޸ļ�¼  ��
 ******************************************************************************/
#include "../include/base_type.h"
#include "../include/export_dll.h"
#include "ftp_client.h"

CFtpClient::CFtpClient()
{
    m_szHostAddr[0]  = '\0';
    m_szUserName[0]  = '\0';
    m_szPassword[0]  = '\0';
    m_szLocalAddr[0] = '\0';
    m_szLastResp[0]  = '\0';

    m_nCtrlPort  = FTP_CTRL_PORT;
    m_nTransMode = FTP_TRANS_BINARY;

    m_hCtrlSocket = -1;    //FTP�����׽���
    m_hDataSocket = -1;    //FTP�����׽���
    m_hLstnSocket = -1;    //FTP�����׽���

    m_nRecvBufLen = FTP_RESPONSE_BUF;
    m_pRecvBuf    = new char[m_nRecvBufLen];
	m_nPassive = 0;
}

CFtpClient::~CFtpClient()
{
    if (-1 != m_hCtrlSocket)
    {
        CLOSE_SOCKET(m_hCtrlSocket);
    }

    if (-1 != m_hDataSocket)
    {
        CLOSE_SOCKET(m_hDataSocket);
    }

    if (-1 != m_hLstnSocket)
    {
        CLOSE_SOCKET(m_hLstnSocket);
    }

    if (NULL != m_pRecvBuf)
    {
        delete [] m_pRecvBuf;
        m_pRecvBuf = NULL;
    }
}

/**************************************************************************
����ԭ��    public: int Connect(char* pszHost, char* pszUser, char* pszPass,
                                char* pszLocalAddr, int nTransMode,
                                short nCtrlPort)

��������    ���ӡ���½FTP SERVER��
�������
       pszHost        : FTP SERVER��ַ
       pszUser        : ��½�û���
       pszPass        : ��½����
       pszLocalAddr   : ���ص�ַ
       nCtrlPort      : FTP��������˿�(Ĭ��Ϊ21)
       nTransMode     : �ļ�����ģʽ

�������    ��
����ֵ
       0 �� ���ӡ���½�ɹ�
     ��0 �� ���ӻ��½ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::Connect(const char* pszHost, 
                        const char* pszUser, 
                        const char* pszPass,
                        const char* pszLocalAddr,
                        int nTransMode,
                        short nCtrlPort)
{
    Disconnect();

    if (pszHost)
    {
        strncpy(m_szHostAddr, pszHost, FTP_HOST_NAME_LEN);
        m_szHostAddr[FTP_HOST_NAME_LEN - 1] = '\0';
    }

    if (pszUser)
    {
        strncpy(m_szUserName, pszUser, FTP_USER_NAME_LEN);
        m_szUserName[FTP_USER_NAME_LEN - 1] = '\0';
    }

    if (pszPass)
    {
        strncpy(m_szPassword, pszPass, FTP_PASSWORD_LEN);
        m_szPassword[FTP_PASSWORD_LEN - 1] = '\0';
    }

    if (pszLocalAddr)
    {
        strncpy(m_szLocalAddr, pszLocalAddr, FTP_HOST_NAME_LEN);
        m_szLocalAddr[FTP_HOST_NAME_LEN - 1] = '\0';
    }

    m_nTransMode = nTransMode;
    m_nCtrlPort  = nCtrlPort;

    m_hCtrlSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_hCtrlSocket < 0)
    {
        return -1;
    }

    if(m_szLocalAddr[0] != '\0')
    {
        struct sockaddr_in LocalAddr;
        memset(&LocalAddr, 0, sizeof(LocalAddr));
        LocalAddr.sin_family      = AF_INET;
        LocalAddr.sin_port        = 0;
        LocalAddr.sin_addr.s_addr = inet_addr(m_szLocalAddr);

        if (bind(m_hCtrlSocket, (sockaddr *)&LocalAddr, sizeof(LocalAddr)) < 0)
        {
            //TRACE("DEBUG", "bind error");
            return -1;
        }
    }
    
    struct sockaddr_in SvrAddr;
    memset(&SvrAddr, 0, sizeof(SvrAddr));
    SvrAddr.sin_family      = AF_INET;
    SvrAddr.sin_port        = htons(m_nCtrlPort);
    SvrAddr.sin_addr.s_addr = inet_addr(m_szHostAddr);

    if (connect(m_hCtrlSocket, (sockaddr *)&SvrAddr, sizeof(SvrAddr)) < 0)
    {
        //TRACE("DEBUG", "connect error");
        return -1;
    }

    //��ɹ�����FTP SERVER��FTP SERVER����Ӧ���ӳɹ�(CODE=220)
    if (FTP_ANS_CONNECT != GetLastResp())
    {
        m_szLastResp[0] = '\0';
        return -1;
    }
    m_szLastResp[0] = '\0';

    //���ӳɹ���Ӧ�������е�½
    if (0 != Login())
    {
        //TRACE("DEBUG", "Login error");
        return -1;
    }

    if(m_szLocalAddr[0] == '\0')
    {
        struct sockaddr_in LocalAddr;
        memset(&LocalAddr, 0, sizeof(LocalAddr));
        int namelen = sizeof(LocalAddr);
#ifdef _PLATFORM_LINUX
        if (getsockname(m_hCtrlSocket, (sockaddr *)&LocalAddr, (socklen_t *)&namelen) < 0)
#else
        if (getsockname(m_hCtrlSocket, (sockaddr *)&LocalAddr, &namelen) < 0)
#endif
        {
            //TRACE("DEBUG", "getsockname error");
            return -1;
        }

        strncpy(m_szLocalAddr, inet_ntoa(LocalAddr.sin_addr), sizeof(m_szLocalAddr));
        m_szLocalAddr[sizeof(m_szLocalAddr) - 1] = '\0';
    }
    return 0;
}

/**************************************************************************
����ԭ��    private: int PrepareDataPort()

��������    ����FTP�������������׽��֣�֪ͨFTP SERVER�������ݶ˿ڡ�
            �ڷ����κ�STOR��RETR����ǰ�������ȵ��ô˺���׼���׽��֡�
            ����FTP���������׽ӿڱ�����ÿ���ļ����͡��ɼ�ǰ���´�����
�������    ��
�������    ��
����ֵ      0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::PrepareDataPort()
{
    if (0 == m_nPassive)
    {
        return PrepareDataPortWithPort();
    }
    else
    {
        return PrepareDataPortWithPasv();
    }
}

/**************************************************************************
����ԭ��    private: int Accept(void)

��������    ����FTP���������׽��֡�
�������    ��
�������    ��
����ֵ      0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::Accept(void)
{
    struct sockaddr_in ClntAddr;
    int nAddrLen = sizeof(sockaddr_in);

    if (-1 != m_hDataSocket)
    {
        CLOSE_SOCKET(m_hDataSocket);
        m_hDataSocket = -1;
    }

#ifdef _PLATFORM_LINUX
    m_hDataSocket = accept(m_hLstnSocket, (sockaddr *)&ClntAddr, (socklen_t *)&nAddrLen);
#else
    m_hDataSocket = accept(m_hLstnSocket, (sockaddr *)&ClntAddr, &nAddrLen);
#endif
    if (m_hDataSocket < 0)
    {
        //TRACE("DEBUG", "accept error");
        return -1;
    }

    return 0;
}

/**************************************************************************
����ԭ��    private: int NotifyDataPort(void)

��������    ֪ͨFTP SERVER����������FTP���������׽��ֵ������˿ڣ��Ա�FTP SERVER
            ���������׽������ӡ�
�������    ��
�������    ��
����ֵ      0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::NotifyDataPort()
{
    short nPort = m_LstnAddr.sin_port;
    unsigned char *pPort = (unsigned char *)&nPort;
    unsigned long nIPAddr = inet_addr(m_szLocalAddr);
    unsigned char *pHost = (unsigned char *)&nIPAddr;

    //ע��FTP PORT��������IP��ַ���˿ڲ������������ֽ���
    char szCmd[255] = "";
    sprintf(szCmd, "PORT %i,%i,%i,%i,%i,%i\r\n",
            (int)pHost[0], (int)pHost[1], (int)pHost[2], (int)pHost[3],
            (int)pPort[0], (int)pPort[1]);
    int nRes1 = Command(szCmd);
    int nRes2 = GetLastResp();
    m_szLastResp[0] = '\0';
    if ((0 == nRes1) && (FTP_ANS_CWD == nRes2 || FTP_ANS_PORT == nRes2 || FTP_ANS_TRANS == nRes2))
    {
        return 0;
    }
    else
        return -1;
}

/**************************************************************************
����ԭ��    private: int Command(const char *fmt, ...)

��������    ����һ��FTP���
�������
        fmt  : FTP����
�������    ��
����ֵ      0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::Command(const char *fmt, ...)
{
    va_list ap;
    char szBuf[FTP_COMMAND_LEN] = "";

    //����FTPδ������������
    if (-1== m_hCtrlSocket)
    {
        return -1;
    }

    //��ʽ��FTP����
    va_start(ap, fmt);
    vsprintf(szBuf, fmt, ap);
    va_end(ap);

/*
    int   oflag;

    oflag = fcntl(m_hCtrlSocket, F_GETFL, 0);
    if (!(oflag & O_NONBLOCK))
    fcntl(m_hCtrlSocket, F_SETFL, oflag | O_NONBLOCK);
    recv(m_hCtrlSocket, m_szLastResp, FTP_RESPONSE_BUF, O_NONBLOCK);
    fcntl(m_hCtrlSocket, F_SETFL, oflag);
*/
    //TRACE("DEBUG", "Send Command : %s", szBuf);
    if (0 >= send(m_hCtrlSocket, szBuf, strlen(szBuf), 0))
    {
        return -1;
    }

    return 0;
}

/**************************************************************************
����ԭ��    private: int Login()

��������    ��½FTP SERVER��
�������    ��
�������    ��
����ֵ      0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::Login()
{
    Command("USER %s\r\n", m_szUserName);
    if (FTP_ANS_USER == GetLastResp())
    {
        m_szLastResp[0] = '\0';
        Command("PASS %s\r\n", m_szPassword);

        while(1)
        {
            int nRet = GetLastResp();
            switch(nRet)
            {
            case FTP_ANS_PASS:
                m_szLastResp[0] = '\0';
                return 0;
                
            case FTP_ANS_202:
            case FTP_ANS_530:
            case FTP_ANS_500:
            case FTP_ANS_501:
            case FTP_ANS_502:
            case FTP_ANS_503:
            case FTP_ANS_421:
            case FTP_ANS_332:
            case FTP_ANS_TIMEOUT://��ʱ
            case -1:
                m_szLastResp[0] = '\0';
                return -1;
                
            default:
                continue;
            }
        }
    }

    return -1;
}

/**************************************************************************
����ԭ��    private: int GetLastResp()

��������    �����һ��FTP�������Ӧ��ע�⣺����һ��FTP�����Ӧ������������Ӧ��
            ����������FTP��Ӧ����ҡ�ÿһ��FTP��Ӧ��Ϣ��ͷ�����ֽڶ������֣�
            ���FTP��Ӧ�룬�μ�RFC 959��
�������    ��
�������    ��
����ֵ      0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::GetLastResp(char *pTmp)
{
    if(pTmp != NULL || m_szLastResp[0] == '\0')
    {
        fd_set rdfs;
        FD_ZERO(&rdfs);
        FD_SET(m_hCtrlSocket,&rdfs);
        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        int nRes = select(m_hCtrlSocket+1,&rdfs,NULL,NULL,&tv);
        //����ǳ�ʱ�Ļ�����FTP_ANS_TIMEOUT;
        if(nRes == 0)
        {
            //TRACE("DEBUG", "Recv code: %d", -1);
            return FTP_ANS_TIMEOUT;
        }
        else if(nRes > 0) //�������ɹ�����
        {
            int nRecvLen = 0;
            if(pTmp)
            {
                nRecvLen = recv(m_hCtrlSocket, pTmp, FTP_READ_FILE_BUF, 0);
                if (0  < nRecvLen)
                {
                    pTmp[nRecvLen] = 0;
                }
                //TRACE("DEBUG", "Recv code: %d", -2);
                return FTP_ANS_TRANS;
            }

            nRecvLen = recv(m_hCtrlSocket, m_szLastResp, FTP_RESPONSE_BUF, 0);
            if (0  >= nRecvLen)
            {
                //TRACE("DEBUG", "Recv code: %d", -3);
                return -1;
            }

            m_szLastResp[nRecvLen] = '\0';
            //TRACE("DEBUG", "Recv : \n%s", m_szLastResp);
        }
    }

    if(m_szLastResp[0] != '\0')
    {
        int  nResp = 0;
        const char* pszDiv = strchr(m_szLastResp, '\n');
        if(pszDiv == NULL)
        {
            m_szLastResp[0] = '\0';
            //TRACE("DEBUG", "Recv code: %d", -4);
            return -1;
        }

        char szRecvTmp[4] = "";
        strncpy(szRecvTmp, m_szLastResp, 3);
        szRecvTmp[3] = '\0';
        nResp = atoi(szRecvTmp);

        strcpy(m_szLastResp, pszDiv + 1);

        //TRACE("DEBUG", "Recv code: %d", nResp);
        return nResp;
    }

    //TRACE("DEBUG", "Recv code: %d", -5);
    return -1;
}

/**************************************************************************
����ԭ��    private: int Disconnect()

��������    �Ͽ�FTP���ӡ�
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CFtpClient::Disconnect()
{
    if (0 <= m_hCtrlSocket)
    {
        CLOSE_SOCKET(m_hCtrlSocket);
        m_hCtrlSocket = -1;
    }

    if (0 <= m_hDataSocket)
    {
        CLOSE_SOCKET(m_hDataSocket);
        m_hDataSocket = -1;
    }

    if (0 <= m_hLstnSocket)
    {
        CLOSE_SOCKET(m_hLstnSocket);
        m_hLstnSocket = -1;
    }
}

/**************************************************************************
����ԭ��    public: PutFile(const char* pszLocalFile,
                             const char* pszRemoteFile)

��������    ��FTP�Զ˷���һ���ļ���
�������
        pszLocalFile  : �����ļ���
        pszRemoteFile : �Զ��ļ���
�������    ��
����ֵ
        0 �� �ɹ�
      ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::PutFile(const char* pszLocalFile,
                        const char* pszRemoteFile)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    if (!pszLocalFile || (0 != PrepareDataPort()))
    {
        return -1;
    }

    if (!pszRemoteFile)
    {
        sprintf(szCmd, "STOR %s\r\n", pszLocalFile);
    }
    else
    {
        sprintf(szCmd, "STOR %s\r\n", pszRemoteFile);
    }

    if (0 != Command(szCmd))
    {
        return -1;
    }

    int nLoop = 1;
    while(nLoop)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
		case FTP_ANS_PASV_STOR:
        case FTP_ANS_STOR:
            nLoop = 0;
            m_szLastResp[0] = '\0';
            break;
                        
        case FTP_ANS_530:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_421:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }    
    
	if (0 == m_nPassive)
		if (0 != Accept())
		{
			return -1;
		}

    FILE* fp = 0;
    char szBuf[FTP_READ_FILE_BUF] = "";

    if (0 == (fp = fopen(pszLocalFile, "rb")))
    {
         return -1;
    }

    while (!feof(fp))
    {

        int nRead = fread(szBuf, 1, FTP_READ_FILE_BUF, fp);
        if (nRead)
        {
            if (nRead != send(m_hDataSocket, szBuf, nRead, 0))
            {
                fclose(fp);
                GetLastResp();
                m_szLastResp[0] = '\0';
                return -1;
            }
        }
    }

    shutdown(m_hDataSocket, 2);
    CLOSE_SOCKET(m_hDataSocket);
    m_hDataSocket = -1;
    fclose(fp);

    //ÿ������һ���ļ��󣬱������GetLastResp()���Զ����ļ����͵Ľ��
    //�������´η���FTP��������ķ����뽫�����
    int nRet = GetLastResp();
    m_szLastResp[0] = '\0';
    if (FTP_ANS_TRANS != nRet)
    {
        return -1;
    }

    return 0;
}

/**************************************************************************
����ԭ��    public: PutFile(const char* pszLocalFile,
                             const char* pszRemoteFile)

��������    ��FTP�Զ�ȡһ���ļ���
�������
        pszLocalFile  : �����ļ���
        pszRemoteFile : �Զ��ļ���
�������    ��
����ֵ
        0 �� �ɹ�
      ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::GetFile(const char* pszLocalFile,
                        const char* pszRemoteFile)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    if (!pszLocalFile || (0 != PrepareDataPort()))
    {
        return -1;
    }

    if (!pszRemoteFile)
    {
        sprintf(szCmd, "RETR %s\r\n", pszLocalFile);
    }
    else
    {
        sprintf(szCmd, "RETR %s\r\n", pszRemoteFile);
    }

    if (0 != Command(szCmd))
    {
        return -1;
    }
    
    int nLoop = 1;
    while(nLoop)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        case FTP_ANS_RETR:
		case FTP_ANS_PASV_STOR:
            nLoop = 0;
            //m_szLastResp[0] = '\0';
            break;
                        
        case FTP_ANS_530:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_421:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }
    
    if ( (0 == m_nPassive) && (0 != Accept()) )
    {
        return -1;
    }

    FILE* fp = 0;
    char szBuf[FTP_READ_FILE_BUF] = "";
    if (0 == (fp = fopen(pszLocalFile, "wb")))
    {
         shutdown(m_hDataSocket, 2);
         CLOSE_SOCKET(m_hDataSocket);
         m_hDataSocket = -1;
         return -1;
    }

    while (1)
    {
        int nRead = recv(m_hDataSocket, szBuf, FTP_READ_FILE_BUF, 0);
        if (0 > nRead)
        {
            shutdown(m_hDataSocket, 2);
            CLOSE_SOCKET(m_hDataSocket);
            m_hDataSocket = -1;
            return -1;
        }
        else if (0 < nRead)
        {
            if ((unsigned int)nRead != fwrite(szBuf, 1, (unsigned int)nRead, fp))
            {
                fclose(fp);
                shutdown(m_hDataSocket, 2);
                CLOSE_SOCKET(m_hDataSocket);
                return -1;
            }
        }
        else
        {
            break;
        }
    }

    shutdown(m_hDataSocket, 2);
    CLOSE_SOCKET(m_hDataSocket);
    m_hDataSocket = -1;
    fclose(fp);

    //ÿȡ��һ���ļ��󣬱������GetLastResp()���Զ����ļ����͵Ľ��
    //�������´η���FTP��������ķ����뽫�����
    if (FTP_ANS_TRANS != GetLastResp())
    {
        m_szLastResp[0] = '\0';
        return -1;
    }
    m_szLastResp[0] = '\0';

    return 0;
}

/**************************************************************************
����ԭ��    private: int SetTransMode(const int nMode)

��������    ����FTP�ļ�����ģʽ��
�������
       nMode : ����ģʽ(0���ı���ʽ��1�������Ʒ�ʽ)
�������    ��
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::SetTransMode(const int nMode)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    m_nTransMode = nMode;

    if (FTP_TRANS_ASCII == nMode)
    {
        sprintf(szCmd, "TYPE A\r\n");
    }
    else
    {
        sprintf(szCmd, "TYPE I\r\n");
    }

    int nRes1 = Command(szCmd);
    if (0 != nRes1)
    {
        return -1;
    }
    
    int nLoop = 1;
    while(nLoop)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        case FTP_ANS_TYPE:
        case FTP_ANS_CWD:
            nLoop = 0;
            m_szLastResp[0] = '\0';
            return 0;
            
        case FTP_ANS_421:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_504:
        case FTP_ANS_530:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }

    return 0;
}

/**************************************************************************
����ԭ��    public: int GetDirectory(char* pszDirectory)

��������    ���FTP�Զ˵�ǰĿ¼��
�������
       pszDirectory : FTP�Զ˵�ǰĿ¼
�������
       pszDirectory : FTP�Զ˵�ǰĿ¼
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::GetDirectory(char* pszDirectory)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    if (!pszDirectory)
    {
         return -1;
    }

    sprintf(szCmd, "PWD\r\n");

    if (0 != Command(szCmd))
    {
        return -1;
    }
    
    int nLoop = 1;
    while(nLoop)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        case FTP_ANS_PWD:
            nLoop = 0;
            break;

        case FTP_ANS_421:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_502:
        case FTP_ANS_550:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    } 
 
    std::string sResp(m_szLastResp);

    int nFirst = sResp.find_first_of("\"") + 1;
    int nLast = sResp.find_last_of("\"");

    if (nFirst >= nLast)
    {
        m_szLastResp[0] = '\0';
        return -1;
    }

    memcpy(pszDirectory, &m_szLastResp[nFirst], nLast - nFirst);
    pszDirectory[nLast - nFirst] = '\0';

    m_szLastResp[0] = '\0';
    return 0;
}

/**************************************************************************
����ԭ��    public: int SetDirectory(const char* pszDirectory)

��������    ����FTP�Զ�ĳ��Ŀ¼��
�������
       pszDirectory : FTP�Զ�Ŀ¼
�������    ��
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::SetDirectory(const char* pszDirectory)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    if (!pszDirectory)
    {
        return -1;
    }

    sprintf(szCmd, "CWD %s\r\n", pszDirectory);

    if(0 != Command(szCmd))
    {
        return -1;
    }

    while(1)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        case FTP_ANS_CWD:
            m_szLastResp[0] = '\0';
            return 0;
            
        case FTP_ANS_421:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_502:
        case FTP_ANS_550:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }
    
    return 0;
}


/**************************************************************************
����ԭ��    public: int MakeDirectory(const char* pszDirectory)

��������    ��FTP�Զ˴���һ��Ŀ¼��
�������
       pszDirectory : ��������FTP�Զ�Ŀ¼
�������    ��
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::MakeDirectory(const char* pszDirectory)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    if (!pszDirectory)
    {
        return -1;
    }

    sprintf(szCmd, "MKD %s\r\n", pszDirectory);

    if (0 != Command(szCmd))
    {
        return -1;
    }

    while(1)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        case FTP_ANS_MKD:
            m_szLastResp[0] = '\0';
            return 0;
            
        case FTP_ANS_421:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_502:
        case FTP_ANS_550:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }
    
    return 0;
}

/**************************************************************************
����ԭ��    public: int DeleteFile(const char* pszFile)

��������    ��FTP�Զ˴���һ��Ŀ¼��
�������
       pszFile : ��ɾ�����ļ�
�������    ��
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::DeleteFile(const char* pszFile)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    if (!pszFile)
    {
        return -1;
    }

    sprintf(szCmd, "DELE %s\r\n", pszFile);
    int nRes1 = Command(szCmd);
    if (0 != nRes1)
    {
        return -1;
    }

        
    while(1)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        //case FTP_ANS_TRANS: //?
        case FTP_ANS_RETR:  //?
        case FTP_ANS_DELE:
        case FTP_ANS_PORT:  //?
            m_szLastResp[0] = '\0';
            return 0;
            
        case FTP_ANS_421:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_502:
        case FTP_ANS_450:
        case FTP_ANS_530:
        case FTP_ANS_550:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }
        
    return -1;
}

/**************************************************************************
����ԭ��    public: int RenameFile(const char* pszOrigName, const char* pszNewName)

��������    ��FTP�Զ˴���һ��Ŀ¼��
�������
       pszOrigName : ԭ�ļ���
       pszNewName  : pszNewName
�������    ��
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
ע    ��
        FTPɾ�ļ���RNFR��RNTO�������ͬ���
***************************************************************************/
int CFtpClient::RenameFile(const char* pszOrigName, const char* pszNewName)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    if (!pszOrigName || !pszNewName)
    {
        return -1;
    }

    sprintf(szCmd, "RNFR %s\r\n", pszOrigName);
    if (0 != Command(szCmd))
    {
         return -1;
    }
    
    int nLoop = 1;
    while(nLoop)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        case FTP_ANS_RNFR:
            nLoop = 0;
            m_szLastResp[0] = '\0';
            break;
                        
        case FTP_ANS_421:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_502:
        case FTP_ANS_450:
        case FTP_ANS_550:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }
    
    sprintf(szCmd, "RNTO %s\r\n", pszNewName);
    if (0 != Command(szCmd))
    {
         return -1;
    }
    
    while(1)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        case FTP_ANS_RNTO:
            m_szLastResp[0] = '\0';
            return 0;
            
        case FTP_ANS_421:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_502:
        case FTP_ANS_532:
        case FTP_ANS_553:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }
    
    return 0;
}

/**************************************************************************
����ԭ��    public: int RemoveDirectory(const char* pszDirectory)

��������    ɾ��FTP�Զ�һ��Ŀ¼��
�������
       pszDirectory : FTP�Զ�Ŀ¼
�������    ��
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
ע    ��
       ����FTPɾ��һ��Ŀ¼ʱ����Ŀ¼����Ϊ�գ����Ա�����Ӧ��֧�ֵݹ����
***************************************************************************/
int CFtpClient::RemoveDirectory(const char* pszDirectory)
{
    char szCmd[FTP_COMMAND_LEN] = "";
    FTP_FILELIST FileList;
    FTP_FILELIST::iterator it;

    if (!pszDirectory)
    {
        return -1;
    }

    if (-1 == ListDirectory(pszDirectory, FileList))
    {
          return -1;
    }

    it = FileList.begin();
    while (it != FileList.end())
    {
         char szTmp[255];

         sprintf(szTmp, "%s/%s", pszDirectory, (*it).szFileName);

         //�����Ŀ¼����ݹ���ã�ɾ����Ŀ¼
         if (FTP_FILE_ATTRIBUTE_DIRECTORY == (*it).nAttr)
         {
              if (RemoveDirectory(szTmp) == -1)
              {
                   return -1;
              }
         }
         //�������ͨ�ļ�����ɾ�����ļ�
         else
         {
              if (-1 == DeleteFile(szTmp))
              {
                   return -1;
              }
         }

         it++;
    }

    sprintf(szCmd, "RMD %s\r\n", pszDirectory);
    int nRes1 = Command(szCmd);
    if (0 != nRes1)
    {
        return -1;
    }
    
    while(1)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        //case FTP_ANS_TRANS:     //?
        case FTP_ANS_STOR:      //? 
        case FTP_ANS_RMD:
        case FTP_ANS_PORT:      //?
            m_szLastResp[0] = '\0';
            return 0;
            
        case FTP_ANS_421:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_502:
        case FTP_ANS_530:
        case FTP_ANS_550:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }
    
    return 0;
}

/**************************************************************************
����ԭ��    public: int ListDirectory(const char* pszDirectory, FTP_FILELIST& FileList)

��������    FTP���ļ�Ŀ¼
�������
       pszDirectory : FTP�Զ�Ŀ¼
�������
       FileList     : ��Ŀ¼���(������"."��".."����Ŀ¼)
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::ListDirectory(const char* pszDirectory, FTP_FILELIST& FileList)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    FileList.clear();

    //FTP��Ŀ¼����Ľ����ͨ��FTP�������׽��ַ��ص�
    if (!pszDirectory || (0 != PrepareDataPort()))
    {
        return -1;
    }

    //������Ŀ¼����
    sprintf(szCmd, "LIST %s\r\n", pszDirectory);
    int nRes1 = Command(szCmd);
    if (0 != nRes1)
    {
        return -1;
    }
    
    int nLoop = 1;
    while(nLoop)
    {
        int nRet = GetLastResp();
        switch(nRet)
        {
        case FTP_ANS_LIST:
        //case FTP_ANS_TRANS:     //?
        case FTP_ANS_CWD:       //? 
        case FTP_ANS_PORT:      //?
            nLoop = 0;
            //m_szLastResp[0] = '\0';
            break;
                        
        case FTP_ANS_421:
        case FTP_ANS_425:
        case FTP_ANS_426:
        case FTP_ANS_451:
        case FTP_ANS_450:
        case FTP_ANS_500:
        case FTP_ANS_501:
        case FTP_ANS_502:
        case FTP_ANS_530:
        case FTP_ANS_TIMEOUT://��ʱ
        case -1:
            m_szLastResp[0] = '\0';
            return -1;
            
        default:
            continue;
        }
    }
    
    if ( (0 == m_nPassive) && (0 != Accept()) )
    {
        return -1;
    }

    char szBuf[FTP_READ_FILE_BUF];
    int nReadTotal = 0;

    while (1)
    {
        int nRead = recv(m_hDataSocket, szBuf, FTP_READ_FILE_BUF, 0);
        if (0 > nRead)
        {
            //TRACE("DEBUG", "Recv error : %d", errno);
            shutdown(m_hDataSocket, 2);
            CLOSE_SOCKET(m_hDataSocket);
            m_hDataSocket = -1;
            return -1;
        }
        else if (0 == nRead)
        {
             break;
        }

        //���ݳ���
        if (m_nRecvBufLen < (nReadTotal + nRead))
        {
            int nResRecvBufLen = m_nRecvBufLen;
            m_nRecvBufLen += 102400;
            m_nRecvBufLen = max(nReadTotal + nRead, m_nRecvBufLen);

            char* pRecvBuf = new char[m_nRecvBufLen];
            if(pRecvBuf == NULL)
            {
                shutdown(m_hDataSocket, 2);
                CLOSE_SOCKET(m_hDataSocket);
                m_hDataSocket = -1;
                //TRACE("DEBUG", "���ݳ��� : %d > %d", nReadTotal + nRead, m_nRecvBufLen);
                return -1;
            }
            
            memcpy(pRecvBuf, m_pRecvBuf, nResRecvBufLen);
            delete [] m_pRecvBuf;
            m_pRecvBuf = pRecvBuf;
        }

        memcpy(&m_pRecvBuf[nReadTotal], szBuf, nRead);
        nReadTotal += nRead;
    }
    //��ֹ��������Ӱ����һ�ζ�
    if (FTP_ANS_TRANS != GetLastResp())
    {
        m_szLastResp[0] = '\0';
        return -1;
    }

    shutdown(m_hDataSocket, 2);
    CLOSE_SOCKET(m_hDataSocket);
    m_hDataSocket = -1;

    m_pRecvBuf[nReadTotal] = '\0';

    //����FTP LIST�ķ��ر���m_szRecvBuf
    int nLine = 0;
    char *pStr = NULL;
    char *pTemp = m_pRecvBuf;

    //ͳ��LIST����ص�����
    while (pStr = strstr(pTemp, "\n"))
    {
        nLine++;
        pTemp = pStr +1;
    }

    //modify by chenliangwei, 2003-04-29
    //����Զ����Unixϵͳ�������ڵ�һ�з��ش˴��б����ݵ��ֽ�����
    //����Զ����NTϵͳ���򲻻᣻����Զ��ΪNTϵͳʱ��ֻ��һ��
    //�ļ������Ҳֻ�᷵��һ�����ݣ���˲������������ж�

    //���ֻ��1�У������pszDirectory�²������ļ�
    //if (1 >= nLine)

    if (1 > nLine)
    {
        m_szLastResp[0] = '\0';
        return 0;
    }

    int nFileNum = nLine;
    pTemp = m_pRecvBuf;

    char szLine[2048];
    int nChar;
    SFtpFile AFile;

    //����һ��ֻ�����е�ʱ������ͷһ��
    int nColum = 0;
    pStr = strstr(pTemp, "\n");
    strncpy(szLine,pTemp,pStr-pTemp);
    szLine[2047] = '\0';
    char *pToken = strtok(szLine," ");
    while(pToken)
    {
        nColum += 1;
        pToken = strtok(NULL," ");
    }
    if(nColum <= 2)
    {
        pTemp = pStr + 1;
        nFileNum -= 1;
    }

    while (nFileNum> 0)
    {
        pStr = strstr(pTemp, "\n");
        nChar = pStr - pTemp;
        strncpy(szLine, pTemp, nChar);
        szLine[nChar] = '\0';
        pTemp = pStr + 1;
        nFileNum --;

        if (0 == AnalyzeAFileLine(szLine, AFile))
        {
             //���г���Ŀ¼(..)�͵�ǰĿ¼(.)
             if ((strcmp(AFile.szFileName, ".") != 0)
              && (strcmp(AFile.szFileName, "..") != 0))
             {
                 FileList.push_back(AFile);
             }
        }
    }

    m_szLastResp[0] = '\0';
    return 0;
}

/**************************************************************************
����ԭ��    private: int AnalyzeAFileLine(const char* pszLine, SFtpFile& AFile)

��������    ����FTP��Ŀ¼���ļ�����
�������
       pszLine : �ļ���
�������
       AFile   : �ļ��������
����ֵ
            0 �� �ɹ�
          ��0 �� ʧ��
�׳��쳣    ��
***************************************************************************/
int CFtpClient::AnalyzeAFileLine(char* pszLine, SFtpFile& AFile)
{
    if (!pszLine)
    {
        return -1;
    }

    int nYear, nMon, nDay;

    char* pTemp = pszLine;
    int nLoop = 0;;
    char szDate1[10];
    char szDate2[10];
    char szDate3[10];

    char* pToken;

    //�����ɿո�ֿ��ĸ���
    pToken = strtok(pTemp, " ");
    while (pToken && (nLoop < 9))
    {
        //��1�У��ļ�����
        if (nLoop == 0)
        {
            switch (pToken[0])
            {
            case '-':
                AFile.nAttr = FTP_FILE_ATTRIBUTE_NORMAL;
                break;

            case 'd':
               AFile.nAttr = FTP_FILE_ATTRIBUTE_DIRECTORY;
               break;

            case 'l':
               AFile.nAttr = FTP_FILE_ATTRIBUTE_LINK;
               break;

            default:
               AFile.nAttr = FTP_FILE_ATTRIBUTE_OTHER;
               break;
            }
        }

        //added by chenliangwei
        if (nLoop == 4)
        {
            AFile.nFileSize = (unsigned long)atoi(pToken);
        }
        //added end

        //��6����8�У��ļ���������
        if (nLoop == 5)
        {
            strncpy(szDate1, pToken, 8);
            szDate1[8] = '\0';
        }

        if (nLoop == 6)
        {
            strncpy(szDate2, pToken, 8);
            szDate2[8] = '\0';
        }

        if (nLoop == 7)
        {
            strncpy(szDate3, pToken, 8);
            szDate3[8] = '\0';
        }

        //��9�У��ļ���
        if (nLoop == 8)
        {
            strncpy(AFile.szFileName, pToken, 255);
            AFile.szFileName[254] = '\0';
            int nLen = strlen(AFile.szFileName);
            if(AFile.szFileName[nLen-1] == 13)
            {
                //Windowsƽ̨��\r\n,Unix��\n
                AFile.szFileName[nLen-1] = '\0';
            }
            break;
        }

        nLoop ++;
        pToken = strtok(NULL, " ");
    }

    //FTP��Ŀ¼�����ļ��ǵ��괴��ʱ������ʾ�� �� ʱ:�֣����
    //���ǵ��괴��������ʾ�� �� ��
    /*
        ���İ��ʽ��
        -rwxr-xr-x   1 root     root           0 12��  4 19:34 root
        drwxr-xr-x   2 root     sys         1024 2002  11�� 11 sbin
        Ӣ�İ��ʽ��
        dr-xr-xr-x  32 bin        bin           8192 Dec  5 14:15 var
        -rw-rw-rw-   1 root       sys              0 Dec  9  2002 wf.txt
    */
    if(strstr(szDate1, "��") || strstr(szDate2, "��"))
    {
        if (pToken = strstr(szDate3, ":"))       //�� �� ʱ:�� ��ʽ
        {
            time_t tTime;

            time(&tTime);
            tm tmLocal = *localtime(&tTime);
            nYear = tmLocal.tm_year + 1900;

            nDay = atoi(szDate2);
            int nIndex = 0;
            int nStrLen = strlen(szDate1);

            //��FTP SERVERΪ���Ĳ���ϵͳʱ����ʾ���·����ֺ��һ�����֡��¡���
            //��ˣ��ڼ����·ݵ�ʱ���ܰѴ˺��ֵ�������ת��
            while ((szDate1[nIndex] >= '0') && (szDate1[nIndex] <= '9') && (nIndex < nStrLen))
            {
                nIndex ++;
            }

            szDate1[nIndex] = '\0';
            nMon = atoi(szDate1);
        }
        else                                     //�� �� �� ��ʽ
        {
            nYear = atoi(szDate1);
            nDay = atoi(szDate3);

            int nIndex = 0;
            int nStrLen = strlen(szDate1);

            while ((szDate2[nIndex] >= '0') && (szDate2[nIndex] <= '9') && (nIndex < nStrLen))
            {
                nIndex ++;
            }

            szDate2[nIndex] = '\0';
            nMon = atoi(szDate2);
        }
    }
    else
    {
        if (pToken = strstr(szDate3, ":"))       //�� �� ʱ:�� ��ʽ
        {
            time_t tTime;

            time(&tTime);
            tm tmLocal = *localtime(&tTime);
            nYear = tmLocal.tm_year + 1900;
            nDay = atoi(szDate2);
            nMon = GetMonFromStr(szDate1);
        }
        else                                     //�� �� �� ��ʽ
        {
            nYear = atoi(szDate3);
            nDay = atoi(szDate2);
            nMon = GetMonFromStr(szDate1);
        }
    }

    SNPRINTF(AFile.szDate, sizeof(AFile.szDate), "%04d%02d%02d", nYear, nMon, nDay);
    AFile.szDate[sizeof(AFile.szDate) - 1] = '\0';

    return 0;
}

int CFtpClient::GetMonFromStr(const char* szMonth)
{
	if (strcmp(szMonth, "Jan") == 0)
	{
        return 1;
	}
	else if (strcmp(szMonth, "Feb") == 0)
	{
		return 2;
	}
	else if (strcmp(szMonth, "Mar") == 0)
	{
		return 3;
	}
	else if (strcmp(szMonth, "Apr") == 0)
	{
		return 4;
	}
	else if (strcmp(szMonth, "May") == 0)
	{
		return 5;
	}
	else if (strcmp(szMonth, "Jun") == 0)
	{
		return 6;
	}
	else if (strcmp(szMonth, "Jul") == 0)
	{
		return 7;
	}
	else if (strcmp(szMonth, "Aug") == 0)
	{
		return 8;
	}
	else if (strcmp(szMonth, "Sep") == 0)
	{
		return 9;
	}
	else if (strcmp(szMonth, "Oct") == 0)
	{
		return 10;
	}
	else if (strcmp(szMonth, "Nov") == 0)
	{
		return 11;
	}
	else if (strcmp(szMonth, "Dec") == 0)
	{
		return 12;
	}
    
	return 0;
}

int CFtpClient::PrepareDataPortWithPort()
{
	
    if (-1 != m_hLstnSocket)
    {
        CLOSE_SOCKET(m_hLstnSocket);
        m_hLstnSocket = -1;
    }
	
    memset(&m_LstnAddr, 0, sizeof(m_LstnAddr));
    m_LstnAddr.sin_family      = AF_INET;
	
    //ע�⣺��ΪFTP�ͻ��ˣ���Ӧ��ָ�����ذ󶨵Ķ˿ڣ����������ɰ�ʧ��
    //���˿ں�ָ��Ϊ0��ʾ�ɲ���ϵͳָ���˿ں�(1024-5000֮��)
    m_LstnAddr.sin_port        = htons(0);
    m_LstnAddr.sin_addr.s_addr = inet_addr(m_szLocalAddr);
	
    m_hLstnSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_hLstnSocket < 0)
    {
        //TRACE("DEBUG", "create socket error");
        return -1;
    }
	
    if (bind(m_hLstnSocket, (sockaddr *)&m_LstnAddr, sizeof(m_LstnAddr)) < 0)
    {
        //TRACE("DEBUG", "bind error");
        return -1;
    }
	
    int nLen = sizeof(m_LstnAddr);
    //��ü����׽���ʵ�ʵ������˿�
#ifdef _PLATFORM_LINUX
    getsockname(m_hLstnSocket, (sockaddr *)&m_LstnAddr, (socklen_t*)&nLen);
#else
    getsockname(m_hLstnSocket, (sockaddr *)&m_LstnAddr, &nLen);
#endif
	
    if (listen(m_hLstnSocket, 1) < 0)
    {
        //TRACE("DEBUG", "listen error");
        return -1;
    }
	
    //֪ͨFTP SERVER���������׽��ֶ˿�
    //����FTP�ļ�����ģʽ(FTPĬ�ϰ�ASCII��ʽ����)
    //����������ļ�ʹ���ı���ʽ���䣬������ļ����䲻��ȷ
    if ((0 != NotifyDataPort())
		|| (0 != SetTransMode(m_nTransMode)))
    {
        //TRACE("DEBUG", "NotifyDataPort error");
        return -1;
    }
	
    return 0;
}


int CFtpClient::PrepareDataPortWithPasv()
{
    if (-1 != m_hDataSocket)
    {
        CLOSE_SOCKET(m_hDataSocket);
        m_hLstnSocket = -1;
    }
    
    char szCmd[255] = "PASV\r\n";
    int nRes1 = Command(szCmd);
    int nRes2 = GetLastResp();
    if (0 == nRes1 && FTP_ANS_PASV == nRes2)
    {
        struct sockaddr_in SvrAddr;
        if(GetAddrInPasvANS (SvrAddr) < 0)
        {
            return -1;
        };
        
        m_hDataSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_hDataSocket < 0)
        {
            return -1;
        }
        
        if(connect(m_hDataSocket,(sockaddr*)&SvrAddr, sizeof(SvrAddr)) < 0)
        {
            return -1;
        }
        
        if (0 != SetTransMode(m_nTransMode))
        {
            return -1;
        }
        
        return 0;
    }
    return -1;
}

int CFtpClient::GetAddrInPasvANS (sockaddr_in& SvrAddr)
{
    char *p, *a;
    char pasv[64];
    char c;
    char *pt;
    int pflag;
    
    p = m_szLastResp + 3;
    pt = pasv;
    pflag = 1;
    
    for(; c = *p++; )
    {
        if (pflag == 1 && isdigit(c))
        {
            pflag = 2;
        }
        if (pflag == 2)
        {
            if (c != '\r' && c != '\n' && c != ')')
            {
                *pt++ = c;
            }
            else
            {
                *pt = '\0';
                break;
            }
        }
    }
    
    int a0, a1, a2, a3, p0, p1;
    if (sscanf(pasv,"%d,%d,%d,%d,%d,%d",
        &a0, &a1, &a2, &a3, &p0, &p1) != 6)
    {
        //TRACE("Passive mode address scan failure. ");
        return -1;
    }
    
    memset(&SvrAddr, 0, sizeof(SvrAddr));
    SvrAddr.sin_family = AF_INET;
    a = (char *)&SvrAddr.sin_addr.s_addr;
    a[0] = a0 & 0xff;
    a[1] = a1 & 0xff;
    a[2] = a2 & 0xff;
    a[3] = a3 & 0xff;
    p = (char *)&SvrAddr.sin_port;
    p[0] = p0 & 0xff;
    p[1] = p1 & 0xff;
    
    return 0;
}












































