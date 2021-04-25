/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CFormat3G
//      Author:  maimaoshi
//        Date:  2002.04.03
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "format_r98.h"
#include "asn1_r98/CDRF_2GCDR_Org.h"
#include "asn1_r98/CDRF_2GCDR_Fin.h"
#include "r98_cdr_browse.h"
#include "../format/condition.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export CFormat* CreateFormatObject(const char* szRootDir, const char* szFmtName)
{
    return new CFormatR98_760(szRootDir, szFmtName);
}

using namespace NS_R98_760;

/************ The CFormatR98_760 Defining ***************************/
// �������������캯��
//
// ���������
//   sRootDir����ʽ���ÿ��·��
//   sFmtName����ʽ��
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CFormatR98_760::CFormatR98_760(const STRING& sRootDir, const STRING& sFmtName)
          :CFormat(sRootDir, sFmtName)
{
	m_bIsDisFormat = FALSE;
	if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_CDR_GPRSFORMATNAME))
		m_nGprsFormatType = S_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), G_CDR_GPRSFORMATNAME))
		m_nGprsFormatType = G_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), M_CDR_GPRSFORMATNAME))
		m_nGprsFormatType = M_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_SMO_CDR_GPRSFORMATNAME))
		m_nGprsFormatType = S_SMO_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_SMT_CDR_GPRSFORMATNAME))
		m_nGprsFormatType = S_SMT_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), L_S_CDR_GPRSFORMATNAME))
		m_nGprsFormatType = L_S_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), L_G_CDR_GPRSFORMATNAME))
		m_nGprsFormatType = L_G_CDR_FORMATTYPE;
	//else if (!ACE_OS::strcasecmp(sFmtName.c_str(), L_M_CDR_GPRSFORMATNAME))
	//	m_nGprsFormatType = L_M_CDR_FORMATTYPE;
	else
	{
		if (sFmtName.find("GPRSBP") != -1)
		{
			m_bIsDisFormat = TRUE;  //���÷ּ��ʽ��־
		}
		m_nGprsFormatType = 0;
	}
    m_bIsMustMerge = FALSE;
    m_pQueryCondition->SetFlag(0);
}
/************ End of The CFormatR98_760 Defining ********************/


/************ The ~CFormatR98_760 Defining **************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CFormatR98_760::~CFormatR98_760()
{
}
/************ End of The ~CFormatR98_760 Defining *******************/


