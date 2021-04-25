#include "qb_task.h"
#include "../include/toolbox.h"

//��ʼ��CBillTask���еľ�̬��Ա��������Щ������INI�ļ��ж�ȡ
UINT4 CBillQBTask::m_uPageSize;

//���캯��
CBillQBTask::CBillQBTask(UINT2 uDeskNo, int nWndNo, UINT4 uPageNo,
                         char* szBillFormat, UINT4 uBillType, char* szResultPath)
{
    m_uDeskNo       = uDeskNo;
    m_nWndNo        = nWndNo;
    m_uBillType     = uBillType;
    STRNCPY(m_szFormatName, szBillFormat, sizeof(m_szFormatName));

    m_uTimeOut      = 0;
    m_bRunning      = FALSE;

    m_pFormatObj    = NULL;
    m_pIQueryBrowse = NULL;
    m_pFormat       = NULL;
    m_pFileGroup    = NULL;
    m_pResult       = NULL;
    m_bDisplayErrBill = FALSE;
    
    strncpy(m_pResultPath, szResultPath, MAX_PATH);
    m_pResultPath[sizeof(m_pResultPath) - 1] = '\0';
}

CBillQBTask::~CBillQBTask()
{
    if (NULL != m_pFileGroup)
    {
        delete m_pFileGroup;
        m_pFileGroup = NULL;
    }
    if (NULL != m_pFormatObj)
    {
        delete m_pFormatObj;
        m_pFormatObj = NULL;
    }

    if (NULL != m_pResult)
    {
        delete m_pResult;
        m_pResult = NULL;
    }
}

void CBillQBTask::SetMustMerge(BOOL bMustMerge)
{
    if(NULL != m_pFileGroup)
    {
        m_pFileGroup->SetMustMerge(bMustMerge);
    }
}

void CBillQBTask::SetFinalSPFileFmt(BOOL bFinalSPFileFmt)
{
    if(NULL != m_pFileGroup)
    {
        m_pFileGroup->SetFinalSPFileFmt(bFinalSPFileFmt);
    }
}

//����һҳ�����Ļ�����¼��������
void CBillQBTask::SetPageSize(UINT4 uPageSize)
{
    m_uPageSize = uPageSize;
}

//�߳��Ƿ���������
BOOL CBillQBTask::IsRunning()
{
    return m_bRunning;
}

//�õ����̵߳Ĵ�����Ϣ
const char* CBillQBTask::GetErrInfo()
{
    return m_szErrInfo;
}

/**********************************************************************
��������    ��ʼ���������������������������
�������    szPathFile----����ȫ·���Ļ����ļ�����
            uAPType----��������ͣ������жϻ����Ƿ���ҪASN.1����
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ�
�׳��쳣    ��
**********************************************************************/
BOOL CBillQBTask::Init(char* szPathFile, UINT4 uAPType, int nAPID)
{
    m_uObjType = BROWSE_TASK;

    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/%s/%s", GetAppPath(),
            "config", "format");
    m_pFormatObj = new CIGWB(szIGWBPath);
    m_pIQueryBrowse
        = (IQueryBrowse* )m_pFormatObj->QueryInterface(IID_FP_IQUERYBROWSE);
    if (NULL == m_pIQueryBrowse)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }
    int nResult = m_pIQueryBrowse->Init(0);
    if (0 != nResult)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }
    m_pFormat = m_pIQueryBrowse->GetBrowseFmt(m_szFormatName);
    if (NULL == m_pFormat)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }

    m_uAPType = uAPType;
    m_pFileGroup = new CFileGroup(m_pFormat, m_uBillType, uAPType, &m_Mutex, nAPID);
    m_pFileGroup->SetDisplayErrBill(m_bDisplayErrBill);
    BOOL bSucc = m_pFileGroup->PrepareFile(szPathFile);
    if (!bSucc)
    {
        return FALSE;
    }

    m_pResult = new CBillQBResult(m_uDeskNo, m_nWndNo, &m_Mutex, m_pFormat,
                                  m_uPageSize, m_pFileGroup, m_pResultPath);
    m_pResult->Init();

    return TRUE;
}

