// com_collect.cpp: implementation of the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/base_type.h"
#include "../bk_link_mgr.h"
#include "ewsd_amas_collect.h"
#include "ewsd_ama_collect.h"

#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include <assert.h>

CEWSD_AMASCollect::CEWSD_AMASCollect(int nSwitchIndex, CLinkGroup* pLinkGroup)
                        :CCollectBase(nSwitchIndex, pLinkGroup)
{
    m_pLinkMgr        = NULL;
    m_szSwitchName[0] = '\0';
}


CEWSD_AMASCollect::~CEWSD_AMASCollect()
{
    VECTOR<CEWSD_AMACollect*>::iterator i;
    for(i = m_AMAColList.begin(); i != m_AMAColList.end(); i++)
    {
        delete *i;
    }

    m_AMAColList.clear();

    if (m_pLinkMgr != NULL)
    {
        delete m_pLinkMgr;
        m_pLinkMgr = NULL;
    }

}


//��ʼ���ɼ�����, ���麯��, �����������ʵ��
int  CEWSD_AMASCollect::Col_Init()
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
    if (szPriLink[0] == '\0' || m_pLinkGroup->GetLink(szPriLink) == NULL)
    {
        COTRACE("�ɼ�����(%s)����·û������,�����ô���",
              m_szSwitchName);
        return ERR_FAIL;
    }

    INIFile.GetString(szSecName,
                      CFG_CO_BAK_LINK,
                      CFG_CO_BAK_LINK_DEFAULT,
                      szBakLink,
                      sizeof(szBakLink));
    m_pLinkMgr = new CBKLinkMgr(m_pLinkGroup);
    m_pLinkMgr->RegisterLink(szPriLink);
    if(szBakLink[0] != '\0' && m_pLinkGroup->GetLink(szBakLink) != NULL)
    {
        m_pLinkMgr->RegisterLink(szBakLink);
    }

    //����Ŀ¼�ɼ�����
    CEWSD_AMACollect* pCollect = NULL;

    int nAMACount = INIFile.GetInt(szSecName,
                                  CFG_CO_COL_AMA_COUNT,
                                  CFG_CO_COL_AMA_COUNT_DEFAULT);
    for(int i = 1; i <= nAMACount; i++)
    {
        /*
            ȡ�ɼ�Ŀ¼����·��Ϣ
        */
        SNPRINTF(szSecName,
                 sizeof(szSecName),
                 "%s%d_ama%d",
                 CFG_CO_SEC_SWITCH,
                 m_nSwitchIndex,
                 i);

        pCollect = new CEWSD_AMACollect(m_pLinkMgr, m_nSwitchIndex, i);
        if (pCollect ==  NULL)
        {
            COTRACE("�ɼ�����(%s)����AMA�Ӷ���(ID=%d)ʧ��",
                  m_szSwitchName, i);
            continue;
        }

        pCollect->SetCfgPath(m_szCfgPath);
        pCollect->SetFileSaveDays(m_nFileSaveDays);
        pCollect->SetSwitchName(m_szSwitchName);

        if(pCollect->Init() != ERR_SUCCESS)
        {
            COTRACE("�ɼ�����(%s)����AMA�Ӷ���(ID=%d)ʧ��",
                  m_szSwitchName, i);
            delete pCollect;
            continue;
        }

        m_AMAColList.push_back(pCollect);
    }

    return ERR_SUCCESS;
}


//�����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��;
int  CEWSD_AMASCollect::Col_GetFile(SColFileReq& stReq)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if((*i)->FindFileInList(stReq.szSrcName) == ERR_SUCCESS)
        {
            return (*i)->GetFile(stReq);
        }
    }

    return ERR_FAIL;
}

//����ļ��б�, ���麯��, �����������ʵ��
int  CEWSD_AMASCollect::Col_GetList(SListOut& pOut)
{
    pOut.FileList.clear();

    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        (*i)->GetList(pOut);
    }

    return ERR_SUCCESS;
}

//��̬������Ŀ¼, ���麯��
BOOL CEWSD_AMASCollect::CreateLocSubDir()
{
    BOOL bRet = TRUE;
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(!((*i)->CreateLocSubDir()))
        {
            bRet = FALSE;
        }
    }

    return bRet;
}

//��������ļ�, ���麯��
BOOL CEWSD_AMASCollect::ClearOldFile()
{
    BOOL bRet = TRUE;
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(!((*i)->ClearOldFile()))
        {
            bRet = FALSE;
        }
    }

    return bRet;
}

//����ļ����к��Ƿ�������������©���ļ������麯��, �����������ʵ��
BOOL CEWSD_AMASCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if((*i)->FindFileInList(szFileName) == ERR_SUCCESS)
        {
            return (*i)->CheckFileSN(szFileName, szPreviousFile);
        }
    }

    return TRUE;

}

//ָ�����ļ��Ƿ�ɲ�
int CEWSD_AMASCollect::CanGetSpecFile(const char* const szModule,
                                      const char* const szSrcFile)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->CanGetSpecFile(szSrcFile);
        }
    }

    return ERR_FAIL;
}

int CEWSD_AMASCollect::GetSpecFile(const char* const szModule,
                                   const char* const szSrcFile,
                                   const char* const szDstFile)
{
    return ERR_FAIL;
}

//��ȡ��������Ϣ
int CEWSD_AMASCollect::GetSwitchInfo(LIST<STRING>& label,
                                     LIST<STRING>& content)
{
    label.push_back("module");

    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        content.push_back((*i)->GetModuleLabel());
    }

    return ERR_SUCCESS;
}

//��ȡģ����Ϣ
int CEWSD_AMASCollect::GetModuleInfo(const char* const szModule,
                                     LIST<STRING>& label,
                                     LIST<STRING>& content)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->GetModuleInfo(label, content);
        }
    }

    return ERR_FAIL;
}

//��ȡ�ļ�����
int CEWSD_AMASCollect::GetFileLen(const char* const szModule,
                                  const char* const szSrcFile)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->GetFileLen(szSrcFile);
        }
    }

    return 0;
}

