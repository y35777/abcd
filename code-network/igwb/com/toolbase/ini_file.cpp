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
 *������������INI�ļ�m_szFileName�����ļ�ָ�����m_pFile
 *���룺
 *      ��
 *�����
 *      �ޡ�
 *���أ�
 *              ��
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
 *�����������ر�INI�ļ���m_szFileName��
 *���룺
 *      ��
 *�����
 *      �ޡ�
 *���أ�
 *              ��
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
 *������������һ����ֵ���ڵ����еõ�����������ӡ�KeyName=NameValue���еõ�"KeyName"
 *���룺
 *      szLine����ֵ���ڵ��У����硰KeyName=NameValue��
 *�����
 *      szKey���ҵ��ļ���KeyName
 *���أ�
 *              ��
 ****************************************************************************/
void CINIFile::GetKeyName(const char *szLine, char *szKey)
{
    int i = 0;

    sprintf(szKey, "%s", "");
    if (strlen(szLine) == 0)            return; //�ַ���Ϊ�գ�����
    if (strchr(szLine, '=') == NULL)    return; //�ַ����и�����������=��������

    while (szLine[i] != '=')
    {
         szKey[i] = szLine[i];
         i++;
    }

    szKey[i] = '\0';
    StringLRTrim(szKey);        //��ȥ���ҵĿո�
}

/************************************************************************
 *����������m_pFileָ��ָ����ļ���INI���ж�ȡ����ΪnLen�ֽڵ����ݣ�����
            pszBuf
 *���룺
 *      pszBuf�����ڴ�Ŷ�����Ļ�����
 *      nLen��  ָ�����ζ����ֽ���
 *      m_pFile��    �ļ�ָ��
 *
 * �����
 *      pszTmp�������ָ��
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

    //by ldf 2002.4.26 ��Ӧ���ⵥD13202
    StringLRTrim(szLine);
    //�޸����

    nStrLen = strlen(szLine);

    if (nStrLen >= 2)   //����ַ���ͷΪ��//�����ߡ�;�������ʾ��һ��ע����
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
            return DATA_SECTION_LINE;   //�С�[]��,��һ���Ƕ���
    }

    szTmp = strchr(szLine, '=');
    if (szTmp != NULL)
    {
        return DATA_KEY_LINE;   //�С���������һ���Ǽ�ֵ
    }

    return DATA_FAULT_LINE;
}

/***************************************************************************
 *������������һ����ֵ���ڵ����еõ���ֵ������ӡ�KeyName=NameValue���е�
            ��"NameValue"
 *���룺
 *      szLine����ֵ���ڵ��У�����KeyName=NameValue
 *�����
 *      szKey���ҵ��ļ���NameValue
 *���أ�
 *      ��
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
    if (szLineTmpPt == NULL)    //�������и���û�С�=��
    {
        return nKeyValueNum;
    }

    szLineTmpPt++;      //�Ƶ�=�ұߵ�һ���ַ���

    while ((*szLineTmpPt == ' ') || (*szLineTmpPt == '\t'))
    {
        szLineTmpPt++;  //�ո񲻼�������
    }

    if (*szLineTmpPt == '\0')
    {
        return nKeyValueNum;
    }
    else
    {
        nKeyValueNum = 1;
    }

    //�õ���ֵ
    while (*szLineTmpPt != '\0')
    {
        if (*szLineTmpPt == ';')        //�����";"�����ڸô�����һ���ո�
        {
            //����������";"�������һ���ո�
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

        //�������������Ƿ��㹻
        i++;
        if (i == nBufLen)
        {
            *szKeyValueTmpPt = '\0';
            return DATA_OVERFLOW;
        }
    }

    //������һ���ַ��ǡ�;���������һ���ո�
    if (nFlag == 1)
    {
        *szKeyValueTmpPt = ' ';
        szKeyValueTmpPt++;
    }

    *szKeyValueTmpPt = '\0';

    return nKeyValueNum;
}

/***************************************************************************
 *������������ָ�������ͼ����׼�ֵ
 *���룺
 *      szSection������������[SectionName]�е�SectionName
 *      szKey    ������������KeyName=KeyValue�е�KeyName
 *      nDefaullt�����û���ҵ�����ȡĬ��ֵnDefault
 *�����
 *      ��
 *���أ�
 *      �õ��ļ�ֵ��������
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
 *��������������ָ������szSection�µļ���ΪszKey�ļ�ֵ���ַ�����
 *���룺
 *      szSection ������������[SectionName]�е�SectionName
 *      szKey     ������������KeyName=KeyValue�е�KeyName
 *      szDefaullt�����û���ҵ�����ȡĬ��ֵszDefault
 *�����
 *      szOut     ����ֵ���
 *      nLen      ��szOut�ĳ���
 *���أ�
 *      ����ӷ������ȡ�
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
    int   nSectionFlag = 0;             //�����ҵ���־
    int   nRetValue;
    char  szTmpSection[MAX_LINE_LEN];
    char  szTmpKey[MAX_LINE_LEN];

    char  szError[200];

    if(szDefault != szOut)
    {
        memmove(szOut, szDefault, min(nLen, strlen(szDefault) + 1));   //�ȸ�һ��Ĭ��ֵszDefault
    }
    szOut[nLen - 1] = '\0';

    if (m_pFile == NULL)
    {
        return 0;      //�ļ�û�д�
    }

    rewind(m_pFile);            //��ת�ļ�ָ�룬���ļ�ָ���ļ�ͷ

    //Ҫ����һ������ֵ����������ҵ��ü����ڵĶ���
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
                if (nSectionFlag == 1)  //�Ѿ��ҵ��˶���
                {
                    GetKeyName(szBuf, szTmpKey);
                    StringLRTrim(szTmpKey);
                    if (ACE_OS::strcasecmp(szKey, szTmpKey) == 0)    //��Сд�����бȽ�
                    {
                        nStringNum = GetKeyValue(szBuf, szTmpBuf, MAX_LINE_LEN);
                        char* pColon = strchr(szTmpBuf, ';');  //ȥ��ע��
                        if (pColon != NULL)
                        {
                            szTmpBuf[pColon - szTmpBuf] = '\0';
                        }
                        //�޸ļ�¼ ��ŷ� ���������һ�У���ȥ�����õĿո��\t����Ȼ������ģ�������Ӧ�Ĵ���
                        StringLRTrim(szTmpBuf);
                        strncpy(szOut, szTmpBuf, nLen);
                        szOut[nLen - 1] = '\0';

                        return strlen(szOut);
                    }
                }
                break;

            case DATA_SECTION_LINE:
                if (nSectionFlag == 1)  //�ҵ���һ��������ȥ�ˣ�����
                {
                    return nStringNum;
                }

                GetSectionName(szBuf, szTmpSection);
                StringLRTrim(szTmpSection);
                if (ACE_OS::strcasecmp(szSection, szTmpSection) == 0) //��Сд�����бȽ�
                {
                    nSectionFlag = 1;           //�ҵ�����
                }
                break;

            default:
                break;
        }
    }
}

/*****************************************************************************
 *����������
 *���룺
 *      szInput   ����;�ָ����ַ���
 *      uTotalNum ��szInput�еķָ�������
 *      uValueNum ��ϣ���������ַ������
 *�����
 *      szKeyValue�����ҵ����ַ���
 *���أ�
 *      TRUE�����ҳɹ���FALSE������ʧ��
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
 *�������������ַ����в��ҳ�INI�����������[SectionName]���ҳ�SectionName
 *���룺
 *      szLine   �� һ���ַ���������[SectionName]
 *�����
 *      szSection�����ҵ��Ķ��������û���ҵ�����szSection���Ϊ��
 *���أ�
 *      �ޡ�
 ****************************************************************************/
