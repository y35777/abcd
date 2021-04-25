#include "bill_proc.h"
#include "../include/msg_def.h"
#include "proc_chanl.h"
#include "time.h"
#include "../include/toolbox.h"
#include "../include/frame.h"
#include "resource.h"
#include "../include/perf_item.h"

CBillProcess::CBillProcess()
{
	m_pIBillProcessor = NULL;
	m_pIGWB = NULL;
	m_ChannelArray = NULL;
	m_pMerge = NULL;
	m_pParser = NULL;
	m_nErrorCDRCount = 0;
	m_pMergeQueue = 0;
	m_nReDistribute = 0;
}

CBillProcess::~CBillProcess()
{
    if (NULL != m_pIGWB)
    {
        delete m_pIGWB;
		m_pIGWB = NULL;
    }
    if (NULL != m_ChannelArray)
    {
        for (int i = 0; i < m_nChannelCount; i++)
        {
            if (NULL != m_ChannelArray[i])
            {
                delete m_ChannelArray[i];
				m_ChannelArray[i] = NULL;
            }
        }
        delete[] m_ChannelArray;
		m_ChannelArray = NULL;
    }
    if (NULL != m_pMerge)
    {
        delete m_pMerge;
		m_pMerge = NULL;
    }
	if (NULL != m_pParser)
	{
		delete m_pParser;
		m_pParser = NULL;
	}
}

INT4 CBillProcess::GetChannelCount()
{
    return m_nChannelCount;
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
��ע        ��������ģ��Ϊ���̣߳�����ͬһʱ�������ֻ������һ�������
            ���������У���˱���빲��ͬһ���ڴ�
**********************************************************************/
BOOL CBillProcess::Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge)
{
    m_bEmptyPacket = FALSE;
    m_bIsAssistUpdate = FALSE;
    m_uAPID = uAPID;
    m_bNeedConvert = bNeedConvert;
    m_bNeedMerge = bNeedMerge;
    m_pMerge = NULL;
	m_pParser =	NULL;
    m_bBillRemain = FALSE;

    //��ʼ��m_pIBillProcessor
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/%s/%s", GetAppPath(), 
            "config", "format");
    m_pIGWB = new CIGWB(szIGWBPath);
    m_pIBillProcessor 
        = (IBillProcessor* )m_pIGWB->QueryInterface(IID_FP_IBILLPROCESSOR);
	if (NULL == m_pIBillProcessor)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_INIT_FMT);
        return FALSE;
    }
    int nResult = m_pIBillProcessor->Init(m_uAPID);     
    if (0 != nResult)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_INIT_FMT);
        return FALSE;
    }

    //��ȡͨ����������ȱʡͨ��0
    LIST<STRING> ChlNames;
    m_nChannelCount = m_pIBillProcessor->GetChannelNames(ChlNames) + 1;      
    m_ChannelArray = new CProcChannel*[m_nChannelCount];
    if (NULL != m_ChannelArray)
    {
        for (int i = 0; i < m_nChannelCount; i++)
        {
			m_ChannelArray[i] = NULL;
        }
    }

	//Z30971���� ��ȡ�Ƿ�֧����ʱ�Ʋ��� 2002-12-10
    CINIFile cfgIniFile(GetCfgFilePath());
    BOOL bSucc = cfgIniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_OPEN_INI);
        return ERR_FAIL;
    }
   	char szAPSection[25];
    sprintf(szAPSection, "%s%d", CFG_SEC_AP, m_uAPID);
    m_bSupportDST = cfgIniFile.GetInt(szAPSection, CFG_SUPPORT_DST, 
                                          CFG_SUPPORT_DST_DEFAULT);
    //���ӽ���

    //Added by ZhengYuqun 2003-11-04 SWPD02238
    m_uMaxDuration = cfgIniFile.GetInt(szAPSection, CFG_MAX_DURATION, 
                                          CFG_MAX_DURATION_DEFAULT);

    m_bProcBillCsn = cfgIniFile.GetInt(szAPSection, CFG_PROC_BILL_CSN, 
                                          CFG_PROC_BILL_CSN_DEFAULT);

	//add by yangguang
    m_nReDistribute = cfgIniFile.GetInt(szAPSection, CFG_REDISTRIBUTE, 0);


    m_pMergeQueue = CPerfItem::GetItem(m_uAPID, 
									   MT_BILLPROCESSOR,
									   PERF_MERGE_QUEUE);

    return TRUE;
}

