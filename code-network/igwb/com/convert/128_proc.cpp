#include "128_proc.h"
#include "proc_chanl.h"

C128Channel::C128Channel(BOOL bProcBillCsn) : CProcChannel()
{
    m_bProcBillCsn = bProcBillCsn;
}

C128Channel::~C128Channel()
{

}

/**********************************************************************
功能描述    处理128话单的流水号
输入参数    pABill----待处理的话单
输出参数    pABill----进行了流水号处理后的话单
返回值      无
抛出异常    无
**********************************************************************/
void C128Channel::ProcessSerialNo(SBill*& pABill)
{
    *(UINT4* )(&pABill->pBillBuf[0]) = m_ChannelSynInfo.uBillCsn;
    m_ChannelSynInfo.uBillCsn++;
}

/**********************************************************************
功能描述    转换一张话单
输入参数    pABill----单张话单对象的指针，根据相应标记判断是否释放 
            pDestFmt----格式转换的工作对象
输出参数    pConvertedBill----转换后的话单对象，结构的内存在函数外申请和释放
返回值      转换是否成功，表示pConvertedBill是否有效
                TRUE----成功
                FALSE----失败
抛出异常    无
**********************************************************************/
BOOL C128Channel::ConvertABill(SBill* pABill, SBill* pConvertedBill, 
                               CFormat* pDestFmt)
{
    if(m_bProcBillCsn)
    {
        //如果是缺省通道的话单，需要恢复增加的序列号。
        if(pDestFmt == NULL)
        {
            SBill stTmpBill;
            stTmpBill.nLen = pABill->nLen - sizeof(UINT4);
            stTmpBill.pBillBuf = pABill->pBillBuf + sizeof(UINT4);
            return CProcChannel::ConvertABill(&stTmpBill, pConvertedBill, pDestFmt);
        }

        ProcessSerialNo(pABill);
    }

    return CProcChannel::ConvertABill(pABill, pConvertedBill, pDestFmt);
}



//128处理类
CBP128::CBP128() : CBillProcess()
{
    m_stTmpBill.nLen = 0;
    m_stTmpBill.pBillBuf = NULL;
}

CBP128::~CBP128()
{
    if(m_stTmpBill.pBillBuf != NULL)
    {
        delete []m_stTmpBill.pBillBuf;
        m_stTmpBill.pBillBuf = NULL;
    }
}

/**********************************************************************
功能描述    初始化函数
输入参数    uAPID----当前接入点的ID号
            bNeedConvert----是否需要格式转换
            bNeedMerge----是否需要合并
输出参数    无
返回值      初始化是否成功 
                TRUE----初始化成功 
                FALSE----初始化失败
抛出异常    无
**********************************************************************/
BOOL CBP128::Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge)
{
    BOOL bSucc = CBillProcess::Init(uAPID, bNeedConvert, bNeedMerge);
    if (!bSucc)
    {
        return FALSE;
    }

    m_bNeedMerge = FALSE;
    m_uOrigLen = m_pIBillProcessor->GetOrigLen();

    if(m_uOrigLen == 0)
    {
        return FALSE;
    }

    if(m_bProcBillCsn)
    {
        m_stTmpBill.pBillBuf = new BYTE[m_uOrigLen];
        if(m_stTmpBill.pBillBuf == NULL)
        {
            return FALSE;
        }
        
        if(m_uOrigLen < sizeof(UINT4))
        {
            return FALSE;
        }
        m_stTmpBill.nLen = m_uOrigLen;
        m_uOrigLen -= sizeof(UINT4);
    }

    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        m_ChannelArray[nChannelNo] = new C128Channel(m_bProcBillCsn);
        m_ChannelArray[nChannelNo]->Init(m_uAPID, nChannelNo, m_bNeedConvert);
    }

    return TRUE;
}

/**********************************************************************
功能描述    话单预处理，将话单包拆分为单张话单
输入参数    pBills----解包当前指针，从此处开始读取话单记录 
            uLeftLen----话单包剩余长度
输出参数    pBills----读取话单记录后的解包当前指针，从此处开始读取话单记录 
            uLeftLen----读取话单记录后的话单包剩余长度
            pABill----从话单包中读出的一条预处理后的话单记录，
                      结构占用的内存在函数外申请和释放
返回值      话单预处理是否成功
                TRUE----预处理成功
                FALSE----预处理失败
抛出异常    无
**********************************************************************/
BOOL CBP128::PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
    BOOL bSucc = GetFixedLenBill(pBills, uLeftLen, pABill);

    if((m_bProcBillCsn) && (bSucc))
    {
        memcpy(m_stTmpBill.pBillBuf + sizeof(UINT4), 
               pABill->pBillBuf,
               pABill->nLen);
       pABill->nLen = m_stTmpBill.nLen;
       pABill->pBillBuf = m_stTmpBill.pBillBuf;
    }
    
    return bSucc;
}

