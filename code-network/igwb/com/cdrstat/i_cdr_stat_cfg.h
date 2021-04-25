#ifndef __I_CDR_STAT_CFG_H__
#define __I_CDR_STAT_CFG_H__

#include "stat_cfg_info.h"

class ICDRStatCfg
{
public:
    ICDRStatCfg(){};
    virtual ~ICDRStatCfg(){};

public:
    //���ò��������г�ʼ������
    virtual int Init(UINT4 uAccessPoint) = 0;

     //�������ͳ�������Ŀ��
    virtual void GetStatItemNameList(LIST<STRING>& listStatItemName) = 0;

    //���ָ��ͳ����Ŀ����ͳ��������Ϣ
    virtual SStatItemCfg* GetStatItem(const char* const szStatItemName) = 0;

     //���ָ��ͳ����Ŀ����ͳ��������Ϣ
    virtual void GetStatItem(const char* const szStatItemName, LIST<SOneStatSubItemCfg*>& subItemList) = 0;

    //��������ͳ�������������Ϣ
    virtual int SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList) = 0;

    //��ý�����
    virtual UINT4 GetAccessPoint() = 0;

    //��ý��������
    virtual const char* GetAccessPointName() = 0;

};

#endif  //__I_CDR_STAT_CFG_H__

