#ifndef __COLLECT_STRUCT_H__
#define __COLLECT_STRUCT_H__


//公用数据结构定义

/*
结构类型SCreateCollect用于创建不同的采集对象，然后采集对象
再创建不同的文件传输协议对象，定义如下：
*/
struct SCreateCollect
{
    int  nSwitchIndex;
    char szSwitchName[40];  //交换机名
    char szCollectId[40];   //采集对象标识串,"5ESS"表示创建C5ESSCollect对象
    char szProtocolId[40];  /*文件协议标识串,"Vertel"表示采用Vertel的FTAM对象*/

};

//数据结构定义
//结构类型SColFileReq用于定义采集单个文件时的输入参数，定义如下：
struct SColFileReq
{
    char  szSrcName[MAX_PATH];  //源文件名
    char  szDstName[MAX_PATH];  //目的文件名
    int   nFileSize;            //文件大小，用于控制采集文件时的最大等待时间(5ESS要用到)
};

//数据结构定义

/*
结构类型SGetFileReq ，是文件拷贝成员函数的输入数据，定义如下
*/
struct SGetFileReq
{
    char   szSrcPath[256];  //源文件路径
    char   szSrcName[100];  //源文件名
    char   szDstPath[256];  //目的文件路径
    char   szDstName[100];  //目的文件名
    int    nFileSize;       //文件大小，可以用来控制等待时间(optional)
};

enum E_COLLECT_STATUS
{
    CS_NOT_COLLECTED        = 0,
    CS_COLLECTING           = 1,
    CS_SETTING_COLLECTED    = 2,
    CS_COLLECTED            = 3,
};

#define S_CS_NOT_COLLECTED      "未采集"
#define S_CS_COLLECTING         "正在采集"
#define S_CS_COLLECTED          "已采集"

// 文件信息数据结构定义
struct  SFileInfo
{
    char            szFileName[MAX_PATH]; //文件名
    char            szModTime[10];        //修改时间
    int             nAttr;                //文件属性：ATTR_DIR, ATTR_FILE
    unsigned long   nFileSize;            //文件大小
    int             nCollectStatus;       //文件采集状态，参见E_COLLECT_STATUS
};

/*
类型SListOut, 获得文件服务器可用的文件列表的函数的
输出数据类型,定义如下:
*/
struct SListOut
{
    VECTOR <SFileInfo> FileList;  //返回的文件信息列表
};

//SS1240TPInfo保存每个S1240交换机TP的基本信息
struct  SS1240TPInfo
{
    char  szNename[S1240_CFG_LEN];             //Nename
    char  szTPID[S1240_CFG_LEN];               //TP名
    char  szBilltype[S1240_CFG_LEN];           //话单类型
    BOOL  bDumped;                             //采集的文件是否是DUMPED
    int   nFinalStatus;                        //目标状态(-1:null|dumped|release)
    char  szLocalPath[MAX_PATH];               //本地根目录
};

struct SSwitchBaseInfo
{
    char  szRemoteAddr[S1240_CFG_LEN];         //交换机地址
    char  szRemoteUsr[S1240_CFG_LEN];          //交换机用户名
    char  szRemotePwd[S1240_CFG_LEN];          //交换机密码

    char  szLocalAddr[S1240_CFG_LEN];          //本地地址
    char  szLocalUsr[S1240_CFG_LEN];           //本地用户名
    char  szLocalPwd[S1240_CFG_LEN];           //本地密码
};

struct SS1240SwitchInfo
{
    char  szSwitchName[S1240_CFG_LEN];
    char  szOfficeNum[S1240_CFG_LEN];
    char  szSwitchCode[S1240_CFG_LEN];
    char  szRemotePath[MAX_PATH];              //TP根目录
    char  szLocalPath[MAX_PATH];               //本地根目录
    char  nActivedTP;                          //主用的链路0-primary;1-backup

    SSwitchBaseInfo         primary;
    SSwitchBaseInfo         backup;
    MAP <int, SS1240TPInfo> S1240TPMap;
};

struct SS1240FilePoolMsg
{
    char szFilePoolId[255];
    int  nFileCount;
    int  nFileFullCount;
    int  nAlarmLimit;
};

struct SS1240FileMsg
{
    char          szFileId[255];
    int           nFileStatus;
    int           nFileMode;
    char          szFileIdDup[255];
    int           nFileCounter;
    int           nFileLocalDumped;
    unsigned long uFileSize;
};

struct SLinkStatusMsg
{
    int  nStatus;
    int  nAlarmPara;
    char szLocalAddr[50];
    char szRemoteAddr[50];
};

#endif //__COLLECT_STRUCT_H__
