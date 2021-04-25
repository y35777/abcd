#ifndef __CMCC_CS_BS_PATCH_H__
#define __CMCC_CS_BS_PATCH_H__

#include "CMCC_CS_R4_CDR.h"

#define MTS_BACKSAVE_PATCH     "MT_BACKSAVEPATCH"

class CEventBillFile;

class CBSPatchForCMCC
{
public:
    static BOOL Init(char* szConfigPath, BYTE nApID, UINT4 nChannelCount);
    static BOOL Process(char* szFileName, char*& szDestFileName, UINT4 nChannelNum);
    static BOOL Terminate();
    static void Clear_Memory(CMCC_CS_CallEventDataFile& DataFile);

	//by 杨善	2004-6-1	对应问题单SWPD05564
	static	BOOL CanClose(char* szFileName);
	//the end	 对应问题单SWPD05564

private:

    static int s_nBillLen;               //话单长度
    static int s_nChannelCount;          //通道数

    static CEventBillFile* s_pEventFile; //存放事件话单的文件
    static char* s_pBillBuf;             //读写话单缓冲区
    static int   s_nBillBufSize;         //话单缓冲区长度
    static SET<int>   s_LCSSet;          //LCS类话单类型集合

    static void* s_pASN1Global;          //ASN1的全局变量
    static char* s_pMemFile;             //ASN1编码文件缓冲区
    static int   s_nMemFileLen;          //ASN1编码文件缓冲区长度
};

#endif
