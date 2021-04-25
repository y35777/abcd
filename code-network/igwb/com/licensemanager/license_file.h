#ifndef _LICENSE_FILE_H_
#define _LICENSE_FILE_H_

#include <time.h>
#include "../include/toolbox.h"

enum LICENSE_ID
{
    INVALID_LICENSE_ID      = 0,        //�Ƿ���License ID
    LICENSE_ID_NETBACKUP    = 1,        //���籸�ݹ���
    LICENSE_ID_CDRSTAT      = 2,        //����ͳ�ƹ���
    LICENSE_ID_R98          = 3,        //2.5G����
    LICENSE_ID_R99          = 4,        //3GPP R99����
    LICENSE_ID_R4           = 5,        //3GPP R4����
    LICENSE_ID_CC08         = 6,        //����128ģ
    LICENSE_ID_MSC          = 7,        //MSC����
    LICENSE_ID_STP          = 8,        //STP����
    LICENSE_ID_S3K          = 9,        //SoftX3000����
    LICENSE_ID_WLAN         = 10,       //WLAN����
    LICENSE_ID_CO           = 11        //�ɼ���
};

//ÿ����Ȩ��License���԰���License ID����Ȩ�ĸ���
//�����ݽṹ������Ҫ������"../include/base_type.h"�ļ���
struct SLicenseElement
{
    LICENSE_ID      nLicenseID;     //��ӦLICENSE_ID�е�ö��ֵ
    int             nLicense;       //License��Ȩ����
    int             nAPID;          //License��Ȩ�Ľ����ID
};

#define ACCESS_POINT_ANY    0       //��������
#define PERMANENT_LICENSE   -1      //������License

class CLicenseFile
{
public:
    CLicenseFile(const char* szLicenseDir);
    virtual ~CLicenseFile();

    int Init(void);
    int GetLicense(LICENSE_ID nLicenseID, int nAPID);
    int Refresh(void);
    BOOL IsPermanent(void);
    BOOL IsTemporary(void);

    BOOL IsValid(void);
    BOOL IsExpired(void);
    time_t GetExpireDate(void);
    BOOL DecodeLicenseKey(char* pszCoded, int nCoded,
                          char* pszDecoded, int& nDecoded);

    //add by zkq 2004-06-19
    void GetLicenseToClient(LIST<STRING>& ContentList); //���license�ļ�����
    //end
protected:
    void GetHostID(void);
    int ParseDate(char* pszLine);
    int ParseFeature(char* pszLine);
    int CheckHostID(char* szHostID);
    int AddAFeature(STRING& sFeature, int& nAPID);

protected:
    char m_szLicenseDir[MAX_PATH];                  //License�ļ����Ŀ¼
    typedef LIST<SLicenseElement>   FEATURE_LIST;
    FEATURE_LIST m_FeatureList;                     //License�����б�
    time_t  m_tExpireDate;                          //License��Ч����
    BOOL    m_bValidLicense;                        //License�Ϸ��Ա�־
    typedef LIST<STRING> HOSTID_LIST;
    HOSTID_LIST m_HostID;                           //host id/MAC��ַ�б�

public:
	VECTOR<int>  m_LicenseVector;	//������Ӧ��License����
};

#endif //_LICENSE_FILE_H_
