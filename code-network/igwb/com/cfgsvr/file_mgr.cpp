/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : file_mgr.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CCfgFileMgr class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/
#include "file_mgr.h"
#include "file_grp.h"

//��¼�������ò���ʧ��ԭ�򣬵�ʧ��ʱ��������Ϣ�������ͻ���; ������cfg_svr.cpp��
extern char g_szErrorReason[20*1024];

//////////////////////////////////////////////////////////////////////
// CCfgFileMgr Class
//////////////////////////////////////////////////////////////////////

/*******************************************************************
�������ƣ�CCfgFileMgr()
�������������캯��
���������
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CCfgFileMgr::CCfgFileMgr()
{   
}

/*******************************************************************
�������ƣ�~CCfgFileMgr()
������������������
���������
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CCfgFileMgr::~CCfgFileMgr()
{
    //���ӳ���m_mapCfgFileGrp��
    MAP< int, CCfgFileGrp* >::iterator it;
    for(it=m_mapCfgFileGrp.begin(); it != m_mapCfgFileGrp.end(); it++)
    {
        delete (*it).second;
    }
    m_mapCfgFileGrp.clear();
    
    //���ӳ���m_mapCfgFileClass
    m_mapCfgFileClass.clear();
}

/*******************************************************************
�������ƣ�AddCfgFileGrp(int nGrpNo, char* szFilePath)
�������������������ļ���
���������
   ����1��int nGrpNo�����
   ����2��int nFileNo���ļ���
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFileMgr::AddCfgFileGrp(int nGrpNo, const char *szFilePath)
{
    if (szFilePath == NULL)
    {
        return FALSE;
    }

	//����һ�������ļ���pCfgFileGrp��ִ�г�ʼ����pCfgFileGrp->Init()��
    CCfgFileGrp *pCfgFileGrp = new  CCfgFileGrp();
    if (!pCfgFileGrp->Init(szFilePath))
    {
        delete pCfgFileGrp;
        return FALSE;
    }
    //��m_mapCfgFileGrp����nGrpNo��pCfgFileGrpӳ���ϵ��
    PAIR< MAP< int, CCfgFileGrp * >::iterator, bool > result;
    result = m_mapCfgFileGrp.insert( PAIR< int, CCfgFileGrp * >(nGrpNo, 
        pCfgFileGrp) );
    if (!result.second)
    {
        //�ļ�����ظ�
        delete pCfgFileGrp;
        return FALSE;
    }
    //��m_mapCfgFileClass����nGrpNo��szFilePath�Ĺ�ϵ��
    m_mapCfgFileClass.insert(PAIR< int, STRING >(nGrpNo, STRING(szFilePath)));
    
    return TRUE;
}

/*******************************************************************
�������ƣ�GetAllSectName(LIST<STRING>& SectNameList, int nGrpNo, int nFileNo)
����������������ź��ļ��Ż�ö��б�
���������
   ����1��int nGrpNo�����
   ����2��int nFileNo���ļ���
���������
   ����1��LIST<STRING>& SectNameList�������б�
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgFileMgr::GetAllSectName(LIST< STRING >& SectNameList, 
                                 int nGrpNo, int nFileNo)
{
    //���������m_mapCfgFileGrpӳ������ҵ���Ӧ�������ļ���pCfgFileGrp��
    //����pCfgFileGrp->GetAllSectName()
    MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo);
    if ( it != m_mapCfgFileGrp.end() )
    {
        CCfgFileGrp *pCfgFileGrp = (*it).second;
        if (pCfgFileGrp != NULL)
        {
            pCfgFileGrp->GetAllSectName(SectNameList,nFileNo);
        }
    }
}

/*******************************************************************
�������ƣ�GetCfgListFromSectName(const char* szSectName, 
                                 int nGrpNo, int nFileNo)
����������������š��ļ��źͶ������ĳ�����ļ�ĳ�ε�����������
���������
   ����1��const char* szSectName������
   ����2��int nGrpNo�����
   ����3��int nFileNo���ļ���
���������
�������ݣ�CFGITEM_LIST* ,����ָ�������Ĳ�������Ϣ�б�ָ��
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CFGITEM_LIST* CCfgFileMgr::GetCfgListFromSectName(const char *szSectName, 
                                                  int nGrpNo, int nFileNo)
{
	if (szSectName == NULL)
    {
        return FALSE;
    }
    
    //���������m_mapCfgFileGrpӳ������ҵ���Ӧ�������ļ���pCfgFileGrp��
    //����pCfgFileGrp->GetCfgListFromSectName()��
    MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo);
    if ( it != m_mapCfgFileGrp.end() )
    {
        CCfgFileGrp *pCfgFileGrp = (*it).second;
        if (pCfgFileGrp != NULL)
        {
            return pCfgFileGrp->GetCfgListFromSectName(szSectName,nFileNo);
        }
    }
	return NULL;
}

/*******************************************************************
�������ƣ�SetAllCfgItem(const char* szAllCfgItem, int nGrpNo, int nFileNo)
����������������ź��ļ������ò���������Ϣ
���������
   ����1��int nGrpNo�����
   ����2��int nFileNo���ļ���
���������
   ����1��char* szAllCfgItem������������Ϣ
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFileMgr::SetAllCfgItem(const char *szAllCfgItem, int nGrpNo,
                                int nFileNo)
{
	if (szAllCfgItem == NULL)
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s",
                 S_CFG_SET_ITEMINFO_INVALID);
        return FALSE;
    }

    //���������m_mapCfgFileGrpӳ������ҵ���Ӧ�������ļ���pCfgFileGrp��
    //����pCfgFileGrp->SetAllCfgItem()��
    MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo) ;
    if ( it != m_mapCfgFileGrp.end() )
    {
        CCfgFileGrp *pCfgFileGrp = (*it).second;
        if (pCfgFileGrp != NULL)
        {
            return pCfgFileGrp->SetAllCfgItem(szAllCfgItem, nFileNo);
        }
    }

    SNPRINTF(g_szErrorReason,
             sizeof(g_szErrorReason),
             "%s",
             S_CFG_SET_ITEMINFO_INVALID);
    return FALSE;
}


/*******************************************************************
�������ƣ�GetCfgFileClass(LIST< STRING >& GrpNameList)
������������ȡ�����ļ�����
���������
���������
   ����1��LIST< STRING >& GrpNameList; �ļ������б�
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/

void CCfgFileMgr::GetCfgFileClass(LIST< STRING >& GrpNameList)
{
    //��m_mapCfgFileClassӳ����а������˳��ȡ����Ӧ����������·������
    //��ȡ�������������뵽GrpNameList��
    MAP< int, STRING >::iterator it;
    
    GrpNameList.clear();
    for(it=m_mapCfgFileClass.begin(); it != m_mapCfgFileClass.end(); it++)
    {
        GrpNameList.push_back( (*it).second );
    }
}


/*******************************************************************
�������ƣ�GetAllCfgItem(char* szAllCfgItem, int nGrpNo, int nFileNo)
����������������ź��ļ��Ż�ò���������Ϣ
���������
   ����1��int nGrpNo�����
   ����2��int nFileNo���ļ���
���������
   ����1��STRING &strAllCfgItem������������Ϣ
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgFileMgr::GetAllCfgItem(STRING &strAllCfgItem , int nGrpNo, 
                                int nFileNo)
{
    //���������m_mapCfgFileGrpӳ������ҵ���Ӧ�������ļ���pCfgFileGrp��
    MAP< int, CCfgFileGrp* >::iterator itFind = m_mapCfgFileGrp.find(nGrpNo);
    if (itFind != m_mapCfgFileGrp.end())
    {
        //����pCfgFileGrp->GetAllCfgItem()��
        CCfgFileGrp *pCfgFileGrp = (*itFind).second;
        
        if (pCfgFileGrp != NULL)
        {
            pCfgFileGrp->GetAllCfgItem(strAllCfgItem, nFileNo);
        }
    }
}

/*******************************************************************
�������ƣ�UpdateCurrentCfg(int nGrpNo, char* szFilePath)
��������������ĳ��ĵ�ǰ�����ļ�����
���������
   ����1��int nGrpNo�����
   ����2��int nFileNo���ļ���
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFileMgr::UpdateCurrentCfg(int nGrpNo, const char* szFilePath)
{
	if (szFilePath == NULL)
    {
        return FALSE;
    }

	MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo);
	if (it != m_mapCfgFileGrp.end())
	{
		return ((*it).second)->UpdateCurrentCfg(szFilePath);
	}

	return FALSE;
}

/*******************************************************************
�������ƣ�UpdateLastCfg(int nGrpNo, char* szFilePath)
��������������ĳ��ĵ�ǰ�����ļ�����
���������
   ����1��int nGrpNo�����
   ����2��int nFileNo���ļ���
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFileMgr::UpdateLastCfg(int nGrpNo, const char* szFilePath)
{
	if (szFilePath == NULL)
    {
        return FALSE;
    }

	MAP< int, CCfgFileGrp* >::iterator it = m_mapCfgFileGrp.find(nGrpNo);
	if (it != m_mapCfgFileGrp.end())
	{
		return ((*it).second)->UpdateLastCfg(szFilePath);
	}

	return FALSE;
}