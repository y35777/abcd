#ifndef  __GTP_RELATED_H__
#define  __GTP_RELATED_H__

#include "../include/frame.h"
#include "../comm/comm_struct.h"
#include "protocol_stack.h"
#include "protocol_track.h"
#include "net_app.h"
#include "resource.h"

//֧��GTP'Э���Ӧ�ò�ÿ���һ��ԭʼ��������������һ����Ӧ��
//״̬�ļ�����״̬�ļ��а�������Э��ջ�ύ���ð�ԭʼ�����е�
//�������кš�CGtpSNFileRef�����˶Ը�״̬�ļ�������
class CGtpSNFileRef
{
public:
    CGtpSNFileRef(unsigned long const uFileNumber);
    virtual ~CGtpSNFileRef();

    //��ñ������Ӧ��״̬�ļ��ı��
    unsigned long GetFileNumber(void);

    //����������ü�����һ
    unsigned long AddRef(void);

    //����������ü�����һ
    unsigned long Release(BOOL bDeleteFile = TRUE);

protected:

    //�ó�Ա������ʾ����Ӧ״̬�ļ������ü�����
    unsigned long m_uRefCount;

    //�ó�Ա������ʾ��Ӧ״̬�ļ��ı�ţ���Ϊ��Ӧ��ԭʼ�������İ���
    unsigned long m_uFileNumber;

};

typedef struct _GtpSNFile
{
    //����״̬�ļ��Ľṹ
    FILE *pStatusFile;

    //״̬�ļ�����
    CGtpSNFileRef *pFileRef;

}SGtpSNFile;

typedef struct _GtpSNListInFile 
{   
    //���к�״̬�ļ�����
    CGtpSNFileRef *pFileRef;

    //���к�����
    LIST<unsigned long> *pListSeqNum;

}SGtpSNListInFile;


//����״̬��ʾ֡��״̬
#define FS_NULL         0   //֡������
#define FS_NEW          1   //���յ�
#define FS_SUBMITTED    2   //���ύ������ģ��
#define FS_SAVED        3   //�ѳɹ�����

//װ�뷽ʽ����
#define LA_DISCARD      0   //�������ݶ���
#define LA_SAVED        1   //װ������Ϊ�ѱ���
#define LA_SUBMITTED    2   //��������Ϊ���ύ

//CSNList������GTP'Э��ջ�еġ����к��б�
class CSNList
{
public:
    CSNList(void);
    virtual ~CSNList(void);

    //��״̬�ļ��еĵ�ǰλ��װ�����к�
    int Load(SGtpSNFile* const pSNFile, int const nAction);

    //���Ѿ��ύ�����кű��浽״̬�ļ���
    int Save(SGtpSNFile* const pSNFile);

    //����µ����к�
    void Add(unsigned long const uSeqNum);

    //�ύnCount���µ����к�
    void Submit(int const nCount);

    //����Ѿ��ύ�����к��б�
    LIST<unsigned long>* const GetSubmitted(void);

    //��õ�һ���ļ���
    unsigned long GetFirstFileNumber(void);

    //�ж�nSeqNumber�Ƿ����Ѿ��ڱ������к��б���
    int Lookup(unsigned long const uSeqNum);

    //ǰ����ģ��Ӧ��󣬶����к��б������Ӧ�Ĵ���
    void OnResponse(BOOL const bSaved);

    //����������е��������к�
    void Clear(BOOL bDeleteFile);

protected:

    //�ó�Ա������ʾɾ����ʱ���к�ʱ������������ʷ���кŵĸ���
    //��m_nAllFrameCount����m_nHisFrameCountʱҪ���һЩ��¼
    //�ó�Ա������ȱʡֵΪ3000�����������ã�
    int m_nHisFrameCount;

    //�ó�Ա������ʾ�����к��б��е�ǰ����״̬���Ѿ���ǰ����ģ
    //�鱣��ġ��Ѿ��ύ��ǰ����ģ�黹û�б���ġ���û���ύ�ģ�
    //���кŵ�����
    int m_nAllFrameCount;

    typedef LIST<SGtpSNListInFile*> LIST_PGTPSNLISTINFILE;
    //�������е�Ԫ��ΪSGtpSNListInFile�ṹ��ָ��
    LIST_PGTPSNLISTINFILE m_Saved;

    //�������е�Ԫ��Ϊ�Ѿ��ύ��ǰ����ģ�黹û�гɹ���������кţ�
    LIST<unsigned long>* m_pSubmitted;

    //�������е�Ԫ��Ϊ��û���ύ��ǰ����ģ����б�������кţ�
    LIST<unsigned long> m_New;

