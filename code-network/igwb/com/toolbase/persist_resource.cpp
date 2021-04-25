/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:persist_resource.cpp
    描述信息:

    包含CPersistResource类的定义,该类用于在整个进程中管理Resource信息.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    李杜芳,2002-05-30,创建.
*/
#include "persist_resource.h"
#include "../include/toolbox.h"

CPersistResource* CPersistResource::g_pResource = NULL;

/*
    函数功能:析构函数.

    删除ResourceString中存放的描述信息.
    
    修改记录:
    李杜芳,2002-05-30,创建.
*/
CPersistResource::~CPersistResource()
{
    char* p = NULL;
    MAP<INT4,STRING>::iterator i = ResourceString.begin();

    ResourceString.clear();
}

CPersistResource* CPersistResource::Instance()
{
    static ACE_Mutex mutex;

    mutex.acquire();

    if(g_pResource == NULL)
    {
        g_pResource = new CPersistResource;

	    //不检查初始化结果.
	    g_pResource->Init();
    }

    mutex.release();

    return g_pResource;
}

void CPersistResource::Destory(void)
{
    if(g_pResource != NULL)
    {
        delete g_pResource;
        g_pResource = NULL;
    }
}

/*
    函数功能:访问磁盘文件resource.dat,将相关String信息读入到ResourceString中.

    返回:初始化成功返回TRUE,否则返回FALSE.

    修改记录:
    李杜芳,2002-05-30,创建.
*/

BOOL CPersistResource::Init()
{

    const char* szResRootPath = GetAppPath();

    STRING szResPath  = szResRootPath;
    szResPath  = szResPath  + ACE_DIRECTORY_SEPARATOR_STR + S_RESSTR_FILE_NAME; 

    FILE* fp = NULL;
    
    if ((fp = fopen(szResPath.c_str(), "r")) == NULL)
    {
        return FALSE;
    } 
    
    INT4 nID;
    char szLine[1024];
	
    while (!feof(fp))
    {
        if(fgets(szLine, 1023, fp) != NULL)
        {
	        char *szLineTmpPt = szLine;
	        szLineTmpPt = strchr(szLine, '=');		
	        if (szLineTmpPt == NULL)	//输入行中根本没有“=”
	        {
                    continue;
	        }
					    
	        char *szLineTmp = szLineTmpPt;
            szLineTmpPt++;  //移到=右边的一个字符处
	        *szLineTmp = '\0';

       	    //这里szLine得到 nID
            StringLRTrim(szLine);
	        nID = atoi(szLine);

            //这里szLineTmpPt得到了字符串			
            StringLRTrim(szLineTmpPt);
            STRING ResString = szLineTmpPt;

            ResourceString[nID] = ResString;
        }
    }

    fclose(fp);
    return TRUE;
}

/*
    函数功能:在ResourceString中查询nID对应的Resource描述.

    参数:
    nID,  为Resource 中的 String

    返回:找到则返回指向该描述字符串的指针,否则返回NULL.

    修改记录:
    李杜芳,2002-05-30,创建.
*/
const char* CPersistResource::GetStringFromID(INT4 nID)
{
    
    m_Mutex.acquire();
    
    MAP<INT4,STRING>::iterator i = ResourceString.find(nID);
    if(i == ResourceString.end())
    {
        m_Mutex.release();
				
        return "-";
    }
    
    const char* p = ((i->second).c_str());

    m_Mutex.release();
    
    return p;
}
