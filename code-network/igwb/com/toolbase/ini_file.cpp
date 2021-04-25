#include "../include/export_dll.h"
#include "../include/toolbox.h"

CINIFile::CINIFile(const char *pszFileName)
{
    strncpy(m_szFileName, pszFileName, MAX_PATH);
    m_szFileName[MAX_PATH - 1] = '\0';

    StringLRTrim(m_szFileName);

    m_pFile     = NULL;
    m_lFileSize = 0;
}

CINIFile::~CINIFile()
{
    Close();
}

BOOL CINIFile::IsOpen()
{
    return (m_pFile != NULL);
}

/***************************************************************************
 *功能描述：打开INI文件m_szFileName，将文件指针存入m_pFile
 *输入：
 *      无
 *输出：
 *      无。
 *返回：
 *              无
 ***************************************************************************/
BOOL CINIFile::Open()
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }

    if (strlen(m_szFileName) == 0)
    {
        return FALSE;
    }

    m_pFile = fopen(m_szFileName, "rb+");
    if(m_pFile)
    {
        fseek(m_pFile,0,SEEK_END);
        m_lFileSize = ftell(m_pFile);
    }
    return (m_pFile != NULL);
}


/*********************************************************************************
 *功能描述：关闭INI文件“m_szFileName”
 *输入：
 *      无
 *输出：
 *      无。
 *返回：
 *              无
 *********************************************************************************/
void CINIFile::Close()
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

/*****************************************************************************
 *功能描述：从一个键值所在的行中得到键名，例如从“KeyName=NameValue”中得到"KeyName"
 *输入：
 *      szLine：键值所在的行，例如“KeyName=NameValue”
 *输出：
 *      szKey：找到的键名KeyName
 *返回：
 *              无
 ****************************************************************************/
void CINIFile::GetKeyName(const char *szLine, char *szKey)
{
    int i = 0;

    sprintf(szKey, "%s", "");
    if (strlen(szLine) == 0)            return; //字符串为空，返回
    if (strchr(szLine, '=') == NULL)    return; //字符串中根本不包含“=”，返回

    while (szLine[i] != '=')
    {
         szKey[i] = szLine[i];
         i++;
    }

    szKey[i] = '\0';
    StringLRTrim(szKey);        //移去左右的空格
}

/************************************************************************
 *功能描述：m_pFile指针指向的文件（INI）中读取长度为nLen字节的数据，放入
            pszBuf
 *输入：
 *      pszBuf：用于存放读结果的缓冲区
 *      nLen：  指定本次读的字节数
 *      m_pFile：    文件指针
 *
 * 输出：
 *      pszTmp：读结果指针
 ************************************************************************/
char* CINIFile::GetLine(char *szOut, int nLen)
{
    char *szTmp = NULL;

    if (m_pFile == NULL)        return NULL;

    clearerr(m_pFile);
    szTmp = fgets(szOut, nLen, m_pFile);

    if (ferror(m_pFile))
    {
        return NULL;
    }

    return szTmp;
}

int CINIFile::GetLineType(char *szLine, int nLen)
{
    char    *szTmp;
    int     nStrLen;

    szTmp = GetLine(szLine, nLen);

    if (szTmp == NULL)
    {
        if (feof(m_pFile))              return DATA_EOF;
        else if (ferror(m_pFile))       return DATA_ERR;
    }

    nStrLen = strlen(szLine);
    if ((nStrLen == nLen - 1) && (szLine[nStrLen - 1] != '\n'))
    {
        return DATA_OVERFLOW;
    }

    //by ldf 2002.4.26 对应问题单D13202
    StringLRTrim(szLine);
    //修改完毕

    nStrLen = strlen(szLine);

    if (nStrLen >= 2)   //如果字符串头为“//”或者“;”，则表示是一个注释行
    {
        if (((szLine[0] == '/') && (szLine[1] == '/'))
          || (szLine[0] == ';'))
            return DATA_NOTE_LINE;

    }
    else if (nStrLen == 1)
    {
        if (szLine[0] == ';')   return DATA_NOTE_LINE;
    }

    char *pTmp1 = strchr(szLine, '[');
    char *pTmp2 = strchr(szLine, ']');

    if ((pTmp1 != NULL)  && (pTmp2 != NULL))
    {
        if (pTmp1 < pTmp2)
            return DATA_SECTION_LINE;   //有“[]”,这一行是段名
    }

    szTmp = strchr(szLine, '=');
    if (szTmp != NULL)
    {
        return DATA_KEY_LINE;   //有“＝”，这一行是键值
    }

    return DATA_FAULT_LINE;
}

