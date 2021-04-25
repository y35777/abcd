#ifndef     _GPRS_BILL_H
#define     _GPRS_BILL_H

#include "../../include/type_def.h"
#include <assert.h>
#include "../../merge/bill.h"
#include "../asn1_r98/CDRF_2GCDR_Org.h"
#include "../asn1_r98/CDRF_2GCDR_Fin.h"
#include "../asn1_r98/oss_asn1_r98.h"

#define GPRS_S_CDR     Org_CallEventRecordType_sgsnPDPRecord
#define GPRS_G_CDR     Org_CallEventRecordType_ggsnPDPRecord
#define GPRS_M_CDR     Org_CallEventRecordType_sgsnMMRecord
#define GPRS_SMO_CDR   Org_CallEventRecordType_sgsnSMORecord
#define GPRS_SMT_CDR   Org_CallEventRecordType_sgsnSMTRecord

#define MAX_MERGED_BILLS     255          //最大的话单合并限制


//CGPRS_BillParser继承自CBillParser类，管理GPRS话单类型和话单结构解析。
class CGPRS_BillParser:public CBillParser
{


protected:
    CBill* m_pBill;

public:
    CGPRS_BillParser();
    virtual ~CGPRS_BillParser();

    BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//根据pBuf生成一个话单对象。
    CBill* GetBillRecord();                     //返回话单记录对象。

    UINT4 GetBillTypeCount();                   //返回话单类型数。
    UINT4 GetBillTypeID(UINT4 uIndex);          //根据索引返回话单类型ID。
    void RegisterBillType(CMerger* pMerger);    //在CMerger对象中注册话单类型。
	//获得指定的话单类型ID的话单类型描述
	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
};

/****************************************************************************
模板T必须具有如下的定义：
    struct T
    {
		T* next;
		...
	};
****************************************************************************/
//清除指定链表中的所有节点
template <class T>
void ClearList(T*& pHead)
{
	T* pTmp = NULL;

	while (NULL != pHead)
	{
		pTmp = pHead->next;
		delete pHead;
		pHead = pTmp;
	}
}

//遍历链表，得到指定链表中的节点个数以及最后一个节点
template <class T>
int TravelList(const T* pHead, T*& pTail)
{
	int nCount = 0;
	T* pTmp = (T* )pHead;

	pTail = NULL;

	while (NULL != pTmp)
	{
		if (NULL == pTmp->next)
		{
			pTail = pTmp;
		}

		pTmp = pTmp->next;
		nCount++;
	}

	return nCount;
}

//克隆一个链表
template <class T>
void CloneList(const T* pSrcHead, T*& pDstHead)
{
	T* pNode = NULL;
	T* pSrcTmp = (T* )pSrcHead;
	T* pDstTmp = NULL;

	pDstHead = NULL;

	while (NULL != pSrcTmp)
	{
		pNode = new T;
		if (NULL == pNode)
		{
			throw "Fail to new T when clone list.";
		}
		memcpy(pNode, pSrcTmp, sizeof(T));
		pNode->next = NULL;
		if (NULL == pDstHead)
		{
			pDstHead = pDstTmp = pNode;
		}
		else
		{
			pDstTmp->next = pNode;
			pDstTmp = pNode;
		}

		pSrcTmp = pSrcTmp->next;
	}
}        

//本类封装了采用OSS编码工具的130版本话单的一些公共数据操作方法
class CGPRS_130_OSS_CDR : public CBill
{
	
protected:
    BOOL m_bDataPrepared;  //表示是否已经将原始话单数据转换为最终话单数据

public:
	//获取两个时刻间的时间间隔，以秒为单位
	Fin_CallDuration GetDuration(const Fin_TimeStamp* pSmallTime, 
								 const Org_TimeStamp* pLargeTime);
	//克隆Org_ManagementExtensions_的链表
	void CloneExtensionsList(const Org_ManagementExtensions_* pSrc, 
								   Org_ManagementExtensions_*& pDst);
	//删除Org_ManagementExtensions_的链表
	void ClearExtensionsList(Org_ManagementExtensions_*& pExtension);

protected:
	CGPRS_130_OSS_CDR();
	virtual ~CGPRS_130_OSS_CDR();

