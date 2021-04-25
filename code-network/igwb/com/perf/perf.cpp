/**************************************************************************
 *产品名    : iGateway Bill V200        
 *模块名    : 性能模块(Perf)
 *文件名    : perf.cpp
 *描述信息  : 本文件是类CPerf的实现
 *版本说明  : V200R001i00M00B00D1130A   
 *版权信息  : 版权所有(c) 2001-2002 华为技术有限公司
 *作者      : 方卫华,张玉新
 *创建时间  : 2001-11-05
 *修改记录  : 
 **************************************************************************/

#include "../include/toolbox.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "perf.h"
#include "resource.h"

/**************************************************************************
函数原型    public: CPerf(void);
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CPerf::CPerf(void)
{
    m_PerfStatID           = 0;
    m_ApConnTimer          = 0;
    m_uSavedDays           = 0;
    m_uBillsvrNo           = 0;
    memset(m_perfHeart,0,sizeof(SPerfItem) * 2);

    m_uMinDiskAlarmRoom    = 0;
    m_uMaxDiskAlarmRoom    = 0;    
    m_uMinDiskHandoverRoom = 0;
    m_uMinBakDeviceRoom    = 0;
    m_bFrontSaveAlarm      = FALSE;
    m_bBackSaveAlarm       = FALSE;
    m_bBakDeviceAlarm      = FALSE;
    m_bFrontSaveMinorAlarm = FALSE;
    m_bBackSaveMinorAlarm  = FALSE;
    m_bBakDeviceMinorAlarm = FALSE;    
    m_bFrontSaveHandover   = FALSE;
    m_bBackSaveHandover    = FALSE;
    m_bBakDeviceHandover   = FALSE;

    m_uAPNum               = 0;
    m_pOSPerfDB            = NULL;
    m_pMMLReport           = NULL;
    m_nLinkMaxCounter      = 10;
    m_Timer1SecId          = 0;
    m_pCPUTotalArray       = NULL;
    m_pExCPUUsedArray      = NULL;
    m_szFileName[0]        = '\0';
    m_pM2000Manager        = NULL;
    m_pPerfUtil            = NULL;
}

/**************************************************************************
函数原型    public: ~CPerf( );
功能描述    析构函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CPerf::~CPerf()
{
    if(m_pOSPerfDB)
    {
        delete m_pOSPerfDB;
    }
        
    PERF_MAP::iterator i;
    
    for(i = m_PerfMap.begin();i != m_PerfMap.end(); i++)
    {
        delete[] (*i).second;
    }
    m_PerfMap.clear();

    if(NULL != m_pPerfUtil)
    {
        m_pPerfUtil->CloseFile();
        delete m_pPerfUtil;
        m_pPerfUtil = NULL;
    }

    if(NULL != m_pM2000Manager)
    {
        delete m_pM2000Manager;
        m_pM2000Manager = NULL;
    }
}

/**************************************************************************
函数原型    protected: MODULE_TYPE GetModuleType(void);
功能描述    获得本模块的类型
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
MODULE_TYPE CPerf::GetModuleType(void)
{
    return MT_PERF;
}

/**************************************************************************
函数原型    protected: int InitInstance(void);
功能描述    初始化
输入参数    无
输出参数    无
返回值      初始化成功则返回ERR_SUCCESS
抛出异常    无
***************************************************************************/
int CPerf::InitInstance(void)
{
    int nRet = 0;

    if((nRet = CWorkModule::InitInstance()) != ERR_SUCCESS)
    {
        return nRet;
    }

    // 性能统计时间间隔
    UINT4 uPerfInterval = 0;

    // 打开配置文件
    char *szConfigPath = const_cast <char *>(GetCfgFilePath());
    CINIFile ConfigFile(szConfigPath);
    if(ConfigFile.Open())
    {
        // 从配置库里获取性能文件存放的根路径
        char temp[MAX_PATH];

        ConfigFile.GetString(CFG_SEC_PERF,
                             CFG_PERF_ROOT_DIR, 
                             CFG_PERF_ROOT_DIR_DEFAULT,
                             temp,
                             MAX_PATH);
        m_szBasePath = temp;

        // 从配置库里获取性能文件保留天数
        m_uSavedDays = ConfigFile.GetInt(CFG_SEC_PERF, 
                                         CFG_PERF_SAVE_DAY, 
                                         CFG_PERF_SAVE_DAY_DEFAULT);

        // 获取最小告警、倒换和备份空间配置
        // 存盘路径从CFG_SEC_DISK_FILE段读取
        m_uMinDiskAlarmRoom = ConfigFile.GetInt(CFG_SEC_DISK_FILE, 
                                                CFG_PERF_MIN_ALM_ROOM,
                                                CFG_PERF_MIN_ALM_ROOM_DEFAULT);
        m_uMaxDiskAlarmRoom = ConfigFile.GetInt(CFG_SEC_DISK_FILE,
                                                CFG_PERF_MAX_ALM_ROOM,
                                                CFG_PERF_MAX_ALM_ROOM_DEFAULT);
        if(m_uMinDiskAlarmRoom >= m_uMaxDiskAlarmRoom)
        {
            m_uMinDiskAlarmRoom = CFG_PERF_MIN_ALM_ROOM_DEFAULT;
            m_uMaxDiskAlarmRoom = CFG_PERF_MAX_ALM_ROOM_DEFAULT;
            
        }        

        m_uMinDiskHandoverRoom = ConfigFile.GetInt(CFG_SEC_DISK_FILE, 
                                                   CFG_PERF_MIN_HANDOVER_ROOM,
                                                   CFG_PERF_MIN_HANDOVER_ROOM_DEFAULT);

        m_uMinBakDeviceRoom = ConfigFile.GetInt(CFG_SEC_DISK_FILE,
                                                CFG_PERF_MIN_BACKUP_ROOM, 
                                                CFG_PERF_MIN_BACKUP_ROOM_DEFAULT);

        // 从配置库里获取前存盘、后存盘和备份介质路径
        // 存盘路径从CFG_SEC_DISK_FILE段读取

        ConfigFile.GetString(CFG_SEC_DISK_FILE,
                             CFG_FRONT_SAVE_ROOT_DIR,
                             CFG_FRONT_SAVE_ROOT_DIR_DEFAULT,
                             temp,
                             MAX_PATH);
        m_szFrontSavePath = temp;
        
        ConfigFile.GetString(CFG_SEC_DISK_FILE,
                             CFG_BACK_SAVE_ROOT_DIR, 
                             CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                             temp,
                             MAX_PATH);
        m_szBackSavePath  = temp;
        
        ConfigFile.GetString(CFG_SEC_DISK_FILE,
                             CFG_BACKUP_DEST_ROOT_DIR, 
                             CFG_BACKUP_DEST_ROOT_DIR_DEFAULT,
                             temp,
                             MAX_PATH);
        m_szBakDevicePath = temp;

        // 从配置库里获取接入点数
        // 接入点数从CFG_SEC_COMMON段读取
        m_uAPNum = ConfigFile.GetInt(CFG_SEC_COMMON,
                                     CFG_COMMON_KEY_APCOUNT,
                                     CFG_COMMON_KEY_APCOUNT_DEFAULT);

        //读取性能状态检查定时器时长
        uPerfInterval = ConfigFile.GetInt(CFG_SEC_PERF, 
                                          CFG_PERF_STAT_INTERVAL,
                                          CFG_PERF_STAT_INTERVAL_DEFAULT);

        //读取主备机信息
        m_uBillsvrNo = ConfigFile.GetInt(CFG_SEC_COMMON,CFG_SERVERNO,0);

		//张玉新 2002-12-07 
	    //读入统计周期
		m_nPeriod = ConfigFile.GetInt(CFG_SEC_PERF,
                                      CFG_PERF_MONITOR_PERIOD,
                                      CFG_PERF_MONITOR_PERIOD_DEFAULT);
		if(m_nPeriod > 60 || m_nPeriod < 5)
		{
			m_nPeriod = CFG_PERF_MONITOR_PERIOD_DEFAULT;
		}
		
		m_nPerfPeriod = ConfigFile.GetInt(CFG_SEC_PERF,
										  CFG_PERF_STAT_PERIOD,
										  CFG_PERF_STAT_PERIOD_DEFAULT);

		//CPU过载的上限
		m_nCPUmaxLimit = ConfigFile.GetInt(CFG_SEC_PERF,
                                           CFG_PERF_CPU_MAX_LIMIT,
                                           CFG_PERF_CPU_MAX_LIMIT_DEFAULT);
		if(m_nCPUmaxLimit > 90 || m_nCPUmaxLimit < 70)
		{
			m_nCPUmaxLimit = CFG_PERF_CPU_MAX_LIMIT_DEFAULT;
		}
		//CPU过载的下限
		m_nCPUminLimit = ConfigFile.GetInt(CFG_SEC_PERF,
                                           CFG_PERF_CPU_MIN_LIMIT,
                                           CFG_PERF_CPU_MIN_LIMIT_DEFAULT);
		if(m_nCPUminLimit > 50 || m_nCPUminLimit < 20)
		{
			m_nCPUminLimit = CFG_PERF_CPU_MIN_LIMIT_DEFAULT;
		}

        //Memeory重要告警的阀值
        m_nMemMaxLimit = ConfigFile.GetInt(CFG_SEC_PERF,
                                           CFG_PERF_MEM_MAX_LIMIT,
                                           CFG_PERF_MEM_MAX_LIMIT_DEFAULT);
        //ldf对应问题单SWPD01534 2003-09-10 
        if(m_nMemMaxLimit > 90 || m_nMemMaxLimit < 70)
        {
            m_nMemMaxLimit = CFG_PERF_MEM_MAX_LIMIT_DEFAULT;
        }
        
        //memrory次要告警的阀值
        m_nMemMidLimit = ConfigFile.GetInt(CFG_SEC_PERF,
            CFG_PERF_MEM_MID_LIMIT,
            CFG_PERF_MEM_MID_LIMIT_DEFAULT);
        if(m_nMemMidLimit > 80 || m_nMemMidLimit < 50)
        {
            m_nMemMidLimit = CFG_PERF_MEM_MID_LIMIT_DEFAULT;
        }
        
        //Memeory告警回复的阀值
        m_nMemMinLimit = ConfigFile.GetInt(CFG_SEC_PERF,
            CFG_PERF_MEM_MIN_LIMIT,
            CFG_PERF_MEM_MIN_LIMIT_DEFAULT);
        if(m_nMemMinLimit > 50 || m_nMemMinLimit < 20)
        {
            m_nMemMinLimit = CFG_PERF_MEM_MIN_LIMIT_DEFAULT;
        }
        //修改完毕
        
		//读入均值周期
		m_nExSpan = ConfigFile.GetInt(CFG_SEC_PERF,
                                      CFG_PERF_EX_SPAN,
                                      CFG_PERF_EX_SPAN_DEFAULT);
		if(m_nExSpan >250 || m_nExSpan < 5)
		{
			m_nExSpan = CFG_PERF_EX_SPAN_DEFAULT;
		}

        m_bUseXml = ConfigFile.GetInt(CFG_SEC_PERF, 
                                      CFG_USE_XML, 
                                      CFG_USE_XML_DEFAULT);

        m_pPerfUtil = CreatePerfUtilObj();
        if(NULL == m_pPerfUtil)
        {
            return ERR_FAIL;
        }

        //张玉新 增加结束
        // 关闭配置文件
        ConfigFile.Close();
        m_CollectStatistic.Init(m_szFrontSavePath.c_str());
    }
    else
    {
        return ERR_FAIL;
    }
    
// 针对不同平台，创建性能数据库
#ifdef _PLATFORM_WIN32   
	//张玉新 2002-04-13 增加
    m_pOSPerfDB = (COSPerfDB *)new CNTPerfDB(m_szFrontSavePath.c_str(),
											 m_szBackSavePath.c_str(),
											 m_szBakDevicePath.c_str());
	CNTPerfDB *pNTPerf = static_cast<CNTPerfDB *>(m_pOSPerfDB);
	if(pNTPerf->Init() == FALSE)
	{
		TRACE(THISMODULE,S_PERF_OSDBINIT_FAIL);
		return ERR_FAIL;
	}
	//结束
#else
    m_pOSPerfDB = (COSPerfDB *)new CUNIXPerfDB(m_szFrontSavePath.c_str(),
                                               m_szBackSavePath.c_str());
#endif /* _PLATFORM_WIN32 */    
    
    // 注册定时器，定时获得性能数据
    m_PerfStatID = SetTimer(uPerfInterval * 1000);

    m_ApConnTimer = SetTimer(AP_CONN_STATISTIC_TIMER);
    
	//张玉新 2002-12-07
    m_nMonitorID = SetTimer(m_nPeriod * 1000);
	//增加结束
    // 删除限定天数前的性能数据文件
    DeleteOutdatedFile();
    
    // 当前性能文件名，格式为yyyymmdd_orig.prf
    char filename[9];
    time_t curtime = ACE_OS::time(NULL);
    ACE_OS::strftime(filename, 9, "%Y%m%d", ACE_OS::localtime(&curtime));
    
    STRING szPerfFile = MAKE_QUALIFIED_PATH(m_szBasePath, filename);
    szPerfFile += PERF_FILE_SUFFIX;

    //创建性能统计数据文件存放目录
    if(!CreateDeepDir(m_szBasePath.c_str()))
        return ERR_FAIL;

	//added by maimaoshi
	m_pMMLReport = new CMMLReport("PERF");

	MSG_INTRA *pMsg      = new MSG_INTRA;
	pMsg->SenderPid     = GetProcessType(); 
	pMsg->SenderMid     = GetModuleType();
	pMsg->ReceiverPid   = PT_BROADCAST;
	pMsg->ReceiverMid   = MT_MANAGER;
	pMsg->CmdCode       = CMD_GET_APNAME;

	SendMsg(pMsg);

	//设置秒事件定时器
    m_Timer1SecId = SetTimer(1000);
	//end by maimaoshi

    sprintf(m_szFileName, "%s%s", szPerfFile.c_str(), GetPerfFileExt());
    if (ERR_SUCCESS != m_pPerfUtil->OpenFile(m_szFileName))
	{
		TRACE(THISMODULE,S_PERF_OPEN_PERF_FAIL,m_szFileName);
		return ERR_FAIL;
	}

	//张玉新 2002-12-24 平安夜
	SOSPerfStat OSPerf;
	m_pOSPerfDB->GetPerfStatistic(OSPerf);
	m_pCPUTotalArray = new unsigned short[OSPerf.nCPUs];
    m_pExCPUUsedArray = new float[OSPerf.nCPUs];
    memset(m_pCPUTotalArray,0,sizeof(unsigned short) * OSPerf.nCPUs);
    memset(m_pExCPUUsedArray,0,sizeof(float) * OSPerf.nCPUs);
	m_nSpanCount = 0;
	m_uMEMTotal = 0;

    CreateDeepDir(m_szFrontSavePath.c_str());
    CreateDeepDir(m_szBackSavePath.c_str());
    CreateDeepDir(m_szBakDevicePath.c_str());
    // 获取存储设备可用空间
    int nFront = 0,nBack = 0,nBak = 0,nMountLen = 0;
    for(int i = OSPerf.nLogDisk - 1;i >= 0;i--)
    {
        char* pName = OSPerf.Disks[i].szName;
        nMountLen = strlen(pName);
        
        STRING strFrontSub = m_szFrontSavePath.substr(0,nMountLen);
        if(!strFrontSub.compare(pName))
        {
            if(nMountLen > nFront)
            {
            	nFront = nMountLen;
                m_nTotalFrontSaveRoom = OSPerf.Disks[i].uTot;
            }
        }
        
        STRING strBackSub = m_szBackSavePath.substr(0,nMountLen);
        if(!strBackSub.compare(pName))
        {
            if(nMountLen > nBack)
            {
            	nBack = nMountLen;
                m_nTotalBackSaveRoom = OSPerf.Disks[i].uTot;
            }
        }
        
        if (!m_szBakDevicePath.empty())
        {
            STRING strBak = m_szBakDevicePath.substr(0,nMountLen);
            if(!strBak.compare(pName))
            {
                if(nMountLen > nBak)
                {
                    nBak = nMountLen;
                    m_nTotalBakDeviceRoom = OSPerf.Disks[i].uTot;
                }
            }
        }
    }
    
    UINT4 nMaxTotalRoom = m_nTotalFrontSaveRoom;
    if(m_nTotalFrontSaveRoom < m_nTotalBackSaveRoom)
    {
        nMaxTotalRoom = m_nTotalBackSaveRoom;
    }
    if(nMaxTotalRoom == 0)
    {
        TRACE(MTS_PERF, GetStringFromID(5003));
        return ERR_FAIL;
    }    
    //如果倒换空间<2倍的告警空间
    else if( 2 * m_uMinDiskHandoverRoom >  m_uMinDiskAlarmRoom * nMaxTotalRoom)
    {
        
        m_uMinDiskAlarmRoom = 2 * m_uMinDiskHandoverRoom / nMaxTotalRoom ;
        m_uMaxDiskAlarmRoom = 2 * m_uMinDiskHandoverRoom / nMaxTotalRoom ;
        TRACE(MTS_PERF,S_PERF_ADJUST_ALARM_SPACE);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_ADJUST_ALARM_SPACE);
    }    

