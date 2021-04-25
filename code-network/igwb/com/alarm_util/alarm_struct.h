#ifndef _ALARM_STRUCT
#define _ALARM_STRUCT

enum  ALARM_RESPONSE
{
    AR_NOT_RECEIVED,        //�澯δ�յ���Ӧ
    AR_RECEIVED             //�澯�յ���Ӧ
};

//SActiveAlarm�кܶ���Ϊö����,����yFuncID��ʾ�澯��澯����,ö����,�ʶ���:
enum ALARM_FUNCTION_ID
{
    AI_COMM_IN_MODULE,      //0:ģ���ͨѶϵͳ
    AI_CONTROL_SYSTEM,      //1:����ϵͳ
    AI_NETWORK_SYSTEM,      //2:����ϵͳ
    AI_COMM_IN_MAIN_NODE,   //3:���ڵ�ͨ��ϵͳ
    AI_SIGNALING_SYSTEM,    //4:����ϵͳ
    AI_TIME_SYSTEM,         //5:ʱ��ϵͳ
    AI_USER_SYSTEM,         //6:�û�ϵͳ
    AI_TRUNK_SYSTEM,        //7:�м�ϵͳ
    AI_TEST_SYSTEM,         //8:����ϵͳ
    AI_POWER_SYSTEM,        //9:��Դϵͳ
    AI_ENVIROMENT_SYSTEM,   //10:�����澯
    AI_TRANSFER_SYSTEM,     //11:����ϵͳ
    AI_DATABASE_SYSTEM,     //12:���ݿ�ϵͳ
    AI_BILLING_SYSTEM,      //13:�����澯
    AI_CALL_IDENTITY,       //14:����ʶ��
    AI_SOFTWARE_RUNNING,    //15:������и澯
    AI_COMM_FROM_TO         //16:ǰ��̨ͨѶ
};

//iGateway bill�ĸ澯�̶���дyFuncIDΪ13,�������澯.
//yAlarmTypeΪ�澯����,������:�¼��澯,���ϸ澯,�ָ��澯,ͬʱΪ��
//MML�ͻ��˵ĸ澯�����ѯ��Ҫ,����"��ʷ�澯"����.����ö������
/*enum ALARM_TYPE
{
    AT_EVENT,           //�¼��澯
    AT_FAULT,           //���ϸ澯
    AT_RESTORE,         //�ָ��澯
    AT_HISTORY,         //��ʷ�澯
    AT_NOT_DEFINE       //δ��������
};*/

//yAlarmLevel��ʾ�澯����,Ϊö����,�ʶ���:
enum ALARM_LEVEL
{
    AL_EMERGENCY,   //0:�����澯
    AL_IMPORTANT,   //1:��Ҫ�澯
    AL_MINOR,       //2:��Ҫ�澯
    AL_WARNING      //3:����澯
};

//ySubSysID��ʾ�澯���ܷ���,ö����,�ʶ���:
enum ALARM_SUBSYSTEM
{
    AS_POWER_SYSTEM,        //0:��Դϵͳ
    AS_ENVIROMENT_SYSTEM,   //1:����ϵͳ
    AS_SIGNALING_SYSTEM,    //2:����ϵͳ
    AS_TRUNK_SYSTEM,        //3:�м�ϵͳ
    AS_HARDWARE_SYSTEM,     //4:Ӳ��ϵͳ
    AS_SOFTWARE_SYSTEM,     //5:���ϵͳ
    AS_RUNNING_SYSTEM       //6:����ϵͳ
};

//iGateway bill�ĸ澯�̶���дySubSysIDΪ5,�����ϵͳ.

//����yResType��ʾ�澯�ָ���־,ȡֵΪö����,��������:
enum ALARM_RECOVER_TYPE
{
    NOT_RECOVER,        //0:δ�ָ�
    AUTO_RECOVER,       //1:�����ָ�
    RESET_RECOVER,      //2:��λ�ָ�
    MANNUAL_RECOVER,    //3:�ֹ��ָ�
    EXPIRE_RECOVER      //4:���ڻָ�
};

