#ifndef __NET_STRUCT_H_
#define __NET_STRUCT_H_

#include "../include/base_type.h"
#include "../include/toolbox.h"

typedef struct _NetFrame
{
    BYTE SenderPid;         //发送模块所在的进程号
    BYTE SenderMid;         //发送模块的模块号
    BYTE ReceiverPid;       //接收模块所在的进程号
    BYTE ReceiverMid;       //接收模块的模块号
    UINT2 AppType;          //应用类型
    UINT2 ClientNo;         //某种类型的MML客户端的编号
    struct _INetAddrPair
    {
        UINT4 ServerIp;
        UINT4 ServerPort;
        UINT4 ClientIp;
        UINT4 ClientPort;
    }SNetAddrPair;
    BYTE unused[32];
    UINT4 IPAddr;           //MML客户端的IP地址
    UINT2 CmdCode;          //命令码
    UINT4 Len;              //Data域的长度
    BYTE Data[1];           //用于存放各种参数
}SNetFrame, *PNetFrame;


//颜文远2002.04.08添加
/*
    与滑动窗口协议相关结构的定义！所有的消息必须是1字节对齐的INTEL字节序！
    这里用一个宏来定义所有需要序列化的数据成员，每个数据单元包括
    一个私有的成员变量，一个get函数及一个set函数。在get/set函数
    中提供序列化及字节序的转换，保证整个结构为INTEL字节序的字节流，
    在定义一个数据单元时，输入参数如下：
        ietype ——表示成员名
        ntype  ——表示成员的数据类型
        NorSorL——是一个枚举，N表示不进行字节序的转换
                               S表示进行两个字节的字节序转换
                               L表示进行四个字节的字节序转换
*/
#define ITOHN(a)        (a)
#define HTOIN(a)        (a)
#define INTEL_MEMBER_ITEM(item, ntype, NorSorL) \
private: \
    BYTE byte_##item[sizeof(ntype)]; \
