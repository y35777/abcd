#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "collect_task_obj.h"
#include "collect_task_obj_factory.h"
#include "include/adapter_include.h"
#include "collect_task/collect_define.h"
#include "collect_task/collect_struct.h"
#include "collect_task/collect_task.h"
#include "include/res_config.h"
#include "resource.h"

VECTOR<CCollectTaskObj*>* CCollectTaskObjFactory::CreateCollectTaskObj(const char* const szCfgPath)
{
    CINIFile INIFile(szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("打开配置文件(%s)失败", szCfgPath);
        return NULL;
    }

    VECTOR<CCollectTaskObj*>* pColTaskObjList = new VECTOR<CCollectTaskObj*>;
    int nSwitchCount = INIFile.GetInt(CFG_CO_SEC_SYSTEM, 
                                      CFG_CO_SWITCH_COUNT,
                                      CFG_CO_SWITCH_COUNT_DEFAULT);

    int i;
    for(i = 1; i <= nSwitchCount; i++)
    {
        char szBuf[40] = "";
        char szSwitchName[40];

        SNPRINTF(szBuf,sizeof(szBuf),"%s%d", CFG_CO_SEC_SWITCH,i);
        //获取采集任务名
        INIFile.GetString(szBuf,
                          CFG_CO_NAME,
                          CFG_CO_NAME_DEFAULT,
                          szSwitchName,
                          sizeof(szSwitchName));
        if(szSwitchName[0] == '\0')
        {
            COTRACE(S_COL_TASK_NAME_MUST_BE_CFG, szBuf);
            continue;
        }

        CCollectTaskObj* pTask = new CCollectTaskObj(i, szSwitchName, szCfgPath);

        if(pTask != NULL)
        {
            int nRet = pTask->Init();
            if(nRet != ERR_SUCCESS)
            {
                COTRACE(S_FAILED_TO_INIT_CO_TASK, pTask->GetTaskID());
                continue;
            }
        }
        pColTaskObjList->push_back(pTask);
    }

    if(pColTaskObjList->size() == 0)
    {
        delete pColTaskObjList;
        pColTaskObjList = NULL;
    }

    return pColTaskObjList;
}
