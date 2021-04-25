// task_creator.cpp: implementation of the CTaskCreator class.
//
//////////////////////////////////////////////////////////////////////
#include "task_creator.h"
CTaskCreator::CTaskCreator()
{
    m_pDdaSet = NULL; //���ֻ������
    m_nPageLimit = 48*1024;
}

CTaskCreator::~CTaskCreator()
{

}

int CTaskCreator::Init(STRING szPerfRoot)
{
    //����szPerfRoot·�����״̬�ļ���ȫ·����
    //����m_CreateProxy.Init()����������ֵ����m_pDdaSet
    //�в����ô�ֵ��ʼ��m_Query����ʼ��gTaskMap(�Բ�������ʽ
    //���ݸ�m_CreateProxy���ɹ�����TRUE��ʧ�ܷ���FALSE��
	//by ldf 2003-12
    #ifdef _PLATFORM_WIN32 
     STRING szPerfName = szPerfRoot + "\\prf_status.dbf";
    #else
	    STRING szPerfName = szPerfRoot + "/prf_status.dbf";
    #endif

    TaskMap *pTaskMap = &(g_TaskInfo.g_TaskMap);
    m_pDdaSet = m_CreateProxy.Init(szPerfName,pTaskMap);
    if(!m_pDdaSet)
        return FALSE;
    m_Query.Attach(m_pDdaSet);
    return TRUE;
}

int CTaskCreator::CreateTask(CParseResult *pParse)
{
    //����pParse����������SInformation��Ϣ
    //����̬���� һ��SInformation����
    if(pParse->GetCmdCode() != MML_CRE_PRFTSK)
        return FALSE;
    SInformation *pInfo = new SInformation;
    int nRes = 0;
    if(GenerateInformation(pParse,pInfo))
    {
        //����m_CreateProxy.CreateTask(SInformation*) 
        //����ͳ�������������NULL�������ɴ���ʧ�ܵ�MML���ģ�
        //�����͡�����FALSE�������ɹ��Ļ��ͽ����������뵽
        //gTaskMap�У�����gTaskIDToStringMap����������
        //���ɴ����ɹ���MML���ģ������͡�����TRUE��
        CPerfTask *pTask = m_CreateProxy.CreateTask(pInfo);
        if(pTask == NULL)
        {
            nRes = -1;
            delete pInfo,pInfo = NULL;
            m_szError = m_CreateProxy.GetLastError();
        }
        else
        {
            g_TaskInfo.g_TaskMap[pInfo->m_szTaskName] = pTask;
            nRes = 0;
        }
    }
    else
    {
        delete pInfo, pInfo = NULL;
        //by ldf 2004-04-26 M2000V2B02 �������������ӷ��ش�����
        if(m_szError.compare(S_PERF_TASKNAME_EXIST) == 0)
        {
            nRes = ERROR_TASKNAME_EXIST;
        }
        else
        {
            nRes = -1;
        }
        //end
	
    }
    //by ldf 2003-12-24
	//GenerateCommonReport(g_TaskInfo.g_pReport,pParse->GetCmdLine(),S_PERF_CREATE_SUCCESS,nRes);
	GenerateCommonReport(&g_TaskInfo,pParse->GetCmdLine(),S_PERF_CREATE_SUCCESS,nRes);
    SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
    return (nRes == 0);
}

