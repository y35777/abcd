#include "CDRF_2GCDR_Fin_M.h"

/******************************************************
* ������:  DeCodeGPRSBill_final_M_mt
* ����:    Zhou Yonghui
* ����:    01-6-1
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
*01-6-1        Zhou Yonghui     �����ӹ�OMM���л�����ѯ��
*01-11-5       Zhou Yonghui ���Ӳ���pGlobal,OSS������
*               ֧�ֶ��̵߳�Ҫ��.
******************************************************/

extern "C"
int DeCodeBill_final_M_mt(void* pGlobal,
                          char* pVal,     /* ����������� */
                          int* pValLen,   /* �������ݳ���,����������ĳ��� */
                          char* pBuf,     /* ����������Ļ����� */
                          int nBufLen)    /* �������ĳ��� */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char *)pVal;
    encodedData.length = *pValLen;

    int pdu = Fin_M_BSRecord_PDU;

    int nErr = 0;
    if(nErr = ossSetDecodingLength(world,nBufLen))
    {
/*
        ossPrint(world, "Set decoding length fail:%s\n",
                 ossGetErrMsg(world));
*/
        return nErr;
    }

    if(nErr = ossDecode(world,&pdu,&encodedData, (void**)&pBuf))
    {
/*
        ossPrint(world, "Decode GPRS_CDR_FIN fail.Error code:%d\n",nErr);
*/
        return nErr;
    }

    long nDecodeLen = ossGetDecodingLength(world);
    if(nDecodeLen == -1)
    {
/*
        ossPrint(world,"ossGetDecodingLength fail.\n");
*/
        return -1;
    }

    *pValLen = nDecodeLen;

    return 0;
}


/******************************************************
* ������:   EnCodeGPRSBill_final_M_mt
* ����:    Zhou Yonghui
* ����:    01-6-1
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
*01-6-1        Zhou Yonghui
*01-11-5       Zhou Yonghui     ���Ӳ���pGlobal,OSS������
*                   ֧�ֶ��̵߳�Ҫ��.
******************************************************/

extern "C"
char* EnCodeBill_final_M_mt(void* pGlobal,
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
    if(nErr = ossEncode(world, Fin_M_BSRecord_PDU,(void*)pVal,
        &encodedData))
    {
        ossPrint(world, "Encode GPRS_CDR_FIN fail.Error code:%d\n",nErr);
        return NULL;
    }

    *pBufLen = encodedData.length;

    /*
     *���ܷ���pBuf,��ΪencodedData.value
     *���ܱ��޸Ķ�ָ��pBuf������λ�á�
     */
    return (char*)encodedData.value;
}


extern "C"
void* Init_OSS_Lib_Final_M()
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

    if(ossinit(world, CDRF_R98_Fin_M))
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

extern "C"
void Release_OSS_Lib_Final_M(void* pGlobal)
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


