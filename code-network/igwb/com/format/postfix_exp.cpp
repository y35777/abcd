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
// �������������캯��
//
// �����������
//
// �����������
//
//   ����ֵ����
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
// �������������캯��
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CPostfixExp::~CPostfixExp()
{
}
/************ End of The ~CPostfixExp Defining *****************/


/************ The GetDataType Defining *************************/
// �����������ж�һ���ַ�������EPostfixEleType��������һ��
//
// ���������
//      sData�����жϵ��ַ�������
//
// �����������
//
//   ����ֵ�����ж��ַ�������������
//
//Modified by XDB on 2003.04.01
//��������Ա������Ϊ��̬��Ա�������Ա��������ط�����
/***************************************************************/
EPostfixEleType CPostfixExp::GetDataType(const char* sData)
{
    int  nLen = 0;  //�ַ����ĳ���

    //�ж��ַ����ĳ����Ƿ�Ϊ��
    nLen = strlen(sData);
    if(nLen == 0)
    {
        return ET_NOTDEFINE;       //δ֪����
    }

    //����ַ�������������
    if(IsDigit(sData) == 0)
    {
        return ET_INT_CONST;       //���ͳ���
    }
    
    //����ַ�����ʮ��������������
    if(IsHex(sData) == 0)
    {
        return ET_HEX_CONST;   //ʮ������
    }


    //���ַ����в�����������
    const char* pPosStart = strchr(sData, '(');
    const char* pPosEnd   = strchr(sData, ')');

    //����ַ�����ͬʱ������������
    if((pPosStart != NULL) && (pPosEnd != NULL))
    {
        return ET_FUN;            //��������
    }

    //����ַ�����ֻ���������Ż�������
    else if(   ((pPosStart != NULL) && (pPosEnd == NULL)) 
            || ((pPosStart == NULL) && (pPosEnd != NULL)))
    {
        return ET_NOTDEFINE;      //δ֪����
    }

    //����ַ����ĳ��ȵ���3���ҵ�һ���ַ��͵������ַ����ǵ�����
    if((nLen == 3) && (sData[0] == '\'') && (sData[2] == '\''))
    {
        return ET_UNSIGNED_CHAR_CONST;        //�ַ�����
    }

    //����ַ����ĳ��ȴ���1���������ַ��������ҵ�һ�������һ���ַ���'@'���Ż�˫���ţ�
    //'@'�����ַ������ߵ�˫���ţ��ǻ���̨�뱾ϵͳ֮���Э��
    if(   (nLen > 1)
       && (   ((sData[0] == '@')  && (sData[nLen-1] == '@')) 
           || ((sData[0] == '\"') && (sData[nLen-1] == '\"'))))
    {
        return ET_STRING_CONST;               //�ַ�������
    }

    //�ַ���Ϊ�����������
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
        return ET_ARITH_OPERATOR;            //���������
    }

    //�ַ���Ϊ˫���������
    if(   (nLen == 2) 
       && (   (sData[0] == '>' && sData[1] == '>')
           || (sData[0] == '<' && sData[1] == '<')))
    {
        return ET_ARITH_OPERATOR;           //���������
    }

    //�ַ���Ϊ���߼������
    if((nLen == 1) && ((sData[0] == '<') || (sData[0] == '>') || (sData[0] == '!')))
    {
        return ET_LOGIC_OPERATOR;          //�߼������
    }

    //�ַ���Ϊ˫�߼������
    if(   (nLen == 2) 
       && (   (sData[0] == '|' && sData[1] == '|')
           || (sData[0] == '=' && sData[1] == '=')
           || (sData[0] == '>' && sData[1] == '=')
           || (sData[0] == '<' && sData[1] == '=')
           || (sData[0] == '!' && sData[1] == '=')
           || (sData[0] == '&' && sData[1] == '&')))
    {
        return ET_LOGIC_OPERATOR;          //�߼������
    }

    //Ĭ��Ϊ����
    return ET_VARIANT;                     //����
}
/************ End of The GetDataType Defining ******************/