/************ The Init Defining ********************************/
// ������������ʼ������
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormatR98_760::Init(void)
{
    //���û���ĳ�ʼ������
    if(CFormat::Init() == -1)
    {
        return -1;
    }

    //��ȡ���ڲ�ѯ������������
    //ע�⣺�ڽ��и�ʽ������ʱ��GPRS������Ϊ��ѯ��������͸����ǹ̶��ģ�
    //�����ڻ������е��Ⱥ�˳��Ҳ�ǹ̶��ģ�������л�ȡ��ѯ��ֵ��˳��һ�£�
    SQUERY_COND_FIELDS_VALUE FV;
    SFieldInfo* pFI = NULL;
    for(int i = 0; i < m_uFieldNum; i++)
    {
        pFI = m_pFieldsArray[i];
        if(pFI->flgQuery)
        {
            FV.nFieldIndex = pFI->nFieldIndex;
            FV.sFieldName  = pFI->szFieldName;
            FV.sFieldValue = "";
            m_QueryCondFieldsValue.push_back(FV);
        }
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The InitFormatInfo Defining **********************/
// ��������������һ����ʽ����������ֽ�ƫ�ơ�λƫ���Լ��ֽ��ܳ��Ⱥ���
//
// �����������
//
// �����������
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormatR98_760::InitFormatInfo(void)
{
    //ȷ������ʽ�����������
    if(m_uFieldNum == 0)
    {
        return -1;
    }

    //���ñ���ʽ�����ֽڳ��ȣ�����Gprs�����Ǳ䳤�ģ��������ó���Ϊ��
    m_uFmtLen = 0;

    //��ʼ������ʽ�е�ÿһ������ֽ�ƫ������λƫ������
    //����Gprs�����Ǳ䳤�ģ�ÿһ����ĳ��Ȳ��̶�������ȫ������Ϊ��
    SFieldInfo*  pFI   = NULL;   //��ǰ������ָ��
    for(int i = 0; i < m_uFieldNum; i++)
    {
        //���õ�ǰ����ֽ�ƫ������λƫ����
        pFI = m_pFieldsArray[i];
        pFI->nByteOffset = 0;
        pFI->nBitOffset = 0;
    }

    return 0;
}
/************ End of The InitFormatInfo Defining ***************/


/************ The IsTheFormat Defining *************************/
// �����������жϴ����һ�Ż����Ƿ���ϱ���ʽ
//
// ���������
//     pABill������ĵ��Ż���
//       uLen��һ�Ż����ĳ��ȣ���pABill��ռ�ڴ��С
//
// �����������
//
//   ����ֵ�����ϱ���ʽ����0�������ϱ���ʽ����1��������-1
//
/***************************************************************/
int CFormatR98_760::IsTheFormat(const void* pABill,  const UINT4 uLen)
{
    //�����ϱ���ʽ
    if((pABill == NULL) || (uLen == 0))
    {
        return 1;
    }

    //ԭʼ�������͵�S-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_CDR_GPRSFORMATNAME) == 0)
    {
        Org_CallEventRecord* pFrontCDR = (Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == Org_sgsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //ԭʼ�������͵�G-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), G_CDR_GPRSFORMATNAME) == 0)
    {
        Org_CallEventRecord* pFrontCDR = (Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == Org_ggsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //ԭʼ�������͵�M-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), M_CDR_GPRSFORMATNAME) == 0)
    {
        Org_CallEventRecord* pFrontCDR = (Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == Org_sgsnMMRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //S_SMO_CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_SMO_CDR_GPRSFORMATNAME) == 0)
    {
        Org_CallEventRecord* pFrontCDR = (Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == Org_sgsnSMORecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //S_SMT_CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_SMT_CDR_GPRSFORMATNAME) == 0)
    {
        Org_CallEventRecord* pFrontCDR = (Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == Org_sgsnSMTRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //���ջ������͵�S-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), L_S_CDR_GPRSFORMATNAME) == 0)
    {
        Fin_CallEventRecord* pBackCDR = (Fin_CallEventRecord*)pABill;
        if(pBackCDR->choice == Fin_sgsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //���ջ������͵�G-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), L_G_CDR_GPRSFORMATNAME) == 0)
    {
        Fin_CallEventRecord* pBackCDR = (Fin_CallEventRecord*)pABill;
        if(pBackCDR->choice == Fin_ggsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    /*
    //���ջ������͵�M-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), L_M_CDR_GPRSFORMATNAME) == 0)
    {
        Fin_CallEventRecord* pBackCDR = (Fin_CallEventRecord*)pABill;
        if(pBackCDR->choice == Fin_sgsnMMRecord_chosen)
        {
            return 0;
        }		
        else

        {
            return 1;
        }
    }
    */

    //���л�����ʽ����
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), TMP_CDR_NAME) == 0)
    {
        //Do Nothing
        return 0;
    }
	if (m_sFmtName.find("GPRSBP") != -1)
	{
		return 0;	
	}

    //�����Ƿ��ĸ�ʽ����
    return -1;
}
/************ End of The IsTheFormat Defining ******************/


/************ The ConvertABill Defining ************************/
// �����������������һ�Ż���ת����Ŀ���ʽ
//
// ���������
//   pASrcBill������ĵ��Ż���
// uSrcBillLen��һ�Ż����ĳ��ȣ���pABill��ռ�ڴ��С
//
// ���������
//   pADestBill��ת����Ļ�����ָ����ڴ��ɱ��������䣬�ɱ������ĵ����߸����ͷ�
// uDestBillLen��ת����Ļ�������
//
//   ����ֵ����ȷת������0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormatR98_760::ConvertABill(const void* pASrcBill,
                                 const UINT4 uSrcBillLen, 
                                 void*& pADestBill,
                                 UINT4& uDestBillLen)
{
    //��ʼ���������
    pADestBill = NULL;
    uDestBillLen  = 0;

    //ȷ����������ĺϷ���
    if((pASrcBill == NULL) || (uSrcBillLen == 0))
    {
        return -1;
    }

    //��ȡ���ת���󻰵����ݵ�ȫ�ֻ�����
    CGlobalBuffer::acquire();
    char *pBuffer = CGlobalBuffer::GetBillBuffer();
    Org_CallEventRecord *pTemp = (Org_CallEventRecord*)pASrcBill;

    int nLen = 0;
    try
    {
        //�ѵ�ǰ����ת���������ʽ���ı�
        nLen = GetAllFieldTextValueOfGPRSCDR((char*)(&pTemp->u),
                                             m_nGprsFormatType,
                                             pBuffer,
                                             _BROWSEBUFFERLENGTH);
    }
    catch (...)
    {
        CGlobalBuffer::release();
        return -1;
    }

    //���ת���ɹ�
    if (nLen > 0)
    {
        uDestBillLen = nLen;
        pADestBill = new BYTE[nLen + 1];
        memcpy(pADestBill, pBuffer, (nLen + 1));
        CGlobalBuffer::release();
    }

    //���ת��ʧ��
    else
    {
        CGlobalBuffer::release();
        return -1;
    }

    return 0;
}
/************ End of The ConvertABill Defining *****************/


/************ The GetFieldValueByName Defining *****************/
// �������������ݻ���������һ�Ż����л�ø����ֵ
//
// ���������
//     pABill������ĵ��Ż���
//   bThisFmt������Ļ����Ƿ���ϱ���ʽ��0��ʾ���ϱ���ʽ��
//             ��������Ҫת����Ŀ���ʽ
//     uIndex����ʽ��������SFieldInfo�ṹ�е�nFieldIndex��Ӧ
//   nIsQuery���Ƿ��ѯ��־��1��ʾ�ú���Ϊ��ѯ����ת����
//             0��ʾ����Ϊ��ѯ����ת��
//
// ���������
// pFieldValue����õ���ֵ
//        uLen�����򳤶�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormatR98_760::GetFieldValueByName(const void* pABill,
                                        const int bThisFmt,
                                        const STRING& sFieldName,  
                                        void*& pFieldValue,
                                        UINT4& uLen,
                                        int nIsQuery)
{
    //��ʼ����������ͱ���
    pFieldValue = NULL;
    uLen = 0;

    //ȷ�������������ȷ��
    if(sFieldName.length() == 0)
    {
        return -1;
    }

	if (m_bIsDisFormat)
	{
		return GetFieldValueByNameEx(pABill, uLen, sFieldName, pFieldValue);
	}

    //��ȡ��Ӧ��������Ӧ����ֵ
    QUERY_COND_FIELDS_VALUE::iterator it = m_QueryCondFieldsValue.begin();
    while(it != m_QueryCondFieldsValue.end())
    {
        if(!(it->sFieldName.compare(sFieldName)))
        {
            uLen = it->sFieldValue.length();
            pFieldValue = new BYTE[uLen + 1];
            strcpy((char*)pFieldValue, it->sFieldValue.c_str());

            return 0;
        }

        it++;
    }

    return -1;
}
/************ End of The GetFieldValueByName Defining **********/

/***************************************************************/
// �������������ݻ���������һ�Ż����л�ø����ֵ
//
// ���������
//     pABill������ĵ��Ż���
//   bThisFmt������Ļ����Ƿ���ϱ���ʽ��0��ʾ���ϱ���ʽ��
//             ��������Ҫת����Ŀ���ʽ
//     uIndex����ʽ��������SFieldInfo�ṹ�е�nFieldIndex��Ӧ
//   nIsQuery���Ƿ��ѯ��־��1��ʾ�ú���Ϊ��ѯ����ת����
//             0��ʾ����Ϊ��ѯ����ת��
//
// ���������
// pFieldValue����õ���ֵ
//        uLen�����򳤶�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormatR98_760::GetFieldContentByIndex(const void* pABill,
                                           const int bThisFmt,
                                           UINT2 nFieldIndex,
                                           void* pFieldValue,
                                           UINT4& uLen,
                                           int nIsQuery)
{
    if (m_bIsDisFormat)
    {
        //��ȡ��Ӧ��������Ӧ����ֵ
        return GetFieldContentByNameEx(pABill,
                                       uLen,
                                       m_pFieldsArray[nFieldIndex]->szFieldName,
                                       pFieldValue);
    }

    //��ȡ��Ӧ��������Ӧ����ֵ
    QUERY_COND_FIELDS_VALUE::iterator it = m_QueryCondFieldsValue.begin();
    while(it != m_QueryCondFieldsValue.end())
    {
        if((it->nFieldIndex == nFieldIndex))
        {
            uLen = it->sFieldValue.length();
            strcpy((char*)pFieldValue, it->sFieldValue.c_str());            
            return 0;
        }
        
        it++;
    }
    
    return -1;

}

/************ The IsTheQueryCondition Defining *****************/
// �����������ж�һ�Ż����Ƿ����ָ���Ĳ�ѯ����
//
// ���������
//   pASrcBill������ĵ��Ż���
//  sQueryCond����ѯ��������׺���ʽ
//
// �����������
//
//   ����ֵ�����ϸ�ʽ����0�������Ϸ���1��������-1
//
/***************************************************************/
int CFormatR98_760::IsTheQueryCondition(const void* pASrcBill,
                                        const STRING& sQueryCond)
{
    SVariant RetVar;    //�������ʽ����������

    //ȷ�������������ȷ��
    if((pASrcBill == NULL) || ((m_sQueryCond.length() == 0)
        && (sQueryCond.length() == 0)))
    {
        return -1;
    }

    //����������ʽ������Ϊ�գ�����׺���ʽת��Ϊ��׺���ʽ
    if(sQueryCond.length() != 0)
    {
        if(m_pQueryCondition->Infix2Postfix(sQueryCond.c_str()) == -1)
        {
            //assert(0);
            return -1;
        }
        m_sQueryCond = sQueryCond;
    }

    //��ȡ��ѯ�������ֵ
    if(GetQueryCondFieldsValue(pASrcBill) == -1)
    {
        return -1;
    }

    //�����������ʽ
    if(m_pQueryCondition->Caculate(pASrcBill, m_uFmtLen, RetVar) == -1)
    {
        //assert(0);
        return -1;
    }

    //����������
    if(RetVar.n4Value == 0)
    {
        return 1;
    }

    //��������
    return 0;
}
/************ End of The IsTheQueryCondition Defining **********/


/************ The GetQueryCondFieldsValue Defining *************/
// ������������ȡ��ѯ������ֵ
//
// ���������
//     pABill������ĵ��Ż���
//
// �����������
//
//   ����ֵ���ɹ�����0��������-1
//
/***************************************************************/
int CFormatR98_760::GetQueryCondFieldsValue(const void* pABill)
{
    //��ȡȫ�ֻ�����
    CGlobalBuffer::acquire();
    char *pBuffer = CGlobalBuffer::GetBillBuffer();
    Org_CallEventRecord *pTemp = (Org_CallEventRecord*)pABill;

    int nLen = 0;
    try
    {
        //��ȡ��Ϊ��ѯ�������ֵ
        nLen = GetQueryFieldTextValueOfGPRSCDR((char*)(&pTemp->u),
                                               m_nGprsFormatType,
                                               pBuffer,
                                               _BROWSEBUFFERLENGTH,
                                               &m_QueryCondFieldsValue);
    }
    catch (...)
    {
        CGlobalBuffer::release();
        return -1;
    }

    //�����ȡ��ѯ������ֵʧ��
    if (nLen <= 0)
    {
        CGlobalBuffer::release();
        return -1;
    }

/*
	//��ֻ�����ע:�������Լ����ָ���Ϊ'^'
	char *ss0 = NULL;
    char *ss1 = NULL;
    int i = 0;
	ss0 = pBuffer;
	while ((ss1 = strchr(ss0, '^')) != NULL)
	{
		*ss1 = 0;

        //����ֵ����Ӧ��������Ӧ������
        //ע�⣺�ڽ��и�ʽ������ʱ��GPRS������Ϊ��ѯ��������͸����ǹ̶��ģ�
        //�����ڻ������е��Ⱥ�˳��Ҳ�ǹ̶��ģ�������л�ȡ��ѯ��ֵ��˳��һ�£�
		m_QueryCondFieldsValue[i++].sFieldValue = ss0;

		ss0 = ss1+1;
		*ss1 = '^';
    }

    //�����ֵ�ַ���������'^'��β�����ȡ���һ����ֵ
    if(*ss0 != '\0')
    {
        m_QueryCondFieldsValue[i++].sFieldValue = ss0;
    }
*/
    CGlobalBuffer::release();

    return 0;
}
/************ End of The GetQueryCondFieldsValue Defining ******/


/************ The GetFieldValueByNameEx Defining ************************/
// ������������������������ֵ�����ڴ���GPRS�ּ�
//
// ���������
//     pABill����������һ�Ż�������ָ��
//       uLen��pABill�����ݳ���
// sFieldName������
//
// ���������
//pFieldValue����õ���ֵ
//       uLen�����򳤶�
//
//   ����ֵ�����ϱ�ͨ������0�����򷵻�-1
//
/***************************************************************/
int CFormatR98_760::GetFieldValueByNameEx(const void* pABill,
                                          UINT4& uLen,
                                          const STRING& sFieldName,
                                          void*& pFieldValue)
{
    if(sFieldName == "recordType")
	{
		if (m_bIsMustMerge)
		{
			uLen = sizeof(Fin_CallEventRecordType);			
	    }
		else
		{
			uLen = sizeof(Org_CallEventRecordType);			
		}
		pFieldValue = new BYTE[uLen];
		memset(pFieldValue, 0, uLen);
    }
    else
    {
        uLen = 0;
        return -1;
    }

    int nRet = GetFieldContentByNameEx(pABill, uLen, sFieldName, pFieldValue);
    if(0 != nRet)
    {
	    if (pFieldValue != NULL)
	    {
		    delete[] pFieldValue;
		    pFieldValue = NULL;
	    }
        uLen = 0;
    }

    return nRet;
}

//���ݻ���������һ�Ż����л�ø����ֵ(���ⲿ�����ڴ�, ����r98�ּ�)
int CFormatR98_760::GetFieldContentByNameEx(const void* pABill,
                                            UINT4& uLen,
                                            const STRING& sFieldName,
                                            void* pFieldValue)
{
    //�жϷּ�����,�����ǰ����Ǹ��ֶηּ�
    if(sFieldName == "recordType")
	{
		//����ǰ���recordType�ּ�
		//recordType == 18 ---SCDR
		//recordType == 19 ---GCDR
		//recordType == 20 ---MCDR
		//recordType == 21 ---SMOCDR
		//recordType == 22 ---SMTCDR
		if (m_bIsMustMerge)
		{
			Fin_CallEventRecord* pBackCDR = (Fin_CallEventRecord*)pABill;
            uLen = sizeof(Fin_CallEventRecordType);
			switch (pBackCDR->choice)
			{
			case 1:
				memcpy(pFieldValue, &pBackCDR->u.sgsnPDPRecord.recordType, uLen);
				return 0;
			case 2:
				memcpy(pFieldValue, &pBackCDR->u.ggsnPDPRecord.recordType, uLen);
				return 0;
			case 3:
				memcpy(pFieldValue, &pBackCDR->u.sgsnMMRecord.recordType, uLen);
				return 0;
			case 4:
				memcpy(pFieldValue, &pBackCDR->u.sgsnSMORecord.recordType, uLen);
				return 0;			
			case 5:
				memcpy(pFieldValue, &pBackCDR->u.sgsnSMTRecord.recordType, uLen);
				return 0;		
			default:
				break;
			}		
	    }
		else
		{
			Org_CallEventRecord* pBackCDR = (Org_CallEventRecord*)pABill;
            uLen = sizeof(Org_CallEventRecordType);
			switch (pBackCDR->choice)
			{
			case 1:
				memcpy(pFieldValue, &pBackCDR->u.sgsnPDPRecord.recordType, uLen);
				return 0;
			case 2:
				memcpy(pFieldValue, &pBackCDR->u.ggsnPDPRecord.recordType, uLen);
				return 0;
			case 3:
				memcpy(pFieldValue, &pBackCDR->u.sgsnMMRecord.recordType, uLen);
				return 0;
			case 4:
				memcpy(pFieldValue, &pBackCDR->u.sgsnSMORecord.recordType, uLen);
				return 0;			
			case 5:
				memcpy(pFieldValue, &pBackCDR->u.sgsnSMTRecord.recordType, uLen);
				return 0;		
			default:
				break;
			}		
		}		
	}

	return -1;	
}
/************ End of The GetFieldValueByNameEx Defining ***********************/

