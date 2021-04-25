/***************************************************************************
 *产品名        ：iGateway Bill V200        
 *模块名    ：agent
 *文件名    ：Cluster.h
 *描述信息      ：类CCluster的定义文件，该类封装了获得SUN SC集群系统的配置
                  以及运行时信息的功能。 
 *版本说明  ：V200R002i00M00B00D1130A   
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：邱意平
 *创建时间  ：2001-10-30
 *修改记录  ：
****************************************************************************/
#ifndef _CLUSTER3_H_
#define _CLUSTER3_H_

#include "agent_hdr.h"
#include "cluster.h"

class CCluster3 : public CCluster
{
public:
    CCluster3(const char* pszConfigFile);    //构造函数
    virtual ~CCluster3(){};                            //析构函数

    BOOL          IsClusterRunning(void);    //检查集群是否在运行
    int           GetSCCurrMemNum(const char* szFilePath);//获得SC当前活动成员数
    BOOL          GetPrvNetStatus(int PrvNetID);  //获得指定ID的私网的当前状态
    BOOL          GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets); //获得SC当前活动的私网列表
    int           GetCurrMemNum(void);       //获得当前逻辑主机中活动成员数量    
    BOOL          GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes);

private:
    BOOL          GetHeartBeats(const char* szMatch_heartbeats, LIST <STRING>& Cluster_Heartbeats);//获得SC中所有心跳
    BOOL          GetDiskGroups(const char* szMatch_diskgroup, LIST <STRING>& Cluster_Diskgroups);
    BOOL          GetNafos(LIST <STRING>& Cluster_NAFO);  

};

#endif   //_CLUSTER3_H_
