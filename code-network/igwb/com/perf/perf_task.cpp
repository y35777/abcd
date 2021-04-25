// perf_task.cpp: implementation of the CPerfTask class.
//
//////////////////////////////////////////////////////////////////////

#include "perf_task.h"
#include "perf_task_global.h" 

STaskInfo g_TaskInfoBroad;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPerfTask::CPerfTask()
{
    m_pSave = new CSaveProxy;
    m_tStart = 0;
    m_tEnd = 0;
    m_nStatisticCount = 0;
}

CPerfTask::~CPerfTask()
{

}

int CPerfTask::Init(SInformation *pInfo, STRING szFilePath)
{
    //m_TaskInfo = pInfo;
    int nPos = szFilePath.find_last_of("/");
    if(nPos == STRING::npos)
    {
        nPos = szFilePath.find_last_of("\\");
        if(nPos == STRING::npos)
        {   
            m_szError = "Status file path is invalid!"; 
            return FALSE;
        }
    }
    m_szPerfFilePath = szFilePath.substr(0,nPos +1);
    m_szPerfFilePath += pInfo->m_szTaskName;
    m_pTaskInfo = pInfo;
    m_tStart = 0;
    m_tEnd = 0;
    m_nStatisticCount = 0;
    //m_Save.Init(������,szFilePath); ������ֵ����m_pDdaSet�У�
    //�ɹ�(����ֵ����NULL)�ͷ���TRUE�����򷵻�FALSE.
    CounterMap *pCounterMap = &(pInfo->m_mCouterMap);
    
    int nRes = m_pSave->Init(pInfo->m_szTaskName,m_szPerfFilePath,pCounterMap);
    if(nRes != TRUE)
    {
        m_szError = m_pSave->GetLastError();
        return FALSE;
    }
    return TRUE;
}
//�������
int CPerfTask::ProcStatistic(SPerfItem *pPerfs, int nCount)
{
    //1.�ж�����״̬(m_pTaskInfo->m_nStatus)�����Ϊ�����
    //ֱ�ӷ���TRUE������ִ����һ����
    if(m_pTaskInfo->m_nStatus == TASK_SUSPEND)
        return TRUE;
    //2.����Expire()�ж������Ƿ���ڣ��������ֵΪTURE��
    //��ʾ���ڣ�ִ�е�4����������Ϊ�˱�֤���һ��ͳ���ܹ�
    //��ȷ���̣�,����ִ����һ����
    int nExpire = Expire();
    if(!nExpire)
    {
        //3.����IsInPeriod()���ݷ���ֵ�ж������Ƿ���ͳ��ʱ����ڣ�
        //����ͳ��ʱ�����ת����4������ͳ��ʱ����ڣ�ִ�е�5����
        if(IsInPeriod())
        {
            //5.�ж�m_tStart�Ƿ�Ϊ0�����Ϊ���time(&m_tStart)��
            //���ʾΪ��һ��ͳ��
            if(m_tStart == 0)
            //by ldf 2004-04-24 M2000V2B02����,�ϱ�Ϊ����ʱ���
            //����5��������8:02�ִ���Ϊ00:00��24:00����ô��һ���ϱ�Ϊ8:10�ϱ� 8:05��8:10��
            {
                time(&m_tStart);
                m_tStart = m_tStart + ((m_pTaskInfo->m_nPeriod)*60) - m_tStart%((m_pTaskInfo->m_nPeriod)*60);
            }
            //end
			
            //6.ѭ��nCount�Σ���ָ��1-8��ֵ�ֱ�ӵ���Ӧ�Ĺ�ϣ����
            //����Ӧֵ�ϣ�����ָ��IDΪ9��10�Ĳ���ָ�꣬�ж�
            //SPerfItem�еĽ���ID��ע��Ҫ��PT_BASE�ټ�1���Ƿ�
            //��m_pTaskInfo->m_mObjectInstance[2]��ȣ�������,
            //��Ⱦͽ�ָ��ֵ����SPerfItem->nValue�����������
            CounterMap::iterator it;
            for(int i = 0; i < nCount; i++)
            {
                if(pPerfs[i].uID <= 8)
                {
                    it = m_pTaskInfo->m_mCouterMap.find(pPerfs[i].uID);
                    if(it != m_pTaskInfo->m_mCouterMap.end())
                    {
                        it->second += pPerfs[i].uValue;
                        //��Ӧ�ļ�������+1
                        m_pTaskInfo->m_mCounterStatisticCount[pPerfs[i].uID] ++;
                    }
                }
                else if(O2IVIsEqual(pPerfs[i].yAPID))
                {
                    it = m_pTaskInfo->m_mCouterMap.find(pPerfs[i].uID);
                    if(it != m_pTaskInfo->m_mCouterMap.end())
                    {
                        it->second += pPerfs[i].uValue;
                        //����CDR��ͳ������˵������Ҫ����ƽ��ֵ����Ӧ�ļ���������Ŀǰ����Ϊ1
                        m_pTaskInfo->m_mCounterStatisticCount[pPerfs[i].uID] = 1;
                    } 
                }
            } //for
        } //if InPeroid
    }
    return TRUE;
}
void CPerfTask::Timer1Sec()
{
    if(m_pTaskInfo->m_nStatus == TASK_SUSPEND)
        return;
    //2.����Expire()�ж������Ƿ���ڣ��������ֵΪTURE��
    //��ʾ���ڣ�ִ�е�4����������Ϊ�˱�֤���һ��ͳ���ܹ�
    //��ȷ���̣�,����ִ����һ����
    int nExpire = Expire();
    if(!nExpire)
    {
        //3.����IsInPeriod()���ݷ���ֵ�ж������Ƿ���ͳ��ʱ����ڣ�
        //����ͳ��ʱ�����ת����4������ͳ��ʱ����ڣ�ִ�е�5����
        if(IsInPeriod())
        {
            m_nStatisticCount++;
        } //if InPeroid
    }
    //���û�й��ڳ���ͳ�����ھ�Ҫ����
    //�����������ȻҪ��֤���һ�δ��̣�

    //ldf ����Ҫ��prd��Ϊ1440��Ӧ����Խ����� ��Ӧ���ⵥSWPD00425
    if((m_nStatisticCount / 60) >= (m_pTaskInfo->m_nPeriod) ||
        (nExpire && m_nStatisticCount > 0))
    //end
    {
        CounterMap::iterator it;
        time(&m_tEnd);
        int nSpan = m_nStatisticCount;
        for(it = m_pTaskInfo->m_mCouterMap.begin(); it != m_pTaskInfo->m_mCouterMap.end(); it++)
        {
            //ȡͳ�Ƶ�ƽ��ֵ
            float fTotal = it->second;
            int nCnt = m_pTaskInfo->m_mCounterStatisticCount[it->first];
            if(nCnt == 0)
                it->second = 0;
            else
                it->second = fTotal / nCnt;
        }
        if(!m_pSave->Save(m_tStart, m_tEnd))
        {
            TRACE(MTS_PERF,m_pSave->GetLastError());
        }
		else
		{
			//by ldf 2003-12-16
			//���������ϱ����ĸ�6002�˿�
			//��ӱ���ͷ "+++ iGWB ...."
			
			g_TaskInfoBroad.g_pReport->MakeReportHead();
            g_TaskInfoBroad.g_pReport->AddString("TRAFFIC");
			g_TaskInfoBroad.g_pReport->AddString(STR_NEWLINE);
			g_TaskInfoBroad.g_pReport->AddString(S_PERF_TSK_RES);
			g_TaskInfoBroad.g_pReport->AddString(STR_NEWLINE);
			//by ldf 
			int nStrLen = strlen(S_PERF_TSK_RES);
            int j;
			for(j = 0; j < nStrLen; j++)
			{
				g_TaskInfoBroad.g_pReport->AddString("-");
			}
			//g_TaskInfo.g_pReport->AddString(STR_TITLE_SPIN);
			//end

			tm tmStartDate = *localtime(&m_tStart);
			char szDateTime[32];
            SNPRINTF(szDateTime, 32, "%04d-%02d-%02d %02d:%02d:%02d",
                     tmStartDate.tm_year+1900, tmStartDate.tm_mon+1, tmStartDate.tm_mday,
					 tmStartDate.tm_hour, tmStartDate.tm_min, tmStartDate.tm_sec);

			g_TaskInfoBroad.g_pReport->AddString(STR_NEWLINE);
			g_TaskInfoBroad.g_pReport->MakeReportItem(S_PERF_TSKN,
                                                      m_pTaskInfo->m_szTaskName.c_str(),
                                                      24,
                                                      ALIGN_RIGHT);
			//g_TaskInfoBroad.g_pReport->MakeReportItem(S_PERF_SERIAL_NO,"numbers",24,ALIGN_RIGHT);
			g_TaskInfoBroad.g_pReport->MakeReportItem(S_PERF_STATISTIC_START,
                                                      szDateTime,
                                                      24,
                                                      ALIGN_RIGHT);


			//by ldf 2003-12-25
			char szTmp[MAX_PATH];
			memset(szTmp,0,MAX_PATH);
			SNPRINTF(szTmp,MAX_PATH,"%d",m_pTaskInfo->m_nPeriod);
			g_TaskInfoBroad.g_pReport->MakeReportItem(S_PERF_PRD,szTmp,24,ALIGN_RIGHT);

			char szMUTmp[256];
			char *szMUValueBuf = &szMUTmp[0];

			IntVector::iterator itMU = m_pTaskInfo->m_vMU.begin();
			for(j = 0;j < m_pTaskInfo->m_vMU.size(); j++)
			{
				int nMu = m_pTaskInfo->m_vMU[j];
				SNPRINTF(szMUValueBuf,5,"%-d&",nMu);
				if(nMu >= 10)
					szMUValueBuf += 3;
				else
					szMUValueBuf += 2;
			}
			szMUValueBuf--;
			*szMUValueBuf ='\0';

			g_TaskInfoBroad.g_pReport->MakeReportItem(S_PERF_MU,szMUTmp,24,ALIGN_RIGHT);

            OIVMap::iterator itOIV = m_pTaskInfo->m_mObjectInstance.begin(); 
			for(j = 0;j < m_pTaskInfo->m_mObjectInstance.size(); j++)
			{
				char szOIVTitle[MAX_PATH];
				char szOIVValue[10];

				//by ldf 2004-04-14 M2000V2B02����
				SNPRINTF(szOIVTitle,MAX_PATH,"%s",S_PERF_OIV);
				//end
				
				SNPRINTF(szOIVValue,10,"%d",itOIV->second);
				g_TaskInfoBroad.g_pReport->MakeReportItem(szOIVTitle,szOIVValue,24,ALIGN_RIGHT);
				itOIV++;
			}

			//����ָ����Ϣ
			int nItem = 0;
			int nTmp = 0;

			char szTemp[MAX_PATH];
			memset(szTemp,0,MAX_PATH);

			for(it = m_pTaskInfo->m_mCouterMap.begin(); it != m_pTaskInfo->m_mCouterMap.end(); it++)
			{

				//SNPRINTF(szTmp,MAX_PATH,"COUNTER%d",it->first);
				//szTemp = m_DdaSet.GetFieldStringValue(nItem,szTmp);
				switch(it->first) 
				{
				case 1:
					strncpy(szTmp,S_PERF_COUNTER_CPU1,MAX_PATH);
					break;
				case 2:
					strncpy(szTmp,S_PERF_COUNTER_CPU2,MAX_PATH);
					break;
				case 3:
					strncpy(szTmp,S_PERF_COUNTER_PMA,MAX_PATH);
					break;
				case 4:
					strncpy(szTmp,S_PERF_COUNTER_PMT,MAX_PATH);
					break;
				case 5:
					strncpy(szTmp,S_PERF_COUNTER_FSA,MAX_PATH);
					break;
				case 6:
					strncpy(szTmp,S_PERF_COUNTER_FST,MAX_PATH);
					break;
				case 7:
					strncpy(szTmp,S_PERF_COUNTER_BSA,MAX_PATH);
					break;
				case 8:
					strncpy(szTmp,S_PERF_COUNTER_BST,MAX_PATH);
					break;
				case 9:
					strncpy(szTmp,S_PERF_COUNTER_RSCDR,MAX_PATH);
					break;
				case 10:
					strncpy(szTmp,S_PERF_COUNTER_RPSCDR,MAX_PATH);
					break;
				case 11:
					strncpy(szTmp,S_PERF_COUNTER_RGCDR,MAX_PATH);
					break;
				case 12:
					strncpy(szTmp,S_PERF_COUNTER_RPGCDR,MAX_PATH);
					break;
				case 13:
					strncpy(szTmp,S_PERF_COUNTER_RMCDR,MAX_PATH);
					break;
				case 14:
					strncpy(szTmp,S_PERF_COUNTER_RPMCDR,MAX_PATH);
					break;
				case 15:
					strncpy(szTmp,S_PERF_COUNTER_RSMO,MAX_PATH);
					break;
				case 16:
					strncpy(szTmp,S_PERF_COUNTER_RSMT,MAX_PATH);
					break;
				case 17:
					strncpy(szTmp,S_PERF_COUNTER_RLCSMO,MAX_PATH);
					break;
				case 18:
					strncpy(szTmp,S_PERF_COUNTER_RLCSMT,MAX_PATH);
					break;
				case 19:
					strncpy(szTmp,S_PERF_COUNTER_RLCSNI,MAX_PATH);
					break;
				default:
					break;
				}

                int nVal = int(it->second * 100);
				SNPRINTF(szTemp,MAX_PATH,"%-20.2f",float(nVal)/100);
				g_TaskInfoBroad.g_pReport->MakeReportItem(szTmp,szTemp,24,ALIGN_RIGHT);
								
			}
			
			g_TaskInfoBroad.g_pReport->AddString(STR_NEWLINE);
			g_TaskInfoBroad.g_pReport->AddString(STR_NEWLINE);
			g_TaskInfoBroad.g_pReport->MakeReportTail();
			SendMMLReport(g_TaskInfoBroad.g_pReport,APP_MML_PERF,0);
			
			//end 
		}
        m_tEnd = m_tStart = 0;
        for(it = m_pTaskInfo->m_mCouterMap.begin(); it != m_pTaskInfo->m_mCouterMap.end(); it++)
        {
            it->second = 0;
            m_pTaskInfo->m_mCounterStatisticCount[it->first] = 0;

        }
        m_nStatisticCount = 0;
    }
}
int CPerfTask::DelPerf()
{
    //����m_pTaskInfo->m_nResultSaveDays��
    //ɾ��Perf��Ŀ¼����Ӧ����������Ŀ¼�Ĺ��ڵ����������ļ���
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    int nFileDate = 0, nCurrent = 0;
    time_t tCurrent = 0;
    char szTmp[16];
    time(&tCurrent);
    tm tmCurrent = *localtime(&tCurrent);
    strftime(szTmp,16,"%Y%m%d",&tmCurrent);
    nCurrent = atoi(szTmp);
    pDir = ACE_OS::opendir(m_szPerfFilePath.c_str());
    if(pDir == NULL)
    {
        m_szError = strerror(errno);
        return FALSE;
    }
    char szSrcFilePath[MAX_PATH];
    
    //�ѱ����������ļ�
    LIST<STRING> FileToDelList;
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        if(strcmp(pDirent->d_name,".") == 0 
            || strcmp(pDirent->d_name,"..") == 0)
        {
            continue;
        }
        //���ļ�������ȡʱ��
        memset(szTmp,0,16);
        memcpy(szTmp,pDirent->d_name,8);
        nFileDate = atoi(szTmp);
        //����ļ����ھ�ɾ��
        if((nCurrent - nFileDate) >= m_pTaskInfo->m_nResultSaveDays)
        {
            SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",m_szPerfFilePath.c_str(),pDirent->d_name);
            FileToDelList.push_back(STRING(szSrcFilePath));
        }
    }
    ACE_OS::closedir(pDir);
    while(FileToDelList.size())
    {
        if(!FileDel(FileToDelList.front().c_str()))
        {
            m_szError = strerror(errno);
            TRACE(MTS_PERF,strerror(errno));
        }
        FileToDelList.pop_front();
    }
    return TRUE;
}

