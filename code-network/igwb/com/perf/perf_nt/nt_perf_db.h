// nt_perf_db.h: interface for the CNTPerfDB class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _PLATFORM_WIN32

#ifndef __NT_PERF_DB__
#define __NT_PERF_DB__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../os_perf_db.h"
#include "PdhUtility.h"
#include "../../utility/mml_report.h"
#define INVALID_DISKSP_INFO     0XCCCCCCCC  //��Ч�Ĵ��̿ռ���Ϣ                                   
#define MAX_CPU_SURPPORT                 2  //����CPU֧�ָ���

class CNTPerfDB : public COSPerfDB  
{
public:
	BOOL Init();
	virtual void GetPerfStatistic(SOSPerfStat& OSPerfStat);
	virtual void GetOSStatus(BYTE *pbPara,int nNum,char*& szAllPerf);
	void GetFirstProcTime();
	CNTPerfDB(const char *szFront, const char *szBack, const char *szBak);
	virtual ~CNTPerfDB();
	
private:
    void GetDiskInfo();
	void GetCpuRate();
	void GetMemInfo();
    CPerfTitleDatabase * m_pPerfTitleDatabase;  //�������ݿ�
    __int64 m_iProcessorTime[MAX_CPU_SURPPORT]; //cpuʱ��
	DWORD m_dwFirstTick;                        //��һ�β���CPUʱ��ʱ��tickֵ
    BYTE m_nProcessorCount;                   //CPU��Ŀ
    char m_szBakPath[MAX_PATH];                 //����·��
   SOSPerfStat m_OSPerfStat; 

};

#endif

#endif _PLATFORM_WIN32

