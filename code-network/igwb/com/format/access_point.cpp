/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CAccessPoint
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format_lib.h"
#include "format.h"
#include "format_global.h"
#include "access_point.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"


IGWB_Svc_Export enum CBB_OPERATOR_ID g_nCBB_OI;



/************ The CAccessPoint Defining ************************/
// �������������캯��
//
// ���������
// sFmtRootDir����ʽ�����ø�Ŀ¼
//       uAPID���������ID
//     pFmtLib����ʽ�����ָ��
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CAccessPoint::CAccessPoint(const STRING& sFmtRootDir, const UINT4 uAPID, 
                           CFormatLib* pFmtLib): m_sFmtRootDir(sFmtRootDir),
                           m_uAPID(uAPID), m_pFmtLib(pFmtLib)
{
    //���ݲ�����ʼ����Ӧ�ĳ�Ա����
    //m_sFmtRootDir = sFmtRootDir;
    //m_uAPID = uAPID;
    //m_pFmtLib = (CFormatLib*)pFmtLib;
    m_nAPType = -1;  //��ʼ��Ϊ��Ч�ý��������

    //��ʼ������ָ���Ա����
    m_pChannels = NULL;
    m_pOrigFmtMaps = NULL;
    m_pOrigBrowseFmtMaps = NULL;

    m_pOrgFmtNameList = NULL;  //Added by ZhengYuqun 2004-01-18 SWPD03392
    g_nCBB_OI = CBB_OPERATOR_ID_DEFAULT;

}
/************ End of The CAccessPoint Defining *****************/


/************ The ~CAccessPoint Defining ***********************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CAccessPoint::~CAccessPoint(void)
{
    //ɾ����������е�����ͨ�������б�
    if(m_pChannels != NULL)
    {
        CHL_MAP::iterator item = m_pChannels->begin();
        while(item != m_pChannels->end())
        {
            delete item->second;  //ɾ��ͨ������
            item++;
        }
        delete m_pChannels;
        m_pChannels = NULL;
    }

    FMT_MAP::iterator it;

    //ɾ����ͨ����Ӧ��ԭʼ������ʽ�����б�
    if(m_pOrigFmtMaps != NULL)
    {
        it = m_pOrigFmtMaps->begin();
        while(it != m_pOrigFmtMaps->end())
        {
            delete it->second;  //ɾ���б��е�SFmtMap�ṹ
            it++;
        }
        delete m_pOrigFmtMaps;
        m_pOrigFmtMaps = NULL;
    }

    //ɾ����ͨ����Ӧ�����ջ��������ʽ�б�
    if(m_pOrigBrowseFmtMaps != NULL)
    {
        it = m_pOrigBrowseFmtMaps->begin();
        while(it != m_pOrigBrowseFmtMaps->end())
        {
            delete it->second;  //ɾ���б��е�SFmtMap�ṹ
            it++;
        }
        delete m_pOrigBrowseFmtMaps;
        m_pOrigBrowseFmtMaps = NULL;
    }

    //Added by ZhengYuqun 2004-01-18 SWPD03392
    if(m_pOrgFmtNameList)
    {
        delete m_pOrgFmtNameList;
        m_pOrgFmtNameList = NULL;
    }
}
/************ End of The ~CAccessPoint Defining ****************/