/**********************************************************************
��������    �����뻰���ϲ���صĳ�ʼ��
�������    nHashSize----�����ϲ����õ��Ĺ�ϣ��Ĵ�С
			nMValue----�����ϲ��е�Mֵ�Ĵ�С
			nMaxQwLen----�����ϲ��е����ȴ����еĳ���
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ� 
                TRUE----��ʼ���ɹ� 
                FALSE----��ʼ��ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CBillProcess::InitMerge(int nHashSize, int nMValue, int nMaxQwLen)
{
	if ((NULL != m_pMerge) && (NULL != m_pParser))
	{
		m_pMerge->SetHashSize(nHashSize);
        m_pMerge->SetM(nMValue);
        m_pMerge->SetMaxQwLength(nMaxQwLen);

        //ע�Ự�����͡�
        m_pParser->RegisterBillType(m_pMerge);
	}

	return TRUE;
}

//���ø���������־
void CBillProcess::SetUpdateFlag(BOOL bIsAssistUpdate)  
{
    m_bIsAssistUpdate = bIsAssistUpdate;
}

/**********************************************************************
��������    �жϸ��������Ƿ����
�������    ��
�������    ��
����ֵ      ���������Ƿ��Ѿ���� 
                TRUE----���������Ѿ���� 
                FALSE----��������δ���
�׳��쳣    ��
**********************************************************************/
BOOL CBillProcess::IsAssistUpdateOver()
{
    if (m_bIsAssistUpdate)
    {
        if (m_bNeedMerge)   //��Ҫ�ϲ�
        {
            return !m_bBillRemain;
        }
        else
        {
            return m_bEmptyPacket;
        }
    }

    return FALSE;
}

/**********************************************************************
��������    ����ͬ����Ϣ
�������    pInfo----ͬ����Ϣָ�룬�ռ��ں������ͷ� 
            uLen----ͬ����Ϣ����
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcess::ProcessSynInfo(const BYTE* pInfo, UINT4 uLen)
{
    SInformStatus* pStatus = (SInformStatus* )pInfo;

    //��ͨ�����Դ���ͬ����Ϣ
    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        SChannelSynInfo SynData;
        SynData.uLastPID = pStatus->uLastPID;
        SynData.uBillCsn = pStatus->uaBillCsns[nChannelNo];
        m_ChannelArray[nChannelNo]->SynChannelInfo(&SynData);
    }
}

/**********************************************************************
��������    ���ݺ����ģ�����󱣴���ż����ǰ���̵Ļָ���ʼ����
�������    uLastPID----����̵���󱣴����
�������    ��
����ֵ      ǰ���̵Ļָ���ʼ����
�׳��쳣    ��
**********************************************************************/
UINT4 CBillProcess::GetResumePID(UINT4 uLastPID)
{
    if (!m_bNeedMerge)
    {
        return uLastPID + 1;
    }
    else
    {
        return m_pMerge->GetResumePID(uLastPID);
    }
}

