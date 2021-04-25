#ifndef _LICENSE_MANAGER_H_
#define _LICENSE_MANAGER_H_

#include "../frame/wrk_module.h"
#include "license_file.h"

struct SAPRelease
{
	int    nAPID;			//�����ID
	int    nAPType;			//���������
	char   szRelease[64];	//�����������ΪAP_PSʱ��LicenseManager����Ҫ�ж�
							//����ģ����õ�Э������(R98��R99��R4��)
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
	long			m_nTimerIDCheckLic;		//��ʱ���License
	long			m_nTimerInterval;		//License��ʱ�����
	CLicenseFile*	m_pLicenseFile;	
	char			m_szLicenseDir[MAX_PATH];
	time_t			m_tStartTime;
	time_t			m_tLastRefreshTime;

	typedef MAP<int, SAPRelease, std::less<int> > MAP_AP_RELEASE;	
	MAP_AP_RELEASE		m_mapAPRelease;

};

#endif