/************ The Init Defining ********************************/
// ������������ʼ������㣬������������ļ������ڴ棬�����������ĸ���ͨ������
//
// ���������
//   pFmtLib����ʽ�����CAccessPoint�����ͷ�֮
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::Init(CFormatLib* pFmtLib)
{
    //ȷ��m_pFmtLib����Ϊ��
    if((pFmtLib == NULL) && (m_pFmtLib == NULL))
    {
        return -1;
    }
    else if(pFmtLib != NULL)
    {
        m_pFmtLib = pFmtLib;
    }

    //added by maimaoshi at 2002-12-24
    /***** �������ļ��ж�ȡ��������� ****************/
    int nPos  = m_sFmtRootDir.find_last_of('/');
#ifdef _PLATFORM_WIN32
    int nPos2 = m_sFmtRootDir.find_last_of('\\');
#else //_PLATFORM_WIN32
    int nPos2 = -1;
#endif //_PLATFORM_WIN32
    nPos      = max(nPos, nPos2);
    if (nPos <= 0)
    {
        return ERR_FAIL;
    }

	STRING sConfigPath;
    sConfigPath = m_sFmtRootDir.substr(0, nPos + 1);
	sConfigPath = sConfigPath + "ini/igwb.ini";
	
    //��ȡ��ǰ������������
    char szTmp[10];
    sprintf(szTmp, "%u", m_uAPID);
    STRING sAPSection = FMT_INI_APSection;
    sAPSection += szTmp;

    //�������ļ�
    CINIFile* pIni = new CINIFile(sConfigPath.c_str());
    if(!pIni->Open())
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sConfigPath.c_str());
        delete pIni;
        return -1;
    }

	//�������ļ��ж�ȡ��ǰ���������
	char szAPName[255];
    pIni->GetString(sAPSection.c_str(), CFG_AP_NAME, sAPSection.c_str(), szAPName, 255);
	m_sAPName = szAPName;

    //�������ļ��ж�ȡ��ǰ���������
    m_nAPType = pIni->GetInt(sAPSection.c_str(), FMT_INI_APTYPE, -1);

	//��CollectType����0,��ʾ�ǲɼ������
	int nCollectType = pIni->GetInt(sAPSection.c_str(), CFG_AP_KEY_COLTYPE, 
                                        CFG_AP_KEY_COLTYPE_DEFAULT);
	if (nCollectType)
	{
		m_nAPType = AP_OTHER + nCollectType;
	}

    BOOL bMustMerge = pIni->GetInt(sAPSection.c_str(),
                                   CFG_AP_KEY_MUST_MERGE,
                                   CFG_AP_KEY_MUST_MERGE_DEFAULT);
    m_pFmtLib->SetMustMerge(bMustMerge);

    char szCBBOI[255];
    pIni->GetString(sAPSection.c_str(), CFG_CBB_OI, CFG_CBB_OI_DEFAULT, szCBBOI, 255);
    if (StringiCmp(szCBBOI, "CMCC"))
    {
    	g_nCBB_OI = CBB_OPERATOR_ID_CMCC;
    }
    else
    {
    	g_nCBB_OI = CBB_OPERATOR_ID_DEFAULT;
    }
    
    pIni->Close();
    delete pIni;
    if(m_nAPType == -1)
    {
        TRACE(MTS_FORMAT, S_FMT_UNKONWN_APTYPE, 
              m_sAPName.c_str(), sConfigPath.c_str());
        return -1;
    }
    /***** �������ļ��ж�ȡ������������ ************/
   //end by maimaoshi at 2002-12-24


    //�ѽ���������ļ������ڴ�
    if(LoadConfig() != 0)
    {
        return -1;
    }

    //��������Ҫ�õ�����ʱ����
    CFormat* pFMT = NULL;
    FMT_MAP::iterator it;

    if(m_pOrigFmtMaps != NULL)
    {
        //��ʼ���Ը�ʽ��Ϊ������ԭʼ������ʽ�б�
        it = m_pOrigFmtMaps->begin();
        while(it != m_pOrigFmtMaps->end())
        {
            //��ȡ��ʽ����Ӧ�ĸ�ʽ����
            pFMT = m_pFmtLib->GetFormat(it->first, m_uAPID);
            if(pFMT == NULL)
            {
                TRACE(MTS_FORMAT, S_FMT_ORIGBILL_FMT_NOT_FOUND, 
                      m_uAPID, it->first.c_str());
                return -1;
            }
            it->second->pFmt = pFMT;

            it++;
        }
    }

    if(m_pOrigBrowseFmtMaps != NULL)
    {
        //��ʼ���Ը�ʽ��Ϊ������ֵ��ԭʼ���������ʽ��
        it = m_pOrigBrowseFmtMaps->begin();
        while(it != m_pOrigBrowseFmtMaps->end())
        {
            //��ȡ��ʽ����Ӧ�ĸ�ʽ����
            pFMT = m_pFmtLib->GetFormat(it->first, m_uAPID);
            if(pFMT == NULL)
            {
                TRACE(MTS_FORMAT, S_FMT_ORIGBILL_BRW_FMT_NOT_FOUND, 
                      m_uAPID, it->first.c_str());
                return -1;
            }
            it->second->pFmt = pFMT;

            it++;
        }
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The GetAPID Defining *****************************/
// ������������ý����ID
//
// �����������
//
// �����������
//
//   ����ֵ�����ر������ID
//
/***************************************************************/
UINT4 CAccessPoint::GetAPID(void) const
{
    return m_uAPID;
}
/************ End of The GetAPID Defining **********************/


/************ The GetAPName Defining ***************************/
// ������������ý��������
//
// ���������
//   sAPName�����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CAccessPoint::GetAPName(STRING& sAPName) const
{
    sAPName = m_sAPName;
}
/************ End of The GetAPName Defining ********************/


/************ The LoadConfig Defining **************************/
// �����������������������ļ�
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::LoadConfig(void)
{
    char    szFile[MAX_PATH];      //����������ļ�ȫ·��
    sprintf(szFile, "%s/%d.apt", m_sFmtRootDir.c_str(), m_uAPID);
    int nFileFmt = VerifyFileFmt(szFile);
    
    if (nFileFmt == FILE_FMT_BINARY)
    {
        return LoadBinaryConfig(szFile);
    }
    else
    {
        return LoadAsciiConfig(szFile);
    }
    
    return 0;
}
/************ End of The LoadConfig Defining *******************/


/************ The GetOrigLen Defining **************************/
// �������������ԭʼ������ʽ���ȣ�����MSC��128��STP��CDMA�Ȳ�Ʒ�Ļ�����
//           ԭʼ�����ǹ̶����ȣ����������ظ�ԭʼ�������ȣ�����GPRS��
//           3G��Ʒ��ԭʼ�����Ǳ䳤�ģ�����������0
//
// �����������
//
// �����������
//
//   ����ֵ��ԭʼ������ʽ����
//
/***************************************************************/
UINT4 CAccessPoint::GetOrigLen(void) const
{
    UINT4 uLen = 0;

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pOrigFmtMaps == NULL)
    {
        return 0;
    }

    FMT_MAP::iterator it = m_pOrigFmtMaps->begin();
    uLen = it->second->pFmt->GetFmtLen();
    while(it != m_pOrigFmtMaps->end())
    {
        //����������ε��û�õĻ������Ȳ�һ�£����ʾ
        //�ý���㻰���Ǳ䳤�ģ�����0
        if(uLen != it->second->pFmt->GetFmtLen())
        {
            return 0;
        }

        it++;
    }

    return uLen;
}
/************ End of The GetOrigLen Defining *******************/

UINT4 CAccessPoint::GetFinalLen(UINT4 uChlID) const
{
    if (m_pChannels->find(uChlID) != m_pChannels->end())
    {
        return (*m_pChannels)[uChlID]->GetFinalLen();
    }

    return 0;
}

/************ The GetChannelNames Defining *********************/
// ������������ñ������������ͨ�����ƣ�������ȱʡ��0ͨ��
//
// �����������
//
// ���������
//  ChlNames �����ͨ�������б�
//
//   ����ֵ��ͨ������
//
/***************************************************************/
UINT4 CAccessPoint::GetChannelNames(LIST<STRING>& ChlNames) const
{
    ChlNames.clear();
    STRING sChlName;

    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pChannels == NULL)
    {
        return 0;
    }

    //����������������ͨ��
    CHL_MAP::iterator it = m_pChannels->begin();
    while(it != m_pChannels->end())
    {
        //��ȡ��ǰͨ������
        it->second->GetChlName(sChlName);
        ChlNames.push_back(sChlName);

        it++;
    }

    return ChlNames.size();
}
/************ End of The GetChannelNames Defining **************/


