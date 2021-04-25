#include "../../include/base_type.h"
#include "../../include/toolbox.h"
#include "../include/i_collect_task.h"
#include "../include/collect_task_creator.h"
#include "../include/adapter_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "collect_task.h"

ICollectTask* CCollectTaskFactory::CreateCollectTask(int nSwitchIndex, const char* const szCfgPath)
{
    ICollectTask* pCollectTask = new CCollectTask(nSwitchIndex);

    if(NULL != pCollectTask)
    {
        pCollectTask->SetCfgPath(szCfgPath);
    }

    return pCollectTask;
}
