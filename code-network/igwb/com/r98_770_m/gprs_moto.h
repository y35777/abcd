#ifndef __R98_770_MOTO_PATCH_H__
#define __R98_770_MOTO_PATCH_H__

struct SMEM_LINK
{
    SMEM_LINK* pNext;
    UINT4 nLength;            //�ڴ�ӳ���ļ��ĳ���
    BYTE* pValue;             //��������ָ��
};

class CR98_770PatchForMoto
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

    static char  g_szCgID_GPRS[15];           //CG�ĵ�ַ
    static char  g_szGsnID_GPRS[15];          //Gsn�ĵ�ַ
    static BOOL  g_bMustMerged_GPRS;          //�����Ƿ�ϲ�
    static BOOL  g_bCMCC_GPRS;                //Motorola�Ƿ�����CMCC
    static BYTE* g_pMemFile_GPRS;             //�ڴ�ӳ���ļ�ָ��
    static UINT4 g_nMemFileLen_GPRS;          //�ڴ�ӳ���ļ��ĳ���
    static UINT4 g_nMemDataLen_GPRS;          //�ڴ�ӳ���ļ���Ч���ݵĳ���
    static void* g_pASN1Global_GPRS;          //���ڽ��жԺϲ��Ļ������н���Ļ�������
    static void* g_pASN1Global_M_GPRS;        //���ڽ��жԲ��ϲ��Ļ������н���Ļ�������
    static void* g_pBSRecord_GPRS;            //M�Ļ����ļ��ṹ
    static void* g_pBSSummaryRecord_GPRS;     //M�Ļ����ļ��ṹ�ı�ͷ����
    static SMEM_LINK* g_pMemLink_GPRS;
    //add by zkq 2004-07-08
    static BYTE* g_pMemDestFile  ;       //�����ڴ�ӳ���ļ�ָ��  
    static int   g_nMemDestFileLen ;          //�����ڴ�ӳ���ļ��ĳ���
    //end
};

#endif //__R98_770_MOTO_PATCH_H__

