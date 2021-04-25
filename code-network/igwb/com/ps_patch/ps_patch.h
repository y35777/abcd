#ifndef __PS_PATCH_H__
#define __PS_PATCH_H__

#define MTS_BACKSAVE_PATCH     "MT_BACKSAVEPATCH"

class CEventBillFile;

class CBSPatchForPS
{
public:
    static BOOL Init(const char* szConfigPath, BYTE nApID, UINT4 nChannelCount);
    static BOOL Process(const char* szFileName, char*& szDestFileName, UINT4 nChannelNum);
    static BOOL Terminate();

private:

    static int s_nChannelCount;          //通道数

    static BYTE* s_pBillBuf;             //读话单缓冲区
    static int   s_nBillBufSize;         //话单缓冲区长度

    static BYTE* s_pMemFile;             //写话单文件缓冲区
};

#endif
