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

//����Զ2002.04.04��ӣ����ⵥ��D12661
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

//��ȡͨ����
const char* CSaveChannel::GetChlName()
{
    return m_szChannelName.c_str();
}

/**********************************************************************
��������    ��ʼ������
�������    uAPID----������
            uChannelID----ͨ����
            szChannelName----ͨ����
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ�
                TRUE----��ʼ���ɹ�
                FALSE----��ʼ��ʧ��
�׳��쳣    ��
��ע        Ϊ���ܹ���ʵ�ʿ����е����ù���ͬʱ�ﵽ����Ե�Ҫ�󣬺����
            �в��ö༶���õķ�ʽ�����ò��������ȼ��ӵ͵����ܹ���Ϊ
            ����/�����/ͨ��3����������߼���Ĳ���û�����ã�
            ��ȡ�ͼ�������ò���
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

    //�������ļ��ж�ȡ���ջ����ļ����ȣ����ò����Ķ�ȡԭ�������ͷ�еı�ע
    m_uFileLen = cfgIniFile.GetInt(CFG_SEC_DISK_FILE, CFG_FINAL_FILE_MAX_SIZE,
                                   CFG_FINAL_FILE_MAX_SIZE_DEFAULT);
    m_uFileLen = cfgIniFile.GetInt(szAPSection, CFG_FINAL_FILE_MAX_SIZE,
                                   m_uFileLen);
    m_uFileLen = cfgIniFile.GetInt(szChlSection, CFG_FINAL_FILE_MAX_SIZE,
                                   m_uFileLen);
    //added by maimaoshi,2002-10-29
    /*
    //m_uFileLen��ֵ�������64K
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
        //������ֵС����Сֵ64Kʱ,����Ϊ��Сָ64K
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_MINSIZE, m_uFileLen * 1024);
    }else if (m_uFileLen > 20 * 1024)
    {
        m_uFileLen = 20 * 1024;
        //������ֵ�������ֵ20Mʱ,����Ϊ���ֵ20M
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_MAXSIZE, m_uFileLen * 1024);

    }
    //end
    m_uFileLen *= 1024;     //���д�K���ֽڵ�ת��

    //�������ļ��ж�ȡ�ļ�����ʱ�䣬���ò����Ķ�ȡԭ�������ͷ�еı�ע
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
        //������ֵС����Сֵ5ʱ������Ϊ��Сֵ5
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_MINTIME, m_uFileTime);
    }else if (m_uFileTime > 3600)
    {
        m_uFileTime = 3600;
        //������ֵ�������ֵ3600ʱ������Ϊ���ֵ3600
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_FILE_MAXTIME, m_uFileTime);
    }
    //end

    sprintf(m_szPrefix, "%02d_", m_uChannelID);
    strcpy(m_szPostfix, "tmp");

    //�������ļ��ж�ȡ�ļ����кų��ȣ����ò����Ķ�ȡԭ�������ͷ�еı�ע
    m_nCsnLen = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                  CFG_CSN_LEN, CFG_CSN_LEN_DEFAULT);
    m_nCsnLen = cfgIniFile.GetInt(szAPSection, CFG_CSN_LEN, m_nCsnLen);
    m_nCsnLen = cfgIniFile.GetInt(szChlSection, CFG_CSN_LEN, m_nCsnLen);

    //�������ļ��ж�ȡ��ʼ���кţ����ò����Ķ�ȡԭ�������ͷ�еı�ע
    m_uFileCsnStart = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                     CFG_START_CSN, CFG_START_CSN_DEFAULT);
    m_uFileCsnStart = cfgIniFile.GetInt(szAPSection,
                                     CFG_START_CSN, m_uFileCsnStart);
    m_uFileCsnStart = cfgIniFile.GetInt(szChlSection,
                                     CFG_START_CSN, m_uFileCsnStart);

    //�������ļ��ж�ȡ�������кţ����ò����Ķ�ȡԭ�������ͷ�еı�ע
    UINT4 uDefaultEndCsn;

	//by	����	2004-5-29	��Ӧ���ⵥSWPD05385	
    if (m_nCsnLen >= 10 || m_nCsnLen < 0)
	//the end  2004-5-29	��Ӧ���ⵥSWPD05385
	
    //by    ����    2004-6-1    ��Ӧ���ⵥSWPD05593
    //����UINT4�Ĵ�С���ƣ������������к�ֻ֧�ֵ�9��9
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
    //the end   ��Ӧ���ⵥSWPD05593
   
    m_uFileCsnEnd = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                      CFG_END_CSN, uDefaultEndCsn);
    m_uFileCsnEnd = cfgIniFile.GetInt(szAPSection, CFG_END_CSN, m_uFileCsnEnd);
    m_uFileCsnEnd = cfgIniFile.GetInt(szChlSection, CFG_END_CSN, m_uFileCsnEnd);

    //�������ļ��ж�ȡ���кŲ��������ò����Ķ�ȡԭ�������ͷ�еı�ע
    m_uFileCsnStep = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                       CFG_CSN_STEP, CFG_CSN_STEP_DEFAULT);
    m_uFileCsnStep = cfgIniFile.GetInt(szAPSection,
                                       CFG_CSN_STEP, m_uFileCsnStep);
    m_uFileCsnStep = cfgIniFile.GetInt(szChlSection,
                                       CFG_CSN_STEP, m_uFileCsnStep);

    //�������ļ��ж�ȡ��ʽ�ļ���ǰ׺�����ò����Ķ�ȡԭ�������ͷ�еı�ע
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_PREFIX, CFG_PREFIX_DEFAULT,
                         m_szFormalPrefix, 512);
    cfgIniFile.GetString(szAPSection, CFG_PREFIX, m_szFormalPrefix,
                         m_szFormalPrefix, 512);
    cfgIniFile.GetString(szChlSection, CFG_PREFIX, m_szFormalPrefix,
                         m_szFormalPrefix, 512);

    //�������ļ��ж�ȡ��ʽ�ļ�����׺�����ò����Ķ�ȡԭ�������ͷ�еı�ע
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_MIDFIX, CFG_MIDFIX_DEFAULT,
                         m_szFormalMidfix, 512);
    cfgIniFile.GetString(szAPSection, CFG_MIDFIX, m_szFormalMidfix,
                         m_szFormalMidfix, 512);
    cfgIniFile.GetString(szChlSection, CFG_MIDFIX, m_szFormalMidfix,
                         m_szFormalMidfix, 512);

    //�������ļ��ж�ȡ��ʽ�ļ��ĺ�׺�����ò����Ķ�ȡԭ�������ͷ�еı�ע
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_POSTFIX, CFG_POSTFIX_DEFAULT,
                         m_szFormalPostfix, 512);
    cfgIniFile.GetString(szAPSection, CFG_POSTFIX, m_szFormalPostfix,
                         m_szFormalPostfix, 512);
    cfgIniFile.GetString(szChlSection, CFG_POSTFIX, m_szFormalPostfix,
                         m_szFormalPostfix, 512);

    //�õ����������ջ�����ŵĸ�Ŀ¼
    char szFinalRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACK_SAVE_ROOT_DIR,
                         CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                         szFinalRootDir, MAX_PATH);
    CreateDeepDir(szFinalRootDir);

    //�õ���������״̬�ļ��ĸ�Ŀ¼
    char szMainStatusRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_MAIN_STATUS_ROOT_DIR,
                         CFG_MAIN_STATUS_ROOT_DIR_DEFAULT,
                         szMainStatusRootDir, MAX_PATH);
    CreateDeepDir(szMainStatusRootDir);

    //�õ���������״̬�ļ��ĸ�Ŀ¼
    char szBakStatusRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_BAK_STATUS_ROOT_DIR,
                         CFG_BAK_STATUS_ROOT_DIR_DEFAULT,
                         szBakStatusRootDir, MAX_PATH);
    CreateDeepDir(szBakStatusRootDir);

    //�õ����������������Ӧ�Ľ����Ŀ¼
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

    //�������ļ��е�DiskFile�У���ȡ�����ļ��ı�����������ʼ����Ա����
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

    //�����ļ��ı�������������Ҫ7��,�����45��
    if(m_uBillSavedDays < 7)
    {
        m_uBillSavedDays = 7;
        //��ʾ���õ�ֵС����Сֵ7������Ϊ��Сֵ7
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_AUTODEL_MINDAY, m_uBillSavedDays);
    }else if (m_uBillSavedDays > 180) //Modified by ZhengYuqun SWPD04451 2004-03-27
    {
        m_uBillSavedDays = 180;
        //��ʾ���õ�ֵ�������ֵ180������Ϊ���ֵ180
        TRACE(m_szModuName, S_BS_TRC_ERR_CFG_AUTODEL_MAXDAY, m_uBillSavedDays);
    }
    //end

    ACE_OS::mkdir(szChannelPath);
    sprintf(szMainStatusPath, "%s/%s", szMainStatusRootDir, szAPName);
    ACE_OS::mkdir(szMainStatusPath);
    sprintf(szBakStatusPath, "%s/%s", szBakStatusRootDir, szAPName);
    ACE_OS::mkdir(szBakStatusPath);

    //��������״̬�ļ�Ŀ¼
    sprintf(szMainStatusPath, "%s/%s/%s",
            szMainStatusRootDir, szAPName, "save");
    CreateDeepDir(szMainStatusPath);
    sprintf(szBakStatusPath, "%s/%s/%s",
            szBakStatusRootDir, szAPName, "save");
    CreateDeepDir(szBakStatusPath);

    //����ͨ����Ŀ¼����ʱ�ļ���Ŀ¼
    m_szDatePath = STRING(szChannelPath) + "/" + m_szChannelName;
    ACE_OS::mkdir(m_szDatePath.c_str());
    m_szFilePath = STRING(szChannelPath) + "/" + "temp";
    ACE_OS::mkdir(m_szFilePath.c_str());

    //��������״̬�ļ�����ز���
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

    //��״̬ͷ�������������ȱʡֵ�����״̬�ļ������ڣ���ȱʡֵд��״̬�ļ�
    BS_STATUS_FILE_HEAD* pHead = (BS_STATUS_FILE_HEAD* )m_pHead;
    pHead->uBillCsn = 1;

    //����Զ2002.05.18�޸ģ����ⵥ��D13767
    pHead->uFileCsn = m_uFileCsnStart - m_uFileCsnStep;
    //����Զ2002.05.18�޸Ľ���

    pHead->uLastPID = 0;
    pHead->uStatusFileWritePos = m_uHeadLen;
    m_InfoMain.CalcCheckSum(m_pHead);

    //�������ļ��ж�ȡFlush�İ���������ò����Ķ�ȡԭ�������ͷ�еı�ע
    m_uMaxFlushSpan = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                CFG_MAX_FLUSH_SPAN, CFG_MAX_FLUSH_SPAN_DEFAULT);
    m_uMaxFlushSpan = cfgIniFile.GetInt(szAPSection,
                                CFG_MAX_FLUSH_SPAN, m_uMaxFlushSpan);
    m_uMaxFlushSpan = cfgIniFile.GetInt(szChlSection,
                                CFG_MAX_FLUSH_SPAN, m_uMaxFlushSpan);

    //���ú͵ڶ������ջ�����صĲ���
    //�������ļ��ж�ȡ��ʽ�ļ��ĺ�׺�����ò����Ķ�ȡԭ�������ͷ�еı�ע
    //���ú͵ڶ������ջ�����صĲ���
    //�������ļ��ж�ȡ��ʽ�ļ��ĺ�׺�����ò����Ķ�ȡԭ�������ͷ�еı�ע
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
        //����Զ2002.04.28��ӣ����ⵥ��D12661
        //�޸ĵڶ��ݻ����ļ���Ȩ��Ϊ�ɶ�д���Ա�Ʒ������ܹ�ɾ��
#ifndef _PLATFORM_WIN32
        if (0 != chmod(m_szSecondPath.c_str(),
                       S_IRUSR | S_IWUSR | S_IXUSR
                       | S_IRGRP | S_IWGRP | S_IXGRP
                       | S_IROTH | S_IWOTH | S_IXOTH))
        {
            return FALSE;
        }
#endif
        //����Զ2002.04.28��ӽ���
    }
    //�������ļ��ж�ȡ�Ʒ�����ȡ�����ĳ�ʱʱ�䣬��λΪ���ӣ�
    //���ò����Ķ�ȡԭ�������ͷ�еı�ע
    m_nFetchFileTimeOut = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,
                                            CFG_FETCH_FILE_TIMEOUT,
                                            CFG_FETCH_FILE_TIMEOUT_DEFAULT);
    m_nFetchFileTimeOut = cfgIniFile.GetInt(szAPSection,
                                            CFG_FETCH_FILE_TIMEOUT,
                                            m_nFetchFileTimeOut);
    m_nFetchFileTimeOut = cfgIniFile.GetInt(szChlSection,
                                            CFG_FETCH_FILE_TIMEOUT,
                                            m_nFetchFileTimeOut);
    m_nFetchFileTimeOut *= 60;  //���дӷֵ����ת��
    m_bFetchFileAlarmSend = FALSE;
    m_szCheckedFilePath = FindFirstFileInSecondPath(); //Modified by ZhengYuqun SWPD03912 2004-03-03
    m_nCheckTimeCounter = 0;
/* Begin:   Adder: Gan Zhiliang   Date: 2003-4-23*/
    m_uResFileProductTime = cfgIniFile.GetInt(szAPSection,
                                            "ResFileProductTime",
                                            0);