int CTaskCreator::DeleteTask(CParseResult *pParse)
{
    //�Ӳ����еõ���������
    //����m_CreateProxy.DeleteTask(szTaskName)ɾ���������
    //���ʧ�ܣ�����ʧ�ܵı��ġ�ɾ���ɹ����ͽ�gTaskMap��
    //gTaskIDToStringMap�е���Ϣɾ�������سɹ���MML���ġ�
    //����TRUE��
    //ȡ��һ��������������
    STRING szTaskName = pParse->GetStringVal(1);
    int nRes = 0;
    if(m_CreateProxy.DeleteTask(szTaskName))
    {
        delete g_TaskInfo.g_TaskMap[szTaskName], g_TaskInfo.g_TaskMap[szTaskName] = NULL;
        g_TaskInfo.g_TaskMap.erase(szTaskName);
        nRes = 0;
    }
    else
    {
        m_szError = m_CreateProxy.GetLastError();
        //by ldf 2004-04-26 M2000�����������ѷ��ش������M2000ƥ��
        if(m_szError.compare(S_PERF_NOTHISTASK) == 0)
        {
            nRes=ERROR_NOTHISTASK;
        }
        else
        {
            nRes = -1;
        }
        //end
    }
	//by ldf 2003-12-24
    //GenerateCommonReport(g_TaskInfo.g_pReport,pParse->GetCmdLine(),
	GenerateCommonReport(&g_TaskInfo,pParse->GetCmdLine(),
                         S_PERF_DELETE_SUCCESS,nRes);
    SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
    return (nRes == 0);
}
int CTaskCreator::ListTaskInfo(CParseResult *pParse)
{
    int nRet = REPORT_MORE;
    m_Query.ClearBind();
    try{
        //ȡpParse��CHT������INFO��������CHT����ֵѡ���֧��
        //1��ִ�з�֧1��2��ִ�з�֧2��3��ִ�з�֧3��4��ִ�з�֧4��
        //5��ִ�з�֧5   
        int nCHT = 0,nINFO = 0;
        //ȡ��һ������ CHT(��ѯ����) ��Ҫ����
        if(!pParse->GetDigitVal(&nCHT,1))
            throw "failed to get the first parameter";
        //ȡ��2������ INFO(��ʾ��ϸ��Ϣ) ��ѡ��Ĭ��0
        if(!pParse->GetDigitVal(&nINFO,2))
            nINFO = 0;
        CDdaQuery *pQuery = NULL;
        switch(nCHT)
        {
        case 1:
            {
                //ȡ��3������ TSKN�����������������
                const char *pTaskName = pParse->GetStringVal(3);
                //GetStringVal���ؿ�����""
                if(strcmp(pTaskName,"") != 0)
                {
                    //���TSKN����ָ������������
                    //����m_Query.BindAnd("������",���������ơ�,OpEqual)
                    //�������ﶨ����ѯ�����ϣ�Ȼ�����m_Query�Ĳ�ѯ���
                    //�õ������¼��������m_pDdaSet��ȡ����Ӧ�ļ�¼Ȼ��
                    m_Query.AddBindAnd("TSKN",pTaskName,opEqual);
                    m_Query.DoQuery();
                    pQuery = &m_Query;
                }
                
                break;
            }
        case 2:
            {
                //ȡMS���������Ӳ��������Ӳ�����Ϊ��������
                //m_Query.BindAnd�����󶨵�m_Query�У�Ȼ��ȡ�ý������
                //���ô������m_pDdaSet��ȡ�ü�¼���ٸ���INFO����Ҫ��
                //�������Ļ�����Ϣ����ϸ��Ϣ��MML����
                INTMAP MsMap;
                INTMAP::iterator it;
                int nBufPos = 0;
                char *pBuf =  new char[ 3 * COUNTER_NUM];
                GetEnumValue(pParse,5,10,MsMap); 
                for(it = MsMap.begin(); it!=MsMap.end(); it++)
                {                    
                    SNPRINTF(pBuf + nBufPos,COUNTER_NUM,
                        "%d&",it->first);
                    if(it->first >= 10)
                        nBufPos += 3;
                    else
                        nBufPos += 2;       
                }
                pBuf[nBufPos-1] = 0;
                m_Query.AddBindAnd("ITM",STRING(pBuf),opSet);
                m_Query.DoQuery();
                pQuery = &m_Query;
                break;
            }
        case 3:
            {
                //ȡSTAT���������û�״̬��Ϊ�����󶨵�m_Query�ϣ�
                //ȡ�÷��������ļ�¼����INFO ����Ҫ���������Ļ�����Ϣ
                //����ϸ��Ϣ��MML����
                int nSTAT = 0;
                if(!pParse->GetDigitVal(&nSTAT,6))
                    nSTAT = 0;
                m_Query.AddBindAnd("RUN",nSTAT,opEqual);
                m_Query.DoQuery();
                pQuery = &m_Query;
                break;
            }
        case 4:
            {
                //ȡUSER���������û����󶨵�m_Query��Ȼ��ȡ�÷�������
                //�ļ�¼����INFO ����Ҫ���������Ļ�����Ϣ����ϸ��Ϣ��
                //MML����
                const char *szUser = pParse->GetStringVal(UINT2(4));
                m_Query.AddBindAnd(STRING("USER"),STRING(szUser),opEqual);
                m_Query.DoQuery();
                pQuery = &m_Query;
                break;
            }
        case 5:
            {   
                //ȡRDS��RDE���������������ڽ��û����󶨵�m_Query��Ȼ��
                //ȡ�÷��������ļ�¼����INFO ����Ҫ���������Ļ�����Ϣ��
                //��ϸ��Ϣ��MML����
                int nYear = 0, nMon = 0, nDay = 0;
                if(!pParse->GetDigitVal(&nYear,7,1)
                    || !pParse->GetDigitVal(&nMon,7,2)
                    || !pParse->GetDigitVal(&nDay,7,3))
                    throw "bad start date";
                char szDate[10] = "";
                SNPRINTF(szDate,10,"%04d%02d%02d",nYear,nMon,nDay);       
                m_Query.AddBindAnd("SD",STRING(szDate),opGreater);
                
                if(!pParse->GetDigitVal(&nYear,8,1)
                    || !pParse->GetDigitVal(&nMon,8,2)
                    || !pParse->GetDigitVal(&nDay,8,3))
                    throw "bad end date";
                SNPRINTF(szDate,10,"%04d%02d%02d",nYear,nMon,nDay);       
                m_Query.AddBindAnd("SD",STRING(szDate),opLess);
                m_Query.DoQuery();
                pQuery = &m_Query;
                break;
            }
        } // switch
        
        do {
            nRet = GenerateTaskInfoReport(&g_TaskInfo,
                                          pParse->GetCmdLine(),
                                          nINFO,pQuery);
            if(nRet == REPORT_ERR)
				GenerateCommonReport(&g_TaskInfo,
                                     pParse->GetCmdLine(),
                                     "", REPORT_ERR);
            SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
        } while(nRet == REPORT_MORE);
        
    }
    catch(char *pError)
    {
        m_szError = pError;
        GenerateCommonReport(&g_TaskInfo,
                             pParse->GetCmdLine(),
                             "", REPORT_ERR);
        SendMMLReport(g_TaskInfo.g_pReport,APP_MML_NORM,pParse->GetIP());
        nRet =  REPORT_ERR;
    }
    return (nRet != REPORT_ERR);
}
int CTaskCreator::GetEnumValue(CParseResult *pParse, int nID, int nMaxLen, INTMAP &EnumMap)
{
    STRING szParameter;
    int nPos = 0;
    int nValue = 0;
    int nKeyPos = 0;
    for (int i = 1; i <= nMaxLen; i ++)
    {
        szParameter = pParse->GetStringVal(nID,i);
        if (szParameter.compare("") != 0)
        {
            nPos = szParameter.find("-");
            nKeyPos = atoi((szParameter.substr(0, nPos)).c_str());
            nValue = atoi((szParameter.substr(nPos + 1)).c_str());
            EnumMap[nKeyPos] = nValue;
        }
        else
            break;
    }
    return EnumMap.size();
}
int CTaskCreator::GenerateInformation(CParseResult *pParse,SInformation *pInfo)
{
    try{
        int i = 0;
        //ȡ��1���������ܲ�����������,��ѡ����
        pInfo->m_szTaskName = pParse->GetStringVal(1);
        if(pInfo->m_szTaskName.compare("") == 0)
            throw "failed to get first parameter";
        if(g_TaskInfo.g_TaskMap.find(pInfo->m_szTaskName) != g_TaskInfo.g_TaskMap.end())
            throw S_PERF_TASKNAME_EXIST;
        //ȡ��2���������ܲ�����������,��ѡ����
        pInfo->m_szTaskDes = pParse->GetStringVal(2); //""
        pInfo->m_szUser = pParse->GetUserName();

        //by ldf 2004-04-22 ��M2000V2����������19715ȷ�ϣ�
        //���д����⴦����M2000����������ͳһֻ�á�M2000����ʶ������Ĵ�����
        if( ACE_OS::strncasecmp(pParse->GetUserName(),"M2000", 4) == 0 )
        {
            pInfo->m_szUser ="M2000";
        }
        //end
		
        //ȡ��3������������ԪID,��ѡ����
        //ö��
        INTMAP EnumMap; 
        INTMAP::iterator map_it;
        if(GetEnumValue(pParse,3,2,EnumMap))
        {
            for(map_it = EnumMap.begin(); map_it != EnumMap.end(); map_it++)
            {
                pInfo->m_vMU.push_back(map_it->first);
            }
        }
        else
            throw "failed to get the third parameter";
        //ȡ��4�������Ƿ���������,��ѡ����
        if(!(pParse->GetDigitVal(&(pInfo->m_nStatus),4)))
            pInfo->m_nStatus = TASK_NORMAL;
        //ȡ��5������ͳ������,��ѡ����
        if(!(pParse->GetDigitVal(&(pInfo->m_nPeriod),5)))
            pInfo->m_nPeriod = 30; //Ĭ��:30 minute
        //ȡ��6��������ʼ����YY&MM&DD����ѡ��
        tm tmDate;
        memset(&tmDate,0,sizeof(tm));
        if(pParse->GetDigitVal(&(tmDate.tm_year),6,1) 
            && pParse->GetDigitVal(&(tmDate.tm_mon),6,2)
            && pParse->GetDigitVal(&(tmDate.tm_mday),6,3))
        {
            tmDate.tm_year -= 1900;
            tmDate.tm_mon -= 1;
            pInfo->m_tStartTime = mktime(&tmDate);
        }
        else
            time(&(pInfo->m_tStartTime));
        //ȡ��7������ͳ��ʱ��εĸ���,��ѡ����
        int nTimeSpanNum = 0;
        if(pParse->GetDigitVal(&nTimeSpanNum,7))
        {
            if(nTimeSpanNum > 3 || nTimeSpanNum < 1)
                throw "the seventh parameter overflow limit";
            int nParaLoc = 0;
            sTimeSpan sTime;
            memset(&sTime, 0, sizeof(sTimeSpan));
            for(i = 0; i < nTimeSpanNum; i++)
            {
                //ȡ��8-13������
                //SP1-3S(��ʼʱ��1-3)����ѡ
                //SP1-3E(����ʱ��1-3)����ѡ
                int nStartHH = 0, nStartMM = 0,nEndHH = 0, nEndMM = 0;
                nParaLoc = 8 + i*2;
                BOOL bRet1 = pParse->GetDigitVal(&nStartHH,nParaLoc,1);
                BOOL bRet2 = pParse->GetDigitVal(&nStartMM,nParaLoc,2);
                if(!bRet1 || !bRet2)
                    throw "no start time";
                
                sTime.tStartTime.tm_hour = nStartHH;
                sTime.tStartTime.tm_min = nStartMM;
                
                nParaLoc = 8 + i*2 + 1;
                bRet1 = pParse->GetDigitVal(&nEndHH,nParaLoc,1);
                bRet2 = pParse->GetDigitVal(&nEndMM,nParaLoc,2);
                if(!bRet1 || !bRet2)
                    throw "no end time";
                
                sTime.tEndTime.tm_hour = nEndHH;
                sTime.tEndTime.tm_min = nEndMM;
                
                pInfo->m_vTimeSpan.push_back(sTime);
                
            }
        }
        else
            throw "failed to get the 7th parameter";
        //ȡ��14������TSKTP(��������)��ѡ������������ 
        //Ĭ��ȡֵ�ǰ���������
        if(!pParse->GetDigitVal(&(pInfo->m_nTaskType),14))
            pInfo->m_nTaskType = TASK_PERMANENCE;
        //ȡ��15������LPT(������������)
        //���ʱ������Ϊ0�򲻿�ѡ, ���Ϊ1���ѡ
        //Ĭ��ȡֵ��7��; ע������Ϊ0��ȡ7Ҳ����ν����������
        if(!pParse->GetDigitVal(&(pInfo->m_nDayLimit),15))
            pInfo->m_nDayLimit = 7;
        //ȡ��16������TT��ʱ�����ͣ���ѡ���� �����ѡ����ȱʡΪ0
        if(!pParse->GetDigitVal(&(pInfo->m_nTimeType),16))
            pInfo->m_nTaskType = 0;
        //ȡ��17������STS(ͳ��ʱ�伯�ϣ�λ�����͵��ֽ���Ϊ38
        //���ʱ��������DAY,�򲻿�ѡ;�����WEEK ���ѡ��
        //ȡֵ��Χ��SUN��SAT;�����MONTH,���ѡ��ȡֵ��Χ��D1��D31;
        //�ɶ�ѡ����&����
        //ö��
        EnumMap.clear();
        if(GetEnumValue(pParse,17,38,EnumMap))
        {
            int nDay = 0;
            for(map_it = EnumMap.begin();map_it != EnumMap.end(); map_it++)
            {
                nDay = map_it->first; 
                if(pInfo->m_nTimeType == TT_WEEK
                   &&(nDay > 6 || nDay < 0))
                {
                    throw "17th parameter overflow while Time Type is TT_WEEK";
                }
                else if(pInfo->m_nTimeType == TT_MONTH
                        && (nDay < 6 || nDay >38))
                {
                    throw "17th parameter overflow while Time Type is TT_MONTH";
                }
                pInfo->m_vTimeSet.push_back(nDay);
            }
        }
        else if(pInfo->m_nTimeType == TT_DAY)
        {
            pInfo->m_vTimeSet.clear();
        }
        else
        {
            throw "17th parameter is null";
        }

        //ȡ��18������SRST�����������������ѡ������Ĭ��ȡֵ��30
        if(!pParse->GetDigitVal(&(pInfo->m_nResultSaveDays),18))
            pInfo->m_nResultSaveDays = 30;
        //ȡ��19������ITM������ָ�꣩��ѡ���� ���10��ָ��
        //ö��
        int nCounter = 0;
        EnumMap.clear();
		//by ldf 2004-05-19 �����19��ͳ����
        if(GetEnumValue(pParse,19,19,EnumMap))
        {
            for(map_it = EnumMap.begin(); map_it != EnumMap.end(); map_it++)
            {
                nCounter = map_it->first;
                pInfo->m_mCouterMap[nCounter] = 0;
                pInfo->m_mCounterStatisticCount[nCounter] = 0;
            }
        }
        else
        {
            throw "19th paremeter is null";
        }
        //ȡ20������OIV ����������ʵ��ֵ��
        int nKey = 0;
        char szOIV[32];
        strncpy(szOIV,pParse->GetStringVal(20),32);
        char *pTok = strtok(szOIV,"&");
        for(i = 1; pTok; i++, pTok = strtok(NULL,"&"))
        {
            nKey = pInfo->m_vMU[i-1];
            pInfo->m_mObjectInstance[nKey] = atoi(pTok);
        }
    }
    catch(const char *pError)
    {
        m_szError = pError;
        return FALSE;
    }
    return TRUE;
}

