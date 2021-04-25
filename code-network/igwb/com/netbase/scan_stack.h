#ifndef __SCAN_STACK_H__
#define __SCAN_STACK_H__


struct SFileListInfo
{
    time_t tFileTime;
    STRING strFilePath;
    bool operator<(const struct SFileListInfo& right) const
    {
        return (tFileTime < right.tFileTime);
    }
};

class CProtocolStack;
struct SScanStackInfo;
class CScanStack : public CProtocolStack
{
public:
    CScanStack(const char* const szPath, 
               CNetApp* const pNetApp, 
               UINT4 const uStackId);
    virtual ~CScanStack(void);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ��
    //����Ļ������ݵ��ֽ���
    virtual int GetReadyDataSize(void);

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ��
    //����Ļ������ݵļ�¼��
    virtual int GetReadyRecordCount(void);

    //����Э��ջ���ύ��ǰ����ģ�鱣��Ļ������ݷ���*pDataBuf������
    virtual void GetReadyData(CDataBuffer* const pDataBuf);

    //���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
    virtual void OnResponse(BOOL const bSaved);

    //����״̬�ļ��ж�ȡ��Э��ջ��״̬��Ϣ
    virtual int LoadStatus(void);

    //����״̬�ļ���д�뱾Э��ջ��״̬��Ϣ
    virtual int SaveStatus(void);

    //���¼�������
    virtual void Timer1Sec(void);

    //��ȡ����״̬
    BOOL GetFailureStatus();

protected:

    //���Ӧ������ģ���GSN����Э��֡
    virtual int SendFrame2Ex(void* pFrame);

    //����szPathĿ¼�������ļ�
    int PrepareFiles(const char* const szPath);

    LIST<SFileListInfo> m_FileList;

    //ɨ��������λ����
    int m_nScanInv;

    //ɨ����������
    int m_nScanCounter;

    //�Ƿ��ǹ���״̬
    int m_bFailure;

    //�Ƿ����ύ�ļ�
    BOOL m_bSubmitFile;

    //�ύ���ݻ�����ָ��
    char* m_pSubmitBuffer;
    int   m_nSubmitBufLen;

    //�ύ���ݻ���������Ч����
    int m_nSubmitLen;

    //Э��ջ��Ϣ
    SScanStackInfo m_ScanStackInfo;

    //��С�ɷָԪ����
    int m_nBillRecordSize;

    //�ͳ�����С
    int m_uLowAtSize;
};

#endif //__SCAN_STACK_H__
