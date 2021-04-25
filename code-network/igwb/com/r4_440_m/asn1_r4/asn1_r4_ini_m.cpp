#pragma warning(disable:4706)

#include "CDRF_3GPPR4_Org_M.h"

/******************************************************
* 函数名:   DeCodeR4Bill_ini_M_mt
* 作者:
* 日期:    01-10-8
* 描述:
* 输入:
*       参数1 :char* pVal
*       参数2 :int nValLen
*       参数3 :char* pBuf
*       参数4 :int BufLen
*
* 返回值: int,解码成功返回TRUE，失败返回FALSE。
* 说明: 这个函数是线程安全的。
* 更新记录:
*
* 日期            姓名                  描述
* ========== ================== ======================
*2003-06-02                     供OMM进行话单查询。
******************************************************/

extern "C"
int DeCodeBill_ini_M_mt(void* pGlobal,
                        char* pVal,     /* 待解码的数据 */
                        int* pValLen,   /* 输入数据长度, 输出解码结果的长度 */
                        char* pBuf,     /* 存放输出结果的缓冲区 */
                        int nBufLen)    /* 缓冲区的长度 */
{
    struct ossGlobal* world = (struct ossGlobal*)pGlobal;

    OssBuf encodedData;
    encodedData.value = (unsigned char *)pVal;
    encodedData.length = *pValLen;

    int pdu = R4_Org_M_BSRecord_PDU;

    int nErr = 0;
    if(nErr = ossSetDecodingLength(world,nBufLen))
    {
		//zyx 去掉注释
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
* 函数名:   EnCodeR4Bill_ini_M_mt
* 作者:    Guofeng, Guan
* 日期:    2003-06-02
* 描述:
* 输入:
*       参数1 :char* pVal
*       参数2 :char* pBuf
*       参数2 :int* pBufLen
*
* 返回值: char*,存放返回结果的缓冲区。
* 说明: 该函数支持多线程。
* 更新记录:
*
* 日期            姓名                  描述
* ========== ================== ======================
*2003-06-02                      供OMM进行话单查询。
******************************************************/

extern "C"
char* EnCodeBill_ini_M_mt(void* pGlobal,
                          char* pVal,      /* 待编码的数据指针 */
                          char* pBuf,      /* 编码结果函数运算使用的缓冲区 */
                          int* pBufLen)    /* 输入缓冲区pBuf的长度,*/
                                           /* 并返回编码结果的长度 (in,out) */
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
     * 不能返回pBuf,因为encodedData.value
     * 可能被修改而指向pBuf的其他位置。
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