/**********************************************************************
��������    ��һ�Ż����ּ�������ͨ�������и�ʽת���Ȼ�������
�������    pABill----���ּ���Ļ���
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcess::Distribute(SBill *pABill)
{
    UINT4 uChlID;
    CFormat* pDestFmt = NULL;

    //��ȡ�ּ�ͨ���ź�ת����ʽ���󣬲����л����Ĵ���
    m_pIBillProcessor->Distribute(pABill->pBillBuf, pABill->nLen, 
                                  uChlID, pDestFmt);
    m_ChannelArray[uChlID]->ProcessABill(pABill, pDestFmt);
}


//ʯ��ׯccs����  add by yangguang 20040804
void CBillProcess::SpecDistribute(SBill *pABill)
{
    VECTOR<UINT4> uChlID;
    VECTOR<CFormat*> pDestFmt;
	
    //��ȡ�ּ�ͨ���ź�ת����ʽ���󣬲����л����Ĵ���
    m_pIBillProcessor->SpecDistribute(pABill->pBillBuf, pABill->nLen, 
								  uChlID, pDestFmt);
	for (int i=0; i<uChlID.size(); ++i) 
	{
		m_ChannelArray[uChlID[i]]->ProcessABill(pABill, pDestFmt[i]);	
	}
	if (0 == uChlID.size())
	{
		m_ChannelArray[0]->ProcessABill(pABill, NULL);
	}    
}

/**********************************************************************
��������    ����һ������
�������    uPID----�������İ��� 
            pPacket----��������ʼָ�룬�ں����������ͷ� 
            uPacketLen----���������ȣ����ֽ�Ϊ��λ
�������    uFrontBillCount----��ǰ����������������ģ��Ļ������� 
            uBackBillCount----�ӻ�������ģ����������̵Ļ������� 
            uMergeTime----�����ϲ���ASN.1���루����еĻ���ռ�õ�ʱ�� 
            uProcessTime----�������������ּ�͸�ʽת�������ܻ�
                            ��ASN.1���룩ռ�õ�ʱ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcess::ProcessBillPacket(UINT4 uPID, BYTE* pPacket, 
                                     UINT4 uPacketLen, UINT4& uFrontBillCount, 
                                     UINT4& uBackBillCount, 
                                     UINT4& uMergeTime, UINT4& uProcessTime)
{
    static ACE_Profile_Timer timer;   //���ڽ�������ͳ�ƣ���ȷ������
    static ACE_Profile_Timer::ACE_Elapsed_Time et;
    timer.start();

    uFrontBillCount = uBackBillCount = 0;
    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        m_ChannelArray[nChannelNo]->SetLastPID(uPID);
    }
    m_bEmptyPacket = (0 == uPacketLen);

    BOOL bSucc;

    if (m_bNeedMerge)         //��Ҫ�ϲ�
    {
		//ÿ��100����ӡһ��ÿ�ֻ�����Qt��Qw���еĳ���
		if (0 == uPID % 100)
		{
			char szBillTypeDesc[100];
			UINT4 uBillTypeID, uQwLength, uQtLength;
			UINT4 uQwtLen = 0;
			for (int i = 0; i < m_pMerge->GetBillTypeCount(); i++)
			{
				uBillTypeID = m_pMerge->GetBillTypeID(i);
				uQwLength = m_pMerge->GetQwLength(uBillTypeID);
				uQtLength = m_pMerge->GetQtLength(uBillTypeID);

				uQwtLen += uQwLength + uQtLength;
				
				m_pParser->GetBillTypeDesc(uBillTypeID, szBillTypeDesc);

				MSGOUT(MTS_BILLPROCESSOR, MSGOUT_LEVEL_NORMAL, 
					   S_CV_MSG_PRINT_QUEUE_LEN, 
					   szBillTypeDesc, uQwLength, uQtLength);
			}
			m_pMergeQueue->Set(uQwtLen);
		}

        //�����ڴ���һ������֮ǰ���ɺڻ�����
        m_pMerge->GenerateBlackBill(uPID);

        //���л����ϲ�
        BYTE* pLeftBills = pPacket;
        UINT4 uLeftLen = uPacketLen;
        SBill ABill;
		CBill* pBill = NULL;
        while (uLeftLen > 0)
        {
            //��õ��Ż���
            bSucc = PreProcess(pLeftBills, uLeftLen, &ABill);
            if (!bSucc)
            {
                continue;
            }

            uFrontBillCount++;

            //�ѻ�����¼���ݽ���Ϊ��������
			bSucc = m_pParser->ParseARecord((char* )ABill.pBillBuf, ABill.nLen);
			if (!bSucc)	//���������úϲ���ֱ�ӷ��뻰��������
			{
	            //�Ե��Ż������зּ��ת������
	            Distribute(&ABill);
	            uBackBillCount++;
				continue;
			}

            pBill = m_pParser->GetBillRecord();
			
			//Z30971����Ƿ�֧����ʱ�Ʊ�־
			pBill->SetDSTFlag(m_bSupportDST);

            //Added by ZhengYuqun 2003-11-04 SWPD02238
            pBill->SetMaxDuration(m_uMaxDuration);
            
			//�Ե��Ż������кϲ�����
            m_pMerge->MergeABill(pBill, uPID);  
        }

        //��ȡ�ϲ�ռ�õ�ʱ��
        timer.stop();
        timer.elapsed_time(et);
        uMergeTime = et.real_time;

        timer.start();

        //���л�������
		SBill AMergedBill;
        while (pBill = m_pMerge->GetOKBill())
        {
			AMergedBill.nLen = pBill->GetLength();
            AMergedBill.pBillBuf = (BYTE* )pBill->GetContents();
			if (NULL != AMergedBill.pBillBuf)
			{
	            //�Ե��Ż������зּ��ת������
	            Distribute(&AMergedBill);
	            uBackBillCount++;
			}

			delete pBill;
			pBill = NULL;
        }

        m_bBillRemain = m_pMerge->IsBillRemain();

        //��ȡ��������ռ�õ�ʱ��
        timer.stop();
        timer.elapsed_time(et);
        uProcessTime = et.real_time;
    }
    else
    {
        uMergeTime = 0;

        //���л�������
        BYTE* pLeftBills = pPacket;
        UINT4 uLeftLen = uPacketLen;
        SBill OrgBill;
        SBill PreBill;
        while (uLeftLen > 0)
        {
            BYTE* pLeftBills1 = pLeftBills;
            UINT4 uLeftLen1 = uLeftLen;
            GetVarLenBill(pLeftBills1, uLeftLen1, &OrgBill);

            //��õ��Ż���
            bSucc = PreProcess(pLeftBills, uLeftLen, &PreBill);
            if (!bSucc)
            {
                continue;
            }

            uFrontBillCount++;
            bSucc = ProcessABill(OrgBill, PreBill); //�Ե��Ż������кϲ�����
            if(bSucc) uBackBillCount++;
        }

        //��ȡ��������ռ�õ�ʱ��
        timer.stop();
        timer.elapsed_time(et);
        uProcessTime = et.real_time;
    }
}

//����һ�Ų���Ҫ�ϲ��Ļ���
BOOL CBillProcess::ProcessABill(SBill& OrgBill, SBill& PreBill)
{
	if (m_nReDistribute == 0)
		Distribute(&PreBill); //�Ե��Ż������кϲ�����
	else
		SpecDistribute(&PreBill);

    return TRUE;
}

/**********************************************************************
��������    �õ�ĳһָ��ͨ���µ����ջ�����
�������    uChannelID----ָ����ͨ����
�������    pPacket----������ָ�� 
            uPacketLen----����������
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcess::GetChannelPacket(UINT4 uChannelID, 
                                    BYTE*& pPacket, UINT4& uPacketLen)
{
    m_ChannelArray[uChannelID]->GetBillPacket(pPacket, uPacketLen);
}

/**********************************************************************
��������    �õ�ָ��ͨ���µ�ͬ����Ϣ
�������    uChannelID----ָ����ͨ����
�������    ��
����ֵ      ָ��ͨ���µ�ͬ����Ϣ
�׳��쳣    ��
**********************************************************************/
SChannelSynInfo* CBillProcess::GetChannelSynInfo(UINT4 uChannelID)
{
    return m_ChannelArray[uChannelID]->GetSynInfo();
}

