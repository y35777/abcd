#include "format_r4.h"
#include "asn1_r4/CDRF_3GPPR4_Org.h"
#include "asn1_r4/CDRF_3GPPR4_Fin.h"
#include "r4_cdr_browse.h"
#include "../format/condition.h"
#include "../include/export_dll.h"

extern "C" IGWB_Svc_Export CFormat* CreateFormatObject(const char* szRootDir, const char* szFmtName)
{
    return new CFormatR4(szRootDir, szFmtName);
}

/************ The CFormat3G Defining ***************************/
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
CFormatR4::CFormatR4(const STRING& sRootDir, const STRING& sFmtName)
          :CFormat(sRootDir, sFmtName)
{
	m_bIsDisFormat = FALSE;
	if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_CDR_R4FORMATNAME))
		m_n3GR4FormatType = S_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), G_CDR_R4FORMATNAME))
		m_n3GR4FormatType = G_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), M_CDR_R4FORMATNAME))
		m_n3GR4FormatType = M_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_SMO_CDR_R4FORMATNAME))
		m_n3GR4FormatType = S_SMO_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), S_SMT_CDR_R4FORMATNAME))
		m_n3GR4FormatType = S_SMT_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), LCS_MT_CDR_R4FORMATNAME))
		m_n3GR4FormatType = LCS_MT_CDR;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), LCS_MO_CDR_R4FORMATNAME))
		m_n3GR4FormatType = LCS_MO_CDR;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), LCS_NI_CDR_R4FORMATNAME))
		m_n3GR4FormatType = LCS_NI_CDR;	
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), L_S_CDR_R4FORMATNAME))
		m_n3GR4FormatType = L_S_CDR_FORMATTYPE;
	else if (!ACE_OS::strcasecmp(sFmtName.c_str(), L_G_CDR_R4FORMATNAME))
		m_n3GR4FormatType = L_G_CDR_FORMATTYPE;	
	else
	{
		if (sFmtName.find("R4BP") != -1)
		{
			m_bIsDisFormat = TRUE;  //���÷ּ��ʽ��־
		}
			
		m_n3GR4FormatType = 0;
	}
    
    m_pQueryCondition->SetFlag(0);
}
/************ End of The CFormatR4 Defining ********************/


