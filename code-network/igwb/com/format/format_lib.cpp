/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CFormatLib
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "format.h"
#include "format_global.h"
#include "format_lib.h"

/************ The CFormatLib Defining **************************/
// �������������캯��
//
// ���������
//   sRootDir����ʽ���Ŀ¼·��ʽ
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CFormatLib::CFormatLib(const STRING& sRootDir): m_sFmtRootDir(sRootDir)
{
    //��ʼ����Ա����
    //m_sFmtRootDir = sRootDir;
    m_pFmtsMap = NULL;
    m_bIsMustMerge = FALSE;
}
/************ End of The CFormatLib Defining *******************/


/************ The ~CFormatLib Defining *************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CFormatLib::~CFormatLib()
{
    FORMAT_MAP::iterator item;

    if(m_pFmtsMap != NULL)
    {
        //ɾ�����еĸ�ʽ�����
        item = m_pFmtsMap->begin();
        while(item != m_pFmtsMap->end())
        {
            delete item->second;
            item->second = NULL;

            item++;
        }

        //ɾ����ʽ�������ʽ������ӳ���ϵ����
        delete m_pFmtsMap;
        m_pFmtsMap = NULL;
    }

    while(m_mapAPToFmtSpDll.size())
    {
        ACE_DLL* pDll = m_mapAPToFmtSpDll.begin()->second;
        pDll->close();
        delete pDll;
        m_mapAPToFmtSpDll.erase(m_mapAPToFmtSpDll.begin());
    }
}
/************ End of The ~CFormatLib Defining ******************/

extern "C" {
typedef CFormat* (*CREATEFORMATOBJECT)(const char* szRootDir, const char* szFmtName);
}
CFormat* CFormatLib::CreateFormatObject(const char* szDllName,
                                        const char* szRootDir, 
                                        const char* szFmtName,
                                        int nAP)
{
    ACE_DLL* pDll = NULL;
    if(m_mapAPToFmtSpDll.find(nAP) == m_mapAPToFmtSpDll.end())
    {
        pDll = new ACE_DLL;
        int nRet = pDll->open(szDllName);
        if(0 != nRet)
        {
            return NULL;
        }
    }
    else
    {
        pDll = m_mapAPToFmtSpDll[nAP];
    }

    CREATEFORMATOBJECT f = (CREATEFORMATOBJECT)pDll->symbol("CreateFormatObject");
    if(NULL == f)
    {
        return NULL;
    }

    return f(szRootDir, szFmtName);
}

/************ The Init Defining ********************************/
// ������������ʼ������
//
// ���������
//       bCfg��Ϊ0��ʾ�����IFormatCfg�ӿڽ��г�ʼ��
//
// �����������
//
//   ����ֵ����ʼ���ɹ�����0�����򷵻�-1
//
/***************************************************************/
int CFormatLib::Init(int bCfg)
{
    //�������ļ�
    CINIFile Ini(GetCfgFilePath());
    Ini.Open();

    //�������ļ��ж�ȡ��ǰ���������
    UINT4 nAPCount = Ini.GetInt(CFG_SEC_COMMON, 
                                CFG_COMMON_KEY_APCOUNT, 
                                CFG_COMMON_KEY_APCOUNT_DEFAULT);
    int i = 0 ;
    char szFmtDll[40];
    for(i = 1; i <= nAPCount; i++)
    {
        char strSec[20];
        sprintf(strSec, "%s%d", CFG_SEC_AP, i);
        Ini.GetString(strSec,
                      CFG_AP_KEY_FMT_DLL,
                      CFG_AP_KEY_FMT_DLL_DEFAULT,
                      szFmtDll,
                      sizeof(szFmtDll));
        if(szFmtDll[0] != '\0')
        {
            m_mapAPToFmtSpName[i] = szFmtDll;
        }
    }
    
    if(m_pFmtsMap == NULL)
    {
        m_pFmtsMap = new FORMAT_MAP;
    }

    //�������Ը�ʽ���ý��г�ʼ�����򲻴����κ�CFormat����
    if(bCfg != 0)
    {
        return 0;
    }

    //�������ÿһ����ʽ�����ļ�������һ��CFormat*����---->
    STRING sFmtLibDir = m_sFmtRootDir;
    
    ACE_DIR *pDir = NULL;
    dirent* pDirent =  NULL;

    //�򿪸�ʽ�����ļ�Ŀ¼
    pDir = ACE_OS::opendir(sFmtLibDir.c_str());

    //�����Ŀ¼ʧ�ܣ�ʧ��ԭ����Ŀ¼������
    if((pDir == NULL) && (errno == ENOENT))
    {
        //�򴴽��������ļ���Ŀ¼
        if(!CreateDeepDir(m_sFmtRootDir.c_str()))
        {
            return -1;  //����Ŀ¼ʧ��
        }

        return 0;
    }

    //�����Ŀ¼ʧ�ܣ�ʧ��ԭ����Ŀ¼������
    else if(pDir == NULL)
    {
        return -1;
    }
    
    //���ļ��Ѵ�, ��֮�ر�
    if(pDir != NULL)
    {
        ACE_OS::closedir(pDir);
    }
    
    
    return 0;
}


