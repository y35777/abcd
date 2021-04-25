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
功能描述    初始化函数
输入参数    uAPID----当前接入点的ID号
            bNeedConvert----是否需要格式转换
            bNeedMerge----是否需要合并
输出参数    无
返回值      初始化是否成功 
                TRUE----初始化成功 
                FALSE----初始化失败
抛出异常    无
备注        话单处理模块为单线程，所以同一时间内最多只可能有一个编解码
            操作在运行，因此编解码共用同一块内存
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

    //初始化m_pIBillProcessor
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

    //获取通道数，包括缺省通道0
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

	//Z30971增加 读取是否支持夏时制参数 2002-12-10
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
    //增加结束

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
功能描述    进行与话单合并相关的初始化
输入参数    nHashSize----话单合并中用到的哈希表的大小
			nMValue----话单合并中的M值的大小
			nMaxQwLen----话单合并中的最大等待队列的长度
输出参数    无
返回值      初始化是否成功 
                TRUE----初始化成功 
                FALSE----初始化失败
抛出异常    无
**********************************************************************/
BOOL CBillProcess::InitMerge(int nHashSize, int nMValue, int nMaxQwLen)
{
	if ((NULL != m_pMerge) && (NULL != m_pParser))
	{
		m_pMerge->SetHashSize(nHashSize);
        m_pMerge->SetM(nMValue);
        m_pMerge->SetMaxQwLength(nMaxQwLen);

        //注册话单类型。
        m_pParser->RegisterBillType(m_pMerge);
	}

	return TRUE;
}

//设置辅助升级标志
void CBillProcess::SetUpdateFlag(BOOL bIsAssistUpdate)  
{
    m_bIsAssistUpdate = bIsAssistUpdate;
}

