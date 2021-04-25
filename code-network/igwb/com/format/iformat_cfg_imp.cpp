/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the IFormatCfgImp
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
#include "iformat_cfg_imp.h"

/************ The IFormatCfgImp Defining ***********************/
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
IFormatCfgImp::IFormatCfgImp(CIGWB *pIGWB): m_pIGWB(pIGWB)
{
    //ȷ������Ĳ�����Ϊ��
    //m_pIGWB = pIGWB;
    assert(m_pIGWB != NULL);
}
/************ End of The IFormatCfgImp Defining ****************/


/************ The ~IFormatCfgImp Defining **********************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
IFormatCfgImp::~IFormatCfgImp()
{
}
/************ End of The ~IFormatCfgImp Defining ***************/


/************ The Init Defining ********************************/
// ������������ʼ����ʽ���ýӿ�
//
// ���������
//     nAPID����Ҫ��ʼ���Ľ����ı�ţ���nAPID = 0�����ʾ�������
//            �Ľ������г�ʼ����ע�⣺ ��ʼ��IFormatCfg�ӿ�ʱ��
//            ����������еĽ������г�ʼ������nAPID����Ϊ0
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
//   �ؼ��㣺�����Ը�ʽ���ý��г�ʼ������CIGWB����ĳ�ʼ������
//           int CIGWB::Init(const UINT4 nAPID = 0, const int bCfg = -1)
//           �еڶ�������bCfg����Ϊ�㣬����Ϊ����
//
/***************************************************************/
int IFormatCfgImp::Init(const UINT4 nAPID)
{
    //ȷ����Ӧ��Ա������Ϊ��
    if(m_pIGWB == NULL)
    {
        return -1;
    }

    //���г�ʼ���������س�ʼ�����
    return m_pIGWB->Init(nAPID, 0);
}
/************ End of The Init Defining *************************/


/************ The AddFormat Defining ***************************/
// �����������ڸ�ʽ��������һ������ΪsFmtName�Ļ�����ʽ
//
// ���������
//   sFmtName�����ӵĸ�ʽ��ĸ�ʽ����
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::AddFormat(const STRING& sFmtName)
{
    //ȷ������Ĳ�����ȷ
    if(sFmtName.length() == 0)
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return -1;
    }

    //����ָ���ĸ�ʽ�����������ӳɹ����Ľ��
    return m_pIGWB->m_pFmtLib->AddFormat(sFmtName);
}
/************ End of The AddFormat Defining ********************/


/************ The SetFormatGen Defining ************************/
// ����������ָ��һ�ָ�ʽ��sFmtName�����û��޸����ʽ����sFmtDes
//           �Ͷ�Ӧ��Դ��ʽ��sSrcFmtName
//
// ���������
//    sFmtName��ָ���޸ĵĸ�ʽ����
//     sFmtDes��ָ����ʽ�ĸ�ʽ����
// sSrcFmtName��ָ����ʽ��Դ��ʽ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::SetFormatGen(const STRING& sFmtName, const STRING& sFmtDes, 
                                const STRING& sSrcFmtName)
{
    //ȷ������Ĳ�����ȷ
    if((sFmtName.length() == 0) || (sFmtDes.length() == 0) 
       || (sSrcFmtName.length() == 0))
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
         return -1;
    }

    //��ȡָ���ĸ�ʽ����ָ��
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return -1;  //��ȡָ����ʽ����ָ��ʧ��
    }

    //����ָ����ʽ�ĸ�ʽ������ԭ��ʽ��
    pFmt->SetFormatGen(sFmtDes, sSrcFmtName);

    return 0;
}
/************ End of The SetFormatGen Defining *****************/


