
#ifndef _FRAME_RESOURCE_H_
#define _FRAME_RESOURCE_H_


    /* 进程名称常量字符串 */

    #define PTS_UNKNOWN                     "PT_UNKNOWN"    //未知  
    #define PTS_OUTSIDE                     "PT_OUTSIDE"    //计费网关之外的进程
    #define PTS_SELF                        "PT_SELF"       //本进程
    #define PTS_KERNEL                      "PT_KERNEL"     //KERNEL
    #define PTS_CLSTR                       "PT_CLSTR"      //双机进程
    #define PTS_OM                          "PT_OM"         //操作维护进程
    #define PTS_AP_BASE                     "PT_AP_BASE"    //接入点进程
    #define PTS_CO_BASE                     "PT_CO_BASE"    //接入点进程
    #define PTS_BROADCAST                   "PT_BROADCAST"  //广播消息的接收进程
    #define PTS_CFGSVR                      "PT_CFGSVR"     //远程参数配置服务端进程

    //GTP'计费接口增加 Z30971 2004/06  
    #define PTS_BS                          "PT_BS"         //计费接口进程

    /* 模块名称常量字符串 */

    #define MTS_UNKNOWN                     "MT_UNKNOWN"        //未知模块
    #define MTS_MANAGER                     "MT_MANAGER"        //管理模块
    #define MTS_COMM                        "MT_COMM"           //网络通信层
    #define MTS_NETWORK                     "MT_NETWORK"        //网络模块
    #define MTS_FRONTSAVE                   "MT_FRONTSAVE"      //前存盘模块
    #define MTS_BILLPROCESSOR               "MT_BILLPROCESSOR"  //话单处理模块
    #define MTS_BACKSAVE                    "MT_BACKSAVE"       //后存盘模块
    #define MTS_MONITOR                     "MT_MONITOR"        //监控模块
    #define MTS_CLSTRINTERFACE              "MT_CLSTRINTERFACE" //双机接口模块
    #define MTS_ALARM                       "MT_ALARM"          //告警模块
    #define MTS_BACKUP                      "MT_BACKUP"         //备份模块
    #define MTS_MMLSERVER                   "MT_MMLSERVER"      //MML SERVER模块
    #define MTS_MMLDEBUG                    "MT_MMLDEBUG"       //MML运行调试台
    #define MTS_BILLBROWSER                 "MT_BILLBROWSER"    //话单浏览查询模块
    #define MTS_LOG                         "MT_LOG"            //日志管理模块
    #define MTS_MSGDIST                     "MT_MSGDIST"        //消息中转模块
    #define MTS_CLSTR                       "MT_CLSTR"          //双机模块
    #define MTS_PERF                        "MT_PERF"           //性能模块
    #define MTS_MMLDEBUG                    "MT_MMLDEBUG"       //MML运行调试台
    #define MTS_BILLTASK                    "MT_BILLTASK"       //SCP话单恢复模块
	#define MTS_CDRSTAT						"MT_CDRSTAT"		//话单统计模块
    #define MTS_CFGSVR                      "MT_CFGSVR"         //远程参数配置模块
	#define MTS_LICENSE_MANAGER				"MT_LICENSEMANGER"	//License管理模块
    #define MTS_COLLECTOR                   "MT_COLLECTOR"      //采集模块
    #define MTS_COLAGENT                    "MT_COLAGENT"       //采集MML代理模块
    #define MTS_COLCONVERTOR                "MT_COLCONVERTOR"   //采集话单转换模块
    #define MTS_FORMAT                      "MT_FORMAT"         //格式处理模块
    #define MTS_METERACC                    "MT_METERACC"

    //GTP'计费接口增加 Z30971 2004/06    
    #define MTS_CDRSENDER                   "MT_CDRSENDER"      //话单传送模块

    /* OMCSERVER 的接口模块 */

    #define MTS_OMCSERVERINTERFACE          "MT_OMCSERVERINTERFACE"

#endif

