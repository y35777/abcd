#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/igwb_file_creator.h"
#include "cdr_stat_file.h"


/*******************************************************************
�������ƣ�CCDRStatFile()
�������������캯��
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
CCDRStatFile::CCDRStatFile()
{
    m_pIGWBFile = NULL;
}
/*******************************************************************
�������ƣ�~CCDRStatFile()
������������������
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
CCDRStatFile::~CCDRStatFile()
{
    if (NULL != m_pIGWBFile)
    {
        delete m_pIGWBFile;
        m_pIGWBFile = NULL;
    }
}

/*******************************************************************
�������ƣ�Init(UINT4 uAPType,  UINT4 uBillType,  BOOL  bMustMerge,
               UINT4 uBillLen, int nAPID)
�������������ò��������г�ʼ������
���������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatFile::Init(UINT4 uAPType,
                       UINT4 uBillType,
                       BOOL  bMustMerge,
                       BOOL  bFinalSPFileFmt,
                       UINT4 uBillLen,
                       int   nAPID)
{
    if(NULL == m_pIGWBFile)
    {
        m_pIGWBFile = CIGWBFileCreator::CreateIGWBFile(uBillType,
                                                       uAPType,
                                                       uBillLen,
                                                       nAPID);
    }

    m_pIGWBFile->SetMustMerge(bMustMerge);
    m_pIGWBFile->SetFinalSPFileFmt(bFinalSPFileFmt);

    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�Open(const char* const szPathFile)
������������ָ���Ļ����ļ�
���������const char* const szPathFile,�ļ�·��
�����������
�������ݣ����ɹ�����TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatFile::Open(const char* const szPathFile)
{
    return m_pIGWBFile->Open(szPathFile, 0);
}

/*******************************************************************
�������ƣ�Close()
�����������ر��ļ�����
�����������
�����������
�������ݣ����ɹ�����TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatFile::Close()
{
    if (NULL != m_pIGWBFile)
    {
        m_pIGWBFile->Close();
        return TRUE;
    }

    return FALSE;
}

/*******************************************************************
�������ƣ���ȡ��һ�Ż�����¼����
����������GetNextBill(BYTE*& pCDRRecord, UINT4& nCDRLen, BOOL&  bRemain)
�����������
���������BYTE*& pCDRRecord, ��������
          UINT4& nCDRLen, ���ݳ���
          BOOL&  bRemain,�Ƿ��л���
�������ݣ����ɹ�����TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatFile::GetNextBill(BYTE*& pCDRRecord, UINT4& nCDRLen, BOOL&  bRemain)
{
    return m_pIGWBFile->Read((void*&)pCDRRecord, nCDRLen, bRemain);
}