/************ The GetFieldsMap Defining ************************/
// �������������ָ����ʽ��sFmtName�����еĻ�������ϢMAPָ��
//
// ���������
//   sFmtName��ָ���ĸ�ʽ��
//
// �����������
//
//   ����ֵ�����ظ�ʽ��ΪsFmtName�ĸ�ʽ��Ϣ
//
/***************************************************************/
const FIELD_MAP* IFormatCfgImp::GetFieldsMap(const STRING& sFmtName)
{
    //ȷ������Ĳ�����ȷ
    if(sFmtName.length() == 0)
    {
        return NULL;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return NULL;
    }

    //��ȡָ���ĸ�ʽ����ָ��
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return NULL;  //��ȡָ����ʽ����ָ��ʧ��
    }

    //��ȡָ����ʽ�Ļ�������ϢMAPָ��
    return pFmt->GetFieldsMap();
}
/************ End of The GetFieldsMap Defining *****************/


/************ The GetFieldsArray Defining **********************/
// �������������ָ����ʽ��sFmtName�����еĻ�������ϢSFieldInfo*����ָ��
//
// ���������
//   sFmtName��ָ���ĸ�ʽ��
//
// �����������
//
//   ����ֵ�����ظ�ʽ��ΪsFmtName�ĸ�ʽ��Ϣ
//
/***************************************************************/
const SFieldInfo** IFormatCfgImp::GetFieldsArray(const STRING& sFmtName)
{
    //ȷ������Ĳ�����ȷ
    if(sFmtName.length() == 0)
    {
        return NULL;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return NULL;
    }

    //��ȡָ���ĸ�ʽ����ָ��
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return NULL;  //��ȡָ����ʽ����ָ��ʧ��
    }

    //��ȡָ����ʽ�Ļ�������ϢSFieldInfo*����ָ��
    return pFmt->GetFieldsArray();
}
/************ End of The GetFieldsArray Defining ***************/


/************ The SetDistingCondition Defining *****************/
// ��������������ָ����ʽ��ΪsFmtName�ĸ�ʽ��ĸ�ʽʶ������
//
// ���������
//           sFmtName��ָ���ĸ�ʽ��
//  sDistingCondition���ø�ʽ��ʶ���ʽ��Ϊ��׺���ʽ
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void IFormatCfgImp::SetDistingCondition(const STRING& sFmtName, 
                                        const STRING& sDistingCondition)
{
    //ȷ������Ĳ�����ȷ
    if((sFmtName.length() == 0) || (sDistingCondition.length() == 0))
    {
        return;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return;
    }

    //��ȡָ���ĸ�ʽ����ָ��
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return;  //��ȡָ����ʽ����ָ��ʧ��
    }

    //����ָ����ʽ�ĸ�ʽʶ������
    pFmt->SetDistingCondition(sDistingCondition);
}
/************ End of The SetDistingCondition Defining **********/


/************ The GetDistingCondition Defining *****************/
// �������������ָ����ʽ��ΪsFmtName�ĸ�ʽ��ĸ�ʽʶ��������
//
// ���������
//          sFmtName��ָ���ĸ�ʽ��
//
// ���������
// sDistingCondition����õ�ʶ���ʽ��Ϊ��׺���ʽ
//
//   ����ֵ����
//
/***************************************************************/
void IFormatCfgImp::GetDistingCondition(const STRING& sFmtName, 
                                        STRING& sDistingCondition)
{
    //��ʼ���������
    sDistingCondition = "";

    //ȷ������Ĳ�����ȷ
    if(sFmtName.length() == 0)
    {
        return;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return;
    }

    //��ȡָ���ĸ�ʽ����ָ��
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return;  //��ȡָ����ʽ����ָ��ʧ��
    }

    //��ȡָ����ʽ�ĸ�ʽʶ������
    pFmt->GetDistingCondition(sDistingCondition);
}
/************ End of The GetDistingCondition Defining **********/


/************ The DelFormat Defining ***************************/
// ����������ɾ��ָ����ʽ��ΪsFmtName�Ļ�����ʽ
//
// ���������
//   sFmtName��ָ���ĸ�ʽ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::DelFormat(const STRING& sFmtName)
{
    //ȷ������Ĳ�����ȷ
    if(sFmtName.length() == 0)
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return -1;
    }

    //ɾ��ָ���ĸ�ʽ��������ִ�н��
    return m_pIGWB->m_pFmtLib->DelFormat(sFmtName);
}
/************ End of The DelFormat Defining ********************/


