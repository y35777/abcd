/*
    文件名:base_type.h
    版本:0.1
    日期:2001-10-11

    本文件中定义了系统中使用全局数据类型.
*/

#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__

#ifdef _PLATFORM_WIN32
    #pragma warning(disable:4786)
    #pragma warning(disable:4786)
    #pragma warning(disable:4503)
    #pragma warning(disable:4511)
    #pragma warning(disable:4663)
    #pragma warning(disable:4244)
    #pragma warning(disable:4100)
    #pragma warning(disable:4127)
    #pragma warning(disable:4706)
    #pragma warning(disable:4701)
#endif

/* 定义系统使用的类型别名 */

#include "type_def.h"

/* 标准C支持头文件 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* STL支持头文件 */

#include <list>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <stack>
//#include <fstream>
#include <algorithm>

/* 包含Windows和Unix系统头文件 */

#ifdef _PLATFORM_WIN32
#else

    /* Unix标准头文件 */

    #include <unistd.h>
    #include <sys/types.h>
    #include <limits.h>
    #include <strings.h>

#endif

/* 包含ACE支持库 */

#include "ace/Process_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Thread.h"
#include "ace/OS.h"
#include "ace/Synch_T.h"
#include "ace/Synch.h"
#include "ace/Thread_Manager.h"
#include "ace/Message_Queue.h"
#include "ace/Reactor.h"
#include "ace/Dirent.h"
#include "ace/Profile_Timer.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"


#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))


#ifdef _PLATFORM_WIN32
    #define SNPRINTF    _snprintf
    #define VSNPRINTF   _vsnprintf
#else
    #define SNPRINTF    snprintf
    #define VSNPRINTF   vsnprintf
#endif

#define STRNCPY(szDst, szSrc, nLen)     strncpy(szDst, szSrc, nLen); \
                                        (szDst)[nLen - 1] = '\0'
#define STRNCAT(szDst, szSrc, nLen)     strncat(szDst, szSrc, nLen - strlen(szDst) - 1); \
                                        (szDst)[nLen - 1] = '\0'

/* 定义系统使用的基本数据结构 */

#define LESS        std::less
#define STRING      std::string
#define MAP         std::map
#define LIST        std::list
#define VECTOR      std::vector
#define SET         std::set
#define PAIR        std::pair
#define STACK       std::stack
#define IOS_BASE    std::ios_base
/* 消息输出的紧急程度 */

#define MSGOUT_LEVEL_NONE       0x00    //Forbid
#define MSGOUT_LEVEL_URGENT     0x01    //紧急
#define MSGOUT_LEVEL_IMPORTANT  0x02    //重要
#define MSGOUT_LEVEL_SIDLY      0x04    //次要
#define MSGOUT_LEVEL_NORMAL     0x08    //一般

/* 性能统计项ID */
#define PERF_CPU1_USAGE         1       //CPU1的利用率			：1
#define PERF_CPU2_USAGE         2       //CPU2的利用率			：2
#define PERF_PHYSICAL_MEM_AVA   3       //物理内存可用空间		：3
#define PERF_PHYSICAL_MEM_TOTAL 4       //物理内存的总空间		：4
#define PERF_FRONT_SAVE_AVA     5       //前存盘可用空间		：5
#define PERF_FRONT_SAVE_TOTAL   6       //前存盘总空间			：6
#define PERF_BACK_SAVE_AVA      7       //后存盘可用空间		：7
#define PERF_BACK_SAVE_TOTAL    8       //后存盘的总空间		：8

//by ldf 2004-06-01 Perf接入CDR统计
#define PERF_TOTAL_SCDR			9		//接收的S-CDR总数		：9
#define PERF_TOTAL_PART_SCDR	10		//接收的部分S-CDR总数	：10
#define PERF_TOTAL_GCDR			11		//接收的G-CDR总数		：11
#define PERF_TOTAL_PART_GCDR	12		//接收的G-CDR总数		：12
#define PERF_TOTAL_MCDR			13		//接收的M-CDR总数		：13
#define PERF_TOTAL_PART_MCDR	14		//接收的部分M-CDR总数	：14
#define PERF_TOTAL_SMOCDR		15		//接收的S-SMO-CDR总数	：15
#define PERF_TOTAL_SMTCDR		16		//接收的S-SMT-CDR总数	：16
#define PERF_TOTAL_LCSMOCDR		17		//接收的LCS-MO-CDR总数	：17
#define PERF_TOTAL_LCSMTCDR		18		//接收的LCS-MT-CDR总数	：18
#define PERF_TOTAL_LCSNICDR		19		//接收的LCS-NI-CDR总数	：19
#define PERF_RECEIVED_CDR       20       //接收话单张数			：20
#define PERF_PROCESSED_CDR      21      //处理话单张数			：21


#define PERF_MANAGER_ID         100     //各进程中的管理模块
#define PERF_HEARTBEAT          101     //心跳数据
#define PERF_NETWORK_ID         200     //网络模块
#define GTP_REPEATDATA_FRAME    201     //GTP'重复话单数据帧数
#define GTP_NORMALDATA_FRAME    202     //GTP'正常话单数据帧数
#define GTP_ALL_FRAME           203     //GTP'所有数据帧数
#define GTP_ALLGSN_CONNECT      204     //与所有GSN的连接数
#define PERF_GTP_QUEUE_SIZE     205     //统计GTP'的队列长度
#define PERF_GTP_BANDWIDTH      206     //统计GTP'已用带宽
#define PERF_GTP_THROUGHPUT     207     //统计GTP'帧的吞吐量

#define PERF_GTP_RESTIME        208     //统计Ga接口平均响应时间
#define PERF_MERGE_QUEUE        209     //统计合并队列

#define SLD_REPEATDATA_FRAME    251     //滑动窗口重复话单数据帧数
#define SLD_NORMALDATA_FRAME    252     //滑动窗口正常话单数据帧数
#define SLD_ALL_FRAME           253     //滑动窗口所有数据帧数
#define GTP_ALLPRI_CONNECT      254     //与所有主MEM/SPC板的连接数
#define GTP_ALLSEC_CONNECT      255     //与所有备MEM/SPC板的连接数

#define PERF_FRONTSAVE_ID       300     //前存盘模块
#define FS_PACKET_COUNT         301
#define FS_PACKET_SIZE          302
#define FS_WRITE_TIME           303
#define FS_FILE_CSN             304
#define FS_LAST_PID             305

#define PERF_BILLPROCESSOR_ID   400     //话单处理模块
#define BP_PACKET_COUNT         401
#define BP_PACKET_SIZE          402
#define BP_MERGE_TIME           403
#define BP_FRONT_COUNT          404
#define BP_PROCESS_TIME         405
#define BP_BACK_COUNT           406

#define PERF_BACKSAVE_ID        3500    //后存盘模块
#define BS_PACKET_COUNT         3501
#define BS_PACKET_SIZE          3502
#define BS_WRITE_TIME           3503
#define BS_FILE_CSN             3504
#define BS_LAST_PID             3505

