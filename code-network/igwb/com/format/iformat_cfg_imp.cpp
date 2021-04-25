/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the IFormatCfgImp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format.h"
#include "igwb.h"
#include "access_point.h"
#include "format_lib.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "iformat_cfg_imp.h"

/************ The IFormatCfgImp Defining ***********************/
// 功能描述：构造函数
//
// 输入参数：
//       pIGWB：CIGWB指针，不能为空
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
IFormatCfgImp::IFormatCfgImp(CIGWB *pIGWB): m_pIGWB(pIGWB)
{
    //确保输入的参数不为空
    //m_pIGWB = pIGWB;
    assert(m_pIGWB != NULL);
}
/************ End of The IFormatCfgImp Defining ****************/


/************ The ~IFormatCfgImp Defining **********************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
IFormatCfgImp::~IFormatCfgImp()
{
}
/************ End of The ~IFormatCfgImp Defining ***************/


/************ The Init Defining ********************************/
// 功能描述：初始化格式配置接口
//
// 输入参数：
//     nAPID：需要初始化的接入点的编号，若nAPID = 0，则表示针对所有
//            的接入点进行初始化。注意： 初始化IFormatCfg接口时，
//            必须针对所有的接入点进行初始化，即nAPID必须为0
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
//   关键点：如果针对格式配置进行初始化，则CIGWB对象的初始化函数
//           int CIGWB::Init(const UINT4 nAPID = 0, const int bCfg = -1)
//           中第二个参数bCfg必须为零，否则为非零
//
/***************************************************************/
int IFormatCfgImp::Init(const UINT4 nAPID)
{
    //确保相应成员变量不为空
    if(m_pIGWB == NULL)
    {
        return -1;
    }

    //进行初始化，并返回初始化结果
    return m_pIGWB->Init(nAPID, 0);
}
/************ End of The Init Defining *************************/


/************ The AddFormat Defining ***************************/
// 功能描述：在格式库中增加一种名字为sFmtName的话单格式
//
// 输入参数：
//   sFmtName：增加的格式库的格式名称
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::AddFormat(const STRING& sFmtName)
{
    //确保输入的参数正确
    if(sFmtName.length() == 0)
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return -1;
    }

    //增加指定的格式，并返回增加成功与否的结果
    return m_pIGWB->m_pFmtLib->AddFormat(sFmtName);
}
/************ End of The AddFormat Defining ********************/


/************ The SetFormatGen Defining ************************/
// 功能描述：指定一种格式名sFmtName，设置或修改其格式描述sFmtDes
//           和对应的源格式名sSrcFmtName
//
// 输入参数：
//    sFmtName：指定修改的格式名称
//     sFmtDes：指定格式的格式描述
// sSrcFmtName：指定格式的源格式名
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::SetFormatGen(const STRING& sFmtName, const STRING& sFmtDes, 
                                const STRING& sSrcFmtName)
{
    //确保输入的参数正确
    if((sFmtName.length() == 0) || (sFmtDes.length() == 0) 
       || (sSrcFmtName.length() == 0))
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
         return -1;
    }

    //获取指定的格式对象指针
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return -1;  //获取指定格式对象指针失败
    }

    //设置指定格式的格式描述和原格式名
    pFmt->SetFormatGen(sFmtDes, sSrcFmtName);

    return 0;
}
/************ End of The SetFormatGen Defining *****************/


/************ The GetFieldsMap Defining ************************/
// 功能描述：获得指定格式名sFmtName的所有的话单域信息MAP指针
//
// 输入参数：
//   sFmtName：指定的格式名
//
// 输出参数：无
//
//   返回值：返回格式名为sFmtName的格式信息
//
/***************************************************************/
const FIELD_MAP* IFormatCfgImp::GetFieldsMap(const STRING& sFmtName)
{
    //确保输入的参数正确
    if(sFmtName.length() == 0)
    {
        return NULL;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return NULL;
    }

    //获取指定的格式对象指针
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return NULL;  //获取指定格式对象指针失败
    }

    //获取指定格式的话单域信息MAP指针
    return pFmt->GetFieldsMap();
}
/************ End of The GetFieldsMap Defining *****************/


