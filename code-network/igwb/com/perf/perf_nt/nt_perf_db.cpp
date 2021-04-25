// nt_perf_db.cpp: implementation of the CNTPerfDB class.
//
//////////////////////////////////////////////////////////////////////
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/ 
#ifdef _PLATFORM_WIN32

#include "nt_perf_db.h"
#include "../resource.h"   //Perf的resource
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNTPerfDB::~CNTPerfDB()
{
    if (m_pPerfTitleDatabase != NULL)
	{
		delete m_pPerfTitleDatabase;
        m_pPerfTitleDatabase = NULL;
	}
    if(m_OSPerfStat.CPUPercent != NULL)
    {
        delete[] m_OSPerfStat.CPUPercent;
        m_OSPerfStat.CPUPercent = NULL;
    }
    if(m_OSPerfStat.Disks != NULL)
    {
        delete[] m_OSPerfStat.Disks;
        m_OSPerfStat.Disks = NULL;
    }
}

CNTPerfDB::CNTPerfDB(const char *szFront, const char *szBack, const char *szBak):
           COSPerfDB(szFront, szBack)
{
    strncpy(m_szBakPath,szBak,MAX_PATH);
    m_szBakPath[MAX_PATH - 1] = 0;
    memset(&m_OSPerfStat,0,sizeof(SOSPerfStat));
    m_dwFirstTick = 0;
    memset(&m_iProcessorTime[0],0,MAX_CPU_SURPPORT * sizeof(__int64));
	m_pPerfTitleDatabase = NULL;
	m_nProcessorCount = 0;
}

void CNTPerfDB::GetCpuRate()
{
    CPerfSnapshot snapshot( m_pPerfTitleDatabase);
	BOOL bResult = snapshot.TakeSnapshot( _T("Processor") );
	if ( bResult != TRUE)   
	{
		return;
	}

	CPerfObjectList objectList( &snapshot, m_pPerfTitleDatabase );

	PCPerfObject pProcessor = objectList.GetPerfObject( _T("Processor") );
	if ( pProcessor == NULL )    
	{
		return ;
	}

	PCPerfObjectInstance pPerfObjInst = pProcessor->GetFirstObjectInstance();
    __int64 iRecentTime[MAX_CPU_SURPPORT];
    memset(iRecentTime, 0, MAX_CPU_SURPPORT * sizeof(__int64));
    BYTE i = 0;
	for(i = 0; pPerfObjInst != NULL && i< m_nProcessorCount && i < MAX_CPU_SURPPORT; i++)
	{
		//得到Processer Time
		CPerfCounter* pCounter = pPerfObjInst->GetCounterByName("% Processor Time");
		if(pCounter == NULL)
		{
			delete pProcessor;
			delete pPerfObjInst;
			return ;
		}

		DWORD dwType; 
		__int64 iTemp;
		pCounter->GetData((PBYTE)&iTemp, 8, &dwType);
		iRecentTime[i] += iTemp;
		delete pCounter;
		pCounter = NULL;

		delete pPerfObjInst, pPerfObjInst = NULL;
		pPerfObjInst = pProcessor->GetNextObjectInstance();
	}

    if(pPerfObjInst != NULL)
    {
		delete pPerfObjInst, pPerfObjInst = NULL;
    }
	delete pProcessor, pProcessor = NULL;

    __int64 iTimeDelta[MAX_CPU_SURPPORT];
    memset(iTimeDelta, 0, MAX_CPU_SURPPORT * sizeof(__int64));
    //计算总时间
	DWORD dwRecentTick = ::GetTickCount();
	__int64 iTickDelta = dwRecentTick - m_dwFirstTick;
    //时间以100ns为单位
	iTickDelta *= 10000;

	//计算CPU占用率
    for(i = 0; i < MAX_CPU_SURPPORT && i < m_nProcessorCount; i++)
    {
        //首先计算IDLE进程占用的时间
        iTimeDelta[i] = iRecentTime[i] - m_iProcessorTime[i];
        //计算IDLE进程占用率，以％为单位
        if(iTickDelta == 0)
        {
            m_OSPerfStat.CPUPercent[i] = 100;
        }
        else
        {
            m_OSPerfStat.CPUPercent[i] = (short)((iTimeDelta[i] * 100) / iTickDelta);
            //CPU占用率 ＝ 100 - IDLE进程占用率
            m_OSPerfStat.CPUPercent[i] = 100 - m_OSPerfStat.CPUPercent[i];
        }
        //CPU占用率在0~100范围那
        if(m_OSPerfStat.CPUPercent[i] < 0)
        {
            m_OSPerfStat.CPUPercent[i] = 0;
        }
        if(m_OSPerfStat.CPUPercent[i] > 100)
        {
            m_OSPerfStat.CPUPercent[i] = 100;
        }
    }
	
    GetFirstProcTime();
	return ;
}