/**********************************************************************
功能描述    判断辅助升级是否完成
输入参数    无
输出参数    无
返回值      辅助升级是否已经完成 
                TRUE----辅助升级已经完成 
                FALSE----辅助升级未完成
抛出异常    无
**********************************************************************/
BOOL CBillProcess::IsAssistUpdateOver()
{
    if (m_bIsAssistUpdate)
    {
        if (m_bNeedMerge)   //需要合并
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
功能描述    处理同步信息
输入参数    pInfo----同步信息指针，空间在函数外释放 
            uLen----同步信息长度
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcess::ProcessSynInfo(const BYTE* pInfo, UINT4 uLen)
{
    SInformStatus* pStatus = (SInformStatus* )pInfo;

    //各通道各自处理同步信息
    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        SChannelSynInfo SynData;
        SynData.uLastPID = pStatus->uLastPID;
        SynData.uBillCsn = pStatus->uaBillCsns[nChannelNo];
        m_ChannelArray[nChannelNo]->SynChannelInfo(&SynData);
    }
}

/**********************************************************************
功能描述    根据后存盘模块的最后保存包号计算出前存盘的恢复起始包号
输入参数    uLastPID----后存盘的最后保存包号
输出参数    无
返回值      前存盘的恢复起始包号
抛出异常    无
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
功能描述    将一张话单分拣到所属的通道并进行格式转换等话单处理
输入参数    pABill----待分拣处理的话单
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcess::Distribute(SBill *pABill)
{
    UINT4 uChlID;
    CFormat* pDestFmt = NULL;

    //获取分拣通道号和转换格式对象，并进行话单的处理
    m_pIBillProcessor->Distribute(pABill->pBillBuf, pABill->nLen, 
                                  uChlID, pDestFmt);
    m_ChannelArray[uChlID]->ProcessABill(pABill, pDestFmt);
}


//石家庄ccs需求  add by yangguang 20040804
void CBillProcess::SpecDistribute(SBill *pABill)
{
    VECTOR<UINT4> uChlID;
    VECTOR<CFormat*> pDestFmt;
	
    //获取分拣通道号和转换格式对象，并进行话单的处理
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
功能描述    处理一包话单
输入参数    uPID----话单包的包号 
            pPacket----话单包起始指针，在函数外申请释放 
            uPacketLen----话单包长度，以字节为单位
输出参数    uFrontBillCount----从前存盘送往话单处理模块的话单张数 
            uBackBillCount----从话单处理模块送往后存盘的话单张数 
            uMergeTime----话单合并与ASN.1解码（如果有的话）占用的时间 
            uProcessTime----话单处理（包括分拣和格式转换，可能还
                            有ASN.1编码）占用的时间
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcess::ProcessBillPacket(UINT4 uPID, BYTE* pPacket, 
                                     UINT4 uPacketLen, UINT4& uFrontBillCount, 
                                     UINT4& uBackBillCount, 
                                     UINT4& uMergeTime, UINT4& uProcessTime)
{
    static ACE_Profile_Timer timer;   //用于进行性能统计，精确到毫秒
    static ACE_Profile_Timer::ACE_Elapsed_Time et;
    timer.start();

    uFrontBillCount = uBackBillCount = 0;
    for (int nChannelNo = 0; nChannelNo < m_nChannelCount; nChannelNo++)
    {
        m_ChannelArray[nChannelNo]->SetLastPID(uPID);
    }
    m_bEmptyPacket = (0 == uPacketLen);

    BOOL bSucc;

    if (m_bNeedMerge)         //需要合并
    {
		//每隔100包打印一次每种话单的Qt、Qw队列的长度
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

        //必须在处理一包话单之前生成黑话单。
        m_pMerge->GenerateBlackBill(uPID);

        //进行话单合并
        BYTE* pLeftBills = pPacket;
        UINT4 uLeftLen = uPacketLen;
        SBill ABill;
		CBill* pBill = NULL;
        while (uLeftLen > 0)
        {
            //获得单张话单
            bSucc = PreProcess(pLeftBills, uLeftLen, &ABill);
            if (!bSucc)
            {
                continue;
            }

            uFrontBillCount++;

            //把话单记录数据解析为话单对象
			bSucc = m_pParser->ParseARecord((char* )ABill.pBillBuf, ABill.nLen);
			if (!bSucc)	//本话单不用合并，直接放入话单缓冲区
			{
	            //对单张话单进行分拣和转换处理
	            Distribute(&ABill);
	            uBackBillCount++;
				continue;
			}

            pBill = m_pParser->GetBillRecord();
			
			//Z30971添加是否支持夏时制标志
			pBill->SetDSTFlag(m_bSupportDST);

            //Added by ZhengYuqun 2003-11-04 SWPD02238
            pBill->SetMaxDuration(m_uMaxDuration);
            
			//对单张话单进行合并处理
            m_pMerge->MergeABill(pBill, uPID);  
        }

        //获取合并占用的时间
        timer.stop();
        timer.elapsed_time(et);
        uMergeTime = et.real_time;

        timer.start();

        //进行话单处理
		SBill AMergedBill;
        while (pBill = m_pMerge->GetOKBill())
        {
			AMergedBill.nLen = pBill->GetLength();
            AMergedBill.pBillBuf = (BYTE* )pBill->GetContents();
			if (NULL != AMergedBill.pBillBuf)
			{
	            //对单张话单进行分拣和转换处理
	            Distribute(&AMergedBill);
	            uBackBillCount++;
			}

			delete pBill;
			pBill = NULL;
        }

        m_bBillRemain = m_pMerge->IsBillRemain();

        //获取话单处理占用的时间
        timer.stop();
        timer.elapsed_time(et);
        uProcessTime = et.real_time;
    }
    else
    {
        uMergeTime = 0;

        //进行话单处理
        BYTE* pLeftBills = pPacket;
        UINT4 uLeftLen = uPacketLen;
        SBill OrgBill;
        SBill PreBill;
        while (uLeftLen > 0)
        {
            BYTE* pLeftBills1 = pLeftBills;
            UINT4 uLeftLen1 = uLeftLen;
            GetVarLenBill(pLeftBills1, uLeftLen1, &OrgBill);

            //获得单张话单
            bSucc = PreProcess(pLeftBills, uLeftLen, &PreBill);
            if (!bSucc)
            {
                continue;
            }

            uFrontBillCount++;
            bSucc = ProcessABill(OrgBill, PreBill); //对单张话单进行合并处理
            if(bSucc) uBackBillCount++;
        }

        //获取话单处理占用的时间
        timer.stop();
        timer.elapsed_time(et);
        uProcessTime = et.real_time;
    }
}

//处理一张不需要合并的话单
BOOL CBillProcess::ProcessABill(SBill& OrgBill, SBill& PreBill)
{
	if (m_nReDistribute == 0)
		Distribute(&PreBill); //对单张话单进行合并处理
	else
		SpecDistribute(&PreBill);

    return TRUE;
}

/**********************************************************************
功能描述    得到某一指定通道下的最终话单包
输入参数    uChannelID----指定的通道号
输出参数    pPacket----话单包指针 
            uPacketLen----话单包长度
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcess::GetChannelPacket(UINT4 uChannelID, 
                                    BYTE*& pPacket, UINT4& uPacketLen)
{
    m_ChannelArray[uChannelID]->GetBillPacket(pPacket, uPacketLen);
}

/**********************************************************************
功能描述    得到指定通道下的同步信息
输入参数    uChannelID----指定的通道号
输出参数    无
返回值      指定通道下的同步信息
抛出异常    无
**********************************************************************/
SChannelSynInfo* CBillProcess::GetChannelSynInfo(UINT4 uChannelID)
{
    return m_ChannelArray[uChannelID]->GetSynInfo();
}

/**********************************************************************
功能描述    在话单长度固定的情况下，从话单包中拆分出一张话单，在128、GSM、STP等
            产品中有效。被PreProcess函数调用
输入参数    pBills----解包当前指针，从此处开始读取话单记录 
            uLeftLen----话单包剩余长度
输出参数    pBills----读取话单记录后的解包当前指针，从此处开始读取话单记录 
            uLeftLen----读取话单记录后的话单包剩余长度
            pABill----读取得话单记录，结构的内存在函数外申请和释放
返回值      有没有读取成功一条话单，标志pABill是否有效
                TRUE----读取成功
                FALSE----读取失败
抛出异常    无
**********************************************************************/
BOOL CBillProcess::GetFixedLenBill(BYTE*& pBills, UINT4 &uLeftLen, 
                                   SBill* pABill)
{
    BOOL bSucc = FALSE;

    if (uLeftLen >= m_uOrigLen)
    {
        pABill->nLen = m_uOrigLen;
        pABill->pBillBuf = pBills;

        //修正pBills和uLeftLen的值
        pBills += m_uOrigLen;
        uLeftLen -= m_uOrigLen;

        bSucc = TRUE;
    }
    else if ((uLeftLen > 0) && (uLeftLen < m_uOrigLen))
    //剩下的长度还不够一张话单，则忽略这小部分内容
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
功能描述    在话单包中的话单存放方式为2位长度＋话单内容的情况下，从话单包中拆分
            出一张话单，在GPRS、3G等产品中有效。被PreProcess函数调用
输入参数    pBills----解包当前指针，从此处开始读取话单记录 
            uLeftLen----话单包剩余长度
输出参数    pBills----读取话单记录后的解包当前指针，从此处开始读取话单记录 
            uLeftLen----读取话单记录后的话单包剩余长度
            pABill----读取得话单记录，结构的内存在函数外申请和释放
返回值      有没有读取成功一条话单，标志pABill是否有效
                TRUE----读取成功
                FALSE----读取失败
抛出异常    无
**********************************************************************/
BOOL CBillProcess::GetVarLenBill(BYTE*& pBills, UINT4 &uLeftLen, SBill* pABill)
{
    BOOL bSucc = FALSE;

    if (uLeftLen >= 2)
    {
        UINT2 nBillLen;
        //由于UNIX下的字节对齐问题，不能直接进行赋值，而必须进行内存拷贝
        memcpy(&nBillLen, pBills, 2); 
        nBillLen = NTOHS(nBillLen);

        if (uLeftLen < nBillLen + 2)
        //剩下的长度还不够一张话单，则忽略这小部分内容
        {
            //by ldf 2003-03-24 对应问题单D07718 注释掉此处
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

            //修正pBills和uLeftLen的值
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
功能描述    获得异常话单张数
输入参数    无            
输出参数    无
返回值      异常话单数
抛出异常    无
**********************************************************************/
int CBillProcess::GetErrorCDRCount(void)
{
	return m_nErrorCDRCount;
}

/**********************************************************************
功能描述    复位异常话单张数
输入参数    无            
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CBillProcess::ResetErrorCDRCount()
{
	m_nErrorCDRCount = 0;
}