/************ The GetFormats Defining **************************/
// ������������ø�ʽ�������и�ʽ��
//
// �����������
//
// ���������
//    Formats��������и�ʽ���б�
//
//   ����ֵ����ʽ������
//
/***************************************************************/
UINT4 IFormatCfgImp::GetFormats(LIST<STRING>& Formats)
{
    //��ʼ���������
    Formats.clear();

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return 0;
    }

    //��ø�ʽ�������и�ʽ���������ظ�ʽ������
    return m_pIGWB->m_pFmtLib->GetFormats(Formats);
}
/************ End of The GetFormats Defining *******************/


/************ The GetFormatGen Defining ************************/
// �������������ָ����ʽ���ĸ�ʽ���һ����Ϣ������øø�ʽ������
//           ��Դ��ʽ��
//
// ���������
//    sFmtName��ָ���ĸ�ʽ��
//
// ���������
//     sFmtDes���ø�ʽ������
// sSrcFmtName���ø�ʽ��Դ��ʽ��
//
//   ����ֵ����
//
/***************************************************************/
void IFormatCfgImp::GetFormatGen(const STRING& sFmtName, STRING& sFmtDes, 
                                 STRING& sSrcFmtName)
{
    //��ʼ���������
    sFmtDes = "";
    sSrcFmtName = "";

    //ȷ������Ĳ�����ȷ
    if(sFmtName.length() == 0)
    {
        return;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pFmtLib == NULL))
    {
        return;
    }

    //��ȡָ���ĸ�ʽ����ָ��
    CFormat* pFmt = m_pIGWB->m_pFmtLib->GetFormat(sFmtName);
    if(pFmt == NULL)
    {
        return;  //��ȡָ����ʽ����ָ��ʧ��
    }

    //��ȡָ����ʽ�ĸ�ʽ������ԭ��ʽ��
    pFmt->GetFormatGen(sFmtDes, sSrcFmtName);
}
/************ End of The GetFormatGen Defining *****************/


/************ The AddAccessPoint Defining **********************/
// ����������ָ��������ID����������ƣ�����һ�������ĸ�ʽ������Ϣ
//
// ���������
//      nAPID��ָ���Ľ����ID
//    sAPName��ָ���Ľ���������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::AddAccessPoint(const UINT4 nAPID, const STRING& sAPName)
{
    //ȷ�����������ȷ
    if((nAPID == 0) || (sAPName.length() == 0))
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL) || (m_pIGWB->m_pFmtLib))
    {
        return -1;
    }

    //�������н�����б�
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //������Ѿ�����
            return -1;
        }

        it++;
    }

    //����㲻���ڣ������µĽ����
    CAccessPoint* pAP = new CAccessPoint(m_pIGWB->m_sFmtRootDir, nAPID,
                                         m_pIGWB->m_pFmtLib);
    pAP->SetAPName(sAPName);
    int nRet = pAP->Create();

    //���������������������ļ�ʧ��
    if(nRet != 0)
    {
        delete pAP;
        return -1;
    }

    //�����������������ļ��ɹ�
    m_pIGWB->m_pAPs->push_back(pAP);

    return 0;
}
/************ End of The AddAccessPoint Defining ***************/


/************ The AddOrgBrowseNames Defining *******************/
// ����������ָ��������ID��׷�Ӹý�����ԭʼ���������ʽ��
//
// ���������
//      nAPID��ָ���Ľ����ID
//     nCount�����õ������ʽ������
//  szFormats�������ʽ������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::AddOrgBrowseNames(const UINT4 nAPID, const UINT4 nCount, 
                                     const char* szFormats[])
{
    //ȷ�����������ȷ
    if((nAPID == 0) || (nCount == 0) || (szFormats == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nCount; i++)
    {
        if(szFormats[i] == NULL)
        {
            return -1;
        }
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //�������н�����б�
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //����ҵ�ָ�������
            return (*it)->AddBrowseNames(nCount, szFormats);
        }

        it++;
    }

    return -1;
}
/************ End of The AddOrgBrowseNames Defining ************/