/************ The GetFieldsArray Defining **********************/
// 功能描述：获得指定格式名sFmtName的所有的话单域信息SFieldInfo*数组指针
//
// 输入参数：
//   sFmtName：指定的格式名
//
// 输出参数：无
//
//   返回值：返回格式名为sFmtName的格式信息
//
/***************************************************************/
const SFieldInfo** IFormatCfgImp::GetFieldsArray(const STRING& sFmtName)
{
    //确保输入的参数正确
    if(sFmtName.length() == 0)
    {
        return NULL;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return NULL;
    }

    //获取指定的格式对象指针
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return NULL;  //获取指定格式对象指针失败
    }

    //获取指定格式的话单域信息SFieldInfo*数组指针
    return pFmt->GetFieldsArray();
}
/************ End of The GetFieldsArray Defining ***************/


/************ The SetDistingCondition Defining *****************/
// 功能描述：设置指定格式名为sFmtName的格式库的格式识别条件
//
// 输入参数：
//           sFmtName：指定的格式名
//  sDistingCondition：该格式的识别格式，为中缀表达式
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void IFormatCfgImp::SetDistingCondition(const STRING& sFmtName, 
                                        const STRING& sDistingCondition)
{
    //确保输入的参数正确
    if((sFmtName.length() == 0) || (sDistingCondition.length() == 0))
    {
        return;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return;
    }

    //获取指定的格式对象指针
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return;  //获取指定格式对象指针失败
    }

    //设置指定格式的格式识别条件
    pFmt->SetDistingCondition(sDistingCondition);
}
/************ End of The SetDistingCondition Defining **********/


/************ The GetDistingCondition Defining *****************/
// 功能描述：获得指定格式名为sFmtName的格式库的格式识别条件数
//
// 输入参数：
//          sFmtName：指定的格式名
//
// 输出参数：
// sDistingCondition：获得的识别格式，为中缀表达式
//
//   返回值：无
//
/***************************************************************/
void IFormatCfgImp::GetDistingCondition(const STRING& sFmtName, 
                                        STRING& sDistingCondition)
{
    //初始化输出参数
    sDistingCondition = "";

    //确保输入的参数正确
    if(sFmtName.length() == 0)
    {
        return;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return;
    }

    //获取指定的格式对象指针
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return;  //获取指定格式对象指针失败
    }

    //获取指定格式的格式识别条件
    pFmt->GetDistingCondition(sDistingCondition);
}
/************ End of The GetDistingCondition Defining **********/


/************ The DelFormat Defining ***************************/
// 功能描述：删除指定格式名为sFmtName的话单格式
//
// 输入参数：
//   sFmtName：指定的格式名
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::DelFormat(const STRING& sFmtName)
{
    //确保输入的参数正确
    if(sFmtName.length() == 0)
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return -1;
    }

    //删除指定的格式，并返回执行结果
    return m_pIGWB->m_pFmtLib->DelFormat(sFmtName);
}
/************ End of The DelFormat Defining ********************/


/************ The GetFormats Defining **************************/
// 功能描述：获得格式库中所有格式名
//
// 输入参数：无
//
// 输出参数：
//    Formats：获得所有格式名列表
//
//   返回值：格式名个数
//
/***************************************************************/
UINT4 IFormatCfgImp::GetFormats(LIST<STRING>& Formats)
{
    //初始化输出参数
    Formats.clear();

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return 0;
    }

    //获得格式库中所有格式名，并返回格式名个数
    return m_pIGWB->m_pFmtLib->GetFormats(Formats);
}
/************ End of The GetFormats Defining *******************/


/************ The GetFormatGen Defining ************************/
// 功能描述：获得指定格式名的格式库的一般信息，即获得该格式的描述
//           和源格式名
//
// 输入参数：
//    sFmtName：指定的格式名
//
// 输出参数：
//     sFmtDes：该格式的描述
// sSrcFmtName：该格式的源格式名
//
//   返回值：无
//
/***************************************************************/
void IFormatCfgImp::GetFormatGen(const STRING& sFmtName, STRING& sFmtDes, 
                                 STRING& sSrcFmtName)
{
    //初始化输出参数
    sFmtDes = "";
    sSrcFmtName = "";

    //确保输入的参数正确
    if(sFmtName.length() == 0)
    {
        return;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return;
    }

    //获取指定的格式对象指针
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return;  //获取指定格式对象指针失败
    }

    //获取指定格式的格式描述和原格式名
    pFmt->GetFormatGen(sFmtDes, sSrcFmtName);
}
/************ End of The GetFormatGen Defining *****************/


