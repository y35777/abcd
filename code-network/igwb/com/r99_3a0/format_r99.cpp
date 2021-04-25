/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CFormatR99
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "format_r99.h"
#include "asn1_r99/CDRF_CMCC_3GCDR.h"
#include "asn1_r99/CDRF_CMCC_3GCDR_CG.h"
#include "r99_cdr_browse.h"
#include "../format/condition.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export CFormat* CreateFormatObject(const char* szRootDir, const char* szFmtName)
{
    return new CFormatR99(szRootDir, szFmtName);
}


/************ The CFormatR99 Defining ***************************/
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
CFormatR99::CFormatR99(const STRING& sRootDir, const STRING& sFmtName)
          :CFormat(sRootDir, sFmtName)
{
    m_bIsDisFormat = FALSE;
	if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_CDR_3GFORMATNAME))
		m_n3GFormatType = S_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), G_CDR_3GFORMATNAME))
		m_n3GFormatType = G_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), M_CDR_3GFORMATNAME))
		m_n3GFormatType = M_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_SMO_CDR_3GFORMATNAME))
		m_n3GFormatType = S_SMO_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_SMT_CDR_3GFORMATNAME))
		m_n3GFormatType = S_SMT_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), L_S_CDR_3GFORMATNAME))
		m_n3GFormatType = L_S_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), L_G_CDR_3GFORMATNAME))
		m_n3GFormatType = L_G_CDR_FORMATTYPE;
	/*
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), L_M_CDR_3GFORMATNAME))
        m_n3GFormatType = L_M_CDR_FORMATTYPE;
    */
    else
    {
        if (sFmtName.find("3GBP") != -1)
        {
            m_bIsDisFormat = TRUE;  //���÷ּ��ʽ��־
        }

        m_n3GFormatType = 0;
    }
    m_bIsMustMerge = FALSE;

    m_pQueryCondition->SetFlag(0);
}
/************ End of The CFormatR99 Defining ********************/


