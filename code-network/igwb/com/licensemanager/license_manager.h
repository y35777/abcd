#ifndef _LICENSE_MANAGER_H_
#define _LICENSE_MANAGER_H_

#include "../frame/wrk_module.h"
#include "license_file.h"

struct SAPRelease
{
	int    nAPID;			//接入点ID
	int    nAPType;			//接入点类型
	char   szRelease[64];	//当接入点类型为AP_PS时，LicenseManager还需要判断
							//网络模块采用的协议类型(R98、R99、R4等)
};

class CLicenseManager : public CWorkModule
{
public:
    CLicenseManager(void);
    virtual ~CLicenseManager(void);

protected:
    int InitInstance(void);
	void ExitInstance(void);
	MODULE_TYPE GetModuleType(void);
	void OnTimer(const long nTimerID);
	void ParseMsg(MSG_INTRA* pMsg);

private:
	LICENSE_ID GetLicenseID(PROCESS_TYPE apt, MODULE_TYPE mt);
	void RefreshLicense(MSG_INTRA* pMsg);
	void ResponseLicense(MSG_INTRA* pMsg);
	//add by zkq 2004-06-19
	void SendLicenseToClient(MSG_INTRA* pMsg);
	//end 
    
private:
	long			m_nTimerIDCheckLic;		//定时检查License
	long			m_nTimerInterval;		//License定时检查间隔
	CLicenseFile*	m_pLicenseFile;	
	char			m_szLicenseDir[MAX_PATH];
	time_t			m_tStartTime;
	time_t			m_tLastRefreshTime;

	typedef MAP<int, SAPRelease, std::less<int> > MAP_AP_RELEASE;	
	MAP_AP_RELEASE		m_mapAPRelease;

};

#endif
