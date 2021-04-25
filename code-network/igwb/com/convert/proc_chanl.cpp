#include "proc_chanl.h"
#include "convert_comm.h"
#include "../include/format_out.h"
#include "../include/frame.h"
#include "resource.h"

CBillBuffer::CBillBuffer()
{
	m_uBillDataLen = 0;
}

CBillBuffer::~CBillBuffer()
{
    SBill* pBill = NULL;
    //��������ɾ�������ڵ�����ݺͽڵ㱾��
    while (TRUE)
    {
        if (m_BillList.empty())
        {
            break;
        }

        pBill = *m_BillList.begin();
        ASSERT(NULL != pBill);

        m_BillList.pop_front();

        ASSERT(NULL != pBill->pBillBuf);

        delete[] pBill->pBillBuf;
		pBill->pBillBuf = NULL;
        delete pBill;
		pBill = NULL;
    }
}

//��ȡ�������еĻ��������ܳ���
UINT4 CBillBuffer::GetBillDataLength()
{
    return m_uBillDataLen;
}

/**********************************************************************
��������    �򻺳��������һ�Ż���
�������    pABill----Ҫ��ӽ��������Ļ�����Ӧ��SBill��¼�������߸��������ڴ�
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CBillBuffer::AddABill(SBill* pABill)
{
    m_BillList.push_back(pABill);
    m_uBillDataLen += pABill->nLen;
}

/**********************************************************************
��������    �ӻ�������ȡ����һ�Ż����������û�����������ɾ��
�������    ��
�������    ��
����ֵ      �����е�һ���ڵ��Ӧ��SBill*
�׳��쳣    ��
**********************************************************************/
SBill* CBillBuffer::GetABill()
{
	if (m_BillList.empty())
	{
		return NULL;
	}

    SBill* pBill = m_BillList.front();
    m_BillList.pop_front();
    m_uBillDataLen -= pBill->nLen;

    return pBill;
}


CProcChannel::CProcChannel()
{
    m_bCorrectChkSum = FALSE;
    m_nCheckSumPos = 0;
    m_nCheckPosLow = 0;
    m_nCheckPosHigh = 0;
}

CProcChannel::~CProcChannel()
{
}

void CProcChannel::SetLastPID(UINT4 uLastPID)
{
    m_ChannelSynInfo.uLastPID = uLastPID;
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
BOOL CProcChannel::Init(UINT4 uAPID, INT4 nChannelID, BOOL bNeedConvert)
{
    m_uAPID = uAPID;
    m_nChannelID = nChannelID;
    m_bNeedConvert = bNeedConvert;

    CINIFile cfgIniFile(GetCfgFilePath());
    BOOL bSucc = cfgIniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_OPEN_INI);
        return ERR_FAIL;
    }

    char szAPSection[25] = "";
    SNPRINTF(szAPSection, sizeof(szAPSection), "%s%d", 
             CFG_SEC_AP, m_uAPID);
    szAPSection[sizeof(szAPSection) - 1] = '\0';

    char szChlSection[40] = "";
    SNPRINTF(szChlSection, sizeof(szChlSection), CFG_SEC_CHANNEL, 
             m_uAPID, m_nChannelID);
    szChlSection[sizeof(szChlSection) - 1] = '\0';

    //���У��������ջ����е�λ��
    m_nCheckSumPos = cfgIniFile.GetInt(szAPSection, CFG_CHECK_SUM_POS, 
                                       CFG_CHECK_SUM_POS_DEFAULT);
    m_nCheckSumPos = cfgIniFile.GetInt(szChlSection, CFG_CHECK_SUM_POS, 
                                       m_nCheckSumPos);    
    
    //���У��������ջ����еķ�Χ    
    char szChkSumRange[32] = "";
    cfgIniFile.GetString(szAPSection, CFG_CHECK_SUM_RANGE, CFG_CHECK_SUM_RANGE_DEFAULT, 
                         szChkSumRange, sizeof(szChkSumRange));
    cfgIniFile.GetString(szChlSection, CFG_CHECK_SUM_RANGE, szChkSumRange, 
                         szChkSumRange, sizeof(szChkSumRange));
    
    if((m_nCheckSumPos != 0) && (szChkSumRange[0] == '\0'))
    {
        TRACE(MTS_BILLPROCESSOR, 
              S_CV_TRC_INVALID_CONF_PARA, 
              CFG_CHECK_SUM_RANGE);
        return FALSE;
    }

    if((m_nCheckSumPos == 0) && (szChkSumRange[0] != '\0'))
    {
        TRACE(MTS_BILLPROCESSOR, 
              S_CV_TRC_INVALID_CONF_PARA, 
              CFG_CHECK_SUM_POS);
        return FALSE;
    }

    //λ�úͷ�Χ�����õ����
    if((m_nCheckSumPos != 0) && (szChkSumRange[0] != '\0'))
    {
        char* pDiv = strchr(szChkSumRange, '-');
        if (pDiv == NULL)
        {
            //�����ֽڵ����
            StringLRTrim(szChkSumRange);
            if(!IsDigitValue(szChkSumRange))
            {
                TRACE(MTS_BILLPROCESSOR, 
                      S_CV_TRC_INVALID_CONF_PARA, 
                      CFG_CHECK_SUM_RANGE);
                return FALSE;
            }

            m_nCheckPosLow = atoi(szChkSumRange);
            m_nCheckPosHigh = m_nCheckPosLow;
        }
        else
        {
            //�ֽڷ�Χ�����
            pDiv[0] = '\0';
            char* szCheckPosLow = szChkSumRange;
            char* szCheckPosHigh = pDiv + 1;

            StringLRTrim(szCheckPosLow);
            StringLRTrim(szCheckPosHigh);
            if(!IsDigitValue(szCheckPosLow) || !IsDigitValue(szCheckPosHigh))
            {
                TRACE(MTS_BILLPROCESSOR, 
                      S_CV_TRC_INVALID_CONF_PARA, 
                      CFG_CHECK_SUM_RANGE);
                return FALSE;
            }

            m_nCheckPosLow = atoi(szCheckPosLow);
            m_nCheckPosHigh = atoi(szCheckPosHigh);
        }

        m_bCorrectChkSum = TRUE;
    }
    else
    {
        m_bCorrectChkSum = FALSE;
    }

    return TRUE;
}