    //判断是否闰年。
    BOOL isLeapYear(int nYear);
    //输入年月，返回该月的天数。
    int GetDaysOfMonth(int nYear, int nMonth);
};

//S-CDR话单类
class CGPRS_130_OSS_SCDR : public CGPRS_130_OSS_CDR
{
#ifdef _ONLY_TEST_
	friend class CSCDRManipulate;
#endif

public:
	CGPRS_130_OSS_SCDR();
	virtual ~CGPRS_130_OSS_SCDR();

    BOOL Init(char* pBuf,UINT4 uLen); //使用话单记录初始化话单对象。

    BOOL CanSafeMerge(CBill* pBill);//判断当前话单是否允许与pBill合并。
    BOOL Merge(CBill* pBill);       //把pBill合入当前话单中。
    BOOL IsPDPCompleted();          //判断PDP是否已经结束。
    BOOL NeedWaiting();             //判断是否需要继续等待后续话单。

    char* GetContents();            //返回最终格式的话单记录

    BOOL IsPartial();               //判断是否部分话单。
    BOOL IsFirstPartialRec();       //判断是否PDP的第一张部分话单。
    BOOL IsLastPartialRec();        //判断是否PDP的末张部分话单。
    BOOL IsDuplicated(CBill* pBill);  //判断是否重复话单    

    UINT4 GetBillType();            //返回话单类型ID值。

    CKey CreateKey();               //创建索引对象。

	int GetQosLen();						//获得listOfTrafficVolumes链表的长度
	int GetExtensionLen();					//获得recordExtensions链表的长度
	//获得listOfTrafficVolumes链表的头和尾
	void GetQosList(Fin__seqof2_*& pHead, Fin__seqof2_*& pTail);
	//获得recordExtensions链表的头和尾
	void GetExtensionList(Fin_ManagementExtensions_*& pHead, 
						  Fin_ManagementExtensions_*& pTail);
	//去除listOfTrafficVolumes队列，在本话单被合并到其它话单后调用
	void RemoveQosList();					
	//去除recordExtensions队列，在本话单被合并到其它话单后调用
	void RemoveExtensionList();				
	const Org_SGSNPDPRecord* GetOrgRec();	//获取原始话单结构的指针
#ifdef _ONLY_TEST_
public:
#else
protected:
#endif

    BOOL PrepareData();         //把原始话单记录转换为符合最终格式的话单记录


	//各个链表的尾指针以及链表的长度
    Fin__seqof2_*               m_pQosTail;            
	int                         m_nQosLen;
    Fin_RecordNumberList_*      m_pSequenceTail;
	int                         m_nSequenceLen;
    Fin_ManagementExtensions_*  m_pExtensionTail;
	int                         m_nExtensionLen;

    Org_SGSNPDPRecord*   m_pOrgRec;            //原始话单。
    Fin_CallEventRecord* m_pDestEventRec;      //合并后话单。
	Fin_SGSNPDPRecord*   m_pDestRec;
};

//G-CDR话单类
class CGPRS_130_OSS_GCDR : public CGPRS_130_OSS_CDR
{
#ifdef _ONLY_TEST_
	friend class CGCDRManipulate;
#endif
public:
	CGPRS_130_OSS_GCDR();
	virtual ~CGPRS_130_OSS_GCDR();

    BOOL Init(char* pBuf,UINT4 uLen); //使用话单记录初始化话单对象。

    BOOL CanSafeMerge(CBill* pBill);//判断当前话单是否允许与pBill合并。
    BOOL Merge(CBill* pBill);       //把pBill合入当前话单中。
    BOOL IsPDPCompleted();          //判断PDP已经结束。
    BOOL NeedWaiting();             //判断是否需要继续等待后续话单。

    char* GetContents();            //返回最终格式的话单记录

    BOOL IsPartial();               //判断是否部分话单。
    BOOL IsFirstPartialRec();       //判断是否PDP的第一张部分话单。
    BOOL IsLastPartialRec();        //判断是否PDP的末张部分话单。
    BOOL IsDuplicated(CBill* pBill);   //判断是否重复话单
    
    UINT4 GetBillType();            //返回话单类型ID值。

