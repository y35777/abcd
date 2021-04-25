/*************************************************************************************
 *产品名        ：iGateway Bill V200
 *模块名        ：双机接口(ClusterInterface)
 *文件名        ：cluster_hdr.h
 *描述信息      ：本文件包含双机接口模块需要使用的头文件和数据结构。
 *版本说明      ：V200R001i00M00B00D1130A
 *版权信息      ：版权所有（C）2001-2002 华为技术有限公司
 *作者          ：邱意平
 *创建时间      ：2001-10-30
 *修改记录      ：
**************************************************************************************/
#ifndef _CLUSTER_HEADER_H_
#define _CLUSTER_HEADER_H_

#include "../include/resource.h"
#include "../include/frame.h"
#include "../include/precom.h"
#include "../include/toolbox.h"

//双机接口模块初始化状态枚举值
enum ECLSTRINTSTATUS
{
    CLSTRINT_INIT,              //初始化状态
    CLSTRINT_ACTIVING,	        //正在执行激活的状态。
    CLSTRINT_ACTIVE,	        //激活状态。
    CLSTRINT_DEACTIVING,        //正在执行去激活的状态。
    CLSTRINT_DEACTIVE,	        //去激活状态。
    CLSTRINT_FAILURE,	        //故障状态。
    CLSTRINT_FAILOVER           //正在执行故障倒换的状态。
};

#endif  //_CLUSTER_HEADER_H_