/************ The AddAccessPoint Defining **********************/
// 功能描述：指定接入点的ID、接入点名称，增加一个接入点的格式配置信息
//
// 输入参数：
//      nAPID：指定的接入点ID
//    sAPName：指定的接入点的名称
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::AddAccessPoint(const UINT4 nAPID, const STRING& sAPName)
{
    //确保输入参数正确
    if((nAPID == 0) || (sAPName.length() == 0))
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL) || (m_pIGWB->m_pFmtLib))
    {
        return -1;
    }

    //遍历所有接入点列表
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //接入点已经存在
            return -1;
        }

        it++;
    }

    //接入点不存在，创建新的接入点
    CAccessPoint* pAP = new CAccessPoint(m_pIGWB->m_sFmtRootDir, nAPID,
                                         m_pIGWB->m_pFmtLib);
    pAP->SetAPName(sAPName);
    int nRet = pAP->Create();

    //如果创建接入点配置配置文件失败
    if(nRet != 0)
    {
        delete pAP;
        return -1;
    }

    //如果创建接入点配置文件成功
    m_pIGWB->m_pAPs->push_back(pAP);

    return 0;
}
/************ End of The AddAccessPoint Defining ***************/


/************ The AddOrgBrowseNames Defining *******************/
// 功能描述：指定接入点的ID，追加该接入点的原始话单浏览格式名
//
// 输入参数：
//      nAPID：指定的接入点ID
//     nCount：设置的浏览格式名个数
//  szFormats：浏览格式名数组
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::AddOrgBrowseNames(const UINT4 nAPID, const UINT4 nCount, 
                                     const char* szFormats[])
{
    //确保输入参数正确
    if((nAPID == 0) || (nCount == 0) || (szFormats == NULL))
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

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //遍历所有接入点列表
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //如果找到指定接入点
            return (*it)->AddBrowseNames(nCount, szFormats);
        }

        it++;
    }

    return -1;
}
/************ End of The AddOrgBrowseNames Defining ************/


/************ The GetOrgBrowseNames Defining *******************/
// 功能描述：指定接入点的ID，获得该接入点所有原始话单浏览格式名
//
// 输入参数：
//      nAPID：指定的接入点ID
//
// 输出参数：
//    Formats：获得浏览格式名列表
//
//   返回值：原始话单浏览格式名个数
//
/***************************************************************/
UINT4 IFormatCfgImp::GetOrgBrowseNames(const UINT4 nAPID, LIST<STRING>& Formats)
{
    //初始化输出参数
    Formats.clear();

    //如果输入的通道ID是零，即就是缺省通道
    if(nAPID == 0)
    {
        return 0;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //遍历所有接入点
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点是否等于指定的接入点
        if((*it)->GetAPID() == nAPID)
        {
            LIST<STRING> Descs;  //浏览格式描述列表变量

            //如果找到指定的接入点对象，获取其原始话单浏览格式
            //名称列表和格式描述列表
            return (*it)->GetOrigBrowseNames(Formats, Descs);
        }

        it++;
    }

    //如果没有找到指定接入点对象
    return 0;
}
/************ End of The GetOrgBrowseNames Defining ************/


/************ The AddChannel Defining **************************/
// 功能描述：在指定接入点中增加指定的通道
//
// 输入参数：
//      nAPID：指定的接入点ID
//     nChlID：新增加的通道的ID
//   sChlName：新增加的通道名称
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::AddChannel(const UINT4 nAPID, const UINT4 nChlID , 
                              const STRING& sChlName)
{
    //确保输入参数正确
    if((nAPID == 0) || (nChlID == 0) || (sChlName.length() == 0))
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //遍历所有接入点
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //如果找到指定的接入点，则在此接入点中增加指定的通道
            return (*it)->AddChannel(nChlID,  sChlName);
        }

        it++;
    }

    return -1;
}
/************ End of The AddChannel Defining *******************/


