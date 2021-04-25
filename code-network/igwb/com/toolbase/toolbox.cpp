#include "../include/precom.h"
#include "../include/export_dll.h"
#include "../include/toolbox.h"
#include <errno.h>

/* CTrace类的定义头文件 */
#include "trace.h"

/* CPersistErr类的定义头文件 */
#include "persist_err.h"

/* Trace对象 */
CTrace g_TraceFile;

/* 主目录路径名,如:"c:\\billsvr"、"."或"" */
IGWB_Svc_Export char g_szAppPath[MAX_PATH] = "";

/* 配置文件绝对路径名,如:"c:\\billsvr\\ini\\billsvr.ini" */
IGWB_Svc_Export char g_szCfgPathName[MAX_PATH] = "";

/* 程序名不带路径名和后缀 */
IGWB_Svc_Export char g_szAppName[MAX_PATH] = "";


IGWB_Svc_Export void CreateTrace(const char* szTraceFileName)
{
    g_TraceFile.Open(szTraceFileName);
}

#ifdef  _PLATFORM_WIN32
IGWB_Svc_Export void TraceNTException(LPEXCEPTION_POINTERS pException)
{
    g_TraceFile.TraceNTException(pException);
}
#endif  //_PLATFORM_WIN32

IGWB_Svc_Export void Trace(const char* szModuleName,
                           const char* szFmt, ...)
{
    char buf[1024];
    char* pBuf = buf;
    int nLen = 0;

    va_list argptr;

    SNPRINTF(pBuf,sizeof(buf),"(%s) ",szModuleName);
    buf[sizeof(buf) - 1] = 0;

    nLen = strlen(pBuf);
    pBuf += nLen;

    va_start(argptr, szFmt);
    VSNPRINTF(pBuf,(sizeof(buf) - nLen),szFmt,argptr);
    buf[sizeof(buf) - 1] = 0;

    va_end(argptr);

    if (strlen(buf) > (nLen + 1) || ((strlen(buf) == (nLen + 1)) && (buf[nLen] != '-')))
    {
        g_TraceFile.Trace(buf);
    }
}

IGWB_Svc_Export void TraceAssert(const BOOL expression,
                                 const char* szSourceFile,
                                 const int nLineNum)
{
    g_TraceFile.TraceAssert(expression, szSourceFile, nLineNum);
}

IGWB_Svc_Export const char* GetCfgFilePath(void)
{
    return g_szCfgPathName;
}

IGWB_Svc_Export const char* GetAppPath()
{
    return g_szAppPath;
}

IGWB_Svc_Export const char* GetAppName()
{
    return g_szAppName;
}

IGWB_Svc_Export void TraceModuleErr(const char* szModule, int nErrNo)
{
    const char* p = CPersistErr::Instance()->GetError(nErrNo);
    if((p != NULL) && (p[0] != '\0'))
    {
        TRACE(szModule, p);
    }
}

//by ldf 对应问题单D20379 2003-02-21
/***********************************************
*功能描述  填充nFill个szFillChar.
*输入参数
* szFillString 输入的源字符串
* nFill -要加入的字符数
* szFillChar 要加入的字符
* alignment 加入方式左加入还是右加入，可选为 ALIGN_LEFT 和 ALIGN_RIGHT
*李杜芳 2003-02-21
*************************************************/
IGWB_Svc_Export void FillChar(char* szFillString, int nFill, char szFillChar,int alignment)
{
    char* szTemp = szFillString;

    char szTempFill[MAXLEN];

    int nstrlen = strlen(szFillString);

    if(nFill + nstrlen >= MAXLEN)
    {
        return;
    }

    for(int i=0; i<=nFill; i++)
    {
        szTempFill[i] = szFillChar;
    }
    szTempFill[nFill] = '\0';

    if(ALIGN_LEFT==alignment)
    {
        strcat(szTempFill,szTemp);
        strncpy(szFillString,szTempFill,nFill+nstrlen);
    }
    else
    {
        strcat(szTemp,szTempFill);
        strncpy(szFillString,szTemp,nFill+nstrlen);
    }

}


/****************************************************************************
 *函数功能：利用标准C函数实现文件拷贝操作
 *输入：
 *     pszSrcFile：源文件名（全路径）
 *     pszDstFile：目标文件名（全路径）
 *输出：
 *     无
 *返回：
 *     拷贝执行成功返回TRUE，否则返回FALSE.
 *****************************************************************************/
