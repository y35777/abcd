#ifndef __R99_3A0_MOTO_PATCH_H__
#define __R99_3A0_MOTO_PATCH_H__

class CR99_3a0PatchForMoto
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

    static BOOL ProcessNotMerged(const char* szTmpFileName,
                                 char*& szDestFileName, 
                                 UINT4 nChannelID);
    static BOOL ProcessMerged(const char* szTmpFileName, 
                              char*& szDestFileName,
                              UINT4 nChannelID);
    static BOOL PreProcess(const char* szTmpFileName,
                           STRING& sDestFileName);

private:

    static char  g_szCgID[15];           //CG的地址
    static char  g_szGsnID[15];          //Gsn的地址
    static BOOL  g_bMustMerged;          //话单是否合并
    static BOOL  g_bCMCC;                //Motorola是否用在CMCC
    static BYTE* g_pMemFile;             //内存映射文件指针
    static UINT4 g_nMemFileLen;          //内存映射文件的长度
    static UINT4 g_nMemDataLen;          //内存映射文件有效数据的长度
    static void* g_pASN1Global;          //用于进行对合并的话单进行解码的环境变量
    static char* g_pASN1RecContent;      //用于进行对合并的话单形成M结构的编码内存
    static int   g_nASN1RecContentLen;   //用于进行对合并的话单形成M结构的编码内存长度
    static void* g_pASN1Global_M;        //用于进行对不合并的话单进行解码的环境变量
    static void* g_pBSRecord;            //M的话单文件结构
    static void* g_pBSSummaryRecord;     //M的话单文件结构的表头部分
    static BYTE* g_pMemLink;             //内存映射文件指针
    static UINT4 g_nMemLinkLen;          //内存映射文件的长度
    //add by zkq 2004-07-08
    static BYTE* g_pMemDestFile  ;       //解码内存映射文件指针  
    static int   g_nMemDestFileLen ;     //解码内存映射文件的长度
    //end
};

#endif //__R99_3A0_MOTO_PATCH_H__

