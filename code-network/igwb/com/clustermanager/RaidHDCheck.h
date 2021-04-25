/*****************************************************
//		RaidHDCheck.h
//*****************************************************
//Copyright 2001 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       Wangfeng
// Date:         01-8-10
// Description:  ������ֻ֧��һ��IBM RAID����Ӳ�̼�⣬
//				 ������ڶ��RAID�����򴴽��������ÿ��
//				 ����ͨ�����캯�����벻ͬ�Ŀ������š�ÿ��
//				 �����⹹�캯����ָ�����ŵ�Ӳ��״̬
// �÷���		 1)��������
//				 2)����IsIBMRaidController()����Ƿ��
//				   ��ָ���������ŵ�IBM��RAID������������ڣ���ɾ������
//				 3)����CheckIBMRaidHD()
//				   ʵʩ�ÿ�����������Ӳ��״̬�ļ�⡣
//				 4)�˺�ɵ���GetFailedHDCount()��ȡ���ϵ�Ӳ������
//				 5)���μ��֮���ʱ�䲻��С�������ӣ�����������Ԥ�ϵ����⡣	
// 
// Side Effects: NONE
// Functions:
// Notes:    
// 
// Update: 01-6-18 ���� 02:35:29
// Date              Name              Description 
============== ================ ======================
## 01-6-18       Wangfeng
## 01-8-10       Wangfeng		1�����ӶԶ�RAID����֧��
								2���򻯼����̣�ȥ����ͨ���š�SCSI ID��ָ��
								3�����ټ����������Ӽ��İ�ȫ��
*****************************************************/
// IBMRaidHDCheck.h: interface for the CIBMRaidHDCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBMRAIDHDCHECK_H__0A9E3CB5_637F_11D5_BD5E_0050BA1A651E__INCLUDED_)
#define AFX_IBMRAIDHDCHECK_H__0A9E3CB5_637F_11D5_BD5E_0050BA1A651E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FIRST_SERVERAID 1
#define SECOND_SERVERAID 2
#define THIRD_SERVERAID 3

class CRaidHDCheck  //����Raid����״̬���Ļ���
{
public:
	CRaidHDCheck(int nController = FIRST_SERVERAID);
	virtual ~CRaidHDCheck();
	virtual int GetFailedHDID() = 0;		//���ع���Ӳ�̵�����
	virtual int GetFailedHDCount() = 0;		//���ع���Ӳ�̵���Ŀ

	virtual int CheckRaidHD() = 0 ;			//���m_nControllerָ��
											//������������Ӳ��״̬

protected:
	int m_nController;		//����������Ե�RAID����
};

enum HDSTATUS
{
	HD_NULL = 0,	//ʧ��
	HD_EMP,			//Empty
	HD_RDY,			//Ready
	HD_HSP,			//Hot spare
	HD_SHS,			//Standby hot spare
	HD_DDD,			//Defunct disk drive
	HD_DHS,			//Defunct hot spare
	HD_RBL,			//Rebuild
	HD_SBY,			//Standby
	HD_ONL			//Online
};

class CIBMRaidHDCheck:public CRaidHDCheck  
{
public:
	HDSTATUS GetIBMHDStatus(int nHDIndex);	//����ָ��Ӳ�̵�״̬
	int GetFailedHDID();					//���ع���Ӳ�̵�����
	int GetFailedHDCount();					//���ع���Ӳ�̵���Ŀ

	LPCSTR GetStatusDesp(HDSTATUS nStatus); //ȡָ��Ӳ��״̬������
	int CheckRaidHD();					    //���m_nControllerָ��
											//������������Ӳ��״̬

	static char * GetIBMRaidOutput();		//����m_nController������
											//��ѯ״̬������ı�

	static BOOL IsIBMRaidController();		//����Ƿ����IBM��RAID��
	CIBMRaidHDCheck(int nController = FIRST_SERVERAID);	
											//����RAID�������ţ������
											//���ж��RAID�����������
											//�����������Ķ��󣬽���
											//ͬ�Ŀ������ŷֱ𴫵�����
											//�����У�ȱʡΪ��һ�ſ�������
	virtual ~CIBMRaidHDCheck();

private:
	HDSTATUS m_HDStatus[16];				//��������Ӳ�̵�״̬

	HDSTATUS SearchHDStatusInString(char * pInBuf);
};


class CDellRaidHDCheck:public CRaidHDCheck  
{
public:
	CDellRaidHDCheck(int nController = FIRST_SERVERAID);
	virtual ~CDellRaidHDCheck();
	int GetFailedHDID();		//���ع���Ӳ�̵�����
	int GetFailedHDCount();		//���ع���Ӳ�̵���Ŀ
	int CheckRaidHD();			//���dellӲ��
	static BOOL IsDellRaidController();
private:
	int m_nCurrentHDStatus[32], m_nHDStatus[32];
	HANDLE hPerc;
};

#endif // !defined(AFX_IBMRAIDHDCHECK_H__0A9E3CB5_637F_11D5_BD5E_0050BA1A651E__INCLUDED_)
