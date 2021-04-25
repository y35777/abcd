#include "../include/frame.h"
#include "scp_bill_file.h"
#include "scp_rvy_task.h"
#include "../querybrowse/qb_comm.h"

/********************************************************
* ����: CScpBillFile::CScpBillFile
* ����: ���캯��
* ����: pFormat     : ��ʽ������ָ��
*       uBillType   : ԭʼ�����������ջ���
*       uAPType     : ���������
*       pScpRvyTask : CScpRvyTask�����ָ��
* ���: 
* ����: 
********************************************************/
CScpBillFile::CScpBillFile(CFormat *pFormat, 
                           UINT4 uBillType, 
                           UINT4 uAPType, 
                           CScpRvyTask* pScpRvyTask)
{
    m_pFormat       = pFormat;
    m_uBillType     = uBillType;
    m_uAPType       = uAPType;
    m_uBillLen      = pFormat->GetSrcFmtLen();

    m_nMemFileLen   = 4*1024*1024;
    m_pMemFile      = new BYTE[m_nMemFileLen];
    m_nMemDataLen   = 0;
    m_pFileStream   = NULL;
    ASSERT(pScpRvyTask != NULL);

    m_pScpRvyTask = pScpRvyTask;
}


/********************************************************
* ����: CScpBillFile::~CScpBillFile
* ����: ��������
* ����: 
* ���: 
* ����: 
********************************************************/
CScpBillFile::~CScpBillFile()
{
    m_pScpRvyTask = NULL;
    if (NULL != m_pFileStream)
    {
        fclose(m_pFileStream);
    }

    //������ASN1������صĳ�Ա�������ͷ�
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
BOOL CScpBillFile::Open(const char* szPathFile, int nOffset)
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
        size_t nBytesRead = fread(m_pMemFile, m_nMemDataLen,
                                  1, m_pFileStream);
        if (1 != nBytesRead)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/********************************************************
* ����: CScpBillFile::IsMatchedBill
* ����: ��һ�Ż������������ж�
* ����: pData : ��������ָ��
*       nLen  : ��������
* ���: 
* ����: BOOL �Ƿ��������
********************************************************/
BOOL CScpBillFile::IsMatchedBill(void* pData, int nLen)
{
    return m_pScpRvyTask->IsMatchedScpBill((BYTE*)pData, nLen);
}


/**********************************************************************
��������    ��ȡ��һ�ŷ��������Ļ�����¼���ݺ���
�������    ��
�������    pBillRecord---- ������¼����ָ��
            nCurOffset----�ҵ�һ�Ż�����¼��ĵ�ǰ�ڻ����ļ��е�λ��
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pBillRecord��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼
    2002.3.30   ����Զ      �޸Ĵ��룬�ڻ������ݴ���ʱ���ô�����Ϣ
**********************************************************************/
BOOL CScpBillFile::GetNextBill(CBillRecord*& pBillRecord,
                            int& nCurOffset, BOOL& bRemain)
{
    void* pBillData = NULL;
    UINT4 uBillLen;

    //�����ǰû�д��ļ�����ֱ�ӷ���
    if (NULL == m_pFileStream)
    {
        bRemain = FALSE;
        return TRUE;
    }

    BOOL bSucc = TRUE, bRemainInFile = TRUE;
    CBillRecord* pRecord = NULL;
    nCurOffset = m_nCurOffset;
    //���������һ��������¼����Ҫ���򷵻ظû�����¼��
    //���������ȡ��һ��������ֱ���ļ��������ҵ����������Ļ���
    while (1)
    {
        bSucc = Read(pBillData, uBillLen, bRemainInFile);
        if (!bSucc)
        {
            return FALSE;
        }
        else if (!bRemainInFile)
        {
            bRemain = FALSE;
            return TRUE;
        }

        //����Զ2002.3.30��ӣ����ⵥ��D12265
        if (NULL == pBillData)  //�������ݴ���
        {
            continue;
        }
        //����Զ2002.3.30��ӽ���

        //�жϻ�����¼�Ƿ������ѯ�������Ծ������ظü�¼���Ǽ����ж���һ������
        int nResult = m_pFormat->IsTheFormat(pBillData, uBillLen);
        if (0 == nResult)   //������¼���ϵ�ǰ�����ʽ
        {
            if ((0 == m_pFormat->IsTheQueryCondition(pBillData)) &&
                IsMatchedBill(pBillData, uBillLen))
            //�û�����¼�����ѯ�������򷵻ر�������¼
            {
                pRecord = new CBillRecord();
                bSucc = pRecord->SetContent((BYTE* )pBillData,
                                            uBillLen, m_pFormat);
                if (!bSucc)
                {
                    delete pRecord;
                    return FALSE;
                }
                else
                {
                    pBillRecord = pRecord;
                    bRemain = TRUE;
                    nCurOffset = m_nCurOffset;
                    return TRUE;
                }
            }
        }
    }
}

//��ȡ�ļ��ĵ�ǰ��дλ�ú���
int CScpBillFile::GetCurOffset()
{
    return m_nCurOffset;
}

//�����ļ��Ķ�дָ��λ�ú���
BOOL CScpBillFile::SetOffset(int nOffset)
{
    m_nCurOffset = nOffset;

    return TRUE;
}

//���ò�ѯ�����ַ�������
BOOL CScpBillFile::SetQueryCond(char* szQueryCond)
{
    int nResult = 0;
    if (strlen(szQueryCond) > 0)
    {
        nResult = m_pFormat->SetQueryCond(szQueryCond);
    }

    return (0 == nResult);
}

//�ر��ļ�����
BOOL CScpBillFile::Close()
{
    if (NULL != m_pFileStream)
    {
        fclose(m_pFileStream);
        m_pFileStream = NULL;
    }

    return TRUE;
}

/**********************************************************************
��������    ��ȡһ�Ż�����¼���ݺ���
�������    ��
�������    pData---- ������¼���ݵ�ָ�룬ʹ�ù̶����ڴ�
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CScpBillFile::Read(void*& pData, UINT4& uDataLen, BOOL& bRemain)
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

        BOOL bValid = TRUE;
        if (ORIG_BILL == m_uBillType)
        {
            bValid = FALSE;

            //�ж��Ƿ�����Ļ������ݶ���0
            for (int i = 0; i < m_uBillLen; i++)
            {
                if (0 != pBillRecord[i])
                {
                    bValid = TRUE;
                    break;
                }
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