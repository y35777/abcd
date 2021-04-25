#include "stp_proc.h"

CSTPChannel::CSTPChannel() : CProcChannel()
{

}

CSTPChannel::~CSTPChannel()
{

}

/**********************************************************************
��������    ����STP��������ˮ��
�������    pABill----������Ļ���
�������    pABill----��������ˮ�Ŵ����Ļ���
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSTPChannel::ProcessSerialNo(SBill* &pABill)
{
    *(UINT4* )(&pABill->pBillBuf[14]) = m_ChannelSynInfo.uBillCsn;
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
BOOL CBPSTP::PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
    return GetFixedLenBill(pBills, uLeftLen, pABill);
}
