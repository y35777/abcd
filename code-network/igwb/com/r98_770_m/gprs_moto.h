#ifndef __R98_770_MOTO_PATCH_H__
#define __R98_770_MOTO_PATCH_H__

struct SMEM_LINK
{
    SMEM_LINK* pNext;
    UINT4 nLength;            //内存映射文件的长度
    BYTE* pValue;             //话单链表指针
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

    static char  g_szCgID_GPRS[15];           //CG的地址
    static char  g_szGsnID_GPRS[15];          //Gsn的地址
    static BOOL  g_bMustMerged_GPRS;          //话单是否合并
    static BOOL  g_bCMCC_GPRS;                //Motorola是否用在CMCC
    static BYTE* g_pMemFile_GPRS;             //内存映射文件指针
    static UINT4 g_nMemFileLen_GPRS;          //内存映射文件的长度
    static UINT4 g_nMemDataLen_GPRS;          //内存映射文件有效数据的长度
    static void* g_pASN1Global_GPRS;          //用于进行对合并的话单进行解码的环境变量
    static void* g_pASN1Global_M_GPRS;        //用于进行对不合并的话单进行解码的环境变量
    static void* g_pBSRecord_GPRS;            //M的话单文件结构
    static void* g_pBSSummaryRecord_GPRS;     //M的话单文件结构的表头部分
    static SMEM_LINK* g_pMemLink_GPRS;
    //add by zkq 2004-07-08
    static BYTE* g_pMemDestFile  ;       //解码内存映射文件指针  
    static int   g_nMemDestFileLen ;          //解码内存映射文件的长度
    //end
};

#endif //__R98_770_MOTO_PATCH_H__

