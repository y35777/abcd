#ifndef     _WLAN_BILL_H
#define     _WLAN_BILL_H

#include "../include/type_def.h"
#include <assert.h>
#define  WLAN_S_CDR   24
#include <memory.h>
#define WLAN_FINAL_CDR_LENGH 140
#define MAX_MERGED_BILLS     255          //最大的话单合并限制

//CWLAN_BillParser继承自CBillParser类，管理WLAN话单类型和话单结构解析。
class CWLAN_BillParser:public CBillParser
{

protected:
    CBill* m_pBill;

public:
    CWLAN_BillParser();
    virtual ~CWLAN_BillParser();

    BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//根据pBuf生成一个话单对象。
    CBill* GetBillRecord();                     //返回话单记录对象。

    UINT4 GetBillTypeCount();                   //返回话单类型数。
    UINT4 GetBillTypeID(UINT4 uIndex);          //根据索引返回话单类型ID。
    void RegisterBillType(CMerger* pMerger);    //在CMerger对象中注册话单类型。
	//获得指定的话单类型ID的话单类型描述
	void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc);
};


struct WLANListRecordSequenceNumber
{
	WLANListRecordSequenceNumber* pNext;
	UINT4 value;
	WLANListRecordSequenceNumber()
	{
		pNext = NULL;
		memset(&value, 0xcc, sizeof(value));
	}
};
typedef struct WLANListRecordSequenceNumber TWLANListRecordSequenceNumber;

//modify by chenliangwei, 2002-09-27, 第四期修改
enum    WLAN_CauseForRecClosing { 
	    //WLAN_NormalRelease = 0x01,
        //WLAN_TimeOut = 0x02,
		//WLAN_AbnormalRelease = 0x03,
        //WLAN_IdleTimeOut = 0x04,  //空闲超时
        //WLAN_SessionTimeOut = 0x05,
		
        //add by chenliangwei, 2002-12-11, AS透传AC的话终原因
        WLAN_UserRequest = 0x01,               //用户请求
        WLAN_LostCarrier = 0x02,               //无运营商        
        WLAN_LostService = 0x03,               //业务不存在
        WLAN_IdleTimeout = 0x04,               //空闲超时
        WLAN_SessionTimeout = 0x05,            //会话超时 
        WLAN_AdminReset = 0x06,                //管理员复位
        WLAN_AdminReboot = 0x07,               //管理员重起 
        WLAN_PortError = 0x08,                 //端口错误
        WLAN_NASError = 0x09,                  //NAS错误
        WLAN_NASRequest = 0x0A,                //NAS请求
        WLAN_NASReboot = 0x0B,                 //NAS重起
        WLAN_PortUnneeded = 0x0C,              //端口不再需要
        WLAN_PortPreempted = 0x0D,             //端口已占用
        WLAN_PortSuspended = 0x0E,             //端口挂起
        WLAN_ServiceUnavailable = 0x0F,        //业务繁忙
        WLAN_Callback = 0x10,                  //回呼
        WLAN_UserError = 0x11,                 //用户错误
        WLAN_HostRequest = 0x12,               //主机请求
        //add end
        WLAN_PartialRec = 0xFE  //部分话单
};



//必须定义的话单结构如下：
//合并前的话单结构

typedef struct struct_WLAN_ORIGIAL_CDR
{
	BYTE ucRecordType; 		   // Rec_type
	UINT4 ulLocalSequenceNumber;   // AS设备的话单发送序列号
	UINT4 ulChargingID; 		   // 计费ID
	UINT4 ulRecordSequenceNumber;  // 一次连接的中间话单序列号
	BYTE ucOperType;			   //Oper_type
	BYTE ucOperID; 			   //Oper_ID
	BYTE ucServedMSISDN[8];	   // Charge_Dn, 即MSISDN，不足位F填充
	BYTE ucServedIMSI[8];		   // IMSI，不足位F填充
	BYTE ucUserType;			   // User_type
	BYTE ucAuthType;			   // Auth_type
	BYTE ucStartTime[14];		   // YYYYMMDDHHMISS,该张话单的计费开始时间
	BYTE ucStopTime[14];		   // YYYYMMDDHHMISS,该张话单的计费停止时间
	UINT4 ulDuration;			   // 该张话单的时长
	UINT4 ulDataFlowUp; 		   // 该张话单的上行数据流量
	UINT4 ulDataFlowDn; 		   // 该张话单的下行数据流量
//	BYTE ucHotSpotID[14];		   // Hotspot_ID
	BYTE ucHotSpotID[16];		   // Hotspot_ID modify by chenliangwei, 2002-09-27, 第四期修改
	UINT4 ulACAddress;			   // AC_address
	UINT4 ulASAddress;			   // AS_address
    BYTE ucServiceID[8];          // Service_ID add by chenliangwei, 2002-09-27, 第四期修改
    BYTE ucIspID[6];              // ISP_ID   add by chenliangwei, 2002-09-27, 第四期修改
	BYTE ucCloseCause; 		   // Logout_Cause，中间话单的原因值为0xFE

    BYTE ucDomain[32];           // 移动话单中Charge_dn中的域名部分, add by lucy 2005.5.25
}WLAN_ORIGIAL_CDR;


