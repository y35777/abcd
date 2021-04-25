#ifndef  __NET_STATUS_H__
#define  __NET_STATUS_H__

#include "../utility/config.h"
#include "../include/frame.h"

//Э��ջ����
enum STACK_TYPE
{
    STACK_SLIDE     = 0,
    STACK_GTP       = 1,
    STACK_SCAN      = 2,
    STACK_WLAN_AS   = 3
};


//����ģ�黬������Э���״̬�ṹ
typedef struct _SlideWndModInfo
{
    //ģ��ID������һ��SMģ���Ӧһ��Module
    unsigned char yModule;
    struct
    {
        //��ǰ��������ͷ���к�
        unsigned long uBaseSeqNum;

        //���ύ���ݼ�¼��
        unsigned long uSubmitRecCnt;
    }SSm;
}SSlideWndModInfo;
#define SLIDE_MOD_INFO_LENGTH sizeof(SSlideWndModInfo)


//����ģ��GTP'Э���״̬�ṹ
typedef struct _GtpNodeInfo
{
    unsigned long uStackID;             //Stack ID
    unsigned long uAppType;             //Э������,ȡֵ��APP_TYPE
    unsigned long nActivedGsnIP;        //��GSN��ǰ���IP��ַ
    unsigned long uGsnIp[GSN_IP_TABLE_IPCOUNT_MAX];           //�����GSNԤ��10��IP��ַ
    unsigned long uGsnPort;             //GSN�Ķ˿ں�
    unsigned long uLocalIp;             //����IP��ַ,ֻ��������õ��Ǹ�
    unsigned long uLocalPort;           //���ض˿ں�,ֻ��������õ��Ǹ�
    unsigned long uGsnRestartCount;     //GSN����������
    unsigned long uFirstSNFile;         //��һ����Ч��SNFILE�����к�
    unsigned long uPIDAtGSNRestart;     //GSN�ϴ�����ʱ��PID��
    unsigned long uGtpVersion;          //��ǰGSN�İ汾��
    unsigned long uReserved[50];        //����һ���ֿռ䣬�����´�������ʱҪɾ��ԭ״̬�ļ�
}SGtpNodeInfo;
#define GTP_NODE_INFO_LENGTH sizeof(SGtpNodeInfo)


//��������Э�������ģ���״̬�ṹ
typedef struct _SlideWndStatusInfo
{
    //��ǰ��ID
    unsigned long uPacketId;

    //�Ƿ�ȴ�ǰ����Ӧ�����ύ��֡����
    BOOL bWaitForResponse;

    SSlideWndModInfo SlideModInfo[1];//����������

}SSlideWndStatusInfo, *PSlideWndStatusInfo;


//GTP'Э�������ģ���״̬�ṹ
typedef struct _GtpStatusInfo
{
    //��ǰ��ID
    unsigned long uPacketId;

    //�Ƿ�ȴ�ǰ����Ӧ�����ύ��֡����
    BOOL bWaitForResponse;

    SGtpNodeInfo GtpNodeInfo[1];//����������

}SGtpStatusInfo;


#define SF_LOAD         1
#define SF_SAVE         2

#define INVALID_STACK_ID    0xFFFFFFFF
//����������ģ�����״̬�ļ���
class CNetStatusInfo
{
public:
    CNetStatusInfo(UINT4 const uBufferLen);
    virtual ~CNetStatusInfo();

    typedef LIST<CNetStatusInfo*> LIST_PNETSTATUSINFO;

    //��ʼ����״̬�ļ�
    static int Init(const char* const szStatusFileName);

    //����״̬��Ϣ
    void Set(const BYTE* const pBuf, UINT4 const uLen);

    //��ȡ״̬��Ϣ
    UINT4 Get(BYTE* const pBuf, UINT4 const uLen, 
              BOOL const bReadFile = TRUE);

    //ˢ�±����������״̬���ݵ���״̬�ļ���
    virtual int Flush(void);

    //����״̬�ļ��л�õ�nNum��Э��ջ�ı��
    static UINT4 EnumStackId(int const nNum, 
                             int const nStackInfoType);

