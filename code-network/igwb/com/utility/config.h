 /*
    �ļ���:config.h
    �汾:0.1
    ����:2001-10-11

    ���ļ��ж�����ϵͳ��ʹ�����ò�����ȱʡֵ.
*/

#ifndef __CONFIG_DEFINE_H__
#define __CONFIG_DEFINE_H__

#ifdef _PLATFORM_WIN32
#include "../include/product.h"
#endif //_PLATFORM_WIN32

#define CFG_SEC_COMMON                              "Common"
#define CFG_COMMON_KEY_APCOUNT                      "APCount"
#define CFG_COMMON_KEY_APCOUNT_DEFAULT              0
#define CFG_COMMON_KEY_MSGSVR_PORT                  "LocalPortToMsg"
#define CFG_COMMON_KEY_MSGSVR_PORT_DEFAULT          9999
#define CFG_COMMON_KEY_NO_CLUSTER                   "NOCluster"
#define CFG_COMMON_KEY_NO_CLUSTER_DEFAULT           -1
#define CFG_COMMON_KEY_CLUSTER_MODE                 "ClusterMode"
#define CFG_COMMON_KEY_CLUSTER_MODE_TEST            -1
#define CFG_COMMON_KEY_CLUSTER_MODE_DEFAULT         1
#define CFG_COMMON_KEY_MSGOUTLEVEL                  "MSGOUTLevel"                   //������ 2002-03-13 ���ⵥD11728
#define CFG_USE_XML                                 "UseXml"
#define CFG_USE_XML_DEFAULT                         0
#define CFG_XML_FONT_SET                            "XmlFontSet"
#define CFG_XML_FONT_SET_DEFAULT                    "ISO8859-1"
#define CFG_REUSE_ADDRESS                           "ReuseAddr"
#define CFG_REUSE_ADDRESS_DEFAULT                   1

#ifdef  _PRODUCT_MOTO
//by ldf ��Ӧ���ⵥ D20358
#define CFG_AP_KEY_USEIN_CMCC                       "UseInCMCC"
#define CFG_AP_KEY_USEIN_CMCC_DEFAULT               1
//end
#endif //_PRODUCT_MOTO

//������ 2002-12-25 ����IP��ַ����
#define CFG_AP_KEY_IP_MODE                          "IpFilterMode"
#define CFG_AP_KEY_IP_MODE_DEFAULT                  2
#define CFG_AP_IP_LIST_NUM                          "IPNumber"
#define CFG_AP_IP_LIST_PREFIX                       "IP"
//������
//��������Ϣ

#define CFG_SERVERNO                                "ServerNo"

#define CFG_SEC_CHANNEL                             "Channel%d-%d"
#define CFG_SEC_AP                                  "AccessPoint"
#define CFG_AP_KEY_APTYPE                           "APType"

#ifdef  _PRODUCT_CCS
#define CFG_AP_KEY_APTYPE_DEFAULT                   5
#else
#define CFG_AP_KEY_APTYPE_DEFAULT                   -1
#endif

#define CFG_AP_KEY_COLTYPE                          "CollectType"
#define CFG_AP_KEY_COLTYPE_DEFAULT                  0
#define CFG_AP_KEY_MUST_MERGE                       "MustMerge"
#define CFG_AP_KEY_MUST_MERGE_DEFAULT               0
//by ldf ��Ӧ���ⵥ D20358
#define CFG_AP_KEY_USEIN_CMCC                       "UseInCMCC"
#define CFG_AP_KEY_USEIN_CMCC_DEFAULT               1
//end

#define CFG_AP_KEY_FINAL_SPFILEFMT                  "FinalSPFileFmt"
#define CFG_AP_KEY_FINAL_SPFILEFMT_DEFAULT          1
#define CFG_SUPPORT_DST                             "SupportDST"
#define CFG_SUPPORT_DST_DEFAULT                     0
#define CFG_MUST_CONVERT                            "MustConvert"
#define CFG_MUST_CONVERT_DEFAULT                    1
#define CFG_MAX_WAIT_USERS                          "MaxWaitUsrs"
#define CFG_MAX_WAIT_USERS_DEFAULT                  200000
#define CFG_MAX_WAIT_PACKAGES                       "MaxWaitPackages"
#define CFG_MAX_WAIT_PACKAGES_DEFAULT               10800
//Added by ZhengYuqun 2003-11-04 SWPD02238
#define CFG_MAX_DURATION                            "MaxDuration"
#define CFG_MAX_DURATION_DEFAULT                    10800
//End
#define CFG_PROC_BILL_CSN                           "ProcBillCSN"
#define CFG_PROC_BILL_CSN_DEFAULT                   0

#define CFG_CHECK_SUM_POS                           "CheckSumPos"
#define CFG_CHECK_SUM_POS_DEFAULT                   0
#define CFG_CHECK_SUM_RANGE                         "CheckSumRange"
#define CFG_CHECK_SUM_RANGE_DEFAULT                 ""


#define CFG_AP_NAME                                 "APName"
#define CFG_CSN_LEN                                 "CsnLen"

#ifdef  _PRODUCT_MOTO
#define CFG_CSN_LEN_DEFAULT                         6
#else
#define CFG_CSN_LEN_DEFAULT                         8
#endif

#define CFG_START_CSN                               "StartCsn"
#define CFG_START_CSN_DEFAULT                       1
#define CFG_END_CSN                                 "EndCsn"
#define CFG_CSN_STEP                                "CsnStep"
#define CFG_CSN_STEP_DEFAULT                        1
#define CFG_PREFIX                                  "Prefix"
#define CFG_PREFIX_DEFAULT                          "b"
#define CFG_POSTFIX                                 "Postfix"
#define CFG_POSTFIX_DEFAULT                         "dat"
#define CFG_MIDFIX                                  "Infix"
#define CFG_MIDFIX_DEFAULT                          ""
#define CFG_MAX_FLUSH_SPAN                          "MaxFlushSpan"
#define CFG_MAX_FLUSH_SPAN_DEFAULT                  0
#define CFG_SAVE_SECOND                             "SaveSecond"
#define CFG_SAVE_SECOND_DEFAULT                     0
#define CFG_HOT_SAVE_SECOND                         "HotSaveSecond"
#define CFG_HOT_SAVE_SECOND_DEFAULT                 0
#define CFG_CHECK_LOG_PREFIX                        "CheckLogPrefix"
#define CFG_CHECK_LOG_PREFIX_DEFAULT                "%Y%m%d"
#define CFG_CHECK_LOG_POSTFIX                       "CheckLogPostfix"
#define CFG_CHECK_LOG_POSTFIX_DEFAULT               ".log"


