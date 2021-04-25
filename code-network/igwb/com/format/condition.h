#if !defined(_FORMAT_CONDITION_H_)
#define _FORMAT_CONDITION_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CCondition
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "postfix_exp.h"

class CFormat;

/****************** The CCondition class Defining *******************/
// 功能描述：一种格式的识别条件、查询条件、通道的分拣条件类
//
/********************************************************************/
class IGWB_Svc_Export CCondition: public CPostfixExp
{
public:

    //构造函数
    CCondition(CFormat* pFmtOwning);
    CCondition(CFormat* pFmtOwning, const STRING& sDisCondition);

    virtual ~CCondition();    //析构函数

    //设置格式标志函数
    void SetFlag(const int bThisFmt);

    //设置查询条件标志函数
    void SetQueryFlag(int nIsQuery);

    int  ParseExpress(const char* const  szExpress, LIST<STRING>& FieldList);
    
    virtual int Infix2Postfix(const char* szInfix);

public:

    //获得一个变量的实际取值，重载基类的虚函数
    virtual int GetVarValue(const void* szInput, const char*  szVarName, SVariant& stVar);

    //计算一个函数的输出值，重载基类的虚函数
    virtual int GetFunReturn(const void* szInput, const char* szFun, SVariant& stVar);

protected:

    CFormat*    m_pFmtOwning;      //本条件所属的格式

    //表示在作条件分析时，传入的数据是否符合本格式，
    //  0：表示输入的数据符合m_pFmtOwning指向的格式，
    // -1：表示输入的数据符合m_pFmtOwning指向的对象的源格式
    int         m_bThisFmt;

    //是否是查询条件的的标志，
    //1：表示查询条件，0：表示不是查询条件
    int         m_nIsQuery;

};
/****************** End of The CCondition class Defining ************/

#endif  //!defined(_FORMAT_CONDITION_H_)
