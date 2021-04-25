// igwb_file.cpp: implementation of the CIGWBFile class.
//
//////////////////////////////////////////////////////////////////////
#include "igwb_file.h"
#include "../include/toolbox.h"
#include "../include/frame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIGWBFile::	CIGWBFile(UINT4 uBillType, UINT4 uBillLen, int nAPID)
{
	m_uBillType       = uBillType;
    m_uBillLen        = uBillLen;
	m_nMemFileLen     = 4*1024*1024;
    m_pMemFile        = new BYTE[m_nMemFileLen];
    m_nMemDataLen     = 0;
    m_pFileStream     = NULL;
    m_bFinalSPFileFmt = CFG_AP_KEY_FINAL_SPFILEFMT_DEFAULT;
    m_nAPID           = nAPID;
    m_nCurOffset      = 0;
    m_bMustMerge      = FALSE;
}

CIGWBFile::~CIGWBFile()
{
	if (NULL != m_pFileStream)
    {
        fclose(m_pFileStream);
    }  

	if (NULL != m_pMemFile)
    {
        delete[] m_pMemFile;
    }
}

/**********************************************************************
��������    ��ָ���Ļ����ļ���������Ӧ�Ķ�дλ��ָ�뺯��
�������    szPathFile----�����ļ����ƣ�����ȫ·����
            nOffset----��дλ��ƫ����
�������    ��
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CIGWBFile::Open(const char* szPathFile, int nOffset)
{
    //�����ǰ���ļ��򿪣���ر�֮
    if (NULL != m_pFileStream)    
    {
        Close();
    }

    strcpy(m_szFileFullPath, szPathFile);
    m_nCurOffset = nOffset;

    //���ļ���������ƫ����
    
    m_pFileStream = fopen(szPathFile, "rb");
    if (NULL == m_pFileStream)
    {
        return FALSE;
    }
    else
    {
        m_nMemDataLen = FileSize(m_pFileStream);
        if(m_nMemFileLen < m_nMemDataLen)
        {
            m_nMemFileLen = m_nMemDataLen;

            ASSERT(m_pMemFile != NULL);
            delete [] m_pMemFile;
            m_pMemFile = new BYTE[m_nMemFileLen];
        }

        if(m_nMemDataLen > 0)
        {
            size_t nBytesRead = fread(m_pMemFile, m_nMemDataLen, 
                                      1, m_pFileStream);
            if (1 != nBytesRead)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}
            
//��ȡ�ļ��ĵ�ǰ��дλ�ú���
int CIGWBFile::GetCurOffset()
{
    return m_nCurOffset;
}

//�����ļ��Ķ�дָ��λ�ú���
BOOL CIGWBFile::SetOffset(int nOffset)
{
    m_nCurOffset = nOffset;

    return TRUE;
}

//�ر��ļ�����
BOOL CIGWBFile::Close()
{
    if (NULL != m_pFileStream)
    {
        fclose(m_pFileStream);
        m_pFileStream = NULL;
    }

    return TRUE;
}

//�����Ƿ�ϲ���־
void CIGWBFile::SetMustMerge(BOOL bMustMerge)
{
    m_bMustMerge = bMustMerge;
}

void CIGWBFile::SetFinalSPFileFmt(BOOL bFinalSPFileFmt)
{
    m_bFinalSPFileFmt = bFinalSPFileFmt;
}

/**********************************************************************
��������    ��ȡһ�Ż�����¼���ݺ���
�������    ��
�������    pData----������¼���ݵ�ָ�룬ʹ�ù̶��ڴ�
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CIGWBFile::Read(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
	BOOL bSucc;

    if (ORIG_BILL == m_uBillType)
	{
		bSucc = ReadOriginal(pData, uDataLen, bRemain);
	}
	else
	{
		bSucc = ReadFinal(pData, uDataLen, bRemain);
	}

    return bSucc;
}


/**********************************************************************
��������    ��ȡһ��ԭʼ������¼����
�������    ��
�������    pData---- ������¼���ݵ�ָ�룬ʹ�ù̶����ڴ�
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CIGWBFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    ASSERT(NULL != m_pMemFile);

    BYTE* pBillRecord = NULL;
    if (m_nMemDataLen < m_uBillLen + m_nCurOffset)
    //�ڴ�ӳ���ļ��е�ʣ�೤�Ȳ���һ�������ĳ��ȣ�������������Ч
    {
        bRemain = FALSE;
    }
    else  
    {
        pBillRecord = m_pMemFile + m_nCurOffset;
        //����������Ļ�������ȫ��Ϊ0����˵�����ļ���ǰ���̵����һ��������
        //�����Ļ��������Լ����ļ��ĺ������ݱ��붪������bRemain����ΪFALSE

        BOOL   bValid = FALSE;
        //�ж��Ƿ�����Ļ������ݶ���0
        for (int i = 0; i < m_uBillLen; i++)
        {
			if (0 != pBillRecord[i])
            {
				bValid = TRUE;
                break;
             }
        }

        //����������ݶ���0����˵���û�������Ч�ģ����붪��
        if (!bValid)
        {
            bRemain = FALSE;
        }
        else
        {
            m_nCurOffset += m_uBillLen;
            pData = pBillRecord;
            uDataLen = m_uBillLen;
            bRemain = TRUE;
        }
    }

    return TRUE;
}


/**********************************************************************
��������    ��ȡһ�����ջ���������¼����
�������    ��
�������    pData---- ������¼���ݵ�ָ�룬ʹ�ù̶����ڴ�
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CIGWBFile::ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
	ASSERT(NULL != m_pMemFile);

    BYTE* pBillRecord = NULL;
    if (m_nMemDataLen < m_uBillLen + m_nCurOffset)
    //�ڴ�ӳ���ļ��е�ʣ�೤�Ȳ���һ�������ĳ��ȣ�������������Ч
    {
        bRemain = FALSE;
    }
    else  
    {
        pBillRecord = m_pMemFile + m_nCurOffset;
        m_nCurOffset += m_uBillLen;
        pData = pBillRecord;
        uDataLen = m_uBillLen;
        bRemain = TRUE;
    }

    return TRUE;   
}