#define PERF_MONITOR_ID         3600    //监控模块
#define PERF_CLSTRINTERFACE_ID  3700    //双机接口模块
#define PERF_ALARM_ID           3800    //告警模块
#define PERF_BACKUP_ID          3900    //备份模块
#define PERF_MMLSERVER_ID       4000    //MML模块
#define PERF_BILLBROWSER_ID     4100    //话单浏览查询模块
#define PERF_LOG_ID             4200    //日志管理模块
#define PERF_MSGDIST_ID         4300    //消息中转模块
#define PERF_CLSTR_ID           4400    //UNIX下的双机代理模块,
                                        //或WINNT下的双机管理模块
#define PERF_OMCSERVERINTERFACE_ID  4500    //OMCSERVER的接口模块
#define PERF_PERF_ID            4600        //性能模块

/*告警模块需要的定义*/
#define MAX_ALARM_PARA          80          //参数个数
#define MAX_ALARM_CSN           999999999   //告警流水号极大值

/* 定义系统进程号 */

enum PROCESS_TYPE
{
    PT_BROADCAST    = 255,  //广播消息的接收进程号
    PT_UNKNOWN      = 0,    //进程号不确定
    PT_OUTSIDE      = 1,    //计费网关之外的进程
    PT_SELF         = 2,    //代表进程自己
    PT_KERNEL       = 3,    //KERNEL进程
    PT_CLSTR        = 4,    //双机进程
    PT_OM           = 5,    //操作维护进程
    PT_CFGSVR       = 6,    //远程参数配置进程(服务端)
    PT_METERACC     = 7,    //计次表累加进程
    
    //GTP'计费接口增加 Z30971 2004/06
    PT_BS           = 8,    //计费接口进程
    
    PT_AP_BASE      = 10,   //各接入点进程的起始编号
    PT_CO_BASE      = 50    //采集进程的起始编号
};
#define MAX_PT_AP_TYPE      (PT_CO_BASE - 1)
#define MAX_PT_CO_TYPE      (PT_CO_BASE + 50)
/* 定义接入点类型 */
enum  AP_TYPE
{
    AP_128,                 //128或B型机类的接入点
    AP_GSM,                 //GSM类接入点
    AP_STP,                 //STP类接入点
    AP_PS,                  //分组域类型的接入点
    AP_PS_RESERVED,         //分组域类型的接入点
    AP_SOFTX,               //SoftX300类接入点
    AP_WLAN,                //WLAN类接入点
    AP_OTHER = 20           //其他类型接入点
};

/* 定义系统模块号 */

enum MODULE_TYPE
{
    MT_UNKNOWN                  = 0,    //模块号不确定
    MT_MANAGER                  = 1,    //各进程中的管理模块
    MT_NETWORK                  = 2,    //网络模块
    MT_FRONTSAVE                = 3,    //前存盘模块
    MT_BILLPROCESSOR            = 4,    //话单处理模块
    MT_BACKSAVE                 = 5,    //后存盘模块
    MT_MONITOR                  = 6,    //监控模块
    MT_CLSTRINTERFACE           = 7,    //双机接口模块
    MT_ALARM                    = 8,    //告警模块
    MT_BACKUP                   = 9,    //备份模块
    MT_MMLSERVER                = 10,   //MML模块
    MT_BILLBROWSER              = 11,   //话单浏览查询模块
    MT_LOG                      = 12,   //日志管理模块
    MT_MSGDIST                  = 13,   //消息中转模块

    MT_CLSTR                    = 14,   //UNIX下的双机代理模块,
                                        //或WINNT下的双机管理模块

    MT_OMCSERVERINTERFACE       = 15,   //OMCSERVER的接口模块
    MT_PERF                     = 16,   //性能模块
    MT_MMLDEBUG                 = 17,   //MML调试信息输出端
    MT_BILLTASK                 = 18,   //SCP话单恢复模块
    MT_CFGSVR                   = 19,   //远程参数配置模块(服务端)
    MT_CDRSTAT                  = 20,   //话单统计模块
    MT_LICENSE_MANAGER          = 21,   //License Manager
    MT_COLLECTOR                = 22,   //采集模块
    MT_COLAGENT                 = 23,   //采集代理模块
    MT_COLCONVERTOR             = 24,   //采集转换模块
    MT_METERACC                 = 25,    //计次表累加模块 

    ///////////////////////////////////////////////////////////////////////////////
    //GTP'计费接口增加 Z30971 2004/06
    MT_CDRSENDER                = 26    //话单传送模块
};

enum CMD_TYPE
{
    /* Network模块发送的命令码 */

    CMD_QUIT                    = 1,    //退出
    CMD_MSG_OUT                 = 2,    //MessageOut输出
    CMD_PROTOCOL_TRACK          = 3,    //协议跟踪输出
    CMD_INNER_ALARM             = 4,    //告警消息
    CMD_SYNC_DATA               = 5,    //同步数据消息

    CMD_ACTIVE                  = 6,    //激活命令
    CMD_DEACTIVE                = 7,    //去激活命令
    CMD_ANS_DEACTIVE            = 8,    //去激活应答
    CMD_ANS_INIT                = 9,    //初始化应答

    CMD_AUXUPGRADE              = 10,   //辅助升级命令
    CMD_ANS_AUXUPGRADE          = 11,   //辅助升级应答
    CMD_SENDEMPTYPACKET         = 12,   //网络模块已经开始发送空包
    CMD_DISKSPACERECOVERY       = 13,   //磁盘空间恢复
    CMD_AUXUPGRADE_FAIL         = 14,   //辅助升级失败   //by ldf 2002.4.8 对应问题单 D12433

    /* 请求各个接入点进程的网络模块暂停收发话单 */
    CMD_STOP_NETWORK            = 15,

    /* 请求曾停止的网络模块重新开始收发话单 */
    CMD_RESUME_NETWORK          = 16,

    CMD_REG_PROCESS             = 17,   //业务模块注册消息
    CMD_HOST_DATA               = 18,   //主机发来的数据
    CMD_MEM_HANDSHAKE           = 19,   //与MEM板的握手消息

    /* 双机接口模块向监控模块发消息,其退出  */
    CMD_EXIT                    = 20,

    CMD_TRACKMSG_ENABLE         = 21,   //打开消息跟踪
    CMD_TRACKMSG_DISABLE        = 22,   //取消消息跟踪
    CMD_TRACKPROTOCOL_ENABLE    = 23,   //打开协议跟踪
    CMD_TRACKPROTOCOL_DISABLE   = 24,   //取消协议跟踪

    //问题单：SWPD00562，2003年7月23日，Zou Yongsheng修改
    CMD_SET_MSGOUT_LEVEL        = 25,   //设置消息输入级别
    //修改结束

    CMD_PERFITERM_ENABLE        = 30,   //打开性能统计
    CMD_PERFITERM_DISABLE       = 31,   //取消性能统计
    CMD_TRACKMSG                = 32,   //跟踪消息(消息类型)
    CMD_WRITE_LOG               = 33,   //日志消息
    CMD_READY                   = 34,   //模块已初始化成功
    CMD_MODULE_ACTIVE           = 35,   //激活业务模块

    //接点（接入点、采集点）进程向性能模块发送接点名称
    CMD_AP_NAME                 = 36,

