#ifndef _PACKET_SWITCH_PROCESS_H_
#define _PACKET_SWITCH_PROCESS_H_

#include "proc_chanl.h"
#include "bill_proc.h"
#include "../toolbase/ps_asn_encap.h"
#include "../include/perf_item.h"

enum PACKET_SWITCH_RELEASE
{
	PACKET_SWITCH_UNKNOWN	= 0,
	PACKET_SWITCH_R98		= 1,
	PACKET_SWITCH_R99		= 2,
	PACKET_SWITCH_R4		= 3
};

class CPSChannel : public CProcChannel  
{
public:
    CPSChannel(PACKET_SWITCH_RELEASE nPSRelease);
    virtual ~CPSChannel();

    void SetASN1Para(BYTE* pBuffer);    //���ú�ASN1��صĲ���
    virtual BOOL AddABill(SBill& ABill);
    //����ͨ�������ò��������г�ʼ��
    virtual BOOL Init(UINT4 uAPID, INT4 nChannelID, BOOL bNeedConvert);

protected:
    //����ASN1����
    BOOL EncodeABill(const SBill* pABill, BYTE*& pBillBuffer, int& nLen);

    //ת��һ�Ż���
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, CFormat* pDestFmt);

	virtual void InitOSSLib(void);
	virtual void ReleaseOSSLib(void);

protected:
	PACKET_SWITCH_RELEASE		 m_nPSRelease;	     //����������Э��汾(R98/R99/R4)
    BYTE*    m_pASN1RecBuffer;   //���ڴ��ASN1�����Ļ������ݵ��ڴ�
    void*    m_pASN1FinalGlobal; //ASN1����ʱ�õ��Ļ�������
    CPSAsnEncap m_PSAsnClosor;
};

class CPSBillProcess : public CBillProcess  
{
public:
    CPSBillProcess(PACKET_SWITCH_RELEASE nPSRelease);
    virtual ~CPSBillProcess();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //��ʼ������

protected:
    //����ASN1����
    virtual int DecodeABill(SBill* pABill, SBill* pDecodedBill);
	virtual void InitOSSLib(void);
	virtual void ReleaseOSSLib(void);
    //��һ�Ż����ּ�������ͨ�������и�ʽת���Ȼ�������
    virtual void Distribute(SBill *pABill);

    //����Ԥ���������������Ϊ���Ż���
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);

    //����һ������Ҫ�ϲ��Ļ���
    virtual BOOL ProcessABill(SBill& OrgBill, SBill& PreBill);
protected:
	PACKET_SWITCH_RELEASE   m_nPSRelease;	     //����������Э��汾(R98/R99/R4)
    BYTE* m_pASN1RecContent; //���ڴ��ASN1�����Ļ������ݵ��ڴ�
    void* m_pASN1IniGlobal;  //ASN1����ʱ�õ��Ļ�������
    char  m_szOMCfgFile[MAX_PATH];
    BOOL  m_bCheckOMField;

    //Added by ZhengYuqun SWPD04700
    BOOL  m_bCDRFilterAPNNI;             //��APN_NI���˻���
    
    CPSAsnEncap m_PSAsnClosor;
	
	CPerfItem*	m_pTotalGCDR;		//���յ�G-CDR����ͳ��
	CPerfItem*	m_pTotalPartGCDR;	//���յĲ���G-CDR����ͳ��
	CPerfItem*	m_pTotalMCDR;		//���յ�M-CDR����ͳ��
	CPerfItem*	m_pTotalPartMCDR;	//���յĲ���M-CDR����ͳ��
	CPerfItem*	m_pTotalSCDR;		//���յ�S-CDR����ͳ��
	CPerfItem*	m_pTotalPartSCDR;	//���յĲ���S-CDR����ͳ��
	CPerfItem*	m_pTotalSMOCDR;		//���յ�S-SMO-CDR����ͳ��
	CPerfItem*	m_pTotalSMTCDR;		//���յ�S-SMT-CDR����ͳ��
	CPerfItem*	m_pTotalLCSMOCDR;	//���յ�LCS-MO-CDR����ͳ��
	CPerfItem*	m_pTotalLCSMTCDR;	//���յ�LCS-MT-CDR����ͳ��
	CPerfItem*	m_pTotalLCSNICDR;	//���յ�LCS-NI-CDR����ͳ��
    CPerfItem*  m_pTotalCDR;        //�����CDR����ͳ��
};


#endif //_PACKET_SWITCH_PROCESS_H_
