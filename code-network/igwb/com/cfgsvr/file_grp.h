/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : file_grp.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CCfgFileGrp  class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __FILE_GROUP_H__
#define __FILE_GROUP_H__

#include "cfg_def.h"

#include "cfg_file.h"

/*===============================================================================
�����������ļ�����
Զ�����ù���֧�ֶ����ļ������ã����ж�ÿ�ֶ���һ���ļ���
(1) ��������:     .bas
(2) �ϴ�����: .lst
(3) ��ǰ����:     .ini
===============================================================================*/
class CCfgFileGrp  
{
public:
    CCfgFileGrp();
    virtual ~CCfgFileGrp();
    
    //��ʼ�������ļ���
    BOOL Init(const char* szFilePath);
    //ͨ���ļ��Ż�ô��ļ������ж���
    void GetAllSectName(LIST< STRING >& SectNameList, int nFileNo);
    //���ݶ�����ô˶ε��������б�
    CFGITEM_LIST* GetCfgListFromSectName(const char* szSectName, int nFileNo);
	
    //����ĳ�����ļ�������������
    BOOL SetAllCfgItem(const char* szAllCfgItem, int nFileNo);
    //ȡ����������
    void GetAllCfgItem(STRING &strAllCfgItem , int nFileNo);
    //���µ�ǰ�����ļ�
	BOOL UpdateCurrentCfg(const char* szFilePath);
    //�����ϴ������ļ�
	BOOL UpdateLastCfg(const char* szFilePath);
    
protected:
    CCfgFile*  m_pFacFile;     //�����������ļ�
    CCfgFile*  m_pSucFile;     //�ϴε������ļ�
    CCfgFile*  m_pCurFile;     //��ǰ�������ļ�
};

#endif //__FILE_GROUP_H__
