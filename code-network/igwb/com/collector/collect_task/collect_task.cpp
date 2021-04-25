#include "../../include/base_type.h"
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "collect_base.h"
#include "file_protocol.h"
#include "collect_factory.h"
#include "global_fun.h"

#include "collect_task.h"
#include "../../include/toolbox.h"
#include "../include/adapter_include.h"
#include "../include/res_config.h"

CCollectTask::CCollectTask(int nSwitchIndex)
{
    m_bActive         = TRUE;
    time_t tNow = time(NULL);
    m_tTestLink       = tNow - 50;
    m_nSwitchIndex    = nSwitchIndex;
    m_szSwitchName[0] = '\0';
    m_nLastColTime    = 0;
    m_nLastSucColTime = 0;
    m_nColBeginTime   = 0;
    m_nColEndTime     = 0;
    m_nColSucCycle    = CFG_CO_COL_SUC_CYCLE_DEFAULT;
    m_nCntFailCycle   = CFG_CO_CNT_FAIL_CYCLE_DEFAULT;
    m_nColFailCycle   = CFG_CO_COL_FAIL_CYCLE_DEFAULT;
    m_nFailedWaitTime = 0;
    m_bAskForExit     = FALSE;
    memset(&m_stCreate, 0, sizeof(m_stCreate));
    m_bOnceColAct     = FALSE;
    m_pCollector      = NULL;
    m_bLicensed       = FALSE;
    m_lLastWaitTime   = 0;
}

CCollectTask::~CCollectTask()
{
    if(m_pCollector != NULL)
    {
        delete m_pCollector;
        m_pCollector = NULL;
    }
}

//���������ļ�ȫ·����
void CCollectTask::SetCfgPath(const char* const szCfgPath)
{
    strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
    m_szCfgPath[MAX_PATH - 1] = '\0';
}

