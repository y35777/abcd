#include "save_chanl.h"
#include "../include/toolbox.h"
#include <math.h>
#include <time.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include "../include/msg_def.h"
/*
#include "../wlan_cmcc_s/wlan_cmcc_s.h"
#include "../3g_cs_cmcc/CMCC_CS_BS_Patch.h"
#include "../ps_patch/ps_patch.h"
*/
#include "resource.h"
#include "../include/toolbox.h"
#include "../frame/msg_proc.h"

//颜文远2002.04.04添加，问题单号D12661
#ifndef _PLATFORM_WIN32
    #include <sys/types.h>
    #include <sys/stat.h>
#endif

CSaveChannel::CSaveChannel(CPatchEncap* pPatchEncap) : CSaveTransaction()
{
    m_pPatchEncap = pPatchEncap;
    m_bSecondCopy = FALSE;
    m_nBillLen = 0;
}

CSaveChannel::~CSaveChannel()
{

}

//获取通道名
const char* CSaveChannel::GetChlName()
{
    return m_szChannelName.c_str();
}

/**********************************************************************
功能描述    初始化函数
输入参数    uAPID----接入点号
            uChannelID----通道号
            szChannelName----通道名
输出参数    无
返回值      初始化是否成功
                TRUE----初始化成功
                FALSE----初始化失败
抛出异常    无
备注        为了能够简化实际开局中的配置工作同时达到灵活性的要求，后存盘
            中采用多级配置的方式。配置参数的优先级从低到高总共分为
            总体/接入点/通道3个级别，如果高级别的参数没有配置，
            则取低级别的配置参数
**********************************************************************/
BOOL CSaveChannel::Init(UINT4 uAPID, UINT4 uChannelID, STRING& szChannelName, int nBillLen)
{
    strcpy(m_szModuNameEx, MTS_BACKSAVE);
    sprintf(m_szModuName, "%s_%s", MTS_BACKSAVE, szChannelName.c_str());

    m_uAPID = uAPID;
    m_uChannelID = uChannelID;
    m_szChannelName = szChannelName;
    m_nBillLen = nBillLen;
    m_uFileExistTime = 0;
    m_uHeadLen = sizeof(BS_STATUS_FILE_HEAD);
    m_uUnitLen = sizeof(BS_STATUS_FILE_ELEMENT);
    m_pHead = new BYTE[m_uHeadLen];
    memset(m_pHead, 0, m_uHeadLen);
    m_pLastInfoUnit = new BYTE[m_uUnitLen];
    memset(m_pLastInfoUnit, 0, m_uUnitLen);

    char szAPSection[25], szChlSection[40];
    sprintf(szAPSection, "%s%d", CFG_SEC_AP, m_uAPID);
    sprintf(szChlSection, CFG_SEC_CHANNEL, m_uAPID, m_uChannelID);
    CINIFile cfgIniFile(GetCfgFilePath());
    BOOL bSucc = cfgIniFile.Open();
    if (!bSucc)
    {
        TRACE(m_szModuName, S_BS_TRC_FAIL_OPEN_INI);
        return FALSE;
    }

    //从配置文件中读取最终话单文件长度，配置参数的读取原则见函数头中的备注
    m_uFileLen = cfgIniFile.GetInt(CFG_SEC_DISK_FILE, CFG_FINAL_FILE_MAX_SIZE,
                                   CFG_FINAL_FILE_MAX_SIZE_DEFAULT);
    m_uFileLen = cfgIniFile.GetInt(szAPSection, CFG_FINAL_FILE_MAX_SIZE,
                                   m_uFileLen);
    m_uFileLen = cfgIniFile.GetInt(szChlSection, CFG_FINAL_FILE_MAX_SIZE,
                                   m_uFileLen);
    //added by maimaoshi,2002-10-29
    /*
    //m_uFileLen的值必须大于64K
    if ((m_uFileLen < 64) || (m_uFileLen > 20 * 1024))
    {
        m_uFileLen = CFG_FINAL_FILE_MAX_SIZE_DEFAULT;
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_SIZE, m_uFileLen);
    }
    */

    if(m_uFileLen == 0)
    {
        m_bAlwaysGenFile = TRUE;
    }
    if (m_uFileLen < 64)
    {
        m_uFileLen = 64;
        //当配置值小于最小值64K时,设置为最小指64K
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_MINSIZE, m_uFileLen * 1024);
    }else if (m_uFileLen > 20 * 1024)
    {
        m_uFileLen = 20 * 1024;
        //当配置值大于最大值20M时,设置为最大值20M
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_MAXSIZE, m_uFileLen * 1024);

    }
    //end
    m_uFileLen *= 1024;     //进行从K到字节的转换

    //从配置文件中读取文件生成时间，配置参数的读取原则见函数头中的备注
    m_uFileTime = cfgIniFile.GetInt(CFG_SEC_DISK_FILE, CFG_FINAL_FILE_MAX_TIME,
                                    CFG_FINAL_FILE_MAX_TIME_DEFAULT);
    m_uFileTime = cfgIniFile.GetInt(szAPSection, CFG_FINAL_FILE_MAX_TIME,
                                    m_uFileTime);
    m_uFileTime = cfgIniFile.GetInt(szChlSection, CFG_FINAL_FILE_MAX_TIME,
                                    m_uFileTime);
    //added by maimaoshi,2002-10-29
    if (m_uFileTime < 5)
    {
        m_uFileTime = 5;
        //当配置值小于最小值5时，设置为最小值5
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_MINTIME, m_uFileTime);
    }else if (m_uFileTime > 3600)
    {
        m_uFileTime = 3600;
        //当配置值大于最大值3600时，设置为最大值3600
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_MAXTIME, m_uFileTime);
    }
    //end

    sprintf(m_szPrefix, "%02d_", m_uChannelID);
    strcpy(m_szPostfix, "tmp");

    //从配置文件中读取文件序列号长度，配置参数的读取原则见函数头中的备注
    m_nCsnLen = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                  CFG_CSN_LEN, CFG_CSN_LEN_DEFAULT);
    m_nCsnLen = cfgIniFile.GetInt(szAPSection, CFG_CSN_LEN, m_nCsnLen);
    m_nCsnLen = cfgIniFile.GetInt(szChlSection, CFG_CSN_LEN, m_nCsnLen);

    //从配置文件中读取起始序列号，配置参数的读取原则见函数头中的备注
    m_uFileCsnStart = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                     CFG_START_CSN, CFG_START_CSN_DEFAULT);
    m_uFileCsnStart = cfgIniFile.GetInt(szAPSection,
                                     CFG_START_CSN, m_uFileCsnStart);
    m_uFileCsnStart = cfgIniFile.GetInt(szChlSection,
                                     CFG_START_CSN, m_uFileCsnStart);

    //从配置文件中读取结束序列号，配置参数的读取原则见函数头中的备注
    UINT4 uDefaultEndCsn;

	//by	杨善	2004-5-29	对应问题单SWPD05385	
    if (m_nCsnLen >= 10 || m_nCsnLen < 0)
	//the end  2004-5-29	对应问题单SWPD05385
	
    //by    杨善    2004-6-1    对应问题单SWPD05593
    //由于UINT4的大小限制，所以最大的序列号只支持到9个9
    {
        m_nCsnLen = 9;
        uDefaultEndCsn = 999999999;
    }
    else if (m_nCsnLen == 0)
    {
        uDefaultEndCsn = 999999999;
    }
    else
    {
        uDefaultEndCsn = pow((double)10, m_nCsnLen) - 1;
    }
    //the end   对应问题单SWPD05593
   
    m_uFileCsnEnd = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                      CFG_END_CSN, uDefaultEndCsn);
    m_uFileCsnEnd = cfgIniFile.GetInt(szAPSection, CFG_END_CSN, m_uFileCsnEnd);
    m_uFileCsnEnd = cfgIniFile.GetInt(szChlSection, CFG_END_CSN, m_uFileCsnEnd);

    //从配置文件中读取序列号步长，配置参数的读取原则见函数头中的备注
    m_uFileCsnStep = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                       CFG_CSN_STEP, CFG_CSN_STEP_DEFAULT);
    m_uFileCsnStep = cfgIniFile.GetInt(szAPSection,
                                       CFG_CSN_STEP, m_uFileCsnStep);
    m_uFileCsnStep = cfgIniFile.GetInt(szChlSection,
                                       CFG_CSN_STEP, m_uFileCsnStep);

    //从配置文件中读取正式文件的前缀，配置参数的读取原则见函数头中的备注
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_PREFIX, CFG_PREFIX_DEFAULT,
                         m_szFormalPrefix, 512);
    cfgIniFile.GetString(szAPSection, CFG_PREFIX, m_szFormalPrefix,
                         m_szFormalPrefix, 512);
    cfgIniFile.GetString(szChlSection, CFG_PREFIX, m_szFormalPrefix,
                         m_szFormalPrefix, 512);

    //从配置文件中读取正式文件的中缀，配置参数的读取原则见函数头中的备注
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_MIDFIX, CFG_MIDFIX_DEFAULT,
                         m_szFormalMidfix, 512);
    cfgIniFile.GetString(szAPSection, CFG_MIDFIX, m_szFormalMidfix,
                         m_szFormalMidfix, 512);
    cfgIniFile.GetString(szChlSection, CFG_MIDFIX, m_szFormalMidfix,
                         m_szFormalMidfix, 512);

    //从配置文件中读取正式文件的后缀，配置参数的读取原则见函数头中的备注
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_POSTFIX, CFG_POSTFIX_DEFAULT,
                         m_szFormalPostfix, 512);
    cfgIniFile.GetString(szAPSection, CFG_POSTFIX, m_szFormalPostfix,
                         m_szFormalPostfix, 512);
    cfgIniFile.GetString(szChlSection, CFG_POSTFIX, m_szFormalPostfix,
                         m_szFormalPostfix, 512);

    //得到并创建最终话单存放的根目录
    char szFinalRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR,
                         CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                         szFinalRootDir, MAX_PATH);
    CreateDeepDir(szFinalRootDir);

    //得到并创建主状态文件的根目录
    char szMainStatusRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_MAIN_STATUS_ROOT_DIR,
                         CFG_MAIN_STATUS_ROOT_DIR_DEFAULT,
                         szMainStatusRootDir, MAX_PATH);
    CreateDeepDir(szMainStatusRootDir);

    //得到并创建备状态文件的根目录
    char szBakStatusRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_BAK_STATUS_ROOT_DIR,
                         CFG_BAK_STATUS_ROOT_DIR_DEFAULT,
                         szBakStatusRootDir, MAX_PATH);
    CreateDeepDir(szBakStatusRootDir);

    //得到接入点名并创建相应的接入点目录
    char szAPName[255];
    cfgIniFile.GetString(szAPSection, CFG_AP_NAME,
                         szAPSection, szAPName, 255);
    char szChannelPath[MAX_PATH], szMainStatusPath[MAX_PATH],
              szBakStatusPath[MAX_PATH];
    sprintf(szChannelPath, "%s/%s", szFinalRootDir, szAPName);

    //Added bu maimaoshi at 2002-09-23
    SNPRINTF(m_szSrcBasePath,MAX_PATH,"%s",
        MakePath(STRING(szFinalRootDir), szAPName).c_str());
    m_szSrcBasePath[MAX_PATH - 1] = 0 ;

    //从配置文件中的DiskFile中，读取话单文件的保留天数，初始化成员变量
    //m_uBillSavedDays;
    m_uBillSavedDays = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,             //DiskFile
                                      CFG_SAVE_DEADLINE_DEL,         //DeadLineOfAutoDel
                                      CFG_SAVE_DEADLINE_DEL_DEFAULT);
    m_uBillSavedDays = cfgIniFile.GetInt(szAPSection,             //AccessPoint%d
                                      CFG_SAVE_DEADLINE_DEL,         //DeadLineOfAutoDel
                                      m_uBillSavedDays);
    m_uBillSavedDays = cfgIniFile.GetInt(szChlSection,             //Channel%d-%d
                                      CFG_SAVE_DEADLINE_DEL,         //DeadLineOfAutoDel
                                      m_uBillSavedDays);

    //话单文件的保留天数至少需要7天,最大是45天
    if(m_uBillSavedDays < 7)
    {
        m_uBillSavedDays = 7;
        //提示配置的值小于最小值7，设置为最小值7
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_AUTODEL_MINDAY, m_uBillSavedDays);
    }else if (m_uBillSavedDays > 180) //Modified by ZhengYuqun SWPD04451 2004-03-27
    {
        m_uBillSavedDays = 180;
        //提示配置的值大于最大值180，设置为最大值180
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_AUTODEL_MAXDAY, m_uBillSavedDays);
    }
    //end

    ACE_OS::mkdir(szChannelPath);
    sprintf(szMainStatusPath, "%s/%s", szMainStatusRootDir, szAPName);
    ACE_OS::mkdir(szMainStatusPath);
    sprintf(szBakStatusPath, "%s/%s", szBakStatusRootDir, szAPName);
    ACE_OS::mkdir(szBakStatusPath);

    //创建主备状态文件目录
    sprintf(szMainStatusPath, "%s/%s/%s",
            szMainStatusRootDir, szAPName, "save");
    CreateDeepDir(szMainStatusPath);
    sprintf(szBakStatusPath, "%s/%s/%s",
            szBakStatusRootDir, szAPName, "save");
    CreateDeepDir(szBakStatusPath);

    //创建通道子目录和临时文件子目录
    m_szDatePath = STRING(szChannelPath) + "/" + m_szChannelName;
    ACE_OS::mkdir(m_szDatePath.c_str());
    m_szFilePath = STRING(szChannelPath) + "/" + "temp";
    ACE_OS::mkdir(m_szFilePath.c_str());

    //设置主备状态文件的相关参数
    char szMainStatusName[255], szBakStatusName[255];
    sprintf(szMainStatusName, "%s_BS_%02d_MAIN.stf", szAPName, m_uChannelID);
    sprintf(szBakStatusName, "%s_BS_%02d_BAK.stf", szAPName, m_uChannelID);
    char szBackupPath[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACKUP_STATUS_SRC_DIR,
                         CFG_BACKUP_STATUS_SRC_DIR_DEFAULT,
                         szBackupPath, MAX_PATH);
    CreateDeepDir(szBackupPath);
    m_InfoMain.SetInfoFilePath(szMainStatusName, szMainStatusPath);
    m_InfoMain.SetBackupFilePath(szMainStatusName, szBackupPath);
    m_InfoMain.SetFileStruct(sizeof(BS_STATUS_FILE_HEAD),
                             sizeof(BS_STATUS_FILE_ELEMENT),
                             MAX_UNIT_NUM);
    m_InfoBak.SetInfoFilePath(szBakStatusName, szBakStatusPath);
    m_InfoBak.SetBackupFilePath(szBakStatusName, szBackupPath);
    m_InfoBak.SetFileStruct(sizeof(BS_STATUS_FILE_HEAD),
                             sizeof(BS_STATUS_FILE_ELEMENT),
                             MAX_UNIT_NUM);

    //在状态头中填入各变量的缺省值，如果状态文件不存在，则将缺省值写入状态文件
    BS_STATUS_FILE_HEAD* pHead = (BS_STATUS_FILE_HEAD* )m_pHead;
    pHead->uBillCsn = 1;

    //颜文远2002.05.18修改，问题单号D13767
    pHead->uFileCsn = m_uFileCsnStart - m_uFileCsnStep;
    //颜文远2002.05.18修改结束

    pHead->uLastPID = 0;
    pHead->uStatusFileWritePos = m_uHeadLen;
    m_InfoMain.CalcCheckSum(m_pHead);

    //从配置文件中读取Flush的包间隔，配置参数的读取原则见函数头中的备注
    m_uMaxFlushSpan = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                CFG_MAX_FLUSH_SPAN, CFG_MAX_FLUSH_SPAN_DEFAULT);
    m_uMaxFlushSpan = cfgIniFile.GetInt(szAPSection,
                                CFG_MAX_FLUSH_SPAN, m_uMaxFlushSpan);
    m_uMaxFlushSpan = cfgIniFile.GetInt(szChlSection,
                                CFG_MAX_FLUSH_SPAN, m_uMaxFlushSpan);

    //设置和第二份最终话单相关的参数
    //从配置文件中读取正式文件的后缀，配置参数的读取原则见函数头中的备注
    //设置和第二份最终话单相关的参数
    //从配置文件中读取正式文件的后缀，配置参数的读取原则见函数头中的备注
    if (!m_bSecondCopy)
    {
        m_bSecondCopy = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                          CFG_SAVE_SECOND, CFG_SAVE_SECOND_DEFAULT);
        m_bSecondCopy = cfgIniFile.GetInt(szAPSection,
                                          CFG_SAVE_SECOND, m_bSecondCopy);
        m_bSecondCopy = cfgIniFile.GetInt(szChlSection,
                                          CFG_SAVE_SECOND, m_bSecondCopy);
    }

    if (m_bSecondCopy)
    {
        char szSecondDir[MAX_PATH], szSecondChannelPath[MAX_PATH];
        sprintf(szSecondDir, "%s/%s", szFinalRootDir, "Second");
        ACE_OS::mkdir(szSecondDir);
        sprintf(szSecondChannelPath, "%s/%s", szSecondDir, szAPName);
        m_szSecondChannelPath = STRING(szSecondChannelPath);
        ACE_OS::mkdir(szSecondChannelPath);
        m_szSecondPath = STRING(szSecondChannelPath) + "/" + m_szChannelName;
        ACE_OS::mkdir(m_szSecondPath.c_str());
        //颜文远2002.04.28添加，问题单号D12661
        //修改第二份话单文件的权限为可读写，以便计费中心能够删除
#ifndef _PLATFORM_WIN32
        if (0 != chmod(m_szSecondPath.c_str(),
                       S_IRUSR | S_IWUSR | S_IXUSR
                       | S_IRGRP | S_IWGRP | S_IXGRP
                       | S_IROTH | S_IWOTH | S_IXOTH))
        {
            return FALSE;
        }
#endif
        //颜文远2002.04.28添加结束
    }
    //从配置文件中读取计费中心取话单的超时时间，单位为分钟，
    //配置参数的读取原则见函数头中的备注
    m_nFetchFileTimeOut = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                            CFG_FETCH_FILE_TIMEOUT,
                                            CFG_FETCH_FILE_TIMEOUT_DEFAULT);
    m_nFetchFileTimeOut = cfgIniFile.GetInt(szAPSection,
                                            CFG_FETCH_FILE_TIMEOUT,
                                            m_nFetchFileTimeOut);
    m_nFetchFileTimeOut = cfgIniFile.GetInt(szChlSection,
                                            CFG_FETCH_FILE_TIMEOUT,
                                            m_nFetchFileTimeOut);
    m_nFetchFileTimeOut *= 60;  //进行从分到秒的转换
    m_bFetchFileAlarmSend = FALSE;
    m_szCheckedFilePath = FindFirstFileInSecondPath(); //Modified by ZhengYuqun SWPD03912 2004-03-03
    m_nCheckTimeCounter = 0;
