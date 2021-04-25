/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CPostfixExp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format_global.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "postfix_exp.h"
/************ The CPostfixExp Defining *************************/
// 功能描述：构造函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CPostfixExp::CPostfixExp()
{
    memset(m_chOperator, -1, sizeof(m_chOperator));
    m_chOperator['('] = 0;
    m_chOperator['+'] = 0;
    m_chOperator['-'] = 0;
    m_chOperator['*'] = 0;
    m_chOperator['/'] = 0;
    m_chOperator['%'] = 0;
    m_chOperator['<'] = 0;
    m_chOperator['>'] = 0;
    m_chOperator['='] = 0;
    m_chOperator['!'] = 0;
    m_chOperator['&'] = 0;
    m_chOperator['^'] = 0;
    m_chOperator['|'] = 0;
}
/************ End of The CPostfixExp Defining ******************/


/************ The ~CPostfixExp Defining ************************/
// 功能描述：构造函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CPostfixExp::~CPostfixExp()
{
}
/************ End of The ~CPostfixExp Defining *****************/


/************ The GetDataType Defining *************************/
// 功能描述：判断一个字符串属于EPostfixEleType类型中哪一种
//
// 输入参数：
//      sData：待判断的字符串数据
//
// 输出参数：无
//
//   返回值：待判断字符串的数据类型
//
//Modified by XDB on 2003.04.01
//将常量成员函数改为静态成员函数，以便于其它地方共用
/***************************************************************/
EPostfixEleType CPostfixExp::GetDataType(const char* sData)
{
    int  nLen = 0;  //字符串的长度

    //判断字符串的长度是否为零
    nLen = strlen(sData);
    if(nLen == 0)
    {
        return ET_NOTDEFINE;       //未知类型
    }

    //如果字符串是数字类型
    if(IsDigit(sData) == 0)
    {
        return ET_INT_CONST;       //整型常量
    }
    
    //如果字符串是十六进制数字类型
    if(IsHex(sData) == 0)
    {
        return ET_HEX_CONST;   //十六进制
    }


    //在字符串中查找左右括号
    const char* pPosStart = strchr(sData, '(');
    const char* pPosEnd   = strchr(sData, ')');

    //如果字符串中同时存在左右括号
    if((pPosStart != NULL) && (pPosEnd != NULL))
    {
        return ET_FUN;            //函数类型
    }

    //如果字符串中只包含左括号或右括号
    else if(   ((pPosStart != NULL) && (pPosEnd == NULL)) 
            || ((pPosStart == NULL) && (pPosEnd != NULL)))
    {
        return ET_NOTDEFINE;      //未知类型
    }

    //如果字符串的长度等于3，且第一个字符和第三个字符都是单引号
    if((nLen == 3) && (sData[0] == '\'') && (sData[2] == '\''))
    {
        return ET_UNSIGNED_CHAR_CONST;        //字符常量
    }

    //如果字符串的长度大于1（包括空字符串），且第一个和最后一个字符是'@'符号或双引号，
    //'@'代表字符串两边的双引号，是话单台与本系统之间的协议
    if(   (nLen > 1)
       && (   ((sData[0] == '@')  && (sData[nLen-1] == '@')) 
           || ((sData[0] == '\"') && (sData[nLen-1] == '\"'))))
    {
        return ET_STRING_CONST;               //字符串常量
    }

    //字符串为单算术运算符
    if((nLen == 1) 
       && ((sData[0] == '+')
           || (sData[0] == '-')
           || (sData[0] == '*')
           || (sData[0] == '/')
           || (sData[0] == '%')
           || (sData[0] == '^')
           || (sData[0] == '|')
           || (sData[0] == '&')))
    {
        return ET_ARITH_OPERATOR;            //算术运算符
    }

    //字符串为双算术运算符
    if(   (nLen == 2) 
       && (   (sData[0] == '>' && sData[1] == '>')
           || (sData[0] == '<' && sData[1] == '<')))
    {
        return ET_ARITH_OPERATOR;           //算术运算符
    }

    //字符串为单逻辑运算符
    if((nLen == 1) && ((sData[0] == '<') || (sData[0] == '>') || (sData[0] == '!')))
    {
        return ET_LOGIC_OPERATOR;          //逻辑运算符
    }

    //字符串为双逻辑运算符
    if(   (nLen == 2) 
       && (   (sData[0] == '|' && sData[1] == '|')
           || (sData[0] == '=' && sData[1] == '=')
           || (sData[0] == '>' && sData[1] == '=')
           || (sData[0] == '<' && sData[1] == '=')
           || (sData[0] == '!' && sData[1] == '=')
           || (sData[0] == '&' && sData[1] == '&')))
    {
        return ET_LOGIC_OPERATOR;          //逻辑运算符
    }

    //默认为变量
    return ET_VARIANT;                     //变量
}
/************ End of The GetDataType Defining ******************/

/************ The GetOperatorType Defining *************************/
// 功能描述：判断一个字符串所属的操作符类型，是几目操作符
//
// 输入参数：
//      sData：待判断的字符串数据
//
// 输出参数：无
//
//   返回值：待判断字符串的数据类型
//
/***************************************************************/
EOperatorType CPostfixExp::GetOperatorType(const char* sData)
{
    int  nLen = 0;  //字符串的长度

    //判断字符串的长度是否为零
    nLen = strlen(sData);
    if(nLen == 0)
    {
        return OT_NOTDEFINE;       //未知类型
    }

    if((nLen == 1) && (sData[0] == '!'))
    {
        return OT_UNARY_OPERATOR;
    }
    
    return OT_BINARY_OPERATOR;
}
/************ End of The GetOperatorType Defining ******************/

/************ The GetISP Defining ******************************/
// 功能描述：获取操作符的栈内优先级
//
// 输入参数：
//      sData：待判断的操作符
//
// 输出参数：无
//
//   返回值：如果出错返回-1，否则返回操作符的栈内优先级
//
/***************************************************************/
int CPostfixExp::GetISP(const SOperatorSymbol& op) const
{
    //在操作符的栈内优先级数组中查找待判断的操作符
    for(int i = 0; i < MAX_OPERATOR_NUM; i++)
    {
        //如果找到，则返回其优先级
        if((g_ISP[i].Left == op.Left) && (g_ISP[i].Right == op.Right))
        {
            return g_ISP[i].nPriority;
        }
    }

    //如果操作符的栈内优先级数组中不存在待判断的操作符
    return -1;
}
/************ End of The GetISP Defining ***********************/


