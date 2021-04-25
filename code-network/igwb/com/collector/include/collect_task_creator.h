#ifndef __COLLECT_TASK_CREATOR_H__
#define __COLLECT_TASK_CREATOR_H__

class ICollectTask;

class CCollectTaskFactory
{
public:
    static ICollectTask* CreateCollectTask(int nSwitchIndex, const char* const szCfgPath);
};

#endif //__COLLECT_TASK_CREATOR_H__
