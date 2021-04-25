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

	//by ����	2004-6-1	��Ӧ���ⵥSWPD05564
	static	BOOL CanClose(char* szFileName);
	//the end	 ��Ӧ���ⵥSWPD05564

private:

    static int s_nBillLen;               //��������
    static int s_nChannelCount;          //ͨ����

    static CEventBillFile* s_pEventFile; //����¼��������ļ�
    static char* s_pBillBuf;             //��д����������
    static int   s_nBillBufSize;         //��������������
    static SET<int>   s_LCSSet;          //LCS�໰�����ͼ���

    static void* s_pASN1Global;          //ASN1��ȫ�ֱ���
    static char* s_pMemFile;             //ASN1�����ļ�������
    static int   s_nMemFileLen;          //ASN1�����ļ�����������
};

#endif