typedef struct SAlarmExplain
{
    UINT2   uAlarmID;           //�澯���
    BYTE    yAlarmType;         //�澯����,ALARM_TYPEö����
    char    szAlarmName[64];    //�澯����
    BYTE    yAlarmLevel;        //�澯����,ALARM_LEVELö����
    BYTE    yFuncID;            //�澯�ӹ��ܺ�,ALARM_FUNCTION_IDö����
    BYTE    ySubSysID;          //�澯��ϵͳ��,ALARM_SUBSYSTEMö����
    BYTE    ySendToOMC;         //�����ܱ�־
    BYTE    ySendToBox;         //�͸澯���־
    BYTE    yParaLen;           //�澯�����򳤶�
    char    szDetail[151];      //�澯��ϸ˵��
    char    szFixAdvice[201];   //�澯�޸�����
    char    szParaExp[201];     //�澯��������
    BYTE    yParaNum;           //�澯��������,iGateway billĿǰֻ��һ���澯����
    UINT2   uProductType;       //�澯��������Ʒ����    
} *PAlarmExplain;

typedef struct IGWB_Svc_Export SAlarmDetail
{
    UINT4   uCsn;               //�澯��ˮ��,˳�����,�ָ��澯�����Ӧ��
                                //���ϸ澯uCsn��ͬ
    //by ldf 2004-01-30 OM����
    UINT4	uSyn;		//�澯ͬ����,˳�����,�ָ��澯�͹��ϸ澯��˳�����
    //end

    BYTE    yModuleNO;          //���͸澯������ģ���,����iGateway bill,
                                //��Ϊ0,��Ϊ1
    UINT2   uAlarmID;           //�澯���
    BYTE    yAlarmType;         //�澯����
    BYTE    yFuncID;            //�澯��澯����
    BYTE    yAlarmLevel;        //�澯����
    BYTE    ySubSysID;          //���ܸ澯����

    //�澯���������ڡ�ʱ��
    UINT2   uYear;              //4λ��
    BYTE    yMonth;             //2λ��
    BYTE    yDay;               //2λ��
    BYTE    yHour;              //2λʱ
    BYTE    yMin;               //2λ��
    BYTE    ySec;               //2λ��
    BYTE    yCPUPos;            //A(0) B(1)��ָʾ,����iGateway bill,
                                //��Ϊ0,��Ϊ1
    BYTE    yPID;               //�澯�����Ľ��̱�ʶ
    BYTE    yPara[MAX_ALARM_PARA];  //�澯����
//����Ϊ����
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
    //by ldf 2004-01-08 OM����
    UINT4 GetSYNNO();
    //end

} *PAlarmDetail;

typedef struct SActiveAlarm
{
    UINT4   uCsn;               //�澯��ˮ��,˳�����,�ָ��澯�����Ӧ�Ĺ��ϸ澯uCsn��ͬ
    //by ldf 2004-01-07 OM����
    UINT4	uSyn;		//�澯ͬ����,˳�����,�ָ��澯�͹��ϸ澯��˳�����
    //end

    BYTE    yModuleNO;          //���͸澯������ģ���,����iGateway bill,
                                //��Ϊ0,��Ϊ1
    UINT2   uAlarmID;           //�澯���
    BYTE    yAlarmType;         //�澯����
    BYTE    yFuncID;            //�澯��澯����
    BYTE    yAlarmLevel;        //�澯����
    BYTE    ySubSysID;          //���ܸ澯����

    //�澯����������,ʱ��
    UINT2   uYear;              //4λ��
    BYTE    yMonth;             //2λ��
    BYTE    yDay;               //2λ��
    BYTE    yHour;              //2λʱ
    BYTE    yMin;               //2λ��
    BYTE    ySec;               //2λ��
    BYTE    yCPUPos;            //A(0) B(1)��ָʾ,����iGateway bill,
                                //��Ϊ0,��Ϊ1
    BYTE    yPID;               //�澯�����Ľ��̱�ʶ
    BYTE    yPara[MAX_ALARM_PARA];  //�澯����

    BYTE    ySendTimes;         //�澯���ʹ���
    BYTE    yReceived;          //�澯�յ���־,ΪALARM_RESPONSEö����
    //by ldf 2004-01-30 OM����
    int		yRecoverType; //�����лָ��澯����
    //end

    SAlarmDetail *GetAlarmDetail()
    {
        return (SAlarmDetail *)(&this->uCsn);
    }
} *PActiveAlarm;

