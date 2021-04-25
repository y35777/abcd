/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : para_file.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CParaFile class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/

#include "para_file.h"
#include "resource.h"

/*******************************************************************
�������ƣ�CParaFile()
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
CParaFile::CParaFile() :  m_pCurParaItem(NULL)
{
    m_szFilePath[0] = '\0';
	m_szCurSectName[0] = '\0';
}

/*******************************************************************
�������ƣ�~CParaFile()
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
CParaFile::~CParaFile()
{
    //���m_mapParaSecttӳ�������
    PARASECT_MAP::iterator  it;    
	
    for(it=m_mapParaSect.begin(); it != m_mapParaSect.end(); it++)
	{
		PARAITEM_LIST &ItemList = (*it).second;
        while( !ItemList.empty() )
        {
            SParaItem *pParaItem = ItemList.front();
            ItemList.pop_front();
            delete pParaItem;
        }
	}
    m_mapParaSect.clear();
    if (m_pCurParaItem != NULL)
    {
        delete m_pCurParaItem;
        m_pCurParaItem = NULL;
    }
}

/*******************************************************************
�������ƣ�Init(const char* szFilePath)
������������ʼ���ļ�
���������
   ����1��const char* szFilePath���ļ�·��
���������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CParaFile::Init(const char *szFileName)
{
	SNPRINTF(m_szFilePath, MAX_PATH, "%s", szFileName);
	m_szFilePath[MAX_PATH - 1] = '\0';
    
    FILE *pFile = fopen(m_szFilePath,"rt");
    if (pFile == NULL)
    {
        TRACE(MTS_CFGSVR,
              "%s(%s)",
              S_CFG_OPEN_CFGFILE_ERR,
              m_szFilePath);
        return FALSE;
    }
    
    char szLineBuf[MAX_LINE+1];
    //ѭ����ȡÿһ�����ݣ�
    while(!feof(pFile))
    {
        szLineBuf[0] = '\0';
        if (fgets(szLineBuf,MAX_LINE, pFile) == NULL)
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
        //ȥ���س������з���ǰ��ո�
        StringLRTrim(szLineBuf);
		if (szLineBuf[0] != '\0')
		{
			//����һ��
			if (!ParseALine(szLineBuf))
			{
				//������
				fclose(pFile);
				return FALSE;
			}
		}
    }
	//�����һ��������Ϣ���뵽�����νṹ�б�
	if (!AddParaItemToParaSectMap())
	{
		fclose(pFile);
        return FALSE;
	}

    //�ر��ļ���
    fclose(pFile);
    
    return TRUE;
}

/*******************************************************************
�������ƣ�AddParaItemToParaSectMap()
������������һ��������Ϣ���뵽�����νṹ�б�
���������
���������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CParaFile::AddParaItemToParaSectMap()
{
	//�ж�m_pCurParaItem�Ƿ�ΪNULL����ΪNULL��ʾ������һ��������Ϣ���������ӳ���
    if (m_pCurParaItem != NULL)
    {
        //����������Ϣ�������ԣ���������Ϣ����ָ��������������������
		if (m_SectNameList.empty() ||                       //�������������������Ϊ��
			m_pCurParaItem->nItemType == PARATYPE_UNKNOWN)           //�������������ͱ���ָ��
		{
			TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PRARFILE_SECTINFO_INVALID,
                  m_szFilePath,
                  m_szCurSectName);
            
			return FALSE;
		}
		
        LIST< STRING >::iterator it = m_SectNameList.begin();
        LIST< STRING >::iterator itNext = m_SectNameList.begin();
        LIST< STRING >::iterator itEnd = m_SectNameList.end();
        for( ; it!=m_SectNameList.end(); it++)
        {
            PARASECT_MAP::iterator itSect; //add by zkq 2004-09-01
            for(itSect = m_mapParaSect.begin(); itSect != m_mapParaSect.end(); itSect++)
            {
                if(StringiCmp((*it).c_str(),(*itSect).first.c_str()))
                {
                    (*it) = (*itSect).first ; //��Сд��ͬʱֻ����һ��  
                    break;
                }                
            }           
            //end     
            itNext++;
            PARASECT_MAP::iterator itFind = m_mapParaSect.find( (*it) );
            //�����������ڶεĶ����б��Ƿ��Ѽ���ӳ���
            if (itFind == m_mapParaSect.end())
            {
                //��δ���룬������PARAITEM_LIST�б�,������ӳ���
                PARAITEM_LIST* pItemList = new PARAITEM_LIST; 
                //�粻��������Ĳ��������뿽��һ�ݸ������ټ����б�
                //����ֱ�Ӽ�����
                if (itNext != itEnd)
                {
                    SParaItem *pInsParaItem = new SParaItem;
                    *pInsParaItem = *m_pCurParaItem;
                    pItemList->push_back(pInsParaItem);
                }
                else
                {
                    pItemList->push_back(m_pCurParaItem);
                }
                
                m_mapParaSect.insert( PAIR< STRING, PARAITEM_LIST >((*it), 
                    *pItemList ));
                if(pItemList != NULL) //�ͷſռ� add by zkq 2004-08-31
                {
                    delete pItemList;
                    pItemList = NULL;
                }
            }
            else
            {
                //���Ѽ���
                PARAITEM_LIST& ItemList = (*itFind).second;
				PARAITEM_LIST::iterator itItem;
				//���ͬһ���в��ܴ���������������ͬ�Ĳ�����
				for(itItem=ItemList.begin(); itItem != ItemList.end(); itItem++)
				{
					if (strcmp((*itItem)->szItemName, m_pCurParaItem->szItemName) == 0) 
					{
						//����������������ͬ�Ĳ�������ͬһ����
                        TRACE(MTS_CFGSVR,
                              "%s(%s:%s:%s)",
                              S_CFG_ITEMINFO_REP_IN_ONERELA,
                              m_szFilePath,
                              m_pCurParaItem->szItemName,
                              (*it).c_str());
					    //������������ͬ�Ĳ�������ͬһ����,����TRACE
                        if(m_pCurParaItem != NULL) //add by zkq 2004-08-31
                        {
                            delete m_pCurParaItem;
                            m_pCurParaItem = NULL;
                        }
                        m_SectNameList.clear();
					    return TRUE;
					}
				}
                if (itNext != itEnd)
                {
                    SParaItem *pInsParaItem = new SParaItem;
                    *pInsParaItem = *m_pCurParaItem;
                    ItemList.push_back(pInsParaItem);
                }
                else
                {
                    ItemList.push_back(m_pCurParaItem);
                }
            }
        }
        //�Ѳ����б�,����ն����б�׼ȷ������һ����
        m_pCurParaItem = NULL;
        m_SectNameList.clear();
    }
    
	return TRUE;
}

/*******************************************************************
�������ƣ�ParseALine(char* szLine)
��������������һ��
���������
   ����1��char* szLine��һ������(��ȥ���س������з���ǰ��ո�)
���������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CParaFile::ParseALine(const char *szLine)
{   
    //�����е�1���ַ��ǡ�#��,��������Ϊע���У����ԣ�
    if (szLine[0] == '#') 
    {
        return TRUE;
    }
    //�����е�1���ַ�Ϊ��[���������ҡ�]��������Ҳ���������ʧ�ܣ�
    if (szLine[0] == '[')
    {
        const char *pdest = strchr(szLine,']');
        
        if (pdest == NULL)
        {
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PARAFILE_SECTNAME_INVALID, 
                  m_szFilePath,
                  szLine);
           
			return FALSE;
        }
        //����ҵ�˵�����С�[]���ڵ��ǲ���������
        char szSection[MAX_LINE+1];
        int  nLength = pdest - szLine -1;
        
        strncpy(szSection, &szLine[1], nLength);
        szSection[nLength] = '\0';
        StringLRTrim(szSection);
        nLength = strlen(szSection); 
		
        if (nLength == 0 || nLength >= MAX_ITEMNAME_LEN)
		{
			//��������Ϊ�ջ�̫��
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PARAFILE_SECTNAME_INVALID,
                  m_szFilePath,
                  szSection);
            
			return FALSE;
		}
        strcpy(m_szCurSectName,szSection);
        
		//����һ��������Ϣ���뵽�����νṹ�б�
		if (!AddParaItemToParaSectMap())
		{
			return FALSE;
		}
        		
        //�������µĲ�����,�½�һ��������Ϣ�ṹ,��ָ��������
		m_pCurParaItem = new  SParaItem;
		strcpy(m_pCurParaItem->szItemName,szSection);

        return TRUE;	
    }
    //���÷��������еĺ���ParseParaLine()��������
    return ParseParaLine(szLine);
}

/*******************************************************************
�������ƣ�ParseParaLine(const char *szLine)
��������������һ������������
���������
   ����1��const char* szLine��һ������(��ȥ���س������з���ǰ��ո�)
���������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CParaFile::ParseParaLine(const char *szLine)
{
    //��������Ϣ���ĵ�һ�в��Ƕ�����Ϣ,���ǲ�����Ϣʱ,m_pCurParaItemΪ��
    if (m_szCurSectName[0] == '\0')
    {
        TRACE(MTS_CFGSVR,
              "%s(%s)",
              S_CFG_PARAFILE_STYLE_INVALID,
              m_szFilePath);
        return FALSE;
    }

	//���ҡ�=�����Ҳ����������������Ϊ�գ�����FALSE��
    const char *pdest = strchr(szLine, '=');
    if (pdest == NULL || pdest == szLine)
    {
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_PARAFILE_ITEM_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szLine);
        return FALSE;
    }
    char szItem[MAX_LINE+1];
    char szValue[MAX_LINE+1];
    int  nLength = pdest - szLine;
    
    //ȡ���������ַ���
    strncpy(szItem, szLine, nLength);
    szItem[nLength] = '\0';
    StringLRTrim(szItem);
	if (szItem[0] == '\0')
	{
		//������������Ϊ��
        TRACE(MTS_CFGSVR,
              "%s(%s:%s:%s)",
              S_CFG_PARAFILE_ITEM_INVALID,
              m_szFilePath,
              m_szCurSectName,
              szLine);
 		return FALSE;
	}
    
    //ȡ������ֵ�ַ���
    strcpy(szValue, pdest+1);
    StringLRTrim(szValue);
	    
	//��������������������ֵ
    if (strcmp(szItem,SECTLIST) == 0)
    {
        //�����SECTLIST����ʾ�ǵ�ǰ���������������(��"����1^����2^����3")��
        //��������Ķ����б�
        m_SectNameList.clear();
        char *token = strtok(szValue,"^");
        while (token != NULL)
        {
            StringLRTrim(token);
			if( strlen(token) >= MAX_SECTNAME_LEN )
			{
				//����̫��
                TRACE(MTS_CFGSVR,
                      "%s(%s:%s:%s)",
                      S_CFG_PARAFILE_RELASECT_TOOLEN,
                      m_szFilePath,
                      m_szCurSectName,
                      token);
				
				return FALSE;
			}
			else if (strlen(token) > 0)
			{
                //�������Ƿ����ظ���û������뵱ǰ�����б�
				LIST< STRING >::iterator it;
                it = std::find(m_SectNameList.begin(),m_SectNameList.end(),STRING(token));
				if (it == m_SectNameList.end())
				{
                    m_SectNameList.push_back(STRING(token));
				}
			}
            token = strtok(NULL,"^");
        }//end while
        //��������������������Ϊ��
		if (m_SectNameList.empty())
        {
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PARAFILE_RELASECT_NULL,
                  m_szFilePath,
                  m_szCurSectName);

            return FALSE;
        }
        return TRUE;      
    }
    if (strcmp(szItem,RELATION) == 0)
    {
        //�����SECTRELA����ʾ����ض���
        strncpy(m_pCurParaItem->szParaRelaSect,szValue,MAX_SECTNAME_LEN-1);
        m_pCurParaItem->szParaRelaSect[MAX_SECTNAME_LEN-1] = '\0';
        return TRUE;
    }
    if (strcmp(szItem,TYPE) == 0)
    {
        //�����TPYE����ʾ������: ������"NUM","STR","ENUM","IP"�ַ�����ʾ 
        m_pCurParaItem->nItemType = GetParaDataType(szValue);
        if (m_pCurParaItem->nItemType == PARATYPE_UNKNOWN)
        {	//��Ч��������
            TRACE(MTS_CFGSVR,
                  "%s(%s:%s)",
                  S_CFG_PARAFILE_DATATYPE_INVALID,
                  m_szFilePath,
                  m_szCurSectName);
            
            return FALSE;
        }
        return TRUE;
    }
    if (strcmp(szItem,DESC) == 0)
    {
        //�����DESC����ʾ������
        strncpy(m_pCurParaItem->szItemDesc,szValue,MAX_ITEMDESC_LEN-1);
        m_pCurParaItem->szItemDesc[MAX_ITEMDESC_LEN-1] = '\0';
        return TRUE;
    }
    if (strcmp(szItem,DEFAULT) == 0)
    {
        //�����DEFAULT����ʾ��ȱʡֵ
        strncpy(m_pCurParaItem->szDefaultValue,szValue,MAX_ITEMVALUE_LEN-1);
        m_pCurParaItem->szDefaultValue[MAX_ITEMVALUE_LEN-1] = '\0';
        return TRUE;
    }
    if (strcmp(szItem,MUSTFLAG) == 0)
    {
        //�����MUSTFLAG����ʾ�Ǳ����־
        if (strcmp(szValue,"YES") == 0)
        {
            m_pCurParaItem->bIsMustCfg = TRUE;
        }
               
        return TRUE;
    }
    if (strcmp(szItem,UPDATEFLAG) == 0)
    {   //�����UPDATEFLAG����ʾ�ǿɸı�־
        if (strcmp(szValue,"YES") == 0)
        {
            m_pCurParaItem->bIsCanChg = TRUE;
        }
        
        return TRUE;
    }
    
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
void CParaFile::GetAllSectName(LIST< STRING >& SectNameList)
{
    //����ӳ���m_mapParaSect���Ѷ���ȫ��ȡ�������ӵ�SectNameList
    SectNameList.clear();
    
    PARASECT_MAP::iterator it;
    for(it=m_mapParaSect.begin(); it != m_mapParaSect.end(); it++)
    {
        SectNameList.push_back( (*it).first );
    }   
}

/*******************************************************************
�������ƣ�GetParaListFromSectName(const char* szSectName)
�����������Ӷ�����ô˶����в�����Ϣ
���������
   ����1��const char* szSectName������
���������
�������ݣ�PARAITEM_LIST* ,����ָ�������Ĳ�������Ϣ�б�ָ��
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
PARAITEM_LIST* CParaFile::GetParaListFromSectName(const char *szSectName)
{	
    //�������Ϸ���
    if (szSectName == NULL) 
    {
        return NULL;
    }

    PARASECT_MAP::iterator it;
    //��ӳ���m_mapParaSect�в��Ҷ���ΪszSectName��ӳ�䣻
    if ( (it = m_mapParaSect.find( STRING(szSectName) )) != m_mapParaSect.end() )
    {
        return &((*it).second);
    }
	return NULL;
}

/*******************************************************************
�������ƣ�GetParaDataType(const char* szValue)
����������ͨ�����������ַ��������������
���������
   ����1��const char* szValue�����������ַ���
���������
�������ݣ�PARATYPE ,������������
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
PARATYPE CParaFile::GetParaDataType(const char* szValue) const
{
    if (strcmp(szValue,"NUM") == 0)
    {   //��������
        return PARATYPE_NUM;
    }
    else if (strcmp(szValue,"STR") == 0)
    {   //�ַ�������
        return PARATYPE_STR;
    }
    else if (strcmp(szValue,"ENUM") == 0)
    {
        //ö������
        return PARATYPE_ENUM;
    }
    else if (strcmp(szValue,"IP") == 0)
    {	//IP��ַ����
        return PARATYPE_IP;
    }
    else if (strcmp(szValue,"DATE") == 0)
    {
        //��������
        return PARATYPE_DATE;
    }
    else if (strcmp(szValue,"TIME") == 0)
    {
        //ʱ������
        return PARATYPE_TIME;
    }
    else if (strcmp(szValue,"DATETIME") == 0)
    {
        //����ʱ������
        return PARATYPE_DATETIME;
    }
    else
    {	
        //��Ч��������
        return PARATYPE_UNKNOWN;
    }
}
