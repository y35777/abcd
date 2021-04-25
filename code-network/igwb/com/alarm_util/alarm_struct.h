#ifndef _ALARM_STRUCT
#define _ALARM_STRUCT

enum  ALARM_RESPONSE
{
    AR_NOT_RECEIVED,        //告警未收到响应
    AR_RECEIVED             //告警收到响应
};

//SActiveAlarm中很多域为枚举型,其中yFuncID表示告警箱告警分类,枚举型,故定义:
enum ALARM_FUNCTION_ID
{
    AI_COMM_IN_MODULE,      //0:模块间通讯系统
    AI_CONTROL_SYSTEM,      //1:控制系统
    AI_NETWORK_SYSTEM,      //2:网络系统
    AI_COMM_IN_MAIN_NODE,   //3:主节点通信系统
    AI_SIGNALING_SYSTEM,    //4:信令系统
    AI_TIME_SYSTEM,         //5:时钟系统
    AI_USER_SYSTEM,         //6:用户系统
    AI_TRUNK_SYSTEM,        //7:中继系统
    AI_TEST_SYSTEM,         //8:测试系统
    AI_POWER_SYSTEM,        //9:电源系统
    AI_ENVIROMENT_SYSTEM,   //10:环境告警
    AI_TRANSFER_SYSTEM,     //11:传输系统
    AI_DATABASE_SYSTEM,     //12:数据库系统
    AI_BILLING_SYSTEM,      //13:话单告警
    AI_CALL_IDENTITY,       //14:呼叫识别
    AI_SOFTWARE_RUNNING,    //15:软件运行告警
    AI_COMM_FROM_TO         //16:前后台通讯
};

//iGateway bill的告警固定填写yFuncID为13,即话单告警.
//yAlarmType为告警类型,分三类:事件告警,故障告警,恢复告警,同时为了
//MML客户端的告警浏览查询需要,增加"历史告警"类型.定义枚举类型
/*enum ALARM_TYPE
{
    AT_EVENT,           //事件告警
    AT_FAULT,           //故障告警
    AT_RESTORE,         //恢复告警
    AT_HISTORY,         //历史告警
    AT_NOT_DEFINE       //未定义类型
};*/

//yAlarmLevel表示告警级别,为枚举型,故定义:
enum ALARM_LEVEL
{
    AL_EMERGENCY,   //0:紧急告警
    AL_IMPORTANT,   //1:重要告警
    AL_MINOR,       //2:次要告警
    AL_WARNING      //3:警告告警
};

//ySubSysID表示告警网管分类,枚举型,故定义:
enum ALARM_SUBSYSTEM
{
    AS_POWER_SYSTEM,        //0:电源系统
    AS_ENVIROMENT_SYSTEM,   //1:环境系统
    AS_SIGNALING_SYSTEM,    //2:信令系统
    AS_TRUNK_SYSTEM,        //3:中继系统
    AS_HARDWARE_SYSTEM,     //4:硬件系统
    AS_SOFTWARE_SYSTEM,     //5:软件系统
    AS_RUNNING_SYSTEM       //6:运行系统
};

//iGateway bill的告警固定填写ySubSysID为5,即软件系统.

//其中yResType表示告警恢复标志,取值为枚举型,定义如下:
enum ALARM_RECOVER_TYPE
{
    NOT_RECOVER,        //0:未恢复
    AUTO_RECOVER,       //1:正常恢复
    RESET_RECOVER,      //2:复位恢复
    MANNUAL_RECOVER,    //3:手工恢复
    EXPIRE_RECOVER      //4:过期恢复
};

