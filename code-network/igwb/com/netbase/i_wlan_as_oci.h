#ifndef __I_WLAN_AS_OCI_H__
#define __I_WLAN_AS_OCI_H__



class IWlanAsOci
{
public:
    IWlanAsOci(void){};
    virtual ~IWlanAsOci(void){};

    //��ʼ��
    virtual int Init(int nRecordSize,
                     int nLowAtSize,
                     int nMaxPackageRecord) = 0;

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ��
    //����Ļ������ݵ��ֽ���
    virtual int GetReadyDataSize(void) = 0;

    //���ر������е�ǰ�ѽ��յ��ҿ����ύ��ǰ����ģ��
    //����Ļ������ݵļ�¼��
    virtual int GetReadyRecordCount(void) = 0;

    //����Э��ջ���ύ��ǰ����ģ�鱣��Ļ������ݷ���pDataBuf��
    virtual int GetReadyData(char*& pDataBuf, int& nLen) = 0;

    //���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
    virtual void OnResponse(BOOL const bSaved) = 0;

    //���¼�������
    virtual void Timer1Sec(void) = 0;
    
};

#endif //__I_WLAN_AS_OCI_H__
