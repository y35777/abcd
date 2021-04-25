#ifndef __STAT_RST_MGR_H__
#define __STAT_RST_MGR_H__

#include "../include/toolbox.h"
#include "stat_rst_encap.h"

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

class ICDRStatRst;

class CCDRStatRstMgr
{
public:
    CCDRStatRstMgr();
    virtual ~CCDRStatRstMgr();

    //���г�ʼ������
    int Init(UINT4 nApCount);

    //��������ICDRStatRst�ӿ���ָ��Ľӿ�
    void GetStatRstList(LIST<ICDRStatRst*>& listStatRst);

    //ͨ�������Ų��Ҷ�Ӧ��ICDRStatRst�ӿ���ָ��
    ICDRStatRst* GetStatRstInterface(int nAccessPoint);

    //ͨ����������Ʋ��Ҷ�Ӧ��ICDRStatRst�ӿ���ָ��
    ICDRStatRst* GetStatRstInterface(STRING sAPName);

protected:
    // ���ͳ�ƽ���ļ��Ķ�д��װ����
    ICDRStatRst* CreateCDRStatRst ( void );

    //ICDRStatRst�����ָ���б�
    LIST<ICDRStatRst*>   m_statRstList;
    BOOL                 m_bUseXml;

    CStatRstEncap        m_StatRstEncap;
};

#endif //__STAT_RST_MGR_H__
