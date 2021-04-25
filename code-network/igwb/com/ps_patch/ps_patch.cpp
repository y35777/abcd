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
// ������: 	CBSPatchForPS::Init
// ����:    chenliangwei
// ʱ��:    03-12-09
// ����:    PS����̲�����ʼ��    
// ����:
//       ����1 :char* szConfigPath
//       ����2 :BYTE nApID
//       ����3 :UINT4 nChannelCount
// ����ֵ: BOOL 
******************************************************/
BOOL CBSPatchForPS::Init(const char* szConfigPath, BYTE nApID, UINT4 nChannelCount)
{
    s_nChannelCount = nChannelCount;
    if(s_nChannelCount <= 0)
        return FALSE;

    //��ȡINI�ļ�
    CINIFile IniFile(szConfigPath);
    BOOL bSucc = IniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_BS_TRC_FAIL_OPEN_INI);
        return FALSE;
    }

    
	char szSec[20];
	sprintf(szSec, "%s%d", CFG_SEC_AP, nApID);

    //����������������
    s_nBillBufSize = IniFile.GetInt(szSec, "BSReadBillBuf", 
                       DEST_FILE_SIZE);
    s_pBillBuf = new BYTE[s_nBillBufSize];
    if (s_pBillBuf == NULL)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_MEM_ALLOC_FAIL);
        return FALSE;
    }
    
    //����д�����ļ�������
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
// ������: 	CBSPatchForPS::Terminate
// ����:    chenliangwei
// ʱ��:    03-12-09
// ����:    �������
// ����:
// ����ֵ: BOOL 
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
// ������: 	CBSPatchForPS::Process
// ����:    chenliangwei
// ʱ��:    03-12-09
// ����:    ����PS��Ļ����ļ���
            ��һ������ǰ��4���ֽڳ���ָʾ�������ֽ���ȥ��
// ����:
//       ����1 :char* szFileName
//       ����2 :char*& szDestFileName
//       ����3 :UINT4 nChannelNum       ��1��ʼ�ģ����磺ͨ��1,nChannelNum =1     

// ����ֵ: BOOL 
******************************************************/
BOOL CBSPatchForPS::Process(const char* szFileName, char*& szDestFileName, UINT4 nChannelNum)
{ 
    if(nChannelNum < 1)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_BS_ILLEGAL_CHL_ID, nChannelNum);
        return FALSE;
    }
    
    /*
     * �ֶ�ΰ����������ļ������ڴ棬����ת����
     */
    FILE*  pSrcFile = fopen(szFileName, "rb");
    if (NULL == pSrcFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, S_BS_FAIL_OPEN_FILE, szFileName);
        return FALSE;
    }
    
   
    UINT4 nFileLen = FileSize(pSrcFile);

    //�ļ���С̫�����·���ռ�
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
	 * ��ÿ���������д���
	 */
    UINT4 nBillLen = 0;
    UINT4 nFileOffset = 0;
    UINT4 nDestFileLen = 0; 

	while (nFileOffset < nFileLen)
	{
        //���볤��
        memcpy(&nBillLen, 
               s_pBillBuf + nFileOffset, 
               sizeof(nBillLen));
        nBillLen = NTOHL(nBillLen);

        //����һ��������������
        if (nBillLen > nFileLen - nFileOffset - 4)
        {
            TRACE(MTS_BACKSAVE_PATCH, 
                 S_BS_FIND_ERR_BILL, 
                 szFileName, nFileOffset);
            break;
        }

        //��������
        memcpy(s_pMemFile + nDestFileLen, 
               s_pBillBuf + nFileOffset + 4, 
               nBillLen);

        nFileOffset += (nBillLen + 4);
        nDestFileLen += nBillLen;
    }

    fclose(pSrcFile);
    pSrcFile = NULL;

    //�����ļ�
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

