/**************************************************************************
 *产品名    : iGateway Bill V200        
 *模块名    : 性能模块(Perf)
 *文件名    : perf_item.cpp
 *描述信息  : 本文件是类CPerfItem的实现
 *版本说明  : V200R001i00M00B00D1130A   
 *版权信息  : 版权所有(c) 2001-2002 华为技术有限公司
 *作者      : 方卫华
 *创建时间  : 2001-11-05
 *修改记录  : 
 **************************************************************************/
#include "../include/perf_item.h"

#include "ace/OS.h"

LIST<CPerfItem *> CPerfItem::g_Items;
BOOL CPerfItem::g_bPerfOn = FALSE;
ACE_Mutex CPerfItem::g_Mutex;
LINKSTAT_MAP  CLinkStatItem::g_LinkStat;
ACE_Mutex CLinkStatItem::g_LinkStat_Mutex;

/**************************************************************************
函数原型    protected: CPerfItem (const BYTE yAPID, 
                                  const BYTE yModuleID, 
                                  const UINT2 uItemID,
                                  const PERF_STATS_STRATEGY nStatsStrategy);
功能描述    构造函数，通过静态函数CPerfItem::GetItem创建CPerfItem对象
输入参数    yAPID: 进程号 
            yModuleID: 模块号
            uItemID: 性能统计项的编号
            nStatsStrategy: 统计策略
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CPerfItem::CPerfItem(const BYTE yAPID, const BYTE yModuleID, const UINT2 uItemID,
                     const PERF_STATS_STRATEGY nStatsStrategy)
{
    m_Data.yAPID     = yAPID;
    m_Data.yModuleID = yModuleID;
    m_Data.uID       = uItemID;
    ACE_OS::time(&(m_Data.timeBegin));
    m_Data.timeEnd   = m_Data.timeBegin;
    m_Data.uValue    = 0;

	m_bNewPeriod     = FALSE;
    m_nStatsStrategy = nStatsStrategy;
}

/**************************************************************************
函数原型    public: void Set ( const UINT4 uNewValue );
功能描述    设置性能数据
输入参数    uNewValue: 新的性能统计值
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerfItem::Set(const UINT4 uNewValue)
{
	if(g_bPerfOn == FALSE)
		return;

    m_Data.uValue    = uNewValue;
    ACE_OS::time(&(m_Data.timeBegin));
    m_Data.timeEnd   = m_Data.timeBegin;  
}

/**************************************************************************
函数原型    public: void Add ( const UINT4 uValueAdded );
功能描述    追加性能数据
输入参数    uNewValue: 增加的性能统计值
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerfItem::Add(const UINT4 uValueAdded)
{
	if(g_bPerfOn == FALSE)
		return;

/*  张玉新 清除  
    if(m_bNewPeriod == TRUE) // 新的统计周期开始
    {
        m_bNewPeriod = FALSE;
        m_Data.uValue = 0;
    }
*/
    m_Data.uValue += uValueAdded;
}

/**************************************************************************
函数原型    public: static CPerfItem* GetItem (const BYTE yAPID, 
                                               const BYTE yModuleID, 
                                               const UINT2 uItemID,
                                               const PERF_STATS_STRATEGY nStatsStrategy);
功能描述    获得CPerfItem实例
输入参数    yAPID: 进程号 
            yModuleID: 模块号
            uItemID: 性能统计项的编号
            nStatsStrategy: 统计策略
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CPerfItem* CPerfItem::GetItem(const BYTE yAPID, 
							  const BYTE yModuleID, 
							  const UINT2 uItemID,
                              const PERF_STATS_STRATEGY nStatsStrategy)
{
	CPerfItem* pItem = NULL;
	g_Mutex.acquire();

    // 在g_Items中查找编号为uItemID的CPerfItem对象

    for(LIST<CPerfItem *>::iterator Iterator = g_Items.begin();
									Iterator != g_Items.end(); Iterator++)
    {
		pItem = *Iterator;
        if((pItem->m_Data.yAPID == yAPID)
			&& (pItem->m_Data.yModuleID == yModuleID)
			&& (pItem->m_Data.uID == uItemID))
        {
			g_Mutex.release();
            return  pItem;
        }
    }

    // g_Items中没有编号为uItemID对象，创建CPerfItem对象并插入列表

    pItem = new CPerfItem(yAPID, yModuleID, uItemID, nStatsStrategy);
    g_Items.push_back(pItem);

	g_Mutex.release();
    return pItem;
}

/**************************************************************************
函数原型    public: static void ReleaseAllItems ( void );
功能描述    释放所有实例
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerfItem::ReleaseAllItems(void)
{
	g_Mutex.acquire();

    for(LIST<CPerfItem *>::iterator Iterator = g_Items.begin();
									Iterator != g_Items.end(); Iterator++)
    {
        delete (*Iterator);
    }

    g_Items.clear();
	g_Mutex.release();
}

/**************************************************************************
函数原型    public: static void BeginStatistic ( void );
功能描述    设置性能统计周期的开始时刻
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerfItem::BeginStatistic(void)
{
	g_Mutex.acquire();
    CPerfItem *pPerfItem = NULL;
    LIST<CPerfItem*>::iterator Iterator = g_Items.begin();
    for(; Iterator != g_Items.end(); Iterator++)
    {
        pPerfItem = (*Iterator);
        ACE_OS::time(&(pPerfItem->m_Data.timeBegin));
        pPerfItem->m_Data.timeEnd   = (*Iterator)->m_Data.timeBegin;
        pPerfItem->m_bNewPeriod     = TRUE;
        pPerfItem->m_Data.uValue    = 0;
	}

	g_Mutex.release();

	/*
		打开性能统计.
	*/
	g_bPerfOn = TRUE;
}