//by ldf 2004-02-24 �������ּ�����
#define CFG_DISPATCH_TYPE                           "DispatchType"
#define CFG_DISPATCH_DEFAULT                        ""
#define CFG_DISPATCH_ALLCDR_CH                      "DispatchAllCDRCH"
#define CFG_DISPATCH_ALLCDR_CH_DEFAULT              1
//end

//by yg ����ּ�
#define CFG_REDISTRIBUTE                             "redistribute"

#define CFG_AP_KEY_PACKETLOWLIMIT                   "PacketLowLimit"
#define CFG_AP_KEY_PACKETLOWLIMIT_DEFAULT           200
#define CFG_AP_KEY_PACKETHIGHLIMIT                  "PacketHightLimit"
#define CFG_AP_KEY_PACKETHIGHLIMIT_DEFAULT          400
#define CFG_AP_KEY_REDIRECTION_ADDR                 "RedirAddress"
#define CFG_AP_KEY_REDIRECTION_ADDR_DEFAULT         "0.0.0.0"
#define CFG_AP_KEY_LOCALPORTTOEX                    "LocalPortToEx"
#define CFG_AP_KEY_LOCALPORTTOEX_DEFAULT            3386

#define CFG_CDR_CHECK_SPAN                          "CDRCheckSpan"
#define CFG_CDR_CHECK_SPAN_DEFAULT                  3                               //���󻰵����ֵļ�����ƣ�Ĭ��ȡ3��
#define CFG_CDR_CHECK_COUNT                         "CDRCheckCount"
#define CFG_CDR_CHECK_COUNT_DEFAULT                 5                               //���󻰵����ִ������ƣ�Ĭ��ȡ5��
#define CFG_CHECK_OM_FIELD                          "CheckOMField"
#define CFG_CHECK_OM_FIELD_DEFAULT                  0
#define CFG_OM_FLD_CFG_FILE                         "OMFieldCfgFile"
#define CFG_OM_FLD_CFG_FILE_DEFAULT                 ""

//Added by ZhengYuqun SWPD04700
#define CFG_CDR_FILTER                              "CDRFilter"
#define CFG_CDR_FILTER_DEFAULT                      "None"

#define CFG_AP_KEY_UDP_TCP_MASK                     "UdpTcpMask"
#define CFG_AP_KEY_UDP_ENABLE                       1
#define CFG_AP_KEY_TCP_ENABLE                       2
#define CFG_AP_KEY_UDP_TCP_MASK_DEFAULT             3                               /*��0λ��ʾUDP����һλ��ʾTCP*/

#define CFG_AP_KEY_LOCAL_IP_TO_PM                   "LocalIpToEx"
#define CFG_AP_KEY_LOCAL_IP_TO_PM_DEFAULT           "0.0.0.0"
#define CFG_AP_KEY_LOCAL_PORT_TO_PM                 "LocalPortToEx"
#define CFG_AP_KEY_LOCAL_PORT_TO_PM_DEFAULT         9900
#define CFG_AP_KEY_LOCAL_PORT_TO_PM_GSN_DEFAULT     3386
#define CFG_AP_KEY_LOCAL_IP_TO_FM                   "LocalIpToExBak"
#define CFG_AP_KEY_LOCAL_IP_TO_FM_DEFAULT           "0.0.0.0"
#define CFG_AP_KEY_LOCAL_IP_TO_FM_GSN_DEFAULT       ""
#define CFG_AP_KEY_LOCAL_PORT_TO_FM                 "LocalPortToExBak"
#define CFG_AP_KEY_LOCAL_PORT_TO_FM_DEFAULT         9900
#define CFG_AP_KEY_LOCAL_PORT_TO_FM_GSN_DEFAULT     3386

#define CFG_AP_KEY_LOCAL_IP_TO_PM_UDP               "LocalIpToExUDP"
#define CFG_AP_KEY_LOCAL_PORT_TO_PM_UDP             "LocalPortToExUDP"
#define CFG_AP_KEY_LOCAL_IP_TO_FM_UDP               "LocalIpToExBakUDP"
#define CFG_AP_KEY_LOCAL_PORT_TO_FM_UDP             "LocalPortToExBakUDP"
#define CFG_AP_KEY_MULTI_IP_GSN_COUNT               "MultiIPGsnCount"
#define CFG_AP_KEY_MULTI_IP_GSN_COUNT_DEFAULT       0
#define CFG_AP_KEY_GSN_IP_TABLE                     "GsnIPTable"
#define GSN_IP_TABLE_IPCOUNT_MAX                    10
#define CFG_AP_KEY_DEL_GSN_IP                       "DelGsnIP"
#define CFG_AP_KEY_DEL_GSN_IP_DEFAULT               ""

#define CFG_AP_KEY_HIS_FRM_COUNT                    "HisFrmCount"
#define CFG_AP_KEY_HIS_FRM_COUNT_DEFAULT            1500
#define CFG_AP_KEY_MAX_PACKAGE_SIZE                 "MaxPackageSize"
#define CFG_AP_KEY_MAX_PACKAGE_SIZE_DEFAULT         5000                            /*��λΪKB*/
#define CFG_AP_KEY_MAX_PACKAGE_SIZE_WLAN_DEFAULT    5000                            /*��λΪ��¼��*/
#define CFG_AP_KEY_MAX_NEW_FRM_COUNT                "MaxNewFrmCount"
#define CFG_AP_KEY_MAX_NEW_FRM_COUNT_DEFAULT        1500
#define CFG_AP_KEY_LOW_AT_TIME                      "LowAtTime"
#define CFG_AP_KEY_LOW_AT_TIME_DEFAULT              1500                            /*��λΪ����*/
#define CFG_AP_KEY_LOW_AT_SIZE                      "LowAtSize"
#define CFG_AP_KEY_LOW_AT_SIZE_DEFAULT              1024                            /*��λΪKB*/
#define CFG_AP_KEY_LOW_AT_SIZE_WLAN_DEFAULT         1024                            /*��λΪ��¼��*/
#define CFG_AP_KEY_MAX_EMPTY_PACKAGE                "MaxEmptyPackage"
#define CFG_AP_KEY_MAX_EMPTY_PACKAGE_DEFAULT        50
#define CFG_AP_KEY_ECHO_REQ_INV                     "EchoReqInv"
#define CFG_AP_KEY_ECHO_REQ_INV_DEFAULT             60                              /*��λΪ��*/
#define CFG_AP_KEY_REQ_TIMEOUT                      "ReqTimeOut"
#define CFG_AP_KEY_REQ_TIMEOUT_DEFAULT              2                               /*��λΪ��*/
#define CFG_AP_KEY_TIMEOUT_RPT                      "TimeOutRpt"
#define CFG_AP_KEY_TIMEOUT_RPT_DEFAULT              5
#define CFG_AP_KEY_NODE_ALIVE_REQ_INV               "NodeAliveReqInv"
#define CFG_AP_KEY_NODE_ALIVE_REQ_INV_DEFAULT       300                             /*��λΪ��*/
#define CFG_AP_KEY_GTP_BUFFER_LIMIT                 "GtpBufLimit"
#define CFG_AP_KEY_GTP_BUFFER_LIMIT_DEFAULT         8192
#define CFG_AP_KEY_ADD_MOD_NUM                      "AddModuleNum"
#define CFG_AP_KEY_ADD_MOD_NUM_DEFAULT              0