/**********************************************************************
��������    ת��һ�Ż������������д���Ի������и�ʽת���Ĳ���
�������    pABill----���Ż��������ָ�� 
            pDestFmt----��ʽת���Ĺ�������
�������    pConvertedBill----ת����Ļ������󣬽ṹ���ڴ��ں�����������ͷ�
����ֵ      ת���Ƿ�ɹ�����ʾpConvertedBill�Ƿ���Ч
                TRUE----�ɹ�
                FALSE----ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CProcChannel::ConvertABill(SBill* pABill, SBill* pConvertedBill, 
                                CFormat* pDestFmt)
{
    if (m_bNeedConvert)
    {
        void* pBillBuf;
        UINT4 uLen;

        if (NULL != pDestFmt)
        {
            int nResult = pDestFmt->ConvertABill(pABill->pBillBuf, 
                                                 pABill->nLen, 
                                                 pBillBuf, uLen);
            if (0 != nResult)
            {
                return FALSE;
            }
            
            if (m_bCorrectChkSum)
            {
                if(!CorrectChkSum((BYTE*)pBillBuf, uLen, m_nCheckSumPos,
                                   m_nCheckPosLow, m_nCheckPosHigh))
                {
                    return FALSE;
                }
            }
        }
        else    //��ʱΪͨ��0������ת����ֱ�ӽ����ڴ濽��
        {
            uLen = pABill->nLen;
            pBillBuf = new BYTE[uLen];
            memcpy(pBillBuf, pABill->pBillBuf, uLen);
        }
        pConvertedBill->pBillBuf = (BYTE* )pBillBuf;
        pConvertedBill->nLen = (int)uLen;
    }
    else
    {
        pConvertedBill->nLen = pABill->nLen;
        pConvertedBill->pBillBuf = new BYTE[pConvertedBill->nLen];
        memcpy(pConvertedBill->pBillBuf, pABill->pBillBuf, pABill->nLen);
    }

    return TRUE;
}

/**********************************************************************
��������    ����һ�Ż���
�������    pABill----������ĵ��Ż��������ָ�� 
            pDestFmt----��ʽת���Ĺ�������
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CProcChannel::ProcessABill(SBill* pABill, CFormat* pDestFmt)
{
    SBill* pConvertedBill = new SBill;
    BOOL bSucc = ConvertABill(pABill, pConvertedBill, pDestFmt);
    if (!bSucc)
    {
        delete pConvertedBill;
		pConvertedBill = NULL;
        TRACE(MTS_BILLPROCESSOR, S_CV_TRC_FAIL_CONVERT_REC);
    }
    else
    {
        m_BillBuffer.AddABill(pConvertedBill);
    }
}

BOOL CProcChannel::AddABill(SBill& ABill)
{
    if((ABill.nLen <= 0) || (ABill.pBillBuf == NULL))
    {
        return FALSE;
    }

    SBill* pConvertedBill = new SBill;

    pConvertedBill->nLen = ABill.nLen;
    pConvertedBill->pBillBuf = new BYTE[ABill.nLen];

    memcpy(pConvertedBill->pBillBuf, ABill.pBillBuf, ABill.nLen);

    m_BillBuffer.AddABill(pConvertedBill);

    return TRUE;
}

/**********************************************************************
��������    ���������ݴ��
�������    ��
�������    pPacket----��������ʼָ�룬�������ռ��ɺ����ڲ����룬�ں������ͷ�
            uPacketLen----���������ȣ����ֽ�Ϊ��λ
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CProcChannel::GetBillPacket(BYTE* &pPacket, UINT4 &uPacketLen)
{
    uPacketLen = m_BillBuffer.GetBillDataLength();
	if (0 == uPacketLen)
	{
		pPacket = NULL;
		return;
	}
    pPacket = new BYTE[uPacketLen];

    UINT4 uWritePos = 0;    //�������еĿ���ָ��
    while (TRUE)
    {
        //ȡ���������е�һ�Ż���
        SBill* pABill = m_BillBuffer.GetABill();
        if (NULL == pABill)
        {
            break;
        }
        else
        {
            ASSERT(NULL != pABill->pBillBuf);

            memcpy(pPacket + uWritePos, pABill->pBillBuf, pABill->nLen);
            uWritePos += pABill->nLen;

            delete[] pABill->pBillBuf;
			pABill->pBillBuf = NULL;
            delete pABill;
			pABill = NULL;
        }
    }
}

/**********************************************************************
��������    ����Ӻ����ģ�鷢�͹�����ͬ����Ϣ
�������    pInfo----ͬ����Ϣ��ָ�룬�����߸����ͷŸ�ָ��Ŀռ�
**********************************************************************/
void CProcChannel::SynChannelInfo(const SChannelSynInfo* pInfo)
{
    m_ChannelSynInfo.uLastPID = pInfo->uLastPID;
    m_ChannelSynInfo.uBillCsn = pInfo->uBillCsn;
}

