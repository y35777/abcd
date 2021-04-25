/**************************************************************************
 *��Ʒ��    : iGateway Bill V200        
 *ģ����    : ����ģ��(Perf)
 *�ļ���    : perf_item.cpp
 *������Ϣ  : ���ļ�����CPerfItem��ʵ��
 *�汾˵��  : V200R001i00M00B00D1130A   
 *��Ȩ��Ϣ  : ��Ȩ����(c) 2001-2002 ��Ϊ�������޹�˾
 *����      : ������
 *����ʱ��  : 2001-11-05
 *�޸ļ�¼  : 
 **************************************************************************/
#include "../include/perf_item.h"

#include "ace/OS.h"

LIST<CPerfItem *> CPerfItem::g_Items;
BOOL CPerfItem::g_bPerfOn = FALSE;
ACE_Mutex CPerfItem::g_Mutex;
LINKSTAT_MAP  CLinkStatItem::g_LinkStat;
ACE_Mutex CLinkStatItem::g_LinkStat_Mutex;

/**************************************************************************
����ԭ��    protected: CPerfItem (const BYTE yAPID, 
                                  const BYTE yModuleID, 
                                  const UINT2 uItemID,
                                  const PERF_STATS_STRATEGY nStatsStrategy);
��������    ���캯����ͨ����̬����CPerfItem::GetItem����CPerfItem����
�������    yAPID: ���̺� 
            yModuleID: ģ���
            uItemID: ����ͳ����ı��
            nStatsStrategy: ͳ�Ʋ���
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    public: void Set ( const UINT4 uNewValue );
��������    ������������
�������    uNewValue: �µ�����ͳ��ֵ
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    public: void Add ( const UINT4 uValueAdded );
��������    ׷����������
�������    uNewValue: ���ӵ�����ͳ��ֵ
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CPerfItem::Add(const UINT4 uValueAdded)
{
	if(g_bPerfOn == FALSE)
		return;

/*  ������ ���  
    if(m_bNewPeriod == TRUE) // �µ�ͳ�����ڿ�ʼ
    {
        m_bNewPeriod = FALSE;
        m_Data.uValue = 0;
    }
*/
    m_Data.uValue += uValueAdded;
}

/**************************************************************************
����ԭ��    public: static CPerfItem* GetItem (const BYTE yAPID, 
                                               const BYTE yModuleID, 
                                               const UINT2 uItemID,
                                               const PERF_STATS_STRATEGY nStatsStrategy);
��������    ���CPerfItemʵ��
�������    yAPID: ���̺� 
            yModuleID: ģ���
            uItemID: ����ͳ����ı��
            nStatsStrategy: ͳ�Ʋ���
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
CPerfItem* CPerfItem::GetItem(const BYTE yAPID, 
							  const BYTE yModuleID, 
							  const UINT2 uItemID,
                              const PERF_STATS_STRATEGY nStatsStrategy)
{
	CPerfItem* pItem = NULL;
	g_Mutex.acquire();

    // ��g_Items�в��ұ��ΪuItemID��CPerfItem����

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

    // g_Items��û�б��ΪuItemID���󣬴���CPerfItem���󲢲����б�

    pItem = new CPerfItem(yAPID, yModuleID, uItemID, nStatsStrategy);
    g_Items.push_back(pItem);

	g_Mutex.release();
    return pItem;
}

/**************************************************************************
����ԭ��    public: static void ReleaseAllItems ( void );
��������    �ͷ�����ʵ��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    public: static void BeginStatistic ( void );
��������    ��������ͳ�����ڵĿ�ʼʱ��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
		������ͳ��.
	*/
	g_bPerfOn = TRUE;
}

/**************************************************************************
����ԭ��    public: static void EndStatistic (SPerfItem*& pAllItems, 
                                              UINT2& uItemCount);
��������    ��������ͳ�����ڵĽ���ʱ��
�������    ��
�������    pAllItems: ��������ͳ������
            uItemCount: ����ͳ��������
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CPerfItem::EndStatistic(SPerfItem*& pAllItems, UINT2& uItemCount)
{
	/*
		���ȹر�����ͳ��,��ֻ��Ϊ���������ͳ�Ƶľ�ȷ��,��Ҳ������ȫ׼ȷ.
		��Ϊ����ͳ�Ʋ�̫�ں����ݵ�׼ȷ��,���ַ���������߳��������.
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

            //���ͳ�Ʋ�����ȡͳ������ľ�ֵ�������ƽ��ֵ
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
����ԭ��    CLinkStatItem ()
��������    ���캯��
�������    
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
CLinkStatItem::CLinkStatItem()
{
   
}
/**************************************************************************
����ԭ��    ~CLinkStatItem()
��������    ��������
�������    
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
CLinkStatItem::~CLinkStatItem()
{
   
}
/**************************************************************************
����ԭ��    ReleaseAllItems()
��������    �ͷ���·����
�������    
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    CommitLinkStat(const BYTE yAPID,SLinkStat LinkStat)
��������    ������ģ�����,ͳ����·״̬.
�������    
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CLinkStatItem::CommitLinkStat(const BYTE yAPID,SLinkStat LinkStat)
{
    g_LinkStat_Mutex.acquire();

	LINKSTAT_LIST* pLinkList = NULL;

	LINKSTAT_MAP::iterator Iterator = g_LinkStat.find(yAPID);

	//���ݽ����������·��������ҵ�
	if (Iterator != g_LinkStat.end())
	{		
		//����ģ�����ƺ���·���Ʋ�����·
		pLinkList = g_LinkStat[yAPID];

		LINKSTAT_LIST::iterator it;
		SLinkStat TempData;
		for(it = pLinkList->begin(); it != pLinkList->end(); it++)
		{
			TempData = *it;
			if (strcmp(TempData.szModuleLabel, LinkStat.szModuleLabel) == 0 
                && strcmp(TempData.szLinkLabel, LinkStat.szLinkLabel) == 0)
			{
				//�ҵ���������·��״̬
				TempData.yStat = LinkStat.yStat;
				break;
			}			
		}

		//û���ҵ�����������·
		if (it == pLinkList->end())
		{	
			pLinkList->push_back(LinkStat);
		}
	}
	else
	{
		//û�д˽���㣬����˽����
	
		pLinkList = new LINKSTAT_LIST;

		pLinkList->push_back(LinkStat);

		g_LinkStat[yAPID] = pLinkList;
	}

    g_LinkStat_Mutex.release();
}
/**************************************************************************
����ԭ��    GetLinkStatData(LINKSTAT_LIST*& pLinkList,const BYTE yAPID)
��������    ������ģ�����,�����·״̬����.
�������    
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/ 
void CLinkStatItem::GetLinkStatData(LINKSTAT_LIST*& pLinkList,const BYTE yAPID)
{
    g_LinkStat_Mutex.acquire();
	
	//���ݽ����Ľ��̺Ų��Ҵ˽�������·״̬	
	LINKSTAT_MAP::iterator Iterator = g_LinkStat.find(yAPID);

	if (Iterator != g_LinkStat.end())
	{
		pLinkList = g_LinkStat[yAPID];
	}

    g_LinkStat_Mutex.release();
}

//End
