#ifndef _PS_ASN_ENCAP_H_
#define _PS_ASN_ENCAP_H_

#include "../include/base_type.h"
#include "ace/DLL.h"

extern "C" {
typedef void (*RELEASE_OSS_LIB)(void*);

typedef void* (*INIT_OSS_LIB)();

typedef int (*DECODE_BILL)(void* pGlobal,
                           char* pVal,     /* 待解码的数据 */
                           int* pValLen,   /* 输入数据长度,输出解码结果的长度 */
                           char* pBuf,     /* 存放输出结果的缓冲区 */
                           int nBufLen);   /* 缓冲区的长度 */

typedef char* (*ENCODE_BILL)(void* pGlobal,
                               char* pVal,      /* 待编码的数据指针 */
                               char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                               int* pBufLen);   /* 输入缓冲区pBuf的长度，*/
                                                /* 并返回编码结果的长度 (in,out) */

typedef void* (*CREATE_PARSER_OBJECT)();

typedef int (*GETORGINALCDRTYPE)(const void* pABill, 
                                 PS_CDR_TYPE& cdr_type);
typedef int (*CHECKOMFIELD)(const char* szCfgFile,
                            const void* pABill,
                            BOOL        bMerged);

//Added by ZhengYuqun SWPD04700
typedef int (*CDRFILTERAPNNI)(const void* pABill);
}

class IGWB_Svc_Export CPSAsnEncap
{
public:
    CPSAsnEncap();
    ~CPSAsnEncap();
    int Init(int nAPId);
    void Clear(void);

    /*原始话单*/
    int DeCodeBill_ini_mt(void* pGlobal,
                          char* pVal,     /* 待解码的数据 */
                          int* pValLen,   /* 输入数据长度, 输出解码结果的长度 */
                          char* pBuf,     /* 存放输出结果的缓冲区 */
                          int nBufLen);   /* 缓冲区的长度 */

    char* EnCodeBill_ini_mt(void* pGlobal,
                            char* pVal,      /* 待编码的数据指针 */
                            char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                            int* pBufLen);   /* 输入缓冲区pBuf的长度,*/
                                             /* 并返回编码结果的长度 (in,out) */

    void* Init_OSS_Lib_Ini();

    void Release_OSS_Lib_Ini(void* pGlobal);

    /*最终话单*/
    int DeCodeBill_final_mt(void* pGlobal,
                            char* pVal,     /* 待解码的数据 */
                            int* pValLen,   /* 输入数据长度,输出解码结果的长度 */
                            char* pBuf,     /* 存放输出结果的缓冲区 */
                            int nBufLen);   /* 缓冲区的长度 */

    char* EnCodeBill_final_mt(void* pGlobal,
                              char* pVal,      /* 待编码的数据指针 */
                              char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                              int* pBufLen);   /* 输入缓冲区pBuf的长度，*/
                                               /* 并返回编码结果的长度 (in,out) */
                                               
    void* Init_OSS_Lib_Final();

    void Release_OSS_Lib_Final(void* pGlobal);

    void* CreateParserObject();

    BOOL GetbAviable();

    int GetOrginalCDRType(const void* pABill, 
                          PS_CDR_TYPE& cdr_type);
    int CheckOMField(const char* szCfgFile,
                     const void* pABill,
                     BOOL        bMerged);

    //Added by ZhengYuqun SWPD04700
    int CDRFilterAPNNI(const void* pABill);

protected:
    INIT_OSS_LIB          m_pInitIniFun;
    INIT_OSS_LIB          m_pInitFinFun;
    RELEASE_OSS_LIB       m_pReleaseIniFun;
    RELEASE_OSS_LIB       m_pReleaseFinFun;
    
    DECODE_BILL           m_pDecodeIniFun;
    DECODE_BILL           m_pDecodeFinFun;
    ENCODE_BILL           m_pEncodeIniFun;
    ENCODE_BILL           m_pEncodeFinFun;

    CREATE_PARSER_OBJECT  m_pCreateParserFun;

    GETORGINALCDRTYPE     m_pGetCDRTypeFun;
    CHECKOMFIELD          m_pCheckOMFieldFun;

    //Added by ZhengYuqun SWPD04700
    CDRFILTERAPNNI        m_pCDRFilterAPNNIFun;

    ACE_DLL*              m_pDll;

    BOOL                  m_bAviable;
};

#endif //_PS_ASN_ENCAP_H_
