#pragma warning(disable:4706)

#include "CDRF_3GPPR4_Org_M.h"

/******************************************************
* ������:   DeCodeR4Bill_ini_M_mt
* ����:
* ����:    01-10-8
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
*2003-06-02                     ��OMM���л�����ѯ��
******************************************************/

extern "C"
int DeCodeBill_ini_M_mt(void* pGlobal,
                        char* pVal,     /* ����������� */
                        int* pValLen,   /* �������ݳ���, ����������ĳ��� */
                        char* pBuf,     /* ����������Ļ����� */
                        int nBufLen)    /* �������ĳ��� */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char *)pVal;
    encodedData.length = *pValLen;

    int pdu = R4_Org_M_BSRecord_PDU;

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
        ossPrint(world, "Decode R4_CDR fail.Error code:%d\n",nErr);
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
* ������:   EnCodeR4Bill_ini_M_mt
* ����:    Guofeng, Guan
* ����:    2003-06-02
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
*2003-06-02                      ��OMM���л�����ѯ��
******************************************************/

extern "C"
char* EnCodeBill_ini_M_mt(void* pGlobal,
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
    if(nErr = ossEncode(world, R4_Org_M_BSRecord_PDU, (void*)pVal,
        &encodedData))
    {
        ossPrint(world, "Encode R4_CDR fail.Error code:%d\n",nErr);
        return NULL;
    }

    *pBufLen = encodedData.length;

    /*
     * ���ܷ���pBuf,��ΪencodedData.value
     * ���ܱ��޸Ķ�ָ��pBuf������λ�á�
     */
    return (char*)encodedData.value;
}

extern "C"
void* Init_OSS_Lib_Ini_M()
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

    if(ossinit(world, CDRF_R4_Org_M))
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
void Release_OSS_Lib_Ini_M(void* pGlobal)
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

