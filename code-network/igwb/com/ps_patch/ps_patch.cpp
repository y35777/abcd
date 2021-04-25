#include "../include/frame.h"
#include "../include/toolbox.h"
#include "ps_patch.h"
#include "../include/export_dll.h"

#define SOURCE_FILE_SIZE      1.5 * 1024 * 1024
#define DEST_FILE_SIZE        1.5 * 1024 * 1024

int CBSPatchForPS::s_nChannelCount = 0; 
BYTE* CBSPatchForPS::s_pBillBuf = NULL;
int CBSPatchForPS::s_nBillBufSize = 0;
BYTE* CBSPatchForPS::s_pMemFile = NULL;

extern "C" IGWB_Svc_Export BOOL PC_Init(const char* szConfigPath, BYTE nApID, UINT4 nChannelCount)
{
    return CBSPatchForPS::Init(szConfigPath, nApID, nChannelCount);
}
extern "C" IGWB_Svc_Export BOOL PC_Process(const char* szFileName, char*& szDestFileName, UINT4 nChannelNum)
{
    return CBSPatchForPS::Process(szFileName, szDestFileName, nChannelNum);
}
extern "C" IGWB_Svc_Export BOOL PC_Terminate()
{
    return CBSPatchForPS::Terminate();
}

/******************************************************
// 函数名: 	CBSPatchForPS::Init
// 作者:    chenliangwei
// 时期:    03-12-09
// 描述:    PS后存盘补丁初始化    
// 输入:
//       参数1 :char* szConfigPath
//       参数2 :BYTE nApID
//       参数3 :UINT4 nChannelCount
// 返回值: BOOL 
******************************************************/
BOOL CBSPatchForPS::Init(const char* szConfigPath, BYTE nApID, UINT4 nChannelCount)
{
    s_nChannelCount = nChannelCount;
    if(s_nChannelCount <= 0)
        return FALSE;

    //读取INI文件
    CINIFile IniFile(szConfigPath);
    BOOL bSucc = IniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_BS_TRC_FAIL_OPEN_INI);
        return FALSE;
    }

    
	char szSec[20];
	sprintf(szSec, "%s%d", CFG_SEC_AP, nApID);

    //创建读话单缓冲区
    s_nBillBufSize = IniFile.GetInt(szSec, "BSReadBillBuf", 
                       DEST_FILE_SIZE);
    s_pBillBuf = new BYTE[s_nBillBufSize];
    if (s_pBillBuf == NULL)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_MEM_ALLOC_FAIL);
        return FALSE;
    }
    
    //创建写话单文件缓冲区
    s_pMemFile = new BYTE[s_nBillBufSize];
    if (s_pMemFile == NULL)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_MEM_ALLOC_FAIL);

        delete []s_pBillBuf;
        s_pBillBuf = NULL;

        return FALSE;
    }

    return TRUE;
}


/******************************************************
// 函数名: 	CBSPatchForPS::Terminate
// 作者:    chenliangwei
// 时期:    03-12-09
// 描述:    清除函数
// 输入:
// 返回值: BOOL 
******************************************************/
BOOL CBSPatchForPS::Terminate()
{
    if (s_pBillBuf != NULL)
    {
        delete []s_pBillBuf;
        s_pBillBuf = NULL;
    }

    if (s_pMemFile != NULL)
    {
        delete []s_pMemFile;
        s_pMemFile = NULL;
    }

    return TRUE;
}

/******************************************************
// 函数名: 	CBSPatchForPS::Process
// 作者:    chenliangwei
// 时期:    03-12-09
// 描述:    处理PS域的话单文件：
            将一条话单前面4个字节长度指示域（网络字节序）去掉
// 输入:
//       参数1 :char* szFileName
//       参数2 :char*& szDestFileName
//       参数3 :UINT4 nChannelNum       从1开始的，比如：通道1,nChannelNum =1     

// 返回值: BOOL 
******************************************************/
BOOL CBSPatchForPS::Process(const char* szFileName, char*& szDestFileName, UINT4 nChannelNum)
{ 
    if(nChannelNum < 1)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_BS_ILLEGAL_CHL_ID, nChannelNum);
        return FALSE;
    }
    
    /*
     * 分多次把整个话单文件读入内存，进行转换。
     */
    FILE*  pSrcFile = fopen(szFileName, "rb");
    if (NULL == pSrcFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_BS_FAIL_OPEN_FILE, szFileName);
        return FALSE;
    }
    
   
    UINT4 nFileLen = FileSize(pSrcFile);

    //文件大小太大，重新分配空间
    if (s_nBillBufSize < nFileLen)
    {
        s_nBillBufSize = nFileLen;

        delete []s_pBillBuf;
        s_pBillBuf = new BYTE[s_nBillBufSize];
        
        delete []s_pMemFile;
        s_pMemFile = new BYTE[s_nBillBufSize];
    }

    size_t nBytesRead = fread(s_pBillBuf, nFileLen, 1, pSrcFile);
    if (nBytesRead != 1)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_BS_FAIL_READ_FILE, szFileName);
        return FALSE;
    }

    /*
	 * 对每条话单进行处理。
	 */
    UINT4 nBillLen = 0;
    UINT4 nFileOffset = 0;
    UINT4 nDestFileLen = 0; 

	while (nFileOffset < nFileLen)
	{
        //读入长度
        memcpy(&nBillLen, 
               s_pBillBuf + nFileOffset, 
               sizeof(nBillLen));
        nBillLen = NTOHL(nBillLen);

        //不足一条话单，不处理
        if (nBillLen > nFileLen - nFileOffset - 4)
        {
            TRACE(MTS_BACKSAVE_PATCH, 
                 S_BS_FIND_ERR_BILL, 
                 szFileName, nFileOffset);
            break;
        }

        //拷贝数据
        memcpy(s_pMemFile + nDestFileLen, 
               s_pBillBuf + nFileOffset + 4, 
               nBillLen);

        nFileOffset += (nBillLen + 4);
        nDestFileLen += nBillLen;
    }

    fclose(pSrcFile);
    pSrcFile = NULL;

    //存入文件
    BOOL bRet = TRUE;
    if (nDestFileLen != 0)
    {
        FILE* pDestFile = fopen(szDestFileName, "wb");
        if (pDestFile == NULL)
        {
            TRACE(MTS_BACKSAVE_PATCH, S_BS_FAIL_OPEN_FILE, szDestFileName);
            bRet = FALSE;
        }
        else
        {
            if (fwrite(s_pMemFile, nDestFileLen, 1, pDestFile) != 1)
            {
                TRACE(MTS_BACKSAVE_PATCH, S_BS_FAIL_WRITE_FILE, szDestFileName);
                bRet = FALSE;
            }
 
            fclose(pDestFile);
            pDestFile = NULL;
        }
    }

    return bRet;
}

