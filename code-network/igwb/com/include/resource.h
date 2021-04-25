/*
    文件名:resource.h
    本文件中定义了系统中使用的字符串常量.
*/
#ifndef _RESOURCE_DEFINE_H_
#define _RESOURCE_DEFINE_H_

#ifdef _PLATFORM_WIN32
#include "product.h"
#endif //_PLATFORM_WIN32
#include "../frame/resource.h"

#ifdef _PRODUCT_MOTO
#define S_IGATEWAY_BILL            "CGW"
#define MOTO_OFFSET                3000
#elif defined(_PRODUCT_CCS)
#define S_IGATEWAY_BILL            "iGateway CCS"
#else //_PRODUCT_MOTO
#define S_IGATEWAY_BILL            "iGateway bill"
#endif //_PRODUCT_MOTO

//版本号
//请注意在每次合版本时修改此版本号
#define S_IGATEWAY_BILL_VER        "iGWBV200R002B02D000"

//File Name for Config
#define S_CFG_FILE_NAME            "config/ini/igwb.ini"
#define S_RESET_INFO_FILE_NAME     "config/alarm/resetinfo.ini"

//by ldf 2002.6.7 对应问题单D14393
//File for ResourceString
#define S_RESSTR_FILE_NAME         "config/resource/resource.dat"
//修改完毕


/*定义KERNEL进程的可执行文件名*/
#define S_APPNAME_KERNEL           "knl_proc"

/*定义操作维护进程的可执行文件名*/
#define S_APPNAME_OM               "om_proc"

/*定义接入点进程的可执行文件名*/
#define S_APPNAME_AP               "ap_proc"

/*定义采集点进程的可执行文件名*/
#define S_APPNAME_CO               "co_proc"

/*定义双机代理进程的可执行文件名*/
#define S_APPNAME_AGENT            "cls_proc"

//缺省通道目录的名字
#define CHANNEL_0_NAME              "default"

//定义XML应用的动态库名
#ifdef _PLATFORM_WIN32
    #define XML_UTIL_DLL_NAME       "xml_util.dll"
#elif defined(_PLATFORM_SOLARIS) || defined (_PLATFORM_LINUX) 
    #define XML_UTIL_DLL_NAME       "libxml_util.so"
#elif defined(_PLATFORM_HPUX)
    #define XML_UTIL_DLL_NAME       "libxml_util.sl"
#else
    #error macro _PLATFORM_??? undefined.
#endif

#define S_AGENT_MML_CMD_SWP                     "SWP"
#define S_AGENT_MML_CMD_RESET                   "RESET"

#define S_CLUSTER_EXIT_MUTEX                    "iGateway bill V200 Cluster Exit Mutex"
#define S_CLUSTER_SHOWICON_MUTEX                "iGateway bill V200 Cluster ShowIcon Mutex"
#define S_KERNEL_MUTEX                          "iGateway bill V200 Kernel"
#define S_CLUSTER_MUTEX                         "iGateway bill V200 CLUSTER"
#define S_AP_MUTEX                              "iGateway bill V200 AP"
#define S_CFG_MUTEX                             "iGateway bill V200 CFG"
#define S_COLLECTOR_MUTEX                       "iGateway bill V200 Collector"
#define S_OM_MUTEX                              "iGateway bill V200 OM"
#define S_METERACC_MUTEX                        "iGateway Bill V200 METERACC" //计次累加表 2004.5.10

///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06
#define S_BS_MUTEX                              "iGateway Bill V200 BS"

#define S_AGENT_MML_CMDDESC_SWP                 GetStringFromID(1)
#define S_AGENT_HANDOVER_FAIL                   GetStringFromID(2)
#define S_AGENT_HANDOVER_SUCCESS                GetStringFromID(3)
#define S_AGENT_START_KNL_SUCCESS               GetStringFromID(4)
#define S_AGENT_START_KNL_FAILED                GetStringFromID(5)
#define S_AGENT_AGENT_INIT_SUCCESS              GetStringFromID(6)
#define S_AGENT_KNL_IS_RUNNING                  GetStringFromID(7)
#define S_AGENT_KNL_NOT_RUNNING                 GetStringFromID(8)
#define S_AGENT_SEND_MSG_TO_STOP_KNL            GetStringFromID(9)
#define S_AGENT_SEND_SIG_TO_KILL_KNL            GetStringFromID(10)
#define S_AGENT_KNL_ACTIVE_SUCCESS              GetStringFromID(11)
#define S_AGENT_KNL_ACTIVE_FAIL                 GetStringFromID(12)
#define S_AGENT_PEER_CANNOT_HANDOVER            GetStringFromID(13)
#define S_AGENT_ASK_FAILOVER                    GetStringFromID(14)
#define S_AGENT_RCV_SIGTERM                     GetStringFromID(15)
#define S_AGENT_RCV_KNL_ACTIVE                  GetStringFromID(16)
#define S_AGENT_SEND_QUIT_TO_MGR                GetStringFromID(17)
#define S_AGENT_RCV_MANUALOVER_MSG              GetStringFromID(18)
#define S_AGENT_RCV_SIGCLD                      GetStringFromID(19)
#define S_AGENT_CANNOT_FINDFILE                 GetStringFromID(20)
#define S_ERR_WRITEFILE                         GetStringFromID(21)
#define S_ERR_NOENOUGHDISK_PRIMARY              GetStringFromID(22)
#define S_ERR_NOENOUGHDISK_SECONDARY            GetStringFromID(23)
#define S_ERR_PROCESS_EXCEPTION                 GetStringFromID(24)
#define S_ERR_ACTIVE                            GetStringFromID(25)
#define S_ERR_RESOURCE                          GetStringFromID(26)
#define S_ERR_HARDDISK                          GetStringFromID(27)
#define S_ERR_UNRECOGNIZED                      GetStringFromID(28)
#define S_AGENT_CANNOT_HANDOVER                 GetStringFromID(29)
#define S_AGENT_EXECL_KNL_FAILED                GetStringFromID(30)
#define S_AGENT_MML_CMDDESC_RESET               GetStringFromID(31)
#define S_MANNUL_RESET_SUCCESS                  GetStringFromID(32)

//by ldf 2004-01-30 OM 整改
#define S_ALARM_SYN_NO                          GetStringFromID(34)
#define S_ALARM_FILE_ERITESYN_FAIL              GetStringFromID(35)
#define S_ALARM_FAIL_FLUSH_SYN                  GetStringFromID(36)
#define S_ALARM_FILE_ERITEMMLBASEID_FAIL        GetStringFromID(37)
#define S_ALARM_FAIL_FLUSH_MMLBASEID            GetStringFromID(38)
#define S_ALARM_NEFLAG                          GetStringFromID(39)
//end
#define S_ALARM_ALARM_RESPONSE_PACKAGE_ERROR    GetStringFromID(40)
#define S_ALARM_CORRECT_BIN_RESPONSE            GetStringFromID(41)
#define S_ALARM_RECEIVE_WRONG_RESPONSE          GetStringFromID(42)
#define S_ALARM_ALARMREPORT_TOTALNUM            GetStringFromID(43)
#define S_ALARM_ALARMLIST_TOTALNUM              GetStringFromID(44)
#define S_ALARM_SEND_BIN_ALARM                  GetStringFromID(45)
#define S_ALARM_RESEND_NEXT                     GetStringFromID(46)
#define S_ALARM_RECIEVE_RESPONSE                GetStringFromID(47)
#define S_ALARM_CHANGE_STATE_ERROR              GetStringFromID(48)
#define S_ALARM_CHECKSUM_ERR                    GetStringFromID(49)
#define S_ALARM_RESTOREERROR                    GetStringFromID(50)
#define S_ALARM_ALARMREVTIME                    GetStringFromID(51)
#define S_ALARM_ALARMTIME                       GetStringFromID(52)
#define S_ALARM_MODULENO                        GetStringFromID(53)
#define S_ALARM_ALARMNAME                       GetStringFromID(54)
#define S_ALARM_ALARMPARA                       GetStringFromID(55)
#ifdef _PRODUCT_MOTO
#define S_ALARM_ALARMACKHEAD                    GetStringFromID(56+MOTO_OFFSET)
#else //_PRODUCT_MOTO
#define S_ALARM_ALARMACKHEAD                    GetStringFromID(56)
#endif //_PRODUCT_MOTO
#define S_ALARM_CONFIRMOPR                      GetStringFromID(57)
#define S_ALARM_ALARMFLAG                       GetStringFromID(58)
#define S_ALARM_ALARMCSN                        GetStringFromID(59)
#define S_ALARM_ALARMTYPE                       GetStringFromID(60)
#define S_ALARM_ALARMLEVEL                      GetStringFromID(61)
#define S_ALARM_ALARMSOURCE                     GetStringFromID(62)
#define S_ALARM_ALARMID                         GetStringFromID(63)
#define S_ALARM_SUBSYSID                        GetStringFromID(64)
#define S_ALARM_ALARMPARAMETER                  GetStringFromID(65)
#define S_ALARM_ALARMEXPLAIN                    GetStringFromID(66)
#define S_ALARM_ALARMPARAEXPLAIN                GetStringFromID(67)
#define S_ALARM_BEGINCSN                        GetStringFromID(68)
#define S_ALARM_ENDCSN                          GetStringFromID(69)
#define S_ALARM_RECOVERSTATUS                   GetStringFromID(70)
#define S_ALARM_ACKSTATUS                       GetStringFromID(71)
#define S_ALARM_ALARMACKOPR                     GetStringFromID(72)
#define S_ALARM_ALARMACKDATE                    GetStringFromID(73)
#define S_ALARM_ALARMRECOVERACKOPR              GetStringFromID(74)
#define S_ALARM_ALARMRECOVERACKDATE             GetStringFromID(75)
#define S_ALARM_ALARMFIX                        GetStringFromID(76)
#define S_ALARM_BEGINDATE                       GetStringFromID(77)
#define S_ALARM_ENDDATE                         GetStringFromID(78)
#define S_ALARM_OPRSUCCESS                      GetStringFromID(79)
#define S_ALARM_OPRFAIL                         GetStringFromID(80)
#define S_ALARM_NOUSE                           GetStringFromID(81)
#define S_ALARM_ACKVALUE                        GetStringFromID(82)
#define S_ALARM_NOACKVALUE                      GetStringFromID(83)
#define S_ALARM_AT_EVENT                        GetStringFromID(84)
#define S_ALARM_AT_FAULT                        GetStringFromID(85)
#define S_ALARM_AT_RESTORE                      GetStringFromID(86)
#define S_ALARM_AT_HISTORY                      GetStringFromID(87)
#define S_ALARM_AT_NOT_DEFINE                   GetStringFromID(88)
#define S_ALARM_AL_EMERGENCY                    GetStringFromID(89)
#define S_ALARM_AL_IMPORTANT                    GetStringFromID(90)
#define S_ALARM_AL_MINOR                        GetStringFromID(91)
#ifdef _OM_MODIFY
    #define  S_ALARM_AL_WARNING                     GetStringFromID(1903)
#else
    #define S_ALARM_AL_WARNING                      GetStringFromID(92)
#endif
#define S_ALARM_AS_POWER_SYSTEM                 GetStringFromID(93)
#define S_ALARM_AS_ENVIROMENT_SYSTEM            GetStringFromID(94)
#define S_ALARM_AS_SIGNALING_SYSTEM             GetStringFromID(95)
#define S_ALARM_AS_TRUNK_SYSTEM                 GetStringFromID(96)
#define S_ALARM_AS_HARDWARE_SYSTEM              GetStringFromID(97)
#define S_ALARM_AS_SOFTWARE_SYSTEM              GetStringFromID(98)
#define S_ALARM_AS_RUNNING_SYSTEM               GetStringFromID(99)
#define S_ALARM_AR_NOT_RECEIVED                 GetStringFromID(100)
#define S_ALARM_AR_RECEIVED                     GetStringFromID(101)
#define S_ALARM_AI_COMM_IN_MODULE               GetStringFromID(102)
#define S_ALARM_AI_CONTROL_SYSTEM               GetStringFromID(103)
#define S_ALARM_AI_NETWORK_SYSTEM               GetStringFromID(104)
#define S_ALARM_AI_COMM_IN_MAIN_NODE            GetStringFromID(105)
#define S_ALARM_AI_SIGNALING_SYSTEM             GetStringFromID(106)
#define S_ALARM_AI_TIME_SYSTEM                  GetStringFromID(107)
#define S_ALARM_AI_USER_SYSTEM                  GetStringFromID(108)
#define S_ALARM_AI_TRUNK_SYSTEM                 GetStringFromID(109)
#define S_ALARM_AI_TEST_SYSTEM                  GetStringFromID(110)
#define S_ALARM_AI_POWER_SYSTEM                 GetStringFromID(111)
#define S_ALARM_AI_ENVIROMENT_SYSTEM            GetStringFromID(112)
#define S_ALARM_AI_TRANSFER_SYSTEM              GetStringFromID(113)
#define S_ALARM_AI_DATABASE_SYSTEM              GetStringFromID(114)
#define S_ALARM_AI_BILLING_SYSTEM               GetStringFromID(115)
#define S_ALARM_AI_CALL_IDENTITY                GetStringFromID(116)
#define S_ALARM_AI_SOFTWARE_RUNNING             GetStringFromID(117)
#define S_ALARM_AI_COMM_FROM_TO                 GetStringFromID(118)
#define S_ALARM_NOT_RECOVER                     GetStringFromID(119)
#define S_ALARM_AUTO_RECOVER                    GetStringFromID(120)
#define S_ALARM_RESET_RECOVER                   GetStringFromID(121)
#define S_ALARM_MANNUAL_RECOVER                 GetStringFromID(122)
#define S_ALARM_NO_RESULT                       GetStringFromID(123)
#define S_ALARM_CFG_NULL                        GetStringFromID(124)
#define S_ALARM_LOAD_CFG_FAIL                   GetStringFromID(125)
#define S_ALARM_BASE_INIT_FAIL                  GetStringFromID(126)
#define S_ALARM_INI_OPEN_FAIL                   GetStringFromID(127)
#define S_ALARM_BIN_INIT_FAIL                   GetStringFromID(128)
#define S_ALARM_MML_INIT_FAIL                   GetStringFromID(129)
#define S_ALARM_OPEN_HISTORY_FAIL               GetStringFromID(130)
#define S_ALARM_FILE_ERITECSN_FAIL              GetStringFromID(131)
#define S_ALARM_WRITE_CSN_FAIL                  GetStringFromID(132)
#define S_ALARM_FAIL_FLUSH_CSN                  GetStringFromID(133)
#define S_ALARM_ADD_HIS_FAIL                    GetStringFromID(134)
#define S_ALARM_FLUSH_HIS_FAIL                  GetStringFromID(135)
#define S_ALARM_WRITE_HIS_ERR                   GetStringFromID(136)
#define S_ALARM_FILE_NULL                       GetStringFromID(137)
#define S_MEM_ALLOC_FAIL                        GetStringFromID(138)
#define S_ALARM_SEEK_HIS_FAIL                   GetStringFromID(139)
#define S_ALARM_DEL_HIS_ERR                     GetStringFromID(140)
#define S_ALARM_RECEIVE                         GetStringFromID(141)
#define S_ALARM_SEND_ALARM                      GetStringFromID(142)
#define S_ALARM_CLEAN_HIS                       GetStringFromID(143)
#define S_ALARM_TOBECONITUE                     GetStringFromID(144)
#define S_ALARM_IDNOTDEFINE                     GetStringFromID(145)
#define S_ALARM_TYPEERROR                       GetStringFromID(146)
#define S_ALARM_ACK_WRITE_ERR                   GetStringFromID(147)
#define S_ALARM_ACK_REACH_LIMIT                 GetStringFromID(148)
#define S_ALARM_ACK_REPEAT                      GetStringFromID(149)
#define S_ALARM_DEL_ALM_LIMIT                   GetStringFromID(150)
#define S_ALARM_RECV_INVALIDALARM               GetStringFromID(151)
#define S_ALARM_INITADD_FAIL                    GetStringFromID(152)
#define S_ALARM_ALARM_RESPONSE_ERROR            GetStringFromID(153) //zhangyuxin modify
#define S_ALARM_BOX_INIT_NOT_QUERY_LGT          GetStringFromID(154)
#define S_ALARM_BOX_INIT_NOT_QUERY_VER          GetStringFromID(155)
#define S_ALARM_BOX_INIT_NOT_OFF_LGT            GetStringFromID(156)
#define S_ALARM_BOX_INIT_NOT_STOP_SND           GetStringFromID(157)
#define S_ALARM_BOX_INIT_NOT_HANDLE             GetStringFromID(158)
#define S_ALARM_BOX_DISCONNECT_NOT_QUERY_LGT    GetStringFromID(159)
#define S_ALARM_BOX_DISCONNECT_NOT_QUERY_VER    GetStringFromID(160)
#define S_ALARM_BOX_DISCONNECT_NOT_OFF_LGT      GetStringFromID(161)
#define S_ALARM_BOX_DISCONNECT_NOT_STOP_SND     GetStringFromID(162)
#define S_ALARM_BOX_DISCONNECT_NOT_HANDLE       GetStringFromID(163)
#define S_ALARM_EMERGENCY_CNT                   GetStringFromID(164)
#define S_ALARM_IMPORTANT_CNT                   GetStringFromID(165)
#define S_ALARM_MINOR_CNT                       GetStringFromID(166)
#define S_ALARM_NORMAL_CNT                      GetStringFromID(167)
#define S_ALARM_BOX_VERSION                     GetStringFromID(168)
#define S_ALARM_QUERY_NO_RESULT                 GetStringFromID(169)
#define S_ALARM_LOCATION_ERROR                  GetStringFromID(170)
#define S_ALARM_RECEIVE_MMLCMD                  GetStringFromID(171)
#define S_ALARM_UPDATE_BOX_ALARM                GetStringFromID(172)
#define S_ALARM_BOX_INIT_FAIL                   GetStringFromID(173)
#define S_ALARM_ANALYSIZE_BOX_RESPONSE          GetStringFromID(174)
#define S_ALARM_DESTROY_STATE                   GetStringFromID(175)
#define S_ALARM_MML_REPORT                      GetStringFromID(176)
#define S_ALARM_INIT                            GetStringFromID(177)
#define S_ALARM_DISCONNECT                      GetStringFromID(178)
#define S_ALARM_NORMAL                          GetStringFromID(179)

