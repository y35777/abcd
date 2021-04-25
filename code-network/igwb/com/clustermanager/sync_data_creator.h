#ifndef __SYNC_DATA_CREATOR_H__
#define __SYNC_DATA_CREATOR_H__

#include "sync_data.h"

class CSyncDataCreator
{
public:
    CSyncDataCreator();
    ~CSyncDataCreator();

    //创建接入点号到同步数据对象的MAP
    static MAP<BYTE, CSyncData*>* CreateSyncDataList();
};

#endif //__SYNC_DATA_CREATOR_H__