int CTaskCreator::GenerateCommonReport(STaskInfo *p,const char *pCmdLine, const char *pContent,int nRes)
{
    /*
    +++    HUAWEI iGWB        2002-05-24 09:25:13
    TASKCMD    #0
    %%CRE TSK: TSKN="CG Performance", TSKDES="CG���ܲ���", MU=1&&2, RUN=FALSE, SPD=1, SP1S=1, SP1E=24, TSKTP=1, LPT=10, TT=WEEK, STS=SUN&SAT, SRST=20, SRCD=10000, OIV=1&1, ITM=1&2&3&4&5&6&7;%%
    RETCODE = 0  ִ�гɹ�
    ��������ɹ�
    ---    END
    */
    p->g_pReport->MakeReportHead();
    p->g_pReport->MakeCmdDisplay(pCmdLine,p->nReportCsn++,nRes,"O&M");

    //by ldf 2004-04-26 M2000����
    if(nRes != 0)
    //end
    {
        p->g_pReport->MakeReportItem(S_PERF_FAIL_REASON,
                                m_szError.c_str(),20,
                                ALIGN_RIGHT);
        p->g_pReport->AddString(STR_NEWLINE);
        p->g_pReport->AddString(STR_NEWLINE);
    }
    else
    {
        p->g_pReport->AddString(pContent);
        p->g_pReport->AddString(STR_NEWLINE);

    }
    p->g_pReport->MakeReportTail();
    return TRUE;
}