public: \
    inline void item(ntype data) \
    {  \
        data = HTOI##NorSorL(data); \
        memcpy(&byte_##item[0], (void *)&data, sizeof(ntype)); \
    }; \
    inline ntype item() \
    { \
        ntype tmpData; \
        memcpy(&tmpData, &byte_##item[0], sizeof(ntype)); \
        tmpData = ITOH##NorSorL(tmpData); \
        return tmpData; \
    };

#define NTOHN(a)        (a)
#define HTONN(a)        (a)
#define MEMBER_ITEM(ietype, ntype, NorSorL) \
private: \
    BYTE byte_##ietype[sizeof(ntype)]; \
public: \
    inline void ietype(ntype data) \
    {  \
        data = HTON##NorSorL(data); \
        memcpy(&byte_##ietype[0], (void *)&data, sizeof(ntype)); \
    }; \
    inline ntype ietype() \
    { \
        ntype tmpData; \
        memcpy(&tmpData, &byte_##ietype[0], sizeof(ntype)); \
        tmpData = NTOH##NorSorL(tmpData); \
        return tmpData; \
    };
//
//以下为在GSM/128/CDMA等产品中用到的通信帧结构
#define _MEMFRAME_HEAD_LENGTH 4
typedef struct _MEMFrame
{
    INTEL_MEMBER_ITEM(nCmd, UINT2, S)   //命令字
    INTEL_MEMBER_ITEM(nLen, UINT2, S)   //数据区长度
    BYTE* Data()
    {
        return (BYTE* )this + _MEMFRAME_HEAD_LENGTH;
    }
    //重载的new操作符，nSize为Data域的长度
    void* operator new(size_t classSize, int nSize)
    {
        UINT4 uLen = nSize + _MEMFRAME_HEAD_LENGTH;
        void* ptr = new BYTE[uLen];
        if (NULL != ptr)
        {
            memset(ptr, 0, uLen);
            ((_MEMFrame* )ptr)->nLen(nSize);
        }

        return ptr;
    }
    UINT4 size()
    {
        return _MEMFRAME_HEAD_LENGTH + nLen();
    }
}SMemFrame;

//SMemFrame中的命令码
enum MemCmd
{
    MEM_CMD_DATA      = 0,  //数据帧
    MEM_CMD_HANDSHAKE = 2  //握手帧
};

#define _MEM_HANDSHAKE_LENGTH   5
typedef struct _MEMHandShakeData
{
    INTEL_MEMBER_ITEM(nNotUsed1, UINT2, S)  //保留字节
    INTEL_MEMBER_ITEM(nNotUsed2, BYTE, N)   //保留字节
    INTEL_MEMBER_ITEM(nModu, BYTE, N)       //模块号
    INTEL_MEMBER_ITEM(nNotUsed3, BYTE, N)   //保留字节
}SMemHandShakeData, *PMemHandShakeData;

#define _APPFRAME_HEAD_LENGTH   6
#define _APPFRAME_EXPAND_LENGTH 7
#define _APPFRAME_EXHEAD_LENGTH (_APPFRAME_HEAD_LENGTH + _APPFRAME_EXPAND_LENGTH)
typedef struct _AppFrame
{
    INTEL_MEMBER_ITEM(sender_mid, BYTE, N)      //发送者模块号
    INTEL_MEMBER_ITEM(sender_pid, BYTE, N)      //发送者PID
    INTEL_MEMBER_ITEM(receiver_mid, BYTE, N)    //接收者模块号
    INTEL_MEMBER_ITEM(receiver_pid, BYTE, N)    //接收者PID
    INTEL_MEMBER_ITEM(nLen, UINT2, S)           //长度

    INTEL_MEMBER_ITEM(byNotUsedNow1, BYTE, N)   //保留字节，填零
    INTEL_MEMBER_ITEM(sender_desk, BYTE, N)     //发送台号
    INTEL_MEMBER_ITEM(sender_app, BYTE, N)      //发送App号
    INTEL_MEMBER_ITEM(receiver_desk, BYTE, N)   //接收台号
    INTEL_MEMBER_ITEM(receiver_app, BYTE, N)    //接收App号
    INTEL_MEMBER_ITEM(byCmd, BYTE, N)           //命令码
    INTEL_MEMBER_ITEM(byNotUsedNow2, BYTE, N)   //保留字节，填零
    BYTE* Data()
    {
        return (BYTE* )this + _APPFRAME_EXHEAD_LENGTH;
    }
    //重载的new操作符，nSize为Data域的长度
    void* operator new(size_t classSize, int nSize)
    {
        UINT4 uLen = nSize + _APPFRAME_EXHEAD_LENGTH;
        void* ptr = new BYTE[uLen];
        if (NULL != ptr)
        {
            memset(ptr, 0, uLen);
            ((_AppFrame* )ptr)->nLen(nSize + _APPFRAME_EXPAND_LENGTH);
        }

        return ptr;
    }
    UINT4 size()
    {
        return _APPFRAME_HEAD_LENGTH + nLen();
    }
}SAppFrame, *PAppFrame;

//SAppFrame中的命令码
enum AppCmd
{
    APP_CMD_REQ_SYN     = 0x88,    //话单同步命令
    APP_CMD_REQ_DATA    = 0x89,    //话单数据命令
    APP_CMD_REQ_ACKNUM  = 0x8a,    //应答帧号同步命令

    APP_CMD_RSP_ACK     = 0x08,    //应答命令
    APP_CMD_RSP_REJ     = 0x09,    //拒绝命令
    APP_CMD_RSP_ACKNUM  = 0x0a    //应答帧号同步应答
};

//SAppFrame中的PID
enum AppPid
{
    APP_PID_HTMAIL = 0x1B,
    APP_PID_BILL = 0x20,
    APP_PID_ALARM = 0x25,
    APP_PID_MEMMAIL = 0x35,
    APP_PID_BAU     = 69
};

//SAppFrame中的MID
enum AppMid
{
    APP_MID_NULL = 0xFE,    //空模块号
    APP_MID_BAM = 0
};

//SAppFrame中的台号
enum AppDesk
{
    APP_DESK_NULL = 0xFE   //空台号
};

//SAppFrame中的App号
enum AppApp
{
    APP_APP_NULL = 0x7E   //空的App号
};


//
//以下为在SoftX3000产品中用到的通信帧结构
#define _SYSFRAME_HEAD_LENGTH   20
#define _SYSFRAME_EXPAND_LENGTH 7
#define _SYSFRAME_EXHEAD_LENGTH (_SYSFRAME_HEAD_LENGTH + _SYSFRAME_EXPAND_LENGTH)
typedef struct _SysFrame
{
    INTEL_MEMBER_ITEM(sender_mid, UINT4, L)     //发送模块号
    INTEL_MEMBER_ITEM(sender_pid, UINT4, L)     //发送PID
    INTEL_MEMBER_ITEM(receiver_mid, UINT4, L)   //接收模块号
    INTEL_MEMBER_ITEM(receiver_pid, UINT4, L)   //接收PID
    INTEL_MEMBER_ITEM(nLen, UINT4, L)           //长度

    INTEL_MEMBER_ITEM(ucChannelNo, BYTE, N)
    INTEL_MEMBER_ITEM(ulBamTaskID, UINT4, L)
    INTEL_MEMBER_ITEM(ucCmdCode, BYTE, N)
    INTEL_MEMBER_ITEM(ucHandle, BYTE, N)

    BYTE* Data()
    {
        return (BYTE* )this + _SYSFRAME_EXHEAD_LENGTH;
    }

    //重载的new操作符，nSize为Data域的长度
    void* operator new(size_t classSize, int nSize = 0)
    {
        if(nSize < 0) nSize = 0;

        UINT4 uLen = nSize + _SYSFRAME_EXHEAD_LENGTH;
        void* ptr = new BYTE[uLen];
        if (NULL != ptr)
        {
            memset(ptr, 0, uLen);
            ((_SysFrame* )ptr)->nLen(nSize + _SYSFRAME_EXPAND_LENGTH);
        }

        return ptr;
    }
    UINT4 size()
    {
        return _SYSFRAME_HEAD_LENGTH + nLen();
    }
}SSysFrame, *PSysFrame;

//SSysFrame中的命令码
enum SysCmd
{
    //以下的命令码为与SAppFrame中一致的命令码
    SYS_CMD_REQ_SYN     = 0x88,     //话单同步命令
    SYS_CMD_REQ_DATA    = 0x89,     //话单数据命令
    SYS_CMD_REQ_ACKNUM  = 0x8a,     //应答帧号同步命令

    SYS_CMD_RSP_ACK     = 0x08,     //应答命令
    SYS_CMD_RSP_REJ     = 0x09,     //拒绝命令
    SYS_CMD_RSP_ACKNUM  = 0x0a,     //应答帧号同步应答

    //以下的命令码为SSysFrame中特有的命令码
    //modified by wangzhaoyang 2002/07/08
    SYS_CMD_SYS_HAND_SHAKE = 2,     //SYS板发往BAU的握手命令
    SYS_CMD_BAU_HAND_SHAKE = 2,     //BAU发往SYS板的握手命令
    SYS_CMD_HAND_SHAKE_ACK = 3,     //握手相应命令，暂时不用
    //end 2002/07/08
    SYS_CMD_ALARM          = 0xFB  //BAU发送告警的命令
};

//SSysFrame中的PID
enum SysPid
{
    SYS_PID_HTCOMM = 51,    //modified by wangzhaoyang 2002/07/08，原为1，联调后修改
    SYS_PID_ALARM  = 79,    //modified by wangzhaoyang 2002/07/08，原为37，联调后修改
    SYS_PID_BAU    = 69
};

//SSysFrame中的MID
enum SysMid
{
    SYS_MID_BAM = 0,    //BAM的模块号
    SYS_MID_BAU = 1    //BAU的模块号
};


//
//以下为在滑动窗口协议中用到的结构
//同步应答帧号消息的内容
typedef struct _SyncAckNum
{
    INTEL_MEMBER_ITEM(nAckNum, UINT4, L)
}SSyncAckNum;

//同步话单流水号消息的内容
typedef struct _SyncSequ
{
    INTEL_MEMBER_ITEM(nSequ, INT4, L)
}SSyncSequ;

//单张话单数据消息的结构
#define _BILLMSG_HEAD_LENGTH 4
typedef struct _BillMessage
{
    INTEL_MEMBER_ITEM(nSequ, INT4, L)
    BYTE* Data()
    {
        return (BYTE* )this + _BILLMSG_HEAD_LENGTH;
    }
}SBillMessage;

//BAU向主机应答消息的内容
typedef struct _AckOrRejRes
{
    INTEL_MEMBER_ITEM(nSequ, INT4, L)
    INTEL_MEMBER_ITEM(nAckNum, INT4, L)
}SAckOrRejRes;


//以下为告警消息中的结构

//在GSM、128等产品中用到的SMemFrame结构中的告警息结构，放在SMemFrame的Data域中
#define _MEM_ALARM_INFO_HEAD_LENGTH     6
#define _MEM_ALARM_INFO_EXPAND_LENGTH   4
#define _MEM_ALARM_INFO_EXHEAD_LENGTH   (_MEM_ALARM_INFO_HEAD_LENGTH + _MEM_ALARM_INFO_EXPAND_LENGTH)
struct SMemAlarmInfo
{
    INTEL_MEMBER_ITEM(sender_mid, BYTE, N)      //发送者模块号
    INTEL_MEMBER_ITEM(sender_pid, BYTE, N)      //发送者进程号
    INTEL_MEMBER_ITEM(receiver_mid, BYTE, N)    //接收者模块号
    INTEL_MEMBER_ITEM(receiver_pid, BYTE, N)    //接收者进程号
    INTEL_MEMBER_ITEM(nLen, UINT2, S)           //域长度
    INTEL_MEMBER_ITEM(byBauNo, BYTE, N)         //发出告警的服务器号
    INTEL_MEMBER_ITEM(uAlarmID, UINT2, S)       //服务器告警ID
    INTEL_MEMBER_ITEM(byAllRestore, BYTE, N)    //是否全部恢复标记,
                                                //0:全部，1：部分
    BYTE* Paras()   //告警参数
    {
        return (BYTE* )this + _MEM_ALARM_INFO_EXHEAD_LENGTH;
    }
};

//在SoftX3000中用到的告警消息中的结构，放在SSysFrame的Data域中
#define _SYS_ALARM_INFO_LENGTH  (36 + MAX_ALARM_PARA)   //modify by lx SWPD04504
struct SSysAlarmInfo
{
    MEMBER_ITEM(ulAlarmSN, UINT4, L)        //告警序列号，不断累加，后台可以
                                            //检查这个字段看是否有丢失告警
    MEMBER_ITEM(ulAlarmID, UINT4, L)        //告警ID
    MEMBER_ITEM(ulCauseID, UINT4, L)        //告警原因ID
    MEMBER_ITEM(ucAlarmType, BYTE, N)       //告警类型
    MEMBER_ITEM(ucByNMSType, BYTE, N)       //Send to NMS or not
    MEMBER_ITEM(ucAlarmBoxType, BYTE, N)    //告警箱分类 Function ID
    MEMBER_ITEM(ucAlarmLevel, BYTE, N)      //告警级别
    MEMBER_ITEM(ucSubSystemId, BYTE, N)     //网管告警分类
    MEMBER_ITEM(uwYear, UINT2, S)           //下面为告警时间
    MEMBER_ITEM(ucMonth, BYTE, N)
    MEMBER_ITEM(ucDate, BYTE, N)
    MEMBER_ITEM(ucHour, BYTE, N)
    MEMBER_ITEM(ucMinute, BYTE, N)
    MEMBER_ITEM(ucSecond, BYTE, N)
    MEMBER_ITEM(ulCpuID, UINT4, L)          //告警模块号
    MEMBER_ITEM(ulAlarmPID, UINT4, L)       //告警PID
    MEMBER_ITEM(uwReserve, UINT2, S)        //保留字段
    MEMBER_ITEM(uwParaLength, UINT2, S)     //告警参数长度
    BYTE  ucPara[MAX_ALARM_PARA];           //告警参数
};

//颜文远2002.04.08添加结束

//增加对告警箱的操作
enum AlarmCmd
{
  ALARM_CMD_BAU_TO_CONVERT = 0xAA,
  ALARM_CMD_CONVERT_TO_BAU = 0xAA
};

//BAU与Convert之间的通信帧结构。
#define _APPCVTFRM_HEAD_LENGTH  4
struct SAppConvertFrame
{
    BYTE byCmd;     //命令码，iGateway Bill网络通信层发送到CONVERT固定为0xAA
                    //CONVERT发送到OM端固定为0xAA
    BYTE byError;   //错误码，iGateway Bill网络通信层发送到CONVERT固定为0x00
                    //对于CONVERT来说，把告警箱的串口信息发送给iGateway Bill网
                    //络通信层时，固定为0x00,当CONVERT接收到错误的信息，回应
                    //给iGateway Bill网络通信层错误码为0x01
    UINT2 wLen;     //发送的数据区的数据长度，告警转发程序根据这一信息，与收到
                    //的数据长度比较，若不一致，说明接收的信息有误。
    BYTE* Data()    //数据区
    {
        return (BYTE*)this + _APPCVTFRM_HEAD_LENGTH;
    }

    void* operator new(size_t classSize, size_t DataLength = 0)
    {
        void *ptr = NULL;
        UINT4 uLen = (DataLength > 0) ? DataLength : 0;

        UINT4 length = uLen + _APPCVTFRM_HEAD_LENGTH;
        if (length > 65535)
        {
            return NULL;
        }

        ptr = ::new char[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SAppConvertFrame*)ptr)->wLen = htons(uLen);
        }

        return ptr;
    };
};


// GTP协议头中使用到的常数
#define GTP_V0_TAG1     0xE         //第1字节标志 GTP' V0
#define GTP_V06_TAG1    0xF         //第1字节标志 GTP' V0,帧头为6个字节
#define GTP_V1_TAG1     0x2E        //第1字节标志 GTP' V1
#define GTP_V16_TAG1    0x2F        //第1字节标志 GTP' V1,帧头为6个字节
#define GTP_V2_TAG1     0x4E        //第1字节标志 GTP' V2

#define GTP_FRAME_HEAD_LENGTH_V1        20
#define GTP_FRAME_HEAD_LENGTH_V2        6
#define GTP_FRAME_HEAD_LENGTH_V1_EXTRA  14

/*
    _GtpFrame帧结构说明
    该帧结构支持GTP'V0,V1及V2版本，在创建帧实例时，
    首先根据协议帧的版本号分配内存空间，然后在第一
    时间设置yTag版本标志，将版本信息写入帧头中，在
    此操作之后，Data，GetHeadSize及GetSize等函数才
    能使用;
*/
typedef struct _GtpFrame
{
    //标志，包含GTP'版本信息，该域的取值恒为0x4E
    MEMBER_ITEM(yTag,    unsigned char,  N)

    //消息类型
    MEMBER_ITEM(yType,   unsigned char,  N)

    //数据区的长度，即Data域的字节数；
    MEMBER_ITEM(uLen,    unsigned short, S)

    //序列号
    MEMBER_ITEM(uSeqNum, unsigned short, S)

    //取数据区地址，考虑了字节对齐问题
    inline BYTE* Data()
    {
        return (BYTE*)this + GetHeadLen();
    }

    //根据版本号获取协议帧头长,缺省为6字节
    inline static UINT2 GetHeadLen(UINT4 uGtpVersion)
    {
        if((uGtpVersion == GTP_V0_TAG1) || (uGtpVersion == GTP_V1_TAG1))
        {
            return GTP_FRAME_HEAD_LENGTH_V1;
        }

        return GTP_FRAME_HEAD_LENGTH_V2;
    }

    //取帧头长
    inline UINT2 GetHeadLen()
    {
        return GetHeadLen(yTag());
    }

    //取帧的全长
    inline UINT2 GetSize()
    {
        return GetHeadLen() + uLen();
    }

    //验证版本号
    inline bool CheckGTPFrame()
    {
        switch(yTag())
        {
        case GTP_V0_TAG1:
        case GTP_V06_TAG1:
        case GTP_V1_TAG1:
        case GTP_V16_TAG1:
        case GTP_V2_TAG1:
            return true;

        default:
            break;
        }
        return false;
    }

    //填写V0及V1中20个字节头长的扩充部分
    inline void FillV0HeadEx()
    {
        if((yTag() == GTP_V0_TAG1) || (yTag() == GTP_V1_TAG1))
        {
            memset((BYTE*)this + GTP_FRAME_HEAD_LENGTH_V2, 0, 
                   GTP_FRAME_HEAD_LENGTH_V1 - GTP_FRAME_HEAD_LENGTH_V2);

            memset((BYTE*)this + 9, 0xFF, 3);
        }
    }

}SGtpFrame, *PGtpFrame;

#define IETV_HEAD_LENGTH      1
#define IETLV_HEAD_LENGTH     3

/*
  CHECK_TV检查一个TV结构。
    pframe ——包含ietype结构的SGtpFrame的指针
    ielen  ——为ietype结构的长度
    pie    ——为一指向ietype的指针
    noffset——指明TV位于pframe数据段中的偏移量，检查内容包括:

   1. 检查由pframe指向的SGtpFrame长度是否足够；
   2. 相应TV结构的类型码是否相符
*/
//2002-11-23 李杜芳 对应问题单 D19425
template<class Tframe, class Tietype>
inline int CHECK_TV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset);

//CHECK_TLV检查一个TLV结构。该宏首先进行TV结构检查。然后检
//查帧长度是否足够。
template<class Tframe, class Tietype>
inline int CHECK_TLV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset);