    //ˢ������״̬���ݵ���״̬�ļ���
    static int FlushAll(void);

    //��ȡ�ڴ�������״̬��Ϣ����
    static int GetAllStatusData(BYTE* &pBuffer, int nBufLen);

    //��ȡ�ڴ�������״̬��Ϣ���ݵĳ���
    static int GetAllStatusDataLen();

protected:

    //��״̬��Ϣ�����������ݴ���״̬�ļ��ĵ�ǰλ��д����״̬��
    //��m_StatusFile��
    virtual int WriteFile(void);

    //����״̬�ļ�m_StatusFile��ȡ״̬��Ϣ����ͬCNetStatusInfo
    //��д״̬�ļ����״̬��Ϣ�Ļ����ǲ�ͬ�ģ�ֻ�и�������֪��
    //���ʵ�֣����Ըú���Ϊ�յ��麯������ҪCNetStatusInfo����
    //���ظú���
    virtual int ReadFile(void);

    //���е�CNetStatusInfo������󶼶�дͬһ����״̬�ļ�
    //�ýṹָ���ڳ�ʼ�������д������������������ͷ�
    static FILE* m_pStatusFile;

    //��CNetWorkStatusInfo�ڹ��캯���н��������ָ�븳��
    //m_pNetWorkStatusInfo�����������ط�����Ըñ�����ֵ
    static CNetStatusInfo* m_pNetworkStatusInfo;

    //CNetStatusInfo�����ָ���б�
    static LIST_PNETSTATUSINFO m_StackStatusInfoList;

    //���л��������ۼ��ܳ���
    static UINT4 m_uTotalBufLen;

    //״̬��Ϣ������,�û������ڹ��캯���з��䣬�������������ͷ�
    unsigned char* m_pBuffer;

    //״̬��Ϣ�������ĳ���
    UINT4 m_uBufferLen;

    //״̬�ļ����ļ���
    static char m_szFileName[MAX_PATH];
};

typedef struct _NetWorkStatusInfo
{
    //��ǰ��ID
    unsigned long uPacketId;

    //�Ƿ�ȴ�ǰ����Ӧ�����ύ��֡����
    BOOL bWaitForResponse;

}SNetWorkStatusInfo;
#define NET_STATUS_INFO_HEADLENGTH sizeof(SNetWorkStatusInfo)


//CNetWorkStatusInfo�����˶�����ģ����״̬�ļ�ͷ�Ķ�д����
class CNetWorkStatusInfo:public CNetStatusInfo
{
public:
    CNetWorkStatusInfo(UINT4 const uBufferLen);
    virtual ~CNetWorkStatusInfo();

    //ˢ�±����������״̬���ݵ���״̬�ļ���
    virtual int Flush(void);

protected:

    //����״̬�ļ�m_pStatusFile��ȡ�ļ�ͷ��Ϣ
    virtual int ReadFile(void);

};

//CStackStatusInfo�����˶�Э��ջ״̬��Ϣ�Ķ�д
class CStackStatusInfo:public CNetStatusInfo
{
public:
    CStackStatusInfo(int const nStackStatusInfoType,
                     UINT4 uStackId);
    virtual ~CStackStatusInfo();


    //ˢ�±����������״̬���ݵ���״̬�ļ���
    virtual int Flush(void);

protected:

    //����״̬�ļ�m_pStatusFile��ȡ�������Ӧ��Э��ջ��Ϣ
    virtual int ReadFile(void);

    //Э��ջ����
    int m_nStackStatusInfoType;

    //Э��ջ���
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

//�ļ�ɨ��Э�������ģ���״̬�ṹ
struct SScanStatusInfo
{
    //��ǰ��ID
    unsigned long uPacketId;

    //�Ƿ�ȴ�ǰ����Ӧ�����ύ��֡����
    BOOL bWaitForResponse;

    SScanStackInfo ScanNodeInfo[1];//����������

};
#define SCAN_STATUS_INFO_LEN     sizeof(SScanStatusInfo)


#endif  //__NET_STATUS_H__
