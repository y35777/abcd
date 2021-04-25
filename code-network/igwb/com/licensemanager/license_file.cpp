#include "license_file.h"
#include "ace/OS.h"
#include "../utility/encrypt.h"

#ifdef _PLATFORM_WIN32
    #include <windows.h>
    #include <Iptypes.h>
    #include <Iphlpapi.h>
    #pragma comment(lib, "Iphlpapi.lib")
#endif

CLicenseFile::CLicenseFile(const char* pszLicenseDir)
{
    m_szLicenseDir[0] = '\0';

    //pszLicenseDir是License文件存放目录
    if (pszLicenseDir)
    {
        strncpy(m_szLicenseDir, pszLicenseDir, MAX_PATH);
        m_szLicenseDir[MAX_PATH - 1] = '\0';
    }

    int nLen = strlen(m_szLicenseDir);

    if (nLen)
    {
        if ((m_szLicenseDir[nLen - 1] != '/') && (m_szLicenseDir[nLen - 1] != '\\'))
        {
            strcat(m_szLicenseDir, "/");
        }
    }

    m_bValidLicense = FALSE;
    m_tExpireDate   = 0;
    m_LicenseVector.clear();
}

CLicenseFile::~CLicenseFile()
{
}


/********************************************************************
 函数原型   public:  BOOL GetLicense(const LICENSE_ID nLicenseID,
                                     int nAPID, int& nLicense);
 功能描述   判断执行的业务属性(nLicenseID)是否授权。
 输入参数
            nLicenseID : 业务属性
 输出参数
            nAPID   : 实际的接入点号
 返回值     是否授权。
 抛出异常   无
********************************************************************/
int CLicenseFile::GetLicense(LICENSE_ID nLicenseID, int nAPID) 
{
    int nLicenseCount = 0;
    LIST<SLicenseElement>::iterator it = m_FeatureList.begin();

	int nPointNum = 1; //表示相同接入点类型在LICENSE INFO = ...中出现的序号
	int nFactApNum = 0;//接入点号nApID在实际的接入点顺序集合中同种类型的序号

	if (nAPID > m_LicenseVector.size())
	{
		return 0;
	}
	
	for (int i = 0; i < nAPID; i++) //寻找他之前的接入点，并统计同种类型的个数
	{
		if (m_LicenseVector[i] == nLicenseID)
		{
			nFactApNum++;
		}
	}

    while (it != m_FeatureList.end())
    {   
        //License ID都要匹配
		if ((*it).nLicenseID == nLicenseID)
		{
			//对备份模块或者是统计模块
			if ((nLicenseID == LICENSE_ID_CDRSTAT) || (nLicenseID == LICENSE_ID_NETBACKUP)
				&& ((*it).nAPID == nAPID))
			{
				nLicenseCount = (*it).nLicense;
				break;
			}
			else
			{
				// 普通的接入点或者是采集接入点
				if (nPointNum == nFactApNum)
				{
					nLicenseCount = (*it).nLicense;
					break;
				}
				else
				{
					nPointNum++;
				} 
			}
		}
        it++;
    }

    return nLicenseCount;
}

/********************************************************************
 函数原型    public:  BOOL IsValid(void);
 功能描述    License是否合法。
 输入参数    无。
 输出参数    无。
 返回值      License是否合法。
 抛出异常    无
********************************************************************/
BOOL CLicenseFile::IsValid(void)
{
    return m_bValidLicense;
}

/********************************************************************
 函数原型    public:  BOOL IsPermanent(void);
 功能描述    是否是永久性License。
 输入参数    无。
 输出参数    无。
 返回值      是否是永久性License。
 抛出异常    无
********************************************************************/
BOOL CLicenseFile::IsPermanent(void)
{
    return ((m_bValidLicense == TRUE) && (m_tExpireDate == PERMANENT_LICENSE));
}

/********************************************************************
 函数原型    public:  BOOL IsTemporary(void);
 功能描述    是否是临时License。
 输入参数    无。
 输出参数    无。
 返回值      是否是临时License。
 抛出异常    无
********************************************************************/
BOOL CLicenseFile::IsTemporary(void)
{
    return ((m_bValidLicense == TRUE) && (m_tExpireDate != PERMANENT_LICENSE));
}