int CTaskCreator::GenerateTaskInfoReport(STaskInfo *p,
                                         const char *pCmdLine, 
                                         int nINFO, 
                                         CDdaQuery *pQuery)
{
    static int nResPos = 0; //��һ�ε�λ�ñ���
    static int nPages  = 1; //�������˶����ű���
    int i = 0; //ѭ������;
    static UINTArray ResArray;
    UINTArray *pResArray;
    try{
        if(pQuery)
        {
            pResArray = &(pQuery->m_ResultArray);
        }
        else
        {
            if(nResPos == 0)
            {
                ResArray.clear();
                for(i = nResPos; i < m_pDdaSet->GetRecordCount(); i++)
                    ResArray.push_back(i);
            }
            pResArray = &ResArray;
        }
        /* ��Ҫ����ϸ�ı��ĸ�ʽ
        +++    HUAWEI iGWB        2002-06-10 16:49:53
        TASKCMD    #0
        %%LST TSK: CHT=1, INFO=0;%%
        RETCODE = 0  ִ�гɹ�
        
        ���������Ϣ
        ------------
        ��������  = ����                                                     
        �����  =  0                                                            
        ��������  =  2000��8��CGӲ������
        ����ʼʱ�� = 2002-08-01                                                
        ����״̬  =  ����                                                         
        �����������  =  5000                                                         
          
        ��������  =  ��ïʶ                                                     
        �����  =  1                                                            
        ��������  =  2000��12��CGҵ������
        ����ʼʱ�� = 2002-08-01                                              
        ����״̬  =  ����                                                         
        �����������  =  100
        (������� = 2)
            
        ---    END
        */
        p->g_pReport->MakeReportHead();
        p->g_pReport->MakeCmdDisplay(pCmdLine,p->nReportCsn++,0,"O&M");
        STRING szTmp;
        const char *pTmp = NULL;
        int nVal = 0;
        int nItem = 0;
        for(i = nResPos; i < pResArray->size(); i++)
        {   nItem =  pResArray->at(i);
            MakeSubTitle(p->g_pReport,S_PERF_TASK_SIMPLE_INFO);
            p->g_pReport->MakeReportItem(S_PERF_TSKN,
                                         m_pDdaSet->GetFieldStringValue(nItem,"TSKN").c_str(),
                                         24,
                                         ALIGN_RIGHT);
            p->g_pReport->MakeReportItem(S_PERF_TSKDES,
                                         m_pDdaSet->GetFieldStringValue(nItem,"TSKDES").c_str(),
                                         24,
                                         ALIGN_RIGHT);
            p->g_pReport->MakeReportItem(S_PERF_CRE_TSK_USER,
                                         m_pDdaSet->GetFieldStringValue(nItem,"USER").c_str(),
                                         24,
                                         ALIGN_RIGHT);
            pTmp = ConvertDate(m_pDdaSet->GetFieldStringValue(nItem,"SD").c_str());
            p->g_pReport->MakeReportItem(S_PERF_SD,pTmp,24,ALIGN_RIGHT);
            nVal = atoi(m_pDdaSet->GetFieldStringValue(nItem,"RUN").c_str());
            if(nVal == TASK_NORMAL)
            {
                szTmp = S_PERF_STAT_ACT;
            }
            else
            {
                szTmp = S_PERF_STAT_SUS;
            }
            p->g_pReport->MakeReportItem(S_PERF_TSK_STSTUS,szTmp.c_str(),24,ALIGN_RIGHT);
            p->g_pReport->MakeReportItem(S_PERF_SRST,
                                         m_pDdaSet->GetFieldStringValue(nItem,"SRST").c_str(),
                                         24,
                                         ALIGN_RIGHT);

			//by ldf 204-04-23 M2000����
            //p->g_pReport->AddString(STR_NEWLINE);
			//end
			
            //�������ϸ��ʽ��Ҫ���������Ϣ
            if(nINFO)
            {
                /*
                ʱ����Ϣ
                --------
                ��������  =  ��������                                                       
                ʱ������  =  ����ͳ��                                                       
                ��ʼ����  =  2001-01-01                                                     
                ��������  =  <NULL>                                                         
                ͳ������  =  60                                                             
                ͳ��ʱ�伯��  =  <NULL>                                                         
                ͳ��ʱ���  =  1                                                              
                ��ʼʱ��1  =  00:00                                                          
                ����ʱ��1  =  24:00                                                          
                ��ʼʱ��2  =  <NULL>                                                         
                ����ʱ��2  =  <NULL>                                                         
                ��ʼʱ��3  =  <NULL>                                                         
                ����ʱ��3  =  <NULL>                                                         
                �������ʱ��  =  100
                */
                MakeSubTitle(p->g_pReport,S_PERF_TIME_INFO);
                int nTASKTP = atoi(m_pDdaSet->GetFieldStringValue(nItem,"TSKTP").c_str()); 
                if(nTASKTP) //�����1������������
                {
                    p->g_pReport->MakeReportItem(S_PERF_TSKTP,S_PERF_LIMIT,24,ALIGN_RIGHT);
                }
                else
                {
                    p->g_pReport->MakeReportItem(S_PERF_TSKTP,S_PERF_PERMANENCE,24,ALIGN_RIGHT);
                }
                
                pTmp = ConvertTimeType(m_pDdaSet->GetFieldStringValue(nItem,"TT").c_str());
                p->g_pReport->MakeReportItem(S_PERF_TT,pTmp,24,ALIGN_RIGHT);

				STRING szDateTmp = m_pDdaSet->GetFieldStringValue(nItem,"SD");
                pTmp = ConvertDate(szDateTmp.c_str());
                p->g_pReport->MakeReportItem(S_PERF_SD,pTmp,24,ALIGN_RIGHT);

                if(nTASKTP)
                {
					//by ldf pTmp�����ݸ�ʽ�Ѿ���Ϊ�� 2003-12-25��������20031225
                    pTmp = AddDaysToDBFData(szDateTmp.c_str(),atoi(m_pDdaSet->GetFieldStringValue(nItem,"LPT").c_str()));
                    szTmp = ConvertDate(pTmp);
                }
                else
                {
                    szTmp = S_PERF_TSK_NULL;
                }
                //by ldf 2004-04-24 M2000����
                p->g_pReport->MakeReportItem(S_PERF_ED,szTmp.c_str(),24,ALIGN_RIGHT,50,ALIGN_LEFT,1);
                //end
                p->g_pReport->MakeReportItem(S_PERF_PRD,
                                             m_pDdaSet->GetFieldStringValue(nItem,"PRD").c_str(),
                                             24,
                                             ALIGN_RIGHT);
                //���szTmp = ""��ConvertStatisticTimeSet����<NULL>
                pTmp = ConvertStatisticTimeSet(m_pDdaSet->GetFieldStringValue(nItem,"STS").c_str());

                //by ldf 2004-04-24 M2000����
                p->g_pReport->MakeReportItem(S_PERF_STS,pTmp,24,ALIGN_RIGHT,50,ALIGN_LEFT,1);
                //end
				
                p->g_pReport->MakeReportItem(S_PERF_SPD,
                                             m_pDdaSet->GetFieldStringValue(nItem,"SPD").c_str(),
                                             24,
                                             ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP1S").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP1S,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP1E").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP1E,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP2S").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP2S,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP2E").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP2E,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP3S").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP3S,pTmp,24,ALIGN_RIGHT);
                pTmp = ConvertStatisticTime(m_pDdaSet->GetFieldStringValue(nItem,"SP3E").c_str());
                p->g_pReport->MakeReportItem(S_PERF_SP3E,pTmp,24,ALIGN_RIGHT);
                p->g_pReport->MakeReportItem(S_PERF_SRST,
                                             m_pDdaSet->GetFieldStringValue(nItem,"SRST").c_str(),
                                             24,
                                             ALIGN_RIGHT);            
                /*
                ָ����Ϣ
                --------
                ����ָ��1  =  CPU1ʹ���ʣ�OFFINT��
                ����ָ��2  =  CPU2ʹ���ʣ�OFFINT��
                ����ָ��3  =  �����ڴ���ÿռ䣨OFFINT��
                ����ָ��4  =  �����ڴ��ܿռ䣨OFFINT��
                ����ָ��5  =  ǰ���̿��ÿռ䣨OFFINT��
                ����ָ��6  =  ǰ�����ܿռ䣨OFFINT��
                ����ָ��7  =  ����̿��ÿռ䣨OFFINT��
                ����ָ��8  =  ������ܿռ䣨OFFINT��
                ����ָ��9  =  ���ջ���������OFFINT��
                ����ָ��10  =  ������������OFFOUT��                                       
                */
                MakeSubTitle(p->g_pReport,S_PERF_COUNTER_INFO);
                char szCounterTitle[16];
                char szCounter[32] = "";
				//by ldf 2004-05-28
                memcpy(szCounter,m_pDdaSet->GetFieldStringValue(nItem,"ITM").c_str(),38);
                char *pSubTok = strtok(szCounter,"&");
                for(int j = 1; pSubTok; j++)
                {
                    pTmp = ConvertCounter(pSubTok);
                    SNPRINTF(szCounterTitle,16,"%s%d",S_PERF_ITM,j);
                    p->g_pReport->MakeReportItem(szCounterTitle,pTmp,24,ALIGN_RIGHT);
                    pSubTok = strtok(NULL,"&");
                }
                /*
                ����������Ϣ
                ------------
                ��������ʵ��1  =  Ӳ������
                ��������ʵ��2  =  �����1��ҵ������                                      
                */
                MakeSubTitle(p->g_pReport,S_PERF_OBJECT_INFO);
                //�ȶ�MU��ֵȷ�����ַ���
                VECTOR<int> vMu,vOIV;
                ConvertStrToVector(m_pDdaSet->GetFieldStringValue(nItem,"MU").c_str(),vMu);
                ConvertStrToVector(m_pDdaSet->GetFieldStringValue(nItem,"OIV").c_str(),vOIV);
                //Ҫ��Ӧ���Ե�Ԫ

                char szTitle[32];
                char szContent[32];
                int nOIV = 0;
                for(int k = 0; k < vMu.size(); k++)
                {
                    SNPRINTF(szTitle,32,S_PERF_INSTANCE,k+1);
					nOIV = vOIV[k];
					sprintf(szContent,"%d",nOIV);
                    p->g_pReport->MakeReportItem(szTitle,szContent,24,ALIGN_RIGHT);
                }
        }
        nResPos ++;
        if(p->g_pReport->GetReportLen() >= m_nPageLimit)
            break;
    }//for�������ɱ���
    p->g_pReport->AddString(STR_NEWLINE);
    //by ldf 204-04-23 M2000����
    //p->g_pReport->AddString(STR_NEWLINE);
    //end
	
    //����ǵ������ļ�¼,���ɱ���β��
    if(nResPos >= pResArray->size())
    {
        char szTmp[32];
        SNPRINTF(szTmp,32,S_PERF_RESULT_NUM,pResArray->size());
        p->g_pReport->AddString(szTmp);
        p->g_pReport->AddString(STR_NEWLINE);
        SNPRINTF(szTmp,32,s_PERF_PAGE_NUM,nPages);
        p->g_pReport->AddString(szTmp);
        p->g_pReport->AddString(STR_NEWLINE);
        nResPos = 0;
        nPages = 1;
        p->g_pReport->AddString(MML_END);
        if(!pQuery)
        {
            ResArray.clear();
        }
        return REPORT_NONE;
    }
    else
    {
        p->g_pReport->AddString(S_PERF_REPORT_MORE);
        p->g_pReport->AddString(STR_NEWLINE);
        nPages ++;
        p->g_pReport->AddString(MML_END);
        return REPORT_MORE;
    }
  }
  catch(std::exception &e)
  {
       m_szError = e.what();
       return REPORT_ERR;
  }

}