/************ The GetOrgBrowseNames Defining *******************/
// ����������ָ��������ID����øý��������ԭʼ���������ʽ��
//
// ���������
//      nAPID��ָ���Ľ����ID
//
// ���������
//    Formats����������ʽ���б�
//
//   ����ֵ��ԭʼ���������ʽ������
//
/***************************************************************/
UINT4 IFormatCfgImp::GetOrgBrowseNames(const UINT4 nAPID, LIST<STRING>& Formats)
{
    //��ʼ���������
    Formats.clear();

    //��������ͨ��ID���㣬������ȱʡͨ��
    if(nAPID == 0)
    {
        return 0;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //�������н����
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ������Ƿ����ָ���Ľ����
        if((*it)->GetAPID() == nAPID)
        {
            LIST<STRING> Descs;  //�����ʽ�����б����

            //����ҵ�ָ���Ľ������󣬻�ȡ��ԭʼ���������ʽ
            //�����б�͸�ʽ�����б�
            return (*it)->GetOrigBrowseNames(Formats, Descs);
        }

        it++;
    }

    //���û���ҵ�ָ����������
    return 0;
}
/************ End of The GetOrgBrowseNames Defining ************/


/************ The AddChannel Defining **************************/
// ������������ָ�������������ָ����ͨ��
//
// ���������
//      nAPID��ָ���Ľ����ID
//     nChlID�������ӵ�ͨ����ID
//   sChlName�������ӵ�ͨ������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::AddChannel(const UINT4 nAPID, const UINT4 nChlID , 
                              const STRING& sChlName)
{
    //ȷ�����������ȷ
    if((nAPID == 0) || (nChlID == 0) || (sChlName.length() == 0))
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //�������н����
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //����ҵ�ָ���Ľ���㣬���ڴ˽����������ָ����ͨ��
            return (*it)->AddChannel(nChlID,  sChlName);
        }

        it++;
    }

    return -1;
}
/************ End of The AddChannel Defining *******************/


/************ The SetDisCondition Defining *********************/
// ����������ָ��������ID��ͨ����ţ����ø�ͨ�������һ������
//           ����Դ��ʽ�ķּ�����
//
// ���������
//         nAPID��ָ���Ľ����ID
//        nChlID��ָ����ͨ����ID
//    sDisFormat����ͨ����һ�����ջ�����ʽ��Դ��ʽ�ĸ�ʽ��
// sDisCondition���ּ�����
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::SetDisCondition(const UINT4 nAPID, const UINT4 nChlID , 
                                   const STRING& sDisFormat,
                                   const STRING& sDisCondition)
{
    //ȷ�����������ȷ
    if((nAPID == 0) || (nChlID == 0) || (sDisFormat.length() == 0) 
        || (sDisCondition.length() == 0))
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //�������н����
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //����ҵ�ָ���Ľ����
            return (*it)->SetDistingCondition(nChlID, sDisFormat, sDisCondition);
        }

        it++;
    }

    return -1;
}
/************ End of The SetDisCondition Defining **************/


