#ifndef _PS_ASN_ENCAP_H_
#define _PS_ASN_ENCAP_H_

#include "../include/base_type.h"
#include "ace/DLL.h"

extern "C" {
typedef void (*RELEASE_OSS_LIB)(void*);

typedef void* (*INIT_OSS_LIB)();

typedef int (*DECODE_BILL)(void* pGlobal,
                           char* pVal,     /* ����������� */
                           int* pValLen,   /* �������ݳ���,����������ĳ��� */
                           char* pBuf,     /* ����������Ļ����� */
                           int nBufLen);   /* �������ĳ��� */

typedef char* (*ENCODE_BILL)(void* pGlobal,
                               char* pVal,      /* �����������ָ�� */
                               char* pBuf,      /* ��������������ʹ�õĻ����� */
                               int* pBufLen);   /* ���뻺����pBuf�ĳ��ȣ�*/
                                                /* �����ر������ĳ��� (in,out) */

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

    /*ԭʼ����*/
    int DeCodeBill_ini_mt(void* pGlobal,
                          char* pVal,     /* ����������� */
                          int* pValLen,   /* �������ݳ���, ����������ĳ��� */
                          char* pBuf,     /* ����������Ļ����� */
                          int nBufLen);   /* �������ĳ��� */

    char* EnCodeBill_ini_mt(void* pGlobal,
                            char* pVal,      /* �����������ָ�� */
                            char* pBuf,      /* ��������������ʹ�õĻ����� */
                            int* pBufLen);   /* ���뻺����pBuf�ĳ���,*/
                                             /* �����ر������ĳ��� (in,out) */

    void* Init_OSS_Lib_Ini();

    void Release_OSS_Lib_Ini(void* pGlobal);

    /*���ջ���*/
    int DeCodeBill_final_mt(void* pGlobal,
                            char* pVal,     /* ����������� */
                            int* pValLen,   /* �������ݳ���,����������ĳ��� */
                            char* pBuf,     /* ����������Ļ����� */
                            int nBufLen);   /* �������ĳ��� */

    char* EnCodeBill_final_mt(void* pGlobal,
                              char* pVal,      /* �����������ָ�� */
                              char* pBuf,      /* ��������������ʹ�õĻ����� */
                              int* pBufLen);   /* ���뻺����pBuf�ĳ��ȣ�*/
                                               /* �����ر������ĳ��� (in,out) */
                                               
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
