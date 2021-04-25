#ifndef __BILLPROCESS_H__
#define __BILLPROCESS_H__

#include "../include/precom.h"
#include "convert_comm.h"
#include "../include/format_out.h"
#include "proc_chanl.h"
#include "../include/merge.h"

class CPerfItem;
class CBillProcess  
{
public:
    //���졢��������
    CBillProcess();
    virtual ~CBillProcess();

    //�������úͻ�ȡ����
    INT4 GetChannelCount();

    //��ʼ������
    virtual BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); 
	//�����뻰���ϲ���صĳ�ʼ��
	virtual BOOL InitMerge(int nHashSize, int nMValue, int nMaxQwLen);
    void SetUpdateFlag(BOOL bIsAssistUpdate);            //���ø���������־
    BOOL IsAssistUpdateOver();                           //�жϸ��������Ƿ����
    void ProcessSynInfo(const BYTE* pInfo, UINT4 uLen);  //����ͬ����Ϣ
    //���ݺ����ģ�����󱣴���ż����ǰ���̵Ļָ���ʼ����
    UINT4 GetResumePID(UINT4 uLastPID);
    //��һ�Ż����ּ�������ͨ�������и�ʽת���Ȼ�������
    virtual void Distribute(SBill *pABill);
	virtual void SpecDistribute(SBill *pABill);
    //����һ������
    void ProcessBillPacket(UINT4 uPID, BYTE* pPacket, UINT4 uPacketLen, 
                           UINT4& uFrontBillCount, UINT4& uBackBillCount, 
                           UINT4& uMergeTime, UINT4& uProcessTime);
    //����һ������Ҫ�ϲ��Ļ���
    virtual BOOL ProcessABill(SBill& OrgBill, SBill& PreBill);

    //�õ�ĳһָ��ͨ���µ����ջ�����
    void GetChannelPacket(UINT4 uChannelID, BYTE*& pPacket, UINT4& uPacketLen);
    //�õ�ָ��ͨ���µ�ͬ����Ϣ
    SChannelSynInfo* GetChannelSynInfo(UINT4 uChannelID);

	int GetErrorCDRCount(void);		//����쳣������
	void ResetErrorCDRCount();		//��λ�쳣������
	
protected:
    //�ڻ������ȹ̶�������£��ӻ������в�ֳ�һ�Ż�����
    //��128��GSM��STP�Ȳ�Ʒ����Ч����PreProcess��������
    BOOL GetFixedLenBill(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
    //�ڻ������еĻ�����ŷ�ʽΪ���ȣ��������ݵ�����£�
    //�ӻ������в�ֳ�һ�Ż�������GPRS��3G�Ȳ�Ʒ����Ч����PreProcess��������
    BOOL GetVarLenBill(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);
    //����Ԥ���������������Ϊ���Ż���
    virtual BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill) = 0;

protected:
    IBillProcessor* m_pIBillProcessor;  //��������ӿ�
    //�ּ���ͨ������ͨ�����±��ʾͨ���ţ������Ԫ��ΪCProcChannel���ָ��
    CProcChannel** m_ChannelArray;      
    //��ǰ����Ļ������Ƿ�Ϊ�հ������жϸ��������Ƿ����ʱʹ��
    BOOL     m_bEmptyPacket;
    BOOL     m_bIsAssistUpdate; //����������־
    CIGWB*   m_pIGWB;           //��ʽ��ӿڶ���
    INT4     m_nChannelCount;   //ͨ����Ŀ������ͨ��0��
    UINT4    m_uOrigLen;        //ԭʼ�������ȣ���ԭʼ�������ȶ���ʱ��Ч
    CMerger* m_pMerge;          //�����ϲ����󣬵�������Ҫ�ϲ�ʱ��Ч
	CBillParser* m_pParser;     //�����������󣬵�������Ҫ�ϲ�ʱ��Ч
    BOOL     m_bBillRemain;     //�Ƿ���ڵȴ��ϲ�������־
    UINT4    m_uAPID;           //��ǰ�Ľ����ID��
    BOOL     m_bNeedConvert;    //�Ƿ���Ҫ��ʽת��
    BOOL     m_bNeedMerge;      //�Ƿ���Ҫ�ϲ�
    BOOL     m_bSupportDST;     //Z30971���� �Ƿ�֧����ʱ�� 
    int      m_nErrorCDRCount;	//�쳣������
    UINT4    m_uMaxDuration;    //�ϲ�ʱ������ͨ��ʱ�������� SWPD02238 ZhengYuqun 2003-11-04
    BOOL     m_bProcBillCsn;    //�Ƿ�Ҫ�Ի������кŽ������⴦��
	CPerfItem* m_pMergeQueue;
	int      m_nReDistribute;  //ʯ��ׯ����ּ�
};

#endif 