/************ The GetFormats Defining **************************/
// ������������õ�ǰ��ʽ�������и�ʽ��
//
// �����������
//
// ���������
//  FmtNamesList����õ����и�ʽ���б�
//
//   ����ֵ����ʽ������
//
/***************************************************************/
UINT4 CFormatLib::GetFormats(LIST<STRING>& FmtNamesList) const
{
    FORMAT_MAP::iterator item;  //��ʽ�����ʽ����ӳ���ϵ������


    //��ʼ���������
    FmtNamesList.clear();
    if(m_pFmtsMap == NULL)
    {
        return 0;
    }

    //���и�ʽ�����ʽ����ӳ�䣬��ȡ���ʽ����
    item = m_pFmtsMap->begin();
    while(item != m_pFmtsMap->end())
    {
        //����ȡ�ĸ�ʽ�����뵽��������б���ȥ
        FmtNamesList.push_back(item->first);
        item++;
    }

    //���ظ�ʽ������
    return FmtNamesList.size();
}
/************ End of The GetFormats Defining *******************/


/************ The GetFormat Defining ***************************/
// �������������ָ����ʽ���ĸ�ʽ����
//
// ���������
//   sFmtName����ʽ��
//
// ���������
//
//   ����ֵ��ָ����ʽ���ĸ�ʽ����
//
/***************************************************************/
CFormat* CFormatLib::GetFormat(const STRING& sFmtName, int nAP)
{
    CFormat*  pFmt = NULL;      //��ʽ����ָ��
    FORMAT_MAP::iterator item;  //ָ����ʽ���ڸ�ʽ����ӳ���ϵ�еĽڵ�

    //ȷ������Ĳ�����ȷ��
    if(sFmtName.length() == 0)
    {
        return NULL;
    }

    //ȷ����ʽ�����ʽ����ӳ���ϵ����
    if(m_pFmtsMap == NULL)
    {
        m_pFmtsMap = new FORMAT_MAP;
    }
    else
    {
        //����ָ����ʽ���ĸ�ʽ����
        item = m_pFmtsMap->find(sFmtName);
        if(item != m_pFmtsMap->end())
        {
            //�ҵ�ָ����ʽ���ĸ�ʽ����
            pFmt = item->second;
            return pFmt;
        }
    }

    if(nAP > 0)
    {	
        
        if(m_mapAPToFmtSpName.find(nAP) != m_mapAPToFmtSpName.end())
        {
            STRING& strFmtDll = m_mapAPToFmtSpName[nAP];
            if (WLAN_DLL_NAME == strFmtDll) 
            {
                pFmt = new CFormat(m_sFmtRootDir, sFmtName);
            }
            else
            {            
                //������ǰ��ʽ����
                pFmt = CreateFormatObject(strFmtDll.c_str(), 
                                          m_sFmtRootDir.c_str(),
                                          sFmtName.c_str(),
                                          nAP);
            }
        }
        else        
        {
            pFmt = new CFormat(m_sFmtRootDir, sFmtName);
        }       


        if(NULL == pFmt)
        {
            //�����ʼ����ǰ��ʽ����ʧ��
            return NULL;
        }
        pFmt->SetMustMerge(m_bIsMustMerge);

        if(pFmt->Init() != 0)
        {
            //�����ʼ����ǰ��ʽ����ʧ��
            delete pFmt;
            return NULL;
        }

        STRING  sSrcFmtName;
        pFmt->GetSrcFmtName(sSrcFmtName);

        //�������ʽ��������Դ��ʽ������ԭ��ʽ��Ϊ��
        if((sSrcFmtName.length() != 0) && (sSrcFmtName != sFmtName))
        {
            //��ȡ����ʽ�����ԭ��ʽ����
            CFormat* pSrcFmt = GetFormat(sSrcFmtName, nAP);
            if(pSrcFmt == NULL)
            {
                delete pFmt;
                return NULL;
            }

            //���ñ���ʽ��ԭ��ʽ���󣬲��ѱ���ʽ������뵽��ʽ����ӳ���ϵ��
            if(pFmt->SetSrcFmt(pSrcFmt) != 0)
            {
                delete pFmt;
                return NULL;
            }
        }

        //����ʽ��Դ��ʽΪ�Լ�
        else if(sSrcFmtName == sFmtName)
        {
            //���ñ���ʽ��ԭ��ʽ���󣬲��ѱ���ʽ������뵽��ʽ����ӳ���ϵ��
            if(pFmt->SetSrcFmt(pFmt) != 0)
            {
                delete pFmt;
                return NULL;
            }
        }
        //�������ʽ��ԭ��ʽΪ��
/*
        else
        {
            //
        }
*/
        (*m_pFmtsMap)[sFmtName] = pFmt;

        return pFmt;
    }

    //ָ����ʽ���ĸ�ʽ���󲻴���
    return NULL;
}
/************ End of The GetFormat Defining ********************/


