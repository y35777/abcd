/**************************************************************
**文件名：      sendalarm.h             
**描述信息：    告警帧的基本处理函数
**版本说明：    V100R001
**版权信息：    华为技术有限公司
**作者：        郑利锋（34756）
**创建时间：    2002.11.15
**主要类列表：  CSunAlarm    SNMP告警处理类
**主要函数列表：见以下定义
**修改记录：    
**  1、作者：郭小金 
**     日期：2003-12-24
**     修改说明：舍弃原来有关接收SNMP的部分，
**               将接收SNMP TRAP部分更改成查寻SUN计算机系统日志形式
**
**
***************************************************************/

#ifndef _SUN_ALARM_INTERFACE_H_
#define _SUN_ALARM_INTERFACE_H_

#include "hardware_alarm.h"


const int PATH_MAX_LEN          = 1024;       // 存储路径最大字节数
const int INSPECT_TIME          = 60;         // 读系统日志间隔时间(秒)
const int KEYWORD_MAX_LEN       = 50;         // 定义需要查找的关键字最大字节数

#define ALARM_SRC_LEN       64
#define ALARM_KEYWORD_LEN   128

#define  S_SUN_HD_KEYWORD_FILE      "sunhdkeyword.ini"
#define  S_SUN_HD_EVENT_FILE         "sunhdevent.ini"

struct SAlarmKeyword
{
    char szAlarmSrc[ALARM_SRC_LEN];        //告警源(CPU、风扇、硬盘等)       
    char szKeyword[ALARM_KEYWORD_LEN];     //告警日志的关键字
};

typedef std::list<SAlarmKeyword> KEYWORD_LIST;

class CSunAlarm : public CHardwareAlarm
{
public:
    CSunAlarm();
    virtual ~CSunAlarm();

    virtual bool StartRcvThread(const char *szCfgPath,      // 存放配置文件的路径
                                int nPort = 162);           // 线程启动函数

    virtual bool ExitRcvThread();      // 退出线程

protected:
    static void * LogExecutionThread(void *param);              // 读系统日志文件线程执行函数
    bool LogInspect(void);                                      // 读日志文件，构造告警帧
    
    bool QueryKeyword(char* pszLocation, char* pszErrorType);   // 匹配关键字
    bool ReadAlarmCfg(char* szErrorType);                       // 读上报故障内容的配置文件
    bool ConstructAD(char* pszLocation);                        // 填充告警帧
    void ConstructAlarmFrame(char *pszErrType,char *pszFullStr);// 构造告警帧  
    bool ReadAlarmKeyword(void);

protected:

    void StringLTrim(char *szStr);
    void StringRTrim(char *szStr);
    void StringLRTrim(char *szStr);

protected:
    ACE_thread_t                    m_hReadSysLogThread;     //读系统日志线程句柄
	bool                            m_nReadSysLogRunning;    //标识接收Trap线程是否运行
    
    char                            m_szKeyWordFile[256];    //关键字配置文件的全路径
    char                            m_szAlarmIDFile[256];    //告警ID配置文件的全路径
    KEYWORD_LIST                    m_KeywordList;           //告警关键字
    SWarnData                       m_WarnData;              //告警帧

};

#endif //_SUN_ALARM_INTERFACE_H_
