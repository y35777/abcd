/*****************************************************
//		RaidHDCheck.h
//*****************************************************
//Copyright 2001 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       Wangfeng
// Date:         01-8-10
// Description:  本程序只支持一个IBM RAID卡的硬盘检测，
//				 如果存在多个RAID卡，则创建多个对象，每个
//				 对象通过构造函数输入不同的控制器号。每个
//				 对象检测构造函数中指定卡号的硬盘状态
// 用法：		 1)创建对象
//				 2)调用IsIBMRaidController()检测是否存
//				   在指定控制器号的IBM的RAID卡，如果不存在，则删除对象
//				 3)调用CheckIBMRaidHD()
//				   实施该控制器下所有硬盘状态的检测。
//				 4)此后可调用GetFailedHDCount()获取故障的硬盘数。
//				 5)二次检测之间的时间不能小于三分钟，否则会出不可预料的问题。	
// 
// Side Effects: NONE
// Functions:
// Notes:    
// 
// Update: 01-6-18 下午 02:35:29
// Date              Name              Description 
============== ================ ======================
## 01-6-18       Wangfeng
## 01-8-10       Wangfeng		1、增加对多RAID卡的支持
								2、简化检测过程，去掉对通道号、SCSI ID的指定
								3、减少检测次数，增加检测的安全性
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

class CRaidHDCheck  //所有Raid磁盘状态检测的基类
{
public:
	CRaidHDCheck(int nController = FIRST_SERVERAID);
	virtual ~CRaidHDCheck();
	virtual int GetFailedHDID() = 0;		//返回故障硬盘的掩码
	virtual int GetFailedHDCount() = 0;		//返回故障硬盘的数目

	virtual int CheckRaidHD() = 0 ;			//检测m_nController指定
											//控制器的所有硬盘状态

protected:
	int m_nController;		//本检测程序针对的RAID卡号
};

enum HDSTATUS
{
	HD_NULL = 0,	//失败
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
	HDSTATUS GetIBMHDStatus(int nHDIndex);	//返回指定硬盘的状态
	int GetFailedHDID();					//返回故障硬盘的掩码
	int GetFailedHDCount();					//返回故障硬盘的数目

	LPCSTR GetStatusDesp(HDSTATUS nStatus); //取指定硬盘状态的描述
	int CheckRaidHD();					    //检测m_nController指定
											//控制器的所有硬盘状态

	static char * GetIBMRaidOutput();		//返回m_nController控制器
											//查询状态的输出文本

	static BOOL IsIBMRaidController();		//检测是否存在IBM的RAID卡
	CIBMRaidHDCheck(int nController = FIRST_SERVERAID);	
											//输入RAID控制器号，如果本
											//机有多个RAID控制器，则可
											//创建多个本类的对象，将不
											//同的控制器号分别传到构造
											//函数中，缺省为第一号控制器。
	virtual ~CIBMRaidHDCheck();

private:
	HDSTATUS m_HDStatus[16];				//存在所有硬盘的状态

	HDSTATUS SearchHDStatusInString(char * pInBuf);
};


class CDellRaidHDCheck:public CRaidHDCheck  
{
public:
	CDellRaidHDCheck(int nController = FIRST_SERVERAID);
	virtual ~CDellRaidHDCheck();
	int GetFailedHDID();		//返回故障硬盘的掩码
	int GetFailedHDCount();		//返回故障硬盘的数目
	int CheckRaidHD();			//检测dell硬盘
	static BOOL IsDellRaidController();
private:
	int m_nCurrentHDStatus[32], m_nHDStatus[32];
	HANDLE hPerc;
};

#endif // !defined(AFX_IBMRAIDHDCHECK_H__0A9E3CB5_637F_11D5_BD5E_0050BA1A651E__INCLUDED_)