/************ The SetDisCondition Defining *********************/
// 功能描述：指定接入点的ID、通道编号，设置该通道的针对一种最终
//           话单源格式的分拣条件
//
// 输入参数：
//         nAPID：指定的接入点ID
//        nChlID：指定的通道的ID
//    sDisFormat：该通道的一种最终话单格式的源格式的格式名
// sDisCondition：分拣条件
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::SetDisCondition(const UINT4 nAPID, const UINT4 nChlID , 
                                   const STRING& sDisFormat,
                                   const STRING& sDisCondition)
{
    //确保输入参数正确
    if((nAPID == 0) || (nChlID == 0) || (sDisFormat.length() == 0) 
        || (sDisCondition.length() == 0))
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //遍历所有接入点
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //如果找到指定的接入点
            return (*it)->SetDistingCondition(nChlID, sDisFormat, sDisCondition);
        }

        it++;
    }

    return -1;
}
/************ End of The SetDisCondition Defining **************/


/************ The SetChlFormats Defining ***********************/
// 功能描述：指定接入点ID、通道编号，以追加的方式设置该通道的最终
//           话单格式名和最终话单浏览格式名
//
// 输入参数：
//                nAPID：指定的接入点ID
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
int IFormatCfgImp::SetChlFormats(const UINT4 nAPID, const UINT4 nChlID , 
                                 const UINT4 nFinalFmtCount, 
                                 const char* szFinalFormats[], 
                                 const UINT4 nFinalBrowseFmtCount, 
                                 const char* szFinalBrowseFmts[])
{
    //确保输入参数正确
    if((nAPID == 0) || (nChlID == 0) || (nFinalFmtCount == 0) || (szFinalFormats == NULL)
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

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //遍历所有接入点
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //如果找到指定的接入点
            return (*it)->SetChlFormats(nChlID,  nFinalFmtCount, szFinalFormats,
                                        nFinalBrowseFmtCount, szFinalBrowseFmts);
        }

        it++;
    }

    return -1;
}
/************ End of The SetChlFormats Defining ****************/


/************ The GetChlFormats Defining ***********************/
// 功能描述：指定接入点ID、通道编号，获得该通道所有的最终话单格式名
//           和最终话单浏览格式名
//
// 输入参数：
//       nAPID：指定的接入点ID
//      nChlID：指定的通道的ID
//
// 输出参数：
//          FinalFmts：获得的最终话单格式名列表
// FinalBrowseFormats：获得的最终话单浏览格式名列表
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::GetChlFormats(const UINT4 nAPID, const UINT4 nChlID, 
                                 LIST<STRING>& FinalFmts,
                                 LIST<STRING>& FinalBrowseFormats)
{
    //初始化输出参数
    FinalFmts.clear();
    FinalBrowseFormats.clear();

    //确保输入参数正确
    if((nAPID == 0) || (nChlID == 0))
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //遍历所有接入点
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //如果找到指定的接入点
            return (*it)->GetChlFormats(nChlID, FinalFmts, FinalBrowseFormats);
        }

        it++;
    }

    return -1;
}
/************ End of The GetChlFormats Defining ****************/


/************ The DelChannel Defining **************************/
// 功能描述：指定接入点ID、通道编号，删除该通道的配置信息
//
// 输入参数：
//       nAPID：指定的接入点ID
//      nChlID：指定的通道的ID
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::DelChannel(const UINT4 nAPID, const UINT4 nChlID)
{
    //确保输入参数正确
    if((nAPID == 0) || (nChlID == 0))
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //遍历所有接入点
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            return (*it)->DelChannel(nChlID);
        }

        it++;
    }

    return -1;
}
/************ End of The DelChannel Defining *******************/


/************ The GetChannels Defining *************************/
// 功能描述：指定接入点ID，获得该接入点的所有通道名称
//
// 输入参数：
//       nAPID：指定的接入点ID
//
// 输出参数：
//    ChlNames：获得的通道名称列表
//
//   返回值：通道名称个数
//
/***************************************************************/
UINT4 IFormatCfgImp::GetChannels(const UINT4 nAPID, LIST<STRING>& ChlNames)
{
    //初始化输出参数
    ChlNames.clear();

    //确保输入参数正确
    if(nAPID == 0)
    {
        return 0;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            return (*it)->GetChannelNames(ChlNames);
        }

        it++;
    }

    return 0;
}
/************ End of The GetChannels Defining ******************/