typedef struct IGWB_Svc_Export SHistoryAlarmB01
{
    UINT4   uCsn;           //�澯��ˮ��,˳�����,�ָ��澯�����Ӧ�Ĺ��ϸ澯uCsn��ͬ

    BYTE    yModuleNO;      //���͸澯������ģ���,����iGateway bill,��Ϊ0,��Ϊ1
    UINT2   uAlarmID;       //�澯���
    BYTE    yAlarmType;     //�澯����,ALARM_TYPEö����
    BYTE    yFuncID;        //�澯��澯����,ALARM_FUNCTION_IDö����
    BYTE    yAlarmLevel;    //�澯����,ALARM_LEVELö����
    BYTE    ySubSysID;      //���ܸ澯����,ALARM_SUBSYSTEMö����
    
    //�澯����������,ʱ��
    UINT2   uSYear;         //4λ��
    BYTE    ySMonth;        //2λ��
    BYTE    ySDay;          //2λ��
    BYTE    ySHour;         //2λʱ
    BYTE    ySMin;          //2λ��
    BYTE    ySSec;          //2λ��
    BYTE    yCPUPos;        //A(0),B(1)��ָʾ,����iGateway bill,��Ϊ0,��Ϊ1
    
    BYTE    yPID;           //�澯�����Ľ��̱�ʶ
    BYTE    yPara[MAX_ALARM_PARA];  //�澯����
    
    BYTE    yResType;       //�澯�ָ���־,ALARM_RECOVER_TYPEö����
    
    //�澯�ָ�������,ʱ��
    UINT2   uRYear;         //4λ��
    BYTE    yRMonth;        //2λ��
    BYTE    yRDay;          //2λ��
    BYTE    yRHour;         //2λʱ
    BYTE    yRMin;          //2λ��
    BYTE    yRSec;          //2λ��
    //�澯ȷ�ϵĲ���Ա,����,ʱ��
    char    szFAckOptr[78];
    UINT2   uFAckYear;      //4λ��
    BYTE    yFAckMonth;     //2λ��
    BYTE    yFAckDay;       //2λ��
    BYTE    yFAckHour;      //2λʱ
    BYTE    yFAckMin;       //2λ��
    BYTE    yFAckSec;       //2λ��
    
    //�澯�ָ�ȷ�ϵĲ���Ա,����,ʱ��
    char    szRAckOptr[78];
    UINT2   uRAckYear;      //4λ��
    BYTE    yRAckMonth;     //2λ��
    BYTE    yRAckDay;       //2λ��
    BYTE    yRAckHour;      //2λʱ
    BYTE    yRAckMin;       //2λ��
    BYTE    yRAckSec;       //2λ��
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
//b01���ļ�ͷֻ��csn

typedef struct IGWB_Svc_Export SHistoryAlarm
{
    UINT4   uCsn;           //�澯��ˮ��,˳�����,�ָ��澯�����Ӧ�Ĺ��ϸ澯uCsn��ͬ
    //by ldf 2004-01-30 OM����
    UINT4	uSyn;		//�澯ͬ����,˳�����,�ָ��澯�͹��ϸ澯��˳�����
    //end

    BYTE    yModuleNO;      //���͸澯������ģ���,����iGateway bill,��Ϊ0,��Ϊ1
    UINT2   uAlarmID;       //�澯���
    BYTE    yAlarmType;     //�澯����,ALARM_TYPEö����
    BYTE    yFuncID;        //�澯��澯����,ALARM_FUNCTION_IDö����
    BYTE    yAlarmLevel;    //�澯����,ALARM_LEVELö����
    BYTE    ySubSysID;      //���ܸ澯����,ALARM_SUBSYSTEMö����

    //�澯����������,ʱ��
    UINT2   uSYear;         //4λ��
    BYTE    ySMonth;        //2λ��
    BYTE    ySDay;          //2λ��
    BYTE    ySHour;         //2λʱ
    BYTE    ySMin;          //2λ��
    BYTE    ySSec;          //2λ��
    BYTE    yCPUPos;        //A(0),B(1)��ָʾ,����iGateway bill,��Ϊ0,��Ϊ1

    BYTE    yPID;           //�澯�����Ľ��̱�ʶ
    BYTE    yPara[MAX_ALARM_PARA];  //�澯����

    BYTE    yResType;       //�澯�ָ���־,ALARM_RECOVER_TYPEö����

    //�澯�ָ�������,ʱ��
    UINT2   uRYear;         //4λ��
    BYTE    yRMonth;        //2λ��
    BYTE    yRDay;          //2λ��
    BYTE    yRHour;         //2λʱ
    BYTE    yRMin;          //2λ��
    BYTE    yRSec;          //2λ��
    //�澯ȷ�ϵĲ���Ա,����,ʱ��
    char    szFAckOptr[78];
    UINT2   uFAckYear;      //4λ��
    BYTE    yFAckMonth;     //2λ��
    BYTE    yFAckDay;       //2λ��
    BYTE    yFAckHour;      //2λʱ
    BYTE    yFAckMin;       //2λ��
    BYTE    yFAckSec;       //2λ��

    //�澯�ָ�ȷ�ϵĲ���Ա,����,ʱ��
    char    szRAckOptr[78];
    UINT2   uRAckYear;      //4λ��
    BYTE    yRAckMonth;     //2λ��
    BYTE    yRAckDay;       //2λ��
    BYTE    yRAckHour;      //2λʱ
    BYTE    yRAckMin;       //2λ��
    BYTE    yRAckSec;       //2λ��
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
//�澯�ļ�ͷ��ˮ�ų���

//���ݸ澯��¼λ�ü���ø澯��¼���ļ��е�ƫ����
/*#ifdef _OM_MODIFY    
    #define ALARM_OFFSET(uNum) (SIZEOF_ALARMHEAD*3 +SIZEOF_HISTORYALARM*(uNum))
#else
    #define ALARM_OFFSET(uNum) (SIZEOF_ALARMHEAD+SIZEOF_HISTORYALARM*(uNum))
#endif*/
#define ALARM_OFFSET(uNum) (4 + SIZEOF_ALARMHEAD*4 + SIZEOF_HISTORYALARM*(uNum))
//"VER:", �汾�ţ�csn��syn��BaseID

struct SAlarmQueryCond
{
    int nAlarmType;         //�澯����
    int nResFlg[3];         //�ָ���־
    int nAckFlg[3];         //ȷ�ϱ�־
    int nModNum;            //ģ���
    int nStartCsn;          //��ʼ��ˮ��
    int nEndCsn;            //��ֹ��ˮ��
    int nStartAlarmID;      //��ʼ�澯���
    int nEndAlarmID;        //��ֹ�澯���
    int nAlmLvl[5];         //�澯����
    int nASS[11];           //���ܷ���
    tm StartDate;
    tm EndDate;
    int nRetCount;          //���ؼ�¼��
};
enum CONNECTED_MODE
{
    NO_ALARM_BOX,          //0:�����澯��
    CONNECTED_BY_LMT,      //1:ͨ��LMT���Ӹ澯��
    CONNECTED_BY_CG,       //2:ͨ��CG���Ӹ澯��
    CONNECTED_BY_BOTH      //3:���ַ�ʽ�����Ӹ澯��
};


#endif