/************ The SetChlFormats Defining ***********************/
// ����������ָ�������ID��ͨ����ţ���׷�ӵķ�ʽ���ø�ͨ��������
//           ������ʽ�������ջ��������ʽ��
//
// ���������
//                nAPID��ָ���Ľ����ID
//               nChlID��ָ����ͨ����ID
//       nFinalFmtCount���������õ����ջ�����ʽ����
//       szFinalFormats�����ջ�����ʽ�����飬������nFinalFmtCount��Ա��
//                       ÿһ����Ա����һ�����ջ�����ʽ��
// nFinalBrowseFmtCount���������õ����ջ��������ʽ����
//    szFinalBrowseFmts�����ջ��������ʽ�����飬������nFinalBrowseFmtCount
//                       ����Ա��ÿһ����Ա����һ�����ջ��������ʽ��
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::SetChlFormats(const UINT4 nAPID, const UINT4 nChlID , 
                                 const UINT4 nFinalFmtCount, 
                                 const char* szFinalFormats[], 
                                 const UINT4 nFinalBrowseFmtCount, 
                                 const char* szFinalBrowseFmts[])
{
    //ȷ�����������ȷ
    if((nAPID == 0) || (nChlID == 0) || (nFinalFmtCount == 0) || (szFinalFormats == NULL)
       || (nFinalBrowseFmtCount == 0) || (szFinalBrowseFmts == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nFinalFmtCount; i++)
    {
        if(szFinalFormats[i] == NULL)
        {
            return -1;
        }
    }
    for(int j = 0; j < nFinalBrowseFmtCount; j++)
    {
        if(szFinalBrowseFmts[j] == NULL)
        {
            return -1;
        }
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //�������н����
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //����ҵ�ָ���Ľ����
            return (*it)->SetChlFormats(nChlID,  nFinalFmtCount, szFinalFormats,
                                        nFinalBrowseFmtCount, szFinalBrowseFmts);
        }

        it++;
    }

    return -1;
}
/************ End of The SetChlFormats Defining ****************/


/************ The GetChlFormats Defining ***********************/
// ����������ָ�������ID��ͨ����ţ���ø�ͨ�����е����ջ�����ʽ��
//           �����ջ��������ʽ��
//
// ���������
//       nAPID��ָ���Ľ����ID
//      nChlID��ָ����ͨ����ID
//
// ���������
//          FinalFmts����õ����ջ�����ʽ���б�
// FinalBrowseFormats����õ����ջ��������ʽ���б�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::GetChlFormats(const UINT4 nAPID, const UINT4 nChlID, 
                                 LIST<STRING>& FinalFmts,
                                 LIST<STRING>& FinalBrowseFormats)
{
    //��ʼ���������
    FinalFmts.clear();
    FinalBrowseFormats.clear();

    //ȷ�����������ȷ
    if((nAPID == 0) || (nChlID == 0))
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //�������н����
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //����ҵ�ָ���Ľ����
            return (*it)->GetChlFormats(nChlID, FinalFmts, FinalBrowseFormats);
        }

        it++;
    }

    return -1;
}
/************ End of The GetChlFormats Defining ****************/


/************ The DelChannel Defining **************************/
// ����������ָ�������ID��ͨ����ţ�ɾ����ͨ����������Ϣ
//
// ���������
//       nAPID��ָ���Ľ����ID
//      nChlID��ָ����ͨ����ID
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::DelChannel(const UINT4 nAPID, const UINT4 nChlID)
{
    //ȷ�����������ȷ
    if((nAPID == 0) || (nChlID == 0))
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //�������н����
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            return (*it)->DelChannel(nChlID);
        }

        it++;
    }

    return -1;
}
/************ End of The DelChannel Defining *******************/


/************ The GetChannels Defining *************************/
// ����������ָ�������ID����øý���������ͨ������
//
// ���������
//       nAPID��ָ���Ľ����ID
//
// ���������
//    ChlNames����õ�ͨ�������б�
//
//   ����ֵ��ͨ�����Ƹ���
//
/***************************************************************/
UINT4 IFormatCfgImp::GetChannels(const UINT4 nAPID, LIST<STRING>& ChlNames)
{
    //��ʼ���������
    ChlNames.clear();

    //ȷ�����������ȷ
    if(nAPID == 0)
    {
        return 0;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            return (*it)->GetChannelNames(ChlNames);
        }

        it++;
    }

    return 0;
}
/************ End of The GetChannels Defining ******************/


