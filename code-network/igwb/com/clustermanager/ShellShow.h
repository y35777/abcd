// ShellShow.h: interface for the CShellShow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLSHOW_H__F96FEFF4_BAEF_11D5_87DF_00C0DF245BEE__INCLUDED_)
#define AFX_SHELLSHOW_H__F96FEFF4_BAEF_11D5_87DF_00C0DF245BEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <shellapi.h>

class CShellShow  
{
public:
	CShellShow(const char* const szAppName);
	virtual ~CShellShow();

public:
	void Add(HWND hWnd, int nBillsvrType);
	void DeActive();
	void Active();
	void Destory();

protected:
	HICON m_IconDim;
	HICON m_IconLight;
	NOTIFYICONDATA* m_pniData;
	STRING m_szBillsvrType;
};

#endif // !defined(AFX_SHELLSHOW_H__F96FEFF4_BAEF_11D5_87DF_00C0DF245BEE__INCLUDED_)
