#ifndef __NET_STRUCT_H_
#define __NET_STRUCT_H_

#include "../include/base_type.h"
#include "../include/toolbox.h"

typedef struct _NetFrame
{
    BYTE SenderPid;         //����ģ�����ڵĽ��̺�
    BYTE SenderMid;         //����ģ���ģ���
    BYTE ReceiverPid;       //����ģ�����ڵĽ��̺�
    BYTE ReceiverMid;       //����ģ���ģ���
    UINT2 AppType;          //Ӧ������
    UINT2 ClientNo;         //ĳ�����͵�MML�ͻ��˵ı��
    struct _INetAddrPair
    {
        UINT4 ServerIp;
        UINT4 ServerPort;
        UINT4 ClientIp;
        UINT4 ClientPort;
    }SNetAddrPair;
    BYTE unused[32];
    UINT4 IPAddr;           //MML�ͻ��˵�IP��ַ
    UINT2 CmdCode;          //������
    UINT4 Len;              //Data��ĳ���
    BYTE Data[1];           //���ڴ�Ÿ��ֲ���
}SNetFrame, *PNetFrame;


//����Զ2002.04.08���
/*
    �뻬������Э����ؽṹ�Ķ��壡���е���Ϣ������1�ֽڶ����INTEL�ֽ���
    ������һ����������������Ҫ���л������ݳ�Ա��ÿ�����ݵ�Ԫ����
    һ��˽�еĳ�Ա������һ��get������һ��set��������get/set����
    ���ṩ���л����ֽ����ת������֤�����ṹΪINTEL�ֽ�����ֽ�����
    �ڶ���һ�����ݵ�Ԫʱ������������£�
        ietype ������ʾ��Ա��
        ntype  ������ʾ��Ա����������
        NorSorL������һ��ö�٣�N��ʾ�������ֽ����ת��
                               S��ʾ���������ֽڵ��ֽ���ת��
                               L��ʾ�����ĸ��ֽڵ��ֽ���ת��
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
//����Ϊ��GSM/128/CDMA�Ȳ�Ʒ���õ���ͨ��֡�ṹ
#define _MEMFRAME_HEAD_LENGTH 4
typedef struct _MEMFrame
{
    INTEL_MEMBER_ITEM(nCmd, UINT2, S)   //������
    INTEL_MEMBER_ITEM(nLen, UINT2, S)   //����������
    BYTE* Data()
    {
        return (BYTE* )this + _MEMFRAME_HEAD_LENGTH;
    }
    //���ص�new��������nSizeΪData��ĳ���
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

//SMemFrame�е�������
enum MemCmd
{
    MEM_CMD_DATA      = 0,  //����֡
    MEM_CMD_HANDSHAKE = 2  //����֡
};

#define _MEM_HANDSHAKE_LENGTH   5
typedef struct _MEMHandShakeData
{
    INTEL_MEMBER_ITEM(nNotUsed1, UINT2, S)  //�����ֽ�
    INTEL_MEMBER_ITEM(nNotUsed2, BYTE, N)   //�����ֽ�
    INTEL_MEMBER_ITEM(nModu, BYTE, N)       //ģ���
    INTEL_MEMBER_ITEM(nNotUsed3, BYTE, N)   //�����ֽ�
}SMemHandShakeData, *PMemHandShakeData;

#define _APPFRAME_HEAD_LENGTH   6
#define _APPFRAME_EXPAND_LENGTH 7
#define _APPFRAME_EXHEAD_LENGTH (_APPFRAME_HEAD_LENGTH + _APPFRAME_EXPAND_LENGTH)
typedef struct _AppFrame
{
    INTEL_MEMBER_ITEM(sender_mid, BYTE, N)      //������ģ���
    INTEL_MEMBER_ITEM(sender_pid, BYTE, N)      //������PID
    INTEL_MEMBER_ITEM(receiver_mid, BYTE, N)    //������ģ���
    INTEL_MEMBER_ITEM(receiver_pid, BYTE, N)    //������PID
    INTEL_MEMBER_ITEM(nLen, UINT2, S)           //����

    INTEL_MEMBER_ITEM(byNotUsedNow1, BYTE, N)   //�����ֽڣ�����
    INTEL_MEMBER_ITEM(sender_desk, BYTE, N)     //����̨��
    INTEL_MEMBER_ITEM(sender_app, BYTE, N)      //����App��
    INTEL_MEMBER_ITEM(receiver_desk, BYTE, N)   //����̨��
    INTEL_MEMBER_ITEM(receiver_app, BYTE, N)    //����App��
    INTEL_MEMBER_ITEM(byCmd, BYTE, N)           //������
    INTEL_MEMBER_ITEM(byNotUsedNow2, BYTE, N)   //�����ֽڣ�����
    BYTE* Data()
    {
        return (BYTE* )this + _APPFRAME_EXHEAD_LENGTH;
    }
    //���ص�new��������nSizeΪData��ĳ���
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

//SAppFrame�е�������
enum AppCmd
{
    APP_CMD_REQ_SYN     = 0x88,    //����ͬ������
    APP_CMD_REQ_DATA    = 0x89,    //������������
    APP_CMD_REQ_ACKNUM  = 0x8a,    //Ӧ��֡��ͬ������

    APP_CMD_RSP_ACK     = 0x08,    //Ӧ������
    APP_CMD_RSP_REJ     = 0x09,    //�ܾ�����
    APP_CMD_RSP_ACKNUM  = 0x0a    //Ӧ��֡��ͬ��Ӧ��
};

//SAppFrame�е�PID
enum AppPid
{
    APP_PID_HTMAIL = 0x1B,
    APP_PID_BILL = 0x20,
    APP_PID_ALARM = 0x25,
    APP_PID_MEMMAIL = 0x35,
    APP_PID_BAU     = 69
};

//SAppFrame�е�MID
enum AppMid
{
    APP_MID_NULL = 0xFE,    //��ģ���
    APP_MID_BAM = 0
};

//SAppFrame�е�̨��
enum AppDesk
{
    APP_DESK_NULL = 0xFE   //��̨��
};

//SAppFrame�е�App��
enum AppApp
{
    APP_APP_NULL = 0x7E   //�յ�App��
};


//
//����Ϊ��SoftX3000��Ʒ���õ���ͨ��֡�ṹ
#define _SYSFRAME_HEAD_LENGTH   20
#define _SYSFRAME_EXPAND_LENGTH 7
#define _SYSFRAME_EXHEAD_LENGTH (_SYSFRAME_HEAD_LENGTH + _SYSFRAME_EXPAND_LENGTH)
typedef struct _SysFrame
{
    INTEL_MEMBER_ITEM(sender_mid, UINT4, L)     //����ģ���
    INTEL_MEMBER_ITEM(sender_pid, UINT4, L)     //����PID
    INTEL_MEMBER_ITEM(receiver_mid, UINT4, L)   //����ģ���
    INTEL_MEMBER_ITEM(receiver_pid, UINT4, L)   //����PID
    INTEL_MEMBER_ITEM(nLen, UINT4, L)           //����

    INTEL_MEMBER_ITEM(ucChannelNo, BYTE, N)
    INTEL_MEMBER_ITEM(ulBamTaskID, UINT4, L)
    INTEL_MEMBER_ITEM(ucCmdCode, BYTE, N)
    INTEL_MEMBER_ITEM(ucHandle, BYTE, N)

    BYTE* Data()
    {
        return (BYTE* )this + _SYSFRAME_EXHEAD_LENGTH;
    }

    //���ص�new��������nSizeΪData��ĳ���
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

//SSysFrame�е�������
enum SysCmd
{
    //���µ�������Ϊ��SAppFrame��һ�µ�������
    SYS_CMD_REQ_SYN     = 0x88,     //����ͬ������
    SYS_CMD_REQ_DATA    = 0x89,     //������������
    SYS_CMD_REQ_ACKNUM  = 0x8a,     //Ӧ��֡��ͬ������

    SYS_CMD_RSP_ACK     = 0x08,     //Ӧ������
    SYS_CMD_RSP_REJ     = 0x09,     //�ܾ�����
    SYS_CMD_RSP_ACKNUM  = 0x0a,     //Ӧ��֡��ͬ��Ӧ��

    //���µ�������ΪSSysFrame�����е�������
    //modified by wangzhaoyang 2002/07/08
    SYS_CMD_SYS_HAND_SHAKE = 2,     //SYS�巢��BAU����������
    SYS_CMD_BAU_HAND_SHAKE = 2,     //BAU����SYS�����������
    SYS_CMD_HAND_SHAKE_ACK = 3,     //������Ӧ�����ʱ����
    //end 2002/07/08
    SYS_CMD_ALARM          = 0xFB  //BAU���͸澯������
};

//SSysFrame�е�PID
enum SysPid
{
    SYS_PID_HTCOMM = 51,    //modified by wangzhaoyang 2002/07/08��ԭΪ1���������޸�
    SYS_PID_ALARM  = 79,    //modified by wangzhaoyang 2002/07/08��ԭΪ37���������޸�
    SYS_PID_BAU    = 69
};

//SSysFrame�е�MID
enum SysMid
{
    SYS_MID_BAM = 0,    //BAM��ģ���
    SYS_MID_BAU = 1    //BAU��ģ���
};


//
//����Ϊ�ڻ�������Э�����õ��Ľṹ
//ͬ��Ӧ��֡����Ϣ������
typedef struct _SyncAckNum
{
    INTEL_MEMBER_ITEM(nAckNum, UINT4, L)
}SSyncAckNum;

//ͬ��������ˮ����Ϣ������
typedef struct _SyncSequ
{
    INTEL_MEMBER_ITEM(nSequ, INT4, L)
}SSyncSequ;

//���Ż���������Ϣ�Ľṹ
#define _BILLMSG_HEAD_LENGTH 4
typedef struct _BillMessage
{
    INTEL_MEMBER_ITEM(nSequ, INT4, L)
    BYTE* Data()
    {
        return (BYTE* )this + _BILLMSG_HEAD_LENGTH;
    }
}SBillMessage;

//BAU������Ӧ����Ϣ������
typedef struct _AckOrRejRes
{
    INTEL_MEMBER_ITEM(nSequ, INT4, L)
    INTEL_MEMBER_ITEM(nAckNum, INT4, L)
}SAckOrRejRes;


//����Ϊ�澯��Ϣ�еĽṹ

//��GSM��128�Ȳ�Ʒ���õ���SMemFrame�ṹ�еĸ澯Ϣ�ṹ������SMemFrame��Data����
#define _MEM_ALARM_INFO_HEAD_LENGTH     6
#define _MEM_ALARM_INFO_EXPAND_LENGTH   4
#define _MEM_ALARM_INFO_EXHEAD_LENGTH   (_MEM_ALARM_INFO_HEAD_LENGTH + _MEM_ALARM_INFO_EXPAND_LENGTH)
struct SMemAlarmInfo
{
    INTEL_MEMBER_ITEM(sender_mid, BYTE, N)      //������ģ���
    INTEL_MEMBER_ITEM(sender_pid, BYTE, N)      //�����߽��̺�
    INTEL_MEMBER_ITEM(receiver_mid, BYTE, N)    //������ģ���
    INTEL_MEMBER_ITEM(receiver_pid, BYTE, N)    //�����߽��̺�
    INTEL_MEMBER_ITEM(nLen, UINT2, S)           //�򳤶�
    INTEL_MEMBER_ITEM(byBauNo, BYTE, N)         //�����澯�ķ�������
    INTEL_MEMBER_ITEM(uAlarmID, UINT2, S)       //�������澯ID
    INTEL_MEMBER_ITEM(byAllRestore, BYTE, N)    //�Ƿ�ȫ���ָ����,
                                                //0:ȫ����1������
    BYTE* Paras()   //�澯����
    {
        return (BYTE* )this + _MEM_ALARM_INFO_EXHEAD_LENGTH;
    }
};

//��SoftX3000���õ��ĸ澯��Ϣ�еĽṹ������SSysFrame��Data����
#define _SYS_ALARM_INFO_LENGTH  (36 + MAX_ALARM_PARA)   //modify by lx SWPD04504
struct SSysAlarmInfo
{
    MEMBER_ITEM(ulAlarmSN, UINT4, L)        //�澯���кţ������ۼӣ���̨����
                                            //�������ֶο��Ƿ��ж�ʧ�澯
    MEMBER_ITEM(ulAlarmID, UINT4, L)        //�澯ID
    MEMBER_ITEM(ulCauseID, UINT4, L)        //�澯ԭ��ID
    MEMBER_ITEM(ucAlarmType, BYTE, N)       //�澯����
    MEMBER_ITEM(ucByNMSType, BYTE, N)       //Send to NMS or not
    MEMBER_ITEM(ucAlarmBoxType, BYTE, N)    //�澯����� Function ID
    MEMBER_ITEM(ucAlarmLevel, BYTE, N)      //�澯����
    MEMBER_ITEM(ucSubSystemId, BYTE, N)     //���ܸ澯����
    MEMBER_ITEM(uwYear, UINT2, S)           //����Ϊ�澯ʱ��
    MEMBER_ITEM(ucMonth, BYTE, N)
    MEMBER_ITEM(ucDate, BYTE, N)
    MEMBER_ITEM(ucHour, BYTE, N)
    MEMBER_ITEM(ucMinute, BYTE, N)
    MEMBER_ITEM(ucSecond, BYTE, N)
    MEMBER_ITEM(ulCpuID, UINT4, L)          //�澯ģ���
    MEMBER_ITEM(ulAlarmPID, UINT4, L)       //�澯PID
    MEMBER_ITEM(uwReserve, UINT2, S)        //�����ֶ�
    MEMBER_ITEM(uwParaLength, UINT2, S)     //�澯��������
    BYTE  ucPara[MAX_ALARM_PARA];           //�澯����
};

//����Զ2002.04.08��ӽ���

//���ӶԸ澯��Ĳ���
enum AlarmCmd
{
  ALARM_CMD_BAU_TO_CONVERT = 0xAA,
  ALARM_CMD_CONVERT_TO_BAU = 0xAA
};

//BAU��Convert֮���ͨ��֡�ṹ��
#define _APPCVTFRM_HEAD_LENGTH  4
struct SAppConvertFrame
{
    BYTE byCmd;     //�����룬iGateway Bill����ͨ�Ų㷢�͵�CONVERT�̶�Ϊ0xAA
                    //CONVERT���͵�OM�˹̶�Ϊ0xAA
    BYTE byError;   //�����룬iGateway Bill����ͨ�Ų㷢�͵�CONVERT�̶�Ϊ0x00
                    //����CONVERT��˵���Ѹ澯��Ĵ�����Ϣ���͸�iGateway Bill��
                    //��ͨ�Ų�ʱ���̶�Ϊ0x00,��CONVERT���յ��������Ϣ����Ӧ
                    //��iGateway Bill����ͨ�Ų������Ϊ0x01
    UINT2 wLen;     //���͵������������ݳ��ȣ��澯ת�����������һ��Ϣ�����յ�
                    //�����ݳ��ȱȽϣ�����һ�£�˵�����յ���Ϣ����
    BYTE* Data()    //������
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


// GTPЭ��ͷ��ʹ�õ��ĳ���
#define GTP_V0_TAG1     0xE         //��1�ֽڱ�־ GTP' V0
#define GTP_V06_TAG1    0xF         //��1�ֽڱ�־ GTP' V0,֡ͷΪ6���ֽ�
#define GTP_V1_TAG1     0x2E        //��1�ֽڱ�־ GTP' V1
#define GTP_V16_TAG1    0x2F        //��1�ֽڱ�־ GTP' V1,֡ͷΪ6���ֽ�
#define GTP_V2_TAG1     0x4E        //��1�ֽڱ�־ GTP' V2

#define GTP_FRAME_HEAD_LENGTH_V1        20
#define GTP_FRAME_HEAD_LENGTH_V2        6
#define GTP_FRAME_HEAD_LENGTH_V1_EXTRA  14

/*
    _GtpFrame֡�ṹ˵��
    ��֡�ṹ֧��GTP'V0,V1��V2�汾���ڴ���֡ʵ��ʱ��
    ���ȸ���Э��֡�İ汾�ŷ����ڴ�ռ䣬Ȼ���ڵ�һ
    ʱ������yTag�汾��־�����汾��Ϣд��֡ͷ�У���
    �˲���֮��Data��GetHeadSize��GetSize�Ⱥ�����
    ��ʹ��;
*/
typedef struct _GtpFrame
{
    //��־������GTP'�汾��Ϣ�������ȡֵ��Ϊ0x4E
    MEMBER_ITEM(yTag,    unsigned char,  N)

    //��Ϣ����
    MEMBER_ITEM(yType,   unsigned char,  N)

    //�������ĳ��ȣ���Data����ֽ�����
    MEMBER_ITEM(uLen,    unsigned short, S)

    //���к�
    MEMBER_ITEM(uSeqNum, unsigned short, S)

    //ȡ��������ַ���������ֽڶ�������
    inline BYTE* Data()
    {
        return (BYTE*)this + GetHeadLen();
    }

    //���ݰ汾�Ż�ȡЭ��֡ͷ��,ȱʡΪ6�ֽ�
    inline static UINT2 GetHeadLen(UINT4 uGtpVersion)
    {
        if((uGtpVersion == GTP_V0_TAG1) || (uGtpVersion == GTP_V1_TAG1))
        {
            return GTP_FRAME_HEAD_LENGTH_V1;
        }

        return GTP_FRAME_HEAD_LENGTH_V2;
    }

    //ȡ֡ͷ��
    inline UINT2 GetHeadLen()
    {
        return GetHeadLen(yTag());
    }

    //ȡ֡��ȫ��
    inline UINT2 GetSize()
    {
        return GetHeadLen() + uLen();
    }

    //��֤�汾��
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

    //��дV0��V1��20���ֽ�ͷ�������䲿��
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
  CHECK_TV���һ��TV�ṹ��
    pframe ��������ietype�ṹ��SGtpFrame��ָ��
    ielen  ����Ϊietype�ṹ�ĳ���
    pie    ����Ϊһָ��ietype��ָ��
    noffset����ָ��TVλ��pframe���ݶ��е�ƫ������������ݰ���:

   1. �����pframeָ���SGtpFrame�����Ƿ��㹻��
   2. ��ӦTV�ṹ���������Ƿ����
*/
//2002-11-23 ��ŷ� ��Ӧ���ⵥ D19425
template<class Tframe, class Tietype>
inline int CHECK_TV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset);

