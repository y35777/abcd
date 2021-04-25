/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:persist_err.h.h
    ������Ϣ:

    ����CPersistErr��Ķ���,�������������������й��������Ϣ.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
#include "persist_err.h"
#include "../include/toolbox.h"

CPersistErr* CPersistErr::g_pErr = NULL;

/*
    ��������:��������.

    1.ɾ��m_ErrMap�д�ŵĴ���������Ϣ.
    
    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
CPersistErr::~CPersistErr()
{
    char* p = NULL;
    MAP<UINT4,char*>::iterator i = m_ErrMap.begin();

    while(i != m_ErrMap.end())
    {
        p = (*i).second;
        i++;
        delete[] p;
    }
    m_ErrMap.clear();
}

CPersistErr* CPersistErr::Instance(void)
{
    if(g_pErr == NULL)
    {
        g_pErr = new CPersistErr;
		/*
			������ʼ�����.
		*/
		g_pErr->Init();
    }

    return g_pErr;
}

void CPersistErr::Destory(void)
{
    if(g_pErr != NULL)
    {
        delete g_pErr;
        g_pErr = NULL;
    }
}

/*
    ��������:���ʴ����ļ�errdef.dat,��������Ϣ���뵽m_ErrMap��.
    
    1.���ȶ���������Ϣ��¼��.
    2.��ѭ����������������Ϣ��¼.
    3.ʹ��CINIFile�����ȡ�ļ�.

    ����:��ʼ���ɹ�����TRUE,���򷵻�FALSE.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
BOOL CPersistErr::Init()
{
#ifdef _PLATFORM_WIN32
//	#error Please define the path of file "errdef.dat".
	STRING strPath = "errdef.dat";
#else
	STRING strPath = "errdef.dat";
#endif

    CINIFile *pIniFile = new CINIFile(strPath.c_str());
    pIniFile->Open(); 

    UINT4 nCount = (UINT4)pIniFile->GetInt("ErrorInfo",
                                           "ErrorNumber",0);
    if(nCount = 0)
    {
        delete pIniFile;
        return FALSE;
    }

    char* p = NULL;
    char szErrno[20],szOut[1024];

    for(UINT4 i = 0;i <= nCount;i++)
    {
        sprintf(szErrno,"%d",i);
        if(pIniFile->GetString("ErrorInfo",szErrno,"",
            szOut,1023) != 0)
        {
            p = new char[strlen(szOut) + 1];
            strcpy(p,szOut);
            m_ErrMap[i] = p;
        }
    }

    delete pIniFile;
    return TRUE;
}

/*
    ��������:��m_ErrMap�в�ѯnErrno��Ӧ�Ĵ�������.

    ����:
    nErrno,�������,Ϊ����.

    ����:�ҵ��򷵻�ָ����������ַ�����ָ��,���򷵻�NULL.

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/
const char* CPersistErr::GetError(INT4 nErrno)
{
    ASSERT(nErrno < 0);
    //by ldf 2002.6.11 ��Ӧ���ⵥD14482
    if(nErrno >= 0) return NULL;
    //�޸����

    MAP<UINT4,char*>::iterator i = m_ErrMap.find(-nErrno);
    if(i == m_ErrMap.end())
    {
        //by ldf 2002.6.11 ��Ӧ���ⵥD14482
        return NULL;
        //�޸����
    }
    
    return (i->second);
}
