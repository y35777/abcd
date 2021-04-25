#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/config.h"
#include "stat_cfg_mgr.h"
#include "cdr_stat_cfg.h"

/*******************************************************************
�������ƣ�CCDRStatCfgMgr()
�������������캯��
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
CCDRStatCfgMgr::CCDRStatCfgMgr()
{
    m_bUseXml = 0;
}

/*******************************************************************
�������ƣ�~CCDRStatCfgMgr()
������������������
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
CCDRStatCfgMgr::~CCDRStatCfgMgr()
{
    LIST<ICDRStatCfg*>::iterator it = m_statCfgList.begin();
    while (it != m_statCfgList.end())
    {
        delete *it;
        it++;
    }
    m_statCfgList.clear();
}

// ���ͳ�������ļ��Ķ�д��װ����
ICDRStatCfg* CCDRStatCfgMgr::CreateCDRStatCfg ( void )
{
    if(m_bUseXml)
    {
        return m_StatCfgEncap.CreateCDRStatCfg();
    }

    return new CCDRStatCfg();
}

/*******************************************************************
�������ƣ�Init()
�������������г�ʼ������
���������UINT4 nApCount
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatCfgMgr::Init(UINT4 nApCount)
{
    CINIFile ConfigFile(GetCfgFilePath());
    ConfigFile.Open();
    m_bUseXml = ConfigFile.GetInt(CFG_SEC_CDRSTAT,
                                  CFG_USE_XML,
                                  CFG_USE_XML_DEFAULT);

    ICDRStatCfg* pTmpCfg = NULL;
    for (UINT4 i = 1; i <= nApCount; i++)
    {
        pTmpCfg = CreateCDRStatCfg();
        pTmpCfg->Init(i);
        m_statCfgList.push_back(pTmpCfg);
    }

    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�GetStatCfgList(LIST<ICDRStatCfg*>& listStatCfg)
������������������ICDRStatCfg�ӿ���ָ��Ľӿ�
�����������
���������ͳ�����ýӿ�ָ������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatCfgMgr::GetStatCfgList(LIST<ICDRStatCfg*>& listStatCfg)
{
    LIST<ICDRStatCfg*>::iterator it = m_statCfgList.begin();
    while (it != m_statCfgList.end())
    {
        listStatCfg.push_back(*it);
        it++;
    }
}


/*******************************************************************
�������ƣ�GetStatCfgInterface(int nAccessPoint)
����������ͨ�������Ų��Ҷ�Ӧ��ICDRStatCfg�ӿ���ָ��
���������int nAccessPoint,������
�����������
�������ݣ��˽�����ͳ�����ýӿ�ָ��
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
ICDRStatCfg* CCDRStatCfgMgr::GetStatCfgInterface(int nAccessPoint)
{
    LIST<ICDRStatCfg*>::iterator it = m_statCfgList.begin();
    while (it != m_statCfgList.end())
    {
        if ((*it)->GetAccessPoint() == nAccessPoint)
        {
            return *it;
        }
        it++;
    }
    return NULL;
}


/*******************************************************************
�������ƣ�GetStatCfgInterface(STRING sApName)
����������ͨ�������Ų��Ҷ�Ӧ��ICDRStatCfg�ӿ���ָ��
���������STRING sApName,���������
�����������
�������ݣ��˽�����ͳ�����ýӿ�ָ��
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
ICDRStatCfg* CCDRStatCfgMgr::GetStatCfgInterface(STRING sApName)
{
    LIST<ICDRStatCfg*>::iterator it = m_statCfgList.begin();
    while (it != m_statCfgList.end())
    {
        if (sApName.compare((*it)->GetAccessPointName()) == 0)
        {
            return *it;
        }
        it++;
    }
    return NULL;
}