//CHECK_TLV���һ��TLV�ṹ���ú����Ƚ���TV�ṹ��顣Ȼ���
//��֡�����Ƿ��㹻��
template<class Tframe, class Tietype>
inline int CHECK_TLV(Tframe* pframe, const char* szTietype, int ielen, Tietype*& pie, int noffset);

//�й�cause�Ķ���
#define CAUSE_SYSTEM_FAILURE        59  //ϵͳ����/System failure
#define CAUSE_TRANS_BUF_FULL        60  //���仺������/Send buffers becoming full
#define CAUSE_RECEIVE_BUF_FULL      61  //���ջ�������/Receive buffers becoming full
#define CAUSE_ANOTHER_GODOWN        62  //��һ����㽫�ر�/Another node is about to go down
#define CAUSE_OWN_GODOWN            63  //����㽫�ر�/This node is about to go down
#define CAUSE_REQUEST_ACCEPTED      128 //�������/Request accepted
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

//casue�ṹ
#define TIECAUSE_LENGTH             2
struct TIECause 
{   
    enum { type = 1 };
    MEMBER_ITEM(yType,      BYTE,  N)   //��Ϣ����
    MEMBER_ITEM(yCause,     BYTE,  N)   //ԭ��ֵ
};

//recovery�ṹ
#define TIERECOVERY_LENGTH          2
struct TIERecovery 
{   
    enum { type = 14 };
    MEMBER_ITEM(yType,        BYTE,N)   //��Ϣ����
    MEMBER_ITEM(yRestartCount,BYTE,N)   //��������
};