#if defined(_PRODUCT_MOTO) || defined(_PRODUCT_CG)
    //张玉新 增加M2000性能接口
    CreateDeepDir(S_PERF_M2000);
    if(NULL == m_pM2000Manager)
    {
        m_pM2000Manager = new CTaskManager;
        if(!m_pM2000Manager->Init(S_PERF_M2000))
        {
            TRACE(MTS_PERF,S_PERF_MANAGER_INIT_FAIL);
            return ERR_FAIL;
        }
    }
#endif
    return ERR_SUCCESS;
}

/**************************************************************************
函数原型    protected: void ExitInstance(void);
功能描述    退出时的清理工作
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerf::ExitInstance(void)
{
    if(m_pOSPerfDB)
    {
        delete m_pOSPerfDB;
        m_pOSPerfDB = NULL;
    }
    
    KillTimer(m_Timer1SecId);
    //张玉新 2002-12-24 删除CPU的累计值数组
	if(m_pCPUTotalArray)
	{
		delete[] m_pCPUTotalArray;
	}
    if(m_pExCPUUsedArray)
        delete[] m_pExCPUUsedArray;

	//增加结束
    CWorkModule::ExitInstance();
}

/**************************************************************************
函数原型    proteced: void OnTimer(const long nTimerID);
功能描述    根据定时器ID分别处理定时器事件
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerf::OnTimer(const long nTimerID)
{
    static int nM2000StatisticCount = 0;
    char szCurDate[9];
    char szCurPerfFileDate[9];
    
    // 获取当前的日期;
    time_t curtime = ACE_OS::time(NULL);
    ACE_OS::strftime(szCurDate, 9, "%Y%m%d", ACE_OS::localtime(&curtime));

    
    // 获得当前性能文件名的日期
    ACE_OS::strncpy(szCurPerfFileDate, 
                    m_szFileName + ACE_OS::strlen(m_szFileName) -
                      ACE_OS::strlen(PERF_FILE_SUFFIX) - 8, 8);
    szCurPerfFileDate[8] = '\0';
    //江国华 结束增加 2003-11-19

    // 当前日期与已打开的文件名的日期不同
    if(ACE_OS::strcmp(szCurDate,szCurPerfFileDate) > 0)
    {
        // 删除限定天数前的性能数据文件
        DeleteOutdatedFile();
        //张玉新 2003-03-04 删除限定天数前的性能数据文件
        if(NULL != m_pM2000Manager)
        {
            m_pM2000Manager->DelOutDatedPerfFile();
        }
        //增加结束

        STRING szPerfFile;

        szPerfFile = MAKE_QUALIFIED_PATH(m_szBasePath, szCurDate);
        szPerfFile += PERF_FILE_SUFFIX;
    
        //打开新的文件
        sprintf(m_szFileName, "%s%s", szPerfFile.c_str(), GetPerfFileExt());
        if (ERR_SUCCESS != m_pPerfUtil->OpenFile(m_szFileName))
		{
			TRACE(THISMODULE, S_PERF_OPEN_PERF_FAIL, m_szFileName);
		}
    }

	//by ldf 2004-07-10 对应问题单SWPD06729
	int nFront = 0,nBack = 0,nBak = 0,nMountLen = 0;
	//end
    if(m_PerfStatID == nTimerID)
    {
        UINT4 uFrontSaveRoom = 0;
        UINT4 uBackSaveRoom  = 0;
        UINT4 uBakDeviceRoom = 0;

        // 获得所需要的性能数据
        SOSPerfStat OSPerfStat;
        m_pOSPerfDB->GetPerfStatistic(OSPerfStat);


        // 获取存储设备可用空间
        for(int i = OSPerfStat.nLogDisk - 1;i >= 0;i--)
        {
            char* pName = OSPerfStat.Disks[i].szName;
            nMountLen = strlen(pName);

            STRING strFrontSub = m_szFrontSavePath.substr(0,nMountLen);
            if(!strFrontSub.compare(pName))
            {
                if(nMountLen > nFront)
                {
                    nFront = nMountLen;
                    uFrontSaveRoom = OSPerfStat.Disks[i].uAva;
                }
            }

            STRING strBackSub = m_szBackSavePath.substr(0,nMountLen);
            if(!strBackSub.compare(pName))
            {
                if(nMountLen > nBack)
                {
                    nBack = nMountLen;
                    uBackSaveRoom = OSPerfStat.Disks[i].uAva;
                }
            }

            if (!m_szBakDevicePath.empty())
            {
                STRING strBak = m_szBakDevicePath.substr(0,nMountLen);
                if(!strBak.compare(pName))
                {
                    if(nMountLen > nBak)
                    {
                        nBak = nMountLen;
                        uBakDeviceRoom = OSPerfStat.Disks[i].uAva;
                    }
                }
            }
        }

        //把K转换为M.
        float uFrontRoomUsed = 100 * ((float)uFrontSaveRoom / (float)m_nTotalFrontSaveRoom);
        float uBackRoomUsed =  100 * ((float)uBackSaveRoom  / (float)m_nTotalBackSaveRoom);
        float uBakDevRoomUsed =100 * ((float)uBakDeviceRoom / (float)m_nTotalBakDeviceRoom);

        uFrontSaveRoom = (UINT4)(uFrontSaveRoom / 1024);
        uBackSaveRoom = (UINT4)(uBackSaveRoom / 1024);
        uBakDeviceRoom = (UINT4)(uBakDeviceRoom / 1024);

        // 前存盘相应处理
        if(uFrontRoomUsed < m_uMinDiskAlarmRoom)
        {
            TRACE(THISMODULE,S_PERF_FRONT_MIN_ROOM_ALM,
                uFrontRoomUsed,m_uMinDiskAlarmRoom);
            MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_FRONT_MIN_ROOM_ALM,
                uFrontRoomUsed,m_uMinDiskAlarmRoom);
            
            // 产生前存盘“磁盘空间不足”故障告警消息
            SInnerAlarm alarm;
            alarm.uAlarmID      = ALARM_ID_DISK_ROOM;      // 磁盘空间不足
            alarm.yAlarmType    = AT_FAULT;                // 故障告警
            alarm.yAlarmPara[0] = ALARM_PARA_FRONT_SAVE;   // 前存盘
            SendAlarm(&alarm);
            m_bFrontSaveAlarm = TRUE;

            if(uFrontSaveRoom < m_uMinDiskHandoverRoom)
            {
                TRACE(THISMODULE, S_PERF_FRONT_HV_ROOM_ALM,
                    uFrontSaveRoom,m_uMinDiskHandoverRoom);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_FRONT_HV_ROOM_ALM,
                    uFrontSaveRoom,m_uMinDiskHandoverRoom);

                // 向KERNEL的双机接口模块发送倒换命令
                AskForFailOver(GetModuleType(),(BYTE)ERROR_NOENOUGHDISK_PRIMARY,NULL,0);
                m_bFrontSaveHandover = TRUE;
            }
        }

        if(uFrontRoomUsed < m_uMaxDiskAlarmRoom && uFrontRoomUsed >= m_uMinDiskAlarmRoom 
           && !m_bFrontSaveAlarm)
        {
            TRACE(THISMODULE,S_PERF_FRONT_MIN_ROOM_ALM,
                uFrontRoomUsed,m_uMaxDiskAlarmRoom);
            MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_FRONT_MIN_ROOM_ALM,
                uFrontRoomUsed,m_uMaxDiskAlarmRoom);

            // 产生前存盘“磁盘空间不足(minor)”故障告警消息
            SInnerAlarm alarm;
            alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // 磁盘空间不足
            alarm.yAlarmType    = AT_FAULT;                // 故障告警
            alarm.yAlarmPara[0] = ALARM_PARA_FRONT_SAVE;   // 前存盘            
            SendAlarm(&alarm);
            m_bFrontSaveMinorAlarm = TRUE;
        }
        
        if(uFrontRoomUsed > m_uMaxDiskAlarmRoom)
        {
            if(m_bFrontSaveHandover)
            {                               
                // 异步通知双机接口模块前存盘空间恢复
                m_bFrontSaveHandover = FALSE;
                MSG_INTRA* pMsg   = new MSG_INTRA;                
                pMsg->SenderPid   = GetProcessType();
                pMsg->SenderMid   = GetModuleType();
                pMsg->ReceiverPid = PT_KERNEL;              // 发送给KERNEL进程
                pMsg->ReceiverMid = MT_CLSTRINTERFACE;      // 发送给双机接口模块
                pMsg->CmdCode     = CMD_DISKSPACERECOVERY;  // 磁盘空间恢复
                pMsg->Data[0] = (BYTE) ERROR_NOENOUGHDISK_PRIMARY;
                SendMsg(pMsg);
            }

            if(m_bFrontSaveAlarm)
            {               
                // 产生前存盘“磁盘空间不足”恢复告警消息
                m_bFrontSaveAlarm = FALSE;
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM;      // 磁盘空间不足恢复
                alarm.yAlarmType    = AT_RESTORE;              // 恢复告警
                alarm.yAlarmPara[0] = ALARM_PARA_FRONT_SAVE;   // 前存盘
                SendAlarm(&alarm);               
            }

            if(m_bFrontSaveMinorAlarm)
            {
                // 产生前存盘“磁盘空间不足(minor)”恢复告警消息
                m_bFrontSaveMinorAlarm = FALSE;
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // 磁盘空间不足恢复
                alarm.yAlarmType    = AT_RESTORE;              // 恢复告警
                alarm.yAlarmPara[0] = ALARM_PARA_FRONT_SAVE;   // 前存盘                    
                SendAlarm(&alarm);
            }
        }   

        // 后存盘相应处理
        if(uBackRoomUsed < m_uMinDiskAlarmRoom)
        {
            TRACE(THISMODULE, S_PERF_BACK_MIN_ROOM_ALM,
                uBackRoomUsed,m_uMinDiskAlarmRoom);
            MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_BACK_MIN_ROOM_ALM,
                uBackRoomUsed,m_uMinDiskAlarmRoom);

            // 产生后存盘“磁盘空间不足”故障告警消息
            SInnerAlarm alarm;
            alarm.uAlarmID       = ALARM_ID_DISK_ROOM;      // 磁盘空间不足
            alarm.yAlarmType     = AT_FAULT;                // 故障告警
            alarm.yAlarmPara[0]  = ALARM_PARA_BACK_SAVE;    // 后存盘
            SendAlarm(&alarm);
            m_bBackSaveAlarm = TRUE;

            if(uBackSaveRoom < m_uMinDiskHandoverRoom)
            {
                TRACE(THISMODULE, S_PERF_BACK_HV_ROOM_ALM,
                    uBackSaveRoom,m_uMinDiskHandoverRoom);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_BACK_HV_ROOM_ALM,
                    uBackSaveRoom,m_uMinDiskHandoverRoom);

                // 向KERNEL的双机接口模块发送倒换命令
                AskForFailOver(GetModuleType(),(BYTE)ERROR_NOENOUGHDISK_SECONDARY,NULL,0);
                m_bBackSaveHandover = TRUE;
            }
        }

        if(uBackRoomUsed < m_uMaxDiskAlarmRoom && uBackRoomUsed >= m_uMinDiskAlarmRoom
           && !m_bBackSaveAlarm)
        {
            TRACE(THISMODULE,S_PERF_BACK_MIN_ROOM_ALM,
                uBackRoomUsed,m_uMaxDiskAlarmRoom);
            MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_BACK_MIN_ROOM_ALM,
                uBackRoomUsed,m_uMaxDiskAlarmRoom);
            
            // 产生前存盘“磁盘空间不足(minor)”故障告警消息
            SInnerAlarm alarm;
            alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // 磁盘空间不足
            alarm.yAlarmType    = AT_FAULT;                // 故障告警
            alarm.yAlarmPara[0] = ALARM_PARA_BACK_SAVE;   // 前存盘            
            SendAlarm(&alarm);
            m_bBackSaveMinorAlarm = TRUE;              
        }
        
        if(uBackRoomUsed > m_uMaxDiskAlarmRoom)
        {   
            if(m_bBackSaveHandover)
            {               
                // 异步通知双机接口模块后存盘空间恢复
                m_bBackSaveHandover = FALSE;
                MSG_INTRA* pMsg   = new MSG_INTRA;                
                pMsg->SenderPid   = GetProcessType();
                pMsg->SenderMid   = GetModuleType();
                pMsg->ReceiverPid = PT_KERNEL;              // 发送给Cluster进程
                pMsg->ReceiverMid = MT_CLSTRINTERFACE;      // 发送给Agent模块 
                pMsg->CmdCode     = CMD_DISKSPACERECOVERY;  // 磁盘空间恢复
                pMsg->Data[0] = (BYTE)ERROR_NOENOUGHDISK_SECONDARY;                
                SendMsg(pMsg);
            }

            if(m_bBackSaveAlarm)
            {
                // 产生后存盘“磁盘空间不足”恢复告警消息
                m_bBackSaveAlarm = FALSE;
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM;      // 磁盘空间不足恢复
                alarm.yAlarmType    = AT_RESTORE;              // 恢复告警
                alarm.yAlarmPara[0] = ALARM_PARA_BACK_SAVE;    // 后存盘
                SendAlarm(&alarm);
            }
            
            if(m_bBackSaveMinorAlarm)
            {
                // 产生前存盘“磁盘空间不足(minor)”恢复告警消息
                m_bBackSaveMinorAlarm = FALSE;
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // 磁盘空间不足恢复
                alarm.yAlarmType    = AT_RESTORE;              // 恢复告警
                alarm.yAlarmPara[0] = ALARM_PARA_BACK_SAVE;   // 前存盘                    
                SendAlarm(&alarm);
            }
        }

        // 备份介质相应处理
        if(!m_szBakDevicePath.empty())
        {
            if(uBakDevRoomUsed < m_uMinDiskAlarmRoom)
            {
                TRACE(THISMODULE, S_PERF_BACKUP_MIN_ROOM_ALM,
                    uBakDevRoomUsed,m_uMinDiskAlarmRoom);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_BACKUP_MIN_ROOM_ALM,
                    uBakDevRoomUsed,m_uMinDiskAlarmRoom);
                
                // 产生备份介质“磁盘空间不足”故障告警消息
                SInnerAlarm alarm;
                alarm.uAlarmID       = ALARM_ID_DISK_ROOM;      // 磁盘空间不足
                alarm.yAlarmType     = AT_FAULT;                // 故障告警
                alarm.yAlarmPara[0]  = ALARM_PARA_BACKUP_DEV;    // 备份介质
                SendAlarm(&alarm);
                m_bBakDeviceAlarm = TRUE;                
            }

            if(uBakDevRoomUsed < m_uMaxDiskAlarmRoom && uBakDevRoomUsed >= m_uMinDiskAlarmRoom
                && !m_bBakDeviceAlarm)
            {
                TRACE(THISMODULE,S_PERF_BACKUP_MIN_ROOM_ALM,
                    uBakDevRoomUsed,m_uMaxDiskAlarmRoom);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_BACKUP_MIN_ROOM_ALM,
                    uBakDevRoomUsed,m_uMaxDiskAlarmRoom);
                
                // 产生前存盘“磁盘空间不足(minor)”故障告警消息
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // 磁盘空间不足
                alarm.yAlarmType    = AT_FAULT;                // 故障告警
                alarm.yAlarmPara[0] = ALARM_PARA_BACKUP_DEV;   // 备份介质            
                SendAlarm(&alarm);
                m_bBakDeviceMinorAlarm = TRUE;              
            }
            
            if(uBakDevRoomUsed > m_uMaxDiskAlarmRoom)
            {                 
                if(m_bBakDeviceAlarm)
                {
                    // 产生后存盘“磁盘空间不足”恢复告警消息
                    m_bBackSaveAlarm = FALSE;
                    SInnerAlarm alarm;
                    alarm.uAlarmID      = ALARM_ID_DISK_ROOM;      // 磁盘空间不足恢复
                    alarm.yAlarmType    = AT_RESTORE;              // 恢复告警
                    alarm.yAlarmPara[0] = ALARM_PARA_BACKUP_DEV;    // 备份介质
                    SendAlarm(&alarm);
                }
                
                if(m_bBakDeviceMinorAlarm)
                {
                    // 产生前存盘“磁盘空间不足(minor)”恢复告警消息
                    m_bBackSaveMinorAlarm = FALSE;
                    SInnerAlarm alarm;
                    alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // 磁盘空间不足恢复
                    alarm.yAlarmType    = AT_RESTORE;              // 恢复告警
                    alarm.yAlarmPara[0] = ALARM_PARA_BACKUP_DEV;   // 备份介质                    
                    SendAlarm(&alarm);
                }
            }           
        }        
    }

    if(nTimerID == m_ApConnTimer)
        SendConnStat();

	//added by maimaoshi,2002-11-29
	//对每个链路进行计数
	LINKCOUNTER_LIST::iterator it;
	if(nTimerID == m_Timer1SecId)
    {
		for (it = m_LinkCounterList.begin(); it != m_LinkCounterList.end(); it++)
		{			
			(*it).nCounter++;
			//当超过最大值，为了避免溢出，把计数值置到最大值+1就行了
			if ((*it).nCounter > m_nLinkMaxCounter)
			{
				(*it).nCounter = m_nLinkMaxCounter + 1;
			}
		}

        if(NULL != m_pM2000Manager)
        {
            nM2000StatisticCount ++;
            if(nM2000StatisticCount >= m_nPerfPeriod)
            {
				MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_PERF_MONITOR_PERIOD);
                SPerfItem PerfData[8];
                memset(PerfData,0,sizeof(SPerfItem) * 8);
                SOSPerfStat OSPerfStat;
                int i = 0;
                for(i=0;i<8;i++)
                {
                    PerfData[i].yAPID = PT_OM;
                    time(&(PerfData[i].timeBegin));
                    PerfData[i].timeEnd = PerfData[i].timeBegin;
                    PerfData[i].yModuleID = MT_PERF;
                }
                m_pOSPerfDB->GetPerfStatistic(OSPerfStat);
                if(OSPerfStat.CPUPercent != NULL)
                {
                    PerfData[0].uID = PERF_CPU1_USAGE;
                    PerfData[0].uValue = OSPerfStat.CPUPercent[0];
                    PerfData[1].uID = PERF_CPU2_USAGE;
                    if(OSPerfStat.nCPUs > 1)
                    {
                        PerfData[1].uValue = OSPerfStat.CPUPercent[1];
                    }  
                }
				//by ldf 2004-07-10 对应问题单SWPD06729
				// 获取存储设备可用空间
				for(int j = OSPerfStat.nLogDisk - 1;j >= 0;j--)
				{
					char* pName_M2000 = OSPerfStat.Disks[j].szName;
					nMountLen = strlen(pName_M2000);
					
					STRING strFrontSub_M2000 = m_szFrontSavePath.substr(0,nMountLen);
					if(!strFrontSub_M2000.compare(pName_M2000))
					{
						if(nMountLen > nFront)
						{
							nFront = nMountLen;
							PerfData[4].uValue = OSPerfStat.Disks[j].uAva/1024;
							PerfData[5].uValue = OSPerfStat.Disks[j].uTot/1024;
						}
					}
					
					STRING strBackSub_M2000 = m_szBackSavePath.substr(0,nMountLen);
					if(!strBackSub_M2000.compare(pName_M2000))
					{
						if(nMountLen > nBack)
						{
							nBack = nMountLen;
							PerfData[6].uValue = OSPerfStat.Disks[j].uAva/1024;
							PerfData[7].uValue = OSPerfStat.Disks[j].uTot/1024;
						}
					}
				}
					
				//end
                PerfData[2].uID = PERF_PHYSICAL_MEM_AVA;
                PerfData[2].uValue = OSPerfStat.uPhyMemAva;
                PerfData[3].uID = PERF_PHYSICAL_MEM_TOTAL;
                PerfData[3].uValue = OSPerfStat.uPhyMemTot;
                PerfData[4].uID = PERF_FRONT_SAVE_AVA;
                PerfData[5].uID = PERF_FRONT_SAVE_TOTAL;
                PerfData[6].uID = PERF_BACK_SAVE_AVA;
                PerfData[7].uID = PERF_BACK_SAVE_TOTAL;
                m_pM2000Manager->ProcStatistic(PerfData,8);
                nM2000StatisticCount = 0;
            }
            m_pM2000Manager->Timer1Sec();
            }
    }

	if(nTimerID == m_nMonitorID)
	{
        MonitorSystem();
	}
}

/**************************************************************************
函数原型    proteced: void ParseMsg(MSG_INTRA* pMsgParsed);
功能描述    解析性能模块收到的消息
输入参数    指向性能模块收到的消息的指针
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerf::ParseMsg(MSG_INTRA* pMsgParsed)
{    
    switch(pMsgParsed->CmdCode)
    {
        // 保存性能数据性能数据,其中包括了各个接入点的网络连接情况.
        case CMD_SAVE_PERF:
        {
            //TRACE(THISMODULE, S_PERF_RECV_SAVE_PERF);

            SPerfItem* pData = (SPerfItem*)(pMsgParsed->Data);
            int nCount = (pMsgParsed->Len) / sizeof(SPerfItem);

            SavePerfItem(pData, nCount);

            //以下代码用于实现连接情况的统计.
            //wangfeng modified for collect
            if(pMsgParsed->SenderPid >= PT_CO_BASE && pMsgParsed->SenderPid <= MAX_PT_CO_TYPE)
            {
                ProcConnectStatus(pMsgParsed->SenderPid - PT_CO_BASE, pData, nCount);
            }
            else if ((pMsgParsed->SenderPid >= PT_AP_BASE) && (pMsgParsed->SenderPid < PT_CO_BASE))//接入点进程
            {
                ProcConnectStatus(pMsgParsed->SenderPid - PT_AP_BASE, pData, nCount);
            }

            if(NULL != m_pM2000Manager)
            {
                //张玉新 2003-03-04
                m_pM2000Manager->ProcStatistic(pData,nCount);
            }
            break;
        }

        // 这是双机模块定时发送的心跳状态数据.
        case CMD_ANS_HB_STATUS:
        {
            m_perfHeart[0].yAPID = pMsgParsed->SenderPid;
            m_perfHeart[0].yModuleID = pMsgParsed->SenderMid;
            m_perfHeart[0].uID = PERF_HEARTBEAT;
            m_perfHeart[0].timeBegin = ACE_OS::time(NULL);
            m_perfHeart[0].uValue    = pMsgParsed->Data[0];//心跳个数
            m_perfHeart[1].uValue    = pMsgParsed->Data[1];//心跳可用个数

            break;
        }
		//added by maimaoshi,2002-11-22
		// 接点进程启动的时候发送接点名称.
        case CMD_AP_NAME:
        {
			SetAllAPName(pMsgParsed);          

            break;
        }
		// 本模块向接点进程请求接点名称
        case CMD_GET_APNAME:
        {           

            break;
        }
		case CMD_GET_APLINK_STATE:
		{
			SLinkStat* pData = (SLinkStat*)(pMsgParsed->Data);
            //wangfeng modified for collect
			//DecideLinkStat(pMsgParsed->SenderPid - PT_AP_BASE, pData->nModelNum, pData->nLinkNum);
			//CLinkStatItem::CommitLinkStat(pMsgParsed->SenderPid - PT_AP_BASE, *pData);
			DecideLinkStat(pMsgParsed->SenderPid - PT_CO_BASE, 
                           *pData);
			CLinkStatItem::CommitLinkStat(pMsgParsed->SenderPid - PT_CO_BASE, 
                                          *pData);
			break;
		}
		//end by maimaoshi
		//这是kernel进程Monitor模块发送过来的进行状态数据
		case CMD_ANS_PROC_STATUS:
		{
			QueryProcStatus(pMsgParsed);
			break;
		}

        case CMD_MML:   // MML命令
        {
            //TRACE(THISMODULE, S_PERF_RECV_MML_CMD);

            CParseResult *pParse = *(CParseResult **)pMsgParsed->Data;
            m_nCurClientNo = pMsgParsed->ClientNo;

            // 查询双机心跳状态.
            switch(pParse->GetCmdCode()) 
			{
		    case MML_LST_PERF:
                //TRACE(THISMODULE, S_PERF_RECV_MML_LST_PERF);
               	QueryStatus(pMsgParsed);
		  	    break;

            case MML_LST_LINK:
                QueryIPAddr(pMsgParsed);
			    break;

		    //added by maimaoshi,2002-11-22
            case MML_LST_ALLAP:
                QueryAllAPName(pMsgParsed);
                break;

            case MML_LST_APLINK:
                QueryLinkState(pMsgParsed);
                break;

            case MML_CRE_ALM:
                CreateAlarm(pParse);
                break;
	        //end
            //added by maimaoshi ,2003-03-14
            case MML_LST_PROC:
	        {
	            //向KERNEL进程中的监控模块发请求进程状态信息
                MSG_INTRA *pMsg     = new MSG_INTRA;
                pMsg->SenderPid     = GetProcessType(); 
                pMsg->SenderMid     = GetModuleType();
                pMsg->ReceiverPid   = PT_KERNEL;
                pMsg->ReceiverMid   = MT_MONITOR;
                pMsg->CmdCode       = CMD_ANS_PROC_STATUS;
                SendMsg(pMsg);
                		
                break;
            }
            case MML_LST_COL_FILE:
                m_CollectStatistic.ProcStatisticCollectFiles(pParse);
                break;
			case MML_SET_PERF_INV:
				SetStatusInv(pMsgParsed);
				break;
			case MML_LST_PERF_INV:
				LstStatusInv(pMsgParsed);
				break;
	        //end
	        default:
                if(NULL != m_pM2000Manager)
                {
                    //张玉新 2003-03-04
                    m_pM2000Manager->Dispatch(pMsgParsed);
                    //结束
                }
                break;
            }

            delete pParse, pParse = NULL;
            break;
        }
        default:
        {
            TRACE(THISMODULE,S_PERF_RECV_OTHER_MSG,pMsgParsed->SenderPid,
                pMsgParsed->SenderMid,
                pMsgParsed->CmdCode);
            break;
        }
    }
}

/**************************************************************************
函数原型    proteced: void QueryStatus(void);
功能描述    让性能模块处于状态查询的状态
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/    
void CPerf::QueryStatus(MSG_INTRA* pMsg)
{
    // 得到系统的信息和MML报文
    BYTE nInfo[3];
    nInfo[0] = (BYTE)m_uBillsvrNo;
    nInfo[1] = (BYTE)m_perfHeart[0].uValue;//心跳个数
    nInfo[2] = (BYTE)m_perfHeart[1].uValue;//心跳可用个数
    
    char *pcMMLReport = NULL;
    m_pOSPerfDB->GetOSStatus(nInfo,3,pcMMLReport);
    
    // 向MML客户端发送MML报文
    MSG_INTRA *pMssage = new(strlen(pcMMLReport) + 1) MSG_INTRA;
    strcpy((char*)pMssage->Data,pcMMLReport);
    delete[] pcMMLReport;
    
    pMssage->SenderPid   = GetProcessType();
    pMssage->SenderMid   = GetModuleType();
    pMssage->ReceiverPid = PT_OUTSIDE;
    pMssage->ReceiverMid = MT_MMLDEBUG;

    strcpy(pMssage->UserName,pMsg->UserName);
    
    pMssage->AppType     = pMsg->AppType;
    pMssage->ClientNo    = pMsg->ClientNo;
    pMssage->IPAddr      = pMsg->IPAddr;
    pMssage->CmdCode     = CMD_MML;
    
    SendMsg(pMssage);
    
}

void CPerf::SetStatusInv(MSG_INTRA* pMsg)
{
    CParseResult* pParseResult = *((CParseResult**)(pMsg->Data));

	int inval=0;
    if (pParseResult->GetDigitVal(&inval, 1))
	{
		char *szConfigPath = const_cast <char *>(GetCfgFilePath());
		CINIFile ConfigFile(szConfigPath);
		if(ConfigFile.Open())
		{
			ConfigFile.WriteInt(CFG_SEC_PERF,
								CFG_PERF_STAT_PERIOD,
								inval);
			ConfigFile.Close();
		}
		m_nPerfPeriod = inval;
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 0 , S_ALARM_OPRSUCCESS);
	}
	return;
}

void CPerf::LstStatusInv(MSG_INTRA* pMsg)
{
    CMMLReport MMLReport(S_PERF_TITLE);
    MMLReport.MakeReportHead();
#ifdef _PRODUCT_MOTO
    MMLReport.MakeCmdDisplay("LST PERFINTVAL:",908,0,"O&M");
#else
    MMLReport.MakeCmdDisplay("LST PERFINTVAL:",908,0,"TASKCMD");
#endif
	MMLReport.AddString(S_PERF_MONITOR_PERIOD);
	MMLReport.AddString(STR_NEWLINE);
	for(int i=0;i<strlen(S_PERF_MONITOR_PERIOD);++i)
		MMLReport.AddString("-");
	MMLReport.AddString(STR_NEWLINE);
	MMLReport.AddString("    PERFINVAL ", 20, ALIGN_LEFT);
	MMLReport.AddString(STR_NEWLINE);
	MMLReport.AddString(STR_NEWLINE);
	char szInval[20];
	sprintf(szInval, "    %d", m_nPerfPeriod);
	MMLReport.AddString(szInval, 20, ALIGN_LEFT);
	MMLReport.AddString(STR_NEWLINE);
	MMLReport.AddString(STR_NEWLINE);
	MMLReport.MakeReportTail();

    char* szAllPerf = new char[strlen(MMLReport.GetReport()) + 1];
    sprintf(szAllPerf,"%s",MMLReport.GetReport());
    // 向MML客户端发送MML报文
    MSG_INTRA *pMssage = new(strlen(szAllPerf) + 1) MSG_INTRA;
    strcpy((char*)pMssage->Data,szAllPerf);
    
    pMssage->SenderPid   = GetProcessType();
    pMssage->SenderMid   = GetModuleType();
    pMssage->ReceiverPid = PT_OUTSIDE;
    pMssage->ReceiverMid = MT_MMLDEBUG;
	
    strcpy(pMssage->UserName,pMsg->UserName);
    
    pMssage->AppType     = pMsg->AppType;
    pMssage->ClientNo    = pMsg->ClientNo;
    pMssage->IPAddr      = pMsg->IPAddr;
    pMssage->CmdCode     = CMD_MML;
    
    SendMsg(pMssage);	
}

void CPerf::QueryIPAddr(MSG_INTRA *pMsg)
{
    CINIFile ini(GetCfgFilePath());
	//生成MML报文
	/*
	+++    iGateway bill                           2002-07-22 09:18:34
        TASKCMD    #0
        %%  LST LINK:; %%
        RETCODE  =  0    操作执行成功   (注意此处"="前后应只有一个空格 by ldf 2002.8.13)
	*/
	CParseResult *pParse = *(CParseResult **)pMsg->Data;
    CMMLReport MMLReport(S_PERF_TITLE);
    MMLReport.MakeReportHead();
