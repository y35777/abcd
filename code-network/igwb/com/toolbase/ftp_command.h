#ifndef _FTP_COMMAND_H_
#define _FTP_COMMAND_H_

//包含了"ace/OS.h"头文件后，在WINDDOWS平台下，应用程序可以不必显式地
//调用"WSAStartup()"初始化WINSOCK
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

//定义FTP控制命令返回码(参见RFC 959)
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
#define FTP_ANS_TYPE       200     //FTP设置文件传输模式
#define FTP_ANS_504        504     // Command not implemented for that parameter. 
 
/*
    for PWD 
    257 
    500, 501, 502, 421, 550 
*/
#define FTP_ANS_PWD        257     //FTP获得当前目录
#define FTP_ANS_550        550     //550 Requested action not taken.File unavailable (e.g., file not found, no access).

/*
    for CWD 
    250 
    500, 501, 502, 421, 530, 550 
*/
#define FTP_ANS_CWD        250     //FTP切换远端目录

/*
    for MKD 
    257 
    500, 501, 502, 421, 530, 550 
*/
#define FTP_ANS_MKD        257     //FTP创建目录

/*
    for DELE 
    250 
    450, 550 
    500, 501, 502, 421, 530 
*/
#define FTP_ANS_DELE       250     //FTP删除文件
#define FTP_ANS_450        450     //450 Requested file action not taken. File unavailable (e.g., file busy). 

/*
    for RNFR 
    450, 550 
    500, 501, 502, 421, 530 
    350 
*/
#define FTP_ANS_RNFR       350     //FTP改名1

/*
    for RNTO 
    250 
    532, 553 
    500, 501, 502, 503, 421, 530 
*/
#define FTP_ANS_RNTO       250     //FTP改名2
#define FTP_ANS_532        532     //Need account for storing files. 
#define FTP_ANS_553        553     //Requested action not taken. File name not allowed. 

/*
    for RMD 
    250 
    500, 501, 502, 421, 530, 550 
*/
#define FTP_ANS_RMD        250     //FTP删除目录

/*
    for LIST 
    125, 150 
    226, 250 
    425, 426, 451 
    450 
    500, 501, 502, 421, 530 
*/
#define FTP_ANS_LIST       150     //FTP列目录成功
#define FTP_ANS_426        426     //Connection closed; transfer aborted. 
#define FTP_ANS_451        451     //Requested action aborted. Local error in processing.  

#define FTP_ANS_TIMEOUT    -2      //Requested action aborted. Local error in processing.  


#define FTP_ANS_MKD        257     //FTP创建目录
#define FTP_ANS_CONNECT    220     //FTP控制套接字连接成功
#define FTP_ANS_USER       331     //FTP请求输入用户登陆口令
#define FTP_ANS_PASS       230     //FTP登陆成功
#define FTP_ANS_RETR       150     //FTP取文件命令执行成功
#define FTP_ANS_QUIT       221     //FTP注销
#define FTP_ANS_PORT       200     //FTP设置数据套接字端口
#define FTP_ANS_STOR       150     //FTP发送文件命令执行File status okay; about to open data connection. 
#define FTP_ANS_TRANS      226     //FTP文件发送完毕
#define FTP_ANS_PASV	   227     //ftp set passive mode
#define FTP_ANS_PASV_STOR  125     //FTP发送文件命令执行File status okay; about to open data connection. 

#define FTP_HOST_NAME_LEN  48
#define FTP_USER_NAME_LEN  32
#define FTP_PASSWORD_LEN   32

#define FTP_MAX_LINE_LEN   4096
#define FTP_RESPONSE_BUF   65536   //FTP命令响应侦最大长度
#define FTP_COMMAND_LEN    1024    //FTP发送命令侦最大长度

#define FTP_READ_FILE_BUF  1024    //发送、读取FTP数据时，一次发送、读取得的大长度

#define FTP_CTRL_PORT      21      //FTP控制套接口侦听端口

enum FTP_TRANSFER_MODE
{
    FTP_TRANS_ASCII  = 0,            //按文本方式传送(FTP默认方式)
    FTP_TRANS_BINARY = 1             //按二进制方式传输
};

enum FTP_ACTION
{
    FTP_ACTION_NONE  = 0,
    FTP_ACTION_PUT   = 1,
    FTP_ACTION_GET   = 2
};

enum FTP_FILE_ATTRIBUTE
{
    FTP_FILE_ATTRIBUTE_DIRECTORY = 0,    //目录文件
    FTP_FILE_ATTRIBUTE_LINK      = 1,    //链接文件
    FTP_FILE_ATTRIBUTE_NORMAL    = 2,    //普通文件
    FTP_FILE_ATTRIBUTE_OTHER     = 3     //其它类型的文件
};

struct SFtpFile
{
    char    szFileName[255];   //文件名
    char    szDate[10];        //文件创建日期
    unsigned char    nAttr;    //文件属性（目录文件、链接文件、普通文件等）
    unsigned long   nFileSize;            //文件大小 add by chenliangwei

};

typedef LIST<SFtpFile> FTP_FILELIST;

#endif  //_FTP_COMMAND_H_