    /* Agent模块发送的命令码 */
    CMD_MANUALOVER              = 50,   //手工倒换
    CMD_ANS_ACTIVE              = 51,   //激活应答
    CMD_ASK_FAILOVER            = 52,   //请求自动倒换
    CMD_REQ_STATUS              = 53,   //查询状态
    CMD_ANS_HB_STATUS           = 54,   //应答心跳状态查询

    /* MML SERVER模块发送的命令码 */
    CMD_USER_DOWN               = 69,   //用户非正常掉线
    CMD_MML                     = 70,

    /*  性能模块发送的命令 */
    CMD_SAVE_PERF               = 71,   //
    CMD_ANS_CONNECT_STATUS      = 72,   //

    CMD_GET_APNAME              = 73,   //性能模块向接点进程请求接点名称
    CMD_GET_APLINK_STATE        = 74,   //性能模块向接点进程请求接点链路状态
    CMD_ANS_PROC_STATUS         = 75,   //性能模块和monitor模块的交互


    /*前后存盘、话单处理之间的命令码*/
    CMD_SAVE_PACKET             = 100,  //请求保存原始话单包
    CMD_SYNC_PACKETID           = 101,  //请求同步原始话单包
    CMD_SAVE_SUCCEED            = 102,  //成功保存应答
    CMD_BILL_PROCESS            = 103,  //请求对原始话单包进行计费预处理
    CMD_RESUME                  = 104,  //请求启动恢复
    CMD_RESUME_OVER             = 105,  //通知启动恢复已经完成
    CMD_SAVE_END_BILL           = 106,  //请求保存最终话单包
    CMD_INFORM_STATUS           = 107,  //通知状态
    CMD_ALARM_RESPONSE          = 110,  //告警模块相应命令

    /*通讯层模块处理的命令*/
    CMD_BREAK_OPERATOR          = 130,  //通讯层中断指定用户

    /* 告警模块用到的命令码 */
    CMD_BIN_ALARM               = 150,

    //monitor模块和双机进程的命令码
    CMD_ANS_CLSTR_STATUS        = 160,
    
    /* 采集模块用到的命令码 */
    CMD_CONVERT_BILLFILE        = 180,  //采集模块申请话单转换
    CMD_ANS_CONVERT_BILLFILE    = 181,  //转换模块对采集模块的转换应答
    CMD_REPORT_COLLECTOR_STATUS = 182,  //上报采集任务的状态
    
    CMD_KILL_PROCESS            = 200,   //向Kernel进程发送关闭命令

    /* License Manager用到的命令码*/
    CMD_LICENSE_REQUEST         = 220,  //请求License
    CMD_LICENSE_RESPONSE        = 221,  //应答License

    ///////////////////////////////////////////////////////////////////////////////
    //GTP'计费接口增加 Z30971 2004/06/10
    CMD_BS_DATA                 = 222   //BS发送来的数据
};

/* MML命令码定义 */
enum MML_CMD_CODE
{
    /* MML SERVER (100 - 130) */

    #define MML_MAX_CMDCODE_NETWORK     100
    #define MML_MIN_CMDCODE_NETWORK     100
    #define MML_MAX_CMDCODE_MMLSERVER   130
    #define MML_MIN_CMDCODE_MMLSERVER   101

    //by ldf 2003-03-07 对应问题单D20443
    MML_SHK_HAND        = 100,      //网络握手
    MML_ADD_OP          = 101,      //增加操作员帐号
    MML_RMV_OP          = 102,      //删除操作员帐号
    MML_MOD_OP          = 103,      //修改用户属性
    MML_MOD_PWD         = 104,      //修改操作员的口令
    MML_LST_OP          = 105,      //查询操作员的各种信息
    MML_LGI             = 106,      //操作员登录
    MML_LGO             = 107,      //操作员注销
    MML_SHK             = 108,      //网络握手(对M2000V1增加SHK:;命令)
    MML_SET_OPCG        = 109,      //设置操作员的权限命令组
    MML_SET_OPTM        = 110,      //设置操作员的操作时限
    MML_LST_CMDS        = 111,      //查询当前可执行的命令
    MML_ADD_CCG         = 112,      //增加命令组所包含的命令
    MML_RMV_CCG         = 113,      //删除命令组所包含的命令
    MML_LST_CCG         = 114,      //查询命令组所包含的命令
    MML_QUERY_OP        = 115,      //查询特定用户
    MML_ADD_IP          = 116,      //增加IP限制
    MML_LST_OPTM        = 117,      //查询操作时限
    //by ldf 2004-01-30 OM整改,增加两个命令，用作和M2000 CM （参加OM整改安全篇）
	MML_RVI_OP			= 118,		//M2000请求网元验证操作员身份
	MML_SND_CRG			= 119,		//M2000发送密文到网元
	//end
	MML_DLD_SOFTWARE    = 120,      //下载软件
    //by zkq 2004-07-31,单独增加配置台的登陆命令
    MML_LGI_CFG         = 121,      //配置台的登陆命令  

    /* 日志管理模块 (131 - 150) */

    #define MML_MAX_CMDCODE_LOG     150
    #define MML_MIN_CMDCODE_LOG     131

    MML_LST_LOG         = 131,      //查询日志
    MML_SET_LOG_SDS     = 132,      //设置日志自动删除期限
    MML_RMV_LOG         = 133,      //手工删除日志
    MML_ARC_LOG         = 134,      //手工归档日志
    MML_SET_LOG_ARCTIME = 135,      //设置日志自动归档时间
    MML_GET_LOG_SDS     = 136,      //查询日志自动删除期限
    MML_GET_LOG_ARCTIME = 137,      //查询日志自动归档时间
    MML_EXP_LOG			= 138,		//日志打包
    MML_ULD_LOG			= 139,		//日志上传

    /* 备份模块 (151 - 170) */

    #define MML_MAX_CMDCODE_BACKUP      170
    #define MML_MIN_CMDCODE_BACKUP      151

    MML_LST_AUTOBAK     = 151,      //查询自动备份属性
    MML_SET_AUTOBAK     = 152,      //设置自动备份属性
    MML_BAK_CDR         = 153,      //备份话单文件
    MML_DEL_CDR         = 154,      //删除话单文件
    
	/* om modify add by yangguang ---begin*/

	MML_BKP_CFGFILE    = 155,     //备份配置文件
	MML_ULD_CFGFILE    = 156,     //上载配置文件
	MML_DLD_CFGFILE    = 157,	  //下载配置文件
	MML_RTR_CFGFILE	   = 158,     //恢复配置文件

	/* om modify add by yangguang ---end*/    

    /*
        框架. (171 - 190)
        由MML SERVER先解析,然后向消息中转模块广播.
    */

    #define MML_MAX_CMDCODE_FRAME       190
    #define MML_MIN_CMDCODE_FRAME       171

    //by ldf 2003-03-07 对应问题单D20443
    MML_SET_PTRACK      = 171,      //设置协议跟踪开关
    MML_SET_STRACK      = 172,      //设置业务流程跟踪开关
    MML_SET_DBG         = 173,      //设置调试输出级别
    MML_LST_DBG         = 174,      //查询调试输出级别
    MML_LST_VERSION     = 175,      //查询系统版本信息（长短版本号）
    MML_SET_PERF        = 176,      //设置性能统计开关
    MML_LST_VER         = 177,      //查询系统版本信息（短版本号）
    MML_LST_INTVER      = 178,      //查询系统版本信息（长版本号）
    MML_BRK_OP          = 179,      //强行杀掉操作员

