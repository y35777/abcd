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
��������    ����ͨ�������ò��������г�ʼ��
�������    uAPID----��ǰ������ID��
            nChannelID----��ǰ��ͨ����
            bNeedConvert----�Ƿ���Ҫ��ʽת��
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ�
                TRUE----��ʼ���ɹ�
                FALSE----��ʼ��ʧ��
�׳��쳣    ��
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


//���ú�ASN1��صĲ���
//��ע��Ϊ�˽�ʡ�ռ䣬GPRS�ı���빫��ͬһ���ڴ�
void CPSChannel::SetASN1Para(BYTE* pBuffer)
{
    m_pASN1RecBuffer = pBuffer;
}

/**********************************************************************
��������    ת��һ�Ż���
�������    pABill----���Ż��������ָ�룬������Ӧ����ж��Ƿ��ͷ�
            pDestFmt----��ʽת���Ĺ�������
�������    pConvertedBill----ת����Ļ������󣬽ṹ���ڴ��ں�����������ͷ�
����ֵ      ת���Ƿ�ɹ�����ʾpConvertedBill�Ƿ���Ч
                TRUE----�ɹ�
                FALSE----ʧ��
�׳��쳣    ��
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

    //GPRS�ĺ�����л�������4λ���ȣ��������ݵķ�ʽ��ţ�
    //���ǵ�UNIX�µ��ֽڶ������⣬����ֱ�ӶԻ������ȵ�4���ֽڽ��и�ֵ
    pConvertedBill->nLen = nLen + 4;
    pConvertedBill->pBillBuf = new BYTE[nLen + 4];
    INT4 nNetLen = HTONL(nLen);
    memcpy(pConvertedBill->pBillBuf, &nNetLen, 4);
    memcpy(pConvertedBill->pBillBuf + 4, pBillBuffer, nLen);

    return TRUE;
}

BOOL CPSChannel::EncodeABill(const SBill* pABill, BYTE*& pBillBuffer, int& nLen)
{
    nLen = ASN1_BUF_SIZE;    //nLen�������뺯���ı�Ϊ�����Ļ�������
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
��������    ��ʼ������
�������    uAPID----��ǰ������ID��
            bNeedConvert----�Ƿ���Ҫ��ʽת��
            bNeedMerge----�Ƿ���Ҫ�ϲ�
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ�
                TRUE----��ʼ���ɹ�
                FALSE----��ʼ��ʧ��
�׳��쳣    ��
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

        //����빫��ͬһ���ڴ�
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
��������    ����ASN.1����
�������    pABill----����ǰ�Ļ�����¼
�������    pDecodedBill----�����Ļ�����¼���ڴ��ں�����������ͷ�
����ֵ      ASN.1�����Ƿ�ɹ�
�׳��쳣    ��
**********************************************************************/
int CPSBillProcess::DecodeABill(SBill* pABill, SBill* pDecodedBill)
{
    int nLen = pABill->nLen;   //nLen�������뺯���ı�Ϊ�����Ļ�������
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

    //��ASN.1�����Ļ��������ϱ������Ҵ������ǰ���������һ��������
    //����ֱ��ʹ��m_pASNRecContent��ָ�룬�����ý����ڴ濽��
    pDecodedBill->nLen = nLen;
    pDecodedBill->pBillBuf = m_pASN1RecContent;

    //����ͳ��
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
        //by ldf 2004-06-01 Perf CDRͳ�Ƶ�Ԫ�����޸�
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
��������    ����Ԥ�����������������Ϊ���Ż���
�������    pBills----�����ǰָ�룬�Ӵ˴���ʼ��ȡ������¼
            uLeftLen----������ʣ�೤��
�������    pBills----��ȡ������¼��Ľ����ǰָ�룬�Ӵ˴���ʼ��ȡ������¼
            uLeftLen----��ȡ������¼��Ļ�����ʣ�೤��
            pABill----�ӻ������ж�����һ��Ԥ�����Ļ�����¼��
                      �ṹռ�õ��ڴ��ں�����������ͷ�
����ֵ      ����Ԥ�����Ƿ�ɹ�
                TRUE----Ԥ����ɹ�
                FALSE----Ԥ����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CPSBillProcess::PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
    SBill ASN1Bill;
    BOOL bSucc = GetVarLenBill(pBills, uLeftLen, &ASN1Bill);
    if (!bSucc)
    {
        return FALSE;
    }
    
    //����ASN1����
    int nRet = DecodeABill(&ASN1Bill, pABill);
    if (ERR_SUCCESS != nRet)
    {
        m_nErrorCDRCount++;

        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_DECODE, nRet);
        m_ChannelArray[0]->AddABill(ASN1Bill); //���û�������defaultͨ��
        //add  by YanYong 2004-05-28 ���ⵥ SWPD04793
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

//����һ������Ҫ�ϲ��Ļ���
BOOL CPSBillProcess::ProcessABill(SBill& OrgBill, SBill& PreBill)
{
    //�Ե��Ż������зּ��ת������
    UINT4 uChlID;
    CFormat* pDestFmt = NULL;

    //��ȡ�ּ�ͨ���ź�ת����ʽ���󣬲����л����Ĵ���
    m_pIBillProcessor->Distribute(PreBill.pBillBuf,
                                  PreBill.nLen,
                                  uChlID,
                                  pDestFmt);
    if(uChlID > 0 && m_bCheckOMField)
    {
        if (m_PSAsnClosor.CheckOMField(m_szOMCfgFile, PreBill.pBillBuf, FALSE) != 0)
        {
            //�ŵ�ȱʡͨ����
            uChlID = 0;
        }
    }

    m_ChannelArray[uChlID]->AddABill(OrgBill);

    return TRUE;
}

/**********************************************************************
��������    ��һ�Ż����ּ�������ͨ�������и�ʽת���Ȼ�������
�������    pABill----���ּ���Ļ���
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CPSBillProcess::Distribute(SBill *pABill)
{
    UINT4 uChlID;
    CFormat* pDestFmt = NULL;

    //��ȡ�ּ�ͨ���ź�ת����ʽ���󣬲����л����Ĵ���
    m_pIBillProcessor->Distribute(pABill->pBillBuf,
                                  pABill->nLen, 
                                  uChlID,
                                  pDestFmt);
    if(uChlID > 0 && m_bCheckOMField)
    {
        if (m_PSAsnClosor.CheckOMField(m_szOMCfgFile, pABill->pBillBuf, TRUE) != 0)
        {
            //�ŵ�ȱʡͨ����
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