/* End  : Adder: Gan Zhiliang      Date: 2003-4-23*/

    //�Ƿ�������־����
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

        //������־�ļ���ǰ׺
        cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_CHECK_LOG_PREFIX, 
                             CFG_CHECK_LOG_PREFIX_DEFAULT,
                             m_szCheckLogPrefix, sizeof(m_szCheckLogPrefix));
        cfgIniFile.GetString(szAPSection, CFG_CHECK_LOG_PREFIX, 
                             m_szCheckLogPrefix,
                             m_szCheckLogPrefix, sizeof(m_szCheckLogPrefix));
        cfgIniFile.GetString(szChlSection, CFG_CHECK_LOG_PREFIX, 
                             m_szCheckLogPrefix,
                             m_szCheckLogPrefix, sizeof(m_szCheckLogPrefix));

        //������־�ļ��ĺ�׺
        cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_CHECK_LOG_POSTFIX, 
                             CFG_CHECK_LOG_POSTFIX_DEFAULT,
                             m_szCheckLogPostfix, sizeof(m_szCheckLogPostfix));
        cfgIniFile.GetString(szAPSection, CFG_CHECK_LOG_POSTFIX, 
                             m_szCheckLogPostfix,
                             m_szCheckLogPostfix, sizeof(m_szCheckLogPostfix));
        cfgIniFile.GetString(szChlSection, CFG_CHECK_LOG_POSTFIX, 
                             m_szCheckLogPostfix,
                             m_szCheckLogPostfix, sizeof(m_szCheckLogPostfix));
        
        
        //������־��ͨ��Ŀ¼
        char szCheckLogChlPath[MAX_PATH];
        SNPRINTF(szCheckLogChlPath, 
                 sizeof(szCheckLogChlPath),
                 "%s/%s/%s/%s", 
                 szFinalRootDir, "CheckLog", 
                 szAPName, m_szChannelName.c_str());
        szCheckLogChlPath[sizeof(szCheckLogChlPath) - 1] = '\0';
        CreateDeepDir(szCheckLogChlPath);

        m_szCheckedLogPath = szCheckLogChlPath;

        //���ſɶ�дȨ��
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
    if(bSucc && m_bAlwaysGenFile)    //���Ҫ��������ϵ������ļ�����ôһ�����ʹ�����ʱ�ļ�
    {
        bSucc = CreateNewFile();
    }

    return bSucc;
}