//�й�������Ϣ������
#define CFG_SEC_OPERATOR                            "Operator"
#define CFG_OPERATOR_KEY_NAME                       "UserName"
#define S_RESET_DEFAULT                             "_"
//����Զ2002.04.10���
#define CFG_AP_KEY_LINK_TIMEOUT                     "LinkTimeOut"
#define CFG_AP_KEY_LINK_TIMEOUT_DEFAULT             6
#define CFG_AP_KEY_MPU_WINDOW_SIZE                  "MpuWindowSize"
#define CFG_AP_KEY_MPU_WINDOW_SIZE_DEFAULT          150
#define CFG_AP_KEY_BAU_WINDOW_SIZE                  "BauWindowSize"
#define CFG_AP_KEY_BAU_WINDOW_SIZE_DEFAULT          2000
#define CFG_AP_KEY_BILL_REC_SIZE                    "BillRecSize"
#define CFG_AP_KEY_BILL_REC_SIZE_DEFAULT            261
#define CFG_AP_KEY_ADDITION_STACK                   "AdditionStack"
#define CFG_AP_KEY_ADDITION_STACK_DEFAULT           ""
#define CFG_SCAN_STACK_ROOT_DIR                     "ScanRootDir"
#define CFG_SCAN_STACK_ROOT_DIR_DEFAULT             ""
#define CFG_AP_KEY_SCAN_FILE_INV                    "ScanFileInv"
#define CFG_AP_KEY_SCAN_FILE_INV_DEFAULT            100
#define CFG_AP_KEY_FMT_DLL                          "FmtDll"
#define CFG_AP_KEY_FMT_DLL_DEFAULT                  ""
#define CFG_AP_KEY_WLAN_AS_DLL                      "WlanFmtDll"
#define CFG_AP_KEY_WLAN_AS_DLL_DEFAULT              "wlan_as"


//����Զ2002.04.10��ӽ���
//����Զ2002.04.26���
#define CFG_AP_KEY_PROC_ALL_AP_ALARM                "ProcAllAPAlarm"
#define CFG_AP_KEY_PROC_ALL_AP_ALARM_DEFAULT        1
//����Զ2002.04.26��ӽ���
//�йظ澯���������������
#define CFG_AP_KEY_LOCAL_PORT_TO_AB                 "LocalPortToAlarmBox"
#define CFG_AP_KEY_LOCAL_PORT_TO_AB_DEFAULT         6100
#define CFG_AP_KEY_LOCAL_PORT_TO_SC             "LocalPortToSynConf"
#define CFG_AP_KEY_LOCAL_PORT_TO_SC_DEFAULT     6099

//��������������
#define CFG_AP_ALARM_SEND_TIMEOUT                   "SendAlarmTimeOut"
#define CFG_AP_ALARM_SEND_TIMEOUT_DEFAULT           20
//�޸Ľ���
#define CFG_SEC_KERNEL                              "Kernel"
#define CFG_KERNEL_KEY_DEBUGVERSION                 "IsDebug"
#define CFG_KERNEL_KEY_DEBUGVERSION_DEFAULT         0


#define CFG_SEC_OM                                  "OM"
#define CFG_PREPARE_PAGE                            "PreparePage"
#define CFG_PREPARE_PAGE_DEFAULT                    5
#define CFG_PAGE_SIZE                               "PageSize"
#define CFG_PAGE_SIZE_DEFAULT                       50
//����Զ2002.04.03���
#define CFG_DISPLAY_ERR_BILL                        "DisplayErrBill"
#define CFG_DISPLAY_ERR_BILL_DEFAULT                0
//����Զ2002.04.03��ӽ���


#define CFG_SEC_MML                                 "MML"
#define CFG_MML_KEY_LOCAL_IP                        "LocalIpToMMLClient"
#define CFG_MML_KEY_LOCAL_IP_DEFAULT                "0.0.0.0"
#define CFG_MML_FTP_SERVER_IP                   "FtpServerIP"
#define CFG_MML_FTP_USER_NAME                   "FtpUserName"
#define CFG_MML_FTP_USER_PASSWORD               "FtpPassword"
#define CFG_MML_DLD_SOFT                        "DownLoadSoft"

#define CFG_MML_KEY_CM_PORT                         "LocalPortToCM"
#define CFG_MML_KEY_CM_PORT_DEFAULT                 6000
#define CFG_MML_KEY_AR_PORT                         "LocalPortToAR"
#define CFG_MML_KEY_AR_PORT_DEFAULT                 6001
#define CFG_MML_KEY_RD_PORT                         "LocalPortToRD"
#define CFG_MML_KEY_RD_PORT_DEFAULT                 6007
#define CFG_MML_KEY_CS_PORT                         "LocalPortToCS"
#define CFG_MML_KEY_CS_PORT_DEFAULT                 6010
#define CFG_MML_KEY_PF_PORT                         "LocalPortToPF"
#define CFG_MML_KEY_PF_PORT_DEFAULT                 6002
#define CFG_AP_KEY_LOCAL_PORT_TO_SC             "LocalPortToSynConf"
#define CFG_AP_KEY_LOCAL_PORT_TO_SC_DEFAULT     6099

#define CFG_MML_PERSIST_DATA_DEF                    "config/mml"

#define CFG_MML_PERSIST_USER                        "PersistUser"

#define CFG_MML_MAX_USER_NUMBER                     "MaxUserNumber"
#define CFG_MML_MAX_CONNECT_USER                    "MaxConnectUserNumber"
//������ 2002-12-07
#define CFG_MML_MAX_FAIL_COUNT                      "LoginFailCount"
#define CFG_MML_MAX_FAIL_COUNT_DEFAULT              3
//������ 2002-12-07

#define CFG_MML_PWD_TIME_LIMIT                      "PasswordTimeLimit"
#define CFG_MML_PWD_TIME_LIMIT_DEFAULT              30

