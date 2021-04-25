#include "resource.h"
#include "qb_result.h"

/**********************************************************************
��������    ���캯��
�������    uDeskNo:����̨��
            nWndNo:���ں�
            pFormat:��ʽ�����
            uPageSize:ҳ��С
            pFileGroup:�ļ������
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
CBillQBResult::CBillQBResult(UINT2 uDeskNo, int nWndNo, ACE_Mutex* pMutex, CFormat *pFormat,
                             UINT4 uPageSize, CFileGroup* pFileGroup, char* pResultPath)
{
    ASSERT(NULL != pFormat);
    ASSERT(NULL != pMutex);
    ASSERT(NULL != pFileGroup);
    ASSERT(NULL != pResultPath);

    //��ʼ����ʱ�ļ����
    char szTmpPath[MAX_PATH];
    strncpy(szTmpPath, pResultPath, MAX_PATH);
    szTmpPath[sizeof(szTmpPath) - 1] = '\0';

    int nEnd = strlen(szTmpPath);
    if(nEnd > 0)
    {
#ifdef _PLATFORM_WIN32
        if( szTmpPath[nEnd-1] != '/' && szTmpPath[nEnd-1] != '\\' )
#else
        if( szTmpPath[nEnd-1] != '/' )
#endif
        {
            strncat(szTmpPath, "/", sizeof(szTmpPath));
        }

        CreateDeepDir(szTmpPath);
    }


    m_strTmpFileName   = szTmpPath;
    sprintf(szTmpPath, "%d%d.tmp", uDeskNo, nWndNo);
    m_strTmpFileName  += szTmpPath;
    m_pTmpFile         = NULL;

    m_uPageSize        = uPageSize;
    m_pFormatMutex     = pMutex;
    m_pFormat          = pFormat;
    m_nPageCount       = 1;

    m_uRecordCount     = 0;
    m_nFileNo          = -1;

    m_pFileGroup       = pFileGroup;
}

/**********************************************************************
��������    ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
CBillQBResult::~CBillQBResult()
{
    //����б�m_qRecordList�Լ��б�������������
    LIST<CBillRecord* >::iterator item;
    for (item = m_pRecordList.begin(); item != m_pRecordList.end(); item++)
    {
        delete *item;
    }

    //�ر���ʱ�ļ����
    if (m_pTmpFile != NULL)
    {
        fclose(m_pTmpFile);
        m_pTmpFile = NULL;
    }
    //ɾ����ʱ�ļ�
    remove(m_strTmpFileName.c_str());
}

/**********************************************************************
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBResult::Init()
{
    m_nTotalFileLength = m_pFileGroup->GetTotalFileLength();
    m_qFileNames = m_pFileGroup->GetFileNames();
    m_szPath = m_pFileGroup->GetFilesPath();
    int nPos = 0;
    while(nPos >= 0)
    {
        nPos = m_szPath.find_first_of('\\');
        if(nPos >= 0)
        {
            m_szPath[nPos] = '/'; 
        }
    }    
}

/**********************************************************************
��������    ����һ��������ƫ����
�������    nStartFileNo:�������ڵ��ļ����
            nStartOffset:���������ļ���ƫ����
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBResult::AddARecordOffset(int nStartFileNo, int nStartOffset)
{
    //��ƫ�������뵽�ڴ���
    ResultOffset itemResultOffset;

    itemResultOffset.nFileNo = nStartFileNo;
    itemResultOffset.nOffset = nStartOffset;

    m_Mutex.acquire();
    m_ResultOffsetList.push_back(itemResultOffset);
    m_uRecordCount ++;
    m_Mutex.release();

}

/**********************************************************************
��������    ����¼�µĻ���ƫ����ȫ�����浽�ļ���
�������    nStartFileNo:�������ڵ��ļ����
            nStartOffset:���������ļ���ƫ����
            nBillLen:��������
�������    ��
����ֵ      �����ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CBillQBResult::SaveToFile()
{
    //����ʱ�ļ�
    m_pTmpFile = fopen(m_strTmpFileName.c_str(), "wb");
    if (!m_pTmpFile)
    {
        return FALSE;
    }

    //��ƫ����д����ʱ�ļ�
    m_Mutex.acquire();
    void* pData = NULL;
    for(int i = 0; i < m_uRecordCount; i++)
    {
        pData = &m_ResultOffsetList[i];
        int nWrite = fwrite(pData, sizeof(ResultOffset), 1, m_pTmpFile);
        if(1 != nWrite)
        {
            m_Mutex.release();
            fclose(m_pTmpFile);
            m_pTmpFile = NULL;
            return FALSE;
        }
    }
    //���ƫ�����б�
    m_ResultOffsetList.clear();
    m_Mutex.release();

    //�ر��ļ�
    fclose(m_pTmpFile);
    m_pTmpFile = NULL;

    //������ܵĻ���ҳ
    int nPageCount = ComputePageCount();
    m_nPageCount   = nPageCount;

    return TRUE;
}

/**********************************************************************
��������    ���㻰��ҳ��
�������    ��
�������    ��
����ֵ      ������Ļ���ҳ��
�׳��쳣    ��
**********************************************************************/
int CBillQBResult::ComputePageCount()
{
    int nPageCount;
    int uRecordCount = GetRecordCount();
    nPageCount = uRecordCount/m_uPageSize;
    int nLeft = uRecordCount % m_uPageSize;
    if(nLeft != 0)
    {
        nPageCount = nPageCount + 1;
    }

    m_nPageCount = nPageCount;

    return nPageCount;
}

