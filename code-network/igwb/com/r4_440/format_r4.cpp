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
// 功能描述：构造函数
//
// 输入参数：
//   sRootDir：格式配置库根路径
//   sFmtName：格式名
//
// 输出参数：无
//
//   返回值：无
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
			m_bIsDisFormat = TRUE;  //设置分拣格式标志
		}
			
		m_n3GR4FormatType = 0;
	}
    
    m_pQueryCondition->SetFlag(0);
}
/************ End of The CFormatR4 Defining ********************/


/************ The ~CFormatR4 Defining **************************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CFormatR4::~CFormatR4()
{
}
/************ End of The ~CFormatR4 Defining *******************/


/************ The Init Defining ********************************/
// 功能描述：初始化函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormatR4::Init(void)
{
    //调用基类的初始化函数
    if(CFormat::Init() == -1)
    {
        return -1;
    }

    //获取用于查询条件的域名，
    //注意：在进行格式域配置时，3G可以作为查询条件的域和个数是固定的，
    //而且在话单域中的先后顺序也是固定的（与程序中获取查询域值的顺序一致）
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
// 功能描述：计算一个格式中所有域的字节偏移、位偏移以及字节总长度函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormatR4::InitFormatInfo(void)
{
    //确保本格式域个数大于零
    if(m_uFieldNum == 0)
    {
        return -1;
    }

    //设置本格式的总字节长度，由于3G话单是变长的，所以设置长度为零
    m_uFmtLen = 0;

    //初始化本格式中的每一个域的字节偏移量和位偏移量，
    //由于3G话单是变长的，每一个域的长度不固定，所以全都设置为零
    SFieldInfo*  pFI   = NULL;   //当前域描述指针
    for(int i = 0; i < m_uFieldNum; i++)
    {
        //设置当前域的字节偏移量和位偏移量
        pFI = m_pFieldsArray[i];
        pFI->nByteOffset = 0;
        pFI->nBitOffset = 0;
    }

    return 0;
}
/************ End of The InitFormatInfo Defining ***************/


/************ The IsTheFormat Defining *************************/
// 功能描述：判断传入的一张话单是否符合本格式
//
// 输入参数：
//     pABill：传入的单张话单
//       uLen：一张话单的长度，即pABill所占内存大小
//
// 输出参数：无
//
//   返回值：符合本格式返回0，不符合本格式返回1，出错返回-1
//
/***************************************************************/
int CFormatR4::IsTheFormat(const void* pABill,  const UINT4 uLen)
{
    //不符合本格式
    if((pABill == NULL) || (uLen == 0))
    {
        return 1;
    }

    //原始话单类型的S-CDR
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

    //原始话单类型的G-CDR
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

    //原始话单类型的M-CDR
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

    //最终话单类型的S-CDR
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

    //最终话单类型的G-CDR
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

   

    //所有话单格式类型
    if(ACE_OS::strcasecmp(m_sFmtName.c_str(), W_TMP_CDR_NAME) == 0)
    {
        //Do Nothing
        return 0;
    }
	
	if (m_sFmtName.find("R4BP") != -1)
	{
		return 0;	
	}

    //其它非法的格式类型
    return -1;
}
/************ End of The IsTheFormat Defining ******************/


/************ The ConvertABill Defining ************************/
// 功能描述：将传入的一张话单转换成目标格式
//
// 输入参数：
//   pASrcBill：传入的单张话单
// uSrcBillLen：一张话单的长度，即pABill所占内存大小
//
// 输出参数：
//   pADestBill：转换后的话单，指向的内存由本函数分配，由本函数的调用者负责释放
// uDestBillLen：转换后的话单长度
//
//   返回值：正确转换返回0，失败返回-1
//
/***************************************************************/
int CFormatR4::ConvertABill(const void* pASrcBill,
                            const UINT4 uSrcBillLen, 
                            void*& pADestBill,
                            UINT4& uDestBillLen)
{
    //初始化输出参数
    pADestBill = NULL;
    uDestBillLen  = 0;

    //确保输入参数的合法性
    if((pASrcBill == NULL) || (uSrcBillLen == 0))
    {
        return -1;
    }

    //获取存放转换后话单内容的全局缓冲区
    CGlobalBuffer::acquire();
    char *pBuffer = CGlobalBuffer::GetBillBuffer();
    R4_Org_CallEventRecord *pTemp = (R4_Org_CallEventRecord*)pASrcBill;

    int nLen = 0;
    try
    {
        //把当前话单转换成浏览格式的文本
        nLen = GetAllFieldTextValueOf3GR4CDR((char*)(&pTemp->u), m_n3GR4FormatType, pBuffer, _BROWSEBUFFERLENGTH);
    }
    catch (...)
    {
        CGlobalBuffer::release();
        return -1;
    }

    //如果转换成功
    if (nLen > 0)
    {
        uDestBillLen = nLen;
        pADestBill = new BYTE[nLen + 1];
        //strcpy((char*)pADestBill, pBuffer);
        memcpy(pADestBill, pBuffer, (nLen + 1));
        CGlobalBuffer::release();
    }

    //如果转换失败
    else
    {
        CGlobalBuffer::release();
        return -1;
    }

    return 0;
}
/************ End of The ConvertABill Defining *****************/


/************ The GetFieldValueByName Defining *****************/
// 功能描述：根据话单域名从一张话单中获得该域的值
//
// 输入参数：
//     pABill：传入的单张话单
//   bThisFmt：传入的话单是否符合本格式，0表示符合本格式，
//             非零则需要转换成目标格式
//     uIndex：格式域名，与SFieldInfo结构中的nFieldIndex对应
//   nIsQuery：是否查询标志，1表示该函数为查询条件转换，
//             0表示不是为查询条件转换
//
// 输出参数：
// pFieldValue：获得的域值
//        uLen：该域长度
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormatR4::GetFieldValueByName(const void* pABill,
                                   const int bThisFmt,
                                   const STRING& sFieldName,  
                                   void*& pFieldValue,
                                   UINT4& uLen,
                                   int nIsQuery)
{
    //初始化输出参数和变量
    pFieldValue = NULL;
    uLen = 0;

    //确保输入参数的正确性
    if(sFieldName.length() == 0)
    {
        return -1;
    }

	if (m_bIsDisFormat)
	{
		return GetFieldValueByNameEx(pABill, uLen, sFieldName, pFieldValue);
	}

    //获取相应的域名对应的域值
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
// 功能描述：根据话单域索引从一张话单中获得该域的值
//
// 输入参数：
//     pABill：传入的单张话单
//   bThisFmt：传入的话单是否符合本格式，0表示符合本格式，
//             非零则需要转换成目标格式
//     uIndex：格式域名，与SFieldInfo结构中的nFieldIndex对应
//   nIsQuery：是否查询标志，1表示该函数为查询条件转换，
//             0表示不是为查询条件转换
//
// 输出参数：
// pFieldValue：获得的域值，由外部分配内存
//        uLen：该域长度
//
//   返回值：成功返回0，失败返回-1
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
        //获取相应的域名对应的域值
        return GetFieldContentByNameEx(pABill,
                                       uLen,
                                       m_pFieldsArray[nFieldIndex]->szFieldName,
                                       pFieldValue);
    }

    //获取相应的域名对应的域值
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
// 功能描述：判断一张话单是否符合指定的查询条件
//
// 输入参数：
//   pASrcBill：传入的单张话单
//  sQueryCond：查询条件，中缀表达式
//
// 输出参数：无
//
//   返回值：符合格式返回0，不符合返回1，出错返回-1
//
/***************************************************************/
int CFormatR4::IsTheQueryCondition(const void* pASrcBill, const STRING& sQueryCond)
{
    SVariant RetVar;    //条件表达式计算结果变量

    //确保输入参数的正确性
    if((pASrcBill == NULL) || ((m_sQueryCond.length() == 0)
        && (sQueryCond.length() == 0)))
    {
        return -1;
    }

    //如果条件表达式参数不为空，将中缀表达式转换为后缀表达式
    if(sQueryCond.length() != 0)
    {
        if(m_pQueryCondition->Infix2Postfix(sQueryCond.c_str()) == -1)
        {
            //assert(0);
            return -1;
        }
        m_sQueryCond = sQueryCond;
    }

    //获取查询条件域的值
    if(GetQueryCondFieldsValue(pASrcBill) == -1)
    {
        return -1;
    }

    //计算条件表达式
    if(m_pQueryCondition->Caculate(pASrcBill, m_uFmtLen, RetVar) == -1)
    {
        //assert(0);
        return -1;
    }

    //不符合条件
    if(RetVar.n4Value == 0)
    {
        return 1;
    }

    //符合条件
    return 0;
}
/************ End of The IsTheQueryCondition Defining **********/


/************ The GetQueryCondFieldsValue Defining *************/
// 功能描述：获取查询条件域值
//
// 输入参数：
//     pABill：传入的单张话单
//
// 输出参数：无
//
//   返回值：成功返回0，出错返回-1
//
/***************************************************************/
int CFormatR4::GetQueryCondFieldsValue(const void* pABill)
{
    //获取全局缓冲区
    CGlobalBuffer::acquire();
    char *pBuffer = CGlobalBuffer::GetBillBuffer();
    R4_Org_CallEventRecord *pTemp = (R4_Org_CallEventRecord*)pABill;

    int nLen = 0;
    try
    {
        //获取作为查询条件域的值
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

    //如果获取查询条件域值失败
    if (nLen <= 0)
    {
        CGlobalBuffer::release();
        return -1;
    }

/*
	//拆分话单域，注:浏览话单约定域分隔符为'^'
	char *ss0 = NULL;
    char *ss1 = NULL;
    int i = 0;
	ss0 = pBuffer;
	while ((ss1 = strchr(ss0, '^')) != NULL)
	{
		*ss1 = 0;

        //把域值与相应的域名对应起来，
        //注意：在进行格式域配置时，3G可以作为查询条件的域和个数是固定的，
        //而且在话单域中的先后顺序也是固定的（与程序中获取查询域值的顺序一致）
		m_QueryCondFieldsValue[i++].sFieldValue = ss0;

		ss0 = ss1+1;
		*ss1 = '^';
    }

    //如果域值字符串不是以'^'结尾，则获取最后一个域值
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
// 功能描述：根据域名返回域值（用于处理3G分拣）
//
// 输入参数：
//     pABill：待分析的一张话单数据指针
//       uLen：pABill的数据长度
// sFieldName：域名
//
// 输出参数：
//pFieldValue：获得的域值
//       uLen：该域长度
//
//   返回值：符合本通道返回0，否则返回-1
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
// 根据话单域名从一张话单中获得该域的值(由外部分配内存, 用于3G分拣)
//
// 输入参数：
//     pABill：待分析的一张话单数据指针
//       uLen：pABill的数据长度
// nFieldIndex:域序号
//
// 输出参数：
//pFieldValue：获得的域值
//       uLen：该域长度
//
//   返回值：符合本通道返回0，否则返回-1
//
/***************************************************************/
/***************************************************************/
int CFormatR4::GetFieldContentByNameEx(const void* pABill,
                                       UINT4& uLen,  
                                       const STRING& sFieldName,
                                       void* pFieldValue)
{
	//判断分拣条件,分析是按照那个字段分拣
    if (sFieldName == "recordType")
	{
		//如果是按照recordType分拣
        if (m_bIsMustMerge)
        {
    		//如果是按照recordType分拣
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
    		//如果是按照recordType分拣
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
    		//如果是按照chargingCharacteristics分拣
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
    		//如果是按照chargingCharacteristics分拣
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
    		//如果是按照chargingCharacteristics分拣
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
    		//如果是按照chargingCharacteristics分拣
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
            //如果是按照sgsnPLMNIdentifier分拣
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
            //如果是按照sgsnPLMNIdentifier分拣
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