//有关cause的定义
#define CAUSE_SYSTEM_FAILURE        59  //系统错误/System failure
#define CAUSE_TRANS_BUF_FULL        60  //传输缓冲区满/Send buffers becoming full
#define CAUSE_RECEIVE_BUF_FULL      61  //接收缓冲区满/Receive buffers becoming full
#define CAUSE_ANOTHER_GODOWN        62  //另一个结点将关闭/Another node is about to go down
#define CAUSE_OWN_GODOWN            63  //本结点将关闭/This node is about to go down
#define CAUSE_REQUEST_ACCEPTED      128 //请求接受/Request accepted
#define CAUSE_CDR_DECODE_ERR        177 //CDR decoding error
#define CAUSE_INVALID_MSG_FMT       193 //Invalid message format
#define CAUSE_VER_NOT_SUP           198 //Version not supported
#define CAUSE_NO_RES_AVAILABLE      199 //No resources available
#define CAUSE_SERVICE_NOT_SUP       200 //Service not supported
#define CAUSE_MAND_IE_INCRT         201 //Mandatory IE incorrect
#define CAUSE_MAND_IE_MISSING       202 //Mandatory IE missing
#define CAUSE_OPT_IE_INCRT          203 //Optional IE incorrect
#define CAUSE_SYSTEM_FAILURE1       204 //System failure
#define CAUSE_REQUEST_DUPFULFILLED  252 //Request related to possibly duplicated packet already fulfilled
#define CAUSE_REQUEST_FULFILLED     253 //Request already fulfilled
#define CAUSE_REQ_NOT_FULFILL       255 //Request not fulfilled

