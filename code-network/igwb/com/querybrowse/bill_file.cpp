#include "bill_file.h"
#include "../include/toolbox.h"
#include "qb_comm.h"
#include "../include/frame.h"
#include "../utility/igwb_file_creator.h"


CBillFile::CBillFile(CFormat *pFormat,
                     UINT4 uBillType,
                     UINT4 uAPType,
                     int nAPID,
                     ACE_Mutex* pMutex)
{
    UINT4 uBillLen   = pFormat->GetSrcFmtLen();
    m_pFormat        = pFormat;
    m_pMutex         = pMutex;
    m_pRstIGWBFile   = CIGWBFileCreator::CreateIGWBFile(uBillType, uAPType, uBillLen, nAPID);
    m_pIGWBFile      = CIGWBFileCreator::CreateIGWBFile(uBillType, uAPType, uBillLen, nAPID);
    m_bDisplayErrBill = FALSE;
}

CBillFile::~CBillFile()
{
    if (NULL != m_pIGWBFile)
    {
        delete m_pIGWBFile;
    }
    if (NULL != m_pRstIGWBFile)
    {
        delete m_pRstIGWBFile;
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
BOOL CBillFile::Open(const char* szPathFile, int nOffset)
{
    return m_pIGWBFile->Open(szPathFile, nOffset);
}

/**********************************************************************
��������    ��ָ���Ļ����ļ���������Ӧ�Ķ�дλ��ָ�뺯��
�������    szPathFile----�����ļ����ƣ�����ȫ·����
            nOffset----��дλ��ƫ����
�������    ��
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CBillFile::OpenRstFile(const char* szPathFile, int nOffset)
{
    return m_pRstIGWBFile->Open(szPathFile, nOffset);
}

/**********************************************************************
��������    ��ȡ��һ�ŷ��������Ļ�����¼��ƫ�Ƶ�ַ
�������    ��
�������    nCurOffset----�ҵ�һ�Ż�����¼��ĵ�ǰ�ڻ����ļ��е�λ��
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pBillRecord��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼
    2002.3.30   ����Զ      �޸Ĵ��룬�ڻ������ݴ���ʱ���ô�����Ϣ
**********************************************************************/
BOOL CBillFile::GetNextBillOffset(int& nCurOffset, BOOL& bRemain)
{
    void* pBillData = NULL;
    UINT4 uDataLen  = 0;

    BOOL bSucc = TRUE;
    //���������һ��������¼����Ҫ���򷵻ظû�����¼��
    //���������ȡ��һ��������ֱ���ļ��������ҵ����������Ļ���
    while (1)
    {
        nCurOffset = GetCurOffset();
        bSucc = m_pIGWBFile->Read((void*&)pBillData, uDataLen, bRemain);
        if (!bSucc)
        {
            return FALSE;
        }
        else if (!bRemain)
        {
            return TRUE;
        }

        if (NULL == pBillData)  //�������ݴ���
        {
            //����ʾ���󻰵���Ϊ�������ʱ��ʾΪ���󻰵�
            if (m_bDisplayErrBill)
            {
                bRemain = TRUE;
                return TRUE;
            }

            continue;
        }

        //�жϻ�����¼�Ƿ������ѯ�������Ծ������ظü�¼���Ǽ����ж���һ������
        m_pMutex->acquire();
        int nResult = m_pFormat->IsTheFormat(pBillData, uDataLen);
        m_pMutex->release();

        if (0 == nResult)   //������¼���ϵ�ǰ�����ʽ
        {
            if ('\0' != m_szQueryCond[0]) //�ǲ�ѯ����
            {
                m_pMutex->acquire();
                nResult = m_pFormat->IsTheQueryCondition(pBillData);
                m_pMutex->release();

                if ((0 == nResult) && IsMatchedBill(pBillData, uDataLen))
                //�û�����¼�����ѯ�������򷵻ر�������¼
                {
                    return TRUE;
                }
            }
            else    //���������
            {
                return TRUE;
            }
        }
    }
}

/**********************************************************************
��������    ��ȡָ���ļ�ָ��λ�õĻ�����¼����
�������    nStartOffset----���Ż�����¼�����ڻ����ļ��е�λ��
�������    pBillRecord ----������¼����ָ��
            nBillLen    ----����Ļ�������
����ֵ      ERR_SUCCESS ----�ɹ�
�׳��쳣    ��
**********************************************************************/
int CBillFile::GetRstRecord(int nStartOffset,
                            BYTE*& pBillRecord,
                            UINT4& uBillLen)
{
    m_pRstIGWBFile->SetOffset(nStartOffset);

    BOOL bRemainInFile = TRUE;
    //���������һ��������¼����Ҫ���򷵻ظû�����¼��
    //���������ȡ��һ��������ֱ���ļ��������ҵ����������Ļ���
    BOOL bSucc = m_pRstIGWBFile->Read((void*&)pBillRecord, uBillLen, bRemainInFile);

    //ֻ������Ч����ʱ����ʧ��
    if (!bRemainInFile)
    {
        return ERR_FAIL;
    }

    //�ڶ�ȡ�ɹ��������ʧ��ʱ���سɹ�����ʱpBillRecord=NULL����uBillLen==0��
    return ERR_SUCCESS;
}

//��ȡ�ļ��ĵ�ǰ��дλ�ú���
int CBillFile::GetCurOffset()
{
    return m_pIGWBFile->GetCurOffset();
}

int CBillFile::GetFileLen()
{
    return m_pIGWBFile->GetFileLen();
}

//�����ļ��Ķ�дָ��λ�ú���
BOOL CBillFile::SetOffset(int nOffset)
{
    return m_pIGWBFile->SetOffset(nOffset);
}

//���ò�ѯ�����ַ�������
BOOL CBillFile::SetQueryCond(char* szQueryCond)
{
    STRNCPY(m_szQueryCond, szQueryCond, MAX_COND_LEN);
    int nResult = 0;
    if (strlen(szQueryCond) > 0)
    {
        nResult = m_pFormat->SetQueryCond(szQueryCond);
    }

    return (0 == nResult);
}

//�ر��ļ�����
BOOL CBillFile::Close()
{
    return m_pIGWBFile->Close();
}


//�����Ƿ�ϲ���־
void CBillFile::SetMustMerge(BOOL bMustMerge)
{
    if(m_pIGWBFile)
    {
        m_pIGWBFile->SetMustMerge(bMustMerge);
    }
    if(m_pRstIGWBFile)
    {
        m_pRstIGWBFile->SetMustMerge(bMustMerge);
    }
}

void CBillFile::SetFinalSPFileFmt(BOOL bFinalSPFileFmt)
{
    m_pIGWBFile->SetFinalSPFileFmt(bFinalSPFileFmt);
}

//�����Ƿ���ʾ����Ļ���
void CBillFile::SetDisplayErrBill(BOOL bDisplayErrBill)
{
    m_bDisplayErrBill = bDisplayErrBill;
}