/************ The GetFinalBrowseNames Defining *****************/
// ������������ñ�������ͨ����ΪsChlNameͨ�����������ջ��������ʽ��
//
// ���������
//   sChlName��ͨ������
//
// ���������
//      Names��������ջ��������ʽ���б�
//      Descs�����ջ��������ʽ�����б�
//
//   ����ֵ����ͨ�������ջ��������ʽ������
//
/***************************************************************/
UINT4 CAccessPoint::GetFinalBrowseNames(const STRING& sChlName, LIST<STRING>& Names, 
                                        LIST<STRING>& Descs) const
{
    CChannel*  pChl = NULL;  //ͨ������ָ����ʱ����
    STRING     sTmpChlName;  //ͨ��������ʱ����

    //��ʼ���������
    Names.clear();
    Descs.clear();

    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pChannels == NULL)
    {
        return 0;
    }

    //������������е�����ͨ��
    CHL_MAP::iterator it = m_pChannels->begin();
    while(it != m_pChannels->end())
    {
        //�жϵ�ǰͨ�������Ƿ���ڴ��жϵ�ͨ������
        it->second->GetChlName(sTmpChlName);
        if(!(sChlName.compare(sTmpChlName)))
        {
            //�ҵ�ͨ�����Ƶ��ڴ��жϵ�ͨ������
            pChl = it->second;
            break;
        }

        it++;
    }

    //�����������д���ͨ�����Ƶ��ڴ��жϵ�ͨ�����Ƶ�ͨ��
    if(pChl != NULL)
    {
        //��ȡ��ͨ���е��������ջ��������ʽ���б�
        int nRet = pChl->GetFinalBrowseNames(Names, Descs);
        return nRet;
    }

    TRACE(MTS_FORMAT, S_FMT_CHLNAME_NOT_FOUND, 
          sChlName.c_str(), m_uAPID);
    return 0;
}
/************ End of The GetFinalBrowseNames Defining **********/


/************ The GetOrigBrowseNames Defining ******************/
// ������������ñ������������ԭʼ���������ʽ��
//
// ���������
//
// ���������
//      Names���������������ԭʼ���������ʽ���б�
//       Decs��ԭʼ���������ʽ�����б�
//
//   ����ֵ�����ػ�õĸ�ʽ����Ŀ
//
/***************************************************************/
UINT4 CAccessPoint::GetOrigBrowseNames(LIST<STRING>& Names, LIST<STRING>& Descs) const
{
    //��ʼ���������
    Names.clear();
    Descs.clear();

    //��������Ҫ�õ�����ʱ����
    STRING sFmtDesc;
    STRING sSrcFmtName;

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pOrigBrowseFmtMaps == NULL)
    {
        return 0;
    }

    //Modified by ZhengYuqun 2004-01-18 SWPD03392
    //����������������ԭʼ���������ʽ�б�
    /*FMT_MAP::iterator it = m_pOrigBrowseFmtMaps->begin();
    while(it != m_pOrigBrowseFmtMaps->end())
    {
        //��ȡ��ǰ��ʽ���͸�ʽ����
        Names.push_back(it->first);
        it->second->pFmt->GetFormatGen(sFmtDesc, sSrcFmtName);
        Descs.push_back(sFmtDesc);

        it++;
    }*/

    //�ȴ�LIST�еõ�����Ȼ���ٴ�MAP�еõ�����
    if(m_pOrgFmtNameList != NULL)
    {
        ORG_FMT_NAME_LIST::iterator lst_it = m_pOrgFmtNameList->begin();
        while(lst_it != m_pOrgFmtNameList->end())
        {
            FMT_MAP::iterator it;
            it = m_pOrigBrowseFmtMaps->find(*lst_it);
            if(it != m_pOrigBrowseFmtMaps->end())
            {
                //��ȡ��ǰ��ʽ���͸�ʽ����
                Names.push_back(it->first);
                it->second->pFmt->GetFormatGen(sFmtDesc, sSrcFmtName);
                Descs.push_back(sFmtDesc);
            }

            lst_it++;
        }
    }
    //End SWPD03392

    return Names.size();
}
/************ End of The GetOrigBrowseNames Defining ***********/


/************ The Distribute Defining **************************/
// �����������ּ�һ�Ż�������øû���������ͨ���͸�ʽ�������
//
// ���������
//     pABill��ԭʼ����
//       uLen����������
//
// ���������
//     uChlID�����Ż����ּ��������ͨ��ID�����û�ж�Ӧ��ͨ����
//             ��uChlIDΪ0�����ּ�Ĭ��ͨ����
//   pDestFmt�����Ż������������ջ��������ʽ���󣬵����߲����ͷ�
//             ��õ�pDestFmtָ��
//
//   ����ֵ����
//
/***************************************************************/
void CAccessPoint::Distribute(const void* pABill, const UINT4 uLen, 
                              UINT4& uChlID, CFormat*& pDestFmt)
{
    //��ʼ���������
    pDestFmt = NULL;
    uChlID = 0;

    //������������е�����ͨ��
    if(m_pChannels != NULL)
    {
        CHL_MAP::iterator it = m_pChannels->begin();
        while(it != m_pChannels->end())
        {
            //�ж�����Ļ��������Ƿ���ϵ�ǰͨ��
            int nRet = it->second->IsTheChannel(pABill, uLen, pDestFmt);
            if(nRet == 0)
            {
                //�������Ļ������ݷ��ϵ�ǰͨ��
                uChlID = it->first;
                break;
            }

            it++;
        }
    }
}
/************ End of The Distribute Defining *******************/

