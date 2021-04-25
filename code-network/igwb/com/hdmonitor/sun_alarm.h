/**************************************************************
**�ļ�����      sendalarm.h             
**������Ϣ��    �澯֡�Ļ���������
**�汾˵����    V100R001
**��Ȩ��Ϣ��    ��Ϊ�������޹�˾
**���ߣ�        ֣���棨34756��
**����ʱ�䣺    2002.11.15
**��Ҫ���б�  CSunAlarm    SNMP�澯������
**��Ҫ�����б������¶���
**�޸ļ�¼��    
**  1�����ߣ���С�� 
**     ���ڣ�2003-12-24
**     �޸�˵��������ԭ���йؽ���SNMP�Ĳ��֣�
**               ������SNMP TRAP���ָ��ĳɲ�ѰSUN�����ϵͳ��־��ʽ
**
**
***************************************************************/

#ifndef _SUN_ALARM_INTERFACE_H_
#define _SUN_ALARM_INTERFACE_H_

#include "hardware_alarm.h"


const int PATH_MAX_LEN          = 1024;       // �洢·������ֽ���
const int INSPECT_TIME          = 60;         // ��ϵͳ��־���ʱ��(��)
const int KEYWORD_MAX_LEN       = 50;         // ������Ҫ���ҵĹؼ�������ֽ���

#define ALARM_SRC_LEN       64
#define ALARM_KEYWORD_LEN   128

#define  S_SUN_HD_KEYWORD_FILE      "sunhdkeyword.ini"
#define  S_SUN_HD_EVENT_FILE         "sunhdevent.ini"

struct SAlarmKeyword
{
    char szAlarmSrc[ALARM_SRC_LEN];        //�澯Դ(CPU�����ȡ�Ӳ�̵�)       
    char szKeyword[ALARM_KEYWORD_LEN];     //�澯��־�Ĺؼ���
};

typedef std::list<SAlarmKeyword> KEYWORD_LIST;

class CSunAlarm : public CHardwareAlarm
{
public:
    CSunAlarm();
    virtual ~CSunAlarm();

    virtual bool StartRcvThread(const char *szCfgPath,      // ��������ļ���·��
                                int nPort = 162);           // �߳���������

    virtual bool ExitRcvThread();      // �˳��߳�

protected:
    static void * LogExecutionThread(void *param);              // ��ϵͳ��־�ļ��߳�ִ�к���
    bool LogInspect(void);                                      // ����־�ļ�������澯֡
    
    bool QueryKeyword(char* pszLocation, char* pszErrorType);   // ƥ��ؼ���
    bool ReadAlarmCfg(char* szErrorType);                       // ���ϱ��������ݵ������ļ�
    bool ConstructAD(char* pszLocation);                        // ���澯֡
    void ConstructAlarmFrame(char *pszErrType,char *pszFullStr);// ����澯֡  
    bool ReadAlarmKeyword(void);

protected:

    void StringLTrim(char *szStr);
    void StringRTrim(char *szStr);
    void StringLRTrim(char *szStr);

protected:
    ACE_thread_t                    m_hReadSysLogThread;     //��ϵͳ��־�߳̾��
	bool                            m_nReadSysLogRunning;    //��ʶ����Trap�߳��Ƿ�����
    
    char                            m_szKeyWordFile[256];    //�ؼ��������ļ���ȫ·��
    char                            m_szAlarmIDFile[256];    //�澯ID�����ļ���ȫ·��
    KEYWORD_LIST                    m_KeywordList;           //�澯�ؼ���
    SWarnData                       m_WarnData;              //�澯֡

};

#endif //_SUN_ALARM_INTERFACE_H_
