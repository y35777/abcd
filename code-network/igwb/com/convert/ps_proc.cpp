#include "ps_proc.h"
#include "proc_chanl.h"
#include "convert_comm.h"
#include "resource.h"
#include "../include/toolbox.h"
#include "../include/frame.h"

CPSChannel::CPSChannel(PACKET_SWITCH_RELEASE nPSRelease) : CProcChannel()
{
    m_pASN1RecBuffer    = NULL;
    m_pASN1FinalGlobal  = NULL;
    m_nPSRelease        = nPSRelease;
}

CPSChannel::~CPSChannel()
{
    ReleaseOSSLib();
}

/**********************************************************************
功能描述    根据通道的配置参数，进行初始化
输入参数    uAPID----当前接入点的ID号
            nChannelID----当前的通道号
            bNeedConvert----是否需要格式转换
输出参数    无
返回值      初始化是否成功
                TRUE----初始化成功
                FALSE----初始化失败
抛出异常    无
**********************************************************************/
BOOL CPSChannel::Init(UINT4 uAPID, INT4 nChannelID, BOOL bNeedConvert)
{
    BOOL bRet = CProcChannel::Init(uAPID, nChannelID, bNeedConvert);
    if(!bRet)
    {
        return bRet;
    }

    int nRet = m_PSAsnClosor.Init(uAPID);
    if(ERR_SUCCESS != nRet)
    {
        return FALSE;
    }

    InitOSSLib();

    return TRUE;
}

void CPSChannel::InitOSSLib(void)
{
    m_pASN1FinalGlobal = m_PSAsnClosor.Init_OSS_Lib_Final();
}

void CPSChannel::ReleaseOSSLib(void)
{
    if (m_pASN1FinalGlobal)
    {
        m_PSAsnClosor.Release_OSS_Lib_Final(m_pASN1FinalGlobal);
        m_pASN1FinalGlobal = NULL;
    }
}