/************ The AddFormat Defining ***************************/
// ��������������һ���»�����ʽ����
//
// ���������
//   sFmtName�������ӵĸ�ʽ��������
//
// �����������
//
//   ����ֵ�����ӳɹ�����0�����򷵻�-1
//
/***************************************************************/
int CFormatLib::AddFormat(const STRING& sFmtName)
{
/*
    FORMAT_MAP::iterator item;  //ָ����ʽ���ڸ�ʽ����ӳ���ϵ�еĽڵ�

    //ȷ����ʽ�����ʽ����ӳ���ϵ����
    if(m_pFmtsMap == NULL)
    {
        m_pFmtsMap = new FORMAT_MAP;
    }

    //����ָ����ʽ���ĸ�ʽ����
    item = m_pFmtsMap->find(sFmtName);
    if(item != m_pFmtsMap->end())
    {
        //���ָ����ʽ���ĸ�ʽ�����Ѿ�����
        return -1;
    }

    //ָ����ʽ���ĸ�ʽ���󲻴���
    else
    {
        //����һ��ָ����ʽ�����¸�ʽ����
        CFormat* pFmt = NULL;
        if(!Is3GFormat(sFmtName))
        {
            //�����3G������ʽ
/ *
            pFmt = new CFormatR99(m_sFmtRootDir, sFmtName);
* /
        }
		else if(!Is3GR4Format(sFmtName))
        {
            //�����3GR4������ʽ
/ *
            pFmt = new CFormatR4(m_sFmtRootDir, sFmtName);
* /
        }
		else if(!IsGPRSFormat(sFmtName))
        {
            //�����GPRS������ʽ
/ *
            pFmt = new CFormatR98(m_sFmtRootDir, sFmtName);
* /
        }
        else
        {
            //��3G������ʽ
            pFmt = new CFormat(m_sFmtRootDir, sFmtName);
        }

        int nRet = pFmt->Create();
        if(nRet != 0)
        {
            //������ʽ�����ļ�ʧ��
            delete pFmt;
            return -1;
        }

        m_pFmtsMap->insert(FORMAT_MAP::value_type(sFmtName, pFmt));
        
        return 0;
    }*/
       return 0;
} 

/************ End of The AddFormat Defining ********************/


/************ The DelFormat Defining ***************************/
// ����������ɾ��һ�ֻ�����ʽ����
//
// ���������
//   sFmtName����ɾ���ĵĸ�ʽ��������
//
// �����������
//
//   ����ֵ���ɹ�����0�����򷵻�-1
//
/***************************************************************/
int CFormatLib::DelFormat(const STRING& sFmtName)
{
    CFormat*  pFmt = NULL;      //��ʽ����ָ��
    FORMAT_MAP::iterator item;  //ָ����ʽ���ڸ�ʽ����ӳ���ϵ�еĽڵ�


    //ȷ����ʽ�����ʽ����ӳ���ϵ����
    if(m_pFmtsMap == NULL)
    {
        return -1;
    }

    //����ָ����ʽ���ĸ�ʽ����
    item = m_pFmtsMap->find(sFmtName);
    if(item != m_pFmtsMap->end())
    {
        //���ָ����ʽ���ĸ�ʽ�������
        pFmt = item->second;
        if(pFmt->Delete() != 0)
        {
            return -1;
        }

        //�Ӹ�ʽ����ӳ���ϵ��ɾ��ָ���ĸ�ʽ����
        delete pFmt;
        m_pFmtsMap->erase(item);
        return 0;
    }

    //ָ����ʽ���ĸ�ʽ���󲻴���
    else
    {
        //ָ����ʽ���ĸ�ʽ���󲻴���
        return -1;
    }
}
/************ End of The DelFormat Defining ********************/


/************ The Commit Defining ******************************/
// ����������ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ
//           ���浽������
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormatLib::Commit(void)
{
    CFormat*  pFmt = NULL;      //��ʽ����ָ��
    FORMAT_MAP::iterator item;  //ָ����ʽ���ڸ�ʽ����ӳ���ϵ�еĽڵ�


    if(m_pFmtsMap != NULL)
    {
        //�������и�ʽ����
        item = m_pFmtsMap->begin();
        while(item != m_pFmtsMap->end())
        {
            //�ύ��ǰ��ʽ��������Ϣ
            pFmt = item->second;
            if(pFmt->Commit() != 0)
            {
                return -1;
            }

            item++;
        }
    }

    return 0;
}
/************ End of The Commit Defining ***********************/

void CFormatLib::SetMustMerge(BOOL bMustMerge)
{
     m_bIsMustMerge = bMustMerge;	
}