/************ The GetICP Defining ******************************/
// 功能描述：获取操作符的栈外优先级
//
// 输入参数：
//      sData：待判断的操作符
//
// 输出参数：无
//
//   返回值：如果出错返回-1，否则返回操作符的栈外优先级
//
/***************************************************************/
int CPostfixExp::GetICP(const SOperatorSymbol& op) const
{
    //在操作符的栈外优先级数组中查找待判断的操作符
    for(int i = 0; i < MAX_OPERATOR_NUM; i++)
    {
        //如果找到，则返回其优先级
        if((g_ICP[i].Left == op.Left) && (g_ICP[i].Right == op.Right))
        {
            return g_ICP[i].nPriority;
        }
    }

    //如果操作符的栈外优先级数组中不存在待判断的操作符
    return -1;
}
/************ End of The GetICP Defining ***********************/


/************ The Infix2Postfix Defining ***********************/
// 功能描述：将中缀表达式转换为后缀表达式，并将转换结果存储到m_PosfixExp中
//
// 输入参数：
//    szInfix：中缀表达式
//
// 输出参数：无
//
//   返回值：如果出错返回-1，否则返回0
//
//   注  意：为了计算方便，所有的数字类型都以INT4类型存放
//
/***************************************************************/
int CPostfixExp::Infix2Postfix(const char* szInfix)
{
    STACK<SOperatorSymbol>  OpStack;   //存放操作符的栈
    const char *pCurPos  = NULL;       //中缀表达式的当前位置
    const char *pOldPos  = NULL;       //函数元素的起始位置
    int nLen             = 0;          //函数元素的长度

    char szElement[63];                //表达式的单个元素缓冲区
    char *pCurEle = NULL;              //单个元素缓冲区的当前位置
    
    SPostfixExpEle PEE;                //后缀表达式的单个元素
    SOperatorSymbol Oper;              //当前操作符元素
    SOperatorSymbol TmpOp;             //临时操作符元素

    //初始化局部变量
    szElement[0] = 0;
    pCurEle = szElement;
    pCurPos = szInfix;

    //清空原有的后缀表达式
    m_PostfixExp.clear();

    //确保输入的参数不为空
    if(szInfix == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INFIXEXP_EMPTY);
        return -1;
    }
    m_strInfix = szInfix;

    //判断中缀表达式的每一个字符
    while(*pCurPos != '\0')
    {
        //忽略空格字符
        if(*pCurPos == ' ')
        {
            pCurPos++;              
            continue;
        }

        //如果接下来是一个函数元素，即函数以'$'开始，以右括号')'结束
        else if(*pCurPos == '$')
        {
            //清空当前元素缓冲区
            /*szElement[0] = '\0';
            pCurEle = szElement;*/
            //如果当前元素不为空，则表达式出错
            *pCurEle = '\0';
            if(strlen(szElement) != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_FUNITEM_NOT_ALALYSIS,
                      szInfix, szElement);
                m_PostfixExp.clear();
                return -1;
            }

            pCurPos++;
            pOldPos = pCurPos;        //记录函数元素的开始位置

            //找到函数元素的结束位置，以右括号')'结束
            while((*pCurPos != ')') && (*pCurPos != '\0'))
            {
                pCurPos++;
            }

            //如果没有找到右括号')'，则表达式出错
            if(*pCurPos == '\0')
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_FUNITEM_NEED_ENDCHAR,
                      szInfix);
                m_PostfixExp.clear();
                return -1;
            }

            //表达式位置指针移到右括号')'的下一个位置
            pCurPos++;

            //获取函数字符串，从'$'下一个位置开始，到')'结束
            nLen = pCurPos - pOldPos;
            if(nLen < 0 || nLen >= sizeof(PEE.szEleContent))
            {
                //TRACE(MTS_FORMT, "Line = %d, nLen = %d", __LINE__, nLen);
            }
            memcpy((void*)PEE.szEleContent, (const void*)pOldPos, nLen);
            PEE.szEleContent[nLen] = '\0';
            PEE.yEleType = ET_FUN;

            //如果函数字符串中不存在左括号'('，或第一个字符是'('或')'，
            //则表示错误的函数字符串
            if((PEE.szEleContent[0] == '(') || (PEE.szEleContent[0] == ')')
                || (strchr(PEE.szEleContent, '(') == NULL))
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_ERR_FUNITEM,
                      szInfix, PEE.szEleContent);
                m_PostfixExp.clear();
                return -1;
            }
            
            //把函数元素放入后缀表达式
            m_PostfixExp.push_back(PEE);
        }

        //如果接下来是一个字符串，即字符串以'\"'开始和结束,
        //或者以'@'开始和结束
        else if((*pCurPos == '\"') || (*pCurPos == '@'))
        {
            //清空当前元素缓冲区
            /*szElement[0] = '\0';
            pCurEle = szElement;*/
            //如果当前元素不为空，则表达式出错
            *pCurEle = '\0';
            if(strlen(szElement) != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_STRITEM_NOT_ALALYSIS,
                      szInfix, szElement);
                m_PostfixExp.clear();
                return -1;
            }

            char cTmp = *pCurPos;
            pCurPos++;
            pOldPos = pCurPos;        //记录字符串元素的开始位置

            //找到字符串元素的结束位置，以'\"'结束，或者以'@'结束
            if(cTmp == '\"')
            {
                //字符串以'\"'开始和结束
                while((*pCurPos != '\"') && (*pCurPos != '\0'))
                {
                    pCurPos++;
                }
            }
            else
            {
                //字符串以'@'开始和结束
                while((*pCurPos != '@') && (*pCurPos != '\0'))
                {
                   pCurPos++;
                }
            }

            //如果没有找到右括号'\"'或'@'，则表达式出错
            if(*pCurPos == '\0')
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_STRITEM_NEED_ENDCHAR,
                      szInfix);
                m_PostfixExp.clear();
                return -1;
            }
            
            //获取函数字符串，从'\"'或'@'下一个位置开始，到'\"'或'@'的前一个位置结束
            nLen = pCurPos - pOldPos;

            //表达式位置指针移到右括号'\"'或'@'的下一个位置
            pCurPos++;
			
            if(nLen > 63)
			{
				nLen = 63; 
			}
			
            if(nLen < 0 || nLen >= sizeof(PEE.szEleContent))
            {
                //TRACE(MTS_FORMT, "Line = %d, nLen = %d", __LINE__, nLen);
            }
            memcpy((void*)PEE.szEleContent, (const void*)pOldPos, nLen);
            PEE.szEleContent[nLen] = '\0';
            PEE.yEleType = ET_STRING_CONST;
          
            //把字符串元素放入后缀表达式
            m_PostfixExp.push_back(PEE);
        }

        //当前字符是右括号')'
        else if(*pCurPos == ')')
        {
            //把当前元素放入后缀表达式
            *pCurEle = '\0';
            int nLength = strlen(szElement);
            if(nLength != 0)
            {
                if(nLength < 0 || nLength >= sizeof(PEE.szEleContent))
                {
                    //TRACE(MTS_FORMT, "Line = %d, nLen = %d", __LINE__, nLength);
                }
                strcpy(PEE.szEleContent, szElement);
                PEE.yEleType = GetDataType(szElement);
                if(PEE.yEleType == ET_NOTDEFINE)
                {
                    TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_PARA_TYPE,
                          szInfix, szElement);
                    m_PostfixExp.clear();
                    return -1;
                }
                m_PostfixExp.push_back(PEE);
                
                //清空当前元素缓冲区
                szElement[0] = '\0';
                pCurEle = szElement;
            }
                        
            //把操作符栈中'('操作符之前的所有操作符顺序放入后缀表达式
            while(!OpStack.empty())
            {
                Oper = OpStack.top();    //获取栈顶的元素

                //如果栈顶的元素是'('操作符，则停止出栈
                if((Oper.Left == '(') && (Oper.Right == '#'))
                {
                    OpStack.pop();        //删除栈顶元素'('
                    break;
                }

                //如果栈顶元素不是'('，则把栈顶元素放入后缀表达式
                PEE.szEleContent[0] = Oper.Left;
                if(Oper.Right != '#')     //双操作符
                {
                    PEE.szEleContent[1] = Oper.Right;
                    PEE.szEleContent[2] = '\0';
                }
                else                      //单操作符
                {
                    PEE.szEleContent[1] = '\0';
                }
                PEE.yEleType = GetDataType(PEE.szEleContent);
                if(PEE.yEleType == ET_NOTDEFINE)
                {
                    TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_PARA_TYPE,
                          szInfix, PEE.szEleContent);
                    m_PostfixExp.clear();
                    return -1;
                }
                m_PostfixExp.push_back(PEE);
                
                OpStack.pop();           //删除栈顶元素
            }

            pCurPos++;
        }

        //如果当前字符是一个操作符
        else if(IsOperator(*pCurPos) == 0)
        {
            //把当前元素放入后缀表达式
            *pCurEle = '\0';
            if(*pCurPos == '(')
            {
                //左括号的前面不能有操作数
                if((strlen(szElement) != 0))
                {
                    TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_BRTITEM_NOT_ALALYSIS,
                          szInfix, szElement);
                    m_PostfixExp.clear();
                    return -1;
                }
            }
            int nLength = strlen(szElement);
            if(nLength != 0)
            {
                if(nLength < 0 || nLength >= sizeof(PEE.szEleContent))
                {
                    //TRACE(MTS_FORMT, "Line = %d, nLen = %d", __LINE__, nLength);
                }

                strcpy(PEE.szEleContent, szElement);
                PEE.yEleType = GetDataType(szElement);
                if(PEE.yEleType == ET_NOTDEFINE)
                {
                    TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_PARA_TYPE,
                          szInfix, szElement);
                    m_PostfixExp.clear();
                    return -1;
                }
                m_PostfixExp.push_back(PEE);

                //清空当前元素缓冲区
                szElement[0] = '\0';
                pCurEle = szElement;
            }

            Oper.Left = *pCurPos;        //获取当前操作符
            
            //判断下一个字符是不是操作符
            pCurPos++;
            if((IsOperator(*pCurPos) == 0) && (*pCurPos != '(') && (Oper.Left != '('))
            {
                Oper.Right = *pCurPos;   //双操作符
                pCurPos++;
            }
            else
            {
                Oper.Right = '#';        //单操作符
            }

            //把操作符栈中优先级大于当前操作符的操作符放入后缀表达式
            while(!OpStack.empty())
            {
                TmpOp = OpStack.top();  //获取栈顶操作符
                
                //判断栈顶操作符的优先级是否大于当前操作符
                if(GetISP(TmpOp) >= GetICP(Oper))
                {
                    //把栈顶操作符放入后缀表达式
                    PEE.szEleContent[0] = TmpOp.Left;
                    if(TmpOp.Right != '#')
                    {
                        //双操作符
                        PEE.szEleContent[1] = TmpOp.Right;
                        PEE.szEleContent[2] = '\0';
                    }
                    else
                    {
                        //单操作符
                        PEE.szEleContent[1] = '\0';
                    }
                    
                    //如果栈顶操作符是'('，则不放入后缀表达式
                    if(Oper.Left != '(')
                    {                       
                        PEE.yEleType = GetDataType(PEE.szEleContent);
                        if(PEE.yEleType == ET_NOTDEFINE)
                        {
                            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_PARA_TYPE,
                                  szInfix, PEE.szEleContent);
                            m_PostfixExp.clear();
                            return -1;
                        }
                        m_PostfixExp.push_back(PEE);
                    }

                    OpStack.pop();      //删除栈顶元素
                }

                //如果栈顶操作符优先小于当前操作符，则停止出栈
                else
                {
                    break;
                }
            }

            OpStack.push(Oper);        //当前操作符入栈
        }
        
        //当前字符为普通字符
        else
        {
            *pCurEle++ = *pCurPos++;
        }
    }

    //把当前元素放入后缀表达式
    *pCurEle = '\0';
    int nLength = strlen(szElement);
    if(nLength != 0)
    {
        if(nLength < 0 || nLength >= sizeof(PEE.szEleContent))
        {
            //TRACE(MTS_FORMT, "Line = %d, nLen = %d", __LINE__, nLength);
        }

        strcpy(PEE.szEleContent, szElement);
        PEE.yEleType = GetDataType(szElement);
        if(PEE.yEleType == ET_NOTDEFINE)
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_PARA_TYPE,
                  szInfix, szElement);
            m_PostfixExp.clear();
            return -1;
        }
        m_PostfixExp.push_back(PEE);
        
        //清空当前元素缓冲区
        szElement[0] = '\0';
        pCurEle = szElement;
    }

    //顺序把操作符栈中的操作符放入后缀表达式
    while(!OpStack.empty())
    {
        Oper = OpStack.top();         //获取栈顶操作符
        PEE.szEleContent[0] = Oper.Left;
        if(Oper.Right != '#')
        {
            //双操作符
            PEE.szEleContent[1] = Oper.Right;
            PEE.szEleContent[2] = '\0';
        }
        else
        {
            //单操作符
            PEE.szEleContent[1] = '\0';
        }

        //如果栈顶操作符是'('，则不放入后缀表达式
        if(Oper.Left != '(')
        {
            PEE.yEleType = GetDataType(PEE.szEleContent);
            if(PEE.yEleType == ET_NOTDEFINE)
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_PARA_TYPE,
                      szInfix, PEE.szEleContent);
                m_PostfixExp.clear();
                return -1;
            }
            m_PostfixExp.push_back(PEE);
        }

        OpStack.pop();                 //删除栈顶元素
    }

    return 0;
}
/************ End of The Infix2Postfix Defining ****************/