#define CFG_SEC_ADDPROC                             "AdditionProc"
#define CFG_ADDPROC_SEC_PROCNUM                     "ProcNum"
#define CFG_ADDPROC_SEC_APPNAME                     "AppName"
#define CFG_ADDPROC_SEC_CMDPARA                     "CmdPara"
#define CFG_ADDPROC_SEC_PTYPE                       "PType"


#define CFG_SEC_ALM_AGENT                           "AlarmAgent"
#define CFG_SEC_CLUSTER                             "Cluster"

#define CFG_SEC_DISK_FILE                           "DiskFile"
#define CFG_ORIG_FILE_MAX_SIZE                      "OrigFileMaxSize"
#define CFG_ORIG_FILE_MAX_SIZE_DEFAULT              3072
#define CFG_FINAL_FILE_MAX_SIZE                     "FinalFileMaxSize"
#define CFG_FINAL_FILE_MAX_SIZE_DEFAULT             1024
#define CFG_FINAL_FILE_MAX_TIME                     "FinalFileMaxTime"
#define CFG_FINAL_FILE_MAX_TIME_DEFAULT             1800
#define CFG_FETCH_FILE_TIMEOUT                      "FetchFileTimeOut"
#define CFG_FETCH_FILE_TIMEOUT_DEFAULT              30
#define CFG_FRONT_SAVE_ROOT_DIR                     "FrontSaveRootDir"
#define CFG_BACK_SAVE_ROOT_DIR                      "BackSaveRootDir"
#define CFG_MAIN_STATUS_ROOT_DIR                    "MainStatusRootDir"
#define CFG_BAK_STATUS_ROOT_DIR                     "BakStatusRootDir"
#define CFG_QB_TMP_RESULT_DIR                       "QBTmpResult"

//added by maimaoshi at 2002-09-23
#define CFG_SAVE_DEADLINE_DEL                       "DeadLineOfAutoDel"             //����ɾ������(��)
#define CFG_SAVE_DEADLINE_DEL_DEFAULT               7                               //days
//end

//����ƽ 2004-03-03 ��ӣ�����ӱ�GPRS�ƷѶԽ����⣺
//ǿ�ƽ�AccessPointNameNIת��Ϊ��д
#define CFG_UPPERCASE_APNNI                         "APNIUpperCase"
#define CFG_UPPERCASE_APNNI_DEFAULT                 0
//��ӽ���


//Added by XDB on 2002.11.11
//���Ӳɼ�������·��
#define CFG_COL_ROOT_DIR                            "CollectRootDir"
#define CFG_COT_CFG_PATH                            "CollectCfgPath"
#define CFG_COT_APP_NAME                            "CollectAppName"

#define CFG_SEC_COLLECT                             "Collect"
#define CFG_CCS_CFG_DIR                             "CCSCfgPath"

//���²�����˫������ʹ��

#define CFG_CLUSTER_VER                             "CLUSTVER"                      //SC�İ汾
#define CFG_CLUSTER                                 "CLUST"                         //SC��Ⱥ����
#define CFG_CLUSTER_PATH                            "CLUSTPATH"                     //SC�����װ��
#define CFG_DATA_SERVICE                            "DATASERVICE"                   //�߿������ݷ�����
#define CFG_LOG_HOST                                "LOGHOST"                       //�߼�������
#define CFG_SERVICE_RES                             "SERVICERES"                    //������Դ
#define CFG_VM_TYPE                                 "VMType"                        //������������

#define CFG_FRONT_DG                                "FrontDG"                       //ǰ�������ڵĴ�������
#define CFG_BACK_DG                                 "BackDG"                        //��������ڵĴ�������
#define CFG_FRONT_VOL                               "FrontVolume"                   //ǰ���̵��߼�����
#define CFG_BACK_VOL                                "BackVolume"                    //��������ڵ��߼�����

#define CFG_ALM_INTERVAL                            "AlarmInterval"                 //�澯��ʱ���ʱ����
#define CFG_KNL_ACT_TIMEOUT                         "KnlActTimeout"                 //�ȴ�KERNEL����Ӧ��ʱ
#define CFG_KNL_OFF_TIMEOUT                         "KnlOffTimeout"                 //�ȴ�KERNEL�ر�Ӧ��ʱ
#define CFG_KNL_OFF_INTERVAL                        "KnlOffInterval"                //�ȴ�KERNEL�رռ��ʱ��
#define CFG_KNL_CHECK_INTERVAL                      "KnlCheckInterval"              //���KERNEL��ʱʱ��
#define CFG_KNL_OFF_COUNT                           "KnlOffCount"                   //�ȴ�KENERL�رն�ʱ����


//˫�������ֵ�ȱʡ��������

#define DEFAULT_CLUSTER_PATH                        "/etc/opt/SUNWcluster"          //SCȱʡ��װ·��
#define DEFAULT_CLUSTER_NAME                        "sc-igwb"                       //SC��Ĭ������
#define DEFAULT_CLUSTER_VER                         "3.0"                           //SC�İ汾
#define DEFAULT_DATA_SERVICE                        "igwb"                          //Ĭ�ϸ߿������ݷ�����
#define DEFAULT_LOG_HOST                            "ha-igwb"                       //Ĭ���߼�������
#define DEFAULT_SERVICE_RES                         "FTP"                           //Ĭ�ϵķ�����Դ

#define DEFAULT_FRONT_DG                            "igwbdg"                        //Ĭ��ǰ�������ڵĴ�����
#define DEFAULT_BACK_DG                             "igwbdg"                        //Ĭ�Ϻ�������ڵĴ�����
#define DEFAULT_FRONT_VOL                           "front"                         //Ĭ�Ϻ���̵��߼���
#define DEFAULT_BACK_VOL                            "back"                          //Ĭ�Ϻ���̵��߼���

//����ʱ��Ĭ��ֵ��λ��Ϊ��

#define DEFAULT_ALM_INTERVAL                        300                             //�澯��ʱ���ʱ��Ĭ��ֵ
#define DEFAULT_KNL_ACT_TIMEOUT                     180                             //�ȴ�KERNEL����Ӧ��ʱĬ��ֵ
#define DEFAULT_KNL_OFF_TIMEOUT                     180                             //�ȴ�KERNEL�ر�Ӧ��ʱĬ��ֵ
#define DEFAULT_KNL_OFF_INTERVAL                    30                              //�ȴ�KERNEL�رռ��ʱ��Ĭ��ֵ
#define DEFAULT_KNL_CHECK_INTERVAL                  30                              //���KERNEL��ʱʱ��Ĭ��ֵ
#define DEFAULT_KNL_OFF_COUNT                       3                               //�ȴ�KENERL�رն�ʱ����Ĭ��ֵ

//ȱʡĿ¼����

#ifdef _PLATFORM_WIN32

