/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CChannel
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format.h"
#include "dis_condition.h"
#include "format_lib.h"
#include "format_global.h"
#include "channel.h"
#include "../include/toolbox.h"

/************ The CChannel Defining ****************************/
// 功能描述：构造函数
//
// 输入参数：
// sFmtRootDir：格式库根目录路径
//       uAPID：本通道对应的接入点ID
//      uChlID：本通道对应的通道ID
//     pFmtLib：格式库对象指针
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CChannel::CChannel(const STRING& sFmtRootDir, const UINT4 uAPID,
                   const UINT4 uChlID, CFormatLib *pFmtLib):
                   m_sFmtRootDir(sFmtRootDir), m_uAPID(uAPID),
                   m_uChlID(uChlID), m_pFmtLib(pFmtLib)
{
    //根据参数初始化相应成员变量
    //m_sFmtRootDir = sFmtRootDir;
    //m_uAPID = uAPID;
    //m_uChlID = uChlID;
    //m_pFmtLib = (CFormatLib*)pFmtLib;

    //初始化其它指针成员变量
    m_pDisCondition = NULL;
    m_pFinalFmtMaps = NULL;
    m_pFinalBrowseFmtMaps = NULL;

    m_pFinalFmtNameList = NULL;  //Added by ZhengYuqun 2004-01-18 SWPD03392
}
/************ End of The CChannel Defining *********************/


/************ The ~CChannel Defining ***************************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CChannel::~CChannel()
{
    //删除本通道对应的分拣条件对象
    if(m_pDisCondition != NULL)
    {
        delete m_pDisCondition;
        m_pDisCondition = NULL;
    }

    FMT_MAP::iterator it;

    //删除本通道对应的最终话单格式对象列表
    if(m_pFinalFmtMaps != NULL)
    {
        it = m_pFinalFmtMaps->begin();
        while(it != m_pFinalFmtMaps->end())
        {
            delete it->second;  //删除列表中的SFmtMap结构
            it++;
        }
        delete m_pFinalFmtMaps;
        m_pFinalFmtMaps = NULL;
    }

    //删除本通道对应的最终话单浏览格式列表
    if(m_pFinalBrowseFmtMaps != NULL)
    {
        it = m_pFinalBrowseFmtMaps->begin();
        while(it != m_pFinalBrowseFmtMaps->end())
        {
            delete it->second;  //删除列表中的SFmtMap结构
            it++;
        }
        delete m_pFinalBrowseFmtMaps;
        m_pFinalBrowseFmtMaps = NULL;
    }

    //Added by ZhengYuqun 2004-01-18 SWPD03392
    if(m_pFinalFmtNameList != NULL)
    {
        delete m_pFinalFmtNameList;
        m_pFinalFmtNameList = NULL;
    }
}
/************ End of The ~CChannel Defining ********************/