/**********************************************************************
��������    �ڻ������ȹ̶�������£��ӻ������в�ֳ�һ�Ż�������128��GSM��STP��
            ��Ʒ����Ч����PreProcess��������
�������    pBills----�����ǰָ�룬�Ӵ˴���ʼ��ȡ������¼ 
            uLeftLen----������ʣ�೤��
�������    pBills----��ȡ������¼��Ľ����ǰָ�룬�Ӵ˴���ʼ��ȡ������¼ 
            uLeftLen----��ȡ������¼��Ļ�����ʣ�೤��
            pABill----��ȡ�û�����¼���ṹ���ڴ��ں�����������ͷ�
����ֵ      ��û�ж�ȡ�ɹ�һ����������־pABill�Ƿ���Ч
                TRUE----��ȡ�ɹ�
                FALSE----��ȡʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CBillProcess::GetFixedLenBill(BYTE*& pBills, UINT4 &uLeftLen, 
                                   SBill* pABill)
{
    BOOL bSucc = FALSE;

    if (uLeftLen >= m_uOrigLen)
    {
        pABill->nLen = m_uOrigLen;
        pABill->pBillBuf = pBills;

        //����pBills��uLeftLen��ֵ
        pBills += m_uOrigLen;
        uLeftLen -= m_uOrigLen;

        bSucc = TRUE;
    }
    else if ((uLeftLen > 0) && (uLeftLen < m_uOrigLen))
    //ʣ�µĳ��Ȼ�����һ�Ż������������С��������
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_REJ_DATA_SMALL_LEN, 
				m_uOrigLen, uLeftLen);
        pBills += uLeftLen;
        uLeftLen = 0;
    }
    else
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_REJ_DATA_ZERO_LEN);
        uLeftLen = 0;
    }

    return bSucc;
}

/**********************************************************************
��������    �ڻ������еĻ�����ŷ�ʽΪ2λ���ȣ��������ݵ�����£��ӻ������в��
            ��һ�Ż�������GPRS��3G�Ȳ�Ʒ����Ч����PreProcess��������
�������    pBills----�����ǰָ�룬�Ӵ˴���ʼ��ȡ������¼ 
            uLeftLen----������ʣ�೤��
�������    pBills----��ȡ������¼��Ľ����ǰָ�룬�Ӵ˴���ʼ��ȡ������¼ 
            uLeftLen----��ȡ������¼��Ļ�����ʣ�೤��
            pABill----��ȡ�û�����¼���ṹ���ڴ��ں�����������ͷ�
����ֵ      ��û�ж�ȡ�ɹ�һ����������־pABill�Ƿ���Ч
                TRUE----��ȡ�ɹ�
                FALSE----��ȡʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CBillProcess::GetVarLenBill(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
    BOOL bSucc = FALSE;

    if (uLeftLen >= 2)
    {
        UINT2 nBillLen;
        //����UNIX�µ��ֽڶ������⣬����ֱ�ӽ��и�ֵ������������ڴ濽��
        memcpy(&nBillLen, pBills, 2); 
        nBillLen = NTOHS(nBillLen);

        if (uLeftLen < nBillLen + 2)
        //ʣ�µĳ��Ȼ�����һ�Ż������������С��������
        {
            //by ldf 2003-03-24 ��Ӧ���ⵥD07718 ע�͵��˴�
            //TRACE(MTS_BILLPROCESSOR, S_CV_TRC_REJ_DATA_SMALL_LEN, 
			//		nBillLen, uLeftLen - 2);
			//end
            pBills += uLeftLen;
            uLeftLen = 0;
        }
        else
        {
            pABill->nLen = nBillLen;
            pABill->pBillBuf = pBills + 2;

            //����pBills��uLeftLen��ֵ
            pBills += nBillLen + 2;
            uLeftLen -= nBillLen + 2;

            bSucc = TRUE;
        }
    }
    else if ((uLeftLen > 0) && (uLeftLen < 2))
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_REJ_DATA_LESS_2);
        pBills += uLeftLen;
        uLeftLen = 0;
    }
    else
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_REJ_DATA_ZERO_LEN, uLeftLen);
        uLeftLen = 0;
    }

    return bSucc;
}


/**********************************************************************
��������    ����쳣��������
�������    ��            
�������    ��
����ֵ      �쳣������
�׳��쳣    ��
**********************************************************************/
int CBillProcess::GetErrorCDRCount(void)
{
	return m_nErrorCDRCount;
}

/**********************************************************************
��������    ��λ�쳣��������
�������    ��            
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillProcess::ResetErrorCDRCount()
{
	m_nErrorCDRCount = 0;
}