    /* 性能模块 (191 - 200) */

    #define MML_MAX_CMDCODE_PERF        200
    #define MML_MIN_CMDCODE_PERF        191

    MML_LST_PERF        = 192,      //查询性能统计数据
    MML_LST_LINK        = 193,      //查询GSN的IP地址
    MML_CRE_ALM         = 194,     //产生模拟告警
    MML_LST_PROC        = 195,      //
    MML_LST_ALLAP       = 196,      //请求所有接点名称
    MML_LST_APLINK      = 197,      //请求链路状态
    MML_LST_COL_FILE    = 198,      //查询采集性能
	MML_SET_PERF_INV    = 199,      //设置性能统计时间间隔
	MML_LST_PERF_INV    = 200,      //显示性能统计时间间隔

    /* 监控模块,KERNEL进程 (201 - 205) */

    #define MML_MAX_CMDCODE_MONITOR     205
    #define MML_MIN_CMDCODE_MONITOR     201

    MML_AUP             = 201,      //辅助升级
    MML_PRODUCE_CDR     = 202,    

    /* 双机模块,Agent进程 (206 - 210) */

    #define MML_MAX_CMDCODE_CLSTR       210
    #define MML_MIN_CMDCODE_CLSTR       206

    MML_SWP             = 206,      //手工倒换
    MML_RESET           = 207,      //手工重启

    /* SCP话单恢复模块 (221 - 300) */

    #define MML_MAX_CMDCODE_BILLTASK    300
    #define MML_MIN_CMDCODE_BILLTASK    211

    MML_SCP_RECOVERY    = 211,     //SCP话单恢复请求
    MML_SCP_CLOSE_ALM   = 212,     //SCP关闭告警请求

    /* 话单统计模块 (301-320) */

    #define MML_MAX_CMDCODE_CDRSTAT    320
    #define MML_MIN_CMDCODE_CDRSTAT    301

    MML_LST_STATITEM    = 301,     //话单统计浏览配置项请求
    MML_SET_STATITEM    = 302,     //话单统计设置配置项请求
    MML_BRW_STATINFO    = 303,     //话单统计浏览统计信息请求
    MML_LST_STATPARA    = 304,     //话单统计浏览可选配置域请求

    /* 采集模块 (2001 - 2060) */
    #define MML_MAX_CMDCODE_COLLECTOR   500
    #define MML_MIN_CMDCODE_COLLECTOR   401

    MML_LST_COL_INFO    = 401,      //获取指定的采集任务信息
    MML_LST_COL_STATUS  = 402,      //查询采集任务的状态
    MML_SET_COL_STATUS  = 403,      //设置采集任务的状态（激活或去激活）
    MML_SET_ALARM_LIMIT = 404,      //设置告警门限
    MML_LST_MODULE_INFO = 405,      //查询指定模块的计费信息
    MML_GET_FILE        = 406,      //手动采集指定的文件
    MML_LST_COL_SCHDL   = 407,      //查询采集计划
    MML_SET_COL_SCHDL   = 408,      //设置采集计划
    MML_LST_A_COL_SCHDL = 409,      //查询单次采集计划
    MML_SET_A_COL_SCHDL = 410,      //设置单次采集计划
    MML_SET_CCSCFGINFO  = 411,      //设置采集配置信息
    MML_GET_CCSCFGINFO  = 412,      //获取采集配置信息
    MML_DEL_CCSCFGITEM  = 413,      //删除采集配置信息
    MML_MOD_CCSCFGINFO  = 414,      //修改采集配置信息

    /* License Manager (501-550) */
    #define MML_MAX_CMDCODE_LICENSE_MANAGER    550
    #define MML_MIN_CMDCODE_LICENSE_MANAGER    501

    MML_REFRESH_LICENSE = 501,
    //add by zkq 2004-06-19
    MML_SHOW_LICENSE = 502,
    //end

    /* 查询浏览模块 (1001 - 1500) */

    #define MML_MAX_CMDCODE_BROWSER     1500
    #define MML_MIN_CMDCODE_BROWSER     1001

    MML_LST_AP          = 1001,     //查询接入点名称列表
    MML_LST_BILLTYPE    = 1002,     //查询话单类型
    MML_LST_CHL         = 1003,     //查询通道信息
    MML_LST_ORGDATE     = 1004,     //查询原始话单日期信息
    MML_LST_FINALDATE   = 1005,     //查询最终话单日期信息
    MML_LST_FILES       = 1006,     //查询话单文件
    MML_LST_FMT         = 1007,     //查询格式信息
    MML_LST_QUERYPARA   = 1008,     //请求查询条件参数
    MML_BRW_BILL        = 1009,     //浏览话单数据
    MML_LST_BILL        = 1010,     //查询话单数据
    MML_STP_TASK        = 1011,     //结束话单浏览或查询
    MML_SHK_WND         = 1012,     //窗口握手
    MML_GET_PROGRESS    = 1013,     //获得任务进度
    MML_GET_PAGE_DATA   = 1014,     //获得页数据
    MML_CLS_TASK        = 1015,     //关闭话单浏览或查询
    //Added by MMS on 2002.11.21
    //增加采集话单文件的浏览命令
    MML_LST_COLROOT     = 1101,      //请求采集话单文件根路径
    MML_LST_COLFILE     = 1102,      //请求指定目录下的文件及其子目录
    //End

    /* 远程配置模块 (1601 - 1700) */
    #define MML_MAX_CMDCODE_CFGSVR      1700
    #define MML_MIN_CMDCODE_CFGSVR      1601

    MML_LST_FILECLASS   = 1601,     //查询所有配置文件名称
    MML_LST_PARASECT    = 1602,     //查询参数信息文件的所有段名称
    MML_LST_PARAINFO    = 1603,     //查询参数信息文件指定段的所有参数项信息
    MML_LST_CFGSECT     = 1604,     //查询指定配置文件的所有段名称
    MML_LST_CFGINFO     = 1605,     //查询指定配置文件的指定段的所有配置项信息
    MML_SET_CFGINFO     = 1606,     //设置指定配置文件的配置项信息
    MML_OPR_SYS         = 1607,     //配置台请求启动或关闭iGWB程序

    /* 告警模块 (2001 - 2060) */

    #define MML_MAX_CMDCODE_ALARM       2060
    #define MML_MIN_CMDCODE_ALARM       2001
    //by ldf 2004-01-30 OM整改，LST ALMFE 改为了 LST ALMAF，列出当前活动告警（只列出故障告警）
    MML_LST_ALMAF		= 2001,     //查询系统中的当前告警
    //end    
    MML_LST_ALMLOG      = 2002,     //查询历史告警,包括事件
                                    //告警和故障告警
    MML_LST_IGWALMINF   = 2003,     //查询某个告警流水号的
                                    //详细告警信息
    MML_RMV_ALMFLT      = 2005,     //手工恢复系统中的当前告警
    MML_RMV_IGWALMLOG   = 2006,     //删除两个时间点之间的历史告警
    MML_SET_IGWALMACK   = 2007,     //对故障告警进行确认
    MML_SET_IGWEVTACK   = 2008,     //对事件告警进行确认