/**********************************************************************
��������    �õ�����ҳ��
�������    ��
�������    ��
����ֵ      ����ҳ��
�׳��쳣    ��
**********************************************************************/
int CBillQBResult::GetPageCount()
{
    return m_nPageCount;
}

/**********************************************************************
��������    �õ�������¼����
�������    ��
�������    ��
����ֵ      ������¼��
�׳��쳣    ��
**********************************************************************/
UINT4 CBillQBResult::GetRecordCount()
{
    m_Mutex.acquire();
    UINT4 uRecordCount = m_uRecordCount;
    m_Mutex.release();

    return uRecordCount;
}

/**********************************************************************
��������    ��ò�ѯ������Ľ���
�������    ��
�������    nRecordCount:������¼����
            nPercent:�Ѿ�����İٷֱ�
            nCurPage:��ǰ���ڴ����ҳ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBResult::GetProcess(UINT4& nRecordCount, int& nPercent, int& nCurPage)
{
    INT8 uQueryedLen = m_pFileGroup->GetQueryedLength();
    nPercent = (double)uQueryedLen / m_nTotalFileLength * 100;

    nRecordCount = GetRecordCount();

    nCurPage = ComputePageCount();
    if(nPercent != 100)
    {
        if(nCurPage > 0)
        {
            nCurPage = nCurPage - 1;
        }
    }   
}

/**********************************************************************
��������    ���ָ��ҳ������
�������    nPageNo:����ҳ��
            bRuning:׼������ҳ�߳��Ƿ����еı�־
�������    qResult:������ҳҪ���صĻ�����¼�ĸ�������ַ���������
����ֵ      nResult:�����ӵ��б���ĸ���
�׳��쳣    ��
**********************************************************************/
int CBillQBResult::GetData(int nPageNo, BOOL bRuning, LIST<STRING>& qResult, BOOL bDisplayErrBill)
{

    //�������ҳ�Ŀ�ʼ��¼�Լ���¼����
    int nStartRecord   = (nPageNo - 1) *  m_uPageSize + 1;
    int nCurPageCount  = m_nPageCount;
    UINT4 uRecordCount = GetRecordCount();

    if(nPageNo != nCurPageCount)
    {
        uRecordCount = m_uPageSize;
    }
    else
    {
        uRecordCount = uRecordCount - (nCurPageCount - 1) * m_uPageSize;
    }

    ResultOffset* pResultOffset = new ResultOffset[uRecordCount];

    //������ҳ���м�¼��ƫ����
    BOOL bSucc = FALSE;
    if(bRuning)
    {
        bSucc = GetDataFromMem(nStartRecord, uRecordCount, pResultOffset);
    }
    else
    {
        bSucc = GetDataFromFile(nStartRecord, uRecordCount, pResultOffset);
    }
    if(!bSucc)
    {
        delete []pResultOffset;
        return 0;
    }


    //ת��Ϊ�����ʽ
    BYTE* pBillData = NULL;

    LIST<STRING> BillContentList;
    int nFileNo, nOffset, nFieldCount, nResult = 0;
    UINT4 uBillLen;
    for (int i = 0; i < uRecordCount; i++)
    {
        CBillRecord record;
        //�õ�ÿ����¼��ƫ����
        nFileNo = pResultOffset[i].nFileNo;
        nOffset = pResultOffset[i].nOffset;

        pBillData = NULL;
        uBillLen  = 0;
        int nRet = m_pFileGroup->GetRstRecord(nFileNo, nOffset, pBillData, uBillLen);
        //ֻ������Ч����ʱ����ʧ�ܣ��������ʧ�ܣ��᷵�سɹ�����pBillData=NULL��uBillLen=0
        if(ERR_SUCCESS != nRet)
        {
            continue;
        }

        //���ݻ�����¼ת��Ϊ�ı�����
        bSucc = record.SetContent(pBillData, uBillLen, m_pFormat, m_pFormatMutex);
        if(!bSucc)
        {
            //����ʾ���󻰵���Ϊ�������ʱ��ʾΪ���󻰵�
            if(bDisplayErrBill)
            {
                //���ô��󻰵�ʱ�϶��ɹ�
                bSucc = record.SetContent(NULL, 0, m_pFormat, m_pFormatMutex);
                if(!bSucc)
                {
                    continue;  //by lx SWPD04234
                }
            }
            else
            {
                continue;
            }
        }

        //����������¼�����ݼ��뵽�б�qResult��ȥ
        nFieldCount = record.GetContent(BillContentList);
        STRING szFileName;
        szFileName = m_szPath + '/' + m_qFileNames[nFileNo];        
        BillContentList.push_back(szFileName);       //��ӻ������ڵ��ļ���
        
        qResult.splice(qResult.end(), BillContentList);
        BillContentList.clear();
        nResult ++;
    }

    delete []pResultOffset;

    return nResult;
}

