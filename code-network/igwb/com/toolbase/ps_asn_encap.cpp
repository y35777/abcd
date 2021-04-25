#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "ps_asn_encap.h"


CPSAsnEncap::CPSAsnEncap()
{
    m_pDll              = NULL;

    m_pInitIniFun       = NULL;
    m_pInitFinFun       = NULL;
    m_pReleaseIniFun    = NULL;
    m_pReleaseFinFun    = NULL;

    m_pCreateParserFun  = NULL;
    m_pDecodeIniFun     = NULL;
    m_pDecodeFinFun     = NULL;
    m_pEncodeIniFun     = NULL;
    m_pEncodeFinFun     = NULL;
    m_pGetCDRTypeFun    = NULL;
    m_bAviable          = FALSE;
}

CPSAsnEncap::~CPSAsnEncap()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

BOOL CPSAsnEncap::GetbAviable()
{
    return m_bAviable;
}

void CPSAsnEncap::Clear(void)
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
    m_pInitIniFun       = NULL;
    m_pInitFinFun       = NULL;
    m_pReleaseIniFun    = NULL;
    m_pReleaseFinFun    = NULL;

    m_pCreateParserFun  = NULL;
    m_pDecodeIniFun     = NULL;
    m_pDecodeFinFun     = NULL;
    m_pEncodeIniFun     = NULL;
    m_pEncodeFinFun     = NULL;
    m_pGetCDRTypeFun    = NULL;
    m_pCheckOMFieldFun  = NULL;

    //Added by ZhengYuqun SWPD04700
    m_pCDRFilterAPNNIFun = NULL;

    m_bAviable          = FALSE;
}

int CPSAsnEncap::Init(int nAPId)
{
    if(NULL == m_pDll)
    {
        //打开配置文件
        CINIFile Ini(GetCfgFilePath());
        Ini.Open();

        char szFmtDll[40];

        char strSec[20];
        sprintf(strSec, "%s%d", CFG_SEC_AP, nAPId);
        Ini.GetString(strSec,
                      CFG_AP_KEY_FMT_DLL,
                      CFG_AP_KEY_FMT_DLL_DEFAULT,
                      szFmtDll,
                      sizeof(szFmtDll));
        if(szFmtDll[0] == '\0')
        {
            return -1;
        }
#ifdef _PLATFORM_WIN32
        strncat(szFmtDll, ".dll", sizeof(szFmtDll) - strlen(szFmtDll));
#elif defined(_PLATFORM_SOLARIS) || defined (_PLATFORM_LINUX) 
        STRING strTemp = szFmtDll;
        SNPRINTF(szFmtDll, sizeof(szFmtDll),
                 "lib%s.so", strTemp.c_str());
#elif defined(_PLATFORM_HPUX)
        STRING strTemp = szFmtDll;
        SNPRINTF(szFmtDll, sizeof(szFmtDll),
                 "lib%s.sl", strTemp.c_str());
#else
        #error macro _PLATFORM_??? undefined.
#endif
        szFmtDll[sizeof(szFmtDll) - 1] = '\0';

        m_pDll = new ACE_DLL;
        int nRet = m_pDll->open(szFmtDll);
        if(0 != nRet)
        {
            delete m_pDll;
            m_pDll = NULL;
            return -1;
        }
    }

    m_pCreateParserFun = (CREATE_PARSER_OBJECT)m_pDll->symbol("CreateParserObject");
    if(NULL == m_pCreateParserFun)
    {
        return -1;
    }

    m_pInitIniFun = (INIT_OSS_LIB)m_pDll->symbol("Init_OSS_Lib_Ini");
    if(NULL == m_pInitIniFun)
    {
        Clear();
        return -1;
    }

    m_pInitFinFun = (INIT_OSS_LIB)m_pDll->symbol("Init_OSS_Lib_Final");
    if(NULL == m_pInitFinFun)
    {
        Clear();
        return -1;
    }

    m_pReleaseIniFun = (RELEASE_OSS_LIB)m_pDll->symbol("Release_OSS_Lib_Ini");
    if(NULL == m_pReleaseIniFun)
    {
        Clear();
        return -1;
    }

    m_pReleaseFinFun = (RELEASE_OSS_LIB)m_pDll->symbol("Release_OSS_Lib_Final");
    if(NULL == m_pReleaseFinFun)
    {
        Clear();
        return -1;
    }

    m_pDecodeIniFun = (DECODE_BILL)m_pDll->symbol("DeCodeBill_ini_mt");
    if(NULL == m_pDecodeIniFun)
    {
        Clear();
        return -1;
    }

    m_pDecodeFinFun = (DECODE_BILL)m_pDll->symbol("DeCodeBill_final_mt");
    if(NULL == m_pDecodeFinFun)
    {
        Clear();
        return -1;
    }

    m_pEncodeIniFun = (ENCODE_BILL)m_pDll->symbol("EnCodeBill_ini_mt");
    if(NULL == m_pEncodeIniFun)
    {
        Clear();
        return -1;
    }

    m_pEncodeFinFun = (ENCODE_BILL)m_pDll->symbol("EnCodeBill_final_mt");
    if(NULL == m_pEncodeFinFun)
    {
        Clear();
        return -1;
    }

    m_pGetCDRTypeFun = (GETORGINALCDRTYPE)m_pDll->symbol("GetOrginalCDRType");
    if(NULL == m_pGetCDRTypeFun)
    {
        Clear();
        return -1;
    }

    m_pCheckOMFieldFun = (CHECKOMFIELD)m_pDll->symbol("CheckOMField");
/*
    if(NULL == m_pCheckOMFieldFun)
    {
        Clear();
        return -1;
    }
*/

    //Added by ZhengYuqun SWPD04700
    m_pCDRFilterAPNNIFun = (CDRFILTERAPNNI)m_pDll->symbol("CDRFilterAPNNI");

    m_bAviable = TRUE;
    
    return 0;
}