void CAccessPoint::SpecDistribute(const void* pABill, const UINT4 uLen, VECTOR<UINT4>& uChlID, 
							  VECTOR<CFormat*>& pDestFmt)
{
    //��ʼ���������
    pDestFmt.clear();
    uChlID.clear();
	CFormat* pFmt = NULL;
	
    //������������е�����ͨ��
    if(m_pChannels != NULL)
    {
        CHL_MAP::iterator it = m_pChannels->begin();
        while(it != m_pChannels->end())
        {
			pFmt = NULL;
            //�ж�����Ļ��������Ƿ���ϵ�ǰͨ��
            int nRet = it->second->IsTheChannel(pABill, uLen, pFmt);
            if(nRet == 0)
            {
                //�������Ļ������ݷ��ϵ�ǰͨ��
                uChlID.push_back(it->first);
				pDestFmt.push_back(pFmt);
            }
            it++;
        }
    }
}


/************ The SetAPName Defining ***************************/
// �������������ý��������
//
// ���������
//    sAPName�����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CAccessPoint::SetAPName(const STRING& sAPName)
{
    m_sAPName = sAPName;
}
/************ End of The SetAPName Defining ********************/



/************ The Create Defining ******************************/
// ��������������һ�ֽ����
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::Create(void)
{
    FILE*   pAPFile = NULL;    //����������ļ�ָ��
    STRING  sFilePath;         //����������ļ�ȫ·��
    char    szBuff[50];        //��ʱ������
    char*   pTmpStr = szBuff;  //ָ����ʱ������ָ��
    int     nWriteLen  = 0;    //д�ļ����ݵĳ���


    //��ȡ����������ļ�ȫ·��
    sFilePath = m_sFmtRootDir + "/";
    sprintf(pTmpStr, "%u.apt", m_uAPID);
    sFilePath += pTmpStr;

    //����ָ�������ID�Ľ���������ļ�
    pAPFile = fopen(sFilePath.c_str(), "wb");
    if(pAPFile == NULL)
    {
        //�����һ�δ��ļ�ʧ�ܣ��򴴽����ļ���Ŀ¼
        if(!CreateDeepDir(m_sFmtRootDir.c_str()))
        {
            return -1;  //�����ļ�Ŀ¼ʧ��
        }

        //�ٴ�һ���ļ�
        pAPFile = fopen(sFilePath.c_str(), "wb");
        if(pAPFile == NULL)
        {
            return -1;  //�ڶ��δ��ļ�ʧ��
        }
    }

    //�������������д�������ļ�
    memset((void*)szBuff, 0, 50);
    nWriteLen = m_sAPName.length() + 1;
    if(nWriteLen > 16)
    {
        nWriteLen = 16;  //�����������󳤶���16���ֽ�
    }
    memcpy((void*)szBuff, (void*)m_sAPName.c_str(), nWriteLen);
    if(fwrite((void*)szBuff, 16, 1, pAPFile) != 1)
    {
        fclose(pAPFile);
        return -1;
    }

    //�رս���������ļ�
    fclose(pAPFile);

    return 0;
}
/************ End of The Create Defining ***********************/


/************ The AddBrowseNames Defining **********************/
// ����������׷�Ӹý�����ԭʼ���������ʽ��
//
// ���������
//     nCount�������ʽ������
//  szFormats�������ʽ�����飬������nCountָ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::AddBrowseNames(UINT4 nCount, const char* szFormats[])
{
    //ȷ�������������ȷ��
    if((nCount == 0) || (szFormats == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nCount; i++)
    {
        if(szFormats[i] == NULL)
        {
            return -1;
        }
    }

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pOrigBrowseFmtMaps == NULL)
    {
        m_pOrigBrowseFmtMaps = new FMT_MAP;
    }

    SFmtMap* pFmp = NULL;
    CFormat* pFmt = NULL;
    FMT_MAP::iterator it;
    for(int j = 0; j < nCount; j++)
    {
        //�ж������ʽ���б����Ƿ��Ѿ����ڵ�ǰ�������ʽ��
        it = m_pOrigBrowseFmtMaps->find(szFormats[j]);
        if(it != m_pOrigBrowseFmtMaps->end())
        {
            //����Ѿ����ڣ�����Ե�ǰ�����ʽ��
            //return -1;
            continue;
        }

        //�����µ������ʽ��Ӧ��ϵ
        pFmp = new SFmtMap;
        pFmp->sFmtName = szFormats[j];

        //��ȡ��ʽ����Ӧ�ĸ�ʽ����
        pFmt = m_pFmtLib->GetFormat(pFmp->sFmtName);
        if(pFmt == NULL)
        {
            delete pFmp;
            return -1;
        }

        //�������������ʽ��Ӧ��ϵ���������ʽ�б���
        pFmp->pFmt = pFmt;
        (*m_pOrigBrowseFmtMaps)[pFmp->sFmtName] = pFmp;
    }

    return 0;
}
/************ End of The AddBrowseNames Defining ***************/


/************ The AddChannel Defining **************************/
// ��������������ָ����ͨ��
//
// ���������
//     nChlID��ͨ��ID
//   sChlName��ͨ������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::AddChannel(UINT4 nChlID, const STRING& sChlName)
{
    //ȷ�������������ȷ��
    if((nChlID == 0) || (sChlName.length() == 0))
    {
        return -1;
    }

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pChannels == NULL)
    {
        m_pChannels = new CHL_MAP;
    }

    //���ָ����ͨ���Ѿ�����
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        return -1;
    }

    //�����µ�ͨ��
    CChannel* pChl = new CChannel(m_sFmtRootDir, m_uAPID, nChlID, m_pFmtLib);
    pChl->SetChlName(sChlName);
    int nRet = pChl->Create();

    //�������ָ��ͨ�������ļ�ʧ��
    if(nRet != 0)
    {
        delete pChl;
        return -1;
    }

    (*m_pChannels)[nChlID] = pChl;

    return 0;
}
/************ End of The AddChannel Defining *******************/


