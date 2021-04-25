#ifndef __COLLECT_TASK_OBJ_FACTORY_H__
#define __COLLECT_TASK_OBJ_FACTORY_H__

class CCollectTaskObj;

class CCollectTaskObjFactory
{
public:
    static VECTOR<CCollectTaskObj*>* CreateCollectTaskObj(const char* const szCfgPath);
};

#endif //__COLLECT_TASK_OBJ_FACTORY_H__