/************ The ~CFormatR99 Defining **************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CFormatR99::~CFormatR99()
{
}
/************ End of The ~CFormatR99 Defining *******************/


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
int CFormatR99::Init(void)
{
    //���û���ĳ�ʼ������
    if(CFormat::Init() == -1)
    {
        return -1;
    }

    //��ȡ���ڲ�ѯ������������
    //ע�⣺�ڽ��и�ʽ������ʱ��3G������Ϊ��ѯ��������͸����ǹ̶��ģ�
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
int CFormatR99::InitFormatInfo(void)
{
    //ȷ������ʽ�����������
    if(m_uFieldNum == 0)
    {
        return -1;
    }

    //���ñ���ʽ�����ֽڳ��ȣ�����3G�����Ǳ䳤�ģ��������ó���Ϊ��
    m_uFmtLen = 0;

    //��ʼ������ʽ�е�ÿһ������ֽ�ƫ������λƫ������
    //����3G�����Ǳ䳤�ģ�ÿһ����ĳ��Ȳ��̶�������ȫ������Ϊ��
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
int CFormatR99::IsTheFormat(const void* pABill,  const UINT4 uLen)
{
    //�����ϱ���ʽ
    if((pABill == NULL) || (uLen == 0))
    {
        return 1;
    }

    //ԭʼ�������͵�S-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_CDR_3GFORMATNAME) == 0)
    {
        CMCC_3GCDR_CallEventRecord* pFrontCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
        if(pFrontCDR->choice == CMCC_3GCDR_sgsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //ԭʼ�������͵�G-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), G_CDR_3GFORMATNAME) == 0)
    {
        CMCC_3GCDR_CallEventRecord* pFrontCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
        if(pFrontCDR->choice == CMCC_3GCDR_ggsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //ԭʼ�������͵�M-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), M_CDR_3GFORMATNAME) == 0)
    {
        CMCC_3GCDR_CallEventRecord* pFrontCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
        if(pFrontCDR->choice == CMCC_3GCDR_sgsnMMRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //S_SMO_CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_SMO_CDR_3GFORMATNAME) == 0)
    {
        CMCC_3GCDR_CallEventRecord* pFrontCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
        if(pFrontCDR->choice == CMCC_3GCDR_sgsnSMORecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //S_SMT_CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_SMT_CDR_3GFORMATNAME) == 0)
    {
        CMCC_3GCDR_CallEventRecord* pFrontCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
        if(pFrontCDR->choice == CMCC_3GCDR_sgsnSMTRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //���ջ������͵�S-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), L_S_CDR_3GFORMATNAME) == 0)
    {
        CMCC_3GCDR_CG_CallEventRecord* pBackCDR = (CMCC_3GCDR_CG_CallEventRecord*)pABill;
        if(pBackCDR->choice == CMCC_3GCDR_CG_sgsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //���ջ������͵�G-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), L_G_CDR_3GFORMATNAME) == 0)
    {
        CMCC_3GCDR_CG_CallEventRecord* pBackCDR = (CMCC_3GCDR_CG_CallEventRecord*)pABill;
        if(pBackCDR->choice == CMCC_3GCDR_CG_ggsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    //���л�����ʽ����
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), W_TMP_CDR_NAME) == 0)
    {
        //Do Nothing
        return 0;
    }

    if (m_sFmtName.find("3GBP") != -1)
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
int CFormatR99::ConvertABill(const void* pASrcBill,
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
    CMCC_3GCDR_CallEventRecord *pTemp = (CMCC_3GCDR_CallEventRecord*)pASrcBill;

    int nLen = 0;
    try
    {
        //�ѵ�ǰ����ת���������ʽ���ı�
        nLen = GetAllFieldTextValueOfR99CDR((char*)(&pTemp->u),
                                             m_n3GFormatType,
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
        //strcpy((char*)pADestBill, pBuffer);
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
int CFormatR99::GetFieldValueByName(const void* pABill,
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
int CFormatR99::GetFieldContentByIndex(const void* pABill,
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
int CFormatR99::IsTheQueryCondition(const void* pASrcBill, const STRING& sQueryCond)
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
int CFormatR99::GetQueryCondFieldsValue(const void* pABill)
{
    //��ȡȫ�ֻ�����
    CGlobalBuffer::acquire();
    char *pBuffer = CGlobalBuffer::GetBillBuffer();
    CMCC_3GCDR_CallEventRecord *pTemp = (CMCC_3GCDR_CallEventRecord*)pABill;

    int nLen = 0;
    try
    {
        //��ȡ��Ϊ��ѯ�������ֵ
        nLen = GetQueryFieldTextValueOfR99CDR((char*)(&pTemp->u),
                                              m_n3GFormatType,
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
        //ע�⣺�ڽ��и�ʽ������ʱ��3G������Ϊ��ѯ��������͸����ǹ̶��ģ�
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
// ������������������������ֵ�����ڴ���3G�ּ�
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
int CFormatR99::GetFieldValueByNameEx(const void* pABill,
                                      UINT4& uLen,
                                      const STRING& sFieldName,
                                      void*& pFieldValue)
{
    if(sFieldName == "recordType")
	{
		if (m_bIsMustMerge)
		{
			uLen = sizeof(CMCC_3GCDR_CG_CallEventRecordType);
		}
		else
		{
			uLen = sizeof(CMCC_3GCDR_CallEventRecordType);
		}
		pFieldValue = new BYTE[uLen];
		memset(pFieldValue, 0, uLen);
    }
    else if(sFieldName == "chargingCharacteristics")
	{
		uLen = sizeof(BYTE);
		pFieldValue = new BYTE[uLen];
		memset(pFieldValue, 0, uLen);
    }
    else if(sFieldName == "sgsnPLMNIdentifier")
    {
        uLen = 3;
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

/***************************************************************/
// �������������ݻ���������һ�Ż����л�ø����ֵ(���ⲿ�����ڴ�, ����3G�ּ�)
//
// ���������
//     pABill����������һ�Ż�������ָ��
//       uLen��pABill�����ݳ���
// nFieldIndex:�����
//
// ���������
//pFieldValue����õ���ֵ
//       uLen�����򳤶�
//
//   ����ֵ�����ϱ�ͨ������0�����򷵻�-1
//
/***************************************************************/
/***************************************************************/
int CFormatR99::GetFieldContentByNameEx(const void* pABill,
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
            CMCC_3GCDR_CG_CallEventRecord* pBackCDR = (CMCC_3GCDR_CG_CallEventRecord*)pABill;
            uLen = sizeof(CMCC_3GCDR_CG_CallEventRecordType);
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
            CMCC_3GCDR_CallEventRecord* pBackCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
            uLen = sizeof(CMCC_3GCDR_CallEventRecordType);
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
    else if(sFieldName == "chargingCharacteristics")
    {
        uLen = sizeof(BYTE);
		BYTE nTemp = 0;
        //����ǰ���chargingCharacteristics�ּ�,value[0] & 0x0f
        //--  Bit 1: H (Hot billing)        := '00000001'B��1
        //--  Bit 2: F (Flat rate)          := '00000010'B��2
        //--  Bit 3: P (Prepaid service)    := '00000100'B��4
        //--  Bit 4: N (Normal billing)     := '00001000'B��8
        //--  Bit 5: - (Reserved, set to 0) := '00010000'B
        //--  Bit 6: - (Reserved, set to 0) := '00100000'B
        //--  Bit 7: - (Reserved, set to 0) := '01000000'B
        //--  Bit 8: - (Reserved, set to 0) := '10000000'B

        //���Ӷ�chargingCharacteristicsΪ������µķּ��ǣ���Normal billing����
        //Qiu Yiping 2003-07-24
        if (m_bIsMustMerge)
        {
            CMCC_3GCDR_CG_CallEventRecord* pBackCDR = (CMCC_3GCDR_CG_CallEventRecord*)pABill;

            switch (pBackCDR->choice)
            {
            case 1:
                if (pBackCDR->u.sgsnPDPRecord.bit_mask & CMCC_3GCDR_CG_SGSNPDPRecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.sgsnPDPRecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            case 2:
                if (pBackCDR->u.ggsnPDPRecord.bit_mask & CMCC_3GCDR_CG_GGSNPDPRecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.ggsnPDPRecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            case 3:
                if (pBackCDR->u.sgsnMMRecord.bit_mask & CMCC_3GCDR_CG_SGSNMMRecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.sgsnMMRecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            case 4:
                if (pBackCDR->u.sgsnSMORecord.bit_mask & CMCC_3GCDR_CG_SGSNSMORecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.sgsnSMORecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            case 5:
                if (pBackCDR->u.sgsnSMTRecord.bit_mask & CMCC_3GCDR_CG_SGSNSMTRecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.sgsnSMTRecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            default:
                break;
            }
        }
        else
        {
            CMCC_3GCDR_CallEventRecord* pBackCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
            switch (pBackCDR->choice)
            {
            case 1:
                if (pBackCDR->u.sgsnPDPRecord.bit_mask & CMCC_3GCDR_SGSNPDPRecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.sgsnPDPRecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            case 2:
                if (pBackCDR->u.ggsnPDPRecord.bit_mask & CMCC_3GCDR_GGSNPDPRecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.ggsnPDPRecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            case 3:
                if (pBackCDR->u.sgsnMMRecord.bit_mask & CMCC_3GCDR_SGSNMMRecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.sgsnMMRecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            case 4:
                if (pBackCDR->u.sgsnSMORecord.bit_mask & CMCC_3GCDR_SGSNSMORecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.sgsnSMORecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            case 5:
                if (pBackCDR->u.sgsnSMTRecord.bit_mask & CMCC_3GCDR_SGSNSMTRecord_chargingCharacteristics_present)
                {
                    nTemp = pBackCDR->u.sgsnSMTRecord.chargingCharacteristics.value[0] & 0x0f;
                }
                else
                {
                    nTemp = 0x08;   //���chargingCharacteristics��Ϊ�գ���Normal billing����
                }

                memcpy(pFieldValue, &nTemp, uLen);
                return 0;

            default:
                break;
            }
        }
    }
    else if (sFieldName == "sgsnPLMNIdentifier")
    {
        uLen = sizeof(BYTE);        
        char pBuf[48];
        memset(pBuf, 0, 48);
        
        if (m_bIsMustMerge)
        {
            //����ǰ���sgsnPLMNIdentifier�ּ�
            CMCC_3GCDR_CG_CallEventRecord* pBackCDR = (CMCC_3GCDR_CG_CallEventRecord*)pABill;
            
            switch (pBackCDR->choice)
            {    
            case 2:
                uLen = CvtBCDOctecToStr2((LPOCTECSTRING )&(pBackCDR->u.ggsnPDPRecord.sgsnPLMNIdentifier), 
                    pBuf, 48);                
                memcpy(pFieldValue, pBuf, uLen);
                return 0;              
                
            default:
                break;
            }
        }
        else
        {
            //����ǰ���sgsnPLMNIdentifier�ּ�
            CMCC_3GCDR_CallEventRecord* pBackCDR = (CMCC_3GCDR_CallEventRecord*)pABill;
            
            switch (pBackCDR->choice)
            {
            case 2:
                uLen = CvtBCDOctecToStr2((LPOCTECSTRING )&(pBackCDR->u.ggsnPDPRecord.sgsnPLMNIdentifier), 
                    pBuf, 48);                
                memcpy(pFieldValue, pBuf, uLen);
                return 0;    	    
                
            default:
                break;
            }
        }
    }    

    return -1;
}
/************ End of The GetFieldValueByNameEx Defining ***********************/