/************ The SetDistingCondition Defining *****************/
// ��������������ָ��ͨ�������һ�����ջ���Դ��ʽ�ķּ�����
//
// ���������
//     nChlID��ͨ��ID
// sDisFormat����ͨ����һ�����ջ�����ʽ��Դ��ʽ�ĸ�ʽ��
//   sDisCond���ּ�����
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::SetDistingCondition(UINT4 nChlID, const STRING& sDisFormat,
                                      const STRING& sDisCond)
{
    //ȷ�����������ȷ��
    if((nChlID == 0) || (sDisFormat.length() == 0) || (sDisCond.length() == 0))
    {
        return -1;
    }

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pChannels == NULL)
    {
        return -1;
    }

    //����ҵ�ָ����ͨ��
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        return it->second->SetDistingCondition(sDisFormat, sDisCond);
    }

    return -1;
}
/************ End of The SetDistingCondition Defining **********/


/************ The SetChlFormats Defining ***********************/
// ������������׷�ӵķ�ʽ����ָ��ͨ�������ջ�����ʽ�������ջ��������ʽ��
//
// ���������
//               nChlID��ָ����ͨ����ID
//       nFinalFmtCount���������õ����ջ�����ʽ����
//       szFinalFormats�����ջ�����ʽ�����飬������nFinalFmtCount��Ա��
//                       ÿһ����Ա����һ�����ջ�����ʽ��
// nFinalBrowseFmtCount���������õ����ջ��������ʽ����
//    szFinalBrowseFmts�����ջ��������ʽ�����飬������nFinalBrowseFmtCount
//                       ����Ա��ÿһ����Ա����һ�����ջ��������ʽ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::SetChlFormats(UINT4 nChlID, UINT4 nFinalFmtCount, const char* szFinalFormats[],
                                UINT4 nFinalBrowseFmtCount, const char* szFinalBrowseFmts[])
{
    //ȷ�����������ȷ��
    if((nChlID == 0) || (nFinalFmtCount == 0) || (szFinalFormats == NULL)
       || (nFinalBrowseFmtCount == 0) || (szFinalBrowseFmts == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nFinalFmtCount; i++)
    {
        if(szFinalFormats[i] == NULL)
        {
            return -1;
        }
    }
    for(int j = 0; j < nFinalBrowseFmtCount; j++)
    {
        if(szFinalBrowseFmts[j] == NULL)
        {
            return -1;
        }
    }

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pChannels == NULL)
    {
        return -1;
    }

    //����ָ����ͨ��
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        //����ҵ�ָ����ͨ�������ڸ�ͨ��������ָ�������ջ�����ʽ�������ջ��������ʽ��
        return it->second->AddChlFormats(nFinalFmtCount, szFinalFormats,
                                         nFinalBrowseFmtCount, szFinalBrowseFmts);
    }

    return -1;
}
/************ End of The SetChlFormats Defining ****************/


/************ The GetChlFormats Defining ***********************/
// ����������ָ��ͨ����ţ���ø�ͨ�����е����ջ�����ʽ��
//           �����ջ��������ʽ��
//
// ���������
//      nChlID��ָ����ͨ����ID
//
// ���������
//          FinalFmts����õ����ջ�����ʽ���б�
// FinalBrowseFormats����õ����ջ��������ʽ���б�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::GetChlFormats(const UINT4 nChlID, LIST<STRING>& FinalFmts,
                                LIST<STRING>& FinalBrowseFormats)  const
{
    //��ʼ���������
    FinalFmts.clear();
    FinalBrowseFormats.clear();

    //ȷ�����������ȷ
    if(nChlID == 0)
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if(m_pChannels == NULL)
    {
        return -1;
    }

    //����ָ����ͨ��
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        //����ҵ�ָ����ͨ��
        return it->second->GetChlFormats(FinalFmts, FinalBrowseFormats);
    }

    TRACE(MTS_FORMAT, S_FMT_CHLNAME_NOT_FOUND, 
          nChlID, m_uAPID);
    return -1;
}
/************ End of The GetChlFormats Defining ****************/


/************ The DelChannel Defining **************************/
// ����������ɾ��ָ��ͨ��
//
// ���������
//      nChlID��ָ����ͨ����ID
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::DelChannel(UINT4 nChlID)
{
    //ȷ�����������ȷ
    if(nChlID == 0)
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if(m_pChannels == NULL)
    {
        return -1;
    }

    //����ָ����ͨ��
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        //����ҵ�ָ����ͨ��
        CChannel* pChl = it->second;
        if(pChl->Delete() != 0)
        {
            return -1;
        }

        //��ͨ������ӳ���ϵ��ɾ��ָ����ͨ������
        delete pChl;
        m_pChannels->erase(it);
        return 0;
    }

    return -1;
}
/************ End of The DelChannel Defining *******************/


/************ The AddOrgFmtNames Defining **********************/
// ����������׷�Ӹý�����ԭʼ������ʽ��
//
// ���������
//     nCount����ʽ������
//  szFormats����ʽ�����飬������nCountָ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::AddOrgFmtNames(UINT4 nCount, const char* szFormats[])
{
    //ȷ�������������ȷ��
    if((nCount == 0) || (szFormats == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nCount; i++)
    {
        if(szFormats[i] == NULL)
        {
            return -1;
        }
    }

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pOrigFmtMaps == NULL)
    {
        m_pOrigFmtMaps = new FMT_MAP;
    }

    SFmtMap* pFmp = NULL;
    CFormat* pFmt = NULL;
    FMT_MAP::iterator it;
    for(int j = 0; j < nCount; j++)
    {
        //�жϸ�ʽ���б����Ƿ��Ѿ����ڵ�ǰ�ĸ�ʽ��
        it = m_pOrigFmtMaps->find(szFormats[j]);
        if(it != m_pOrigFmtMaps->end())
        {
            //����Ѿ����ڣ�����Ե�ǰ��ʽ��
            //return -1;
            continue;
        }

        //�����µ������ʽ��Ӧ��ϵ
        pFmp = new SFmtMap;
        pFmp->sFmtName = szFormats[j];

        //��ȡ��ʽ����Ӧ�ĸ�ʽ����
        pFmt = m_pFmtLib->GetFormat(pFmp->sFmtName);
        if(pFmt == NULL)
        {
            delete pFmp;
            return -1;
        }

        //�������ĸ�ʽ��Ӧ��ϵ�����ʽ�б���
        pFmp->pFmt = pFmt;
        (*m_pOrigFmtMaps)[pFmp->sFmtName] = pFmp;
    }

    return 0;
}
/************ End of The AddOrgFmtNames Defining ***************/


