/******************************************************************************
 *产品名    ：iGateway Bill V200
 *模块名    ：
 *文件名    ：ftp_client.cpp
 *描述信息  ：类CFtpClient的实现文件，该类封装了FTP客户端的常用功能。
              对于与FTP SERVER的通信，统一使用原始Berkeley套接口编程，
              阻塞模式。
 *版本说明  ：
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：Qiu Yiping
 *创建时间  ：2002-05-25
 *修改记录  ：
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

    m_hCtrlSocket = -1;    //FTP控制套接字
    m_hDataSocket = -1;    //FTP数据套接字
    m_hLstnSocket = -1;    //FTP侦听套接字

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
函数原型    public: int Connect(char* pszHost, char* pszUser, char* pszPass,
                                char* pszLocalAddr, int nTransMode,
                                short nCtrlPort)

功能描述    连接、登陆FTP SERVER。
输入参数
       pszHost        : FTP SERVER地址
       pszUser        : 登陆用户名
       pszPass        : 登陆口令
       pszLocalAddr   : 本地地址
       nCtrlPort      : FTP控制命令端口(默认为21)
       nTransMode     : 文件传输模式

输出参数    无
返回值
       0 － 连接、登陆成功
     非0 － 连接或登陆失败
抛出异常    无
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

    //如成功连接FTP SERVER，FTP SERVER将响应连接成功(CODE=220)
    if (FTP_ANS_CONNECT != GetLastResp())
    {
        m_szLastResp[0] = '\0';
        return -1;
    }
    m_szLastResp[0] = '\0';

    //连接成功后应立即进行登陆
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
函数原型    private: int PrepareDataPort()

功能描述    创建FTP数据连接侦听套接字，通知FTP SERVER本端数据端口。
            在发送任何STOR、RETR命令前必须首先调用此函数准备套接字。
            而且FTP数据连接套接口必须在每次文件发送、采集前重新创建。
输入参数    无
输出参数    无
返回值      0 － 成功
          非0 － 失败
抛出异常    无
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
函数原型    private: int Accept(void)

功能描述    接收FTP数据连接套接字。
输入参数    无
输出参数    无
返回值      0 － 成功
          非0 － 失败
抛出异常    无
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
函数原型    private: int NotifyDataPort(void)

功能描述    通知FTP SERVER，本端用于FTP数据侦听套接字的侦听端口，以便FTP SERVER
            进行数据套接字连接。
输入参数    无
输出参数    无
返回值      0 － 成功
          非0 － 失败
抛出异常    无
***************************************************************************/
int CFtpClient::NotifyDataPort()
{
    short nPort = m_LstnAddr.sin_port;
    unsigned char *pPort = (unsigned char *)&nPort;
    unsigned long nIPAddr = inet_addr(m_szLocalAddr);
    unsigned char *pHost = (unsigned char *)&nIPAddr;

    //注意FTP PORT命令后跟的IP地址，端口参数都是网络字节序
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
函数原型    private: int Command(const char *fmt, ...)

功能描述    发送一条FTP命令。
输入参数
        fmt  : FTP命令
输出参数    无
返回值      0 － 成功
          非0 － 失败
抛出异常    无
***************************************************************************/
int CFtpClient::Command(const char *fmt, ...)
{
    va_list ap;
    char szBuf[FTP_COMMAND_LEN] = "";

    //错误：FTP未建立控制连接
    if (-1== m_hCtrlSocket)
    {
        return -1;
    }

    //格式化FTP命令
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
函数原型    private: int Login()

功能描述    登陆FTP SERVER。
输入参数    无
输出参数    无
返回值      0 － 成功
          非0 － 失败
抛出异常    无
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
            case FTP_ANS_TIMEOUT://超时
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
函数原型    private: int GetLastResp()

功能描述    获得上一个FTP命令的响应。注意：发送一条FTP命令后应立即读出其响应，
            否则可能造成FTP响应码错乱。每一条FTP响应消息的头三个字节都是数字，
            组成FTP响应码，参见RFC 959。
输入参数    无
输出参数    无
返回值      0 － 成功
          非0 － 失败
抛出异常    无
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
        //如果是超时的话返回FTP_ANS_TIMEOUT;
        if(nRes == 0)
        {
            //TRACE("DEBUG", "Recv code: %d", -1);
            return FTP_ANS_TIMEOUT;
        }
        else if(nRes > 0) //如果句柄成功返回
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
函数原型    private: int Disconnect()

功能描述    断开FTP连接。
输入参数    无
输出参数    无
返回值      无
抛出异常    无
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
函数原型    public: PutFile(const char* pszLocalFile,
                             const char* pszRemoteFile)

功能描述    向FTP对端发送一个文件。
输入参数
        pszLocalFile  : 本地文件名
        pszRemoteFile : 对端文件名
输出参数    无
返回值
        0 － 成功
      非0 － 失败
抛出异常    无
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
        case FTP_ANS_TIMEOUT://超时
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

    //每发送完一个文件后，必须调用GetLastResp()，以读出文件发送的结果
    //，否则，下次发送FTP控制命令的返回码将会错乱
    int nRet = GetLastResp();
    m_szLastResp[0] = '\0';
    if (FTP_ANS_TRANS != nRet)
    {
        return -1;
    }

    return 0;
}

/**************************************************************************
函数原型    public: PutFile(const char* pszLocalFile,
                             const char* pszRemoteFile)

功能描述    从FTP对端取一个文件。
输入参数
        pszLocalFile  : 本地文件名
        pszRemoteFile : 对端文件名
输出参数    无
返回值
        0 － 成功
      非0 － 失败
抛出异常    无
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
        case FTP_ANS_TIMEOUT://超时
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

    //每取完一个文件后，必须调用GetLastResp()，以读出文件发送的结果
    //，否则，下次发送FTP控制命令的返回码将会错乱
    if (FTP_ANS_TRANS != GetLastResp())
    {
        m_szLastResp[0] = '\0';
        return -1;
    }
    m_szLastResp[0] = '\0';

    return 0;
}

/**************************************************************************
函数原型    private: int SetTransMode(const int nMode)

功能描述    设置FTP文件传输模式。
输入参数
       nMode : 传输模式(0－文本方式，1－二进制方式)
输出参数    无
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
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
        case FTP_ANS_TIMEOUT://超时
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
函数原型    public: int GetDirectory(char* pszDirectory)

功能描述    获得FTP对端当前目录。
输入参数
       pszDirectory : FTP对端当前目录
输出参数
       pszDirectory : FTP对端当前目录
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
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
        case FTP_ANS_TIMEOUT://超时
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
函数原型    public: int SetDirectory(const char* pszDirectory)

功能描述    进入FTP对端某个目录。
输入参数
       pszDirectory : FTP对端目录
输出参数    无
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
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
        case FTP_ANS_TIMEOUT://超时
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
函数原型    public: int MakeDirectory(const char* pszDirectory)

功能描述    在FTP对端创建一个目录。
输入参数
       pszDirectory : 待创建的FTP对端目录
输出参数    无
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
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
        case FTP_ANS_TIMEOUT://超时
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
函数原型    public: int DeleteFile(const char* pszFile)

功能描述    在FTP对端创建一个目录。
输入参数
       pszFile : 待删除的文件
输出参数    无
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
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
        case FTP_ANS_TIMEOUT://超时
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
函数原型    public: int RenameFile(const char* pszOrigName, const char* pszNewName)

功能描述    在FTP对端创建一个目录。
输入参数
       pszOrigName : 原文件名
       pszNewName  : pszNewName
输出参数    无
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
注    释
        FTP删文件由RNFR和RNTO两个命令共同完成
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
        case FTP_ANS_TIMEOUT://超时
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
        case FTP_ANS_TIMEOUT://超时
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
函数原型    public: int RemoveDirectory(const char* pszDirectory)

功能描述    删除FTP对端一个目录。
输入参数
       pszDirectory : FTP对端目录
输出参数    无
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
注    释
       利用FTP删除一个目录时，该目录必须为空，所以本函数应该支持递归调用
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

         //如果是目录，则递归调用，删除子目录
         if (FTP_FILE_ATTRIBUTE_DIRECTORY == (*it).nAttr)
         {
              if (RemoveDirectory(szTmp) == -1)
              {
                   return -1;
              }
         }
         //如果是普通文件，则删除该文件
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
        case FTP_ANS_TIMEOUT://超时
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
函数原型    public: int ListDirectory(const char* pszDirectory, FTP_FILELIST& FileList)

功能描述    FTP列文件目录
输入参数
       pszDirectory : FTP对端目录
输出参数
       FileList     : 列目录结果(不包含"."和".."两个目录)
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
***************************************************************************/
int CFtpClient::ListDirectory(const char* pszDirectory, FTP_FILELIST& FileList)
{
    char szCmd[FTP_COMMAND_LEN] = "";

    FileList.clear();

    //FTP列目录命令的结果是通过FTP的数据套接字返回的
    if (!pszDirectory || (0 != PrepareDataPort()))
    {
        return -1;
    }

    //发送列目录命令
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
        case FTP_ANS_TIMEOUT://超时
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

        //数据超长
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
                //TRACE("DEBUG", "数据超长 : %d > %d", nReadTotal + nRead, m_nRecvBufLen);
                return -1;
            }
            
            memcpy(pRecvBuf, m_pRecvBuf, nResRecvBufLen);
            delete [] m_pRecvBuf;
            m_pRecvBuf = pRecvBuf;
        }

        memcpy(&m_pRecvBuf[nReadTotal], szBuf, nRead);
        nReadTotal += nRead;
    }
    //防止残留命令影响下一次读
    if (FTP_ANS_TRANS != GetLastResp())
    {
        m_szLastResp[0] = '\0';
        return -1;
    }

    shutdown(m_hDataSocket, 2);
    CLOSE_SOCKET(m_hDataSocket);
    m_hDataSocket = -1;

    m_pRecvBuf[nReadTotal] = '\0';

    //分析FTP LIST的返回报文m_szRecvBuf
    int nLine = 0;
    char *pStr = NULL;
    char *pTemp = m_pRecvBuf;

    //统计LIST命令返回的行数
    while (pStr = strstr(pTemp, "\n"))
    {
        nLine++;
        pTemp = pStr +1;
    }

    //modify by chenliangwei, 2003-04-29
    //对于远端是Unix系统，它会在第一行返回此次列表数据的字节数；
    //对于远端是NT系统，则不会；所以远端为NT系统时，只有一个
    //文件的情况也只会返回一行数据，因此不能作出如下判断

    //如果只有1行，则表明pszDirectory下不存在文件
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

    //当第一行只有两列的时候跳过头一行
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
             //不列出父目录(..)和当前目录(.)
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
函数原型    private: int AnalyzeAFileLine(const char* pszLine, SFtpFile& AFile)