void CTaskCreator::ConvertStrToVector(const char *pStr, VECTOR<int> &Vector)
{
    char szTmp[16];
    strncpy(szTmp,pStr,16);
    char *pTok = strtok(szTmp,"&");
    while(pTok)
    {
        Vector.push_back(atoi(pTok));
        pTok = strtok(NULL,"&");
    }
}

const char *CTaskCreator::ConvertDate(const char *pDate, const char *pSeparater, int nNation)
{
    int nDate = atoi(pDate);
    int nYear = nDate / 10000;
    int nMonth = (nDate % 10000)/100;
    int nDay = nDate % 100;
    if(nNation)
    {
        SNPRINTF(m_szConvertCatch,CONVERT_CATCH_LEN,"%04d%s%02d%s%02d",
                 nYear,pSeparater,nMonth,pSeparater,
                 nDay, pSeparater);   
    }
    else
    {
        SNPRINTF(m_szConvertCatch,CONVERT_CATCH_LEN,"%02d%s%02d%s%04d",
                 nMonth,pSeparater,nDay,pSeparater,
                 nYear,pSeparater);
    }
    return m_szConvertCatch;
}
//
const char *CTaskCreator::AddDaysToDBFData(const char *pDBFData, int nDays)
{
    int nDate = atoi(pDBFData);
    int nYear = nDate / 10000;
    int nMonth = (nDate % 10000)/100;
    int nDay = nDate % 100;
    tm tmDate;
    memset(&tmDate,0,sizeof(tm));
    tmDate.tm_year = nYear - 1900;
    tmDate.tm_mon = nMonth - 1;
    //by ldf 2004-04-24 M2000������ӦM2000Ҫ�󣬵���Ҳ�㣬����CG�ټ�һ��
    //Ҫ�ټ�һ��Ż�������,�����Ϊ90��ģ�ֻ��89��
    tmDate.tm_mday = nDay -1;
    //end
	
    time_t tTime = mktime(&tmDate);
    tTime += nDays * 24 * 3600; 
    tmDate = *localtime(&tTime);
    strftime(m_szConvertCatch,CONVERT_CATCH_LEN,"%Y%m%d",&tmDate);
    return m_szConvertCatch;
}

