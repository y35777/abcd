#ifndef  _TOOL_BOX_H_
#define  _TOOL_BOX_H_

#include "precom.h"
//#include "../utility/mml_report.h"
#include "../toolbase/ini_file.h"
#include "../toolbase/ini_file_ex.h"
#include "../utility/config.h"
//#include "../utility/mml_para.h"
//by ldf 2002.6.7 对应问题单D14393
#include "../toolbase/persist_resource.h"
#include "../toolbase/persist_err.h"
//修改完毕

//by ldf 对应问题单D20379  2003-02-21
#define CHAR_SPACE				' ' 	      //空格
#define MAXLEN					1024
#define ALIGN_LEFT				0	      //左对齐方式
#define ALIGN_RIGHT				1	      //右对齐方式
//end

#ifdef _PLATFORM_WIN32

IGWB_Svc_Export HWND GetConsoleWindowHandle();

#endif

//MAX_LINE_LEN是用来限制INI文件中一行的最多字节数，如果在MACRO.INI
//中出现一行字节数超过该值，则读INI文件会出现DATA_OVERFLOW错误。
#define	 MAX_LINE_LEN	     2048

//利用共享内存实现文件拷贝
IGWB_Svc_Export BOOL FileCopy(const char* pszSrcFile, const char* pszDstFile);

//利用共享内存实现文件拷贝，如果目标文件存在，则把源文件追加到目标文件后面
IGWB_Svc_Export BOOL FileCopyEx(const char* pszSrcFile, const char* pszDstFile);

//by ldf 对应问题单 D20379 2003-02-21
//填充字符
IGWB_Svc_Export void FillChar(char* szFillString, int nFill=0, char szFillChar=CHAR_SPACE, int alignment=ALIGN_LEFT);
//end

//移动文件
IGWB_Svc_Export BOOL FileMove(const char* pszSrcFile, const char* pszDstfFile);	

//删除文件
IGWB_Svc_Export BOOL FileDel(const char* pszFile);

//创建多级目录
IGWB_Svc_Export BOOL CreateDeepDir(const char* pszPath);	

//删除指定目录
IGWB_Svc_Export int SweepDirectory(char* szWorkDir);

//一组常用的字符串处理函数

//将字符串转换成大写
IGWB_Svc_Export void StringToUpper(char *pszBuffer);

//将字符串转换成小写
IGWB_Svc_Export void StringToLower(char *pszBuffer);

//去掉字符串中左边的空格
IGWB_Svc_Export void StringLTrim(char *szStr);

//去掉字符串右边的空格
IGWB_Svc_Export void StringRTrim(char *szStr);

//去掉字符串左右的空格
IGWB_Svc_Export void StringLRTrim(char *szStr);

//字符串大小写敏感比较
IGWB_Svc_Export int StringCmp(const char *szS1, const char *szS2);

//字符串大小写不敏感比较
IGWB_Svc_Export int StringiCmp(const char *szStr1, const char * szStr2);

//判断一个字符串是否数字
IGWB_Svc_Export BOOL IsDigitValue(const char *szValue);

//替换字符串里的子字符串
IGWB_Svc_Export void ReplaceSubStr(const char* szSrcStr, char* szDstStr, 
                                  const char* szOldSubStr, const char* szNewSubStr);

//得到一个指定文件的长度
IGWB_Svc_Export long FileSize(FILE* pFile);

//得到一个指定文件的长度
IGWB_Svc_Export long FileSize(const char* szFileName);

//将两个UINT2合成一个UINT4
IGWB_Svc_Export UINT4	MAKEUINT4(UINT2 nLowWord, UINT2 nHighWord);

//取UINT4的低2字节
IGWB_Svc_Export UINT2	LOWUINT2(UINT4 nDWORD);

//取UINT4的高2字节
IGWB_Svc_Export UINT2	HIGHUINT2(UINT4 nDWORD);

//by ldf 2002.6.7 对应问题单D14393
IGWB_Svc_Export const char* GetStringFromID(INT4 nID);
//修改完毕

# define SWAP_LONG(L) ((SWAP_WORD ((L) & 0xFFFF) << 16) \
        | SWAP_WORD(((L) >> 16) & 0xFFFF))