/********************************************************************
 函数原型    public:  int Init(void);
 功能描述    License信息初始化。
 输入参数    无
 输出参数    无
 返回值      初始化是否成功。
 抛出异常    无
********************************************************************/
int CLicenseFile::Init(void)
{
    char szLicenseFile[MAX_PATH] = "";      //License文件全路径名
    FILE* fp = NULL;

    m_bValidLicense = FALSE;            //初始化License为非法
    m_tExpireDate   = 0;
    m_FeatureList.clear();

    //获取License文件名全路径，license文件名固定为license.dat
    sprintf(szLicenseFile, "%slicense.dat", m_szLicenseDir);

    //以只读、文本方式打开License文件
    if ((fp = fopen(szLicenseFile, "rt")) == NULL)
    {
        return -1;
    }

    char szTemp1[128] = "";
    char szTemp2[128] = "";
    char szTemp3[128] = "";

    char szLicenseDate[1024] = "";
    char szFetureList[1024]  = "";
    char szHostID[1024]      = "";

    //读第一行：License有限期限，行格式为“公司名   版本    日期”
    fscanf(fp, "%s %s %s", szTemp1, szTemp2, szLicenseDate);
    if (0 != ParseDate(szLicenseDate))
    {
        fclose(fp);
        return -1;
    }

    //读HOST ID，行格式为“HOST ID = hostid”
    fscanf(fp, "%s %s %s %s", szTemp1, szTemp2, szTemp3, szHostID);
    if (0 != CheckHostID(szHostID))
    {
        fclose(fp);
        return -1;
    }

    //读业务特性，行格式为“LICENSE INFO = Feature1-N1:Feature2-N2:...:Featuren-Nn”
    fscanf(fp, "%s %s %s %s", szTemp1, szTemp2, szTemp3, szFetureList);
    if (0 != ParseFeature(szFetureList))
    {
        fclose(fp);
        return -1;
    }

    char szCodedKey[3072]    = "";
    char szDecodedKey[3072]  = "";
    int nLen = 1024;

    //License加密信息
    fscanf(fp, "%s %s %s %s", szTemp1, szTemp2, szTemp3, szCodedKey);
    StringLRTrim(szCodedKey);
    char szComposed[3072]    = "";

    //将License日期、host id和License授权信息三个字符串串接起来
    sprintf(szComposed, "%s%s%s", szLicenseDate, szHostID, szFetureList);

    DecodeLicenseKey(szCodedKey, strlen(szCodedKey), szDecodedKey, nLen);

    //License加密信息校验
    if (strcmp(szComposed, szDecodedKey) != 0)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    m_bValidLicense = TRUE;

    return 0;
}

/********************************************************************
 函数原型    private: int ParseDate(const char* pszLine);
 功能描述    此函数为功能函数，从License文件第一行中获得的License
             有效期限。
 输入参数  无。

 输出参数
            pszLine  : License日期行
 返回值     是否成功。
 抛出异常   无
********************************************************************/
int CLicenseFile::ParseDate(char* pszLine)
{
    if (!pszLine)
    {
        return -1;
    }

    char szDate[128] = "";
    strncpy(szDate, pszLine, 128);
    StringLRTrim(szDate);

    //是否是永久性License
    if (strcmp(szDate, "PERMANENT") == 0)
    {
        m_tExpireDate = PERMANENT_LICENSE;
        return 0;
    }

    //如果不是标准的DD-MON-YYYY形式的日期，则认为日期非法
    if (strlen(szDate) != 11)
    {
        return -1;
    }

    char szDay[3]   = "";
    char szMon[4]   = "";
    char szYear[5]  = "";

    //取出两位的日
    strncpy(szDay, &szDate[0], 2);
    szDay[2] = '\0';

    //取出三位的月
    strncpy(szMon, &szDate[3], 3);
    szMon[3] = '\0';

    //取出四位的年
    strncpy(szYear, &szDate[7], 4);
    szYear[4] = '\0';

    if (!IsDigitValue(szDay) || !IsDigitValue(szYear))
    {
        return -1;
    }

    struct tm tExpireDate;

    //初始化tExpireDate中的所有域
    memset(&tExpireDate, 0, sizeof(struct tm));

    tExpireDate.tm_year = atoi(szYear) - 1900;
    tExpireDate.tm_mday = atoi(szDay);

    if (strcmp(szMon, "JAN") == 0)
    {
        tExpireDate.tm_mon = 0;
    }
    else if (strcmp(szMon, "FEB") == 0)
    {
        tExpireDate.tm_mon = 1;
    }
    else if (strcmp(szMon, "MAR") == 0)
    {
        tExpireDate.tm_mon = 2;
    }
    else if (strcmp(szMon, "APR") == 0)
    {
        tExpireDate.tm_mon = 3;
    }
    else if (strcmp(szMon, "MAY") == 0)
    {
        tExpireDate.tm_mon = 4;
    }
    else if (strcmp(szMon, "JUN") == 0)
    {
        tExpireDate.tm_mon = 5;
    }
    else if (strcmp(szMon, "JUL") == 0)
    {
        tExpireDate.tm_mon = 6;
    }
    else if (strcmp(szMon, "AUG") == 0)
    {
        tExpireDate.tm_mon = 7;
    }
    else if (strcmp(szMon, "SEP") == 0)
    {
        tExpireDate.tm_mon = 8;
    }
    else if (strcmp(szMon, "OCT") == 0)
    {
        tExpireDate.tm_mon = 9;
    }
    else if (strcmp(szMon, "NOV") == 0)
    {
        tExpireDate.tm_mon = 10;
    }
    else if (strcmp(szMon, "DEC") == 0)
    {
        tExpireDate.tm_mon = 11;
    }
    else
    {
        return -1;
    }

    m_tExpireDate = mktime(&tExpireDate);
    if (m_tExpireDate == -1)
    {
        return -1;
    }

    return 0;
}

