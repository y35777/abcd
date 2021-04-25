#ifndef     _WLAN_BILL_H
#define     _WLAN_BILL_H

#include "../include/type_def.h"
#include <assert.h>
#define  WLAN_S_CDR   24
#include <memory.h>
#define WLAN_FINAL_CDR_LENGH 140
#define MAX_MERGED_BILLS     255          //���Ļ����ϲ�����

//CWLAN_BillParser�̳���CBillParser�࣬����WLAN�������ͺͻ����ṹ������
class CWLAN_BillParser:public CBillParser
{

protected:
    CBill* m_pBill;

public:
    CWLAN_BillParser();
    virtual ~CWLAN_BillParser();

    BOOL ParseARecord(char* pBuf,UINT4 uBufLen);//����pBuf����һ����������
    CBill* GetBillRecord();                     //���ػ�����¼����

    UINT4 GetBillTypeCount();                   //���ػ�����������
    UINT4 GetBillTypeID(UINT4 uIndex);          //�����������ػ�������ID��
    void RegisterBillType(CMerger* pMerger);    //��CMerger������ע�Ự�����͡�
	//���ָ���Ļ�������ID�Ļ�����������
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

//modify by chenliangwei, 2002-09-27, �������޸�
enum    WLAN_CauseForRecClosing { 
	    //WLAN_NormalRelease = 0x01,
        //WLAN_TimeOut = 0x02,
		//WLAN_AbnormalRelease = 0x03,
        //WLAN_IdleTimeOut = 0x04,  //���г�ʱ
        //WLAN_SessionTimeOut = 0x05,
		
