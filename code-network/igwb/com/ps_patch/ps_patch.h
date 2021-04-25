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

    static int s_nChannelCount;          //ͨ����

    static BYTE* s_pBillBuf;             //������������
    static int   s_nBillBufSize;         //��������������

    static BYTE* s_pMemFile;             //д�����ļ�������
};

#endif
