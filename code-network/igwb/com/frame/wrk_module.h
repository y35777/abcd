/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:wrk_module.h
    ������Ϣ:

    ����CWorkModule��Ķ���,����ҵ��ģ�鶼��Ҫ�������ļ�.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/

#ifndef _WORK_MODULE_H
#define _WORK_MODULE_H

#include "msg_proc.h"

class CWorkModule:public CMsgProcessor
{
public:
    CWorkModule();
    virtual ~CWorkModule();
	
	int GetLicenseCount(void);

protected:
    virtual void ExitInstance(void);
    virtual int InitInstance(void);
    virtual void ParseMsg(MSG_INTRA* pMsg);

	virtual void OnTimer(const long nTimerID);

	void RequestLicense();

protected:
	int	 m_nLicenseCount;		//License��Ȩ����
	int  m_nTimerIDReqLicense;	//License����ʱ��
};

#endif