        //add by chenliangwei, 2002-12-11, AS͸��AC�Ļ���ԭ��
        WLAN_UserRequest = 0x01,               //�û�����
        WLAN_LostCarrier = 0x02,               //����Ӫ��        
        WLAN_LostService = 0x03,               //ҵ�񲻴���
        WLAN_IdleTimeout = 0x04,               //���г�ʱ
        WLAN_SessionTimeout = 0x05,            //�Ự��ʱ 
        WLAN_AdminReset = 0x06,                //����Ա��λ
        WLAN_AdminReboot = 0x07,               //����Ա���� 
        WLAN_PortError = 0x08,                 //�˿ڴ���
        WLAN_NASError = 0x09,                  //NAS����
        WLAN_NASRequest = 0x0A,                //NAS����
        WLAN_NASReboot = 0x0B,                 //NAS����
        WLAN_PortUnneeded = 0x0C,              //�˿ڲ�����Ҫ
        WLAN_PortPreempted = 0x0D,             //�˿���ռ��
        WLAN_PortSuspended = 0x0E,             //�˿ڹ���
        WLAN_ServiceUnavailable = 0x0F,        //ҵ��æ
        WLAN_Callback = 0x10,                  //�غ�
        WLAN_UserError = 0x11,                 //�û�����
        WLAN_HostRequest = 0x12,               //��������
        //add end
        WLAN_PartialRec = 0xFE  //���ֻ���
};



//���붨��Ļ����ṹ���£�
//�ϲ�ǰ�Ļ����ṹ

typedef struct struct_WLAN_ORIGIAL_CDR
{
	BYTE ucRecordType; 		   // Rec_type
	UINT4 ulLocalSequenceNumber;   // AS�豸�Ļ����������к�
	UINT4 ulChargingID; 		   // �Ʒ�ID
	UINT4 ulRecordSequenceNumber;  // һ�����ӵ��м仰�����к�
	BYTE ucOperType;			   //Oper_type
	BYTE ucOperID; 			   //Oper_ID
	BYTE ucServedMSISDN[8];	   // Charge_Dn, ��MSISDN������λF���
	BYTE ucServedIMSI[8];		   // IMSI������λF���
	BYTE ucUserType;			   // User_type
	BYTE ucAuthType;			   // Auth_type
	BYTE ucStartTime[14];		   // YYYYMMDDHHMISS,���Ż����ļƷѿ�ʼʱ��
	BYTE ucStopTime[14];		   // YYYYMMDDHHMISS,���Ż����ļƷ�ֹͣʱ��
	UINT4 ulDuration;			   // ���Ż�����ʱ��
	UINT4 ulDataFlowUp; 		   // ���Ż�����������������
	UINT4 ulDataFlowDn; 		   // ���Ż�����������������
//	BYTE ucHotSpotID[14];		   // Hotspot_ID
	BYTE ucHotSpotID[16];		   // Hotspot_ID modify by chenliangwei, 2002-09-27, �������޸�
	UINT4 ulACAddress;			   // AC_address
	UINT4 ulASAddress;			   // AS_address
    BYTE ucServiceID[8];          // Service_ID add by chenliangwei, 2002-09-27, �������޸�
    BYTE ucIspID[6];              // ISP_ID   add by chenliangwei, 2002-09-27, �������޸�
	BYTE ucCloseCause; 		   // Logout_Cause���м仰����ԭ��ֵΪ0xFE

    BYTE ucDomain[32];           // �ƶ�������Charge_dn�е���������, add by lucy 2005.5.25
}WLAN_ORIGIAL_CDR;


//�ϲ������еĻ����ṹ
typedef struct
{
	UINT4 ulLocalSequenceNumber;   // AS�豸�Ļ����������к�
	UINT4 ulChargingID; 		   // �Ʒ�ID
	TWLANListRecordSequenceNumber *    pRecordSequenceNumber;
	BYTE ucRecordType; 		   // Rec_type
	BYTE ucOperType;			   //Oper_type
	BYTE ucOperID; 			   //Oper_ID
	BYTE ucServedMSISDN[8];	   // Charge_Dn, ��MSISDN������λF���
	BYTE ucServedIMSI[8];		   // IMSI������λF���
	BYTE ucUserType;			   // User_type
	BYTE ucAuthType;			   // Auth_type
	BYTE ucStartTime[14];		   // YYYYMMDDHHMISS,���Ż����ļƷѿ�ʼʱ��
	BYTE ucStopTime[14];		   // YYYYMMDDHHMISS,���Ż����ļƷ�ֹͣʱ��
	UINT4 ulDuration;			   // ���Ż�����ʱ��
	INT8 ulDataFlowUp;		   // ���Ż�����������������
	INT8 ulDataFlowDn;		   // ���Ż�����������������
//	BYTE ucHotSpotID[14];		   // Hotspot_ID
	BYTE ucHotSpotID[16];		   // Hotspot_ID modify by chenliangwei, 2002-09-27, �������޸�
	UINT4 ulACAddress;			   // AC_address
	UINT4 ulASAddress;			   // AS_address
    BYTE ucServiceID[8];          // Service_ID add by chenliangwei, 2002-09-27, �������޸�
    BYTE ucIspID[6];              // ISP_ID   add by chenliangwei, 2002-09-27, �������޸�
	BYTE ucCloseCause; 		   // Logout_Cause���м仰����ԭ��ֵΪ0xFE	 

    BYTE ucDomain[32];           // �ƶ�������Charge_dn�е���������, add by lucy 2005.5.25
}WLAN_INPROCESS_CDR;
//�ϲ��Ժ�Ļ����ṹ��
typedef struct
{
	BYTE ucRecordType; 		   // Rec_type
	BYTE ucOperType;			   //Oper_type
	BYTE ucOperID; 			   //Oper_ID
	BYTE ucServedMSISDN[8];	   // Charge_Dn, ��MSISDN������λF���
	BYTE ucServedIMSI[8];		   // IMSI������λF���
	BYTE ucUserType;			   // User_type
	BYTE ucAuthType;			   // Auth_type
	BYTE ucStartTime[14];		   // YYYYMMDDHHMISS,���Ż����ļƷѿ�ʼʱ��
	BYTE ucStopTime[14];		   // YYYYMMDDHHMISS,���Ż����ļƷ�ֹͣʱ��
	UINT4 ulDuration;			   // ���Ż�����ʱ��
	INT8 ulDataFlowUp;		   // ���Ż�����������������
	INT8 ulDataFlowDn;		   // ���Ż�����������������
//	BYTE ucHotSpotID[14];		   // Hotspot_ID
	BYTE ucHotSpotID[16];		   // Hotspot_ID modify by chenliangwei, 2002-09-27, �������޸�
	UINT4 ulACAddress;			   // AC_address
	UINT4 ulASAddress;			   // AS_address
    BYTE ucServiceID[8];          // Service_ID add by chenliangwei, 2002-09-27, �������޸�
    BYTE ucIspID[6];              // ISP_ID   add by chenliangwei, 2002-09-27, �������޸�
	BYTE ucCloseCause; 		   // Logout_Cause���м仰����ԭ��ֵΪ0xFE
	
    BYTE ucDomain[32];           // �ƶ�������Charge_dn�е���������, add by lucy 2005.5.25
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
    BOOL IsDuplicated(CBill* pBill);  //�ж��Ƿ��ظ�����    
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