    CKey CreateKey();               //创建索引对象。

	int GetSGSNLen();				//获得sgsnAddress链表的长度
	int GetPDPLen();				//获得remotePDPAddress链表的长度
	int GetQosLen();				//获得listOfTrafficVolumes链表的长度
	int GetExtensionLen();			//获得recordExtensions链表的长度
	//获得sgsnAddress链表的头和尾
	void GetSGSNList(Fin__seqof1_*& pHead, Fin__seqof1_*& pTail);
	//获得remotePDPAddress链表的头和尾
	void GetPDPList(Fin__seqof4*& pHead, Fin__seqof4*& pTail);
	//获得listOfTrafficVolumes链表的头和尾
	void GetQosList(Fin__seqof2_*& pHead, Fin__seqof2_*& pTail);
	//获得recordExtensions链表的头和尾
	void GetExtensionList(Fin_ManagementExtensions_*& pHead, 
						  Fin_ManagementExtensions_*& pTail);
	//去除sgsnAddress队列，在本话单被合并到其它话单后调用
	void RemoveSGSNList();
	//去除remotePDPAddress队列，在本话单被合并到其它话单后调用
	void RemovePDPList();
	//去除listOfTrafficVolumes队列，在本话单被合并到其它话单后调用
	void RemoveQosList();
	//去除recordExtensions队列，在本话单被合并到其它话单后调用
	void RemoveExtensionList();
	const Org_GGSNPDPRecord* GetOrgRec();	//获取原始话单结构的指针
#ifdef _ONLY_TEST_
public:
#else
protected:
#endif
    BOOL PrepareData();			//把原始话单记录转换为符合最终格式的话单记录

//protected:
	//各个链表的尾指针以及链表的长度
	Fin__seqof1_*				m_pSGSNTail;
	int							m_nSGSNLen;
	Fin__seqof4*				m_pPDPTail;
	int							m_nPDPLen;
	Fin__seqof2_*				m_pQosTail;
	int							m_nQosLen;
	Fin_RecordNumberList_*		m_pSequenceTail;
	int							m_nSequenceLen;
	Fin_ManagementExtensions_*	m_pExtensionTail;
	int							m_nExtensionLen;

	Org_GGSNPDPRecord*   m_pOrgRec;				//原始话单
    Fin_CallEventRecord* m_pDestEventRec;       //合并后话单。
	Fin_GGSNPDPRecord*   m_pDestRec;
};

//与CGPRS_130_OSS_SCDR话单类对应的索引类。
class CGPRS_130_OSS_SCDR_Key : public CKey
{
public:
    Org_GSNAddress  m_sgsnAddress;
    Org_GSNAddress  m_ggsnAddressUsed;
    Org_ChargingID  m_chargingID;

public:
    CGPRS_130_OSS_SCDR_Key();
    virtual ~CGPRS_130_OSS_SCDR_Key();

	void SetSGSNAddress(const Org_GSNAddress& sgsnAddress);
	void SetGGSNAddress(const Org_GSNAddress& ggsnAddress);
	void SetChargingID(const Org_ChargingID chargingID);
	const Org_GSNAddress& GetSGSNAddress();
	const Org_GSNAddress& GetGGSNAddress();
	Org_ChargingID GetChargingID();

    BOOL IsMatch(CKey* pKey);
    UINT4 GetKey();
};

//与CGPRS_130_OSS_GCDR话单类对应的索引类。
class CGPRS_130_OSS_GCDR_Key : public CKey
{
public:
    Org_GSNAddress  m_ggsnAddress;
    Org_ChargingID  m_chargingID;

public:
    CGPRS_130_OSS_GCDR_Key();
    virtual ~CGPRS_130_OSS_GCDR_Key();

	void SetGGSNAddress(const Org_GSNAddress& sgsnAddress);
	void SetChargingID(const Org_ChargingID chargingID);
	const Org_GSNAddress& GetGGSNAddress();
	Org_ChargingID GetChargingID();

    BOOL IsMatch(CKey* pKey);
    UINT4 GetKey();
};

//比较两个GSN地址
BOOL Match_gsnAddress(const Org_GSNAddress* p1, const Org_GSNAddress* p2);

#endif