void CNTPerfDB::GetDiskInfo()
{
    ULARGE_INTEGER Free1,uTotalDiskRoom,uFreeDiskRoom;
    memset(&Free1, 0, sizeof(ULARGE_INTEGER));
	memset(&uTotalDiskRoom, 0,sizeof(ULARGE_INTEGER));
	memset(&uFreeDiskRoom, 0,sizeof(ULARGE_INTEGER));

    //得到前存盘的空余空间
	GetDiskFreeSpaceEx(m_szFrontSavePath,&Free1,&uTotalDiskRoom,&uFreeDiskRoom);
	m_OSPerfStat.Disks[0].uAva = (UINT4)(uFreeDiskRoom.QuadPart / 1024);//把字节数转换为K
    m_OSPerfStat.Disks[0].uTot = (UINT4)(uTotalDiskRoom.QuadPart / 1024);

	//得到后存盘的空余空间
	GetDiskFreeSpaceEx(m_szBackSavePath,&Free1,&uTotalDiskRoom,&uFreeDiskRoom);
	m_OSPerfStat.Disks[1].uAva = (UINT4)(uFreeDiskRoom.QuadPart / 1024);//把字节数转换为K
    m_OSPerfStat.Disks[1].uTot = (UINT4)(uTotalDiskRoom.QuadPart / 1024);//把字节数转换为K

    //得到备份的剩余空间
    if(m_szBakPath[0] != '\0')
    {
        GetDiskFreeSpaceEx(m_szBakPath,&Free1,&uTotalDiskRoom,&uFreeDiskRoom);
        m_OSPerfStat.Disks[2].uAva = (UINT4)(uFreeDiskRoom.QuadPart / 1024);//把字节数转换为K
        m_OSPerfStat.Disks[2].uTot = (UINT4)(uTotalDiskRoom.QuadPart / 1024);//把字节数转换为K
    }
    else
    {
        m_OSPerfStat.Disks[2].uAva = 0;
        m_OSPerfStat.Disks[2].uTot = 0;
    }
}

void CNTPerfDB::GetMemInfo()
{
    MEMORYSTATUS mstat;  
	GlobalMemoryStatus(&mstat);   
    //得到物理内存的信息
    m_OSPerfStat.uPhyMemAva = (UINT4)mstat.dwAvailPhys /1024/1024;
    m_OSPerfStat.uPhyMemTot = (UINT4)mstat.dwTotalPhys/1024/1024;
    //获得虚拟内存的信息
    CPerfSnapshot snapshot( m_pPerfTitleDatabase);
	BOOL bResult = snapshot.TakeSnapshot( _T("Memory") );
	if ( bResult != TRUE )   
	{
		return;
	}

	CPerfObjectList objectList( &snapshot, m_pPerfTitleDatabase );

	//首先处理内存信息
	PCPerfObject pMemory = objectList.GetPerfObject( _T("Memory") );
	if ( pMemory == NULL)    
	{
		return ;    
	}

	PCPerfObjectInstance pPerfObjInst;    
	pPerfObjInst = pMemory->GetFirstObjectInstance();
	PCPerfCounter pCounter;
	//得到已用虚拟内存
	pCounter = pPerfObjInst->GetCounterByName("Committed Bytes");
	if(pCounter == NULL)
	{
		delete pMemory;
		delete pPerfObjInst;
		return ;
	}
	DWORD dwType;
	DWORD dwCommittedBytes = 0;
	DWORD dwCommitLimit = 0;

	pCounter->GetData((PBYTE)&dwCommittedBytes, 4, &dwType);
	delete pCounter;
	pCounter = NULL;

	//得到最大虚拟内存
	pCounter = pPerfObjInst->GetCounterByName("Commit Limit");
	if(pCounter == NULL)
	{
		delete pMemory;
		delete pPerfObjInst;
		return ;
	}
	pCounter->GetData((PBYTE)&dwCommitLimit, 4, &dwType);
	delete pCounter;
	pCounter = NULL;

	//虚存以兆为单位
	dwCommittedBytes /= 1024*1024;
	dwCommitLimit /= 1024*1024;
    m_OSPerfStat.uVirMemAva = (UINT4)(dwCommitLimit - dwCommittedBytes);
    m_OSPerfStat.uVirMemTot = (UINT4)(dwCommitLimit);    
    
    delete pPerfObjInst;
	pPerfObjInst = NULL;
	delete pMemory;
}

