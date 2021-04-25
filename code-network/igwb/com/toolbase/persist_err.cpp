/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:persist_err.h.h
    描述信息:

    包含CPersistErr类的定义,该类用于在整个进程中管理错误消息.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
*/
#include "persist_err.h"
#include "../include/toolbox.h"

CPersistErr* CPersistErr::g_pErr = NULL;

/*
    函数功能:析构函数.

    1.删除m_ErrMap中存放的错误描述信息.
    
    修改记录:
    周拥辉,2001-10-16,创建.
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
			不检查初始化结果.
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
    函数功能:访问磁盘文件errdef.dat,将出错信息读入到m_ErrMap中.
    
    1.首先读出错误信息记录数.
    2.在循环读出各条错误信息记录.
    3.使用CINIFile对象读取文件.

    返回:初始化成功返回TRUE,否则返回FALSE.

    修改记录:
    周拥辉,2001-10-16,创建.
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
    函数功能:在m_ErrMap中查询nErrno对应的错误描述.

    参数:
    nErrno,错误代码,为负数.

    返回:找到则返回指向错误描述字符串的指针,否则返回NULL.

    修改记录:
    周拥辉,2001-10-16,创建.
*/
const char* CPersistErr::GetError(INT4 nErrno)
{
    ASSERT(nErrno < 0);
    //by ldf 2002.6.11 对应问题单D14482
    if(nErrno >= 0) return NULL;
    //修改完毕

    MAP<UINT4,char*>::iterator i = m_ErrMap.find(-nErrno);
    if(i == m_ErrMap.end())
    {
        //by ldf 2002.6.11 对应问题单D14482
        return NULL;
        //修改完毕
    }
    
    return (i->second);
}