//��ʼ���ɼ�����
int CCollectTask::Init()
{
    if(m_pCollector != NULL)
    {
        COTRACE("�ɼ�����(ID=%d)�ѳ�ʼ��", m_nSwitchIndex);
        return ERR_SUCCESS;
    }

    int  nRet = 0;
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(ID=%d)�������ļ�(%s)ʧ��",
              m_nSwitchIndex,
              m_szCfgPath);
        return ERR_FAIL;
    }

    char szBeginBuf[20];
    //�������ļ��ж�ȡ�ȴ�ʱ����Ϣ
    INIFile.GetString(CFG_CO_SEC_SYSTEM,
                      CFG_CO_BEGIN_TIME,
                      CFG_CO_BEGIN_TIME_DEFAULT,
                      szBeginBuf,
                      sizeof(szBeginBuf));
    m_nColBeginTime = GetTimeFromString(szBeginBuf);

    char szEndBuf[20];
    INIFile.GetString(CFG_CO_SEC_SYSTEM,
                      CFG_CO_END_TIME,
                      CFG_CO_END_TIME_DEFAULT,
                      szEndBuf,
                      sizeof(szEndBuf));
    m_nColEndTime   = GetTimeFromString(szEndBuf);

    m_nMinColCycle  = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                     CFG_CO_MIN_COL_CYCLE,
                                     CFG_CO_MIN_COL_CYCLE_DEFAULT);
    if(m_nMinColCycle < 0)
    {
        m_nMinColCycle = CFG_CO_MIN_COL_CYCLE_DEFAULT;
    }                                 

    m_nColSucCycle  = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                     CFG_CO_COL_SUC_CYCLE,
                                     CFG_CO_COL_SUC_CYCLE_DEFAULT);
    if(m_nMinColCycle > m_nColSucCycle)
    {
        m_nColSucCycle = m_nMinColCycle;
    }                                 

    m_nCntFailCycle = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                     CFG_CO_CNT_FAIL_CYCLE,
                                     CFG_CO_CNT_FAIL_CYCLE_DEFAULT);
    if(m_nMinColCycle > m_nCntFailCycle)
    {
        m_nCntFailCycle = m_nMinColCycle;
    }                                 

    m_nColFailCycle = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                     CFG_CO_COL_FAIL_CYCLE,
                                     CFG_CO_COL_FAIL_CYCLE_DEFAULT);
    if(m_nMinColCycle > m_nColFailCycle)
    {
        m_nColFailCycle = m_nMinColCycle;
    }

    m_nColFailTimes = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                     CFG_CO_COL_FAIL_TIMES,
                                     CFG_CO_COL_FAIL_TIMES_DEFAULT);
    if(m_nColFailTimes < 0)
    {
        m_nColFailTimes = CFG_CO_COL_FAIL_TIMES_DEFAULT;
    }

    //added by chenliangwei
    char szListTimeBuf[1024];
    INIFile.GetString(CFG_CO_SEC_SYSTEM,
                      CFG_CO_COL_LIST_TIME,
                      CFG_CO_COL_LIST_TIME_DEFAULT,
                      szListTimeBuf,
                      sizeof(szListTimeBuf));
    //add end

    //added by chenliangwei, 2003-08-05
    char szOnceListTimeBuf[1024];
    INIFile.GetString(CFG_CO_SEC_SYSTEM,
                      CFG_CO_ONCE_COL_LIST_TIME,
                      CFG_CO_ONCE_COL_LIST_TIME_DEFAULT,
                      szOnceListTimeBuf,
                      sizeof(szOnceListTimeBuf));
    //add end


    //�����ö�Ӧ���������ж�ȡ�ȴ�ʱ����Ϣ
    char szSecName[S1240_CFG_LEN];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[S1240_CFG_LEN - 1] = '\0';

    //��ȡ�ɼ�������
    INIFile.GetString(szSecName,
                      CFG_CO_NAME,
                      CFG_CO_NAME_DEFAULT,
                      m_szSwitchName,
                      sizeof(m_szSwitchName));

    INIFile.GetString(szSecName, CFG_CO_BEGIN_TIME,
                      szBeginBuf,
                      szBeginBuf,
                      sizeof(szBeginBuf));
    m_nColBeginTime = GetTimeFromString(szBeginBuf);

    INIFile.GetString(szSecName, CFG_CO_END_TIME,
                      szEndBuf,
                      szEndBuf,
                      sizeof(szEndBuf));
    m_nColEndTime   = GetTimeFromString(szEndBuf);

    m_nColSucCycle  = INIFile.GetInt(szSecName, CFG_CO_COL_SUC_CYCLE,
                                     m_nColSucCycle);
    if(m_nMinColCycle > m_nColSucCycle)
    {
        m_nColSucCycle = m_nMinColCycle;
    }

    m_nCntFailCycle = INIFile.GetInt(szSecName, CFG_CO_CNT_FAIL_CYCLE,
                                     m_nCntFailCycle);
    if(m_nMinColCycle > m_nCntFailCycle)
    {
        m_nCntFailCycle = m_nMinColCycle;
    }

    m_nColFailCycle = INIFile.GetInt(szSecName, CFG_CO_COL_FAIL_CYCLE,
                                     m_nColFailCycle);
    if(m_nMinColCycle > m_nColFailCycle)
    {
        m_nColFailCycle = m_nMinColCycle;
    }

    m_nColFailTimes = INIFile.GetInt(szSecName,
                                     CFG_CO_COL_FAIL_TIMES,
                                     m_nColFailTimes);
    if(m_nColFailTimes < 0)
    {
        m_nColFailTimes = CFG_CO_COL_FAIL_TIMES_DEFAULT;
    }

    //add by chenliangwei
    INIFile.GetString(szSecName,
                      CFG_CO_COL_LIST_TIME,
                      szListTimeBuf,
                      szListTimeBuf,
                      sizeof(szListTimeBuf));

    if (szListTimeBuf[0] != '\0')
    {
        char *pToken = strtok(szListTimeBuf, ",");
        while(pToken)
        {
            int nTime = GetTimeFromString(pToken);

            BOOL bFind = FALSE;
            LIST<int>::iterator iter = m_ColListTime.begin();
            for(; iter != m_ColListTime.end(); iter++)
            {
                if(nTime == (*iter))
                {
                    bFind = TRUE;
                    break;
                }
            }
            if(!bFind) m_ColListTime.push_back(nTime);
            pToken = strtok(NULL, ",");
        }
    }

    m_ColListTime.sort();

    if (m_ColListTime.size() > 0)
    {
        m_nLastSucColTime = time(NULL);
    }
    else
    {
        m_nLastSucColTime = time(NULL) - m_nColSucCycle;
    }
    //add end

    //add by chenliangwei, 2003-08-05
    INIFile.GetString(szSecName,
                      CFG_CO_ONCE_COL_LIST_TIME,
                      szOnceListTimeBuf,
                      szOnceListTimeBuf,
                      sizeof(szOnceListTimeBuf));

    //����һ��������
    if (szOnceListTimeBuf[0] != '\0')
    {    
        //��ʱ�����е�������','
        int nstrColLen = strlen(szOnceListTimeBuf);
        if (szOnceListTimeBuf[nstrColLen - 1] != ',')
        {
            szOnceListTimeBuf[nstrColLen] = ',';
            szOnceListTimeBuf[nstrColLen + 1] = '\0';
        }
        
        //���һ����ʱ�����У�ע:Լ��ʱ�����зָ���Ϊ'^'
        char *ss0 = NULL;
        char *ss1 = NULL;
        ss0 = szOnceListTimeBuf;
        while ((ss1 = strchr(ss0, ',')) != NULL)
        {
            *ss1 = '\0';
            
            //��ʱ���ʽ(YYYY-MM-DD HH:mm:SS)ת��Ϊtime_t
            tm tmTime = {0};
            const char *pToken = strtok(ss0, "-");
            ss0 = ss1+1;
            
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_year = atoi(pToken) - 1900;
            
            pToken = strtok(NULL, "-");
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_mon = atoi(pToken) - 1;
            
            pToken = strtok(NULL, " ");
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_mday = atoi(pToken);
            
            pToken = strtok(NULL, ":");
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_hour = atoi(pToken);
            
            pToken = strtok(NULL, ":");
            if(pToken == NULL)
            {
                continue;
            }
            tmTime.tm_min = atoi(pToken);
            tmTime.tm_sec = 0;
            
            time_t tTime = mktime(&tmTime);
            
            m_MutexForSchl.acquire();
            
            BOOL bFind = FALSE;
            LIST<time_t>::iterator it = m_OnceColListTime.begin();
            for(; it != m_OnceColListTime.end(); it++)
            {
                if(tTime == (*it))
                {
                    bFind = TRUE;
                    break;
                }
            }
            
            if(!bFind) 
            {
                //����ǹ�ʱ��ʱ�䣬�����뵽һ���Բɼ�������
                if (tTime >= time(NULL))
                {
                    m_OnceColListTime.push_back(tTime);
                }
            }
            
            m_MutexForSchl.release();
        }
        
        m_MutexForSchl.acquire();
        m_OnceColListTime.sort();
        SaveOnceColSchl();
        m_MutexForSchl.release();
    }
    //added end

    m_stCreate.nSwitchIndex = m_nSwitchIndex;
    INIFile.GetString(szSecName, CFG_CO_TYPE,
                      CFG_CO_TYPE_DEFAULT,
                      m_stCreate.szCollectId,
                      sizeof(m_stCreate.szCollectId));
    INIFile.GetString(szSecName, CFG_CO_PROTOCOL,
                      CFG_CO_PROTOCOL_DEFAULT,
                      m_stCreate.szProtocolId,
                      sizeof(m_stCreate.szProtocolId));

    strncpy(m_stCreate.szSwitchName,
            m_szSwitchName,
            sizeof(m_stCreate.szSwitchName));
    m_stCreate.szSwitchName[sizeof(m_stCreate.szSwitchName) - 1] = '\0';

    m_pCollector = CCollectFactory::CreateCollectObject(m_szCfgPath, m_stCreate);
    if(m_pCollector == NULL)
    {
        COTRACE("�����ɼ�����(%s)ʧ��", m_szSwitchName);
        return ERR_FAIL;
    }

    m_pCollector->SetCfgPath(m_szCfgPath);

    //��ʼ���ɼ�����
    nRet = m_pCollector->Col_Init();
    if(nRet != ERR_SUCCESS)
    {
        COTRACE("�ɼ�����(%s)��ʼ��ʧ��", m_szSwitchName);
        return ERR_FAIL;
    }

    //��ʼ����·��Ϣ
    m_MutexForLinkMap.acquire();
    m_pCollector->GetLinkInfo(m_LinkStatusMap);
    m_MutexForLinkMap.release();

    return ERR_SUCCESS;
}

