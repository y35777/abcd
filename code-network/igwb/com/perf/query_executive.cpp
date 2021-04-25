// query_executive.cpp: implementation of the CQueryExecutive class.
//
//////////////////////////////////////////////////////////////////////
#include "query_executive.h"
STaskInfo g_TaskInfo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CQueryExecutive::CQueryExecutive()
{
    m_nPageLimit = 48*1024;
}

CQueryExecutive::~CQueryExecutive()
{

}

int CQueryExecutive::Init(STRING &strPerfRoot,CTaskManager *pManager)
{
    m_szPerfRoot = strPerfRoot;
    m_pManager = pManager;
    return TRUE;
}

int CQueryExecutive::ListTaskResult(CParseResult *pParse)
{
    try{
        //1.ȡ��TSKN��RDS��RTS��RDE��RTE������������ѯ��ʼ���ڣ�
        //��ѯ��ʼʱ�䣬��ѯ�������ںͲ�ѯ����ʱ�䣩5������
        //�����������ҵ���Ӧ������������Ŀ¼���ڸ�����ʼ���ں�
        //�������ڽ�����Ҫ������������ļ���ȫ·�����Ƽ��뵽
        //m_ListFileName���У�ÿ�μ���ǰҪ��գ�
        STRING strTaskName;
        char szRDS[9],szRDE[9],szRTS[9],szRTE[9];
        int nRDS[3], nRTS[2], nRDE[3], nRTE[2];
        //ȡ�������� ��ѡ
        strTaskName = pParse->GetStringVal(1);
        //ȡ��ѯ��ʼ���� ��ѡ
        BOOL nResultTmep;
		nResultTmep = pParse->GetDigitVal(&nRDS[0],2,1);
		if (!nResultTmep)
		{
			nRDS[0] = 0;
		}
        nResultTmep = pParse->GetDigitVal(&nRDS[1],2,2);
		if (!nResultTmep)
		{
			nRDS[1] = 0;
		}
        nResultTmep = pParse->GetDigitVal(&nRDS[2],2,3);
		if (!nResultTmep)
		{
			nRDS[2] = 0;
		}
        SNPRINTF(szRDS,9,"%04d%02d%02d",nRDS[0],nRDS[1],nRDS[2]);
        szRDS[8] = 0;
        //ȡ��ѯ��ʼʱ�� ��ѡ
        nResultTmep = pParse->GetDigitVal(&nRTS[0],3,1);
		if (!nResultTmep)
		{
			nRTS[0] = 0;
		}
        nResultTmep = pParse->GetDigitVal(&nRTS[1],3,2);
		if (!nResultTmep)
		{
			nRTS[1] = 0;
		}
        SNPRINTF(szRTS,9,"%02d:%02d:%02d",nRTS[0],nRTS[1],0);
        szRTS[8] = 0;
        //ȡ��ѯ�������� ��ѡ
        nResultTmep = pParse->GetDigitVal(&nRDE[0],4,1);
		if (!nResultTmep)
		{
			nRDE[0] = 9999;
		}
        nResultTmep = pParse->GetDigitVal(&nRDE[1],4,2);
		if (!nResultTmep)
		{
			nRDE[1] = 12;
		}
        nResultTmep = pParse->GetDigitVal(&nRDE[2],4,3);
		if (!nResultTmep)
		{
			nRDE[2] = 31;
		}
        SNPRINTF(szRDE,9,"%04d%02d%02d",nRDE[0],nRDE[1],nRDE[2]);
        szRDE[8] = 0;
        //ȡ��ѯ����ʱ�� ��ѡ
        nResultTmep = pParse->GetDigitVal(&nRTE[0],5,1);
		if (!nResultTmep)
		{
			nRTE[0] = 23;
		}
        nResultTmep = pParse->GetDigitVal(&nRTE[1],5,2);
		if (!nResultTmep)
		{
			nRTE[1] = 59;
		}
        SNPRINTF(szRTE,9,"%02d:%02d:%02d",nRTE[0],nRTE[1],0);
        szRTE[8] = 0;

        char szPerfTaskPath[MAX_PATH];
        SNPRINTF(szPerfTaskPath,MAX_PATH,"%s/%s",
            m_szPerfRoot.c_str(),strTaskName.c_str());
        GenerateFileList(szPerfTaskPath,szRDS,szRDE);
        //2.���ζ�������е��ļ���������ļ���ʼ��m_DdaSet��Ȼ
        //���������m_DdaSet��ʼ��m_Query���������е�ʱ�������
        //Ϊ�����󶨵�m_Query�ϣ����ò�ѯ�����ѯ���������Ľ����
        //����MML���ģ����ĸ�ʽ�μ���CG��M2000�ӿ�MML�������ĵ�����
        //����һҳ�����������ɶ�ҳ���ġ�
        LIST<STRING>::iterator it;
        STRING szTemp;
        char szStatisticDate[32];
        char szTmp[MAX_PATH];
        char szTmp2[16];
        char *pPos = NULL;
        int nDate = 0;
        memset(szTmp,0,MAX_PATH);
        memset(szTmp2,0,16);
        int nItem = 0;
        int nTmp = 0;
        int nTotalCnt = 0;
        int i = 0; //ѭ������
        bool bFlag = true;//�Ƿ��ǵ�һ���ļ��ı�־
        //���ڲ�ѯ����û��������Ϣ������Ҫ��Manager����ָ����Ϣ
        LIST<STRING> listCounter;
        LIST<STRING>::iterator CounterIt; 
		int nReportTmp = g_TaskInfo.nReportCsn;
		g_TaskInfo.nReportCsn ++;

        m_pManager->QueryCounterInfo(strTaskName,listCounter);
        g_TaskInfo.g_pReport->MakeReportHead();
        g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),nReportTmp,0,"O&M");        
		//by ldf
		g_TaskInfo.g_pReport->AddString(S_PERF_TSK_RES);
		g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
		
		int nStrLen = strlen(S_PERF_TSK_RES);
        int j;
		for(j = 0; j < nStrLen; j++)
		{
			g_TaskInfo.g_pReport->AddString("-");
		}

		BOOL bOnlyOneFile = FALSE;
		if(m_listFileName.size() == 1)
		{
			bOnlyOneFile = TRUE;
		}

        while(!m_listFileName.empty())
        {
            it = m_listFileName.begin();
            if(!m_DdaSet.LoadFromFile(*it))
                throw strerror(errno);
            m_Query.Attach(&m_DdaSet);
            if(bFlag)
            {
				//by ldf 2003-12-24 19:44 
				char szFirstFileDate[9];
				strncpy(szTmp,(*it).c_str(),MAX_PATH);
				pPos = strrchr(szTmp,'/');
				pPos++;
				memcpy(szFirstFileDate,pPos,8);
				szFirstFileDate[8] = 0;
				
				int nFirstFileDate = atoi(szFirstFileDate);
				int nStartDate  = atoi(szRDS);
				
				if( nStartDate < nFirstFileDate )
				{
					SNPRINTF(szRTS,9,"%02d:%02d:%02d",0,0,0);
					szRTS[8] = 0;
				}
				//ֻ�е�һ����ļ��ż���STIME
                m_Query.ClearBind();
                m_Query.AddBindAnd("STIME",szRTS,opEqGtr);
                bFlag = FALSE;
            }
            if(m_listFileName.size() == 1)
            {
				//by ldf 2003-12-24 19:44 
				char szLastFileDate[9];
				strncpy(szTmp,(*it).c_str(),MAX_PATH);
				pPos = strrchr(szTmp,'/');
				pPos++;
				memcpy(szLastFileDate,pPos,8);
				szLastFileDate[8] = 0;

				int nLastFileDate = atoi(szLastFileDate);
				int nEndDate  = atoi(szRDE);
				
				if( nEndDate > nLastFileDate )
				{
					SNPRINTF(szRTE,9,"%02d:%02d:%02d",23,59,59);
					szRTE[8] = 0;
				}
				
                //ֻ�������һ��ʱ����Ҫ����ʱ��
				if(!bOnlyOneFile)
				{
                    m_Query.ClearBind();
				}
                //m_Query.AddBindAnd("ETIME",szRTE,opEqLess);
				//by ldf ��ѯ�������Ƶ�����һ����starttimeΪ׼
				m_Query.AddBindAnd("STIME",szRTE,opEqLess);
            }
            else
            {
                //���ڴ����м��ʱ��
                m_Query.ClearBind();
            }
            m_Query.DoQuery();
            strncpy(szTmp,(*it).c_str(),MAX_PATH);
			//by ldf 2003-12-24 M2000����
            pPos = strrchr(szTmp,'/');
            pPos++;
            memcpy(szTmp2,pPos,8);
            szTmp2[8] = 0;
            nDate = atoi(szTmp2);
            SNPRINTF(szTmp, MAX_PATH, "%04d-%02d-%02d ",
                     nDate/10000, (nDate%10000)/100, nDate%100);
            strncpy(szStatisticDate,szTmp,32);
            int nPos = 0; //��¼λ��
            int nRecordCount = m_Query.m_ResultArray.size(); //��¼����
            while(nPos < nRecordCount)
            {
                for(i = nPos; i < nRecordCount; i++)
                {
                    nItem = m_Query.m_ResultArray[i];
                    strcpy(&szStatisticDate[11],m_DdaSet.GetFieldStringValue(nItem,"STIME").c_str());
                    g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
                    g_TaskInfo.g_pReport->MakeReportItem(S_PERF_TSKN,strTaskName.c_str(),24,ALIGN_RIGHT);
                    g_TaskInfo.g_pReport->MakeReportItem(S_PERF_STATISTIC_START,szStatisticDate,24,ALIGN_RIGHT);
                    szTemp = m_pManager->QueryTaskInfo(strTaskName,STRING("PRD"));
                    g_TaskInfo.g_pReport->MakeReportItem(S_PERF_PRD,szTemp.c_str(),24,ALIGN_RIGHT);
                    szTemp = m_pManager->QueryTaskInfo(strTaskName,STRING("MU"));
                    g_TaskInfo.g_pReport->MakeReportItem(S_PERF_MU,szTemp.c_str(),24,ALIGN_RIGHT);
                    szTemp = m_pManager->QueryTaskInfo(strTaskName,STRING("OIV"));
					
					strncpy(szTmp,szTemp.c_str(),MAX_PATH);
					char *pTok = strtok(szTmp,"&");
					int nOIVNum = 1;
					
					while(pTok)
					{
						char szOIVTitle[MAX_PATH];
						
						//by ldf 2004-04-14
						SNPRINTF(szOIVTitle,MAX_PATH,"%s",S_PERF_OIV);
						//end
						
						g_TaskInfo.g_pReport->MakeReportItem(szOIVTitle,pTok,24,ALIGN_RIGHT);
						
						pTok = strtok(NULL,"&");
						nOIVNum++;
					}
					//g_TaskInfo.g_pReport->MakeReportItem(S_PERF_OIV,szTemp,24,ALIGN_RIGHT);

                    //����ָ����Ϣ
                    CounterIt = listCounter.begin();
                    for(j = 1; CounterIt != listCounter.end(); j++, CounterIt ++)
                    {
                        SNPRINTF(szTmp,MAX_PATH,"COUNTER%d",j);
                        szTemp = m_DdaSet.GetFieldStringValue(nItem,szTmp);
                        g_TaskInfo.g_pReport->MakeReportItem((*CounterIt).c_str(),
                                                             szTemp.c_str(),
                                                             24,
                                                             ALIGN_RIGHT);
                    }
                    //g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
                    nPos ++;
                    if(g_TaskInfo.g_pReport->GetReportLen() >= m_nPageLimit)
                    {
                        if(nPos >= nRecordCount && m_listFileName.size() == 1)
                        {
                            continue; //��Ϊ3��ѭ����������������,��һֱ����
                        }
                        //�������к�̱��������β��
                        g_TaskInfo.g_pReport->AddString(S_PERF_REPORT_MORE);
                        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
                        g_TaskInfo.g_pReport->MakeReportTail();
                        SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
                        g_TaskInfo.g_pReport->MakeReportHead();
                        g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),nReportTmp,0,"O&M");
						g_TaskInfo.g_pReport->AddString(S_PERF_TSKRES_CONTINUE);
						g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
						
						int nStrLen = strlen(S_PERF_TSKRES_CONTINUE);
						for(int j = 0; j < nStrLen; j++)
						{
							g_TaskInfo.g_pReport->AddString("-");
						}
                        break;
                    }
                } //for Report
                                
            } //while Pos ���ѭ������ֹ���ļ��г��ֱ��Ĺ�������
            nTotalCnt += m_Query.m_ResultArray.size();
            //3.���m_DdaSet����m_ListFileName����ʹ�ù����ļ�
            m_DdaSet.Close();
            m_listFileName.pop_front();
        } //while �ļ�
        //�������յĽ�β����
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        SNPRINTF(szTmp,MAX_PATH,S_PERF_RESULT_NUM,nTotalCnt);
        g_TaskInfo.g_pReport->AddString(szTmp);
        g_TaskInfo.g_pReport->AddString(STR_NEWLINE);
        g_TaskInfo.g_pReport->MakeReportTail();
        SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
    } // try 
    catch(const char *pError)
    {
        g_TaskInfo.g_pReport->MakeReportHead();
        g_TaskInfo.g_pReport->MakeCmdDisplay(pParse->GetCmdLine(),g_TaskInfo.nReportCsn++,-1,"O&M");    
        g_TaskInfo.g_pReport->MakeReportItem(S_PERF_FAIL_REASON,pError);
        g_TaskInfo.g_pReport->MakeReportTail();
        SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
        return FALSE;
    }
    return TRUE;
}