/* Begin:   Adder: Gan Zhiliang   Date: 2003-4-23*/
    m_uResFileProductTime = cfgIniFile.GetInt(szAPSection,
                                            "ResFileProductTime",
                                            0);
/* End  : Adder: Gan Zhiliang      Date: 2003-4-23*/

    //是否启动日志特性
    m_bCheckLog = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                    CFG_CHECK_LOG_ENABLE,
                                    CFG_CHECK_LOG_ENABLE_DEFAULT);
    m_bCheckLog = cfgIniFile.GetInt(szAPSection,
                                    CFG_CHECK_LOG_ENABLE,
                                    m_bCheckLog);
    m_bCheckLog = cfgIniFile.GetInt(szChlSection,
                                    CFG_CHECK_LOG_ENABLE,
                                    m_bCheckLog);

    if (m_bCheckLog)
    {
        if (m_nBillLen == 0)
        {
            TRACE(m_szModuName, S_BS_CHECK_ERR_BILL_LEN, m_uChannelID);
            return FALSE;
        }

        //稽核日志文件的前缀
        cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_CHECK_LOG_PREFIX, 
                             CFG_CHECK_LOG_PREFIX_DEFAULT,
                             m_szCheckLogPrefix, sizeof(m_szCheckLogPrefix));
        cfgIniFile.GetString(szAPSection, CFG_CHECK_LOG_PREFIX, 
                             m_szCheckLogPrefix,
                             m_szCheckLogPrefix, sizeof(m_szCheckLogPrefix));
        cfgIniFile.GetString(szChlSection, CFG_CHECK_LOG_PREFIX, 
                             m_szCheckLogPrefix,
                             m_szCheckLogPrefix, sizeof(m_szCheckLogPrefix));

        //稽核日志文件的后缀
        cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_CHECK_LOG_POSTFIX, 
                             CFG_CHECK_LOG_POSTFIX_DEFAULT,
                             m_szCheckLogPostfix, sizeof(m_szCheckLogPostfix));
        cfgIniFile.GetString(szAPSection, CFG_CHECK_LOG_POSTFIX, 
                             m_szCheckLogPostfix,
                             m_szCheckLogPostfix, sizeof(m_szCheckLogPostfix));
        cfgIniFile.GetString(szChlSection, CFG_CHECK_LOG_POSTFIX, 
                             m_szCheckLogPostfix,
                             m_szCheckLogPostfix, sizeof(m_szCheckLogPostfix));
        
        
        //计算日志的通道目录
        char szCheckLogChlPath[MAX_PATH];
        SNPRINTF(szCheckLogChlPath, 
                 sizeof(szCheckLogChlPath),
                 "%s/%s/%s/%s", 
                 szFinalRootDir, "CheckLog", 
                 szAPName, m_szChannelName.c_str());
        szCheckLogChlPath[sizeof(szCheckLogChlPath) - 1] = '\0';
        CreateDeepDir(szCheckLogChlPath);

        m_szCheckedLogPath = szCheckLogChlPath;

        //开放可读写权限