void CINIFile::GetSectionName(const char *szLine, char *szSection)
{
    int   i = 0;
    const char  *szTmp;

    sprintf(szSection, "%s", "");
    szTmp = strchr(szLine, '[');
    if (szTmp == NULL)      return;

    szTmp++;    //�Ƶ�'['��ǰһ���ַ���λ��

    while(*szTmp != ']')
    {
        //�Ѿ������ַ�����β������û���ҵ�']'
        if (*szTmp == '\0')
        {
            i = 0;
            break;
        }

        szSection[i] = *szTmp;
        szTmp++;
        i++;
    }

    szSection[i] = '\0';        //�ַ���β
    StringLRTrim(szSection);    //�Ƶ��ַ��������ҿո�
}

/***************************************************************
��������int CINIFile::WriteString
������ �ɹ����� 1��ʧ�ܷ��� 0
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
��������int CINIFile::InsTxtInFile
������ �ɹ����� 1��ʧ�ܷ��� 0
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

    //����ԭINI�ļ������ļ�����׺��Ϊ".tmp"
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
    do  //��ԭ����INI�ļ�������һ����ʱ�ļ���
    {
        uLineLen = fread(szCopyBuf, 1, sizeof(szCopyBuf), m_pFile);
        if (uLineLen != 0)
        {
            fwrite(szCopyBuf, 1, uLineLen, fpTmpFile);
            //by ldf 2002.5.21 ��Ӧ���ⵥ 13764
            fflush(fpTmpFile);
            //�޸����
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
    
    //���ļ�ָ�붨λ��ָ����λ��
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
    //����ԭ����һ��
    if (fputs(szInsString, m_pFile) == EOF)
    {
        fclose(fpTmpFile);
        remove(szTmpFile);
        fseek(m_pFile, dwFilePos, SEEK_SET);
        return 0;
    }
    fflush(m_pFile);

    //1��ʾ��һ����Ҫ����
    if (1 == yFlag)
    {
        fgets(szCopyBuf, MAX_LINE_LEN, fpTmpFile);
    }

    while(!feof(fpTmpFile))
    {
        const char* p = fgets(szCopyBuf, sizeof(szCopyBuf), fpTmpFile);
        if (p != NULL && fputs(szCopyBuf, m_pFile) == EOF)     //����ԭ����һ��
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

    //by ldf 2003-09-29 ��Ӧ���ⵥSWPD01863
    char pszLineTmp[MAX_PATH];
    strcpy(pszLineTmp,pszLine);

    if (0 != strlen(pszLineTmp))
    {
        strcat(pszLineTmp, ";");
    }

    strcat(pszLineTmp, szKeyValue);

    return (strlen(pszLineTmp));
}
