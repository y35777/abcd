/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:wrk_module.h
    描述信息:

    包含CWorkModule类的定义,所有业务模块都需要包含该文件.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-16,创建.
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
	int	 m_nLicenseCount;		//License授权个数
	int  m_nTimerIDReqLicense;	//License请求定时器
};

#endif