/************ The GetOperatorType Defining *************************/
// �����������ж�һ���ַ��������Ĳ��������ͣ��Ǽ�Ŀ������
//
// ���������
//      sData�����жϵ��ַ�������
//
// �����������
//
//   ����ֵ�����ж��ַ�������������
//
/***************************************************************/
EOperatorType CPostfixExp::GetOperatorType(const char* sData)
{
    int  nLen = 0;  //�ַ����ĳ���

    //�ж��ַ����ĳ����Ƿ�Ϊ��
    nLen = strlen(sData);
    if(nLen == 0)
    {
        return OT_NOTDEFINE;       //δ֪����
    }

    if((nLen == 1) && (sData[0] == '!'))
    {
        return OT_UNARY_OPERATOR;
    }
    
    return OT_BINARY_OPERATOR;
}
/************ End of The GetOperatorType Defining ******************/

/************ The GetISP Defining ******************************/
// ������������ȡ��������ջ�����ȼ�
//
// ���������
//      sData�����жϵĲ�����
//
// �����������
//
//   ����ֵ�����������-1�����򷵻ز�������ջ�����ȼ�
//
/***************************************************************/
int CPostfixExp::GetISP(const SOperatorSymbol& op) const
{
    //�ڲ�������ջ�����ȼ������в��Ҵ��жϵĲ�����
    for(int i = 0; i < MAX_OPERATOR_NUM; i++)
    {
        //����ҵ����򷵻������ȼ�
        if((g_ISP[i].Left == op.Left) && (g_ISP[i].Right == op.Right))
        {
            return g_ISP[i].nPriority;
        }
    }

    //�����������ջ�����ȼ������в����ڴ��жϵĲ�����
    return -1;
}
/************ End of The GetISP Defining ***********************/


/************ The GetICP Defining ******************************/
// ������������ȡ��������ջ�����ȼ�
//
// ���������
//      sData�����жϵĲ�����
//
// �����������
//
//   ����ֵ�����������-1�����򷵻ز�������ջ�����ȼ�
//
/***************************************************************/
int CPostfixExp::GetICP(const SOperatorSymbol& op) const
{
    //�ڲ�������ջ�����ȼ������в��Ҵ��жϵĲ�����
    for(int i = 0; i < MAX_OPERATOR_NUM; i++)
    {
        //����ҵ����򷵻������ȼ�
        if((g_ICP[i].Left == op.Left) && (g_ICP[i].Right == op.Right))
        {
            return g_ICP[i].nPriority;
        }
    }

    //�����������ջ�����ȼ������в����ڴ��жϵĲ�����
    return -1;
}
/************ End of The GetICP Defining ***********************/


