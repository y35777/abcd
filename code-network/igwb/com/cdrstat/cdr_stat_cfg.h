#ifndef __CDR_STAT_CFG_H__
#define __CDR_STAT_CFG_H__

#include "stat_cfg_info.h"
#include "i_cdr_stat_cfg.h"


class CCDRStatCfg: public ICDRStatCfg
{
public:
    CCDRStatCfg();
    virtual ~CCDRStatCfg();

public:
     //���ò��������г�ʼ������
    virtual int Init(UINT4 uAccessPoint);

    //�������ͳ�������Ŀ��
    virtual void GetStatItemNameList(LIST<STRING>& listStatItemName);

    //���ָ��ͳ����Ŀ����ͳ��������Ϣ
    virtual SStatItemCfg* GetStatItem(const char* const szStatItemName);

    //���ָ��ͳ����Ŀ����ͳ��������Ϣ
    virtual void GetStatItem(const char* const szStatItemName,
                             LIST<SOneStatSubItemCfg*>& subItemList);

    //��������ͳ�������������Ϣ
    virtual int SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList);

    //��ý�����
    virtual UINT4 GetAccessPoint();

    //��ý��������
    virtual const char* GetAccessPointName();

protected:
    //������
    UINT4    m_uAccessPoint;
    //���������
    STRING   m_sAPName;
    //ͳ�������ļ�����·����
    char     m_szFileName[MAX_PATH];
    //ͳ��������Ϣ������ָ��
    BYTE*    m_pMemData;
    //ͳ��������Ϣ�ļ�����
    UINT4    m_nMemDataLen;
};
#endif  //__CDR_STAT_FILE_H__

