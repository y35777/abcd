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
// �������������캯��
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CDisCondition::CDisCondition(CFormatLib *pFmtLib): m_pFmtLib(pFmtLib)
{
    //��ʼ����Ա����
    //m_pFmtLib = pFmtLib;
    m_pFmtDisList = new DIS_LIST;
}
/************ End of The CDisCondition Defining ****************/


/************ The ~CDisCondition Defining **********************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CDisCondition::~CDisCondition()
{
    DIS_LIST::iterator item;  //��ʽ��ּ�������Ӧ��ϵ������


    if(m_pFmtDisList != NULL)
    {
        //ɾ����ʽ��ּ�������Ӧ��ϵ�е�ÿһ������
        item = m_pFmtDisList->begin();
        while(item != m_pFmtDisList->end())
        {
            if((*item)->pDisCondition != NULL)
            {
                //ɾ����������
                delete (*item)->pDisCondition;
            }
            delete *item;  //ɾ����ʽ��ּ�������Ӧ��ϵ����

            item++;
        }

        //ɾ����ʽ��ּ�������Ӧ��ϵ�������
        delete m_pFmtDisList;
    }
}
/************ End of The ~CDisCondition Defining ***************/

     
/************ The Init Defining ********************************/
// ������������ʼ������
//
// ���������
//    pFmtLib����ʽ�����ָ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CDisCondition::Init(CFormatLib *pFmtLib)
{
    //ȷ��m_pFmtLib��Ϊ��
    if((pFmtLib == NULL) && (m_pFmtLib == NULL))
    {
        return -1;
    }
    else if(pFmtLib != NULL)
    {
        m_pFmtLib = pFmtLib;
    }

    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pFmtDisList == NULL)
    {
        return -1;
    }

    //����ֲ�����
    CFormat *pFmt = NULL;
	CFormat *pSrcFmt = NULL;
    CCondition *pCond = NULL;
    DIS_LIST::iterator item;

    //����m_pFmtDisList�����г�Ա
    item = m_pFmtDisList->begin();
    while(item != m_pFmtDisList->end())
    {
        //���ø�ʽ����Ӧ�ĸ�ʽ����ָ��
        if((*item)->pFmt == NULL)
        {
            pFmt = m_pFmtLib->GetFormat((*item)->sFmtName);
            if(pFmt == NULL)
            {
                TRACE(MTS_FORMAT, S_FMT_FMT_NOT_FOUND_INIT_DISCOND,
                     (*item)->sFmtName.c_str());
                return -1;
            }
            //�޸��ֽ�������(D12711),��ïʶ��2002-04-13
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

             //������
        }

        //�����������ʽ��Ӧ����������
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
// ��������������һ�ָ�ʽ�����Ӧ�ķּ���������
//
// ���������
//      sFmtName����ʽ��
// sDisCondition����ʽ�ּ���������׺���ʽ
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CDisCondition::AddDisCondition(const STRING& sFmtName, const STRING& sDisCondition)
{    
    DIS_LIST::iterator item;   //��ʽ��ּ�������Ӧ��ϵ������

    //ȷ�������������ȷ��
    if((sFmtName.length() == 0) || (sDisCondition.length() == 0))
    {
        return -1;
    }

    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pFmtDisList == NULL)
    {
        m_pFmtDisList = new DIS_LIST;
    }

    //����m_pFmtDisList�����г�Ա
    item = m_pFmtDisList->begin();
    while(item != m_pFmtDisList->end())
    {
        if(!(sFmtName.compare((*item)->sFmtName)))
        {
            //��ʽ�Ѿ�����
            return -1;
        }
        item++;
    }

    //������ʽ��ּ�������Ӧ��ϵ���󣬲���ʼ��
    SSrcFmtDisMap* pFDMP = new SSrcFmtDisMap;
    pFDMP->pFmt = NULL;
    pFDMP->pDisCondition = NULL;
    pFDMP->sFmtName = sFmtName;
    pFDMP->sDisCondition = sDisCondition;

    //����ʽ��ּ�������Ӧ��ϵ�������������
    m_pFmtDisList->push_back(pFDMP);

    return 0;
}
/************ End of The AddDisCondition Defining **************/


/************ The IsTheDisCondition Defining *******************/
// �����������ж�һ�Ż��������ϵķּ�����
//
// ���������
//    pABill����������ָ��
//      uLen��pABill�ĳ���ʽ
//
// ���������
//    pSrcFmt��pABillָ��Ļ��������ϵ����ջ���Դ��ʽ���󣬵����߲����ͷŸ�ָ��
//
//   ����ֵ���ҵ���Ӧ�ĸ�ʽ���󷵻�0�����򷵻�-1
//
/***************************************************************/
int CDisCondition::IsTheDisCondition(const void* pABill, const UINT4 uLen,
                                     CFormat*& pSrcFmt)
{
    int bFound = -1;   //�Ƿ��ҵ���־
    pSrcFmt = NULL;    //��ʼ���������

    if((pABill == NULL) || (uLen == 0))
    {
        return -1;
    }

    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pFmtDisList == NULL)
    {
        return -1;
    }

    //����m_pFmtDisList�����г�Ա
    DIS_LIST::iterator item = m_pFmtDisList->begin();
    while(item != m_pFmtDisList->end())
    {
        //�ж�����Ļ��������Ƿ���ϸø�ʽ
        bFound = (*item)->pFmt->IsTheFormat(pABill, uLen);
        if(bFound == 0)  //���ϸø�ʽ
        {
            break;
        }

        item++;
    }

    //����ҵ����ϵĸ�ʽ����
    if(bFound == 0)
    {
        //�ж��Ƿ���Ϸּ�����
        SVariant Var;
        int nRet = (*item)->pDisCondition->Caculate(pABill, uLen, Var);
        if(nRet != 0)
        {
            //assert(0);
            return -1;  //���ʽ�������
        }

        //�����ּ�����
        if(Var.n4Value == 0)
        {
            return -1;
        }

        //���Ϸּ�����
        pSrcFmt = (*item)->pFmt;
        return 0;
    }

    //�����Ϸּ�����
    return -1;
}
/************ End of The IsTheDisCondition Defining ************/


/************ The GetDisCondName Defining **********************/
// ������������ȡָ����ʽ����Ӧ�ķּ�����
//
// ���������
//   sFmtName����ʽ��
//
// ���������
//   sDisName����ʽ�ּ���������׺���ʽ
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CDisCondition::GetDisCondName(const STRING& sFmtName, STRING& sDisCondName) const
{
    //��ʼ���������
    sDisCondName = "";

    //ȷ�������������ȷ
    if(sFmtName.length() == 0)
    {
        return -1;
    }

    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pFmtDisList == NULL)
    {
        return -1;
    }

    //����m_pFmtDisList�����г�Ա
    DIS_LIST::iterator item = m_pFmtDisList->begin();
    while(item != m_pFmtDisList->end())
    {
        if(!(sFmtName.compare((*item)->sFmtName)))
        {
            //����ҵ�ָ���ĸ�ʽ��
            sDisCondName = (*item)->sDisCondition;

            return 0;
        }

        item++;
    }

    return -1;
}
/************ End of The GetDisCondName Defining ***************/