功能描述    分析FTP列目录中文件属性
输入参数
       pszLine : 文件行
输出参数
       AFile   : 文件相关属性
返回值
            0 － 成功
          非0 － 失败
抛出异常    无
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

    //分析由空格分开的各行
    pToken = strtok(pTemp, " ");
    while (pToken && (nLoop < 9))
    {
        //第1列：文件属性
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

        //第6－第8列：文件创建日期
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

        //第9列：文件名
        if (nLoop == 8)
        {
            strncpy(AFile.szFileName, pToken, 255);
            AFile.szFileName[254] = '\0';
            int nLen = strlen(AFile.szFileName);
            if(AFile.szFileName[nLen-1] == 13)
            {
                //Windows平台是\r\n,Unix是\n
                AFile.szFileName[nLen-1] = '\0';
            }
            break;
        }

        nLoop ++;
        pToken = strtok(NULL, " ");
    }

    //FTP列目录，当文件是当年创建时，则显示月 日 时:分，如果
    //不是当年创建，则显示年 月 日
    /*
        中文版格式：
        -rwxr-xr-x   1 root     root           0 12月  4 19:34 root
        drwxr-xr-x   2 root     sys         1024 2002  11月 11 sbin
        英文版格式：
        dr-xr-xr-x  32 bin        bin           8192 Dec  5 14:15 var
        -rw-rw-rw-   1 root       sys              0 Dec  9  2002 wf.txt
    */
    if(strstr(szDate1, "月") || strstr(szDate2, "月"))
    {
        if (pToken = strstr(szDate3, ":"))       //月 日 时:分 格式
        {
            time_t tTime;

            time(&tTime);
            tm tmLocal = *localtime(&tTime);
            nYear = tmLocal.tm_year + 1900;

            nDay = atoi(szDate2);
            int nIndex = 0;
            int nStrLen = strlen(szDate1);

            //当FTP SERVER为中文操作系统时，显示的月份数字后跟一个汉字“月”，
            //因此，在计算月份的时候不能把此汉字当成数字转换
            while ((szDate1[nIndex] >= '0') && (szDate1[nIndex] <= '9') && (nIndex < nStrLen))
            {
                nIndex ++;
            }

            szDate1[nIndex] = '\0';
            nMon = atoi(szDate1);
        }
        else                                     //年 月 日 格式
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
        if (pToken = strstr(szDate3, ":"))       //月 日 时:分 格式
        {
            time_t tTime;

            time(&tTime);
            tm tmLocal = *localtime(&tTime);
            nYear = tmLocal.tm_year + 1900;
            nDay = atoi(szDate2);
            nMon = GetMonFromStr(szDate1);
        }
        else                                     //月 日 年 格式
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
	
    //注意：作为FTP客户端，不应该指定本地绑定的端口，否则可能造成绑定失败
    //将端口号指定为0表示由操作系统指定端口号(1024-5000之间)
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
    //获得监听套接字实际的侦听端口
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
	
    //通知FTP SERVER本地数据套接字端口
    //设置FTP文件传输模式(FTP默认按ASCII方式传输)
    //如果二进制文件使用文本方式传输，会造成文件传输不正确
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












