#ifdef _PRODUCT_MOTO
    MMLReport.MakeCmdDisplay(pParse->GetCmdLine(),908,0,"O&M");
#else
    MMLReport.MakeCmdDisplay(pParse->GetCmdLine(),908,0,"TASKCMD");
#endif
    MMLReport.AddString(STR_NEWLINE);
	if(ini.Open() == FALSE)
	{
        MMLReport.AddString(STR_OPRFAIL);
		MMLReport.AddString(STR_NEWLINE);
	}
	else
	{
		int nAPCount = 0;
		nAPCount = ini.GetInt(CFG_SEC_COMMON,CFG_COMMON_KEY_APCOUNT,nAPCount);
		char szAPName[32];
		char szIP[32];
		char szContent[512];
		LIST<STRING> IPList;
		//搜刮IP地址
		for(int i = 1; i <= nAPCount; i++)
		{
			SNPRINTF(szAPName,32,"%s%d",CFG_SEC_AP,i);
			szAPName[31] = 0;
			ini.GetString(szAPName,CFG_AP_KEY_LOCAL_IP_TO_PM,CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT,szIP,32);
			if(std::find(IPList.begin(),IPList.end(),STRING(szIP)) == IPList.end()
				&& strcmp(szIP,CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT) != 0)
			{
				IPList.push_back(szIP);
			}
			ini.GetString(szAPName,CFG_AP_KEY_LOCAL_IP_TO_FM,CFG_AP_KEY_LOCAL_IP_TO_FM_DEFAULT,szIP,32);
			if(std::find(IPList.begin(),IPList.end(),STRING(szIP)) == IPList.end()
				&& strcmp(szIP,CFG_AP_KEY_LOCAL_IP_TO_FM_DEFAULT) != 0)
			{
				IPList.push_back(szIP);
			}
		}
		
		/*
		iGWB连接IP
        ----------------------------
        IP=10.11.243.198
		
		IP=10.11.243.197
		  
		IP=10.11.243.196
			(结果个数 = 3)
			---End
		*/
		MMLReport.AddString(S_PERF_IP_LIST_TITLE); //"iGWB连接IP" = S_PERF_IGWB_IP
		MMLReport.AddString(STR_NEWLINE);
        int nStrLen = strlen(S_PERF_IP_LIST_TITLE);
	    for(int j = 0; j < nStrLen; j++)
		{
		    MMLReport.AddString("-");
		}
		MMLReport.AddString(STR_NEWLINE);
		if(IPList.size() > 0)
		{
			LIST<STRING>::iterator iter = IPList.begin();
			//"Ip" = S_PERF_IP
			SNPRINTF(szContent,512,S_PERF_FMT_STR1,S_PERF_IP,iter->c_str());
			szContent[511] = 0;
			MMLReport.AddString(szContent);
			MMLReport.AddString(STR_NEWLINE);
			for(++iter; iter != IPList.end(); iter++)
			{
				MMLReport.AddString(STR_NEWLINE);
				SNPRINTF(szContent,512,S_PERF_FMT_STR1,S_PERF_IP,iter->c_str());
			    szContent[511] = 0;
				MMLReport.AddString(szContent);
				MMLReport.AddString(STR_NEWLINE);		
			}
		}
		//"结果个数" == S_PERF_RESULT_CNT
		SNPRINTF(szContent,32,S_PERF_FMT_STR2,S_PERF_IP_RESULT,IPList.size());
		szContent[511] = 0;
                //by ldf 2002.8.13
                MMLReport.AddString("(");
                //change end
		MMLReport.AddString(szContent);
                //by ldf 2002.8.13
                MMLReport.AddString(")");
                //change end
		MMLReport.AddString(STR_NEWLINE);
		MMLReport.AddString(STR_NEWLINE);
	}
	MMLReport.MakeReportTail();
	//发送报文
	MSG_INTRA *pMssage = new(strlen(MMLReport.GetReport()) + 1) MSG_INTRA;
	strcpy((char*)pMssage->Data,MMLReport.GetReport());
    
	pMssage->SenderPid   = GetProcessType();
	pMssage->SenderMid   = GetModuleType();
	pMssage->ReceiverPid = PT_OUTSIDE;
	pMssage->ReceiverMid = MT_MMLDEBUG;
	
	strcpy(pMssage->UserName,pMsg->UserName);
    
	pMssage->AppType     = pMsg->AppType;
	pMssage->ClientNo    = pMsg->ClientNo;
	pMssage->IPAddr      = pMsg->IPAddr;
    
	SendMsg(pMssage);
}
/**************************************************************************
函数原型    proteced: void ProcConnectStatus(const BYTE nAccessPointID, 
                                             SPerfItem* pItems, 
                                             const BYTE nCount);
功能描述    处理接入点与主机的连接情况
输入参数    nAccessPointID: 接入点的编号
            pItems: SPerfItem结构数组，一个元素表示一个端口的连接情况
            nCount: SPerfItem结构数组中元素的个数
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerf::ProcConnectStatus(const BYTE nAccessPointID, 
                              SPerfItem *pData, 
                              const BYTE nCount)
{
    SPerfItem *pPerfItem = NULL;
    SPerfItem *pItem = NULL;
    PERF_MAP::iterator iter;
    for(int i = 0;i < nCount;i++)
    {
        //如果找不到对应接入点的连接情况,将他加入MAP中
        if((iter = m_PerfMap.find(pData[i].yAPID)) == m_PerfMap.end())
        {
            if(pData[i].uID == GTP_ALLGSN_CONNECT 
                || pData[i].uID == GTP_ALLPRI_CONNECT
                || pData[i].uID == GTP_ALLSEC_CONNECT)
            {
                pPerfItem = new SPerfItem[2];
                memcpy(&pPerfItem[0],&pData[i],sizeof(SPerfItem));
                memcpy(&pPerfItem[1],&pData[i],sizeof(SPerfItem));
                m_PerfMap[pData[i].yAPID] = pPerfItem;
                if(pData[i].uID == GTP_ALLPRI_CONNECT)
                {
                    pPerfItem[1].uID = GTP_ALLSEC_CONNECT;
                    pPerfItem[1].uValue = 0;
                }
                else if(pData[i].uID == GTP_ALLSEC_CONNECT)
                {                  
                    pPerfItem[0].uID = GTP_ALLPRI_CONNECT;
                    pPerfItem[0].uValue = 0;
                }
            }
        }
        else
        {   //如果找到对应的统计项,就改变其统计值
            pItem = (*iter).second;
            if(pData[i].uID == GTP_ALLGSN_CONNECT 
                && pItem[0].uID == GTP_ALLGSN_CONNECT)
            {
                pItem[0].uValue = pData[i].uValue;
                pItem[1].uValue = pData[i].uValue;
            }
            else if(pData[i].uID == GTP_ALLPRI_CONNECT 
                && pItem[0].uID == GTP_ALLPRI_CONNECT)
            {
                pItem[0].uValue = pData[i].uValue;
            }
            else if(pData[i].uID == GTP_ALLSEC_CONNECT
                && pItem[1].uID == GTP_ALLSEC_CONNECT)
            {
                pItem[1].uValue = pData[i].uValue;
            }
        }
    }
    return;
}

/**************************************************************************
函数原型    proteced: void SavePerfItem(SPerfItem* pItems, 
                                        const BYTE nCount);
功能描述    保存来自各进程的性能数据
输入参数    pItems: SPerfItem结构数组，一个元素表示一项性能数据
            nCount: SPerfItem结构数组中元素的个数
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CPerf::SavePerfItem(SPerfItem* pItems, const BYTE nCount)
{
    if (pItems == NULL)
    {
        return;
    }

    //操作对象指针
    SPerfItem* pOperateObj = NULL;

    //循环增加内容
    for (int i = 0; i < nCount; i++)
    {
        if (ERR_SUCCESS == m_pPerfUtil->AppendPerfItem(*(pItems + i)))
        {
            return;
        }
    }

    //写入到文件
	if (ERR_SUCCESS == m_pPerfUtil->Flush())
	{
		return;
	} 
}

/**************************************************************************
函数原型    private: int GetPerfFileList ( LIST<STRING>& PerfFileList );
功能描述    获取性能文件根目录下所有性能统计文件
输入参数    无
输出参数    性能统计文件列表
返回值      成功则返回ERR_SUCCESS，否则ERR_FAIL
抛出异常    无
***************************************************************************/
int CPerf::GetPerfFileList(LIST<STRING>& PerfFileList)
{
    // 查找性能文件根目录下所有性能统计文件名称
    ACE_DIR* pBaseDIR = ACE_OS_Dirent::opendir(m_szBasePath.c_str());
    if(NULL == pBaseDIR)
    {
        TRACE(THISMODULE, S_PERF_OPEN_DIR_FAIL, m_szBasePath.c_str());
        return ERR_FAIL;
    }
    
    dirent *pDirent = NULL;
    STRING strTemp  = PERF_FILE_SUFFIX;
    strTemp        += GetPerfFileExt();
    while((pDirent  = ACE_OS_Dirent::readdir(pBaseDIR)) != NULL)
    {
        if(strstr(pDirent->d_name, strTemp.c_str()) != NULL)
        {       
            PerfFileList.push_back(pDirent->d_name);
        }
    }

    ACE_OS_Dirent::closedir(pBaseDIR);

    return ERR_SUCCESS;
}

