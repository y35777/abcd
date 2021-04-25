#include "stp_proc.h"

CSTPChannel::CSTPChannel() : CProcChannel()
{

}

CSTPChannel::~CSTPChannel()
{

}

/**********************************************************************
功能描述    处理STP话单的流水号
输入参数    pABill----待处理的话单
输出参数    pABill----进行了流水号处理后的话单
返回值      无
抛出异常    无
**********************************************************************/
void CSTPChannel::ProcessSerialNo(SBill* &pABill)
{
    *(UINT4* )(&pABill->pBillBuf[14]) = m_ChannelSynInfo.uBillCsn;
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
BOOL CSTPChannel::ConvertABill(SBill* pABill, SBill* pConvertedBill,
                               CFormat* pDestFmt)
{
    ProcessSerialNo(pABill);
    return CProcChannel::ConvertABill(pABill, pConvertedBill, pDestFmt);
}


CBPSTP::CBPSTP() : CBillProcess()
{

}

CBPSTP::~CBPSTP()
{

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
BOOL CBPSTP::Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge)
{
    BOOL bSucc = CBillProcess::Init(uAPID, bNeedConvert, bNeedMerge);
    if (!bSucc)
    {
        return FALSE;
    }

    m_uOrigLen = m_pIBillProcessor->GetOrigLen();

    if (m_bNeedMerge)
    {
        m_pMerge = new CMerger();
//		m_pParser = new CSTP_BillParser();
    }

    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        m_ChannelArray[nChannelNo] = new CSTPChannel();
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
BOOL CBPSTP::PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
    return GetFixedLenBill(pBills, uLeftLen, pABill);
}