const char *CTaskCreator::ConvertTimeType(const char *pTT)
{
    switch(atoi(pTT)) {
    case TT_DAY: //
        strncpy(m_szConvertCatch,S_PERF_TT0,CONVERT_CATCH_LEN);
        break;
    case TT_WEEK:
        strncpy(m_szConvertCatch,S_PERF_TT1,CONVERT_CATCH_LEN);
    	break;
    default:
        strncpy(m_szConvertCatch,S_PERF_TT2,CONVERT_CATCH_LEN);
    }
    return m_szConvertCatch;
}

const char *CTaskCreator::ConvertStatisticTimeSet(const char *pSTS)
{
    char szTmp[512] = "";
    strncpy(szTmp,pSTS,512);
    char *pTok = strtok(szTmp,"&");
    if(!pTok)
    {
        strncpy(m_szConvertCatch,S_PERF_TSK_NULL,CONVERT_CATCH_LEN);
        return m_szConvertCatch;
    }
    int nPos = 0;
    int nVal = 0;
    while(pTok)
    {
        nVal = atoi(pTok);
        switch(nVal) 
        {
        case 0:
            //by ldf 2004-04-26 M2000���������з��ֵ�һ���쳣��
            //CONVERT_CATCH_LEN 1024,
            //����Ŀǰ���ֶεĺͣ��ܾ�����1024�ĳ��ȣ���Ȼ�����ڴ�����
            strncpy(&m_szConvertCatch[nPos],S_PERF_SUN,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_SUN);
        	break;
        case 1:
            strncpy(&m_szConvertCatch[nPos],S_PERF_MON,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_MON);
        	break;
        case 2:
            strncpy(&m_szConvertCatch[nPos],S_PERF_TUE,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_TUE);
        	break;
        case 3:
            strncpy(&m_szConvertCatch[nPos],S_PERF_WED,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_WED);
        	break;
        case 4:
            strncpy(&m_szConvertCatch[nPos],S_PERF_THU,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_THU);
        	break;
        case 5:
            strncpy(&m_szConvertCatch[nPos],S_PERF_FRI,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_FRI);
        	break;
        case 6:
            strncpy(&m_szConvertCatch[nPos],S_PERF_SAT,CONVERT_CATCH_LEN-nPos);
            nPos += strlen(S_PERF_SAT);
        	break;
        default:
            //by ldf 2004-04-24 M2000���������������Macro.ini������
            nVal -= 7; //�·��е����ڴ�7��ʼΪ1��
            //end
            SNPRINTF(&m_szConvertCatch[nPos],CONVERT_CATCH_LEN - nPos - 1,
                     "%02d  ",nVal);
            nPos += 2;
        }

        pTok = strtok(NULL,"&");
		
		//by ldf 2003-12-25 ��ÿ��֮��� ��,�� �ֿ�
		if(pTok)
		{
			strncpy(&m_szConvertCatch[nPos], "," , 1);
			nPos ++;
		}
    }
    return m_szConvertCatch;
}