/************ The GetOrgFmtNames Defining **********************/
// ������������ñ������������ԭʼ������ʽ��
//
// �����������
//
// ���������
//      Names���������������ԭʼ������ʽ���б�
//       Decs��ԭʼ������ʽ�����б�
//
//   ����ֵ�����ػ�õĸ�ʽ����Ŀ
//
/***************************************************************/
UINT4 CAccessPoint::GetOrgFmtNames(LIST<STRING>& Names, LIST<STRING>& Descs) const
{
    //��ʼ���������
    Names.clear();
    Descs.clear();

    //ȷ����Ӧ��Ա��������Ϊ��
    if(m_pOrigFmtMaps == NULL)
    {
        return 0;
    }

    //��������Ҫ�õ�����ʱ����
    STRING sFmtDesc;
    STRING sSrcFmtName;

    //����������������ԭʼ������ʽ�б�
    FMT_MAP::iterator it = m_pOrigFmtMaps->begin();
    while(it != m_pOrigFmtMaps->end())
    {
        //��ȡ��ǰ��ʽ���͸�ʽ����
        Names.push_back(it->first);
        it->second->pFmt->GetFormatGen(sFmtDesc, sSrcFmtName);
        Descs.push_back(sFmtDesc);

        it++;
    }

    return Names.size();
}
/************ End of The GetOrgFmtNames Defining ***************/