    //��MAP�ļ�ֵΪ���кţ���������ֵΪ���кŵ�״̬��
    //״̬��ȡֵ���£� FS_SAVED�����Ѿ���ǰ����ģ�鱣�� 
    //                 FS_SUMITTED�����Ѿ��ύ��ǰ����ģ�黹û�гɹ�����
    //                 FS_NEW������û���ύ��ǰ����ģ����б���
    MAP<unsigned long, char> m_SeqMap;

    //��ָ��ָ�����һ�����ʵ����к�״̬�ļ������ö���
    CGtpSNFileRef* m_pLastSNFileRef;

};

//��Ч���������
#define INVALID_RESTARTCOUNT    0xFFFF

//CGtpStack������CProtocolStack��������GTP'Э��ջ
class CGtpStack:public CProtocolStack
{
public:
    CGtpStack(CNetApp* const pNetApp, 
              unsigned long const uStackId);
    virtual ~CGtpStack();

    //�ú��������ʷ��¼��δӦ��֡��������֡
    void Clear(BOOL bDeleteFile);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ��
    //����Ļ������ݵ��ֽ���
    virtual int GetReadyDataSize(void);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ��
    //����Ļ������ݵļ�¼��
    virtual int GetReadyRecordCount(void);

    //����Э��ջ���ύ��ǰ����ģ�鱣��Ļ������ݷ���*pDataBuf������
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //������뱾Э��ջ��Ӧ��GSN���͹�����һ֡���� 
    virtual void OnReceive(void* pFrame);

    //���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
    virtual void OnResponse(BOOL const bSaved);

    //����״̬�ļ��ж�ȡ��Э��ջ��״̬��Ϣ
    virtual int LoadStatus(void);

    //����״̬�ļ���д�뱾Э��ջ��״̬��Ϣ
    virtual int SaveStatus(void);

    //װ�����к��б�
    int LoadSNList(SGtpSNFile *pSNFile);

    //�������к��б�
    int SaveSNList(SGtpSNFile *pSNFile);
    
    //���¼�������
    virtual void Timer1Sec(void);

    //���Ӧ������ģ���GSN����Э��֡
    int SendFrame2Ex(void* pFrame);

    //����Echo Request
    void SendEchoRequest();

    //����Node Alive Request
    void SendNodeAliveRequest(UINT4 uLocalAddr);

    //����GTP����/Ӧ��֡
    void SendGTPRequest(SGtpFrame *pFrame);
    void SendGTPResponse(SGtpFrame *pFrame);

    //����һ��GTP֡
    SGtpFrame* CreateGTPFrame(UINT2 uLen);

    //����һ��GTP֡
    SGtpFrame* CopyGTPFrame(SGtpFrame *pFrame);

    //���Ͱ汾��֧�ֵ�GTP֡
    void SendVersionNotSupported(SGtpFrame *pFrame);

    //��������GSN����RedirectionRequest֡
    void SendRedirectionRequest(char* szIpAddress, 
                                BYTE yCause = CAUSE_SYSTEM_FAILURE);

    //�˳�ʱ�Ƿ�ɾ���ļ�
    BOOL m_bDeleteFileOnDestroy;

    //���±�Э��ջ��GSN��IP��ַ��
    void UpdateGsnIPTable(VECTOR<UINT4>& ip_table);

    //��ñ�Э��ջ��GSN��IP��ַ��
    void GetGsnIPTable(VECTOR<UINT4>& ip_table);

    //ɾ��һ��GSN��IP��ַ
    void DelGsnIp(UINT4 uIPAddr);

    void SetGsnName(const char* szGsnName)
    {
        strncpy(m_szGsnName, szGsnName, sizeof(m_szGsnName));
        m_szGsnName[sizeof(m_szGsnName) - 1] = '\0';

        strncpy(m_szName, szGsnName, sizeof(m_szName));
        m_szName[sizeof(m_szName) - 1] = '\0';
    }

protected:

    //�����ʷ��¼
    void ResetHisRecord();

    //����Data Record Transfer��Ϣ���������ݴ�������
    void OnGTPDataTransferRequest(SGtpFrame *pFrame);
    void OnGTPEchoRequest(SGtpFrame *pFrame);
    void OnGTPEchoResponse(SGtpFrame *pFrame);
    void OnGTPResponse(SGtpFrame *pFrame);
    void OnGTPSendData(SGtpFrame *pFrame, BOOL bMaybeDup);