typedef struct SAlarmExplain
{
    UINT2   uAlarmID;           //告警编号
    BYTE    yAlarmType;         //告警类型,ALARM_TYPE枚举型
    char    szAlarmName[64];    //告警名称
    BYTE    yAlarmLevel;        //告警级别,ALARM_LEVEL枚举型
    BYTE    yFuncID;            //告警子功能号,ALARM_FUNCTION_ID枚举型
    BYTE    ySubSysID;          //告警子系统号,ALARM_SUBSYSTEM枚举型
    BYTE    ySendToOMC;         //送网管标志
    BYTE    ySendToBox;         //送告警箱标志
    BYTE    yParaLen;           //告警参数域长度
    char    szDetail[151];      //告警详细说明
    char    szFixAdvice[201];   //告警修复建议
    char    szParaExp[201];     //告警参数解释
    BYTE    yParaNum;           //告警参数个数,iGateway bill目前只有一个告警参数
    UINT2   uProductType;       //告警项所属产品类型    
} *PAlarmExplain;

typedef struct IGWB_Svc_Export SAlarmDetail
{
    UINT4   uCsn;               //告警流水号,顺序分配,恢复告警与其对应的
                                //故障告警uCsn相同
    //by ldf 2004-01-30 OM整改
    UINT4	uSyn;		//告警同步号,顺序分配,恢复告警和故障告警都顺序分配
    //end

    BYTE    yModuleNO;          //发送告警的主机模块号,对于iGateway bill,
                                //主为0,备为1
    UINT2   uAlarmID;           //告警编号
    BYTE    yAlarmType;         //告警类型
    BYTE    yFuncID;            //告警箱告警分类
    BYTE    yAlarmLevel;        //告警级别
    BYTE    ySubSysID;          //网管告警分类

    //告警产生的日期、时间
    UINT2   uYear;              //4位年
    BYTE    yMonth;             //2位月
    BYTE    yDay;               //2位日
    BYTE    yHour;              //2位时
    BYTE    yMin;               //2位分
    BYTE    ySec;               //2位秒
    BYTE    yCPUPos;            //A(0) B(1)机指示,对于iGateway bill,
                                //主为0,备为1
    BYTE    yPID;               //告警产生的进程标识
    BYTE    yPara[MAX_ALARM_PARA];  //告警参数
//以下为函数
public:
    STRING GetAlarmTime();
    STRING GetAlarmSubSysID();
    STRING GetAlarmID();
    STRING GetAlarmSource();
    STRING GetAlarmLevel();
    STRING GetAlarmType();
    STRING GetFixExplain(CAlarmConfig *pConfig);
    STRING GetParaExplain(CAlarmConfig *pConfig);
    STRING GetExplain(CAlarmConfig *pConfig);
    STRING GetAlarmParas();
    STRING GetAlarmName(CAlarmConfig *pConfig);
    STRING GetModule();
    STRING GetAlarmFuncID();
    //by ldf 2004-01-08 OM整改
    UINT4 GetSYNNO();
    //end

} *PAlarmDetail;

typedef struct SActiveAlarm
{
    UINT4   uCsn;               //告警流水号,顺序分配,恢复告警与其对应的故障告警uCsn相同
    //by ldf 2004-01-07 OM整改
    UINT4	uSyn;		//告警同步号,顺序分配,恢复告警和故障告警都顺序分配
    //end

    BYTE    yModuleNO;          //发送告警的主机模块号,对于iGateway bill,
                                //主为0,备为1
    UINT2   uAlarmID;           //告警编号
    BYTE    yAlarmType;         //告警类型
    BYTE    yFuncID;            //告警箱告警分类
    BYTE    yAlarmLevel;        //告警级别
    BYTE    ySubSysID;          //网管告警分类

    //告警产生的日期,时间
    UINT2   uYear;              //4位年
    BYTE    yMonth;             //2位月
    BYTE    yDay;               //2位日
    BYTE    yHour;              //2位时
    BYTE    yMin;               //2位分
    BYTE    ySec;               //2位秒
    BYTE    yCPUPos;            //A(0) B(1)机指示,对于iGateway bill,
                                //主为0,备为1
    BYTE    yPID;               //告警产生的进程标识
    BYTE    yPara[MAX_ALARM_PARA];  //告警参数

    BYTE    ySendTimes;         //告警发送次数
    BYTE    yReceived;          //告警收到标志,为ALARM_RESPONSE枚举型
    //by ldf 2004-01-30 OM整改
    int		yRecoverType; //队列中恢复告警类型
    //end

    SAlarmDetail *GetAlarmDetail()
    {
        return (SAlarmDetail *)(&this->uCsn);
    }
} *PActiveAlarm;

