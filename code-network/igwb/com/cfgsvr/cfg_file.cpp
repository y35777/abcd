/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : cfg_file.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CCfgFile class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/
#include "cfg_file.h"
#include "resource.h"
#include "../include/toolbox.h"

//��¼�������ò���ʧ��ԭ�򣬵�ʧ��ʱ��������Ϣ�������ͻ���; ������cfg_svr.cpp��
extern char g_szErrorReason[20*1024];

//////////////////////////////////////////////////////////////////////
// CCfgFile Class
//////////////////////////////////////////////////////////////////////

/*******************************************************************
�������ƣ�CCfgFile()
�������������캯��
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CCfgFile::CCfgFile() :  m_pCurItemList(NULL)
{
    m_szFilePath[0] = '\0';
	m_szCurSectName[0] = '\0';
}

/*******************************************************************
�������ƣ�~CCfgFile()
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
CCfgFile::~CCfgFile()
{
    //���m_mapCfgSectӳ�������
    ClearCfgSectMap();
}

/*******************************************************************
�������ƣ�Init(const char* szFilePath)
������������ʼ���������������ļ��ж�ȡ�����������������ֵ��������
          ��ӳ���
���������
   ����1��const char* szFilePath�������ļ�ȫ·��
���������
�������ݣ���ʼ���ɹ�����TRUE, ʧ���򷵻�FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFile::Init(const char *szFileName)
{
    //��ȡ�����ļ���ȫ·����
	SNPRINTF(m_szFilePath, MAX_PATH, "%s", szFileName);
    m_szFilePath[MAX_PATH-1] = '\0';
    
    //�������ļ���
    FILE *pFile = fopen(m_szFilePath,"rt");
    if (pFile == NULL)
    {
        TRACE(MTS_CFGSVR,
              "%s(%s)",
              S_CFG_OPEN_CFGFILE_ERR,
              m_szFilePath);
        return FALSE;
    }
    
    char szLine[MAX_LINE+1];
    //ѭ����ȡÿһ�����ݣ�
    while (!feof(pFile))
    {
        szLine[0] = '\0';
        //��ȡ�����ļ���һ��
        if (fgets(szLine, MAX_LINE, pFile) == NULL)
        {
            //�ļ��Ѷ�ȡ�ļ�β
            if (feof(pFile)) break;
            //���ļ�����
            TRACE(MTS_CFGSVR,
                  "%s(%s)",
                  S_CFG_READ_CFGFILE_ERR,
                  m_szFilePath);
            fclose(pFile);
            return FALSE;
        }
        //ȥ���س������м�ǰ��ո�
        char* pszDiv = strchr(szLine, ';');
        if(NULL != pszDiv)
        {
            pszDiv[0] = '\0';
        }
		StringLRTrim(szLine);
		if (szLine[0] != '\0')
		{
			//���ú���ParseALine()����ÿһ�����ݣ�
			if (!ParseALine(szLine))
			{
				fclose(pFile);
				return FALSE;
			}
		}
    }
    //�������һ�����ö�
    if (!AddItemListToCfgSectMap())
    {
        fclose(pFile);
        return FALSE;
    }
    //�ر��ļ���
    fclose(pFile);
    return TRUE;
}

/*******************************************************************
�������ƣ�AddItemListToCfgSectMap()
������������һ���öε�����������Ϣ�б��������ӳ���
���������
���������
�������ݣ�����ӳ���ɹ�����TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFile::AddItemListToCfgSectMap()
{
    //�������б�ΪNULL���ҵ��¶���������һ���öν�����
    //��Ҫ����m_szCurSectName��m_pCurItemList��m_mapCfgSect�н���ӳ���ϵ��
    if (m_pCurItemList != NULL)
    {
        //��INI�ļ�����û�д�Сд��һ�µ�,�粻һ����ini�����õĵ�һ��Ϊ��
        CFGSECT_MAP::iterator it; //add by zkq 2004-09-02
        for(it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++)
        {
            if(StringiCmp(m_szCurSectName,(*it).first.c_str()))
            {
                strcpy(m_szCurSectName,(*it).first.c_str());
                break;
            }
        }//end
	PAIR< CFGSECT_MAP::iterator, bool > pairRet;
        pairRet = m_mapCfgSect.insert( PAIR< STRING, CFGITEM_LIST >(STRING(m_szCurSectName),
            *m_pCurItemList) );
        if (!pairRet.second)
        {
            //��ͬһӳ����в��������������ͬ�Ķ�����������ϲ������ļ�
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_SECTION_REPEAT_ERR,
                  m_szFilePath,
                  m_szCurSectName);

           // return FALSE; //delete by zkq 2004-08-31
        }
        //��� m_pCurItemList �ռ�//add by zkq 2004-08-31
        /*while(!m_pCurItemList->empty())
        {
            SCfgItem *pCfgItem = m_pCurItemList->front();
            m_pCurItemList->pop_front();
            delete pCfgItem;
        }*/
        delete m_pCurItemList;
        //end
        m_pCurItemList = NULL;
	m_szCurSectName[0] = '\0';
    }
    return TRUE;
}

