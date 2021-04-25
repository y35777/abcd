/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "bk_status_file.h"


/********************************************************************
����ԭ��    public: CBKStatusFile()
��������    ���캯��
�������    �ޡ�
�������    �ޡ�
����ֵ      �ޡ�
�׳��쳣    �ޡ�
*********************************************************************/
CBKStatusFile::CBKStatusFile():m_pFile(NULL)
{
}


/********************************************************************
����ԭ��    public: ~CBKStatusFile()
��������    ��������
�������    �ޡ�
�������    �ޡ�
����ֵ      �ޡ�
�׳��쳣    �ޡ�
********************************************************************/
CBKStatusFile::~CBKStatusFile()
{
    Close();
}


/*******************************************************************
����ԭ��    public: BOOL Open(char* szPathFile);
��������    ��״̬�ļ�
�������    ״̬�ļ���ȫ·������
�������    �ޡ�
����ֵ      ���ش�״̬�ļ��Ƿ�ɹ���ʧ�ܷ���FALSE���ɹ�����TRUE��
�׳��쳣    �ޡ�
*******************************************************************/
BOOL CBKStatusFile::Open(const char *szPathFile)
{   
    //����Ա����m_szPathFileΪszPathFile
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
����ԭ��    public: void Close();
��������    �ر�״̬�ļ�
�������    �ޡ�
�������    �ޡ�
����ֵ      �ޡ�
�׳��쳣    �ޡ�
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
����ԭ��    public: BOOL SetOffset(int nOffset);
��������    ����״̬�ļ��ĵ�ǰ��дָ��λ��
�������    ���ļ�ͷΪ��ʼ��ƫ�Ƴ��ȡ�
�������    �ޡ�
����ֵ      ��������״̬�ļ��ĵ�ǰ��дָ��λ���Ƿ�ɹ���ʧ�ܷ���FALSE���ɹ�����TRUE��
�׳��쳣    �ޡ�
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
����ԭ��    public: UINT4 GetCurOffset();
��������    ��ȡ״̬�ļ��ĵ�ǰ��дָ��λ��
�������    �ޡ�
�������    �ޡ�
����ֵ      ����״̬�ļ��ĵ�ǰ��дָ��λ��
�׳��쳣    �ޡ�
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
����ԭ��    public: BOOL Read(void* pData, UINT4 uLen);
��������    ���ļ���ǰλ�ö�ȡ����
�������    uLen---���ڴ�����ĳ���
�������    pData---��Ҫ������Ԥ�ȷ�����ڴ�����ָ�룬����������������ļ�����
����ֵ      ����ɹ�����TRUE�����򷵻�FALSE
�׳��쳣    �ޡ�
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
����ԭ��    public: BOOL Write(void* pData, UINT4 uLen);
��������    ���ļ��ĵ�ǰλ�ÿ�ʼд������
�������    pData---��д���ļ����ڴ�����ָ�롣 uLen---���ڴ����ݵĳ���
�������    �ޡ�
����ֵ      ����ɹ�����TRUE�����򷵻�FALSE
�׳��쳣    �ޡ�
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
����ԭ��    public: int GetStatInfo (char* szChlName, SStatusNode *pStatInfo);
��������    ���ļ����ȡָ��ͨ����״̬��Ϣ�����ظ���Ϣ�Ŀ�ʼλ��
�������    szChlName---ͨ����
�������    pStatInfo---״̬��Ϣ�ṹ��ָ�룬��Ҫ�����߷����ڴ�����
����ֵ      ����ҵ��ü�¼���򷵻ظü�¼���ļ��е�λ�ã����򷵻�-1;
�׳��쳣    �ޡ�
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
			//���Ӷ�λ����
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

