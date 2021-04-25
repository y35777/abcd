/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:agent_define.h
    描述信息:
    
    定义了Snmp告警代理中使用全局数据类型.

    修改记录:
    陈凉威,  2003-11 ,   创建.   
*/

#ifndef _ALARM_DEFINE_H_
#define _ALARM_DEFINE_H_

#include "./resource.h"
#include "../include/base_type.h"
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"



#define     VALLEN                64

#define QUERY_STATUS_FILE         "/query_status.ini"
#define NMS_STATUS_FILE           "/nms_status.ini"

#define HEAD               "Head"
#define NUMBER             "number"
#define QUERY_TYPE         "QueryType"
#define QUERY_RES_FLAG     "QueryRestoreFlag"
#define QUERY_ACK_FLAG     "QueryAckFlag"
#define QUERY_MODULE_NUM   "QueryModuleNumber" 
#define QUERY_CSN_SCOPE    "QueryCsnScope"      //100-10000
#define QUERY_ID_SCOPE     "QueryIDScope"       //1-21
#define QUERY_LEVEL        "QueryLevel"         //important
#define QUERY_SUBSYS_ID    "QuerySubsystemID"   //6
#define QUERY_DATE_SCOPE   "QueryDateScope"     //2003/06/26-2003/08/12
#define QUERY_TIME_SCOPE   "QueryTimeScope"     //18:00:00-23:00:00
#define QUERY_RETURN_CNT   "ReturnCsn"

struct SQueryStatus
{
    STRING strQueryType;
    STRING strQueryRestoreFlag;
    STRING strQueryAckFlag;
    int  nQueryModuleNumber; 
    STRING strQueryCsnScope;      //100-10000
    STRING strQueryIDScope;       //1-21
    STRING strQueryLevel;         //important
    STRING strQuerySubsystemID;   //6
    STRING strQueryDateScope;     //2003/06/26-2003/08/12
    STRING strQueryTimeScope;     //18:00:00-23:00:00
    int nReturnCsn;
};

#define NMS_IP             "NmsIP"    //10.11.243.197
#define NMS_PORT           "NmsPort"  //161

struct SNMSInfo
{
    STRING strName;
    STRING strNmsIP;
    int    nNmsPort;
};


#endif //_ALARM_DEFINE_H_