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
功能描述    打开指定的话单文件和设置相应的读写位置指针函数
输入参数    szPathFile----话单文件名称（包含全路径）
            nOffset----读写位置偏移量
输出参数    无
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CBillFile::Open(const char* szPathFile, int nOffset)
{
    return m_pIGWBFile->Open(szPathFile, nOffset);
}

/**********************************************************************
功能描述    打开指定的话单文件和设置相应的读写位置指针函数
输入参数    szPathFile----话单文件名称（包含全路径）
            nOffset----读写位置偏移量
输出参数    无
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CBillFile::OpenRstFile(const char* szPathFile, int nOffset)
{
    return m_pRstIGWBFile->Open(szPathFile, nOffset);
}

/**********************************************************************
功能描述    获取下一张符合条件的话单记录的偏移地址
输入参数    无
输出参数    nCurOffset----找到一张话单记录后的当前在话单文件中的位置
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pBillRecord的值无效
返回值      成功与否
抛出异常    无
修改记录
    2002.3.30   颜文远      修改代码，在话单内容错误时设置错误信息
**********************************************************************/
BOOL CBillFile::GetNextBillOffset(int& nCurOffset, BOOL& bRemain)
{
    void* pBillData = NULL;
    UINT4 uDataLen  = 0;

    BOOL bSucc = TRUE;
    //如果读出的一条话单记录满足要求，则返回该话单记录，
    //否则继续读取下一条话单，直到文件结束或找到满足条件的话单
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

        if (NULL == pBillData)  //话单内容错误
        {
            //在显示错误话单且为浏览任务时显示为错误话单
            if (m_bDisplayErrBill)
            {
                bRemain = TRUE;
                return TRUE;
            }

            continue;
        }

        //判断话单记录是否满足查询条件，以决定返回该记录还是继续判断下一条话单
        m_pMutex->acquire();
        int nResult = m_pFormat->IsTheFormat(pBillData, uDataLen);
        m_pMutex->release();

        if (0 == nResult)   //话单记录符合当前浏览格式
        {
            if ('\0' != m_szQueryCond[0]) //是查询任务
            {
                m_pMutex->acquire();
                nResult = m_pFormat->IsTheQueryCondition(pBillData);
                m_pMutex->release();

                if ((0 == nResult) && IsMatchedBill(pBillData, uDataLen))
                //该话单记录满足查询条件，则返回本话单记录
                {
                    return TRUE;
                }
            }
            else    //是浏览任务
            {
                return TRUE;
            }
        }
    }
}

/**********************************************************************
功能描述    获取指定文件指定位置的话单记录数据
输入参数    nStartOffset----本张话单记录数据在话单文件中的位置
输出参数    pBillRecord ----话单记录对象指针
            nBillLen    ----输出的话单长度
返回值      ERR_SUCCESS ----成功
抛出异常    无
**********************************************************************/
int CBillFile::GetRstRecord(int nStartOffset,
                            BYTE*& pBillRecord,
                            UINT4& uBillLen)
{
    m_pRstIGWBFile->SetOffset(nStartOffset);

    BOOL bRemainInFile = TRUE;
    //如果读出的一条话单记录满足要求，则返回该话单记录，
    //否则继续读取下一条话单，直到文件结束或找到满足条件的话单
    BOOL bSucc = m_pRstIGWBFile->Read((void*&)pBillRecord, uBillLen, bRemainInFile);

    //只在无有效数据时返回失败
    if (!bRemainInFile)
    {
        return ERR_FAIL;
    }

    //在读取成功，或解码失败时返回成功（此时pBillRecord=NULL，且uBillLen==0）
    return ERR_SUCCESS;
}

//获取文件的当前读写位置函数
int CBillFile::GetCurOffset()
{
    return m_pIGWBFile->GetCurOffset();
}

int CBillFile::GetFileLen()
{
    return m_pIGWBFile->GetFileLen();
}

//设置文件的读写指针位置函数
BOOL CBillFile::SetOffset(int nOffset)
{
    return m_pIGWBFile->SetOffset(nOffset);
}

//设置查询条件字符串函数
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

//关闭文件函数
BOOL CBillFile::Close()
{
    return m_pIGWBFile->Close();
}


//设置是否合并标志
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

//设置是否显示错误的话单
void CBillFile::SetDisplayErrBill(BOOL bDisplayErrBill)
{
    m_bDisplayErrBill = bDisplayErrBill;
}