/***************************************************************************
 *功能描述：从一个键值所在的行中得到键值，例如从“KeyName=NameValue”中得
            到"NameValue"
 *输入：
 *      szLine：键值所在的行，例如KeyName=NameValue
 *输出：
 *      szKey：找到的键名NameValue
 *返回：
 *      无
 ***************************************************************************/
int CINIFile::GetKeyValue(const char *szLine, char *szKey, int nBufLen)
{
    int  nKeyValueNum = 0;
    int  nFlag = 1;
    const char *szLineTmpPt;
    char *szKeyValueTmpPt = szKey;
    int  i = 0;

    *szKeyValueTmpPt = '\0';
    sprintf(szKey, "%s", "");

    szLineTmpPt = strchr(szLine, '=');
    if (szLineTmpPt == NULL)    //输入行中根本没有“=”
    {
        return nKeyValueNum;
    }

    szLineTmpPt++;      //移到=右边的一个字符处

    while ((*szLineTmpPt == ' ') || (*szLineTmpPt == '\t'))
    {
        szLineTmpPt++;  //空格不计算在内
    }

    if (*szLineTmpPt == '\0')
    {
        return nKeyValueNum;
    }
    else
    {
        nKeyValueNum = 1;
    }

    //得到键值
    while (*szLineTmpPt != '\0')
    {
        if (*szLineTmpPt == ';')        //如果是";"，则在该处插入一个空格
        {
            //两个连续的";"，则插入一个空格
            if (nFlag == 1)
            {
                *szKeyValueTmpPt = ' ';
                szKeyValueTmpPt++;
            }

            *szKeyValueTmpPt = *szLineTmpPt;
            nKeyValueNum++;
            nFlag = 1;
        }
        else
        {
            nFlag = 0;
            *szKeyValueTmpPt = *szLineTmpPt;
        }

        szKeyValueTmpPt++;
        szLineTmpPt++;

        //检查输出缓冲区是否足够
        i++;
        if (i == nBufLen)
        {
            *szKeyValueTmpPt = '\0';
            return DATA_OVERFLOW;
        }
    }

    //如果最后一个字符是“;”，则插入一个空格
    if (nFlag == 1)
    {
        *szKeyValueTmpPt = ' ';
        szKeyValueTmpPt++;
    }

    *szKeyValueTmpPt = '\0';

    return nKeyValueNum;
}

/***************************************************************************
 *功能描述：读指定段名和键名底键值
 *输入：
 *      szSection：段名，例如[SectionName]中的SectionName
 *      szKey    ：键名，例如KeyName=KeyValue中的KeyName
 *      nDefaullt：如果没有找到，则取默认值nDefault
 *输出：
 *      无
 *返回：
 *      得到的键值（整数）
 ***************************************************************************/
int CINIFile::GetInt(const char *szSection,
                     const char *szKey,
                     const int nDefault)
{
    char szReturn[MAX_LINE_LEN]  = "";
    char szDefault[MAX_LINE_LEN] = "";
    sprintf(szDefault, "%d", nDefault);

    GetString(szSection, szKey, szDefault, szReturn, MAX_LINE_LEN);

    return atol(szReturn);
}

/****************************************************************************
 *功能描述：读出指定段名szSection下的键名为szKey的键值（字符串）
 *输入：
 *      szSection ：段名，例如[SectionName]中的SectionName
 *      szKey     ：键名，例如KeyName=KeyValue中的KeyName
 *      szDefaullt：如果没有找到，则取默认值szDefault
 *输出：
 *      szOut     ：键值输出
 *      nLen      ：szOut的长度
 *返回：
 *      获得子符串长度。
 ******************************************************************************/