/**************************************************************************
函数原型    private: int DeleteOutdatedFile(void);
功能描述    删除指定天数前的性能数据文件
输入参数    无
输出参数    无
返回值      成功则返回ERR_SUCCESS,否则ERR_FAIL
抛出异常    无
***************************************************************************/
int CPerf::DeleteOutdatedFile ( void )
{
    LIST<STRING> PerfFileList;
    STRING PerfFile;

    time_t curtime = ACE_OS::time(NULL);
    unsigned long curdate = curtime / (24 * 3600);

    // 获得性能文件列表
    if(GetPerfFileList(PerfFileList) != ERR_SUCCESS)
    {
        return ERR_FAIL;
    }

    // 删除限定天数前的性能数据文件
    for(LIST<STRING>::iterator iter = PerfFileList.begin();
                               iter != PerfFileList.end(); iter++)
    {
        PerfFile = MAKE_QUALIFIED_PATH(m_szBasePath, *iter);

        // 获取文件最后修改时间
        ACE_stat f_stat;
        if(ACE_OS::stat(PerfFile.c_str(), &f_stat))
        {
            TRACE(THISMODULE, S_PERF_GET_PERF_STAT_FAIL, PerfFile.c_str());
            continue;
        }

        unsigned long mdate = f_stat.st_mtime / (24 * 3600);

        if((curdate > mdate) && (curdate - mdate > m_uSavedDays))
        {
            // 删除该性能文件
            if(ACE_OS::unlink(PerfFile.c_str()))
            {
                TRACE(THISMODULE, S_PERF_REMOVE_PERF_FAIL, PerfFile.c_str());
                continue;
            }
            TRACE(THISMODULE, S_PERF_REMOVE_PERF_SUC, PerfFile.c_str());
        }
    }

    return ERR_SUCCESS;
}