/************ The GetAPNames Defining **************************/
// 功能描述：获得格式库所有接入点名称
//
// 输入参数：无
//
// 输出参数：
//    APNames：接入点名称列表
//
//   返回值：接入点名称个数
//
/***************************************************************/
UINT4 IFormatCfgImp::GetAPNames(LIST<STRING>& APNames)
{
    //初始化输出参数
    APNames.clear();

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //遍历所有接入点
    STRING sAPName;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //获取当前接入点名称
        (*it)->GetAPName(sAPName);
        APNames.push_back(sAPName);

        it++;
    }

    return APNames.size();
}
/************ End of The GetAPNames Defining *******************/


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
int IFormatCfgImp::Commit(void)
{
    //提交格式库的所有配置信息
    if(m_pIGWB->m_pFmtLib != NULL)
    {
        if(m_pIGWB->m_pFmtLib->Commit() != 0)
        {
            return -1;
        }
    }

    //提交所有接入点及其包含的通道的配置信息
    if(m_pIGWB->m_pAPs != NULL)
    {
        //遍历所有接入点
        LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
        while(it != m_pIGWB->m_pAPs->end())
        {
            //提交当前接入点及其包含通道的配置信息
            if((*it)->Commit() != 0)
            {
                return -1;
            }

            it++;
        }
    }

    return 0;
}
/************ End of The Commit Defining ***********************/


/************ The AddOrgFmtNames Defining **********************/
// 功能描述：指定接入点的ID，追加该接入点的原始话单格式名
//
// 输入参数：
//      nAPID：指定的接入点ID
//     nCount：设置的格式名个数
//  szFormats：格式名数组
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::AddOrgFmtNames(const UINT4 nAPID, const UINT4 nCount, 
                                  const char* szFormats[])
{
    //确保输入参数正确
    if((nAPID == 0) || (nCount == 0) || (szFormats == NULL))
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

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //遍历所有接入点列表
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //如果找到指定接入点
            return (*it)->AddOrgFmtNames(nCount, szFormats);
        }

        it++;
    }

    return -1;
}
/************ End of The AddOrgFmtNames Defining ***************/


/************ The GetOrgFmtNames Defining **********************/
// 功能描述：指定接入点的ID，获得该接入点所有原始话单格式名
//
// 输入参数：
//      nAPID：指定的接入点ID
//
// 输出参数：
//    Formats：获得格式名列表
//
//   返回值：原始话单格式名个数
//
/***************************************************************/
UINT4 IFormatCfgImp::GetOrgFmtNames(const UINT4 nAPID, LIST<STRING>& Formats)
{
    //初始化输出参数
    Formats.clear();

    //如果输入的通道ID是零，即就是缺省通道
    if(nAPID == 0)
    {
        return 0;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //遍历所有接入点
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点是否等于指定的接入点
        if((*it)->GetAPID() == nAPID)
        {
            LIST<STRING> Descs;  //格式描述列表变量

            //如果找到指定的接入点对象，获取其原始话单格式
            //名称列表和格式描述列表
            return (*it)->GetOrgFmtNames(Formats, Descs);
        }

        it++;
    }

    //如果没有找到指定接入点对象
    return 0;
}
/************ End of The GetOrgFmtNames Defining ***************/


/************ The DelAccessPoint Defining **********************/
// 功能描述：删除指定接入点的配置信息
//
// 输入参数：
//       nAPID：指定的接入点ID
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IFormatCfgImp::DelAccessPoint(UINT4 nAPID)
{
    //确保输入参数正确
    if(nAPID == 0)
    {
        return -1;
    }

    //确保相应的成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //遍历所有接入点
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //如果找到指定的接入点
            if((*it)->Delete() != 0)
            {
                return -1;
            }

            //从接入点对象列表中删除指定的接入点对象
            delete *it;
            m_pIGWB->m_pAPs->erase(it);

            return 0;
        }

        it++;
    }

    return -1;
}
/************ End of The DelAccessPoint Defining ***************/