IGWB_Svc_Export BOOL FileCopy(const char* pszSrcFile, const char* pszDstFile)
{
    static const int FILE_BUFFER_SIZE = 4096;
    char szBuf[FILE_BUFFER_SIZE];
    int nFileLen;
    int nCopiedBytes = 0;
    int nCopyNeed = 0;

    //打开原始文件
    FILE* fOrig = fopen(pszSrcFile, "rb");
    if (NULL == fOrig)
    {
        return FALSE;
    }

    if (0 != fseek(fOrig, 0, SEEK_END))
    {
        fclose(fOrig);
        return FALSE;
    }
    nFileLen = ftell(fOrig);
    if (0 != fseek(fOrig, 0, SEEK_SET))
    {
        fclose(fOrig);
        return FALSE;
    }

    //打开目标文件
    FILE* fDest = fopen(pszDstFile, "wb");
    if (NULL == fDest)
    {
        fclose(fOrig);
        return FALSE;
    }

    while(nFileLen > nCopiedBytes)
    {
        nCopyNeed = min(nFileLen - nCopiedBytes, FILE_BUFFER_SIZE);

        if ((1 != fread(szBuf, nCopyNeed, 1, fOrig)) ||
            (1 != fwrite(szBuf, nCopyNeed, 1, fDest)))
        {
            fclose(fOrig);
            fclose(fDest);
            remove(pszDstFile);
            return FALSE;
        }

        nCopiedBytes += nCopyNeed;
    }
    fclose(fOrig);
    fclose(fDest);
    return TRUE;
}

/****************************************************************************
 *函数功能：利用标准C函数实现文件拷贝操作。这儿扩展了一个功能，
            如果目标文件存在，则把源文件追加到目标文件后面
 *输入：
 *     pszSrcFile：源文件名（全路径）
 *     pszDstFile：目标文件名（全路径）
 *输出：
 *     无
 *返回：
 *     拷贝执行成功返回TRUE，否则返回FALSE. 
 *****************************************************************************/
IGWB_Svc_Export BOOL FileCopyEx(const char* pszSrcFile, const char* pszDstFile)
{
    static const int FILE_BUFFER_SIZE = 4096;
    char szBuf[FILE_BUFFER_SIZE];
    int nFileLen;
    int nCopiedBytes = 0;
    int nCopyNeed = 0;

    //打开原始文件
    FILE* fOrig = fopen(pszSrcFile, "rb");
    if (NULL == fOrig) 
    {
        return FALSE;
    }

    if (0 != fseek(fOrig, 0, SEEK_END))
    {
        fclose(fOrig);
        return FALSE;
    }
    nFileLen = ftell(fOrig);
    if (0 != fseek(fOrig, 0, SEEK_SET))
    {
        fclose(fOrig);
        return FALSE;
    }

    //打开目标文件
    FILE* fDest = fopen(pszDstFile, "rb+");
    if (NULL == fDest) 
    {
        fDest = fopen(pszDstFile, "wb");
        if (NULL == fDest) 
        {
            fclose(fOrig);
            return FALSE;
        }
    }
    else
    {
        if (0 != fseek(fDest, 0, SEEK_END))
        {
            fclose(fOrig);
            fclose(fDest);
            return FALSE;
        }
    }

    while(nFileLen > nCopiedBytes)
    {
        nCopyNeed = min(nFileLen - nCopiedBytes, FILE_BUFFER_SIZE);

        if ((1 != fread(szBuf, nCopyNeed, 1, fOrig)) || 
            (1 != fwrite(szBuf, nCopyNeed, 1, fDest)))
        {
            fclose(fOrig);
            fclose(fDest);
            remove(pszDstFile);
            return FALSE;
        }

        nCopiedBytes += nCopyNeed;
    }
    fclose(fOrig);
    fclose(fDest);
    return TRUE;
}


/****************************************************************************
 *函数功能：移动文件
 *输入：
 *     pszSrcFile：源文件名（全路径）
 *     pszDstFile：目标文件名（全路径）
 *输出：
 *     无
 *返回：
 *     移动执行成功返回TRUE，否则返回FALSE.
 ****************************************************************************/
IGWB_Svc_Export BOOL FileMove(const char* pszSrcFile, const char* pszDstFile)
{
    //利用ACE提供的文件改名操作实现文件移动
    int nRet = ACE_OS::rename(pszSrcFile, pszDstFile);

    if (nRet != 0)
    {
        return FALSE;
    }

    return TRUE;
}

/****************************************************************************
 *函数功能：删除文件
 *输入：
 *     pszFile：文件名（全路径）
 *输出：
 *     无
 *返回：
 *     删除执行成功返回TRUE，否则返回FALSE.
 ****************************************************************************/
IGWB_Svc_Export BOOL FileDel(const char* pszFile)
{
    int nRet = ACE_OS::unlink(pszFile);

    //待删除的文件不存在
    if ((nRet == -1) && (errno != ENOENT))
    {
        return FALSE;
    }

    return TRUE;
}