/**********************************************************************
��������    ��ʼ���������������������������
�������    szPathFile----����ȫ·���Ļ����ļ�����
            uAPType----��������ͣ������жϻ����Ƿ���ҪASN.1����
            queryList---��ѯʱѡ����ļ������ļ����б�
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ�
�׳��쳣    ��
�޸���־��
-----------------------------------------------------------------------
���ⵥ  SWPD05145  add by YanYong 2004-05-25
**********************************************************************/
BOOL CBillQBTask::Init(char* szPath, LIST<STRING> & queryList,
          UINT4 uFlg, char* szQueryCond, UINT4 uAPType, int nAPID)
{
    m_uObjType = QUERY_TASK;
    
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/%s/%s", GetAppPath(),
        "config", "format");
    m_pFormatObj = new CIGWB(szIGWBPath);
    m_pIQueryBrowse
        = (IQueryBrowse* )m_pFormatObj->QueryInterface(IID_FP_IQUERYBROWSE);
    if (NULL == m_pIQueryBrowse)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }
    int nResult = m_pIQueryBrowse->Init(0);
    if (0 != nResult)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }
    m_pFormat = m_pIQueryBrowse->GetBrowseFmt(m_szFormatName);
    if (NULL == m_pFormat)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }
    
    m_uAPType = uAPType;
    m_pFileGroup = new CFileGroup(m_pFormat, m_uBillType, uAPType, &m_Mutex, nAPID);
    m_pFileGroup->SetDisplayErrBill(m_bDisplayErrBill);
    BOOL bSucc = m_pFileGroup->PrepareFiles(szPath, queryList,
        uFlg, szQueryCond);
    if (!bSucc)
    {
        return FALSE;
    }
    
    m_pResult = new CBillQBResult(m_uDeskNo, m_nWndNo, &m_Mutex, m_pFormat,
        m_uPageSize, m_pFileGroup, m_pResultPath);
    m_pResult->Init();
    
    return TRUE;
    
}
/**********************************************************************
��������    ��ʼ����������������Բ�ѯ�������
�������    szPath----��ѯ��Χ��ȫ·��
            szStartRange----��ѯ��Χ��ʼ��ȡֵ���������ڡ��򻰵��ļ���
            szEndRange----��ѯ��Χ������ȡֵ���������ڡ��򻰵��ļ���
            uFlg----�������ֲ�ѯ��Χ������Ŀ¼���ǻ����ļ�����
                    ��QueryRangeType�ж���
            szQueryCond----��ѯ�����ַ���
            uAPType----��������ͣ������жϻ����Ƿ���ҪASN.1����
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ�
�׳��쳣    ��
**********************************************************************/
BOOL CBillQBTask::Init(char* szPath, char* szStartRange, char* szEndRange,
                       UINT4 uFlg, char* szQueryCond, UINT4 uAPType, int nAPID)
{
    m_uObjType = QUERY_TASK;

    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/%s/%s", GetAppPath(),
            "config", "format");
    m_pFormatObj = new CIGWB(szIGWBPath);
    m_pIQueryBrowse
        = (IQueryBrowse* )m_pFormatObj->QueryInterface(IID_FP_IQUERYBROWSE);
    if (NULL == m_pIQueryBrowse)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }
    int nResult = m_pIQueryBrowse->Init(0);
    if (0 != nResult)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }
    m_pFormat = m_pIQueryBrowse->GetBrowseFmt(m_szFormatName);
    if (NULL == m_pFormat)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_INIT_FORMAT);
        return FALSE;
    }

    m_uAPType = uAPType;
    m_pFileGroup = new CFileGroup(m_pFormat, m_uBillType, uAPType, &m_Mutex, nAPID);
    m_pFileGroup->SetDisplayErrBill(m_bDisplayErrBill);
    BOOL bSucc = m_pFileGroup->PrepareFiles(szPath, szStartRange, szEndRange,
                                            uFlg, szQueryCond);
    if (!bSucc)
    {
        return FALSE;
    }

    m_pResult = new CBillQBResult(m_uDeskNo, m_nWndNo, &m_Mutex, m_pFormat,
                                  m_uPageSize, m_pFileGroup, m_pResultPath);
    m_pResult->Init();


    return TRUE;
}

/**********************************************************************
��������    ׼����������ҳ�߳���������
�������    ��
�������    ��
����ֵ      �����߳��Ƿ�ɹ�
�׳��쳣    ��
**********************************************************************/
BOOL CBillQBTask::Run()
{
    m_bRunning = TRUE;

    int nResult = m_tThreadMgr.spawn((ACE_THR_FUNC)ThreadProc, this);

    return (nResult != -1);
}

