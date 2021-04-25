/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:wrk_manager.h
    ������Ϣ:
    
    ����CModuleManager��Ķ���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/

#ifndef _WORK_MODULE_MANAGER_H
#define _WORK_MODULE_MANAGER_H

#include "msg_proc.h"

class CWorkModule;

class CModuleManager:public CMsgProcessor
{
protected:
    typedef std::list<CWorkModule*> LIST_WORK_MODULE;

public:
    CModuleManager();
    virtual ~CModuleManager();

protected:
    LIST_WORK_MODULE m_WorkModules;     //ҵ��ģ��ָ������
    long m_lTimerIDBase;                //��׼��ʱ��ID
    long m_lTimerCounterPerf;           //��������ͳ�ƵĶ�ʱ��������
    EWKMGR_STATUS m_nStatus;
    UINT4 m_nModuleReady;               //��ǰ�ѳ�ʼ���ɹ���ģ����

    /* ��������ҵ��ģ����� */
    virtual int CreateWorkModules(void) = 0;

    virtual int InitInstance(void);
    virtual void ExitInstance(void);

    virtual void ParseMsg(MSG_INTRA* pMsgParsed);

    /* �������ҵ��ģ�� */
    virtual BOOL ActiveWorkModules(void);

    /* ��ֹ����ҵ��ģ�� */
    void DeActiveWorkModules(void);

    virtual void OnTimer(const long nTimerID);
    virtual MODULE_TYPE GetModuleType(){return MT_MANAGER;};
};

#endif