    MML_SET_ALMDBG      = 2009,     //设置告警调试信息输出级别
    MML_SET_ALMLVL      = 2010,     //设置告警级别
    MML_SET_ALMMSK      = 2011,     //设置告警屏蔽开关
    MML_SET_CLTRPT      = 2012,     //设置告警上报开关
    MML_SET_BOXRPT      = 2013,     //设置告警箱上报开关
    MML_LST_BOXLGT      = 2014,     //查询告警灯状态
    MML_LST_BOXVER      = 2015,     //查询告警灯版本
    MML_CLR_BOXLGT      = 2016,     //人工熄灭告警灯
    MML_STP_BOXSND      = 2017,     //停告警音
    MML_RST_ALMBOX      = 2018,     //重启告警箱
    //by ldf 2004-01-30 OM整改，增加命令，含义为M2000给网元下发同步基准号，网元启动后只接收一次，再次接收时直接返回成功不做任何处理。
    MML_CHK_ALM 		= 2019,		//核对告警基准同步号
    MML_LST_IGWALMFE    = 2020,
    MML_LST_IGWALMLOG   = 2021,
    MML_RMV_IGWALMFLT   = 2022,
    //end  
    MML_LST_ALM      = 2023,
    MML_SET_ALMSHIELD  = 2024,

    //张玉新 2003-01-15 性能模块增加
    #define MML_MAX_CMDCODE_M2000       3060
    #define MML_MIN_CMDCODE_M2000       3001
    MML_CRE_PRFTSK      = 3001,     //创建性能测量任务
    MML_RMV_PRFTSK      = 3002,     //删除性能测量任务
    MML_LST_PRFTSK      = 3003,     //查询性能测量任务
    MML_ACT_PRFTSK      = 3004,     //激活任务
    MML_SPD_PRFTSK      = 3005,     //挂起任务
    MML_LST_TSKRST      = 3006      //查询任务结果
};

/* 定义MML客户端发来的MML命令 */
#define MML_CMD_SHK         "SHK"       //MML客户端与网络层的握手命令

/* 定义系统中使用的错误代码 */
enum  ERROR_CODE
{
    ERR_SUCCESS                 = 0,    //执行成功
    ERR_FAIL                    = -1,   //操作失败
    ERR_HANDLE_INVALID          = -2,   //无效句柄

    /* 以下错误码代表文件操作的错误信息 */

    ERR_FILE_NOT_EXIST          = -3,   //文件不存在

    ERR_OPEN_CFG_FILE           = -7,   //参数配置文件无法打开
    ERR_OPEN_FMT_FILE           = -8,   //格式配置文件无法打开
    ERR_OPEN_AP_FILE            = -9,   //接入点配置文件无法打开
    ERR_OPEN_CHL_FILE           = -10,  //通道配置文件无法打开

    /*以下为告警模块用到的错误码*/

    ERR_OPEN_ALM_CFG_FILE       = -11,  //告警配置文件无法打开
    ERR_OPEN_HISTORY_ALARM      = -12,  //无法打开历史告警文件
    ERR_WRITE_HISTORY_ALARM     = -13,  //写历史告警文件失败
    ERR_READ_HISTORY_ALARM      = -14,  //无法读取历史告警文件
    ERR_FAIL_WARN               = -15,  //无效告警类型
    ERR_FAIL_MALLOC             = -16,  //分配内存失败
    ERR_FILE_NULL               = -17,  //文件指针无效
    ERR_WRITE_HISTORYALARM      = -18,  //写历史告警失败
    ERR_POINTER_NULL            = -19,  //指针为NULL

    /* 以下错误码代表MML SERVER语法解析、权限管理时的错误信息 */

    ERR_SYNTAX_ERROR            = -20,  //语法错误
    ERR_USER_NOT_EXIST          = -21,  //用户不存在
    ERR_USER_ALREADY_EXIST      = -22,  //用户已存在
    ERR_NOT_ENOUGH_RIGHT        = -23,  //权限不够
    ERR_INCORRECT_PASSWD        = -24,  //口令错误
    ERR_CANNOT_DEL_ADMIN        = -25,  //不能删除管理员
    ERR_MULTI_LOGIN             = -26,  //重复登陆
    ERR_TOO_MUCH_USER           = -27,  //用户数过多
    ERR_NOT_LOG_IN              = -28,  //用户未登陆
    ERR_CMD_NOT_DEFINE          = -29,  //未定义的命令
    ERR_PASSWD_LEN_EXCCED       = -30,  //密码长度超过规定值
    /*以下是备份模块用到的错误码*/
    ERR_HAVE_FILES_NOT_BACKUP   = -31,
    ERR_TERMINATED              = -32,


    /* 以下错误码代表内存操作的错误信息 */

    ERR_INVALID_POINTER         = -50,  //指针非法
    ERR_MEM_ALLOCATION_ERROR    = -51,  //内存申请失败

    /* 以下错误码代表内部模块通信的错误信息 */

    ERR_INVALID_CMD_CODE        = -70,  //命令不识别
    ERR_NOT_ENOUGH_PARA         = -71,  //命令参数不足
    ERR_QUEUE_FULL              = -72,  //消息队列已满

    /* 以下错误码代表网络操作中的错误信息 */

    ERR_INVALID_IP_ADDR         = -100, //IP地址非法
    ERR_BIND_PORT               = -101, //无法绑定端口
    ERR_LONG_FRAME              = -102, //消息帧过长
    ERR_PEER_NOT_REACHABLE      = -103, //对端不可达
    ERR_APP_TYPE_UNKNOWN        = -104, //应用类型不可识别
    ERR_SOCKET_NOT_PRESENT      = -105, //连接不可用
    ERR_REACTOR_FULL            = -106, //ACE_Reactor的连接数已满
    ERR_SAME_ROUTER_REGISTERED  = -107, //注册CommRouter时冲突
    ERR_COMMROUTER_NOT_FOUND    = -108, //添加CFrmSocket时,不存在对应类型的CommRouter
    ERR_APTYPE_NOT_MATCH        = -109, //接入点类型不匹配
    ERR_SEND_QUEUE_FULL         = -110, //发送队列已满
    ERR_SOCKET_SEND_MSG         = -111, //发送数据时,SOCKET返回失败
    ERR_MML_SOCKET_LIST_FULL    = -112, //MML客户SOCKET队列已满
    ERR_COMM_TASK_OPEN          = -113, //启动CCommTask线程失败
    ERR_LICENSE_EXPIRED         = -114, //License超期或License的使用数超过授权数
    ERR_APP_NOT_ACTIVED         = -115, //应用层未激活

    /* 以下错误码代表框架在管理业务模块的中错误信息 */

    ERR_SUBMODULE_EXIT          = -130, //业务模块异常退出
    ERR_START_SUBMODULE         = -131, //无法创建业务进程

    /*以下错误码代表存盘模块操作中的错误信息*/