int CPerfTask::Expire()
{
    time_t tCurrent;
    time(&tCurrent);
    //1.�ж���ʼ�����Ƿ���ڵ�ʱ�䣬������ھͷ���TRUE��
    //����ִ��2
    if(tCurrent <= m_pTaskInfo->m_tStartTime)
        return TRUE;
    //2.�ж����������Ƿ�Ϊ����������ִ��4������ִ��3
    if(m_pTaskInfo->m_nTaskType != TASK_PERMANENCE)
    {
        //3.�ж�ͳ�ƽ���ʱ�� < ��ʼʱ�� �� ͳ������
        time_t tEndTime = m_pTaskInfo->m_tStartTime + m_pTaskInfo->m_nDayLimit * 3600 * 24;
        if(tCurrent >= tEndTime)
            return TRUE;
    }
    return FALSE;
}

int CPerfTask::IsInPeriod()
{
    //1.ȡ��ǰʱ�䣻
    time_t tTime = 0;
    time(&tTime);
    tm tmCurrent = *localtime(&tTime);
    //2.switch(m_pTaskInfo->m_nTimeType)���������ѭ��ִ��5��
    //�������ѭ��ִ��3���������ѭ��ִ��4
    switch(m_pTaskInfo->m_nTimeType) 
    {
    case TT_DAY: //����ѭ��
        {
            //5.�жϵ�ǰʱ���Ƿ����m_pTaskInfo->m_vTimeSpan���κ�
            //һ��Ԫ�ص������ڣ�����ǵĻ���ִ��6�����򷵻�FALSE
            if(!IsInTimeSpan(&tmCurrent))
                return FALSE;
            break;
        }
    case TT_WEEK: //����ѭ��
        //3.���ݵ�ǰʱ�䣬����tm�ṹȡ��tm_wday,�������������
        //��0-6��ʾ���յ������ģ���˲���ת����ֱ���ж�
        //m_pTaskInfo->m_vTimeSet���Ƿ�����tm_wday��ȵ�Ԫ�أ�
        //�����ת5�����򷵻�FALSE��
        if(!IsInTimeSet(tmCurrent.tm_wday))
            return FALSE;
        if(!IsInTimeSpan(&tmCurrent))
            return FALSE;
    	break;
    case TT_MONTH: //����ѭ��
        //4.���ݵ�ǰʱ�䣬����tm�ṹȡ��tm_mday,�������������
        //��1-31��ʾ1��31�ģ����Ҫ����7�����ж�
        //m_pTaskInfo->m_vTimeSet���Ƿ�����tm_mday+7��ȵ�Ԫ�أ�
        //�����ת5�����򷵻�FALSE��
        if(!IsInTimeSet(tmCurrent.tm_mday + 7))
            return FALSE;
        if(!IsInTimeSpan(&tmCurrent))
            return FALSE;
        break;
    default:
        break;
    }

    //6.�ж�m_nStatisticCount�Ƿ���ڵ���ͳ�����ڣ��ǵĻ�
    //����FALSE�����򷵻�TRUE��
   
   //ldf ����Ҫ��prd��Ϊ1440��Ӧ����Խ����� ��Ӧ���ⵥSWPD00425
   if( (m_nStatisticCount / 60)  >= (m_pTaskInfo->m_nPeriod))
   //end
       return FALSE;
   return TRUE;
}

