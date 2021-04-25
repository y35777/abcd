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
功能描述    转换一张话单
输入参数    pABill----单张话单对象的指针，根据相应标记判断是否释放 
            pDestFmt----格式转换的工作对象
输出参数    pConvertedBill----转换后的话单对象，结构的内存在函数外申请和释放
返回值      转换是否成功，表示pConvertedBill是否有效
                TRUE----成功
                FALSE----失败
抛出异常    无
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
            //打开配置文件
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
BOOL CBPWLAN::PreProcess(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
	 return GetVarLenBill(pBills, uLeftLen, pABill);	
}

//处理一包不需要合并的话单
BOOL CBPWLAN::ProcessABill(SBill& OrgBill, SBill& PreBill)
{
    //对单张话单进行分拣和转换处理
    UINT4 uChlID;
    CFormat* pDestFmt = NULL;

    //获取分拣通道号和转换格式对象，并进行话单的处理
    m_pIBillProcessor->Distribute(PreBill.pBillBuf, PreBill.nLen, 
                                  uChlID, pDestFmt);
    m_ChannelArray[uChlID]->AddABill(OrgBill);

    return TRUE;
}
