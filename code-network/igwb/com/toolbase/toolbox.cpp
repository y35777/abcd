#include "../include/precom.h"
#include "../include/export_dll.h"
#include "../include/toolbox.h"
#include <errno.h>

/* CTrace��Ķ���ͷ�ļ� */
#include "trace.h"

/* CPersistErr��Ķ���ͷ�ļ� */
#include "persist_err.h"

/* Trace���� */
CTrace g_TraceFile;

/* ��Ŀ¼·����,��:"c:\\billsvr"��"."��"" */
IGWB_Svc_Export char g_szAppPath[MAX_PATH] = "";

/* �����ļ�����·����,��:"c:\\billsvr\\ini\\billsvr.ini" */
IGWB_Svc_Export char g_szCfgPathName[MAX_PATH] = "";

/* ����������·�����ͺ�׺ */
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

//by ldf ��Ӧ���ⵥD20379 2003-02-21
/***********************************************
*��������  ���nFill��szFillChar.
*�������
* szFillString �����Դ�ַ���
* nFill -Ҫ������ַ���
* szFillChar Ҫ������ַ�
* alignment ���뷽ʽ����뻹���Ҽ��룬��ѡΪ ALIGN_LEFT �� ALIGN_RIGHT
*��ŷ� 2003-02-21
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
 *�������ܣ����ñ�׼C����ʵ���ļ���������
 *���룺
 *     pszSrcFile��Դ�ļ�����ȫ·����
 *     pszDstFile��Ŀ���ļ�����ȫ·����
 *�����
 *     ��
 *���أ�
 *     ����ִ�гɹ�����TRUE�����򷵻�FALSE.
 *****************************************************************************/
IGWB_Svc_Export BOOL FileCopy(const char* pszSrcFile, const char* pszDstFile)
{
    static const int FILE_BUFFER_SIZE = 4096;
    char szBuf[FILE_BUFFER_SIZE];
    int nFileLen;
    int nCopiedBytes = 0;
    int nCopyNeed = 0;

    //��ԭʼ�ļ�
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

    //��Ŀ���ļ�
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
 *�������ܣ����ñ�׼C����ʵ���ļ����������������չ��һ�����ܣ�
            ���Ŀ���ļ����ڣ����Դ�ļ�׷�ӵ�Ŀ���ļ�����
 *���룺
 *     pszSrcFile��Դ�ļ�����ȫ·����
 *     pszDstFile��Ŀ���ļ�����ȫ·����
 *�����
 *     ��
 *���أ�
 *     ����ִ�гɹ�����TRUE�����򷵻�FALSE. 
 *****************************************************************************/
IGWB_Svc_Export BOOL FileCopyEx(const char* pszSrcFile, const char* pszDstFile)
{
    static const int FILE_BUFFER_SIZE = 4096;
    char szBuf[FILE_BUFFER_SIZE];
    int nFileLen;
    int nCopiedBytes = 0;
    int nCopyNeed = 0;

    //��ԭʼ�ļ�
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

    //��Ŀ���ļ�
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
 *�������ܣ��ƶ��ļ�
 *���룺
 *     pszSrcFile��Դ�ļ�����ȫ·����
 *     pszDstFile��Ŀ���ļ�����ȫ·����
 *�����
 *     ��
 *���أ�
 *     �ƶ�ִ�гɹ�����TRUE�����򷵻�FALSE.
 ****************************************************************************/
IGWB_Svc_Export BOOL FileMove(const char* pszSrcFile, const char* pszDstFile)
{
    //����ACE�ṩ���ļ���������ʵ���ļ��ƶ�
    int nRet = ACE_OS::rename(pszSrcFile, pszDstFile);

    if (nRet != 0)
    {
        return FALSE;
    }

    return TRUE;
}

/****************************************************************************
 *�������ܣ�ɾ���ļ�
 *���룺
 *     pszFile���ļ�����ȫ·����
 *�����
 *     ��
 *���أ�
 *     ɾ��ִ�гɹ�����TRUE�����򷵻�FALSE.
 ****************************************************************************/
IGWB_Svc_Export BOOL FileDel(const char* pszFile)
{
    int nRet = ACE_OS::unlink(pszFile);

    //��ɾ�����ļ�������
    if ((nRet == -1) && (errno != ENOENT))
    {
        return FALSE;
    }

    return TRUE;
}

/****************************************************************************
 *�������ܣ������༶(����һ��)Ŀ¼
 *���룺
 *     pszFile���ļ�����ȫ·����
 *�����
 *     ��
 *���أ�
 *     ɾ��ִ�гɹ�����TRUE�����򷵻�FALSE.
 ****************************************************************************/
IGWB_Svc_Export BOOL CreateDeepDir(const char* pszPath)
{
    //Ŀ¼���в����ڳ��ֵ��ַ�
    const char pInvalidChars[] =
         { '\\', '/', ':', '*', '?', '"', '<', '>', '|', '\0'};

    char szTempDir[MAX_PATH];

    strcpy(szTempDir, pszPath);
    int nLen = strlen(pszPath);
    if (0 == nLen)
    {
        return FALSE;
    }

    //������һ���ַ�����'\\'���߲��Ҳ���'/'�������ַ���β���
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
            //���Ŀ¼���ĺϷ���
            if (strchr(pInvalidChars, szTempDir[i-1]))
            {
                continue;
            }

            szTempDir[i] = 0;    //�����ַ���������

            //�ڴ����༶Ŀ¼ʱ�����Ŀ¼�Ѵ��ڣ�����Ϊ�Ǵ���
            //����������һ��Ŀ¼���������Ŀ¼ʧ�ܣ���ʧ��ԭ��
            //������Ŀ¼�Ѵ��ڣ�����ֹ��������������FALSE��
            //by ldf 2002.4.22 ��Ӧ���ⵥ D13063
            #ifdef _PLATFORM_WIN32
                if((ACE_OS::mkdir(szTempDir) == -1) && (GetLastError() != ERROR_ALREADY_EXISTS) )
            #else
                if((ACE_OS::mkdir(szTempDir) == -1) && (errno != EEXIST) )
            #endif
                {
                    return FALSE;
                }
            //�޸����

            szTempDir[i] = '/';  //�ָ��ַ���
        }
    }

    return TRUE;
}