#define S_BS_LOG_CREATE_BS_DATE                 GetStringFromID(180)
#define S_BS_LOG_CREATE_NEW_FILE                GetStringFromID(181)
#define S_BS_MSG_REC_JUNP_PID                   GetStringFromID(182)
#define S_BS_MSG_CREATE_FORMAL                  GetStringFromID(183)
#define S_BS_MSG_ERR_STATUS                     GetStringFromID(184)
#define S_BS_MSG_NORMAL_STATUS                  GetStringFromID(185)
#define S_BS_MSG_FAIL_SAVE                      GetStringFromID(186)
#define S_BS_MSG_FAIL_TIMER                     GetStringFromID(187)

#define S_BS_TRC_REC_JUNP_PID                   GetStringFromID(188)
#define S_BS_TRC_ERR_CFG_FILE_SIZE              GetStringFromID(189)
#define S_BS_TRC_SUCC_DEL_FILE                  GetStringFromID(190)
#define S_BS_TRC_FAIL_DEL_FILE                  GetStringFromID(191)
#define S_BS_TRC_DEL_LAST_FILE                  GetStringFromID(192)
//#define S_BS_TRC_CREATE_NEW_FILE              GetStringFromID(193)
#define S_BS_TRC_FAIL_INIT_BASE                 GetStringFromID(194)
#define S_BS_TRC_FAIL_INIT_FMT                  GetStringFromID(195)
#define S_BS_TRC_FAIL_OPEN_INI                  GetStringFromID(196)
#define S_BS_TRC_FAIL_SAVE                      GetStringFromID(197)
#define S_BS_TRC_FAIL_TIMER                     GetStringFromID(198)
//#define s_LOAD_PATCH_FAILED                       GetStringFromID(199)
//#define s_PATCH_INIT_FAILED                       GetStringFromID(200)
#define s_PATCH_PROCESS_FAILED                  GetStringFromID(201)
//#define s_PATCH_TERMINATE_FAILED              GetStringFromID(202)
#define S_OPEN_DIR_FAILED                       GetStringFromID(203)
//#define S_CREATE_DIR_FAILED                       GetStringFromID(204)
//#define S_OUTDATED_BILL_FILE_NOT_BACKUP           GetStringFromID(205)
#define S_DELETE_BILL_FILE_FAILED               GetStringFromID(206)
#define S_BACKUP_TIMEERROR                      GetStringFromID(207)


#define S_BS_TRC_ERR_CFG_FILE_MINSIZE           GetStringFromID(208)
#define S_BS_TRC_ERR_CFG_FILE_MAXSIZE           GetStringFromID(209)
#define S_BS_TRC_ERR_CFG_AUTODEL_MINDAY         GetStringFromID(210)
#define S_BS_TRC_ERR_CFG_AUTODEL_MAXDAY         GetStringFromID(211)
#define S_BS_TRC_ERR_CFG_FILE_MINTIME           GetStringFromID(212)
#define S_BS_TRC_ERR_CFG_FILE_MAXTIME           GetStringFromID(213)
#define S_BS_ILLEGAL_CHL_ID                     GetStringFromID(214)
#define S_BS_FAIL_OPEN_FILE                     GetStringFromID(215)
#define S_BS_FAIL_READ_FILE                     GetStringFromID(216)
#define S_BS_FAIL_WRITE_FILE                    GetStringFromID(217)
#define S_BS_FIND_ERR_BILL                      GetStringFromID(218)
#define S_BS_CHECK_ERR_BILL_LEN                 GetStringFromID(219)

#define S_BACKUP_START_FAIL                     GetStringFromID(220)
#define S_BACKUP_CREATE_TASK_FAIL               GetStringFromID(221)
#define S_BACKUP_MANAGER_THREAD_FAIL            GetStringFromID(222)
#define S_BACKUP_MANAGER_THREAD_EXIT            GetStringFromID(223)
#define S_BACKUP_OPEN_TASK_FAIL                 GetStringFromID(224)
#define S_BACKUP_COPY_BILL                      GetStringFromID(225)
#define S_BACKUP_TASK_ISNULL                    GetStringFromID(226)
#define S_BACKUP_INI_OPEN_FAIL                  GetStringFromID(227)
#define S_BACKUP_STATUSFILE_OPERATE_FAIL        GetStringFromID(228)
#define S_BACKUP_DELETE_BILL                    GetStringFromID(229)
#define S_BACKUP_OPEN_DIR_FAIL                  GetStringFromID(230)
#define S_BACKUP_OPEN_STATUS_FAIL               GetStringFromID(231)
#define S_BACKUP_DELETE_FAILED                  GetStringFromID(232)
#define S_BACKUP_END_TASK                       GetStringFromID(233)
#define S_BACKUP_FILE_FAILED                    GetStringFromID(234)
#define S_BACKUP_DELETE_FILE_FAILED             GetStringFromID(235)
#define S_BACKUP_RENAME_DELETE_FAIL             GetStringFromID(236)
#define S_BACKUP_WRITE_STATUS_FAIL              GetStringFromID(237)
#define S_BACKUP_FILE_ATTR_ERR                  GetStringFromID(238)
#define S_BACKUP_CREATE_DIR_FAIL                GetStringFromID(239)
#define S_BACKUP_CONNECT_FAIL                   GetStringFromID(240)
#define S_BACKUP_RENAME_FAIL                    GetStringFromID(241)
#define S_BACKUP_EXCEED_LIMIT                   GetStringFromID(242)
#define S_BACKUP_RETRY                          GetStringFromID(243)
#define S_BACKUP_CONTINUE_LATER                 GetStringFromID(244)
#define S_BACKUP_BEGIN_DEL_DEST_FILE            GetStringFromID(245) //"准备删除目的路径%s下文件(ID=%d)"
#define S_BACKUP_CUR_TIM_DIR_NAME               GetStringFromID(246) //"当前时间为%s,待删除目录或文件为%s(ID=%d)"
#define S_BACKUP_SUCCESS                        GetStringFromID(247) //"成功"
#define S_BACKUP_FAILURE                        GetStringFromID(248) //"失败"
#define S_BACKUP_DEL_DIR                        GetStringFromID(249) //"删除%s%s(ID=%d)"
#define S_BACKUP_SEARCH_DIR                     GetStringFromID(250) //"搜索目录%s(ID=%d)"
#define S_BACKUP_FIND_DIR                       GetStringFromID(251) //"搜索到目录%s(ID=%d)"
#define S_BACKUP_PUSH_ROOT_DIR                  GetStringFromID(252) //"目录%s下没有日期目录,根目录作为二级目录(ID=%d)"
#define S_BACKUP_SEARCH_FILE_IN                 GetStringFromID(253) //"在%s下搜索文件(ID=%d)"
#define S_BACKUP_FIND_FILE_IN                   GetStringFromID(254) //"在%s下找到文件%s(ID=%d)"
#define S_BACKUP_NO_BAK_LAST_FILE               GetStringFromID(255) //"不备份最后一个文件%s(ID=%d)"
#define S_BACKUP_NO_FILE_IN_DIR                 GetStringFromID(256) //"目录%s中没有文件或只有一个文件或准备文件失败(ID=%d)"
#define S_BACKUP_CRE_DEST_DIR                   GetStringFromID(257) //"创建目的路径%s(ID=%d)"
#define S_BACKUP_SLEEP_INTERVAL                 GetStringFromID(258) //"等待%d秒之后继续(ID=%d)"
#define S_BACKUP_TOO_SHORT                      GetStringFromID(259) //"两次连接时间小于%d,上次连接时间为%s(ID=%d)"
#define S_BACKUP_CRE_DIR                        GetStringFromID(260) //"创建目录%s%s(ID=%d)"
#define S_BACKUP_CONNECT                        GetStringFromID(261) //"本机IP=%s,用户名=%s,连接%s%s(ID=%d)"
#define S_BACKUP_TASK_REACT                     GetStringFromID(262) //"重新启动任务%d"
#define S_BACKUP_DEL_FILE_FAILURE               GetStringFromID(263) //"删除文件%s失败"
#define S_NO_BACKUP_LICENSE                     GetStringFromID(264) //没有话单备份的License
#define S_BACKUP_DEL_OUTDATE_FILE               GetStringFromID(265) //"删除过期文件%s"
#define S_BACKUP_DEST_DIR_NOT_CONF              GetStringFromID(266) //"备份任务(%d)的备份目标路径没有配置"

#define S_BACKUP_WRITE_MAN_STATUS               GetStringFromID(267) 
#define S_BACKUP_RUNNING_NOW                    GetStringFromID(268) 
#define S_DELETE_RUNNING_NOW                    GetStringFromID(269) 
#define S_DELETE_SUCCESS                        GetStringFromID(270) 
#define S_DELETE_FAIL                           GetStringFromID(271) 
#define S_BACKUP_NOAUTO_TASK                    GetStringFromID(272) 
#define S_BACKUP_CONNECT_FAILED                 GetStringFromID(273) 
#define S_BACKUP_CRE_DIR_FAILED                 GetStringFromID(274) 
#define S_BACKUP_SET_DIR_FAILED                 GetStringFromID(275) 
#define S_BACKUP_TRANS_FAILED                   GetStringFromID(276) 
#define S_BACKUP_DISCARD_TASK                   GetStringFromID(277)

#define S_CLUSTERINTERFACE_MSG_DEACTIVEME       GetStringFromID(300)
#define S_CLUSTERINTERFACE_MSG_SHUTDOWN         GetStringFromID(301)
#define S_CLUSTERINTERFACE_MSG_MONITORINIT_OK   GetStringFromID(302)
#define S_CLUSTERINTERFACE_MSG_MONITORINIT_FAIL GetStringFromID(303)
#define S_CLUSTERINTERFACE_MSG_RCV_ANSACTIVE    GetStringFromID(304)
#define S_CLUSTERINTERFACE_MSG_RCV_ASKFAILOVER  GetStringFromID(305)
#define S_MSG_DISKFULL_MTNETWORK_PAUSING        GetStringFromID(306)
#define S_MSG_DISKRECOVERY_MTNETWORK_GOINGON    GetStringFromID(307)

#define S_COMM_CONNECT_KERNEL                   GetStringFromID(330)
#define S_COMM_APTYPE_UNRECOGNIZED              GetStringFromID(331)
#define S_COMM_INSTANCE_COMMLAYER               GetStringFromID(332)
#define S_COMM_UNINSTANCE_COMMLAYER             GetStringFromID(333)
#define S_COMM_READ_PM_IP                       GetStringFromID(334)
#define S_COMM_READ_BM_IP                       GetStringFromID(335)
#define S_COMM_READ_MML_IP                      GetStringFromID(336)
#define S_COMM_APTYPE_DISMATCH                  GetStringFromID(337)
#define S_COMM_READ_GSN_IP                      GetStringFromID(338)
#define S_COMM_SHAKE_HAND                       GetStringFromID(339)
#define S_COMM_TRANSMIT_MSG                     GetStringFromID(340)
#define S_COMM_CNT_KNL_CNT_ERR                  GetStringFromID(341)
#define S_COMM_CNT_KNL_REG_ERR                  GetStringFromID(342)
#define S_COMM_CNT_KNL_SEND_ERR                 GetStringFromID(343)
#define S_COMM_RECV_UPD_PACKAGE                 GetStringFromID(344)
#define S_COMM_SEND_UPD_PACKAGE                 GetStringFromID(345)
#define S_COMM_RECV_TCPIP_PACKAGE               GetStringFromID(346)
#define S_COMM_SEND_TCPIP_PACKAGE               GetStringFromID(347)
#define S_COMM_RECV_EXCEED_FRAME                GetStringFromID(348)
#define S_COMM_CRT_SOCKET_ERR                   GetStringFromID(349)
#define S_COMM_OPEN_LISTEN_PORT_ERR             GetStringFromID(350)
#define S_COMM_REG_SOCKET_ERR                   GetStringFromID(351)
#define S_COMM_MEM_LEN_ERR                      GetStringFromID(352)
#define S_ALARM_BOX_FRAME_EXCEED                GetStringFromID(353)
#define S_COMM_INSTANCE_COMMLAYER_FAIL          GetStringFromID(354)
#define S_IP_ADDRESS_ERROR                      GetStringFromID(355)
#define S_REDUP_IP_ADDRESS                      GetStringFromID(356)
#define S_GSN_IP_TOO_MANY                       GetStringFromID(357)

