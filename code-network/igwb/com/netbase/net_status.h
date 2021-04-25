#ifndef  __NET_STATUS_H__
#define  __NET_STATUS_H__

#include "../utility/config.h"
#include "../include/frame.h"

//协议栈类型
enum STACK_TYPE
{
    STACK_SLIDE     = 0,
    STACK_GTP       = 1,
    STACK_SCAN      = 2,
    STACK_WLAN_AS   = 3
};


//网络模块滑动窗口协议的状态结构
typedef struct _SlideWndModInfo
{
    //模块ID，主机一个SM模块对应一个Module
    unsigned char yModule;
    struct
    {
        //当前滑动窗口头序列号
        unsigned long uBaseSeqNum;

        //已提交数据记录数
        unsigned long uSubmitRecCnt;
    }SSm;
}SSlideWndModInfo;
#define SLIDE_MOD_INFO_LENGTH sizeof(SSlideWndModInfo)


//网络模块GTP'协议的状态结构
typedef struct _GtpNodeInfo
{
    unsigned long uStackID;             //Stack ID
    unsigned long uAppType;             //协议类型,取值于APP_TYPE
    unsigned long nActivedGsnIP;        //该GSN当前活动的IP地址
    unsigned long uGsnIp[GSN_IP_TABLE_IPCOUNT_MAX];           //给这个GSN预留10个IP地址
    unsigned long uGsnPort;             //GSN的端口号
    unsigned long uLocalIp;             //本地IP地址,只保留最近用的那个
    unsigned long uLocalPort;           //本地端口号,只保留最近用的那个
    unsigned long uGsnRestartCount;     //GSN的重启次数
    unsigned long uFirstSNFile;         //第一个有效的SNFILE的序列号
    unsigned long uPIDAtGSNRestart;     //GSN上次重启时的PID号
    unsigned long uGtpVersion;          //当前GSN的版本号
    unsigned long uReserved[50];        //保留一部分空间，避免下次增加域时要删除原状态文件
}SGtpNodeInfo;
#define GTP_NODE_INFO_LENGTH sizeof(SGtpNodeInfo)


//滑动窗口协议的网络模块的状态结构
typedef struct _SlideWndStatusInfo
{
    //当前包ID
    unsigned long uPacketId;

    //是否等待前存盘应答已提交的帧数据
    BOOL bWaitForResponse;

    SSlideWndModInfo SlideModInfo[1];//不定长数组

}SSlideWndStatusInfo, *PSlideWndStatusInfo;


//GTP'协议的网络模块的状态结构
typedef struct _GtpStatusInfo
{
    //当前包ID
    unsigned long uPacketId;

    //是否等待前存盘应答已提交的帧数据
    BOOL bWaitForResponse;

    SGtpNodeInfo GtpNodeInfo[1];//不定长数组

}SGtpStatusInfo;


#define SF_LOAD         1
#define SF_SAVE         2

#define INVALID_STACK_ID    0xFFFFFFFF
//抽象了网络模块的主状态文件类
class CNetStatusInfo
{
public:
    CNetStatusInfo(UINT4 const uBufferLen);
    virtual ~CNetStatusInfo();

    typedef LIST<CNetStatusInfo*> LIST_PNETSTATUSINFO;

    //初始化主状态文件
    static int Init(const char* const szStatusFileName);

    //设置状态信息
    void Set(const BYTE* const pBuf, UINT4 const uLen);

    //读取状态信息
    UINT4 Get(BYTE* const pBuf, UINT4 const uLen, 
              BOOL const bReadFile = TRUE);

    //刷新本对象包含的状态数据到主状态文件中
    virtual int Flush(void);

    //从主状态文件中获得第nNum个协议栈的编号
    static UINT4 EnumStackId(int const nNum, 
                             int const nStackInfoType);

    //刷新所有状态数据到主状态文件中
    static int FlushAll(void);

    //获取内存中所有状态信息数据
    static int GetAllStatusData(BYTE* &pBuffer, int nBufLen);

    //获取内存中所有状态信息数据的长度
    static int GetAllStatusDataLen();

protected:

    //将状态信息缓存区中数据从主状态文件的当前位置写入主状态文
    //件m_StatusFile中
    virtual int WriteFile(void);

    //从主状态文件m_StatusFile读取状态信息，不同CNetStatusInfo
    //读写状态文件获得状态信息的机制是不同的，只有各派生类知道
    //如何实现，所以该函数为空的虚函数，需要CNetStatusInfo子类
    //重载该函数
    virtual int ReadFile(void);

    //所有的CNetStatusInfo子类对象都读写同一个主状态文件
    //该结构指针在初始化函数中创建，在析构函数中释放
    static FILE* m_pStatusFile;

    //由CNetWorkStatusInfo在构造函数中将自身对象指针赋给
    //m_pNetWorkStatusInfo变量，其它地方不会对该变量赋值
    static CNetStatusInfo* m_pNetworkStatusInfo;

    //CNetStatusInfo对象的指针列表
    static LIST_PNETSTATUSINFO m_StackStatusInfoList;

    //所有缓冲区的累计总长度
    static UINT4 m_uTotalBufLen;

    //状态信息缓冲区,该缓冲区在构造函数中分配，在析构函数中释放
    unsigned char* m_pBuffer;

    //状态信息缓冲区的长度
    UINT4 m_uBufferLen;

    //状态文件的文件名
    static char m_szFileName[MAX_PATH];
};

typedef struct _NetWorkStatusInfo
{
    //当前包ID
    unsigned long uPacketId;

    //是否等待前存盘应答已提交的帧数据
    BOOL bWaitForResponse;

}SNetWorkStatusInfo;
#define NET_STATUS_INFO_HEADLENGTH sizeof(SNetWorkStatusInfo)


//CNetWorkStatusInfo抽象了对网络模块主状态文件头的读写操作
class CNetWorkStatusInfo:public CNetStatusInfo
{
public:
    CNetWorkStatusInfo(UINT4 const uBufferLen);
    virtual ~CNetWorkStatusInfo();

    //刷新本对象包含的状态数据到主状态文件中
    virtual int Flush(void);

protected:

    //从主状态文件m_pStatusFile读取文件头信息
    virtual int ReadFile(void);

};

//CStackStatusInfo抽象了对协议栈状态信息的读写
class CStackStatusInfo:public CNetStatusInfo
{
public:
    CStackStatusInfo(int const nStackStatusInfoType,
                     UINT4 uStackId);
    virtual ~CStackStatusInfo();


    //刷新本对象包含的状态数据到主状态文件中
    virtual int Flush(void);

protected:

    //从主状态文件m_pStatusFile读取本对象对应的协议栈信息
    virtual int ReadFile(void);

    //协议栈类型
    int m_nStackStatusInfoType;

    //协议栈编号
    UINT4 m_uStackId;
};

struct SScanStackInfo
{
    int  nStackID;
    char szPathName[MAX_PATH];
    int  nSavedOffset;
    int  nSavingOffset;
    char szFileName[MAX_PATH];
};
#define SCAN_STACK_INFO_LEN     sizeof(SScanStackInfo)

//文件扫描协议的网络模块的状态结构
struct SScanStatusInfo
{
    //当前包ID
    unsigned long uPacketId;

    //是否等待前存盘应答已提交的帧数据
    BOOL bWaitForResponse;

    SScanStackInfo ScanNodeInfo[1];//不定长数组

};
#define SCAN_STATUS_INFO_LEN     sizeof(SScanStatusInfo)


#endif  //__NET_STATUS_H__