/****************************************************************************
 *�������ܣ��ַ�����Сд�����бȽ�
 *���룺
 *     szStr1����Ҫ�Ƚϵ��ַ���1
 *     szStr2����Ҫ�Ƚϵ��ַ���2
 *�����
 *     �ַ����Ƿ����, 0�����ȣ�1�����
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
        return 0;       //���Ȳ����
    }

    while(*szTemp1 != '\0')
    {
        cChar1 = *szTemp1;
        cChar2 = *szTemp2;

        szTemp1++;
        szTemp2++;

        if (cChar1 >= 'a' && cChar1 <= 'z')
        {
            cChar1 -= cDiff;  //ת���ɴ�д
        }

        if (cChar2 >= 'a' && cChar2 <= 'z')
        {
            cChar2 -= cDiff;  //ת���ɴ�д
        }

        if (cChar1 != cChar2)
        {
            return 0;
        }
    }

    return 1;
}

/***************************************************************************
 *�������ܣ��ַ�����Сд���бȽ�
 *���룺
 *     szS1����Ҫ�Ƚϵ��ַ���1
 *     szS2����Ҫ�Ƚϵ��ַ���2
 *�����
 *     �ַ����Ƿ����, 0�����ȣ�1�����
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
 *�������ܣ���һ���ַ���ȫ��ת���ɴ�д
 *���룺
 *     szStr����ת�����ַ���
 *�����
 *     szStr��ת����ɵ��ַ���
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
 *�������ܣ�ȥ���ַ�����ߵĿո�
 *���룺
 *     szStr����������ַ���
 *�����
 *     szStr��������ɵ��ַ���
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
    //�޸Ľ���by lx
}

/************************************************************************
 *�������ܣ�ȥ���ַ����ұߵĿո�
 *���룺
 *     szStr����������ַ���
 *�����
 *     szStr��������ɵ��ַ���
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
 *�������ܣ�ȥ���ַ������ҵĿո�
 *���룺
 *     szStr����������ַ���
 *�����
 *     szStr��������ɵ��ַ���
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

    //�ַ���Ϊ��
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
 *�������ܣ�������UINT2�ϳ�һ��UINT4
 *���룺
 *     nLowWord����2�ֽ�
 *     nHighWord����2�ֽ�
 *�����
 *     ��nLowWord��nHighWord�ϳ�һ��UINT4
 ***********************************************************************/
IGWB_Svc_Export UINT4 MAKEUINT4(UINT2 nLowWord, UINT2 nHighWord)
{
    UINT4   nDWORD;

    nDWORD = (nHighWord << 16) + nLowWord;

    return nDWORD;
}