/**************************************************************************
函数原型    public: static void EndStatistic (SPerfItem*& pAllItems, 
                                              UINT2& uItemCount);
功能描述    设置性能统计周期的结束时刻
输入参数    无
输出参数    pAllItems: 所有性能统计数据
            uItemCount: 性能统计项数量
返回值      无
抛出异常    无
***************************************************************************/
void CPerfItem::EndStatistic(SPerfItem*& pAllItems, UINT2& uItemCount)
{
	/*
		首先关闭性能统计,这只是为了提高性能统计的精确度,但也不是完全准确.
		因为性能统计不太在乎数据的准确性,这种方法可以提高程序的性能.
	*/
	g_bPerfOn = FALSE;

	g_Mutex.acquire();

    if(g_Items.empty())
    {
        pAllItems = NULL;
        uItemCount = 0;
    }
    else
    {
        uItemCount = g_Items.size();
        pAllItems = new SPerfItem[uItemCount];

        int i = 0;
        int nSpan = 0;
        CPerfItem *pPerfItem = NULL;
        LIST<CPerfItem*>::iterator Iterator = g_Items.begin();
        for(; Iterator != g_Items.end(); Iterator++,i++)
        {
            pPerfItem = (*Iterator);

            //如果统计策略是取统计区间的均值，则计算平均值
            if (pPerfItem->GetStatsStrategy() == PERF_STATS_AVERAGE_VALUE)
            {
                ACE_OS::time(&(pPerfItem->m_Data.timeEnd));
                nSpan = pPerfItem->m_Data.timeEnd - pPerfItem->m_Data.timeBegin;
                if (nSpan > 0)
                {
                    pPerfItem->m_Data.uValue /= nSpan;
                }
                else
                {
                    pPerfItem->m_Data.uValue = 0;
                }
            }

            memcpy((void *)&(pAllItems[i]),(void *)&(pPerfItem->m_Data),sizeof(SPerfItem));
        }
    }

	g_Mutex.release();
}

void CPerfItem::SetPerfOn(BOOL bOption)
{
	g_bPerfOn = bOption;
}

PERF_STATS_STRATEGY CPerfItem::GetStatsStrategy(void)
{
    return m_nStatsStrategy;
}

/**************************************************************************
函数原型    CLinkStatItem ()
功能描述    构造函数
输入参数    
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CLinkStatItem::CLinkStatItem()
{
   
}
/**************************************************************************
函数原型    ~CLinkStatItem()
功能描述    析构函数
输入参数    
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CLinkStatItem::~CLinkStatItem()
{
   
}
/**************************************************************************
函数原型    ReleaseAllItems()
功能描述    释放链路链表
输入参数    
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CLinkStatItem::ReleaseAllItems()
{
	g_LinkStat_Mutex.acquire();

	LINKSTAT_LIST* pLinkList = NULL;
	LINKSTAT_MAP::iterator Iterator;
    for(Iterator = g_LinkStat.begin();Iterator != g_LinkStat.end(); Iterator++)
    {
		pLinkList = (*Iterator).second;
		pLinkList->clear();
		delete pLinkList;
    }
    g_LinkStat.clear();
	g_LinkStat_Mutex.release();


}
/**************************************************************************
函数原型    CommitLinkStat(const BYTE yAPID,SLinkStat LinkStat)
功能描述    由网络模块调用,统计链路状态.
输入参数    
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CLinkStatItem::CommitLinkStat(const BYTE yAPID,SLinkStat LinkStat)
{
    g_LinkStat_Mutex.acquire();

	LINKSTAT_LIST* pLinkList = NULL;

	LINKSTAT_MAP::iterator Iterator = g_LinkStat.find(yAPID);

	//根据接入点找其链路链表，如果找到
	if (Iterator != g_LinkStat.end())
	{		
		//根据模块名称和链路名称查找链路
		pLinkList = g_LinkStat[yAPID];

		LINKSTAT_LIST::iterator it;
		SLinkStat TempData;
		for(it = pLinkList->begin(); it != pLinkList->end(); it++)
		{
			TempData = *it;
			if (strcmp(TempData.szModuleLabel, LinkStat.szModuleLabel) == 0 
                && strcmp(TempData.szLinkLabel, LinkStat.szLinkLabel) == 0)
			{
				//找到，更新链路的状态
				TempData.yStat = LinkStat.yStat;
				break;
			}			
		}

		//没有找到，增加新链路
		if (it == pLinkList->end())
		{	
			pLinkList->push_back(LinkStat);
		}
	}
	else
	{
		//没有此接入点，加入此接入点
	
		pLinkList = new LINKSTAT_LIST;

		pLinkList->push_back(LinkStat);

		g_LinkStat[yAPID] = pLinkList;
	}

    g_LinkStat_Mutex.release();
}
/**************************************************************************
函数原型    GetLinkStatData(LINKSTAT_LIST*& pLinkList,const BYTE yAPID)
功能描述    由性能模块调用,获得链路状态数据.
输入参数    
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/ 
void CLinkStatItem::GetLinkStatData(LINKSTAT_LIST*& pLinkList,const BYTE yAPID)
{
    g_LinkStat_Mutex.acquire();
	
	//根据接入点的进程号查找此接入点的链路状态	
	LINKSTAT_MAP::iterator Iterator = g_LinkStat.find(yAPID);

	if (Iterator != g_LinkStat.end())
	{
		pLinkList = g_LinkStat[yAPID];
	}

    g_LinkStat_Mutex.release();
}

//End