void CNTPerfDB::GetFirstProcTime()
{
    CPerfSnapshot snapshot( m_pPerfTitleDatabase);
	BOOL bResult = snapshot.TakeSnapshot( _T("Processor") );
	if ( bResult != TRUE )   
	{
		return ;
	}

	m_dwFirstTick = ::GetTickCount();

	CPerfObjectList objectList( &snapshot, m_pPerfTitleDatabase );


	//处理CPU占用率
	PCPerfObject pProcessor = objectList.GetPerfObject( _T("Processor") );
	if ( pProcessor == NULL )    
	{
		return ;    
	}
	
	BYTE i = 0;
	PCPerfObjectInstance pPerfObjInst;   
	pPerfObjInst = pProcessor->GetFirstObjectInstance();
	memset(m_iProcessorTime, 0, MAX_CPU_SURPPORT * sizeof(__int64));

	while(pPerfObjInst != NULL && i < m_nProcessorCount)
	{
		//得到Processer Time
		CPerfCounter*  pCounter = pPerfObjInst->GetCounterByName("% Processor Time");
		if(pCounter == NULL)
		{
			delete pProcessor;
			delete pPerfObjInst;
			return;
		}
        if(i < MAX_CPU_SURPPORT)
        {
            __int64 iProcessorTime = 0;
            DWORD dwType;
            pCounter->GetData((PBYTE)&iProcessorTime, 8, &dwType);
            m_iProcessorTime[i] += iProcessorTime;
            delete pCounter;
            pCounter = NULL;
        }
		delete pPerfObjInst, pPerfObjInst = NULL;
		pPerfObjInst = pProcessor->GetNextObjectInstance();
		i++;
	}

    if(pPerfObjInst != NULL)
    {
		delete pPerfObjInst, pPerfObjInst = NULL;
    }
	delete pProcessor, pProcessor = NULL;
}

