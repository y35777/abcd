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
��������    �����ѯ���ģ���MML����ͷ
�������    szCmd----MML�����Ӧ������
            nRetCode----����Ĵ�����
            nSequ----����ͷ�е���ˮ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CQBMMLReport::MakeQBReportHead(const char* szCmd,
                                    int nRetCode, 
                                    int nSequ)
{
    CMMLReport::MakeReportHead();
	CMMLReport::MakeCmdDisplay(szCmd, nSequ, nRetCode, "TASKCMD");
}

/**********************************************************************
��������    �ڱ����м���һ������������ݣ����Ƿ�ҳ���͵�����
�������    TitleList----���ı���
            ContentList----��������
            szHeadName----������������
            nPageNo----��ǰ���͵ı���ҳ��
�������    ContentList----������MML�����ı�����ݣ������ڱ���MML����
                            ��δ�����͵��ı������
            nPageNo----������MML�����ĵ�ǰ���ҳ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CQBMMLReport::AddQBHoriTable(LIST<STRING>& TitleList, 
                                  LIST<STRING>& ContentList, 
                                  const char* szHeadName,
                                  int& nPageNo)
{
    ASSERT(0 == ContentList.size() % TitleList.size());
    
	LIST<STRING> SendContentList;   //����MML����Ҫ���͵ı������
    LIST<STRING>::iterator ContentLastIter = ContentList.begin();
    //���ݱ�����Ĵ�С���ƾ���Ҫ���Ͷ�������¼������Ϊ1��
    //�ھ����Ĺ����У�ֻ�ǽ���һ��ģ���ĳߴ��жϣ���������
    //�ո񡢻س����ַ�ռ�õĳ��ȣ�����ʵ�ʵı�����ߴ�
    //�����ܻ���ڴ�С������
    int nSize = m_nReportLen;
    int nFieldCount = TitleList.size(); //һ����¼�е������
	//������ 2002-07-30 ����ÿ����ĳ��Ⱥ�һ�еĳ���
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
    //�޸Ľ���
    //��MML�����м���Ҫ���͵ı������
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
		//���б��ķ��ͽ�����ɾ��������Ϣ������һ�����·���
		delete[] m_pLenArray;
		m_pLenArray = NULL;
		m_nLineLen = 0;
	}
}

/**********************************************************************
��������    �ڱ����м���һ������������ݣ����Ƿ�ҳ���͵�����
�������    TitleList----���ı���
            ContentList----��������
            RecTitleList----������¼������
�������    ContentList----������MML�����ı�����ݣ������ڱ���MML����
                            ��δ�����͵��ı������
            RecTitleList----������MML�����ļ�¼���֣������ڱ���MML����
                            ��δ�����͵��ļ�¼������
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CQBMMLReport::AddQBVertTable(LIST<STRING>& TitleList, 
                                  LIST<STRING>& ContentList, 
                                  LIST<STRING>& RecHeadList)
{
    ASSERT(RecHeadList.size() == (ContentList.size() / TitleList.size()));

    LIST<STRING> SendContentList;   //����MML����Ҫ���͵ı������
    LIST<STRING> SendRecHeadList;  //����MML����Ҫ���͵ļ�¼������
    LIST<STRING>::iterator ContentLastIter = ContentList.begin();
	LIST<STRING>::iterator RecHeadLastIter = RecHeadList.begin();
    //���ݱ�����Ĵ�С���ƾ���Ҫ���Ͷ�������¼������Ϊ1��
    //�ھ����Ĺ����У�ֻ�ǽ���һ��ģ���ĳߴ��жϣ���������
    //�ո񡢻س����ַ�ռ�õĳ��ȣ�����ʵ�ʵı�����ߴ�
    //�����ܻ���ڴ�С������

    //������ÿ����¼����ʾ�б��ⲿ��ռ�õĳ���
    int nSize = m_nReportLen;
    int nFieldCount = TitleList.size(); //һ����¼�е������
	//������ 2002-07-30 D15553
	if(m_nRecordLen == 0)
	{
		m_nRecordLen = CalcVertTableRecordLen(TitleList,ContentList,RecHeadList);
    }

    while (nSize + m_nRecordLen <= m_nSizeLimit 
		   && ContentLastIter != ContentList.end())
    //���������Ĵ�С��С�����ƣ����ټ���һ����¼
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
    //��MML�����м���Ҫ���͵ı������
    AddVertTable(TitleList, SendContentList, SendRecHeadList);

    //�ڱ����м������β����β�б����Ƿ���ں�������
    if (!ContentList.empty())
    {
        AddString(S_QB_MML_TO_BE_CONTINUE);
    }
	else
	{
		m_nRecordLen = 0;
	}
}