/************ The Caculate Defining ****************************/
// 功能描述：按照后缀表达式的运算原则，根据表达式的输入获得表达式的输出
//
// 输入参数：
//    szInput：输入数据，例如一张话单
//       uLen：输入数据的长度
//
// 输出参数：
//     Output：运算结果
//
//     返回值：如果出错返回-1，否则返回0
//
//   注  意：为了计算方便，所有的数字类型都以INT4类型存放
//
/***************************************************************/
int CPostfixExp::Caculate(const void* szInput, const UINT4 uLen, SVariant& Output)
{
    PostfixExpEleList::iterator item;   //后缀表达式迭代子
    SVariant  retVar;                   //存放操作数变量
    SVariant  tmpVarX;                  //第一个操作数临时变量
    SVariant  tmpVarY;                  //第二个操作数临时变量

    int    nLen ;                    //字符串元素长度变量
    char   *pTmp ;                //字符串元素首指针变量


    //如果后缀表达式为空
    if(m_PostfixExp.size() == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_EXP_EMPTY);
        return -1;
    }

    //清空存放操作数的栈
    while(!m_CaculateStack.empty())
    {
        m_CaculateStack.pop();
    }

    //顺序获取后缀表达式的每一个元素
    item = m_PostfixExp.begin();
    while(item != m_PostfixExp.end())
    {
        switch (item->yEleType)
		{
		case ET_VARIANT:
			{
				if(GetVarValue(szInput, item->szEleContent, retVar) == 0)
				{
					//获取的变量值入栈
					m_CaculateStack.push(retVar);
				}
				//如果获取变量的值出错
				else
				{
                    TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_GET_VAR_VALUE_FAIL,
                          m_strInfix.c_str(), item->szEleContent);
					return -1;
				}
			}
			break;
			
		case ET_FUN:
			{
				if(GetFunReturn(szInput, item->szEleContent, retVar) == 0)
				{
					//获取的函数返回值入栈
					m_CaculateStack.push(retVar);
				}

				//如果获取函数的返回值出错
				else
				{
                    TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_GET_FUN_VALUE_FAIL,
                          m_strInfix.c_str(), 
                          item->szEleContent);
					return -1;
				}
			}
			break;

		case ET_INT_CONST:
			{
				retVar.yVariantType = FT_ULONG;    
				retVar.u4Value = atol(item->szEleContent);
				m_CaculateStack.push(retVar);			
			}
			break;

        //by lx SWPD03800
        case ET_HEX_CONST:
            {
                retVar.yVariantType = FT_HEX;
                sscanf(item->szEleContent, "%x", &(retVar.u4Value));
                m_CaculateStack.push(retVar);
            }
            break;
        //end by lx SWPD03800

        case ET_UNSIGNED_CHAR_CONST:
			{
				retVar.yVariantType = FT_CHAR;

				//如果字符常量包含单引号，如'a'
				if(strlen(item->szEleContent) == 3)
				{
					retVar.yValue = item->szEleContent[1];
				}

				//字符常量不包含单引号
				else
				{
					retVar.yValue = item->szEleContent[0];
				}

				//字符常量入栈
				m_CaculateStack.push(retVar);
			}
			break;
			
		case ET_STRING_CONST:
			{
				retVar.yVariantType = FT_STRING;

				//如果字符串常量包含'@'或双引号，如@example@或"Example"
				//字符串中的'@'字符代表字符串两边的双引号，是话单台与本系统之间的协议
				if((item->szEleContent[0] == '@') || (item->szEleContent[0] == '\"'))
				{
					nLen = strlen(item->szEleContent);
					pTmp = &item->szEleContent[1];
					memcpy((void*)retVar.szValue, (const void*)pTmp, nLen - 2);
					retVar.szValue[nLen - 2] = 0;
				}

				//字符串常量不包含'@'或双引号
				else
				{
					strcpy(retVar.szValue, item->szEleContent);
				}

				//字符串常量入栈
				m_CaculateStack.push(retVar);
			}
			break;

		case ET_ARITH_OPERATOR:
		case ET_LOGIC_OPERATOR:
            {
                switch(GetOperatorType(item->szEleContent)) 
                {
                case OT_UNARY_OPERATOR:
                    {
                        if(!m_CaculateStack.empty())
                        {
                            tmpVarX = m_CaculateStack.top();
                            m_CaculateStack.pop();
                        }
                        
                        //如果栈为空，则表达式计算出错
                        else
                        {
                            return -1;
                        }
                        
                        if((Caculating(tmpVarX, item->szEleContent, retVar)) != -1)
                        {
                            //计算结果入栈
                            m_CaculateStack.push(retVar);
                        }
                        
                        //如果计算出错
                        else
                        {
                            return -1;
                        }
                    }
                    break;
                    
                case OT_BINARY_OPERATOR:
                    {
                        if(!m_CaculateStack.empty())
                        {
                            tmpVarY = m_CaculateStack.top();
                            m_CaculateStack.pop();
                        }
                        
                        //如果栈为空，则表达式计算出错
                        else
                        {
                            return -1;
                        }
                        
                        //如果栈不为空，则获取第一个操作数
                        if(!m_CaculateStack.empty())
                        {
                            tmpVarX = m_CaculateStack.top();
                            m_CaculateStack.pop();
                        }
                        
                        //如果栈为空，则表达式计算出错
                        else
                        {
                            return -1;
                        }
                        
                        //获取当前操作符				
                        //strcpy(szOper, item->szEleContent);
                        
                        //根据当前操作符对两个操作数进行计算
                        if((Caculating(tmpVarX, tmpVarY, item->szEleContent, retVar)) != -1)
                        {
                            //计算结果入栈
                            m_CaculateStack.push(retVar);
                        }
                        
                        //如果计算出错
                        else
                        {
                            return -1;
                        }
                    }
                    break;
                    
                default:
                    return -1;
                    break;
                }
            }	
            break;
			
		default:
			return -1;
			break;
		}

        //获取后缀表达式的下一个元素
        item++;
    }

    //如果栈不为空，则获取计算结果
    if(!m_CaculateStack.empty())
    {
        Output = m_CaculateStack.top();
        m_CaculateStack.pop();
    }

    //如果栈为空，则表达式计算出错
    else
    {
        return -1;
    }
    
    return 0;
}
/************ End of The Caculate Defining *********************/