void* CPSAsnEncap::CreateParserObject()
{
    if(NULL != m_pCreateParserFun)
    {
        return (*m_pCreateParserFun)();
    }

    return NULL;
}

/*原始话单*/
int CPSAsnEncap::DeCodeBill_ini_mt(void* pGlobal,
                                    char* pVal,     /* 待解码的数据 */
                                    int* pValLen,   /* 输入数据长度, 输出解码结果的长度 */
                                    char* pBuf,     /* 存放输出结果的缓冲区 */
                                    int nBufLen)    /* 缓冲区的长度 */
{
    if(NULL != m_pDecodeIniFun)
    {
        return (*m_pDecodeIniFun)(pGlobal, pVal, pValLen, pBuf, nBufLen);
    }

    return -1;
}
 
char* CPSAsnEncap::EnCodeBill_ini_mt(void* pGlobal,
                                      char* pVal,      /* 待编码的数据指针 */
                                      char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                                      int* pBufLen)    /* 输入缓冲区pBuf的长度,*/
                                                       /* 并返回编码结果的长度 (in,out) */
{
    if(NULL != m_pEncodeIniFun)
    {
        return (*m_pEncodeIniFun)(pGlobal, pVal, pBuf, pBufLen);
    }

    return NULL;
}

void* CPSAsnEncap::Init_OSS_Lib_Ini()
{
    if(NULL != m_pInitIniFun)
    {
        return (*m_pInitIniFun)();
    }

    return NULL;
}

void CPSAsnEncap::Release_OSS_Lib_Ini(void* pGlobal)
{
    if(NULL != m_pReleaseIniFun)
    {
        (*m_pReleaseIniFun)(pGlobal);
    }
}

/*最终话单*/
int CPSAsnEncap::DeCodeBill_final_mt(void* pGlobal,
                                      char* pVal,     /* 待解码的数据 */
                                      int* pValLen,   /* 输入数据长度,输出解码结果的长度 */
                                      char* pBuf,     /* 存放输出结果的缓冲区 */
                                      int nBufLen)    /* 缓冲区的长度 */
{
    if(NULL != m_pDecodeFinFun)
    {
        return (*m_pDecodeFinFun)(pGlobal, pVal, pValLen, pBuf, nBufLen);
    }

    return -1;
}

char* CPSAsnEncap::EnCodeBill_final_mt(void* pGlobal,
                                        char* pVal,      /* 待编码的数据指针 */
                                        char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                                        int* pBufLen)    /* 输入缓冲区pBuf的长度，*/
                                                         /* 并返回编码结果的长度 (in,out) */
{
    if(NULL != m_pEncodeFinFun)
    {
        return (*m_pEncodeFinFun)(pGlobal, pVal, pBuf, pBufLen);
    }

    return NULL;
}
                                           
void* CPSAsnEncap::Init_OSS_Lib_Final()
{
    if(NULL != m_pInitFinFun)
    {
        return (*m_pInitFinFun)();
    }

    return NULL;
}

void CPSAsnEncap::Release_OSS_Lib_Final(void* pGlobal)
{
    if(NULL != m_pReleaseFinFun)
    {
        (*m_pReleaseFinFun)(pGlobal);
    }
}

int CPSAsnEncap::GetOrginalCDRType(const void* pABill, 
                                   PS_CDR_TYPE& cdr_type)
{
    if(NULL != m_pGetCDRTypeFun)
    {
        return (*m_pGetCDRTypeFun)(pABill, cdr_type);
    }

    return -1;
}

int CPSAsnEncap::CheckOMField(const char* szCfgFile,
                              const void* pABill,
                              BOOL        bMerged)
{
    if(NULL != m_pCheckOMFieldFun)
    {
        return (*m_pCheckOMFieldFun)(szCfgFile, pABill, bMerged);
    }

    //不支持OM域检查功能，直接返回正常
    return 0;
}

//Added by ZhengYuqun SWPD04700
int CPSAsnEncap::CDRFilterAPNNI(const void* pABill)
{
    if(NULL != m_pCDRFilterAPNNIFun)
    {
        return (*m_pCDRFilterAPNNIFun)(pABill);
    }
    
    //不支持OM域检查功能，直接返回正常
    return 0;
}
