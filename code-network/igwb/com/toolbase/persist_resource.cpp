/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:persist_resource.cpp
    ������Ϣ:

    ����CPersistResource��Ķ���,�������������������й���Resource��Ϣ.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ŷ�,2002-05-30,����.
*/
#include "persist_resource.h"
#include "../include/toolbox.h"

CPersistResource* CPersistResource::g_pResource = NULL;

/*
    ��������:��������.

    ɾ��ResourceString�д�ŵ�������Ϣ.
    
    �޸ļ�¼:
    ��ŷ�,2002-05-30,����.
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

	    //������ʼ�����.
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
    ��������:���ʴ����ļ�resource.dat,�����String��Ϣ���뵽ResourceString��.

    ����:��ʼ���ɹ�����TRUE,���򷵻�FALSE.

    �޸ļ�¼:
    ��ŷ�,2002-05-30,����.
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
	        if (szLineTmpPt == NULL)	//�������и���û�С�=��
	        {
                    continue;
	        }
					    
	        char *szLineTmp = szLineTmpPt;
            szLineTmpPt++;  //�Ƶ�=�ұߵ�һ���ַ���
	        *szLineTmp = '\0';

       	    //����szLine�õ� nID
            StringLRTrim(szLine);
	        nID = atoi(szLine);

            //����szLineTmpPt�õ����ַ���			
            StringLRTrim(szLineTmpPt);
            STRING ResString = szLineTmpPt;

            ResourceString[nID] = ResString;
        }
    }

    fclose(fp);
    return TRUE;
}

/*
    ��������:��ResourceString�в�ѯnID��Ӧ��Resource����.

    ����:
    nID,  ΪResource �е� String

    ����:�ҵ��򷵻�ָ��������ַ�����ָ��,���򷵻�NULL.

    �޸ļ�¼:
    ��ŷ�,2002-05-30,����.
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
