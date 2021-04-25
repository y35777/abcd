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
// 功能描述：构造函数
//
// 输入参数：
// sFmtRootDir：格式库配置根目录
//       uAPID：本接入点ID
//     pFmtLib：格式库对象指针
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CAccessPoint::CAccessPoint(const STRING& sFmtRootDir, const UINT4 uAPID, 
                           CFormatLib* pFmtLib): m_sFmtRootDir(sFmtRootDir),
                           m_uAPID(uAPID), m_pFmtLib(pFmtLib)
{
    //根据参数初始化相应的成员变量
    //m_sFmtRootDir = sFmtRootDir;
    //m_uAPID = uAPID;
    //m_pFmtLib = (CFormatLib*)pFmtLib;
    m_nAPType = -1;  //初始化为无效得接入点类型

    //初始化其它指针成员变量
    m_pChannels = NULL;
    m_pOrigFmtMaps = NULL;
    m_pOrigBrowseFmtMaps = NULL;

    m_pOrgFmtNameList = NULL;  //Added by ZhengYuqun 2004-01-18 SWPD03392
    g_nCBB_OI = CBB_OPERATOR_ID_DEFAULT;

}
/************ End of The CAccessPoint Defining *****************/


/************ The ~CAccessPoint Defining ***********************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CAccessPoint::~CAccessPoint(void)
{
    //删除本接入点中的所有通道对象列表
    if(m_pChannels != NULL)
    {
        CHL_MAP::iterator item = m_pChannels->begin();
        while(item != m_pChannels->end())
        {
            delete item->second;  //删除通道对象
            item++;
        }
        delete m_pChannels;
        m_pChannels = NULL;
    }

    FMT_MAP::iterator it;

    //删除本通道对应的原始话单格式对象列表
    if(m_pOrigFmtMaps != NULL)
    {
        it = m_pOrigFmtMaps->begin();
        while(it != m_pOrigFmtMaps->end())
        {
            delete it->second;  //删除列表中的SFmtMap结构
            it++;
        }
        delete m_pOrigFmtMaps;
        m_pOrigFmtMaps = NULL;
    }

    //删除本通道对应的最终话单浏览格式列表
    if(m_pOrigBrowseFmtMaps != NULL)
    {
        it = m_pOrigBrowseFmtMaps->begin();
        while(it != m_pOrigBrowseFmtMaps->end())
        {
            delete it->second;  //删除列表中的SFmtMap结构
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
// 功能描述：初始化接入点，将接入点配置文件载入内存，创建本接入点的各个通道对象
//
// 输入参数：
//   pFmtLib：格式库对象，CAccessPoint不得释放之
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::Init(CFormatLib* pFmtLib)
{
    //确保m_pFmtLib不能为空
    if((pFmtLib == NULL) && (m_pFmtLib == NULL))
    {
        return -1;
    }
    else if(pFmtLib != NULL)
    {
        m_pFmtLib = pFmtLib;
    }

    //added by maimaoshi at 2002-12-24
    /***** 从配置文件中读取接入点类型 ****************/
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
	
    //获取当前接入点的区域名
    char szTmp[10];
    sprintf(szTmp, "%u", m_uAPID);
    STRING sAPSection = FMT_INI_APSection;
    sAPSection += szTmp;

    //打开配置文件
    CINIFile* pIni = new CINIFile(sConfigPath.c_str());
    if(!pIni->Open())
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sConfigPath.c_str());
        delete pIni;
        return -1;
    }

	//从配置文件中读取当前接入点名称
	char szAPName[255];
    pIni->GetString(sAPSection.c_str(), CFG_AP_NAME, sAPSection.c_str(), szAPName, 255);
	m_sAPName = szAPName;

    //从配置文件中读取当前接入点类型
    m_nAPType = pIni->GetInt(sAPSection.c_str(), FMT_INI_APTYPE, -1);

	//当CollectType大于0,表示是采集接入点
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
    /***** 从配置文件中读取接入点类型完毕 ************/
   //end by maimaoshi at 2002-12-24


    //把接入点配置文件读入内存
    if(LoadConfig() != 0)
    {
        return -1;
    }

    //定义下面要用到的临时变量
    CFormat* pFMT = NULL;
    FMT_MAP::iterator it;

    if(m_pOrigFmtMaps != NULL)
    {
        //初始化以格式名为索引的原始话单格式列表
        it = m_pOrigFmtMaps->begin();
        while(it != m_pOrigFmtMaps->end())
        {
            //获取格式名对应的格式对象
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
        //初始化以格式名为索引键值的原始话单浏览格式列
        it = m_pOrigBrowseFmtMaps->begin();
        while(it != m_pOrigBrowseFmtMaps->end())
        {
            //获取格式名对应的格式对象
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
// 功能描述：获得接入点ID
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：返回本接入点ID
//
/***************************************************************/
UINT4 CAccessPoint::GetAPID(void) const
{
    return m_uAPID;
}
/************ End of The GetAPID Defining **********************/


/************ The GetAPName Defining ***************************/
// 功能描述：获得接入点名称
//
// 输入参数：
//   sAPName：接入点名称
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CAccessPoint::GetAPName(STRING& sAPName) const
{
    sAPName = m_sAPName;
}
/************ End of The GetAPName Defining ********************/


/************ The LoadConfig Defining **************************/
// 功能描述：载入接入点配置文件
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::LoadConfig(void)
{
    char    szFile[MAX_PATH];      //接入点配置文件全路径
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
// 功能描述：获得原始话单格式长度，对于MSC、128、STP、CDMA等产品的话单，
//           原始话单是固定长度，本函数返回该原始话单长度，对于GPRS、
//           3G产品，原始话单是变长的，本函数返回0
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：原始话单格式长度
//
/***************************************************************/
UINT4 CAccessPoint::GetOrigLen(void) const
{
    UINT4 uLen = 0;

    //确保相应成员变量不能为空
    if(m_pOrigFmtMaps == NULL)
    {
        return 0;
    }

    FMT_MAP::iterator it = m_pOrigFmtMaps->begin();
    uLen = it->second->pFmt->GetFmtLen();
    while(it != m_pOrigFmtMaps->end())
    {
        //如果连续两次调用获得的话单长度不一致，则表示
        //该接入点话单是变长的，返回0
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
// 功能描述：获得本接入点下所有通道名称，不包含缺省的0通道
//
// 输入参数：无
//
// 输出参数：
//  ChlNames ：获得通道名称列表
//
//   返回值：通道个数
//
/***************************************************************/
UINT4 CAccessPoint::GetChannelNames(LIST<STRING>& ChlNames) const
{
    ChlNames.clear();
    STRING sChlName;

    //确保相应成员变量不为空
    if(m_pChannels == NULL)
    {
        return 0;
    }

    //遍历本接入点的所有通道
    CHL_MAP::iterator it = m_pChannels->begin();
    while(it != m_pChannels->end())
    {
        //获取当前通道名称
        it->second->GetChlName(sChlName);
        ChlNames.push_back(sChlName);

        it++;
    }

    return ChlNames.size();
}
/************ End of The GetChannelNames Defining **************/


/************ The GetFinalBrowseNames Defining *****************/
// 功能描述：获得本接入点的通道名为sChlName通道的所有最终话单浏览格式名
//
// 输入参数：
//   sChlName：通道名称
//
// 输出参数：
//      Names：获得最终话单浏览格式名列表
//      Descs：最终话单浏览格式描述列表
//
//   返回值：该通道的最终话单浏览格式名个数
//
/***************************************************************/
UINT4 CAccessPoint::GetFinalBrowseNames(const STRING& sChlName, LIST<STRING>& Names, 
                                        LIST<STRING>& Descs) const
{
    CChannel*  pChl = NULL;  //通道对象指针临时变量
    STRING     sTmpChlName;  //通道名称临时变量

    //初始化输出参数
    Names.clear();
    Descs.clear();

    //确保相应成员变量不为空
    if(m_pChannels == NULL)
    {
        return 0;
    }

    //遍历本接入点中的所有通道
    CHL_MAP::iterator it = m_pChannels->begin();
    while(it != m_pChannels->end())
    {
        //判断当前通道名称是否等于待判断的通道名称
        it->second->GetChlName(sTmpChlName);
        if(!(sChlName.compare(sTmpChlName)))
        {
            //找到通道名称等于待判断的通道名称
            pChl = it->second;
            break;
        }

        it++;
    }

    //如果本接入点中存在通道名称等于待判断的通道名称的通道
    if(pChl != NULL)
    {
        //获取该通道中的所有最终话单浏览格式名列表
        int nRet = pChl->GetFinalBrowseNames(Names, Descs);
        return nRet;
    }

    TRACE(MTS_FORMAT, S_FMT_CHLNAME_NOT_FOUND, 
          sChlName.c_str(), m_uAPID);
    return 0;
}
/************ End of The GetFinalBrowseNames Defining **********/


/************ The GetOrigBrowseNames Defining ******************/
// 功能描述：获得本接入点下所有原始话单浏览格式名
//
// 输入参数：
//
// 输出参数：
//      Names：本接入点下所有原始话单浏览格式名列表
//       Decs：原始话单浏览格式描述列表
//
//   返回值：返回获得的格式名数目
//
/***************************************************************/
UINT4 CAccessPoint::GetOrigBrowseNames(LIST<STRING>& Names, LIST<STRING>& Descs) const
{
    //初始化输出参数
    Names.clear();
    Descs.clear();

    //定义下面要用到的临时变量
    STRING sFmtDesc;
    STRING sSrcFmtName;

    //确保相应成员变量不能为空
    if(m_pOrigBrowseFmtMaps == NULL)
    {
        return 0;
    }

    //Modified by ZhengYuqun 2004-01-18 SWPD03392
    //遍历本接入点的所有原始话单浏览格式列表
    /*FMT_MAP::iterator it = m_pOrigBrowseFmtMaps->begin();
    while(it != m_pOrigBrowseFmtMaps->end())
    {
        //获取当前格式名和格式描述
        Names.push_back(it->first);
        it->second->pFmt->GetFormatGen(sFmtDesc, sSrcFmtName);
        Descs.push_back(sFmtDesc);

        it++;
    }*/

    //先从LIST中得到名字然后再从MAP中得到内容
    if(m_pOrgFmtNameList != NULL)
    {
        ORG_FMT_NAME_LIST::iterator lst_it = m_pOrgFmtNameList->begin();
        while(lst_it != m_pOrgFmtNameList->end())
        {
            FMT_MAP::iterator it;
            it = m_pOrigBrowseFmtMaps->find(*lst_it);
            if(it != m_pOrigBrowseFmtMaps->end())
            {
                //获取当前格式名和格式描述
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
// 功能描述：分拣一张话单，获得该话单所属的通道和格式处理对象
//
// 输入参数：
//     pABill：原始话单
//       uLen：话单长度
//
// 输出参数：
//     uChlID：该张话单分拣后所属的通道ID，如果没有对应的通道，
//             则uChlID为0，即分拣到默认通道下
//   pDestFmt：该张话单所属的最终话单处理格式对象，调用者不得释放
//             获得的pDestFmt指针
//
//   返回值：无
//
/***************************************************************/
void CAccessPoint::Distribute(const void* pABill, const UINT4 uLen, 
                              UINT4& uChlID, CFormat*& pDestFmt)
{
    //初始化输出参数
    pDestFmt = NULL;
    uChlID = 0;

    //遍历本接入点中的所有通道
    if(m_pChannels != NULL)
    {
        CHL_MAP::iterator it = m_pChannels->begin();
        while(it != m_pChannels->end())
        {
            //判断输入的话单数据是否符合当前通道
            int nRet = it->second->IsTheChannel(pABill, uLen, pDestFmt);
            if(nRet == 0)
            {
                //如果输入的话单数据符合当前通道
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
    //初始化输出参数
    pDestFmt.clear();
    uChlID.clear();
	CFormat* pFmt = NULL;
	
    //遍历本接入点中的所有通道
    if(m_pChannels != NULL)
    {
        CHL_MAP::iterator it = m_pChannels->begin();
        while(it != m_pChannels->end())
        {
			pFmt = NULL;
            //判断输入的话单数据是否符合当前通道
            int nRet = it->second->IsTheChannel(pABill, uLen, pFmt);
            if(nRet == 0)
            {
                //如果输入的话单数据符合当前通道
                uChlID.push_back(it->first);
				pDestFmt.push_back(pFmt);
            }
            it++;
        }
    }
}


/************ The SetAPName Defining ***************************/
// 功能描述：设置接入点名称
//
// 输入参数：
//    sAPName：接入点名称
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CAccessPoint::SetAPName(const STRING& sAPName)
{
    m_sAPName = sAPName;
}
/************ End of The SetAPName Defining ********************/



/************ The Create Defining ******************************/
// 功能描述：创建一种接入点
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::Create(void)
{
    FILE*   pAPFile = NULL;    //接入点配置文件指针
    STRING  sFilePath;         //接入点配置文件全路径
    char    szBuff[50];        //临时缓冲区
    char*   pTmpStr = szBuff;  //指向临时缓冲区指针
    int     nWriteLen  = 0;    //写文件数据的长度


    //获取接入点配置文件全路径
    sFilePath = m_sFmtRootDir + "/";
    sprintf(pTmpStr, "%u.apt", m_uAPID);
    sFilePath += pTmpStr;

    //创建指定接入点ID的接入点配置文件
    pAPFile = fopen(sFilePath.c_str(), "wb");
    if(pAPFile == NULL)
    {
        //如果第一次打开文件失败，则创建该文件的目录
        if(!CreateDeepDir(m_sFmtRootDir.c_str()))
        {
            return -1;  //创建文件目录失败
        }

        //再打开一次文件
        pAPFile = fopen(sFilePath.c_str(), "wb");
        if(pAPFile == NULL)
        {
            return -1;  //第二次打开文件失败
        }
    }

    //将本接入点名称写入配置文件
    memset((void*)szBuff, 0, 50);
    nWriteLen = m_sAPName.length() + 1;
    if(nWriteLen > 16)
    {
        nWriteLen = 16;  //接入点名称最大长度是16个字节
    }
    memcpy((void*)szBuff, (void*)m_sAPName.c_str(), nWriteLen);
    if(fwrite((void*)szBuff, 16, 1, pAPFile) != 1)
    {
        fclose(pAPFile);
        return -1;
    }

    //关闭接入点配置文件
    fclose(pAPFile);

    return 0;
}
/************ End of The Create Defining ***********************/


/************ The AddBrowseNames Defining **********************/
// 功能描述：追加该接入点的原始话单浏览格式名
//
// 输入参数：
//     nCount：浏览格式名个数
//  szFormats：浏览格式名数组，个数由nCount指定
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::AddBrowseNames(UINT4 nCount, const char* szFormats[])
{
    //确保输入参数的正确性
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

    //确保相应成员变量不能为空
    if(m_pOrigBrowseFmtMaps == NULL)
    {
        m_pOrigBrowseFmtMaps = new FMT_MAP;
    }

    SFmtMap* pFmp = NULL;
    CFormat* pFmt = NULL;
    FMT_MAP::iterator it;
    for(int j = 0; j < nCount; j++)
    {
        //判断浏览格式名列表中是否已经存在当前的浏览格式名
        it = m_pOrigBrowseFmtMaps->find(szFormats[j]);
        if(it != m_pOrigBrowseFmtMaps->end())
        {
            //如果已经存在，则忽略当前浏览格式名
            //return -1;
            continue;
        }

        //创建新的浏览格式对应关系
        pFmp = new SFmtMap;
        pFmp->sFmtName = szFormats[j];

        //获取格式名对应的格式对象
        pFmt = m_pFmtLib->GetFormat(pFmp->sFmtName);
        if(pFmt == NULL)
        {
            delete pFmp;
            return -1;
        }

        //将创建的浏览格式对应关系放入浏览格式列表中
        pFmp->pFmt = pFmt;
        (*m_pOrigBrowseFmtMaps)[pFmp->sFmtName] = pFmp;
    }

    return 0;
}
/************ End of The AddBrowseNames Defining ***************/


/************ The AddChannel Defining **************************/
// 功能描述：增加指定的通道
//
// 输入参数：
//     nChlID：通道ID
//   sChlName：通道名称
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::AddChannel(UINT4 nChlID, const STRING& sChlName)
{
    //确保输入参数的正确性
    if((nChlID == 0) || (sChlName.length() == 0))
    {
        return -1;
    }

    //确保相应成员变量不能为空
    if(m_pChannels == NULL)
    {
        m_pChannels = new CHL_MAP;
    }

    //如果指定的通道已经存在
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        return -1;
    }

    //创建新的通道
    CChannel* pChl = new CChannel(m_sFmtRootDir, m_uAPID, nChlID, m_pFmtLib);
    pChl->SetChlName(sChlName);
    int nRet = pChl->Create();

    //如果创建指定通道配置文件失败
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
// 功能描述：设置指定通道的针对一种最终话单源格式的分拣条件
//
// 输入参数：
//     nChlID：通道ID
// sDisFormat：该通道的一种最终话单格式的源格式的格式名
//   sDisCond：分拣条件
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::SetDistingCondition(UINT4 nChlID, const STRING& sDisFormat,
                                      const STRING& sDisCond)
{
    //确保输入参数正确性
    if((nChlID == 0) || (sDisFormat.length() == 0) || (sDisCond.length() == 0))
    {
        return -1;
    }

    //确保相应成员变量不能为空
    if(m_pChannels == NULL)
    {
        return -1;
    }

    //如果找到指定的通道
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        return it->second->SetDistingCondition(sDisFormat, sDisCond);
    }

    return -1;
}
/************ End of The SetDistingCondition Defining **********/


/************ The SetChlFormats Defining ***********************/
// 功能描述：以追加的方式设置指定通道的最终话单格式名和最终话单浏览格式名
//
// 输入参数：
//               nChlID：指定的通道的ID
//       nFinalFmtCount：本次设置的最终话单格式个数
//       szFinalFormats：最终话单格式名数组，其中有nFinalFmtCount成员，
//                       每一个成员代表一种最终话单格式名
// nFinalBrowseFmtCount：本次设置的最终话单浏览格式个数
//    szFinalBrowseFmts：最终话单浏览格式名数组，其中有nFinalBrowseFmtCount
//                       个成员，每一个成员代表一种最终话单浏览格式名
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::SetChlFormats(UINT4 nChlID, UINT4 nFinalFmtCount, const char* szFinalFormats[],
                                UINT4 nFinalBrowseFmtCount, const char* szFinalBrowseFmts[])
{
    //确保输入参数正确性
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

    //确保相应成员变量不能为空
    if(m_pChannels == NULL)
    {
        return -1;
    }

    //查找指定的通道
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        //如果找到指定的通道，则在该通道中增加指定的最终话单格式名和最终话单浏览格式名
        return it->second->AddChlFormats(nFinalFmtCount, szFinalFormats,
                                         nFinalBrowseFmtCount, szFinalBrowseFmts);
    }

    return -1;
}
/************ End of The SetChlFormats Defining ****************/


/************ The GetChlFormats Defining ***********************/
// 功能描述：指定通道编号，获得该通道所有的最终话单格式名
//           和最终话单浏览格式名
//
// 输入参数：
//      nChlID：指定的通道的ID
//
// 输出参数：
//          FinalFmts：获得的最终话单格式名列表
// FinalBrowseFormats：获得的最终话单浏览格式名列表
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::GetChlFormats(const UINT4 nChlID, LIST<STRING>& FinalFmts,
                                LIST<STRING>& FinalBrowseFormats)  const
{
    //初始化输出参数
    FinalFmts.clear();
    FinalBrowseFormats.clear();

    //确保输入参数正确
    if(nChlID == 0)
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if(m_pChannels == NULL)
    {
        return -1;
    }

    //查找指定的通道
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        //如果找到指定的通道
        return it->second->GetChlFormats(FinalFmts, FinalBrowseFormats);
    }

    TRACE(MTS_FORMAT, S_FMT_CHLNAME_NOT_FOUND, 
          nChlID, m_uAPID);
    return -1;
}
/************ End of The GetChlFormats Defining ****************/


/************ The DelChannel Defining **************************/
// 功能描述：删除指定通道
//
// 输入参数：
//      nChlID：指定的通道的ID
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::DelChannel(UINT4 nChlID)
{
    //确保输入参数正确
    if(nChlID == 0)
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if(m_pChannels == NULL)
    {
        return -1;
    }

    //查找指定的通道
    CHL_MAP::iterator it = m_pChannels->find(nChlID);
    if(it != m_pChannels->end())
    {
        //如果找到指定的通道
        CChannel* pChl = it->second;
        if(pChl->Delete() != 0)
        {
            return -1;
        }

        //从通道对象映射关系中删除指定的通道对象
        delete pChl;
        m_pChannels->erase(it);
        return 0;
    }

    return -1;
}
/************ End of The DelChannel Defining *******************/


/************ The AddOrgFmtNames Defining **********************/
// 功能描述：追加该接入点的原始话单格式名
//
// 输入参数：
//     nCount：格式名个数
//  szFormats：格式名数组，个数由nCount指定
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::AddOrgFmtNames(UINT4 nCount, const char* szFormats[])
{
    //确保输入参数的正确性
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

    //确保相应成员变量不能为空
    if(m_pOrigFmtMaps == NULL)
    {
        m_pOrigFmtMaps = new FMT_MAP;
    }

    SFmtMap* pFmp = NULL;
    CFormat* pFmt = NULL;
    FMT_MAP::iterator it;
    for(int j = 0; j < nCount; j++)
    {
        //判断格式名列表中是否已经存在当前的格式名
        it = m_pOrigFmtMaps->find(szFormats[j]);
        if(it != m_pOrigFmtMaps->end())
        {
            //如果已经存在，则忽略当前格式名
            //return -1;
            continue;
        }

        //创建新的浏览格式对应关系
        pFmp = new SFmtMap;
        pFmp->sFmtName = szFormats[j];

        //获取格式名对应的格式对象
        pFmt = m_pFmtLib->GetFormat(pFmp->sFmtName);
        if(pFmt == NULL)
        {
            delete pFmp;
            return -1;
        }

        //将创建的格式对应关系放入格式列表中
        pFmp->pFmt = pFmt;
        (*m_pOrigFmtMaps)[pFmp->sFmtName] = pFmp;
    }

    return 0;
}
/************ End of The AddOrgFmtNames Defining ***************/


/************ The GetOrgFmtNames Defining **********************/
// 功能描述：获得本接入点下所有原始话单格式名
//
// 输入参数：无
//
// 输出参数：
//      Names：本接入点下所有原始话单格式名列表
//       Decs：原始话单格式描述列表
//
//   返回值：返回获得的格式名数目
//
/***************************************************************/
UINT4 CAccessPoint::GetOrgFmtNames(LIST<STRING>& Names, LIST<STRING>& Descs) const
{
    //初始化输出参数
    Names.clear();
    Descs.clear();

    //确保相应成员变量不能为空
    if(m_pOrigFmtMaps == NULL)
    {
        return 0;
    }

    //定义下面要用到的临时变量
    STRING sFmtDesc;
    STRING sSrcFmtName;

    //遍历本接入点的所有原始话单格式列表
    FMT_MAP::iterator it = m_pOrigFmtMaps->begin();
    while(it != m_pOrigFmtMaps->end())
    {
        //获取当前格式名和格式描述
        Names.push_back(it->first);
        it->second->pFmt->GetFormatGen(sFmtDesc, sSrcFmtName);
        Descs.push_back(sFmtDesc);

        it++;
    }

    return Names.size();
}
/************ End of The GetOrgFmtNames Defining ***************/


/************ The Commit Defining ******************************/
// 功能描述：使上一次Commit到本次Commit调用之间所修改的配置信息
//           保存到磁盘中
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::Commit(void)
{
    STRING  sFilePath;                  //存放配置文件全路径
    FILE*   pFile   = NULL;             //配置文件指针
    int     nWriteLen  = 0;             //写文件数据的长度
    BYTE    pBuff[100];                 //在写文件数据时长度不足的地方填写0的缓冲区
    char*   pTmpStr  = (char*)pBuff;    //指向缓冲区的指针
    BYTE    yTmpNum = 0;                //写一个字节长度的整数变量

    
    //获取接入点配置文件全路径
    sFilePath = m_sFmtRootDir + "/";
    sprintf(pTmpStr, "%u.apt", m_uAPID);
    sFilePath += pTmpStr;

    //打开指定的接入点配置文件
    pFile = fopen(sFilePath.c_str(), "wb");
    if(pFile == NULL)
    {
        //如果第一次打开文件失败，则创建该文件的目录
        if(!CreateDeepDir(m_sFmtRootDir.c_str()))
        {
            return -1;  //创建文件目录失败
        }

        //再打开一次文件
        pFile = fopen(sFilePath.c_str(), "wb");
        if(pFile == NULL)
        {
            return -1;  //第二次打开文件失败
        }
    }

    //将接入点名称写入配置文件
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sAPName.length() + 1;
    if(nWriteLen > LEN_AP_NAME)
    {
        nWriteLen = LEN_AP_NAME;  //接入点名称最大长度是16个字节
    }
    memcpy((void*)pBuff, (void*)m_sAPName.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_AP_NAME, 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //将接入点类型写入配置文件
    INT4 nTmp = m_nAPType;
    if(fwrite((void*)&nTmp, sizeof(INT4), 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //将通道个数写入配置文件
    if(m_pChannels == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pChannels->size();  //通道个数占用一个字节
    }
    if(fwrite((void*)&yTmpNum, sizeof(BYTE), 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //将原始话单格式个数写入配置文件
    if(m_pOrigFmtMaps == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pOrigFmtMaps->size();  //原始话单格式个数占用一个字节
    }
    if(fwrite((void*)&yTmpNum, sizeof(BYTE), 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //定义下面需要用到的临时变量
    FMT_MAP::iterator it;
    STRING sFmtName;

    //将每一个原始话单格式名写入配置文件
    if(m_pOrigFmtMaps != NULL)
    {
        it = m_pOrigFmtMaps->begin();
        while(it != m_pOrigFmtMaps->end())
        {
            //将当前格式名写入配置文件
            memset((void*)pBuff, 0, 100);
            sFmtName = it->first;
            nWriteLen = sFmtName.length() + 1;
            if(nWriteLen > LEN_AP_ORIG_FMT_NAME)
            {
                nWriteLen = LEN_AP_ORIG_FMT_NAME;  //格式名称最大长度是16个字节
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

    //将原始单浏览格式个数写入配置文件
    if(m_pOrigBrowseFmtMaps == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pOrigBrowseFmtMaps->size();  //原始单浏览格式个数占用一个字节
    }
    if(fwrite((void*)&yTmpNum, sizeof(BYTE), 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //将每一个原始话单浏览格式名写入配置文件
    if(m_pOrigBrowseFmtMaps != NULL)
    {
        it = m_pOrigBrowseFmtMaps->begin();
        while(it != m_pOrigBrowseFmtMaps->end())
        {
            //将当前格式名写入配置文件
            memset((void*)pBuff, 0, 100);
            sFmtName = it->first;
            nWriteLen = sFmtName.length() + 1;
            if(nWriteLen > LEN_AP_ORIGBRS_FMT_NAME)
            {
                nWriteLen = LEN_AP_ORIGBRS_FMT_NAME;  //格式名称最大长度是16个字节
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

    //关闭格式配置文件
    fclose(pFile);

    //保存本接入点包含的所有通道的配置信息
    if(m_pChannels != NULL)
    {
        //遍历所有通道
        CHL_MAP::iterator it = m_pChannels->begin();
        while(it != m_pChannels->end())
        {
            //保存当前通道配置信息
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
// 功能描述：删除接入点
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CAccessPoint::Delete(void)
{
    //删除本接入点中的所有通道配置信息
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

            //从通道对象映射关系中删除当前的通道对象
            delete pChl;
            it++;
        }

        m_pChannels->clear();
        delete m_pChannels;
        m_pChannels = NULL;
    }

    STRING    sFilePath;           //接入点配置文件全路径变量
    char      szBuff[20];          //临时缓冲区
    char*     pTmpStr  = szBuff;   //指向缓冲区的指针
    
    //获取接入点配置文件全路径
    sFilePath = m_sFmtRootDir + "/";
    sprintf(pTmpStr, "%u.apt", m_uAPID);
    sFilePath += pTmpStr;

    //删除指定的配置文件
    if(remove(sFilePath.c_str()) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The Delete Defining ***********************/

/*
  判断接入点配置文件的保存格式是二进制还是文本格式。
  sAPFile : 接入点配置文件的全路径名。
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
        
        //空行，继续读下一行
        if (strlen(szLine) == 0)
        {
            continue;
        }
        
        STRING sLine = szLine;        

		//通道名配置
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
    FILE*   pAPFile = NULL;    //接入点配置文件指针
    char    szBuff[50];        //读取文件数据的临时缓冲区
    char*   pTmpStr = szBuff;  //指向临时缓冲区指针
    BYTE    yTmpNum = 0;       //读取一个字节的整数临时变量


    //以只读方式打开接入点配置文件
    pAPFile = fopen(sAPFile.c_str(), "rb");
    if(pAPFile == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sAPFile.c_str());
        return -1;
    }

    //读入接入点包含的通道个数
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

        //初始化本通道包含的所有通道，通道ID从1开始，顺序递增
        for(int nChlID = 1; nChlID <= yTmpNum; nChlID++)
        {
            pChl = new CChannel(m_sFmtRootDir, m_uAPID, nChlID, m_pFmtLib);
            if((pChl->Init(m_pFmtLib)) == -1)
            {
                //该通道初始化失败
                delete pChl;
                fclose(pAPFile);

                return -1;
            }
            (*m_pChannels)[nChlID] = pChl;
        }
    }

    //定义下面要用到的临时变量
    SFmtMap* pFmtMap = NULL;

    //读入原始话单格式个数
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
            //读原始话单格式名
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

    //读原始话单浏览格式个数
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
            //读出原始话单浏览格式名
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

    //关闭打开的文件
    fclose(pAPFile);

    return 0;
}

/*
  加载文本格式的接入点配置文件，其具体格式为：
  ChannelNumber   n
  OriginalFormat  formatname1
  OriginalFormat  formatname2
      . . .
  OriginalFormat  formatnameN  
  OriginalBrowseForrmat browseformatname1
  OriginalBrowseForrmat browseformatname2
     . . .
  OriginalBrowseForrmat browseformatnameM
  
  其中，ChannelNumber、OriginalFormat和OriginalBrowseForrmat均为关键词。 
*/
BOOL CAccessPoint::LoadAsciiConfig(const STRING& sAPFile)
{
	FILE*   pFile = NULL;				//接入点配置文件指针
    int     nChlNumber = 0;				//读取一个字节的整数临时变量
	char    szFmtName[1024];
    STRING  sFmtName;

    //以只读方式打开接入点配置文件
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
        //空行，继续读下一行
        if (strlen(szLine) == 0)
        {
            continue;
        }
        
        STRING sLine = szLine;        

        char szTemp[1024];
        
		//通道名配置
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

                //初始化本通道包含的所有通道，通道ID从1开始，顺序递增
                for(int nChlID = 1; nChlID <= nChlNumber; nChlID++)
                {
                    pChl = new CChannel(m_sFmtRootDir, m_uAPID, nChlID, m_pFmtLib);
                    if ((pChl->Init(m_pFmtLib)) == -1)
                    {
                        //该通道初始化失败
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

    //关闭打开的文件
    fclose(pFile);

	return 0;
}