void CPerf::SendConnStat()
{
    /* 若性能统计已经关闭,直接返回 */
    if(g_bPerfIterm != TRUE)
        return;

    SPerfItem* pItem = NULL;
    char left[1024];

    /* 构造报文体 */
    PERF_MAP::iterator i = m_PerfMap.begin(),
                       e = m_PerfMap.end();
    while(i != e)
    {
        pItem = i->second;
        if(pItem->uID == GTP_ALLGSN_CONNECT)
        {
            /* 接入点连接GSN */

            sprintf(left,S_PERF_GSN_CONNECT,pItem->yAPID,pItem->uValue);
            MessageOut(THISMODULE,MSGOUT_LEVEL_IMPORTANT,left);
        }
        else if(pItem->uID == GTP_ALLPRI_CONNECT)
        {
            /* 接入点连接MEM板 */

            sprintf(left,S_PERF_MSG_PRI_CONNECT,pItem[0].yAPID,pItem[0].uValue);
            MessageOut(THISMODULE,MSGOUT_LEVEL_IMPORTANT,left);

            sprintf(left,S_PERF_MSG_SEC_CONNECT,pItem[1].yAPID,pItem[1].uValue);
            MessageOut(THISMODULE,MSGOUT_LEVEL_IMPORTANT,left);
        }

        i++;
    }
}
//added by maimaoshi,2002-11-22
//处理客户端询问链路状态
void CPerf::QueryLinkState(MSG_INTRA *pMsg)
{
	CParseResult* pParseResult = *((CParseResult** )(pMsg->Data));

    //从MML命令中得到接入点名
    STRING sApName = (const char* )pParseResult->GetStringVal(1);

	//取得接点号
	BYTE yApID;
	for (MAP<BYTE, STRING>::iterator itt = m_mapAPNumToAPName.begin();
									  itt != m_mapAPNumToAPName.end(); itt++)
	{
		if ((*itt).second == sApName)
		{
			yApID = (*itt).first;
			break;
		}									  
	}
	
	//根据接点号从取出链路状态数据
	LINKSTAT_LIST* pLinkList = NULL;

	CLinkStatItem::GetLinkStatData(pLinkList, yApID);

	if ((pLinkList == NULL) || (pLinkList->size() == 0))
	{
		//如果没有，提示错误
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_PERF_TT_FAILED_GETSTATE);
		return;
	}

	//如果有，则组织所有接点名称报文，返回给客户端	

	//获取待发送的表格的标题和内容
	LIST<STRING> TitleList;
	TitleList.push_back(S_PERF_TT_LOCALIP);
	TitleList.push_back(S_PERF_TT_MODELNUM);
	TitleList.push_back(S_PERF_TT_LINKNUM);
	TitleList.push_back(S_PERF_TT_STATE);

	//获取待发送的内容列表
	LIST<STRING> ContentList;	
	char szBuff[50];

	//把链路状态数据组织成为报文
	for (LINKSTAT_LIST::iterator it = pLinkList->begin(); it != pLinkList->end(); it++)
	{
		sprintf(szBuff, "%s", (*it).szLocalAddr);
		ContentList.push_back(szBuff);

        sprintf(szBuff, "%s", (*it).szModuleLabel);
        ContentList.push_back(szBuff);

        sprintf(szBuff, "%s", (*it).szLinkLabel);
        ContentList.push_back(szBuff);

		//改变状态，当规定时间之内不能收到CMD_GET_APLINK_STATE命令时
		(*it).yStat = GetLinkStat(yApID, *it);

        sprintf(szBuff, "%u", (*it).yStat);
        ContentList.push_back(szBuff);       
       
	}
	

	//把组织的报文发送给客户端
	SendHoriTableReport(pParseResult, TitleList, ContentList, 
							s_PERF_TH_LIST_LINK_STATE);
}