    ERR_WRITE_MAIN_STATUS_FAIL  = -150, //写主状态文件失败
    ERR_WRITE_BAK_STATUS_FAIL   = -151, //写备状态文件失败
    ERR_READ_STATUS_FAIL        = -152, //读状态文件失败
    ERR_READ_BILLFILE_FAIL      = -153, //读话单文件失败
    ERR_WRITE_BILLFILE_FAIL     = -154, //写话单文件失败
    ERR_CREATE_BILLFILE_FAIL    = -155, //创建新话单文件失败
    ERR_PID_TOO_LARGE           = -156, //读取状态记录时要求的包号太大
    ERR_PID_NOT_FOUND           = -157, //读取状态记录时要求的包号没有找到
    ERR_PIDS_NOT_EXIST          = -158, //要求的包号范围在状态文件中都不存在
    ERR_ALREADY_FINISH          = -159, //包号范围内的话单包已经读取完毕了
    ERR_INDEX_INVALID           = -160, //要读取的状态记录的序号不合法
    ERR_FAIL_READ               = -161, //读状态记录失败
    ERR_NO_UNIT_EXIST           = -162, //状态文件为新创建的文件,无状态记录存在
    ERR_CLOSE_BILLFILE_FAIL     = -163, //关闭当前话单文件失败
    
    /*手工备份模块的错误信息*/
    
	#define ERR_START_TOOMANY_TIMES -200
	#define ERR_BAK_FAIL -201
	#define ERR_EVT_EXIT -202
	#define ERR_DEL_FILE -203
	//#define ERR_LICENSE_EXPIRED -204   //license 过期
	#define ERR_CRE_DIR         -205   //创建目标目录失败
	#define ERR_CONNECT_FAILED  -206   //连接目标机器失败 
	#define ERR_SET_DIR         -207   //进入目录目录失败
	#define ERR_TRANS_FILE      -208   //传输话单文件失败    

    /* MML SERVER使用的错误码 */

    ERR_PARA_NOT_EXIST          = -501,  //参数不存在
    ERR_CHILD_PARA_NOT_EXIST    = -502,  //子参数不存在
    ERR_NO_CRU_USER             = -503,  //用户不存在
    ERR_FULL_USER               = -504,  //用户数满
    ERR_NOT_ENOUGHT_RIGHT       = -505,  //用户权限不足
    ERR_PASSWORD_ERR            = -506,  //密码错误
    ERR_PWD_UNMATCH             = -507,  //
    ERR_NOT_LOGIN               = -508,  //用户未登录
    ERR_ACCOUNT_IS_EMPTY        = -509,  //帐号名为空
    ERR_USER_IN_LINE            = -510,  //用户在线
    ERR_USER_LGO_AGAIN          = -511,  //重复LOGOUT
    ERR_PWD_OUTDATED            = -512,  //密码过期
    ERR_EXPIRE_USER				= -514,	 //用户期限过期
    //by ldf 2004-01-06 OM整改
    ERR_ADD_M2000_ACCOUNT		= -513,  //不可添加以M2000开头的用户
    //end


    /* CLUSTER MANAGER使用的错误码 */
    ERR_CLS_STATUS_TRANSFER     = -601, //状态迁移不符合规则
    ERR_FAILURE_STATUS          = -602, //已处于故障状态
    ERR_HEARTBEAT_CREATE        = -603, //创建心跳链路失败
    ERR_COMMTASK_OPEN           = -604, //打开CommTask失败
    ERR_HB_THREAD_SPAWN         = -605, //心跳检测线程启动失败
    ERR_SEND_LENGTH_EXCCEED     = -606, //发送心跳帧超过尺寸上限
    ERR_NO_AVAILABLE_HB         = -607, //不存在可用的心跳链路
    ERR_INIT_CLUSTER_NETWORK    = -608, //初始化双机通讯层失败
    ERR_INIT_CLUSTER_RECOURSE   = -609, //初始化双机资源层失败
    ERR_HEART_BEAT_BROKEN       = -610, //心跳已中断，不能进行FailOver操作
    ERR_SHARED_DISK_FAIL        = -611, //共享阵列故障，不能进行FailOver操作
    ERR_SEND_FORBIDDED          = -612, //程序将要退出，发送操作已初禁止
    ERR_CREATE_RES_OBJECT       = -613, //创建资源对象失败
    ERR_FILE_NOT_PRESENT        = -614, //文件不存在
    ERR_BUFFER_NOT_ENOUGH       = -615, //缓冲区长度不足
    ERR_GET_CHL_NUM             = -616, //获取通道数失败
    ERR_INIT_COM_CLUSTER        = -617, //初始化CComCluster对象失败
    ERR_INIT_SYNC_MGR           = -618, //初始化CSyncManager对象失败
    ERR_INIT_ASYN_OBJ           = -619, //初始化CAsynResKernel对象失败
    ERR_INIT_KERNEL_EXIST       = -620, //初始化时发现KERNEL存在，且杀不掉

    /* Collector使用的错误码 */
    ERR_FILE_BE_DELETED         = -621, //话单文件已被删除，不能再采
    ERR_FILE_BEING_COLLECTED    = -622, //话单文件正在被采集，不能再采
    ERR_NEW_FILE_CANNOT_BE_CO   = -623, //系统不允许采集最新的可能未关闭的话单文件
    ERR_FILE_CANNOT_BE_FOUND    = -624, //指定的文件未找到
    ERR_FILE_STATUS_NOT_CORRECT = -625, //文件状态不正确，不能进行手工采集
    ERR_FILE_NAME_NOT_CORRECT   = -626, //目标文件名不合法
    ERR_FILE_BE_PUT_IN_LIST     = -627,  //指定的文件已列入采集对队列
    
    /*话单统计模块使用的错误码*/
    ERR_STAT_FAIL               = -700,    //统计话单不成功
    ERR_STAT_OPEN_FILE_FAIL     = -701     //话单文件不能打开
};


/* 定义双机倒换原因 */

enum EFAILOVERCAUSE_TYPE
{
    ERROR_WRITEFILE                 = 0, //写文件失败
    ERROR_NOENOUGHDISK_PRIMARY      = 1, //前存盘空间不足
    ERROR_NOENOUGHDISK_SECONDARY    = 2, //后存盘空间不足
    ERROR_PROCESS_EXCEPTION         = 3, //业务进程异常退出
    ERROR_ACTIVE                    = 4, //激活失败
    ERROR_RESOURCE                  = 5, //资源失效
    ERROR_HARDDISK                  = 6  //阵列硬盘坏
};

/* 系统中使用的消息结构. */

struct MSG_INTRA
{
    BYTE    SenderPid;                  //发送模块所在的进程号
    BYTE    SenderMid;                  //发送模块的模块号
    BYTE    ReceiverPid;                //接收模块所在的进程号
    BYTE    ReceiverMid;                //接收模块的模块号
    UINT2   AppType;                    //应用类型
    UINT2   ClientNo;                   //某种类型的MML客户端的编号
    char    UserName[48];               //MML用户名
    UINT4   IPAddr;                     //MML客户端的IP地址
    UINT2   CmdCode;                    //命令码
    UINT4   Len;                        //Data域的长度
    BYTE    Data[1];                    //Data域

    /* 定义消息头长度. */

#define MSG_INTRA_HEADLENGTH  (offsetof(MSG_INTRA,Data))

    /*
        定义单个消息最大长度.
        因为消息可能通过UDP协议发送,限制最大长度为512K.
    */

#define MSG_INTRA_MAXSIZE     512 * 1024