#define S_ENCODE_FAILED                         GetStringFromID(379)
#define S_CV_MSG_REC_PACKET                     GetStringFromID(380)
#define S_CV_MSG_PRINT_QUEUE_LEN                GetStringFromID(381)
#define S_CV_TRC_FAIL_DECODE                    GetStringFromID(382)
#define S_CV_TRC_REJ_DATA_SMALL_LEN             GetStringFromID(383)
#define S_CV_TRC_REJ_DATA_ZERO_LEN              GetStringFromID(384)
#define S_CV_TRC_REJ_DATA_LESS_2                GetStringFromID(385)
#define S_CV_TRC_ERR_AP_TYPE                    GetStringFromID(386)
#define S_CV_TRC_FAIL_GET_PACKET                GetStringFromID(387)
#define S_CV_TRC_FAIL_CONVERT_REC               GetStringFromID(388)
#define S_CV_TRC_CATCH_ERR                      GetStringFromID(389)
#define S_CV_TRC_FAIL_INIT_BASE                 GetStringFromID(390)
#define S_CV_TRC_FAIL_OPEN_INI                  GetStringFromID(391)
#define S_CV_TRC_FAIL_INIT_FMT                  GetStringFromID(392)
#define S_CV_TRC_ERR_RELEASE_VERSION            GetStringFromID(393)
#define S_CV_TRC_INVALID_CONF_PARA              GetStringFromID(394)
#define S_CV_TRC_CONF_PARA_EXCEED_BILL_LEN      GetStringFromID(395)


#define S_FRAME_OPEN_INI_FAIL                   GetStringFromID(420)
#define S_FRAME_INIT_INSTANCE_FAIL              GetStringFromID(421)
#define S_FRAME_RECEIVE_EXIT_CMD                GetStringFromID(422)
#define S_FRAME_SEND_QUIT_MSG_TO_ME             GetStringFromID(423)
#define S_FRAME_CACHE_GET_PACK_FAIL             GetStringFromID(424)
#define S_FRAME_CACHE_PUT_PACK_FAIL             GetStringFromID(425)
#define S_FRAME_CACHE_OPEN_FILE_FAIL            GetStringFromID(426)
#define S_FRAME_CACHE_SEEK_FILE_FAIL            GetStringFromID(427)
#define S_FRAME_CACHE_READ_FILE_FAIL            GetStringFromID(428)
#define S_FRAME_CACHE_NO_PACKETS                GetStringFromID(429)
#define S_FRAME_TM_MGR_START_THR_FAIL           GetStringFromID(430)
#define S_FRAME_TRACE_ASSERT                    GetStringFromID(431)
#define S_FRAME_CREATE_COM_OBJ_FAIL             GetStringFromID(432)
#define S_FRAME_ACTIVE_MODULE_FAIL              GetStringFromID(433)
#define S_FRAME_ACTIVE_MODULE_SUCCEED           GetStringFromID(434)
#define S_FRAME_UNPROCESSED_MSG                 GetStringFromID(435)
#ifdef _PRODUCT_MOTO
#define S_FRAME_MML_REPORT_HEADER_PART0         GetStringFromID(436+MOTO_OFFSET)
#else //_PRODUCT_MOTO
#define S_FRAME_MML_REPORT_HEADER_PART0         GetStringFromID(436)
#endif //_PRODUCT_MOTO
#define S_FRAME_MML_REPORT_HEADER_PART1         GetStringFromID(437)
#ifdef _PRODUCT_MOTO
#define S_FRAME_MML_REPORT_HEADER_PART2         GetStringFromID(438+MOTO_OFFSET)
#else //_PRODUCT_MOTO
#define S_FRAME_MML_REPORT_HEADER_PART2         GetStringFromID(438)
#endif //_PRODUCT_MOTO
#define S_FRAME_SENDER_PID                      GetStringFromID(439)
#define S_FRAME_RECEIVER_PID                    GetStringFromID(440)
#define S_FRAME_SENDER_MID                      GetStringFromID(441)
#define S_FRAME_RECEIVER_MID                    GetStringFromID(442)
#define S_FRAME_APP_TYPE                        GetStringFromID(443)
#define S_FRAME_CLIENT_NO                       GetStringFromID(444)
#define S_FRAME_USER_NAME                       GetStringFromID(445)
#define S_FRAME_CLIENT_IP                       GetStringFromID(446)
#define S_FRAME_CMD_CODE                        GetStringFromID(447)
#define S_FRAME_DATA_LENGTH                     GetStringFromID(448)
#define S_FRAME_MSG_SENDER                      GetStringFromID(449)
#define S_FRAME_MSG_RECEIVER                    GetStringFromID(450)
#define S_FRAME_DIAGRAM_SEQ                     GetStringFromID(451)
#define S_FRAME_MSG_TYPE                        GetStringFromID(452)
#define S_FRAME_PROTOCOL_CONTENT                GetStringFromID(453)
#define S_FRAME_DEBUG_CONTENT                   GetStringFromID(454)
#define S_FRAME_DEBUG_INFO                      GetStringFromID(455)
#define S_FRAME_SEND_MSG_OUT_FAIL               GetStringFromID(456)
#define S_FRAME_MSG_ENQUEUE_FAIL                GetStringFromID(457)
#define S_FRAME_NO_CORRESPONDER                 GetStringFromID(458)
#define S_MSGOUT_LEVEL_URGENT                   GetStringFromID(459)
#define S_MSGOUT_LEVEL_IMPORTANT                GetStringFromID(460)
#define S_MSGOUT_LEVEL_SIDLY                    GetStringFromID(461)
#define S_MSGOUT_LEVEL_NORMAL                   GetStringFromID(462)
#define S_MSG_ENQUE_FAILURE                     GetStringFromID(463)

#define S_FS_LOG_CREATE_FS_DATE                 GetStringFromID(480)
//MSGOUT信息
#define S_FS_MSG_REC_PACKET                     GetStringFromID(481)
#define S_FS_MSG_ERR_STATUS                     GetStringFromID(482)
#define S_FS_MSG_NORMAL_STATUS                  GetStringFromID(483)
#define S_FS_MSG_FAIL_SAVE                      GetStringFromID(484)
#define S_FS_MSG_FAIL_TIMER                     GetStringFromID(485)
//TRACE信息
#define S_FS_TRC_ERR_CFG_FILE_SIZE              GetStringFromID(486)
#define S_FS_TRC_SUCC_DEL_FILE                  GetStringFromID(487)
#define S_FS_TRC_FAIL_DEL_FILE                  GetStringFromID(488)
#define S_FS_TRC_DEL_LAST_FILE                  GetStringFromID(489)
#define S_FS_TRC_CREATE_NEW_FILE                GetStringFromID(490)
#define S_FS_TRC_BEGIN_RECOVER                  GetStringFromID(491)
#define S_FS_TRC_END_RECOVER                    GetStringFromID(492)
#define S_FS_TRC_BOTH_VALID                     GetStringFromID(493)
#define S_FS_TRC_MAIN_DIFF_BAK                  GetStringFromID(494)
#define S_FS_TRC_MAIN_SAME_BAK                  GetStringFromID(495)
#define S_FS_TRC_MAIN_V_BAK_INV                 GetStringFromID(496)
#define S_FS_TRC_MAIN_INV_BAK_V                 GetStringFromID(497)
#define S_FS_TRC_MAIN_INV_BAK_INV               GetStringFromID(498)
#define S_FS_TRC_MAIN_V_BAK_INE                 GetStringFromID(499)
#define S_FS_TRC_MAIN_INV_BAK_INE               GetStringFromID(500)
#define S_FS_TRC_MAIN_INE_BAK_V                 GetStringFromID(501)
#define S_FS_TRC_MAIN_INE_BAK_INV               GetStringFromID(502)
#define S_FS_TRC_MAIN_INE_BAK_INE               GetStringFromID(503)
#define S_FS_TRC_FAIL_INIT_BASE                 GetStringFromID(504)
#define S_FS_TRC_FAIL_OPEN_INI                  GetStringFromID(505)
#define S_FS_TRC_FAIL_CHECK_STATUS              GetStringFromID(506)
#define S_FS_TRC_FAIL_FLUSH_MAIN                GetStringFromID(507)
#define S_FS_TRC_FAIL_FLUSH_BAK                 GetStringFromID(508)
#define S_FS_TRC_FAIL_OPEN_MAIN                 GetStringFromID(509)
#define S_FS_TRC_FAIL_OPEN_BAK                  GetStringFromID(510)
#define S_FS_TRC_FAIL_INITBYSTATUS              GetStringFromID(511)
#define S_FS_TRC_FAIL_CLOSE_BILL                GetStringFromID(512)
#define S_FS_TRC_FAIL_SAVE                      GetStringFromID(513)
#define S_FS_TRC_FAIL_TIMER                     GetStringFromID(514)
#define S_FS_TRC_ERR_CFG_FILE_MINSIZE           GetStringFromID(515)
#define S_FS_TRC_ERR_CFG_FILE_MAXSIZE           GetStringFromID(516)
#define S_FS_TRC_ERR_CFG_AUTODEL_MINDAY         GetStringFromID(517)
#define S_FS_TRC_ERR_CFG_AUTODEL_MAXDAY         GetStringFromID(518)
#define S_FS_TRC_MAKE_EMPTY_PACKAGE             GetStringFromID(521)
/*
    前存盘第%u话单包修正为空包
    Orginal cdr package(ID=%u) changed to be empty
*/
//修改完毕
#define S_MML_PWD_LIMIT_TIME                    GetStringFromID(526)
#define S_LOG_AUTO_ARCHIEVE                     GetStringFromID(527) //自动归档日志
#define S_LOG_FIND_LOG_FILE                     GetStringFromID(528) //搜索到日志文件%s,当前时间为%s
#define S_LOG_FILE_NOEXIST                      GetStringFromID(529) //日志文件不存在(%s)
#define S_LOG_OPEN_CONFIG_FAIL                  GetStringFromID(530)
#define S_LOG_OPEN_LOG_FAIL                     GetStringFromID(531)
#define S_LOG_GET_LOG_STAT_FAIL                 GetStringFromID(532)
#define S_LOG_REMOVE_LOG_FAIL                   GetStringFromID(533)
#define S_LOG_REMOVE_LOG_SUC                    GetStringFromID(534)
#define S_LOG_SEEK_FAIL                         GetStringFromID(535)
#define S_LOG_WRITE_LOG_FAIL                    GetStringFromID(536)
#define S_LOG_READ_LOG_FAIL                     GetStringFromID(537)
#define S_LOG_OPEN_DIR_FAIL                     GetStringFromID(538)
#define S_LOG_RECV_WRITE_LOG                    GetStringFromID(539)
#define S_LOG_RECV_MML_CMD                      GetStringFromID(540)
#define S_LOG_RECV_OTHER_MSG                    GetStringFromID(541)
#define S_LOG_RECV_MML_LST_LOG                  GetStringFromID(542)
#define S_LOG_SERIALNO                          GetStringFromID(543)
#define S_LOG_TYPE                              GetStringFromID(544)
#define S_LOG_USER_NAME                         GetStringFromID(545)
#define S_LOG_OPERATOR_IP                       GetStringFromID(546)
#define S_LOG_DATE                              GetStringFromID(547)
#define S_LOG_OPERATOR_RESULT                   GetStringFromID(548)
#define S_LOG_OPERATOR_CMD                      GetStringFromID(549)
#define S_LOG_CMD_SOURCE                        GetStringFromID(550)
#define S_LOG_TIME_NOT_MATCH                    GetStringFromID(551)
#define S_LOG_TYPE_INVALID                      GetStringFromID(552)
#define S_LOG_QUERY_INVALID_USER                GetStringFromID(553)
#define S_LOG_CMD_EXP                           GetStringFromID(554)
#define S_LOG_LOGBUF_NULL                       GetStringFromID(555)
#define S_LOG_OPENINIFILEFAIL                   GetStringFromID(556)
#define S_LOG_OPENLOGFILEFAIL                   GetStringFromID(557)
#define S_LOG_TABLE                             GetStringFromID(558)
#define S_LOG_QUERY_EMPTY                       GetStringFromID(559)
#define S_LOG_REASON                            GetStringFromID(560)
#define S_MML_OPEN_USER_INI_FAIL                GetStringFromID(561) //打开用户权限文件失败(%s)
#define S_MML_END_TIME                          GetStringFromID(562) //结束时间
#define S_MML_BEGIN_TIME                        GetStringFromID(563) //开始时间
#define S_MML_USER_LOGIN_DEL_FAIL               GetStringFromID(564) //用户在线无法删除
#define S_CMD_END_TIME_TOO_SMALL                GetStringFromID(565) //结束时间小于当前时间
#define S_CMD_TIME_WRONG                        GetStringFromID(566) //起始时间大于等于结束时间
#define S_MML_PRIVILEGE_INFO                    GetStringFromID(568) //"用户权限" //577
#define S_MML_ERR_NAME_NULL                     GetStringFromID(569) //用户名参数为空
#define S_MML_AREA                              GetStringFromID(570) //"地理区域" //579
#define S_MML_CMD_FIND_ERR                      GetStringFromID(571) //"找不到命令%s"
#define S_MML_CMD_NO_IN_SET                     GetStringFromID(572) //"集合%d中没有命令%s"
#define S_MML_CMD_EXE_PARTIALLY                 GetStringFromID(573) //"命令执行部分成功"
#define S_MML_CMD_LIST                          GetStringFromID(574) //"MML命令列表"
#define S_MML_COMMON_USER_RUN_ADMIN             GetStringFromID(575) //"普通用户执行超级命令"
#define S_MML_NEW_USER_NO_ADD_CMD               GetStringFromID(576) //"没有为新增用户赋予权限"
#define S_MML_NOT_ADD_CMD_FOR_ADM               GetStringFromID(577) //"不能对Admin用户操作"
#define S_MML_QUERY_NO_RESULT                   GetStringFromID(578) //"查询无结果"
#define S_MML_CMD_IN_OTHER_SET                  GetStringFromID(579) //"命令%s属于其他组,不能操作"

