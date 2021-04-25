#ifndef __R4_440_MOTO_PATCH_H__
#define __R4_440_MOTO_PATCH_H__

struct SMEM_LINK
{
    SMEM_LINK* pNext;
    UINT4 nLength;            //�ڴ�ӳ���ļ��ĳ���
    BYTE* pValue;             //��������ָ��
};

class CR4_440PatchForMoto
{
public:
    static BOOL Init(const char* szConfigPath,
                     BYTE nApID, 
                     UINT4 nChannelCount);
    static BOOL Process(const char* szFileName, 
                        char*& szDestFileName,
                        UINT4 nChannelID);
    static BOOL Terminate();

private:

    static void DeleteMemLink(SMEM_LINK* p);
    static BOOL ProcessNotMerged(const char* szTmpFileName,
                                 char*& szDestFileName, 
                                 UINT4 nChannelID);
    static BOOL ProcessMerged(const char* szTmpFileName, 
                              char*& szDestFileName,
                              UINT4 nChannelID);
    static BOOL PreProcess(const char* szTmpFileName,
                           STRING& sDestFileName);

private:

	static char  g_szCgID[15];           //CG�ĵ�ַ
	static char  g_szGsnID[15];          //Gsn�ĵ�ַ
	static BOOL  g_bMustMerged;          //�����Ƿ�ϲ�
	static BOOL  g_bCMCC;                //Motorola�Ƿ�����CMCC

	static BYTE* g_pMemFile;             //�ڴ�ӳ���ļ�ָ��
	static UINT4 g_nMemFileLen;          //�ڴ�ӳ���ļ��ĳ���
	static UINT4 g_nMemDataLen;          //�ڴ�ӳ���ļ���Ч���ݵĳ���

	static void* g_pASN1Global;          //���ڽ��жԺϲ��Ļ������н���Ļ�������
	static void* g_pASN1Global_M;        //���ڽ��жԲ��ϲ��Ļ������н���Ļ�������

	static void* g_pBSRecord;            //M�Ļ����ļ��ṹ
	static void* g_pBSSummaryRecord;     //M�Ļ����ļ��ṹ�ı�ͷ����

    static SMEM_LINK* g_pMemLink;
    //add by zkq 2004-07-08
    static BYTE* g_pMemDestFile  ;       //�����ڴ�ӳ���ļ�ָ��  
    static int   g_nMemDestFileLen ;          //�����ڴ�ӳ���ļ��ĳ���
    //end
};

#endif //__R4_440_MOTO_PATCH_H__