#ifndef _PLATFORM_WIN32
        if (0 != chmod(szCheckLogChlPath,
                       S_IRUSR | S_IWUSR | S_IXUSR
                       | S_IRGRP | S_IWGRP | S_IXGRP
                       | S_IROTH | S_IWOTH | S_IXOTH))
        {
            return FALSE;
        }
#endif
    }

    m_nDelTimeCounter = 0;
    bSucc = CSaveTransaction::Init();
    if(bSucc && m_bAlwaysGenFile)    //如果要连续不间断地生成文件，那么一启动就创建临时文件
    {
        bSucc = CreateNewFile();
    }

    return bSucc;
}

//秒事件处理函数
BOOL CSaveChannel::Timer1Sec()
{
    BOOL bSucc = CSaveTransaction::Timer1Sec();
    if (!bSucc)
    {
        return FALSE;
    }
    if(m_nDelTimeCounter == 0)
    {

        DelMoreFiles();
        m_nDelTimeCounter = 60 * 60;
    }
    else
    {
        m_nDelTimeCounter--;
    }

    /* Begin: Adder: Gan Zhiliang   Date: 2003-4-23*/
    //对于WLAN话单要作特殊处理，不使用原来的代码
    if((WLAN_BACKSAVE_DLL_NAME == m_pPatchEncap->GetDllName()) && (m_uChannelID > 0))
    {
        if(m_pPatchEncap->IsFileOutPut_WLAN(m_uChannelID))
        {
            if(NULL == m_pBillFile)
            {
                char szFileName[1024], szFileSuffix[512];
                m_pPatchEncap->GetAEmptyFilePara(szFileName, szFileSuffix, m_uChannelID);
                m_pPatchEncap->CreateAEmptyFile(szFileName, szFileSuffix);
            }
            else
            {
                bSucc = CloseCurFile();
                if (!bSucc)
                {
                    //STRING szFullPath = m_szFilePath + "/" + m_szFileName;
                    //memset(szFileName, 0, MAX_PATH);
                    //sprintf(szFileName, "%s", szFullPath.c_str());
                    return FALSE;
                }
            }
            m_pPatchEncap->ResetRemainTicket_WLAN();
            return TRUE;
        }
        return TRUE;
    }
    /* End  : Adder: Gan Zhiliang     Date: 2003-4-23*/


    //如果当前话单文件已经超时，则关闭当前话单文件
    m_uFileExistTime++;
    if (m_uFileExistTime >= m_uFileTime)
    {
        if (NULL != m_pBillFile)
        {
            bSucc = CloseCurFile();
            if (!bSucc)
            {
                return FALSE;
            }
            //如果要连续不间断地生成文件，那么每生成完一个正式文件就马上生成一个新的临时文件
            else if(m_bAlwaysGenFile)
            {
                if(!CreateNewFile())
                {
                    return FALSE;
                }
            }
        }
    }

    //检查计费中心是否及时的取走了话单文件
    CheckFetchingFile();

    return TRUE;
}