/*******************************************************************
�������ƣ�ParseALine(const char* szLine)
��������������һ��
���������
   ����1��const char* szLine��һ������(��ȥ���س������м�ǰ��ո�)
���������
�������ݣ������Ϊע���У���Ϸ��Ķ�������Ϸ�������������������ֵ��
          ��ɹ�����TRUE������ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFile::ParseALine(const char *szLine)
{
    //�����е�1���ַ�Ϊ��[���������ҡ�]��������Ҳ���������ʧ�ܣ�
    if (szLine[0] == '[')
    {
        const char *pdest = strchr(szLine,']');
        if (pdest == NULL)
        {
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_SECTION_INVALID,
                  m_szFilePath,
                  szLine);
            return FALSE;
        }
        //����ҵ�˵�����С�[]���ڵ��Ƕ���, ��ȡ������
        char szSection[MAX_LINE+1];
        szSection[0] = '\0';
        int  nLength = pdest - szLine -1;
        
        strncpy(szSection, &szLine[1], nLength);
        szSection[nLength] = '\0';
        
		StringLRTrim(szSection);
        nLength = strlen(szSection);
        if (nLength >= MAX_SECTNAME_LEN || nLength == 0) //������󳤶�Ӳ����
        {
            //����������Ϊ��
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_SECTION_INVALID,
                  m_szFilePath,
                  szSection);
            return FALSE;
        }
        
		//���������б�ΪNULL���ҵ��¶���������һ���öν���,����ӳ���
        if (!AddItemListToCfgSectMap())
        {
            return FALSE;
        }
		//���õ�ǰ���ö���
		strcpy(m_szCurSectName,szSection);

        return TRUE;
    }
    //��Ϊ�������У����÷��������еĺ���ParseCfgLine()��������
    return ParseCfgLine(szLine);
}

/*******************************************************************
�������ƣ�ParseCfgLine(const char* szLine)
��������������һ��������
���������
   ����1��const char* szLine����������(��ȥ���س������м�ǰ��ո�)
���������
�������ݣ��ɹ�ȡ�úϷ�������������������ֵ���򷵻�TRUE;
          ���򷵻�FALSE;
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFile::ParseCfgLine(const char *szLine)
{   
	//���ҡ�=�����Ҳ�����"="Ϊ��һ���ַ�������FALSE��
    const char *pdest = strchr(szLine, '=');
    if (pdest == NULL || pdest == szLine)
    {
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_ITEM_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szLine);
        return FALSE;
    }
    char szItemName[MAX_LINE+1];
    char szItemValue[MAX_LINE+1];
    int  nLength = pdest - szLine;
    
    //ȡ���������ַ���
    strncpy(szItemName, szLine, nLength);
    szItemName[nLength] = '\0';
    StringLRTrim(szItemName);
    nLength = strlen(szItemName);
    //����������Ӳ����
    if (nLength >= MAX_ITEMNAME_LEN || nLength == 0) 
    {
        //��������������Ϊ��
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_ITEMNAME_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szItemName);
        return FALSE;
    }
    
    //ȡ������ֵ�ַ���
    strcpy(szItemValue, pdest + 1);
    StringLRTrim(szItemValue);
    nLength = strlen(szItemValue);
    //������ֵ��Ӳ����
    if (nLength >= MAX_ITEMVALUE_LEN) 
    {
        //������ֵ������Ϊ��
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s:%s)",
              S_CFG_ITEMVALUE_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szItemName,
              szItemValue);
        return FALSE;
    }
    
    if (m_pCurItemList != NULL)
    {
        //�ж��Ƿ����ظ�������������ͬһ���ö���
        CFGITEM_LIST::iterator it;
        for(it=m_pCurItemList->begin(); it != m_pCurItemList->end(); it++)
        {
            if (StringiCmp((*it)->szItemName, szItemName))
            {
                TRACE(MTS_CFGSVR,
                      "%s(%s:%s:%s)",
                      S_CFG_ITEMNAME_REP_IN_ONESECT,
                      m_szFilePath,
                      m_szCurSectName,
                      szItemName);

                //return FALSE;
               return TRUE; //���ظ�������������ͬһ���ö���
            }
        }
    }
    else
    {
        m_pCurItemList = new CFGITEM_LIST();
    }
    //����һ��������Ϣ
    SCfgItem *pCfgItem = new SCfgItem();
    strcpy(pCfgItem->szItemName, szItemName);
    strcpy(pCfgItem->szItemValue, szItemValue);
    
    m_pCurItemList->push_back(pCfgItem);
    
    return TRUE;
}

/*******************************************************************
�������ƣ�GetAllSectName(LIST<STRING>& SectNameList)
����������������ж���
���������
���������
   ����1��LIST<STRING>& SectNameList�����ж����б�
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgFile::GetAllSectName(LIST< STRING >& SectNameList)
{
    //����ӳ���m_mapCfgSect���Ѷ���ȫ��ȡ�������ӵ�SectNameList
    CFGSECT_MAP::iterator it;
    
    SectNameList.clear();
    
    for( it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++ )
    {
        SectNameList.push_back( (*it).first );
    }
}

/*******************************************************************
�������ƣ�GetCfgListFromSectName(const char* szSectName)
�����������Ӷ�����ô˶�����������Ϣ
���������
   ����1��const char* szSectName������
���������
�������ݣ�CFGITEM_LIST* ,����ָ�������Ĳ�������Ϣ�б�ָ�� 
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CFGITEM_LIST* CCfgFile::GetCfgListFromSectName(const char *szSectName)
{
    //�������Ϸ���
    if (szSectName == NULL) 
    {
        return NULL;
    }
    
    CFGSECT_MAP::iterator it;

    //��ӳ���m_mapCfgSect�в��Ҷ���ΪszSectName��ӳ�䣻
    if ( (it=m_mapCfgSect.find( STRING(szSectName) )) != m_mapCfgSect.end() )
    {
        //�ҵ��������б�ָ��
        return &((*it).second);
    }

	return NULL;
}


/*******************************************************************
�������ƣ�SetAllCfgItem(const char* szAllCfgItem)
�������������ò���������Ϣ����"����^��������^������ֵ|����^��������^������ֵ",
          ��д����Ӧ�������ļ���
���������
   ����1��const char* szAllCfgItem������������Ϣ
���������
�������ݣ��ɹ�д�������ļ�����TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFile::SetAllCfgItem(const char *szAllCfgItem)
{
    //�����Ϸ��Լ��
    if (szAllCfgItem == NULL)
    {
        return FALSE;
    }
    //�������ԭ����ӳ���
    ClearCfgSectMap();
    
    m_pCurItemList = NULL;
    m_szCurSectName[0] = '\0';
    
    int nPos = 0;    //��ǰָ��λ��
    int nNextPos;
    int nLen;        //һ��������Ϣ����
    nLen = strlen(szAllCfgItem);
	//ѭ��ȡ��ÿ��������
    while (szAllCfgItem[nPos] != '\0')
    {
        const char *pdest = strchr(&szAllCfgItem[nPos],'|');
        if (pdest != NULL)
        {
            nLen = pdest - szAllCfgItem - nPos;
            nNextPos = nPos + nLen + 1;
        }
        else
        {
            nLen = strlen(&szAllCfgItem[nPos]);
            nNextPos = nPos + nLen;
        }
        if (nLen > 0)
        {
            //����洢һ����Ϣ�Ŀռ�
			char *pszCfgItem = new  char[nLen+1];
            //ȡ�����ơ�����^��������^������ֵ����һ�������������
            strncpy(pszCfgItem, szAllCfgItem + nPos, nLen);
            pszCfgItem[nLen] = '\0';
            StringLRTrim(pszCfgItem);
            
            if (pszCfgItem[0] != '\0')
            {
                //���ú���ParseCfgItem()���������ơ�����^��������^������ֵ����һ������������ݣ�
                if ( !ParseCfgItem(pszCfgItem) )
				{
					//����ʧ�ܣ���ָ�ԭ�б�����
                    ClearCfgSectMap();
                    Init(m_szFilePath);
                    //�ͷ���ʱ�ռ�
                    delete [] pszCfgItem;
                    return FALSE;
				}
            }
            //�ͷ���ʱ�ռ�
            delete [] pszCfgItem;
        }
        
        nPos = nNextPos;  //�ƶ�ָ��λ��
    }

	//д�����ļ�
	return WriteAllCfgItem();
}

/*******************************************************************
�������ƣ�ParseCfgItem(const char* szCfgItem)
���������������������"����^��������^������ֵ"
���������
   ����1��const char* szCfgItem����������Ϣ
���������
�������ݣ��ɹ�ȡ�úϷ��Ķ���������������������ֵ����TRUE;
          ���򷵻�FALSE;
���ú�����
����������
����ʱ�䣺denqwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgFile::ParseCfgItem(const char *szCfgItem)
{  
    char szSectName[MAX_LINE+1]  = "";
    char szItemName[MAX_LINE+1]  = "";
    char szItemValue[MAX_LINE+1] = "";

    //�ж��������Ƿ�Ϊ�Ϸ���ʽ��"����^��������^������ֵ"
    const char *pSectDiv = strchr(szCfgItem,'^');
    if (pSectDiv == NULL || pSectDiv == szCfgItem)
    {
        //��¼����ԭ��
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_CFGINFO_INVALID,
                 szCfgItem);
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR, g_szErrorReason);
        return FALSE;
    }

    //�ж϶����Ƿ�̫����Ϊ��
    int nCopyLen = int(pSectDiv - szCfgItem);
    memcpy(szSectName, szCfgItem, nCopyLen);
    szSectName[nCopyLen] = '\0';
    StringLRTrim(szSectName);
    if(szSectName[0] == '\0')
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_SECTNAME_NULL,
                 szCfgItem);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    if(strlen(szSectName) >= MAX_SECTNAME_LEN)
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_SECTNAME_TOOLONG,
                 szCfgItem);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }

    //�ж������Ƿ�̫����Ϊ��
    const char *pItemName = pSectDiv + 1;
    const char* pItemDiv = strchr(pItemName, '^');
    if (pItemDiv == NULL || pItemDiv == pItemName)
    {
        //��¼����ԭ��
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_CFGINFO_INVALID,
                 szCfgItem);
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR, g_szErrorReason);
        return FALSE;
    }
    nCopyLen = int(pItemDiv - pItemName);
    memcpy(szItemName, pItemName, nCopyLen);
    szItemName[nCopyLen] = '\0';
    StringLRTrim(szItemName);
    if(szItemName[0] == '\0')
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_ITEMNAME_NULL,
                 szCfgItem);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    if(strlen(szItemName) >= MAX_ITEMNAME_LEN)
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)",
                 S_CFG_SET_ITEMNAME_TOOLONG,
                 szSectName,
                 szItemName);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }

    //�ж���ֵ�Ƿ�̫����Ϊ��
    const char *pItemValue = pItemDiv + 1;
    strcpy(szItemValue, pItemValue);
    StringLRTrim(szItemValue);
    if (szItemValue[0] == '\0')
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)",
                 S_CFG_SET_ITEMVALUE_NULL,
                 szSectName,
                 szItemName);
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    if(strlen(szItemValue) >= MAX_ITEMVALUE_LEN)
    {
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)",
                 S_CFG_SET_ITEMVALUE_TOOLONG,
                 szSectName,
                 szItemName);
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
/*
    
    //����������szSectName��
    const char *pdest = strchr(szCfgItem,'^');
    if (pdest == NULL || pdest == szCfgItem)
    {   
        //��¼����ԭ��
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)"
                 S_CFG_SET_CFGINFO_INVALID,
                 szCfgItem);
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR, g_szErrorReason);
        return FALSE;
    }
    int nSectNameLen = pdest - szCfgItem;    //��������  
    strncpy(szSectName,szCfgItem,nSectNameLen);
    szSectName[nSectNameLen] = '\0';
    StringLRTrim(szSectName);
    int nLen = strlen(szSectName);
    //�ж϶����Ƿ�̫����Ϊ��
    if ( nLen >= MAX_SECTNAME_LEN  || nLen == 0 )
    {   
        //��¼����ԭ��
        if (nLen == 0)
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s)"
                     S_CFG_SET_SECTNAME_NULL,
                     szCfgItem);
        }
        else
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s)"
                     S_CFG_SET_SECTNAME_TOOLONG,
                     szCfgItem);
        }
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
	//�ѵ��ַ���β�������޲������� 
    if (szCfgItem[nSectNameLen+1] == '\0')
    {
        //��¼����ԭ��
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)"
                 S_CFG_SET_NO_ITEMNAME,
                 szCfgItem);
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    
	//ȡ��������
    szItemName[0] = '\0';
    pdest = strchr(szCfgItem + nSectNameLen + 1,'^');
    //�жϲ��������Ƿ�Ϊ��
    if (pdest == NULL || pdest == szCfgItem + nSectNameLen + 1)
    {
        //ȡ����������Ϊ�˼�¼��־��Ҫ
        if (pdest == NULL)
        {
            strcpy(szItemName, szCfgItem + nSectNameLen + 1);
            StringLRTrim(szItemName);
        }
        //��¼����ԭ��: �޲�����ֵ
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)"
                 S_CFG_SET_NO_ITEMVALUE,
                 szSectName,
                 szItemName);
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR,g_szErrorReason);

		return FALSE;
    }

    int nItemNameLen = pdest - szCfgItem - nSectNameLen -1;
    strncpy(szItemName,szCfgItem + nSectNameLen + 1,nItemNameLen);
    szItemName[nItemNameLen] = '\0';
    StringLRTrim(szItemName);
    nLen = strlen(szItemName);
    //�жϲ��������Ƿ�̫����Ϊ��
    if (nLen >= MAX_ITEMNAME_LEN || nLen == 0)
    {
        //��¼����ԭ��
        if (nLen == 0)
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s:%s)"
                     S_CFG_SET_ITEMNAME_NULL,
                     szSectName,
                     szItemName);
        }
        else
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s:%s)"
                     S_CFG_SET_ITEMNAME_TOOLONG,
                     szSectName,
                     szItemName);
        }
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_SET_ITEMNAME_INVALID,
              m_szFilePath,
              szSectName,
              szItemName);
        return FALSE;
    }
    //�ѵ��ַ���β�������޲�����ֵ
	if (szCfgItem[nSectNameLen + nItemNameLen + 2] == '\0')
    {
        //��¼����ԭ��
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s:%s)"
                 S_CFG_SET_ITEMVALUE_NULL,
                 szSectName,
                 szItemName);
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    
    //ȡ������ֵ
    strncpy(szItemValue, &szCfgItem[nSectNameLen + nItemNameLen + 2], MAX_LINE);
    szItemValue[MAX_LINE] = '\0';
    StringLRTrim(szItemValue);
    nLen = strlen(szItemValue);
    //�жϲ�����ֵ�Ƿ�̫����Ϊ��
    if (nLen >= MAX_ITEMVALUE_LEN || nLen == 0)
    {
        //��¼����ԭ��
        if (nLen == 0)
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s:%s)"
                     S_CFG_SET_ITEMVALUE_NULL,
                     szSectName,
                     szItemName);
        }
        else
        {
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s:%s)"
                     S_CFG_SET_ITEMVALUE_TOOLONG,
                     szSectName,
                     szItemName);
        }
        //дTrace�ļ� 
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    } */

    
    SCfgItem *pCfgItem = new  SCfgItem;
    strcpy(pCfgItem->szItemName, szItemName);
    strcpy(pCfgItem->szItemValue, szItemValue);
    
	//����˶�������ӳ����У��򽫴������������޸Ĵ˶ζ�Ӧ���������б�
	CFGSECT_MAP::iterator it = m_mapCfgSect.find( STRING(szSectName) );
	if (it != m_mapCfgSect.end())
	{
		CFGITEM_LIST &ItemList = (*it).second;
		CFGITEM_LIST::iterator it_list;
		for (it_list = ItemList.begin(); it_list != ItemList.end(); it_list++)
		{
			//�����������б��У�ֻ�޸���ֵ�������������ͬ�ġ�����^����^��ֵ����
            //��ȡ����ֵ
			if (strcmp((*it_list)->szItemName, szItemName) == 0)
			{
				strcpy((*it_list)->szItemValue, szItemValue);
				return TRUE;
			}
		}	
		ItemList.push_back(pCfgItem);
		return TRUE;
	}
	//���򣬼����¶������������б�ӳ�����
	else
	{
		CFGITEM_LIST *pItemList = new CFGITEM_LIST();
        pItemList->push_back(pCfgItem);
		m_mapCfgSect.insert(PAIR< STRING, CFGITEM_LIST >(STRING(szSectName), 
            *pItemList) );
		return TRUE;
	}
    
	return TRUE;
}