int CINIFile::GetString(const char *szSection,
                        const char *szKey,
                        const char *szDefault,
                        char *szOut,
                        int nLen)
{
    char  szBuf[MAX_LINE_LEN];
    char  szTmpBuf[MAX_LINE_LEN];
    int   nStringNum   = 0;
    int   nSectionFlag = 0;             //段名找到标志
    int   nRetValue;
    char  szTmpSection[MAX_LINE_LEN];
    char  szTmpKey[MAX_LINE_LEN];

    char  szError[200];

    if(szDefault != szOut)
    {
        memmove(szOut, szDefault, min(nLen, strlen(szDefault) + 1));   //先赋一个默认值szDefault
    }
    szOut[nLen - 1] = '\0';

    if (m_pFile == NULL)
    {
        return 0;      //文件没有打开
    }

    rewind(m_pFile);            //回转文件指针，将文件指向文件头

    //要查找一个键的值，则必须先找到该键所在的段名
    while (1)
    {
        nRetValue = GetLineType(szBuf, MAX_LINE_LEN);

        switch (nRetValue)
        {
            case DATA_EOF:
                if (ferror(m_pFile))
                {
                    sprintf(szError, "Read INI File Error!\r\n");
                }

                return nStringNum;

            case DATA_ERR:
                return 0;

            case DATA_OVERFLOW:
                return 0;

            case DATA_KEY_LINE:
                if (nSectionFlag == 1)  //已经找到了段名
                {
                    GetKeyName(szBuf, szTmpKey);
                    StringLRTrim(szTmpKey);
                    if (ACE_OS::strcasecmp(szKey, szTmpKey) == 0)    //大小写不敏感比较
                    {
                        nStringNum = GetKeyValue(szBuf, szTmpBuf, MAX_LINE_LEN);
                        char* pColon = strchr(szTmpBuf, ';');  //去掉注释
                        if (pColon != NULL)
                        {
                            szTmpBuf[pColon - szTmpBuf] = '\0';
                        }
                        //修改记录 李杜芳 这里添加了一行，以去掉无用的空格和\t，不然在其他模块得作相应的处理
                        StringLRTrim(szTmpBuf);
                        strncpy(szOut, szTmpBuf, nLen);
                        szOut[nLen - 1] = '\0';

                        return strlen(szOut);
                    }
                }
                break;

            case DATA_SECTION_LINE:
                if (nSectionFlag == 1)  //找到下一个段名上去了，返回
                {
                    return nStringNum;
                }

                GetSectionName(szBuf, szTmpSection);
                StringLRTrim(szTmpSection);
                if (ACE_OS::strcasecmp(szSection, szTmpSection) == 0) //大小写不敏感比较
                {
                    nSectionFlag = 1;           //找到段名
                }
                break;

            default:
                break;
        }
    }
}

/*****************************************************************************
 *功能描述：
 *输入：
 *      szInput   ：以;分隔的字符串
 *      uTotalNum ：szInput中的分隔符总数
 *      uValueNum ：希望搜索的字符串序号
 *输出：
 *      szKeyValue：查找到的字符串
 *返回：
 *      TRUE－查找成功；FALSE－查找失败
 ******************************************************************************/
BOOL CINIFile::GetStringByNo(char *szInput,
                             char *szKeyValue,
                             int uTotalNum,
                             int uValueNum)
{
    int i = 0;
    int j = 0;
    int k = 0;

    if ((uValueNum > uTotalNum) || (uValueNum < 1)
         || (NULL == szInput)           || (NULL == szKeyValue))
    {
        return FALSE;
    }

    do
    {
        if (j == uValueNum - 1)
        {
            do
            {
                szKeyValue[k] = szInput[i];
                i++;
                k++;

            }while ((szInput[i] != ';') && (szInput[i] != '\0'));

            szKeyValue[k] = '\0';

            return TRUE;
        }

        if ((szInput[i] == ';') || (szInput[i] == '\0'))
        {
            j++;
        }

        i++;
    }while (j <= uTotalNum - 1);

    return FALSE;
}

/*****************************************************************************
 *功能描述：从字符串中查找出INI段名，例如从[SectionName]中找出SectionName
 *输入：
 *      szLine   ： 一行字符串，例如[SectionName]
 *输出：
 *      szSection：查找到的段名，如果没有找到，则szSection输出为空
 *返回：
 *      无。
 ****************************************************************************/
void CINIFile::GetSectionName(const char *szLine, char *szSection)
{
    int   i = 0;
    const char  *szTmp;

    sprintf(szSection, "%s", "");
    szTmp = strchr(szLine, '[');
    if (szTmp == NULL)      return;

    szTmp++;    //移到'['的前一个字符的位置

    while(*szTmp != ']')
    {
        //已经到了字符串的尾，但还没有找到']'
        if (*szTmp == '\0')
        {
            i = 0;
            break;
        }

        szSection[i] = *szTmp;
        szTmp++;
        i++;
    }

    szSection[i] = '\0';        //字符串尾
    StringLRTrim(szSection);    //移掉字符串的左右空格
}

