/***************************************************************************
 *产品名        ：iGateway Bill V200
 *模块名        ：agent
 *文件名        ：agent_hdr.h
 *描述信息      ：本文件包含双机代理模块agent需要使用的头文件和数据结构。
 *版本说明      ：V200R002i00M00B00D1130A
 *版权信息      ：版权所有（C）2001-2002 华为技术有限公司
 *作者          ：邱意平
 *创建时间      ：2001-10-30
 *修改记录      ：
****************************************************************************/

#ifndef _AGENT_HEADER_H_
#define _AGENT_HEADER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include "../include/precom.h"
#include "../include/toolbox.h"

//描述组成集群、逻辑主机的一些部件的信息，例如物理主机、磁盘组、
//NAFO组等，通常这些部件有多个实例，每个实例使用一个名字来区分

#define  MAX_SC_NODES    4      //SUN SC支持的最大节点数（目前为4个）

//描述逻辑主机的信息，包括逻辑主机的名字、高可用数据服务的名字以及
//逻辑主机的组成信息
typedef struct _LogicalHost
{
    STRING   sLogHostName;      //逻辑主机名
    STRING   sDataServiceName;  //高可用数据服务名

    LIST <STRING> Cluster_Nodes;
    LIST <STRING> Cluster_Heartbeats; //just used in the SC3.0 now
    //逻辑主机管理的共享磁盘组，可能有多个共享磁盘组，每个磁盘组通过
    //名字来唯一确定
    LIST <STRING> Cluster_Diskgroups;  

    //SC通过NAFO(Network Adapter Failover)来管理虚拟IP地址，每个
    //NAFO组通过名字来唯一确定
    LIST <STRING> Cluster_NAFO;          //NAFO组

    //SResGroup  ResGroup;     //资源组
    //STRING       sResGroupName;    //资源组名
    //LIST<STRING> Resources;   //资源名

}SLogicalHost, *PLogicalHost;

//双机心跳链路类型
enum HBLINKTYPE
{
    HL_UDP = 0,      //"UDP"
    HL_COM = 1       //"COM"
};

//定义心跳状态
struct THBState
{
    HBLINKTYPE  LinkType;       //心跳链路类型
    BOOL        bIsAlive;       //心跳是否可用
};

//SC支持的卷管理软件(Volume Manager)类型
//目前SC2.2支持三种卷管理软件类型：
//1. Solstice DiskSuite           SDS
//2. Sun StorEdge Volume Manager  SSVM
//3. Cluster Volume Manager       CVM
enum VM_TYPE
{
    VT_VXVM,            //Veritas Volume Manager
    VT_SDS,             //Solstice DiskSuite
    VT_CVM              //Cluster Volume Manager(不会用到)
};

#define FRONT_VOLUME      0
#define BACK_VOLUME       1

#endif  //_AGENT_HEADER_H_