/**********************************************************************
��������    ���ļ��л�û�����¼��ƫ����
�������    nStartRecord:��ʼ��¼��
            nRecordCount:��ҳ�ļ�¼����
�������    pResultOffset:������ƫ����
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CBillQBResult::GetDataFromFile(int nStartRecord, int nRecordCount, ResultOffset*& pResultOffset)
{
    //����ʽ����ʱ�ļ�
    if(m_pTmpFile == NULL)
    {
        m_pTmpFile = fopen((char*)(m_strTmpFileName).c_str(), "rb");
    }

    if(m_pTmpFile == NULL)
    {
        return FALSE;
    }

    if (0 != fseek(m_pTmpFile, (nStartRecord - 1) * sizeof(ResultOffset), SEEK_SET))
    {
        fclose(m_pTmpFile);
        m_pTmpFile = NULL;

        return  FALSE;
    }

    int nReadRecordCount = fread(pResultOffset, sizeof(ResultOffset), nRecordCount, m_pTmpFile);

    if(nReadRecordCount != nRecordCount)
    {

        return FALSE;
    }

    return TRUE;
}

/**********************************************************************
��������    ���ڴ��л�û�����¼��ƫ����
�������    nStartRecord:��ʼ��¼��
            nRecordCount:��ҳ�ļ�¼����
�������    pResultOffset:������ƫ����
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CBillQBResult::GetDataFromMem(int nStartRecord, int nRecordCount, ResultOffset*& pResultOffset)
{
    if(nStartRecord < 0 || nRecordCount < 0 || pResultOffset == NULL)
    {
        return FALSE;
    }

    m_Mutex.acquire();
    if(&(m_ResultOffsetList[nStartRecord + nRecordCount - 1]) > m_ResultOffsetList.end())
    {
        m_Mutex.release();
        return FALSE;

    }

    memcpy(pResultOffset, &(m_ResultOffsetList[nStartRecord - 1]), nRecordCount * sizeof(ResultOffset));
    m_Mutex.release();

    return TRUE;
}