void CCollectTask::DeActive()
{
    m_bActive = FALSE;
}

void CCollectTask::Active()
{
    m_bActive = TRUE;
}

//����һ�βɼ�����
BOOL CCollectTask::WaitNextTime()
{
    tm tmLastNow = *localtime(&m_nLastSucColTime);
    long tLastTime = tmLastNow.tm_hour * 3600
                     + tmLastNow.tm_min * 60
                     + tmLastNow.tm_sec;

    m_bOnceColAct = false;
    time(&m_lLastWaitTime);

    while(!m_bAskForExit)
    {
        time_t tNow = time(NULL);
        tm tmNow = *localtime(&tNow);
        long tNowTime = tmNow.tm_hour * 3600
                        + tmNow.tm_min * 60
                        + tmNow.tm_sec;

        //added by chenliangwei, 2003-08-05
        if(m_tTestLink + 60 < tNow)
        {
            TestLink();
            m_tTestLink = tNow;
        }
        //added end

        //����Ƿ����ֶ��ɼ�����
        m_MutexForList.acquire();
        int nSpecSFileCount = m_SpecColFileList.size();
        m_MutexForList.release();

        if ((tNow - m_nLastColTime >=  m_nFailedWaitTime) && (nSpecSFileCount > 0))
        {
            m_nFailedWaitTime = 0;
            return true;
        }

        m_MutexForSchl.acquire();

        //ʱ�����е����ȼ��ϸ�
        if(m_ColListTime.size() > 0)
        {
            LIST<int>::iterator i;
            for(i = m_ColListTime.begin(); i != m_ColListTime.end(); i++)
            {
                //���￼�ǵ��ǿ�������
                if(tNowTime < tLastTime)
                {
                    tLastTime = 0;
                }
                if((tNowTime >= *i) && (tLastTime < *i))
                {
                    m_nLastSucColTime = tNow; //added by chenliangwei, 2003-05-20
                    m_MutexForSchl.release();
                    return true;
                }
            }
        }

        //modifyed by chenliangwei, 2003-08-05,����else
        //ʱ�䷶Χ
        else if(((tNowTime >= m_nColBeginTime) && (tNowTime <= m_nColEndTime))
           || ((0 == m_nColBeginTime) && (0 == m_nColEndTime)))
        {
            //added by chenliangwei, 2003-05-20
            //����ɼ��ɹ�����Ϊ0, ֱ�ӷ���
            if (m_nColSucCycle == 0)
            {
                m_nLastSucColTime = tNow;
                m_MutexForSchl.release();
                return true;
            }
            //added end

            if ((tNow - m_nLastSucColTime >=  m_nColSucCycle) && (tNow - m_nLastColTime >=  m_nFailedWaitTime))
            {
                while(m_nLastSucColTime + m_nColSucCycle <= tNow)
                {
                    m_nLastSucColTime += m_nColSucCycle;
                }
                m_MutexForSchl.release();
                return true;
            }
        }

        //����ٿ��ǵ����ɼ�����
        if(m_OnceColListTime.size() > 0)
        {
            time_t tActTime = m_OnceColListTime.front();
            if(tNow >= tActTime)
            {
                m_OnceColListTime.pop_front();
                SaveOnceColSchl();
                m_bOnceColAct = true;
                m_MutexForSchl.release();
                return true;
            }
        }

        m_MutexForSchl.release();
        ACE_OS::sleep(1);
    }

    return false;
}

