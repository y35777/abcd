/**************************************************************************
 *��Ʒ��    : iGateway Bill V200        
 *ģ����    : ����ģ��(Perf)
 *�ļ���    : perf.cpp
 *������Ϣ  : ���ļ�����CPerf��ʵ��
 *�汾˵��  : V200R001i00M00B00D1130A   
 *��Ȩ��Ϣ  : ��Ȩ����(c) 2001-2002 ��Ϊ�������޹�˾
 *����      : ������,������
 *����ʱ��  : 2001-11-05
 *�޸ļ�¼  : 
 **************************************************************************/

#include "../include/toolbox.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "perf.h"
#include "resource.h"

/**************************************************************************
����ԭ��    public: CPerf(void);
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    public: ~CPerf( );
��������    ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    protected: MODULE_TYPE GetModuleType(void);
��������    ��ñ�ģ�������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
MODULE_TYPE CPerf::GetModuleType(void)
{
    return MT_PERF;
}

/**************************************************************************
����ԭ��    protected: int InitInstance(void);
��������    ��ʼ��
�������    ��
�������    ��
����ֵ      ��ʼ���ɹ��򷵻�ERR_SUCCESS
�׳��쳣    ��
***************************************************************************/
int CPerf::InitInstance(void)
{
    int nRet = 0;

    if((nRet = CWorkModule::InitInstance()) != ERR_SUCCESS)
    {
        return nRet;
    }

    // ����ͳ��ʱ����
    UINT4 uPerfInterval = 0;

    // �������ļ�
    char *szConfigPath = const_cast <char *>(GetCfgFilePath());
    CINIFile ConfigFile(szConfigPath);
    if(ConfigFile.Open())
    {
        // �����ÿ����ȡ�����ļ���ŵĸ�·��
        char temp[MAX_PATH];

        ConfigFile.GetString(CFG_SEC_PERF,
                             CFG_PERF_ROOT_DIR, 
                             CFG_PERF_ROOT_DIR_DEFAULT,
                             temp,
                             MAX_PATH);
        m_szBasePath = temp;

        // �����ÿ����ȡ�����ļ���������
        m_uSavedDays = ConfigFile.GetInt(CFG_SEC_PERF, 
                                         CFG_PERF_SAVE_DAY, 
                                         CFG_PERF_SAVE_DAY_DEFAULT);

        // ��ȡ��С�澯�������ͱ��ݿռ�����
        // ����·����CFG_SEC_DISK_FILE�ζ�ȡ
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

        // �����ÿ����ȡǰ���̡�����̺ͱ��ݽ���·��
        // ����·����CFG_SEC_DISK_FILE�ζ�ȡ

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

        // �����ÿ����ȡ�������
        // ���������CFG_SEC_COMMON�ζ�ȡ
        m_uAPNum = ConfigFile.GetInt(CFG_SEC_COMMON,
                                     CFG_COMMON_KEY_APCOUNT,
                                     CFG_COMMON_KEY_APCOUNT_DEFAULT);

        //��ȡ����״̬��鶨ʱ��ʱ��
        uPerfInterval = ConfigFile.GetInt(CFG_SEC_PERF, 
                                          CFG_PERF_STAT_INTERVAL,
                                          CFG_PERF_STAT_INTERVAL_DEFAULT);

        //��ȡ��������Ϣ
        m_uBillsvrNo = ConfigFile.GetInt(CFG_SEC_COMMON,CFG_SERVERNO,0);

		//������ 2002-12-07 
	    //����ͳ������
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

		//CPU���ص�����
		m_nCPUmaxLimit = ConfigFile.GetInt(CFG_SEC_PERF,
                                           CFG_PERF_CPU_MAX_LIMIT,
                                           CFG_PERF_CPU_MAX_LIMIT_DEFAULT);
		if(m_nCPUmaxLimit > 90 || m_nCPUmaxLimit < 70)
		{
			m_nCPUmaxLimit = CFG_PERF_CPU_MAX_LIMIT_DEFAULT;
		}
		//CPU���ص�����
		m_nCPUminLimit = ConfigFile.GetInt(CFG_SEC_PERF,
                                           CFG_PERF_CPU_MIN_LIMIT,
                                           CFG_PERF_CPU_MIN_LIMIT_DEFAULT);
		if(m_nCPUminLimit > 50 || m_nCPUminLimit < 20)
		{
			m_nCPUminLimit = CFG_PERF_CPU_MIN_LIMIT_DEFAULT;
		}

        //Memeory��Ҫ�澯�ķ�ֵ
        m_nMemMaxLimit = ConfigFile.GetInt(CFG_SEC_PERF,
                                           CFG_PERF_MEM_MAX_LIMIT,
                                           CFG_PERF_MEM_MAX_LIMIT_DEFAULT);
        //ldf��Ӧ���ⵥSWPD01534 2003-09-10 
        if(m_nMemMaxLimit > 90 || m_nMemMaxLimit < 70)
        {
            m_nMemMaxLimit = CFG_PERF_MEM_MAX_LIMIT_DEFAULT;
        }
        
        //memrory��Ҫ�澯�ķ�ֵ
        m_nMemMidLimit = ConfigFile.GetInt(CFG_SEC_PERF,
            CFG_PERF_MEM_MID_LIMIT,
            CFG_PERF_MEM_MID_LIMIT_DEFAULT);
        if(m_nMemMidLimit > 80 || m_nMemMidLimit < 50)
        {
            m_nMemMidLimit = CFG_PERF_MEM_MID_LIMIT_DEFAULT;
        }
        
        //Memeory�澯�ظ��ķ�ֵ
        m_nMemMinLimit = ConfigFile.GetInt(CFG_SEC_PERF,
            CFG_PERF_MEM_MIN_LIMIT,
            CFG_PERF_MEM_MIN_LIMIT_DEFAULT);
        if(m_nMemMinLimit > 50 || m_nMemMinLimit < 20)
        {
            m_nMemMinLimit = CFG_PERF_MEM_MIN_LIMIT_DEFAULT;
        }
        //�޸����
        
		//�����ֵ����
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

        //������ ���ӽ���
        // �ر������ļ�
        ConfigFile.Close();
        m_CollectStatistic.Init(m_szFrontSavePath.c_str());
    }
    else
    {
        return ERR_FAIL;
    }
    
// ��Բ�ͬƽ̨�������������ݿ�
#ifdef _PLATFORM_WIN32   
	//������ 2002-04-13 ����
    m_pOSPerfDB = (COSPerfDB *)new CNTPerfDB(m_szFrontSavePath.c_str(),
											 m_szBackSavePath.c_str(),
											 m_szBakDevicePath.c_str());
	CNTPerfDB *pNTPerf = static_cast<CNTPerfDB *>(m_pOSPerfDB);
	if(pNTPerf->Init() == FALSE)
	{
		TRACE(THISMODULE,S_PERF_OSDBINIT_FAIL);
		return ERR_FAIL;
	}
	//����
#else
    m_pOSPerfDB = (COSPerfDB *)new CUNIXPerfDB(m_szFrontSavePath.c_str(),
                                               m_szBackSavePath.c_str());
#endif /* _PLATFORM_WIN32 */    
    
    // ע�ᶨʱ������ʱ�����������
    m_PerfStatID = SetTimer(uPerfInterval * 1000);

    m_ApConnTimer = SetTimer(AP_CONN_STATISTIC_TIMER);
    
	//������ 2002-12-07
    m_nMonitorID = SetTimer(m_nPeriod * 1000);
	//���ӽ���
    // ɾ���޶�����ǰ�����������ļ�
    DeleteOutdatedFile();
    
    // ��ǰ�����ļ�������ʽΪyyyymmdd_orig.prf
    char filename[9];
    time_t curtime = ACE_OS::time(NULL);
    ACE_OS::strftime(filename, 9, "%Y%m%d", ACE_OS::localtime(&curtime));
    
    STRING szPerfFile = MAKE_QUALIFIED_PATH(m_szBasePath, filename);
    szPerfFile += PERF_FILE_SUFFIX;

    //��������ͳ�������ļ����Ŀ¼
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

	//�������¼���ʱ��
    m_Timer1SecId = SetTimer(1000);
	//end by maimaoshi

    sprintf(m_szFileName, "%s%s", szPerfFile.c_str(), GetPerfFileExt());
    if (ERR_SUCCESS != m_pPerfUtil->OpenFile(m_szFileName))
	{
		TRACE(THISMODULE,S_PERF_OPEN_PERF_FAIL,m_szFileName);
		return ERR_FAIL;
	}

	//������ 2002-12-24 ƽ��ҹ
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
    // ��ȡ�洢�豸���ÿռ�
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
    //��������ռ�<2���ĸ澯�ռ�
    else if( 2 * m_uMinDiskHandoverRoom >  m_uMinDiskAlarmRoom * nMaxTotalRoom)
    {
        
        m_uMinDiskAlarmRoom = 2 * m_uMinDiskHandoverRoom / nMaxTotalRoom ;
        m_uMaxDiskAlarmRoom = 2 * m_uMinDiskHandoverRoom / nMaxTotalRoom ;
        TRACE(MTS_PERF,S_PERF_ADJUST_ALARM_SPACE);
        MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_ADJUST_ALARM_SPACE);
    }    