/************ The Infix2Postfix Defining ***********************/
// ��������������׺���ʽת��Ϊ��׺���ʽ������ת������洢��m_PosfixExp��
//
// ���������
//    szInfix����׺���ʽ
//
// �����������
//
//   ����ֵ�����������-1�����򷵻�0
//
//   ע  �⣺Ϊ�˼��㷽�㣬���е��������Ͷ���INT4���ʹ��
//
/***************************************************************/
int CPostfixExp::Infix2Postfix(const char* szInfix)
{
    STACK<SOperatorSymbol>  OpStack;   //��Ų�������ջ
    const char *pCurPos  = NULL;       //��׺���ʽ�ĵ�ǰλ��
    const char *pOldPos  = NULL;       //����Ԫ�ص���ʼλ��
    int nLen             = 0;          //����Ԫ�صĳ���

    char szElement[63];                //���ʽ�ĵ���Ԫ�ػ�����
    char *pCurEle = NULL;              //����Ԫ�ػ������ĵ�ǰλ��
    
    SPostfixExpEle PEE;                //��׺���ʽ�ĵ���Ԫ��
    SOperatorSymbol Oper;              //��ǰ������Ԫ��
    SOperatorSymbol TmpOp;             //��ʱ������Ԫ��

    //��ʼ���ֲ�����
    szElement[0] = 0;
    pCurEle = szElement;
    pCurPos = szInfix;

    //���ԭ�еĺ�׺���ʽ
    m_PostfixExp.clear();

    //ȷ������Ĳ�����Ϊ��
    if(szInfix == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INFIXEXP_EMPTY);
        return -1;
    }
    m_strInfix = szInfix;

    //�ж���׺���ʽ��ÿһ���ַ�
    while(*pCurPos != '\0')
    {
        //���Կո��ַ�
        if(*pCurPos == ' ')
        {
            pCurPos++;              
            continue;
        }

        //�����������һ������Ԫ�أ���������'$'��ʼ����������')'����
        else if(*pCurPos == '$')
        {
            //��յ�ǰԪ�ػ�����
            /*szElement[0] = '\0';
            pCurEle = szElement;*/
            //�����ǰԪ�ز�Ϊ�գ�����ʽ����
            *pCurEle = '\0';
            if(strlen(szElement) != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_FUNITEM_NOT_ALALYSIS,
                      szInfix, szElement);
                m_PostfixExp.clear();
                return -1;
            }

            pCurPos++;
            pOldPos = pCurPos;        //��¼����Ԫ�صĿ�ʼλ��

            //�ҵ�����Ԫ�صĽ���λ�ã���������')'����
            while((*pCurPos != ')') && (*pCurPos != '\0'))
            {
                pCurPos++;
            }

            //���û���ҵ�������')'������ʽ����
            if(*pCurPos == '\0')
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_FUNITEM_NEED_ENDCHAR,
                      szInfix);
                m_PostfixExp.clear();
                return -1;
            }

            //���ʽλ��ָ���Ƶ�������')'����һ��λ��
            pCurPos++;

            //��ȡ�����ַ�������'$'��һ��λ�ÿ�ʼ����')'����
            nLen = pCurPos - pOldPos;
            if(nLen < 0 || nLen >= sizeof(PEE.szEleContent))
            {
                //TRACE(MTS_FORMT, "Line = %d, nLen = %d", __LINE__, nLen);
            }
            memcpy((void*)PEE.szEleContent, (const void*)pOldPos, nLen);
            PEE.szEleContent[nLen] = '\0';
            PEE.yEleType = ET_FUN;

            //��������ַ����в�����������'('�����һ���ַ���'('��')'��
            //���ʾ����ĺ����ַ���
            if((PEE.szEleContent[0] == '(') || (PEE.szEleContent[0] == ')')
                || (strchr(PEE.szEleContent, '(') == NULL))
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_ERR_FUNITEM,
                      szInfix, PEE.szEleContent);
                m_PostfixExp.clear();
                return -1;
            }
            
            //�Ѻ���Ԫ�ط����׺���ʽ
            m_PostfixExp.push_back(PEE);
        }

        //�����������һ���ַ��������ַ�����'\"'��ʼ�ͽ���,
        //������'@'��ʼ�ͽ���
        else if((*pCurPos == '\"') || (*pCurPos == '@'))
        {
            //��յ�ǰԪ�ػ�����
            /*szElement[0] = '\0';
            pCurEle = szElement;*/
            //�����ǰԪ�ز�Ϊ�գ�����ʽ����
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
            pOldPos = pCurPos;        //��¼�ַ���Ԫ�صĿ�ʼλ��

            //�ҵ��ַ���Ԫ�صĽ���λ�ã���'\"'������������'@'����
            if(cTmp == '\"')
            {
                //�ַ�����'\"'��ʼ�ͽ���
                while((*pCurPos != '\"') && (*pCurPos != '\0'))
                {
                    pCurPos++;
                }
            }
            else
            {
                //�ַ�����'@'��ʼ�ͽ���
                while((*pCurPos != '@') && (*pCurPos != '\0'))
                {
                   pCurPos++;
                }
            }

            //���û���ҵ�������'\"'��'@'������ʽ����
            if(*pCurPos == '\0')
            {
                TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_STRITEM_NEED_ENDCHAR,
                      szInfix);
                m_PostfixExp.clear();
                return -1;
            }
            
            //��ȡ�����ַ�������'\"'��'@'��һ��λ�ÿ�ʼ����'\"'��'@'��ǰһ��λ�ý���
            nLen = pCurPos - pOldPos;

            //���ʽλ��ָ���Ƶ�������'\"'��'@'����һ��λ��
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
          
            //���ַ���Ԫ�ط����׺���ʽ
            m_PostfixExp.push_back(PEE);
        }

        //��ǰ�ַ���������')'
        else if(*pCurPos == ')')
        {
            //�ѵ�ǰԪ�ط����׺���ʽ
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
                
                //��յ�ǰԪ�ػ�����
                szElement[0] = '\0';
                pCurEle = szElement;
            }
                        
            //�Ѳ�����ջ��'('������֮ǰ�����в�����˳������׺���ʽ
            while(!OpStack.empty())
            {
                Oper = OpStack.top();    //��ȡջ����Ԫ��

                //���ջ����Ԫ����'('����������ֹͣ��ջ
                if((Oper.Left == '(') && (Oper.Right == '#'))
                {
                    OpStack.pop();        //ɾ��ջ��Ԫ��'('
                    break;
                }

                //���ջ��Ԫ�ز���'('�����ջ��Ԫ�ط����׺���ʽ
                PEE.szEleContent[0] = Oper.Left;
                if(Oper.Right != '#')     //˫������
                {
                    PEE.szEleContent[1] = Oper.Right;
                    PEE.szEleContent[2] = '\0';
                }
                else                      //��������
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
                
                OpStack.pop();           //ɾ��ջ��Ԫ��
            }

            pCurPos++;
        }

        //�����ǰ�ַ���һ��������
        else if(IsOperator(*pCurPos) == 0)
        {
            //�ѵ�ǰԪ�ط����׺���ʽ
            *pCurEle = '\0';
            if(*pCurPos == '(')
            {
                //�����ŵ�ǰ�治���в�����
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

                //��յ�ǰԪ�ػ�����
                szElement[0] = '\0';
                pCurEle = szElement;
            }

            Oper.Left = *pCurPos;        //��ȡ��ǰ������
            
            //�ж���һ���ַ��ǲ��ǲ�����
            pCurPos++;
            if((IsOperator(*pCurPos) == 0) && (*pCurPos != '(') && (Oper.Left != '('))
            {
                Oper.Right = *pCurPos;   //˫������
                pCurPos++;
            }
            else
            {
                Oper.Right = '#';        //��������
            }

            //�Ѳ�����ջ�����ȼ����ڵ�ǰ�������Ĳ����������׺���ʽ
            while(!OpStack.empty())
            {
                TmpOp = OpStack.top();  //��ȡջ��������
                
                //�ж�ջ�������������ȼ��Ƿ���ڵ�ǰ������
                if(GetISP(TmpOp) >= GetICP(Oper))
                {
                    //��ջ�������������׺���ʽ
                    PEE.szEleContent[0] = TmpOp.Left;
                    if(TmpOp.Right != '#')
                    {
                        //˫������
                        PEE.szEleContent[1] = TmpOp.Right;
                        PEE.szEleContent[2] = '\0';
                    }
                    else
                    {
                        //��������
                        PEE.szEleContent[1] = '\0';
                    }
                    
                    //���ջ����������'('���򲻷����׺���ʽ
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

                    OpStack.pop();      //ɾ��ջ��Ԫ��
                }

                //���ջ������������С�ڵ�ǰ����������ֹͣ��ջ
                else
                {
                    break;
                }
            }

            OpStack.push(Oper);        //��ǰ��������ջ
        }
        
        //��ǰ�ַ�Ϊ��ͨ�ַ�
        else
        {
            *pCurEle++ = *pCurPos++;
        }
    }

    //�ѵ�ǰԪ�ط����׺���ʽ
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
        
        //��յ�ǰԪ�ػ�����
        szElement[0] = '\0';
        pCurEle = szElement;
    }

    //˳��Ѳ�����ջ�еĲ����������׺���ʽ
    while(!OpStack.empty())
    {
        Oper = OpStack.top();         //��ȡջ��������
        PEE.szEleContent[0] = Oper.Left;
        if(Oper.Right != '#')
        {
            //˫������
            PEE.szEleContent[1] = Oper.Right;
            PEE.szEleContent[2] = '\0';
        }
        else
        {
            //��������
            PEE.szEleContent[1] = '\0';
        }

        //���ջ����������'('���򲻷����׺���ʽ
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

        OpStack.pop();                 //ɾ��ջ��Ԫ��
    }

    return 0;
}
/************ End of The Infix2Postfix Defining ****************/