typedef struct IGWB_Svc_Export SHistoryAlarmB01
{
    UINT4   uCsn;           //告警流水号,顺序分配,恢复告警与其对应的故障告警uCsn相同

    BYTE    yModuleNO;      //发送告警的主机模块号,对于iGateway bill,主为0,备为1
    UINT2   uAlarmID;       //告警编号
    BYTE    yAlarmType;     //告警类型,ALARM_TYPE枚举型
    BYTE    yFuncID;        //告警箱告警分类,ALARM_FUNCTION_ID枚举型
    BYTE    yAlarmLevel;    //告警级别,ALARM_LEVEL枚举型
    BYTE    ySubSysID;      //网管告警分类,ALARM_SUBSYSTEM枚举型
    
    //告警产生的日期,时间
    UINT2   uSYear;         //4位年
    BYTE    ySMonth;        //2位月
    BYTE    ySDay;          //2位日
    BYTE    ySHour;         //2位时
    BYTE    ySMin;          //2位分
    BYTE    ySSec;          //2位秒
    BYTE    yCPUPos;        //A(0),B(1)机指示,对于iGateway bill,主为0,备为1
    
    BYTE    yPID;           //告警产生的进程标识
    BYTE    yPara[MAX_ALARM_PARA];  //告警参数
    
    BYTE    yResType;       //告警恢复标志,ALARM_RECOVER_TYPE枚举型
    
    //告警恢复的日期,时间
    UINT2   uRYear;         //4位年
    BYTE    yRMonth;        //2位月
    BYTE    yRDay;          //2位日
    BYTE    yRHour;         //2位时
    BYTE    yRMin;          //2位分
    BYTE    yRSec;          //2位秒
    //告警确认的操作员,日期,时间
    char    szFAckOptr[78];
    UINT2   uFAckYear;      //4位年
    BYTE    yFAckMonth;     //2位月
    BYTE    yFAckDay;       //2位日
    BYTE    yFAckHour;      //2位时
    BYTE    yFAckMin;       //2位分
    BYTE    yFAckSec;       //2位秒
    
    //告警恢复确认的操作员,日期,时间
    char    szRAckOptr[78];
    UINT2   uRAckYear;      //4位年
    BYTE    yRAckMonth;     //2位月
    BYTE    yRAckDay;       //2位日
    BYTE    yRAckHour;      //2位时
    BYTE    yRAckMin;       //2位分
    BYTE    yRAckSec;       //2位秒
public:
    STRING GetRevTime();
    STRING GetRecoverAckStatus();
    STRING GetAckStatus();
    STRING GetRecoverAckTime();
    STRING GetRAckOptr();
    STRING GetFAckOptr();
    STRING GetFAckTime();
    SAlarmDetail * GetAlarmDetail();
} *PHistoryAlarmB01;
//b01的文件头只有csn