void CQueryExecutive::GenerateFileList(const char *szPerfTaskPath, char *szRDS, const char *szRDE)
{
    int nStartDate = atoi(szRDS);
    int nEndDate = atoi(szRDE);
    int nFileDate = 0;
    
    //�����ļ��б�
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    
    m_listFileName.clear();
    pDir = ACE_OS::opendir(szPerfTaskPath);
    if(pDir == NULL)
    {
        return;
    };
    char szSrcFilePath[MAX_PATH];
    
    //�ѱ����������ļ�
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        if(strcmp(pDirent->d_name,".") == 0 
            || strcmp(pDirent->d_name,"..") == 0)
        {
            continue;
        }
        //���ļ�������ȡʱ��
        char szTmp[16];
        memset(szTmp,0,16);
        memcpy(szTmp,pDirent->d_name,8);
        nFileDate = atoi(szTmp);
        //�ļ���,ʱ��
        if(nFileDate >= nStartDate && nFileDate <= nEndDate)
        {
            //�γ�Դ�ļ�·�� /var/other/perf/task1/20021203_task1.prf
            SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",szPerfTaskPath,pDirent->d_name);
            szSrcFilePath[MAX_PATH - 1] = 0;
            //�Ž�����
            m_listFileName.push_back(szSrcFilePath);
        }
    }
    ACE_OS::closedir(pDir);
}