//�����ɼ�����
int CCollectTask::Collect()
{
    int nRet = 0;

    // ��ʼ��
    if(m_pCollector == NULL)
    {
        COTRACE("�ɼ�����(%s)��δ��ʼ���ɹ��������ɼ�����",
              m_szSwitchName);
        nRet = Init();
        if(nRet != ERR_SUCCESS)
        {
            return nRet;
        }
    }

    //������ѭ��
    while(WaitNextTime())
    {
        /*commented by chenliangwei, 2003-05-21
        WRITELOG_C("�ɼ�����(%s)��ʼ��һ�ֵĲɼ�",
                   m_szSwitchName);
        */
        if(!m_bLicensed)
        {
            if(m_lLastWaitTime < 5)
            {
                ACE_OS::sleep(5);
            }
            continue;
        }

        COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
               "�ɼ�����(%s)��ʼ��һ�ֵĲɼ�",
               m_szSwitchName);
        //����
        nRet = m_pCollector->Connect();

        m_nFailedWaitTime = 0;
        if(ERR_SUCCESS != nRet)
        {
            WRITELOG_F("�ɼ�����(%s)����ʧ��(������=%d)",
                       m_szSwitchName,
                       nRet);
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "�ɼ�����(%s)����ʧ��(������=%d)",
                   m_szSwitchName,
                   nRet);
            //aWRITELOG_F("Collector(ID=%d) failed to connect to remote host(ErrCode=%d)", m_nSwitchIndex, nRet);
            //aMSGOUT(MSGOUT_LEVEL_URGENT, "Collector(ID=%d) failed to connect to remote host(ErrCode=%d)",
            //       m_nSwitchIndex, nRet);
            m_nFailedWaitTime = m_nCntFailCycle;
        }
        else
        {
            COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                   "�ɼ�����(%s)���ӳɹ�����ʼ���вɼ�...",
                   m_szSwitchName);

            //aMSGOUT(MSGOUT_LEVEL_IMPORTANT, "Collector(ID=%d) connected, begin to collect...",
            //       m_nSwitchIndex);

            //���һ�������Ĳɼ�����
            nRet = CollectProcess();
            if(ERR_SUCCESS == nRet)
            {
                /*commented by chenliangwei, 2003-05-21
                WRITELOG_C("�ɼ�����(%s)���ֲɼ�����",
                           m_szSwitchName);
                */

                COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                       "�ɼ�����(%s)���ֲɼ�����",
                       m_szSwitchName);
                //aWRITELOG_C("Collector(ID=%d) finished a set of collection", m_nSwitchIndex);
                //aMSGOUT(MSGOUT_LEVEL_IMPORTANT, "Collector(ID=%d) finished a set of collection",
                //       m_nSwitchIndex);
                m_nFailedWaitTime = 0;
            }
            else
            {
                WRITELOG_F("�ɼ�����(%s)���ֲɼ�ʧ��(������=%d)",
                           m_szSwitchName,
                           nRet);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "�ɼ�����(%s)���ֲɼ�ʧ��(������=%d)",
                       m_szSwitchName,
                       nRet);
                //aWRITELOG_C("Collector(ID=%d) failed to do a set of collection(ErrCode=%d)", m_nSwitchIndex, nRet);
                //aMSGOUT(MSGOUT_LEVEL_URGENT, "Collector(ID=%d) failed to do a set of collection(ErrCode=%d)",
                //       m_nSwitchIndex, nRet);
                m_nFailedWaitTime = m_nColFailCycle;
            }

            //�Ͽ�����
            m_pCollector->Disconnect();
            COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                   "�ɼ�����(%s)�Ͽ�����",
                   m_szSwitchName);
            //aMSGOUT(MSGOUT_LEVEL_IMPORTANT, "Collector(ID=%d) disconnect", m_nSwitchIndex);
        }

        //ɾ������ԭʼ�����ļ�
        if(!m_pCollector->ClearOldFile())
        {
            WRITELOG_F("�ɼ�����(%s)ɾ�����ڵ�ԭʼ�����ļ�ʧ��",
                       m_szSwitchName);
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "�ɼ�����(%s)ɾ�����ڵ�ԭʼ�����ļ�ʧ��",
                   m_szSwitchName);
            //aWRITELOG_F("Collector(ID=%d) failed to delete outdate file", m_nSwitchIndex);
            //aMSGOUT(MSGOUT_LEVEL_URGENT, "Collector(ID=%d) failed to delete outdate file", m_nSwitchIndex);
        }

        m_nLastColTime = time(NULL);
        if(m_nFailedWaitTime > 0)
        {
            /*commented by chenliangwei, 2003-05-21
            WRITELOG_C("�ɼ�����(%s)�ȴ�%d��",
                       m_szSwitchName,
                       m_nFailedWaitTime);
            */
            COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                   "�ɼ�����(%s)�ȴ�%d��",
                   m_szSwitchName,
                   m_nFailedWaitTime);
            //aWRITELOG_F("Collector(ID=%d) will wait for %d (s)", m_nSwitchIndex, m_nFailedWaitTime);
            //aMSGOUT(MSGOUT_LEVEL_IMPORTANT, "Collector(ID=%d) will wait for %d (s)",
            //       m_nSwitchIndex, m_nFailedWaitTime);
        }
        else
        {
            /*commented by chenliangwei, 2003-05-21
            WRITELOG_C("�ɼ�����(%s)�ȴ�%d��",
                       m_szSwitchName,
                       m_nColSucCycle);
            */

            COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                   "�ɼ�����(%s)�ȴ�%d��",
                   m_szSwitchName,
                   m_nColSucCycle);
            //aWRITELOG_C("Collector(ID=%d) will wait for %d (s)", m_nSwitchIndex, m_nColSucCycle);
            //aMSGOUT(MSGOUT_LEVEL_IMPORTANT, "Collector(ID=%d) will wait for %d (s)",
            //       m_nSwitchIndex, m_nColSucCycle);
        }
    }

    WRITELOG_C("�ɼ�����(%s)�˳�",
               m_szSwitchName);
    COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
           "�ɼ�����(%s)�˳�",
           m_szSwitchName);
    //aWRITELOG_C("Collector(ID=%d) exit", m_nSwitchIndex);
    //aMSGOUT(MSGOUT_LEVEL_IMPORTANT, "Collector(ID=%d) exit", m_nSwitchIndex);
    return ERR_FAIL;
}

