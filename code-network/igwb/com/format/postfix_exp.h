#if !defined(_FORMAT_POSTFIX_EXP_H_)
#define _FORMAT_POSTFIX_EXP_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CPostfixExp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

/***************The CPostfixExp Class Defining***********************/
// 功能描述：表达式分析类
//
/********************************************************************/
class IGWB_Svc_Export CPostfixExp
{
public:

    //构造函数
    CPostfixExp();

    //析构函数
    virtual ~CPostfixExp();

    //Modified by XDB on 2003.04.01
    //将常量成员函数改为静态成员函数，以便于其它地方共用
    //判断一个字符串属于EPostfixEleType类型中哪一种
    static EPostfixEleType GetDataType(const char* sData);

    //判断一个字符串所属的操作符类型，是几目操作符
    static EOperatorType GetOperatorType(const char* sData);

    //根据输入的操作符op，返回其栈内优先级
    int GetISP(const SOperatorSymbol& op) const;

    //根据输入的操作符op，返回其栈外优先级
    int GetICP(const SOperatorSymbol& op) const;

    //将输入的中缀表达式szInfix转换为后缀表达式，并将转换结果存储到m_PosfixExp中
    virtual int Infix2Postfix(const char* szInfix);

    //按照后缀表达式的运算原则，根据表达式的输入获得表达式的输出
    int  Caculate(const void* szInput, const UINT4 uLen, SVariant& Output);

    //Modified by XDB on 2003.04.01
    //将常量成员函数改为静态成员函数，以便于其它地方共用
    //判断一个字符串是否是数字
    static int IsDigit(const char* sData);

    static int IsHex(const char* szData);

protected:

    //获得一个变量的实际取值
    virtual int GetVarValue(const void* szInput, const char*  szVarName, SVariant& stVar) = 0;
    //计算一个函数输出
    virtual int GetFunReturn(const void* szInput, const char* szFun, SVariant& stVar) = 0;

protected:
    
    //判断一个字符是否是操作符
    int IsOperator(BYTE cChr) const;

    //根据操作数和操作符，得出其计算结果，一目运算
    int Caculating(const SVariant& varX, const char *pOper,
                   SVariant& retVar) const;

    //根据操作数和操作符，得出其计算结果，二目运算
    int Caculating(const SVariant& varX, const SVariant& varY, const char *pOper,
                   SVariant& retVar) const;

protected:

    PostfixExpEleList m_PostfixExp;    //存放后缀表达式的链表
    STACK<SVariant, VECTOR<SVariant> > m_CaculateStack;   //表达式计算时，存放操作数的栈

    STRING  m_strInfix;
    char    m_chOperator[255];
};
/***************End of The CPostfixExp Class Defining***************/

#endif  //!defined(_FORMAT_POSTFIX_EXP_H_)
