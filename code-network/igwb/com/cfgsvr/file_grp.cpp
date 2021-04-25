/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : file_grp.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CCfgFileGrp class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/
#include "file_grp.h"

//////////////////////////////////////////////////////////////////////
// CCfgFileGrp Class
//////////////////////////////////////////////////////////////////////

/*******************************************************************
�������ƣ�CCfgFileGrp()
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
CCfgFileGrp::CCfgFileGrp() : m_pFacFile(NULL), m_pSucFile(NULL), m_pCurFile(NULL)
{
}

/*******************************************************************
�������ƣ�~CCfgFileGrp()
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
CCfgFileGrp::~CCfgFileGrp()
{
    if (m_pFacFile != NULL)
    {
        delete m_pFacFile;
        m_pFacFile = NULL;
    }
    if (m_pSucFile != NULL)
    {
        delete m_pSucFile;
        m_pSucFile = NULL;
    }
    if (m_pCurFile != NULL)
    {
        delete m_pCurFile;
        m_pCurFile = NULL;
    }
}

/*******************************************************************
�������ƣ�Init(char* szFilePath)
������������ʼ���ļ���
���������
   ����1��char* szFilePath���ļ���·��(������չ���������ļ�ȫ·��)
���������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFileGrp::Init(const char *szFilePath)
{
	//�������Ϸ���
    if (szFilePath == NULL)
    {
        return FALSE;
    }
    char szFileFullPath[MAX_PATH];

	sprintf(szFileFullPath, "%s%s", szFilePath, ".bas");
    //��ʼ�������������ļ�m_pFacFile��
    if (m_pFacFile != NULL)
    {
        delete m_pFacFile;
    }
    m_pFacFile = new  CCfgFile();
    if(ACE_OS::access(szFileFullPath, F_OK) != 0)
    {
        STRING strResIni = szFilePath;
        strResIni       += ".ini";
        FileCopy(strResIni.c_str(), szFileFullPath);
    }
    if (!m_pFacFile->Init(szFileFullPath))
    {
        return FALSE;
    }
    
    sprintf(szFileFullPath, "%s%s", szFilePath, ".lst");
    //��ʼ���ϴγɹ��������ļ�m_pSucFile��
    if (m_pSucFile != NULL)
    {
        delete m_pSucFile;
    }
    m_pSucFile = new  CCfgFile();
    if(ACE_OS::access(szFileFullPath, F_OK) != 0)
    {
        STRING strResIni = szFilePath;
        strResIni       += ".ini";
        FileCopy(strResIni.c_str(), szFileFullPath);
    }
    if (!m_pSucFile->Init(szFileFullPath))
    {
        return FALSE;
    }
    
    sprintf(szFileFullPath, "%s%s", szFilePath, ".ini");
    //��ʼ����ǰ�������ļ�m_pCurFile��
    if (m_pCurFile != NULL)
    {
        delete m_pCurFile;
    }
    m_pCurFile = new  CCfgFile();	
    if (!m_pCurFile->Init(szFileFullPath))
    {
        return FALSE;
    }
    
    return TRUE;
}

/*******************************************************************
�������ƣ�GetAllSectName(LIST<STRING>& SectNameList, int nFileNo)
���������������ļ��Ż�����ж���
���������
   ����1��int nFileNo���ļ���
          ע������Լ����0��ʾ�����ģ�1��ʾ�ϴγɹ��ģ�2��ʾ��ǰ��
���������
   ����1��LIST<STRING>& SectNameList�����ж����б�
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgFileGrp::GetAllSectName(LIST< STRING >& SectNameList, int nFileNo)
{
    switch(nFileNo)
    {
    case FILE_TYPE_FAC:
        //��������
        if (m_pFacFile != NULL)
        {
            m_pFacFile->GetAllSectName(SectNameList);
        }
        break;
    case FILE_TYPE_SUC:
        //�ϴ�����
        if (m_pSucFile != NULL)
        {
            m_pSucFile->GetAllSectName(SectNameList);
        }
        break;
    case FILE_TYPE_CUR:
        //��ǰ����
        if (m_pCurFile != NULL)
        {
            m_pCurFile->GetAllSectName(SectNameList);
        }
        break;
    default:
        break;
    }
}

/*******************************************************************
�������ƣ�GetCfgListFromSectName( 
                            const char* szSectName, int nFileNo)
���������������ļ��źͶ�����ô˶εĲ���������Ϣ
���������
   ����1��const char* szSectName������
   ����2��int nFileNo���ļ���
���������
�������ݣ�CFGITEM_LIST* ,����ָ�������Ĳ�������Ϣ�б�ָ��
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CFGITEM_LIST* CCfgFileGrp::GetCfgListFromSectName(const char *szSectName, int nFileNo)
{
	if (szSectName == NULL)
    {
        return FALSE;
    }
    
    switch(nFileNo)
    {
    case FILE_TYPE_FAC:
        //��������
        if (m_pFacFile != NULL)
        {
            return m_pFacFile->GetCfgListFromSectName(szSectName);
        }
        return NULL;
    case FILE_TYPE_SUC:
        //�ϴ�����
        if (m_pSucFile != NULL)
        {
            return m_pSucFile->GetCfgListFromSectName(szSectName);
        }
        return NULL;
    case FILE_TYPE_CUR:
        //��ǰ����
        if (m_pCurFile != NULL)
        {
            return m_pCurFile->GetCfgListFromSectName(szSectName);
        }
        return NULL;
    default:
        break;
    }

	return NULL;
}


/*******************************************************************
�������ƣ�SetAllCfgItem(const char* szAllCfgItem, int nFileNo)
���������������ļ������ò���������Ϣ
���������
   ����1��int nFileNo���ļ���
   ����2��const char* szAllCfgItem�����в���������Ϣ
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFileGrp::SetAllCfgItem(const char *szAllCfgItem, int nFileNo)
{
	switch(nFileNo)
    {
    case FILE_TYPE_FAC:
        //��������
        if (m_pFacFile != NULL)
        {
            return m_pFacFile->SetAllCfgItem(szAllCfgItem);
        }
		break;
    case FILE_TYPE_SUC:
        //�ϴ�����
        if (m_pSucFile != NULL)
        {
            return m_pSucFile->SetAllCfgItem(szAllCfgItem);
        }
		break;
    case FILE_TYPE_CUR:
        //��ǰ����
        if (m_pCurFile != NULL)
        {
            return m_pCurFile->SetAllCfgItem(szAllCfgItem);
        }
        break;
    default:
        break;
    }

    return FALSE;
}

/*******************************************************************
�������ƣ�GetAllCfgItem(char* szAllCfgItem, int nFileNo)
���������������ļ��Ż�����в���������Ϣ
���������
   ����1��int nFileNo���ļ���
���������
   ����1��STRING &szAllCfgItem�����в���������Ϣ
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgFileGrp::GetAllCfgItem(STRING &strAllCfgItem, int nFileNo)
{
	switch(nFileNo)
    {
    case FILE_TYPE_FAC:
        //��������
        if (m_pFacFile != NULL)
        {
            m_pFacFile->GetAllCfgItem(strAllCfgItem);
        }
        break;
    case FILE_TYPE_SUC:
        //�ϴ�����
        if (m_pSucFile != NULL)
        {
            m_pSucFile->GetAllCfgItem(strAllCfgItem);
        }
        break;
    case FILE_TYPE_CUR:
        //��ǰ����
        if (m_pCurFile != NULL)
        {
            m_pCurFile->GetAllCfgItem(strAllCfgItem);
        }
        break;
    default:
        break;
    }
}

/*******************************************************************
�������ƣ�UpdateCurrentCfg(const char* szFilePath)
�������������µ�ǰ�����ļ�
���������
   ����1��const char* szFilePath����ǰ�����ļ�ȫ·��
���������
�������ݣ��ɹ�����TRUE;ʧ�ܷ���FALSE;
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFileGrp::UpdateCurrentCfg(const char* szFilePath)
{
	//�����Ϸ��Լ��
    if (szFilePath == NULL)
    {
        return FALSE;
    }

    //��������ļ��ĺϷ���
    CCfgFile *pNewCurFile = new CCfgFile();
	if (!pNewCurFile->Init(szFilePath))
    {
        //�粻�ܳɹ���ʼ���ֹ����������̴����޸ĵ������ļ�������ԭ����
        //�ļ����������ļ�
        if (m_pCurFile != NULL)
        {
            m_pCurFile->WriteAllCfgItem();
        }
        return FALSE;
    }

    //�����ļ��Ϸ������������ļ�ӳ�������
    if (m_pCurFile != NULL)
	{
		delete m_pCurFile;
	}
    m_pCurFile = pNewCurFile;

    return TRUE;
}

/*******************************************************************
�������ƣ�UpdateLastCfg(const char* szFilePath)
���������������ϴ������ļ�
���������
   ����1��const char* szFilePath���ϴ������ļ�ȫ·��
���������
�������ݣ��ɹ�����TRUE;ʧ�ܷ���FALSE;
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFileGrp::UpdateLastCfg(const char* szFilePath)
{
	//�����Ϸ��Լ��
    if (szFilePath == NULL)
    {
        return FALSE;
    }

    //��������ļ��ĺϷ���
    CCfgFile *pNewLstFile = new CCfgFile();
	if (!pNewLstFile->Init(szFilePath))
    {
        //�粻�ܳɹ���ʼ���ϴ������ļ��������ϴ������ļ�ӳ������ݸ����ϴ������ļ�
        if (m_pSucFile != NULL)
        {
            m_pSucFile->WriteAllCfgItem();
        }
        return FALSE;
    }

    //�����ļ��Ϸ������������ļ�ӳ�������
    if (m_pSucFile != NULL)
	{
		delete m_pSucFile;
	}
    m_pSucFile = pNewLstFile;

    return TRUE;
}