/***************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：agent
 *文件名    ：Cluster.h
 *描述信息  ：类CCluster的定义文件，该类封装了获得SUN SC集群系统的配置
                  以及运行时信息的功能。 
 *版本说明  ：V200R002i00M00B00D1130A   
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：邱意平
 *创建时间  ：2002-11-01
 *修改记录  ：
****************************************************************************/
#ifndef _CLUSTER_2_H_
#define _CLUSTER_2_H_

#include "agent_hdr.h"
#include "cluster.h"

class CCluster2 : public CCluster
{
public:
    CCluster2(const char* pszConfigFile);    //构造函数
    virtual ~CCluster2() {};                            //析构函数

    BOOL          IsClusterRunning(void);    //检查集群是否在运行
    int           GetCurrMemNum(void);       //获得当前逻辑主机中活动成员数量    
    BOOL          GetPrvNetStatus(int PrvNetID);  //获得指定ID的私网的当前状态
    BOOL          GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets); //获得SC当前活动的私网列表
    int           GetSCCurrMemNum(const char* szFilePath);//获得SC当前活动成员数
    BOOL          GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes); //获得SC中所有节点

private:
    const STRING  GetSCDir(void);            //获得集群软件的安装路径    
    const STRING  GetLogicalHost(void);      //获得逻辑主机名
    const STRING  GetDataService(void);      //获得高可用数据服务名称

    BOOL  ReadLogIP(const char* pszFilePath, LIST<STRING>& LogIP);
    int   GetPrvNetNum(const char* szFilePath);
    BOOL  ReadLOGHOST(const char* pszFilePath, STRING& sLogHost,
                   LIST<STRING>& NodeName, LIST<STRING>& DiskGroup);
    BOOL  GetMaster(const char* szLogHostName, char* szMaster);

private:
    STRING         m_sSCDir;           //SC软件安装路径

};

#endif   //_CLUSTER2_H_