/************ The Commit Defining ******************************/
// ����������ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ
//           ���浽������
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::Commit(void)
{
    STRING  sFilePath;                  //��������ļ�ȫ·��
    FILE*   pFile   = NULL;             //�����ļ�ָ��
    int     nWriteLen  = 0;             //д�ļ����ݵĳ���
    BYTE    pBuff[100];                 //��д�ļ�����ʱ���Ȳ���ĵط���д0�Ļ�����
    char*   pTmpStr  = (char*)pBuff;    //ָ�򻺳�����ָ��
    BYTE    yTmpNum = 0;                //дһ���ֽڳ��ȵ���������

    
    //��ȡ����������ļ�ȫ·��
    sFilePath = m_sFmtRootDir + "/";
    sprintf(pTmpStr, "%u.apt", m_uAPID);
    sFilePath += pTmpStr;

    //��ָ���Ľ���������ļ�
    pFile = fopen(sFilePath.c_str(), "wb");
    if(pFile == NULL)
    {
        //�����һ�δ��ļ�ʧ�ܣ��򴴽����ļ���Ŀ¼
        if(!CreateDeepDir(m_sFmtRootDir.c_str()))
        {
            return -1;  //�����ļ�Ŀ¼ʧ��
        }

        //�ٴ�һ���ļ�
        pFile = fopen(sFilePath.c_str(), "wb");
        if(pFile == NULL)
        {
            return -1;  //�ڶ��δ��ļ�ʧ��
        }
    }

    //�����������д�������ļ�
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sAPName.length() + 1;
    if(nWriteLen > LEN_AP_NAME)
    {
        nWriteLen = LEN_AP_NAME;  //�����������󳤶���16���ֽ�
    }
    memcpy((void*)pBuff, (void*)m_sAPName.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_AP_NAME, 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //�����������д�������ļ�
    INT4 nTmp = m_nAPType;
    if(fwrite((void*)&nTmp, sizeof(INT4), 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //��ͨ������д�������ļ�
    if(m_pChannels == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pChannels->size();  //ͨ������ռ��һ���ֽ�
    }
    if(fwrite((void*)&yTmpNum, sizeof(BYTE), 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //��ԭʼ������ʽ����д�������ļ�
    if(m_pOrigFmtMaps == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pOrigFmtMaps->size();  //ԭʼ������ʽ����ռ��һ���ֽ�
    }
    if(fwrite((void*)&yTmpNum, sizeof(BYTE), 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //����������Ҫ�õ�����ʱ����
    FMT_MAP::iterator it;
    STRING sFmtName;

    //��ÿһ��ԭʼ������ʽ��д�������ļ�
    if(m_pOrigFmtMaps != NULL)
    {
        it = m_pOrigFmtMaps->begin();
        while(it != m_pOrigFmtMaps->end())
        {
            //����ǰ��ʽ��д�������ļ�
            memset((void*)pBuff, 0, 100);
            sFmtName = it->first;
            nWriteLen = sFmtName.length() + 1;
            if(nWriteLen > LEN_AP_ORIG_FMT_NAME)
            {
                nWriteLen = LEN_AP_ORIG_FMT_NAME;  //��ʽ������󳤶���16���ֽ�
            }
            memcpy((void*)pBuff, (void*)sFmtName.c_str(), nWriteLen);
            if(fwrite((void*)pBuff, LEN_AP_ORIG_FMT_NAME, 1, pFile) != 1)
            {
                fclose(pFile);
                return -1;
            }

            it++;
        }
    }

    //��ԭʼ�������ʽ����д�������ļ�
    if(m_pOrigBrowseFmtMaps == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pOrigBrowseFmtMaps->size();  //ԭʼ�������ʽ����ռ��һ���ֽ�
    }
    if(fwrite((void*)&yTmpNum, sizeof(BYTE), 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //��ÿһ��ԭʼ���������ʽ��д�������ļ�
    if(m_pOrigBrowseFmtMaps != NULL)
    {
        it = m_pOrigBrowseFmtMaps->begin();
        while(it != m_pOrigBrowseFmtMaps->end())
        {
            //����ǰ��ʽ��д�������ļ�
            memset((void*)pBuff, 0, 100);
            sFmtName = it->first;
            nWriteLen = sFmtName.length() + 1;
            if(nWriteLen > LEN_AP_ORIGBRS_FMT_NAME)
            {
                nWriteLen = LEN_AP_ORIGBRS_FMT_NAME;  //��ʽ������󳤶���16���ֽ�
            }
            memcpy((void*)pBuff, (void*)sFmtName.c_str(), nWriteLen);
            if(fwrite((void*)pBuff, LEN_AP_ORIGBRS_FMT_NAME, 1, pFile) != 1)
            {
                fclose(pFile);
                return -1;
            }

            it++;
        }
    }

    //�رո�ʽ�����ļ�
    fclose(pFile);

    //���汾��������������ͨ����������Ϣ
    if(m_pChannels != NULL)
    {
        //��������ͨ��
        CHL_MAP::iterator it = m_pChannels->begin();
        while(it != m_pChannels->end())
        {
            //���浱ǰͨ��������Ϣ
            if(it->second->Commit() != 0)
            {
                return -1;
            }

            it++;
        }
    }

    return 0;
}
/************ End of The Commit Defining ***********************/


/************ The Delete Defining ******************************/
// ����������ɾ�������
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CAccessPoint::Delete(void)
{
    //ɾ����������е�����ͨ��������Ϣ
    if(m_pChannels != NULL)
    {
        CChannel* pChl = NULL;
        CHL_MAP::iterator it = m_pChannels->begin();
        while(it != m_pChannels->end())
        {
            pChl = it->second;
            if(pChl->Delete() != 0)
            {
                return -1;
            }

            //��ͨ������ӳ���ϵ��ɾ����ǰ��ͨ������
            delete pChl;
            it++;
        }

        m_pChannels->clear();
        delete m_pChannels;
        m_pChannels = NULL;
    }

    STRING    sFilePath;           //����������ļ�ȫ·������
    char      szBuff[20];          //��ʱ������
    char*     pTmpStr  = szBuff;   //ָ�򻺳�����ָ��
    
    //��ȡ����������ļ�ȫ·��
    sFilePath = m_sFmtRootDir + "/";
    sprintf(pTmpStr, "%u.apt", m_uAPID);
    sFilePath += pTmpStr;

    //ɾ��ָ���������ļ�
    if(remove(sFilePath.c_str()) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The Delete Defining ***********************/

/*
  �жϽ���������ļ��ı����ʽ�Ƕ����ƻ����ı���ʽ��
  sAPFile : ����������ļ���ȫ·������
*/
int CAccessPoint::VerifyFileFmt(const STRING& sAPFile)
{
	FILE* pAPFile = NULL;
	if ((pAPFile = fopen(sAPFile.c_str(), "rt")) == NULL)
	{
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
            sAPFile.c_str());        
		return FILE_FMT_UNKNOWN;
	}
	
	BOOL bChlNumFound = FALSE;

	char szLine[1024];
	while (!feof(pAPFile))
	{
		if (NULL == fgets(szLine, 1024, pAPFile))
		{
			break;
		}

        StringLRTrim(szLine);
        
        //���У���������һ��
        if (strlen(szLine) == 0)
        {
            continue;
        }
        
        STRING sLine = szLine;        

		//ͨ��������
		if (sLine.find("ChannelNumber", 0) == 0)
		{
			bChlNumFound = TRUE;
            break;
		}
	}

	fclose(pAPFile);
	if (bChlNumFound)
	{
		return FILE_FMT_ASCII;
	}
	else
	{
		return FILE_FMT_BINARY;
	}
}

BOOL CAccessPoint::LoadBinaryConfig(const STRING& sAPFile)
{
    FILE*   pAPFile = NULL;    //����������ļ�ָ��
    char    szBuff[50];        //��ȡ�ļ����ݵ���ʱ������
    char*   pTmpStr = szBuff;  //ָ����ʱ������ָ��
    BYTE    yTmpNum = 0;       //��ȡһ���ֽڵ�������ʱ����


    //��ֻ����ʽ�򿪽���������ļ�
    pAPFile = fopen(sAPFile.c_str(), "rb");
    if(pAPFile == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sAPFile.c_str());
        return -1;
    }

    //�������������ͨ������
    if((fread((void*)&yTmpNum, sizeof(BYTE), 1, pAPFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sAPFile.c_str());
        fclose(pAPFile);
        return -1;
    }
    if(yTmpNum != 0)
    {
        if(m_pChannels == NULL)
        {
            m_pChannels = new CHL_MAP;
        }
        CChannel* pChl = NULL;

        //��ʼ����ͨ������������ͨ����ͨ��ID��1��ʼ��˳�����
        for(int nChlID = 1; nChlID <= yTmpNum; nChlID++)
        {
            pChl = new CChannel(m_sFmtRootDir, m_uAPID, nChlID, m_pFmtLib);
            if((pChl->Init(m_pFmtLib)) == -1)
            {
                //��ͨ����ʼ��ʧ��
                delete pChl;
                fclose(pAPFile);

                return -1;
            }
            (*m_pChannels)[nChlID] = pChl;
        }
    }

    //��������Ҫ�õ�����ʱ����
    SFmtMap* pFmtMap = NULL;

    //����ԭʼ������ʽ����
    if((fread((void*)&yTmpNum, sizeof(BYTE), 1, pAPFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sAPFile.c_str());
        fclose(pAPFile);
        return -1;
    }
    if(yTmpNum != 0)
    {
        if(m_pOrigFmtMaps == NULL)
        {
            m_pOrigFmtMaps = new FMT_MAP;
        }

        for(int i = 0; i < yTmpNum; i++)
        {
            //��ԭʼ������ʽ��
            if((fread((void*)pTmpStr, LEN_AP_ORIG_FMT_NAME, 1, pAPFile)) != 1)
            {
                TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                      sAPFile.c_str());
                fclose(pAPFile);
                return -1;
            }
            else
            {
                *(pTmpStr + LEN_AP_ORIG_FMT_NAME) = '\0';
            }

            pFmtMap = new SFmtMap;
            pFmtMap->pFmt = NULL;
            pFmtMap->sFmtName = pTmpStr;
            (*m_pOrigFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
        }
    }

    //��ԭʼ���������ʽ����
    if((fread((void*)&yTmpNum, sizeof(BYTE), 1, pAPFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sAPFile.c_str());
        fclose(pAPFile);
        return -1;
    }
    if(yTmpNum != 0)
    {
        if(m_pOrigBrowseFmtMaps == NULL)
        {
            m_pOrigBrowseFmtMaps = new FMT_MAP;
        }

        //Added by ZhengYuqun 2004-01-18 SWPD03392
        if(m_pOrgFmtNameList == NULL)
        {
            m_pOrgFmtNameList = new ORG_FMT_NAME_LIST;
        }
        //End SWPD03392

        for(int j = 0; j < yTmpNum; j++)
        {
            //����ԭʼ���������ʽ��
            if((fread((void*)pTmpStr, LEN_AP_ORIGBRS_FMT_NAME, 1, pAPFile)) != 1)
            {
                TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                      sAPFile.c_str());
                fclose(pAPFile);
                return -1;
            }
            else
            {
                *(pTmpStr + LEN_AP_ORIGBRS_FMT_NAME) = '\0';
            }

            pFmtMap = new SFmtMap;
            pFmtMap->sFmtName = pTmpStr;
            pFmtMap->pFmt = NULL;
            (*m_pOrigBrowseFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
            m_pOrgFmtNameList->push_back(pFmtMap->sFmtName);  //Added by ZhengYuqun 2004-01-18 SWPD03392
        }
    }

    //�رմ򿪵��ļ�
    fclose(pAPFile);

    return 0;
}

/*
  �����ı���ʽ�Ľ���������ļ���������ʽΪ��
  ChannelNumber   n
  OriginalFormat  formatname1
  OriginalFormat  formatname2
      . . .
  OriginalFormat  formatnameN  
  OriginalBrowseForrmat browseformatname1
  OriginalBrowseForrmat browseformatname2
     . . .
  OriginalBrowseForrmat browseformatnameM
  
  ���У�ChannelNumber��OriginalFormat��OriginalBrowseForrmat��Ϊ�ؼ��ʡ� 
*/
BOOL CAccessPoint::LoadAsciiConfig(const STRING& sAPFile)
{
	FILE*   pFile = NULL;				//����������ļ�ָ��
    int     nChlNumber = 0;				//��ȡһ���ֽڵ�������ʱ����
	char    szFmtName[1024];
    STRING  sFmtName;

    //��ֻ����ʽ�򿪽���������ļ�
    if ((pFile = fopen(sAPFile.c_str(), "rt")) == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sAPFile.c_str());        
        return -1;
    }

    char szLine[1024];
	while (!feof(pFile))
	{
		if (NULL == fgets(szLine, 1024, pFile))
		{
			break;
		}


        StringLRTrim(szLine);
        //���У���������һ��
        if (strlen(szLine) == 0)
        {
            continue;
        }
        
        STRING sLine = szLine;        

        char szTemp[1024];
        
		//ͨ��������
		if (sLine.find("ChannelNumber", 0) == 0)
		{            
            if(m_pChannels == NULL)
            {
                m_pChannels = new CHL_MAP;
            }
            
			sscanf(szLine, "%s %d", szTemp, &nChlNumber);
            if (nChlNumber != 0)
            {                                
                CChannel* pChl = NULL;

                //��ʼ����ͨ������������ͨ����ͨ��ID��1��ʼ��˳�����
                for(int nChlID = 1; nChlID <= nChlNumber; nChlID++)
                {
                    pChl = new CChannel(m_sFmtRootDir, m_uAPID, nChlID, m_pFmtLib);
                    if ((pChl->Init(m_pFmtLib)) == -1)
                    {
                        //��ͨ����ʼ��ʧ��
                        delete pChl;
                        fclose(pFile);

                        return -1;
                    }
                    
                    (*m_pChannels)[nChlID] = pChl;
                }
            }
		}
		else if (sLine.find("OriginalFormat", 0) == 0)
		{
            if(m_pOrigFmtMaps == NULL)
            {
                m_pOrigFmtMaps = new FMT_MAP;
            }
            
			sscanf(szLine, "%s %s", szTemp, szFmtName);
            StringLRTrim(szFmtName);
            SFmtMap* pFmtMap = new SFmtMap;
            pFmtMap->pFmt = NULL;
            pFmtMap->sFmtName = szFmtName;
            (*m_pOrigFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
		}
		else if (sLine.find("OriginalBrowseFormat", 0) == 0)
		{
            if(m_pOrigBrowseFmtMaps == NULL)
            {
                m_pOrigBrowseFmtMaps = new FMT_MAP;
            }
            
            if(m_pOrgFmtNameList == NULL)
            {
                m_pOrgFmtNameList = new ORG_FMT_NAME_LIST;
            }
                        
			sscanf(szLine, "%s %s", szTemp, szFmtName);
            StringLRTrim(szFmtName);	
            SFmtMap* pFmtMap = new SFmtMap;
            pFmtMap->sFmtName = szFmtName;
            pFmtMap->pFmt = NULL;
            (*m_pOrigBrowseFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
            m_pOrgFmtNameList->push_back(pFmtMap->sFmtName); 
		}
        else
        {
            continue;
        }
	}

    //�رմ򿪵��ļ�
    fclose(pFile);

	return 0;
}
