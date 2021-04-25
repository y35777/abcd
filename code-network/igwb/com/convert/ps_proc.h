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

    void SetASN1Para(BYTE* pBuffer);    //设置和ASN1相关的参数
    virtual BOOL AddABill(SBill& ABill);
    //根据通道的配置参数，进行初始化
    virtual BOOL Init(UINT4 uAPID, INT4 nChannelID, BOOL bNeedConvert);

protected:
    //进行ASN1编码
    BOOL EncodeABill(const SBill* pABill, BYTE*& pBillBuffer, int& nLen);

    //转换一张话单
    BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, CFormat* pDestFmt);

	virtual void InitOSSLib(void);
	virtual void ReleaseOSSLib(void);

protected:
	PACKET_SWITCH_RELEASE		 m_nPSRelease;	     //分组域具体的协议版本(R98/R99/R4)
    BYTE*    m_pASN1RecBuffer;   //用于存放ASN1编码后的话单内容的内存
    void*    m_pASN1FinalGlobal; //ASN1编码时用到的环境变量
    CPSAsnEncap m_PSAsnClosor;
};

class CPSBillProcess : public CBillProcess  
{
public:
    CPSBillProcess(PACKET_SWITCH_RELEASE nPSRelease);
    virtual ~CPSBillProcess();

    BOOL Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge); //初始化函数

protected:
    //进行ASN1解码
    virtual int DecodeABill(SBill* pABill, SBill* pDecodedBill);
	virtual void InitOSSLib(void);
	virtual void ReleaseOSSLib(void);
    //将一张话单分拣到所属的通道并进行格式转换等话单处理
    virtual void Distribute(SBill *pABill);

    //话单预处理，将话单包拆分为单张话单
    BOOL PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill);

    //处理一包不需要合并的话单
    virtual BOOL ProcessABill(SBill& OrgBill, SBill& PreBill);
protected:
	PACKET_SWITCH_RELEASE   m_nPSRelease;	     //分组域具体的协议版本(R98/R99/R4)
    BYTE* m_pASN1RecContent; //用于存放ASN1解码后的话单内容的内存
    void* m_pASN1IniGlobal;  //ASN1解码时用到的环境变量
    char  m_szOMCfgFile[MAX_PATH];
    BOOL  m_bCheckOMField;

    //Added by ZhengYuqun SWPD04700
    BOOL  m_bCDRFilterAPNNI;             //按APN_NI过滤话单
    
    CPSAsnEncap m_PSAsnClosor;
	
	CPerfItem*	m_pTotalGCDR;		//接收的G-CDR总数统计
	CPerfItem*	m_pTotalPartGCDR;	//接收的部分G-CDR总数统计
	CPerfItem*	m_pTotalMCDR;		//接收的M-CDR总数统计
	CPerfItem*	m_pTotalPartMCDR;	//接收的部分M-CDR总数统计
	CPerfItem*	m_pTotalSCDR;		//接收的S-CDR总数统计
	CPerfItem*	m_pTotalPartSCDR;	//接收的部分S-CDR总数统计
	CPerfItem*	m_pTotalSMOCDR;		//接收的S-SMO-CDR总数统计
	CPerfItem*	m_pTotalSMTCDR;		//接收的S-SMT-CDR总数统计
	CPerfItem*	m_pTotalLCSMOCDR;	//接收的LCS-MO-CDR总数统计
	CPerfItem*	m_pTotalLCSMTCDR;	//接收的LCS-MT-CDR总数统计
	CPerfItem*	m_pTotalLCSNICDR;	//接收的LCS-NI-CDR总数统计
    CPerfItem*  m_pTotalCDR;        //处理的CDR总数统计
};


#endif //_PACKET_SWITCH_PROCESS_H_
