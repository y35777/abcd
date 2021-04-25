/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "bk_status_file.h"


/********************************************************************
函数原型    public: CBKStatusFile()
功能描述    构造函数
输入参数    无。
输出参数    无。
返回值      无。
抛出异常    无。
*********************************************************************/
CBKStatusFile::CBKStatusFile():m_pFile(NULL)
{
}


/********************************************************************
函数原型    public: ~CBKStatusFile()
功能描述    析构函数
输入参数    无。
输出参数    无。
返回值      无。
抛出异常    无。
********************************************************************/
CBKStatusFile::~CBKStatusFile()
{
    Close();
}


/*******************************************************************
函数原型    public: BOOL Open(char* szPathFile);
功能描述    打开状态文件
输入参数    状态文件的全路径名。
输出参数    无。
返回值      返回打开状态文件是否成功，失败返回FALSE，成功返回TRUE。
抛出异常    无。
*******************************************************************/
BOOL CBKStatusFile::Open(const char *szPathFile)
{   
    //赋成员变量m_szPathFile为szPathFile
    SNPRINTF(m_szPathFile,MAX_PATH,"%s", szPathFile);
    m_szPathFile[MAX_PATH - 1] = 0;

    if (m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
    
    m_pFile = fopen(szPathFile, "rb+");
    if (m_pFile == NULL)
    {
        m_pFile = fopen(m_szPathFile, "wb+");
    }

    if(m_pFile != NULL) 
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/************************************************************************
函数原型    public: void Close();
功能描述    关闭状态文件
输入参数    无。
输出参数    无。
返回值      无。
抛出异常    无。
************************************************************************/
void CBKStatusFile::Close()
{
    if (m_pFile != NULL)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}


/************************************************************************
函数原型    public: BOOL SetOffset(int nOffset);
功能描述    设置状态文件的当前读写指针位置
输入参数    以文件头为开始的偏移长度。
输出参数    无。
返回值      返回设置状态文件的当前读写指针位置是否成功，失败返回FALSE，成功返回TRUE。
抛出异常    无。
************************************************************************/
BOOL CBKStatusFile::SetOffset(int nOffset, int POS)
{
    if (m_pFile == NULL)
    {
        return FALSE;
    }

    int nRet =0;
    nRet = fseek(m_pFile, nOffset, POS); 
    if ( nRet == 0)
    {
        return TRUE;
    }
    return FALSE;
}


/**************************************************************************
函数原型    public: UINT4 GetCurOffset();
功能描述    读取状态文件的当前读写指针位置
输入参数    无。
输出参数    无。
返回值      返回状态文件的当前读写指针位置
抛出异常    无。
**************************************************************************/
long CBKStatusFile::GetCurOffset()
{
    if (m_pFile == NULL)
    {
        return -1;
    }
    return ftell(m_pFile);
}


/*****************************************************************************
函数原型    public: BOOL Read(void* pData, UINT4 uLen);
功能描述    从文件当前位置读取数据
输入参数    uLen---该内存区域的长度
输出参数    pData---需要调用者预先分配的内存区域指针，函数将输出读出的文件内容
返回值      如果成功返回TRUE，否则返回FALSE
抛出异常    无。
*******************************************************************************/
BOOL CBKStatusFile::Read(void *pData, UINT4 uLen)
{
    if(NULL == m_pFile)
    {
        return FALSE;
    }

    UINT4 uReadLen = fread(pData, sizeof(BYTE), uLen, m_pFile);
    if(uReadLen == uLen)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*******************************************************************************
函数原型    public: BOOL Write(void* pData, UINT4 uLen);
功能描述    从文件的当前位置开始写入数据
输入参数    pData---待写入文件的内存数据指针。 uLen---该内存数据的长度
输出参数    无。
返回值      如果成功返回TRUE，否则返回FALSE
抛出异常    无。
********************************************************************************/
BOOL CBKStatusFile::Write(void *pData, UINT4 uLen)
{
    if(m_pFile == NULL)
    {
        return FALSE;
    }

    UINT4 uReadLen = fwrite(pData, sizeof(BYTE), uLen, m_pFile);
    int nRet = fflush(m_pFile);
    if(uReadLen == uLen && nRet == 0)
    {
        return TRUE;
    }
    else
    {
        char szTmpBuf[BK_MAX_INFO_LEN];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_BACKUP_STATUSFILE_OPERATE_FAIL;
        sTmpInfo += m_szPathFile;
        MSGOUT(MTS_BACKUP, MSGOUT_LEVEL_IMPORTANT, sTmpInfo.c_str());
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKUP, sTmpInfo.c_str());

        return FALSE;
    }
}


/*******************************************************************************
函数原型    public: int GetStatInfo (char* szChlName, SStatusNode *pStatInfo);
功能描述    从文件里获取指定通道的状态信息，返回该信息的开始位置
输入参数    szChlName---通道名
输出参数    pStatInfo---状态信息结构的指针，需要调用者分配内存区域
返回值      如果找到该记录，则返回该记录在文件中的位置，否则返回-1;
抛出异常    无。
********************************************************************************/
int CBKStatusFile::GetStatInfo(const char *szChlName, SStatusNode *pStatInfo)
{
    SStatusNode StatInfo;
    if (m_pFile == NULL)
    {
        return -1;
    }
    
    int nRet = fseek(m_pFile, 0, SEEK_SET);
    if (nRet != 0)
    {
        return -1;
    }

    nRet = -2;
    while(fread(&StatInfo, sizeof(SStatusNode),1 , m_pFile) == 1)
    {
        if (strcmp(StatInfo.szPath, szChlName) == 0)
        {
			//增加定位功能
			if(pStatInfo != NULL)
			{ 
				memcpy(pStatInfo, &StatInfo, sizeof(SStatusNode));
			}
			nRet = ftell(m_pFile) - sizeof(SStatusNode);
			break;
        }
    }
    return nRet;
}