//设置和ASN1相关的参数
//备注：为了节省空间，GPRS的编解码公用同一块内存
void CPSChannel::SetASN1Para(BYTE* pBuffer)
{
    m_pASN1RecBuffer = pBuffer;
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
BOOL CPSChannel::ConvertABill(SBill* pABill, SBill* pConvertedBill,
                                CFormat* pDestFmt)
{
    BYTE* pBillBuffer = NULL;
    int nLen;

    BOOL bSucc = EncodeABill(pABill, pBillBuffer, nLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //GPRS的后存盘中话单采用4位长度＋话单数据的方式存放，
    //考虑到UNIX下的字节对齐问题，所以直接对话单长度的4个字节进行赋值
    pConvertedBill->nLen = nLen + 4;
    pConvertedBill->pBillBuf = new BYTE[nLen + 4];
    INT4 nNetLen = HTONL(nLen);
    memcpy(pConvertedBill->pBillBuf, &nNetLen, 4);
    memcpy(pConvertedBill->pBillBuf + 4, pBillBuffer, nLen);

    return TRUE;
}

BOOL CPSChannel::EncodeABill(const SBill* pABill, BYTE*& pBillBuffer, int& nLen)
{
    nLen = ASN1_BUF_SIZE;    //nLen将被编码函数改变为编码后的话单长度
    pBillBuffer = NULL;

    pBillBuffer = (BYTE* )m_PSAsnClosor.EnCodeBill_final_mt(m_pASN1FinalGlobal,
                                                            (char* )pABill->pBillBuf,
                                                            (char* )m_pASN1RecBuffer,
                                                            &nLen);

    return (NULL != pBillBuffer);
}

BOOL CPSChannel::AddABill(SBill& ABill)
{
    if((ABill.nLen <= 0) || (ABill.pBillBuf == NULL))
    {
        return FALSE;
    }

    SBill* pConvertedBill = new SBill;

    pConvertedBill->nLen = ABill.nLen + 4;
    pConvertedBill->pBillBuf = new BYTE[ABill.nLen + 4];
    int nNetLen = HTONL(ABill.nLen);
    memcpy(pConvertedBill->pBillBuf, &nNetLen, 4);
    memcpy(pConvertedBill->pBillBuf + 4, ABill.pBillBuf, ABill.nLen);

    m_BillBuffer.AddABill(pConvertedBill);

    return TRUE;
}

CPSBillProcess::CPSBillProcess(PACKET_SWITCH_RELEASE nPSRelease) : CBillProcess()
{
    m_pASN1RecContent = NULL;
    m_pASN1IniGlobal  = NULL;
    m_nPSRelease      = nPSRelease;

    m_pTotalGCDR      = NULL;
    m_pTotalPartGCDR  = NULL;
    m_pTotalMCDR      = NULL;
    m_pTotalPartMCDR  = NULL;
    m_pTotalSCDR      = NULL;
    m_pTotalPartSCDR  = NULL;
    m_pTotalSMOCDR    = NULL;
    m_pTotalSMTCDR    = NULL;
    m_pTotalLCSMOCDR  = NULL;
    m_pTotalLCSMTCDR  = NULL;
    m_pTotalLCSNICDR  = NULL;
    m_szOMCfgFile[0]  = '\0';
    m_bCheckOMField   = FALSE;

    //Added by ZhengYuqun SWPD04700
    m_bCDRFilterAPNNI  = FALSE;
}

CPSBillProcess::~CPSBillProcess()
{
    if (NULL != m_pASN1RecContent)
    {
        delete[] m_pASN1RecContent;
        m_pASN1RecContent = NULL;
    }

    if (NULL != m_pASN1IniGlobal)
    {
        ReleaseOSSLib();
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
BOOL CPSBillProcess::Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge)
{
    BOOL bSucc = CBillProcess::Init(uAPID, bNeedConvert, bNeedMerge);
    if (!bSucc)
    {
        return FALSE;
    }

    int nRet = m_PSAsnClosor.Init(uAPID);
    if(ERR_SUCCESS != nRet)
    {
        return FALSE;
    }
    m_bNeedConvert = FALSE;

    if (m_bNeedMerge)
    {
        m_pMerge = new CMerger();

        m_pParser = (CBillParser*)m_PSAsnClosor.CreateParserObject();
    }

    m_pASN1RecContent = new BYTE[ASN1_BUF_SIZE];
    InitOSSLib();
    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        CPSChannel* pChannel = new CPSChannel(m_nPSRelease);
        pChannel->Init(m_uAPID, nChannelNo, m_bNeedConvert);

        //编解码公用同一块内存
        pChannel->SetASN1Para(m_pASN1RecContent);

        m_ChannelArray[nChannelNo] = pChannel;
    }

    m_pTotalGCDR        = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_GCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalPartGCDR    = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_PART_GCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalMCDR        = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_MCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalPartMCDR    = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_PART_MCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalSCDR        = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_SCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalPartSCDR    = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_PART_SCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalSMOCDR      = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_SMOCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalSMTCDR      = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_SMTCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalLCSMOCDR    = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_LCSMOCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalLCSMTCDR    = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_LCSMTCDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalLCSNICDR    = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_TOTAL_LCSNICDR, PERF_STATS_TOTAL_VALUE);

    m_pTotalCDR         = CPerfItem::GetItem((BYTE)uAPID, MT_BILLPROCESSOR,
                                             PERF_PROCESSED_CDR, PERF_STATS_TOTAL_VALUE);

    CINIFile ini(GetCfgFilePath());
    ini.Open();

	char szAPSection[20];
	sprintf(szAPSection, "%s%d", CFG_SEC_AP, m_uAPID);
    m_bCheckOMField = ini.GetInt(szAPSection,
                                 CFG_CHECK_OM_FIELD,
                                 CFG_CHECK_OM_FIELD_DEFAULT);
    if(m_bCheckOMField)
    {
	    ini.GetString(szAPSection,
                      CFG_OM_FLD_CFG_FILE,
                      CFG_OM_FLD_CFG_FILE_DEFAULT,
                      m_szOMCfgFile,
                      sizeof(m_szOMCfgFile));
    }

    //Added by ZhengYuqun SWPD04700
    char szCDRFilter[49];
    ini.GetString(szAPSection, CFG_CDR_FILTER, CFG_CDR_FILTER_DEFAULT,
        szCDRFilter, 48);
    
    if (StringiCmp(szCDRFilter, "APN_NI"))
    {
        m_bCDRFilterAPNNI = TRUE;
    }

    return TRUE;
}

/**********************************************************************
功能描述    进行ASN.1解码
输入参数    pABill----解码前的话单记录
输出参数    pDecodedBill----解码后的话单记录，内存在函数外申请和释放
返回值      ASN.1解码是否成功
抛出异常    无
**********************************************************************/
int CPSBillProcess::DecodeABill(SBill* pABill, SBill* pDecodedBill)
{
    int nLen = pABill->nLen;   //nLen将被编码函数改变为编码后的话单长度
    int nRet = ERR_FAIL;

    nRet = m_PSAsnClosor.DeCodeBill_ini_mt(m_pASN1IniGlobal,
                                           (char* )pABill->pBillBuf,
                                           &nLen,
                                           (char* )m_pASN1RecContent,
                                           ASN1_BUF_SIZE);

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //在ASN.1解码后的话单将马上被处理，且处理完成前不会解码下一条话单，
    //所以直接使用m_pASNRecContent的指针，而不用进行内存拷贝
    pDecodedBill->nLen = nLen;
    pDecodedBill->pBillBuf = m_pASN1RecContent;

    //话单统计
    m_pTotalCDR->Add(1);

    PS_CDR_TYPE nCDRType;
    if (m_PSAsnClosor.GetOrginalCDRType(pDecodedBill->pBillBuf, nCDRType) == 0)
    {
        switch (nCDRType)
        {
        case PS_GCDR:
            m_pTotalGCDR->Add(1);
            break;

        case PS_PARTIAL_GCDR:
            m_pTotalGCDR->Add(1);
            m_pTotalPartGCDR->Add(1);
            break;

        case PS_MCDR:
            m_pTotalMCDR->Add(1);
            break;

        case PS_PARTIAL_MCDR:
            m_pTotalMCDR->Add(1);
            m_pTotalPartMCDR->Add(1);
            break;

        case PS_SCDR:
            m_pTotalSCDR->Add(1);
            break;

        case PS_PARTIAL_SCDR:
            m_pTotalPartSCDR->Add(1);
        //by ldf 2004-06-01 Perf CDR统计单元测试修改
            m_pTotalSCDR->Add(1);
            break;

        case PS_SMOCDR:
            m_pTotalSMOCDR->Add(1);
            break;

        case PS_SMTCDR:
            m_pTotalSMTCDR->Add(1);
            break;

        case PS_LCSMOCDR:
            m_pTotalLCSMOCDR->Add(1);
            break;

        case PS_LCSMTCDR:
            m_pTotalLCSMTCDR->Add(1);
            break;

        case PS_LCSNICDR:
            m_pTotalLCSNICDR->Add(1);
            break;

        default:
            break;
        }
    }

    return nRet;
}

/**********************************************************************
功能描述    话单预处理，并将话单包拆分为单张话单
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
BOOL CPSBillProcess::PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
    SBill ASN1Bill;
    BOOL bSucc = GetVarLenBill(pBills, uLeftLen, &ASN1Bill);
    if (!bSucc)
    {
        return FALSE;
    }
    
    //进行ASN1解码
    int nRet = DecodeABill(&ASN1Bill, pABill);
    if (ERR_SUCCESS != nRet)
    {
        m_nErrorCDRCount++;

        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_DECODE, nRet);
        m_ChannelArray[0]->AddABill(ASN1Bill); //将该话单放入default通道
        //add  by YanYong 2004-05-28 问题单 SWPD04793
        return FALSE;
    }

    //Added by ZhengYuqun SWPD04700
    if (m_bCDRFilterAPNNI)
    {
        if (m_PSAsnClosor.CDRFilterAPNNI(pABill->pBillBuf) != 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//处理一包不需要合并的话单
BOOL CPSBillProcess::ProcessABill(SBill& OrgBill, SBill& PreBill)
{
    //对单张话单进行分拣和转换处理
    UINT4 uChlID;
    CFormat* pDestFmt = NULL;

    //获取分拣通道号和转换格式对象，并进行话单的处理
    m_pIBillProcessor->Distribute(PreBill.pBillBuf,
                                  PreBill.nLen,
                                  uChlID,
                                  pDestFmt);
    if(uChlID > 0 && m_bCheckOMField)
    {
        if (m_PSAsnClosor.CheckOMField(m_szOMCfgFile, PreBill.pBillBuf, FALSE) != 0)
        {
            //放到缺省通道中
            uChlID = 0;
        }
    }

    m_ChannelArray[uChlID]->AddABill(OrgBill);

    return TRUE;
}

/**********************************************************************
功能描述    将一张话单分拣到所属的通道并进行格式转换等话单处理
输入参数    pABill----待分拣处理的话单
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CPSBillProcess::Distribute(SBill *pABill)
{
    UINT4 uChlID;
    CFormat* pDestFmt = NULL;

    //获取分拣通道号和转换格式对象，并进行话单的处理
    m_pIBillProcessor->Distribute(pABill->pBillBuf,
                                  pABill->nLen, 
                                  uChlID,
                                  pDestFmt);
    if(uChlID > 0 && m_bCheckOMField)
    {
        if (m_PSAsnClosor.CheckOMField(m_szOMCfgFile, pABill->pBillBuf, TRUE) != 0)
        {
            //放到缺省通道中
            uChlID = 0;
        }
    }
    m_ChannelArray[uChlID]->ProcessABill(pABill, pDestFmt);
}

void CPSBillProcess::InitOSSLib(void)
{
    m_pASN1IniGlobal = m_PSAsnClosor.Init_OSS_Lib_Ini();
}

void CPSBillProcess::ReleaseOSSLib(void)
{
    if(m_pASN1IniGlobal)
    {
        m_PSAsnClosor.Release_OSS_Lib_Ini(m_pASN1IniGlobal);
        m_pASN1IniGlobal = NULL;
    }
}