    void* operator new(size_t classSize, size_t DataLength = 1)
    {
        void *ptr = NULL;
        UINT4 uLen = (DataLength > 1) ? DataLength:1;

        UINT4 length = uLen + MSG_INTRA_HEADLENGTH;
        if (length > MSG_INTRA_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new char[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((MSG_INTRA*)ptr)->Len = length - MSG_INTRA_HEADLENGTH;
        }

        return ptr;
    };
};

#define FAILOVERCAUSE_MAXSIZE      MSG_INTRA_MAXSIZE

//定义倒换原因结构
struct SFailOverCause
{
    PROCESS_TYPE PID;           //进程ID
    MODULE_TYPE MID;            //模块ID
    EFAILOVERCAUSE_TYPE Cause;  //倒换原因
    UINT4 Length;               //参数区数据长度
    BYTE Para[1];

#define FAILOVERCAUSE_HEADLENTGH   (offsetof(SFailOverCause,Para))

    //重载new操作符
    void* operator new(size_t classSize, size_t DataLength = 1)
    {
        void *ptr = NULL;
        UINT4 uDataLength = max(DataLength, 1);
        UINT4 length = uDataLength + FAILOVERCAUSE_HEADLENTGH;

        if (length > FAILOVERCAUSE_MAXSIZE)
        {
             return NULL;
        }

        ptr = ::new char[length];
        if (ptr)
        {
            memset(ptr, 0, length);
            ((SFailOverCause *)ptr)->Length = uDataLength;
        }

        return ptr;
    };
};


#define NO_DISK_FULL          0x00    //无磁盘空间不足的异常
#define PRIMARY_DISK_FULL     0x01    //前存盘空间不足
#define SECONDARY_DISK_FULL   0x02    //后存盘空间不足


enum APP_TYPE
{
    APP_MSG = 0,        //消息中转
    APP_MAIN,           //主机应用类型
    APP_MSC_PRIMARY,    //滑动窗口协议的主端口
    APP_MSC_SECOND,     //滑动窗口的备端口
    APP_SOFTX_PRIMARY,  //SoftX3000中的滑动窗口协议的主端口
    APP_SOFTX_SECOND,   //SoftX3000中的滑动窗口协议的备端口
    APP_MML_WARN,       //MML客户端的告警端口
    APP_MML_NORM,       //MML客户端的普通操作维护端口
    APP_MML_DEBUG,      //MML的调试信息输出端口，协议跟
                        //踪、调试消息都通过该端口输出到MML客户端
    APP_MML_WHGUI,      //MML的维护台GUI帧格式端口，暂时不使用
    APP_ALARMBOX,       //告警箱操作
    APP_GTP_TCP,        //TCP方式的GTP'
    APP_GTP_UDP,        //UDP方式的GTP'
	//by ldf 2004-02-12
	APP_MML_PERF,      //MML性能端口
    APP_SYN_CONFIG,       //配置数据同步端口   

    ///////////////////////////////////////////////////////////////////////////////
    //GTP'计费接口增加 Z30971 2004/06/10
    APP_BS_GTP_TCP,    //计费接口TCP 方式的GTP'
    APP_BS_GTP_UDP     //计费接口UDP 方式的GTP' 
};

//定义告警码
enum ALARM_ID
{
    //性能模块告警码
    ALARM_ID_DISK_ROOM           =  1,   //磁盘空间不足

    //前后存盘模块告警码
    ALARM_ID_NO_FETCH_FILE_FAULT =  3,   //长时间未取话单文件的故障告警

    //双机模块告警码
    ALARM_ID_HEARTBEAT           =  5,   //心跳中断告警
    ALARM_ID_FAILOVER            =  7,   //双机倒换告警
    ALARM_ID_PRVNET              =  9,   //私网中断告警
    ALARM_ID_SVCRES              =  11,  //资源失效告警
    ALARM_ID_IPRES               =  13,  //IP资源失效
    ALARM_ID_VOLUME              =  15,  //卷资源失效
    ALARM_ID_DISK                =  17,  //硬盘故障

    //公共告警码
    ALARM_ID_WRITE_FILE_FAILED   =  19,  //写文件失败的事件告警

    //异常话单
    ALARM_ID_ABNORMAL_BILL       =  21,  //异常话单

    ALARM_ID_BACKUP_FAIL         =  23,  //备份文件失败
    //张玉新 2002-12-07
    ALARM_ID_BACKUP_CONNECT_FAIL =  25,   //连接失败
    ALARM_ID_CPU_REACH_LIMIT     =  27,   //CPU 过载
    ALARM_ID_MEM_REACH_LIMIT     =  29,   //Memeory过载(major)    
    ALARM_ID_MML_LOGIN_FAIL_MUCH =  31,   //MML用户登陆失败过多
    ALARM_ID_LICENSE_MANAGEMENT  =  33,   //License管理告警
    ALARM_ID_MANUAL_SWITCHOVER   =  35,   //手工倒换
    ALARM_ID_HARDWARE_ALARM      =  37,   //硬件告警
    ALARM_ID_RESET               =  39,   //系统重启
    ALARM_ID_X25_LINK_FAIL       =  41,   //采集链路故障
    ALARM_ID_FILE_SN_CHECK_FAULT =  42,   //文件序列号校验错误
    ALARM_ID_COLLECT_FILE_FAULT  =  45,   //采集文件失败
    ALARM_ID_COLLECT_FILE_LOST   =  47,   //文件漏采告警
    ALARM_ID_CONVERT_FILE_FAULT  =  49,   //转换文件失败
    ALARM_ID_ALARMBOX_CONNECT_FAIL = 51,    //告警箱断连  
    ALARM_ID_USEREINFO_CHANGED   = 53,    //用户信息更新
    ALARM_ID_DISK_ROOM_MINOR     = 55,    //磁盘空间不足次要告警
    ALARM_ID_MEM_REACH_MID_LIMIT = 57,     //内存过载(minor)
    ALARM_ID_REQUSET_TIMEOUT     =59      //请求帧GSN不响应
   
};

/******** 定义告警参数 *******************/

//公共告警参数
#define ALARM_PARA_NONE           0xFF   //0xFF表示告警无参数

//请求帧GSN不响应
#define ALARM_PATA_GSN_NO_REQUEST       0

//空间不足告警参数
#define ALARM_PARA_FRONT_SAVE     0      //前存盘
#define ALARM_PARA_BACK_SAVE      1      //后存盘
#define ALARM_PARA_BACKUP_DEV     2      //备份介质

//写文件失败告警参数
#define ALARM_PARA_ORIG_BILL      0      //写原始话单文件失败
#define ALARM_PARA_FINAL_BILL     1      //写最终话单文件失败
#define ALARM_PARA_BACKUP         2      //备份文件失败

//双机模块告警参数
#define ALARM_PARA_FRONT_VOLUME   0
#define ALARM_PARA_BACK_VOLUME    1

//过期未备份话单告警参数
#define ALARM_PARA_ORIG_BILL_FILE   0    //原始话单文件
#define ALARM_PARA_FINAL_BILL_FILE  1    //最终话单文件

//异常话单告警参数
#define ALARM_PARA_FRAME_ERR      0     //话单长度出错
#define ALARM_PARA_ENCODING_ERR   1     //话单编码出错
#define ALARM_PARA_DECODING_ERR   2     //话单解码出错
#define ALARM_PARA_TYPE_ERR       3     //话单类型出错
#define ALARM_PARA_DUPLICATED_BILL  4   //重复话单

//license管理
#define ALARM_PARA_LICENSE_INVALID  0    //无效的license
#define ALARM_PARA_LICENSE_EXPIRE   1    //临时license将要过期

//用户信息更新告警
#define ALARM_PARA_ADD_USERINFO       0
#define ALARM_PARA_DEL_USERINFO       1  
#define ALARM_PARA_MOD_USERINFO       2 

//用户非法登录
#define ALARM_PARA_LOGIN_FAIL_TOO_MUCH      0
#define ALARM_PARA_UNAUTHORIZED_LOGIN       1
/******** 告警参数定义完毕 ***************/


/*告警类型定义*/
enum ALARM_TYPE
{
    AT_EVENT,           //事件告警
    AT_FAULT,           //故障告警
    AT_RESTORE,         //恢复告警
    AT_HISTORY,         //历史告警
    AT_NOT_DEFINE       //未定义类型
};

enum ALARM_ADD_RESULT
{
    ADD_OK,             //Add a Alarm to list ok
    ADD_REPEATED,       //Add a repeated alarm to list
    ADD_NOT_MATCH,      //Alarm which to be add to list
                        //is not a standard alarm
    INVALID_ALARM       //Is not a alar
};

/* 告警消息结构 */
typedef struct  _InnerAlarm
{
    BYTE        yAlarmType;     //告警类型,为ALARM_TYPE枚举类型
    UINT2       uAlarmID;       //内部告警ID
    BYTE        yAlarmPara[MAX_ALARM_PARA];     //告警参数(0xFF表示无告警参数)
    _InnerAlarm()
    {
        memset(yAlarmPara, 0xff, sizeof(yAlarmPara));
    };
} SInnerAlarm,  *PInnerAlarm;

struct SLog
{
#define LOG_USR_NAME_LEN 48
#define LOG_OPER_CMD_LEN 20
#define LOG_CMD_DESC_LEN 256
    UINT4 uSerialNo;            //日志序列号，由日志模块自动生成

