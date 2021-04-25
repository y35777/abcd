#ifndef _LICENSE_FILE_H_
#define _LICENSE_FILE_H_

#include <time.h>
#include "../include/toolbox.h"

enum LICENSE_ID
{
    INVALID_LICENSE_ID      = 0,        //非法的License ID
    LICENSE_ID_NETBACKUP    = 1,        //网络备份功能
    LICENSE_ID_CDRSTAT      = 2,        //话单统计功能
    LICENSE_ID_R98          = 3,        //2.5G特性
    LICENSE_ID_R99          = 4,        //3GPP R99特性
    LICENSE_ID_R4           = 5,        //3GPP R4特性
    LICENSE_ID_CC08         = 6,        //固网128模
    LICENSE_ID_MSC          = 7,        //MSC特性
    LICENSE_ID_STP          = 8,        //STP特性
    LICENSE_ID_S3K          = 9,        //SoftX3000特性
    LICENSE_ID_WLAN         = 10,       //WLAN特性
    LICENSE_ID_CO           = 11        //采集点
};

//每个授权的License特性包含License ID和授权的个数
//本数据结构最终需要定义在"../include/base_type.h"文件中
struct SLicenseElement
{
    LICENSE_ID      nLicenseID;     //对应LICENSE_ID中的枚举值
    int             nLicense;       //License授权个数
    int             nAPID;          //License授权的接入点ID
};

#define ACCESS_POINT_ANY    0       //任意接入点
#define PERMANENT_LICENSE   -1      //永久性License

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
    void GetLicenseToClient(LIST<STRING>& ContentList); //获得license文件内容
    //end
protected:
    void GetHostID(void);
    int ParseDate(char* pszLine);
    int ParseFeature(char* pszLine);
    int CheckHostID(char* szHostID);
    int AddAFeature(STRING& sFeature, int& nAPID);

protected:
    char m_szLicenseDir[MAX_PATH];                  //License文件存放目录
    typedef LIST<SLicenseElement>   FEATURE_LIST;
    FEATURE_LIST m_FeatureList;                     //License特性列表
    time_t  m_tExpireDate;                          //License有效期限
    BOOL    m_bValidLicense;                        //License合法性标志
    typedef LIST<STRING> HOSTID_LIST;
    HOSTID_LIST m_HostID;                           //host id/MAC地址列表

public:
	VECTOR<int>  m_LicenseVector;	//接入点对应的License类型
};

#endif //_LICENSE_FILE_H_