    //�������֡
    void OnGTPTestEmptyFrame(SGtpFrame *pFrame);
    void OnGTPMaybeDupData(SGtpFrame *pFrame);
    void OnGTPNormalData(SGtpFrame *pFrame);
    void OnGTPCancelData(SGtpFrame *pFrame);
    void OnGTPReleaseData(SGtpFrame *pFrame);
    void OnGTPNodeAliveRequest(SGtpFrame *pFrame);
    
    //����/ɾ�����ݴ���Ӧ��֡
    SGtpFrame *CreateTransferResponse(BYTE nCause, 
                                      UINT2 nFrameCount);
    void SendTransferResponse(UINT2 uSeqNum, 
                              BYTE bCause = CAUSE_REQUEST_ACCEPTED);
    void DeleteTransferResponse(SGtpFrame *pFrame);
    //�����ظ��������ѱ����֡���к�
    void SendAleadyFulfill();

    //��Ҫ�����״̬��Ϣ
    SGtpNodeInfo m_GtpNodeInfo;

    //�ó�Ա��ʾ��GTP'Э��ջ�ġ����к��б�
    CSNList m_SNList;

    //�������е�Ԫ��ΪSGtpFrame�ṹ��ָ��
    //�������е�Э��֡Ϊ���ǻ�û���ύ��ǰ����ģ����б����
    LIST<SGtpFrame*> m_ListNewFrames;

    //�ó�Ա������ʾ�½��յ���Э��֡�ĸ���
    int m_nNewRecordCount;

    //�ó�Ա������ʾ�½��յ�������Э��֡�ĳ��ȵ��ܺ�
    int m_nNewDataSize;

    //�������е�Э��֡Ϊ�Ѿ��ͷŵ�Э��֡
    LIST<SGtpFrame*> m_ListReleasedFrames;

    //�������е�Ԫ��Ϊ�Ѿ��ͷŵ�Э��֡�����к�
    LIST<unsigned long> m_ListReleasedSN;

    //�������е�Ԫ��Ϊ�ѱ����Э��֡�����к�
    LIST<unsigned long> m_ListSavedSN;

    //��ָ��ָ��Ԥ�ȷ����Ӧ��֡,��֡�ڹ��캯���д�����
    //��֡��������ȱʡ��СΪ10KB,��֡�������������ͷ�
    SGtpFrame* m_pAckFrame;

    //��MAP�ļ�ֵΪ����֡�����кţ�Ԫ��ΪSGtpReqFrameInfo�ṹ��ָ��
    //SGtpReqFrameInfo�����˵ȴ�GSNӦ��������䶨�����£�
    typedef MAP<unsigned long, SGtpReqFrameInfo*> MAP_SN_TO_GTPREQ;
    MAP_SN_TO_GTPREQ m_MapReqFrames;
    
    //�ó�Ա������ʾ��GSN��������֡ʱ����һ�����õ����к�
    //�ó�Ա�����ڹ��캯���г�ʼ��Ϊ1
    unsigned long m_nGTPSeqNumber;

    //����Echo Request֡��ʱ�������
    int m_nEchoReqInvCounter;

    //����Node Alive Request֡��ʱ�������
    int m_nNodeAliveReqInvCounter;

    //����Ŀ¼,��"d:\\statusfile\\network\\10.105.32.24\\"
    char m_szWorkDir[MAX_PATH];

    //Э��֡ͷ��
    int m_nGtpHeadLen;

    //GTP'Э��֡��������
    int m_nBufferLimit;

    //GSN����
    char m_szGsnName[20];
    
    //�ϴε���SendAleadyFulfill��ʱ��
    ACE_Time_Value  m_tLastSendAdyFulfillTime;

	LIST<time_t>	m_ResTime_new;
	LIST<time_t>	m_ResTime_old;
};

//CGtpNetApp������CNetApp��������֧��GTP'Э���Ӧ�ò�
class CPerfItem;
class CGtpNetApp:public CNetApp
{
public:
    CGtpNetApp(CNetWork* const pNetWork);
    virtual ~CGtpNetApp();

    //��ʼ������
    virtual int Init(void);

    //����״̬�ļ��ж�ȡ����Э��ջ��״̬��Ϣ
    virtual int LoadStatus(void);

    //����״̬�ļ���д������Э��ջ��״̬��Ϣ
    virtual int SaveStatus(void);

    //��������Ŀ¼�µ�����״̬�ļ�������������
    int LoadPacketSNStatus();

    //���浱ǰ�������к��б�
    int SavePacketSNStatus();

    //��������������Э��֡
    virtual void OnReceive(SNetFrame* pFrame);

    //�������յ�֡������󳤶�
    int GetMaxNewFrameCount();

    //���ط���Echo Request֡��ʱ��������λΪ����
    int GetEchoReqInv();

