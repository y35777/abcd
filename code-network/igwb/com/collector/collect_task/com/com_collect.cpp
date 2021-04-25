// com_collect.cpp: implementation of the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/base_type.h"
#include "../bk_link_mgr.h"
#include "com_collect.h"
#include "com_dir_collect.h"

#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include <assert.h>

CComCollect::CComCollect(int nSwitchIndex, CLinkGroup* pLinkGroup)
                        :CCollectBase(nSwitchIndex, pLinkGroup)
{
    m_pLinkMgr        = NULL;
    m_szSwitchName[0] = '\0';
}


CComCollect::~CComCollect()
{
    VECTOR<CComDirCollect*>::iterator i;
    for(i = m_DirColList.begin(); i != m_DirColList.end(); i++)
    {
        delete *i;
    }

    m_DirColList.clear();

    if (m_pLinkMgr != NULL)
    {
        delete m_pLinkMgr;
        m_pLinkMgr = NULL;
    }

}


//��ʼ���ɼ�����, ���麯��, �����������ʵ��
int  CComCollect::Col_Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(ID=%d)�������ļ�(%s)ʧ��",
              m_nSwitchIndex,
              m_szCfgPath);
        return ERR_FAIL;
    }


    char szSecName[COM_CFG_LEN];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[COM_CFG_LEN - 1] = '\0';

    //��ȡ�ɼ�������
    INIFile.GetString(szSecName,
                      CFG_CO_NAME,
                      CFG_CO_NAME_DEFAULT,
                      m_szSwitchName,
                      sizeof(m_szSwitchName));

    //�������ļ��ı�������
    m_nFileSaveDays = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT);
    m_nFileSaveDays = INIFile.GetInt(szSecName,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);
    if(m_nFileSaveDays < 0)
    {
        m_nFileSaveDays = CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT;
    }

    //����LinkMgr����
    char szPriLink[50];
    char szBakLink[50];

    INIFile.GetString(szSecName,
                      CFG_CO_PRI_LINK,
                      CFG_CO_PRI_LINK_DEFAULT,
                      szPriLink,
                      sizeof(szPriLink));
    if(szPriLink[0] == '\0' || m_pLinkGroup->GetLink(szPriLink) == NULL)
    {
        COTRACE("�ɼ�����(%s)����·û������,�����ô���",
              m_szSwitchName);
        return ERR_FAIL;
    }
    m_pLinkMgr = new CBKLinkMgr(m_pLinkGroup);
    m_pLinkMgr->RegisterLink(szPriLink);

    INIFile.GetString(szSecName,
                      CFG_CO_BAK_LINK,
                      CFG_CO_BAK_LINK_DEFAULT,
                      szBakLink,
                      sizeof(szBakLink));
    if(szBakLink[0] != '\0' && m_pLinkGroup->GetLink(szBakLink) != NULL)
    {
        m_pLinkMgr->RegisterLink(szBakLink);
    }

    //����Ŀ¼�ɼ�����
    CComDirCollect* pDirCollect = NULL;

    int nDirCount = INIFile.GetInt(szSecName,
                                  CFG_CO_COL_DIR_COUNT,
                                  CFG_CO_COL_DIR_COUNT_DEFAULT);
    for(int i = 1; i <= nDirCount; i++)
    {
        /*
            ȡ�ɼ�Ŀ¼����·��Ϣ
        */
        SNPRINTF(szSecName,
                 sizeof(szSecName),
                 "%s%d_col_dir%d",
                 CFG_CO_SEC_SWITCH,
                 m_nSwitchIndex,
                 i);

        pDirCollect = new CComDirCollect(m_pLinkMgr, m_nSwitchIndex, i);
        if (pDirCollect ==  NULL)
        {
            COTRACE("�ɼ�����(%s)����DirCollect�Ӷ���(ID=%d)ʧ��",
                  m_szSwitchName, i);
            continue;
        }

        pDirCollect->SetCfgPath(m_szCfgPath);
        pDirCollect->SetFileSaveDays(m_nFileSaveDays);
        pDirCollect->SetSwitchName(m_szSwitchName);

        if(pDirCollect->Init() != ERR_SUCCESS)
        {
            COTRACE("�ɼ�����(%s)����DirCollect�Ӷ���(ID=%d)ʧ��",
                  m_szSwitchName, i);
            delete pDirCollect;
            continue;
        }

        m_DirColList.push_back(pDirCollect);
    }

    return ERR_SUCCESS;
}

int  CComCollect::Connect()
{
    return m_pLinkMgr->Connect();
}

int  CComCollect::Disconnect()
{
    return m_pLinkMgr->Disconnect();
}

//�����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��;
int  CComCollect::Col_GetFile(SColFileReq& stReq)
{
    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        if((*i)->FindFileInList(stReq.szSrcName) == ERR_SUCCESS)
        {
            return (*i)->GetFile(stReq);
        }
    }

    return ERR_FAIL;
}

//����ļ��б�, ���麯��, �����������ʵ��
int  CComCollect::Col_GetList(SListOut& pOut)
{
    pOut.FileList.clear();

    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        (*i)->GetList(pOut);
    }

    return ERR_SUCCESS;
}

//��̬������Ŀ¼, ���麯��
BOOL CComCollect::CreateLocSubDir()
{
    BOOL bRet = TRUE;
    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        if(!((*i)->CreateLocSubDir()))
        {
            bRet = FALSE;
        }
    }

    return bRet;
}

//��������ļ�, ���麯��
BOOL CComCollect::ClearOldFile()
{
    BOOL bRet = TRUE;
    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        if(!((*i)->ClearOldFile()))
        {
            bRet = FALSE;
        }
    }

    return bRet;
}

//����ļ����к��Ƿ�������������©���ļ������麯��, �����������ʵ��
BOOL CComCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        if((*i)->FindFileInList(szFileName) == ERR_SUCCESS)
        {
            return (*i)->CheckFileSN(szFileName, szPreviousFile);
        }
    }

    return TRUE;

}

//ָ�����ļ��Ƿ�ɲ�
int CComCollect::CanGetSpecFile(const char* const szModule,
                                const char* const szSrcFile)
{
    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->CanGetSpecFile(szSrcFile);
        }
    }

    return ERR_FILE_CANNOT_BE_FOUND;
}

//��ȡ��������Ϣ
int CComCollect::GetSwitchInfo(LIST<STRING>& label,
                               LIST<STRING>& content)
{
    label.push_back("module");

    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        content.push_back((*i)->GetModuleLabel());
    }

    return ERR_SUCCESS;
}

//��ȡģ����Ϣ
int CComCollect::GetModuleInfo(const char* const szModule,
                               LIST<STRING>& label,
                               LIST<STRING>& content)
{
    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->GetModuleInfo(label, content);
        }
    }

    return ERR_FAIL;
}

//��ȡ�ļ�����
int CComCollect::GetFileLen(const char* const szModule,
                            const char* const szSrcFile)
{
    VECTOR<CComDirCollect*>::iterator i = m_DirColList.begin();
    for(; i != m_DirColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->GetFileLen(szSrcFile);
        }
    }

    return 0;
}

