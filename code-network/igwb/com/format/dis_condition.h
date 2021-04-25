#if !defined(_FORMAT_DISTRIBUTE_CONDITION_H_)
#define _FORMAT_DISTRIBUTE_CONDITION_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CDisCondition
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormat;
class CCondition;
class CFormatLib;

//格式与分拣条件对应关系结构定义
typedef struct _SrcFmtDisMap
{
	STRING         sFmtName;        //最终话单源格式名
	CFormat*       pFmt;            //对应的格式对象
	STRING         sDisCondition;   //对应的分拣条件，中缀表达式
	CCondition*    pDisCondition;   //作为分拣条件的CCondition对象
} SSrcFmtDisMap, *PSrcFmtDisMap;

//格式与分拣条件对应关系链表类型定义
typedef   LIST<SSrcFmtDisMap*> DIS_LIST;

/****************** The CDisCondition class Defining ****************/
// 功能描述：分拣条件类
//
/********************************************************************/
class CDisCondition
{
public:

    //构造函数
    CDisCondition(CFormatLib *pFmtLib = NULL);

    //析构函数
    ~CDisCondition();
     
    //初始化函数
    int Init(CFormatLib *pFmtLib = NULL);

    //增加一种格式及其对应的分拣条件函数
    int AddDisCondition(const STRING& sFmtName, const STRING& sDisCondition);

    //判断一张话单所符合的分拣条件
    int IsTheDisCondition(const void* pABill, const UINT4 uLen, CFormat*& pSrcFmt);

    //获取指定格式名对应的分拣条件
    int GetDisCondName(const STRING& sFmtName, STRING& sDisCondName) const;

protected:

    DIS_LIST*      m_pFmtDisList;   //格式与分拣条件对应关系
    CFormatLib*    m_pFmtLib;       //格式库对象

};
/****************** End of The CDisCondition class Defining *********/


#endif  //!defined(_FORMAT_DISTRIBUTE_CONDITION_H_)