//��ȡͬ����Ϣ
SChannelSynInfo* CProcChannel::GetSynInfo()
{
    return &m_ChannelSynInfo;
}

/**********************************************************************
��������       //����ת����Ļ���У���
�������       uLen               ��������
               nCheckSumPos       У����ڻ����е�λ��
               nCheckPosLow       ҪУ��ĵ��ֽ�λ��
               nCheckPosHigh      ҪУ��ĸ��ֽ�λ��
�����������   pABill----Ҫ����У��;���Ļ���
����ֵ         �����Ƿ�ɹ�
                TRUE----�ɹ�
                FALSE----ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CProcChannel::CorrectChkSum(BYTE* pABill, UINT4 uLen, 
                                 INT4 nCheckSumPos, 
                                 INT4 nCheckPosLow, 
                                 INT4 nCheckPosHigh)
{
    if(pABill == NULL)
    {
        return FALSE;
    }

    if(nCheckSumPos > uLen)
    {
        TRACE(MTS_BILLPROCESSOR, 
              S_CV_TRC_CONF_PARA_EXCEED_BILL_LEN, 
              CFG_CHECK_SUM_POS,
              uLen);
        return FALSE;
    }

    if((nCheckPosLow > uLen) || (nCheckPosHigh > uLen))
    {
        TRACE(MTS_BILLPROCESSOR, 
              S_CV_TRC_CONF_PARA_EXCEED_BILL_LEN, 
              CFG_CHECK_SUM_RANGE,
              uLen);
        return FALSE;
    }

    BYTE byChkSum = 0;
    for(int i = nCheckPosLow - 1; i < nCheckPosHigh; i++)
    {
        byChkSum += pABill[i];
    }

    pABill[nCheckSumPos - 1] = byChkSum;

    return TRUE;
}
