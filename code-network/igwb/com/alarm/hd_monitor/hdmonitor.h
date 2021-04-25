#ifndef _HARDWARE_MONITOR_H_
#define _HARDWARE_MONITOR_H_


#define PARAM_MAX_LEN 80

struct SWarnData
{
    unsigned short	AlarmId;			  //�澯���
    unsigned char	AlarmType;			  //5-�ָ�,6-����,7-�¼�
    unsigned short	Len;				  //��������
    unsigned char	Paras[PARAM_MAX_LEN]; //����ģ���͸��澯ģ��ĸ澯����
    
public:
    SWarnData()
    {
        memset(this, 0, sizeof(SWarnData)); // ��0��ʼ���澯֡
    }
    
};

class IHardwareAlarm;
typedef int (*FUNC)(IHardwareAlarm *pHDAlarm); // ����ע�ắ���Ŀ�ָ��

class IHardwareAlarm
{
public:
    IHardwareAlarm(){};
    virtual ~IHardwareAlarm(){};       //�������� 
    
    //����Ӳ���澯�ļ��ӵ��߳�
    virtual bool StartRcvThread(const char *szCfgPath, 
                                int nPort = 162) = 0;  
    //�˳�Ӳ���澯�ļ��ӵ��߳�
    virtual bool ExitRcvThread() = 0;  
    
    //�Ӷ�����ȡ���澯֡
    virtual bool GetAlarm(SWarnData& alarm) = 0;   
    
    //ע�ắ��
    virtual bool RegisterFunc(FUNC p) = 0;  
    
    //��ȡ������Ϣ
    virtual const char* GetLastError(void) const = 0;  
                  
};

#endif //_HARDWARE_MONITOR_H_
