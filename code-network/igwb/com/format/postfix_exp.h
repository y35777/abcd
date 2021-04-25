#if !defined(_FORMAT_POSTFIX_EXP_H_)
#define _FORMAT_POSTFIX_EXP_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CPostfixExp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

/***************The CPostfixExp Class Defining***********************/
// �������������ʽ������
//
/********************************************************************/
class IGWB_Svc_Export CPostfixExp
{
public:

    //���캯��
    CPostfixExp();

    //��������
    virtual ~CPostfixExp();

    //Modified by XDB on 2003.04.01
    //��������Ա������Ϊ��̬��Ա�������Ա��������ط�����
    //�ж�һ���ַ�������EPostfixEleType��������һ��
    static EPostfixEleType GetDataType(const char* sData);

    //�ж�һ���ַ��������Ĳ��������ͣ��Ǽ�Ŀ������
    static EOperatorType GetOperatorType(const char* sData);

    //��������Ĳ�����op��������ջ�����ȼ�
    int GetISP(const SOperatorSymbol& op) const;

    //��������Ĳ�����op��������ջ�����ȼ�
    int GetICP(const SOperatorSymbol& op) const;

    //���������׺���ʽszInfixת��Ϊ��׺���ʽ������ת������洢��m_PosfixExp��
    virtual int Infix2Postfix(const char* szInfix);

    //���պ�׺���ʽ������ԭ�򣬸��ݱ��ʽ�������ñ��ʽ�����
    int  Caculate(const void* szInput, const UINT4 uLen, SVariant& Output);

    //Modified by XDB on 2003.04.01
    //��������Ա������Ϊ��̬��Ա�������Ա��������ط�����
    //�ж�һ���ַ����Ƿ�������
    static int IsDigit(const char* sData);

    static int IsHex(const char* szData);

protected:

    //���һ��������ʵ��ȡֵ
    virtual int GetVarValue(const void* szInput, const char*  szVarName, SVariant& stVar) = 0;
    //����һ���������
    virtual int GetFunReturn(const void* szInput, const char* szFun, SVariant& stVar) = 0;

protected:
    
    //�ж�һ���ַ��Ƿ��ǲ�����
    int IsOperator(BYTE cChr) const;

    //���ݲ������Ͳ��������ó����������һĿ����
    int Caculating(const SVariant& varX, const char *pOper,
                   SVariant& retVar) const;

    //���ݲ������Ͳ��������ó������������Ŀ����
    int Caculating(const SVariant& varX, const SVariant& varY, const char *pOper,
                   SVariant& retVar) const;

protected:

    PostfixExpEleList m_PostfixExp;    //��ź�׺���ʽ������
    STACK<SVariant, VECTOR<SVariant> > m_CaculateStack;   //���ʽ����ʱ����Ų�������ջ

    STRING  m_strInfix;
    char    m_chOperator[255];
};
/***************End of The CPostfixExp Class Defining***************/

#endif  //!defined(_FORMAT_POSTFIX_EXP_H_)
