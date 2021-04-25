// RaidHDChkCreator.h: interface for the RaidHDChkFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAIDHDCHKCREATOR_H__9B7CA824_2A52_11D6_92C9_00B0D0D37430__INCLUDED_)
#define AFX_RAIDHDCHKCREATOR_H__9B7CA824_2A52_11D6_92C9_00B0D0D37430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRaidHDCheck;

class CRaidHDChkCreator //�����ഴ��CRaidHDCheck���󣬸���ʹ��ʱ��������ʵ��
{
public:
	CRaidHDChkCreator();
	virtual ~CRaidHDChkCreator();
	static CRaidHDCheck* CreatRaidHDCheck();
	static CRaidHDCheck* GetRaidHDCheck();
private:
	static CRaidHDCheck* m_pRaidHDCheck;
};

#endif // !defined(AFX_RAIDHDCHKCREATOR_H__9B7CA824_2A52_11D6_92C9_00B0D0D37430__INCLUDED_)