/**********************************************************************
功能描述    获得启动恢复时需要的状态参数
输入参数    无
输出参数    uLastPID----最后保存的话单包
            uLastBillCsn----所保存话单中最后一张话单的流水号
返回值      无
抛出异常    无
**********************************************************************/
void CSaveChannel::GetResumeParam(UINT4& uLastPID, UINT4& uLastBillCsn)
{
    uLastPID = ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
    uLastBillCsn = ((BS_STATUS_FILE_HEAD* )m_pHead)->uBillCsn;
}

//根据状态文件得到最后一个话单包的包号
UINT4 CSaveChannel::GetLastPID()
{
    return ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
}

//根据状态文件得到话单序列号
UINT4 CSaveChannel::GetBillCsn()
{
    return ((BS_STATUS_FILE_HEAD* )m_pHead)->uBillCsn;
}

//检查计费中心是否已经及时取走话单文件，每秒钟被调用一次
void CSaveChannel::CheckFetchingFile()
{
    m_nCheckTimeCounter++;

    //如果只保存一份，则不用进行检查
    if (!m_bSecondCopy)
    {
        return;
    }

    if (m_nCheckTimeCounter >= m_nFetchFileTimeOut)
    {
        //检查被监视的文件是否已经被取走
        BOOL bFileFetched
             = (0 != ACE_OS::access(m_szCheckedFilePath.c_str(), F_OK));

        if (bFileFetched)
        {
            //Modified by ZhengYuqun SWPD03912 2004-03-03
            if(!m_szFormalFileName.empty())
            {
                m_szCheckedFilePath
                    = m_szSecondPath + "/" + m_szFormalFileName;                
            }
            else
            {
                m_szCheckedFilePath = FindFirstFileInSecondPath();
            }
            //End SWPD03912

            //为了防止发往告警模块的告警消息丢失，
            //只要文件已经被取走，则发送恢复告警
            SInnerAlarm Alarm;
            Alarm.yAlarmType = AT_RESTORE;
            Alarm.uAlarmID = ALARM_ID_NO_FETCH_FILE_FAULT;
            Alarm.yAlarmPara[0] = m_uChannelID;
            CMsgProcessor::SendAlarm(&Alarm);
            m_bFetchFileAlarmSend = FALSE;
        }
        else
        {
            //为了防止发往告警模块的告警消息丢失，
            //只要文件未被取走，则发送故障告警
            SInnerAlarm Alarm;
            Alarm.yAlarmType = AT_FAULT;
            Alarm.uAlarmID = ALARM_ID_NO_FETCH_FILE_FAULT;
            Alarm.yAlarmPara[0] = m_uChannelID;
            CMsgProcessor::SendAlarm(&Alarm);
            m_bFetchFileAlarmSend = TRUE;
        }

        //重新设置检查被监视文件的时间计数器，
        //在发送了故障告警的情况下，将检查的间隔减少为半分钟
        if (m_bFetchFileAlarmSend)
        {
            m_nCheckTimeCounter = m_nFetchFileTimeOut - 30;
        }
        else
        {
            m_nCheckTimeCounter = 0;
        }
    }
}

//得到下一个话单文件存放目录，如果目录不存在，则创建之。
//在后存盘中，临时话单文件的目录永远不会改变
void CSaveChannel::GetNextFilePath()
{
}

/**********************************************************************
功能描述    删除最后一个临时文件，在进行状态文件校验时调用
输入参数    无
输出参数    无
返回值      删除文件是否成功
                TRUE----删除成功
                FALSE----删除失败
抛出异常    无
**********************************************************************/
BOOL CSaveChannel::DeleteTempFile()
{
    BS_STATUS_FILE_ELEMENT LastUnitInfo;

    //读取主状态文件的最后一个状态记录
    int nResult = m_InfoMain.ReadLastUnit((BYTE* )&LastUnitInfo);
    if ((ERR_INDEX_INVALID == nResult) || (ERR_FAIL_READ == nResult))
    {
        return FALSE;
    }
    else if (ERR_NO_UNIT_EXIST == nResult)
    {
        return TRUE;
    }

    //根据最后一个状态记录获得最后一个文件的路径并将其删除
    STRING szFileName, szTempFileFullPath;
    szFileName = m_szPrefix + GetFileCsnStr(LastUnitInfo.uFileCsn)
                 + "." + m_szPostfix;
    szTempFileFullPath = m_szFilePath + "/" + szFileName;
    BOOL bSucc = FileDel(szTempFileFullPath.c_str());
    if (bSucc)
    {
        TRACE(m_szModuName, S_BS_TRC_SUCC_DEL_FILE, LastUnitInfo.uFileCsn);
        return TRUE;
    }
    else
    {
        TRACE(m_szModuName, S_BS_TRC_FAIL_DEL_FILE, LastUnitInfo.uFileCsn);
        return FALSE;
    }
}