//casue结构
#define TIECAUSE_LENGTH             2
struct TIECause 
{   
    enum { type = 1 };
    MEMBER_ITEM(yType,      BYTE,  N)   //消息类型
    MEMBER_ITEM(yCause,     BYTE,  N)   //原因值
};

//recovery结构
#define TIERECOVERY_LENGTH          2
struct TIERecovery 
{   
    enum { type = 14 };
    MEMBER_ITEM(yType,        BYTE,N)   //消息类型
    MEMBER_ITEM(yRestartCount,BYTE,N)   //重启计数
};

//transfer command常数
#define TC_SENDDATA                 1   //发送数据
#define TC_SENDDATAMAYBEDUP         2   //发送可能重复数据
#define TC_CANCELDATA               3   //取消数据
#define TC_RELEASEDATA              4   //释放数据

//transfer command结构
#define TIETRANSFER_CMD_LENGTH      2
struct TIETransferCommand 
{   
    enum { type = 126 };
    MEMBER_ITEM(yType,      BYTE,  N)   //消息类型
    MEMBER_ITEM(yCmd,       BYTE,  N)   //命令码
};

//data record package结构
#define TIEBILL_BACKAGE_HEAD_LENGTH 7
struct TIEBillPackage 
{   
    enum { type = 252 };
    MEMBER_ITEM(yType,      BYTE,  N)   //消息类型
    MEMBER_ITEM(uLen,       UINT2, S)   //数据区长度
    MEMBER_ITEM(yCount,     BYTE,  N)   //记录数
    MEMBER_ITEM(yFormat,    BYTE,  N)   //编码格式
    MEMBER_ITEM(uVersion,   UINT2, S)   //格式版本