/************ The Init Defining ********************************/
// 功能描述：初始化
//
// 输入参数：
//    pFmtLib：格式库对象，CChannel永远不能释放该指针
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CChannel::Init(CFormatLib *pFmtLib)
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

    //缺省通道，无初始化操作
    if(m_uChlID == 0)
    {
        return 0;
    }

    //载入通道配置
    if(LoadConfig() != 0)
    {
        return -1;
    }

    CFormat* pFmt = NULL;
    FMT_MAP::iterator it;

    //初始化本通道对应的最终话单格式对象列表
    if(m_pFinalFmtMaps != NULL)
    {
        it = m_pFinalFmtMaps->begin();
        while(it != m_pFinalFmtMaps->end())
        {
			STRING sFmtName = (it->second)->sFmtName;
			int nCh = m_uChlID;
            pFmt = m_pFmtLib->GetFormat((it->second)->sFmtName, m_uAPID);
            if(pFmt == NULL)
            {
                TRACE(MTS_FORMAT, S_FMT_FINALBILL_FMT_NOT_FOUND, 
                      m_uAPID, m_uChlID, it->first.c_str());
                return -1;
            }
            (it->second)->pFmt = pFmt;

            it++;
        }
    }

    //初始化本通道对应的最终话单浏览格式列表
    if(m_pFinalBrowseFmtMaps != NULL)
    {
        it = m_pFinalBrowseFmtMaps->begin();
        while(it != m_pFinalBrowseFmtMaps->end())
        {
            pFmt = m_pFmtLib->GetFormat((it->second)->sFmtName, m_uAPID);
            if(pFmt == NULL)
            {
                TRACE(MTS_FORMAT, S_FMT_FINALBILL_BRW_FMT_NOT_FOUND, 
                      m_uAPID, m_uChlID, it->first.c_str());
                return -1;
            }
            (it->second)->pFmt = pFmt;

            it++;
        }
    }

    if(m_pDisCondition != NULL)
    {
        if (m_pDisCondition->Init(m_pFmtLib) != 0)
        {
            return -1;
        }
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The LoadConfig Defining **************************/
// 功能描述：将本通道的配置载入内存
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CChannel::LoadConfig(void)
{
    //获取通道配置文件的全路径，型如：根路径/"ap"+接入点ID/通道ID+".chl"
	char szChlFile[MAX_PATH];
	sprintf(szChlFile, "%s/ap%u/%u.chl", m_sFmtRootDir.c_str(), m_uAPID, m_uChlID);
	
	int nChannelFmt = VerifyFileFmt(szChlFile);
	if (nChannelFmt == FILE_FMT_BINARY)
	{
		return LoadBinaryConfig(szChlFile);
	}
	else
	{
		return LoadAsciiConfig(szChlFile);
	}

}
/************ End of The LoadConfig Defining *******************/

UINT4 CChannel::GetFinalLen() const
{
    UINT4 uLen = 0;

    //确保相应成员变量不能为空
    if(m_pFinalFmtMaps == NULL)
    {
        return 0;
    }

    FMT_MAP::iterator it = m_pFinalFmtMaps->begin();
    uLen = it->second->pFmt->GetFmtLen();
    while(it != m_pFinalFmtMaps->end())
    {
        //如果连续两次调用获得的话单长度不一致，则表示
        //该通道最终话单是变长的，返回0
        if(uLen != it->second->pFmt->GetFmtLen())
        {
            return 0;
        }

        it++;
    }

    return uLen;    
}

/************ The GetChlName Defining **************************/
// 功能描述：获得通道的名称
//
// 输入参数：无
//
// 输出参数：
//   sChlName：获得的通道名称
//
//   返回值：无
//
/***************************************************************/
void CChannel::GetChlName(STRING& sChlName) const
{
    sChlName = m_sChlName;
}
/************ End of The GetChlName Defining *******************/


/************ The GetChlID Defining ****************************/
// 功能描述：获得通道的ID
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：获得的通道ID
//
/***************************************************************/
UINT4 CChannel::GetChlID(void) const
{
    return m_uChlID;
}
/************ End of The GetChlID Defining *********************/


/************ The GetFinalBrowseNames Defining *****************/
// 功能描述：获得本通道所有最终话单浏览格式名
//
// 输入参数：无
//
// 输出参数：
//      Names：获得的所有最终话单浏览格式名列表
//      Descs：格式描述列表
//
//   返回值：返回浏览格式个数
//
/***************************************************************/
UINT4 CChannel::GetFinalBrowseNames(LIST<STRING>& Names, LIST<STRING>& Descs) const
{
    CFormat* pFMT = NULL;  //格式对象指针临时变量
    STRING   sFmtDesc;     //格式描述临时变量
    STRING   sSrcFmtName;  //本格式对应的原格式名临时变量

    //初始化输出参数
    Names.clear();
    Descs.clear();

    //确保相应成员变量不为空
    if(m_pFinalBrowseFmtMaps == NULL)
    {
        return 0;
    }

    //Modified by ZhengYuqun 2004-01-18 SWPD03392
    //遍列本通道中的所有浏览格式
    /*FMT_MAP::iterator it = m_pFinalBrowseFmtMaps->begin();
    while(it != m_pFinalBrowseFmtMaps->end())
    {
        //获取浏览格式名
        Names.push_back(it->first);

        //获取浏览格式描述
        pFMT = it->second->pFmt;
        pFMT->GetFormatGen(sFmtDesc, sSrcFmtName);
        Descs.push_back(sFmtDesc);

        it++;
    }*/

    //先从LIST中得到名字然后再从MAP中得到内容
    if(m_pFinalFmtNameList != NULL)
    {
        FINAL_FMT_NAME_LIST::iterator lst_it = m_pFinalFmtNameList->begin();
        while(lst_it != m_pFinalFmtNameList->end())
        {
            //获取当前格式名和格式描述
            FMT_MAP::iterator it;
            it = m_pFinalBrowseFmtMaps->find(*lst_it);
            if(it != m_pFinalBrowseFmtMaps->end())
            {
                //获取浏览格式名
                Names.push_back(it->first);

                //获取浏览格式描述
                pFMT = it->second->pFmt;
                pFMT->GetFormatGen(sFmtDesc, sSrcFmtName);
                Descs.push_back(sFmtDesc);
            }

            lst_it++;
        }
    }
    //End SWPD03392

    return Names.size();
}
/************ End of The GetFinalBrowseNames Defining **********/


/************ The IsTheChannel Defining ************************/
// 功能描述：判断传入的一张话单pABill数否属于本通道
//
// 输入参数：
//     pABill：待分析的一张话单数据指针
//       uLen：pABill的数据长度
//
// 输出参数：
//   pDestFmt：pABill所符合的最终话单格式对象
//
//   返回值：符合本通道返回0，否则返回-1
//
/***************************************************************/
int CChannel::IsTheChannel(const void* pABill, const UINT4 uLen, CFormat*& pDestFmt)
{
    //初始化输出参数
    pDestFmt = NULL;

    //确保输入的参数正确
    if((pABill == NULL) || (uLen == 0))
    {
        return -1;
    }

    //如果分拣条件对象为空
    if(m_pDisCondition == NULL)
    {
        return -1;
    }

    //判断是否符合本通道的分拣条件
    CFormat* pSrcFmt = NULL;
    int nRet = m_pDisCondition->IsTheDisCondition(pABill, uLen, pSrcFmt);
    if(nRet != 0)
    {
        return -1;  //不符合
    }

    //符合本通道的分拣条件，获取其原格式名
    STRING sFmtName;
    pSrcFmt->GetFmtName(sFmtName);

    //判断本通道的最终话单格式中是否有符合分拣条件获取的原格式
    SFmtMap* pFmtMap = NULL;
    STRING sSrcFmtName;
    FMT_MAP::iterator it = m_pFinalFmtMaps->begin();
    while(it != m_pFinalFmtMaps->end())
    {
        pFmtMap = it->second;
        pFmtMap->pFmt->GetSrcFmtName(sSrcFmtName);
        if(!(sSrcFmtName.compare(sFmtName)))
        {
            //找到符合条件的最终话单格式
            pDestFmt = pFmtMap->pFmt;
            return 0;
        }

        it++;
    }

    //输入的话单不符合本通道格式
    return -1;
}
/************ End of The IsTheChannel Defining *****************/


/************ The SetChlName Defining **************************/
// 功能描述：设置通道名称
//
// 输入参数：
//   sChlName：通道名称
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CChannel::SetChlName(const STRING& sChlName)
{
    m_sChlName = sChlName;
}
/************ End of The SetChlName Defining *******************/


/************ The Create Defining ******************************/
// 功能描述：创建一个新的通道
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CChannel::Create(void)
{
    FILE*   pChlFile = NULL;   //通道配置文件指针
    STRING  sFilePath;         //通道配置文件全路径
    char    szBuff[50];        //临时缓冲区
    char*   pTmpStr = szBuff;  //指向临时缓冲区指针
    int     nWriteLen  = 0;    //写文件数据的长度
    char    szPath[MAX_PATH];  //配置文件全路径（不包括文件名）


    //获取通道配置文件的全路径，型如：根路径/"ap"+接入点ID/通道ID+".chl"
    sFilePath = m_sFmtRootDir + "/ap";
    sprintf(pTmpStr, "%u", m_uAPID);
    sFilePath += pTmpStr;
    strcpy(szPath, sFilePath.c_str());  //获取文件全路径（不包括文件名）
    sprintf(pTmpStr, "/%u.chl", m_uChlID);
    sFilePath += pTmpStr;

    //创建指定通道ID的通道配置文件
    pChlFile = fopen(sFilePath.c_str(), "wb");
    if(pChlFile == NULL)
    {
        //如果第一次打开文件失败，则创建该文件的目录
        if(!CreateDeepDir(szPath))
        {
            return -1;  //创建文件目录失败
        }

        //再打开一次文件
        pChlFile = fopen(sFilePath.c_str(), "wb");
        if(pChlFile == NULL)
        {
            return -1;  //第二次打开文件失败
        }
    }

    //将本通道名称写入配置文件
    memset((void*)szBuff, 0, 50);
    nWriteLen = m_sChlName.length() + 1;
    if(nWriteLen > 16)
    {
        nWriteLen = 16;  //通道名称最大长度是16个字节
    }
    memcpy((void*)szBuff, (void*)m_sChlName.c_str(), nWriteLen);
    if(fwrite((void*)szBuff, 16, 1, pChlFile) != 1)
    {
        fclose(pChlFile);
        return -1;
    }

    //关闭通道配置文件
    fclose(pChlFile);

    return 0;
}
/************ End of The Create Defining ***********************/


/************ The SetDistingCondition Defining *****************/
// 功能描述：设置本通道的针对一种最终话单源格式的分拣条件
//
// 输入参数：
// sDisFormat：本通道的一种最终话单格式的源格式的格式名
//   sDisCond：分拣条件
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CChannel::SetDistingCondition(const STRING& sDisFormat, const STRING& sDisCond)
{
    //确保输入参数正确
    if((sDisFormat.length() == 0) || (sDisCond.length() == 0))
    {
        return -1;
    }

    //确保相应成员变量不能为空
    if(m_pDisCondition == NULL)
    {
        m_pDisCondition = new CDisCondition(m_pFmtLib);
    }

    //增加指定原格式的分拣条件，并返回执行结果
    int nRet = m_pDisCondition->AddDisCondition(sDisFormat, sDisCond);
    if(nRet != 0)
    {
        return -1;
    }

    //初始化增加的分拣条件
    nRet = m_pDisCondition->Init(m_pFmtLib);
    if(nRet != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The SetDistingCondition Defining **********/


/************ The AddChlFormats Defining ***********************/
// 功能描述：以追加的方式设置本通道的最终话单格式名和最终话单浏览格式名
//
// 输入参数：
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
int CChannel::AddChlFormats(UINT4 nFinalFmtCount, const char* szFinalFormats[],
                            UINT4 nFinalBrowseFmtCount, const char* szFinalBrowseFmts[])
{
    int i = 0;  //循环计数器


    //确保输入参数的正确性
    if((nFinalFmtCount == 0) || (szFinalFormats == NULL) || (nFinalBrowseFmtCount == 0)
        || (szFinalBrowseFmts == NULL))
    {
        return -1;
    }
    for(i = 0; i < nFinalFmtCount; i++)
    {
        if(szFinalFormats[i] == NULL)
        {
            return -1;
        }
    }
    for(i = 0; i < nFinalBrowseFmtCount; i++)
    {
        if(szFinalBrowseFmts[i] == NULL)
        {
            return -1;
        }
    }

    //确保相应成员变量不能为空
    if(m_pFinalFmtMaps == NULL)
    {
        m_pFinalFmtMaps = new FMT_MAP;
    }
    if(m_pFinalBrowseFmtMaps == NULL)
    {
        m_pFinalBrowseFmtMaps = new FMT_MAP;
    }

    //定义下面需要用到的临时变量
    SFmtMap* pFmp = NULL;
    CFormat* pFmt = NULL;
    FMT_MAP::iterator it;

    //增加最终话单格式
    for(i = 0; i < nFinalFmtCount; i++)
    {
        //判断最终话单格式名列表中是否已经存在当前的格式名
        it = m_pFinalFmtMaps->find(szFinalFormats[i]);
        if(it != m_pFinalFmtMaps->end())
        {
            //如果已经存在，则忽略当前格式名
            //return -1;
            continue;
        }

        //创建新的最终话单格式对应关系
        pFmp = new SFmtMap;
        pFmp->sFmtName = szFinalFormats[i];

        //获取格式名对应的格式对象
        pFmt = m_pFmtLib->GetFormat(pFmp->sFmtName);
        if(pFmt == NULL)
        {
            delete pFmp;
            return -1;
        }

        //将创建的最终话单格式对应关系放入最终话单格式列表中
        pFmp->pFmt = pFmt;
        (*m_pFinalFmtMaps)[pFmp->sFmtName] = pFmp;
    }

    //增加最终话单浏览格式
    for(i = 0; i < nFinalBrowseFmtCount; i++)
    {
        //判断最终话单浏览格式名列表中是否已经存在当前的格式名
        it = m_pFinalBrowseFmtMaps->find(szFinalBrowseFmts[i]);
        if(it != m_pFinalBrowseFmtMaps->end())
        {
            //如果已经存在，则忽略当前格式名
            //return -1;
            continue;
        }

        //创建新的最终浏览话单格式对应关系
        pFmp = new SFmtMap;
        pFmp->sFmtName = szFinalBrowseFmts[i];

        //获取格式名对应的格式对象
        pFmt = m_pFmtLib->GetFormat(pFmp->sFmtName);
        if(pFmt == NULL)
        {
            delete pFmp;
            return -1;
        }

        //将创建的最终话单浏览格式对应关系放入最终话单浏览格式列表中
        pFmp->pFmt = pFmt;
        (*m_pFinalBrowseFmtMaps)[pFmp->sFmtName] = pFmp;
    }

    return 0;
}
/************ End of The AddChlFormats Defining ****************/


/************ The GetChlFormats Defining ***********************/
// 功能描述：获得本通道所有的最终话单格式名
//           和最终话单浏览格式名
//
// 输入参数：无
//
// 输出参数：
//          FinalFmts：获得的最终话单格式名列表
// FinalBrowseFormats：获得的最终话单浏览格式名列表
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CChannel::GetChlFormats(LIST<STRING>& FinalFmts, LIST<STRING>& FinalBrowseFormats) const
{
    //初始化输出参数
    FinalFmts.clear();
    FinalBrowseFormats.clear();
    FMT_MAP::iterator it;

    //遍列本通道中的所有最终话单格式
    if(m_pFinalFmtMaps != NULL)
    {
        it = m_pFinalFmtMaps->begin();
        while(it != m_pFinalFmtMaps->end())
        {
            //获取最终话单格式名
            FinalFmts.push_back(it->first);
            it++;
        }
    }

    //遍列本通道中的所有浏览格式
    if(m_pFinalBrowseFmtMaps != NULL)
    {
        it = m_pFinalBrowseFmtMaps->begin();
        while(it != m_pFinalBrowseFmtMaps->end())
        {
            //获取浏览格式名
            FinalBrowseFormats.push_back(it->first);
            it++;
        }
    }

    return 0;
}
/************ End of The GetChlFormats Defining ****************/


/************ The Delete Defining ******************************/
// 功能描述：删除通道
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CChannel::Delete(void)
{
    STRING    sFilePath;           //通道配置文件全路径变量
    char      szBuff[50];         //临时缓冲区
    char*     pTmpStr  = szBuff;   //指向缓冲区的指针

    
    //获取通道配置文件的全路径，型如：根路径/"ap"+接入点ID/通道ID+".chl"
    sFilePath = m_sFmtRootDir + "/ap";
    sprintf(pTmpStr, "%u/%u.chl", m_uAPID, m_uChlID);
    sFilePath += pTmpStr;

    //删除指定的配置文件
    if(remove(sFilePath.c_str()) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The Delete Defining ***********************/


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
int CChannel::Commit(void)
{
    STRING  sFilePath;                  //存放配置文件全路径
    FILE*   pFile   = NULL;             //配置文件指针
    int     nWriteLen  = 0;             //写文件数据的长度
    BYTE    pBuff[100];                 //在写文件数据时长度不足的地方填写0的缓冲区
    char*   pTmpStr  = (char*)pBuff;    //指向缓冲区的指针
    BYTE    yTmpNum = 0;                //写一个字节长度的整数变量
    char    szPath[MAX_PATH];           //配置文件全路径（不包括文件名）


    //获取通道配置文件的全路径，型如：根路径/"ap"+接入点ID/通道ID+".chl"
    sFilePath = m_sFmtRootDir + "/ap";
    sprintf(pTmpStr, "%u", m_uAPID);
    sFilePath += pTmpStr;
    strcpy(szPath, sFilePath.c_str());  //获取文件全路径（不包括文件名）
    sprintf(pTmpStr, "/%u.chl", m_uChlID);
    sFilePath += pTmpStr;

    //打开指定的通道配置文件
    pFile = fopen(sFilePath.c_str(), "wb");
    if(pFile == NULL)
    {
        //如果第一次打开文件失败，则创建该文件的目录
        if(!CreateDeepDir(szPath))
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

    //将通道名称写入配置文件
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sChlName.length() + 1;
    if(nWriteLen > LEN_CHL_NAME)
    {
        nWriteLen = LEN_CHL_NAME;  //通道名称最大长度是16个字节
    }
    memcpy((void*)pBuff, (void*)m_sChlName.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_CHL_NAME, 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //将最终话单格式个数写入配置文件
    if(m_pFinalFmtMaps == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pFinalFmtMaps->size();  //终话单格式个数占用一个字节
    }
    if(fwrite((void*)&yTmpNum, LEN_BYTE, 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //定义下面需要用到的临时变量
    FMT_MAP::iterator it;
    STRING sFmtName;

    //将每一个最终话单格式名及其分拣条件写入配置文件
    if(m_pFinalFmtMaps != NULL)
    {
        it = m_pFinalFmtMaps->begin();
        STRING sDisCondName;
        while(it != m_pFinalFmtMaps->end())
        {
            //将当前格式名写入配置文件
            memset((void*)pBuff, 0, 100);
            sFmtName = it->first;
            nWriteLen = sFmtName.length() + 1;
            if(nWriteLen > LEN_CHL_FINAL_FMT_NAME)
            {
                nWriteLen = LEN_CHL_FINAL_FMT_NAME;  //格式名称最大长度是16个字节
            }
            memcpy((void*)pBuff, (void*)sFmtName.c_str(), nWriteLen);
            if(fwrite((void*)pBuff, LEN_CHL_FINAL_FMT_NAME, 1, pFile) != 1)
            {
                fclose(pFile);
                return -1;
            }

            //将当前格式名对应的分拣条件写入配置文件——>>

            //如果当前通道不存在分拣条件，则写64个空字符
            if(m_pDisCondition == NULL)
            {
                memset((void*)pBuff, 0, 100);
                if(fwrite((void*)pBuff, LEN_CHL_DIS_CONDITION, 1, pFile) != 1)
                {
                    fclose(pFile);
                    return -1;
                }

                it++;
                continue;
            }

            //当前通道存在分拣条件
            memset((void*)pBuff, 0, 100);
            if(m_pDisCondition->GetDisCondName(sFmtName, sDisCondName) != 0)
            {
                //如果当前格式不存在对应的分拣条件，则写64个空字符
                memset((void*)pBuff, 0, 100);
                if(fwrite((void*)pBuff, LEN_CHL_DIS_CONDITION, 1, pFile) != 1)
                {
                    fclose(pFile);
                    return -1;
                }

                it++;
                continue;
            }

            //当前格式存在对应的分拣条件
            nWriteLen = sDisCondName.length() + 1;
            if(nWriteLen > LEN_CHL_DIS_CONDITION)
            {
                nWriteLen = LEN_CHL_DIS_CONDITION;  //分拣条件最大长度是64个字节
            }
            memcpy((void*)pBuff, (void*)sDisCondName.c_str(), nWriteLen);
            if(fwrite((void*)pBuff, LEN_CHL_DIS_CONDITION, 1, pFile) != 1)
            {
                fclose(pFile);
                return -1;
            }

            it++;
        }
    }

    //将最终话单浏览格式个数写入配置文件
    if(m_pFinalBrowseFmtMaps == NULL)
    {
        yTmpNum = 0;
    }
    else
    {
        yTmpNum = m_pFinalBrowseFmtMaps->size();  //终话单浏览格式个数占用一个字节
    }
    if(fwrite((void*)&yTmpNum, LEN_BYTE, 1, pFile) != 1)
    {
        fclose(pFile);
        return -1;
    }

    //将每一个最终话单浏览格式名写入配置文件
    if(m_pFinalBrowseFmtMaps != NULL)
    {
        it = m_pFinalBrowseFmtMaps->begin();
        while(it != m_pFinalBrowseFmtMaps->end())
        {
            //将当前格式名写入配置文件
            memset((void*)pBuff, 0, 100);
            sFmtName = it->first;
            nWriteLen = sFmtName.length() + 1;
            if(nWriteLen > LEN_CHL_FINALBRS_FMT_NAME)
            {
                nWriteLen = LEN_CHL_FINALBRS_FMT_NAME;  //格式名称最大长度是16个字节
            }
            memcpy((void*)pBuff, (void*)sFmtName.c_str(), nWriteLen);
            if(fwrite((void*)pBuff, LEN_CHL_FINALBRS_FMT_NAME, 1, pFile) != 1)
            {
                fclose(pFile);
                return -1;
            }

            it++;
        }
    }

    //关闭格式配置文件
    fclose(pFile);

    return 0;
}
/************ End of The Commit Defining ***********************/

int CChannel::LoadBinaryConfig(const STRING& sChlFile)
{    
	FILE*     pFile = NULL;
    char      szBuff[255];         //读取配置文件信息的缓冲区
    char*     pTmpStr  = szBuff;   //指向缓冲区的指针
    BYTE      yTmpNum = 0;         //读取一个字节整数的临时变量
    SFmtMap*  pFmtMap = NULL;      //读SFmtMap结构的临时变量

     //创建相应成员变量
    if (m_pDisCondition == NULL)
    {
        m_pDisCondition = new CDisCondition;
    }
	
	if (m_pFinalFmtMaps == NULL)
    {
        m_pFinalFmtMaps = new FMT_MAP;
    }

    //创建相应成员变量
    if (m_pFinalBrowseFmtMaps == NULL)
    {
        m_pFinalBrowseFmtMaps = new FMT_MAP;
    }

	//以二进制只读方式打开通道配置文件
    if ((pFile = fopen(sChlFile.c_str(), "rb")) == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sChlFile.c_str());
        return -1;
    }

    //读取通道名称
    if ((fread((void*)pTmpStr, LEN_CHL_NAME, 1, pFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sChlFile.c_str());
        fclose(pFile);
        return -1;
    }
    else
    {
        *(pTmpStr + LEN_CHL_NAME) = '\0';
        m_sChlName = pTmpStr;
    }

    //读通道最终话单格式个数
    if ((fread((void*)&yTmpNum, LEN_BYTE, 1, pFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sChlFile.c_str());
        fclose(pFile);
        return -1;
    }
    if (yTmpNum == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_INVALID_FINALBILL_FMT_NUM, 
              sChlFile.c_str());

        fclose(pFile);
        return -1;
    }

    for (int i = 0; i < yTmpNum; i++)
    {
        //读最终话单格式名
        if((fread((void*)pTmpStr, LEN_CHL_FINAL_FMT_NAME, 1, pFile)) != 1)
        {
            TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                  sChlFile.c_str());
            fclose(pFile);
            return -1;
        }
        else
        {
            *(pTmpStr + LEN_CHL_FINAL_FMT_NAME) = '\0';
        }

        pFmtMap = new SFmtMap;
        pFmtMap->pFmt = NULL;
        pFmtMap->sFmtName = pTmpStr;
        (*m_pFinalFmtMaps)[pFmtMap->sFmtName] = pFmtMap;

        //读格式分拣条件
        if ((fread((void*)pTmpStr, LEN_CHL_DIS_CONDITION, 1, pFile)) != 1)
        {
            TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                  sChlFile.c_str());
            fclose(pFile);
            return -1;
        }
        else
        {
            *(pTmpStr + LEN_CHL_DIS_CONDITION) = '\0';
        }
        if (strlen(pTmpStr) != 0)
        {
            m_pDisCondition->AddDisCondition(pFmtMap->sFmtName, pTmpStr);
        }
    }

	//读通道的浏览格式个数
    if ((fread((void*)&yTmpNum, LEN_BYTE, 1, pFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sChlFile.c_str());
        fclose(pFile);
        return -1;
    }

    //创建相应成员变量
    if (m_pFinalBrowseFmtMaps == NULL)
    {
        m_pFinalBrowseFmtMaps = new FMT_MAP;
    }
    
    //Added by ZhengYuqun 2004-01-18 SWPD03392
    if(m_pFinalFmtNameList == NULL)
    {
        m_pFinalFmtNameList = new FINAL_FMT_NAME_LIST;
    }
    //End SWPD03392    

    if (yTmpNum != 0)
    {
        for(int j = 0; j < yTmpNum; j++)
        {
            //读出最终话单浏览格式名
            if ((fread((void*)pTmpStr, LEN_CHL_FINALBRS_FMT_NAME, 1, pFile)) != 1)
            {
                TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                      sChlFile.c_str());
                fclose(pFile);
                return -1;
            }
            else
            {
                *(pTmpStr + LEN_CHL_FINALBRS_FMT_NAME) = '\0';
            }

            pFmtMap = new SFmtMap;
            pFmtMap->sFmtName = pTmpStr;
            pFmtMap->pFmt = NULL;
            (*m_pFinalBrowseFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
            m_pFinalFmtNameList->push_back(pFmtMap->sFmtName); //Added by ZhengYuqun 2004-01-18 SWPD03392            
        }
    }

    //关闭打开的文件
    fclose(pFile);

    return 0;
}

int CChannel::LoadAsciiConfig(const STRING& sChlFile)
{
    FILE* pFile = NULL;
    SFmtMap*  pFmtMap = NULL;      //读SFmtMap结构的临时变量

     //创建相应成员变量
    if (m_pDisCondition == NULL)
    {
        m_pDisCondition = new CDisCondition;
    }
	
	if (m_pFinalFmtMaps == NULL)
    {
        m_pFinalFmtMaps = new FMT_MAP;
    }

    //创建相应成员变量
    if (m_pFinalBrowseFmtMaps == NULL)
    {
        m_pFinalBrowseFmtMaps = new FMT_MAP;
    }
    
    //Added by ZhengYuqun 2004-01-18 SWPD03392
    if(m_pFinalFmtNameList == NULL)
    {
        m_pFinalFmtNameList = new FINAL_FMT_NAME_LIST;
    }
    //End SWPD03392    

    //以只读方式打开通道配置文件
    if ((pFile = fopen(sChlFile.c_str(), "rt")) == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sChlFile.c_str());
        return -1;
    }

    char szLine[1024];
    char szTemp[1024];
    char szKeyName[1024];
    STRING sLine;

    while (!feof(pFile))
    {
        szLine[0] = '\0';
        if (fgets(szLine, 1024, pFile) == NULL)
        {
            if (ferror(pFile))
            {
                TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                      sChlFile.c_str());
				fclose(pFile);
				return -1;
            }
        }
        
        sLine = szLine;
        StringLRTrim(szLine);
        //空行，继续读下一行
        if (strlen(szLine) == 0)
        {
            continue;
        }
        
        sLine = szLine;

        //通道名配置
        if (sLine.find("ChannelName", 0) == 0)
        {
            char szChlName[128];
            
            sscanf(szLine, "%s %s", szKeyName, szChlName);
            StringLRTrim(szChlName);
            if (strlen(szChlName) == 0)
            {
                TRACE(MTS_FORMAT, S_FMT_FILED_NULL, 
                      szKeyName, sChlFile.c_str());
                fclose(pFile);
                return -1;
            }
            
            szChlName[LEN_CHL_NAME - 1] = '\0';
            m_sChlName = szChlName;
        }
        //通道对应的最终格式配置
        else if (sLine.find("FinalFormat", 0) == 0)
        {
            char szFormatName[128];
            char szDisCondition[128];
            sscanf(szLine, "%s %s %s", szTemp, szFormatName, szDisCondition);
            
            StringLRTrim(szFormatName);
            StringLRTrim(szDisCondition);
            
            pFmtMap = new SFmtMap;
            pFmtMap->pFmt = NULL;
            pFmtMap->sFmtName = szFormatName;
            (*m_pFinalFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
            
            if (strlen(szDisCondition) != 0)
            {
                m_pDisCondition->AddDisCondition(pFmtMap->sFmtName, szDisCondition);
            }
            else
            {
                TRACE(MTS_FORMAT, S_FMT_FILED_NULL, 
                      szTemp, sChlFile.c_str());
                fclose(pFile);
                return -1;
            }
        }
        else if (sLine.find("FinalBrowseFormat", 0) == 0)
        {
            sscanf(szLine, "%s %s", szKeyName, szTemp);
            StringLRTrim(szTemp);
            if (strlen(szTemp) == 0)
            {
                TRACE(MTS_FORMAT, S_FMT_FILED_NULL, 
                    szKeyName, sChlFile.c_str());
                fclose(pFile);
                return -1;
            }
            
            pFmtMap = new SFmtMap;
            pFmtMap->sFmtName = szTemp;
            pFmtMap->pFmt = NULL;
            (*m_pFinalBrowseFmtMaps)[pFmtMap->sFmtName] = pFmtMap;
            m_pFinalFmtNameList->push_back(pFmtMap->sFmtName); //Added by ZhengYuqun 2004-01-18 SWPD03392
        }
    }
    
    //关闭打开的文件
    fclose(pFile);
    
    return 0;
}

int CChannel::VerifyFileFmt(const STRING& sChlFile)
{
	FILE* pFile = NULL;
	if ((pFile = fopen(sChlFile.c_str(), "rt")) == NULL)
	{
		return FILE_FMT_UNKNOWN;
	}
	
	BOOL bChlNameFound     = FALSE;
	BOOL bFinalFmtFound    = FALSE;
	BOOL bFinalBrwFmtFound = FALSE;

	char szLine[4096];
	while (!feof(pFile))
	{
		if (NULL == fgets(szLine, 4096, pFile))
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
		if (sLine.find("ChannelName", 0) == 0)
		{
			bChlNameFound = TRUE;
		}
		else if (sLine.find("FinalFormat", 0) == 0)
		{
			bFinalFmtFound = TRUE;
		}
		else if (sLine.find("FinalBrowseFormat", 0) == 0)
		{
			bFinalBrwFmtFound = TRUE;
		}
	}

	fclose(pFile);
	if (bChlNameFound && bFinalFmtFound && bFinalBrwFmtFound)
	{
		return FILE_FMT_ASCII;
	}
	else
	{
		return FILE_FMT_BINARY;
	}
}
