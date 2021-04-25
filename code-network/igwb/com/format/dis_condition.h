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

//��ʽ��ּ�������Ӧ��ϵ�ṹ����
typedef struct _SrcFmtDisMap
{
	STRING         sFmtName;        //���ջ���Դ��ʽ��
	CFormat*       pFmt;            //��Ӧ�ĸ�ʽ����
	STRING         sDisCondition;   //��Ӧ�ķּ���������׺���ʽ
	CCondition*    pDisCondition;   //��Ϊ�ּ�������CCondition����
} SSrcFmtDisMap, *PSrcFmtDisMap;

//��ʽ��ּ�������Ӧ��ϵ�������Ͷ���
typedef   LIST<SSrcFmtDisMap*> DIS_LIST;

/****************** The CDisCondition class Defining ****************/
// �����������ּ�������
//
/********************************************************************/
class CDisCondition
{
public:

    //���캯��
    CDisCondition(CFormatLib *pFmtLib = NULL);

    //��������
    ~CDisCondition();
     
    //��ʼ������
    int Init(CFormatLib *pFmtLib = NULL);

    //����һ�ָ�ʽ�����Ӧ�ķּ���������
    int AddDisCondition(const STRING& sFmtName, const STRING& sDisCondition);

    //�ж�һ�Ż��������ϵķּ�����
    int IsTheDisCondition(const void* pABill, const UINT4 uLen, CFormat*& pSrcFmt);

    //��ȡָ����ʽ����Ӧ�ķּ�����
    int GetDisCondName(const STRING& sFmtName, STRING& sDisCondName) const;

protected:

    DIS_LIST*      m_pFmtDisList;   //��ʽ��ּ�������Ӧ��ϵ
    CFormatLib*    m_pFmtLib;       //��ʽ�����

};
/****************** End of The CDisCondition class Defining *********/


#endif  //!defined(_FORMAT_DISTRIBUTE_CONDITION_H_)