/************ The Caculate Defining ****************************/
// �������������պ�׺���ʽ������ԭ�򣬸��ݱ��ʽ�������ñ��ʽ�����
//
// ���������
//    szInput���������ݣ�����һ�Ż���
//       uLen���������ݵĳ���
//
// ���������
//     Output��������
//
//     ����ֵ�����������-1�����򷵻�0
//
//   ע  �⣺Ϊ�˼��㷽�㣬���е��������Ͷ���INT4���ʹ��
//
/***************************************************************/
int CPostfixExp::Caculate(const void* szInput, const UINT4 uLen, SVariant& Output)
{
    PostfixExpEleList::iterator item;   //��׺���ʽ������
    SVariant  retVar;                   //��Ų���������
    SVariant  tmpVarX;                  //��һ����������ʱ����
    SVariant  tmpVarY;                  //�ڶ�����������ʱ����

    int    nLen ;                    //�ַ���Ԫ�س��ȱ���
    char   *pTmp ;                //�ַ���Ԫ����ָ�����


    //�����׺���ʽΪ��
    if(m_PostfixExp.size() == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_EXP_EMPTY);
        return -1;
    }

    //��մ�Ų�������ջ
    while(!m_CaculateStack.empty())
    {
        m_CaculateStack.pop();
    }

    //˳���ȡ��׺���ʽ��ÿһ��Ԫ��
    item = m_PostfixExp.begin();
    while(item != m_PostfixExp.end())
    {
        switch (item->yEleType)
		{
		case ET_VARIANT:
			{
				if(GetVarValue(szInput, item->szEleContent, retVar) == 0)
				{
					//��ȡ�ı���ֵ��ջ
					m_CaculateStack.push(retVar);
				}
				//�����ȡ������ֵ����
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
					//��ȡ�ĺ�������ֵ��ջ
					m_CaculateStack.push(retVar);
				}

				//�����ȡ�����ķ���ֵ����
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

				//����ַ��������������ţ���'a'
				if(strlen(item->szEleContent) == 3)
				{
					retVar.yValue = item->szEleContent[1];
				}

				//�ַ�����������������
				else
				{
					retVar.yValue = item->szEleContent[0];
				}

				//�ַ�������ջ
				m_CaculateStack.push(retVar);
			}
			break;
			
		case ET_STRING_CONST:
			{
				retVar.yVariantType = FT_STRING;

				//����ַ�����������'@'��˫���ţ���@example@��"Example"
				//�ַ����е�'@'�ַ������ַ������ߵ�˫���ţ��ǻ���̨�뱾ϵͳ֮���Э��
				if((item->szEleContent[0] == '@') || (item->szEleContent[0] == '\"'))
				{
					nLen = strlen(item->szEleContent);
					pTmp = &item->szEleContent[1];
					memcpy((void*)retVar.szValue, (const void*)pTmp, nLen - 2);
					retVar.szValue[nLen - 2] = 0;
				}

				//�ַ�������������'@'��˫����
				else
				{
					strcpy(retVar.szValue, item->szEleContent);
				}

				//�ַ���������ջ
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
                        
                        //���ջΪ�գ�����ʽ�������
                        else
                        {
                            return -1;
                        }
                        
                        if((Caculating(tmpVarX, item->szEleContent, retVar)) != -1)
                        {
                            //��������ջ
                            m_CaculateStack.push(retVar);
                        }
                        
                        //����������
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
                        
                        //���ջΪ�գ�����ʽ�������
                        else
                        {
                            return -1;
                        }
                        
                        //���ջ��Ϊ�գ����ȡ��һ��������
                        if(!m_CaculateStack.empty())
                        {
                            tmpVarX = m_CaculateStack.top();
                            m_CaculateStack.pop();
                        }
                        
                        //���ջΪ�գ�����ʽ�������
                        else
                        {
                            return -1;
                        }
                        
                        //��ȡ��ǰ������				
                        //strcpy(szOper, item->szEleContent);
                        
                        //���ݵ�ǰ���������������������м���
                        if((Caculating(tmpVarX, tmpVarY, item->szEleContent, retVar)) != -1)
                        {
                            //��������ջ
                            m_CaculateStack.push(retVar);
                        }
                        
                        //����������
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

        //��ȡ��׺���ʽ����һ��Ԫ��
        item++;
    }

    //���ջ��Ϊ�գ����ȡ������
    if(!m_CaculateStack.empty())
    {
        Output = m_CaculateStack.top();
        m_CaculateStack.pop();
    }

    //���ջΪ�գ�����ʽ�������
    else
    {
        return -1;
    }
    
    return 0;
}
/************ End of The Caculate Defining *********************/