    BYTE LogType;               //日志类型

    /* 日志消息类型 */

    enum LOG_TYPE               //日志类型枚举
    {
        LOG_SYS     = 0,        //系统日志
        LOG_OPER    = 1         //操作日志
    };

    char szUserName[LOG_USR_NAME_LEN];        //产生日志的用户名
                                //如果是系统内部的日志，
                                //则为发生日志的模块名称

    UINT4 uIPAddr;              //MML客户端的IP地址，
                                //如果是系统内部的日志，则为零

    time_t tLogTime;            //日志产生时间，由日志模块填写

    BYTE OperResult;            //操作结果

    /* 操作结果码 */

    enum  OPER_RESULT           //操作结果枚举
    {
        OPER_SUCCESS    = 0,    //成功
        OPER_FAIL       = 1,    //失败
        OPER_UNKNOWN    = 2     //未知
    };

    char szOperCmd[LOG_OPER_CMD_LEN];         //MML命令名或函数名，系统内部日志则赋空
    char szCmdDesc[LOG_CMD_DESC_LEN];        //MML命令名或函数名的详细描述
};

/* 定义模块的状态常量 */

enum EWKMGR_STATUS
{
    WKMGR_INIT_PENDING,     //Module Manager处于初始化状态,
                            //启动并等待业务进程返回

    WKMGR_READY,            //Module Manager初始化成功,
                            //正在等待Kernel的激活命令

    WKMGR_ACTIVE,           //Module Manager处于激活状态
    WKMGR_FAILURE           //Module Manager处于故障的状态
};

//文件名称节点结构
struct SFileNameNode
{
    time_t tFileTime;  //文件修改时间
    //文件名称，包含话单文件名称，或日期目录和话单文件名称
    char szFileName[MAX_PATH];
    bool operator > (const SFileNameNode& y) const
    {
        if (tFileTime == y.tFileTime)
        {
            return (strcmp(szFileName, y.szFileName) > 0);
        }
        else
        {
            return (tFileTime > y.tFileTime);
        }
    }
    bool operator < (const SFileNameNode& y) const
    {
        if (tFileTime == y.tFileTime)
        {
            return (strcmp(szFileName, y.szFileName) < 0);
        }
        else
        {
            return (tFileTime < y.tFileTime);
        }
    }
};

//License响应
struct SLicenseResp
{
    int     nLicenseCount;      //License授权个数
};

/* 各个模块使用的标准时钟 */

#define ONE_MINUTE_IN_SECOND    60          //1分钟,以秒为单位
#define FIVE_MINUTE_IN_SECOND   (5 * 60)    //5分钟,以秒为单位
#define FIVE_SECOND_IN_MILISEC  (5 * 1000)  //5秒钟,以毫秒为单位
#define ONE_SECOND_IN_MILISEC   1000        //1秒钟,以毫秒为单位
#define ONE_DAY_IN_SECOND       (24 * 3600) //1天,以秒为单位

/* Backup use */
#define CHANNEL0        "default"
struct SLinkStat
{
    char  szLocalAddr[50];    //本地IP地址
    char  szModuleLabel[50];  //模块号，对滑动窗口协议是模块号，对GTP'协议是缺省1
    char  szLinkLabel[50];    //链路号,比如BAM号,IP地址
    BYTE  yStat;              //链接状态
};
#define    ONLINK  0
#define    OFFLINK 1

typedef struct _ProcStatus
{
    char szProcName[32];
    int  nPid;
    char szStatus[16];
} SProcStatus,*PProcStatus;

enum  COLLECT_TYPE
{
    AP_IGWB = AP_OTHER,     //计费网关类型接入点，kernel将启动ap_proc
	AP_S1240,               //S1240J接入点类型，kernel将启动co_proc
	AP_5ESS,                //5ESS接入点类型，kernel将启动co_proc
	AP_EWSD,                //EWSD接入点类型，kernel将启动co_proc
	AP_CC08,                //CC08接如点类型，kernel将启动co_proc
	AP_ZXJ,                 //ZXJ接入点类型，kernel将启动co_proc
	AP_TEXT_IN              //北方智能网接入点类型，kernel将启动co_proc
};

//采集任务的启动状态
enum E_CO_STATUS
{
    CS_ACTIVED     = 0,     //启动状态
    CS_DEACTIVED   = 1,     //未启动状态
    CS_LIC_EXPIRED = 2      //未授权
};

#define ASN1_BUF_SIZE (256 * 1024)
//话单类型的枚举定义
enum BillType
{
    ORIG_BILL = 0,      //原始话单
    FINAL_BILL = 1      //最终话单
};

//added by lucy 2004-6-11 WLAN后存盘动态库名称定义
#define WLAN_BACKSAVE_DLL_NAME "wlan_cmcc_s"
//WLAN 动态库名称的定义
#define WLAN_DLL_NAME "wlan"
//end


//GTP'协议的协议方式
enum GTP_MODE
{
    TCP_MODE = 0,    //TCP协议
    UDP_MODE = 1     //UDP协议
};
#endif

