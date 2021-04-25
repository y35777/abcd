/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the IBillProcessorImp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format.h"
#include "igwb.h"
#include "access_point.h"
#include "../include/export_dll.h"
#include "ibill_processor_imp.h"
#include "../include/toolbox.h"

/************ The IBillProcessorImp Defining *******************/
// �������������캯��
//
// ���������
//       pIGWB��CIGWBָ�룬����Ϊ��
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
IBillProcessorImp::IBillProcessorImp(CIGWB* pIGWB): m_pIGWB(pIGWB)
{
    //��ʼ����Ա����
    //m_pIGWB = pIGWB;

    //ȷ������Ĳ�����Ϊ��
    assert(m_pIGWB != NULL);
    
    m_uAPID = -1;
    m_pAP = NULL;
}
/************ End of The IBillProcessorImp Defining ************/


/************ The ~IBillProcessorImp Defining ******************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
IBillProcessorImp::~IBillProcessorImp()
{
}
/************ End of The ~IBillProcessorImp Defining ***********/


/************ The Init Defining ********************************/
// ������������ʼ������
//
// ���������
//       nAPID�������ID������Ϊ0������������ӿ�ֻ������ض��Ľ����
//              ���г�ʼ�������ܶ����н������г�ʼ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IBillProcessorImp::Init(const UINT4 nAPID)
{
    //����Ľ����ID����Ϊ��
    if(nAPID == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_BILLPROCESS_INVALID_APID);
        return -1;
    }
    m_uAPID = nAPID;

    //m_pIGWB����Ϊ��
    if(m_pIGWB == NULL)
    {
        return -1;
    }

    //��ʼ��ָ�������
    int nRet = m_pIGWB->Init(nAPID);
    if(nRet != 0)
    {
        TRACE(MTS_FORMAT, S_FMT_BILLPROCESS_INIT_IGWB_FAIL, 
              nAPID);
        return -1;
    }

    //�������н����
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ�����ID�Ƿ���ڱ������ID
        if((*it)->GetAPID() == m_uAPID)
        {
            m_pAP = (*it);
            return 0;
        }

        it++;
    }

    TRACE(MTS_FORMAT, S_FMT_BILLPROCESS_AP_NOT_FOUND, 
          m_uAPID);

    return -1;  //���س�ʼ�����ؽ��
}
/************ End of The Init Defining *************************/

void IBillProcessorImp::SpecDistribute(const void* pABill, const UINT4 uLen, 
									VECTOR<UINT4>& uChlID, VECTOR<CFormat*>& pDestFmt)
{
    //��ʼ���������
    pDestFmt.clear();
    uChlID.clear();
	
    //ȷ������Ļ������ݲ�Ϊ��
    if((pABill == NULL) || (uLen == 0))
    {
        return;
    }
	
	m_pAP->SpecDistribute(pABill, uLen, uChlID, pDestFmt);
}


/************ The Distribute Defining **************************/
// ��������������һ�Ż������䳤�ȣ�������Ӧ�����ջ���ͨ��ID����
//           ��Ӧ�ĸ�ʽ�������
//
// ���������
//     pABill��ԭʼ����ָ��
//       uLen��pABill��ָ��Ļ�������
//
// ���������
//     uChlID��pABill������ͨ��ID
//   pDestFmt����Ӧ�����ջ�����ʽ����ָ��
//
//   ����ֵ����
//
/***************************************************************/
void IBillProcessorImp::Distribute(const void* pABill, const UINT4 uLen, 
                                   UINT4& uChlID, CFormat*& pDestFmt)
{
    //��ʼ���������
    pDestFmt = NULL;
    uChlID = 0;

    //ȷ������Ļ������ݲ�Ϊ��
    if((pABill == NULL) || (uLen == 0))
    {
        return;
    }

    /*
    //ȷ����Ӧ��Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return;
    }

    //�������н����
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ�����ID�Ƿ���ڱ������ID
        if((*it)->GetAPID() == m_uAPID)
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //����ҵ����ڱ������ID�Ľ�������
    if(pAP != NULL)
    {
        //�ּ�����Ļ�������
        pAP->Distribute(pABill, uLen, uChlID, pDestFmt);
    }*/

    m_pAP->Distribute(pABill, uLen, uChlID, pDestFmt);
}
/************ End of The Distribute Defining *******************/


/************ The GetOrigLen Defining *************************/
// ������������õ�ǰ������ԭʼ��������
//
// �����������
//
// �����������
//
//   ����ֵ����ǰ������ԭʼ��������
//
/***************************************************************/
UINT4 IBillProcessorImp::GetOrigLen(void) const
{
    /*
    //ȷ����Ӧ��Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }
    
    //�������н����
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ�����ID�Ƿ���ڱ������ID
        if((*it)->GetAPID() == m_uAPID)
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //����ҵ����ڱ������ID�Ľ�������
    if(pAP != NULL)
    {
        //��ȡ��������ԭʼ��������
        return pAP->GetOrigLen();
    }

    //���û���ҵ����ڱ������ID�Ľ�������
    return 0;*/

    return m_pAP->GetOrigLen();
}

UINT4 IBillProcessorImp::GetFinalLen(UINT4 uChlID) const
{
    return m_pAP->GetFinalLen(uChlID);
}
/************ End of The GetOrigLen Defining *******************/


/************ The GetChannelNames Defining *********************/
// ������������õ�ǰ���������ͨ������
//
// �����������
//
// ���������
//   ChlNames�����ͨ�����б�
//
//   ����ֵ��ͨ��������������ȱʡͨ��0��
//
/***************************************************************/
UINT4 IBillProcessorImp::GetChannelNames(LIST<STRING>& ChlNames) const
{
    //��ʼ���������
    ChlNames.clear();

    /*
    //ȷ����Ӧ��Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //�������н����
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ�����ID�Ƿ���ڱ������ID
        if((*it)->GetAPID() == m_uAPID)
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //����ҵ����ڱ������ID�Ľ�������
    if(pAP != NULL)
    {
        //��ȡ������������ͨ������
        return pAP->GetChannelNames(ChlNames);
    }

    //���û���ҵ����ڱ������ID�Ľ�������
    return 0;*/

    return m_pAP->GetChannelNames(ChlNames);
}
/************ End of The GetChannelNames Defining **************/