/************ The IsDigit Defining *****************************/
// �����������ж�һ���ַ����Ƿ�������
//
// ���������
//      sData�����жϵ��ַ�������
//
// �����������
//
//   ����ֵ������������ַ�������0�����򷵻�-1
//
//Modified by XDB on 2003.04.01
//��������Ա������Ϊ��̬��Ա�������Ա��������ط�����
/***************************************************************/
int CPostfixExp::IsDigit(const char* szData)
{
    //����ַ������ȵ�����
    int nLen = strlen(szData);
    if(nLen == 0)
    {
        return -1;
    }    
    
    //ȥ��ǰ��Ŀո�
    int i = 0;
    while(szData[i] == ' ')
    {
        i++;
    }
    
    //ȥ������Ŀո�
    while(nLen > 0 && szData[nLen - 1] == ' ')
    {
        nLen--;
    }
    
    //�ж��Ƿ���ʣ����ַ�
    if(nLen - i <= 0)
    {
        return -1;
    }    
    
    //�жϵ�һ���ַ��Ƿ���'+'��'-'����
    if(((szData[i] == '+') || (szData[i] == '-')) && (nLen > 1))
    {
        i++;
    }
	
    //�ж��ַ����е�ÿһ���ַ�
	for(; i < nLen; i++)
    { 
        //���pChar���κ�һ���ַ�����'9'��С��'0'���������ִ�
        if((szData[i] > '9') || (szData[i] < '0'))
        {
            return -1;
        }
		
    }
	
    //����ַ��������ַ���������
    return 0;
}
/************ End of The IsDigit Defining **********************/