/**********************************************************************
��������    ׼����������ҳ�߳���ں���
�������    pBillQBTask----��ѯ����������ָ�룬�ڵ���ʱһ�㴫��thisָ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBTask::ThreadProc(CBillQBTask *pBillQBTask)
{
    pBillQBTask->PrepareBillPages();
}

/**********************************************************************
��������    ׼������ҳ�̺߳���
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBTask::PrepareBillPages()
{
    BOOL bSucc = m_pFileGroup->Open(0, 0);
    if (!bSucc)
    {
        TRACE(MTS_BILLBROWSER, S_QB_TRC_FAIL_OPEN_FILE_00);
        strcpy(m_szErrInfo, S_QB_RI_FAIL_OPEN_FILE);
        /*m_bRunning = FALSE;
        return;*/ 
        //by lx SWPD04804

    }

    int nStartFileNo, nStartOffset;
    BOOL bRemain = TRUE;

    //���ļ������ȡ����ƫ������ֱ�����񱻽�������û�л�������Ϊֹ
    while(m_bRunning)
    {
        bSucc = m_pFileGroup->GetNextRecordOffset(nStartFileNo, nStartOffset, bRemain);
        if (!bSucc)
        {
            return;
        }
        else if(!bRemain)
        {
            break;
        }

        m_pResult->AddARecordOffset(nStartFileNo, nStartOffset);

    }

    //��������¼��ƫ���������ļ�
    m_pResult->SaveToFile();

    m_bRunning = FALSE;
}

/**********************************************************************
��������    ��û������ı�����б�
�������    ��
�������    TitleList----�������ı�����б�
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillQBTask::GetBillTitleList(LIST<STRING>& TitleList)
{
    if (NULL != m_pFormat)
    {
        int nFieldCount = m_pFormat->GetFieldNum();
        const SFieldInfo** FieldArray = m_pFormat->GetFieldsArray();
        for (int i = 0; i < nFieldCount; i++)
        {
            if (0 == strlen(FieldArray[i]->szFieldDescribe))
            //��MML�����л�����ı��ⲻ��Ϊ�գ���"-"����
            {
                TitleList.push_back("-");
            }
            else
            {
                TitleList.push_back(FieldArray[i]->szFieldDescribe);
            }
        }
    }
}


//�ö����Ƿ�ƥ��ָ���Ĳ�ѯ�������
BOOL CBillQBTask::IsMatched(UINT2 uDeskNo, int nWndNo)
{
    return ((uDeskNo == m_uDeskNo) && (nWndNo == m_nWndNo));
}

//�ȴ�׼�������߳̽�������
int CBillQBTask::WaitThreadExit()
{
    return m_tThreadMgr.wait();
}

//����ֹͣ��־Ϊֹͣ״̬����
void CBillQBTask::Stop()
{
    m_bRunning = FALSE;
}

//��ʱ��־����һ����
void CBillQBTask::IncTimeOutFlg()
{
    m_uTimeOut++;
}

//��ʱ��־���㺯��
void CBillQBTask::RsetTimeOutFlg()
{
    m_uTimeOut = 0;
}

//�Ƿ�ʱ����
BOOL CBillQBTask::IsTimeOut(UINT4 uTimeOutLimit)
{
    return (m_uTimeOut > uTimeOutLimit);
}


//�ж�һ������ҳ���Ƿ�Ϸ�
BOOL CBillQBTask::IsValidPage(int nPageNo)
{
    if(m_bRunning)
    {
        if((nPageNo >= m_pResult->GetPageCount()) || (nPageNo < 1))
        {
            return FALSE;
        }
    }
    else
    {
        if((nPageNo > m_pResult->GetPageCount()) || (nPageNo < 1))
        {
            return FALSE;
        }
    }

    return TRUE;
}

//��ò�ѯ������Ľ���
BOOL CBillQBTask::GetProcess(UINT4& nRecordCount, int& nPercent, int& nCurPage)
{
    if(NULL != m_pResult)
    {
        m_pResult->GetProcess(nRecordCount, nPercent, nCurPage);
        return TRUE;
    }
    else
    {
        return  FALSE;
    }

}

//���ָ��ҳ������
int CBillQBTask::GetPageData(int nPageNo, LIST<STRING>& qResult)
{
    if(IsValidPage(nPageNo))
    {
        return m_pResult->GetData(nPageNo, m_bRunning, qResult, m_bDisplayErrBill);
    }

    return 0;
}

//�����Ƿ���ʾ����Ļ���
void CBillQBTask::SetDisplayErrBill(BOOL bDisplayErrBill)
{
    m_bDisplayErrBill = bDisplayErrBill;
}