#define CFG_FRONT_SAVE_ROOT_DIR_DEFAULT             "d:\\frontsave"
#ifdef _PRODUCT_CCS
#define CFG_MAIN_STATUS_ROOT_DIR_DEFAULT            "d:\\frontsave"
#define CFG_BAK_STATUS_ROOT_DIR_DEFAULT             "e:\\backsave"
#define CFG_BACKUP_STATUS_SRC_DIR_DEFAULT           "d:\\other\\backup"
#else  //_PRODUCT_CCS
#define CFG_MAIN_STATUS_ROOT_DIR_DEFAULT            "d:\\StatusFile"
#define CFG_BAK_STATUS_ROOT_DIR_DEFAULT             "e:\\StatusFileB"
#define CFG_BACKUP_STATUS_SRC_DIR_DEFAULT           "d:\\StatusFile\\Status"
#endif  //_PRODUCT_CCS
#define CFG_BACK_SAVE_ROOT_DIR_DEFAULT              "e:\\backsave"
#define CFG_PERF_ROOT_DIR_DEFAULT                   "d:\\other\\perf"
#define CFG_LOG_ROOT_DIR_DEFAULT                    "d:\\other\\log"
#define CFG_BACKUP_DEFBASEPATH                      "d:\\"
#define CFG_QB_TMP_RESULT_DEFAULT                   "d:\\other\\querybrowse"

/*
#define CFG_BACKUP_STATUS_DES_DIR_DEFAULT           "d:\\Status"
*/
#define CFG_BACKUP_STATUS_FILE_DIR_DEFAULT          "d:\\BackupStatus"
#define CFG_MML_PERSIST_USER_DEF                    "d:\\other\\mml"
#define CFG_BACKUP_DEST_ROOT_DIR_DEFAULT            "d:\\frontsave\\backup"         //����·��ȱʡֵ
#define CFG_ALARM_ROOT_DIR_DEFAULT                  "d:\\other\\alarm"
#define CFG_LOG_ARCHIVE_DIR_DEFAULT                 "d:\\other\\log_archive"        //��־�鵵Ĭ��Ŀ¼.
#define CFG_CCS_CFG_DIR_DEFAULT                     "d:\\other\\collect"

//Added by XDB on 2002.11.11
//���Ӳɼ�������·��Ĭ��ֵ
#define CFG_COL_ROOT_DIR_DEFAULT                    "E:\\backsave"
#define CFG_COT_CFG_PATH_DEFAULT                    ""
#define CFG_CDRSTAT_ROOT_DIR_DEFAULT                "d:\\other\\cdrstat"

#else

#define CFG_CDRSTAT_ROOT_DIR_DEFAULT                "/var/other/cdrstat"
#define CFG_FRONT_SAVE_ROOT_DIR_DEFAULT             "/var/frontsave"
#define CFG_MAIN_STATUS_ROOT_DIR_DEFAULT            "/var/frontsave"
#define CFG_BACK_SAVE_ROOT_DIR_DEFAULT              "/var/backsave"
#define CFG_BAK_STATUS_ROOT_DIR_DEFAULT             "/var/backsave"
#ifdef _PRODUCT_CCS
#define CFG_BACKUP_STATUS_SRC_DIR_DEFAULT           "/var/other/backup"
#else  //_PRODUCT_CCS
#define CFG_BACKUP_STATUS_SRC_DIR_DEFAULT           "/var/other/statusfiles"
#endif  //_PRODUCT_CCS
#define CFG_PERF_ROOT_DIR_DEFAULT                   "/var/other/perf"
#define CFG_LOG_ROOT_DIR_DEFAULT                    "/var/other/log"
#define CFG_BACKUP_DEFBASEPATH                      "/mnt/f/backup"
/*
#define CFG_BACKUP_STATUS_DES_DIR_DEFAULT           "/mnt/f/backupstatus"
*/
#define CFG_BACKUP_STATUS_FILE_DIR_DEFAULT          "/var/other/backupstatus"
#define CFG_MML_PERSIST_USER_DEF                    "/var/other/mml"
#define CFG_BACKUP_DEST_ROOT_DIR_DEFAULT            "/var/frontsave/backup"         //����·��ȱʡֵ
#define CFG_LOG_ARCHIVE_DIR_DEFAULT                 "/var/other/log_archive"        //��־�鵵Ĭ��Ŀ¼.
#define CFG_CCS_CFG_DIR_DEFAULT                     "/var/other/collect"
#define CFG_ALARM_ROOT_DIR_DEFAULT                  "/var/other/alarm"
#define CFG_QB_TMP_RESULT_DEFAULT                   "/var/other/querybrowse"

//Added by XDB on 2002.11.11
//���Ӳɼ�����������·��Ĭ��ֵ
#define CFG_COL_ROOT_DIR_DEFAULT                    "/var/backsave"
#define CFG_COT_CFG_PATH_DEFAULT                    ""

#endif

//���²�������־ģ��ʹ��

#define CFG_SEC_LOG                                 "Log"
#define CFG_LOG_SAVE_DAY                            "LogSaveDays"
#define CFG_LOG_SAVE_DAY_DEFAULT                    30                              /* days */
#define CFG_LOG_ROOT_DIR                            "LogRootDir"
#define CFG_LOG_ARCTIME                             "ArchiveTime"
#define CFG_LOG_ARCTIME_DEFAULT                     "02:00:00"                      //��־Ĭ���Զ��鵵ʱ��.
#define CFG_LOG_ARCHIVE_DIR                         "ArchiveDir"                    //��־�鵵Ŀ¼.

//���²���������ģ��ʹ��

#define CFG_SEC_PERF                                "Perf"
#define CFG_PERF_ROOT_DIR                           "PerfRootDir"
#define CFG_PERF_SAVE_DAY                           "PerfSaveDays"
#define CFG_PERF_SAVE_DAY_DEFAULT                   7                               //days
#define CFG_PERF_STAT_INTERVAL                      "StatInterval"                  //ͳ��ʱ��������
//by ldf 2002.4.8 ��Ӧ���ⵥD12434
#define CFG_PERF_STAT_INTERVAL_DEFAULT              180                             //second
#define CFG_PERF_MIN_ALM_ROOM                       "MinDiskAlarmRoom"              //��С�澯�ռ�
#define CFG_PERF_MIN_ALM_ROOM_DEFAULT               5
#define CFG_PERF_MAX_ALM_ROOM                       "MinorDiskAlarmRoom"            //minor�澯�ռ�
#define CFG_PERF_MAX_ALM_ROOM_DEFAULT               15
#define CFG_PERF_MIN_HANDOVER_ROOM                  "MinDiskHandoverRoom"           //��С�����ռ�
#define CFG_PERF_MIN_HANDOVER_ROOM_DEFAULT          400
#define CFG_PERF_MIN_BACKUP_ROOM                    "MinBakDeviceRoom"              //���ݽ��ʵ���С�ռ�
#define CFG_PERF_MIN_BACKUP_ROOM_DEFAULT            50                              //���ݽ��ʵ���С�ռ�
//�޸����
//������ 2002-12-07
#define CFG_PERF_MONITOR_PERIOD                     "MonitorPeriod"
#define CFG_PERF_MONITOR_PERIOD_DEFAULT             60
#define CFG_PERF_CPU_MAX_LIMIT                      "CPUMaxLimit"
#define CFG_PERF_CPU_MAX_LIMIT_DEFAULT              80
#define CFG_PERF_CPU_MIN_LIMIT                      "CPUMinLimit"
#define CFG_PERF_CPU_MIN_LIMIT_DEFAULT              50
#define CFG_PERF_MEM_MAX_LIMIT                      "MemoryMaxLimit"
#define CFG_PERF_MEM_MAX_LIMIT_DEFAULT              80
#define CFG_PERF_MEM_MID_LIMIT                      "MemoryMidLimit"
#define CFG_PERF_MEM_MID_LIMIT_DEFAULT              60
#define CFG_PERF_MEM_MIN_LIMIT                      "MemoryMinLimit"
#define CFG_PERF_MEM_MIN_LIMIT_DEFAULT              50
#define CFG_PERF_EX_SPAN                            "MeanValueSpan"
#define CFG_PERF_EX_SPAN_DEFAULT                    30

#define CFG_PERF_STAT_PERIOD						"PerfStatPeriod"
#define CFG_PERF_STAT_PERIOD_DEFAULT     			10
//���²������ɱ���ģ��ʹ��

#define CFG_SEC_NET_BACKUP                          "NetBackup"
#define CFG_NET_BACKUP                              "BackupTask"
#define CFG_NET_BACKUP_DEL_FILE_DAYS                "FileSaveDays"
#define CFG_NET_BACKUP_DEL_FILE_DAYS_DEFAULT        0
#define CFG_NET_BACKUP_DES_SAVE_DAYS                "DestFilesSaveDays"
#define CFG_NET_BACKUP_DES_SAVE_DAYS_DEFAULT        0
#define CFG_NET_BACKUP_SEARCH_INTERVAL              "SearchFilesInterval"
#define CFG_NET_BACKUP_SEARCH_INTERVAL_DEFAULT      300
#define CFG_NET_BACKUP_SRC_ROOT_DIR                 "SourceDir"
#define CFG_NET_BACKUP_SRC_ROOT_DIR_DEFAULT         CFG_FRONT_SAVE_ROOT_DIR_DEFAULT
#define CFG_NET_BACKUP_DES_ROOT_DIR                 "DestDir"
#define CFG_NET_BACKUP_DES_ROOT_DIR_DEFAULT         CFG_BACKUP_DEST_ROOT_DIR_DEFAULT
#define CFG_BACKUP_DEST_HOST_IP                     "DestHostIP"
#define CFG_BACKUP_DEST_HOST_IP_BAK                 "DestHostIPBak"
#define CFG_BACKUP_DEST_HOST_IP_DEFAULT             "127.0.0.1"
#define CFG_NET_BACKUP_DEL_AFTER_BAK                "DelAfterBak"
#define CFG_NET_BACKUP_DEL_AFTER_BAK_DEFAULT        0
#define CFG_NET_BACKUP_LAST_FILE                    "BackupLastFile"
#define CFG_NET_BACKUP_LAST_FILE_DEFAULT            0
#define CFG_NET_BACKUP_ALARM_TIME_OUT               "BackupTimeout"
#define CFG_NET_BACKUP_ALARM_TIME_OUT_DEFAULT       3000
#define CFG_NET_BACKUP_RETRY_TIMES                  "BackupRetryTimes"
#define CFG_NET_BACKUP_RETRY_TIMES_DEFAULT          2
#define CFG_NET_BACKUP_TASK_COUNT                   "BackupTaskCount"
#define CFG_NET_BACKUP_RECONNECT_INTERVAL           "ReConnectInterval"
#define CFG_NET_BACKUP_RECONNECT_INTERVAL_DEFAULT   300
#define CFG_NET_BACKUP_USER_NAME                    "UserName"
#define CFG_NET_BACKUP_USER_NAME_DEFAULT            "anonymous"
#define CFG_NET_BACKUP_LOCAL_IP                     "LocalIP"
#define CFG_NET_BACKUP_LOCAL_IP_BAK                 "LocalIPBak"
#define CFG_NET_BACKUP_LOCAL_IP_DEFAULT             ""
#define CFG_NET_BACKUP_BEFORE_ENCODE                "BeforeEncode"
#define CFG_NET_BACKUP_BEFORE_ENCODE_DEFAULT        1
#define CFG_NET_BACKUP_FTP_IS_PASSIVE               "FtpIsPassive"
#define CFG_NET_BACKUP_PASSWORD                     "Password"
#define CFG_NET_BACKUP_PASSWORD_DEFAULT             ""
#define CFG_BACKUP_STATUS_FILE_DIR                  "BackupStatusFile"              //����ģ���״̬�ļ����·��
/*
#define CFG_BACKUP_DEADLINE_DEL                     "DeadLineOfAutoDel"             //����ɾ������(��)
#define CFG_BACKUP_DEADLINE_DEL_DEFAULT             15                              //days
*/
#define CFG_BACKUP_STATUS_SRC_DIR                   "StatusBackupSrcDir"            //Ҫ���ݵ�״̬�ļ���Դ·��
/*
#define CFG_BACKUP_STATUS_DES_DIR                   "StatusBackupDesDir"            //Ҫ���ݵ�״̬�ļ���Ŀ��·��
*/
#define CFG_BACKUP_DEST_ROOT_DIR                    CFG_NET_BACKUP_DES_ROOT_DIR
#define CFG_NET_BACKUP_FILE_POOL_ID                 "FilePoolID"
#define CFG_NET_BACKUP_FILE_POOL_ID_DEFAULT         -1
#define CFG_CHECK_LOG_ENABLE                        "CheckLogEnable"
#define CFG_CHECK_LOG_ENABLE_DEFAULT                0
#define CFG_CHECK_TASK_LABEL                        "CheckLabel"
#define CFG_CHECK_TASK_LABEL_DEFAULT                ""
#define CFG_CHECK_LOG_DEST_PATH                     "CheckLogDestDir"
#define CFG_CHECK_LOG_DEST_PATH_DEFAULT             ""