#if defined(_PRODUCT_MOTO) || defined(_PRODUCT_CG)
    //������ ����M2000���ܽӿ�
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
����ԭ��    protected: void ExitInstance(void);
��������    �˳�ʱ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CPerf::ExitInstance(void)
{
    if(m_pOSPerfDB)
    {
        delete m_pOSPerfDB;
        m_pOSPerfDB = NULL;
    }
    
    KillTimer(m_Timer1SecId);
    //������ 2002-12-24 ɾ��CPU���ۼ�ֵ����
	if(m_pCPUTotalArray)
	{
		delete[] m_pCPUTotalArray;
	}
    if(m_pExCPUUsedArray)
        delete[] m_pExCPUUsedArray;

	//���ӽ���
    CWorkModule::ExitInstance();
}

/**************************************************************************
����ԭ��    proteced: void OnTimer(const long nTimerID);
��������    ���ݶ�ʱ��ID�ֱ���ʱ���¼�
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CPerf::OnTimer(const long nTimerID)
{
    static int nM2000StatisticCount = 0;
    char szCurDate[9];
    char szCurPerfFileDate[9];
    
    // ��ȡ��ǰ������;
    time_t curtime = ACE_OS::time(NULL);
    ACE_OS::strftime(szCurDate, 9, "%Y%m%d", ACE_OS::localtime(&curtime));

    
    // ��õ�ǰ�����ļ���������
    ACE_OS::strncpy(szCurPerfFileDate, 
                    m_szFileName + ACE_OS::strlen(m_szFileName) -
                      ACE_OS::strlen(PERF_FILE_SUFFIX) - 8, 8);
    szCurPerfFileDate[8] = '\0';
    //������ �������� 2003-11-19

    // ��ǰ�������Ѵ򿪵��ļ��������ڲ�ͬ
    if(ACE_OS::strcmp(szCurDate,szCurPerfFileDate) > 0)
    {
        // ɾ���޶�����ǰ�����������ļ�
        DeleteOutdatedFile();
        //������ 2003-03-04 ɾ���޶�����ǰ�����������ļ�
        if(NULL != m_pM2000Manager)
        {
            m_pM2000Manager->DelOutDatedPerfFile();
        }
        //���ӽ���

        STRING szPerfFile;

        szPerfFile = MAKE_QUALIFIED_PATH(m_szBasePath, szCurDate);
        szPerfFile += PERF_FILE_SUFFIX;
    
        //���µ��ļ�
        sprintf(m_szFileName, "%s%s", szPerfFile.c_str(), GetPerfFileExt());
        if (ERR_SUCCESS != m_pPerfUtil->OpenFile(m_szFileName))
		{
			TRACE(THISMODULE, S_PERF_OPEN_PERF_FAIL, m_szFileName);
		}
    }

	//by ldf 2004-07-10 ��Ӧ���ⵥSWPD06729
	int nFront = 0,nBack = 0,nBak = 0,nMountLen = 0;
	//end
    if(m_PerfStatID == nTimerID)
    {
        UINT4 uFrontSaveRoom = 0;
        UINT4 uBackSaveRoom  = 0;
        UINT4 uBakDeviceRoom = 0;

        // �������Ҫ����������
        SOSPerfStat OSPerfStat;
        m_pOSPerfDB->GetPerfStatistic(OSPerfStat);


        // ��ȡ�洢�豸���ÿռ�
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

        //��Kת��ΪM.
        float uFrontRoomUsed = 100 * ((float)uFrontSaveRoom / (float)m_nTotalFrontSaveRoom);
        float uBackRoomUsed =  100 * ((float)uBackSaveRoom  / (float)m_nTotalBackSaveRoom);
        float uBakDevRoomUsed =100 * ((float)uBakDeviceRoom / (float)m_nTotalBakDeviceRoom);

        uFrontSaveRoom = (UINT4)(uFrontSaveRoom / 1024);
        uBackSaveRoom = (UINT4)(uBackSaveRoom / 1024);
        uBakDeviceRoom = (UINT4)(uBakDeviceRoom / 1024);

        // ǰ������Ӧ����
        if(uFrontRoomUsed < m_uMinDiskAlarmRoom)
        {
            TRACE(THISMODULE,S_PERF_FRONT_MIN_ROOM_ALM,
                uFrontRoomUsed,m_uMinDiskAlarmRoom);
            MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_FRONT_MIN_ROOM_ALM,
                uFrontRoomUsed,m_uMinDiskAlarmRoom);
            
            // ����ǰ���̡����̿ռ䲻�㡱���ϸ澯��Ϣ
            SInnerAlarm alarm;
            alarm.uAlarmID      = ALARM_ID_DISK_ROOM;      // ���̿ռ䲻��
            alarm.yAlarmType    = AT_FAULT;                // ���ϸ澯
            alarm.yAlarmPara[0] = ALARM_PARA_FRONT_SAVE;   // ǰ����
            SendAlarm(&alarm);
            m_bFrontSaveAlarm = TRUE;

            if(uFrontSaveRoom < m_uMinDiskHandoverRoom)
            {
                TRACE(THISMODULE, S_PERF_FRONT_HV_ROOM_ALM,
                    uFrontSaveRoom,m_uMinDiskHandoverRoom);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_FRONT_HV_ROOM_ALM,
                    uFrontSaveRoom,m_uMinDiskHandoverRoom);

                // ��KERNEL��˫���ӿ�ģ�鷢�͵�������
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

            // ����ǰ���̡����̿ռ䲻��(minor)�����ϸ澯��Ϣ
            SInnerAlarm alarm;
            alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // ���̿ռ䲻��
            alarm.yAlarmType    = AT_FAULT;                // ���ϸ澯
            alarm.yAlarmPara[0] = ALARM_PARA_FRONT_SAVE;   // ǰ����            
            SendAlarm(&alarm);
            m_bFrontSaveMinorAlarm = TRUE;
        }
        
        if(uFrontRoomUsed > m_uMaxDiskAlarmRoom)
        {
            if(m_bFrontSaveHandover)
            {                               
                // �첽֪ͨ˫���ӿ�ģ��ǰ���̿ռ�ָ�
                m_bFrontSaveHandover = FALSE;
                MSG_INTRA* pMsg   = new MSG_INTRA;                
                pMsg->SenderPid   = GetProcessType();
                pMsg->SenderMid   = GetModuleType();
                pMsg->ReceiverPid = PT_KERNEL;              // ���͸�KERNEL����
                pMsg->ReceiverMid = MT_CLSTRINTERFACE;      // ���͸�˫���ӿ�ģ��
                pMsg->CmdCode     = CMD_DISKSPACERECOVERY;  // ���̿ռ�ָ�
                pMsg->Data[0] = (BYTE) ERROR_NOENOUGHDISK_PRIMARY;
                SendMsg(pMsg);
            }

            if(m_bFrontSaveAlarm)
            {               
                // ����ǰ���̡����̿ռ䲻�㡱�ָ��澯��Ϣ
                m_bFrontSaveAlarm = FALSE;
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM;      // ���̿ռ䲻��ָ�
                alarm.yAlarmType    = AT_RESTORE;              // �ָ��澯
                alarm.yAlarmPara[0] = ALARM_PARA_FRONT_SAVE;   // ǰ����
                SendAlarm(&alarm);               
            }

            if(m_bFrontSaveMinorAlarm)
            {
                // ����ǰ���̡����̿ռ䲻��(minor)���ָ��澯��Ϣ
                m_bFrontSaveMinorAlarm = FALSE;
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // ���̿ռ䲻��ָ�
                alarm.yAlarmType    = AT_RESTORE;              // �ָ��澯
                alarm.yAlarmPara[0] = ALARM_PARA_FRONT_SAVE;   // ǰ����                    
                SendAlarm(&alarm);
            }
        }   

        // �������Ӧ����
        if(uBackRoomUsed < m_uMinDiskAlarmRoom)
        {
            TRACE(THISMODULE, S_PERF_BACK_MIN_ROOM_ALM,
                uBackRoomUsed,m_uMinDiskAlarmRoom);
            MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_BACK_MIN_ROOM_ALM,
                uBackRoomUsed,m_uMinDiskAlarmRoom);

            // ��������̡����̿ռ䲻�㡱���ϸ澯��Ϣ
            SInnerAlarm alarm;
            alarm.uAlarmID       = ALARM_ID_DISK_ROOM;      // ���̿ռ䲻��
            alarm.yAlarmType     = AT_FAULT;                // ���ϸ澯
            alarm.yAlarmPara[0]  = ALARM_PARA_BACK_SAVE;    // �����
            SendAlarm(&alarm);
            m_bBackSaveAlarm = TRUE;

            if(uBackSaveRoom < m_uMinDiskHandoverRoom)
            {
                TRACE(THISMODULE, S_PERF_BACK_HV_ROOM_ALM,
                    uBackSaveRoom,m_uMinDiskHandoverRoom);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_BACK_HV_ROOM_ALM,
                    uBackSaveRoom,m_uMinDiskHandoverRoom);

                // ��KERNEL��˫���ӿ�ģ�鷢�͵�������
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
            
            // ����ǰ���̡����̿ռ䲻��(minor)�����ϸ澯��Ϣ
            SInnerAlarm alarm;
            alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // ���̿ռ䲻��
            alarm.yAlarmType    = AT_FAULT;                // ���ϸ澯
            alarm.yAlarmPara[0] = ALARM_PARA_BACK_SAVE;   // ǰ����            
            SendAlarm(&alarm);
            m_bBackSaveMinorAlarm = TRUE;              
        }
        
        if(uBackRoomUsed > m_uMaxDiskAlarmRoom)
        {   
            if(m_bBackSaveHandover)
            {               
                // �첽֪ͨ˫���ӿ�ģ�����̿ռ�ָ�
                m_bBackSaveHandover = FALSE;
                MSG_INTRA* pMsg   = new MSG_INTRA;                
                pMsg->SenderPid   = GetProcessType();
                pMsg->SenderMid   = GetModuleType();
                pMsg->ReceiverPid = PT_KERNEL;              // ���͸�Cluster����
                pMsg->ReceiverMid = MT_CLSTRINTERFACE;      // ���͸�Agentģ�� 
                pMsg->CmdCode     = CMD_DISKSPACERECOVERY;  // ���̿ռ�ָ�
                pMsg->Data[0] = (BYTE)ERROR_NOENOUGHDISK_SECONDARY;                
                SendMsg(pMsg);
            }

            if(m_bBackSaveAlarm)
            {
                // ��������̡����̿ռ䲻�㡱�ָ��澯��Ϣ
                m_bBackSaveAlarm = FALSE;
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM;      // ���̿ռ䲻��ָ�
                alarm.yAlarmType    = AT_RESTORE;              // �ָ��澯
                alarm.yAlarmPara[0] = ALARM_PARA_BACK_SAVE;    // �����
                SendAlarm(&alarm);
            }
            
            if(m_bBackSaveMinorAlarm)
            {
                // ����ǰ���̡����̿ռ䲻��(minor)���ָ��澯��Ϣ
                m_bBackSaveMinorAlarm = FALSE;
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // ���̿ռ䲻��ָ�
                alarm.yAlarmType    = AT_RESTORE;              // �ָ��澯
                alarm.yAlarmPara[0] = ALARM_PARA_BACK_SAVE;   // ǰ����                    
                SendAlarm(&alarm);
            }
        }

        // ���ݽ�����Ӧ����
        if(!m_szBakDevicePath.empty())
        {
            if(uBakDevRoomUsed < m_uMinDiskAlarmRoom)
            {
                TRACE(THISMODULE, S_PERF_BACKUP_MIN_ROOM_ALM,
                    uBakDevRoomUsed,m_uMinDiskAlarmRoom);
                MSGOUT(THISMODULE, MSGOUT_LEVEL_URGENT, S_PERF_BACKUP_MIN_ROOM_ALM,
                    uBakDevRoomUsed,m_uMinDiskAlarmRoom);
                
                // �������ݽ��ʡ����̿ռ䲻�㡱���ϸ澯��Ϣ
                SInnerAlarm alarm;
                alarm.uAlarmID       = ALARM_ID_DISK_ROOM;      // ���̿ռ䲻��
                alarm.yAlarmType     = AT_FAULT;                // ���ϸ澯
                alarm.yAlarmPara[0]  = ALARM_PARA_BACKUP_DEV;    // ���ݽ���
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
                
                // ����ǰ���̡����̿ռ䲻��(minor)�����ϸ澯��Ϣ
                SInnerAlarm alarm;
                alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // ���̿ռ䲻��
                alarm.yAlarmType    = AT_FAULT;                // ���ϸ澯
                alarm.yAlarmPara[0] = ALARM_PARA_BACKUP_DEV;   // ���ݽ���            
                SendAlarm(&alarm);
                m_bBakDeviceMinorAlarm = TRUE;              
            }
            
            if(uBakDevRoomUsed > m_uMaxDiskAlarmRoom)
            {                 
                if(m_bBakDeviceAlarm)
                {
                    // ��������̡����̿ռ䲻�㡱�ָ��澯��Ϣ
                    m_bBackSaveAlarm = FALSE;
                    SInnerAlarm alarm;
                    alarm.uAlarmID      = ALARM_ID_DISK_ROOM;      // ���̿ռ䲻��ָ�
                    alarm.yAlarmType    = AT_RESTORE;              // �ָ��澯
                    alarm.yAlarmPara[0] = ALARM_PARA_BACKUP_DEV;    // ���ݽ���
                    SendAlarm(&alarm);
                }
                
                if(m_bBakDeviceMinorAlarm)
                {
                    // ����ǰ���̡����̿ռ䲻��(minor)���ָ��澯��Ϣ
                    m_bBackSaveMinorAlarm = FALSE;
                    SInnerAlarm alarm;
                    alarm.uAlarmID      = ALARM_ID_DISK_ROOM_MINOR;      // ���̿ռ䲻��ָ�
                    alarm.yAlarmType    = AT_RESTORE;              // �ָ��澯
                    alarm.yAlarmPara[0] = ALARM_PARA_BACKUP_DEV;   // ���ݽ���                    
                    SendAlarm(&alarm);
                }
            }           
        }        
    }

    if(nTimerID == m_ApConnTimer)
        SendConnStat();

	//added by maimaoshi,2002-11-29
	//��ÿ����·���м���
	LINKCOUNTER_LIST::iterator it;
	if(nTimerID == m_Timer1SecId)
    {
		for (it = m_LinkCounterList.begin(); it != m_LinkCounterList.end(); it++)
		{			
			(*it).nCounter++;
			//���������ֵ��Ϊ�˱���������Ѽ���ֵ�õ����ֵ+1������
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
				//by ldf 2004-07-10 ��Ӧ���ⵥSWPD06729
				// ��ȡ�洢�豸���ÿռ�
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
����ԭ��    proteced: void ParseMsg(MSG_INTRA* pMsgParsed);
��������    ��������ģ���յ�����Ϣ
�������    ָ������ģ���յ�����Ϣ��ָ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CPerf::ParseMsg(MSG_INTRA* pMsgParsed)
{    
    switch(pMsgParsed->CmdCode)
    {
        // ��������������������,���а����˸��������������������.
        case CMD_SAVE_PERF:
        {
            //TRACE(THISMODULE, S_PERF_RECV_SAVE_PERF);

            SPerfItem* pData = (SPerfItem*)(pMsgParsed->Data);
            int nCount = (pMsgParsed->Len) / sizeof(SPerfItem);

            SavePerfItem(pData, nCount);

            //���´�������ʵ�����������ͳ��.
            //wangfeng modified for collect
            if(pMsgParsed->SenderPid >= PT_CO_BASE && pMsgParsed->SenderPid <= MAX_PT_CO_TYPE)
            {
                ProcConnectStatus(pMsgParsed->SenderPid - PT_CO_BASE, pData, nCount);
            }
            else if ((pMsgParsed->SenderPid >= PT_AP_BASE) && (pMsgParsed->SenderPid < PT_CO_BASE))//��������
            {
                ProcConnectStatus(pMsgParsed->SenderPid - PT_AP_BASE, pData, nCount);
            }

            if(NULL != m_pM2000Manager)
            {
                //������ 2003-03-04
                m_pM2000Manager->ProcStatistic(pData,nCount);
            }
            break;
        }

        // ����˫��ģ�鶨ʱ���͵�����״̬����.
        case CMD_ANS_HB_STATUS:
        {
            m_perfHeart[0].yAPID = pMsgParsed->SenderPid;
            m_perfHeart[0].yModuleID = pMsgParsed->SenderMid;
            m_perfHeart[0].uID = PERF_HEARTBEAT;
            m_perfHeart[0].timeBegin = ACE_OS::time(NULL);
            m_perfHeart[0].uValue    = pMsgParsed->Data[0];//��������
            m_perfHeart[1].uValue    = pMsgParsed->Data[1];//�������ø���

            break;
        }
		//added by maimaoshi,2002-11-22
		// �ӵ����������ʱ���ͽӵ�����.
        case CMD_AP_NAME:
        {
			SetAllAPName(pMsgParsed);          

            break;
        }
		// ��ģ����ӵ��������ӵ�����
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
		//����kernel����Monitorģ�鷢�͹����Ľ���״̬����
		case CMD_ANS_PROC_STATUS:
		{
			QueryProcStatus(pMsgParsed);
			break;
		}

        case CMD_MML:   // MML����
        {
            //TRACE(THISMODULE, S_PERF_RECV_MML_CMD);

            CParseResult *pParse = *(CParseResult **)pMsgParsed->Data;
            m_nCurClientNo = pMsgParsed->ClientNo;

            // ��ѯ˫������״̬.
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
	            //��KERNEL�����еļ��ģ�鷢�������״̬��Ϣ
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
                    //������ 2003-03-04
                    m_pM2000Manager->Dispatch(pMsgParsed);
                    //����
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
����ԭ��    proteced: void QueryStatus(void);
��������    ������ģ�鴦��״̬��ѯ��״̬
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/    
void CPerf::QueryStatus(MSG_INTRA* pMsg)
{
    // �õ�ϵͳ����Ϣ��MML����
    BYTE nInfo[3];
    nInfo[0] = (BYTE)m_uBillsvrNo;
    nInfo[1] = (BYTE)m_perfHeart[0].uValue;//��������
    nInfo[2] = (BYTE)m_perfHeart[1].uValue;//�������ø���
    
    char *pcMMLReport = NULL;
    m_pOSPerfDB->GetOSStatus(nInfo,3,pcMMLReport);
    
    // ��MML�ͻ��˷���MML����
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
    // ��MML�ͻ��˷���MML����
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
	//����MML����
	/*
	+++    iGateway bill                           2002-07-22 09:18:34
        TASKCMD    #0
        %%  LST LINK:; %%
        RETCODE  =  0    ����ִ�гɹ�   (ע��˴�"="ǰ��Ӧֻ��һ���ո� by ldf 2002.8.13)
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
		//�ѹ�IP��ַ
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
		iGWB����IP
        ----------------------------
        IP=10.11.243.198
		
		IP=10.11.243.197
		  
		IP=10.11.243.196
			(������� = 3)
			---End
		*/
		MMLReport.AddString(S_PERF_IP_LIST_TITLE); //"iGWB����IP" = S_PERF_IGWB_IP
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
		//"�������" == S_PERF_RESULT_CNT
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
	//���ͱ���
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
����ԭ��    proteced: void ProcConnectStatus(const BYTE nAccessPointID, 
                                             SPerfItem* pItems, 
                                             const BYTE nCount);
