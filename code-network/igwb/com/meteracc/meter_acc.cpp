#include "meter_acc.h"
#include "meteracc_task.h"
#include "resource.h"

CMeterAcc::CMeterAcc()
{
    m_pMeterAccTask = NULL;
}

CMeterAcc::~CMeterAcc()
{
    if (m_pMeterAccTask)
    {
        m_pMeterAccTask->StopTask();
        delete m_pMeterAccTask;
    }
}

MODULE_TYPE CMeterAcc::GetModuleType()
{
    return MT_METERACC;
}

/****************************************************************************
函数原型	protected:int CMeterAcc::InitInstance()
功能描述	启动计次表处理任务
输入参数	无
输出参数	无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
*****************************************************************************/
int CMeterAcc::InitInstance()
{
    if (0 != CWorkModule::InitInstance())
    {
        return -1;
    }

    CINIFile CfgFile(GetCfgFilePath());
    if (TRUE != CfgFile.Open())
    {
        TRACE(MTS_METERACC, S_BS_FAIL_OPEN_FILE, GetCfgFilePath);
        return -1;
    }
    
    m_nServerNo = CfgFile.GetInt(CFG_SEC_COMMON, CFG_SERVERNO, 0);

    //限定计次表累加功能只在主节点上运行
    if (0 == m_nServerNo)
    {
        m_pMeterAccTask = new CMeterAccTask();
        if (0 != m_pMeterAccTask->open((void *)GetAppPath()))
        {
            return -1;
        }
    }

    return 0;   //初始化成功
}

void CMeterAcc::ExitInstance()
{
    KillTimer(m_nTimerID);
    CWorkModule::ExitInstance();
}

void CMeterAcc::OnTimer(const long nTimerID)
{
}

void CMeterAcc::ParseMsg(MSG_INTRA* pMsg)
{
}
