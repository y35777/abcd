#pragma warning(disable:4706)

#include "../../include/type_def.h"
#include "CDRF_CMCC_3GCDR.h"
#include "../../include/export_dll.h"

/******************************************************
* ������:   DeCodeR99Bill_ini_mt
* ����:    Guan Guofeng
* ����:    2003-09-28
* ����:    
* ����: 
*       ����1 :char* pVal
*       ����2 :int nValLen
*       ����3 :char* pBuf
*       ����4 :int BufLen
*
* ����ֵ: int,����ɹ�����TRUE��ʧ�ܷ���FALSE��
* ˵��: ����������̰߳�ȫ�ġ�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/

extern "C" IGWB_Svc_Export 
int DeCodeBill_ini_mt(void* pGlobal,
                      char* pVal,     /* ����������� */
                      int* pValLen,   /* �������ݳ���, ����������ĳ��� */
                      char* pBuf,     /* ����������Ļ����� */
                      int nBufLen)    /* �������ĳ��� */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char *)pVal;
    encodedData.length = *pValLen;

    int pdu = CMCC_3GCDR_CallEventRecord_PDU;

    int nErr = 0;
    if(nErr = ossSetDecodingLength(world,nBufLen))
    {
		//zyx ȥ��ע��
/*
        ossPrint(world, "Set decoding length fail:%s\n",
                 ossGetErrMsg(world));
*/
        return nErr;
    }

    if(nErr = ossDecode(world,&pdu,&encodedData, (void**)&pBuf))
    {
/*
        ossPrint(world, "Decode 3G_CDR fail.Error code:%d\n",nErr); 
*/
        return nErr;
    }

    long nDecodeLen = ossGetDecodingLength(world);
    if(nDecodeLen == -1)
    {
        ossPrint(world,"ossGetDecodingLength fail.\n");
        return -1;
    }

    *pValLen = nDecodeLen;

    return 0;
}

/******************************************************
* ������:   EnCodeR99Bill_ini_mt
* ����:    Guan Guofeng
* ����:    2003-09-28
* ����:    
* ����: 
*       ����1 :char* pVal
*       ����2 :char* pBuf
*       ����2 :int* pBufLen
*
* ����ֵ: char*,��ŷ��ؽ���Ļ�������
* ˵��: �ú���֧�ֶ��̡߳�
* ���¼�¼: 
*
* ����            ����                  ���� 
* ========== ================== ======================
*
******************************************************/

extern "C" IGWB_Svc_Export 
char* EnCodeBill_ini_mt(void* pGlobal,
                        char* pVal,      /* �����������ָ�� */
                        char* pBuf,      /* ��������������ʹ�õĻ����� */
                        int* pBufLen)    /* ���뻺����pBuf�ĳ���,*/
                                         /* �����ر������ĳ��� (in,out) */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char*)pBuf;
    encodedData.length = *pBufLen;

    int nErr = 0;
    if(nErr = ossEncode(world, CMCC_3GCDR_CallEventRecord_PDU,(void*)pVal,
        &encodedData))
    {
        ossPrint(world, "Encode 3G_CDR fail.Error code:%d\n",nErr); 
        return NULL;
    }

    *pBufLen = encodedData.length;

    /*
     * ���ܷ���pBuf,��ΪencodedData.value
     * ���ܱ��޸Ķ�ָ��pBuf������λ�á�
     */
    return (char*)encodedData.value;
}

extern "C" IGWB_Svc_Export 
void* Init_OSS_Lib_Ini()
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

    if(ossinit(world, CDRF_R99_Org))
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
void Release_OSS_Lib_Ini(void* pGlobal)
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