��������    �����������������������
�������    nAccessPointID: �����ı��
            pItems: SPerfItem�ṹ���飬һ��Ԫ�ر�ʾһ���˿ڵ��������
            nCount: SPerfItem�ṹ������Ԫ�صĸ���
�������    ��
����ֵ      ��
�׳��쳣    ��
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
        //����Ҳ�����Ӧ�������������,��������MAP��
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
        {   //����ҵ���Ӧ��ͳ����,�͸ı���ͳ��ֵ
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
����ԭ��    proteced: void SavePerfItem(SPerfItem* pItems, 
                                        const BYTE nCount);
��������    �������Ը����̵���������
�������    pItems: SPerfItem�ṹ���飬һ��Ԫ�ر�ʾһ����������
            nCount: SPerfItem�ṹ������Ԫ�صĸ���
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
void CPerf::SavePerfItem(SPerfItem* pItems, const BYTE nCount)
{
    if (pItems == NULL)
    {
        return;
    }

    //��������ָ��
    SPerfItem* pOperateObj = NULL;

    //ѭ����������
    for (int i = 0; i < nCount; i++)
    {
        if (ERR_SUCCESS == m_pPerfUtil->AppendPerfItem(*(pItems + i)))
        {
            return;
        }
    }

    //д�뵽�ļ�
	if (ERR_SUCCESS == m_pPerfUtil->Flush())
	{
		return;
	} 
}

/**************************************************************************
����ԭ��    private: int GetPerfFileList ( LIST<STRING>& PerfFileList );
��������    ��ȡ�����ļ���Ŀ¼����������ͳ���ļ�
�������    ��
�������    ����ͳ���ļ��б�
����ֵ      �ɹ��򷵻�ERR_SUCCESS������ERR_FAIL
�׳��쳣    ��
***************************************************************************/
int CPerf::GetPerfFileList(LIST<STRING>& PerfFileList)
{
    // ���������ļ���Ŀ¼����������ͳ���ļ�����
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
����ԭ��    private: int DeleteOutdatedFile(void);
��������    ɾ��ָ������ǰ�����������ļ�
�������    ��
�������    ��
����ֵ      �ɹ��򷵻�ERR_SUCCESS,����ERR_FAIL
�׳��쳣    ��
***************************************************************************/
int CPerf::DeleteOutdatedFile ( void )
{
    LIST<STRING> PerfFileList;
    STRING PerfFile;

    time_t curtime = ACE_OS::time(NULL);
    unsigned long curdate = curtime / (24 * 3600);

    // ��������ļ��б�
    if(GetPerfFileList(PerfFileList) != ERR_SUCCESS)
    {
        return ERR_FAIL;
    }

    // ɾ���޶�����ǰ�����������ļ�
    for(LIST<STRING>::iterator iter = PerfFileList.begin();
                               iter != PerfFileList.end(); iter++)
    {
        PerfFile = MAKE_QUALIFIED_PATH(m_szBasePath, *iter);

        // ��ȡ�ļ�����޸�ʱ��
        ACE_stat f_stat;
        if(ACE_OS::stat(PerfFile.c_str(), &f_stat))
        {
            TRACE(THISMODULE, S_PERF_GET_PERF_STAT_FAIL, PerfFile.c_str());
            continue;
        }

        unsigned long mdate = f_stat.st_mtime / (24 * 3600);

        if((curdate > mdate) && (curdate - mdate > m_uSavedDays))
        {
            // ɾ���������ļ�
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
    /* ������ͳ���Ѿ��ر�,ֱ�ӷ��� */
    if(g_bPerfIterm != TRUE)
        return;

    SPerfItem* pItem = NULL;
    char left[1024];

    /* ���챨���� */
    PERF_MAP::iterator i = m_PerfMap.begin(),
                       e = m_PerfMap.end();
    while(i != e)
    {
        pItem = i->second;
        if(pItem->uID == GTP_ALLGSN_CONNECT)
        {
            /* ���������GSN */

            sprintf(left,S_PERF_GSN_CONNECT,pItem->yAPID,pItem->uValue);
            MessageOut(THISMODULE,MSGOUT_LEVEL_IMPORTANT,left);
        }
        else if(pItem->uID == GTP_ALLPRI_CONNECT)
        {
            /* ���������MEM�� */

            sprintf(left,S_PERF_MSG_PRI_CONNECT,pItem[0].yAPID,pItem[0].uValue);
            MessageOut(THISMODULE,MSGOUT_LEVEL_IMPORTANT,left);

            sprintf(left,S_PERF_MSG_SEC_CONNECT,pItem[1].yAPID,pItem[1].uValue);
            MessageOut(THISMODULE,MSGOUT_LEVEL_IMPORTANT,left);
        }

        i++;
    }
}
//added by maimaoshi,2002-11-22
//����ͻ���ѯ����·״̬
void CPerf::QueryLinkState(MSG_INTRA *pMsg)
{
	CParseResult* pParseResult = *((CParseResult** )(pMsg->Data));

    //��MML�����еõ��������
    STRING sApName = (const char* )pParseResult->GetStringVal(1);

	//ȡ�ýӵ��
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
	
	//���ݽӵ�Ŵ�ȡ����·״̬����
	LINKSTAT_LIST* pLinkList = NULL;

	CLinkStatItem::GetLinkStatData(pLinkList, yApID);

	if ((pLinkList == NULL) || (pLinkList->size() == 0))
	{
		//���û�У���ʾ����
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                             S_PERF_TT_FAILED_GETSTATE);
		return;
	}

	//����У�����֯���нӵ����Ʊ��ģ����ظ��ͻ���	

	//��ȡ�����͵ı��ı��������
	LIST<STRING> TitleList;
	TitleList.push_back(S_PERF_TT_LOCALIP);
	TitleList.push_back(S_PERF_TT_MODELNUM);
	TitleList.push_back(S_PERF_TT_LINKNUM);
	TitleList.push_back(S_PERF_TT_STATE);

	//��ȡ�����͵������б�
	LIST<STRING> ContentList;	
	char szBuff[50];

	//����·״̬������֯��Ϊ����
	for (LINKSTAT_LIST::iterator it = pLinkList->begin(); it != pLinkList->end(); it++)
	{
		sprintf(szBuff, "%s", (*it).szLocalAddr);
		ContentList.push_back(szBuff);

        sprintf(szBuff, "%s", (*it).szModuleLabel);
        ContentList.push_back(szBuff);

        sprintf(szBuff, "%s", (*it).szLinkLabel);
        ContentList.push_back(szBuff);

		//�ı�״̬�����涨ʱ��֮�ڲ����յ�CMD_GET_APLINK_STATE����ʱ
		(*it).yStat = GetLinkStat(yApID, *it);

        sprintf(szBuff, "%u", (*it).yStat);
        ContentList.push_back(szBuff);       
       
	}
	

	//����֯�ı��ķ��͸��ͻ���
	SendHoriTableReport(pParseResult, TitleList, ContentList, 
							s_PERF_TH_LIST_LINK_STATE);
}

//�������нӵ�����
void CPerf::SetAllAPName(MSG_INTRA *pMsg)
{
    if (pMsg->SenderPid < PT_CO_BASE )
    {
        return;
    }
	//��ѯ�ӵ������������Ƿ��нӵ�������Ϣ
	MAP<BYTE, STRING>::iterator it;

    //wangfeng modified for collect
	//BYTE nApNum    =  pMsg->SenderPid - PT_AP_BASE;
	BYTE nApNum    =  pMsg->SenderPid - PT_CO_BASE;
	const char* sApName = (const char*)pMsg->Data;

	it = m_mapAPNumToAPName.find(nApNum);

	//���ӳ���û�У��ͼ������Ƶ������ŵ�ӳ��
	if (it == m_mapAPNumToAPName.end())
	{
		m_mapAPNumToAPName[nApNum] = sApName;	
	}
	
}

//����ͻ���ѯ�����нӵ�����
void CPerf::QueryAllAPName(MSG_INTRA *pMsg)
{
	CParseResult* pParseResult = *((CParseResult** )(pMsg->Data));

	//��ѯ�ӵ������������Ƿ��нӵ�������Ϣ
	if (m_mapAPNumToAPName.size() == 0)
	{
		//���û�У���ʾ����
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 1,
                                 S_PERF_TT_FAILED_GETAPNAME);			
	}
	else
	{
		//����У�����֯���нӵ����Ʊ��ģ����ظ��ͻ���	
		
		//��ȡ�����͵ı��ı��������
		LIST<STRING> TitleList;
		TitleList.push_back(S_PERF_TT_APNAME);

		//���ݽ��������ֱ�ӻ�ȡ��������(0��1)����д�����������
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

//���ݽӵ�ţ�ģ��ź���·�ţ�������·������ֵ
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
			    //����ҵ�������·�ļ�������0
			    (*it).nCounter = 0;
            }
            else
            {
			    (*it).nCounter = m_nLinkMaxCounter + 1;
            }
		}
	}

	//���û���ҵ����������·�ӵ���·������������
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

//���ݽӵ�ţ�ģ��ţ���·�ţ��͹涨�ļ�����������·״̬
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
				//��·
				return OFFLINK;
			}
			else
			{
				//ͨ·
				return ONLINK;
			}			
		}
	}
	return ONLINK;

}