#define S_MML_NO_CMD_TARGET                     GetStringFromID(580)
#define S_MML_SUCCEED                           GetStringFromID(581)
#define S_MML_FAIL                              GetStringFromID(582)
#define S_MML_NO_RIGHT                          GetStringFromID(583)
#define S_MML_INVALID_CMD                       GetStringFromID(584)
#define S_MML_ACC_NOT_EXIST                     GetStringFromID(585)
#define S_MML_USR_NAME_NULL                     GetStringFromID(586)
#define S_MML_USR_DESP_NULL                     GetStringFromID(587)
#define S_MML_PWD_INCORRECT                     GetStringFromID(588)
#define S_MML_PWD_UNMATCH                       GetStringFromID(589)
#define S_MML_SHK                               GetStringFromID(590)
#define S_MML_OPEN_INI_FAIL                     GetStringFromID(591)
//[ADDOP]命令输出的MML报文.
#define S_MML_ADD_OP_OK                         GetStringFromID(592)
#define S_MML_ADD_OP_FAIL                       GetStringFromID(593)
#define S_MML_ACC_EXIST                         GetStringFromID(594)
#define S_MML_REACH_ACC_LIMIT                   GetStringFromID(595)
#define S_MML_REACH_PWD_LIMIT                   GetStringFromID(596)
#define S_MML_ACCOUNT_IS_EMPTY                  GetStringFromID(597)

//[LSTOP]命令输出的MML报文头.
#define S_MML_LST_OP_NAME                       GetStringFromID(598)
#define S_MML_LST_OP_DESP                       GetStringFromID(599)
#define S_MML_LST_OP_RIGHT                      GetStringFromID(600)
#define S_MML_LST_OP_ONLINE                     GetStringFromID(601)
#define S_MML_ADMIN                             GetStringFromID(602)
#define S_MML_NORMAL                            GetStringFromID(603)
#define S_MML_ONLINE                            GetStringFromID(604)
#define S_MML_OFFLINE                           GetStringFromID(605)
#define S_MML_LOGIN_SUCCEED                     GetStringFromID(606)
#define S_MML_LOGOUT_SUCCEED                    GetStringFromID(607)
#define S_MML_TOO_MANY_USER                     GetStringFromID(608)
#define S_MML_ALREADY_LGGIN                     GetStringFromID(609)
#define S_MML_DEBUG_LEVEL                       GetStringFromID(610)
#define S_MML_PWD_OUTDATED                      GetStringFromID(611)
#define S_MML_DEBUG_NONE                        GetStringFromID(612)
#define S_MML_DEBUG_EMERGENT                    GetStringFromID(613)
#define S_MML_DEBUG_IMPORTANT                   GetStringFromID(614)
#define S_MML_DEBUG_MINOR                       GetStringFromID(615)
#define S_MML_DEBUG_COMMON                      GetStringFromID(616)
#define S_MML_DEBUG_ERROR                       GetStringFromID(617)
#define S_MML_LST_VERSION                       GetStringFromID(618)
#define S_MML_INIT_MACRO_FAIL                   GetStringFromID(619)
#define S_MML_INIT_USER_FAIL                    GetStringFromID(620)
#define S_MML_PARSE_FAIL                        GetStringFromID(621)
#define S_MML_NO_TARGET_RECV                    GetStringFromID(622)
#define S_MML_OPERATION_SUCC                    GetStringFromID(623)
#define S_MML_OPERATION_FAIL                    GetStringFromID(624)
#define S_MML_SET_FLAG                          GetStringFromID(625)
#define S_MML_OPERATION_DENIED                  GetStringFromID(626)
#define S_MML_IGWB_VER_TITLE                    GetStringFromID(627)
#define S_MML_SESSION							GetStringFromID(628)


#ifdef _PRODUCT_MOTO
#define S_MML_IGWB_VER                          GetStringFromID(629+MOTO_OFFSET)
#else //_PRODUCT_MOTO
#define S_MML_IGWB_VER                          GetStringFromID(629)
#endif //_PRODUCT_MOTO

#define S_MML_USER_EXPIRE                       GetStringFromID(630) //"用户不在允许时间范围内"
#define S_MML_NOTE_INFO                         GetStringFromID(631) //"提示信息"
#define S_MML_CMD_ADM_INIT_FAIL                 GetStringFromID(632) //"命令管理文件初始化失败"
#define S_MML_CMD_ADM_NUM_ZERO                  GetStringFromID(633) //"命令文件中Set%d节,命令数目有误"
#define S_MML_CMD_ADM_CRE_USER_FILE             GetStringFromID(634) //"创建用户文件失败:%s"
#define S_MML_CMD_ADM_USER_NMAE_ERR             GetStringFromID(635) //"用户名(%d)出错"
#define S_MML_OPEN_USER_INI_ERR                 GetStringFromID(636) //"无法打开用户配置文件"
#define S_MML_CMD_ADM_USER_ERR1                 GetStringFromID(637) //"读取用户%s的%s出错"
#define S_MML_WRITE_INI_ERR                     GetStringFromID(638) //"写INI出错 "
#define S_MML_DATE_FORMAT_ERR                   GetStringFromID(639) //"日期参数格式出错 "
#define S_MML_DATE_LEN_ERR                      GetStringFromID(640) //"日期参数长度错误"
#define S_MML_NO_CMD_IN_MAP                     GetStringFromID(641) //"命令表中无此命令"
#define S_MML_2_TIME_FORMAT_ERR                 GetStringFromID(642) //"两个时间格式错误"
#define S_MML_USER_NAME_NONEXIST                GetStringFromID(643) //"用户名不存在,或者没有为新增用户赋权限"
#define S_MML_ADMIN_USER_RUN_ALL                GetStringFromID(644) //"admin可以执行所有命令"
#define S_MML_COMMAND_COL1                      GetStringFromID(645) //"命令列1"
#define S_MML_COMMAND_COL2                      GetStringFromID(646) //"命令列2"
#define S_MML_CMD_LIST_TITLE                    GetStringFromID(647) //"MML命令列表"
#define S_MML_CMD_IN_DEFAULT_SET                GetStringFromID(648) //"命令%s属于系统命令,不能操作"
#define S_MML_CMD_NONE_EXIST                    GetStringFromID(649) //"命令%s不存在,不能添加"

#define SZ_CMD_EXIT                             GetStringFromID(650)
#define SZ_KNL_GET_READY                        GetStringFromID(651)
#define SZ_KNL_INIT_FAILURE                     GetStringFromID(652)
#define SZ_KNL_IS_ACTIVE                        GetStringFromID(653)
#define SZ_KNL_ACTIVE_FAILURE                   GetStringFromID(654)
#define SZ_MML_CMDDESC_AUP                      GetStringFromID(655)
#define SZ_AUP_IS_GOINGON                       GetStringFromID(656)
#define SZ_AUP_IS_FAILED                        GetStringFromID(657)
#define SZ_AUP_IS_COMPLETE                      GetStringFromID(658)
#define SZ_MSG_BEGIN_ACTIVE                     GetStringFromID(659)
#define SZ_MSG_BEGIN_DEACTIVE                   GetStringFromID(660)
#define SZ_MSG_STARTPROCESS_FAIL                GetStringFromID(661)
#define SZ_MSG_ALL_PROCESS_OK                   GetStringFromID(662)
#define SZ_MSG_EXECPT_OVER                      GetStringFromID(663)
#define SZ_MSG_WATCHDOG_EXIT                    GetStringFromID(664)
#define SZ_MSG_ASKFOR_FAILOVER                  GetStringFromID(665)
#define SZ_KNL_PROC_NOT_EXIST                   GetStringFromID(666)
#define S_MONITOR_REVMOREAUXUPG                 GetStringFromID(667)
#define S_AP_NAME_DUP                           GetStringFromID(668)

#define S_NET_GSN_RESTART                       GetStringFromID(690)
#define S_NET_LOAD_GTP_STACK                    GetStringFromID(691)
#define S_NET_SAVE_GTP_STATUS                   GetStringFromID(692)
#define S_NET_READ_SEQ_FILE                     GetStringFromID(693)
#define S_NET_CREATE_SEQ_FILE                   GetStringFromID(694)
#define S_NET_CREATE_GTP_STACK                  GetStringFromID(695)
#define S_NET_REMOVE_GTP_STACK                  GetStringFromID(696)
#define S_NET_OPEN_NET_STATUS_FILE              GetStringFromID(697)
#define S_NET_WRITE_NET_STATUS_FILE             GetStringFromID(698)
#define S_NET_READ_NET_STATUS_FILE              GetStringFromID(699)
#define S_NET_FLUSH_NET_STATUS                  GetStringFromID(700)
#define S_NET_READ_MAIN_NET_STATUS              GetStringFromID(701)
#define S_NET_CREATE_NET_STATUS_DIR             GetStringFromID(702)
#define S_NET_AP_TYPE_UNKNOWN                   GetStringFromID(703)
#define S_NET_INIT_NETAPP                       GetStringFromID(704)
#define S_NET_CRT_NET_STATUS_FILE               GetStringFromID(705)
#define S_NET_FAIL_TO_RECOVERY                  GetStringFromID(706)
#define S_NET_ADJUST_PACKAGE_ID                 GetStringFromID(707)
#define S_NET_MAKE_BILL_PACKAGE                 GetStringFromID(708)
#define S_NET_UNRECOGNIZED_FRAME                GetStringFromID(709)
#define S_NET_SEND_NODE_ALIVE_REQ               GetStringFromID(710)
#define S_NET_RECV_NODE_ALIVE_REQ               GetStringFromID(711)
#define S_NET_READ_ERR_IN_RELEASE               GetStringFromID(712)
#define S_NET_WRONG_IEBILLPACKAGE               GetStringFromID(713)
#define S_NET_OPEN_SEQ_FILE_ERR                 GetStringFromID(714)
#define S_NET_CHECK_TV_SIZE                     GetStringFromID(715)
#define S_NET_CHECK_TV_IE_TYPE                  GetStringFromID(716)
#define S_NET_CHECK_TLV_SIZE                    GetStringFromID(717)
#define S_NET_NETAPP_ACTIVE                     GetStringFromID(718)
#define S_NET_NETAPP_DEACTIVE                   GetStringFromID(719)
#define S_NET_INIT_STATUS_FILE                  GetStringFromID(720)
#define S_NET_IGNORE_AUXUPGRADE                 GetStringFromID(721)
#ifdef _PRODUCT_MOTO
#define S_PROTOCOL_TRACK_HEAD                   GetStringFromID(722+MOTO_OFFSET)
#else //_PRODUCT_MOTO
#define S_PROTOCOL_TRACK_HEAD                   GetStringFromID(722)
#endif //_PRODUCT_MOTO
#define S_MML_TRACK_CONTENT                     GetStringFromID(723)
#define S_TRACK_MSG_SENDER                      GetStringFromID(724)
#define S_TRACK_MSG_RECEIVER                    GetStringFromID(725)
#define S_TRACK_MSG_TYPE                        GetStringFromID(726)
#define S_TRACK_SEQ_NUM                         GetStringFromID(727)
#define S_TRACK_RESTART_COUNT                   GetStringFromID(728)
#define S_TRACK_ITEM_TYPE                       GetStringFromID(729)
#define S_IE_SEND_DATA                          GetStringFromID(730)
#define S_IE_SEND_DATA_MAYBE_DUP                GetStringFromID(731)
#define S_IE_CANCEL_DATA                        GetStringFromID(732)
#define S_IE_RELEASE_DATA                       GetStringFromID(733)
#define S_RECORD_COUNT                          GetStringFromID(734)
#define S_RECORD_SIZE                           GetStringFromID(735)
#define S_VERSION_NOT_SUPPORTED                 GetStringFromID(736)
#define S_ECHO_REQUEST                          GetStringFromID(737)
#define S_ECHO_RESPONSE                         GetStringFromID(738)
#define S_DATA_TRANSFER_REQUEST                 GetStringFromID(739)
#define S_DATA_TRANSFER_RESPONSE                GetStringFromID(740)
#define S_NODE_ALIVE_REQUEST                    GetStringFromID(741)
#define S_NODE_ALIVE_RESPONSE                   GetStringFromID(742)
#define S_REDIRECTION_REQUEST                   GetStringFromID(743)
#define S_REDIRECTION_RESPONSE                  GetStringFromID(744)
#define S_UNRECOGNIZED                          GetStringFromID(745)
#define S_IP_ADDRESS_V6                         GetStringFromID(746)
#define S_REPONSE_CAUSE                         GetStringFromID(747)
#define S_CAUSE_REQUEST_ACCEPTED                GetStringFromID(748)
#define S_CAUSE_REQUEST_DUPFULFILLED            GetStringFromID(749)
#define S_CAUSE_SYSTEM_FAILURE                  GetStringFromID(750)
#define S_CAUSE_TRANS_BUF_FULL                  GetStringFromID(751)
#define S_CAUSE_RECEIVE_BUF_FULL                GetStringFromID(752)
#define S_CAUSE_ANOTHER_GODOWN                  GetStringFromID(753)
#define S_CAUSE_OWN_GODOWN                      GetStringFromID(754)
#define S_CAUSE_UNRECOGNIZED                    GetStringFromID(755)
#define S_REDIRECTION_CAUSE                     GetStringFromID(756)
#define S_REDIRECTION_ADDRESS                   GetStringFromID(757)
#define S_NODE_ALIVE_ADDRESS                    GetStringFromID(758)
#define S_FIRST_FRAME_SEQ                       GetStringFromID(759)
#define S_FRAME_NOT_CORRECT                     GetStringFromID(760)
#define S_GTP_BUFFER_EXCEED                     GetStringFromID(761)
#define S_RECV_GSN_IP_0                         GetStringFromID(762)
#define S_MML_EXPIRE_USER						GetStringFromID(764)
/*
    接收到来自0.0.0.0的GTP'协议包
    Received GTP' package from 0.0.0.0
*/
#define S_REMOVE_SCAN_STACK                     GetStringFromID(763)
/*
    删除扫描协议栈(%d)
    Delete ScanStack(NO.%d)
*/

#define S_PERF_OPEN_PERF_FAIL                   GetStringFromID(780)
#define S_PERF_WRITE_PERF_FAIL                  GetStringFromID(781)
#define S_PERF_OPEN_DIR_FAIL                    GetStringFromID(782)
#define S_PERF_GET_PERF_STAT_FAIL               GetStringFromID(783)
#define S_PERF_REMOVE_PERF_FAIL                 GetStringFromID(784)
#define S_PERF_REMOVE_PERF_SUC                  GetStringFromID(785)
#define S_PERF_FRONT_MIN_ROOM_ALM               GetStringFromID(786)
#define S_PERF_BACK_MIN_ROOM_ALM                GetStringFromID(787)
#define S_PERF_BACKUP_MIN_ROOM_ALM              GetStringFromID(788)
#define S_PERF_FRONT_HV_ROOM_ALM                GetStringFromID(789)
#define S_PERF_BACK_HV_ROOM_ALM                 GetStringFromID(790)
#define S_PERF_RECV_SAVE_PERF                   GetStringFromID(791)
#define S_PERF_RECV_OTHER_MSG                   GetStringFromID(792)
#define S_PERF_RECV_HB_ANS                      GetStringFromID(793)
#define S_PERF_RECV_MML_CMD                     GetStringFromID(794)
#define S_PERF_RECV_MML_LST_PERF                GetStringFromID(795)
#define S_PERF_LOGIC_DISK_NAME                  GetStringFromID(796)
#define S_PERF_LOGIC_DISK_TOTAL                 GetStringFromID(797)
#define S_PERF_LOGIC_DISK_AVA                   GetStringFromID(798)
#define S_PERF_OS                               GetStringFromID(799)
#define S_PERF_SP                               GetStringFromID(800)
#define S_PERF_COMPUTER_NAME                    GetStringFromID(801)
#define S_PERF_CPU_PERCENT                      GetStringFromID(802)
#define S_PERF_CPU_NUM                          GetStringFromID(803)
#define S_PERF_PHY_MEM_TOTAL                    GetStringFromID(804)
#define S_PERF_PHY_MEM_AVA                      GetStringFromID(805)
#define S_PERF_LOG_MEM_TOTAL                    GetStringFromID(806)
#define S_PERF_LOG_MEM_AVA                      GetStringFromID(807)
#define S_PERF_LOGICAL_DISK_NUM                 GetStringFromID(808)
#define S_PERF_LOGICAL_DISK_INFO                GetStringFromID(809)
#define S_PERF_BILLSVRNO                        GetStringFromID(810)
#define S_PERF_HEARTBEAT                        GetStringFromID(811)
#define S_PERF_CPU_PER                          GetStringFromID(812)
#define S_PERF_FRONTSAVE                        GetStringFromID(813)
#define S_PERF_BACKSAVE                         GetStringFromID(814)
#define S_PERF_MEM                              GetStringFromID(815)
#define S_PERF_GSN_CONNECT                      GetStringFromID(816)
#define S_PERF_MSG_PRI_CONNECT                  GetStringFromID(817)
#define S_PERF_MSG_SEC_CONNECT                  GetStringFromID(818)
#ifdef _PRODUCT_MOTO
#define S_PERF_MML_REPORT_HEADER                GetStringFromID(819+MOTO_OFFSET)
#else //_PRODUCT_MOTO
#define S_PERF_MML_REPORT_HEADER                GetStringFromID(819)
#endif //_PRODUCT_MOTO

