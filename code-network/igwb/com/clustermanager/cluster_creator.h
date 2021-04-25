#ifndef __CLUSTER_CREATOR_H__
#define __CLUSTER_CREATOR_H__

#include "com_cluster.h"

class CClusterCreator
{
public:
    CClusterCreator();
    ~CClusterCreator();

    static CComCluster* CreateComCluster();
};

#endif //__CLUSTER_CREATOR_H__