/********************************************************************
Function:        int  CollectProcess( );
Description:     ����������5ESS�ɼ������ļ���ȫ���̣�
Called By:       ���ʹ����
Table Accessed:  ��
Table Updated:   ��
Input:           ������ָ��
Output:          ��
Return:          int �ͣ�����ֵ�ĺ������£�
                 ERR_SUCCESS, ����ɹ���
                 FT_CONNFAIL, ��������ʧ��
                 COL_LISTFAIL ,�ļ��б�ʧ��
                 COL_COLLECTFAIL, �ɼ�����ʧ��

Others:         ��
*********************************************************************/
int CCollectTask::CollectProcess()
{

    if(NULL == m_pCollector)
    {
        COTRACE("�ɼ�����(%s)δ��ʼ��", m_szSwitchName);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s)δ��ʼ��",
               m_szSwitchName);
        return ERR_FAIL;
    }


    //���òɼ��ļ������´����Ŀ¼m_strCurDir
    int nRet = m_pCollector->CreateLocSubDir();

    SColFileReq stReq;

    //�õ��ļ��б�
    SListOut listOut;
    nRet = m_pCollector->Col_GetList(listOut);
    if(nRet != ERR_SUCCESS)
    {
        WRITELOG_F("�ɼ�����(%s)��ȡ�ļ��б�ʧ��",
                   m_szSwitchName);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s)��ȡ�ļ��б�ʧ��",
               m_szSwitchName);
    }
    else
    {
        m_ColFileList.clear();
    }

    while(listOut.FileList.size() > 0)
    {
        memset(&stReq, 0, sizeof(stReq));
        stReq.nFileSize = listOut.FileList.front().nFileSize;
        strncpy(stReq.szSrcName,
                listOut.FileList.front().szFileName,
                sizeof(stReq.szSrcName));
        m_ColFileList.push_back(stReq);
        listOut.FileList.erase(listOut.FileList.begin());
    }

    m_MutexForList.acquire();
    LIST<SColFileReq>::iterator i = m_SpecColFileList.begin();
    for(; i != m_SpecColFileList.end(); i++)
    {
        LIST<SColFileReq>::iterator j = m_ColFileList.begin();
        for(; j != m_ColFileList.end(); j++)
        {
            if(strcmp((*i).szSrcName, (*j).szSrcName) == 0)
            {
                if((*i).szDstName[0] == '\0') 
                {
                    m_ColFileList.erase(j);
                }
                break;
            }
        }
    }

    int nFileCount = m_SpecColFileList.size() + m_ColFileList.size();
    m_MutexForList.release();

    COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
           "�ɼ�����(%s)��ǰ��%d���ļ��ȴ��ɼ�",
           m_szSwitchName,
           nFileCount);
    WRITELOG_C("�ɼ�����(%s)��ǰ��%d���ļ��ȴ��ɼ�",
               m_szSwitchName,
               nFileCount);

    if(nFileCount == 0)
    {
        return ERR_SUCCESS;
    }

    int  nFailCount = 0;  //�����ļ�ʧ�ܵĴ���
    int nReturn = ERR_FAIL;
    m_MutexForList.acquire();
    int nSpecSFileCount = m_SpecColFileList.size();
    m_MutexForList.release();

    while(((m_ColFileList.size() > 0)
           || (nSpecSFileCount > 0))
          && !m_bAskForExit)
    {

        stReq = m_ColFileList.front();
        if(nSpecSFileCount > 0)
        {
            m_MutexForList.acquire();
            stReq = m_SpecColFileList.front();
            if(stReq.szDstName[0] == '\0')
            {
                LIST<SColFileReq>::iterator j = m_ColFileList.begin();
                for(; j != m_ColFileList.end(); j++)
                {
                    if(strcmp(stReq.szSrcName, (*j).szSrcName) == 0)
                    {
                        m_ColFileList.erase(j);
                        break;
                    }
                }
            }
            m_MutexForList.release();
        }
        
        if(!m_bActive && (nSpecSFileCount == 0) && !m_bOnceColAct)
        {
            return ERR_SUCCESS;
        }
        
        time_t tBegin;
        time(&tBegin);
        nRet = m_pCollector->Col_GetFile(stReq);
        time_t tEnd;
        time(&tEnd);
        long   lCostSecond = tEnd - tBegin;
        if(lCostSecond <= 0)
        {
            lCostSecond = 1;
        }
        float  fSpeed = stReq.nFileSize;
        fSpeed /= 1024;
        fSpeed /= lCostSecond;
        if (nRet == ERR_SUCCESS)
        {
            char szTraceMsg[1024];
            SNPRINTF(szTraceMsg,
                     sizeof(szTraceMsg),
                     //"Collector(ID=%d) collect file successfully(%s to %s,Length = %d bytes)",
                     "�ɼ�����(%s)�ɼ��ļ��ɹ�(%s->%s,����Ϊ%d�ֽ�,�ٶ�Ϊ%.2fKB/s)",
                     m_szSwitchName,
                     stReq.szSrcName,
                     stReq.szDstName,
                     stReq.nFileSize,
                     fSpeed);
            WRITELOG_C(szTraceMsg);
            COMSGOUT(MSGOUT_LEVEL_IMPORTANT, szTraceMsg);

            if(nSpecSFileCount > 0)
            {
                m_MutexForList.acquire();
                m_SpecColFileList.pop_front();
                m_MutexForList.release();
            }
            else
            {
                //added by chenliangwei
                char szPreviousFile[60];

                if (!m_pCollector->CheckFileSN(stReq.szSrcName, szPreviousFile))
                {
                    SNPRINTF(szTraceMsg,
                             sizeof(szTraceMsg),
                             "�ɼ�����(%s)�ɼ����ļ�(%s)��ǰһ���ɼ��ļ�(%s)�����кŲ�����",
                             m_szSwitchName,
                             stReq.szSrcName,
                             szPreviousFile);

                    WRITELOG_C(szTraceMsg);
                    COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);

                    SInnerAlarm Alarm;
                    Alarm.yAlarmType    = AT_EVENT;
                    Alarm.uAlarmID      = ALARM_ID_FILE_SN_CHECK_FAULT;
                    Alarm.yAlarmPara[0] = ALARM_PARA_NONE;
                    SENDALARM(&Alarm);
                }
                //added end


                m_ColFileList.pop_front();
            }

            //added by chenliangwei, 2003-05-20
            nFailCount = 0;
            //added end

            nReturn = ERR_SUCCESS;
        }
        else
        {
            WRITELOG_F("�ɼ�����(%s)�ɼ��ļ�(%s)ʧ��(������=%d)",
                       m_szSwitchName,
                       stReq.szSrcName,
                       nRet);
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "�ɼ�����(%s)�ɼ��ļ�(%s)ʧ��(������=%d)",
                   m_szSwitchName,
                   stReq.szSrcName,
                   nRet);
            nFailCount++;

            if(nFailCount >= m_nColFailTimes)
            {
                WRITELOG_F("�ɼ�����(%s)�ɼ��ļ�(%s)����ʧ��%d��,�����ɼ����ļ�",
                           m_szSwitchName,
                           stReq.szSrcName,
                           m_nColFailTimes);
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                       "�ɼ�����(%s)�ɼ��ļ�(%s)����ʧ��%d��,�����ɼ����ļ�",
                       m_szSwitchName,
                       stReq.szSrcName,
                       m_nColFailTimes);

                if(nSpecSFileCount > 0)
                {
                    m_MutexForList.acquire();
                    m_SpecColFileList.pop_front();
                    m_MutexForList.release();
                }
                else
                {
                    m_ColFileList.pop_front();
                }

                SInnerAlarm Alarm;
                Alarm.yAlarmType    = AT_EVENT;
                Alarm.uAlarmID      = ALARM_ID_COLLECT_FILE_FAULT;
                Alarm.yAlarmPara[0] = ALARM_PARA_NONE;
                SENDALARM(&Alarm);

                nFailCount = 0;
            }
            else
            {
                ACE_OS::sleep(2);
            }
        }

        m_MutexForList.acquire();
        nSpecSFileCount = m_SpecColFileList.size();
        m_MutexForList.release();
    }

    return  nReturn;
}