void CNTPerfDB::GetOSStatus(BYTE *pbPara, int nNum, char *&szAllPerf)
{
    CMMLReport MMLReport(S_PERF_TITLE);
    MMLReport.MakeReportHead();
#ifdef _PRODUCT_MOTO
    MMLReport.MakeCmdDisplay("LST PERF:",908,0,"O&M");
#else
    MMLReport.MakeCmdDisplay("LST PERF:",908,0,"TASKCMD");
#endif
    MMLReport.AddString(S_PERF_BILLSVRNO,8,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_HEARTBEAT,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_CPU_PER,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_CPU_PER,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_FRONTSAVE,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_BACKSAVE,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_MEM,16,ALIGN_LEFT);
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.AddString(STR_NEWLINE);

    char szTmp[512];

	if(pbPara == NULL)
	{
		//主备信息
        MMLReport.AddString("-",8,ALIGN_LEFT);
		//心跳信息
		MMLReport.AddString("-",8,ALIGN_LEFT);
	}
	else if(pbPara != NULL && nNum == 1)
	{
        //主备信息
        sprintf(szTmp,"%d",pbPara[0]);
		MMLReport.AddString(szTmp,8,ALIGN_LEFT);
		//心跳信息
		MMLReport.AddString("-",8,ALIGN_LEFT);
	}
	else
	{
		//主备信息
		sprintf(szTmp,"%d",pbPara[0]);
		MMLReport.AddString(szTmp,8,ALIGN_LEFT);
		
		//心跳信息
		sprintf(szTmp,"%d/%d",pbPara[2],pbPara[1]);
		MMLReport.AddString(szTmp,8,ALIGN_LEFT);
    }
    //CPU使用信息
    GetCpuRate();
    if(m_nProcessorCount == 2)
    {
        sprintf(szTmp,"%d",m_OSPerfStat.CPUPercent[0]);
        MMLReport.AddString(szTmp,16,ALIGN_LEFT);
        
        sprintf(szTmp,"%d",m_OSPerfStat.CPUPercent[1]);
        MMLReport.AddString(szTmp,16,ALIGN_LEFT);
    }
    else
    {
        sprintf(szTmp,"%d",m_OSPerfStat.CPUPercent[0]);
        MMLReport.AddString(szTmp,16,ALIGN_LEFT);
        
        MMLReport.AddString("-",16,ALIGN_LEFT);
    }
    
    //前后存盘磁盘空间
    GetDiskInfo();
    sprintf(szTmp, "%d/%d", m_OSPerfStat.Disks[0].uAva/1024, m_OSPerfStat.Disks[0].uTot/1024);
    MMLReport.AddString(szTmp,16,ALIGN_LEFT);
    sprintf(szTmp,"%d/%d",m_OSPerfStat.Disks[1].uAva/1024, m_OSPerfStat.Disks[1].uTot/1024);
    MMLReport.AddString(szTmp,16,ALIGN_LEFT);
    
    //内存使用信息

    GetMemInfo();
    sprintf(szTmp,"%d/%d",m_OSPerfStat.uPhyMemAva, m_OSPerfStat.uPhyMemTot);
    MMLReport.AddString(szTmp,16,ALIGN_LEFT);
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportTail();

    szAllPerf = new char[strlen(MMLReport.GetReport()) + 1];
    sprintf(szAllPerf,"%s",MMLReport.GetReport());
}

void CNTPerfDB::GetPerfStatistic(SOSPerfStat &OSPerfStat)
{
    GetDiskInfo();
    GetCpuRate();
    GetMemInfo();
    memcpy(&OSPerfStat,&m_OSPerfStat,sizeof(SOSPerfStat));
    OSPerfStat.CPUPercent = new BYTE[MAX_CPU_SURPPORT];
    //有三个分区信息，前存盘、后存盘和备份盘
    OSPerfStat.Disks = new SLogicalDisk[3];
    memcpy(OSPerfStat.CPUPercent,m_OSPerfStat.CPUPercent,MAX_CPU_SURPPORT * sizeof(BYTE));
    memcpy(OSPerfStat.Disks,m_OSPerfStat.Disks,3 * sizeof(SLogicalDisk));
}

BOOL CNTPerfDB::Init()
{
    //Init()对类成员进行初使化  
    m_OSPerfStat.CPUPercent = new BYTE[MAX_CPU_SURPPORT];
	memset(m_OSPerfStat.CPUPercent,0,MAX_CPU_SURPPORT * sizeof(BYTE));
    //有三个分区信息，前存盘、后存盘和备份盘
    m_OSPerfStat.Disks = new SLogicalDisk[3];
    strncpy(m_OSPerfStat.Disks[0].szName,m_szFrontSavePath,MAX_PATH);
    m_OSPerfStat.Disks[0].szName[MAX_PATH - 1] = 0;
    strncpy(m_OSPerfStat.Disks[1].szName,m_szBackSavePath,MAX_PATH);
    m_OSPerfStat.Disks[1].szName[MAX_PATH - 1] = 0;
    strncpy(m_OSPerfStat.Disks[2].szName,m_szBakPath,MAX_PATH);
    m_OSPerfStat.Disks[2].szName[MAX_PATH - 1] = 0;
    m_OSPerfStat.nLogDisk = 3;
    m_OSPerfStat.nCPUs = 0;
	//初始化性能数据库，以便于计算cpu利用率
   	m_pPerfTitleDatabase = new CPerfTitleDatabase;
	if(m_pPerfTitleDatabase == NULL)
	{
		return FALSE;
	}
    SYSTEM_INFO sInfo;
	GetSystemInfo(&sInfo);
	m_nProcessorCount = (BYTE)sInfo.dwNumberOfProcessors;
	m_OSPerfStat.nCPUs = m_nProcessorCount;
	GetFirstProcTime();
	return TRUE;
}

#endif //_PLATFORM_WIN32
