#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "collect_convert_obj.h"
#include "collect_convert_obj_factory.h"
#include "include/collect_convert_include.h"

VECTOR<CCollectConvertObj*>* CCollectConvertObjFactory::CreateCollectConvertObj(const char* const szCfgPath)
{
    CINIFile INIFile(szCfgPath);
    if (!INIFile.Open())
    {
        return NULL;
    }

    VECTOR<CCollectConvertObj*>* pColConvertObjList = new VECTOR<CCollectConvertObj*>;
    int nSwitchCount = INIFile.GetInt(CFG_CO_SEC_SYSTEM, 
                                      CFG_CO_SWITCH_COUNT,
                                      CFG_CO_SWITCH_COUNT_DEFAULT);

    int i;
    for(i = 1; i <= nSwitchCount; i++)
    {
        CCollectConvertObj* pObj = new CCollectConvertObj(i, szCfgPath);

        if(pObj != NULL)
        {
            int nRet = pObj->Init();
            if(nRet != ERR_SUCCESS)
            {
//                COTRACE(S_FAILED_TO_INIT_CO_TASK, pObj->GetConvertID());
                continue;
            }
        }
        pColConvertObjList->push_back(pObj);
    }

    if(pColConvertObjList->size() == 0)
    {
        delete pColConvertObjList;
        pColConvertObjList = NULL;
    }

    return pColConvertObjList;
}
