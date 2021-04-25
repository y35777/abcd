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
    //遍历链表，删除各个节点的内容和节点本身
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

//获取缓冲区中的话单数据总长度
UINT4 CBillBuffer::GetBillDataLength()
{
    return m_uBillDataLen;
}

/**********************************************************************
功能描述    向缓冲区中添加一张话单
输入参数    pABill----要添加进缓冲区的话单对应的SBill记录，调用者负责申请内存
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillBuffer::AddABill(SBill* pABill)
{
    m_BillList.push_back(pABill);
    m_uBillDataLen += pABill->nLen;
}

/**********************************************************************
功能描述    从缓冲区中取出第一张话单，并将该话单从链表中删除
输入参数    无
输出参数    无
返回值      链表中第一个节点对应的SBill*
抛出异常    无
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

    //获得校验和在最终话单中的位置
    m_nCheckSumPos = cfgIniFile.GetInt(szAPSection, CFG_CHECK_SUM_POS, 
                                       CFG_CHECK_SUM_POS_DEFAULT);
    m_nCheckSumPos = cfgIniFile.GetInt(szChlSection, CFG_CHECK_SUM_POS, 
                                       m_nCheckSumPos);    
    
    //获得校验和在最终话单中的范围    
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

    //位置和范围都配置的情况
    if((m_nCheckSumPos != 0) && (szChkSumRange[0] != '\0'))
    {
        char* pDiv = strchr(szChkSumRange, '-');
        if (pDiv == NULL)
        {
            //单个字节的情况
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
            //字节范围的情况
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
功能描述    转换一张话单，本函数中处理对话单进行格式转换的操作
输入参数    pABill----单张话单对象的指针 
            pDestFmt----格式转换的工作对象
输出参数    pConvertedBill----转换后的话单对象，结构的内存在函数外申请和释放
返回值      转换是否成功，表示pConvertedBill是否有效
                TRUE----成功
                FALSE----失败
抛出异常    无
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
        else    //此时为通道0，不作转换，直接进行内存拷贝
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
功能描述    处理一张话单
输入参数    pABill----待处理的单张话单对象的指针 
            pDestFmt----格式转换的工作对象
输出参数    无
返回值      无
抛出异常    无
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
功能描述    将话单数据打包
输入参数    无
输出参数    pPacket----话单包起始指针，话单包空间由函数内部申请，在函数外释放
            uPacketLen----话单包长度，以字节为单位
返回值      无
抛出异常    无
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

    UINT4 uWritePos = 0;    //话单包中的拷贝指针
    while (TRUE)
    {
        //取出缓冲区中的一张话单
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
功能描述    处理从后存盘模块发送过来的同步信息
输出参数    pInfo----同步信息的指针，调用者负责释放该指针的空间
**********************************************************************/
void CProcChannel::SynChannelInfo(const SChannelSynInfo* pInfo)
{
    m_ChannelSynInfo.uLastPID = pInfo->uLastPID;
    m_ChannelSynInfo.uBillCsn = pInfo->uBillCsn;
}

//获取同步信息
SChannelSynInfo* CProcChannel::GetSynInfo()
{
    return &m_ChannelSynInfo;
}

/**********************************************************************
功能描述       //纠正转换后的话单校验和
输入参数       uLen               话单长度
               nCheckSumPos       校验和在话单中的位置
               nCheckPosLow       要校验的低字节位置
               nCheckPosHigh      要校验的高字节位置
输入输出参数   pABill----要进行校验和纠错的话单
返回值         纠错是否成功
                TRUE----成功
                FALSE----失败
抛出异常    无
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
