#include "file_group.h"
#include <algorithm>
#include "qb_comm.h"
#include "../include/toolbox.h"
#include "../include/frame.h"
#include "bill_qb.h"

//���캯��
CFileGroup::CFileGroup(CFormat *pFormat, UINT4 uBillType, UINT4 uAPType, ACE_Mutex* pMutex, int nAPID)
{
    m_nCurFileNo       = -1;
    m_nCurOffset       = -1;
    m_nQueryedFileLen  = 0;
    m_nTotalFileLength = 0;
    m_nStartFileNo     = -1;

    m_pBillFile        = new CBillFile(pFormat, uBillType, uAPType, nAPID, pMutex);
}

CFileGroup::~CFileGroup()
{
    if (NULL != m_pBillFile)
    {
        delete m_pBillFile;
    }
}

void CFileGroup::SetMustMerge(BOOL bMustMerge)
{
    if(NULL != m_pBillFile)
    {
        m_pBillFile->SetMustMerge(bMustMerge);
    }
}

void CFileGroup::SetFinalSPFileFmt(BOOL bFinalSPFileFmt)
{
    if(NULL != m_pBillFile)
    {
        m_pBillFile->SetFinalSPFileFmt(bFinalSPFileFmt);
    }
}

/**********************************************************************
��������    ��ָ���Ļ����ļ������������дλ��Ϊָ����λ��
�������    nFileNo----ָ���Ļ����ļ����
            nOffset----ָ���Ķ�дλ��
�������    ��
����ֵ      �򿪳ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CFileGroup::Open(int nFileNo, int nOffset)
{
    ASSERT(NULL != m_pBillFile);

    BOOL bSucc = FALSE;

    if (m_nCurFileNo == nFileNo)
    {
        bSucc = m_pBillFile->SetOffset(nOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }
    else
    {
        //�رյ�ǰ�ļ�����ָ���ļ���������дλ��
        bSucc = m_pBillFile->Close();
        if (!bSucc)
        {
            return FALSE;
        }

        STRING szFileName = m_qFileNames[nFileNo];

        char szFileFullPath[MAX_PATH];
        sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName.c_str());
        bSucc = m_pBillFile->Open(szFileFullPath, nOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    m_nCurFileNo = nFileNo;
    m_nCurOffset = nOffset;

    return TRUE;
}

//��ȡ��ǰ�򿪵Ļ����ļ���ź��䵱ǰ��дλ��
void CFileGroup::GetCurPosition(int& nFileNo, int& nOffset)
{
    nFileNo = m_nCurFileNo;
    nOffset = m_nCurOffset;
}

/**********************************************************************
��������    ��ȡ��һ�ŷ��������Ļ�����¼���ݵ�ƫ�Ƶ�ַ
�������    ��
�������    nStartFileNo----���Ż�����¼�������ڻ����ļ����
            nStartOffset----���Ż�����¼�����ڻ����ļ��е�λ��
            nBillLen---������¼���ݵĳ���
            bRemain----�Ƿ��к��������ݣ�ΪFALSEʱ��pBillRecord��Ч
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CFileGroup::GetNextRecordOffset(int& nStartFileNo, int& nStartOffset,
                                     BOOL& bRemain)
{
    BOOL bRemainInFile = TRUE;
    while (1)
    {
        BOOL bSucc = m_pBillFile->GetNextBillOffset(m_nCurOffset, bRemainInFile);
        if (!bSucc)
        {
            return FALSE;
        }

        if (bRemainInFile)  //�ڵ�ǰ���ļ����ҵ��˻�����¼
        {
            nStartFileNo = m_nCurFileNo;
            nStartOffset = m_nCurOffset;
            bRemain = TRUE;
            return TRUE;
        }
        else    //���򣬴���һ���ļ������������һ�Ż�����¼
        {
            BOOL bRemainInGroup;
            bSucc = OpenNextFile(bRemainInGroup);
            
            //by lx SWPD04804
            if (!bSucc && bRemainInGroup)
            {
                continue;
            }
            //end SWPD04804

            if (!bRemainInGroup)
            {
                bRemain = FALSE;
                return TRUE;
            }
        }
    }
}

/**********************************************************************
��������    ��ȡָ���ļ�ָ��λ�õĻ�����¼����
�������    nStartFileNo----���Ż�����¼�������ڻ����ļ����
            nStartOffset----���Ż�����¼�����ڻ����ļ��е�λ��
�������    pBillRecord ----������¼����ָ��
            nBillLen    ----����Ļ�������
����ֵ      ERR_SUCCESS        ---�ɹ�
�׳��쳣    ��
**********************************************************************/
int CFileGroup::GetRstRecord(int nStartFileNo,
                             int nStartOffset,
                             BYTE*& pBillRecord,
                             UINT4& uBillLen)
{
    if(m_nStartFileNo != nStartFileNo)
    {
        char szFileName[MAX_PATH];
        SNPRINTF(szFileName,
                 sizeof(szFileName),
                 "%s/%s",
                 m_szPath,
                 m_qFileNames[nStartFileNo].c_str());
        BOOL bRet = m_pBillFile->OpenRstFile(szFileName, nStartOffset);
        if(!bRet)
        {
            return ERR_FAIL;
        }
        m_nStartFileNo = nStartFileNo;
    }

    int nRet = m_pBillFile->GetRstRecord(nStartOffset,
                                         pBillRecord,
                                         uBillLen);

    return nRet;
}