#define S_PERF_OSDBINIT_FAIL                    GetStringFromID(820)
#ifdef _PRODUCT_MOTO
#define S_PERF_IP_LIST_TITLE                    GetStringFromID(821+MOTO_OFFSET)
#else //_PRODUCT_MOTO
#define S_PERF_IP_LIST_TITLE                    GetStringFromID(821)
#endif //_PRODUCT_MOTO
#define S_PERF_FMT_STR1                         GetStringFromID(822)
#define S_PERF_IP                               GetStringFromID(823)
#define S_PERF_FMT_STR2                         GetStringFromID(824)
#define S_PERF_IP_RESULT                        GetStringFromID(825)
#define S_PERF_ADJUST_ALARM_SPACE               GetStringFromID(826)
#define S_PERF_PROCNAME                         GetStringFromID(827)
#define S_PERFPROCID                            GetStringFromID(828)
#define S_PERF_PROCSTATUS                       GetStringFromID(829)
#define S_PERF_FILE_NAME                        GetStringFromID(830)
#define S_PERF_CRE_DATE                         GetStringFromID(831) //"创建日期"
#define S_PERF_FILE_SIZE                        GetStringFromID(832) //"文件大小(KB)"
#define S_PERF_COL_POINT                        GetStringFromID(833) //"采集点名称"
#define S_PERF_COL_DATE                         GetStringFromID(834) //"采集日期"
#define S_PERF_COL_FILE_NUM                     GetStringFromID(835) //"文件数量"
#define S_PERF_ALL_FILE_SIZE                    GetStringFromID(836) //"所有文件长度(KB)"
#define S_PERF_COL_STATISTIC                    GetStringFromID(837) //"采集情况统计"

#define S_QB_PRODUCT_ANME                       GetStringFromID(5006)
#define S_QB_PRODUCT_INDEX                      GetStringFromID(5007)
#define S_QB_TH_LIST_PRODUCT_NAME               GetStringFromID(840)
#define S_QB_TH_LIST_BILL_TYPE                  GetStringFromID(841)
#define S_QB_TH_LIST_CHL                        GetStringFromID(842)
#define S_QB_TH_LIST_ORIG_DATE                  GetStringFromID(843)
#define S_QB_TH_LIST_FINAL_DATE                 GetStringFromID(844)
#define S_QB_TH_LIST_FILE                       GetStringFromID(845)
#define S_QB_TH_LIST_FMT                        GetStringFromID(846)
#define S_QB_TH_QUERY_COND                      GetStringFromID(847)
#define S_QB_TH_PAGE_INFO                       GetStringFromID(848)
#define S_QB_TH_BILL_DATA                       GetStringFromID(849)

//以下是MML报告中的表标题中的字符串资源
#define S_QB_TT_PRODUCT_NAME                    GetStringFromID(850)
#define S_QB_TT_BILL_TYPE                       GetStringFromID(851)
#define S_QB_TT_AP_NAME                         GetStringFromID(852)
#define S_QB_TT_CHANNEL_NO                      GetStringFromID(853)
#define S_QB_TT_CHANNEL_NAME                    GetStringFromID(854)
#define S_QB_TT_ORIG_DATE_NO                    GetStringFromID(855)
#define S_QB_TT_ORIG_DATE_NAME                  GetStringFromID(856)
#define S_QB_TT_CHL_DATE_NO                     GetStringFromID(857)
#define S_QB_TT_CHL_DATE_NAME                   GetStringFromID(858)
#define S_QB_TT_FILE_NAME                       GetStringFromID(859)
#define S_QB_TT_FORMAT_NAME                     GetStringFromID(860)
#define S_QB_TT_FORMAT_DESC                     GetStringFromID(861)
#define S_QB_TT_CHL_INFO                        GetStringFromID(862)
#define S_QB_TT_FIELD_NAME                      GetStringFromID(863)
#define S_QB_TT_FIELD_DESC                      GetStringFromID(864)
#define S_QB_TT_FIELD_TYPE                      GetStringFromID(865)
#define S_QB_TT_FIELD_OPERATOR                  GetStringFromID(866)
#define S_QB_TT_FIELD_UNIT                      GetStringFromID(867)
#define S_QB_TT_ENUM_FLAG                       GetStringFromID(868)
#define S_QB_TT_ENUM_VALUE                      GetStringFromID(869)
#define S_QB_TT_HAVE_NEXT_PAGE                  GetStringFromID(870)
#define S_QB_TT_CUR_PAGE_NO                     GetStringFromID(871)
#define S_QB_TT_CUR_PERCENT                     GetStringFromID(872)
#define S_QB_RI_PAGE_NO_INVALID                 GetStringFromID(873)
#define S_QB_RI_TASK_NOT_EXIST                  GetStringFromID(874)
#define S_QB_RI_SUCCESS_CLOSE_TASK              GetStringFromID(875)
#define S_QB_RI_SHAKE_CORRECT                   GetStringFromID(876)
#define S_QB_RI_FAIL_GET_PAGE_DATA              GetStringFromID(877)
#define S_QB_RI_FAIL_PREPARE_FILE               GetStringFromID(878)
#define S_QB_RI_FAIL_RUN_TASK                   GetStringFromID(879)
#define S_QB_RI_FAIL_DELETE_PAGE                GetStringFromID(880)
#define S_QB_RI_FAIL_OPEN_FILE                  GetStringFromID(881)
#define S_QB_RI_FIND_EMPTY_PAGE                 GetStringFromID(882)
#define S_QB_RI_FAIL_PREPARE_DATA               GetStringFromID(883)
#define S_QB_RI_FAIL_GET_FORMAT                 GetStringFromID(884)
#define S_QB_TRC_ERR_CFG_PREP_PAGE              GetStringFromID(885)
#define S_QB_TRC_ERR_CFG_PAGE_SIZE              GetStringFromID(886)
#define S_QB_TRC_REC_ERR_CMD                    GetStringFromID(887)
#define S_QB_TRC_FAIL_INIT_FORMAT               GetStringFromID(888)
#define S_QB_TRC_FAIL_OPEN_FILE_00              GetStringFromID(889)
#define S_QB_TRC_FAIL_DEL_MORE_PAGE             GetStringFromID(890)
#define S_QB_TRC_FAIL_OPEN_FILE                 GetStringFromID(891)
#define S_QB_TRC_FAIL_PREP_DATA                 GetStringFromID(892)
#define S_QB_TRC_FAIL_INIT_BASE                 GetStringFromID(893)
#define S_QB_TRC_FAIL_INIT_QB_FMT               GetStringFromID(894)
#define S_QB_TRC_FAIL_OPEN_INI                  GetStringFromID(895)
#define S_QB_ERROR_FIELD                        GetStringFromID(896)
#define S_FILE_TYPE_FOLDER                      GetStringFromID(897)
#define S_FILE_TYPE_FILE                        GetStringFromID(898)
#define S_QB_TH_PROGESS_INFO                    GetStringFromID(899)
#define S_QB_TT_RECORD_COUNT                    GetStringFromID(900)
#define S_QB_TT_FIRST_RECORD_COUNT              GetStringFromID(901)
#define S_QB_TT_LAST_RECORD_COUNT               GetStringFromID(902)
#define S_QB_RI_FAIL_GET_PROGRESS               GetStringFromID(903)
#define S_QB_TRC_FAIL_SAVE_RESULT               GetStringFromID(904)
#define S_QB_RI_SUCCESS_STOP_TASK               GetStringFromID(905)
#define S_QB_RI_SUCCESS_START_TASK              GetStringFromID(906)

#define S_PROCESS_START                         GetStringFromID(940)
#define S_PROCESS_STOP                          GetStringFromID(941)
#define S_RECV_SIGNAL_IGNORED                   GetStringFromID(942)
#define S_RECV_SIGNAL_EXIT                      GetStringFromID(943)
#define S_RECV_SIGNAL_DEFAULT                   GetStringFromID(944)
#define S_RECV_UNKNOW_SIGNAL                    GetStringFromID(945)
#define S_PROCESS_OPEN_INIFAIL                  GetStringFromID(946)
#define SZ_PROCESS_OM_DES                       GetStringFromID(947)
#define SZ_PROCESS_KNL_DES                      GetStringFromID(948)
#define SZ_PROCESS_CFG_DES                      GetStringFromID(949)
#define SZ_PROCESS_MTR_DES                      GetStringFromID(950)

///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06
#define SZ_PROCESS_BS_DES                       GetStringFromID(951) 


#define STR_OPRSUCCESS                          GetStringFromID(960)
#define STR_OPRFAIL                             GetStringFromID(961)
#define STR_USERFILE_UPDATE_FAILED              GetStringFromID(962)
#define S_NET_LOAD_SLIDE_STACK                  GetStringFromID(1000)
#define S_NET_SAVE_SLIDE_STATUS                 GetStringFromID(1001)
#define S_NET_CREATE_SLIDE_STACK                GetStringFromID(1002)
#define S_NET_RECEIVE_ERR_FRAME                 GetStringFromID(1003)
#define S_NET_RECEIVE_ACKNUM_FRAME              GetStringFromID(1004)
#define S_NET_RECEIVE_SYNC_FRAME                GetStringFromID(1005)
#define S_NET_RECEIVE_UNKNOWN_FRAME             GetStringFromID(1006)
#define S_NET_NO_LINK_TO_SEND_ALARM             GetStringFromID(1007)
#ifdef _PRODUCT_MOTO
#define S_BAU                                   GetStringFromID(1008+MOTO_OFFSET)
#else //_PRODUCT_MOTO
#define S_BAU                                   GetStringFromID(1008)
#endif //_PRODUCT_MOTO
#define S_MODULE_I                              GetStringFromID(1009)
#define S_CMD_ALARM_RESPONSE                    GetStringFromID(1010)
#define S_CONTENT_ALARM_RESPONSE                GetStringFromID(1011)
#define S_ERR_LENGTH                            GetStringFromID(1012)
#define S_CMD_REQ_ACKNUM                        GetStringFromID(1013)
#define S_CONTENT_REQ_ACKNUM                    GetStringFromID(1014)
#define S_CMD_REQ_SYN                           GetStringFromID(1015)
#define S_CONTENT_REQ_SYN                       GetStringFromID(1016)
#define S_CMD_REQ_DATA                          GetStringFromID(1017)
#define S_CONTENT_REQ_DATA                      GetStringFromID(1018)
#define S_CMD_UNKNOWN                           GetStringFromID(1019)
#define S_CMD_BIN_ALARM                         GetStringFromID(1020)
#define S_CONTENT_BIN_ALARM                     GetStringFromID(1021)
#define S_CMD_RSP_ACKNUM                        GetStringFromID(1022)
#define S_CMD_RSP_ACK                           GetStringFromID(1023)
#define S_CONTENT_RSP_ACK                       GetStringFromID(1024)
#define S_CMD_RSP_REJ                           GetStringFromID(1025)
#define S_CONTENT_RSP_REJ                       GetStringFromID(1026)
#define S_CAUSE_CDR_DECODE_ERR                  GetStringFromID(1027)
#define S_CAUSE_INVALID_MSG_FMT                 GetStringFromID(1028)
#define S_CAUSE_VER_NOT_SUP                     GetStringFromID(1029)
#define S_CAUSE_NO_RES_AVAILABLE                GetStringFromID(1030)
#define S_CAUSE_SERVICE_NOT_SUP                 GetStringFromID(1031)
#define S_CAUSE_MAND_IE_INCRT                   GetStringFromID(1032)
#define S_CAUSE_MAND_IE_MISSING                 GetStringFromID(1033)
#define S_CAUSE_OPT_IE_INCRT                    GetStringFromID(1034)
#define S_CAUSE_REQUEST_FULFILLED               GetStringFromID(1035)
#define S_CAUSE_REQ_NOT_FULFILL                 GetStringFromID(1036)
#define S_ERR_LICENSE_EXPIRED                   GetStringFromID(1037)
#define S_ERR_APP_NOT_ACTIVED                   GetStringFromID(1038)
#define S_ERR_INNER_ERROR                       GetStringFromID(1039)
#define S_ERR_UNKNOWN_ERROR                     GetStringFromID(1040)
#define S_ERR_SOCKET_SEND_MSG                   GetStringFromID(1041)

