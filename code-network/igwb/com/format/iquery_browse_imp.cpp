/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the IQueryBrowseImp
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
#include "format_lib.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "iquery_browse_imp.h"

/************ The IQueryBrowseImp Defining *********************/
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
IQueryBrowseImp::IQueryBrowseImp(CIGWB* pIGWB): m_pIGWB(pIGWB)
{
    //��ʼ����Ա����
    //m_pIGWB = pIGWB;
    assert(m_pIGWB != NULL);
}
/************ End of The IQueryBrowseImp Defining **************/


/************ The ~IQueryBrowseImp Defining ********************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
IQueryBrowseImp::~IQueryBrowseImp()
{
}
/************ End of The ~IQueryBrowseImp Defining *************/


/************ The Init Defining ********************************/
// �������������������ѯ�ӿڳ�ʼ��
//
// ���������
//      nAPID�������ID����ʼ�����������ѯ�ӿ�ʱ�������������
//             �������г�ʼ����nAPID��0����Ҳ����ֻ���һ��
//             �������г�ʼ����nAPID���㣩
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IQueryBrowseImp::Init(const UINT4 nAPID)
{
    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pIGWB == NULL)
    {
        return -1;
    }

    //���г�ʼ���������س�ʼ�����
    return m_pIGWB->Init(nAPID);
}
/************ End of The Init Defining *************************/


/************ The GetAPNames Defining **************************/
// ������������ø�ʽ�����õ����н��������
//
// �����������
//
// ���������
//    APNames����ø�ʽ�����õ����н���������б�
//
//   ����ֵ�����ؽ�������
//
/***************************************************************/
UINT4 IQueryBrowseImp::GetAPNames(LIST<STRING>& APNames) const
{
    //��ʼ���������
    APNames.clear();

    //ȷ����Ӧ��Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //�������н����
    STRING sAPName;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //��ȡ��ǰ���������
        (*it)->GetAPName(sAPName);
        APNames.push_back(sAPName);

        it++;
    }

    return APNames.size();
}
/************ End of The GetAPNames Defining *******************/


/************ The GetChlNames Defining *************************/
// ���������������ΪsAPName�Ľ���������ͨ������
//
// ���������
//    sAPName�����������
//
// ���������
//   ChlNames�����ص�����ͨ�������б�
//
//   ����ֵ������ͨ������
//
/***************************************************************/
UINT4 IQueryBrowseImp::GetChlNames(const STRING& sAPName, LIST<STRING>& ChlNames) const
{
    //��ʼ���������
    ChlNames.clear();

    //ȷ����Ӧ��Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //�������н����
    STRING sTmpAPName;
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ����������Ƿ����ָ�����������
        (*it)->GetAPName(sTmpAPName);
        if(!(sAPName.compare(sTmpAPName)))
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //����ҵ�����ָ����������ƵĽ�������
    if(pAP != NULL)
    {
        //��ȡ���ӽ���������ͨ������
        return pAP->GetChannelNames(ChlNames);
    }

    //���û���ҵ�����ָ����������ƵĽ�������
    return 0;
}
/************ End of The GetChlNames Defining ******************/


/************ The GetOrigBrowseNames Defining ******************/
// �����������������ΪsAPName�Ľ���������ԭʼ���������ʽ����
//           ��Ӧ�ĸ�ʽ����
//
// ���������
//    sAPName�����������
//
// ���������
//      Names�����ص�����ԭʼ���������ʽ���б�
//      Descs������ԭʼ���������ʽ�������б�
//
//   ����ֵ������ԭʼ���������ʽ����
//
/***************************************************************/
UINT4 IQueryBrowseImp::GetOrigBrowseNames(const STRING& sAPName, LIST<STRING>& Names,
                                          LIST<STRING>& Descs) const
{
    //��ʼ���������
    Names.clear();
    Descs.clear();

    //ȷ����Ӧ��Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //�������н����
    STRING sTmpAPName;
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ����������Ƿ����ָ�����������
        (*it)->GetAPName(sTmpAPName);
        if(!(sAPName.compare(sTmpAPName)))
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //����ҵ�����ָ����������ƵĽ�������
    if(pAP != NULL)
    {
        //��ȡ���ӽ���������ԭʼ���������ʽ������Ӧ�ĸ�ʽ����
        return pAP->GetOrigBrowseNames(Names, Descs);
    }

    //���û���ҵ�����ָ����������ƵĽ�������
    return 0;
}
/************ End of The GetOrigBrowseNames Defining ***********/


/************ The GetFinalBrowseNames Defining *****************/
// ��������������һ����������ƣ����ĳ��ͨ�����������ջ������
//           ��ʽ������Ӧ�ĸ�ʽ����
//
// ���������
//    sAPName�����������
//   sChlName��ͨ������
//
// ���������
//      Names�����ص��������ջ��������ʽ���б�
//      Descs���������ջ��������ʽ�������б�
//
//   ����ֵ���������ջ��������ʽ����
//
/***************************************************************/
UINT4 IQueryBrowseImp::GetFinalBrowseNames(const STRING& sAPName, const STRING& sChlName,
                                           LIST<STRING>& Names, LIST<STRING>& Descs) const
{

    //��ʼ���������
    Names.clear();
    Descs.clear();

    //ȷ����Ӧ��Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //�������н����
    STRING sTmpAPName;
    CAccessPoint* pAP = NULL;
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ����������Ƿ����ָ�����������
        (*it)->GetAPName(sTmpAPName);
        if(!(sAPName.compare(sTmpAPName)))
        {
            pAP = (*it);
            break;
        }

        it++;
    }

    //����ҵ�����ָ����������ƵĽ�������
    if(pAP != NULL)
    {
        //��ȡ���ӽ���㡢ָ��ͨ���µ��������ջ��������ʽ��
        //����Ӧ�ĸ�ʽ����
        return pAP->GetFinalBrowseNames(sChlName, Names, Descs);
    }

    //���û���ҵ�����ָ����������ƵĽ�������
    return 0;
}
/************ End of The GetFinalBrowseNames Defining **********/


/************ The GetBrowseFmt Defining ************************/
// �����������������ΪsBrowseName�������ʽ�ĸ�ʽ�������
//
// ���������
// sBrowseName�������ʽ��
//
// �����������
//
//   ����ֵ�������ʽ���󣬵����߲����ͷŸ�ָ��
//
/***************************************************************/
CFormat* IQueryBrowseImp::GetBrowseFmt(const STRING& sBrowseName)
{
    //ȷ����Ӧ��Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return NULL;
    }

    //��ȡָ����ʽ�ĸ�ʽ����
    CFormat* pFMT = NULL;
    pFMT = m_pIGWB->m_pFmtLib->GetFormat(sBrowseName);

    return pFMT;
}
/************ End of The GetBrowseFmt Defining *****************/