/************ The IsDigit Defining *****************************/
// 功能描述：判断一个字符串是否是数字
//
// 输入参数：
//      sData：待判断的字符串数据
//
// 输出参数：无
//
//   返回值：如果是数字字符串返回0，否则返回-1
//
//Modified by XDB on 2003.04.01
//将常量成员函数改为静态成员函数，以便于其它地方共用
/***************************************************************/
int CPostfixExp::IsDigit(const char* szData)
{
    //如果字符串长度等于零
    int nLen = strlen(szData);
    if(nLen == 0)
    {
        return -1;
    }    
    
    //去掉前面的空格
    int i = 0;
    while(szData[i] == ' ')
    {
        i++;
    }
    
    //去掉后面的空格
    while(nLen > 0 && szData[nLen - 1] == ' ')
    {
        nLen--;
    }
    
    //判断是否还有剩余的字符
    if(nLen - i <= 0)
    {
        return -1;
    }    
    
    //判断第一个字符是否是'+'或'-'符号
    if(((szData[i] == '+') || (szData[i] == '-')) && (nLen > 1))
    {
        i++;
    }
	
    //判断字符串中的每一个字符
	for(; i < nLen; i++)
    { 
        //如果pChar中任何一个字符大于'9'或小于'0'，则不是数字串
        if((szData[i] > '9') || (szData[i] < '0'))
        {
            return -1;
        }
		
    }
	
    //如果字符串所有字符都是数字
    return 0;
}
/************ End of The IsDigit Defining **********************/