/************ The ~CFormatR4 Defining **************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CFormatR4::~CFormatR4()
{
}
/************ End of The ~CFormatR4 Defining *******************/


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
int CFormatR4::Init(void)
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
int CFormatR4::InitFormatInfo(void)
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
int CFormatR4::IsTheFormat(const void* pABill,  const UINT4 uLen)
{
    //�����ϱ���ʽ
    if((pABill == NULL) || (uLen == 0))
    {
        return 1;
    }

    //ԭʼ�������͵�S-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_CDR_R4FORMATNAME) == 0)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == R4_Org_sgsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //ԭʼ�������͵�G-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), G_CDR_R4FORMATNAME) == 0)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == R4_Org_ggsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //ԭʼ�������͵�M-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), M_CDR_R4FORMATNAME) == 0)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == R4_Org_sgsnMMRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //S_SMO_CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_SMO_CDR_R4FORMATNAME) == 0)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == R4_Org_sgsnSMORecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //r4-LCSMT
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), LCS_MT_CDR_R4FORMATNAME) == 0)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == R4_Org_sgsnLCTRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    
	//r4-LCSMO
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), LCS_MO_CDR_R4FORMATNAME) == 0)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == R4_Org_sgsnLCORecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }

    }

	//r4-LCSNI
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), LCS_NI_CDR_R4FORMATNAME) == 0)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == R4_Org_sgsnLCNRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
	
	
	
	//S_SMT_CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), S_SMT_CDR_R4FORMATNAME) == 0)
    {
        R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
        if(pFrontCDR->choice == R4_Org_sgsnSMTRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //���ջ������͵�S-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), L_S_CDR_R4FORMATNAME) == 0)
    {
        R4_Fin_CallEventRecord* pBackCDR = (R4_Fin_CallEventRecord*)pABill;
        if(pBackCDR->choice == R4_Fin_sgsnPDPRecord_chosen)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    //���ջ������͵�G-CDR
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), L_G_CDR_R4FORMATNAME) == 0)
    {
        R4_Fin_CallEventRecord* pBackCDR = (R4_Fin_CallEventRecord*)pABill;
        if(pBackCDR->choice == R4_Fin_ggsnPDPRecord_chosen)
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
	
	if (m_sFmtName.find("R4BP") != -1)
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
int CFormatR4::ConvertABill(const void* pASrcBill,
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
    R4_Org_CallEventRecord *pTemp = (R4_Org_CallEventRecord*)pASrcBill;

    int nLen = 0;
    try
    {
        //�ѵ�ǰ����ת���������ʽ���ı�
        nLen = GetAllFieldTextValueOf3GR4CDR((char*)(&pTemp->u), m_n3GR4FormatType, pBuffer, _BROWSEBUFFERLENGTH);
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
int CFormatR4::GetFieldValueByName(const void* pABill,
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

/************ The GetFieldContentByIndex Defining *****************/
// �������������ݻ�����������һ�Ż����л�ø����ֵ
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
// pFieldValue����õ���ֵ�����ⲿ�����ڴ�
//        uLen�����򳤶�
//
//   ����ֵ���ɹ�����0��ʧ�ܷ���-1
//
/***************************************************************/
int CFormatR4::GetFieldContentByIndex(const void* pABill,
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
int CFormatR4::IsTheQueryCondition(const void* pASrcBill, const STRING& sQueryCond)
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
int CFormatR4::GetQueryCondFieldsValue(const void* pABill)
{
    //��ȡȫ�ֻ�����
    CGlobalBuffer::acquire();
    char *pBuffer = CGlobalBuffer::GetBillBuffer();
    R4_Org_CallEventRecord *pTemp = (R4_Org_CallEventRecord*)pABill;

    int nLen = 0;
    try
    {
        //��ȡ��Ϊ��ѯ�������ֵ
        nLen = GetQueryFieldTextValueOf3GR4CDR((char*)(&pTemp->u),
                                               m_n3GR4FormatType,
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
int CFormatR4::GetFieldValueByNameEx(const void* pABill,
                                     UINT4& uLen,  
							         const STRING& sFieldName,
                                     void*& pFieldValue)
{
    if(sFieldName == "recordType")
	{
		if (m_bIsMustMerge)
		{
		    uLen = sizeof(R4_Fin_CallEventRecordType);
		}
		else
		{
		    uLen = sizeof(R4_Org_CallEventRecordType);
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
    else if(sFieldName == "servedMSISDN")
	{
		uLen = 9;
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
// ���ݻ���������һ�Ż����л�ø����ֵ(���ⲿ�����ڴ�, ����3G�ּ�)
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
int CFormatR4::GetFieldContentByNameEx(const void* pABill,
                                       UINT4& uLen,  
                                       const STRING& sFieldName,
                                       void* pFieldValue)
{
	//�жϷּ�����,�����ǰ����Ǹ��ֶηּ�
    if (sFieldName == "recordType")
	{
		//����ǰ���recordType�ּ�
        if (m_bIsMustMerge)
        {
    		//����ǰ���recordType�ּ�
    		R4_Fin_CallEventRecord* pBackCDR = (R4_Fin_CallEventRecord*)pABill;
            uLen = sizeof(R4_Fin_CallEventRecordType);
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
    		case 6:
    			memcpy(pFieldValue, &pBackCDR->u.sgsnLCTRecord.recordType, uLen);
    			return 0;	
    		case 7:
    			memcpy(pFieldValue, &pBackCDR->u.sgsnLCORecord.recordType, uLen);
    			return 0;	
    		case 8:
    			memcpy(pFieldValue, &pBackCDR->u.sgsnLCNRecord.recordType, uLen);
    			return 0;	
    		default:
    			break;
    		}		
    	}
        else
        {
    		//����ǰ���recordType�ּ�
    		R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
            uLen = sizeof(R4_Org_CallEventRecordType);
    		switch (pFrontCDR->choice)
    		{
    		case 1:
    			memcpy(pFieldValue, &pFrontCDR->u.sgsnPDPRecord.recordType, uLen);
    			return 0;
    		case 2:
    			memcpy(pFieldValue, &pFrontCDR->u.ggsnPDPRecord.recordType, uLen);
    			return 0;
    		case 3:
    			memcpy(pFieldValue, &pFrontCDR->u.sgsnMMRecord.recordType, uLen);
    			return 0;
    		case 4:
    			memcpy(pFieldValue, &pFrontCDR->u.sgsnSMORecord.recordType, uLen);
    			return 0;			
    		case 5:
    			memcpy(pFieldValue, &pFrontCDR->u.sgsnSMTRecord.recordType, uLen);
    			return 0;
    		case 6:
    			memcpy(pFieldValue, &pFrontCDR->u.sgsnLCTRecord.recordType, uLen);
    			return 0;	
    		case 7:
    			memcpy(pFieldValue, &pFrontCDR->u.sgsnLCORecord.recordType, uLen);
    			return 0;	
    		case 8:
    			memcpy(pFieldValue, &pFrontCDR->u.sgsnLCNRecord.recordType, uLen); 
    			return 0;	
    		default:
    			break;
    		}		
        }
    	
        return -1;
	
	}
    else if (sFieldName == "chargingCharacteristics")
	{
		BYTE nTemp = 0;
        uLen = sizeof(BYTE);

        if (m_bIsMustMerge)
        {
    		//����ǰ���chargingCharacteristics�ּ�
    		R4_Fin_CallEventRecord* pBackCDR = (R4_Fin_CallEventRecord*)pABill;
    
    		switch (pBackCDR->choice)
    		{
    		case 1:
    			nTemp = pBackCDR->u.sgsnPDPRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;				
    
            case 2:
    			nTemp = pBackCDR->u.ggsnPDPRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 3:
    			nTemp = pBackCDR->u.sgsnMMRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 4:
    			nTemp = pBackCDR->u.sgsnSMORecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;				
    
            case 5:
    			nTemp = pBackCDR->u.sgsnSMTRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 6:
    			nTemp = pBackCDR->u.sgsnLCTRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 7:
    			nTemp = pBackCDR->u.sgsnLCORecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 8:
    			nTemp = pBackCDR->u.sgsnLCNRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            default:
    			break;
    		}
    	}
        else
        {
    		//����ǰ���chargingCharacteristics�ּ�
    		R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
    
    		switch (pFrontCDR->choice)
    		{
    		case 1:
    			nTemp = pFrontCDR->u.sgsnPDPRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;				
    
            case 2:
    			nTemp = pFrontCDR->u.ggsnPDPRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 3:
    			nTemp = pFrontCDR->u.sgsnMMRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 4:
    			nTemp = pFrontCDR->u.sgsnSMORecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;				
    
            case 5:
    			nTemp = pFrontCDR->u.sgsnSMTRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 6:
    			nTemp = pFrontCDR->u.sgsnLCTRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 7:
    			nTemp = pFrontCDR->u.sgsnLCORecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            case 8:
    			nTemp = pFrontCDR->u.sgsnLCNRecord.chargingCharacteristics.value[0] & 0x0f;
    			memcpy(pFieldValue, &nTemp, uLen);
    			return 0;
    
            default:
    			break;
    		}
    	}
	}
    else if (sFieldName == "servedMSISDN")
    {
        uLen = sizeof(BYTE);

        if (m_bIsMustMerge)
        {
    		//����ǰ���chargingCharacteristics�ּ�
    		R4_Fin_CallEventRecord* pBackCDR = (R4_Fin_CallEventRecord*)pABill;
    
    		switch (pBackCDR->choice)
    		{
    		case 1:
    			uLen = pBackCDR->u.sgsnPDPRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pBackCDR->u.sgsnPDPRecord.servedMSISDN.value, 
                      pBackCDR->u.sgsnPDPRecord.servedMSISDN.length);
    			return 0;				
    
            case 2:
    			uLen = pBackCDR->u.ggsnPDPRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pBackCDR->u.ggsnPDPRecord.servedMSISDN.value, 
                      pBackCDR->u.ggsnPDPRecord.servedMSISDN.length);
    			return 0;
    
            case 3:
    			uLen = pBackCDR->u.sgsnMMRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pBackCDR->u.sgsnMMRecord.servedMSISDN.value, 
                      pBackCDR->u.sgsnMMRecord.servedMSISDN.length);
                return 0;

            case 4:
    			uLen = pBackCDR->u.sgsnSMORecord.servedMSISDN.length;
    			memcpy(pFieldValue, pBackCDR->u.sgsnSMORecord.servedMSISDN.value, 
                      pBackCDR->u.sgsnSMORecord.servedMSISDN.length);
                return 0;


            case 5:
    			uLen = pBackCDR->u.sgsnSMTRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pBackCDR->u.sgsnSMTRecord.servedMSISDN.value, 
                      pBackCDR->u.sgsnSMTRecord.servedMSISDN.length);
                return 0;
    
            case 6:
    			uLen = pBackCDR->u.sgsnLCTRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pBackCDR->u.sgsnLCTRecord.servedMSISDN.value, 
                      pBackCDR->u.sgsnLCTRecord.servedMSISDN.length);
                return 0;
    
            case 7:
    			uLen = pBackCDR->u.sgsnLCORecord.servedMSISDN.length;
    			memcpy(pFieldValue, pBackCDR->u.sgsnLCORecord.servedMSISDN.value, 
                      pBackCDR->u.sgsnLCORecord.servedMSISDN.length);
                return 0;
    
            case 8:
    			uLen = pBackCDR->u.sgsnLCNRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pBackCDR->u.sgsnLCNRecord.servedMSISDN.value, 
                      pBackCDR->u.sgsnLCNRecord.servedMSISDN.length);
                return 0;
   
            default:
    			break;
    		}
    	}
        else
        {
    		//����ǰ���chargingCharacteristics�ּ�
    		R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
    
    		switch (pFrontCDR->choice)
    		{
    		case 1:
    			uLen = pFrontCDR->u.sgsnPDPRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pFrontCDR->u.sgsnPDPRecord.servedMSISDN.value, 
                      pFrontCDR->u.sgsnPDPRecord.servedMSISDN.length);
    			return 0;				

            case 2:
    			uLen = pFrontCDR->u.ggsnPDPRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pFrontCDR->u.ggsnPDPRecord.servedMSISDN.value, 
                      pFrontCDR->u.ggsnPDPRecord.servedMSISDN.length);
    			return 0;				
    
            case 3:
    			uLen = pFrontCDR->u.sgsnMMRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pFrontCDR->u.sgsnMMRecord.servedMSISDN.value, 
                      pFrontCDR->u.sgsnMMRecord.servedMSISDN.length);
    			return 0;				
    
            case 4:
    			uLen = pFrontCDR->u.sgsnSMORecord.servedMSISDN.length;
    			memcpy(pFieldValue, pFrontCDR->u.sgsnSMORecord.servedMSISDN.value, 
                      pFrontCDR->u.sgsnSMORecord.servedMSISDN.length);
    			return 0;				
    
            case 5:
    			uLen = pFrontCDR->u.sgsnSMTRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pFrontCDR->u.sgsnSMTRecord.servedMSISDN.value, 
                      pFrontCDR->u.sgsnSMTRecord.servedMSISDN.length);
    			return 0;				
    
            case 6:
    			uLen = pFrontCDR->u.sgsnLCTRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pFrontCDR->u.sgsnLCTRecord.servedMSISDN.value, 
                      pFrontCDR->u.sgsnLCTRecord.servedMSISDN.length);
    			return 0;				
    
            case 7:
    			uLen = pFrontCDR->u.sgsnLCORecord.servedMSISDN.length;
    			memcpy(pFieldValue, pFrontCDR->u.sgsnLCORecord.servedMSISDN.value, 
                      pFrontCDR->u.sgsnLCORecord.servedMSISDN.length);
    			return 0;				
    
            case 8:
    			uLen = pFrontCDR->u.sgsnLCNRecord.servedMSISDN.length;
    			memcpy(pFieldValue, pFrontCDR->u.sgsnLCNRecord.servedMSISDN.value, 
                      pFrontCDR->u.sgsnLCNRecord.servedMSISDN.length);
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
            R4_Fin_CallEventRecord* pBackCDR = (R4_Fin_CallEventRecord*)pABill;
            
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
            R4_Org_CallEventRecord* pFrontCDR = (R4_Org_CallEventRecord*)pABill;
            
            switch (pFrontCDR->choice)
            {
            case 2:
                uLen = CvtBCDOctecToStr2((LPOCTECSTRING )&(pFrontCDR->u.ggsnPDPRecord.sgsnPLMNIdentifier), 
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


