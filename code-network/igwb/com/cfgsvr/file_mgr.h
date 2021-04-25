/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : file_mgr.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CCfgFileMgr  class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __FILE_MGR_H__
#define __FILE_MGR_H__

#include "cfg_def.h"

#include "cfg_file.h"

class CCfgFileGrp;
/*===============================================================================
�����������ļ�������
���ڶ�����������ļ�������Ӧ��Ӧ��������ļ��顣��CCfgFileMgr
���ڹ�����Щ�����ļ��顣
===============================================================================*/
class CCfgFileMgr  
{
public:
    CCfgFileMgr();
    virtual ~CCfgFileMgr();
    
    //����һ��������
    BOOL AddCfgFileGrp(int nGrpNo, const char* szFilePath);
    //������ź��ļ��Ż��ĳ�����ļ������ж���
    void GetAllSectName(LIST< STRING >& SectNameList, int nGrpNo, int nFileNo);
    //������š��ļ��źͶ������ĳ�����ļ�ĳ�ε�����������
    CFGITEM_LIST * GetCfgListFromSectName(const char* szSectName,
                                       int nGrpNo, int nFileNo);
    //������ź��ļ�������ĳ�ļ�������������
    BOOL SetAllCfgItem(const char* szAllCfgItem, int nGrpNo, int nFileNo);
    
    //����ļ�����
    void GetCfgFileClass(LIST< STRING >& GrpNameList);
    //ȡ��������������Ϣ
    void GetAllCfgItem(STRING &strAllCfgItem, int nGrpNo, int nFileNo);
    //����ָ����ĵ�ǰ�����ļ�
	BOOL UpdateCurrentCfg(int nGrpNo, const char* szFilePath);
    //����ָ������ϴ������ļ�
	BOOL UpdateLastCfg(int nGrpNo, const char* szFilePath);
    
protected:
    MAP< int, CCfgFileGrp* >  m_mapCfgFileGrp;    //�����ļ���ź������ļ����ӳ���
    MAP< int, STRING >        m_mapCfgFileClass;  //�����ļ���ź������ļ�������ӳ���
};


#endif //__FILE_MGR_H__