/**********************************************************************
��������    ���ͱ�������ֻ�м򵥵���Ϣ��MML����
�������    szCmd----MML�����Ӧ������
            nRetCode----����Ĵ�����
            szInfo----�������еļ���Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ��
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
��������    ���ͱ�����Ϊһ���������MML����
�������    pParseResult----MML���������Ķ���
            TitleList----���ı���
            ContentList----��������
            szTableHeadName----������������
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CPerf::SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName)
{
    //���ڱ������ݿ��ܻ�ܴ����Ա��뿼���Է�ҳ�ķ�ʽ����MML����
    
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
��������    ��MML����ת����MSG_INTRA����ʽ���ͳ�ȥ
�������    szReport----�����͵�MML����
�������    ��
����ֵ      ��
�׳��쳣    ��
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
	//�Ƿ��͹��澯�ı�־
	static BOOL bCPUAlarm = FALSE;
	static BOOL bMEMAlarm = FALSE;
    static BOOL bMEMMinorAlarm = FALSE;    
	SOSPerfStat OSPerfStat;
	SInnerAlarm sAlarm;
	int i = 0;
	sAlarm.yAlarmPara[0] = 0xFF;
    m_pOSPerfDB->GetPerfStatistic(OSPerfStat);
	UINT4 nUsedMem = OSPerfStat.uPhyMemTot - OSPerfStat.uPhyMemAva;
	//���� CPU��Memeory�ĸ���ͳ��ֵ
	m_nSpanCount ++;
	//�����ڴ��ʹ�������ܺ�
	m_uMEMTotal += nUsedMem;
	//��������CPU��ʹ�����ܺ�
	if(OSPerfStat.CPUPercent != NULL)
	{
		for(i = 0; i < OSPerfStat.nCPUs; i++)
			m_pCPUTotalArray[i] += (unsigned short)(OSPerfStat.CPUPercent[i]);
	}
	//���˼����ֵ��ͳ������
	if(m_nSpanCount >= m_nExSpan)
	{
		//��������ֵ
		double uExMemUsed = (double)m_uMEMTotal / (double)m_nExSpan;
		float *pExCPUUsedArray = m_pExCPUUsedArray;
		for(i = 0; i < OSPerfStat.nCPUs; i++)
			pExCPUUsedArray[i] = (float)m_pCPUTotalArray[i] / (float)m_nExSpan;
		//���ظ澯�ж�
		//Memeory ���ؼ��

        //ldf��Ӧ���ⵥSWPD01534 2003-09-10
        float uExMemUsedRate = uExMemUsed / (double)OSPerfStat.uPhyMemTot;
        uExMemUsedRate = uExMemUsedRate * 100;

        if(uExMemUsedRate > m_nMemMaxLimit && !bMEMAlarm)
        {
            //���͸澯(major)            
            sAlarm.uAlarmID = ALARM_ID_MEM_REACH_LIMIT;
            sAlarm.yAlarmType = AT_FAULT;
            SendAlarm(&sAlarm);
            bMEMAlarm = TRUE;
        }
        if(uExMemUsedRate > m_nMemMidLimit && uExMemUsedRate <= m_nMemMaxLimit && !bMEMMinorAlarm
           && !bMEMMinorAlarm && !bMEMAlarm)            
        {
            //���͸澯(minor)            
            sAlarm.uAlarmID = ALARM_ID_MEM_REACH_MID_LIMIT;
            sAlarm.yAlarmType = AT_FAULT;
            SendAlarm(&sAlarm);
            bMEMMinorAlarm = TRUE;
        }
        if(uExMemUsedRate < m_nMemMinLimit)
        {
            if(bMEMAlarm)
            {
                //���ͻָ��澯(major)
                sAlarm.uAlarmID = ALARM_ID_MEM_REACH_LIMIT;
                sAlarm.yAlarmType = AT_RESTORE;
                SendAlarm(&sAlarm);
                bMEMAlarm = FALSE;
            }
            if(bMEMMinorAlarm)
            {
                //���ͻָ��澯(minor)
                sAlarm.uAlarmID = ALARM_ID_MEM_REACH_MID_LIMIT;
                sAlarm.yAlarmType = AT_RESTORE;
                SendAlarm(&sAlarm);
                bMEMMinorAlarm = FALSE;
            }            
        }      

		//CPU���ص��ж�
		BYTE nAlarmCount = 0, nRestoreCount = 0;
		for(i = 0; i < OSPerfStat.nCPUs; i++)
		{
			if(pExCPUUsedArray[i] >= m_nCPUmaxLimit)
				nAlarmCount ++;
			if(pExCPUUsedArray[i] <= m_nCPUminLimit)
				nRestoreCount ++;
		}
		//�������е�CPU���زŻ�澯
		if(nAlarmCount >= OSPerfStat.nCPUs && !bCPUAlarm)
		{
			//���͸澯
			sAlarm.uAlarmID = ALARM_ID_CPU_REACH_LIMIT;
			sAlarm.yAlarmType = AT_FAULT;
			SendAlarm(&sAlarm);
			bCPUAlarm = TRUE;
		}
		//�������е�CPU�ﵽ���޲Żᷢ�ͻָ��澯
		if(nRestoreCount >= OSPerfStat.nCPUs && bCPUAlarm)
		{
			sAlarm.uAlarmID = ALARM_ID_CPU_REACH_LIMIT;
			sAlarm.yAlarmType = AT_RESTORE;
			SendAlarm(&sAlarm);
			bCPUAlarm = FALSE;
		}
		//ʹ����Ϻ��������ݹ���
		m_nSpanCount = 0; //��������
		m_uMEMTotal = 0;  //Memeory ����
		for(i = 0; i < OSPerfStat.nCPUs; i++)
			m_pCPUTotalArray[i] = 0;
        
	}
}

//added by maimaoshi,2003-03-14
//��ѯ���̼��
void CPerf::QueryProcStatus(MSG_INTRA* pMsgParsed)
{
	SProcStatus* pData = (SProcStatus*)(pMsgParsed->Data);
    int nCount = (pMsgParsed->Len) / sizeof(SProcStatus);

	//������������,��װ����	
	CMMLReport MMLReport(S_PERF_TITLE);

    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("LST PROC:",908,0,"TASKCMD");
    MMLReport.AddString(S_PERF_PROCNAME,16,ALIGN_LEFT);//��������
    MMLReport.AddString(S_PERFPROCID,16,ALIGN_LEFT);//����ID
    MMLReport.AddString(S_PERF_PROCSTATUS,16,ALIGN_LEFT);//����״̬

    MMLReport.AddString(STR_NEWLINE);
    MMLReport.AddString(STR_NEWLINE);

    char szTmp[512];

	for(int i = 0; i < nCount; i++)
	{
		//��������
		sprintf(szTmp, "%s", pData[i].szProcName);
		MMLReport.AddString(szTmp,32,ALIGN_LEFT);

		//��������
		sprintf(szTmp, "%d", pData[i].nPid);
		MMLReport.AddString(szTmp,16,ALIGN_LEFT);

		//����״̬
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


	//���͵��ͻ���
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

