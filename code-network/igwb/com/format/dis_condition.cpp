/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CDisCondition
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "format.h"
#include "condition.h"
#include "format_lib.h"
#include "dis_condition.h"
#include "../include/toolbox.h"

/************ The CDisCondition Defining ***********************/
// 功能描述：构造函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CDisCondition::CDisCondition(CFormatLib *pFmtLib): m_pFmtLib(pFmtLib)
{
    //初始化成员变量
    //m_pFmtLib = pFmtLib;
    m_pFmtDisList = new DIS_LIST;
}
/************ End of The CDisCondition Defining ****************/


/************ The ~CDisCondition Defining **********************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CDisCondition::~CDisCondition()
{
    DIS_LIST::iterator item;  //格式与分拣条件对应关系迭代子


    if(m_pFmtDisList != NULL)
    {
        //删除格式与分拣条件对应关系中的每一个对象
        item = m_pFmtDisList->begin();
        while(item != m_pFmtDisList->end())
        {
            if((*item)->pDisCondition != NULL)
            {
                //删除条件对象
                delete (*item)->pDisCondition;
            }
            delete *item;  //删除格式与分拣条件对应关系对象

            item++;
        }

        //删除格式与分拣条件对应关系链表对象
        delete m_pFmtDisList;
    }
}
/************ End of The ~CDisCondition Defining ***************/

     
/************ The Init Defining ********************************/
// 功能描述：初始化函数
//
// 输入参数：
//    pFmtLib：格式库对象指针
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CDisCondition::Init(CFormatLib *pFmtLib)
{
    //确保m_pFmtLib不为空
    if((pFmtLib == NULL) && (m_pFmtLib == NULL))
    {
        return -1;
    }
    else if(pFmtLib != NULL)
    {
        m_pFmtLib = pFmtLib;
    }

    //确保相应成员变量不为空
    if(m_pFmtDisList == NULL)
    {
        return -1;
    }

    //定义局部变量
    CFormat *pFmt = NULL;
	CFormat *pSrcFmt = NULL;
    CCondition *pCond = NULL;
    DIS_LIST::iterator item;

    //遍历m_pFmtDisList中所有成员
    item = m_pFmtDisList->begin();
    while(item != m_pFmtDisList->end())
    {
        //设置格式名对应的格式对象指针
        if((*item)->pFmt == NULL)
        {
            pFmt = m_pFmtLib->GetFormat((*item)->sFmtName);
            if(pFmt == NULL)
            {
                TRACE(MTS_FORMAT, S_FMT_FMT_NOT_FOUND_INIT_DISCOND,
                     (*item)->sFmtName.c_str());
                return -1;
            }
            //修改字节序问题(D12711),麦茂识，2002-04-13
			STRING sSrcFmtName;
			pFmt->GetSrcFmtName(sSrcFmtName);
			pSrcFmt = m_pFmtLib->GetFormat(sSrcFmtName);
			if (pSrcFmt == NULL)
			{
                TRACE(MTS_FORMAT, S_FMT_FMT_NOT_FOUND_INIT_DISCOND,
                      sSrcFmtName.c_str());
				return -1;
			}

			(*item)->pFmt     = pSrcFmt;
			(*item)->sFmtName = sSrcFmtName;

             //添加完毕
        }

        //创建条件表达式对应的条件对象
        if((*item)->pDisCondition == NULL)
        {
            pCond = new CCondition(pSrcFmt, (*item)->sDisCondition);
            (*item)->pDisCondition = pCond;
        }

        item++;
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The AddDisCondition Defining *********************/
// 功能描述：增加一种格式及其对应的分拣条件函数
//
// 输入参数：
//      sFmtName：格式名
// sDisCondition：格式分拣条件，中缀表达式
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CDisCondition::AddDisCondition(const STRING& sFmtName, const STRING& sDisCondition)
{    
    DIS_LIST::iterator item;   //格式与分拣条件对应关系迭代子

    //确保输入参数的正确性
    if((sFmtName.length() == 0) || (sDisCondition.length() == 0))
    {
        return -1;
    }

    //确保相应成员变量不为空
    if(m_pFmtDisList == NULL)
    {
        m_pFmtDisList = new DIS_LIST;
    }

    //遍历m_pFmtDisList中所有成员
    item = m_pFmtDisList->begin();
    while(item != m_pFmtDisList->end())
    {
        if(!(sFmtName.compare((*item)->sFmtName)))
        {
            //格式已经存在
            return -1;
        }
        item++;
    }

    //创建格式与分拣条件对应关系对象，并初始化
    SSrcFmtDisMap* pFDMP = new SSrcFmtDisMap;
    pFDMP->pFmt = NULL;
    pFDMP->pDisCondition = NULL;
    pFDMP->sFmtName = sFmtName;
    pFDMP->sDisCondition = sDisCondition;

    //将格式与分拣条件对应关系对象加入链表中
    m_pFmtDisList->push_back(pFDMP);

    return 0;
}
/************ End of The AddDisCondition Defining **************/


/************ The IsTheDisCondition Defining *******************/
// 功能描述：判断一张话单所符合的分拣条件
//
// 输入参数：
//    pABill：话单数据指针
//      uLen：pABill的长度式
//
// 输出参数：
//    pSrcFmt：pABill指向的话单所符合的最终话单源格式对象，调用者不得释放该指针
//
//   返回值：找到对应的格式对象返回0，否则返回-1
//
/***************************************************************/
int CDisCondition::IsTheDisCondition(const void* pABill, const UINT4 uLen,
                                     CFormat*& pSrcFmt)
{
    int bFound = -1;   //是否找到标志
    pSrcFmt = NULL;    //初始化输出参数

    if((pABill == NULL) || (uLen == 0))
    {
        return -1;
    }

    //确保相应成员变量不为空
    if(m_pFmtDisList == NULL)
    {
        return -1;
    }

    //遍历m_pFmtDisList中所有成员
    DIS_LIST::iterator item = m_pFmtDisList->begin();
    while(item != m_pFmtDisList->end())
    {
        //判断输入的话单数据是否符合该格式
        bFound = (*item)->pFmt->IsTheFormat(pABill, uLen);
        if(bFound == 0)  //符合该格式
        {
            break;
        }

        item++;
    }

    //如果找到符合的格式对象
    if(bFound == 0)
    {
        //判断是否符合分拣条件
        SVariant Var;
        int nRet = (*item)->pDisCondition->Caculate(pABill, uLen, Var);
        if(nRet != 0)
        {
            //assert(0);
            return -1;  //表达式计算出错
        }

        //不符分拣条件
        if(Var.n4Value == 0)
        {
            return -1;
        }

        //符合分拣条件
        pSrcFmt = (*item)->pFmt;
        return 0;
    }

    //不符合分拣条件
    return -1;
}
/************ End of The IsTheDisCondition Defining ************/


/************ The GetDisCondName Defining **********************/
// 功能描述：获取指定格式名对应的分拣条件
//
// 输入参数：
//   sFmtName：格式名
//
// 输出参数：
//   sDisName：格式分拣条件，中缀表达式
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CDisCondition::GetDisCondName(const STRING& sFmtName, STRING& sDisCondName) const
{
    //初始化输出参数
    sDisCondName = "";

    //确保输入参数的正确
    if(sFmtName.length() == 0)
    {
        return -1;
    }

    //确保相应成员变量不为空
    if(m_pFmtDisList == NULL)
    {
        return -1;
    }

    //遍历m_pFmtDisList中所有成员
    DIS_LIST::iterator item = m_pFmtDisList->begin();
    while(item != m_pFmtDisList->end())
    {
        if(!(sFmtName.compare((*item)->sFmtName)))
        {
            //如果找到指定的格式名
            sDisCondName = (*item)->sDisCondition;

            return 0;
        }

        item++;
    }

    return -1;
}
/************ End of The GetDisCondName Defining ***************/
