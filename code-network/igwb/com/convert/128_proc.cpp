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
��������    ����128��������ˮ��
�������    pABill----������Ļ���
�������    pABill----��������ˮ�Ŵ����Ļ���
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void C128Channel::ProcessSerialNo(SBill*& pABill)
{
    *(UINT4* )(&pABill->pBillBuf[0]) = m_ChannelSynInfo.uBillCsn;
    m_ChannelSynInfo.uBillCsn++;
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
BOOL C128Channel::ConvertABill(SBill* pABill, SBill* pConvertedBill, 
                               CFormat* pDestFmt)
{
    if(m_bProcBillCsn)
    {
        //�����ȱʡͨ���Ļ�������Ҫ�ָ����ӵ����кš�
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



//128������
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
��������    ����Ԥ���������������Ϊ���Ż���
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