#define CFG_NET_BEGIN_TIME                          "backup_begin_time"
#define CFG_NET_END_TIME                            "backup_end_time"
#define CFG_NET_LIST_TIME                           "backup_list_time"
#define CFG_NET_ONCE_LIST_TIME                      "once_bk_list_time"   //added by chenliangwei, 2003-08-05

#define CFG_NET_BEGIN_TIME_DEFAULT                  "00:00"
#define CFG_NET_END_TIME_DEFAULT                    "23:59"
#define CFG_NET_LIST_TIME_DEFAULT                   ""
#define CFG_NET_ONCE_LIST_TIME_DEFAULT              ""       //added by chenliangwei, 2003-08-05

// �����ɸ澯ģ��ʹ��

#define CFG_SEC_ALARM                               "Alarm"
#define CFG_ALARM_ALMSHIELD                         "AlarmShield"

//by ����	2004-5-19	��Ӧ���ⵥSWPD5267
#define CFG_MML_ALARM_ALMSHIELD						"MMLAlarmShield"
//the end	2004-5-19	��Ӧ���ⵥSWPD5267

#define CFG_ALARM_HISTORY                           "AlarmHistoryPath"
#define CFG_ALARM_CFG                               "AlarmConfigPath"
#define CFG_ALARM_BINALMSEND                        "BinAlarmSend"
#define CFG_ALARM_APID                              "AccessPointID"
#define CFG_ALARM_BASEID                            "BaseID"
#define CFG_ALARM_ROOT_DIR                          "AlarmRootDir"

#define CFG_ALARM_TRACE_DEBUG                       "TRACEDebug"
#define CFG_ALARM_TRACE_DEBUG_DEFAULT               0
#define CFG_ALARM_BOX_PORT_NAME                     "AlarmBoxPortName"

#define CFG_ALARM_HD_MONITOR_PORT                   "HDMonitorPort"
#define CFG_ALARM_HD_MONITOR_PORT_DEFAULT           162

#define CFG_ALARM_AS_SNMP_AGENT                     "AsSnmpAgent"
#define CFG_ALARM_SNMP_AGENT_IP                     "SnmpAgentIp"
#define CFG_ALARM_SNMP_AGENT_PORT                   "SnmpAgentPort"
#define CFG_ALARM_AS_SNMP_AGENT_DEFAULT             0
#define CFG_ALARM_SNMP_AGENT_IP_DEFAULT             "0.0.0.0"
#define CFG_ALARM_SNMP_AGENT_PORT_DEFAULT           161
#define CFG_ALARM_BOX_USING_MODE                    "AlarmBoxUsingMode"
#define BOX_USING_MODE                              0


// ������˫������ģ��ʹ��
#define CFG_CLSTR_MIN_ACTIVE_INV                    "MinActiveInv"
#define CFG_CLSTR_SWITCH_MODE                       "SwitchMode"
#define CFG_CLSTR_MAX_ACTIVE_FAIL_TIMES             "MaxActiveFailTimes"
#define CFG_CLSTR_HEART_BEAT_BROKEN                 "HeartBeatBroken"
#define CFG_CLSTR_INSTALL_DISK_ARRAY                "InstallShareDiskArray"
#define CFG_CLSTR_HEART_BEAT_COUNT                  "HeartBeatCount"
#define CFG_CLSTR_HEART_BEAT_TYPE                   "Type"
#define CFG_CLSTR_HB_NAME                           "Name"
#define CFG_CLSTR_HB_LOCAL_IP                       "LocalIP"
#define CFG_CLSTR_HB_PEER_IP                        "PeerIP"
#define CFG_CLSTR_HB_PORT                           "Port"
#define CFG_CLSTR_RES_COUNT                         "ResourceCount"
#define CFG_CLSTR_RES_TYPE                          "ResType"
#define CFG_CLSTR_STATIC_RES                        "StaticRes"
#define CFG_CLSTR_RES_NAME                          "ResName"
#define CFG_CLSTR_RES_LOCAL_REC                     "LocalRecovery"
#define CFG_CLSTR_RES_ORGINAL_IP                    "OrginalIP"
#define CFG_CLSTR_RES_VIRTUAL_IP                    "VirtualIP"
#define CFG_CLSTR_RES_VIRTUAL_MASK                  "VirtualMask"
#define CFG_CLSTR_RES_SERVICE                       "Service"
#define CFG_CLSTR_RES_APP_PATH                      "AppPath"
#define CFG_CLSTR_RES_VOLUME                        "Volume"
/*
#define CFG_CLSTR_RES_DEVICE_NO                     "DeviceNo"
#define CFG_CLSTR_RES_PARTITION_NO                  "PartitionNo"
*/
#define CFG_CLSTR_RES_USER_CREATE                   "Create"
#define CFG_CLSTR_RES_USER_ACTIVE                   "Active"
#define CFG_CLSTR_RES_USER_DEACTIVE                 "DeActive"
#define CFG_CLSTR_RES_USER_CHECK                    "CheckStatus"
#define CFG_CLSTR_ASYN_ACTIVE_TIME                  "AsynResActiveTimeOut"
#define CFG_CLSTR_ASYN_DEACTIVE_TIME                "AsynResDeActiveTimeOut"
#define CFG_CLSTR_ALARM_ID                          "AlarmId"
#define CFG_CLSTR_ALARM_PARA                        "AlarmPara"
#define CFG_CLSTR_SWITCH_GROUP                      "SwitchGroup"
#define CFG_CHECK_DISCONN                           "CheckDisConn"
#define CFG_CHECK_DISCONN_DEFUALT                   1


#define CFG_CLSTR_HB_TYPE_UDP                       "UDP"
#define CFG_CLSTR_HB_TYPE_COM                       "COM"
#define CFG_CLSTR_RES_LOCAL_REC_DEFAULT             0
#define CFG_CLSTR_ASYN_ACTIVE_TIME_DEFAULT          180
#define CFG_CLSTR_ASYN_DEACTIVE_TIME_DEFAULT        180

//����SCP��maimaoshi,2002-08-08
#define CFG_AP_KEY_CODE_KEY_COUNT                   "CodeKeyCount"
#define CFG_AP_KEY_CODE_KEY_COUNT_DEFAULT           0
#define CFG_AP_KEY_CODE_KEY                         "CodeKey"
#define CFG_AP_KEY_CODE_KEY_DEFAULT                 ""
#define CFG_AP_KEY_NI_COUNT                         "NICount"
#define CFG_AP_KEY_NI_COUNT_DEFAULT                 0
#define CFG_AP_KEY_DCP                              "DPC"
#define CFG_AP_KEY_DCP_DEFAULT                      ""

