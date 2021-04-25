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
// ����������һ�ָ�ʽ��ʶ����������ѯ������ͨ���ķּ�������
//
/********************************************************************/
class IGWB_Svc_Export CCondition: public CPostfixExp
{
public:

    //���캯��
    CCondition(CFormat* pFmtOwning);
    CCondition(CFormat* pFmtOwning, const STRING& sDisCondition);

    virtual ~CCondition();    //��������

    //���ø�ʽ��־����
    void SetFlag(const int bThisFmt);

    //���ò�ѯ������־����
    void SetQueryFlag(int nIsQuery);

    int  ParseExpress(const char* const  szExpress, LIST<STRING>& FieldList);
    
    virtual int Infix2Postfix(const char* szInfix);

public:

    //���һ��������ʵ��ȡֵ�����ػ�����麯��
    virtual int GetVarValue(const void* szInput, const char*  szVarName, SVariant& stVar);

    //����һ�����������ֵ�����ػ�����麯��
    virtual int GetFunReturn(const void* szInput, const char* szFun, SVariant& stVar);

protected:

    CFormat*    m_pFmtOwning;      //�����������ĸ�ʽ

    //��ʾ������������ʱ������������Ƿ���ϱ���ʽ��
    //  0����ʾ��������ݷ���m_pFmtOwningָ��ĸ�ʽ��
    // -1����ʾ��������ݷ���m_pFmtOwningָ��Ķ����Դ��ʽ
    int         m_bThisFmt;

    //�Ƿ��ǲ�ѯ�����ĵı�־��
    //1����ʾ��ѯ������0����ʾ���ǲ�ѯ����
    int         m_nIsQuery;

};
/****************** End of The CCondition class Defining ************/

#endif  //!defined(_FORMAT_CONDITION_H_)