/****************************************************************************
 *函数功能：创建多级(包括一级)目录
 *输入：
 *     pszFile：文件名（全路径）
 *输出：
 *     无
 *返回：
 *     删除执行成功返回TRUE，否则返回FALSE.
 ****************************************************************************/
IGWB_Svc_Export BOOL CreateDeepDir(const char* pszPath)
{
    //目录名中部用于出现的字符
    const char pInvalidChars[] =
         { '\\', '/', ':', '*', '?', '"', '<', '>', '|', '\0'};

    char szTempDir[MAX_PATH];

    strcpy(szTempDir, pszPath);
    int nLen = strlen(pszPath);
    if (0 == nLen)
    {
        return FALSE;
    }

    //如果最后一个字符不是'\\'或者并且不是'/'，则在字符串尾添加
    //'/'
    if ((szTempDir[nLen] != '\\') && (szTempDir[nLen] != '/'))
    {
        strcat(szTempDir, "/");
    }

    nLen = strlen(szTempDir);

    for (int i = 0; i < nLen; i++)
    {
        if (((szTempDir[i] == '\\') || (szTempDir[i] == '/'))
         && (i > 0))
        {
            //检查目录名的合法性
            if (strchr(pInvalidChars, szTempDir[i-1]))
            {
                continue;
            }

            szTempDir[i] = 0;    //设置字符串结束符

            //在创建多级目录时，如果目录已存在，则不认为是错误，
            //继续创建下一级目录；如果创建目录失败，但失败原因
            //并不是目录已存在，则终止创建，函数返回FALSE。
            //by ldf 2002.4.22 对应问题单 D13063
            #ifdef _PLATFORM_WIN32
                if((ACE_OS::mkdir(szTempDir) == -1) && (GetLastError() != ERROR_ALREADY_EXISTS) )
            #else
                if((ACE_OS::mkdir(szTempDir) == -1) && (errno != EEXIST) )
            #endif
                {
                    return FALSE;
                }
            //修改完毕

            szTempDir[i] = '/';  //恢复字符串
        }
    }

    return TRUE;
}

/****************************************************************************
 *函数功能：字符串大小写不敏感比较
 *输入：
 *     szStr1：需要比较得字符串1
 *     szStr2：需要比较得字符串2
 *输出：
 *     字符串是否相等, 0－不等，1－相等
 ****************************************************************************/
IGWB_Svc_Export int StringiCmp(const char *szStr1, const char *szStr2)
{
    const char *szTemp1 = szStr1;
    const char *szTemp2 = szStr2;
    char cChar1;
    char cChar2;
    char cDiff = 'a' - 'A';

    if (strlen(szStr1) != strlen(szStr2))
    {
        return 0;       //长度不相等
    }

    while(*szTemp1 != '\0')
    {
        cChar1 = *szTemp1;
        cChar2 = *szTemp2;

        szTemp1++;
        szTemp2++;

        if (cChar1 >= 'a' && cChar1 <= 'z')
        {
            cChar1 -= cDiff;  //转换成大写
        }

        if (cChar2 >= 'a' && cChar2 <= 'z')
        {
            cChar2 -= cDiff;  //转换成大写
        }

        if (cChar1 != cChar2)
        {
            return 0;
        }
    }

    return 1;
}

/***************************************************************************
 *函数功能：字符串大小写敏感比较
 *输入：
 *     szS1：需要比较得字符串1
 *     szS2：需要比较得字符串2
 *输出：
 *     字符串是否相等, 0－不等，1－相等
 ***************************************************************************/