# define SWAP_WORD(L) ((((L) & 0x00FF) << 8) | (((L) & 0xFF00) >> 8))

#if defined (_PLATFORM_WIN32) || defined (_PLATFORM_LINUX)
    # define ITOHS(S) (S)
    # define ITOHL(L) (L)

    # define HTOIS(S) (S)
    # define HTOIL(L) (L)

    # define NTOHS(S) SWAP_WORD(S)
    # define NTOHL(L) SWAP_LONG(L)

    # define HTONS(S) SWAP_WORD(S)
    # define HTONL(L) SWAP_LONG(L)

#else

    # define ITOHS(S) SWAP_WORD(S)
    # define ITOHL(L) SWAP_LONG(L)

    # define HTOIS(S) SWAP_WORD(S)
    # define HTOIL(L) SWAP_LONG(L)

    # define NTOHS(S) (S)
    # define NTOHL(L) (L)

    # define HTONS(S) (S)
    # define HTONL(L) (L)

#endif //_PLATFORM_WIN32

IGWB_Svc_Export int GetFileList(const char* szPath, LIST<STRING>& qResult); 

//Added by ZhengYuqun 2003-08-27 SWPD01198
//将BCD码串转换为带尾0的字符串
IGWB_Svc_Export UINT4 BCDtoString(const BYTE* pSrcBCD,
                                  char* szDstStr,
                                  UINT4 uSrcLen,
                                  UINT4 uDstLen = 0);
//Added by Lucy 2004-05-28 OPT Version
//将RBCD码串转换为带尾0的字符串
IGWB_Svc_Export UINT4 RBCDtoString(const BYTE* pSrcRBCD,
                                   char* szDstStr,
                                   UINT4 uSrcLen,
                                   UINT4 uDstLen = 0);
//将带尾0的字符串转换为BCD码串
IGWB_Svc_Export UINT4 StringtoBCD(const char* szSrcStr,
                                  BYTE* pDstBCD,
                                  UINT4 uDstLen = 0,
                                  UINT4 uSrcLen = 0);

IGWB_Svc_Export int BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen);

//从time_t转换成yyyy-mm-dd hh:mm:ss格式
IGWB_Svc_Export STRING ConvertTimeToString(time_t timeSource);

//从yyyy-mm-dd hh:mm:ss格式转换成time_t
IGWB_Svc_Export time_t ConvertTimeToTimeT(const char* pszSource);

//从time_t转换成yyyy-mm-dd格式
IGWB_Svc_Export STRING ConvertTimeToString_YMD(time_t timeSource);

//从yyyy-mm-dd格式转换成time_t
IGWB_Svc_Export time_t ConvertTimeToTimeT_YMD(const char* pszSource);

IGWB_Svc_Export int exe_pro(const char* const szCmdLine,
                           int nTimeOut = -1);
IGWB_Svc_Export int exe_pro(const char* const szAppName,
                           const char* const szParaLine,
                           int nTimeOut = -1);

IGWB_Svc_Export void divide_file_path_name(const char* szSrcFileName,
                                          STRING& strPath,
                                          STRING& strFile);

IGWB_Svc_Export void CreateTrace(const char* szTraceFileName);

IGWB_Svc_Export void Trace(const char* szModuleName,
                          const char* szFmt, ...);

IGWB_Svc_Export void TraceAssert(const BOOL expression, 
                                const char* szSourceFile, 
                                const int nLineNum);

#ifdef  _PLATFORM_WIN32
IGWB_Svc_Export void TraceNTException(LPEXCEPTION_POINTERS pException);
#endif  //_PLATFORM_WIN32

#define TRACE ::Trace

#define ASSERT(expression) do \
{ \
    if(!(expression)) \
        TraceAssert(expression, __FILE__, __LINE__); \
}while(0);

/* 写错误处理的宏定义 */
IGWB_Svc_Export void TraceModuleErr(const char* szModule, int nErrNo);

IGWB_Svc_Export const char* GetCfgFilePath(void);

IGWB_Svc_Export const char* GetAppPath();

IGWB_Svc_Export const char* GetAppName();

//从系统中获取指定目录下面的所有日期目录返回日期目录个数
IGWB_Svc_Export int GetDateDir(const char* szPath, LIST<STRING>& qResult);


#endif  //_TOOL_BOX_H_
