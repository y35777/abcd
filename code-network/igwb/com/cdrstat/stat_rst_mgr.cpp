#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/config.h"
#include "stat_rst_mgr.h"
#include "cdr_stat_rst.h"

/*******************************************************************
�������ƣ�CCDRStatRstMgr()
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
CCDRStatRstMgr::CCDRStatRstMgr()
{
    m_bUseXml = 0;
}

/*******************************************************************
�������ƣ�~CCDRStatRstMgr()
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
CCDRStatRstMgr::~CCDRStatRstMgr()
{
    LIST<ICDRStatRst*>::iterator it = m_statRstList.begin();
    while (it != m_statRstList.end())
    {
        delete *it;
        it++;
    }
    m_statRstList.clear();
}

//���ͳ�ƽ���ļ��Ķ�д��װ����
ICDRStatRst* CCDRStatRstMgr::CreateCDRStatRst ( void )
{
    if(m_bUseXml)
    {
        return m_StatRstEncap.CreateCDRStatRst();
    }

    return new CCDRStatRst();
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
int CCDRStatRstMgr::Init(UINT4 nApCount)
{
    CINIFile ConfigFile(GetCfgFilePath());
    ConfigFile.Open();
    m_bUseXml = ConfigFile.GetInt(CFG_SEC_CDRSTAT,
                                  CFG_USE_XML,
                                  CFG_USE_XML_DEFAULT);

    ICDRStatRst* pTmpRst = NULL;
    for (UINT4 i = 1; i <= nApCount; i++)
    {
        pTmpRst = CreateCDRStatRst();
        pTmpRst->Init(i);
        m_statRstList.push_back(pTmpRst);
    }

    return ERR_SUCCESS;
}

/*******************************************************************
�������ƣ�GetStatRstList(LIST<ICDRStatRst*>& listStatRst)
������������������ICDRStatRst�ӿ���ָ��Ľӿ�
�����������
���������ICDRStatRst*>& listStatRst��ͳ�ƽ���ӿ�ָ���б�
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatRstMgr::GetStatRstList(LIST<ICDRStatRst*>& listStatRst)
{
    LIST<ICDRStatRst*>::iterator it = m_statRstList.begin();
    while (it != m_statRstList.end())
    {
        listStatRst.push_back(*it);
        it++;
    }

}

/*******************************************************************
�������ƣ�GetStatRstInterface(int nAccessPoint)
����������ͨ�������Ų��Ҷ�Ӧ��ICDRStatRst�ӿ���ָ��
���������int nAccessPoint��������
�����������
�������ݣ��˽ӵ��ͳ�ƽ���ӿ�ָ��
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
ICDRStatRst* CCDRStatRstMgr::GetStatRstInterface(int nAccessPoint)
{
    LIST<ICDRStatRst*>::iterator it = m_statRstList.begin();
    while (it != m_statRstList.end())
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
�������ƣ�GetStatRstInterface(STRING sAPName)
����������ͨ�������Ų��Ҷ�Ӧ��ICDRStatRst�ӿ���ָ��
���������STRING sAPName
�����������
�������ݣ��˽ӵ��ͳ�ƽ���ӿ�ָ��
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
ICDRStatRst* CCDRStatRstMgr::GetStatRstInterface(STRING sAPName)
{
    LIST<ICDRStatRst*>::iterator it = m_statRstList.begin();
    while (it != m_statRstList.end())
    {
        if (sAPName.compare((*it)->GetAccessPointName()) == 0)
        {
            return *it;
        }
        it++;
    }
    return NULL;
}
