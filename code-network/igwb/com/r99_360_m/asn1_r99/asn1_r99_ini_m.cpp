#include "CDRF_3GCDR_M.h"

/******************************************************
* 函数名:  DeCode3GBill_ini_M_mt
* 作者:    maimaoshi
* 日期:    02-12-13
* 描述:    遵循oss公司规范的ASN.1解码函数。
* 输入:
*       参数1 :char* pIn,待解码的话单记录缓冲区。
*       参数2 :int* pLen,话单记录的长度，并返回解码后话单的长度。
* 返回值: void*,存放编码结果的缓冲区,该缓冲区是静态内存，不能释放。
*          失败返回NULL。
* 说明:
* 更新记录:
*
* 日期            姓名                  描述
* ========== ================== ======================

******************************************************/

extern "C"
int DeCodeBill_ini_M_mt(void* pGlobal,
                        char* pVal,     /* 待解码的数据 */
                        int* pValLen,   /* 输入数据长度,输出解码结果的长度 */
                        char* pBuf,     /* 存放输出结果的缓冲区 */
                        int nBufLen)    /* 缓冲区的长度 */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char *)pVal;
    encodedData.length = *pValLen;

    int pdu = CDR3G_M_BSRecord_PDU;

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
* 函数名:  EnCode3GBill_ini_M_mt
* 作者:    maimaoshi
* 日期:    02-12-13
* 描述:
* 输入:
*       参数1 :char** pCdr,待编码的话单记录。
*       参数2 :int* pLen,返回编码结果的长度。
* 返回值: char*,返回输出编码结果的缓冲区，失败返回NULL。
*
* 说明: 返回的缓冲区指针指向静态内存，不能释放。
* 更新记录:
*
* 日期            姓名                  描述
* ========== ================== ======================

******************************************************/

extern "C"
char* EnCodeBill_ini_M_mt(void* pGlobal,
                          char* pVal,      /* 待编码的数据指针 */
                          char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                          int* pBufLen)    /* 输入缓冲区pBuf的长度，*/
                                           /* 并返回编码结果的长度 (in,out) */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char*)pBuf;
    encodedData.length = *pBufLen;

    int nErr = 0;
    if(nErr = ossEncode(world, CDR3G_M_BSRecord_PDU,(void*)pVal,
        &encodedData))
    {
		return NULL;
    }

    *pBufLen = encodedData.length;

    /*
     *不能返回pBuf,因为encodedData.value
     *可能被修改而指向pBuf的其他位置。
     */
    return (char*)encodedData.value;
}


extern "C"
void* Init_OSS_Lib_Ini_M()
{
    struct ossGlobal* world = new struct ossGlobal;

#ifdef ONE_DLL
    /*
        Windows平台的多线程要求。
        ACE要求链接多线程动态库，则OSS必须链接toedcomd.lib，
        这要求首先调用ossInitSync()以保证线程安全。
        并要求定义编译选项ONE_DLL。
    */
    if(ossInitSync()) return NULL;
#endif

    if(ossinit(world, CDRF_3GCDR_M))
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
    //Unix平台的多线程要求。
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
        与ossInitSync()调用对应。
    */
    ossTermSync();
#endif

    delete world;
}


