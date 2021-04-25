#pragma warning(disable:4706)

#include "../../include/type_def.h"
#include "CDRF_CMCC_3GCDR_CG.h"
#include "../../include/export_dll.h"

/******************************************************
* ������:  DeCodeR99Bill_final
* ����:    Guan Guofeng
* ����:    2003-09-28
* ����:    ��ѭoss��˾�淶��ASN.1���뺯����
* ����: 
*       ����1 :char* pIn,������Ļ�����¼��������
*       ����2 :int* pLen,������¼�ĳ��ȣ������ؽ���󻰵��ĳ��ȡ�
* ����ֵ: void*,��ű������Ļ�����,�û������Ǿ�̬�ڴ棬�����ͷš�
*          ʧ�ܷ���NULL��
* ˵��:
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/

extern "C" IGWB_Svc_Export 
int DeCodeBill_final_mt(void* pGlobal,
                        char* pVal,     /* ����������� */
                        int* pValLen,   /* �������ݳ���,����������ĳ��� */
                        char* pBuf,     /* ����������Ļ����� */
                        int nBufLen)    /* �������ĳ��� */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char *)pVal;
    encodedData.length = *pValLen;

    int pdu = CMCC_3GCDR_CG_CallEventRecord_PDU;

    int nErr = 0;
    if(nErr = ossSetDecodingLength(world,nBufLen))
    {
        return nErr;
    }

    if(nErr = ossDecode(world,&pdu,&encodedData, (void**)&pBuf))
    {
        return nErr;
    }

    long nDecodeLen = ossGetDecodingLength(world);
    if(nDecodeLen == -1)
    {
        return -1;
    }

    *pValLen = nDecodeLen;

    return 0;
}


/******************************************************
* ������:   EnCodeR99Bill_final_mt
* ����:    Guan Guofeng
* ����:    2003-09-28
* ����:    
* ����: 
*       ����1 :char** pCdr,������Ļ�����¼��
*       ����2 :int* pLen,���ر������ĳ��ȡ�
* ����ֵ: char*,��������������Ļ�������ʧ�ܷ���NULL��
*
* ˵��: ���صĻ�����ָ��ָ��̬�ڴ棬�����ͷš�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/

extern "C" IGWB_Svc_Export 
char* EnCodeBill_final_mt(void* pGlobal,
                          char* pVal,      /* �����������ָ�� */
                          char* pBuf,      /* ��������������ʹ�õĻ����� */
                          int* pBufLen)    /* ���뻺����pBuf�ĳ��ȣ�*/
                                           /* �����ر������ĳ��� (in,out) */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char*)pBuf;
    encodedData.length = *pBufLen;

    int nErr = 0;
    if(nErr = ossEncode(world, CMCC_3GCDR_CG_CallEventRecord_PDU,(void*)pVal,
        &encodedData))
    {
        return NULL;
    }

    *pBufLen = encodedData.length;

    /*
     *���ܷ���pBuf,��ΪencodedData.value
     *���ܱ��޸Ķ�ָ��pBuf������λ�á�
     */
    return (char*)encodedData.value;
}


extern "C" IGWB_Svc_Export 
void* Init_OSS_Lib_Final()
{
    struct ossGlobal* world = new struct ossGlobal;

#ifdef ONE_DLL
    /*
        Windowsƽ̨�Ķ��߳�Ҫ��
        ACEҪ�����Ӷ��̶߳�̬�⣬��OSS��������toedcomd.lib��
        ��Ҫ�����ȵ���ossInitSync()�Ա�֤�̰߳�ȫ��
        ��Ҫ�������ѡ��ONE_DLL��
    */
    if(ossInitSync()) return NULL;
#endif

    if(ossinit(world, CDRF_R99_Fin))
    {
        delete world;
        return NULL;
    }

    if(ossSetEncodingRules(world, OSS_BER))
    {
        ossterm(world);
        delete world;
        return NULL;
    }

    if(ossSetEncodingFlags(world,DEBUGPDU|DEFINITE|
                            AUTOMATIC_ENCDEC))
    {
        ossterm(world);
        delete world;
        return NULL;
    }

#ifndef ONE_DLL
    //Unixƽ̨�Ķ��߳�Ҫ��
    if(ossSetFlags(world,NOTRAPPING))
    {
        ossterm(world);
        delete world;
        return NULL;
    }
#endif

    return world;
}

extern "C" IGWB_Svc_Export  
void Release_OSS_Lib_Final(void* pGlobal)
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;
    ossterm(world);

#ifdef ONE_DLL
    /*
        ��ossInitSync()���ö�Ӧ��
    */
    ossTermSync();
#endif

    delete world;
}