#define S_UNKNOWN_HEART_BEAT_TYPE               GetStringFromID(1050)
#define S_CREATE_HEART_BEAT_LINK                GetStringFromID(1051)
#define S_HEART_BEAT_LINK_BROKEN                GetStringFromID(1052)
#define S_HEART_BEAT_LINK_RECOVERY              GetStringFromID(1053)
#define S_HEART_BEAT_BROKEN                     GetStringFromID(1054)
#define S_HEART_BEAT_RECOVERY                   GetStringFromID(1055)
#define S_START_RES_FAIL                        GetStringFromID(1056)
#define S_STOP_RES_FAIL                         GetStringFromID(1057)
#define S_RESOURCE_FAIL                         GetStringFromID(1058)
#define S_AYSN_RESOURCE_FAIL                    GetStringFromID(1059)
#define S_HARDDISK_FAIL                         GetStringFromID(1060)
#define S_HARDDISK_RECOVERY                     GetStringFromID(1061)
#define S_START_ASYNC_RES_FAIL                  GetStringFromID(1062)
#define S_CLSTR_MANNULOVER_SUCCESS              GetStringFromID(1063)
#define S_CLSTR_MANNULOVER_FAIL                 GetStringFromID(1064)
#define S_ACTIVE_SUCCESS                        GetStringFromID(1065)
#define S_ACTIVE_FAILED                         GetStringFromID(1066)
#define S_DEACTIVE_SUCCESS                      GetStringFromID(1067)
#define S_BEGIN_FAIL_OVER                       GetStringFromID(1068)
#define S_CANNOT_START_FAILOVER                 GetStringFromID(1069)
#define S_FAIL_OVER_SUCCESS                     GetStringFromID(1070)
#define S_FAIL_OVER_FAILED                      GetStringFromID(1071)
#define S_FAIL_OVER_RESP_SUCCESS                GetStringFromID(1072)
#define S_FAIL_OVER_RESP_FAILED                 GetStringFromID(1073)
#define S_AGGRESSIVE_ACTIVE                     GetStringFromID(1074)
#define S_AGGRESSIVE_ACTIVE_SUCCESS             GetStringFromID(1075)
#define S_AGGRESSIVE_ACTIVE_FAIL                GetStringFromID(1076)
#define S_AGGRESSIVE_DEACTIVE                   GetStringFromID(1077)
#define S_AGGRESSIVE_DEACTIVE_COMPLETE          GetStringFromID(1078)
#define S_BEGIN_HANDOVER                        GetStringFromID(1079)
#define S_HANDOVER_SUCCESS                      GetStringFromID(1080)
#define S_HANDOVER_FAIL                         GetStringFromID(1081)
#define S_PEERNODE_RESTART                      GetStringFromID(1082)
#define S_LOCALNODE_FAILURE                     GetStringFromID(1083)
#define S_GET_IP_ADAPTER_FAIL                   GetStringFromID(1084)
#define S_VIRTUAL_IP_EXIST_IN_HOST              GetStringFromID(1085)
#define S_VIRTUAL_IP_EXIST_IN_LAN               GetStringFromID(1086)
#define S_TAKEOVER_IBM_RAID_FAIL                GetStringFromID(1087)
#define S_VOLUME_LABEL_DISMATCH                 GetStringFromID(1088)
/*
    卷资源名(%s)应与该驱动器的卷标(现在为%s)一致
    The name of volume resource(%s) need to match the driver label(now %s)
*/

#define S_ADD_DRIVE_FAIL                        GetStringFromID(1089)
//#define S_VOLUME_VERIFY_FAIL                  GetStringFromID(1090)
#define S_VOLUME_IS_SYSTEM_DRV                  GetStringFromID(1091)
/*
    卷资源(%s)不能配置为操作系统所在的驱动器
    Volume resource(%s) can not be configured as the system driver
*/

#define S_EXECUTE_IPSHAHTO_SUCCESS              GetStringFromID(1092)
#define S_EXECUTE_IPSHAHTO_FAIL                 GetStringFromID(1093)
#define S_OPEN_DELL_RAID_FAIL                   GetStringFromID(1094)
#define S_IBM_RAID_DETECTED                     GetStringFromID(1095)
#define S_DELL_RAID_DETECTED                    GetStringFromID(1096)
#define S_PING_TIME_OUT                         GetStringFromID(1097)
#define S_PING_RECV_NONE_ECHO                   GetStringFromID(1098)
#define S_PING_RECV_OTHER_PACKET                GetStringFromID(1099)
#define S_PING_RECV_BYTE_FROM                   GetStringFromID(1100)
#define S_CANNOT_ACTIVE_ME                      GetStringFromID(1101)
#define S_BEGIN_CHECK_RESOURCE                  GetStringFromID(1102)
#define S_RES_LOCAL_RECOVERY                    GetStringFromID(1103)
#define S_RES_LOCAL_RECOVERY_SUCCESS            GetStringFromID(1104)
#define S_CHECK_RESOURCE_OK                     GetStringFromID(1105)
#define S_LOCAL_STATUS_NOTIFY                   GetStringFromID(1106)
#define S_PEER_STATUS_NOTIFY                    GetStringFromID(1107)
#define S_HEARTBEAT_STATUS_NOTIFY               GetStringFromID(1108)
#define S_RECV_HANDOVER_CMD                     GetStringFromID(1109)
#define S_RECV_HANDOVER_ANS                     GetStringFromID(1110)
#define S_RECV_FAILOVER_CMD                     GetStringFromID(1111)
#define S_RECV_FAILOVER_ANS                     GetStringFromID(1112)
#define S_RECV_SYNC_DATA_FROM_LOCAL             GetStringFromID(1113)
#define S_RECV_SYNC_DATA_FROM_PEER              GetStringFromID(1114)
#define S_SEND_SYNC_DATA                        GetStringFromID(1115)
#define S_RECV_ACTIVE_ANS                       GetStringFromID(1116)
#define S_CANNOT_CREATE_ICON                    GetStringFromID(1117)
#define S_CLUSTER_INIT_FAILED                   GetStringFromID(1118)
#define S_ADD_VIRTUAL_IP_FAILED                 GetStringFromID(1119)
#define S_FAILED_TO_LOCK_VOLUME                 GetStringFromID(1120)
#define S_FAILED_TO_REFRESH_VOLUME              GetStringFromID(1121)
#define S_CANNOT_FIND_VOLUME                    GetStringFromID(1122)
#define S_RES_NUM_EXCEED                        GetStringFromID(1123)
/*
    "资源数(%d)超过上限(%d)"
    "Resources total(%d) is larger than upper limit(%d)"
*/
#define S_RESOURCE_RECOVERY                     GetStringFromID(1124)
/*
    "心跳还未恢复,请稍后再试"
    "heartbeat is not recoveried yet, try later"
*/
#define S_MANSWCH_HEARTBEAT_NO_RECOV            GetStringFromID(1125)
/*
    "对端正在执行不可中断的操作%s,请稍后再试"
    "peer node is executing operation%s, try later"
*/
#define S_MANSWCH_OPERATION_IS_DOING            GetStringFromID(1126)
/*
    "对端处于中间状态%s,请稍后再试"
    "peer node has an instable status%s, try later"
*/
#define S_MANSWCH_STATUS_INSTABLE               GetStringFromID(1127)
/*
    "手工倒换命令发送失败%s,请稍后再试"
    "failed to send SWP command%s, try later"
*/
#define S_MANSWCH_SEND_CMD                      GetStringFromID(1128)


//供硬件检测使用
#define S_ALARM_HDMONITOR_INIT_FAIL             GetStringFromID(1150)
#define S_ALARM_HDMONITOR_CRT_FAIL              GetStringFromID(1151)
#define S_ALARM_HDMONI_OPEN_CVT_FILE_FAIL       GetStringFromID(1152)

//供SNMP告警代理使用
#define S_ALARM_SNMP_AGENT_INIT_FAIL            GetStringFromID(1160)
#define S_ALARM_SNMP_AGENT_CRT_FAIL             GetStringFromID(1161)
#define S_ALARM_GET_SNMP_AGENT_FAIL             GetStringFromID(1162)
#define S_ALARM_START_SNMPAGENTTHD_SUCC         GetStringFromID(1163)

#define S_ALARM_SNMP_SESSION_CRT_FAIL        GetStringFromID(1164)
#define S_ALARM_COMPANY_NAME                 GetStringFromID(1165)
#define S_ALARM_START_SNMPAGENTTHD_FAIL      GetStringFromID(1166)

#define S_ALARM_OPEN_NMS_STATUS_FILE_FAIL    GetStringFromID(1167)
#define S_ALARM_FIND_ERR_NMS_FILE            GetStringFromID(1168)
#define S_ALARM_ADD_NMS_FAIL                 GetStringFromID(1169)
#define S_ALARM_DEL_NMS_FAIL                 GetStringFromID(1170)
#define S_ALARM_MOD_NMS_IP_FAIL              GetStringFromID(1171)
#define S_ALARM_MOD_NMS_PORT_FAIL            GetStringFromID(1172)
#define S_ALARM_NMS_NOT_EXIST                GetStringFromID(1173)
#define S_ALARM_NMS_ALREADY_EXIST            GetStringFromID(1174)
#define S_ALARM_GET_NMS_LIST_FAIL            GetStringFromID(1175)

#define S_ALARM_OPEN_QUERY_STATUS_FILE_FAIL  GetStringFromID(1176)
#define S_ALARM_FIND_ERR_QUERY_COND_FILE     GetStringFromID(1177)
#define S_ALARM_ADD_QUERY_COND_FAIL          GetStringFromID(1178)
#define S_ALARM_MOD_QUERY_COND_FAIL          GetStringFromID(1179)
#define S_ALARM_DEL_QUERY_COND_FAIL          GetStringFromID(1180)
#define S_ALARM_QUERY_COND_NOT_EXIST         GetStringFromID(1181)
#define S_ALARM_QUERY_COND_ALREADY_EXIST     GetStringFromID(1182)

#define S_ALARM_SET_VB_ERR                   GetStringFromID(1183)
#define S_ALARM_SEND_TRAP_FAIL               GetStringFromID(1184)
#define S_ALARM_INVALID_ADRR_INFO            GetStringFromID(1185)
#define S_ALARM_OID_VALUE_IS_NULL            GetStringFromID(1186)
#define S_ALARM_OID_NUMBER_TOO_MANY          GetStringFromID(1187)
#define S_ALARM_SET_PDU_ERR                  GetStringFromID(1188)
#define S_ALARM_REG_SVR_TOO_MANY             GetStringFromID(1189)
#define S_ALARM_QUERY_VAL_INVALID            GetStringFromID(1190)
#define S_ALARM_QUERY_LEN_INVALID            GetStringFromID(1191)

#define S_AP_COTYPE_DISMATCHED          	GetStringFromID(1200)
/*
    接入点号为%d的采集类型(CollectType)必须为1
    Accesspoint(%d)'s CollectType must be 1
*/
#define S_FAIL_TO_OPEN_COTASK           	GetStringFromID(1201)
/*
    采集进程(%s)的采集任务(%d)启动失败
    Fail to open collector task(ID=%d) in process(%s)
*/
#define S_CO_TASK_FAILED                	GetStringFromID(1202)
/*
    采集任务(%d)已永久失效
    Collector task(ID=%d) failed
*/
#define S_FAILED_TO_CREATE_CO_TASK_OBJ  	GetStringFromID(1203)
/*
    创建采集任务列表失败
    Failed to create collect task list
*/
#define S_REOPEN_COTASK                 	GetStringFromID(1204)
/*
    采集任务(%d)异常退出,重启...
    collector task(ID=%d) failed, restarting...
*/
#define S_FAIL_TO_REOPEN_COTASK         	GetStringFromID(1205)
/*
    重新启动采集任务(%d)失败
    Fail to reopen collector task(ID=%d)
*/
#define S_FAILED_TO_INIT_CO_TASK        	GetStringFromID(1206)
/*
    初始化采集任务(%d)失败
    Fail to initiate collector task(ID=%d)
*/
#define S_CVT_FAILED_TO_OPEN_STATUS     	GetStringFromID(1207)
/*
    转换文件池对象打开/创建状态文件(%s)失败
    Convertor pool failed to open/create status file(%s)
*/
#define S_CVT_FAILED_TO_READ_STATUS     	GetStringFromID(1208)
/*
    转换文件池对象读状态文件(%s)失败
    Convertor pool failed to read status file(%s)
*/
#define S_CVT_FAILED_TO_WRITE_STATUS    	GetStringFromID(1209)
/*
    转换文件池对象写状态文件(%s)失败
    Convertor pool failed to write status file(%s)
*/
#define S_COL_TASK_NAME_MUST_BE_CFG     	GetStringFromID(1210)
/*
    %s的采集任务名配置不能为空
    Collect task name must be configurated in %s 
*/
#define S_COL_FILE_SN_MGR               	GetStringFromID(1211)
/*
    采集对象(%s)文件序列号管理器
    Collector(%s)'s file sn manager 
*/
#define S_COL_MUST_CFG_PRI_LINK         	GetStringFromID(1212)
/*
    必须为(%s)配置主链路
    No primary link for %s
*/
#define S_COL_FILE_NOT_FOUND            	GetStringFromID(1213)
/*
    采集对象(%s)采集文件(%s)失败:文件不存在
    Collector(%s) failed to collect file(%s):file not be found
*/
#define S_COL_FILE_DISMATCHED_LENGTH    	GetStringFromID(1214)
/*
    采集对象(%s)采集的文件(%s)长度不一致(远端:%d, 本端:%d)
    Collector(%s) collected file(%s), having dismatched length(remote:%d, local:%d)
*/
#define S_COL_FAILED_TO_MOVE_FILE       	GetStringFromID(1215)
/*
    采集对象(%s)移动文件失败:%s to %s
    Collector(%s) failed to move file:%s to %s
*/
#define S_COL_FAILED_TO_COLLECT_FILE    	GetStringFromID(1216)
/*
    采集对象(%s)采集文件失败:%s to %s
    Collector(%s) failed to collect file:%s to %s
*/
#define S_COL_FAILED_TO_GET_FILE_LIST   	GetStringFromID(1217)
/*
    采集对象(%s)取文件列表失败:无可用的链路
    Collector(%s) failed to get filelist:No available link
*/
#define S_COL_FAILED_TO_GET_FNA         	GetStringFromID(1218)
/*
    采集对象(%s)获取指示文件失败(错误码:%d)
    Collector(%s) failed to get fna file(ErrCode:%d)
*/
#define S_COL_FAILED_TO_OPEN_FNA        	GetStringFromID(1219)
/*
    采集对象(%s)打开本地指示文件(%s)失败(错误码:%d)
    Collector(%s) failed to open fna file(%s)(ErrCode:%d)
*/
#define S_COL_FAILED_TO_MAN_COL_FILE    	GetStringFromID(1220)
/*
    采集对象(%s)手动采集文件(%s)失败:文件不存在
    Collector(%s) failed to manually collect file(%s):file not be found
*/
#define S_COL_AUTO_RECOL_FILE           	GetStringFromID(1221)
/*
    自动重采文件(%s->%s):已成功加入采集队列
    Auto collecting file(%s->%s):have be added in collecting list
*/
#define S_COL_AUTO_RECOL_FILE_FAILED    	GetStringFromID(1222)
/*
    自动重采文件(%s->%s):未能成功加入采集队列
    Auto collecting file(%s->%s):Failed to be added in collecting list
*/
#define S_COL_FILE_CANNOT_BE_RECOL      	GetStringFromID(1223)
/*
    文件(%s)已自动重采过一次,不能再继续重采了
    File(%s) failed to be recollected:this file already have be recollected once
*/
#define S_COLLECTOR_NOT_BE_FOUND        	GetStringFromID(1224)
/*
    未找到指定的采集任务
    Collector be not found
*/
#define S_COL_TO_BE_RUNNING_AND_ACT     	GetStringFromID(1225)
/*
    采集任务状态已设置为启动
    Collector is to be running, and be active
*/
#define S_COL_TO_BE_RUNNING_BUT_NOT_ACT 	GetStringFromID(1226)
/*
    采集任务状态已设置为启动,但磁盘空间不足,采集操作将被禁止
    Collector have be set running, but be inactive, due to insufficient disk space
*/
#define S_COLLECTOR_TO_BE_STOPPED       	GetStringFromID(1227)
/*
    采集任务状态已设置为停止
    Collector is to be stopped
*/
#define S_COL_FAILED_TO_SET_ALARMLIMIT  	GetStringFromID(1228)
/*
    设置告警门限失败
    Fail to set AlarmLimit
*/
#define S_COL_SET_ALARMLIMIT            	GetStringFromID(1229)
/*
    设置告警门限成功
    Succeed in setting AlarmLimit
*/
#define S_COL_FAIL_TO_GET_MOD_INFO      	GetStringFromID(1230)
/*
    获取模块信息失败
    Fail to get module information
*/
#define S_COL_FILE_NAME_ILLEGAL         	GetStringFromID(1231)
/*
    目标文件名不合法
    Target file name is illegal
*/
#define S_COL_FILE_BE_PUT_IN_LIST       	GetStringFromID(1232)
/*
    文件已放置到待采列表中
    The designate file have be added in collecting list
*/
#define S_COL_FILE_HAVE_BE_REMOVED      	GetStringFromID(1233)
/*
    话单文件已被删除,不能再采
    Failed to collect file:the designate file have be removed
*/
#define S_COL_FILE_BE_COLLECTING        	GetStringFromID(1234)
/*
    话单文件正在被采集,不能再采
    Failed to collect file:the designate file is being collected
*/
#define S_COL_FILE_BE_NOT_CLOSE         	GetStringFromID(1235)
/*
    系统不允许采集最新的可能未关闭的话单文件
    Failed to collect file:the designate file is not close
*/
#define S_COL_FILE_BE_NOT_FOUND         	GetStringFromID(1236)
/*
    指定的文件未找到
    Failed to collect file:the designate file is not found
*/
#define S_COL_FILE_STATUS_ERROR         	GetStringFromID(1237)
/*
    文件状态不正确,不能进行手工采集
    Failed to collect file:the designate file have incorrect status
*/
#define S_COL_FILE_CANNOT_BE_MAN_COL    	GetStringFromID(1238)
/*
    该文件不允许被手动采集
    Failed to collect file:unknown reason
*/
#define S_CMU_FAILED_TO_OPEN_STATUS     	GetStringFromID(1239)
/*
    采集模块打开/创建状态文件(%s)失败
    Collect module failed to open/create status file(%s)
*/
#define S_CMU_FAILED_TO_READ_STATUS     	GetStringFromID(1240)
/*
    采集模块读状态文件(%s)失败
    Collect module failed to read status file(%s)
*/
#define S_CMU_FAILED_TO_WRITE_STATUS    	GetStringFromID(1241)
/*
    采集模块写状态文件(%s)失败
    Collect module failed to write status file(%s)
*/
#define S_COL_SCHEDULE_TYPE             	GetStringFromID(1242)
/*
    采集计划类型
    schedule type
*/
#define S_COL_SCHEDULE_INFO             	GetStringFromID(1243)
/*
    采集计划信息
    schedule info
*/
#define S_CNT_RETRY_CYCLE               	GetStringFromID(1244)
/*
    连接重试时间
    collection retry cycle
*/
#define S_RECOL_RETRY_CYCLE             	GetStringFromID(1245)
/*
    采集重试时间
    collection retry cycle
*/
#define S_RECOL_RETRY_TIMES             	GetStringFromID(1246)
/*
    采集重试次数
    collection retry times
*/
#define S_FAILED_TO_QUERY_COL_SCH       	GetStringFromID(1247)
/*
    查询采集计划失败
    Failed to query collection schedule
*/
#define S_COL_SCH_INFO_INVALID          	GetStringFromID(1248)
/*
    采集计划信息格式不正确
    The format of collection schedule info is invalid
*/
#define S_FAILED_TO_SET_COL_SCH         	GetStringFromID(1249)
/*
    设置采集计划失败
    Failed to set collection schedule
*/
#define S_SUC_SET_COL_SCH               	GetStringFromID(1250)
/*
    设置采集计划成功
    Succeed in setting collection schedule
*/
#define S_COL_SCH_INFO                  	GetStringFromID(1251)
/*
    采集计划信息
    collection schedule info
*/
#define S_FAILED_TO_QUERY_ONCE_SCH      	GetStringFromID(1252)
/*
    查询单次采集计划信息失败
    Failed to query once collection schedule info
*/
#define S_COL_SCH_TIME_INVALID          	GetStringFromID(1253)
/*
    采集计划时间格式不正确
    The format of collection schedule time is invalid
*/
#define S_COL_SCH_INTERVAL_SMALL        	GetStringFromID(1254)
/*
    新增的单次采集任务与现有任务的时间间隔过小
    The interval of the new schedule to old schedules is too small
*/
#define S_COL_SCH_OLDER_THAN_NOW        	GetStringFromID(1255)
/*
    新增的单次采集任务不能小于当前时间
    The new schedule could not be older than now
*/
#define S_COL_SCH_UNKNOWN_ERROR         	GetStringFromID(1256)
/*
    未知错误
    Unknown error
*/
#define S_FAILED_TO_OPEN_DIR            	GetStringFromID(1257)
/*
    打开目录(%s)失败(错误码:%d)
    Failed to open direction(%s),(ErrCode:%d)
*/
#define S_FAILED_TO_REMOVE_OUTDATED_DIR 	GetStringFromID(1258)
/*
    删除过期的原始话单目录%s失败
    Failed to remove outdated CDR file direction
*/
#define S_REMOVE_OUTDATED_CDRS          	GetStringFromID(1259)
/*
    删除过期的原始话单文件%s
    Succeed in removing outdated CDR file direction
*/
#define S_FAILED_TO_RMV_OUTDATED_CDRS   	GetStringFromID(1260)
/*
    删除过期的原始话单文件%s失败
    Failed to remove outdated CDR files
*/

