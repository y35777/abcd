/***************************************************************************
 *��Ʒ��    ��iGateway Bill V200
 *ģ����    ��monitor
 *�ļ���    ��monitor.h
 *������Ϣ  ����CMonitor�Ķ��塣
 *�汾˵��  ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ������ƽ
 *����ʱ��  ��2001-10-30
 *�޸ļ�¼  ��
*****************************************************************************/

#ifndef _PROCESS_MONITOR_H_
#define _PROCESS_MONITOR_H_

#include "monitor_hdr.h"
#include "proc_mgr.h"
#include "../toolbase/ini_file_ex.h"

class CMonitor:public CWorkModule
{
public:
    CMonitor();
    ~CMonitor();
    
    static int SendMsgToManager(PROCESS_TYPE nPType,    //��ָ�����̵Ĺ���ģ�鷢������ 
                                UINT4 uCmdCode);

protected: 
    virtual int InitInstance();     
    virtual void ExitInstance();
    virtual void ParseMsg(MSG_INTRA* pMsgParsed);
    virtual MODULE_TYPE GetModuleType();
    int Active();                                       //����
    void DeActive();                                    //ȥ����
    virtual void OnTimer(const long nTimerID);
    void Timer5Sec();
    int SendAnsToClstrInterface(UINT4 uCmdCode,         //��˫���ӿ�ģ�鷢Ӧ��
                                UINT4 uResult);
	void SendInfoToAlarm();                             //��澯ģ�鷢�澯
    void SendAnsProcStatus();

private:
    CProcessManager m_ProcMgr;                          //���̹����߶���
    EKERNELSTATUS m_nStatus;                            //Kernelģ�鵱ǰ״̬
    long m_lTimerID5Sec;                                //5�붨ʱ��ID
    
    //add by maimaoshi,2003-03-15
    long m_lTimerID1Sec;                                //1�붨ʱ��
	BOOL m_bClusterprocIsExist;                         //˫�����̴��ڱ�־
	long m_nCounter;
	//end
};

#endif //_MONITOR_H_