/************ The IsOperator Defining **************************/
// 功能描述：判断一个字符是否是操作符
//
// 输入参数：
//       cChr：待判断的字符
//
// 输出参数：无
//
//   返回值：如果是操作符返回0，否则返回-1
//
/***************************************************************/
int CPostfixExp::IsOperator(BYTE cChr) const
{
    //如果没有找到，表示待判断的字符不是操作符
    return m_chOperator[cChr];
}
/************ End of The IsOperator Defining *******************/

/************ The Caculating Defining **************************/
// 功能描述：根据操作数和操作符，得出其计算结果，一目运算
//
// 输入参数：
//      varX： 操作数
//      pOper：操作符
//
// 输出参数：
//     retVar：计算的结果
//
//   返回值：如果出错返回-1，否则返回0
//
//   注  意：为了计算方便，所有的计算结果都以INT4类型存放
//
/***************************************************************/
int CPostfixExp::Caculating(const SVariant& varX, 
                            const char *pOper, SVariant& retVar) const
{
    if(varX.yVariantType == FT_CHAR)
    {
        retVar.yVariantType = FT_ULONG;
        if(pOper[0] == '!' && pOper[1] == 0)
        {
            retVar.u4Value = !varX.yValue;
        }

        //非法运算
        else
        {
            return -1;
        }
    }
    else if(varX.yVariantType == FT_LONG)
    {
        retVar.yVariantType = FT_ULONG;
        if(pOper[0] == '!' && pOper[1] == 0)
        {
            retVar.u4Value = !varX.n4Value;
        }

        //非法运算
        else
        {
            return -1;
        }
    }
    else if(varX.yVariantType == FT_ULONG)
    {
        retVar.yVariantType = FT_ULONG;
        if(pOper[0] == '!' && pOper[1] == 0)
        {
            retVar.u4Value = !varX.u4Value;
        }

        //非法运算
        else
        {
            return -1;
        }
    }
    else if(varX.yVariantType == FT_HEX)
    {
        retVar.yVariantType = FT_ULONG;
        if(pOper[0] == '!' && pOper[1] == 0)
        {
            retVar.u4Value = !varX.u4Value;
        }

        //非法运算
        else
        {
            return -1;
        }
    }

    //非法运算关系
    else
    {
        return -1;
    }

    return 0;

}
/************ End of The Caculating Defining *******************/

