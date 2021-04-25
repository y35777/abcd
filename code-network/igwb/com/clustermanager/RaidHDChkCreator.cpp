// RaidHDChkCreator.cpp: implementation of the RaidHDChkCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/frame.h"
#include "RaidHDChkCreator.h"
#include "RaidHDCheck.h"
#include "resource.h"

CRaidHDCheck* CRaidHDChkCreator::m_pRaidHDCheck = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaidHDChkCreator::CRaidHDChkCreator()
{
}

CRaidHDChkCreator::~CRaidHDChkCreator()
{
	
}

/******************************************************
// 函数名: 	CreatRaidHDCheck
// 作者:    wangzhaoyang
// 时期:    02-2-28
// 描述:    创建CRaidHDCheck对象
// 输入:
// 返回值:  所创建的CRaidHDCheck子类对象
// 注释: 
// 更新:    
// 日期            姓名                  描述     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          创建
******************************************************/
CRaidHDCheck* CRaidHDChkCreator::CreatRaidHDCheck()
{
	if(CIBMRaidHDCheck::IsIBMRaidController())
	{	
		TRACE(MTS_CLSTR, S_IBM_RAID_DETECTED);
		return new CIBMRaidHDCheck;
	}
	else if(CDellRaidHDCheck::IsDellRaidController())
	{
		TRACE(MTS_CLSTR, S_DELL_RAID_DETECTED);
		return new CDellRaidHDCheck;
	}
	return NULL;
}


/******************************************************
// 函数名: 	GetRaidHDCheck
// 作者:    wangzhaoyang
// 时期:    02-2-28
// 描述:    返回CRaidHDCheck对象
// 输入:
// 返回值:  返回CRaidHDCheck对象
// 注释:    singleton模式，保证只有一个实例
// 更新:    
// 日期            姓名                  描述     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          创建
******************************************************/
CRaidHDCheck* CRaidHDChkCreator::GetRaidHDCheck()
{
	if(!m_pRaidHDCheck)
	{
		m_pRaidHDCheck = CreatRaidHDCheck();
	}
	return m_pRaidHDCheck;
}