/*************************************************************************
 *�������ܣ�ȡ��UINT4�еĵ�2�ֽ�
 *���룺
 *     nDWORD��UINT4����
 *�����
 *     nDWORD�ĵ�2�ֽ�
 ************************************************************************/
IGWB_Svc_Export UINT2 LOWUINT2(UINT4 nDWORD)
{
    return (UINT2)(nDWORD & 0xFFFF);
}

/************************************************************************
 *�������ܣ�ȡ��UINT4�еĸ�2�ֽ�
 *���룺
 *     nDWORD��UINT4����
 *�����
 *     nDWORD�ĸ�2�ֽ�
 *************************************************************************/
IGWB_Svc_Export UINT2 HIGHUINT2(UINT4 nDWORD)
{
    return (UINT2)((nDWORD & 0xFFFF0000) >> 16);
}

/************************************************************************
 *�������ܣ��滻�ַ���������ַ���
 *���������
 *    zSrcStr�������ַ���
 *    szOldSubStr��Ҫ���滻�����ַ���
 *    szNewSubStr�������滻�����ַ���
 *���������
 *    szDstStr���滻�����ַ�����������ַ������ռ��ں�����������ͷ�
 ************************************************************************/
IGWB_Svc_Export void ReplaceSubStr(const char* szSrcStr, char* szDstStr,
                                  const char* szOldSubStr, const char* szNewSubStr)
{
    int nStrLen = strlen(szSrcStr);
    int nOldSubStrLen = strlen(szOldSubStr);
    int nNewSubStrLen = strlen(szNewSubStr);
    int nReadPos = 0, nWritePos = 0;

    //���ַ�����˳���������е��ַ���szOldSubStr����szNewSubStr���濽����szDstStr��,
    //�����������ַ�������ֱ�ӿ�����szDstStr��
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
 *�������ܣ��õ�һ��ָ���ļ��ĳ���
 *���������
 *    pFile��ָ�����ļ�����
 *����ֵ��
 *    ָ���ļ��ĳ���
 ************************************************************************/
IGWB_Svc_Export long FileSize(FILE* pFile)
{
    long lOldPos = ftell(pFile);

    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    fseek(pFile, lOldPos, SEEK_SET);

    return lSize;
}

//�õ�һ��ָ���ļ��ĳ���
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
 *�������ܣ�ɾ��ָ��Ŀ¼�����е��ļ�
 *���������
 *    szWorkDir��ָ����Ŀ¼·��
 *����ֵ��
 *    ɾ��Ŀ¼�Ƿ�ɹ�
 ************************************************************************/
IGWB_Svc_Export int SweepDirectory(char* szWorkDir)
{
    /*Ҫ��ACE�汾Ϊ4.1.19����*/
    char szPath[MAX_PATH];


    //ɾ��ָ��Ŀ¼�������ļ�
    ACE_DIR *pdir = ACE_OS::opendir(szWorkDir);
    dirent *pdirent;

    //��ŷ� �޸� 12.17
    if (pdir==NULL)
        return ERR_FAIL;
    //�޸����

    //by ldf 2003-09-29 ��Ӧ���ⵥSWPD01863
    char szWorkDirTmp[MAX_PATH];
    strcpy(szWorkDirTmp,szWorkDir);

    //��ŷ� �޸� 12.14
    if (szWorkDir[strlen(szWorkDirTmp) - 1] != '/')
    {
        strcat(szWorkDirTmp, "/");
    }

    //�޸����

    while (pdirent = ACE_OS::readdir(pdir))
    {
        sprintf(szPath,
                "%s%s",
                szWorkDirTmp,
                pdirent->d_name);
        remove(szPath);
    }

    ACE_OS::closedir(pdir);

    //ɾ��ָ��Ŀ¼
    return rmdir(szWorkDirTmp);
}


//by ldf 2002.6.7 ��Ӧ���ⵥD14393
IGWB_Svc_Export const char* GetStringFromID(INT4 nID)
{
   return(CPersistResource::Instance()->GetStringFromID(nID));
}
//�޸����

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
 *�������ܣ���ȡ�����̵Ŀ���̨���ھ��
 *�����������
 *�� �� ֵ�������̵Ŀ���̨���ھ��
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
��������    ��ϵͳ�л�ȡָ��Ŀ¼��������л����ļ�����
�������    szPath  : ����Ŀ¼
�������    qResult : �����޸�ʱ��Ĵ�С���й��������ļ��б�
����ֵ      ���ػ����ļ�����
�׳��쳣    ��
**********************************************************************/
IGWB_Svc_Export int GetFileList(const char* szPath, LIST<STRING>& qResult)
{
    LIST<SFileNameNode> FileInfoList;
    SFileNameNode TmpNode;

    int nCount = 0;
    ACE_Dirent FindDir((const ACE_TCHAR* )szPath);

    //����ָ����Ŀ¼���ҳ�Ŀ¼�����е��ļ��������뵽qResult
    dirent* pDirent;
    ACE_stat FileInfo;
    char szFileFullPath[MAX_PATH], szFileName[MAX_PATH];
    pDirent = FindDir.read();
    while(pDirent)
    {
        strcpy(szFileName, pDirent->d_name);
        sprintf(szFileFullPath, "%s/%s", szPath, szFileName);
        ACE_OS::stat(szFileFullPath, &FileInfo);

        //����ҵ�����һ���ļ��������Ҫ��
        if (S_IFDIR != (FileInfo.st_mode & S_IFMT))
        {
            strcpy(TmpNode.szFileName, szFileName);
            TmpNode.tFileTime = FileInfo.st_mtime;
            FileInfoList.push_back(TmpNode);
            nCount++;
        }

        pDirent = FindDir.read();
    }

    //�����޸�ʱ���FileInfoList��������Ȼ��ݴ˵õ������Ļ����ļ����б�
    FileInfoList.sort();
    LIST<SFileNameNode>::iterator i;
    for (i = FileInfoList.begin(); i != FileInfoList.end(); i++)
    {
        qResult.push_back((*i).szFileName);
    }

    return nCount;
}


/************************************************************************
 �������ƣ�BCDtoString
 �������ܣ���BCD�봮ת��Ϊ��β0���ַ���
 ���������
 const BYTE* pSrcBCD  : ��ת����BCD�봮
 char* szDstStr       : ת������ַ���
 UINT4 uSrcLen        : ��ת����BCD�봮�ĳ���
 UINT4 uDstLen        : ת������ַ����ĳ��ȣ�ȱʡΪ0����ʾ������
 �� �� ֵ��ת�����ַ����ĳ��ȣ�����β0
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
        if(0 == uDstLen || (0 != uDstLen && j + 1 <= uDstLen)) //�����Ŀ�괮�г�������
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
        if(0 == uDstLen || (0 != uDstLen && j + 1 <= uDstLen)) //�����Ŀ�괮�г�������
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
�������ƣ�RBCDtoString
�������ܣ���RBCD�봮ת��Ϊ��β0���ַ���
���������
const BYTE* pSrcBCD  : ��ת����RBCD�봮
char* szDstStr       : ת������ַ���
UINT4 uSrcLen        : ��ת����RBCD�봮�ĳ���
UINT4 uDstLen        : ת������ַ����ĳ��ȣ�ȱʡΪ0����ʾ������
�� �� ֵ��ת�����ַ����ĳ��ȣ�����β0
//Added by Lucy 2004-05-28 OPT version
*************************************************************************/
IGWB_Svc_Export UINT4 RBCDtoString(const BYTE* pSrcRBCD,
                                   char* szDstStr,
                                   UINT4 uSrcLen,
                                   UINT4 uDstLen)
{
    int    nIndex     = 0;      //�ı���ʽ��RBCD���ַ�������
    int    nRBcdIndex = 0;      //RBCD����������
    BYTE   yCurRBcd   = 0;      //��ǰRBCD��ֵ
    BYTE   yTmpRBcd   = 0;      //��ʱRBCD��ֵ
    
    assert(NULL != pSrcRBCD && NULL != szDstStr);
    
    //˳��ɨ��RBCD�������ÿһ��Ԫ��
    while(nRBcdIndex < uSrcLen)
    {
        //��ȡ��ǰRBCD���ֵ
        yCurRBcd = pSrcRBCD[nRBcdIndex++];
        
        //���ε�ǰRBCD������ֵ�ĸ���λ
        yTmpRBcd = yCurRBcd & 0x0F;
        
        //�������λ��������RBCD��ֵ����С��16
        if(yTmpRBcd < 0x0F)
        {
            //�����ǰRBCD��ֵС��10
            if(yTmpRBcd < 0x0A)
            {
                szDstStr[nIndex++] = yTmpRBcd + '0';
            }
            
            //�����ǰRBCD��ֵ����10
            else
            {
                szDstStr[nIndex++] = yTmpRBcd - 10 + 'A';
            }
        }
        
        //����RBCD���������λ��
        else
        {
            break;
        }
        
        //�������λ��������RBCD��ֵ����С��16
        if(yCurRBcd < 0xF0)
        {
            //�����ǰRBCD��ֵС��10
            if(yCurRBcd < 0xA0)
            {
                szDstStr[nIndex++] = (yCurRBcd >> 4) + '0';
            }
            
            //�����ǰRBCD��ֵ����10
            else
            {
                szDstStr[nIndex++] = (yCurRBcd >> 4) - 10 + 'A';
            }
        }
        
        //����RBCD���������λ��
        else
        {
            break;
        }
    }
    
    //�����ַ����Ľ�����
    szDstStr[nIndex] = '\0';
    
    return	nIndex;
}

/************************************************************************
 �������ƣ�StringtoBCD
 �������ܣ�����β0���ַ���ת��ΪBCD�봮
 ���������
 const char* szSrcStr   : ��ת�����ַ������������β0������ָ��puSrcLen����
 BYTE* pDstBCD          : ��ת����BCD�봮
 UINT4 uDstLen          : ת�����BCD�봮�ĳ��ȣ�ȱʡΪ0����ʾ������
 UINT4 uSrcLen          : ��ת���ַ����ĳ��ȣ�ȱʡΪ0
 �� �� ֵ��BCD�봮��������д�����ֽ���
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
        memset(pDstBCD, 0xFF, uDstLen); //�Ƚ������ʼ��Ϊ0xFF
    }

    for(; i < uSrcLength; i++)
    {
        if(0 != uDstLen && j == uDstLen)
        {
            break;
        }

        if (szSrcStr[i] >= 'a' && szSrcStr[i] <= 'f')  //��Сд��a��fת���ɴ�д��
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

    //������ʵ�ĳ���
    for (; i < nLen; i++)
    {
        if ((pBCD[i] & 0x0F) == 0x0F || (pBCD[i] & 0xF0) == 0xF0)
        {
            break;
        }
    }

    //�������һ���ֽ�Ϊ 0xFF �����
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
��������    ��time_tת����yyyy-mm-dd hh:mm:ss��ʽ
�������    timeSource  : time_tֵ
�������    ��
����ֵ      ָ����ʽ�����ڴ�
�׳��쳣    ��
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
��������    ��yyyy-mm-dd hh:mm:ss��ʽת����time_t
�������    pszSource  : yyyy-mm-dd hh:mm:ss
�������    ��
����ֵ      ָ�����ڴ���Ӧ��time_tֵ
�׳��쳣    ��
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
��������    ��time_tת����yyyy-mm-dd hh:mm:ss��ʽ
�������    timeSource  : time_tֵ
�������    ��
����ֵ      ָ����ʽ�����ڴ�
�׳��쳣    ��
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
��������    ��yyyy-mm-dd��ʽת����time_t
�������    pszSource  : yyyy-mm-dd
�������    ��
����ֵ      ָ�����ڴ���Ӧ��time_tֵ
�׳��쳣    ��
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
��������    ��ϵͳ�л�ȡָ��Ŀ¼�������������Ŀ¼
�������    szPath----��Ϣͷ
�������    qResult----���е�����Ŀ¼���б���������Ŀ¼�Ĵ�С���й�����
����ֵ      ��������Ŀ¼����
�׳��쳣    ��
**********************************************************************/
IGWB_Svc_Export int GetDateDir(const char* szPath, LIST<STRING>& qResult)
{
    int nCount = 0;
    ACE_Dirent FindDir((const ACE_TCHAR* )szPath);

    //����ָ����Ŀ¼���ҳ�Ŀ¼�³���.��..�������Ŀ¼���������뵽qResult
    dirent* pDirent;
    ACE_stat FileInfo;
    char szFileFullPath[MAX_PATH], szFileName[MAX_PATH];
    pDirent = FindDir.read();
    while(pDirent)
    {
        strcpy(szFileName, pDirent->d_name);
        sprintf(szFileFullPath, "%s/%s", szPath, szFileName);
        ACE_OS::stat(szFileFullPath, &FileInfo);
        if (S_IFDIR == (FileInfo.st_mode & S_IFMT))    //�ҵ�����һ��Ŀ¼
        {
            //����ȷ��Ŀ¼����.��..����Ϊ8λ�������֣�������Ŀ¼�ŷ���Ҫ��
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

    //Ҫ�����ڰ��մ�С���������Ա�ȷ���ڻ���̨��ʾ��ȷ
    qResult.sort();

    return nCount;
}
