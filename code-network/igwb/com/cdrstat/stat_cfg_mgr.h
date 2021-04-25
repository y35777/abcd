#ifndef __STAT_CFG_MGR_H__
#define __STAT_CFG_MGR_H__

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

#include "../include/toolbox.h"
#include "stat_cfg_encap.h"

class ICDRStatCfg;

class CCDRStatCfgMgr
{
public:
    CCDRStatCfgMgr();
    virtual ~CCDRStatCfgMgr();

    //���г�ʼ������
    int Init(UINT4 nApCount);

    //��������ICDRStatCfg�ӿ���ָ��Ľӿ�
    void GetStatCfgList(LIST<ICDRStatCfg*>& listStatCfg);

    //ͨ�������Ų��Ҷ�Ӧ��ICDRStatCfg�ӿ���ָ��
    ICDRStatCfg* GetStatCfgInterface(int nAccessPoint);

    //ͨ����������Ʋ��Ҷ�Ӧ��ICDRStatCfg�ӿ���ָ��
    ICDRStatCfg* GetStatCfgInterface(STRING sApName);

protected:
    // ���ͳ�������ļ��Ķ�д��װ����
    ICDRStatCfg* CreateCDRStatCfg ( void );

    //ICDRStatCfg�����ָ���б�
    LIST<ICDRStatCfg*>   m_statCfgList;
    BOOL                 m_bUseXml;

    CStatCfgEncap        m_StatCfgEncap;
};
#endif  //__STAT_CFG_MGR_H__