//���ڼƷ�ϵͳ����,maimaoshi,2003-01-12
#define CFG_BACK_SAVE_PATCH_NAME                    "BackSavePatchName"
#define CFG_BACK_SAVE_PATCH_NAME_DEFAULT            ""
#define CFG_AP_KEY_GSNIP                            "GsnIp"
#define CFG_AP_KEY_GSNIP_DEFAULT                    "127.0.0.1"


#define CFG_AP_KEY_RELEASE_VERSION                  "Release"
#define CFG_AP_KEY_RELEASE_VERSION_DEFAULT          "R98"
#define CFG_AP_KEY_RELEASE_R98                      "R98"
#define CFG_AP_KEY_RELEASE_R99                      "R99"
#define CFG_AP_KEY_RELEASE_R4                       "R4"

#define CFG_SEC_CDRSTAT                             "CdrStat"
#define CFG_CDRSTAT_ROOT_DIR                        "CdrStatRootDir"

#ifdef _PLATFORM_WIN32
#define CFG_CDRSTAT_ROOT_DIR_DEFAULT                "d:\\other\\cdrstat"
#else
#define CFG_CDRSTAT_ROOT_DIR_DEFAULT                "/var/other/cdrstat"
#endif

#define CFG_AP_CDRSTAT_BILLTYPE                     "StatBillType"
#define CFG_AP_CDRSTAT_BILLTYPE_DEFAULT             0
#define CFG_CDRSTAT_BEGINTIME                       "StatBeginTime"
#define CFG_CDRSTAT_BEGINTIME_DEFAULT               "02:00:00"

#define CFG_SEC_LICENSE                             "License"
#define CFG_LICENSE_DIR                             "LicenseDir"
#define CFG_LICENSE_TIMER_INTERVAL                  "LicenseTimer"
#define DEFAULT_LICENSE_TIMER_INTERVAL              30
#define DEFAULT_LICENSE_DIR                         ""

//����̩���ƴ��ۼӱ���,MeterAccģ���õ��Ĳ���, 2004-05-08

#define CFG_SEC_METER_ACC                           "MeterAcc"
#define CFG_METER_ACC_SRC_DIR                       "MeterAccSrcDir"
#define CFG_METER_ACC_SRC_DIR_DEFAULT               "e:\\backsave\\Second\\X3KF\\meter"

#define CFG_METER_ACC_LOCAL_DIR                     "MeterAccLocalDir"
#define CFG_METER_ACC_LOCAL_DIR_DEFAULT             "e:\\meteracc\\local"

#define CFG_METER_ACC_BAK_DIR                       "MeterAccBakDir"
#define CFG_METER_ACC_BAK_DIR_DEFAULT               "e:\\meteracc\\backup"

#define CFG_METER_ACC_BS_DIR                        "MeterAccBSDir"
#define CFG_METER_ACC_BS_DIR_DEFAULT                "e:\\meteracc\\billingsystem"

#define CFG_METER_ACC_INDEX_DIR                     "MeterAccIndexDir"
#define CFG_METER_ACC_INDEX_DIR_DEFAULT             "e:\\meteracc\\index"

#define CFG_METER_ACC_TABLES                        "MeterTables"
#define CFG_METER_ACC_TABLES_DEFAULT                2

#define CFG_METER_ACC_TIME_CATEGORY                 "SubmitTimeInd"
#define CFG_METER_ACC_TIME_CATEGORY_DEFAULT         6

#define CFG_METER_ACC_PREFIX                        "Prefix"
#define CFG_METER_ACC_PREFIX_DEFAULT                "meteracc"

#define CFG_METER_ACC_SUFFIX                        "Suffix"
#define CFG_METER_ACC_SUFFIX_DEFAULT                "dat"

#define METER_ACC_DEST_FORMAT_NAME                  "X3KF_Ma_BP_D"
#define METER_ACC_SRC_FORMAT_NAME                   "X3KF_Mt_BP_D"

#define METER_ACC_SUBMIT_TIME                       "SubmitTime%d"
#define METER_ACC_SUBMIT_TIME_DEFAULT               "15:12"

#define METER_ACC_FOR_TTAT                          "ForTTAT"       //�Ƿ�Ϊ̩��TT&T�ֵ�(��2��Ҫ�ۼӵ���1��)
#define METER_ACC_FOR_TTAT_DEFAULT                  1

#define METER_ACC_DEBUG                             "IsDebug"
#define METER_ACC_DEBUG_DEFAULT                     0

#define METER_ACC_DAYS_FOR_BACKUP                   "BackupDays"
#define METER_ACC_DAYS_FOR_BACKUP_DEFAULT           90

#define METER_ACC_FFLUSH_TIME                       1      //������������ �ļ�ˢ�¼�� (��λ��һ�δ���ʱ��Ķ��ٱ�)


///////////////////////////////////////////////////////////////////////////////
//GTP'�Ʒѽӿ����� Z30971 2004/06/10
#define CFG_SEC_BS                                  "BS"

#define CFG_BS_KEY_GTP_VERSION                      "GtpVersion"
#define CFG_BS_KEY_GTP_VERSION_DEFAULT              2
#define CFG_BS_KEY_GTP_HEADLEN                      "GtpHeadLength"
#define CFG_BS_KEY_GTP_HEADLEN_DEFAULT              6

#define CFG_BS_KEY_GTP_SVR_PORT                     "GtpSvrPort"
#define CFG_BS_KEY_GTP_SVR_PORT_DEFAULT             3386
#define CFG_BS_KEY_GTP_SVR_IP                       "GtpSvrIP"
#define CFG_BS_KEY_GTP_SVR_IP_DEFAULT               "" 

#define CFG_BS_KEY_GTP_CLI_PORT                     "LocalPort"
#define CFG_BS_KEY_GTP_CLI_PORT_DEFAULT             3386
#define CFG_BS_KEY_GTP_CLI_IP                       "LocalIP"
#define CFG_BS_KEY_GTP_CLI_IP_DEFAULT               "0.0.0.0"

#define CFG_BS_KEY_GTP_MODE                         "GtpMode"
#define CFG_BS_KEY_GTP_MODE_DEFAULT                 1        //UDP 

#define CFG_BS_KEY_TASK_COUNT                       "GtpTaskCount"
#define CFG_BS_KEY_GTP_TASK                         "SrcFileDir" 

#define CFG_BS_KEY_STATUS_FILE_DIR                  "SenderStatusPath"
#define CFG_BS_KEY_GTP_STATUS_DIR                   "GtpStatusPath"

#define CFG_CBB_OI                                  "CBBOI"
#define CFG_CBB_OI_DEFAULT                          "Default"

#endif    //__CONFIG_DEFINE_H__