//设置所有接点名称
void CPerf::SetAllAPName(MSG_INTRA *pMsg)
{
    if (pMsg->SenderPid < PT_CO_BASE )
    {
        return;
    }
	//查询接点名称链表，看是否有接点名称信息
	MAP<BYTE, STRING>::iterator it;

    //wangfeng modified for collect
	//BYTE nApNum    =  pMsg->SenderPid - PT_AP_BASE;
	BYTE nApNum    =  pMsg->SenderPid - PT_CO_BASE;
	const char* sApName = (const char*)pMsg->Data;

	it = m_mapAPNumToAPName.find(nApNum);

	//如果映射表没有，就加入名称到接入点号的映射
	if (it == m_mapAPNumToAPName.end())
	{
		m_mapAPNumToAPName[nApNum] = sApName;	
	}
	
}

//处理客户端询问所有接点名称
void CPerf::QueryAllAPName(MSG_INTRA *pMsg)
{
	CParseResult* pParseResult = *((CParseResult** )(pMsg->Data));

	//查询接点名称链表，看是否有接点名称信息
	if (m_mapAPNumToAPName.size() == 0)
	{
		//如果没有，提示错误
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_PERF_TT_FAILED_GETAPNAME);			
	}
	else
	{
		//如果有，则组织所有接点名称报文，返回给客户端	
		
		//获取待发送的表格的标题和内容
		LIST<STRING> TitleList;
		TitleList.push_back(S_PERF_TT_APNAME);

		//根据接入点名称直接获取话单类型(0和1)并填写报告表格的内容
		LIST<STRING> ContentList;
		
		MAP<BYTE, STRING>::iterator it;
		for (it = m_mapAPNumToAPName.begin(); it != m_mapAPNumToAPName.end(); it++)
		{
			ContentList.push_back((*it).second);				

		}	
		
		SendHoriTableReport(pParseResult, TitleList, ContentList, 
							s_PERF_TH_LIST_ALL_APNAME);
	}
}