/************ The IsOperator Defining **************************/
// �����������ж�һ���ַ��Ƿ��ǲ�����
//
// ���������
//       cChr�����жϵ��ַ�
//
// �����������
//
//   ����ֵ������ǲ���������0�����򷵻�-1
//
/***************************************************************/
int CPostfixExp::IsOperator(BYTE cChr) const
{
    //���û���ҵ�����ʾ���жϵ��ַ����ǲ�����
    return m_chOperator[cChr];
}
/************ End of The IsOperator Defining *******************/

/************ The Caculating Defining **************************/
// �������������ݲ������Ͳ��������ó����������һĿ����
//
// ���������
//      varX�� ������
//      pOper��������
//
// ���������
//     retVar������Ľ��
//
//   ����ֵ�����������-1�����򷵻�0
//
//   ע  �⣺Ϊ�˼��㷽�㣬���еļ���������INT4���ʹ��
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

        //�Ƿ�����
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

        //�Ƿ�����
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

        //�Ƿ�����
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

        //�Ƿ�����
        else
        {
            return -1;
        }
    }

    //�Ƿ������ϵ
    else
    {
        return -1;
    }

    return 0;

}
/************ End of The Caculating Defining *******************/

/************ The Caculating Defining **************************/
// �������������ݲ������Ͳ��������ó������������Ŀ����
//
// ���������
//       varX����һ��������
//       varY���ڶ���������
//      pOper��������
//
// ���������
//     retVar������Ľ��
//
//   ����ֵ�����������-1�����򷵻�0
//
//   ע  �⣺Ϊ�˼��㷽�㣬���еļ���������INT4���ʹ��
//
/***************************************************************/
int CPostfixExp::Caculating(const SVariant& varX, const SVariant& varY,
                            const char *pOper, SVariant& retVar) const
{
        //������������������ַ�
    if((varX.yVariantType == FT_CHAR) && (varY.yVariantType == FT_CHAR))
    {
        retVar.yVariantType = FT_ULONG;

        //��������
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
        
        //�߼�����
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

        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //��һ�����������ַ����ڶ���������������
    else if((varX.yVariantType == FT_CHAR) && (varY.yVariantType == FT_ULONG))
    {
        retVar.yVariantType = FT_ULONG;

        //��������
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

        //�߼�����
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

        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //��һ�����������������ڶ������������ַ�
    else if((varX.yVariantType == FT_ULONG) && (varY.yVariantType == FT_CHAR))
    {
        retVar.yVariantType = FT_ULONG;
                
        //��������
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
			
        //�߼�����
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

        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //������������������
    else if((varX.yVariantType == FT_ULONG) && (varY.yVariantType == FT_ULONG))
    {
        retVar.yVariantType = FT_ULONG;
        
        //��������
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
                 //�������������
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

        //�߼�����
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

        //��ϵ����
		else if(pOper[0] == '&' && pOper[1] == '&' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value && varY.u4Value);
        }
		else if(pOper[0] == '|' && pOper[1] == '|' && pOper[2] == 0)
        {
            retVar.u4Value = (varX.u4Value || varY.u4Value);
        }

        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //������������������ַ���
    else if((varX.yVariantType == FT_STRING) && (varY.yVariantType == FT_STRING))
    {             
        retVar.yVariantType = FT_LONG;

        //�߼�����
        if(pOper[0] == '=' && pOper[1] == '=' && pOper[2] == 0)
        {
            int nYLen = strlen(varY.szValue);      
            if(varY.szValue[0] == '[' && varY.szValue[nYLen - 1] == ']')//���Ϊ�Ӵ�ƥ��
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
            else //���Ϊ�ֹ�ƥ��
            {                
                //ȥ����ͷ�Ŀո�
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

        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //�������������ַ������Ҳ�������ʮ������
    else if((varX.yVariantType == FT_STRING) && (varY.yVariantType == FT_HEX))
    {
        retVar.yVariantType = FT_ULONG;
        INT4 uDataX = 0;
        
        //�ж���������Ƿ�Ϊʮ�������ַ���
        char szTmp[63] ;
        memset(szTmp, 0, sizeof(szTmp));
        int varXLen = strlen(varX.szValue);
        memcpy(szTmp, varX.szValue, varXLen);        
        szTmp[varXLen] = 'H';
        
        if(IsHex(szTmp) != 0)
        {
            return -1;
        }  

        //���������ת��������
        sscanf(varX.szValue, "%x", &uDataX);
        
        //�߼�����
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
        
        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }
    
    //�������������ַ������Ҳ�����������
    else if((varX.yVariantType == FT_STRING) && (varY.yVariantType == FT_ULONG))
    {
        retVar.yVariantType = FT_ULONG;
        INT4 uDataX = 0;

        //�����ʵ��������ȡ��
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
        
        //���������ת��������
        if(IsDigit(szTmp) != 0)
        {
            return -1;  //�������������������ַ���
        }
        
        uDataX = atol(szTmp);        

        //�߼�����
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

        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //��һ�����������ַ����ڶ�����������FT_LONG
    else if((varX.yVariantType == FT_CHAR) && (varY.yVariantType == FT_LONG))
    {
        retVar.yVariantType = FT_LONG;
        
        //��������
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.n4Value = varX.yValue + varY.n4Value;
        }
        else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.n4Value = varX.yValue - varY.n4Value;
        }
        
        //�߼�����
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
        
        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }
    
    //��һ�����������������ڶ������������ַ�
    else if((varX.yVariantType == FT_LONG) && (varY.yVariantType == FT_CHAR))
    {
        retVar.yVariantType = FT_LONG;
        
        //��������
        if(pOper[0] == '+' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value + varY.yValue;
        }
        else if(pOper[0] == '-' && pOper[1] == 0)
        {
            retVar.n4Value = varX.n4Value - varY.yValue;
        }
        
        //�߼�����
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
        
        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }
    
    //������������������
    else if((varX.yVariantType == FT_LONG) && (varY.yVariantType == FT_LONG))
    {
        retVar.yVariantType = FT_LONG;
        
        //��������
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
                //�������������
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
        
        //�߼�����
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
        
        //��ϵ����
        else if(pOper[0] == '&' && pOper[1] == '&' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value && varY.n4Value);
        }
        else if(pOper[0] == '|' && pOper[1] == '|' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value || varY.n4Value);
        }
        
        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //�������������ַ������Ҳ�����������
    else if((varX.yVariantType == FT_STRING) && (varY.yVariantType == FT_LONG))
    {
        retVar.yVariantType = FT_LONG;
        INT4 uDataX = 0;
        
        //���������ת��������
        if(IsDigit(varX.szValue) != 0)
        {
            return -1;  //�������������������ַ���
        }
        
        uDataX = atol(varX.szValue);
        
        //�߼�����
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
        
        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //������������������
    else if((varX.yVariantType == FT_ULONG) && (varY.yVariantType == FT_LONG))
    {
        retVar.yVariantType = FT_LONG;
        
        //��������
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
                //�������������
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
        
        //�߼�����
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
        
        //��ϵ����
        else if(pOper[0] == '&' && pOper[1] == '&' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.u4Value && varY.n4Value);
        }
        else if(pOper[0] == '|' && pOper[1] == '|' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.u4Value || varY.n4Value);
        }
        
        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }

    //������������������
    else if((varX.yVariantType == FT_LONG) && (varY.yVariantType == FT_ULONG))
    {
        retVar.yVariantType = FT_LONG;
        
        //��������
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
                //�������������
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
        
        //�߼�����
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
        
        //��ϵ����
        else if(pOper[0] == '&' && pOper[1] == '&' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value && varY.u4Value);
        }
        else if(pOper[0] == '|' && pOper[1] == '|' && pOper[2] == 0)
        {
            retVar.n4Value = (varX.n4Value || varY.u4Value);
        }
        
        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_POSTFIXEXP_INVALID_OPR_FOUND,
                  m_strInfix.c_str(), 
                  pOper);
            return -1;
        }
    }
    //�Ƿ������ϵ
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
    //����ַ������ȵ�����
    int nLen = strlen(szData);
    if(nLen == 0)
    {
        return -1;
    }    
    
    //ȥ��ǰ��Ŀո�
    int i = 0;
    while(szData[i] == ' ')
    {
        i++;
    }
    
    //ȥ������Ŀո�
    while(nLen > 0 && szData[nLen - 1] == ' ')
    {
        nLen--;
    }
    
    //�ж��Ƿ���ʣ����ַ�
    if(nLen - i - 1 <= 0)
    {
        return -1;
    }
    
    if(szData[nLen - i - 1] != 'H' && szData[nLen - i - 1] != 'h')
    {
        return -1;
    }
    
    nLen --;
    //�жϵ�һ���ַ��Ƿ���'+'��'-'����
    if(((szData[i] == '+') || (szData[i] == '-')) && (nLen > 1))
    {
        i++;
    }
    
    //�ж��ַ����е�ÿһ���ַ�
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
    
    //����ַ��������ַ���������
    return 0;
}
//end by lx SWPD03800
