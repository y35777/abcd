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
// ������: 	CreatRaidHDCheck
// ����:    wangzhaoyang
// ʱ��:    02-2-28
// ����:    ����CRaidHDCheck����
// ����:
// ����ֵ:  ��������CRaidHDCheck�������
// ע��: 
// ����:    
// ����            ����                  ����     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          ����
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
// ������: 	GetRaidHDCheck
// ����:    wangzhaoyang
// ʱ��:    02-2-28
// ����:    ����CRaidHDCheck����
// ����:
// ����ֵ:  ����CRaidHDCheck����
// ע��:    singletonģʽ����ֻ֤��һ��ʵ��
// ����:    
// ����            ����                  ����     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          ����
******************************************************/
CRaidHDCheck* CRaidHDChkCreator::GetRaidHDCheck()
{
	if(!m_pRaidHDCheck)
	{
		m_pRaidHDCheck = CreatRaidHDCheck();
	}
	return m_pRaidHDCheck;
}
