#ifndef _COLLECTOR_FILE_POOL_H__
#define _COLLECTOR_FILE_POOL_H__

#include "../include/base_type.h"
#include "../include/collect_msg_def.h"


class  CCollectorFilePool
{

public:

    CCollectorFilePool(const char* szStatusFile);
    ~CCollectorFilePool();

    int Init();

    SSwitchFileInfo& Front();

    int PushBack(SSwitchFileInfo& stFileInfo);

    int PopFront();

    int GetSize() {return m_nFileCount;}

protected:
    int LoadStatus();

    int SaveStatus();

    int m_nFileCount;

    LIST<SSwitchFileInfo>  m_FileList;

    char  m_szStatusFile[MAX_PATH];

    ACE_Recursive_Thread_Mutex m_FilePoolMutex;

};

#endif //_COLLECTOR_FILE_POOL_H__