const char *CTaskCreator::ConvertStatisticTime(const char *pTime)
{
    if(strcmp(pTime,"") == 0)
    {
        strncpy(m_szConvertCatch,S_PERF_TSK_NULL,CONVERT_CATCH_LEN);
    }
    else
    {
        strncpy(m_szConvertCatch,pTime,CONVERT_CATCH_LEN);
    }
    return m_szConvertCatch;
}
//
void CTaskCreator::MakeSubTitle(CMMLReport *pReport,const char *pContent)
{
    pReport->AddString(STR_NEWLINE);
    pReport->AddString(pContent);
    pReport->AddString(STR_NEWLINE);

	//by ldf 
	int nStrLen = strlen(pContent);
	for(int j = 0; j < nStrLen; j++)
	{
		pReport->AddString("-");
	}
	//pReport->AddString(STR_TITLE_SPIN);
	//end

    pReport->AddString(STR_NEWLINE);
}
//
const char * CTaskCreator::ConvertCounter(const char *pCounter)
{
    int nVal = atoi(pCounter);
    switch(nVal) 
    {
    case 1:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_CPU1,CONVERT_CATCH_LEN);
    	break;
    case 2:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_CPU2,CONVERT_CATCH_LEN);
    	break;
    case 3:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_PMA,CONVERT_CATCH_LEN);
    	break;
    case 4:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_PMT,CONVERT_CATCH_LEN);
    	break;
    case 5:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_FSA ,CONVERT_CATCH_LEN);
    	break;
    case 6:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_FST,CONVERT_CATCH_LEN);
    	break;
    case 7:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_BSA,CONVERT_CATCH_LEN);
    	break;
    case 8:
        strncpy(m_szConvertCatch,S_PERF_COUNTER_BST,CONVERT_CATCH_LEN);
    	break;
	case 9:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RSCDR,CONVERT_CATCH_LEN);
    	break;
    case 10:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RPSCDR,CONVERT_CATCH_LEN);
		break;
	case 11:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RGCDR,CONVERT_CATCH_LEN);
		break;
	case 12:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RPGCDR,CONVERT_CATCH_LEN);
		break;
	case 13:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RMCDR,CONVERT_CATCH_LEN);
		break;
	case 14:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RPMCDR,CONVERT_CATCH_LEN);
		break;
	case 15:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RSMO,CONVERT_CATCH_LEN);
		break;
	case 16:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RSMT,CONVERT_CATCH_LEN);
		break;
	case 17:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RLCSMO,CONVERT_CATCH_LEN);
		break;
	case 18:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RLCSMT,CONVERT_CATCH_LEN);
		break;
	case 19:
		strncpy(m_szConvertCatch,S_PERF_COUNTER_RLCSNI,CONVERT_CATCH_LEN);
    	break;
    default:
        break;
    }
    return m_szConvertCatch;
}

