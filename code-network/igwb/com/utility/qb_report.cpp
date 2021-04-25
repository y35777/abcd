//file: qb_report.cpp
#include "qb_report.h"

CQBMMLReport::CQBMMLReport() : CMMLReport(S_QB_MML_IGWB)
{
    m_nSizeLimit = MAX_REPORT_SIZE;
	m_pLenArray = NULL;
	m_nLineLen = 0;
	m_nRecordLen = 0;
}

CQBMMLReport::~CQBMMLReport()
{
    if(m_pLenArray != NULL)
	{
		delete[] m_pLenArray;
	}
}

/**********************************************************************
功能描述    构造查询浏览模块的MML报告头
输入参数    szCmd----MML报告对应的命令
            nRetCode----报告的错误码
            nSequ----报告头中的流水号
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CQBMMLReport::MakeQBReportHead(const char* szCmd,
                                    int nRetCode, 
                                    int nSequ)
{
    CMMLReport::MakeReportHead();
	CMMLReport::MakeCmdDisplay(szCmd, nSequ, nRetCode, "TASKCMD");
}

/**********************************************************************
功能描述    在报告中加入一个横向表格的内容，考虑分页传送的问题
输入参数    TitleList----表格的标题
            ContentList----表格的内容
            szHeadName----整个表格的名字
            nPageNo----当前发送的表格的页码
输出参数    ContentList----发送了MML报告后的表格内容，包含在本次MML报告
                            中未被发送掉的表格内容
            nPageNo----发送了MML报告后的当前表格页码
返回值      无
抛出异常    无
**********************************************************************/
void CQBMMLReport::AddQBHoriTable(LIST<STRING>& TitleList, 
                                  LIST<STRING>& ContentList, 
                                  const char* szHeadName,
                                  int& nPageNo)
{
    ASSERT(0 == ContentList.size() % TitleList.size());
    
	LIST<STRING> SendContentList;   //本次MML报告要发送的表格内容
    LIST<STRING>::iterator ContentLastIter = ContentList.begin();
    //根据报文体的大小限制决定要发送多少条记录，至少为1条
    //在决定的过程中，只是进行一种模糊的尺寸判断，并不计算
    //空格、回车等字符占用的长度，所以实际的报文体尺寸
    //将可能会大于大小的限制
    int nSize = m_nReportLen;
    int nFieldCount = TitleList.size(); //一条记录中的域个数
	//张玉新 2002-07-30 计算每个域的长度和一行的长度
	if(m_pLenArray == NULL)
	{
		m_pLenArray = new int[nFieldCount];
		m_nLineLen = CalcHoriTableFieldLens(TitleList,
			                 ContentList,m_pLenArray);
	}
	
    while(nSize + m_nLineLen < m_nSizeLimit 
		&& ContentLastIter != ContentList.end()) 
    {
        for (int i = 0; i < nFieldCount; i++)
        {
            ContentLastIter++;
        }
		nSize += m_nLineLen;
    }
    SendContentList.splice(SendContentList.end(),
		                   ContentList,
		                   ContentList.begin(),
						   ContentLastIter);
    //修改结束
    //在MML报告中加入要发送的表格内容
    char szTableTitle[255];
    SNPRINTF(szTableTitle, 255, S_QB_MML_TABLE_TITLE, szHeadName, nPageNo);
	szTableTitle[254] = '\0';
	nPageNo++;
    AddHoriTable(TitleList, SendContentList, szTableTitle,m_pLenArray);

    if (!ContentList.empty())
    {
        AddString(S_QB_MML_TO_BE_CONTINUE);
    }
	else
	{
		//所有报文发送结束后删除长度信息便于下一次重新分配
		delete[] m_pLenArray;
		m_pLenArray = NULL;
		m_nLineLen = 0;
	}
}

/**********************************************************************
功能描述    在报告中加入一个纵向表格的内容，考虑分页传送的问题
输入参数    TitleList----表格的标题
            ContentList----表格的内容
            RecTitleList----各条记录的名字
输出参数    ContentList----发送了MML报告后的表格内容，包含在本次MML报告
                            中未被发送掉的表格内容
            RecTitleList----发送了MML报告后的记录名字，包含在本次MML报告
                            中未被发送掉的记录的名字
返回值      无
抛出异常    无
**********************************************************************/
void CQBMMLReport::AddQBVertTable(LIST<STRING>& TitleList, 
                                  LIST<STRING>& ContentList, 
                                  LIST<STRING>& RecHeadList)
{
    ASSERT(RecHeadList.size() == (ContentList.size() / TitleList.size()));

    LIST<STRING> SendContentList;   //本次MML报告要发送的表格内容
    LIST<STRING> SendRecHeadList;  //本次MML报告要发送的记录的名字
    LIST<STRING>::iterator ContentLastIter = ContentList.begin();
	LIST<STRING>::iterator RecHeadLastIter = RecHeadList.begin();
    //根据报文体的大小限制决定要发送多少条记录，至少为1条
    //在决定的过程中，只是进行一种模糊的尺寸判断，并不计算
    //空格、回车等字符占用的长度，所以实际的报文体尺寸
    //将可能会大于大小的限制

    //计算在每条记录的显示中标题部分占用的长度
    int nSize = m_nReportLen;
    int nFieldCount = TitleList.size(); //一条记录中的域个数
	//张玉新 2002-07-30 D15553
	if(m_nRecordLen == 0)
	{
		m_nRecordLen = CalcVertTableRecordLen(TitleList,ContentList,RecHeadList);
    }

    while (nSize + m_nRecordLen <= m_nSizeLimit 
		   && ContentLastIter != ContentList.end())
    //如果报文体的大小还小于限制，则再加入一条记录
    {
        for (int i = 0; i < nFieldCount; i++)
        {
            ContentLastIter ++;       
        }
        RecHeadLastIter ++;
        nSize += m_nRecordLen;
    }
    SendContentList.splice(SendContentList.end(),
		                   ContentList,
						   ContentList.begin(),
						   ContentLastIter);
    SendRecHeadList.splice(SendRecHeadList.end(),
		                   RecHeadList,
						   RecHeadList.begin(),
						   RecHeadLastIter);
    //在MML报告中加入要发送的表格内容
    AddVertTable(TitleList, SendContentList, SendRecHeadList);

    //在报告中加入表格的尾，在尾中表明是否存在后续报告
    if (!ContentList.empty())
    {
        AddString(S_QB_MML_TO_BE_CONTINUE);
    }
	else
	{
		m_nRecordLen = 0;
	}
}
