#ifndef __BILLPROCESSOR_H__
#define __BILLPROCESSOR_H__

#include "bill_proc.h"
#include "../include/frame.h"
#include "../include/perf_item.h"

class CBPCreator  
{
public:
    CBPCreator();
    virtual ~CBPCreator();

    //���ݽ�������ʹ�����Ӧ��CBillProcess������Ķ���
	//2003-06-02 edit by ggf
    static CBillProcess* CreateBillProcess(AP_TYPE typeID, const char* szReleaseVer = "");
	//end of edit ggf
};

class CBillProcessor : public CWorkModule  
{
public:
    CBillProcessor(const UINT4 uAPID, CPacketCache* pPacketCache);
    virtual ~CBillProcessor();

    MODULE_TYPE GetModuleType(); //��ȡģ������
    int InitInstance();        //��ʼ������
    void ExitInstance();        //ģ���˳�ʱ��������
    void ParseMsg(MSG_INTRA* pMsgParsed);   //��Ϣ�ַ�����
	
protected:
    void ProcBillProcMsg(MSG_INTRA* pMsg);  //����"����Ʒ�Ԥ����"��Ϣ
    void ProcResumeMsg(MSG_INTRA* pMsg);    //����"���������ָ�"��Ϣ
    void ProcSynInfoMsg(MSG_INTRA* pMsg);   //����"֪ͨ״̬"����Ϣ
    void OnTimer(const long nTimerID);		//��ʱ��������

protected:
    CPacketCache* m_pPacketCache;       //������������
    CBillProcess* m_pBillProcess;       //�����������
    UINT4         m_uAPID;              //��ǰ�Ľ����ID��
    INT4          m_nChannelCount;      //����ͨ����������ͨ��0��
	long          m_nTimerID1Sec;		//1�붨ʱ��ID

	//�����ָ����ʱ�����쳣�����ﵽָ���ĸ�����������澯
	int           m_nCDRCheckSpan;      //�쳣�������ֵ�ʱ������
	int           m_nCDRCheckCount;     //�쳣�������ֵĴ�������
};

#endif