IGWB_Svc_Export int StringCmp(const char *szS1,  const char *szS2)
{
    const char *szTmp1 = szS1;
    const char *szTmp2 = szS2;

    if (strlen(szS1) != strlen(szS2))
    {
        return 0;
    }

    while (*szTmp1 != '\0')
    {
        if (*szTmp1 == *szTmp2)
        {
            szTmp1++;
            szTmp2++;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

/**************************************************************************
 *函数功能：将一个字符串全部转换成大写
 *输入：
 *     szStr：待转换的字符串
 *输出：
 *     szStr：转换完成的字符串
 **************************************************************************/
IGWB_Svc_Export void StringToUpper(char *szStr)
{
    char cDiff = 'a' - 'A';

    while (*szStr)
    {
        if (*szStr >= 'a' && *szStr <= 'z')
        {
            *szStr -= cDiff;
        }

        szStr++;
    }
}

void StringToLower(char *szStr)
{
    char cDiff = 'a' - 'A';

    while (*szStr)
    {
        if (*szStr >= 'A' && *szStr <= 'Z')
        {
            *szStr += cDiff;
        }

        szStr++;
    }
}

/***************************************************************************
 *函数功能：去掉字符串左边的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 **************************************************************************/
IGWB_Svc_Export void StringLTrim(char *szStr)
{
    char* szTemp = NULL;

    szTemp = szStr;

    while ((*szTemp == ' ')  || (*szTemp == '\t')
        || (*szTemp == '\r') || (*szTemp == '\n'))
    {
        szTemp++;
    }

    //by lx SWPD01788
    if(szTemp != szStr)
    {
        strcpy(szStr, szTemp);
    }
    //修改结束by lx
}

/************************************************************************
 *函数功能：去掉字符串右边的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 ************************************************************************/
IGWB_Svc_Export void StringRTrim(char *szStr)
{
    char *szTemp = NULL;
    char ch;
    int  nLen;

    nLen = strlen(szStr);

    szTemp = szStr + nLen;

    while (szTemp != szStr)
    {
        szTemp--;
        ch = *szTemp;
        if((ch == ' ') || (ch == '\n')
           || (ch == '\t') || (ch == '\r'))
        {
            *szTemp = '\0';
        }
        else
        {
            break;
        }
    }
}

/*************************************************************************
 *函数功能：去掉字符串左右的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 *************************************************************************/
IGWB_Svc_Export void StringLRTrim(char *szStr)
{
    StringLTrim(szStr);
    StringRTrim(szStr);
}

IGWB_Svc_Export BOOL IsDigitValue(const char *szValue)
{
    char ch;

    ch = *szValue;

    //字符串为空
    if (ch == '\0') return FALSE;

    while (ch != '\0')
    {
        if (ch < '0' || ch > '9')
        {
            return (FALSE);
        }

        ch = *(++szValue);
    }

    return (TRUE);
}

/***********************************************************************
 *函数功能：将两个UINT2合成一个UINT4
 *输入：
 *     nLowWord：低2字节
 *     nHighWord：高2字节
 *输出：
 *     由nLowWord和nHighWord合成一个UINT4
 ***********************************************************************/
IGWB_Svc_Export UINT4 MAKEUINT4(UINT2 nLowWord, UINT2 nHighWord)
{
    UINT4   nDWORD;

    nDWORD = (nHighWord << 16) + nLowWord;

    return nDWORD;
}

/*************************************************************************
 *函数功能：取出UINT4中的低2字节
 *输入：
 *     nDWORD：UINT4数据
 *输出：
 *     nDWORD的低2字节
 ************************************************************************/
IGWB_Svc_Export UINT2 LOWUINT2(UINT4 nDWORD)
{
    return (UINT2)(nDWORD & 0xFFFF);
}

/************************************************************************
 *函数功能：取出UINT4中的高2字节
 *输入：
 *     nDWORD：UINT4数据
 *输出：
 *     nDWORD的高2字节
 *************************************************************************/
IGWB_Svc_Export UINT2 HIGHUINT2(UINT4 nDWORD)
{
    return (UINT2)((nDWORD & 0xFFFF0000) >> 16);
}

/************************************************************************
 *函数功能：替换字符串里的子字符串
 *输入参数：
 *    zSrcStr：整个字符串
 *    szOldSubStr：要被替换的子字符串
 *    szNewSubStr：用来替换的子字符串
 *输出参数：
 *    szDstStr：替换了子字符串后的整个字符串，空间在函数外申请和释放
 ************************************************************************/
IGWB_Svc_Export void ReplaceSubStr(const char* szSrcStr, char* szDstStr,
                                  const char* szOldSubStr, const char* szNewSubStr)
{
    int nStrLen = strlen(szSrcStr);
    int nOldSubStrLen = strlen(szOldSubStr);
    int nNewSubStrLen = strlen(szNewSubStr);
    int nReadPos = 0, nWritePos = 0;

    //在字符串中顺序搜索所有的字符串szOldSubStr，用szNewSubStr代替拷贝到szDstStr中,
    //对于其它的字符串，则直接拷贝到szDstStr中
    while (nReadPos < nStrLen)
    {
        const char* szSubStr = strstr(szSrcStr + nReadPos, szOldSubStr);

        if (NULL != szSubStr)
        {
            memcpy(szDstStr + nWritePos,
                   szSrcStr + nReadPos,
                   szSubStr - szSrcStr - nReadPos);
            nWritePos += szSubStr - szSrcStr - nReadPos;
            memcpy(szDstStr + nWritePos,
                   szNewSubStr,
                   nNewSubStrLen);
            nWritePos += nNewSubStrLen;
            nReadPos = szSubStr - szSrcStr + nOldSubStrLen;
        }
        else
        {
            memcpy(szDstStr + nWritePos,
                   szSrcStr + nReadPos,
                   nStrLen - nReadPos);
            nWritePos += nStrLen - nReadPos;
            nReadPos = nStrLen;
        }
    }

    szDstStr[nWritePos] = '\0';
}

/************************************************************************
 *函数功能：得到一个指定文件的长度
 *输入参数：
 *    pFile：指定的文件对象
 *返回值：
 *    指定文件的长度
 ************************************************************************/
IGWB_Svc_Export long FileSize(FILE* pFile)
{
    long lOldPos = ftell(pFile);

    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    fseek(pFile, lOldPos, SEEK_SET);

    return lSize;
}

//得到一个指定文件的长度
IGWB_Svc_Export long FileSize(const char* szFileName)
{
    if(NULL != szFileName)
    {
        FILE* f = fopen(szFileName, "rb");
        if(NULL != f)
        {
            int nFileLen = FileSize(f);
            fclose(f);

            return nFileLen;
        }
    }

    return 0;
}

/************************************************************************
 *函数功能：删除指定目录及其中的文件
 *输入参数：
 *    szWorkDir：指定的目录路径
 *返回值：
 *    删除目录是否成功
 ************************************************************************/
IGWB_Svc_Export int SweepDirectory(char* szWorkDir)
{
    /*要求ACE版本为4.1.19以上*/
    char szPath[MAX_PATH];


    //删除指定目录下所有文件
    ACE_DIR *pdir = ACE_OS::opendir(szWorkDir);
    dirent *pdirent;

    //李杜芳 修改 12.17
    if (pdir==NULL)
        return ERR_FAIL;
    //修改完毕

    //by ldf 2003-09-29 对应问题单SWPD01863
    char szWorkDirTmp[MAX_PATH];
    strcpy(szWorkDirTmp,szWorkDir);

    //李杜芳 修改 12.14
    if (szWorkDir[strlen(szWorkDirTmp) - 1] != '/')
    {
        strcat(szWorkDirTmp, "/");
    }

    //修改完毕

    while (pdirent = ACE_OS::readdir(pdir))
    {
        sprintf(szPath,
                "%s%s",
                szWorkDirTmp,
                pdirent->d_name);
        remove(szPath);
    }

    ACE_OS::closedir(pdir);

    //删除指定目录
    return rmdir(szWorkDirTmp);
}


//by ldf 2002.6.7 对应问题单D14393
IGWB_Svc_Export const char* GetStringFromID(INT4 nID)
{
   return(CPersistResource::Instance()->GetStringFromID(nID));
}
//修改完毕

IGWB_Svc_Export void divide_file_path_name(const char* szSrcFileName,
                                          STRING& strPath,
                                          STRING& strFile)
{
    char szFileName[MAX_PATH] = "";
    char szFile[MAX_PATH] = "";

    strncpy(szFileName, szSrcFileName, sizeof(szFileName));
    szFileName[sizeof(szFileName) - 1] = '\0';

    strPath = "";
    strFile = "";
    char* p1 = strrchr(szFileName, '/');
#ifdef _PLATFORM_WIN32
    char* p2 = strrchr(szFileName, '\\');
#else //_PLATFORM_WIN32
    char* p2 = NULL;
#endif //_PLATFORM_WIN32
    char* pszDivide = max(p1, p2);
    if(pszDivide != NULL)
    {
        *pszDivide = '\0';
        strPath = szFileName;
        strFile = pszDivide + 1;

        return; //added by chenlianwei
    }

    strFile = szFileName;
}

#ifdef _PLATFORM_WIN32
extern "C" {
typedef HWND (*GETCONSOLEWINDOWS)(void);
}
/************************************************************************
 *函数功能：获取本进程的控制台窗口句柄
 *输入参数：无
 *返 回 值：本进程的控制台窗口句柄
 ************************************************************************/
IGWB_Svc_Export HWND GetConsoleWindowHandle()
{
    static HWND hWnd = NULL;
    static ACE_Mutex mutex;

    mutex.acquire();

    if(hWnd != NULL)
    {
        mutex.release();
        return hWnd;
    }

    if((GetVersion() & 0xFF) >= 5)
    {
        GETCONSOLEWINDOWS f = NULL;
        HMODULE h = LoadLibrary("kernel32.dll");
        if(h != NULL)
        {
            f = (GETCONSOLEWINDOWS)GetProcAddress(h, "GetConsoleWindow");
            if(NULL != f)
            {
                hWnd = f();
            }
            FreeLibrary(h);
        }
    }

    if(NULL == hWnd)
    {
        char szTitle[MAX_PATH];
        szTitle[0] = '\0';

        DWORD nRetLen = GetConsoleTitle(szTitle, sizeof(szTitle));

        if(nRetLen && (szTitle[0] != '\0'))
        {
            hWnd = FindWindow(NULL, szTitle);
        }
    }

    mutex.release();

    return hWnd;
}
#endif

/**********************************************************************
功能描述    从系统中获取指定目录下面的所有话单文件名称
输入参数    szPath  : 搜索目录
输出参数    qResult : 按照修改时间的大小进行过排序后的文件列表
返回值      返回话单文件个数
抛出异常    无
**********************************************************************/
IGWB_Svc_Export int GetFileList(const char* szPath, LIST<STRING>& qResult)
{
    LIST<SFileNameNode> FileInfoList;
    SFileNameNode TmpNode;

    int nCount = 0;
    ACE_Dirent FindDir((const ACE_TCHAR* )szPath);

    //搜索指定的目录，找出目录下所有的文件，并加入到qResult
    dirent* pDirent;
    ACE_stat FileInfo;
    char szFileFullPath[MAX_PATH], szFileName[MAX_PATH];
    pDirent = FindDir.read();
    while(pDirent)
    {
        strcpy(szFileName, pDirent->d_name);
        sprintf(szFileFullPath, "%s/%s", szPath, szFileName);
        ACE_OS::stat(szFileFullPath, &FileInfo);

        //如果找到的是一个文件，则符合要求
        if (S_IFDIR != (FileInfo.st_mode & S_IFMT))
        {
            strcpy(TmpNode.szFileName, szFileName);
            TmpNode.tFileTime = FileInfo.st_mtime;
            FileInfoList.push_back(TmpNode);
            nCount++;
        }

        pDirent = FindDir.read();
    }

    //按照修改时间对FileInfoList进行排序，然后据此得到排序后的话单文件名列表
    FileInfoList.sort();
    LIST<SFileNameNode>::iterator i;
    for (i = FileInfoList.begin(); i != FileInfoList.end(); i++)
    {
        qResult.push_back((*i).szFileName);
    }

    return nCount;
}


/************************************************************************
 函数名称：BCDtoString
 函数功能：将BCD码串转换为带尾0的字符串
 输入参数：
 const BYTE* pSrcBCD  : 待转换的BCD码串
 char* szDstStr       : 转换后的字符串
 UINT4 uSrcLen        : 待转换的BCD码串的长度
 UINT4 uDstLen        : 转换后的字符串的长度，缺省为0，表示不限制
 返 回 值：转换后字符串的长度，不含尾0
 //Added by ZhengYuqun 2003-08-27 SWPD01198
*************************************************************************/
IGWB_Svc_Export UINT4 BCDtoString(const BYTE* pSrcBCD,
                                  char* szDstStr,
                                  UINT4 uSrcLen,
                                  UINT4 uDstLen)
{
    BYTE yCurBcd;
    char cCurChar;
    UINT4 i = 0, j = 0;

    assert(NULL != pSrcBCD && NULL != szDstStr);

    for(; i < uSrcLen; i++)
    {
        yCurBcd = *(pSrcBCD + i);

        if(yCurBcd < 0xA0)
        {
            cCurChar = (yCurBcd >> 4) + '0';
        }
        else
        {
            cCurChar = (yCurBcd >> 4) - 10 + 'A';
        }
        if(0 == uDstLen || (0 != uDstLen && j + 1 <= uDstLen)) //如果对目标串有长度限制
        {
            szDstStr[j++] = cCurChar;
        }
        else
        {
            break;
        }

        yCurBcd &= 0x0F;

        if(yCurBcd < 0x0A)
        {
            cCurChar = yCurBcd + '0';
        }
        else
        {
            cCurChar = yCurBcd - 10 + 'A';
        }
        if(0 == uDstLen || (0 != uDstLen && j + 1 <= uDstLen)) //如果对目标串有长度限制
        {
            szDstStr[j++] = cCurChar;
        }
        else
        {
            break;
        }

    }
    szDstStr[j] = '\0';

    return j;
}

/************************************************************************
函数名称：RBCDtoString
函数功能：将RBCD码串转换为带尾0的字符串
输入参数：
const BYTE* pSrcBCD  : 待转换的RBCD码串
char* szDstStr       : 转换后的字符串
UINT4 uSrcLen        : 待转换的RBCD码串的长度
UINT4 uDstLen        : 转换后的字符串的长度，缺省为0，表示不限制
返 回 值：转换后字符串的长度，不含尾0
//Added by Lucy 2004-05-28 OPT version
*************************************************************************/
IGWB_Svc_Export UINT4 RBCDtoString(const BYTE* pSrcRBCD,
                                   char* szDstStr,
                                   UINT4 uSrcLen,
                                   UINT4 uDstLen)
{
    int    nIndex     = 0;      //文本格式的RBCD码字符串索引
    int    nRBcdIndex = 0;      //RBCD码数组索引
    BYTE   yCurRBcd   = 0;      //当前RBCD码值
    BYTE   yTmpRBcd   = 0;      //临时RBCD码值
    
    assert(NULL != pSrcRBCD && NULL != szDstStr);
    
    //顺序扫描RBCD码数组的每一个元素
    while(nRBcdIndex < uSrcLen)
    {
        //获取当前RBCD码的值
        yCurRBcd = pSrcRBCD[nRBcdIndex++];
        
        //屏蔽当前RBCD码数组值的高四位
        yTmpRBcd = yCurRBcd & 0x0F;
        
        //如果低四位是正常的RBCD码值，即小于16
        if(yTmpRBcd < 0x0F)
        {
            //如果当前RBCD码值小于10
            if(yTmpRBcd < 0x0A)
            {
                szDstStr[nIndex++] = yTmpRBcd + '0';
            }
            
            //如果当前RBCD码值大于10
            else
            {
                szDstStr[nIndex++] = yTmpRBcd - 10 + 'A';
            }
        }
        
        //到了RBCD码数组结束位置
        else
        {
            break;
        }
        
        //如果高四位是正常的RBCD码值，即小于16
        if(yCurRBcd < 0xF0)
        {
            //如果当前RBCD码值小于10
            if(yCurRBcd < 0xA0)
            {
                szDstStr[nIndex++] = (yCurRBcd >> 4) + '0';
            }
            
            //如果当前RBCD码值大于10
            else
            {
                szDstStr[nIndex++] = (yCurRBcd >> 4) - 10 + 'A';
            }
        }
        
        //到了RBCD码数组结束位置
        else
        {
            break;
        }
    }
    
    //设置字符串的结束符
    szDstStr[nIndex] = '\0';
    
    return	nIndex;
}

/************************************************************************
 函数名称：StringtoBCD
 函数功能：将带尾0的字符串转换为BCD码串
 输入参数：
 const char* szSrcStr   : 待转换的字符串，如果不带尾0，必须指定puSrcLen参数
 BYTE* pDstBCD          : 待转换的BCD码串
 UINT4 uDstLen          : 转换后的BCD码串的长度，缺省为0，表示不限制
 UINT4 uSrcLen          : 待转换字符串的长度，缺省为0
 返 回 值：BCD码串缓冲区被写过的字节数
 //Added by ZhengYuqun 2003-08-27 SWPD01198
*************************************************************************/
IGWB_Svc_Export UINT4 StringtoBCD(const char* szSrcStr,
                                  BYTE* pDstBCD,
                                  UINT4 uDstLen,
                                  UINT4 uSrcLen)
{
    int i = 0, j = 0;
    char cCurChar;
    UINT4 uSrcLength;

    assert(NULL != szSrcStr && NULL != pDstBCD);

    uSrcLength = strlen(szSrcStr);
    if(0 != uSrcLen)
    {
        uSrcLength = min(uSrcLength, uSrcLen);
    }

    if(0 != uDstLen)
    {
        memset(pDstBCD, 0xFF, uDstLen); //先将结果初始化为0xFF
    }

    for(; i < uSrcLength; i++)
    {
        if(0 != uDstLen && j == uDstLen)
        {
            break;
        }

        if (szSrcStr[i] >= 'a' && szSrcStr[i] <= 'f')  //将小写的a到f转换成大写的
        {
            cCurChar = szSrcStr[i] + ('A' - 'a');
        }
        else
        {
            cCurChar = szSrcStr[i];
        }

        assert( (cCurChar >= '0' && cCurChar <= '9') || (cCurChar >= 'A' && cCurChar <= 'F'));

        BYTE yCurVal = 0;
        if(cCurChar >= '0' && cCurChar <= '9')
        {
            yCurVal = cCurChar - '0';
        }
        else if(cCurChar >= 'A' && cCurChar <= 'F')
        {
            yCurVal = cCurChar - 'A' + 0x0A;
        }

        if(i % 2 == 0)
        {
            ((BYTE*)pDstBCD)[j] = yCurVal << 4;
            ((BYTE*)pDstBCD)[j] |= 0x0F;
        }
        else
        {
            ((BYTE*)pDstBCD)[j] &= yCurVal | 0xF0;
            j++;
        }
    }

    if(i % 2 != 0)
    {
        j++;
    }
    if(0 != uDstLen)
    {
        j = uDstLen;
    }

    return j;
}

IGWB_Svc_Export int BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen)
{
    int i = 0;
    int nActLen = 0;

    //计算真实的长度
    for (; i < nLen; i++)
    {
        if ((pBCD[i] & 0x0F) == 0x0F || (pBCD[i] & 0xF0) == 0xF0)
        {
            break;
        }
    }

    //考虑最后一个字节为 0xFF 的情况
    if ((pBCD[i] & 0xF0) == 0xF0)
    {
        nActLen = i;
    }
    else
    {
        nActLen = i + 1;
    }

    //Added by ZhengYuqun SWPD04257 2004-03-25
    if(nActLen > nLen)
    {
        nActLen = nLen;
    }
    //End    

    for (i = 0; i < nActLen; i++)
    {
        pRBCD[i] = pBCD[i] << 4 | pBCD[i] >> 4;
    }

    return nActLen;
}

/**********************************************************************
功能描述    从time_t转换成yyyy-mm-dd hh:mm:ss格式
输入参数    timeSource  : time_t值
输出参数    无
返回值      指定格式的日期串
抛出异常    无
**********************************************************************/
IGWB_Svc_Export STRING ConvertTimeToString(time_t timeSource)
{
    struct tm *newtime;
    newtime = localtime(&timeSource);
    char szTemp[20];
    sprintf(szTemp,
            "%04d-%02d-%02d %02d:%02d:%02d",
            newtime->tm_year + 1900,
            newtime->tm_mon + 1,
            newtime->tm_mday,
            newtime->tm_hour,
            newtime->tm_min,
            newtime->tm_sec);
    return STRING(szTemp);
}

/**********************************************************************
功能描述    从yyyy-mm-dd hh:mm:ss格式转换成time_t
输入参数    pszSource  : yyyy-mm-dd hh:mm:ss
输出参数    无
返回值      指定日期串对应的time_t值
抛出异常    无
**********************************************************************/
IGWB_Svc_Export time_t ConvertTimeToTimeT(const char* pszSource)
{
    if (pszSource == NULL)
    {
        return 0;
    }
    struct tm when;
    sscanf(pszSource,
           "%04d-%02d-%02d %02d:%02d:%02d",
           &when.tm_year,
           &when.tm_mon,
           &when.tm_mday,
           &when.tm_hour,
           &when.tm_min,
           &when.tm_sec);

    when.tm_year -= 1900;
    when.tm_mon -= 1;

    return mktime(&when);
}

/**********************************************************************
功能描述    从time_t转换成yyyy-mm-dd hh:mm:ss格式
输入参数    timeSource  : time_t值
输出参数    无
返回值      指定格式的日期串
抛出异常    无
**********************************************************************/
IGWB_Svc_Export STRING ConvertTimeToString_YMD(time_t timeSource)
{
    struct tm *newtime;
    newtime = localtime(&timeSource);
    char szTemp[20];
    sprintf(szTemp,
            "%04d-%02d-%02d",
            newtime->tm_year + 1900,
            newtime->tm_mon + 1,
            newtime->tm_mday);
    return STRING(szTemp);
}

/**********************************************************************
功能描述    从yyyy-mm-dd格式转换成time_t
输入参数    pszSource  : yyyy-mm-dd
输出参数    无
返回值      指定日期串对应的time_t值
抛出异常    无
**********************************************************************/
IGWB_Svc_Export time_t ConvertTimeToTimeT_YMD(const char* pszSource)
{
    if (pszSource == NULL)
    {
        return 0;
    }
    struct tm when;
    sscanf(pszSource, "%04d-%02d-%02d", &when.tm_year, &when.tm_mon, &when.tm_mday);
    when.tm_hour = 0;
    when.tm_min = 0;
    when.tm_sec = 0;

    when.tm_year -= 1900;
    when.tm_mon -= 1;

    return mktime(&when);
}

/**********************************************************************
功能描述    从系统中获取指定目录下面的所有日期目录
输入参数    szPath----消息头
输出参数    qResult----所有的日期目录的列表，按照日期目录的大小进行过排序
返回值      返回日期目录个数
抛出异常    无
**********************************************************************/
IGWB_Svc_Export int GetDateDir(const char* szPath, LIST<STRING>& qResult)
{
    int nCount = 0;
    ACE_Dirent FindDir((const ACE_TCHAR* )szPath);

    //搜索指定的目录，找出目录下除了.和..外的所有目录名，并加入到qResult
    dirent* pDirent;
    ACE_stat FileInfo;
    char szFileFullPath[MAX_PATH], szFileName[MAX_PATH];
    pDirent = FindDir.read();
    while(pDirent)
    {
        strcpy(szFileName, pDirent->d_name);
        sprintf(szFileFullPath, "%s/%s", szPath, szFileName);
        ACE_OS::stat(szFileFullPath, &FileInfo);
        if (S_IFDIR == (FileInfo.st_mode & S_IFMT))    //找到的是一个目录
        {
            //必须确保目录不是.和..，且为8位长的数字，这样的目录才符合要求
            if ((0 != strcmp(szFileName, "."))
                && (0 != strcmp(szFileName, ".."))
                && (8 == strlen(szFileName))
                && IsDigitValue(szFileName))
            {
                qResult.push_back(szFileName);
                nCount++;
            }
        }

        pDirent = FindDir.read();
    }

    //要对日期按照大小进行排序，以便确保在话单台显示正确
    qResult.sort();

    return nCount;
}