/********************************************************************
 函数原型    private: int ParseFeature(const char* pszLine);
 功能描述    此函数为功能函数，分析License授权列表。
 输入参数  无。

 输出参数
            pszLine  : License授权信息行
 返回值     是否成功。
 抛出异常   无
********************************************************************/
int CLicenseFile::ParseFeature(char* pszLine)
{
    if (!pszLine)
    {
        return -1;
    }

    STRING sLine = pszLine;
    STRING sFeature;
    int nAPID = 1;

    int nPos;
    while ((nPos = sLine.find(":")) != -1)
    {
        sFeature = sLine.substr(0, nPos);
        sLine    = sLine.substr(nPos+1, sLine.length());

        if (AddAFeature(sFeature, nAPID) == -1)
        {
            return -1;
        }
    }

    if (AddAFeature(sLine, nAPID) == -1)
    {
        return -1;
    }

    return 0;
}

int CLicenseFile::AddAFeature(STRING& sFeature, int& nAPID)
{
    SLicenseElement le;

    //话单统计
    if (strcmp(sFeature.c_str(), "CDRSTAT") == 0)
    {
        le.nLicenseID = LICENSE_ID_CDRSTAT;
        le.nLicense   = 1;
        le.nAPID      = ACCESS_POINT_ANY;
        m_FeatureList.push_back(le);
    }
    //网络备份
    else if (strcmp(sFeature.c_str(), "NETBACKUP") == 0)
    {
        le.nLicenseID = LICENSE_ID_NETBACKUP;
        le.nLicense   = 1;
        le.nAPID      = ACCESS_POINT_ANY;
        m_FeatureList.push_back(le);
    }
    else
    {
        STRING sNum;
        int nNum;
        int nDash;

        if ((nDash = sFeature.find("-")) == -1)
        {
            return -1;
        }

        //License授权个数
        sNum = sFeature.substr(nDash+1, sFeature.length());
        if (!IsDigitValue(sNum.c_str()))
        {
            return -1;
        }
        else
        {
            nNum = atoi(sNum.c_str());
        }

        if (sFeature.find("R98-") != -1)
        {
            le.nLicenseID = LICENSE_ID_R98;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else if (sFeature.find("R99-") != -1)
        {
            le.nLicenseID = LICENSE_ID_R99;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else if (sFeature.find("R4-") != -1)
        {
            le.nLicenseID = LICENSE_ID_R4;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else if (sFeature.find("MSC-") != -1)
        {
            le.nLicenseID = LICENSE_ID_MSC;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else if (sFeature.find("CC08-") != -1)
        {
            le.nLicenseID = LICENSE_ID_CC08;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else if (sFeature.find("STP-") != -1)
        {
            le.nLicenseID = LICENSE_ID_STP;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else if (sFeature.find("S3K-") != -1)
        {
            le.nLicenseID = LICENSE_ID_S3K;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else if (sFeature.find("WLAN-") != -1)
        {
            le.nLicenseID = LICENSE_ID_WLAN;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else if (sFeature.find("CO-") != -1)
        {
            le.nLicenseID = LICENSE_ID_CO;
            le.nLicense   = nNum;
            le.nAPID      = nAPID;
            m_FeatureList.push_back(le);

            nAPID++;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

/********************************************************************
 函数原型    private: void GetHostID(void);
 功能描述    此函数为功能函数，读本机的host id/MAC地址，对于UNIX平台，
             使用host id，对于WINDOWS平台，使用任意网卡的MAC地址。
 输入参数  无。
 输出参数  无。
 返回值    无。
 抛出异常  无。
********************************************************************/
void CLicenseFile::GetHostID(void)
{
    m_HostID.clear();

    //在WIN32平台下，用任意网卡的MAC地址作为host id
#ifdef _PLATFORM_WIN32
    UINT4 uSize  = 0;
    PIP_ADAPTER_INFO pIpAdaptersInfo = NULL;

    UINT4 uRet = GetAdaptersInfo(pIpAdaptersInfo, &uSize);
    if ((ERROR_BUFFER_OVERFLOW == uRet) && (uSize > 0))
    {
        pIpAdaptersInfo = (IP_ADAPTER_INFO*) new BYTE[uSize];
        uRet = GetAdaptersInfo(pIpAdaptersInfo, &uSize);
    }

    if ((ERROR_SUCCESS == uRet) && (NULL != pIpAdaptersInfo))
    {
        PIP_ADAPTER_INFO pIpAdaptersInfotmp = pIpAdaptersInfo;

        while ( NULL != pIpAdaptersInfotmp )
        {
            if( 0 < pIpAdaptersInfotmp->AddressLength)
            {
                char szMacAddr[20] = "";
                for (int i = 0 ; i < pIpAdaptersInfotmp->AddressLength ; i++)
                {
                    int nValue = pIpAdaptersInfotmp->Address[i];

                    char szMacTmp[3];
                    sprintf(szMacTmp,"%02hX", nValue);
                    szMacTmp[2]= '\0';

                    strcat(szMacAddr, szMacTmp);
                }

                m_HostID.push_back(szMacAddr);
            }

            pIpAdaptersInfotmp = pIpAdaptersInfotmp->Next;
        }
    }

    if (NULL != pIpAdaptersInfo)
    {
        delete[] pIpAdaptersInfo;
    }

    //在UNIX平台，使用本机的host id。
#else
    char szHostID[20];

    //调用系统函数 gethostid 得到 本机器的hostid
    UINT4 nHostID = gethostid();

    //将host id转换成十六进制的字符串
    sprintf(szHostID,"%lX",nHostID);
    m_HostID.push_back(szHostID);
#endif
}

/********************************************************************
 函数原型    private: CheckHostID(const char* szHostID);
 功能描述    检查License文件中给出的Host ID是否与本机的Host ID/MAC
             地址匹配。
 输入参数
            szHostID : 从License文件中读出的host id。
 输出参数  无。
 返回值
           0 : host id匹配。
           1 : host id不匹配。
 抛出异常  无。
********************************************************************/
int CLicenseFile::CheckHostID(char* szHostID)
{
    StringLRTrim(szHostID);
    GetHostID();

    LIST<STRING>::iterator it = m_HostID.begin();
    while (it != m_HostID.end())
    {
        if (strcmp((*it).c_str(), szHostID) == 0)
        {
            return 0;
        }

        it++;
    }

    return -1;
}

/********************************************************************
 函数原型  public: time_t GetExpireDate(void);
 功能描述  获取License的有效期限。
 输入参数  无。
 输出参数  无。
 返回值    m_tExpireDate。
 抛出异常  无。
********************************************************************/
time_t CLicenseFile::GetExpireDate(void)
{
    return m_tExpireDate;
}

/********************************************************************
 函数原型  public: BOOL DecodeLicenseKey(const char* pszCoded, const int nCoded,
                                         char* pszDecoded, int& nDecoded);
 功能描述  解密License信息。
 输入参数
           pszCoded   : 加密后的License信息。
           nCoded     : pszCoded字符串的长度。
           nDecoded   : pszDecoded缓冲区的长度。
 输出参数
           pszDecoded : 解密后的信息。
           nDecoded   : 解密后的信息长度。

 返回值    解密是否成功。
 抛出异常  无。
********************************************************************/
BOOL CLicenseFile::DecodeLicenseKey(char* pszCoded, int nCoded,
                                    char* pszDecoded, int& nDecoded)
{
    if (!pszCoded || !pszDecoded)
    {
        return FALSE;
    }

    decode(pszCoded, pszDecoded);
    nDecoded = strlen(pszDecoded);

    //加密/解密算法目前未定
    //strcpy(pszDecoded, pszCoded);

    return TRUE;
}

/********************************************************************
 函数原型  public: void Refresh(void);
 功能描述  License文件更新。
 输入参数  无。
 输出参数  无。
 返回值    无。
 抛出异常  无。
********************************************************************/
int CLicenseFile::Refresh(void)
{
    //重新初始化License信息
    return Init();
}

/********************************************************************
 函数原型  public: BOOL IsExpired(void);
 功能描述  判断当前License是否过期。
 输入参数  无。
 输出参数  无。
 返回值    License是否过期。
 抛出异常  无。
********************************************************************/
BOOL CLicenseFile::IsExpired(void)
{
    if (IsTemporary())      //只有临时License才存在是否过期的问题
    {
        //获取当前时间
        time_t tNow;
        time(&tNow);

        //当前时间大于License有效期限则为过期
        int nDay = (tNow - m_tExpireDate)/(24*3600);
        return ( nDay >0 );  
    }

    return FALSE;
}
//add by zkq 2004-06-19
/********************************************************************
 函数原型  public: void GetLicenseToClient(LIST<STRING>& ContentList)
 功能描述  获得当前的license 文件内容
 输入参数  无。
 输出参数  无。
 返回值    ContentList: 当前的license 文件内容
 抛出异常  无。
********************************************************************/
void CLicenseFile::GetLicenseToClient(LIST<STRING>& ContentList)
{
	char szTime[30],tmp[30] ;
	char szLcsCount[4];
	char szLicenseID[15] = " ";	 
	int  nAPType = -1;
	BYTE bNet = 0, //是否网络备份
		 bCdr = 0;    //是否话单统计
	
	FEATURE_LIST::iterator it;  //搜索License 逐项内容   
	
	if(IsPermanent())
	{
		strcpy(szTime,"PERMANENT");		
	}
	else
	{      
		szTime[0] = '\0';
		strftime ( tmp, sizeof ( tmp ), "%Y", localtime ( &m_tExpireDate ) );
		strcat(szTime,tmp);
		strftime ( tmp, sizeof ( tmp ), "%m", localtime ( &m_tExpireDate ) );
		strcat(szTime,tmp);
		strftime ( tmp, sizeof ( tmp ), "%d", localtime ( &m_tExpireDate ) );
		strcat(szTime,tmp);
	}
	
	for (it = m_FeatureList.begin(); it != m_FeatureList.end(); it++)
	{
		nAPType = it->nLicenseID;
		switch(nAPType)
		{
		case INVALID_LICENSE_ID:              //非法的License ID
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID, "INVALIDE ID");        
			break;   
		case LICENSE_ID_NETBACKUP:           //网络备份功能
			bNet = 1;
			sprintf(szLicenseID,"%s","NETBACKUP");
			break;
		case  LICENSE_ID_CDRSTAT :           //话单统计功能
			bCdr = 1;
			sprintf(szLicenseID,"%s","CDRSTAT");
			break;
		case    LICENSE_ID_R98:              //2.5G特性
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"R98");
			break;
		case 	LICENSE_ID_R99:             //3GPP R99特性
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"R99"); 
			break;
		case 	LICENSE_ID_R4 :             //3GPP R4特性
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"R4");
			break;
		case 	LICENSE_ID_CC08:            //固网128模
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"128");
			break;
		case 	LICENSE_ID_MSC:             //MSC特性
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"MSC");
			break;  
		case LICENSE_ID_STP:               //STP特性
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"STP");
			break;
		case LICENSE_ID_S3K:               //SoftX3000特性
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"S3K");
			break;
		case 	LICENSE_ID_WLAN:           //WLAN特性
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"WLAN");
			break;
		case 	LICENSE_ID_CO:             //采集点
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"CO");
			break;
		default:
			break;
		}
		szLicenseID[14] = '\0';		
		if ((bNet == 1) || (bCdr == 1)) 
		{
			continue;	//这里不处理网络备份和话单统计	    
		}		
		sprintf(szLcsCount,"%d",it->nLicense);					
		ContentList.push_back(szLicenseID);
		ContentList.push_back(szLcsCount);
		ContentList.push_back(szTime);			
	}	
	//没有网络备份或话单统计也要显示
	if (bNet == 1) 
	{
		ContentList.push_back("NETBACKUP");
		ContentList.push_back("YES");
		ContentList.push_back(szTime);	
	}
	else
	{
		ContentList.push_back("NETBACKUP");
		ContentList.push_back("NO");
		ContentList.push_back("INVALID");
		
	}
	if (bCdr == 1) 
	{
		ContentList.push_back("CDRSTAT");
		ContentList.push_back("YES");
		ContentList.push_back(szTime);	
	}
	else
	{
		ContentList.push_back("CDRSTAT");
		ContentList.push_back("NO");
		ContentList.push_back("INVALID");	
	}
	
}
