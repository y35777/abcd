// ShellShow.cpp: implementation of the CShellShow class.
//
//////////////////////////////////////////////////////////////////////

#include <afxwin.h>
#include <string>
#define STRING      std::string
#include "ShellShow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShellShow::CShellShow(const char* const szAppName)
{
	m_IconDim = ExtractIcon(0, szAppName, 1);
    m_IconLight = ExtractIcon(0, szAppName, 2);
	m_pniData = NULL;
}

CShellShow::~CShellShow()
{
    Destory();
}

void CShellShow::Active()
{
	if(m_pniData != 0)
    {
        m_pniData->hIcon = m_IconLight;
        sprintf(m_pniData->szTip,"%s: Active", LPCTSTR(m_szBillsvrType.c_str()));
        Shell_NotifyIcon(NIM_MODIFY, m_pniData);
    }
}

void CShellShow::Add(HWND hWnd, int nBillsvrType)
{
	if(nBillsvrType == 0)
	{
		m_szBillsvrType = "Master";
	}
	else
	{
		m_szBillsvrType = "Slave";
	}
	if(!m_pniData)
	{
		m_pniData = new NOTIFYICONDATA;
		m_pniData->cbSize = sizeof(NOTIFYICONDATA);
		m_pniData->hWnd = hWnd;
		m_pniData->uID = 123;
		m_pniData->uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		m_pniData->hIcon = m_IconDim;
		m_pniData->uCallbackMessage = 7897;
		sprintf(m_pniData->szTip, "%s: DeActive", LPCTSTR(m_szBillsvrType.c_str()));
	}
	Shell_NotifyIcon(NIM_ADD, m_pniData);
}

void CShellShow::DeActive()
{
	if(m_pniData != 0)
    {
        m_pniData->hIcon = m_IconDim;
        sprintf(m_pniData->szTip, "%s: DeActive", LPCTSTR(m_szBillsvrType.c_str()));
        Shell_NotifyIcon(NIM_MODIFY, m_pniData);
    }
}

void CShellShow::Destory()
{
	if(m_pniData)
	{
		Shell_NotifyIcon(NIM_DELETE, m_pniData);
		delete m_pniData, m_pniData = NULL;
	}
}