#define S_START_SCP_TASK_FAILED             GetStringFromID(1261)
#define S_START_SCP_TASK_SUCCESS            GetStringFromID(1262)
#define S_SCP_TASK_SUCCESS                  GetStringFromID(1263)
#define S_SCP_TASK_FAIL                     GetStringFromID(1264)
#define S_SCP_TASK_CREATE_ROOT              GetStringFromID(1265)
#define S_SCP_MML_CLOSE_ALM_RESP            GetStringFromID(1266)

#define S_INIT_LICENSE_SUCCESS              GetStringFromID(1270)
#define S_INIT_LICENSE_FAILED               GetStringFromID(1271)
#define S_REFRESH_LICENSE_SUCCESS           GetStringFromID(1272)
#define S_REFRESH_LICENSE_FAILED            GetStringFromID(1273)
#define S_INVALID_LICENSE                   GetStringFromID(1274)
#define S_TEMPORARY_LICENSE                 GetStringFromID(1275)


#define S_CANNOT_FIND_MATCHED_PT        	GetStringFromID(1280)
/* 
    未能找到进程号为%d的配置信息
    Can not find the configuration of process(PT=%d)
*/

//从1300到1309禁用

//LST STATITEM命令的标题头
#define s_CDRSTAT_AP_NAME                       GetStringFromID(1330)
#define s_CDRSTAT_STATITEM_NAME                 GetStringFromID(1331)
#define s_CDRSTAT_SUBITEM_NAME                  GetStringFromID(1332)
#define s_CDRSTAT_SUBITEM_EXPRESS               GetStringFromID(1333)

//BRW STATINFO命令的标题头
#define s_CDRSTAT_STAT_CYCLE                    GetStringFromID(1334)
#define S_CDRSTAT_FIELD_NAME                    GetStringFromID(1335)
#define S_CDRSTAT_FIELD_DESC                    GetStringFromID(1336)
#define S_CDRSTAT_FIELD_TYPE                    GetStringFromID(1337)
#define S_CDRSTAT_FIELD_OPERATOR                GetStringFromID(1338)
#define S_CDRSTAT_FIELD_UNIT                    GetStringFromID(1339)
#define S_CDRSTAT_ENUM_FLAG                     GetStringFromID(1340)
#define S_CDRSTAT_ENUM_VALUE                    GetStringFromID(1341)
#define S_CDRSTAT_QUERY_COND                    GetStringFromID(1342)
#define S_NO_CDRSTAT_LICENSE                    GetStringFromID(1343)
#define S_START_CDRSTAT_TASK                    GetStringFromID(1344)

//错误信息
#define S_CDRSTAT_FAIL_GET_CFGINTERFACE         GetStringFromID(1350)

//BRW STATINFO命令的标题头
#define s_CDRSTAT_STAT_CYCLE_YEAR               GetStringFromID(1355)
#define s_CDRSTAT_STAT_CYCLE_MONTH              GetStringFromID(1356)
#define s_CDRSTAT_STAT_CYCLE_DAY                GetStringFromID(1357)

//返回报文的标题
#define S_CDRSTAT_TH_LIST_STAT_CFG              GetStringFromID(1360)
#define S_CDRSTAT_TH_LIST_STAT_RESULT           GetStringFromID(1361)

#define S_PERF_MANAGER_INIT_FAIL                GetStringFromID(1400)
#define S_PERF_ACT_PERF_TASK                    GetStringFromID(1401)
#define S_PERF_SUSPEND_PERF_TASK                GetStringFromID(1402)
#define S_PERF_TASK_NAME                        GetStringFromID(1403)
#define S_PERF_OP_RESULT                        GetStringFromID(1404)
#define S_PERF_ACT_SUCCESS                      GetStringFromID(1405)
#define S_PERF_ACT_FAIL                         GetStringFromID(1406)
#define S_PERF_SUSPEND_SUCCESS                  GetStringFromID(1407)
#define S_PERF_SUSPEND_FAIL                     GetStringFromID(1408)
#define S_PERF_RESULT_NUM                       GetStringFromID(44)
#define S_PERF_REPORT_MORE                      GetStringFromID(144)
#define s_PERF_PAGE_NUM                         GetStringFromID(1409)
#define S_PERF_TSKN                             GetStringFromID(1410)
#define S_PERF_TASK_ID                          GetStringFromID(1411)
#define S_PERF_TIME_INFO                        GetStringFromID(1412)
#define S_PERF_COUNTER_INFO                     GetStringFromID(1413)
#define S_PERF_OBJECT_INFO                      GetStringFromID(1414)
#define S_PERF_TSKDES                           GetStringFromID(1415)
#define S_PERF_CRE_TSK_USER                     GetStringFromID(1416)
#define S_PERF_TSK_RES                          GetStringFromID(1417)
#define S_PERF_MU                               GetStringFromID(1418)
#define S_PERF_RUN                              GetStringFromID(1419)
#define S_PERF_PRD                              GetStringFromID(1420)
#define S_PERF_SD                               GetStringFromID(1421)
#define S_PERF_ED                               GetStringFromID(1422)
#define S_PERF_TSK_STSTUS                       GetStringFromID(1423)
#define S_PERF_SPD                              GetStringFromID(1424)
#define S_PERF_SP1S                             GetStringFromID(1425)
#define S_PERF_SP1E                             GetStringFromID(1426)
#define S_PERF_SP2S                             GetStringFromID(1427)
#define S_PERF_SP2E                             GetStringFromID(1428)	
#define S_PERF_SP3S                             GetStringFromID(1429)
#define S_PERF_SP3E                             GetStringFromID(1430)
#define S_PERF_TSKTP                            GetStringFromID(1431)
#define S_PERF_LPT                              GetStringFromID(1432)
#define S_PERF_TT                               GetStringFromID(1433)
#define S_PERF_TT0                              GetStringFromID(1434)
#define S_PERF_TT1                              GetStringFromID(1435)
#define S_PERF_TT2                              GetStringFromID(1436)
#define S_PERF_STS                              GetStringFromID(1437)
#define S_PERF_SRST                             GetStringFromID(1438)
#define S_PERF_ITM                              GetStringFromID(1439)
#define S_PERF_OIV                              GetStringFromID(1440)
#define S_PERF_FAIL_REASON                      GetStringFromID(1441)
#define S_PERF_CREATE_SUCCESS                   GetStringFromID(1442)
#define S_PERF_DELETE_SUCCESS                   GetStringFromID(1443)
#define S_PERF_TASK_SIMPLE_INFO                 GetStringFromID(1444)
#define S_PERF_TSK_NULL                         GetStringFromID(1445)
#define S_PERF_PERMANENCE                       GetStringFromID(1446)
#define S_PERF_LIMIT                            GetStringFromID(1447)
#define S_PERF_SUN                              GetStringFromID(1448)
#define S_PERF_MON                              GetStringFromID(1449)
#define S_PERF_TUE                              GetStringFromID(1450)
#define S_PERF_WED                              GetStringFromID(1451)
#define S_PERF_THU                              GetStringFromID(1452)
#define S_PERF_FRI                              GetStringFromID(1453)
#define S_PERF_SAT                              GetStringFromID(1454)
#define S_PERF_COUNTER_CPU1                     GetStringFromID(1455)
#define S_PERF_COUNTER_CPU2                     GetStringFromID(1456)
#define S_PERF_COUNTER_PMA                      GetStringFromID(1457)
#define S_PERF_COUNTER_PMT                      GetStringFromID(1458)
#define S_PERF_COUNTER_FSA                      GetStringFromID(1459)
#define S_PERF_COUNTER_FST                      GetStringFromID(1460)
#define S_PERF_COUNTER_BSA                      GetStringFromID(1461)
#define S_PERF_COUNTER_BST                      GetStringFromID(1462)

#define S_PERF_COUNTER_RSCDR                    GetStringFromID(1463)
#define S_PERF_COUNTER_RPSCDR                   GetStringFromID(1464)
#define S_PERF_STATISTIC_START                  GetStringFromID(1465)
#define S_PERF_TSKRES_CONTINUE                  GetStringFromID(1466)
#define S_PERF_TITLE                            GetStringFromID(1467)//"Perf"
#define S_PERF_INSTANCE                         GetStringFromID(1468)
#define S_PERF_HARDWARE_PERF                    GetStringFromID(1469)
#define S_PERF_AP_PERF                          GetStringFromID(1470)
#define S_PERF_STAT_ACT                         GetStringFromID(1471)
#define S_PERF_STAT_SUS                         GetStringFromID(1472)
#define S_PERF_TASKNAME_EXIST                   GetStringFromID(1473)
//by ld 2004-04-26 M2000联调
#define S_PERF_NOTHISTASK                       GetStringFromID(1474)
//end                
//by ldf 2004-05-18
#define S_PERF_COUNTER_RGCDR                    GetStringFromID(1475)
#define S_PERF_COUNTER_RPGCDR                   GetStringFromID(1476)
#define S_PERF_COUNTER_RMCDR                    GetStringFromID(1477)
#define S_PERF_COUNTER_RPMCDR                   GetStringFromID(1478)
#define S_PERF_COUNTER_RSMO                     GetStringFromID(1479)
#define S_PERF_COUNTER_RSMT                     GetStringFromID(1480)
#define S_PERF_COUNTER_RLCSMO                   GetStringFromID(1481)
#define S_PERF_COUNTER_RLCSMT                   GetStringFromID(1482)
#define S_PERF_COUNTER_RLCSNI                   GetStringFromID(1483)
#define S_PERF_MONITOR_PERIOD                   GetStringFromID(1484)
//end
                