//合并过程中的话单结构
typedef struct
{
	UINT4 ulLocalSequenceNumber;   // AS设备的话单发送序列号
	UINT4 ulChargingID; 		   // 计费ID
	TWLANListRecordSequenceNumber *    pRecordSequenceNumber;
	BYTE ucRecordType; 		   // Rec_type
	BYTE ucOperType;			   //Oper_type
	BYTE ucOperID; 			   //Oper_ID
	BYTE ucServedMSISDN[8];	   // Charge_Dn, 即MSISDN，不足位F填充
	BYTE ucServedIMSI[8];		   // IMSI，不足位F填充
	BYTE ucUserType;			   // User_type
	BYTE ucAuthType;			   // Auth_type
	BYTE ucStartTime[14];		   // YYYYMMDDHHMISS,该张话单的计费开始时间
	BYTE ucStopTime[14];		   // YYYYMMDDHHMISS,该张话单的计费停止时间
	UINT4 ulDuration;			   // 该张话单的时长
	INT8 ulDataFlowUp;		   // 该张话单的上行数据流量
	INT8 ulDataFlowDn;		   // 该张话单的下行数据流量
//	BYTE ucHotSpotID[14];		   // Hotspot_ID
	BYTE ucHotSpotID[16];		   // Hotspot_ID modify by chenliangwei, 2002-09-27, 第四期修改
	UINT4 ulACAddress;			   // AC_address
	UINT4 ulASAddress;			   // AS_address
    BYTE ucServiceID[8];          // Service_ID add by chenliangwei, 2002-09-27, 第四期修改
    BYTE ucIspID[6];              // ISP_ID   add by chenliangwei, 2002-09-27, 第四期修改
	BYTE ucCloseCause; 		   // Logout_Cause，中间话单的原因值为0xFE	 

    BYTE ucDomain[32];           // 移动话单中Charge_dn中的域名部分, add by lucy 2005.5.25
}WLAN_INPROCESS_CDR;
//合并以后的话单结构，
typedef struct
{
	BYTE ucRecordType; 		   // Rec_type
	BYTE ucOperType;			   //Oper_type
	BYTE ucOperID; 			   //Oper_ID
	BYTE ucServedMSISDN[8];	   // Charge_Dn, 即MSISDN，不足位F填充
	BYTE ucServedIMSI[8];		   // IMSI，不足位F填充
	BYTE ucUserType;			   // User_type
	BYTE ucAuthType;			   // Auth_type
	BYTE ucStartTime[14];		   // YYYYMMDDHHMISS,该张话单的计费开始时间
	BYTE ucStopTime[14];		   // YYYYMMDDHHMISS,该张话单的计费停止时间
	UINT4 ulDuration;			   // 该张话单的时长
	INT8 ulDataFlowUp;		   // 该张话单的上行数据流量
	INT8 ulDataFlowDn;		   // 该张话单的下行数据流量
//	BYTE ucHotSpotID[14];		   // Hotspot_ID
	BYTE ucHotSpotID[16];		   // Hotspot_ID modify by chenliangwei, 2002-09-27, 第四期修改
	UINT4 ulACAddress;			   // AC_address
	UINT4 ulASAddress;			   // AS_address
    BYTE ucServiceID[8];          // Service_ID add by chenliangwei, 2002-09-27, 第四期修改
    BYTE ucIspID[6];              // ISP_ID   add by chenliangwei, 2002-09-27, 第四期修改
	BYTE ucCloseCause; 		   // Logout_Cause，中间话单的原因值为0xFE
	
    BYTE ucDomain[32];           // 移动话单中Charge_dn中的域名部分, add by lucy 2005.5.25
} WLAN_FINAL_CDR;  

class CWLANBill_Key : public CKey  
{
public:
	UINT4 m_ulASaddress;
	UINT4 m_ulchargingID;
	CWLANBill_Key(){};
	virtual ~CWLANBill_Key(){};
	BOOL IsMatch(CKey *pKey)
	{
		CWLANBill_Key* p = (CWLANBill_Key*)pKey;
		return ((p->m_ulASaddress == m_ulASaddress) && (p->m_ulchargingID == m_ulchargingID));
	}
	UINT4 GetKey()
	{
		return m_ulchargingID;
	}
	
	
};

class CWLANBill : public CBill	
{
public:
	void SerialDstBill(char * pBuffer);
	void SerialBillBuf(char * pIn, WLAN_ORIGIAL_CDR *pOut);
	TWLANListRecordSequenceNumber *m_pSequenceTail;
	BOOL Merge(CBill* pBill);
	BOOL CanSafeMerge(CBill* pBill);
	BOOL PrepareData();
	BOOL Init(char* pBuf,UINT4 nLen);
	char* GetContents();
	UINT4 GetBillType();
	CKey CreateKey();
	BOOL IsPDPCompleted();
	BOOL NeedWaiting();
	BOOL IsLastPartialRec();
	BOOL IsFirstPartialRec();
    BOOL IsDuplicated(CBill* pBill);  //判断是否重复话单    
	BOOL IsPartial();
	WLAN_INPROCESS_CDR *m_pInProcessRec;
	WLAN_ORIGIAL_CDR *m_pOrgRec;
	CWLANBill();
	virtual ~CWLANBill();
	INT4 GetLength();
	
protected:
	BOOL m_bDataPrepared;
};


#endif
