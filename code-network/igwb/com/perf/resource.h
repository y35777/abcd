#ifndef _PERF_RESOURCE_H_
#define _PERF_RESOURCE_H_

#define PERF_FILE_SUFFIX        "_orig"

//by ldf 2004-04-26 M2000����
//MML������󷵻��� CG ֻʵ��2001,7003���������������뿴������success
//<Code Type = "CRE" Value = "7001"/> ��ʾReturn Code of CG BAM(NE) job  name already exists
//<Code Type = "DEL" Value = "7003"/> ��ʾ CG BAM(NE) job not exists.
//<Code Type = "ACT" Value = "7010"/> ��ʾCG BAM(NE) job has already been suspended.
//<Code Type = "SPD" Value = "7009"/> ��ʾCG BAM(NE) job has already been resumed.
enum MML_PERF_FAILCAUSE
{
    ERROR_TASKNAME_EXIST        = 7001, //�������Ѿ����� "task name exist"
    ERROR_NOTHISTASK            = 7003, //���񲻴��ڡ�no this task��
};
//end

//��������
#define TASK_PERMANENCE 0
#define TASL_TRANSIENT 1
//����״̬
#define TASK_NORMAL 1
#define TASK_SUSPEND 0
//ʱ������
//����ѭ��
#define TT_DAY 0  
//����ѭ��	
#define TT_WEEK 1
//����ѭ��	
#define TT_MONTH 2
//������ĳ���,ע����Sun�µ�snprintf��Windows�Ĳ�һ��
//��Sun��1���ַ��ĳ��Ȼ�������Ϊ"";
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
//by ldf 2004-05-19 �˴���47Ϊ ͳ�����&�ĸ��������硰1&2&3��
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