//�ȴ��˳��Ĳ���
void CCollectTask::WaitForExit()
{
    m_bAskForExit = TRUE;
}

//added by chenliangwei, 2003-08-05
//�������ӷ����
int  CCollectTask::TestLink()
{
    //�����·
    if (m_pCollector == NULL)
    {
        return ERR_FAIL;
    }

    MAP<STRING, SLinkStatusMsg> link_msg_map;
    m_pCollector->TestLink(link_msg_map);
    
    
    //���µ�ǰ����·״̬��Ϣ
    m_MutexForLinkMap.acquire();

    MAP<STRING, SLinkStatusMsg>::iterator iter;
    int nLinkIndex = 0;
    for(iter = link_msg_map.begin();
        iter != link_msg_map.end(); 
        iter++)
    {
        SLinkStatusMsg& cur_link_msg = iter->second;
        SLinkStatusMsg last_link_msg = cur_link_msg;
        last_link_msg.nStatus        = 0;
        if(m_LinkStatusMap.find(iter->first) != m_LinkStatusMap.end())
        {
            last_link_msg = m_LinkStatusMap[iter->first];
        }

        //���͸澯
        if(0 == cur_link_msg.nStatus && 0 != last_link_msg.nStatus)
        {
            SInnerAlarm Alarm;
            Alarm.yAlarmType    = AT_RESTORE;
            Alarm.uAlarmID      = ALARM_ID_X25_LINK_FAIL;
            Alarm.yAlarmPara[0] = cur_link_msg.nAlarmPara;
            SENDALARM(&Alarm);
            WRITELOG_C("�ɼ�����(%s)���߼���·(%s)�ָ�",
                       m_szSwitchName,
                       iter->first.c_str());
        }
        else if(0 == last_link_msg.nStatus && 0 != cur_link_msg.nStatus)
        {
            SInnerAlarm Alarm;
            Alarm.yAlarmType    = AT_FAULT;
            Alarm.uAlarmID      = ALARM_ID_X25_LINK_FAIL;
            Alarm.yAlarmPara[0] = cur_link_msg.nAlarmPara;
            SENDALARM(&Alarm);
            WRITELOG_F("�ɼ�����(%s)���߼���·(%s)����",
                       m_szSwitchName,
                       iter->first.c_str());
        }

        //��ʾ��ǰ��·��״̬
        if(cur_link_msg.nStatus == 0)
        {
            COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                   "�ɼ�����(%s)���߼���·(%s)����",
                   m_szSwitchName,
                   iter->first.c_str());
        }
        else
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "�ɼ�����(%s)���߼���·(%s)����",
                   m_szSwitchName,
                   iter->first.c_str());
        }

        m_LinkStatusMap[iter->first] = cur_link_msg;

    }

    m_MutexForLinkMap.release();    

    return ERR_SUCCESS;
  
}
//added end

int CCollectTask::SetAlarmLimit(const char* const szTP,
                                int nAlarmLimit)
{
    if(NULL != m_pCollector)
    {
        return m_pCollector->SetAlarmLimit(szTP, nAlarmLimit);
    }
    return ERR_FAIL;
}

int CCollectTask::GetSpecFile(const char* const szModule,
                              const char* const szSrcFile,
                              const char* const szDstFile)
{
    if(NULL == m_pCollector)
    {
        return ERR_FILE_CANNOT_BE_FOUND;
    }

    int nRet = m_pCollector->CanGetSpecFile(szModule, szSrcFile);
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    SColFileReq stReq;
    memset(&stReq, 0, sizeof(stReq));
    strncpy(stReq.szSrcName,
            szSrcFile,
            sizeof(stReq.szSrcName));

    if((szDstFile != 0) && (szDstFile[0] != 0))
    {
        strncpy(stReq.szDstName,
                szDstFile,
                sizeof(stReq.szDstName));
    }
    stReq.nFileSize = m_pCollector->GetFileLen(szModule, szSrcFile);
    if(stReq.nFileSize <= 0)
    {
        return ERR_FAIL;
    }

    m_MutexForList.acquire();
    LIST<SColFileReq>::iterator i = m_SpecColFileList.begin();
    for(; i != m_SpecColFileList.end(); i++)
    {
        if(strcmp((*i).szSrcName, stReq.szSrcName) == 0)
        {
            m_MutexForList.release();
            return ERR_FILE_BE_PUT_IN_LIST;
        }
    }
    m_SpecColFileList.push_back(stReq);
    m_MutexForList.release();

    return ERR_SUCCESS;
}