/**********************************************************************
��������    ׼�������ļ�������������
�������    szFileFullPath----����ȫ·���Ļ����ļ�����
�������    ��
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CFileGroup::PrepareFile(const char* szFileFullPath)
{
    m_pBillFile->SetQueryCond((char* )(""));

    //��ȫ·���л���ļ�·�����ļ���
    char* pPos = (char* )strrchr(szFileFullPath, '/');
    if (NULL == pPos)   //���û���ҵ�'/'������'\'����
    {
        pPos = (char* )strrchr(szFileFullPath, '\\');
        if (NULL == pPos)
        {
            return FALSE;
        }
    }
    memcpy(m_szPath, szFileFullPath, pPos - szFileFullPath);
    m_szPath[pPos - szFileFullPath] = '\0';
    STRING szFileName = (pPos + 1);

    m_qFileNames.push_back(szFileName);

    CacalTotalFileLength();

    return TRUE;
}


/**********************************************************************
��������    ׼�������ļ�����Բ�ѯ����
�������    szPath----Ŀ¼·��
            szStartRange----��ѯ�Ŀ�ʼ��Χ��ȡֵ���������ڡ��򻰵��ļ���
            szEndRange----��ѯ������Χ��ȡֵ���������ڡ��򻰵��ļ���
            uFlg----��ѯ��Χ��־���������ֲ�ѯ��Χ������Ŀ¼���ǻ����ļ����ƣ�
                    ȡֵ��QueryRangeType�ﶨ��
            szQueryCond----��ѯ�����ַ���
�������    ��
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CFileGroup::PrepareFiles(const char* szPath, const char* szStartRange,
                              const char* szEndRange,
                              UINT4 uFlg, char* szQueryCond)
{
    strcpy(m_szPath, szPath);
    BOOL bSucc = m_pBillFile->SetQueryCond(szQueryCond);
    if (!bSucc)
    {
        return FALSE;
    }

    LIST<STRING> FileList;    //�����������ļ������б�
    if (DATE_RANGE == uFlg)
    {
        //��ȡ�Ź������������Ŀ¼��Ȼ����еõ���Χ�ڵ���������Ŀ¼
        LIST<STRING> DateList;
        STRING szDateName;
        GetDateDir(m_szPath, DateList);
        while (1)     //��ȥ����Ŀ¼�б���ͷ����������������
        {
            if (DateList.empty())
            {
               break;
            }

            szDateName = DateList.front();
            if (0 == strcmp(szDateName.c_str(), szStartRange))
            {
                break;
            }
            else
            {
                DateList.pop_front();
            }
        }
        while (1)     //��ȥ����Ŀ¼�б���β����������������
        {
            if (DateList.empty())
            {
                break;
            }

            szDateName = DateList.back();
            if (0 == strcmp(szDateName.c_str(), szEndRange))
            {
                break;
            }
            else
            {
                DateList.pop_back();
            }
        }

        //����õ�������Ŀ¼�б�Ϊ�գ�˵����ʼ����Ŀ¼����ֹ����
        //Ŀ¼��Ӳ�����Ѿ������ڣ���ʱ����ʧ��
        if (DateList.empty())
        {
            return FALSE;
        }

        //��ȡ������Χ�ڵ�����Ŀ¼�µ������ļ���
        //�ļ�����ʽΪ����Ŀ¼/�ļ����ķ�ʽ
        char szFilePath[MAX_PATH];
        LIST<STRING> FileSubList;     //���һ������Ŀ¼�µ������ļ����б�
        LIST<STRING>::iterator i, j;
        for (i = DateList.begin(); i != DateList.end(); i++)
        {
            szDateName = *i;
            sprintf(szFilePath, "%s/%s", m_szPath, szDateName.c_str());
            GetFileList(szFilePath, FileSubList);

            //GetFileList���ص��б���ֻ�����ļ��������Ա��뽫����Ŀ¼��Ϣ����ȥ
            for (j = FileSubList.begin(); j != FileSubList.end(); j++)
            {
                *j = szDateName + "/" + *j;
            }

            //��FileSubList�е����ݼ��뵽FileList��ĩβ
            FileList.splice(FileList.end(), FileSubList);
            FileSubList.clear();
        }
        if (FileList.empty())
        {
            return FALSE;
        }
    }
    else
    {
        //��ȡĿ¼���Ź���������ļ���Ȼ����еõ���Χ�ڵ������ļ�
        GetFileList(m_szPath, FileList);
        STRING szFileName;
        while (1)     //��ȥ�ļ��б���ͷ����������������
        {
            if (FileList.empty())
            {
                break;
            }

            szFileName = FileList.front();
            if (0 == strcmp(szFileName.c_str(), szStartRange))
            {
                break;
            }
            else
            {
                FileList.pop_front();
            }
        }
        while (1)     //��ȥ�ļ��б���β����������������
        {
            if (FileList.empty())
            {
                break;
            }

            szFileName = FileList.back();
            if (0 == strcmp(szFileName.c_str(), szEndRange))
            {
                break;
            }
            else
            {
                FileList.pop_back();
            }
        }

        //����õ����ļ��б�Ϊ�գ�˵����ʼ�ļ�����ֹ�ļ�
        //��Ӳ�����Ѿ������ڣ���ʱ����ʧ��
        if (FileList.empty())
        {
            return FALSE;
        }
    }

    //���õ����ļ��б��뵽m_qFileNames��
    LIST<STRING>::iterator i;
    for (i = FileList.begin(); i != FileList.end(); i++)
    {
        m_qFileNames.push_back(*i);
    }

    CacalTotalFileLength();

    return TRUE;
}


/**********************************************************************
��������    ׼�������ļ�����Բ�ѯ����
�������    szPath----Ŀ¼·��
            fileList----���ڻ��ߵĴ���ѯ�Ķ���ļ��б�
            uFlg----��ѯ��Χ��־���������ֲ�ѯ��Χ������Ŀ¼���ǻ����ļ����ƣ�
                    ȡֵ��QueryRangeType�ﶨ��
            szQueryCond----��ѯ�����ַ���
�������    ��
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼:
------------------------------------------------------------------------
���ⵥ  SWPD05145       add by YanYong 2004-05-25
**********************************************************************/
BOOL CFileGroup::PrepareFiles(const char* szPath, LIST<STRING> &queryList, 
                  UINT4 uFlg, char* szQueryCond)
{
    strcpy(m_szPath, szPath);
    BOOL bSucc = m_pBillFile->SetQueryCond(szQueryCond);
    if (!bSucc)
    {
        return FALSE;
    }

    LIST<STRING> FileList;    //�����������ļ������б�
    if (DATE_RANGE == uFlg)
    {
        LIST<STRING>& DateList = queryList;
        STRING szDateName;

        //����õ�������Ŀ¼�б�Ϊ�գ�˵��Ŀ¼��Ӳ�����Ѿ������ڣ���ʱ����ʧ��
        if (DateList.empty())
        {
            return FALSE;
        }

        //�ļ�����ʽΪ����Ŀ¼/�ļ����ķ�ʽ
        char szFilePath[MAX_PATH];
        LIST<STRING> FileSubList;     //���һ������Ŀ¼�µ������ļ����б�
        LIST<STRING>::iterator i, j;
        for (i = DateList.begin(); i != DateList.end(); i++)
        {
            szDateName = *i;
            sprintf(szFilePath, "%s/%s", m_szPath, szDateName.c_str());
            GetFileList(szFilePath, FileSubList);

            //GetFileList���ص��б���ֻ�����ļ��������Ա��뽫����Ŀ¼��Ϣ����ȥ
            for (j = FileSubList.begin(); j != FileSubList.end(); j++)
            {
                *j = szDateName + "/" + *j;
            }

            //��FileSubList�е����ݼ��뵽FileList��ĩβ
            FileList.splice(FileList.end(), FileSubList);
            FileSubList.clear();
        }
        if (FileList.empty())
        {
            return FALSE;
        }
    }
    else
    {
        //��ȡĿ¼���������ļ�
        FileList = queryList;
        STRING szFileName;
    
        //����õ����ļ��б�Ϊ�գ�˵����ʼ�ļ�����ֹ�ļ�
        //��Ӳ�����Ѿ������ڣ���ʱ����ʧ��
        if (FileList.empty())
        {
            return FALSE;
        }
    }

    //���õ����ļ��б��뵽m_qFileNames��
    LIST<STRING>::iterator i;
    for (i = FileList.begin(); i != FileList.end(); i++)
    {
        m_qFileNames.push_back(*i);
    }

    CacalTotalFileLength();
    queryList.clear(); //����б�.

    return TRUE;    
}
/**********************************************************************
��������    ����һ�������ļ�
�������    ��
�������    bRemain----��ʾ�ļ������Ƿ��Ѿ�û���ļ����Դ��ˣ�����
                       ����ΪFALSEʱ��m_pBillFile��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CFileGroup::OpenNextFile(BOOL& bRemain)
{
    //�����Ѳ�ѯ�ļ����ܳ���
    m_Mutex.acquire();
    m_nQueryedFileLen = m_nQueryedFileLen + m_pBillFile->GetFileLen();
    m_Mutex.release();
    m_pBillFile->SetOffset(0);

    //���ȹرյ�ǰ���ļ�
    BOOL bSucc = m_pBillFile->Close();
    if (!bSucc)
    {
        return FALSE;
    }

    if (m_nCurFileNo == m_qFileNames.size() - 1)
    //�����ǰ�Ѿ����ļ����е����һ���ļ�����ֱ�ӷ���
    {
        bRemain = FALSE;
        return TRUE;
    }
    else
    //�������һ���ļ�����ˢ�µ�ǰλ��
    {
        bRemain = TRUE;
        m_nCurFileNo++;
        m_nCurOffset = 0;

        char szFileName[MAX_PATH], szFileFullPath[MAX_PATH];
        strcpy(szFileName, m_qFileNames[m_nCurFileNo].c_str());
        sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName);

        bSucc = m_pBillFile->Open(szFileFullPath, m_nCurOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

INT8 CFileGroup::GetQueryedLength()
{

    INT8 nQueryFileLen = m_pBillFile->GetCurOffset();

    m_Mutex.acquire();
    nQueryFileLen += m_nQueryedFileLen;
    m_Mutex.release();

    return nQueryFileLen;
}

INT8 CFileGroup::CacalTotalFileLength()
{
    FILE* pFile = NULL;
    INT8  nTotalLen = 0;
    VECTOR<STRING>::iterator it;

    char szFilePath[MAX_PATH];
    it = m_qFileNames.begin();

    while (it != m_qFileNames.end())
    {
        sprintf(szFilePath, "%s/%s", m_szPath, (*it).c_str());
        nTotalLen += FileSize(szFilePath);
        it++;
    }

    m_nTotalFileLength = nTotalLen;

    return nTotalLen;
}

INT8 CFileGroup::GetTotalFileLength()
{
    return m_nTotalFileLength;
}

VECTOR<STRING> CFileGroup::GetFileNames()
{
    return m_qFileNames;
}

STRING CFileGroup::GetFilesPath()
{
    return m_szPath;
}

