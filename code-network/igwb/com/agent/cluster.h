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
#ifndef _CLUSTER_H_
#define _CLUSTER_H_

#include "agent_hdr.h"

//类CCluster
class CCluster
{
public:
    CCluster(const char* pszConfigFile);            //构造函数
    virtual ~CCluster();                            //析构函数

public:
    const STRING   GetCluster(void);                //获得集群名称
    const STRING   GetLocalNodename(void);          //获得本主机节点名    
    LIST<STRING>&  GetDiskGroup(void);              //获得磁盘组
    LIST<STRING>&  GetNAFO(void);                   //获得NAFO组名
    BOOL           GetHostname(const char* szFilePath,STRING& sHostname);  //获得本主机名字
    int            GetNumOfPrvNet(void);            //获得私网数量
    VM_TYPE        GetVMType(void);                 //获得卷管理软件类型
    BOOL           GetSCConfig(FILE* fp, const char* szMatch, char* szOutput);
    int            GetSCNodeNum(void);              //获得SC中节点个数

public:
    virtual  BOOL  IsClusterRunning(void);          //检查集群是否在运行
    virtual  int   GetCurrMemNum(void);             //获得当前逻辑主机中活动成员数量    
    virtual  BOOL  GetPrvNetStatus(int PrvNetID);   //获得指定ID的私网的当前状态
    virtual  BOOL  GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets); //获得SC当前活动的私网列表
    virtual  int   GetSCCurrMemNum(const char* szFilePath);//获得SC当前活动成员数
    virtual  BOOL  GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes); //获得SC中所有节点

    virtual  const STRING  GetLogicalHost(void);    //获得逻辑主机名
    virtual  const STRING  GetDataService(void);    //获得高可用数据服务名称


public:
    STRING         m_sCluster;                      //集群名字，安装SC时所指定的名字
    SLogicalHost   m_LogicalHost;                   //逻辑主机信息
    int            m_nrOfPrvNet;                    //私网数量
    STRING         m_hostname;                      //当前主机名
    VM_TYPE        m_VMType;                        //卷管理软件类型
};

#endif   //_CLUSTER_H_