//transfer command����
#define TC_SENDDATA                 1   //��������
#define TC_SENDDATAMAYBEDUP         2   //���Ϳ����ظ�����
#define TC_CANCELDATA               3   //ȡ������
#define TC_RELEASEDATA              4   //�ͷ�����

//transfer command�ṹ
#define TIETRANSFER_CMD_LENGTH      2
struct TIETransferCommand 
{   
    enum { type = 126 };
    MEMBER_ITEM(yType,      BYTE,  N)   //��Ϣ����
    MEMBER_ITEM(yCmd,       BYTE,  N)   //������
};

//data record package�ṹ
#define TIEBILL_BACKAGE_HEAD_LENGTH 7
struct TIEBillPackage 
{   
    enum { type = 252 };
    MEMBER_ITEM(yType,      BYTE,  N)   //��Ϣ����
    MEMBER_ITEM(uLen,       UINT2, S)   //����������
    MEMBER_ITEM(yCount,     BYTE,  N)   //��¼��
    MEMBER_ITEM(yFormat,    BYTE,  N)   //�����ʽ
    MEMBER_ITEM(uVersion,   UINT2, S)   //��ʽ�汾

    //ȡ��������ַ���������ֽڶ�������
    inline BYTE*  Data()
    {
        return (BYTE*)this + TIEBILL_BACKAGE_HEAD_LENGTH;
    }
};