/**********************************************************************
功能描述    根据收到的话单包，更新状态文件的内容及各成员变量
输入参数    pPacketInfo----话单包的相关信息，具体内容在子类里各自解释，
                在函数外负责申请和释放
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSaveChannel::RefreshStatus(BYTE* pPacketInfo)
{
    //对传入的信息进行类型转换
    SBackPacketInfo* pBackPacketInfo = (SBackPacketInfo* )pPacketInfo;
    m_uPID = pBackPacketInfo->uPID;

    //根据传入的信息更新文件头，无需更新当前状态记录
    BS_STATUS_FILE_HEAD* pHead = (BS_STATUS_FILE_HEAD* )m_pHead;
    pHead->uLastPID = pBackPacketInfo->uPID;
    pHead->uFileCsn = m_uCurCsn;
    pHead->uBillCsn = pBackPacketInfo->uBillCsn;
    pHead->uFileWritePos += pBackPacketInfo->uLen;
    m_InfoMain.CalcCheckSum(m_pHead);
}

/**********************************************************************
功能描述    关闭话单文件的后续处理
输入参数    无
输出参数    无
返回值      处理是否成功
                TRUE----处理成功
                FALSE----处理失败
抛出异常    无
**********************************************************************/
BOOL CSaveChannel::DoWhenFileClose()
{
    ASSERT(NULL == m_pBillFile);

        
    BOOL bSucc;

    //截断当前的话单文件
    UINT4 uWriteFilePos = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos;
    STRING szFileFullPath = m_szFilePath + "/" + m_szFileName;
    int nResult = ACE_OS::truncate(szFileFullPath.c_str(), uWriteFilePos);
    if (0 != nResult)
    {
        return FALSE;
    }

    //求文件中所含话单的记录数    
    int nRecordCnt = 0;
    if(m_nBillLen != 0)
    {
        nRecordCnt = uWriteFilePos / m_nBillLen;
    }
    
    //计算正式的话单文件名
    STRING szPrefix = FormatCfgStr(m_szFormalPrefix, 
                                   m_tmCreateTime, 
                                   m_uChannelID,
                                   nRecordCnt);


    m_uCurCsn += m_uFileCsnStep;
    if ((m_uCurCsn > m_uFileCsnEnd) || ((int)m_uCurCsn < (int)(m_uFileCsnStart - m_uFileCsnStep)))
    {
        m_uCurCsn = m_uFileCsnStart - m_uFileCsnStep;
    }
    STRING szFileCsn = GetFileCsnStr(m_uCurCsn);

    STRING szMidfix = FormatCfgStr(m_szFormalMidfix, 
                                   m_tmCreateTime, 
                                   m_uChannelID,
                                   nRecordCnt);

    if (0 == strlen(m_szFormalPostfix))
    {
        m_szFormalFileName = szPrefix + szFileCsn + szMidfix;
    }
    else
    {
        m_szFormalFileName
             = szPrefix + szFileCsn + szMidfix + "." + m_szFormalPostfix;
    }
	//by	杨善	2004-5-29	对应问题单SWPD05385
    if (0 == strlen(m_szFormalPostfix))
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        m_szFormalFileName = szPrefix + szFileCsn + szMidfix;
    }
    else
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}

        m_szFormalFileName
			= STRING(szPrefix) + szFileCsn + szMidfix + "." + m_szFormalPostfix;
    }
	//the end 	2004-5-29	对应问题单SWPD05385
	
    if(m_bCheckLog)
    {
        STRING szSrcFileFullPath = m_szFilePath + "/" + m_szFileName;
        STRING szFileInfo;
        szFileInfo = GetBillFileInfo(szSrcFileFullPath.c_str());
        SaveCheckLog(szFileInfo.c_str());
    }

    //如果保存两份，则从临时文件拷贝得到送计费中心的第二份文件
    if (m_bSecondCopy)
    {
        STRING szSrcFileFullPath = m_szFilePath + "/" + m_szFileName;
        STRING szDstFileFullPath = m_szSecondPath + "/" + m_szFormalFileName;

        //如果需要进行话单适配转换
        if ((m_pPatchEncap->GetbAviable()) && (m_uChannelID > 0) && (uWriteFilePos > 0))
        {
            char* szDest;
            szDest = (char*)szDstFileFullPath.c_str();
            BOOL bRet = m_pPatchEncap->Process(szSrcFileFullPath.c_str(), szDest, m_uChannelID);
            if (!bRet)
            {
                //发送话单异常告警  modify by lx SWPD03595
                SInnerAlarm sAlarm;
                sAlarm.yAlarmType = AT_EVENT;
                sAlarm.uAlarmID = ALARM_ID_ABNORMAL_BILL;
                sAlarm.yAlarmPara[0] = ALARM_PARA_ENCODING_ERR;
                CMsgProcessor::SendAlarm(&sAlarm);
                
                //将此文件copy一份到second目录下的default子目录中，并将后缀名改为.err
                int nLen = m_szFileName.length();
                STRING szFileName = m_szFileName.substr(0, nLen - 3) + "err";
                
                STRING szDefaultFilePath = m_szSecondChannelPath + "/" + CHANNEL_0_NAME;
                szDefaultFilePath = szDefaultFilePath + "/" + szFileName;
                
                bSucc = FileCopyEx(szSrcFileFullPath.c_str(),
                                 szDefaultFilePath.c_str());

                TRACE(MTS_BACKSAVE,s_PATCH_PROCESS_FAILED);
                //end by lx SWPD03595
            }
            szDstFileFullPath = szDest;
        }
        else
        {
            bSucc = FileCopyEx(szSrcFileFullPath.c_str(),
                             szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
        }

        //颜文远2002.04.04添加，问题单号D12661
        //修改第二份话单文件的权限为可读写，以便计费中心能够删除
        #ifndef _PLATFORM_WIN32
        if (0 != chmod(szDstFileFullPath.c_str(),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH))
        {
            return FALSE;
        }
        #endif
        //颜文远2002.04.04添加结束

    }

    //如果当前文件的创建日期与上个文件的创建日期不一样，则必须创建新的日期目录
    if ((m_tmCreateTime.tm_year != m_tmLastFileTime.tm_year)
            || (m_tmCreateTime.tm_mon != m_tmLastFileTime.tm_mon)
            || (m_tmCreateTime.tm_mday != m_tmLastFileTime.tm_mday))
    {
        char szDateName[20];
        strftime(szDateName, 20, "%Y%m%d", &m_tmCreateTime);
        m_szFirstPath = m_szDatePath + "/" + szDateName;

        //by ldf 2004-02-18 对应问题单 SWPD03439
        ACE_DIR *pDir = NULL;
        pDir = ACE_OS::opendir(m_szFirstPath.c_str());
        if(pDir == NULL)
        {
            ACE_OS::mkdir(m_szFirstPath.c_str());
            char szLogInfo[255];
            sprintf(szLogInfo, S_BS_LOG_CREATE_BS_DATE,
                m_szChannelName.c_str(), szDateName);
            CMsgProcessor::WriteLog(MTS_BACKSAVE, szLogInfo);
            MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_IMPORTANT, szLogInfo);
        }
        else
        {
            ACE_OS_Dirent::closedir(pDir);
        }
        //end

        //added by maimaoshi at 2002-09-23
        //删除多余的文件
        //DelMoreFiles();
        //end
    }

    //将临时文件移为正式文件
    STRING szSrcFileFullPath = m_szFilePath + "/" + m_szFileName;
    STRING szDstFileFullPath = m_szFirstPath + "/" + m_szFormalFileName;
    bSucc = FileCopyEx(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
    if (!bSucc)
    {
        return FALSE;
    }
    FileDel(szSrcFileFullPath.c_str());


    //刷新并写状态文件
    ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn = m_uCurCsn;
    m_InfoMain.CalcCheckSum(m_pHead);

    //颜文远2002.04.12添加，问题单号D12568
    bSucc = m_InfoMain.WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }
    //颜文远2002.04.12添加结束
    bSucc = m_InfoMain.Flush();
    if (!bSucc)
    {
        return FALSE;
    }
    //颜文远2002.04.12添加，问题单号D12568
    bSucc = m_InfoBak.WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }
    //颜文远2002.04.12添加结束
    bSucc = m_InfoBak.Flush();
    if (!bSucc)
    {
        return FALSE;
    }

    //Added by ZhengYuqun 生成空文件时同步状态 SWPD04389 2004-03-26
    //发送同步数据
    SSaveSyncData SyncData;
    SyncData.uPID     = GetLastPID();
    SyncData.uFileCsn = GetCurCsn();
    SyncData.uBillCsn = GetBillCsn();
    CMsgProcessor::WriteSyncData(MT_BACKSAVE,
                                 &SyncData, 
                                 sizeof(SSaveSyncData),
                                 m_uChannelID);
    //End SWPD04389

    MSGOUT(MTS_BACKSAVE, MSGOUT_LEVEL_IMPORTANT,
           S_BS_MSG_CREATE_FORMAL, m_szFormalFileName.c_str(),
           m_szChannelName.c_str());

    //颜文远2002.03.16修改，问题单号D11867
    char szLogInfo[255];
    SNPRINTF(szLogInfo, 255, S_BS_LOG_CREATE_NEW_FILE,
            m_szChannelName.c_str(), m_szFormalFileName.c_str());
    CMsgProcessor::WriteLog(MTS_BACKSAVE, szLogInfo);
    //颜文远2002.03.16修改结束

    return TRUE;
}