    //取数据区地址，考虑了字节对齐问题
    inline BYTE*  Data()
    {
        return (BYTE*)this + TIEBILL_BACKAGE_HEAD_LENGTH;
    }
};

//计算data record package的数据段长度
//该宏使用TIEBillPackage作为参数
/*#define DATASIZE_OF_IEBILLPACKAGE(pPackage) \
    (int)(pPackage->uLen() + IETLV_HEAD_LENGTH \
     - TIEBILL_BACKAGE_HEAD_LENGTH)

//该宏使用SGtpFrame作为参数
#define DATASIZE_OF_BILLPACKAGE(pframe) \
    (((pframe->yTag() == GTP_V0_TAG1) \
      || (pframe->yTag() == GTP_V06_TAG1)) \
     ? (DATASIZE_OF_IEBILLPACKAGE((TIEBillPackage *) pframe->Data())) \
     : (DATASIZE_OF_IEBILLPACKAGE(((TIEBillPackage *) \
        (pframe->Data() + TIETRANSFER_CMD_LENGTH)))))
        */
inline int DATASIZE_OF_IEBILLPACKAGE(TIEBillPackage* pPackage)
{
    return pPackage->uLen() + IETLV_HEAD_LENGTH - TIEBILL_BACKAGE_HEAD_LENGTH;
}

