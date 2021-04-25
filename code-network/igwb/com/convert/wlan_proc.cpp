#include "wlan_proc.h"
#include "proc_chanl.h"
#include "convert_comm.h"
#include "resource.h"
#include "../include/toolbox.h"
#include "../include/frame.h"


CWLANChannel::CWLANChannel() : CProcChannel()
{
	
}

CWLANChannel::~CWLANChannel()
{
    
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
BOOL CWLANChannel::ConvertABill(SBill* pABill, SBill* pConvertedBill,
                                CFormat* pDestFmt)
{
    return CProcChannel::ConvertABill(pABill, pConvertedBill, pDestFmt);
    return TRUE;
}

BOOL CWLANChannel::AddABill(SBill& ABill)
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


CBPWLAN::CBPWLAN() : CBillProcess()
{
    m_pDll = NULL;
}

CBPWLAN::~CBPWLAN()
{
    if(NULL != m_pDll)
    {
        m_pDll->close();
        delete m_pDll;
        m_pDll = NULL;
    }
}

extern "C" {
typedef void* (*CREATE_PARSER_OBJECT)();
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
BOOL CBPWLAN::Init(UINT4 uAPID, BOOL bNeedConvert, BOOL bNeedMerge)
{
    BOOL bSucc = CBillProcess::Init(uAPID, bNeedConvert, bNeedMerge);
    if (!bSucc)
    {
        return FALSE;
    }

    m_bNeedConvert = TRUE;
	m_uOrigLen = m_pIBillProcessor->GetOrigLen();

    if (m_bNeedMerge)
    {
        m_pMerge  = new CMerger();
        if(NULL == m_pDll)
        {
            //�������ļ�
            CINIFile Ini(GetCfgFilePath());
            Ini.Open();

            char szFmtDll[40];

            char strSec[20];
            sprintf(strSec, "%s%d", CFG_SEC_AP, uAPID);
            Ini.GetString(strSec,
                          CFG_AP_KEY_FMT_DLL,
                          CFG_AP_KEY_FMT_DLL_DEFAULT,
                          szFmtDll,
                          sizeof(szFmtDll));
            if(szFmtDll[0] == '\0')
            {
                return FALSE;
            }

            m_pDll = new ACE_DLL;
            int nRet = m_pDll->open(szFmtDll);
            if(0 != nRet)
            {
                delete m_pDll;
                m_pDll = NULL;
                return FALSE;
            }
        }
        
        CREATE_PARSER_OBJECT f = (CREATE_PARSER_OBJECT)m_pDll->symbol("CreateParserObject");
        if(NULL == f)
        {
            m_pDll->close();
            delete m_pDll;
            m_pDll = NULL;
            return FALSE;
        }
        
        m_pParser = (CBillParser*)(*f)();
        if(NULL == m_pParser)
        {
            return FALSE;
        }
    }

   
    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        m_ChannelArray[nChannelNo] = new CWLANChannel();
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
BOOL CBPWLAN::PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
	 return GetVarLenBill(pBills, uLeftLen, pABill);	
}

//����һ������Ҫ�ϲ��Ļ���
BOOL CBPWLAN::ProcessABill(SBill& OrgBill, SBill& PreBill)
{
    //�Ե��Ż������зּ��ת������
    UINT4 uChlID;
    CFormat* pDestFmt = NULL;

    //��ȡ�ּ�ͨ���ź�ת����ʽ���󣬲����л����Ĵ���
    m_pIBillProcessor->Distribute(PreBill.pBillBuf, PreBill.nLen, 
                                  uChlID, pDestFmt);
    m_ChannelArray[uChlID]->AddABill(OrgBill);

    return TRUE;
}