/***************************************************************
函数名：int CINIFile::WriteString
本函数 成功返回 1，失败返回 0
****************************************************************/
int CINIFile::WriteString(const char *szSectionName,
                          const char *szKeyName,
                          const char *szKeyValue)
{
    UINT4  uSectionFlag = DATA_NO;
    UINT4  uLineLen;
    int    dwFilePos;
    int    dwInsFilePos = 0;
    UINT4  uRetValue;
    char    szBuf[MAX_LINE_LEN];
    char    szTmpSectionName[MAX_LINE_LEN];
    char    szTmpKeyName[MAX_LINE_LEN];

    if ((m_pFile == NULL)   || (szSectionName == NULL)
         || (szKeyName == NULL) || (szKeyValue == NULL))
    {
        printf("File Not Opened\n");
        return 0;
    }

    if ((0 == strlen(szSectionName)) || (0 == strlen(szKeyName)))
    {
        return 0;
    }

    uLineLen = strlen(szSectionName) + 4;  /* [, ], \r\n */
    if (uLineLen > MAX_LINE_LEN)
    {
        return 0;
    }

    /* check the key line length */
    uLineLen = strlen(szKeyName) + strlen(szKeyValue) + 3;  /* =, \r\n */
    if (uLineLen > MAX_LINE_LEN)
    {
        return 0;
    }

    rewind(m_pFile);

    /* begin the work */
    for (;;)
    {
        dwFilePos = ftell(m_pFile);
        if (dwFilePos == -1)
        {
            return 0;
        }

        uRetValue = GetLineType(szBuf, MAX_LINE_LEN);
        if(szBuf[0] == '\0')
        {
            dwInsFilePos = dwFilePos;
        }

        switch (uRetValue)
        {
            case DATA_ERR:
                return 0;

            case DATA_EOF:
                if (uSectionFlag == DATA_YES)
                {
#ifdef _PLATFORM_WIN32
                    sprintf(szBuf, "%s=%s\r\n", szKeyName, szKeyValue);
#else
                    sprintf(szBuf, "%s=%s\n", szKeyName, szKeyValue);
#endif
                    if (fputs(szBuf, m_pFile) == EOF)
                    {
                        return 0;
                    }
                    m_lFileSize = ftell(m_pFile);
                    return 1;
                }
                else
                {
#ifdef _PLATFORM_WIN32
                    sprintf(szBuf, "[%s]\r\n%s=%s\r\n",
                            szSectionName, szKeyName, szKeyValue);
#else
                    sprintf(szBuf, "[%s]\n%s=%s\n",
                            szSectionName, szKeyName, szKeyValue);
#endif

                    if (EOF == fputs(szBuf, m_pFile))
                    {
                        return 0;
                    }
                    m_lFileSize = ftell(m_pFile);
                    return 1;
                }

            case DATA_SECTION_LINE:
                if (uSectionFlag == DATA_YES)
                {
#ifdef _PLATFORM_WIN32
                    sprintf(szBuf, "%s=%s\r\n", szKeyName, szKeyValue);
#else
                    sprintf(szBuf, "%s=%s\n", szKeyName, szKeyValue);
#endif
                    if(dwInsFilePos == 0)
                    {
                        dwInsFilePos = dwFilePos;
                    }
                    if (!InsTxtInFile(dwInsFilePos, szBuf, 0))
                    {
                        return 0;
                    }
                    return 1;
                }

                GetSectionName(szBuf, szTmpSectionName);
                dwInsFilePos = 0;
                if (ACE_OS::strcasecmp(szSectionName, szTmpSectionName) == 0)
                {
                    uSectionFlag = DATA_YES;
                }
                break;

            case DATA_KEY_LINE:
                if (uSectionFlag == DATA_YES)
                {
                    GetKeyName(szBuf, szTmpKeyName);
                    dwInsFilePos = 0;
                    if (ACE_OS::strcasecmp(szKeyName, szTmpKeyName) == 0)
                    {
#ifdef _PLATFORM_WIN32
                        sprintf(szBuf, "%s=%s\r\n", szKeyName, szKeyValue);
#else
                        sprintf(szBuf, "%s=%s\n", szKeyName, szKeyValue);
#endif

                        if (!InsTxtInFile(dwFilePos, szBuf, 1))
                        {
                            return 0;
                        }

                        return 1;
                    }
                }
                break;

            default:
                break;
        }
    }
}