inline int DATASIZE_OF_BILLPACKAGE(SGtpFrame* pframe)
{
    if((pframe->yTag() == GTP_V0_TAG1) || (pframe->yTag() == GTP_V06_TAG1))
    {
        return DATASIZE_OF_IEBILLPACKAGE(((TIEBillPackage *) pframe->Data()));
    }
    else
    {
        return DATASIZE_OF_IEBILLPACKAGE(((TIEBillPackage *) 
                                  (pframe->Data() + TIETRANSFER_CMD_LENGTH)));
    }
}


//和node address有关的常数
#define LEN_NODEADDRESS             4   //IPV4地址长度

//node address结构
#define TIENODE_ADDRESS_LENGTH      7
struct TIENodeAddress 
{   
    enum { type = 251 };
    MEMBER_ITEM(yType,      BYTE,  N)   //消息类型
    MEMBER_ITEM(uLen ,      UINT2, S)   //数据区长度
    MEMBER_ITEM(uAddress,   UINT4, N)   //IPV4地址
};

//重定向时推荐的IP地址
#define TIERECMD_NODE_ADDR_LENGTH   7
struct TIERecommendNodeAddress 
{   
    enum { type = 254 };
    MEMBER_ITEM(yType,      BYTE,  N)   //消息类型
    MEMBER_ITEM(uLen ,      UINT2, S)   //数据区长度
    MEMBER_ITEM(uAddress,   UINT4, N)   //IPV4地址
};

