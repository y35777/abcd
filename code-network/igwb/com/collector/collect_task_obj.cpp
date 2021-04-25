#include "../include/base_type.h"
#include "collect_task_obj.h"
#include "include/collect_task_creator.h"

CCollectTaskObj::CCollectTaskObj(int nTaskID, 
                                 const char* const szColLabel,
                                 const char* const szCfgPath)
{
    m_bActive               = FALSE;
    m_pCollectTask          = NULL;
    m_szCfgPath[0]          = '\0';
    m_nTaskID               = nTaskID;
    m_tLastActiveTime       = 0;
    m_nRestartTimes         = -1;
    m_nAcceptedActiveInv    = 30 * 60;  //30分钟
    m_nAcceptedRestartTimes = 3;
    m_bManualStop           = false;
    m_bLicensed             = FALSE;

    if(szCfgPath != NULL)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }

    assert(0 != szColLabel);
    m_strColLabel = szColLabel;
}

CCollectTaskObj::~CCollectTaskObj()
{
    if(NULL != m_pCollectTask)
    {
        delete m_pCollectTask;
        m_pCollectTask = NULL;
    }
}

//初始化
int CCollectTaskObj::Init()
{
    if((NULL == m_pCollectTask) && (m_szCfgPath[0] != '\0'))
    {
        m_pCollectTask = CCollectTaskFactory::CreateCollectTask(m_nTaskID, m_szCfgPath);
        if(NULL != m_pCollectTask)
        {
            return m_pCollectTask->Init();
        }
    }

    return ERR_FAIL;
}

//等待退出的操作
void CCollectTaskObj::WaitForExit()
{
    if(NULL != m_pCollectTask)
    {
        m_pCollectTask->WaitForExit();
    }

    wait();
}

//设置等待退出的标志
void CCollectTaskObj::SetFlagForExit()
{
    if(NULL != m_pCollectTask)
    {
        m_pCollectTask->WaitForExit();
    }
}

void CCollectTaskObj::DeActive()
{
    if(NULL != m_pCollectTask)
    {
        m_pCollectTask->DeActive();
    }
    m_bActive = FALSE;
}

void CCollectTaskObj::Active()
{
    if(NULL != m_pCollectTask)
    {
        m_pCollectTask->Active();
    }
    m_bActive = TRUE;
}

//ActiveObject对象的线程函数，通过run函数来启动
int CCollectTaskObj::svc()
{
    if((m_nRestartTimes > 0)
       && (time(NULL) - m_tLastActiveTime >= m_nAcceptedActiveInv))
    {
        m_nRestartTimes = 0;
    }

    if(m_nRestartTimes >= m_nAcceptedRestartTimes)
    {
        m_bFailure = TRUE;
        return -1;
    }

    m_bActive         = TRUE;
    m_tLastActiveTime = time(NULL);
    m_nRestartTimes++;

    m_pCollectTask->Collect();

    m_bActive         = FALSE;

    return 0;
}
//采集指定的文件
int CCollectTaskObj::GetSpecFile(const char* const szModule,
                                 const char* const szSrcFile, 
                                 const char* const szDstFile)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->GetSpecFile(szModule,
                                           szSrcFile,
                                           szDstFile);
    }

    return ERR_FILE_CANNOT_BE_FOUND;
}

//获取交换机信息
int CCollectTaskObj::GetSwitchInfo(LIST<STRING>& label, LIST<STRING>& content)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->GetSwitchInfo(label, content);
    }

    return ERR_FAIL;
}

//获取模块信息
int CCollectTaskObj::GetModuleInfo(const char* const szModule,
                                   LIST<STRING>& label, 
                                   LIST<STRING>& content)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->GetModuleInfo(szModule,
                                             label,
                                             content);
    }

    return ERR_FAIL;
}

int CCollectTaskObj::GetLinkStatus(LIST<SLinkStat>& LinkStat)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->GetLinkStatus(LinkStat);
    }

    return ERR_FAIL;
}

//增加一个单次采集任务
int CCollectTaskObj::SetOnceColSchl(LIST<time_t>& OnceSchlList)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->SetOnceColSchl(OnceSchlList);
    }

    return ERR_FAIL;
}

//获取单次采集任务列表
int CCollectTaskObj::GetOnceColSchl(LIST<time_t>& OnceSchlList)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->GetOnceColSchl(OnceSchlList);
    }

    return ERR_FAIL;
}

//获取采集重试参数
int CCollectTaskObj::GetColRetryPara(int& nCntFailCycle,
                                     int& nColFailCycle,
                                     int& nFileRetryTimes)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->GetColRetryPara(nCntFailCycle,
                                               nColFailCycle,
                                               nFileRetryTimes);
    }

    return ERR_FAIL;
}


//设置采集重试参数
int CCollectTaskObj::SetColRetryPara(int nCntFailCycle,
                                     int nColFailCycle,
                                     int nFileRetryTimes)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->SetColRetryPara(nCntFailCycle,
                                               nColFailCycle,
                                               nFileRetryTimes);
    }

    return ERR_FAIL;
}

//获取周期性采集参数
int CCollectTaskObj::GetColCyclePara(int& nBegin,
                                     int& nEnd, 
                                     int& nCycle)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->GetColCyclePara(nBegin,
                                               nEnd,
                                               nCycle);
    }

    return ERR_FAIL;
}

//设置周期性采集参数
int CCollectTaskObj::SetColCyclePara(int nBegin,
                                     int nEnd,
                                     int nCycle)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->SetColCyclePara(nBegin,
                                               nEnd,
                                               nCycle);
    }

    return ERR_FAIL;
}

//获取序列方式采集的参数
int CCollectTaskObj::GetColTimeList(LIST<int>& TimeList)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->GetColTimeList(TimeList);
    }

    return ERR_FAIL;
}

//设置序列方式采集的参数
int CCollectTaskObj::SetColTimeList(LIST<int>& TimeList)
{
    if(NULL != m_pCollectTask)
    {
        return m_pCollectTask->SetColTimeList(TimeList);
    }

    return ERR_FAIL;
}