#define S_QB_TT_NAME                            GetStringFromID(1500)
#define S_QB_TT_TYPE                            GetStringFromID(1501)
#define S_QB_TT_SIZE                            GetStringFromID(1502)
#define S_QB_TT_MTIME                           GetStringFromID(1503)
#define S_QB_TT_CTIME                           GetStringFromID(1504)
#define S_QB_TT_INVALID_PATH                    GetStringFromID(1505)
#define S_QB_TT_FAILED_READDIR                  GetStringFromID(1506)

#define s_QB_TH_LIST_ROOT_DIR                   GetStringFromID(1550)
#define s_QB_TH_LIST_BILL_FILE                  GetStringFromID(1551)

#define S_PERF_TT_APNAME                        GetStringFromID(1600)
#define S_PERF_TT_LOCALIP                       GetStringFromID(1601)
#define S_PERF_TT_MODELNUM                      GetStringFromID(1602)
#define S_PERF_TT_LINKNUM                       GetStringFromID(1603)
#define S_PERF_TT_STATE                         GetStringFromID(1604)
#define S_PERF_TT_FAILED_GETAPNAME              GetStringFromID(1605)
#define S_PERF_TT_FAILED_GETSTATE               GetStringFromID(1606)
#define s_PERF_TH_LIST_ALL_APNAME               GetStringFromID(1607)
#define s_PERF_TH_LIST_LINK_STATE               GetStringFromID(1608)

//文件组
#define S_CFG_TT_CFGGRP_TIME                    GetStringFromID(1701)
#define S_CFG_TT_CFGGRP_NO                      GetStringFromID(1702)
#define S_CFG_TT_CFGGRP_NAME                    GetStringFromID(1703)
#define S_CFG_TH_CFGFILE                        GetStringFromID(1704)
//所有参数段名
#define S_CFG_TT_PARA_SECTNAME                  GetStringFromID(1705)
#define S_CFG_TH_PARA_SECTNAME                  GetStringFromID(1706)
//参数项
#define S_CFG_TT_PARA_NOITEM                    GetStringFromID(1707)
#define S_CFG_TT_PARA_NAME                      GetStringFromID(1708)
#define S_CFG_TT_PARA_DESC                      GetStringFromID(1709)
#define S_CFG_TT_PARA_TYPE                      GetStringFromID(1710)
#define S_CFG_TT_PARA_DAFAULT                   GetStringFromID(1711)
#define S_CFG_TT_PARA_RELASECT                  GetStringFromID(1712)
#define S_CFG_TT_PARA_MUSTFLAG                  GetStringFromID(1713)
#define S_CFG_TT_PARA_CHNFLAG                   GetStringFromID(1714)
#define S_CFG_TH_PARA_PARAITEM                  GetStringFromID(1716)
//所有配置段名
#define S_CFG_TT_CFG_SECTNAME                   GetStringFromID(1717)
#define S_CFG_TH_CFG_SECTNAME                   GetStringFromID(1718)
//配置项
#define S_CFG_TT_CFG_NOITEM                     GetStringFromID(1719)
#define S_CFG_TT_CFG_NAME                       GetStringFromID(1720)
#define S_CFG_TT_CFG_VALUE                      GetStringFromID(1721)
#define S_CFG_TH_CFG_CFGITEM                    GetStringFromID(1722)
//配置文件设置
#define S_CFG_TT_SETCFGFAIL                     GetStringFromID(1723)
#define S_CFG_TT_SETCFGSUCCESS                  GetStringFromID(1724)
//写Trace文件所用字符串
#define S_CFG_OPEN_CFGFILE_ERR                  GetStringFromID(1730)
#define S_CFG_READ_CFGFILE_ERR                  GetStringFromID(1731)
#define S_CFG_SECTION_REPEAT_ERR                GetStringFromID(1732)
#define S_CFG_SECTION_INVALID                   GetStringFromID(1733)
#define S_CFG_ITEM_INVALID                      GetStringFromID(1734)
#define S_CFG_ITEMNAME_INVALID                  GetStringFromID(1735)
#define S_CFG_ITEMVALUE_INVALID                 GetStringFromID(1736)
#define S_CFG_ITEMNAME_REP_IN_ONESECT           GetStringFromID(1737)
#define S_CFG_SET_OPENFILE_ERROR                GetStringFromID(1738)
#define S_CFG_SET_WRITEFILE_ERROR               GetStringFromID(1739)
#define S_CFG_PRARFILE_SECTINFO_INVALID         GetStringFromID(1742)
#define S_CFG_ITEMINFO_REP_IN_ONERELA           GetStringFromID(1743)
#define S_CFG_PARAFILE_SECTNAME_INVALID         GetStringFromID(1744)
#define S_CFG_PARAFILE_STYLE_INVALID            GetStringFromID(1745)
#define S_CFG_PARAFILE_ITEM_INVALID             GetStringFromID(1746)
#define S_CFG_PARAFILE_RELASECT_TOOLEN          GetStringFromID(1748)
#define S_CFG_PARAFILE_RELASECT_NULL            GetStringFromID(1749)
#define S_CFG_PARAFILE_DATATYPE_INVALID         GetStringFromID(1750)
#define S_CFG_MODULE_STARTUP_FAIL               GetStringFromID(1751)
#define S_CFG_MODULE_STARTUP_SUCCESS            GetStringFromID(1752)
#define S_CFG_SET_ITEMINFO_INVALID              GetStringFromID(1753)
#define S_CFG_SET_ITEMNAME_INVALID              GetStringFromID(1755)
#define S_CFG_SET_CFGINFO_INVALID               GetStringFromID(1757)
#define S_CFG_SET_SECTNAME_NULL                 GetStringFromID(1758)
#define S_CFG_SET_SECTNAME_TOOLONG              GetStringFromID(1759)
#define S_CFG_SET_NO_ITEMNAME                   GetStringFromID(1760)
#define S_CFG_SET_NO_ITEMVALUE                  GetStringFromID(1761)
#define S_CFG_SET_ITEMNAME_NULL                 GetStringFromID(1762)
#define S_CFG_SET_ITEMNAME_TOOLONG              GetStringFromID(1763)
#define S_CFG_SET_ITEMVALUE_NULL                GetStringFromID(1764)
#define S_CFG_SET_ITEMVALUE_TOOLONG             GetStringFromID(1765)

#define S_FMT_OPEN_FILE_FAIL                    GetStringFromID(1801)
#define S_FMT_READ_FILE_FAIL                    GetStringFromID(1802)
#define S_FMT_UNKONWN_APTYPE                    GetStringFromID(1803)
#define S_FMT_ORIGBILL_FMT_NOT_FOUND            GetStringFromID(1804)
#define S_FMT_ORIGBILL_BRW_FMT_NOT_FOUND        GetStringFromID(1805)
#define S_FMT_CHLNAME_NOT_FOUND                 GetStringFromID(1806)
#define S_FMT_CHLID_NOT_FOUND                   GetStringFromID(1807)
#define S_FMT_FINALBILL_FMT_NOT_FOUND           GetStringFromID(1808)
#define S_FMT_FINALBILL_BRW_FMT_NOT_FOUND       GetStringFromID(1809)
#define S_FMT_INVALID_FINALBILL_FMT_NUM         GetStringFromID(1810)
#define S_FMT_FILED_NULL                        GetStringFromID(1811)
#define S_FMT_GET_VALUE_BY_IDX_FAIL             GetStringFromID(1812)
#define S_FMT_VAL_LEN_EXCEED_LIMIT_GET_BY_IDX   GetStringFromID(1813)
#define S_FMT_VAL_LEN_EXCEED_INT_GET_BY_IDX     GetStringFromID(1814)
#define S_FMT_UNKONWN_FIELD_TYPE                GetStringFromID(1815)
#define S_FMT_FUN_CAN_NOT_ALALYSIS              GetStringFromID(1816)
#define S_FMT_FUN_NOT_FOUND                     GetStringFromID(1817)
#define S_FMT_FUN_GET_VALUE_BY_NAME_FAIL        GetStringFromID(1818)
#define S_FMT_INVALID_PARA_TYPE                 GetStringFromID(1819)
#define S_FMT_EXPRESS_FIELD_PARA_NOT_FOUND      GetStringFromID(1820)
#define S_FMT_CONFUN_FIELD_PARA_NOT_FOUND       GetStringFromID(1821)
#define S_FMT_CONFUN_CAN_NOT_ALALYSIS           GetStringFromID(1822)
#define S_FMT_CONFUN_DEFAULT_CAN_NOT_ALALYSIS   GetStringFromID(1823)
#define S_FMT_CONFUN_INVALID_PARA_TYPE          GetStringFromID(1824)
#define S_FMT_CONFUN_NOT_FOUND                  GetStringFromID(1825)
#define S_FMT_CONFUN_PARA_NUM_EXCEED_LIMIT      GetStringFromID(1826)
#define S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL     GetStringFromID(1827)
#define S_FMT_CONVERT_FIELD_LEN_EMPTY           GetStringFromID(1828)
#define S_FMT_CONVERT_FIELD_LEN_EXCEED_BITFLD   GetStringFromID(1829)
#define S_FMT_CONVERT_FAIL                      GetStringFromID(1830)
#define S_FMT_FLDNAME_NOT_FONND_CONVERT_A_FLD   GetStringFromID(1831)
#define S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD GetStringFromID(1832)
#define S_FMT_CONVERT_FAIL_CONVERT_A_FLD        GetStringFromID(1833)
#define S_FMT_CONFUN_ERR_PARA_NUM               GetStringFromID(1834)
#define S_FMT_FMT_NOT_FOUND_INIT_DISCOND        GetStringFromID(1835)
#define S_FMT_LOAD_FMT_CONFIG_FAIL              GetStringFromID(1836)
#define S_FMT_FIELD_NUM_EMPTY                   GetStringFromID(1837)
#define S_FMT_NOT_SEQUENTIAL_BITFLD             GetStringFromID(1838)
#define S_FMT_BITFLD_LEN_OVERFLOW               GetStringFromID(1839)
#define S_FMT_INVALID_FMT_LEN                   GetStringFromID(1840)
#define S_FMT_BILLPROCESS_INVALID_APID          GetStringFromID(1841)
#define S_FMT_BILLPROCESS_INIT_IGWB_FAIL        GetStringFromID(1842)
#define S_FMT_BILLPROCESS_AP_NOT_FOUND          GetStringFromID(1843)
#define S_FMT_IGWB_INIT_FMTLIB_FAIL             GetStringFromID(1844)
#define S_FMT_POSTFIXEXP_INFIXEXP_EMPTY         GetStringFromID(1845)
#define S_FMT_POSTFIXEXP_FUNITEM_NOT_ALALYSIS   GetStringFromID(1846)
#define S_FMT_POSTFIXEXP_FUNITEM_NEED_ENDCHAR   GetStringFromID(1847)
#define S_FMT_POSTFIXEXP_ERR_FUNITEM            GetStringFromID(1848)
#define S_FMT_POSTFIXEXP_STRITEM_NOT_ALALYSIS   GetStringFromID(1849)
#define S_FMT_POSTFIXEXP_STRITEM_NEED_ENDCHAR   GetStringFromID(1850)
#define S_FMT_POSTFIXEXP_INVALID_PARA_TYPE      GetStringFromID(1851)
#define S_FMT_POSTFIXEXP_BRTITEM_NOT_ALALYSIS   GetStringFromID(1852)
#define S_FMT_POSTFIXEXP_EXP_EMPTY              GetStringFromID(1853)
#define S_FMT_POSTFIXEXP_GET_VAR_VALUE_FAIL     GetStringFromID(1854)
#define S_FMT_POSTFIXEXP_GET_FUN_VALUE_FAIL     GetStringFromID(1855)
#define S_FMT_POSTFIXEXP_INVALID_OPR_FOUND      GetStringFromID(1856)
#define S_FMT_POSTFIXEXP_DIV_BY_ZERO            GetStringFromID(1857)
#define S_FMT_POSTFIXEXP_INVALID_OPRATOR        GetStringFromID(1858)
#define S_FMT_CONFUN_INVALID_PARA_NUM           GetStringFromID(1859)

//by ldf 2004-01-05 OM整改 MML使用
#define S_MML_TOKEN                             GetStringFromID(1900)
#define S_MML_CIPHERTXT                         GetStringFromID(1901)
#define S_MML_ADD_M2000_ACCOUNT                 GetStringFromID(1902)
//end
#define S_ALARM_ID                              GetStringFromID(1904)
#define S_ALARM_NAME                            GetStringFromID(1905)
#define S_ALARM_SHIELD                          GetStringFromID(1906)
///////////////////////////////////////////////////////////////////////////////
//GTP'计费接口增加 Z30971 2004/06

/*
**  Failed to register socket,error code returned %d,while connecting BS
**  注册连接计费中心的SOCKET 失败, 返回码: %d
*/

#define S_COMM_CNT_BS_REG_ERR                   GetStringFromID(2000)  

/*
**  Failed to connect BS: ip= "%s", port = "%d"
**	连接计费中心失败: ip= "%s", port= "%d"
*/
#define S_COMM_CONNECT_BS                       GetStringFromID(2001)

/*
**  Failed to open ini file
**	打开配置文件失败
*/
#define S_BS_OPEN_INI_FAIL                      GetStringFromID(2002)
/*
**  Failed to get task%d path
**	获取任务%d路径失败
*/
#define S_BS_GET_TASK_PATH_FAIL                 GetStringFromID(2003)
/*
**  Failed to get config info
**	获取参数配置信息失败
*/
#define S_BS_GET_CONFIG_INFO_FAIL               GetStringFromID(2004)
/*
**  Failed to get status info
**	获取状态信息失败
*/
#define S_BS_GET_STATUS_FAIL                    GetStringFromID(2005)
/*
**  Failed to open directory
**  打开目录失败
*/
#define S_BS_OPEN_DIR_FAIL                      GetStringFromID(2006)
/*
**  The content of status file error
**  状态文件内容出错
*/
#define S_BS_STATUS_FILE_ERR                    GetStringFromID(2007)

/*
**  Failed to open cdr file: %s
**  打开话单文件: %s 失败
*/
#define S_BS_OPEN_CDR_FILE_ERR                  GetStringFromID(2008)

/*
**  Failed to fseek file= %s, offset=%d
**  设置文件(%s)偏移量(%d)失败
*/
#define S_BS_FSEEK_ERR                          GetStringFromID(2009)
/*
**  Failed to allocate memory
**  分配内存失败
*/
#define S_BS_ALLOC_ERR                          GetStringFromID(2010)
/*
**  Failed to read file : %s
**  读取文件: %s 失败
*/
#define S_BS_READ_FILE_ERR                      GetStringFromID(2011)
/*
**  Failed to write status file
**  写状态文件失败
*/
#define S_BS_WRITE_STATUS_ERR                   GetStringFromID(2012)

/*
**  Succeeded in send CDR packet:id=%d,filename=%s,pos=%d
**  发送话单包成功:id=%d,filename=%s,pos=%d
*/
#define S_BS_SEND_SUCC                          GetStringFromID(2013)

#define S_ALARM_SET_ALARMSHEILD_FAIL            GetStringFromID(5005)

#endif  //_RESOURCE_DEFINE_H_

