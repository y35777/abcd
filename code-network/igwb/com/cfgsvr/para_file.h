/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : para_file.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CParaFile class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __PARA_FILE_H__
#define __PARA_FILE_H__

#include "cfg_def.h"

/*================================================================================
�����ļ���ʽ

  �����ļ���ʽ����allpara�ļ��ĸ�ʽ��ÿ������������Զ�Ҫ�оٳ����������ʽ���£�
  [����������] 
  SectList= ����1^����2^����3�������Σ�
  SectRela= &����(�����)
  Desc=����
  Type=����
  Default=ȱʡֵ
  MustFlag=�����־ 
  UpdateFlag=�ɸı�־
  
================================================================================*/

//����������ö�ٽṹ
enum  PARATYPE
{ 
    PARATYPE_UNKNOWN	= -1,      //��Ч��������
    PARATYPE_NUM		= 0,       //��ֵ����
    PARATYPE_STR		= 1,       //�ַ������� 
    PARATYPE_ENUM		= 2,       //ö������
    PARATYPE_IP			= 3,       //IP��ַ����
    PARATYPE_DATE		= 4,       //��������
    PARATYPE_TIME		= 5,       //ʱ������
    PARATYPE_DATETIME	= 6        //����ʱ������
};

//��������Ϣ�ṹ
struct  SParaItem
{
    char      szItemName[MAX_ITEMNAME_LEN];      //��������
    char      szItemDesc[MAX_ITEMDESC_LEN];      //���������� 
    PARATYPE  nItemType;                         //����������
    char      szDefaultValue[MAX_ITEMVALUE_LEN]; //��������ENUMʱ���������о٣�ȱʡֵ��ö��1��ö��2��
    char      szParaRelaSect[MAX_SECTNAME_LEN];  //��������ض���
    BOOL      bIsMustCfg;                        //�Ƿ����������
    BOOL      bIsCanChg;                         //�ܷ����޸Ĵ�������

	//���캯��
	SParaItem()
	{
		szItemName[0] = '\0';
		szItemDesc[0] = '\0';
		nItemType = PARATYPE_UNKNOWN;       //ָ���Ƕ������ͣ������жϲ�����Ϣ�ļ��Ƿ�ָ����������������
		szDefaultValue[0] = '\0';
		szParaRelaSect[0] = '\0';
		bIsMustCfg = FALSE;
		bIsCanChg = FALSE;
	}
};

//�����ɶ����������ɵģ���ҪΪһ�������б���һ������
typedef  LIST< SParaItem* >  PARAITEM_LIST;

//����ӳ�䵽�˶εĲ������б���Ҫ����һ��ӳ������
typedef  MAP< STRING, PARAITEM_LIST >  PARASECT_MAP;

/*===============================================================================
������������Ϣ�ļ���
ÿ��������Ϣ�ļ�ʹ��һ��CParaFile����г���
===============================================================================*/
class CParaFile  
{
public:
    CParaFile();
    virtual ~CParaFile();
    
    //��ʼ�����������ļ��������ļ����ݣ���ʼ��m_mapParaSect
    BOOL Init(const char *szFileName);
    //������ж���
    void GetAllSectName(LIST< STRING >& SectNameList);
    //ͨ��������ô˶ε����в������б�
    PARAITEM_LIST* GetParaListFromSectName(const char *szSectName);

protected:
    char                m_szFilePath[MAX_PATH];             //�����ļ�·��
    char                m_szCurSectName[MAX_SECTNAME_LEN];  //��ǰ�ε�����
    SParaItem*          m_pCurParaItem;                     //��ǰ������
    PARASECT_MAP        m_mapParaSect;                      //�����νṹ�б�
    LIST< STRING >      m_SectNameList;                     //�����б�
    
    //���������ļ�ÿһ������
    BOOL ParseALine(const char* szLine);
    //����������������
    BOOL ParseParaLine(const char* szLine);

//��������
private:
	//��һ��������Ϣ���뵽�����νṹ�б�
	BOOL AddParaItemToParaSectMap();
    //ͨ�����������ַ��������������
    PARATYPE GetParaDataType(const char *szValue) const;
};

#endif //__PARA_FILE_H__