/************ The Caculating Defining **************************/
// 功能描述：根据操作数和操作符，得出其计算结果，二目运算
//
// 输入参数：
//       varX：第一个操作数
//       varY：第二个操作数
//      pOper：操作符
//
// 输出参数：
//     retVar：计算的结果
//
//   返回值：如果出错返回-1，否则返回0
//
//   注  意：为了计算方便，所有的计算结果都以INT4类型存放
//
/***************************************************************/
int CPostfixExp::Caculating(const SVariant& varX, const SVariant& varY,
                            const char *pOper, SVariant& retVar) const
{
        //如果两个操作数都是字符
    if((varX.yVariantType == FT_CHAR) && (varY.yVariantType == FT_CHAR))
    {
        retVar.yVariantType = FT_ULONG;

        //算术运算
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.yVariantType = FT_LONG;
            retVar.n4Value = varX.yValue + varY.yValue;
        }
		else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.yVariantType = FT_LONG;
            retVar.n4Value = varX.yValue - varY.yValue;
        }
        
        //逻辑运算
		else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.yValue == varY.yValue);
        }
		else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.yValue != varY.yValue);
        }
		else if(pOper[0] == '>' && pOper[1] == 0)
        {
            retVar.u4Value = (varX.yValue > varY.yValue);
        }
		else if(pOper[0] == '<' && pOper[1] == 0)
        {
            retVar.u4Value = (varX.yValue < varY.yValue);
        }
		else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.yValue >= varY.yValue);
        }
		else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.yValue <= varY.yValue);
        }

        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //第一个操作数是字符，第二个操作数是整数
    else if((varX.yVariantType == FT_CHAR) && (varY.yVariantType == FT_ULONG))
    {
        retVar.yVariantType = FT_ULONG;

        //算术运算
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.yVariantType = FT_LONG;
            retVar.n4Value = varX.yValue + varY.u4Value;
        }
		else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.yVariantType = FT_LONG;
            retVar.n4Value = varX.yValue - varY.u4Value;
        }

        //逻辑运算
		else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.yValue == varY.u4Value);
        }
		else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.yValue != varY.u4Value);
        }
		else if(pOper[0] == '>' && pOper[1] == 0)
        {
            retVar.u4Value = (varX.yValue > varY.u4Value);
        }
		else if(pOper[0] == '<' && pOper[1] == 0)
        {
            retVar.u4Value = (varX.yValue < varY.u4Value);
        }
		else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.yValue >= varY.u4Value);
        }
		else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.yValue <= varY.u4Value);
        }

        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //第一个操作数是整数，第二个操作数是字符
    else if((varX.yVariantType == FT_ULONG) && (varY.yVariantType == FT_CHAR))
    {
        retVar.yVariantType = FT_ULONG;
                
        //算术运算
		if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.yVariantType = FT_LONG;
            retVar.n4Value = varX.u4Value + varY.yValue;
        }
		else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.yVariantType = FT_LONG;
            retVar.n4Value = varX.u4Value - varY.yValue;
        }
			
        //逻辑运算
		else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value == varY.yValue);
        }
		else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value != varY.yValue);
        }
		else if(pOper[0] == '>' && pOper[1] == 0)
        {
            retVar.u4Value = (varX.u4Value > varY.yValue);
        }
		else if(pOper[0] == '<' && pOper[1] == 0)
        {
            retVar.u4Value = (varX.u4Value < varY.yValue);
        }
		else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value >= varY.yValue);
        }
		else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value <= varY.yValue);
        }

        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //如两个操作数是整数
    else if((varX.yVariantType == FT_ULONG) && (varY.yVariantType == FT_ULONG))
    {
        retVar.yVariantType = FT_ULONG;
        
        //算术运算
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.u4Value = varX.u4Value + varY.u4Value;
        }
        else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.yVariantType = FT_LONG;
            retVar.n4Value = varX.u4Value - varY.u4Value;
        }
        else if(pOper[0] == '*' && pOper[1] == 0)
        {
            retVar.u4Value = varX.u4Value * varY.u4Value;
        }
        else if(pOper[0] == '/' && pOper[1] == 0)
        {
            if(varY.u4Value != 0)
            {
                retVar.u4Value = varX.u4Value / varY.u4Value;
            }
            else
            {
                 //如果除数等于零
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_DIV_BY_ZERO,
                      m_strInfix.c_str());               
                return -1;
            }
        }
		else if(pOper[0] == '%' && pOper[1] == 0)			
        {
            retVar.u4Value = varX.u4Value % varY.u4Value;
        }
		else if(pOper[0] == '&' && pOper[1] == 0)
        {
            retVar.u4Value = varX.u4Value & varY.u4Value;
        }
		else if(pOper[0] == '|' && pOper[1] == 0)			
        {
            retVar.u4Value = varX.u4Value | varY.u4Value;
        }
		else if(pOper[0] == '^' && pOper[1] == 0)			
        {
            retVar.u4Value = varX.u4Value ^ varY.u4Value;
        }
		else if(pOper[0] == '>' && pOper[1] == '>' && pOper[2] == 0)			
        {
            retVar.u4Value = varX.u4Value >> varY.u4Value;
        }
		else if(pOper[0] == '<' && pOper[1] == '<' && pOper[2] == 0)			
        {
            retVar.u4Value = varX.u4Value << varY.u4Value;
        }

        //逻辑运算
		else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)			
        {
            retVar.u4Value = (varX.u4Value == varY.u4Value);
        }
		else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)			
        {
            retVar.u4Value = (varX.u4Value != varY.u4Value);
        }
		else if(pOper[0] == '>' && pOper[1] == 0)			
        {
            retVar.u4Value = (varX.u4Value > varY.u4Value);
        }
		else if(pOper[0] == '<' && pOper[1] == 0)			
        {
            retVar.u4Value = (varX.u4Value < varY.u4Value);
        }
		else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value >= varY.u4Value);
        }
		else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)       
        {
            retVar.u4Value = (varX.u4Value <= varY.u4Value);
        }

        //关系运算
		else if(pOper[0] == '&' && pOper[1] == '&' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value && varY.u4Value);
        }
		else if(pOper[0] == '|' && pOper[1] == '|' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value || varY.u4Value);
        }

        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //如果两个操作数都是字符串
    else if((varX.yVariantType == FT_STRING) && (varY.yVariantType == FT_STRING))
    {             
        retVar.yVariantType = FT_LONG;

        //逻辑运算
        if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            int nYLen = strlen(varY.szValue);      
            if(varY.szValue[0] == '[' && varY.szValue[nYLen - 1] == ']')//如果为子串匹配
            {
                char szTmp[63];
                memset(szTmp, 0 ,63);
                memcpy(szTmp, &(varY.szValue[1]), nYLen - 2);
                if(strstr(varX.szValue, szTmp) != NULL)
                {
                    retVar.u4Value = 1;
                }                       
                else
                {
                    retVar.u4Value = 0;
                }
            }
            else //如果为字冠匹配
            {                
                //去掉开头的空格
                char szTmp[63];
                for(int j = 0; j < 62; j++)
                {
                    if(varX.szValue[j] != ' ')
                    {
                        memcpy(szTmp, &varX.szValue[j], 63-j);
                        break;
                    }
                }                
                
                if(strncmp(szTmp, varY.szValue, nYLen) == 0)
                {
                    retVar.u4Value = 1;
                }
                else
                {
                    retVar.u4Value = 0;
                }
             }  
        }        
		else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            if(strcmp(varX.szValue, varY.szValue) != 0)
            {
                retVar.u4Value = 1;
            }
            else
            {
                retVar.u4Value = 0;
            }
        }
		else if(pOper[0] == '>' && pOper[1] == 0)
        {
            if(strcmp(varX.szValue, varY.szValue) > 0)
            {
                retVar.u4Value = 1;
            }
            else
            {
                retVar.u4Value = 0;
            }
        }
		else if(pOper[0] == '<' && pOper[1] == 0)
        {
            if(strcmp(varX.szValue, varY.szValue) < 0)
            {
                retVar.u4Value = 1;
            }
            else
            {
                retVar.u4Value = 0;
            }
        }
		else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            if(strcmp(varX.szValue, varY.szValue) >= 0)
            {
                retVar.u4Value = 1;
            }
            else
            {
                retVar.u4Value = 0;
            }
        }
		else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            if(strcmp(varX.szValue, varY.szValue) <= 0)
            {
                retVar.u4Value = 1;
            }
            else
            {
                retVar.u4Value = 0;
            }
        }

        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //如果左操作数是字符串，右操作数是十六进制
    else if((varX.yVariantType == FT_STRING) && (varY.yVariantType == FT_HEX))
    {
        retVar.yVariantType = FT_ULONG;
        INT4 uDataX = 0;
        
        //判断左操作数是否为十六进制字符串
        char szTmp[63] ;
        memset(szTmp, 0, sizeof(szTmp));
        int varXLen = strlen(varX.szValue);
        memcpy(szTmp, varX.szValue, varXLen);        
        szTmp[varXLen] = 'H';
        
        if(IsHex(szTmp) != 0)
        {
            return -1;
        }  

        //把左操作数转换成整数
        sscanf(varX.szValue, "%x", &uDataX);
        
        //逻辑运算
        if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (uDataX == varY.u4Value);
        }
        else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (uDataX != varY.u4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == 0)
        {
            retVar.u4Value = (uDataX > varY.u4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == 0)
        {
            retVar.u4Value = (uDataX < varY.u4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (uDataX >= varY.u4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (uDataX <= varY.u4Value);
        }
        
        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }
    
    //如果左操作数是字符串，右操作数是整数
    else if((varX.yVariantType == FT_STRING) && (varY.yVariantType == FT_ULONG))
    {
        retVar.yVariantType = FT_ULONG;
        INT4 uDataX = 0;

        //如果是实数则向下取整
        char szTmp[63];
        strncpy(szTmp, varX.szValue, 63);
        int nPos = 0;
        for(; nPos < strlen(szTmp); nPos++)
        {
            if(szTmp[nPos] == '.')
            {
                break;
            }
        }
        if(nPos < 63)
        {
            szTmp[nPos] = '\0';
        }
        
        //把左操作数转换成整数
        if(IsDigit(szTmp) != 0)
        {
            return -1;  //如果左操作数不是数字字符串
        }
        
        uDataX = atol(szTmp);        

        //逻辑运算
		if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (uDataX == varY.u4Value);
        }
		else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (uDataX != varY.u4Value);
        }
		else if(pOper[0] == '>' && pOper[1] == 0)
        {
            retVar.u4Value = (uDataX > varY.u4Value);
        }
		else if(pOper[0] == '<' && pOper[1] == 0)
        {
            retVar.u4Value = (uDataX < varY.u4Value);
        }
		else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (uDataX >= varY.u4Value);
        }
		else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.u4Value = (uDataX <= varY.u4Value);
        }

        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //第一个操作数是字符，第二个操作数是FT_LONG
    else if((varX.yVariantType == FT_CHAR) && (varY.yVariantType == FT_LONG))
    {
        retVar.yVariantType = FT_LONG;
        
        //算术运算
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.n4Value = varX.yValue + varY.n4Value;
        }
        else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.n4Value = varX.yValue - varY.n4Value;
        }
        
        //逻辑运算
        else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.yValue == varY.n4Value);
        }
        else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.yValue != varY.n4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == 0)
        {
            retVar.n4Value = (varX.yValue > varY.n4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == 0)
        {
            retVar.n4Value = (varX.yValue < varY.n4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.yValue >= varY.n4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.yValue <= varY.n4Value);
        }
        
        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }
    
    //第一个操作数是整数，第二个操作数是字符
    else if((varX.yVariantType == FT_LONG) && (varY.yVariantType == FT_CHAR))
    {
        retVar.yVariantType = FT_LONG;
        
        //算术运算
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value + varY.yValue;
        }
        else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value - varY.yValue;
        }
        
        //逻辑运算
        else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value == varY.yValue);
        }
        else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value != varY.yValue);
        }
        else if(pOper[0] == '>' && pOper[1] == 0)
        {
            retVar.n4Value = (varX.n4Value > varY.yValue);
        }
        else if(pOper[0] == '<' && pOper[1] == 0)
        {
            retVar.n4Value = (varX.n4Value < varY.yValue);
        }
        else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value >= varY.yValue);
        }
        else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value <= varY.yValue);
        }
        
        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }
    
    //如两个操作数是整数
    else if((varX.yVariantType == FT_LONG) && (varY.yVariantType == FT_LONG))
    {
        retVar.yVariantType = FT_LONG;
        
        //算术运算
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value + varY.n4Value;
        }
        else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value - varY.n4Value;
        }
        else if(pOper[0] == '*' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value * varY.n4Value;
        }
        else if(pOper[0] == '/' && pOper[1] == 0)
        {
            if(varY.n4Value != 0)
            {
                retVar.n4Value = varX.n4Value / varY.n4Value;
            }
            else
            {
                //如果除数等于零
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_DIV_BY_ZERO,
                      m_strInfix.c_str());
                return -1;
            }
        }
        else if(pOper[0] == '%' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.n4Value % varY.n4Value;
        }
        else if(pOper[0] == '&' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value & varY.n4Value;
        }
        else if(pOper[0] == '|' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.n4Value | varY.n4Value;
        }
        else if(pOper[0] == '^' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.n4Value ^ varY.n4Value;
        }
        else if(pOper[0] == '>' && pOper[1] == '>' && pOper[2] == 0)			
        {
            retVar.n4Value = varX.n4Value >> varY.n4Value;
        }
        else if(pOper[0] == '<' && pOper[1] == '<' && pOper[2] == 0)			
        {
            retVar.n4Value = varX.n4Value << varY.n4Value;
        }
        
        //逻辑运算
        else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)			
        {
            retVar.n4Value = (varX.n4Value == varY.n4Value);
        }
        else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)			
        {
            retVar.n4Value = (varX.n4Value != varY.n4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == 0)			
        {
            retVar.n4Value = (varX.n4Value > varY.n4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == 0)			
        {
            retVar.n4Value = (varX.n4Value < varY.n4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value >= varY.n4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)       
        {
            retVar.n4Value = (varX.n4Value <= varY.n4Value);
        }
        
        //关系运算
        else if(pOper[0] == '&' && pOper[1] == '&' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value && varY.n4Value);
        }
        else if(pOper[0] == '|' && pOper[1] == '|' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value || varY.n4Value);
        }
        
        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //如果左操作数是字符串，右操作数是整数
    else if((varX.yVariantType == FT_STRING) && (varY.yVariantType == FT_LONG))
    {
        retVar.yVariantType = FT_LONG;
        INT4 uDataX = 0;
        
        //把左操作数转换成整数
        if(IsDigit(varX.szValue) != 0)
        {
            return -1;  //如果左操作数不是数字字符串
        }
        
        uDataX = atol(varX.szValue);
        
        //逻辑运算
        if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (uDataX == varY.n4Value);
        }
        else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (uDataX != varY.n4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == 0)
        {
            retVar.n4Value = (uDataX > varY.n4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == 0)
        {
            retVar.n4Value = (uDataX < varY.n4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (uDataX >= varY.n4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (uDataX <= varY.n4Value);
        }
        
        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //如两个操作数是整数
    else if((varX.yVariantType == FT_ULONG) && (varY.yVariantType == FT_LONG))
    {
        retVar.yVariantType = FT_LONG;
        
        //算术运算
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.n4Value = varX.u4Value + varY.n4Value;
        }
        else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.n4Value = varX.u4Value - varY.n4Value;
        }
        else if(pOper[0] == '*' && pOper[1] == 0)
        {
            retVar.n4Value = varX.u4Value * varY.n4Value;
        }
        else if(pOper[0] == '/' && pOper[1] == 0)
        {
            if(varY.n4Value != 0)
            {
                retVar.n4Value = varX.u4Value / varY.n4Value;
            }
            else
            {
                //如果除数等于零
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                      m_strInfix.c_str(), 
                      pOper);
                return -1;
            }
        }
        else if(pOper[0] == '%' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.u4Value % varY.n4Value;
        }
        else if(pOper[0] == '&' && pOper[1] == 0)
        {
            retVar.n4Value = varX.u4Value & varY.n4Value;
        }
        else if(pOper[0] == '|' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.u4Value | varY.n4Value;
        }
        else if(pOper[0] == '^' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.u4Value ^ varY.n4Value;
        }
        else if(pOper[0] == '>' && pOper[1] == '>' && pOper[2] == 0)			
        {
            retVar.n4Value = varX.u4Value >> varY.n4Value;
        }
        else if(pOper[0] == '<' && pOper[1] == '<' && pOper[2] == 0)			
        {
            retVar.n4Value = varX.u4Value << varY.n4Value;
        }
        
        //逻辑运算
        else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)			
        {
            retVar.n4Value = (varX.u4Value == varY.n4Value);
        }
        else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)			
        {
            retVar.n4Value = (varX.u4Value != varY.n4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == 0)			
        {
            retVar.n4Value = (varX.u4Value > varY.n4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == 0)			
        {
            retVar.n4Value = (varX.u4Value < varY.n4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.u4Value >= varY.n4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)       
        {
            retVar.n4Value = (varX.u4Value <= varY.n4Value);
        }
        
        //关系运算
        else if(pOper[0] == '&' && pOper[1] == '&' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.u4Value && varY.n4Value);
        }
        else if(pOper[0] == '|' && pOper[1] == '|' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.u4Value || varY.n4Value);
        }
        
        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //如两个操作数是整数
    else if((varX.yVariantType == FT_LONG) && (varY.yVariantType == FT_ULONG))
    {
        retVar.yVariantType = FT_LONG;
        
        //算术运算
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value + varY.u4Value;
        }
        else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value - varY.u4Value;
        }
        else if(pOper[0] == '*' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value * varY.u4Value;
        }
        else if(pOper[0] == '/' && pOper[1] == 0)
        {
            if(varY.n4Value != 0)
            {
                retVar.n4Value = varX.n4Value / varY.u4Value;
            }
            else
            {
                //如果除数等于零
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_DIV_BY_ZERO,
                      m_strInfix.c_str());
                return -1;
            }
        }
        else if(pOper[0] == '%' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.n4Value % varY.u4Value;
        }
        else if(pOper[0] == '&' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value & varY.u4Value;
        }
        else if(pOper[0] == '|' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.n4Value | varY.u4Value;
        }
        else if(pOper[0] == '^' && pOper[1] == 0)			
        {
            retVar.n4Value = varX.n4Value ^ varY.u4Value;
        }
        else if(pOper[0] == '>' && pOper[1] == '>' && pOper[2] == 0)			
        {
            retVar.n4Value = varX.n4Value >> varY.u4Value;
        }
        else if(pOper[0] == '<' && pOper[1] == '<' && pOper[2] == 0)			
        {
            retVar.n4Value = varX.n4Value << varY.u4Value;
        }
        
        //逻辑运算
        else if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)			
        {
            retVar.n4Value = (varX.n4Value == varY.u4Value);
        }
        else if(pOper[0] == '!' && pOper[1] == '=' && pOper[2] == 0)			
        {
            retVar.n4Value = (varX.n4Value != varY.u4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == 0)			
        {
            retVar.n4Value = (varX.n4Value > varY.u4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == 0)			
        {
            retVar.n4Value = (varX.n4Value < varY.u4Value);
        }
        else if(pOper[0] == '>' && pOper[1] == '=' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value >= varY.u4Value);
        }
        else if(pOper[0] == '<' && pOper[1] == '=' && pOper[2] == 0)       
        {
            retVar.n4Value = (varX.n4Value <= varY.u4Value);
        }
        
        //关系运算
        else if(pOper[0] == '&' && pOper[1] == '&' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value && varY.u4Value);
        }
        else if(pOper[0] == '|' && pOper[1] == '|' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value || varY.u4Value);
        }
        
        //非法运算
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }
    //非法运算关系
    else
    {
        
        return -1;
    }

    return 0;
}
/************ End of The Caculating Defining *******************/
//by lx SWPD03800
int CPostfixExp::IsHex(const char* szData)
{
    //如果字符串长度等于零
    int nLen = strlen(szData);
    if(nLen == 0)
    {
        return -1;
    }    
    
    //去掉前面的空格
    int i = 0;
    while(szData[i] == ' ')
    {
        i++;
    }
    
    //去掉后面的空格
    while(nLen > 0 && szData[nLen - 1] == ' ')
    {
        nLen--;
    }
    
    //判断是否还有剩余的字符
    if(nLen - i - 1 <= 0)
    {
        return -1;
    }
    
    if(szData[nLen - i - 1] != 'H' && szData[nLen - i - 1] != 'h')
    {
        return -1;
    }
    
    nLen --;
    //判断第一个字符是否是'+'或'-'符号
    if(((szData[i] == '+') || (szData[i] == '-')) && (nLen > 1))
    {
        i++;
    }
    
    //判断字符串中的每一个字符
    for(; i < nLen; i++)
    {         
        if((szData[i] >= '0') && (szData[i] <= '9'))
        {
            continue;
        }
        
        if((szData[i] >= 'a') && (szData[i] <= 'f'))
        {
            continue;
        }
        
        if((szData[i] >= 'A') && (szData[i] <= 'F'))
        {
            continue;
        }
        return -1;
        
    }
    
    //如果字符串所有字符都是数字
    return 0;
}
//end by lx SWPD03800
