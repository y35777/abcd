#ifndef _FTP_COMMAND_H_
#define _FTP_COMMAND_H_

//������"ace/OS.h"ͷ�ļ�����WINDDOWSƽ̨�£�Ӧ�ó�����Բ�����ʽ��
//����"WSAStartup()"��ʼ��WINSOCK
#include "ace/OS.h"

#ifdef _PLATFORM_WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    #define CLOSE_SOCKET   closesocket
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define CLOSE_SOCKET   close
#endif

#include <list>
#define LIST        std::list

typedef     int         BOOL;

#define  FALSE           0
#define  TRUE            1

//����FTP�����������(�μ�RFC 959)
#define FTP_ANS_202        202     //Command not implemented, superfluous at this site
#define FTP_ANS_332        332     //Need account for login.
#define FTP_ANS_421        421     //Service not available, closing control connection. This may be a reply to any command if the service knows it must shut down. 
#define FTP_ANS_500        500     //Syntax error, command unrecognized
#define FTP_ANS_501        501     //Syntax error in parameters or arguments. 
#define FTP_ANS_502        502     //Command not implemented. 
#define FTP_ANS_503        503     //Bad sequence of commands. 
#define FTP_ANS_530        530     //Not logged in.

//for STOR
#define FTP_ANS_125        125     //Data connection already open; transfer starting. 
#define FTP_ANS_425        425     //Can't open data connection

/*
    for TYPE
    200 
    500, 501, 504, 421, 530
*/
#define FTP_ANS_TYPE       200     //FTP�����ļ�����ģʽ
#define FTP_ANS_504        504     // Command not implemented for that parameter. 
 
/*
    for PWD 
    257 
    500, 501, 502, 421, 550 
*/
#define FTP_ANS_PWD        257     //FTP��õ�ǰĿ¼
#define FTP_ANS_550        550     //550 Requested action not taken.File unavailable (e.g., file not found, no access).

/*
    for CWD 
    250 
    500, 501, 502, 421, 530, 550 
*/
#define FTP_ANS_CWD        250     //FTP�л�Զ��Ŀ¼

/*
    for MKD 
    257 
    500, 501, 502, 421, 530, 550 
*/
#define FTP_ANS_MKD        257     //FTP����Ŀ¼

/*
    for DELE 
    250 
    450, 550 
    500, 501, 502, 421, 530 
*/
#define FTP_ANS_DELE       250     //FTPɾ���ļ�
#define FTP_ANS_450        450     //450 Requested file action not taken. File unavailable (e.g., file busy). 

/*
    for RNFR 
    450, 550 
    500, 501, 502, 421, 530 
    350 
*/
#define FTP_ANS_RNFR       350     //FTP����1

/*
    for RNTO 
    250 
    532, 553 
    500, 501, 502, 503, 421, 530 
*/
#define FTP_ANS_RNTO       250     //FTP����2
#define FTP_ANS_532        532     //Need account for storing files. 
#define FTP_ANS_553        553     //Requested action not taken. File name not allowed. 

/*
    for RMD 
    250 
    500, 501, 502, 421, 530, 550 
*/
#define FTP_ANS_RMD        250     //FTPɾ��Ŀ¼

/*
    for LIST 
    125, 150 
    226, 250 
    425, 426, 451 
    450 
    500, 501, 502, 421, 530 
*/
#define FTP_ANS_LIST       150     //FTP��Ŀ¼�ɹ�
#define FTP_ANS_426        426     //Connection closed; transfer aborted. 
#define FTP_ANS_451        451     //Requested action aborted. Local error in processing.  

#define FTP_ANS_TIMEOUT    -2      //Requested action aborted. Local error in processing.  


#define FTP_ANS_MKD        257     //FTP����Ŀ¼
#define FTP_ANS_CONNECT    220     //FTP�����׽������ӳɹ�
#define FTP_ANS_USER       331     //FTP���������û���½����
#define FTP_ANS_PASS       230     //FTP��½�ɹ�
#define FTP_ANS_RETR       150     //FTPȡ�ļ�����ִ�гɹ�
#define FTP_ANS_QUIT       221     //FTPע��
#define FTP_ANS_PORT       200     //FTP���������׽��ֶ˿�
#define FTP_ANS_STOR       150     //FTP�����ļ�����ִ��File status okay; about to open data connection. 
#define FTP_ANS_TRANS      226     //FTP�ļ��������
#define FTP_ANS_PASV	   227     //ftp set passive mode
#define FTP_ANS_PASV_STOR  125     //FTP�����ļ�����ִ��File status okay; about to open data connection. 

#define FTP_HOST_NAME_LEN  48
#define FTP_USER_NAME_LEN  32
#define FTP_PASSWORD_LEN   32

#define FTP_MAX_LINE_LEN   4096
#define FTP_RESPONSE_BUF   65536   //FTP������Ӧ����󳤶�
#define FTP_COMMAND_LEN    1024    //FTP������������󳤶�

#define FTP_READ_FILE_BUF  1024    //���͡���ȡFTP����ʱ��һ�η��͡���ȡ�õĴ󳤶�

#define FTP_CTRL_PORT      21      //FTP�����׽ӿ������˿�

enum FTP_TRANSFER_MODE
{
    FTP_TRANS_ASCII  = 0,            //���ı���ʽ����(FTPĬ�Ϸ�ʽ)
    FTP_TRANS_BINARY = 1             //�������Ʒ�ʽ����
};

enum FTP_ACTION
{
    FTP_ACTION_NONE  = 0,
    FTP_ACTION_PUT   = 1,
    FTP_ACTION_GET   = 2
};

enum FTP_FILE_ATTRIBUTE
{
    FTP_FILE_ATTRIBUTE_DIRECTORY = 0,    //Ŀ¼�ļ�
    FTP_FILE_ATTRIBUTE_LINK      = 1,    //�����ļ�
    FTP_FILE_ATTRIBUTE_NORMAL    = 2,    //��ͨ�ļ�
    FTP_FILE_ATTRIBUTE_OTHER     = 3     //�������͵��ļ�
};

struct SFtpFile
{
    char    szFileName[255];   //�ļ���
    char    szDate[10];        //�ļ���������
    unsigned char    nAttr;    //�ļ����ԣ�Ŀ¼�ļ��������ļ�����ͨ�ļ��ȣ�
    unsigned long   nFileSize;            //�ļ���С add by chenliangwei

};

typedef LIST<SFtpFile> FTP_FILELIST;

#endif  //_FTP_COMMAND_H_