//sequence canceled结构
#define TIESN_CANCELED_HEAD_LENGTH  3
struct TIESNCanceled 
{   
    enum { type = 250 };
    MEMBER_ITEM(yType,      BYTE,  N)   //消息类型
    MEMBER_ITEM(uLen,       UINT2, S)   //数据区长度

    //取序列号数组数据区地址，要考虑字节对齐问题
    inline BYTE* Data()
    {
        return (BYTE*)this + TIESN_CANCELED_HEAD_LENGTH;
    };

    inline UINT2 uSeqNums(int nIndex)
    {
        UINT2 uOutSeqNum;
        memcpy(&uOutSeqNum, Data() + nIndex * 2, sizeof(UINT2));
        uOutSeqNum = NTOHS(uOutSeqNum);
        return uOutSeqNum;
    };

    inline void uSeqNums(int nIndex, UINT2 uInSeqNum)
    {
        uInSeqNum = HTONS(uInSeqNum);
        memcpy(Data() + nIndex * 2, &uInSeqNum, sizeof(UINT2));
    };
};

//sequence released结构
#define TIESN_RELEASED_HEAD_LENGTH  3
struct TIESNReleased 
{   
    enum { type = 249 };
    MEMBER_ITEM(yType,      BYTE,  N)   //消息类型
    MEMBER_ITEM(uLen,       UINT2, S)   //数据区长度