//根据接点号，模块号和链路号，决定链路计数器值
void CPerf::DecideLinkStat(BYTE nAPId, SLinkStat& link_stat)
{
	LINKCOUNTER_LIST::iterator it;

	BOOL bIsFound = FALSE;
	for (it = m_LinkCounterList.begin(); it != m_LinkCounterList.end(); it++)
	{
		if (((*it).nApID == nAPId) 
            && (strcmp((*it).szModuleLabel, link_stat.szModuleLabel) == 0)
            && (strcmp((*it).szLinkLabel, link_stat.szLinkLabel) == 0))
		{
			bIsFound = TRUE;
            if(link_stat.yStat == ONLINK)
            {
			    //如果找到，此链路的计数器置0
			    (*it).nCounter = 0;
            }
            else
            {
			    (*it).nCounter = m_nLinkMaxCounter + 1;
            }
		}
	}

	//如果没有找到，把这个链路加到链路计数器链表中
	if (!bIsFound)
	{
		SLinkCounter data;
		data.nApID = nAPId;
        strncpy(data.szModuleLabel, link_stat.szModuleLabel, sizeof(data.szModuleLabel));
        strncpy(data.szLinkLabel, link_stat.szLinkLabel, sizeof(data.szLinkLabel));
		data.nCounter  = 0;

		m_LinkCounterList.push_back(data);
	}
}

//根据接点号，模块号，链路号，和规定的计数器决定链路状态
BYTE CPerf::GetLinkStat(BYTE nAPId, SLinkStat& link_stat)
{
	LINKCOUNTER_LIST::iterator it;

	for (it = m_LinkCounterList.begin(); it != m_LinkCounterList.end(); it++)
	{
		if (((*it).nApID == nAPId)
            && (strcmp((*it).szModuleLabel, link_stat.szModuleLabel) == 0)
            && (strcmp((*it).szLinkLabel, link_stat.szLinkLabel) == 0))
		{
			if ((*it).nCounter > m_nLinkMaxCounter)
			{
				//断路
				return OFFLINK;
			}
			else
			{
				//通路
				return ONLINK;
			}			
		}
	}
	return ONLINK;

}