/**********************************************************************
功能描述    在保存话单包事务中的写状态文件操作
输入参数    pInfoFile----要写入的状态文件
            bFlush----是否进行Flush操作
输出参数    无
返回值      写状态文件是否成功
                TRUE----写成功
                FALSE----写失败
抛出异常    无
**********************************************************************/
BOOL CSaveChannel::WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush)
{
    //后存盘中，只用写状态头，且写状态文件和Flush操作必须同时进行，
    //所以，只有在要求进行Flush操作的情况下才进行写状态文件操作
    if (bFlush)
    {
        BOOL bSucc = pInfoFile->WriteHead(m_pHead, m_uHeadLen);
        if (!bSucc)
        {
            return FALSE;
        }
        bSucc = pInfoFile->Flush();
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************
功能描述    在保存话单包事务中的写话单文件操作
输入参数    pPacketInfo----话单包的相关信息，在函数外负责申请和释放
            bFlush----是否进行Flush操作
输出参数    无
返回值      返回值说明如下：
                0----读取成功
                1----读取失败
                <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值：
                    ERR_WRITE_BILLFILE_FAIL----写话单文件失败
抛出异常    无
**********************************************************************/
int CSaveChannel::WriteBillFile(BYTE* pPacketInfo, BOOL bFlush)
{
    BOOL bSucc;
    //对传入的信息进行类型转换
    SBackPacketInfo* pBackPacketInfo = (SBackPacketInfo* )pPacketInfo;

    //只有在话单包非空的情况下，才作实际的创建新文件、写盘等操作，
    //否则不作任何操作
    if (pBackPacketInfo->uLen >0)
    {
        bSucc = WritePacketData(pBackPacketInfo->pPacket,
                              pBackPacketInfo->uLen);
        if (!bSucc)
        {
            return ERR_WRITE_BILLFILE_FAIL;
        }

        if (bFlush)
        {
            //进行Flush操作
            int nResult = fflush(m_pBillFile);
            if (0 != nResult)
            {
                return ERR_WRITE_BILLFILE_FAIL;
            }
        }
    }

    return ERR_SUCCESS;
}

/**********************************************************************
功能描述    处理启动校验时主备状态文件不一致的情况
输入参数    pMainHead----主状态文件的文件头
            pBakHead----备状态文件的文件头
            pMainLastUnit----主状态文件的最后一个状态记录
            pBakLastUnit----备状态文件的最后一个状态记录
输出参数    无
返回值      操作是否成功
                TRUE----处理成功
                FALSE----处理失败
抛出异常    无
**********************************************************************/
BOOL CSaveChannel::DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead,
                                     BYTE* pMainLastUnit, BYTE* pBakLastUnit)
{
    BOOL bSucc;

    //颜文远2002.05.18修改，问题单号D13767
    //当主备状态文件的文件序列号不等时，删除最后一个话单文件
    if (((BS_STATUS_FILE_ELEMENT* )pMainLastUnit)->uFileCsn
              > ((BS_STATUS_FILE_ELEMENT* )pBakLastUnit)->uFileCsn)
    {
        TRACE(m_szModuName, S_BS_TRC_DEL_LAST_FILE,
              ((BS_STATUS_FILE_ELEMENT* )pMainLastUnit)->uFileCsn);
        bSucc = DeleteTempFile();
        if (!bSucc)
        {
            return FALSE;
        }

        //将备状态文件头写入主状态文件,使主状态文件头和备状态文件头一致
        bSucc = m_InfoMain.WriteHead(pBakHead, m_uHeadLen);
        if (!bSucc)
        {
            return FALSE;
        }
        bSucc = m_InfoMain.Flush();
        if (!bSucc)
        {
            return FALSE;
        }
    }
    //颜文远2002.05.18修改结束
    else
    //备状态文件新于主状态文件
    //将备状态文件覆盖主状态文件,使主状态文件头和备状态文件一致
    {
        BOOL bSucc = m_InfoMain.Delete();
        if (!bSucc)
        {
            return FALSE;
        }

        bSucc =
            FileCopy(m_InfoBak.GetFileFullPath().c_str(), m_InfoMain.GetFileFullPath().c_str());
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************
功能描述    根据状态文件里的信息对相关的成员变量进行初始化
输入参数    无
输出参数    无
返回值      操作是否成功
                TRUE----操作成功
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CSaveChannel::InitByStatusFile()
{
    BOOL bSucc = m_InfoMain.ReadHead(m_pHead);
    if (!bSucc)
    {
        return FALSE;
    }
    //颜文远2002.05.18修改，问题单号D13767
    m_uCurCsn = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn;
    m_uPID = ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
    //颜文远2002.05.18修改结束

    int nResult = m_InfoMain.ReadLastUnit(m_pLastInfoUnit);
    if ((ERR_FAIL_READ == nResult) || (ERR_INDEX_INVALID == nResult))
    {
        TraceModuleErr(MTS_BACKSAVE, nResult);
        return FALSE;
    }
    else if (ERR_SUCCESS == nResult)
    //读取状态文件成功，则根据状态文件内容初始化相关成员变量
    {
        //得到最后一个文件的全路径，并据此初始化m_pBillFile
        //颜文远2002.04.13修改，问题单号D12665
/*      m_szFileName = m_szPrefix + GetFileCsnStr(m_uCurCsn)
                       + "." + m_szPostfix;*/
        //在双机对状态文件进行同步的情况下，m_pHead以及m_pLastInfoUnit
        //中的文件序列号可能不一致，必须以m_pLastInfoUnit中的为准打开
        //上次运行时产生的最后文件
        m_szFileName = m_szPrefix
            + GetFileCsnStr(((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFileCsn)
            + "." + m_szPostfix;
        //颜文远2002.04.13修改结束
        STRING szFileFullPath = m_szFilePath + "/" + m_szFileName;
        m_pBillFile = fopen(szFileFullPath.c_str(), "rb+");

        //初始化m_tmCreateTime
        if (NULL != m_pBillFile)
        {
            //by yangshan 2004-6-23  SWPD06194
            fseek(m_pBillFile,((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos,SEEK_SET);
            //the end 2004-6-23  SWPD06194
            
            /*modify by chenliangwei, 2004-06-02, Unix下的生成时间不能由文件的st_ctime决定，
              因此改由从状态文件的uReserved字段获取*/
            time_t tFileCreateTime = ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uReserved;

            //状态文件中的时间无效的话，从文件属性中读取
            if(tFileCreateTime == 0)
            {
                ACE_stat FileStat;
                ACE_OS::stat(szFileFullPath.c_str(), &FileStat);
                tFileCreateTime = FileStat.st_ctime;
            }
            
            tm* ptmCreateTime = localtime(&tFileCreateTime);
            m_tmCreateTime = *ptmCreateTime;

            //为了防止最后一个日期目录被删除，所以初始化时创建一下该日期目录
            char szLastDate[20];
            strftime(szLastDate, 20, "%Y%m%d", &m_tmCreateTime);
            m_szFirstPath = m_szDatePath + "/" + szLastDate;
            ACE_OS::mkdir(m_szFirstPath.c_str());
        }
    }

    return TRUE;
}

/**********************************************************************
功能描述    创建新的话单文件以及相关的状态文件刷新操作
输入参数    无
输出参数    无
返回值      创建新文件是否成功：
                TRUE----创建成功
                FALSE----创建失败
抛出异常    无
备注        后存盘中，一个话单文件对应状态文件中的一个状态记录，所以
            在创建新的话单文件后，必须创建一个新的状态记录，往硬盘中
            写状态头和该状态记录，并进行Flush操作
**********************************************************************/
BOOL CSaveChannel::CreateNewFile()
{
    BOOL bSucc = CSaveTransaction::CreateNewFile();
    if (!bSucc)
    {
        return FALSE;
    }

    //刷新状态文件

    //颜文远2002.05.18修改，问题单号D13767
    //刷新状态记录，并写该状态记录
    ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFirstPID
                = ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
    ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFileCsn = m_uCurCsn;
    ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uBillCsn
                = ((BS_STATUS_FILE_HEAD* )m_pHead)->uBillCsn;
    ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uReserved
                = mktime(&m_tmCreateTime);

    bSucc = m_InfoMain.WriteUnit(m_pLastInfoUnit, m_uUnitLen);
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoBak.WriteUnit(m_pLastInfoUnit, m_uUnitLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //刷新状态头，并写状态头
    m_InfoMain.IncreaseIndex(m_pHead);

    //回退一步，让状态文件头部中的文件CSN始终定义为正式文件的CSN
    m_uCurCsn = m_uCurCsn - m_uFileCsnStep;
    if ((m_uCurCsn > m_uFileCsnEnd) || ((int)m_uCurCsn < (int)(m_uFileCsnStart - m_uFileCsnStep)))
    {
        m_uCurCsn = m_uFileCsnStart - m_uFileCsnStep;
    }
    
    ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn = m_uCurCsn;
    ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos = 0;
    m_InfoMain.CalcCheckSum(m_pHead);
    bSucc = m_InfoMain.WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoMain.Flush();
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoBak.WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoBak.Flush();
    if (!bSucc)
    {
        return FALSE;
    }
    //颜文远2002.05.18修改结束

    m_uFileExistTime = 0;

    return TRUE;
}

//根据收到的包信息判断是否为空包
BOOL CSaveChannel::IsEmptyPacket(BYTE* pInfo)
{
    SBackPacketInfo* pBackPacketInfo = (SBackPacketInfo* )pInfo;

    return (0 == pBackPacketInfo->uLen);
}

/****************************************************************************
函数原型    public: virtual void DelMoreFiles()
功能描述    删除多余文件任务处理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*****************************************************************************/
void CSaveChannel::DelMoreFiles()
{
    ACE_DIR *pDir = NULL;
    ACE_DIR *pChnDir = NULL;
    dirent *pFileDirent = NULL;
    dirent *pChnDirent = NULL;

    //ACE_Time_Value curTime;
    time_t DirTime,curTime;
    ACE_stat sDirStat;

    char szSrcFilePath[MAX_PATH];
    int nRet = 0;

    //通道0不用删除
    if(0 == strcmp(m_szChannelName.c_str(), CHANNEL0))
    {
        return;
    }
    //以*.*作为条件在sChlName指向的目录中出第一个文件；
    strcpy(szSrcFilePath, m_szDatePath.c_str());
    pChnDir = ACE_OS::opendir(szSrcFilePath);
    if(pChnDir == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_OPEN_DIR_FAILED;
        sTmpInfo += szSrcFilePath;
        CWorkModule::MessageOut(MTS_BACKSAVE, MSGOUT_LEVEL_IMPORTANT, sTmpInfo.c_str());
        //by ldf 2004-01-28 对core的定位发现此处存在问题
        sTmpInfo =szSrcFilePath;
        sTmpInfo += ',';
        //end
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKSAVE, sTmpInfo.c_str());
        //Added by ZhengYuqun 2003-09-08 SWPD01517
        //by ldf 2004-01-28 对core的定位发现此处存在问题
        //ACE_OS::closedir(pDir);
        //end
        return;
    }

    while((pFileDirent = ACE_OS::readdir(pChnDir)) != NULL)
    {
        SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",
                    m_szDatePath.c_str(), pFileDirent->d_name);
        szSrcFilePath[MAX_PATH - 1] = 0;

        ACE_OS::stat(szSrcFilePath, &sDirStat);
        if(S_IFDIR != (sDirStat.st_mode & S_IFMT) || TRUE != IsDigitValue(pFileDirent->d_name))
        {
            //查找下一个dir;
            continue;
        }

        int nDateNum = ACE_OS::atoi(pFileDirent->d_name);
        tm tTmp;
        memset(&tTmp,0,sizeof(tm));
        tTmp.tm_year = (nDateNum / 10000) - 1900;
        tTmp.tm_mon = (nDateNum % 10000)/100 - 1;
        tTmp.tm_mday = (nDateNum % 10000)%100;
        DirTime = mktime(&tTmp); //得到目录时间的基准值
        //转换当前文件日期
        time_t DirTime2 = sDirStat.st_mtime;
        tTmp = *localtime(&DirTime2);
        tTmp.tm_hour = 0;
        tTmp.tm_min = 0;
        tTmp.tm_sec = 0;
        DirTime2 = mktime(&tTmp);
        //by ldf 2004-01-29 这个的trace没有什么意义，资源文件也未定义，去掉之
        //if(DirTime2 != DirTime)
        //{
        //  TRACE(MTS_BACKSAVE,S_BACKUP_TIMEERROR,DirTime,DirTime2);
        //}
        //取当前时间
        time(&curTime);
        tTmp = *localtime(&curTime);
        tTmp.tm_hour = 0;
        tTmp.tm_min = 0;
        tTmp.tm_sec = 0;
        curTime = mktime(&tTmp); //得到当前的日期，时分秒都为0
        //修改结束
        if(difftime(curTime,DirTime) <= m_uBillSavedDays * 24 * 3600)
        {
            //查找下一个文件;
            continue;
        }

        nRet = PrepareFiles(szSrcFilePath, 0);
        if (nRet != ERR_SUCCESS)
        {
            break;
        }
        if(0 == m_FileList.size())
        {
            //查找下一个文件;
            SweepDirectory(szSrcFilePath);
            continue;
        }


        //删除指定目录下的话单文件
        DelOneDirBillFiles(szSrcFilePath);

        //删除话单文件列表，包括内部结构的指针;
        LISTFILELISTNODE::iterator iter = m_FileList.begin();
        while(iter != m_FileList.end())
        {
            delete (*iter);
            iter ++;
        }
        m_FileList.clear();
        //查找下一个文件。
    }

    //删除话单文件列表内容，包括内部结构的指针;
    if(m_FileList.size() != 0)
    {
        LISTFILELISTNODE::iterator iter = m_FileList.begin();
        while(iter != m_FileList.end())
        {
            delete (*iter);
            iter ++;
        }
        m_FileList.clear();
    }
    ACE_OS::closedir(pChnDir);

}

//Added by ZhengYuqun SWPD03912 2004-03-03
//查找第二份话单目录下的第一个文件
STRING CSaveChannel::FindFirstFileInSecondPath()
{
#ifdef _PLATFORM_WIN32
#define S_ISDIR(mode)   (((mode) & S_IFMT) == S_IFDIR)
#endif

    int nRet = 0;
    dirent *pDirent;
    ACE_stat astat;
    STRING szCurFile;

    if(m_bSecondCopy && !m_szSecondPath.empty())
    {
        ACE_DIR *pSecondDIR = ACE_OS_Dirent::opendir(m_szSecondPath.c_str());
        if( pSecondDIR != NULL)
        {
            // 查找目录下的第一个文件
            while((pDirent = ACE_OS_Dirent::readdir(pSecondDIR)) != NULL)
            {
                // 忽略目录
                szCurFile = m_szSecondPath;
                szCurFile = MakePath(szCurFile, pDirent->d_name);
                nRet = ACE_OS::stat(szCurFile.c_str(), &astat);
                if(nRet != 0)
                {
                    continue;
                }

                nRet = S_ISDIR(astat.st_mode);
                if(nRet != 0 )
                {
                    continue;
                }
                else
                {
                    ACE_OS_Dirent::closedir(pSecondDIR); //SWPD05411 Added by ZhengYuqun 2004-05-25
                    return szCurFile;
                }
            }
            ACE_OS_Dirent::closedir(pSecondDIR); //SWPD05411 Added by ZhengYuqun 2004-05-25
        }
    }

    return STRING("");
}

//格式化前缀字符串
STRING CSaveChannel::FormatCfgStr(const char* szFormat, 
                                  const tm&   tmTime, 
                                  const UINT4 uChannelID,
                                  const INT4  nRecordCnt)
{
    //替换前缀中的特殊字符
    //by ldf 2004-02-17 阿联酋
    char szYear[10], szYear2[10], szMonth[10], szDay[10], szChannel[10];
    char szHour[10], szHour2[10], szMinute[10], szSecond[10];
    char szAmPm[10],szWeek[10];
    //end
    sprintf(szYear,  "%04d", tmTime.tm_year + 1900);
    sprintf(szYear2, "%02d", tmTime.tm_year%100);
    sprintf(szMonth, "%02d", tmTime.tm_mon + 1);
    sprintf(szDay,   "%02d", tmTime.tm_mday);

    sprintf(szHour,  "%02d", tmTime.tm_hour);
    if(tmTime.tm_hour%12 != 0)
    {
        sprintf(szHour2, "%02d", tmTime.tm_hour%12);
    }
    else
    {
        sprintf(szHour2, "%02d", 12);
    }

    if(tmTime.tm_hour > 11 )
    {
        sprintf(szAmPm, "%s", "p.m.");
    }
    else
    {
        sprintf(szAmPm, "%s", "a.m.");
    }

    sprintf(szMinute, "%02d", tmTime.tm_min);
    sprintf(szSecond, "%02d", tmTime.tm_sec);

    sprintf(szWeek,   "%d",   tmTime.tm_wday);

    sprintf(szChannel, "%02d", uChannelID);

    char szPrefix[512], szPrefix1[512], szPrefix2[512], szPrefix3[512],
        szPrefix4[512], szPrefix5[512], szPrefix6[512], szPrefix7[512],
        szPrefix8[512], szPrefix9[512], szPrefix10[512], szPrefix11[512],
        szPrefix12[512];
    ReplaceSubStr(szFormat,  szPrefix1, "%y", szYear2);
    ReplaceSubStr(szPrefix1, szPrefix2, "%Y", szYear);
    ReplaceSubStr(szPrefix2, szPrefix3, "%m", szMonth);
    ReplaceSubStr(szPrefix3, szPrefix4, "%d", szDay);

    ReplaceSubStr(szPrefix4, szPrefix5, "%H", szHour);
    ReplaceSubStr(szPrefix5, szPrefix6, "%I", szHour2);
    ReplaceSubStr(szPrefix6, szPrefix7, "%M", szMinute);
    ReplaceSubStr(szPrefix7, szPrefix8, "%S", szSecond);

    ReplaceSubStr(szPrefix8, szPrefix9, "%p", szAmPm);

    ReplaceSubStr(szPrefix9, szPrefix10, "%w", szWeek);
    ReplaceSubStr(szPrefix10, szPrefix11, "%%", "%");
    ReplaceSubStr(szPrefix11, szPrefix12, "%c", szChannel);
    ReplaceSubStr(szPrefix12, szPrefix, "%C", szChannel);

    //文件名中增加对文件记录数的处理，可以填为"%04N"的方式
    char* pDiv = strchr(szPrefix, '%');
    if(pDiv != NULL)
    {
        int i = 1;
        while(pDiv[i] >= '0' && pDiv[i] <= '9')
        {
            i++;
        }

        if(pDiv[i] == 'N')
        {
            //找出格式字符串，将'N'替换为'd'
            char szFmtStr[20] = "";
            memcpy(szFmtStr, pDiv, i + 1);
            szFmtStr[i] = 'd';
            szFmtStr[i + 1] = '\0';

            //格式化记录数字符
            char szRecordCnt[20] = "";
            SNPRINTF(szRecordCnt, sizeof(szRecordCnt), 
                     szFmtStr, nRecordCnt);
            szRecordCnt[sizeof(szRecordCnt) - 1] = '\0';

            //将'd'替换为'N'后，再用记录数字符串替换文件名
            char szPrefix13[512];
            STRNCPY(szPrefix13, szPrefix, sizeof(szPrefix13));
            szPrefix13[sizeof(szPrefix13) - 1] = '\0';
            szFmtStr[i] = 'N';
            ReplaceSubStr(szPrefix13, szPrefix, szFmtStr, szRecordCnt);
        }
    }
    //end
    
    return STRING(szPrefix);
}

STRING CSaveChannel::GetBillFileInfo(const char* szFilePath)
{
    if ((NULL == szFilePath) || ('\0' == szFilePath[0]))
    {
        return "";
    }

    char szFileInfo[63];
    SNPRINTF(szFileInfo, 51, "%-50s", m_szFormalFileName.c_str());
    szFileInfo[50] = '\0';    

    int nFileLen = FileSize(szFilePath);
    int nRecordCnt = nFileLen / m_nBillLen;
    SNPRINTF(&szFileInfo[50], 11, "%010d", nRecordCnt);
    szFileInfo[60] = '\r';
    szFileInfo[61] = '\n';
    szFileInfo[62] = '\0';

    return szFileInfo;
}

BOOL CSaveChannel::IsRepeatLog(const char* szLogMsg, FILE* pFile)
{
    if ((NULL == pFile) || (FileSize(pFile) < 62))
    {
        return FALSE;
    }

    char szLastLine[63] = "";
    int  nLineLen = 62;
    if (0 != fseek(pFile, -(nLineLen), SEEK_END))
    {
        return FALSE;
    }

    if (1 != fread(szLastLine, nLineLen, 1, pFile))
    {
        return FALSE;
    }
    szLastLine[62] = '\0';

    if (strncmp(szLastLine, szLogMsg, 50) == 0)
    {
        return TRUE;
    }

    return FALSE;

}

BOOL CSaveChannel::SaveCheckLog(const char* szLogMsg)
{
    if (szLogMsg == NULL)
    {
        return FALSE;
    }

    //计算稽核日志前缀
    STRING szLogPrefix = FormatCfgStr(m_szCheckLogPrefix, 
                                      m_tmCreateTime, 
                                      m_uChannelID);

    //计算稽核文件路径
    STRING szCheckLogFile = szLogPrefix + m_szCheckLogPostfix;
    szCheckLogFile = m_szCheckedLogPath + "/" + szCheckLogFile;

    FILE* pFile = fopen(szCheckLogFile.c_str(), "rb+");
    if (NULL == pFile)
    {
        pFile = fopen(szCheckLogFile.c_str(), "wb+");
        if (NULL == pFile)
        {
            TRACE(m_szModuName, 
                  S_BS_FAIL_OPEN_FILE, 
                  szCheckLogFile.c_str());
            return FALSE;
        }
    }

    //没有重复的日志，则写入
    if (!IsRepeatLog(szLogMsg, pFile))
    {
        if ((0 != fseek(pFile, 0, SEEK_END))
            || (1 != fwrite(szLogMsg, strlen(szLogMsg), 1, pFile))) 
        {
            TRACE(m_szModuName, 
                  S_BS_FAIL_WRITE_FILE, 
                  szCheckLogFile.c_str());
            fclose(pFile);
            pFile = NULL;
            return FALSE;
        }
        fflush(pFile);
    }

    fclose(pFile);
    pFile = NULL;

    return TRUE;

}

//by	杨善	2004-6-1	问题单SWPD05564
BOOL CSaveChannel::CloseCurFile()
{
	//如果需要进行话单适配转换
	if (m_pPatchEncap->GetbAviable() && m_uChannelID > 0 && NULL != m_pBillFile)
	{
		STRING szSrcFileFullPath = m_szFilePath + "/" + m_szFileName;		
		
		//截断当前的话单文件
		UINT4 uWriteFilePos = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos;
		int nResult = ACE_OS::truncate(szSrcFileFullPath.c_str(), uWriteFilePos);
		if (0 != nResult)
		{
			return FALSE;
		}
		
		if (!m_pPatchEncap->CanClose((char*)szSrcFileFullPath.c_str()))
		{
			return TRUE;
		}	
	}
	
	return CSaveTransaction::CloseCurFile();
}
//the end	问题单SWPD05564
