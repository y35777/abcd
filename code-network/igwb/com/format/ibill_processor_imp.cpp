/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the IBillProcessorImp
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
#include "../include/export_dll.h"
#include "ibill_processor_imp.h"
#include "../include/toolbox.h"

/************ The IBillProcessorImp Defining *******************/
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
IBillProcessorImp::IBillProcessorImp(CIGWB* pIGWB): m_pIGWB(pIGWB)
{
    //初始化成员变量
    //m_pIGWB = pIGWB;

    //确保输入的参数不为空
    assert(m_pIGWB != NULL);
    
    m_uAPID = -1;
    m_pAP = NULL;
}
/************ End of The IBillProcessorImp Defining ************/


/************ The ~IBillProcessorImp Defining ******************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
IBillProcessorImp::~IBillProcessorImp()
{
}
/************ End of The ~IBillProcessorImp Defining ***********/


/************ The Init Defining ********************************/
// 功能描述：初始化函数
//
// 输入参数：
//       nAPID：接入点ID，不能为0，即话单处理接口只能针对特定的接入点
//              进行初始化，不能对所有接入点进行初始化
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int IBillProcessorImp::Init(const UINT4 nAPID)
{
    //输入的接入点ID不能为零
    if(nAPID == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_BILLPROCESS_INVALID_APID);
        return -1;
    }
    m_uAPID = nAPID;

    //m_pIGWB不能为空
    if(m_pIGWB == NULL)
    {
        return -1;
    }

    //初始化指定接入点
    int nRet = m_pIGWB->Init(nAPID);
    if(nRet != 0)
    {
        TRACE(MTS_FORMAT, S_FMT_BILLPROCESS_INIT_IGWB_FAIL, 
              nAPID);
        return -1;
    }

    //遍历所有接入点
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点ID是否等于本接入点ID
        if((*it)->GetAPID() == m_uAPID)
        {
            m_pAP = (*it);
            return 0;
        }

        it++;
    }

    TRACE(MTS_FORMAT, S_FMT_BILLPROCESS_AP_NOT_FOUND, 
          m_uAPID);

    return -1;  //返回初始化返回结果
}
/************ End of The Init Defining *************************/

void IBillProcessorImp::SpecDistribute(const void* pABill, const UINT4 uLen, 
									VECTOR<UINT4>& uChlID, VECTOR<CFormat*>& pDestFmt)
{
    //初始化输出参数
    pDestFmt.clear();
    uChlID.clear();
	
    //确保输入的话单内容不为空
    if((pABill == NULL) || (uLen == 0))
    {
        return;
    }
	
	m_pAP->SpecDistribute(pABill, uLen, uChlID, pDestFmt);
}


/************ The Distribute Defining **************************/
// 功能描述：输入一张话单及其长度，输出其对应的最终话单通道ID及其
//           对应的格式处理对象
//
// 输入参数：
//     pABill：原始话单指针
//       uLen：pABill所指向的话单长度
//
// 输出参数：
//     uChlID：pABill所属的通道ID
//   pDestFmt：对应的最终话单格式处理指针
//
//   返回值：无
//
/***************************************************************/
void IBillProcessorImp::Distribute(const void* pABill, const UINT4 uLen, 
                                   UINT4& uChlID, CFormat*& pDestFmt)
{
    //初始化输出参数
    pDestFmt = NULL;
    uChlID = 0;

    //确保输入的话单内容不为空
    if((pABill == NULL) || (uLen == 0))
    {
        return;
    }

    /*
    //确保相应成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return;
    }

    //遍历所有接入点
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点ID是否等于本接入点ID
        if((*it)->GetAPID() == m_uAPID)
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //如果找到等于本接入点ID的接入点对象
    if(pAP != NULL)
    {
        //分拣输入的话单数据
        pAP->Distribute(pABill, uLen, uChlID, pDestFmt);
    }*/

    m_pAP->Distribute(pABill, uLen, uChlID, pDestFmt);
}
/************ End of The Distribute Defining *******************/


/************ The GetOrigLen Defining *************************/
// 功能描述：获得当前接入点的原始话单长度
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：当前接入点的原始话单长度
//
/***************************************************************/
UINT4 IBillProcessorImp::GetOrigLen(void) const
{
    /*
    //确保相应成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }
    
    //遍历所有接入点
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点ID是否等于本接入点ID
        if((*it)->GetAPID() == m_uAPID)
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //如果找到等于本接入点ID的接入点对象
    if(pAP != NULL)
    {
        //获取本接入点的原始话单长度
        return pAP->GetOrigLen();
    }

    //如果没有找到等于本接入点ID的接入点对象
    return 0;*/

    return m_pAP->GetOrigLen();
}

UINT4 IBillProcessorImp::GetFinalLen(UINT4 uChlID) const
{
    return m_pAP->GetFinalLen(uChlID);
}
/************ End of The GetOrigLen Defining *******************/


/************ The GetChannelNames Defining *********************/
// 功能描述：获得当前接入点所有通道名称
//
// 输入参数：无
//
// 输出参数：
//   ChlNames：获得通道名列表
//
//   返回值：通道个数（不包含缺省通道0）
//
/***************************************************************/
UINT4 IBillProcessorImp::GetChannelNames(LIST<STRING>& ChlNames) const
{
    //初始化输出参数
    ChlNames.clear();

    /*
    //确保相应成员变量不为空
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //遍历所有接入点
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //判断当前接入点ID是否等于本接入点ID
        if((*it)->GetAPID() == m_uAPID)
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //如果找到等于本接入点ID的接入点对象
    if(pAP != NULL)
    {
        //获取本接入点的所有通道名称
        return pAP->GetChannelNames(ChlNames);
    }

    //如果没有找到等于本接入点ID的接入点对象
    return 0;*/

    return m_pAP->GetChannelNames(ChlNames);
}
/************ End of The GetChannelNames Defining **************/