void CTaskCreator::QueryCounterInfo(STRING &TaskName, LIST<STRING> &listCounter)
{
    m_Query.ClearBind();
    m_Query.AddBindAnd("TSKN",TaskName,opEqual);
    int nItem = m_Query.QueryFirst();
    STRING strCounter = m_pDdaSet->GetFieldStringValue(nItem,"ITM");
    char szTmp[32];
    strncpy(szTmp,strCounter.c_str(),32);
    char *pTok = strtok(szTmp,"&");
    while(pTok != NULL)
    {
        listCounter.push_back(ConvertCounter(pTok));
        pTok = strtok(NULL,"&");
    }
}
STRING CTaskCreator::QueryTaskInfo(STRING &TaskName, STRING &Field)
{
    m_Query.ClearBind();
    m_Query.AddBindAnd("TSKN",TaskName,opEqual);
    int nItem = m_Query.QueryFirst();
    return m_pDdaSet->GetFieldStringValue(nItem,Field.c_str());
}

int CTaskCreator::SaveTask(STRING &szTskn)
{
    m_Query.ClearBind();
    m_Query.AddBindAnd("TSKN",szTskn,opEqual);
    int nItem = m_Query.QueryFirst();
    return m_pDdaSet->UpdateRecord(nItem);
}

void CTaskCreator::SetTaskFieldValue(STRING &szTskn, STRING &szField, STRING &szValue)
{
    m_Query.ClearBind();
    m_Query.AddBindAnd("TSKN",szTskn,opEqual);
    int nItem = m_Query.QueryFirst();
    m_pDdaSet->SetFieldStringValue(nItem,szField.c_str(),szValue);
}
