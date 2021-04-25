#ifndef _PERF_RESOURCE_H_
#define _PERF_RESOURCE_H_

#define PERF_FILE_SUFFIX        "_orig"

//by ldf 2004-04-26 M2000联调
//MML命令错误返回码 CG 只实现2001,7003，对于另外两种请看，返回success
//<Code Type = "CRE" Value = "7001"/> 表示Return Code of CG BAM(NE) job  name already exists
//<Code Type = "DEL" Value = "7003"/> 表示 CG BAM(NE) job not exists.
//<Code Type = "ACT" Value = "7010"/> 表示CG BAM(NE) job has already been suspended.
//<Code Type = "SPD" Value = "7009"/> 表示CG BAM(NE) job has already been resumed.
enum MML_PERF_FAILCAUSE
{
    ERROR_TASKNAME_EXIST        = 7001, //任务名已经存在 "task name exist"
    ERROR_NOTHISTASK            = 7003, //任务不存在“no this task”
};
//end

//任务类型
#define TASK_PERMANENCE 0
#define TASL_TRANSIENT 1
//任务状态
#define TASK_NORMAL 1
#define TASK_SUSPEND 0
//时间类型
//按天循环
#define TT_DAY 0  
//按周循环	
#define TT_WEEK 1
//按月循环	
#define TT_MONTH 2
//各个域的长度,注意在Sun下的snprintf与Windows的不一样
//在Sun下1个字符的长度会造成输出为"";
#define TSKN_LEN    64
#define TSKDES_LEN  48
#define USER_LEN    48
#define MU_LEN      16
#define RUN_LEN     2
#define PRD_LEN     4
#define SD_LEN      8
#define SPD_LEN     2
#define SP_LEN      5
#define TSKTP_LEN   2
#define LPT_LEN     4
#define TT_LEN      2
#define STS_LEN     90
#define SRST_LEN    2
//by ldf 2004-05-19 此处的47为 统计项加&的个数，比如“1&2&3”
#define ITM_LEN     47
#define OIV_LEN     6
#define TSK_REC_LEN (TSKN_LEN + TSKDES_LEN + MU_LEN \
                    + RUN_LEN + PRD_LEN + SD_LEN + SPD_LEN \
                    + SP_LEN * 6 + TSKTP_LEN + LPT_LEN \
                    + TT_LEN + STS_LEN + SRST_LEN \
                    + ITM_LEN + OIV_LEN)
#define COUNTER_LEN 20
#define SETIME_LEN  8
//by ldf 2003-12
#ifdef _PLATFORM_WIN32 
#define S_PERF_M2000  "d:\\other\\perf_M2000"
#else
#define S_PERF_M2000  "/var/other/perf_M2000"
#endif

#endif

