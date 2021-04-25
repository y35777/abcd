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

    //pszLicenseDir��License�ļ����Ŀ¼
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
 ����ԭ��   public:  BOOL GetLicense(const LICENSE_ID nLicenseID,
                                     int nAPID, int& nLicense);
 ��������   �ж�ִ�е�ҵ������(nLicenseID)�Ƿ���Ȩ��
 �������
            nLicenseID : ҵ������
 �������
            nAPID   : ʵ�ʵĽ�����
 ����ֵ     �Ƿ���Ȩ��
 �׳��쳣   ��
********************************************************************/
int CLicenseFile::GetLicense(LICENSE_ID nLicenseID, int nAPID) 
{
    int nLicenseCount = 0;
    LIST<SLicenseElement>::iterator it = m_FeatureList.begin();

	int nPointNum = 1; //��ʾ��ͬ�����������LICENSE INFO = ...�г��ֵ����
	int nFactApNum = 0;//������nApID��ʵ�ʵĽ����˳�򼯺���ͬ�����͵����

	if (nAPID > m_LicenseVector.size())
	{
		return 0;
	}
	
	for (int i = 0; i < nAPID; i++) //Ѱ����֮ǰ�Ľ���㣬��ͳ��ͬ�����͵ĸ���
	{
		if (m_LicenseVector[i] == nLicenseID)
		{
			nFactApNum++;
		}
	}

    while (it != m_FeatureList.end())
    {   
        //License ID��Ҫƥ��
		if ((*it).nLicenseID == nLicenseID)
		{
			//�Ա���ģ�������ͳ��ģ��
			if ((nLicenseID == LICENSE_ID_CDRSTAT) || (nLicenseID == LICENSE_ID_NETBACKUP)
				&& ((*it).nAPID == nAPID))
			{
				nLicenseCount = (*it).nLicense;
				break;
			}
			else
			{
				// ��ͨ�Ľ��������ǲɼ������
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
 ����ԭ��    public:  BOOL IsValid(void);
 ��������    License�Ƿ�Ϸ���
 �������    �ޡ�
 �������    �ޡ�
 ����ֵ      License�Ƿ�Ϸ���
 �׳��쳣    ��
********************************************************************/
BOOL CLicenseFile::IsValid(void)
{
    return m_bValidLicense;
}

/********************************************************************
 ����ԭ��    public:  BOOL IsPermanent(void);
 ��������    �Ƿ���������License��
 �������    �ޡ�
 �������    �ޡ�
 ����ֵ      �Ƿ���������License��
 �׳��쳣    ��
********************************************************************/
BOOL CLicenseFile::IsPermanent(void)
{
    return ((m_bValidLicense == TRUE) && (m_tExpireDate == PERMANENT_LICENSE));
}

/********************************************************************
 ����ԭ��    public:  BOOL IsTemporary(void);
 ��������    �Ƿ�����ʱLicense��
 �������    �ޡ�
 �������    �ޡ�
 ����ֵ      �Ƿ�����ʱLicense��
 �׳��쳣    ��
********************************************************************/
BOOL CLicenseFile::IsTemporary(void)
{
    return ((m_bValidLicense == TRUE) && (m_tExpireDate != PERMANENT_LICENSE));
}

/********************************************************************
 ����ԭ��    public:  int Init(void);
 ��������    License��Ϣ��ʼ����
 �������    ��
 �������    ��
 ����ֵ      ��ʼ���Ƿ�ɹ���
 �׳��쳣    ��
********************************************************************/
int CLicenseFile::Init(void)
{
    char szLicenseFile[MAX_PATH] = "";      //License�ļ�ȫ·����
    FILE* fp = NULL;

    m_bValidLicense = FALSE;            //��ʼ��LicenseΪ�Ƿ�
    m_tExpireDate   = 0;
    m_FeatureList.clear();

    //��ȡLicense�ļ���ȫ·����license�ļ����̶�Ϊlicense.dat
    sprintf(szLicenseFile, "%slicense.dat", m_szLicenseDir);

    //��ֻ�����ı���ʽ��License�ļ�
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

    //����һ�У�License�������ޣ��и�ʽΪ����˾��   �汾    ���ڡ�
    fscanf(fp, "%s %s %s", szTemp1, szTemp2, szLicenseDate);
    if (0 != ParseDate(szLicenseDate))
    {
        fclose(fp);
        return -1;
    }

    //��HOST ID���и�ʽΪ��HOST ID = hostid��
    fscanf(fp, "%s %s %s %s", szTemp1, szTemp2, szTemp3, szHostID);
    if (0 != CheckHostID(szHostID))
    {
        fclose(fp);
        return -1;
    }

    //��ҵ�����ԣ��и�ʽΪ��LICENSE INFO = Feature1-N1:Feature2-N2:...:Featuren-Nn��
    fscanf(fp, "%s %s %s %s", szTemp1, szTemp2, szTemp3, szFetureList);
    if (0 != ParseFeature(szFetureList))
    {
        fclose(fp);
        return -1;
    }

    char szCodedKey[3072]    = "";
    char szDecodedKey[3072]  = "";
    int nLen = 1024;

    //License������Ϣ
    fscanf(fp, "%s %s %s %s", szTemp1, szTemp2, szTemp3, szCodedKey);
    StringLRTrim(szCodedKey);
    char szComposed[3072]    = "";

    //��License���ڡ�host id��License��Ȩ��Ϣ�����ַ�����������
    sprintf(szComposed, "%s%s%s", szLicenseDate, szHostID, szFetureList);

    DecodeLicenseKey(szCodedKey, strlen(szCodedKey), szDecodedKey, nLen);

    //License������ϢУ��
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
 ����ԭ��    private: int ParseDate(const char* pszLine);
 ��������    �˺���Ϊ���ܺ�������License�ļ���һ���л�õ�License
             ��Ч���ޡ�
 �������  �ޡ�

 �������
            pszLine  : License������
 ����ֵ     �Ƿ�ɹ���
 �׳��쳣   ��
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

    //�Ƿ���������License
    if (strcmp(szDate, "PERMANENT") == 0)
    {
        m_tExpireDate = PERMANENT_LICENSE;
        return 0;
    }

    //������Ǳ�׼��DD-MON-YYYY��ʽ�����ڣ�����Ϊ���ڷǷ�
    if (strlen(szDate) != 11)
    {
        return -1;
    }

    char szDay[3]   = "";
    char szMon[4]   = "";
    char szYear[5]  = "";

    //ȡ����λ����
    strncpy(szDay, &szDate[0], 2);
    szDay[2] = '\0';

    //ȡ����λ����
    strncpy(szMon, &szDate[3], 3);
    szMon[3] = '\0';

    //ȡ����λ����
    strncpy(szYear, &szDate[7], 4);
    szYear[4] = '\0';

    if (!IsDigitValue(szDay) || !IsDigitValue(szYear))
    {
        return -1;
    }

    struct tm tExpireDate;

    //��ʼ��tExpireDate�е�������
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
 ����ԭ��    private: int ParseFeature(const char* pszLine);
 ��������    �˺���Ϊ���ܺ���������License��Ȩ�б�
 �������  �ޡ�

 �������
            pszLine  : License��Ȩ��Ϣ��
 ����ֵ     �Ƿ�ɹ���
 �׳��쳣   ��
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

    //����ͳ��
    if (strcmp(sFeature.c_str(), "CDRSTAT") == 0)
    {
        le.nLicenseID = LICENSE_ID_CDRSTAT;
        le.nLicense   = 1;
        le.nAPID      = ACCESS_POINT_ANY;
        m_FeatureList.push_back(le);
    }
    //���籸��
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

        //License��Ȩ����
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
 ����ԭ��    private: void GetHostID(void);
 ��������    �˺���Ϊ���ܺ�������������host id/MAC��ַ������UNIXƽ̨��
             ʹ��host id������WINDOWSƽ̨��ʹ������������MAC��ַ��
 �������  �ޡ�
 �������  �ޡ�
 ����ֵ    �ޡ�
 �׳��쳣  �ޡ�
********************************************************************/
void CLicenseFile::GetHostID(void)
{
    m_HostID.clear();

    //��WIN32ƽ̨�£�������������MAC��ַ��Ϊhost id
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

    //��UNIXƽ̨��ʹ�ñ�����host id��
#else
    char szHostID[20];

    //����ϵͳ���� gethostid �õ� ��������hostid
    UINT4 nHostID = gethostid();

    //��host idת����ʮ�����Ƶ��ַ���
    sprintf(szHostID,"%lX",nHostID);
    m_HostID.push_back(szHostID);
#endif
}

/********************************************************************
 ����ԭ��    private: CheckHostID(const char* szHostID);
 ��������    ���License�ļ��и�����Host ID�Ƿ��뱾����Host ID/MAC
             ��ַƥ�䡣
 �������
            szHostID : ��License�ļ��ж�����host id��
 �������  �ޡ�
 ����ֵ
           0 : host idƥ�䡣
           1 : host id��ƥ�䡣
 �׳��쳣  �ޡ�
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
 ����ԭ��  public: time_t GetExpireDate(void);
 ��������  ��ȡLicense����Ч���ޡ�
 �������  �ޡ�
 �������  �ޡ�
 ����ֵ    m_tExpireDate��
 �׳��쳣  �ޡ�
********************************************************************/
time_t CLicenseFile::GetExpireDate(void)
{
    return m_tExpireDate;
}

/********************************************************************
 ����ԭ��  public: BOOL DecodeLicenseKey(const char* pszCoded, const int nCoded,
                                         char* pszDecoded, int& nDecoded);
 ��������  ����License��Ϣ��
 �������
           pszCoded   : ���ܺ��License��Ϣ��
           nCoded     : pszCoded�ַ����ĳ��ȡ�
           nDecoded   : pszDecoded�������ĳ��ȡ�
 �������
           pszDecoded : ���ܺ����Ϣ��
           nDecoded   : ���ܺ����Ϣ���ȡ�

 ����ֵ    �����Ƿ�ɹ���
 �׳��쳣  �ޡ�
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

    //����/�����㷨Ŀǰδ��
    //strcpy(pszDecoded, pszCoded);

    return TRUE;
}

/********************************************************************
 ����ԭ��  public: void Refresh(void);
 ��������  License�ļ����¡�
 �������  �ޡ�
 �������  �ޡ�
 ����ֵ    �ޡ�
 �׳��쳣  �ޡ�
********************************************************************/
int CLicenseFile::Refresh(void)
{
    //���³�ʼ��License��Ϣ
    return Init();
}

/********************************************************************
 ����ԭ��  public: BOOL IsExpired(void);
 ��������  �жϵ�ǰLicense�Ƿ���ڡ�
 �������  �ޡ�
 �������  �ޡ�
 ����ֵ    License�Ƿ���ڡ�
 �׳��쳣  �ޡ�
********************************************************************/
BOOL CLicenseFile::IsExpired(void)
{
    if (IsTemporary())      //ֻ����ʱLicense�Ŵ����Ƿ���ڵ�����
    {
        //��ȡ��ǰʱ��
        time_t tNow;
        time(&tNow);

        //��ǰʱ�����License��Ч������Ϊ����
        int nDay = (tNow - m_tExpireDate)/(24*3600);
        return ( nDay >0 );  
    }

    return FALSE;
}
//add by zkq 2004-06-19
/********************************************************************
 ����ԭ��  public: void GetLicenseToClient(LIST<STRING>& ContentList)
 ��������  ��õ�ǰ��license �ļ�����
 �������  �ޡ�
 �������  �ޡ�
 ����ֵ    ContentList: ��ǰ��license �ļ�����
 �׳��쳣  �ޡ�
********************************************************************/
void CLicenseFile::GetLicenseToClient(LIST<STRING>& ContentList)
{
	char szTime[30],tmp[30] ;
	char szLcsCount[4];
	char szLicenseID[15] = " ";	 
	int  nAPType = -1;
	BYTE bNet = 0, //�Ƿ����籸��
		 bCdr = 0;    //�Ƿ񻰵�ͳ��
	
	FEATURE_LIST::iterator it;  //����License ��������   
	
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
		case INVALID_LICENSE_ID:              //�Ƿ���License ID
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID, "INVALIDE ID");        
			break;   
		case LICENSE_ID_NETBACKUP:           //���籸�ݹ���
			bNet = 1;
			sprintf(szLicenseID,"%s","NETBACKUP");
			break;
		case  LICENSE_ID_CDRSTAT :           //����ͳ�ƹ���
			bCdr = 1;
			sprintf(szLicenseID,"%s","CDRSTAT");
			break;
		case    LICENSE_ID_R98:              //2.5G����
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"R98");
			break;
		case 	LICENSE_ID_R99:             //3GPP R99����
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"R99"); 
			break;
		case 	LICENSE_ID_R4 :             //3GPP R4����
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"R4");
			break;
		case 	LICENSE_ID_CC08:            //����128ģ
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"128");
			break;
		case 	LICENSE_ID_MSC:             //MSC����
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"MSC");
			break;  
		case LICENSE_ID_STP:               //STP����
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"STP");
			break;
		case LICENSE_ID_S3K:               //SoftX3000����
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"S3K");
			break;
		case 	LICENSE_ID_WLAN:           //WLAN����
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"WLAN");
			break;
		case 	LICENSE_ID_CO:             //�ɼ���
			sprintf(szLicenseID,"AP%d(%s)",it->nAPID,"CO");
			break;
		default:
			break;
		}
		szLicenseID[14] = '\0';		
		if ((bNet == 1) || (bCdr == 1)) 
		{
			continue;	//���ﲻ�������籸�ݺͻ���ͳ��	    
		}		
		sprintf(szLcsCount,"%d",it->nLicense);					
		ContentList.push_back(szLicenseID);
		ContentList.push_back(szLcsCount);
		ContentList.push_back(szTime);			
	}	
	//û�����籸�ݻ򻰵�ͳ��ҲҪ��ʾ
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
