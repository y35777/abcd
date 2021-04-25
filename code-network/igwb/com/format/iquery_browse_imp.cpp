/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the IQueryBrowseImp
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
#include "iquery_browse_imp.h"

/************ The IQueryBrowseImp Defining *********************/
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
IQueryBrowseImp::IQueryBrowseImp(CIGWB* pIGWB): m_pIGWB(pIGWB)
{
    //初始化成员变量
    //m_pIGWB = pIGWB;
    assert(m_pIGWB != NULL);
}
/************ End of The IQueryBrowseImp Defining **************/


/************ The ~IQueryBrowseImp Defining ********************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
IQueryBrowseImp::~IQueryBrowseImp()
{
}
/************ End of The ~IQueryBrowseImp Defining *************/


/************ The Init Defining ********************************/
// 功能描述：话单浏览查询接口初始化
//
// 输入参数：
//      nAPID：接入点ID，初始化话单浏览查询接口时，可以针对所有
//             接入点进行初始化（nAPID＝0），也可以只针对一个
//             接入点进行初始化（nAPID非零）
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IQueryBrowseImp::Init(const UINT4 nAPID)
{
    //确保相应成员变量不为空
    if(m_pIGWB == NULL)
    {
        return -1;
    }

    //进行初始化，并返回初始化结果
    return m_pIGWB->Init(nAPID);
}
/************ End of The Init Defining *************************/


/************ The GetAPNames Defining **************************/
// 功能描述：获得格式库配置的所有接入点名称
//
// 输入参数：无
//
// 输出参数：
//    APNames：获得格式库配置的所有接入点名称列表
//
//   返回值：返回接入点个数
//
/***************************************************************/
UINT4 IQueryBrowseImp::GetAPNames(LIST<STRING>& APNames) const
{
    //初始化输出参数
    APNames.clear();

    //确保相应成员变量不为空
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


/************ The GetChlNames Defining *************************/
// 功能描述：获得名为sAPName的接入点的所有通道名称
//
// 输入参数：
//    sAPName：接入点名称
//
// 输出参数：
//   ChlNames：返回的所有通道名称列表
//
//   返回值：返回通道个数
//
/***************************************************************/
UINT4 IQueryBrowseImp::GetChlNames(const STRING& sAPName, LIST<STRING>& ChlNames) const
{
    //初始化输出参数
    ChlNames.clear();

    //确保相应成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //遍历所有接入点
    STRING sTmpAPName;
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点名称是否等于指定接入点名称
        (*it)->GetAPName(sTmpAPName);
        if(!(sAPName.compare(sTmpAPName)))
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //如果找到等于指定接入点名称的接入点对象
    if(pAP != NULL)
    {
        //获取定接接入点的所有通道名称
        return pAP->GetChannelNames(ChlNames);
    }

    //如果没有找到等于指定接入点名称的接入点对象
    return 0;
}
/************ End of The GetChlNames Defining ******************/


/************ The GetOrigBrowseNames Defining ******************/
// 功能描述：获得名称为sAPName的接入点的所有原始话单浏览格式名及
//           对应的格式描述
//
// 输入参数：
//    sAPName：接入点名称
//
// 输出参数：
//      Names：返回的所有原始话单浏览格式名列表
//      Descs：所有原始话单浏览格式的描述列表
//
//   返回值：返回原始话单浏览格式总数
//
/***************************************************************/
UINT4 IQueryBrowseImp::GetOrigBrowseNames(const STRING& sAPName, LIST<STRING>& Names,
                                          LIST<STRING>& Descs) const
{
    //初始化输出参数
    Names.clear();
    Descs.clear();

    //确保相应成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //遍历所有接入点
    STRING sTmpAPName;
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点名称是否等于指定接入点名称
        (*it)->GetAPName(sTmpAPName);
        if(!(sAPName.compare(sTmpAPName)))
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //如果找到等于指定接入点名称的接入点对象
    if(pAP != NULL)
    {
        //获取定接接入点的所有原始话单浏览格式名及对应的格式描述
        return pAP->GetOrigBrowseNames(Names, Descs);
    }

    //如果没有找到等于指定接入点名称的接入点对象
    return 0;
}
/************ End of The GetOrigBrowseNames Defining ***********/


/************ The GetFinalBrowseNames Defining *****************/
// 功能描述：输入一个接入点名称，获得某个通道下所有最终话单浏览
//           格式名及对应的格式描述
//
// 输入参数：
//    sAPName：接入点名称
//   sChlName：通道名称
//
// 输出参数：
//      Names：返回的所有最终话单浏览格式名列表
//      Descs：所有最终话单浏览格式的描述列表
//
//   返回值：返回最终话单浏览格式总数
//
/***************************************************************/
UINT4 IQueryBrowseImp::GetFinalBrowseNames(const STRING& sAPName, const STRING& sChlName,
                                           LIST<STRING>& Names, LIST<STRING>& Descs) const
{

    //初始化输出参数
    Names.clear();
    Descs.clear();

    //确保相应成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //遍历所有接入点
    STRING sTmpAPName;
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点名称是否等于指定接入点名称
        (*it)->GetAPName(sTmpAPName);
        if(!(sAPName.compare(sTmpAPName)))
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //如果找到等于指定接入点名称的接入点对象
    if(pAP != NULL)
    {
        //获取定接接入点、指定通道下的所有最终话单浏览格式名
        //及对应的格式描述
        return pAP->GetFinalBrowseNames(sChlName, Names, Descs);
    }

    //如果没有找到等于指定接入点名称的接入点对象
    return 0;
}
/************ End of The GetFinalBrowseNames Defining **********/


/************ The GetBrowseFmt Defining ************************/
// 功能描述：获得名称为sBrowseName的浏览格式的格式处理对象
//
// 输入参数：
// sBrowseName：浏览格式名
//
// 输出参数：无
//
//   返回值：浏览格式对象，调用者不得释放该指针
//
/***************************************************************/
CFormat* IQueryBrowseImp::GetBrowseFmt(const STRING& sBrowseName)
{
    //确保相应成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return NULL;
    }

    //获取指定格式的格式对象
    CFormat* pFMT = NULL;
    pFMT = m_pIGWB->m_pFmtLib->GetFormat(sBrowseName);

    return pFMT;
}
/************ End of The GetBrowseFmt Defining *****************/
