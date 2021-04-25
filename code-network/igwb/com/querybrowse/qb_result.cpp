#include "resource.h"
#include "qb_result.h"

/**********************************************************************
功能描述    构造函数
输入参数    uDeskNo:工作台号
            nWndNo:窗口号
            pFormat:格式库对象
            uPageSize:页大小
            pFileGroup:文件组对象
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
CBillQBResult::CBillQBResult(UINT2 uDeskNo, int nWndNo, ACE_Mutex* pMutex, CFormat *pFormat,
                             UINT4 uPageSize, CFileGroup* pFileGroup, char* pResultPath)
{
    ASSERT(NULL != pFormat);
    ASSERT(NULL != pMutex);
    ASSERT(NULL != pFileGroup);
    ASSERT(NULL != pResultPath);

    //初始化临时文件句柄
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
功能描述    析构函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
CBillQBResult::~CBillQBResult()
{
    //清空列表m_qRecordList以及列表中所带的内容
    LIST<CBillRecord* >::iterator item;
    for (item = m_pRecordList.begin(); item != m_pRecordList.end(); item++)
    {
        delete *item;
    }

    //关闭临时文件句柄
    if (m_pTmpFile != NULL)
    {
        fclose(m_pTmpFile);
        m_pTmpFile = NULL;
    }
    //删除临时文件
    remove(m_strTmpFileName.c_str());
}

/**********************************************************************
功能描述    初始化函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
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
功能描述    增加一条话单的偏移量
输入参数    nStartFileNo:话单所在的文件序号
            nStartOffset:话单所在文件的偏移量
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillQBResult::AddARecordOffset(int nStartFileNo, int nStartOffset)
{
    //将偏移量加入到内存中
    ResultOffset itemResultOffset;

    itemResultOffset.nFileNo = nStartFileNo;
    itemResultOffset.nOffset = nStartOffset;

    m_Mutex.acquire();
    m_ResultOffsetList.push_back(itemResultOffset);
    m_uRecordCount ++;
    m_Mutex.release();

}

/**********************************************************************
功能描述    将记录下的话单偏移量全部保存到文件中
输入参数    nStartFileNo:话单所在的文件序号
            nStartOffset:话单所在文件的偏移量
            nBillLen:话单长度
输出参数    无
返回值      操作成功与否
抛出异常    无
**********************************************************************/
BOOL CBillQBResult::SaveToFile()
{
    //打开临时文件
    m_pTmpFile = fopen(m_strTmpFileName.c_str(), "wb");
    if (!m_pTmpFile)
    {
        return FALSE;
    }

    //将偏移量写入临时文件
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
    //清空偏移量列表
    m_ResultOffsetList.clear();
    m_Mutex.release();

    //关闭文件
    fclose(m_pTmpFile);
    m_pTmpFile = NULL;

    //计算出总的话单页
    int nPageCount = ComputePageCount();
    m_nPageCount   = nPageCount;

    return TRUE;
}

/**********************************************************************
功能描述    计算话单页数
输入参数    无
输出参数    无
返回值      计算出的话单页数
抛出异常    无
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
功能描述    得到话单页数
输入参数    无
输出参数    无
返回值      话单页数
抛出异常    无
**********************************************************************/
int CBillQBResult::GetPageCount()
{
    return m_nPageCount;
}

/**********************************************************************
功能描述    得到话单记录条数
输入参数    无
输出参数    无
返回值      话单记录数
抛出异常    无
**********************************************************************/
UINT4 CBillQBResult::GetRecordCount()
{
    m_Mutex.acquire();
    UINT4 uRecordCount = m_uRecordCount;
    m_Mutex.release();

    return uRecordCount;
}

/**********************************************************************
功能描述    获得查询或浏览的进度
输入参数    无
输出参数    nRecordCount:话单记录条数
            nPercent:已经处理的百分比
            nCurPage:当前正在处理的页号
返回值      无
抛出异常    无
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
功能描述    获得指定页的数据
输入参数    nPageNo:话单页号
            bRuning:准备话单页线程是否运行的标志
输出参数    qResult:包含本页要返回的话单记录的各个域的字符串链表无
返回值      nResult:新增加的列表项的个数
抛出异常    无
**********************************************************************/
int CBillQBResult::GetData(int nPageNo, BOOL bRuning, LIST<STRING>& qResult, BOOL bDisplayErrBill)
{

    //计算出该页的开始记录以及记录总数
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

    //读出该页所有记录的偏移量
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


    //转换为浏览格式
    BYTE* pBillData = NULL;

    LIST<STRING> BillContentList;
    int nFileNo, nOffset, nFieldCount, nResult = 0;
    UINT4 uBillLen;
    for (int i = 0; i < uRecordCount; i++)
    {
        CBillRecord record;
        //得到每条记录的偏移量
        nFileNo = pResultOffset[i].nFileNo;
        nOffset = pResultOffset[i].nOffset;

        pBillData = NULL;
        uBillLen  = 0;
        int nRet = m_pFileGroup->GetRstRecord(nFileNo, nOffset, pBillData, uBillLen);
        //只在无有效数据时返回失败，如果解码失败，会返回成功，但pBillData=NULL且uBillLen=0
        if(ERR_SUCCESS != nRet)
        {
            continue;
        }

        //根据话单记录转换为文本描述
        bSucc = record.SetContent(pBillData, uBillLen, m_pFormat, m_pFormatMutex);
        if(!bSucc)
        {
            //在显示错误话单且为浏览任务时显示为错误话单
            if(bDisplayErrBill)
            {
                //设置错误话单时肯定成功
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

        //将本话单记录的内容加入到列表qResult中去
        nFieldCount = record.GetContent(BillContentList);
        STRING szFileName;
        szFileName = m_szPath + '/' + m_qFileNames[nFileNo];        
        BillContentList.push_back(szFileName);       //添加话单所在的文件名
        
        qResult.splice(qResult.end(), BillContentList);
        BillContentList.clear();
        nResult ++;
    }

    delete []pResultOffset;

    return nResult;
}

/**********************************************************************
功能描述    从文件中获得话单记录的偏移量
输入参数    nStartRecord:起始记录号
            nRecordCount:本页的记录条数
输出参数    pResultOffset:读出的偏移量
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CBillQBResult::GetDataFromFile(int nStartRecord, int nRecordCount, ResultOffset*& pResultOffset)
{
    //读方式打开临时文件
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
功能描述    从内存中获得话单记录的偏移量
输入参数    nStartRecord:起始记录号
            nRecordCount:本页的记录条数
输出参数    pResultOffset:读出的偏移量
返回值      成功与否
抛出异常    无
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