//��ȡ��������Ϣ
int CCollectTask::GetSwitchInfo(LIST<STRING>& label,
                                LIST<STRING>& content)
{
    if(NULL != m_pCollector)
    {
        return m_pCollector->GetSwitchInfo(label, content);
    }

    return ERR_FAIL;
}

//��ȡģ����Ϣ
int CCollectTask::GetModuleInfo(const char* const szModule,
                                LIST<STRING>& label,
                                LIST<STRING>& content)
{
    if(NULL != m_pCollector)
    {
        return m_pCollector->GetModuleInfo(szModule,
                                           label,
                                           content);
    }

    return ERR_FAIL;
}

//modifyed by chenliangwei, 2003-08-05
int CCollectTask::GetLinkStatus(LIST<SLinkStat>& LinkStat)
{

    //�õ���ǰ����·״̬��Ϣ
    m_MutexForLinkMap.acquire();

    MAP<STRING, SLinkStatusMsg>::iterator iter;
    for(iter = m_LinkStatusMap.begin();
        iter != m_LinkStatusMap.end(); 
        iter++)
    {
        SLinkStat link_stat;
        strncpy(link_stat.szModuleLabel, 
                m_szSwitchName,
                sizeof(link_stat.szModuleLabel));
        link_stat.szModuleLabel[sizeof(link_stat.szModuleLabel) - 1] = '\0';

       
        SLinkStatusMsg& link_msg = iter->second;

        SNPRINTF(link_stat.szLinkLabel, 
                 sizeof(link_stat.szLinkLabel),
                 "%s(%s)",
                iter->first.c_str(),
                link_msg.szRemoteAddr);
        link_stat.szLinkLabel[sizeof(link_stat.szLinkLabel) - 1] = '\0';


        strncpy(link_stat.szLocalAddr, 
                link_msg.szLocalAddr,
                sizeof(link_stat.szLocalAddr));
        link_stat.szLocalAddr[sizeof(link_stat.szLocalAddr) - 1] = '\0';

        if(link_msg.nStatus == 0)
        {
            link_stat.yStat = ONLINK;
        }
        else
        {
            link_stat.yStat = OFFLINK;
        }
        LinkStat.push_back(link_stat);
    }

    m_MutexForLinkMap.release();

    return ERR_SUCCESS;
}
//modifyed end

//modifyed by chenliangwei, 2003-08-05
//���õ��βɼ�����
int CCollectTask::SetOnceColSchl(LIST<time_t>& OnceSchlList)
{
    m_MutexForSchl.acquire();

    m_OnceColListTime.clear();

    LIST<time_t>::iterator i;
    for(i = OnceSchlList.begin(); i != OnceSchlList.end(); i++)
    {
        m_OnceColListTime.push_back(*i);
    }
   
    SaveOnceColSchl();

    m_MutexForSchl.release();

    return 0;
        
}
//modifyed end

//added by chenliangwei, 2003-08-05
int CCollectTask::SaveOnceColSchl()
{
    m_MutexForSchl.acquire();

    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(ID=%d)�������ļ�(%s)ʧ��",
              m_nSwitchIndex,
              m_szCfgPath);
        m_MutexForSchl.release();
        return ERR_FAIL;
    }    

    char szColTime[1024] = "";
    LIST<time_t>::iterator i;
    for(i = m_OnceColListTime.begin(); i != m_OnceColListTime.end(); i++)
    {
        tm tmTime = *localtime(&(*i));
        SNPRINTF(szColTime,
                 sizeof(szColTime),
                 "%s%04d-%02d-%02d %02d:%02d:%02d,",
                 szColTime,
                 tmTime.tm_year + 1900,
                 tmTime.tm_mon + 1,
                 tmTime.tm_mday,
                 tmTime.tm_hour,
                 tmTime.tm_min,
                 tmTime.tm_sec);
        
        szColTime[sizeof(szColTime) - 1] = '\0';
    }

    if(strlen(szColTime) > 0)
    {
        szColTime[strlen(szColTime) - 1] = '\0';
    }

	char szSwitch[40];
	SNPRINTF(szSwitch, sizeof(szSwitch),"%s%d",
		CFG_CO_SEC_SWITCH, m_nSwitchIndex);
	szSwitch[sizeof(szSwitch) - 1] = '\0';
    INIFile.WriteString(szSwitch,
                        CFG_CO_ONCE_COL_LIST_TIME,
                        szColTime);

    m_MutexForSchl.release();
    
    return 0;
}
//added end

//��ȡ���βɼ������б�
int CCollectTask::GetOnceColSchl(LIST<time_t>& OnceSchlList)
{
    m_MutexForSchl.acquire();

    LIST<time_t>::iterator i;
    for(i = m_OnceColListTime.begin(); i != m_OnceColListTime.end(); i++)
    {
        OnceSchlList.push_back(*i);
    }

    m_MutexForSchl.release();

    return 0;
}

//��ȡ�ɼ����Բ���
int CCollectTask::GetColRetryPara(int& nCntFailCycle,
                                  int& nColFailCycle,
                                  int& nFileRetryTimes)
{
    m_MutexForSchl.acquire();

    nCntFailCycle   = m_nCntFailCycle;
    nColFailCycle   = m_nColFailCycle;
    nFileRetryTimes = m_nColFailTimes;

    m_MutexForSchl.release();

    return 0;
}

