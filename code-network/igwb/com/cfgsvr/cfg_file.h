/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : cfg_file.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CCfgFile.  class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __CFG_FILE_H__
#define __CFG_FILE_H__


#include "cfg_def.h"

/*================================================================================
�����ļ���ʽ

  �����ļ���ʽ�ֶ��к����У�
  [����]--------------------------------------->���и�ʽ
  ��������=������ֵ--------------------->���и�ʽ
  
================================================================================*/

//�����ļ���������ṹ
struct SCfgItem
{
    char szItemName[MAX_ITEMNAME_LEN];       //����������
    char szItemValue[MAX_ITEMVALUE_LEN];     //������ֵ
};

//�������б����ڴ洢���ö���Ϣ��ÿ�����ö��ɶ����������ɣ�
typedef LIST< SCfgItem * > CFGITEM_LIST;

//���ö�ӳ������ڴ洢�����ļ���Ϣ��ÿ�������ļ��ɶ�����ö���ɣ�
typedef MAP< STRING, CFGITEM_LIST > CFGSECT_MAP;

/*===============================================================================
�����������ļ���
ÿ�������ļ�ʹ��һ��CCfgFile����г���
===============================================================================*/
class CCfgFile  
{
public:
    CCfgFile();
    virtual ~CCfgFile();
    
    //��ʼ�����������ļ��������ļ����ݣ���ʼ��m_mapCfgSectt��
    BOOL Init(const char* szFileName);
    //������ж���
    void GetAllSectName(LIST< STRING >&  SectNameList);
    //ͨ��������ô˶ε����в������б�
    CFGITEM_LIST* GetCfgListFromSectName(const char* szSectName);
    //����������������Ϣ
    BOOL SetAllCfgItem(const char* szAllCfgItem);
    
    //ȡ��������������Ϣ
    void GetAllCfgItem(STRING &strAllCfgItem); 

    //�����������д�������ļ���
    BOOL WriteAllCfgItem();    
protected:
    char          m_szFilePath[MAX_PATH];               //�����ļ���·��
    char          m_szCurSectName[MAX_SECTNAME_LEN];    //��ǰ�ε�����
    CFGITEM_LIST* m_pCurItemList;                       //��ǰ�ε������б�
    CFGSECT_MAP   m_mapCfgSect;                         //���öνṹ�б�
    
    //����ÿһ������
    BOOL ParseALine(const char* szLine);
    //����������������
    BOOL ParseCfgLine(const char* szLine);
    //����һ������������
    BOOL ParseCfgItem(const char* szCfgItem);
private:
    //��һ���öε�����������Ϣ�б��������ӳ���
    BOOL AddItemListToCfgSectMap();
    //�������ӳ���
    void ClearCfgSectMap();
};


#endif //__CFG_FILE_H__