int CPerfTask::IsInTimeSpan(tm *pCurrent)
{
    char szCurrent[16],szStart[16],szEnd[16];
    int nCurrent = 0, nStart = 0, nEnd = 0;
    sTimeSpan *pSpan = NULL;
    int nRet = FALSE;
    strftime(szCurrent,16,"%H%M%S",pCurrent);
    nCurrent = atoi(szCurrent);
    for(int i = 0; i< m_pTaskInfo->m_vTimeSpan.size(); i++)
    {
        pSpan = &(m_pTaskInfo->m_vTimeSpan[i]);
        strftime(szStart,16,"%H%M%S",&(pSpan->tStartTime));
        nStart = atoi(szStart);
        strftime(szEnd,16,"%H%M%S",&(pSpan->tEndTime));
        nEnd = atoi(szEnd);
        if(nCurrent >= nStart && nCurrent <= nEnd)
            nRet = TRUE;
    }
    return nRet;
}

int CPerfTask::IsInTimeSet(int DateVal)
{
    IntVector::iterator it = std::find(m_pTaskInfo->m_vTimeSet.begin(),
                                       m_pTaskInfo->m_vTimeSet.end(),
                                       DateVal);
    if(it != m_pTaskInfo->m_vTimeSet.end())
        return TRUE;
    else
        return FALSE;
}

int CPerfTask::O2IVIsEqual(int ApID)
{
    //int nID = ApID - PT_AP_BASE + 1;
    int nObjInstance = 0;
    //printf("APID = %d\n",ApID);
    //�鿴�ڶ������Ե�ԪҪ��Ĳ���ʵ����ApID
    OIVMap::iterator it = m_pTaskInfo->m_mObjectInstance.find(2);
    if(it != m_pTaskInfo->m_mObjectInstance.end())
    {
        nObjInstance = m_pTaskInfo->m_mObjectInstance[2];
        if(nObjInstance == ApID)
            return TRUE;
    }
    return FALSE;
}

