#ifndef  _TOOL_BOX_H_
#define  _TOOL_BOX_H_

#include "precom.h"
//#include "../utility/mml_report.h"
#include "../toolbase/ini_file.h"
#include "../toolbase/ini_file_ex.h"
#include "../utility/config.h"
//#include "../utility/mml_para.h"
//by ldf 2002.6.7 ��Ӧ���ⵥD14393
#include "../toolbase/persist_resource.h"
#include "../toolbase/persist_err.h"
//�޸����

//by ldf ��Ӧ���ⵥD20379  2003-02-21
#define CHAR_SPACE				' ' 	      //�ո�
#define MAXLEN					1024
#define ALIGN_LEFT				0	      //����뷽ʽ
#define ALIGN_RIGHT				1	      //�Ҷ��뷽ʽ
//end

#ifdef _PLATFORM_WIN32

IGWB_Svc_Export HWND GetConsoleWindowHandle();

#endif

//MAX_LINE_LEN����������INI�ļ���һ�е�����ֽ����������MACRO.INI
//�г���һ���ֽ���������ֵ�����INI�ļ������DATA_OVERFLOW����
#define	 MAX_LINE_LEN	     2048

//���ù����ڴ�ʵ���ļ�����
IGWB_Svc_Export BOOL FileCopy(const char* pszSrcFile, const char* pszDstFile);

//���ù����ڴ�ʵ���ļ����������Ŀ���ļ����ڣ����Դ�ļ�׷�ӵ�Ŀ���ļ�����
IGWB_Svc_Export BOOL FileCopyEx(const char* pszSrcFile, const char* pszDstFile);

//by ldf ��Ӧ���ⵥ D20379 2003-02-21
//����ַ�
IGWB_Svc_Export void FillChar(char* szFillString, int nFill=0, char szFillChar=CHAR_SPACE, int alignment=ALIGN_LEFT);
//end

//�ƶ��ļ�
IGWB_Svc_Export BOOL FileMove(const char* pszSrcFile, const char* pszDstfFile);	

//ɾ���ļ�
IGWB_Svc_Export BOOL FileDel(const char* pszFile);

//�����༶Ŀ¼
IGWB_Svc_Export BOOL CreateDeepDir(const char* pszPath);	

//ɾ��ָ��Ŀ¼
IGWB_Svc_Export int SweepDirectory(char* szWorkDir);

//һ�鳣�õ��ַ���������

//���ַ���ת���ɴ�д
IGWB_Svc_Export void StringToUpper(char *pszBuffer);

//���ַ���ת����Сд
IGWB_Svc_Export void StringToLower(char *pszBuffer);

//ȥ���ַ�������ߵĿո�
IGWB_Svc_Export void StringLTrim(char *szStr);

//ȥ���ַ����ұߵĿո�
IGWB_Svc_Export void StringRTrim(char *szStr);

//ȥ���ַ������ҵĿո�
IGWB_Svc_Export void StringLRTrim(char *szStr);

//�ַ�����Сд���бȽ�
IGWB_Svc_Export int StringCmp(const char *szS1, const char *szS2);

//�ַ�����Сд�����бȽ�
IGWB_Svc_Export int StringiCmp(const char *szStr1, const char * szStr2);

//�ж�һ���ַ����Ƿ�����
IGWB_Svc_Export BOOL IsDigitValue(const char *szValue);

//�滻�ַ���������ַ���
IGWB_Svc_Export void ReplaceSubStr(const char* szSrcStr, char* szDstStr, 
                                  const char* szOldSubStr, const char* szNewSubStr);

//�õ�һ��ָ���ļ��ĳ���
IGWB_Svc_Export long FileSize(FILE* pFile);

//�õ�һ��ָ���ļ��ĳ���
IGWB_Svc_Export long FileSize(const char* szFileName);

//������UINT2�ϳ�һ��UINT4
IGWB_Svc_Export UINT4	MAKEUINT4(UINT2 nLowWord, UINT2 nHighWord);

//ȡUINT4�ĵ�2�ֽ�
IGWB_Svc_Export UINT2	LOWUINT2(UINT4 nDWORD);

//ȡUINT4�ĸ�2�ֽ�
IGWB_Svc_Export UINT2	HIGHUINT2(UINT4 nDWORD);

//by ldf 2002.6.7 ��Ӧ���ⵥD14393
IGWB_Svc_Export const char* GetStringFromID(INT4 nID);
//�޸����

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
//��BCD�봮ת��Ϊ��β0���ַ���
IGWB_Svc_Export UINT4 BCDtoString(const BYTE* pSrcBCD,
                                  char* szDstStr,
                                  UINT4 uSrcLen,
                                  UINT4 uDstLen = 0);
//Added by Lucy 2004-05-28 OPT Version
//��RBCD�봮ת��Ϊ��β0���ַ���
IGWB_Svc_Export UINT4 RBCDtoString(const BYTE* pSrcRBCD,
                                   char* szDstStr,
                                   UINT4 uSrcLen,
                                   UINT4 uDstLen = 0);
//����β0���ַ���ת��ΪBCD�봮
IGWB_Svc_Export UINT4 StringtoBCD(const char* szSrcStr,
                                  BYTE* pDstBCD,
                                  UINT4 uDstLen = 0,
                                  UINT4 uSrcLen = 0);

IGWB_Svc_Export int BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen);

//��time_tת����yyyy-mm-dd hh:mm:ss��ʽ
IGWB_Svc_Export STRING ConvertTimeToString(time_t timeSource);

//��yyyy-mm-dd hh:mm:ss��ʽת����time_t
IGWB_Svc_Export time_t ConvertTimeToTimeT(const char* pszSource);

//��time_tת����yyyy-mm-dd��ʽ
IGWB_Svc_Export STRING ConvertTimeToString_YMD(time_t timeSource);

//��yyyy-mm-dd��ʽת����time_t
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

/* д������ĺ궨�� */
IGWB_Svc_Export void TraceModuleErr(const char* szModule, int nErrNo);

IGWB_Svc_Export const char* GetCfgFilePath(void);

IGWB_Svc_Export const char* GetAppPath();

IGWB_Svc_Export const char* GetAppName();

//��ϵͳ�л�ȡָ��Ŀ¼�������������Ŀ¼��������Ŀ¼����
IGWB_Svc_Export int GetDateDir(const char* szPath, LIST<STRING>& qResult);


#endif  //_TOOL_BOX_H_