//���¼�������
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
    //����WLAN����Ҫ�����⴦����ʹ��ԭ���Ĵ���
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


    //�����ǰ�����ļ��Ѿ���ʱ����رյ�ǰ�����ļ�
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
            //���Ҫ��������ϵ������ļ�����ôÿ������һ����ʽ�ļ�����������һ���µ���ʱ�ļ�
            else if(m_bAlwaysGenFile)
            {
                if(!CreateNewFile())
                {
                    return FALSE;
                }
            }
        }
    }

    //���Ʒ������Ƿ�ʱ��ȡ���˻����ļ�
    CheckFetchingFile();

    return TRUE;
}

/**********************************************************************
��������    ��������ָ�ʱ��Ҫ��״̬����
�������    ��
�������    uLastPID----��󱣴�Ļ�����
            uLastBillCsn----�����滰�������һ�Ż�������ˮ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSaveChannel::GetResumeParam(UINT4& uLastPID, UINT4& uLastBillCsn)
{
    uLastPID = ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
    uLastBillCsn = ((BS_STATUS_FILE_HEAD* )m_pHead)->uBillCsn;
}

//����״̬�ļ��õ����һ���������İ���
UINT4 CSaveChannel::GetLastPID()
{
    return ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
}

//����״̬�ļ��õ��������к�
UINT4 CSaveChannel::GetBillCsn()
{
    return ((BS_STATUS_FILE_HEAD* )m_pHead)->uBillCsn;
}

//���Ʒ������Ƿ��Ѿ���ʱȡ�߻����ļ���ÿ���ӱ�����һ��
void CSaveChannel::CheckFetchingFile()
{
    m_nCheckTimeCounter++;

    //���ֻ����һ�ݣ����ý��м��
    if (!m_bSecondCopy)
    {
        return;
    }

    if (m_nCheckTimeCounter >= m_nFetchFileTimeOut)
    {
        //��鱻���ӵ��ļ��Ƿ��Ѿ���ȡ��
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

            //Ϊ�˷�ֹ�����澯ģ��ĸ澯��Ϣ��ʧ��
            //ֻҪ�ļ��Ѿ���ȡ�ߣ����ͻָ��澯
            SInnerAlarm Alarm;
            Alarm.yAlarmType = AT_RESTORE;
            Alarm.uAlarmID = ALARM_ID_NO_FETCH_FILE_FAULT;
            Alarm.yAlarmPara[0] = m_uChannelID;
            CMsgProcessor::SendAlarm(&Alarm);
            m_bFetchFileAlarmSend = FALSE;
        }
        else
        {
            //Ϊ�˷�ֹ�����澯ģ��ĸ澯��Ϣ��ʧ��
            //ֻҪ�ļ�δ��ȡ�ߣ����͹��ϸ澯
            SInnerAlarm Alarm;
            Alarm.yAlarmType = AT_FAULT;
            Alarm.uAlarmID = ALARM_ID_NO_FETCH_FILE_FAULT;
            Alarm.yAlarmPara[0] = m_uChannelID;
            CMsgProcessor::SendAlarm(&Alarm);
            m_bFetchFileAlarmSend = TRUE;
        }

        //�������ü�鱻�����ļ���ʱ���������
        //�ڷ����˹��ϸ澯������£������ļ������Ϊ�����
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

//�õ���һ�������ļ����Ŀ¼�����Ŀ¼�����ڣ��򴴽�֮��
//�ں�����У���ʱ�����ļ���Ŀ¼��Զ����ı�
void CSaveChannel::GetNextFilePath()
{
}

/**********************************************************************
��������    ɾ�����һ����ʱ�ļ����ڽ���״̬�ļ�У��ʱ����
�������    ��
�������    ��
����ֵ      ɾ���ļ��Ƿ�ɹ�
                TRUE----ɾ���ɹ�
                FALSE----ɾ��ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveChannel::DeleteTempFile()
{
    BS_STATUS_FILE_ELEMENT LastUnitInfo;

    //��ȡ��״̬�ļ������һ��״̬��¼
    int nResult = m_InfoMain.ReadLastUnit((BYTE* )&LastUnitInfo);
    if ((ERR_INDEX_INVALID == nResult) || (ERR_FAIL_READ == nResult))
    {
        return FALSE;
    }
    else if (ERR_NO_UNIT_EXIST == nResult)
    {
        return TRUE;
    }

    //�������һ��״̬��¼������һ���ļ���·��������ɾ��
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
��������    �����յ��Ļ�����������״̬�ļ������ݼ�����Ա����
�������    pPacketInfo----�������������Ϣ��������������������Խ��ͣ�
                �ں����⸺��������ͷ�
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSaveChannel::RefreshStatus(BYTE* pPacketInfo)
{
    //�Դ������Ϣ��������ת��
    SBackPacketInfo* pBackPacketInfo = (SBackPacketInfo* )pPacketInfo;
    m_uPID = pBackPacketInfo->uPID;

    //���ݴ������Ϣ�����ļ�ͷ��������µ�ǰ״̬��¼
    BS_STATUS_FILE_HEAD* pHead = (BS_STATUS_FILE_HEAD* )m_pHead;
    pHead->uLastPID = pBackPacketInfo->uPID;
    pHead->uFileCsn = m_uCurCsn;
    pHead->uBillCsn = pBackPacketInfo->uBillCsn;
    pHead->uFileWritePos += pBackPacketInfo->uLen;
    m_InfoMain.CalcCheckSum(m_pHead);
}

/**********************************************************************
��������    �رջ����ļ��ĺ�������
�������    ��
�������    ��
����ֵ      �����Ƿ�ɹ�
                TRUE----����ɹ�
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveChannel::DoWhenFileClose()
{
    ASSERT(NULL == m_pBillFile);

        
    BOOL bSucc;

    //�ضϵ�ǰ�Ļ����ļ�
    UINT4 uWriteFilePos = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos;
    STRING szFileFullPath = m_szFilePath + "/" + m_szFileName;
    int nResult = ACE_OS::truncate(szFileFullPath.c_str(), uWriteFilePos);
    if (0 != nResult)
    {
        return FALSE;
    }

    //���ļ������������ļ�¼��    
    int nRecordCnt = 0;
    if(m_nBillLen != 0)
    {
        nRecordCnt = uWriteFilePos / m_nBillLen;
    }
    
    //������ʽ�Ļ����ļ���
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
	//by	����	2004-5-29	��Ӧ���ⵥSWPD05385
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
	//the end 	2004-5-29	��Ӧ���ⵥSWPD05385
	
    if(m_bCheckLog)
    {
        STRING szSrcFileFullPath = m_szFilePath + "/" + m_szFileName;
        STRING szFileInfo;
        szFileInfo = GetBillFileInfo(szSrcFileFullPath.c_str());
        SaveCheckLog(szFileInfo.c_str());
    }

    //����������ݣ������ʱ�ļ������õ��ͼƷ����ĵĵڶ����ļ�
    if (m_bSecondCopy)
    {
        STRING szSrcFileFullPath = m_szFilePath + "/" + m_szFileName;
        STRING szDstFileFullPath = m_szSecondPath + "/" + m_szFormalFileName;

        //�����Ҫ���л�������ת��
        if ((m_pPatchEncap->GetbAviable()) && (m_uChannelID > 0) && (uWriteFilePos > 0))
        {
            char* szDest;
            szDest = (char*)szDstFileFullPath.c_str();
            BOOL bRet = m_pPatchEncap->Process(szSrcFileFullPath.c_str(), szDest, m_uChannelID);
            if (!bRet)
            {
                //���ͻ����쳣�澯  modify by lx SWPD03595
                SInnerAlarm sAlarm;
                sAlarm.yAlarmType = AT_EVENT;
                sAlarm.uAlarmID = ALARM_ID_ABNORMAL_BILL;
                sAlarm.yAlarmPara[0] = ALARM_PARA_ENCODING_ERR;
                CMsgProcessor::SendAlarm(&sAlarm);
                
                //�����ļ�copyһ�ݵ�secondĿ¼�µ�default��Ŀ¼�У�������׺����Ϊ.err
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

        //����Զ2002.04.04��ӣ����ⵥ��D12661
        //�޸ĵڶ��ݻ����ļ���Ȩ��Ϊ�ɶ�д���Ա�Ʒ������ܹ�ɾ��
        #ifndef _PLATFORM_WIN32
        if (0 != chmod(szDstFileFullPath.c_str(),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH))
        {
            return FALSE;
        }
        #endif
        //����Զ2002.04.04��ӽ���

    }

    //�����ǰ�ļ��Ĵ����������ϸ��ļ��Ĵ������ڲ�һ��������봴���µ�����Ŀ¼
    if ((m_tmCreateTime.tm_year != m_tmLastFileTime.tm_year)
            || (m_tmCreateTime.tm_mon != m_tmLastFileTime.tm_mon)
            || (m_tmCreateTime.tm_mday != m_tmLastFileTime.tm_mday))
    {
        char szDateName[20];
        strftime(szDateName, 20, "%Y%m%d", &m_tmCreateTime);
        m_szFirstPath = m_szDatePath + "/" + szDateName;

        //by ldf 2004-02-18 ��Ӧ���ⵥ SWPD03439
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
        //ɾ��������ļ�
        //DelMoreFiles();
        //end
    }

    //����ʱ�ļ���Ϊ��ʽ�ļ�
    STRING szSrcFileFullPath = m_szFilePath + "/" + m_szFileName;
    STRING szDstFileFullPath = m_szFirstPath + "/" + m_szFormalFileName;
    bSucc = FileCopyEx(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
    if (!bSucc)
    {
        return FALSE;
    }
    FileDel(szSrcFileFullPath.c_str());


    //ˢ�²�д״̬�ļ�
    ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn = m_uCurCsn;
    m_InfoMain.CalcCheckSum(m_pHead);

    //����Զ2002.04.12��ӣ����ⵥ��D12568
    bSucc = m_InfoMain.WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }
    //����Զ2002.04.12��ӽ���
    bSucc = m_InfoMain.Flush();
    if (!bSucc)
    {
        return FALSE;
    }
    //����Զ2002.04.12��ӣ����ⵥ��D12568
    bSucc = m_InfoBak.WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }
    //����Զ2002.04.12��ӽ���
    bSucc = m_InfoBak.Flush();
    if (!bSucc)
    {
        return FALSE;
    }

    //Added by ZhengYuqun ���ɿ��ļ�ʱͬ��״̬ SWPD04389 2004-03-26
    //����ͬ������
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

    //����Զ2002.03.16�޸ģ����ⵥ��D11867
    char szLogInfo[255];
    SNPRINTF(szLogInfo, 255, S_BS_LOG_CREATE_NEW_FILE,
            m_szChannelName.c_str(), m_szFormalFileName.c_str());
    CMsgProcessor::WriteLog(MTS_BACKSAVE, szLogInfo);
    //����Զ2002.03.16�޸Ľ���

    return TRUE;
}

/**********************************************************************
��������    �ڱ��滰���������е�д״̬�ļ�����
�������    pInfoFile----Ҫд���״̬�ļ�
            bFlush----�Ƿ����Flush����
�������    ��
����ֵ      д״̬�ļ��Ƿ�ɹ�
                TRUE----д�ɹ�
                FALSE----дʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveChannel::WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush)
{
    //������У�ֻ��д״̬ͷ����д״̬�ļ���Flush��������ͬʱ���У�
    //���ԣ�ֻ����Ҫ�����Flush����������²Ž���д״̬�ļ�����
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
��������    �ڱ��滰���������е�д�����ļ�����
�������    pPacketInfo----�������������Ϣ���ں����⸺��������ͷ�
            bFlush----�Ƿ����Flush����
�������    ��
����ֵ      ����ֵ˵�����£�
                0----��ȡ�ɹ�
                1----��ȡʧ��
                <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ��
                    ERR_WRITE_BILLFILE_FAIL----д�����ļ�ʧ��
�׳��쳣    ��
**********************************************************************/
int CSaveChannel::WriteBillFile(BYTE* pPacketInfo, BOOL bFlush)
{
    BOOL bSucc;
    //�Դ������Ϣ��������ת��
    SBackPacketInfo* pBackPacketInfo = (SBackPacketInfo* )pPacketInfo;

    //ֻ���ڻ������ǿյ�����£�����ʵ�ʵĴ������ļ���д�̵Ȳ�����
    //�������κβ���
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
            //����Flush����
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
��������    ��������У��ʱ����״̬�ļ���һ�µ����
�������    pMainHead----��״̬�ļ����ļ�ͷ
            pBakHead----��״̬�ļ����ļ�ͷ
            pMainLastUnit----��״̬�ļ������һ��״̬��¼
            pBakLastUnit----��״̬�ļ������һ��״̬��¼
�������    ��
����ֵ      �����Ƿ�ɹ�
                TRUE----����ɹ�
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveChannel::DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead,
                                     BYTE* pMainLastUnit, BYTE* pBakLastUnit)
{
    BOOL bSucc;

    //����Զ2002.05.18�޸ģ����ⵥ��D13767
    //������״̬�ļ����ļ����кŲ���ʱ��ɾ�����һ�������ļ�
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

        //����״̬�ļ�ͷд����״̬�ļ�,ʹ��״̬�ļ�ͷ�ͱ�״̬�ļ�ͷһ��
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
    //����Զ2002.05.18�޸Ľ���
    else
    //��״̬�ļ�������״̬�ļ�
    //����״̬�ļ�������״̬�ļ�,ʹ��״̬�ļ�ͷ�ͱ�״̬�ļ�һ��
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
��������    ����״̬�ļ������Ϣ����صĳ�Ա�������г�ʼ��
�������    ��
�������    ��
����ֵ      �����Ƿ�ɹ�
                TRUE----�����ɹ�
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveChannel::InitByStatusFile()
{
    BOOL bSucc = m_InfoMain.ReadHead(m_pHead);
    if (!bSucc)
    {
        return FALSE;
    }
    //����Զ2002.05.18�޸ģ����ⵥ��D13767
    m_uCurCsn = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn;
    m_uPID = ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
    //����Զ2002.05.18�޸Ľ���

    int nResult = m_InfoMain.ReadLastUnit(m_pLastInfoUnit);
    if ((ERR_FAIL_READ == nResult) || (ERR_INDEX_INVALID == nResult))
    {
        TraceModuleErr(MTS_BACKSAVE, nResult);
        return FALSE;
    }
    else if (ERR_SUCCESS == nResult)
    //��ȡ״̬�ļ��ɹ��������״̬�ļ����ݳ�ʼ����س�Ա����
    {
        //�õ����һ���ļ���ȫ·�������ݴ˳�ʼ��m_pBillFile
        //����Զ2002.04.13�޸ģ����ⵥ��D12665
/*      m_szFileName = m_szPrefix + GetFileCsnStr(m_uCurCsn)
                       + "." + m_szPostfix;*/
        //��˫����״̬�ļ�����ͬ��������£�m_pHead�Լ�m_pLastInfoUnit
        //�е��ļ����кſ��ܲ�һ�£�������m_pLastInfoUnit�е�Ϊ׼��
        //�ϴ�����ʱ����������ļ�
        m_szFileName = m_szPrefix
            + GetFileCsnStr(((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFileCsn)
            + "." + m_szPostfix;
        //����Զ2002.04.13�޸Ľ���
        STRING szFileFullPath = m_szFilePath + "/" + m_szFileName;
        m_pBillFile = fopen(szFileFullPath.c_str(), "rb+");

        //��ʼ��m_tmCreateTime
        if (NULL != m_pBillFile)
        {
            //by yangshan 2004-6-23  SWPD06194
            fseek(m_pBillFile,((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos,SEEK_SET);
            //the end 2004-6-23  SWPD06194
            
            /*modify by chenliangwei, 2004-06-02, Unix�µ�����ʱ�䲻�����ļ���st_ctime������
              ��˸��ɴ�״̬�ļ���uReserved�ֶλ�ȡ*/
            time_t tFileCreateTime = ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uReserved;

            //״̬�ļ��е�ʱ����Ч�Ļ������ļ������ж�ȡ
            if(tFileCreateTime == 0)
            {
                ACE_stat FileStat;
                ACE_OS::stat(szFileFullPath.c_str(), &FileStat);
                tFileCreateTime = FileStat.st_ctime;
            }
            
            tm* ptmCreateTime = localtime(&tFileCreateTime);
            m_tmCreateTime = *ptmCreateTime;

            //Ϊ�˷�ֹ���һ������Ŀ¼��ɾ�������Գ�ʼ��ʱ����һ�¸�����Ŀ¼
            char szLastDate[20];
            strftime(szLastDate, 20, "%Y%m%d", &m_tmCreateTime);
            m_szFirstPath = m_szDatePath + "/" + szLastDate;
            ACE_OS::mkdir(m_szFirstPath.c_str());
        }
    }

    return TRUE;
}

/**********************************************************************
��������    �����µĻ����ļ��Լ���ص�״̬�ļ�ˢ�²���
�������    ��
�������    ��
����ֵ      �������ļ��Ƿ�ɹ���
                TRUE----�����ɹ�
                FALSE----����ʧ��
�׳��쳣    ��
��ע        ������У�һ�������ļ���Ӧ״̬�ļ��е�һ��״̬��¼������
            �ڴ����µĻ����ļ��󣬱��봴��һ���µ�״̬��¼����Ӳ����
            д״̬ͷ�͸�״̬��¼��������Flush����
**********************************************************************/
BOOL CSaveChannel::CreateNewFile()
{
    BOOL bSucc = CSaveTransaction::CreateNewFile();
    if (!bSucc)
    {
        return FALSE;
    }

    //ˢ��״̬�ļ�

    //����Զ2002.05.18�޸ģ����ⵥ��D13767
    //ˢ��״̬��¼����д��״̬��¼
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

    //ˢ��״̬ͷ����д״̬ͷ
    m_InfoMain.IncreaseIndex(m_pHead);

    //����һ������״̬�ļ�ͷ���е��ļ�CSNʼ�ն���Ϊ��ʽ�ļ���CSN
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
    //����Զ2002.05.18�޸Ľ���

    m_uFileExistTime = 0;

    return TRUE;
}

//�����յ��İ���Ϣ�ж��Ƿ�Ϊ�հ�
BOOL CSaveChannel::IsEmptyPacket(BYTE* pInfo)
{
    SBackPacketInfo* pBackPacketInfo = (SBackPacketInfo* )pInfo;

    return (0 == pBackPacketInfo->uLen);
}

/****************************************************************************
����ԭ��    public: virtual void DelMoreFiles()
��������    ɾ�������ļ���������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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

    //ͨ��0����ɾ��
    if(0 == strcmp(m_szChannelName.c_str(), CHANNEL0))
    {
        return;
    }
    //��*.*��Ϊ������sChlNameָ���Ŀ¼�г���һ���ļ���
    strcpy(szSrcFilePath, m_szDatePath.c_str());
    pChnDir = ACE_OS::opendir(szSrcFilePath);
    if(pChnDir == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_OPEN_DIR_FAILED;
        sTmpInfo += szSrcFilePath;
        CWorkModule::MessageOut(MTS_BACKSAVE, MSGOUT_LEVEL_IMPORTANT, sTmpInfo.c_str());
        //by ldf 2004-01-28 ��core�Ķ�λ���ִ˴���������
        sTmpInfo =szSrcFilePath;
        sTmpInfo += ',';
        //end
        sTmpInfo += szTmpBuf;
        TRACE(MTS_BACKSAVE, sTmpInfo.c_str());
        //Added by ZhengYuqun 2003-09-08 SWPD01517
        //by ldf 2004-01-28 ��core�Ķ�λ���ִ˴���������
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
            //������һ��dir;
            continue;
        }

        int nDateNum = ACE_OS::atoi(pFileDirent->d_name);
        tm tTmp;
        memset(&tTmp,0,sizeof(tm));
        tTmp.tm_year = (nDateNum / 10000) - 1900;
        tTmp.tm_mon = (nDateNum % 10000)/100 - 1;
        tTmp.tm_mday = (nDateNum % 10000)%100;
        DirTime = mktime(&tTmp); //�õ�Ŀ¼ʱ��Ļ�׼ֵ
        //ת����ǰ�ļ�����
        time_t DirTime2 = sDirStat.st_mtime;
        tTmp = *localtime(&DirTime2);
        tTmp.tm_hour = 0;
        tTmp.tm_min = 0;
        tTmp.tm_sec = 0;
        DirTime2 = mktime(&tTmp);
        //by ldf 2004-01-29 �����traceû��ʲô���壬��Դ�ļ�Ҳδ���壬ȥ��֮
        //if(DirTime2 != DirTime)
        //{
        //  TRACE(MTS_BACKSAVE,S_BACKUP_TIMEERROR,DirTime,DirTime2);
        //}
        //ȡ��ǰʱ��
        time(&curTime);
        tTmp = *localtime(&curTime);
        tTmp.tm_hour = 0;
        tTmp.tm_min = 0;
        tTmp.tm_sec = 0;
        curTime = mktime(&tTmp); //�õ���ǰ�����ڣ�ʱ���붼Ϊ0
        //�޸Ľ���
        if(difftime(curTime,DirTime) <= m_uBillSavedDays * 24 * 3600)
        {
            //������һ���ļ�;
            continue;
        }

        nRet = PrepareFiles(szSrcFilePath, 0);
        if (nRet != ERR_SUCCESS)
        {
            break;
        }
        if(0 == m_FileList.size())
        {
            //������һ���ļ�;
            SweepDirectory(szSrcFilePath);
            continue;
        }


        //ɾ��ָ��Ŀ¼�µĻ����ļ�
        DelOneDirBillFiles(szSrcFilePath);

        //ɾ�������ļ��б������ڲ��ṹ��ָ��;
        LISTFILELISTNODE::iterator iter = m_FileList.begin();
        while(iter != m_FileList.end())
        {
            delete (*iter);
            iter ++;
        }
        m_FileList.clear();
        //������һ���ļ���
    }

    //ɾ�������ļ��б����ݣ������ڲ��ṹ��ָ��;
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
//���ҵڶ��ݻ���Ŀ¼�µĵ�һ���ļ�
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
            // ����Ŀ¼�µĵ�һ���ļ�
            while((pDirent = ACE_OS_Dirent::readdir(pSecondDIR)) != NULL)
            {
                // ����Ŀ¼
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

//��ʽ��ǰ׺�ַ���
STRING CSaveChannel::FormatCfgStr(const char* szFormat, 
                                  const tm&   tmTime, 
                                  const UINT4 uChannelID,
                                  const INT4  nRecordCnt)
{
    //�滻ǰ׺�е������ַ�
    //by ldf 2004-02-17 ������
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

    //�ļ��������Ӷ��ļ���¼���Ĵ���������Ϊ"%04N"�ķ�ʽ
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
            //�ҳ���ʽ�ַ�������'N'�滻Ϊ'd'
            char szFmtStr[20] = "";
            memcpy(szFmtStr, pDiv, i + 1);
            szFmtStr[i] = 'd';
            szFmtStr[i + 1] = '\0';

            //��ʽ����¼���ַ�
            char szRecordCnt[20] = "";
            SNPRINTF(szRecordCnt, sizeof(szRecordCnt), 
                     szFmtStr, nRecordCnt);
            szRecordCnt[sizeof(szRecordCnt) - 1] = '\0';

            //��'d'�滻Ϊ'N'�����ü�¼���ַ����滻�ļ���
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

    //���������־ǰ׺
    STRING szLogPrefix = FormatCfgStr(m_szCheckLogPrefix, 
                                      m_tmCreateTime, 
                                      m_uChannelID);

    //��������ļ�·��
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

    //û���ظ�����־����д��
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

//by	����	2004-6-1	���ⵥSWPD05564
BOOL CSaveChannel::CloseCurFile()
{
	//�����Ҫ���л�������ת��
	if (m_pPatchEncap->GetbAviable() && m_uChannelID > 0 && NULL != m_pBillFile)
	{
		STRING szSrcFileFullPath = m_szFilePath + "/" + m_szFileName;		
		
		//�ضϵ�ǰ�Ļ����ļ�
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
//the end	���ⵥSWPD05564