/************ The GetAPNames Defining **************************/
// ������������ø�ʽ�����н��������
//
// �����������
//
// ���������
//    APNames������������б�
//
//   ����ֵ����������Ƹ���
//
/***************************************************************/
UINT4 IFormatCfgImp::GetAPNames(LIST<STRING>& APNames)
{
    //��ʼ���������
    APNames.clear();

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
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


/************ The Commit Defining ******************************/
// ����������ʹ��һ��Commit������Commit����֮�����޸ĵ�������Ϣ
//           ���浽������
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::Commit(void)
{
    //�ύ��ʽ�������������Ϣ
    if(m_pIGWB->m_pFmtLib != NULL)
    {
        if(m_pIGWB->m_pFmtLib->Commit() != 0)
        {
            return -1;
        }
    }

    //�ύ���н���㼰�������ͨ����������Ϣ
    if(m_pIGWB->m_pAPs != NULL)
    {
        //�������н����
        LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
        while(it != m_pIGWB->m_pAPs->end())
        {
            //�ύ��ǰ����㼰�����ͨ����������Ϣ
            if((*it)->Commit() != 0)
            {
                return -1;
            }

            it++;
        }
    }

    return 0;
}
/************ End of The Commit Defining ***********************/


/************ The AddOrgFmtNames Defining **********************/
// ����������ָ��������ID��׷�Ӹý�����ԭʼ������ʽ��
//
// ���������
//      nAPID��ָ���Ľ����ID
//     nCount�����õĸ�ʽ������
//  szFormats����ʽ������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::AddOrgFmtNames(const UINT4 nAPID, const UINT4 nCount, 
                                  const char* szFormats[])
{
    //ȷ�����������ȷ
    if((nAPID == 0) || (nCount == 0) || (szFormats == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nCount; i++)
    {
        if(szFormats[i] == NULL)
        {
            return -1;
        }
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //�������н�����б�
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //����ҵ�ָ�������
            return (*it)->AddOrgFmtNames(nCount, szFormats);
        }

        it++;
    }

    return -1;
}
/************ End of The AddOrgFmtNames Defining ***************/


/************ The GetOrgFmtNames Defining **********************/
// ����������ָ��������ID����øý��������ԭʼ������ʽ��
//
// ���������
//      nAPID��ָ���Ľ����ID
//
// ���������
//    Formats����ø�ʽ���б�
//
//   ����ֵ��ԭʼ������ʽ������
//
/***************************************************************/
UINT4 IFormatCfgImp::GetOrgFmtNames(const UINT4 nAPID, LIST<STRING>& Formats)
{
    //��ʼ���������
    Formats.clear();

    //��������ͨ��ID���㣬������ȱʡͨ��
    if(nAPID == 0)
    {
        return 0;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return 0;
    }

    //�������н����
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        //�жϵ�ǰ������Ƿ����ָ���Ľ����
        if((*it)->GetAPID() == nAPID)
        {
            LIST<STRING> Descs;  //��ʽ�����б����

            //����ҵ�ָ���Ľ������󣬻�ȡ��ԭʼ������ʽ
            //�����б�͸�ʽ�����б�
            return (*it)->GetOrgFmtNames(Formats, Descs);
        }

        it++;
    }

    //���û���ҵ�ָ����������
    return 0;
}
/************ End of The GetOrgFmtNames Defining ***************/


/************ The DelAccessPoint Defining **********************/
// ����������ɾ��ָ��������������Ϣ
//
// ���������
//       nAPID��ָ���Ľ����ID
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int IFormatCfgImp::DelAccessPoint(UINT4 nAPID)
{
    //ȷ�����������ȷ
    if(nAPID == 0)
    {
        return -1;
    }

    //ȷ����Ӧ�ĳ�Ա������Ϊ��
    if((m_pIGWB == NULL) || (m_pIGWB->m_pAPs == NULL))
    {
        return -1;
    }

    //�������н����
    LIST<CAccessPoint*>::iterator it = m_pIGWB->m_pAPs->begin();
    while(it != m_pIGWB->m_pAPs->end())
    {
        if((*it)->GetAPID() == nAPID)
        {
            //����ҵ�ָ���Ľ����
            if((*it)->Delete() != 0)
            {
                return -1;
            }

            //�ӽ��������б���ɾ��ָ���Ľ�������
            delete *it;
            m_pIGWB->m_pAPs->erase(it);

            return 0;
        }

        it++;
    }

    return -1;
}
/************ End of The DelAccessPoint Defining ***************/