//���òɼ����Բ���
int CCollectTask::SetColRetryPara(int nCntFailCycle,
                                  int nColFailCycle,
                                  int nFileRetryTimes)
{

    if(nCntFailCycle < m_nMinColCycle)
    {
        nCntFailCycle = m_nMinColCycle;
    }

    if(nColFailCycle < m_nMinColCycle)
    {
        nColFailCycle = m_nMinColCycle;
    }

    if(nFileRetryTimes < 0)
    {
        nFileRetryTimes = m_nColFailTimes;
    }


    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(ID=%d)�������ļ�(%s)ʧ��",
              m_nSwitchIndex,
              m_szCfgPath);
        return ERR_FAIL;
    }

    //modified by chenliangwei, 2003-07-05
    m_MutexForSchl.acquire();
    m_nCntFailCycle = nCntFailCycle;
    m_nColFailCycle = nColFailCycle;
    m_nColFailTimes = nFileRetryTimes;
    m_MutexForSchl.release();
    //modified end

	char szSwitch[40];
	SNPRINTF(szSwitch, sizeof(szSwitch),"%s%d",
		CFG_CO_SEC_SWITCH, m_nSwitchIndex);
	szSwitch[sizeof(szSwitch) - 1] = '\0';
    INIFile.WriteInt(szSwitch,
                     CFG_CO_CNT_FAIL_CYCLE,
                     m_nCntFailCycle);

    INIFile.WriteInt(szSwitch,
                     CFG_CO_COL_FAIL_CYCLE,
                     m_nColFailCycle);

    INIFile.WriteInt(szSwitch,
                     CFG_CO_COL_FAIL_TIMES,
                     m_nColFailTimes);


    return 0;
}


//��ȡ�����Բɼ�����
int CCollectTask::GetColCyclePara(int& nBegin,
                                  int& nEnd, 
                                  int& nCycle)
{
    m_MutexForSchl.acquire();

    nBegin = m_nColBeginTime;
    nEnd   = m_nColEndTime;
    nCycle = m_nColSucCycle;

    m_MutexForSchl.release();

    return 0;
}

//���������Բɼ�����
int CCollectTask::SetColCyclePara(int nBegin,
                                  int nEnd,
                                  int nCycle)
{

    if(nBegin < 0)
    {
        nBegin = m_nColBeginTime;
    }

    if(nEnd < 0)
    {
        nEnd = m_nColEndTime;
    }

    if(nCycle < m_nMinColCycle)
    {
        nCycle = m_nMinColCycle;
    }

    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(ID=%d)�������ļ�(%s)ʧ��",
              m_nSwitchIndex,
              m_szCfgPath);
        return ERR_FAIL;
    }

    //modified by chenliangwei, 2003-07-05
    m_MutexForSchl.acquire();
    m_nColEndTime   = nEnd;
    m_nColBeginTime = nBegin;
    m_nColSucCycle  = nCycle;
    m_MutexForSchl.release();
    //modified end

    char szColTime[50];
    SNPRINTF(szColTime,
             sizeof(szColTime),
             "%02d:%02d",
             m_nColBeginTime/3600,
             m_nColBeginTime/60%60);
    szColTime[sizeof(szColTime) - 1] = '\0';

	//---add yang
	char szSwitch[40];
	SNPRINTF(szSwitch, sizeof(szSwitch),"%s%d",
		CFG_CO_SEC_SWITCH, m_nSwitchIndex);
	szSwitch[sizeof(szSwitch) - 1] = '\0';
    //====add end
    INIFile.WriteString(szSwitch,
                        CFG_CO_BEGIN_TIME,
                        szColTime);

    SNPRINTF(szColTime,
             sizeof(szColTime),
             "%02d:%02d",
             m_nColEndTime/3600,
             m_nColEndTime/60%60);
    szColTime[sizeof(szColTime) - 1] = '\0';

    INIFile.WriteString(szSwitch,
                        CFG_CO_END_TIME,
                        szColTime);

    INIFile.WriteInt(szSwitch,
                     CFG_CO_COL_SUC_CYCLE,
                     nCycle);


    return 0;
}

//��ȡ���з�ʽ�ɼ��Ĳ���
int CCollectTask::GetColTimeList(LIST<int>& TimeList)
{
    m_MutexForSchl.acquire();

    LIST<int>::iterator i;
    for(i = m_ColListTime.begin(); i != m_ColListTime.end(); i++)
    {
        TimeList.push_back(*i);
    }
    
    m_MutexForSchl.release();

    return 0;
}

//�������з�ʽ�ɼ��Ĳ���
int CCollectTask::SetColTimeList(LIST<int>& TimeList)
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(ID=%d)�������ļ�(%s)ʧ��",
              m_nSwitchIndex,
              m_szCfgPath);
        return ERR_FAIL;
    }

    m_MutexForSchl.acquire();

    m_ColListTime.clear();

    char szColTime[1024] = "";
    LIST<int>::iterator i;
    for(i = TimeList.begin(); i != TimeList.end(); i++)
    {
        SNPRINTF(szColTime,
                 sizeof(szColTime),
                 "%s%02d:%02d,",
                 szColTime,
                 (*i)/3600,
                 (*i)/60%60);
        szColTime[sizeof(szColTime) - 1] = '\0';
        m_ColListTime.push_back(*i);
    }
    if(strlen(szColTime) > 0)
    {
        szColTime[strlen(szColTime) - 1] = '\0';
    }

    m_ColListTime.sort();

    m_MutexForSchl.release();


	char szSwitch[40];
	SNPRINTF(szSwitch, sizeof(szSwitch),"%s%d",
		     CFG_CO_SEC_SWITCH, m_nSwitchIndex);
	szSwitch[sizeof(szSwitch) - 1] = '\0';
	INIFile.WriteString(szSwitch,
							CFG_CO_COL_LIST_TIME,
							szColTime);

    return 0;
}