//����data record package�����ݶγ���
//�ú�ʹ��TIEBillPackage��Ϊ����
/*#define DATASIZE_OF_IEBILLPACKAGE(pPackage) \
    (int)(pPackage->uLen() + IETLV_HEAD_LENGTH \
     - TIEBILL_BACKAGE_HEAD_LENGTH)

//�ú�ʹ��SGtpFrame��Ϊ����
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


//��node address�йصĳ���
#define LEN_NODEADDRESS             4   //IPV4��ַ����

//node address�ṹ
#define TIENODE_ADDRESS_LENGTH      7
struct TIENodeAddress 
{   
    enum { type = 251 };
    MEMBER_ITEM(yType,      BYTE,  N)   //��Ϣ����
    MEMBER_ITEM(uLen ,      UINT2, S)   //����������
    MEMBER_ITEM(uAddress,   UINT4, N)   //IPV4��ַ
};

//�ض���ʱ�Ƽ���IP��ַ
#define TIERECMD_NODE_ADDR_LENGTH   7
struct TIERecommendNodeAddress 
{   
    enum { type = 254 };
    MEMBER_ITEM(yType,      BYTE,  N)   //��Ϣ����
    MEMBER_ITEM(uLen ,      UINT2, S)   //����������
    MEMBER_ITEM(uAddress,   UINT4, N)   //IPV4��ַ
};

//sequence canceled�ṹ
#define TIESN_CANCELED_HEAD_LENGTH  3
struct TIESNCanceled 
{   
    enum { type = 250 };
    MEMBER_ITEM(yType,      BYTE,  N)   //��Ϣ����
    MEMBER_ITEM(uLen,       UINT2, S)   //����������

    //ȡ���к�������������ַ��Ҫ�����ֽڶ�������
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

//sequence released�ṹ
#define TIESN_RELEASED_HEAD_LENGTH  3
struct TIESNReleased 
{   
    enum { type = 249 };
    MEMBER_ITEM(yType,      BYTE,  N)   //��Ϣ����
    MEMBER_ITEM(uLen,       UINT2, S)   //����������

    //ȡ���к�������������ַ��Ҫ�����ֽڶ�������
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
    MEMBER_ITEM(yType,      BYTE,  N)   //��Ϣ����
    MEMBER_ITEM(uLen,       UINT2, S)   //����������

    //ȡ���к�������������ַ��Ҫ�����ֽڶ�������
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

//SGtpReqFrameInfo�����˵ȴ�GSNӦ�������
typedef struct _GtpReqFrameInfo 
{
    //��Ҫ���͵�֡
    SGtpFrame* pFrame;

    //��ʱ����
    unsigned char yTimeOut;

    //�ط�����
    unsigned char yRepeatTimes;

}SGtpReqFrameInfo;

///////////////////////////////////////////////////////
// GTP'��ʵ�õ�����Ϣ����
#define ECHO_REQUEST            1   //��������
#define ECHO_RESPONSE           2   //����Ӧ��
#define VERSION_NOT_SUPPORTED   3   //�汾��֧��  ����Զ2001/07/21���
#define NODE_ALIVE_REQUEST      4   //�ڵ�����
#define NODE_ALIVE_RESPONSE     5   //�ڵ�Ӧ��
#define REDIRECTION_REQUEST     6   //�ض�������
#define REDIRECTION_RESPONSE    7   //�ض���Ӧ��
#define DATA_TRANSFER_REQUEST   240 //���ݴ�������
#define DATA_TRANSFER_RESPONSE  241 //���ݴ���Ӧ��

#endif //__NET_STRUCT_H_