/***************************************************************
函数名：int CINIFile::InsTxtInFile
本函数 成功返回 1，失败返回 0
****************************************************************/
int CINIFile::InsTxtInFile(int dwFilePos, const char *szInsString, BYTE yFlag)
{
    int   uRetCode;
    char  szCopyBuf[MAX_LINE_LEN];
    FILE* fpTmpFile = NULL;

    if (m_pFile == NULL)
    {
        return 0;
    }

    //备份原INI文件，将文件名后缀改为".tmp"
    char szTmpFile[MAX_PATH] = "";
    strcpy(szTmpFile, m_szFileName);
    int nLen = strlen(szTmpFile);
    szTmpFile[nLen - 3] = 't';
    szTmpFile[nLen - 2] = 'm';
    szTmpFile[nLen - 1] = 'p';

    fpTmpFile = fopen(szTmpFile, "wb");
    if (fpTmpFile == NULL)
    {
        remove(szTmpFile);
        printf("Failed to open tmp macro.ini file!\n");
        return 0;
    }
    if (fseek(m_pFile, 0, SEEK_SET) != 0)
    {
        fclose(fpTmpFile);
        remove(szTmpFile);
        return 0;
    }

    UINT4 uLineLen;
    do  //将原来的INI文件拷贝到一个临时文件中
    {
        uLineLen = fread(szCopyBuf, 1, sizeof(szCopyBuf), m_pFile);
        if (uLineLen != 0)
        {
            fwrite(szCopyBuf, 1, uLineLen, fpTmpFile);
            //by ldf 2002.5.21 对应问题单 13764
            fflush(fpTmpFile);
            //修改完毕
        }

        if (feof(m_pFile))
        {
            break;
        }

    }while (uLineLen == sizeof(szCopyBuf));
    fflush(fpTmpFile);
    fclose(fpTmpFile);

    fpTmpFile = fopen(szTmpFile, "rb");
    if (fpTmpFile == NULL)
    {
        remove(szTmpFile);
        printf("Failed to open tmp macro.ini file!\n");
        return 0;
    }
    
    //将文件指针定位到指定的位置
    if (fseek(fpTmpFile, dwFilePos, SEEK_SET) != 0)
    {
        fclose(fpTmpFile);
        remove(szTmpFile);
        return 0;
    }
    if (fseek(m_pFile, dwFilePos, SEEK_SET) != 0)
    {
        fclose(fpTmpFile);
        remove(szTmpFile);
        return 0;
    }

    uRetCode = 1;
    //覆盖原来的一行
    if (fputs(szInsString, m_pFile) == EOF)
    {
        fclose(fpTmpFile);
        remove(szTmpFile);
        fseek(m_pFile, dwFilePos, SEEK_SET);
        return 0;
    }
    fflush(m_pFile);

    //1标示第一行需要覆盖
    if (1 == yFlag)
    {
        fgets(szCopyBuf, MAX_LINE_LEN, fpTmpFile);
    }

    while(!feof(fpTmpFile))
    {
        const char* p = fgets(szCopyBuf, sizeof(szCopyBuf), fpTmpFile);
        if (p != NULL && fputs(szCopyBuf, m_pFile) == EOF)     //覆盖原来的一行
        {
            fclose(fpTmpFile);
            fclose(m_pFile);
            m_pFile = NULL;
            remove(m_szFileName);
            rename(szTmpFile, m_szFileName);
            if(Open())
            {
                fseek(m_pFile, dwFilePos, SEEK_SET);
            }
            return 0;
        }
    }
    fflush(m_pFile);

    m_lFileSize = ftell(m_pFile);
    fclose(fpTmpFile);
    remove(szTmpFile);

    fclose(m_pFile);
    m_pFile = NULL;
    ACE_OS::truncate(m_szFileName, m_lFileSize);
    Open();

    return uRetCode;
}


int CINIFile::WriteInt(const char *szSectionName,
                       const char *szKeyName,
                       UINT4 uKeyValue)
{
    char   szTmp[100];
    int    bRet = 0 ;
    sprintf(szTmp, "%d", uKeyValue);

    bRet = WriteString(szSectionName, szKeyName, szTmp);

    return bRet;
}

int CINIFile::StringToLine(const char *szKeyValue, const char *pszLine)
{
    if ((NULL == szKeyValue) || (NULL == pszLine))
    {
        return 0;
    }

    /* =, \r\n\0, at least one char for key name */
    if ((strlen(szKeyValue) + strlen(pszLine)) >= MAX_LINE_LEN - 5)
    {
        return 0;
    }

    //by ldf 2003-09-29 对应问题单SWPD01863
    char pszLineTmp[MAX_PATH];
    strcpy(pszLineTmp,pszLine);

    if (0 != strlen(pszLineTmp))
    {
        strcat(pszLineTmp, ";");
    }

    strcat(pszLineTmp, szKeyValue);

    return (strlen(pszLineTmp));
}