typedef struct IGWB_Svc_Export SHistoryAlarm
{
    UINT4   uCsn;           //告警流水号,顺序分配,恢复告警与其对应的故障告警uCsn相同
    //by ldf 2004-01-30 OM整改
    UINT4	uSyn;		//告警同步号,顺序分配,恢复告警和故障告警都顺序分配
    //end

    BYTE    yModuleNO;      //发送告警的主机模块号,对于iGateway bill,主为0,备为1
    UINT2   uAlarmID;       //告警编号
    BYTE    yAlarmType;     //告警类型,ALARM_TYPE枚举型
    BYTE    yFuncID;        //告警箱告警分类,ALARM_FUNCTION_ID枚举型
    BYTE    yAlarmLevel;    //告警级别,ALARM_LEVEL枚举型
    BYTE    ySubSysID;      //网管告警分类,ALARM_SUBSYSTEM枚举型

    //告警产生的日期,时间
    UINT2   uSYear;         //4位年
    BYTE    ySMonth;        //2位月
    BYTE    ySDay;          //2位日
    BYTE    ySHour;         //2位时
    BYTE    ySMin;          //2位分
    BYTE    ySSec;          //2位秒
    BYTE    yCPUPos;        //A(0),B(1)机指示,对于iGateway bill,主为0,备为1

    BYTE    yPID;           //告警产生的进程标识
    BYTE    yPara[MAX_ALARM_PARA];  //告警参数

    BYTE    yResType;       //告警恢复标志,ALARM_RECOVER_TYPE枚举型

    //告警恢复的日期,时间
    UINT2   uRYear;         //4位年
    BYTE    yRMonth;        //2位月
    BYTE    yRDay;          //2位日
    BYTE    yRHour;         //2位时
    BYTE    yRMin;          //2位分
    BYTE    yRSec;          //2位秒
    //告警确认的操作员,日期,时间
    char    szFAckOptr[78];
    UINT2   uFAckYear;      //4位年
    BYTE    yFAckMonth;     //2位月
    BYTE    yFAckDay;       //2位日
    BYTE    yFAckHour;      //2位时
    BYTE    yFAckMin;       //2位分
    BYTE    yFAckSec;       //2位秒

    //告警恢复确认的操作员,日期,时间
    char    szRAckOptr[78];
    UINT2   uRAckYear;      //4位年
    BYTE    yRAckMonth;     //2位月
    BYTE    yRAckDay;       //2位日
    BYTE    yRAckHour;      //2位时
    BYTE    yRAckMin;       //2位分
    BYTE    yRAckSec;       //2位秒
public:
    STRING GetRevTime();
    STRING GetRecoverAckStatus();
    STRING GetAckStatus();
    STRING GetRecoverAckTime();
    STRING GetRAckOptr();
    STRING GetFAckOptr();
    STRING GetFAckTime();
    SAlarmDetail * GetAlarmDetail();
} *PHistoryAlarm;

#define SIZEOF_HISTORYALARM sizeof(SHistoryAlarm)

#define SIZEOF_ALARMHEAD sizeof(UINT4)
//告警文件头流水号长度

//根据告警记录位置计算该告警记录在文件中的偏移量
/*#ifdef _OM_MODIFY    
    #define ALARM_OFFSET(uNum) (SIZEOF_ALARMHEAD*3 +SIZEOF_HISTORYALARM*(uNum))
#else
    #define ALARM_OFFSET(uNum) (SIZEOF_ALARMHEAD+SIZEOF_HISTORYALARM*(uNum))
#endif*/
#define ALARM_OFFSET(uNum) (4 + SIZEOF_ALARMHEAD*4 + SIZEOF_HISTORYALARM*(uNum))
//"VER:", 版本号，csn，syn，BaseID

struct SAlarmQueryCond
{
    int nAlarmType;         //告警类型
    int nResFlg[3];         //恢复标志
    int nAckFlg[3];         //确认标志
    int nModNum;            //模块号
    int nStartCsn;          //起始流水号
    int nEndCsn;            //终止流水号
    int nStartAlarmID;      //起始告警编号
    int nEndAlarmID;        //终止告警编号
    int nAlmLvl[5];         //告警级别
    int nASS[11];           //网管分类
    tm StartDate;
    tm EndDate;
    int nRetCount;          //返回记录数
};
enum CONNECTED_MODE
{
    NO_ALARM_BOX,          //0:不连告警箱
    CONNECTED_BY_LMT,      //1:通过LMT连接告警箱
    CONNECTED_BY_CG,       //2:通过CG连接告警箱
    CONNECTED_BY_BOTH      //3:两种方式都连接告警箱
};


#endif
