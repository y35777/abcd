#ifndef __BACKSTORAGE_H__
#define __BACKSTORAGE_H__

#include "../include/precom.h"
#include "../include/frame.h"
#include "patch_encap.h"

class CSaveChannel;
class CPerfItem;

//�������µ����ݽṹ������һ��ͨ������Ϣ
struct SChannelInfo
{
    CSaveChannel*   pSaveChannel;   //��Ӧ��ͨ������
    UINT4           uLastPID;       //��ͨ�������һ����������
};

class CBackStorage : public CWorkModule  
{
public:
    //���졢��������
    CBackStorage(const UINT4 uAPID);
    virtual ~CBackStorage();

protected:
    //�̳еĽӿں���
    int InitInstance();        //��ʼ������
    void ExitInstance();        //ģ���˳�ʱ��������
    void OnTimer(const long nTimerID);    //��ʱ���¼�������
    MODULE_TYPE GetModuleType();       //��ȡģ������
    void ParseMsg(MSG_INTRA* pMsgParsed); //ʵ����Ϣ�ַ�����ش���

    //�ڲ�����
    void ProcSavePacketMsg(MSG_INTRA* pMessage); //����"���������ջ�����"��Ϣ
    void ProcResumeMsg(MSG_INTRA* pMessage);    //����"���������ָ�"��Ϣ
    void ProcUpdateOverMsg(MSG_INTRA* pMessage);//����"֪ͨ�����������"��Ϣ
    void ProduceCDR(MSG_INTRA* pMessage);   

protected:
    //����ͨ�����������Ϣ�����飬�±�Ϊͨ����
    SChannelInfo* m_ChannelInfoArray;   
    UINT4         m_uChannelCount;      //ͨ����
    long          m_lSecondTimerID;     //���¼�ʱ��
    BOOL          m_bError;             //�Ƿ���ڴ���ı�־
    UINT4         m_uAPID;              //������
    UINT4         m_uApType;            //���������
    CPatchEncap   m_PatchEncap;         //�����������

    //by ldf 2004-02-24
    //�Ƿ�Ҫ�������ּ�ʽ
    char          m_szDispatchType[49];  //�����Ƿ�����UAE���͵ķּ�ʽ
	UINT4         m_uDispatchAllCDRCH;   //���ô�� ���л������� ��ͨ��
    //end
	
};

#endif
