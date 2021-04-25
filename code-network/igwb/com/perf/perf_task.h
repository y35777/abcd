// perf_task.h: interface for the CPerfTask class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PERF_TASK_H__
#define _PERF_TASK_H__

#include "../include/base_type.h"
#include "../include/perf_item.h"
#include "dda_proxy.h"
#include "resource.h"

class CSaveProxy;
class CDdaSet;

struct sTimeSpan 
{
    tm  tStartTime;
    tm  tEndTime; 
};
#define INTMAP MAP<int,int,LESS<int> >
typedef VECTOR<sTimeSpan> TimeVector;

struct SInformation{
    int     m_nStatus;     //����״̬
    STRING  m_szTaskName;  //��������
    STRING  m_szTaskDes;   //��������
    STRING  m_szUser;       //������������û�
    IntVector   m_vMU;     //������ԪID����
    int     m_nPeriod;     //ͳ������
    time_t  m_tStartTime;  //��ʼ����
    TimeVector  m_vTimeSpan; //ʱ��εĶ���
    int     m_nTaskType;     //��������
    int     m_nDayLimit;     //������������
    int     m_nTimeType;     //ʱ������
    IntVector   m_vTimeSet;  //ͳ��ʱ�伯��
    int	    m_nResultSaveDays;     //�����������
    OIVMap   m_mObjectInstance; //����ʵ����ϣ��
    CounterMap	m_mCouterMap;      //����ָ��Ĺ�ϣ��
    INTMAP m_mCounterStatisticCount; //ÿ������ָ���ͳ�ƴ���
};

class CPerfTask{
public:
	int IsInPeriod();
    void Timer1Sec();
	int Expire();
	int DelPerf();
	int ProcStatistic(SPerfItem *pPerfs,int nCount);
	int Init(SInformation *pInfo, STRING szFilePath);
    const char *GetLastError(){
        return m_szError.c_str();
    }
    CPerfTask();
	virtual ~CPerfTask();
    SInformation    *m_pTaskInfo; //������Ϣ
#ifdef UNIT_TEST
public:
#else
protected:
#endif
	int O2IVIsEqual(int ApID);
	int IsInTimeSet(int DateVal);
	int IsInTimeSpan(tm *pCurrent);
    CSaveProxy *m_pSave;     //�����������ݴ���
    int	    m_nStatisticCount;  //ͳ�ƴ���һ��һ�룬����30��ʾͳ����30��
    time_t  m_tStart;           //����ͳ�Ƶ���ʼʱ��
    time_t  m_tEnd;             //����ͳ�ƵĽ���ʱ��
    //CDdaSet *m_pDdaSet;         //DBF�ļ�������
    STRING m_szError;           //������Ϣ
    STRING m_szPerfFilePath;     //�������·��
};

#endif // _PERF_TASK_H_