/**********************************************************************
功能描述    发送报告体中只有简单的消息的MML报告
输入参数    szCmd----MML报告对应的命令
            nRetCode----报告的错误码
            szInfo----报告体中的简单消息
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CPerf::SendSimpleInfoReport(const char* szCmd, int nRetCode, 
                                         const char* szInfo) 
{
	m_pMMLReport->MakeReportHead();
	m_pMMLReport->MakeCmdDisplay(szCmd, 908, nRetCode, "TASKCMD");
    m_pMMLReport->AddString(szInfo);
	m_pMMLReport->AddString("\r\n");
    m_pMMLReport->MakeReportTail();
    const char* szReport = m_pMMLReport->GetReport();

    SendMMLReport(szReport);
}

/**********************************************************************
功能描述    发送报告体为一个横向表格的MML报告
输入参数    pParseResult----MML报告解析后的对象
            TitleList----表格的标题
            ContentList----表格的内容
            szTableHeadName----整个表格的名字
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CPerf::SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName)
{
    //由于表格的数据可能会很大，所以必须考虑以分页的方式发送MML报告
    
    const char* szReport;
	m_pMMLReport->MakeReportHead();
	m_pMMLReport->MakeCmdDisplay(pParseResult->GetCmdLine(), 908, 0, "TASKCMD");
    m_pMMLReport->AddHoriTable(TitleList, ContentList, 
                                szTableHeadName);
    m_pMMLReport->MakeReportTail();

    szReport = m_pMMLReport->GetReport();
    SendMMLReport(szReport);

}


/**********************************************************************
功能描述    将MML报告转换成MSG_INTRA的形式发送出去
输入参数    szReport----待发送的MML报告
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CPerf::SendMMLReport(const char* szReport)
{
    int nStrLen = strlen(szReport);

    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_PERF;
	pMsg->ReceiverPid = PT_OM;
	pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_NORM;
    pMsg->ClientNo = m_nCurClientNo;

    memcpy(pMsg->Data, szReport, nStrLen + 1);

    SendMsg(pMsg);
}
void CPerf::MonitorSystem()
{
	//是否发送过告警的标志
	static BOOL bCPUAlarm = FALSE;
	static BOOL bMEMAlarm = FALSE;
    static BOOL bMEMMinorAlarm = FALSE;    
	SOSPerfStat OSPerfStat;
	SInnerAlarm sAlarm;
	int i = 0;
	sAlarm.yAlarmPara[0] = 0xFF;
    m_pOSPerfDB->GetPerfStatistic(OSPerfStat);
	UINT4 nUsedMem = OSPerfStat.uPhyMemTot - OSPerfStat.uPhyMemAva;
	//计算 CPU和Memeory的各种统计值
	m_nSpanCount ++;
	//计算内存的使用数的总和
	m_uMEMTotal += nUsedMem;
	//计算两个CPU的使用数总和
	if(OSPerfStat.CPUPercent != NULL)
	{
		for(i = 0; i < OSPerfStat.nCPUs; i++)
			m_pCPUTotalArray[i] += (unsigned short)(OSPerfStat.CPUPercent[i]);
	}
	//到了计算均值的统计周期
	if(m_nSpanCount >= m_nExSpan)
	{
		//计算期望值
		double uExMemUsed = (double)m_uMEMTotal / (double)m_nExSpan;
		float *pExCPUUsedArray = m_pExCPUUsedArray;
		for(i = 0; i < OSPerfStat.nCPUs; i++)
			pExCPUUsedArray[i] = (float)m_pCPUTotalArray[i] / (float)m_nExSpan;
		//过载告警判断
		//Memeory 过载检测

        //ldf对应问题单SWPD01534 2003-09-10
        float uExMemUsedRate = uExMemUsed / (double)OSPerfStat.uPhyMemTot;
        uExMemUsedRate = uExMemUsedRate * 100;

        if(uExMemUsedRate > m_nMemMaxLimit && !bMEMAlarm)
        {
            //发送告警(major)            
            sAlarm.uAlarmID = ALARM_ID_MEM_REACH_LIMIT;
            sAlarm.yAlarmType = AT_FAULT;
            SendAlarm(&sAlarm);
            bMEMAlarm = TRUE;
        }
        if(uExMemUsedRate > m_nMemMidLimit && uExMemUsedRate <= m_nMemMaxLimit && !bMEMMinorAlarm
           && !bMEMMinorAlarm && !bMEMAlarm)            
        {
            //发送告警(minor)            
            sAlarm.uAlarmID = ALARM_ID_MEM_REACH_MID_LIMIT;
            sAlarm.yAlarmType = AT_FAULT;
            SendAlarm(&sAlarm);
            bMEMMinorAlarm = TRUE;
        }
        if(uExMemUsedRate < m_nMemMinLimit)
        {
            if(bMEMAlarm)
            {
                //发送恢复告警(major)
                sAlarm.uAlarmID = ALARM_ID_MEM_REACH_LIMIT;
                sAlarm.yAlarmType = AT_RESTORE;
                SendAlarm(&sAlarm);
                bMEMAlarm = FALSE;
            }
            if(bMEMMinorAlarm)
            {
                //发送恢复告警(minor)
                sAlarm.uAlarmID = ALARM_ID_MEM_REACH_MID_LIMIT;
                sAlarm.yAlarmType = AT_RESTORE;
                SendAlarm(&sAlarm);
                bMEMMinorAlarm = FALSE;
            }            
        }      

		//CPU过载的判断
		BYTE nAlarmCount = 0, nRestoreCount = 0;
		for(i = 0; i < OSPerfStat.nCPUs; i++)
		{
			if(pExCPUUsedArray[i] >= m_nCPUmaxLimit)
				nAlarmCount ++;
			if(pExCPUUsedArray[i] <= m_nCPUminLimit)
				nRestoreCount ++;
		}
		//必须所有的CPU过载才会告警
		if(nAlarmCount >= OSPerfStat.nCPUs && !bCPUAlarm)
		{
			//发送告警
			sAlarm.uAlarmID = ALARM_ID_CPU_REACH_LIMIT;
			sAlarm.yAlarmType = AT_FAULT;
			SendAlarm(&sAlarm);
			bCPUAlarm = TRUE;
		}
		//必须所有的CPU达到下限才会发送恢复告警
		if(nRestoreCount >= OSPerfStat.nCPUs && bCPUAlarm)
		{
			sAlarm.uAlarmID = ALARM_ID_CPU_REACH_LIMIT;
			sAlarm.yAlarmType = AT_RESTORE;
			SendAlarm(&sAlarm);
			bCPUAlarm = FALSE;
		}
		//使用完毕后所有数据归零
		m_nSpanCount = 0; //计数归零
		m_uMEMTotal = 0;  //Memeory 清零
		for(i = 0; i < OSPerfStat.nCPUs; i++)
			m_pCPUTotalArray[i] = 0;
        
	}
}

//added by maimaoshi,2003-03-14
//查询进程监控
void CPerf::QueryProcStatus(MSG_INTRA* pMsgParsed)
{
	SProcStatus* pData = (SProcStatus*)(pMsgParsed->Data);
    int nCount = (pMsgParsed->Len) / sizeof(SProcStatus);

	//分析命令内容,组装报文	
	CMMLReport MMLReport(S_PERF_TITLE);

    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("LST PROC:",908,0,"TASKCMD");
    MMLReport.AddString(S_PERF_PROCNAME,16,ALIGN_LEFT);//进程名称
    MMLReport.AddString(S_PERFPROCID,16,ALIGN_LEFT);//进程ID
    MMLReport.AddString(S_PERF_PROCSTATUS,16,ALIGN_LEFT);//进程状态

    MMLReport.AddString(STR_NEWLINE);
    MMLReport.AddString(STR_NEWLINE);

    char szTmp[512];

	for(int i = 0; i < nCount; i++)
	{
		//进程名称
		sprintf(szTmp, "%s", pData[i].szProcName);
		MMLReport.AddString(szTmp,32,ALIGN_LEFT);

		//进程类型
		sprintf(szTmp, "%d", pData[i].nPid);
		MMLReport.AddString(szTmp,16,ALIGN_LEFT);

		//进程状态
		sprintf(szTmp, "%s", pData[i].szStatus);
		MMLReport.AddString(szTmp,16,ALIGN_LEFT);

		MMLReport.AddString(STR_NEWLINE);
    
	}

	MMLReport.AddString(STR_NEWLINE);
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportTail();

	char* szReport;
    szReport = new char[strlen(MMLReport.GetReport()) + 1];
    sprintf(szReport,"%s",MMLReport.GetReport());


	//发送到客户端
	int nStrLen = strlen(szReport);
    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid   = GetProcessType();
    pMsg->SenderMid   = GetModuleType();
    pMsg->ReceiverPid = PT_OUTSIDE;
    pMsg->ReceiverMid = MT_MMLDEBUG;

    strcpy(pMsg->UserName,pMsgParsed->UserName);
    
    pMsg->AppType = APP_MML_NORM;
    pMsg->ClientNo = m_nCurClientNo;
    pMsg->IPAddr      = pMsgParsed->IPAddr;
    pMsg->CmdCode     = CMD_MML;

    memcpy(pMsg->Data, szReport, nStrLen + 1);
	delete[] szReport;

    SendMsg(pMsg);
}
void CPerf::CreateAlarm(CParseResult *pParse)
{
    int nAlarmID = 0, nAlarmType = AT_EVENT, nAlarmPara = 0xFF;
    char szInfo[256];
    SInnerAlarm sAlarm;
    
    pParse->GetDigitVal(&nAlarmID,1);
    pParse->GetDigitVal(&nAlarmType,2);
    if (pParse->GetDigitVal(&nAlarmPara,3))
    {
        sAlarm.yAlarmPara[0] = nAlarmPara;
    }
    else
	{
        sAlarm.yAlarmPara[0] = 0xFF;
	}

    sAlarm.uAlarmID = nAlarmID;
    sAlarm.yAlarmType = nAlarmType;
    SendAlarm(&sAlarm);
    if(sAlarm.yAlarmType == AT_EVENT)
    {
        strcpy(szInfo,"Send Event Alarm(ID = %d,Para = %d)");
    }
    else if(sAlarm.yAlarmType == AT_FAULT)
    {
        strcpy(szInfo,"Send Fault Alarm(ID = %d,Para = %d)");
    }
    else if(sAlarm.yAlarmType = AT_RESTORE)
    {
        strcpy(szInfo,"Send Restore Alarm(ID = %d,Para = %d)");
    }   
    MSGOUT(MTS_COMM, MSGOUT_LEVEL_URGENT,szInfo, sAlarm.uAlarmID,sAlarm.yAlarmPara);
    TRACE(MTS_COMM, szInfo, sAlarm.uAlarmID,sAlarm.yAlarmPara);
}

