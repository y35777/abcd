#include "qb_task.h"
#include "../include/toolbox.h"

//初始化CBillTask类中的静态成员变量，这些变量从INI文件中读取
UINT4 CBillQBTask::m_uPageSize;

//构造函数
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

//设置一页包含的话单记录数据条数
void CBillQBTask::SetPageSize(UINT4 uPageSize)
{
    m_uPageSize = uPageSize;
}

//线程是否正在运行
BOOL CBillQBTask::IsRunning()
{
    return m_bRunning;
}

//得到本线程的错误信息
const char* CBillQBTask::GetErrInfo()
{
    return m_szErrInfo;
}

/**********************************************************************
功能描述    初始化函数，本函数针对浏览任务对象
输入参数    szPathFile----包含全路径的话单文件名称
            uAPType----接入点类型，用于判断话单是否需要ASN.1解码
输出参数    无
返回值      初始化是否成功
抛出异常    无
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
功能描述    初始化函数，本函数针对浏览任务对象
输入参数    szPathFile----包含全路径的话单文件名称
            uAPType----接入点类型，用于判断话单是否需要ASN.1解码
            queryList---查询时选择的文件或者文件夹列表
输出参数    无
返回值      初始化是否成功
抛出异常    无
修改日志：
-----------------------------------------------------------------------
问题单  SWPD05145  add by YanYong 2004-05-25
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
功能描述    初始化函数，本函数针对查询任务对象
输入参数    szPath----查询范围的全路径
            szStartRange----查询范围开始，取值可能是日期、或话单文件名
            szEndRange----查询范围结束，取值可能是日期、或话单文件名
            uFlg----用于区分查询范围是日期目录还是话单文件名称
                    在QueryRangeType中定义
            szQueryCond----查询条件字符串
            uAPType----接入点类型，用于判断话单是否需要ASN.1解码
输出参数    无
返回值      初始化是否成功
抛出异常    无
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
功能描述    准备话单数据页线程启动函数
输入参数    无
输出参数    无
返回值      启动线程是否成功
抛出异常    无
**********************************************************************/
BOOL CBillQBTask::Run()
{
    m_bRunning = TRUE;

    int nResult = m_tThreadMgr.spawn((ACE_THR_FUNC)ThreadProc, this);

    return (nResult != -1);
}

/**********************************************************************
功能描述    准备话单数据页线程入口函数
输入参数    pBillQBTask----查询浏览任务对象指针，在调用时一般传入this指针
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBTask::ThreadProc(CBillQBTask *pBillQBTask)
{
    pBillQBTask->PrepareBillPages();
}

/**********************************************************************
功能描述    准备话单页线程函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
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

    //从文件组里获取话单偏移量，直到任务被结束或已没有话单数据为止
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

    //将话单记录的偏移量存入文件
    m_pResult->SaveToFile();

    m_bRunning = FALSE;
}

/**********************************************************************
功能描述    获得话单表格的标题的列表
输入参数    无
输出参数    TitleList----话单表格的标题的列表
返回值      无
抛出异常    无
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
            //在MML报告中话单域的标题不能为空，用"-"代替
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


//该对象是否匹配指定的查询浏览任务
BOOL CBillQBTask::IsMatched(UINT2 uDeskNo, int nWndNo)
{
    return ((uDeskNo == m_uDeskNo) && (nWndNo == m_nWndNo));
}

//等待准备话单线程结束函数
int CBillQBTask::WaitThreadExit()
{
    return m_tThreadMgr.wait();
}

//设置停止标志为停止状态函数
void CBillQBTask::Stop()
{
    m_bRunning = FALSE;
}

//超时标志增加一函数
void CBillQBTask::IncTimeOutFlg()
{
    m_uTimeOut++;
}

//超时标志置零函数
void CBillQBTask::RsetTimeOutFlg()
{
    m_uTimeOut = 0;
}

//是否超时函数
BOOL CBillQBTask::IsTimeOut(UINT4 uTimeOutLimit)
{
    return (m_uTimeOut > uTimeOutLimit);
}


//判断一个请求页号是否合法
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

//获得查询或浏览的进度
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

//获得指定页的数据
int CBillQBTask::GetPageData(int nPageNo, LIST<STRING>& qResult)
{
    if(IsValidPage(nPageNo))
    {
        return m_pResult->GetData(nPageNo, m_bRunning, qResult, m_bDisplayErrBill);
    }

    return 0;
}

//设置是否显示错误的话单
void CBillQBTask::SetDisplayErrBill(BOOL bDisplayErrBill)
{
    m_bDisplayErrBill = bDisplayErrBill;
}