    //����REQUEST��Ϣ���ͳ�ʱʱ��
    int GetRequestTimeOut();

    //���س�ʱ�ط�����
    int GetTimeOutRepeat();

    //����Node Alive Request֡��ʱ��������λΪ����
    int GetNodeAliveReqInv();

    //��ñ�����ַ
    const char* GetLocalAddress();

    //��ǰ����ģ��֪ͨ����ģ�������ָ��Ѿ���ɺ󣬱�����������
    virtual void Active(void);

    //�����ȥ����
    virtual void DeActive(void);

    //������GSN���ض�����Ϣ
    void SendRedirection(void);

    //���¼�������
    virtual void Timer1Sec(void);

    //���Ӧ������ģ���GSN����Э��֡
    virtual int SendFrame2Ex(MSG_INTRA* pMsg, CProtocolStack* pStack);

    //ͳ��GTP'�ظ���������֡��(GTP_REPEATDATA_FRAME)
    CPerfItem* m_pRptBillPerf;

    //ͳ��GTP'������������֡��(GTP_NORMALDATA_FRAME)
    CPerfItem* m_pNmlBillPerf;

    //ͳ��GTP'�����ظ�����֡��(GTP_ALL_FRAME)
    CPerfItem* m_pAllFrmPerf;

    //ͳ��������GSN��������(GTP_ALLGSN_CONNECT)
    CPerfItem* m_pGSNConnectPerf;

    //ͳ������Ķ��г���(PERF_GTP_QUEUE_SIZE)
    CPerfItem* m_pGtpQueueSize;

    //ͳ��GTP'֡��������(PERF_GTP_THROUGHPUT)
    CPerfItem* m_pGtpThroughPut;

	//ͳ��Ga�ӿ�ƽ����Ӧʱ��
	CPerfItem* m_pGtpResTime;

protected:
    //�»������г���
    int m_nMaxNewFrameCount;

    //����Echo Request֡��ʱ��������λΪ����
    int m_nEchoReqInv;

    //REQUEST��Ϣ���ͳ�ʱʱ��
    int m_nRequestTimeOut;

    //��ʱ�ط�����
    int m_nTimeOutRepeat;

    //����Node Alive Request֡��ʱ��������λΪ����(��������Ҫ���)
    int m_nNodeAliveReqInv;

    //������ַ
    char m_szLocalAddr[20];

    //�ض����Ƽ���ַ
    char m_szRedirectionAddr[20];

    //��IP��ַ��Ӧ��m_GsnMultiIPTable��GSN��������
    MAP<UINT4, int>        m_GsnMultiIPMap;

    //���ж��ַGSN��IP��ַ��
    VECTOR<VECTOR<UINT4> > m_GsnMultiIPTable;

    //��IP��ַ��CGtpStackָ���MAP
    MAP<UINT4, CGtpStack*> m_IPToStackMap;

    //��IP��ַ��Э��ջ����MAP
    MAP<UINT4, STRING>     m_IPToStackName;

    //��ǰ��Э��ջID
    UINT4                  m_uCurrentStackID;

    //��ɾ����GSN��IP��ַ��ֻ�Ե�IP��ַ��GSN��Ч
    VECTOR<UINT4>          m_DelGsnIPTable;
};

//CGtpTrack������CProtocolTrack��ʵ����GTP'Э������֡�ķ���
class CGtpTrack:public CProtocolTrack
{
public:
    CGtpTrack();
    virtual ~CGtpTrack();

    //ʵʩ��GTP'Э��֡�ķ���,�ɺ����ڲ������ڴ棬�����߲���Ҫ�ͷ�
    virtual const char* Analysis(void* const pFrame, int nProcRst = 0);

protected:

    //����Data Record Transfer��Ϣ���������ݴ�������
    void OnGTPDataTransferRequest(SGtpFrame *pFrame);
    void OnGTPDataTransferResponse(SGtpFrame *pFrame);

    void OnGTPEchoResponse(SGtpFrame *pFrame);
    void OnGTPSendData(SGtpFrame *pFrame);

    //�������֡
    void OnGTPCancelData(SGtpFrame *pFrame);
    void OnGTPReleaseData(SGtpFrame *pFrame);
    void OnGTPNodeAliveRequest(SGtpFrame *pFrame);
    void OnGTPRedirectionRequest(SGtpFrame *pFrame);
    void OnGTPRedirectionResponse(SGtpFrame *pFrame);

    //���Ӧ��ԭ��ֵ������
    const char* GetResponseCauseDesp(int nCause);

};

#endif //__GTP_RELATED_H__