    //取序列号数组数据区地址，要考虑字节对齐问题
    inline BYTE* Data()
    {
        return (BYTE*)this + TIESN_RELEASED_HEAD_LENGTH;
    };

    inline UINT2 uSeqNums(int nIndex)
    {
        UINT2 uOutSeqNum;
        memcpy(&uOutSeqNum, Data() + nIndex * 2, sizeof(UINT2));
        uOutSeqNum = NTOHS(uOutSeqNum);
        return uOutSeqNum;
    };

    inline void uSeqNums(int nIndex, UINT2 uInSeqNum)
    {
        uInSeqNum = HTONS(uInSeqNum);
        memcpy(Data() + nIndex * 2, &uInSeqNum, sizeof(UINT2));
    };
};

//Requests Responded
#define TIESN_RESPONSE_HEAD_LENGTH  3
struct TIESNResponse 
{   
    enum { type = 253 };
    MEMBER_ITEM(yType,      BYTE,  N)   //消息类型
    MEMBER_ITEM(uLen,       UINT2, S)   //数据区长度

    //取序列号数组数据区地址，要考虑字节对齐问题
    inline BYTE* Data()
    {
        return (BYTE*)this + TIESN_RESPONSE_HEAD_LENGTH;
    };

    inline UINT2 uSeqNums(int nIndex)
    {
        UINT2 uOutSeqNum;
        memcpy(&uOutSeqNum, Data() + nIndex * 2, sizeof(UINT2));
        uOutSeqNum = NTOHS(uOutSeqNum);
        return uOutSeqNum;
    };

    inline void uSeqNums(int nIndex, UINT2 uInSeqNum)
    {
        uInSeqNum = HTONS(uInSeqNum);
        memcpy(Data() + nIndex * 2, &uInSeqNum, sizeof(UINT2));
    };
};

#define PREPARED_ACKFRAME_SIZE  (1024 * 10)

//SGtpReqFrameInfo描述了等待GSN应答的请求
typedef struct _GtpReqFrameInfo 
{
    //需要发送的帧
    SGtpFrame* pFrame;

    //超时计数
    unsigned char yTimeOut;

    //重发次数
    unsigned char yRepeatTimes;

}SGtpReqFrameInfo;

///////////////////////////////////////////////////////
// GTP'所实用到的消息类型
#define ECHO_REQUEST            1   //回响请求
#define ECHO_RESPONSE           2   //回响应答
#define VERSION_NOT_SUPPORTED   3   //版本不支持  颜文远2001/07/21添加
#define NODE_ALIVE_REQUEST      4   //节点活动请求
#define NODE_ALIVE_RESPONSE     5   //节点活动应答
#define REDIRECTION_REQUEST     6   //重定向请求
#define REDIRECTION_RESPONSE    7   //重定向应答
#define DATA_TRANSFER_REQUEST   240 //数据传输请求
#define DATA_TRANSFER_RESPONSE  241 //数据传输应答

#endif //__NET_STRUCT_H_