/*******************************************************************
�������ƣ�WriteAllCfgItem()
����������д���в���������Ϣ
���������
���������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE��
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
#ifdef _PLATFORM_WIN32
#define S_LN_CR "\r\n"
#else
#define S_LN_CR "\n"
#endif
BOOL CCfgFile::WriteAllCfgItem()
{
    //�������ļ�������Ϊ��д�ʹ���ģʽ��
    FILE *pFile = fopen(m_szFilePath,"wb");//by zkq  2004-0713 SWPD06718
    if (pFile == NULL)
    {
        //��¼����ԭ��
        SNPRINTF(g_szErrorReason,
                 sizeof(g_szErrorReason),
                 "%s(%s)",
                 S_CFG_SET_OPENFILE_ERROR,
                 m_szFilePath);
        //дTrace�ļ�
        TRACE(MTS_CFGSVR,g_szErrorReason);
        return FALSE;
    }
    
    //����m_mapCfgSectӳ�������ȡ��һ���κʹ˶ε������
    CFGSECT_MAP::iterator it;
    for(it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++)
    {
        //д��һ������
        char szOutBuf[2048];
        SNPRINTF(szOutBuf,
                 sizeof(szOutBuf),
                 "[%s]%s",
                 ((*it).first).c_str(),
                 S_LN_CR);
        int nLen = strlen(szOutBuf);
        if ( fwrite(szOutBuf, nLen, 1, pFile) != 1 )
        { 
            //��¼����ԭ��
            fclose(pFile);
            SNPRINTF(g_szErrorReason,
                     sizeof(g_szErrorReason),
                     "%s(%s)",
                     S_CFG_SET_WRITEFILE_ERROR,
                     m_szFilePath);
            //дTrace�ļ�
            TRACE(MTS_CFGSVR,g_szErrorReason);
            return FALSE;
        }
        //дһ���˶ε����������
        CFGITEM_LIST *pItemList = &((*it).second);
        
        CFGITEM_LIST::iterator itItem;
        for( itItem=pItemList->begin(); itItem != pItemList->end(); itItem++ )
        {
            SCfgItem *Item = (*itItem);
            if (Item != NULL)
            {
                SNPRINTF(szOutBuf,
                         sizeof(szOutBuf),
                         "%s=%s%s",
                         Item->szItemName,
                         Item->szItemValue,
                         S_LN_CR);
                int nItemLen = strlen(szOutBuf);
                if ( fwrite(szOutBuf, nItemLen, 1, pFile) != 1 )
                {
                    //��¼����ԭ��
                    fclose(pFile);
                    SNPRINTF(g_szErrorReason,
                             sizeof(g_szErrorReason),
                             "%s(%s)",
                             S_CFG_SET_WRITEFILE_ERROR,
                             m_szFilePath);
                    //дTrace�ļ�
                    TRACE(MTS_CFGSVR,g_szErrorReason);
                    return FALSE;
                }
            }
        }
        //ÿ�μ����һ����
        fprintf(pFile, S_LN_CR);
    }
    fclose(pFile);
    return TRUE;
}

/*******************************************************************
�������ƣ�GetAllCfgItem(STRING &strAllCfgItem)
����������������в���������Ϣ
���������
���������
   ����1��STRING &strAllCfgItem������������Ϣ 
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgFile::GetAllCfgItem(STRING &strAllCfgItem)
{    
    //������ַ�����
    strAllCfgItem = "";
    
    //��ӳ�����ȡ������������Ϣ����װΪ"����^��������^������ֵ|"���ַ�����
    CFGSECT_MAP::iterator it;
    for( it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++)
    {
        CFGITEM_LIST &ItemList = (*it).second;
        CFGITEM_LIST::iterator itItem;
        
        char szItemBuf[MAX_LINE];
        for(itItem=ItemList.begin(); itItem != ItemList.end(); itItem++)
        {
            sprintf(szItemBuf, "%s^%s^%s|", ((*it).first).c_str(), 
                (*itItem)->szItemName,(*itItem)->szItemValue); 
            strAllCfgItem.append(szItemBuf); 
        }
    }

    //ȥ�����һ��"|"
    if (strAllCfgItem != "")
    {
        strAllCfgItem[strAllCfgItem.size()-1] = '\0';
    }
}

/*******************************************************************
�������ƣ�ClearCfgSectMap()
�����������������ӳ���
���������
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgFile::ClearCfgSectMap()
{
    //���ӳ���
    CFGSECT_MAP::iterator it;
    for(it=m_mapCfgSect.begin(); it != m_mapCfgSect.end(); it++)
    {
        //����б�����
        CFGITEM_LIST &ItemList = (*it).second;
        while( !ItemList.empty() )
        {
            SCfgItem *pCfgItem = ItemList.front();
            ItemList.pop_front();
            delete pCfgItem;
        }
    }
    m_mapCfgSect.clear();
    //���δ����������б� 
    if (m_pCurItemList != NULL)
    {
        while(!m_pCurItemList->empty())
        {
            SCfgItem *pCfgItem = m_pCurItemList->front();
            m_pCurItemList->pop_front();
            delete pCfgItem;
        }
        delete m_pCurItemList;
        m_pCurItemList = NULL;
    }
}
