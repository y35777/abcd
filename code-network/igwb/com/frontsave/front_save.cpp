#include "front_save.h"
#include "../include/toolbox.h"
#include "time.h"
#include "../include/msg_def.h"
#include "../include/frame.h"
#include "resource.h"

CFrontSave::CFrontSave() : CSaveTransaction()
{
}

CFrontSave::~CFrontSave()
{
}

/**********************************************************************
��������    �����ʼ��ʱ�Ĺ���
�������    uAPID----������
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ� 
                TRUE----��ʼ���ɹ� 
                FALSE----��ʼ��ʧ��
�׳��쳣    ��
��ע        Ϊ���ܹ���ʵ�ʿ����е����ù���ͬʱ�ﵽ����Ե�Ҫ��ǰ����
            �в��ö༶���õķ�ʽ�����ò��������ȼ��Ӹߵ����ܹ���Ϊ
            ����/�����2����������߼���Ĳ���û�����ã���ȡ�ͼ�������ò���
**********************************************************************/
BOOL CFrontSave::Init(UINT4 uAPID)
{
	strcpy(m_szModuNameEx, MTS_FRONTSAVE);	
    strcpy(m_szModuName, MTS_FRONTSAVE);
    m_uAPID = uAPID;
    char szAPSection[25];
    SNPRINTF(szAPSection, 25, "%s%d", CFG_SEC_AP, m_uAPID);
    szAPSection[24] = 0;
    m_uHeadLen = sizeof(FS_STATUS_FILE_HEAD);
    m_uUnitLen = sizeof(FS_STATUS_FILE_ELEMENT);
    m_pHead = new BYTE[m_uHeadLen];
    memset(m_pHead, 0, m_uHeadLen);
    m_pLastInfoUnit = new BYTE[m_uUnitLen];
    memset(m_pLastInfoUnit, 0, m_uUnitLen);
    CINIFile cfgIniFile(GetCfgFilePath());
    BOOL bSucc = cfgIniFile.Open();
    if (!bSucc)
    {
        TRACE(MTS_FRONTSAVE, S_FS_TRC_FAIL_OPEN_INI);
        return FALSE;
    }

    //�������ļ��ж�ȡԭʼ�����ļ����ȣ����ò����Ķ�ȡԭ�������ͷ�еı�ע
    m_uFileLen = cfgIniFile.GetInt(CFG_SEC_DISK_FILE, CFG_ORIG_FILE_MAX_SIZE, 
                                   CFG_ORIG_FILE_MAX_SIZE_DEFAULT);
    m_uFileLen = cfgIniFile.GetInt(szAPSection, CFG_ORIG_FILE_MAX_SIZE, 
                                   m_uFileLen);
	//added by maimaoshi,2002-10-29
    /*
	//m_uFileLen�������3M
    if ((m_uFileLen < 3 * 1024) || (m_uFileLen > 20 * 1024))
    {
        m_uFileLen = CFG_ORIG_FILE_MAX_SIZE_DEFAULT;
        TRACE(MTS_FRONTSAVE, S_FS_TRC_ERR_CFG_FILE_SIZE, m_uFileLen);
    }
	*/
	
	if (m_uFileLen < 3 * 1024)
    {
        m_uFileLen = 3 * 1024;
		//������ֵС����Сֵ3M,����Ϊ��Сֵ3M
        TRACE(MTS_FRONTSAVE, S_FS_TRC_ERR_CFG_FILE_MINSIZE, m_uFileLen * 1024);
    }else if  (m_uFileLen > 20 * 1024)
	{
		m_uFileLen = 20 * 1024;
		//������ֵ�������ֵ20M,����Ϊ���ֵ20M
        TRACE(MTS_FRONTSAVE, S_FS_TRC_ERR_CFG_FILE_MAXSIZE, m_uFileLen * 1024);
	}
	//end
    m_uFileLen *= 1024;     //���д�K���ֽڵ�ת��

    //ԭʼ�������ļ����̶�����������
    strcpy(m_szPrefix, "b");
    m_nCsnLen = 10;
    m_uFileCsnStart = 1;
    m_uFileCsnEnd = 999999999;  //10��9�Ѿ�����UINT4�ķ�Χ������ֻ��9��9
    m_uFileCsnStep = 1;
    strcpy(m_szPostfix, "bil");
    m_uMaxFlushSpan = 1;    //ǰ���̱���ÿ����һ��������Flush����

    //�õ�������ԭʼ������ŵĸ�Ŀ¼
    char szOrigRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_FRONT_SAVE_ROOT_DIR, 
                         CFG_FRONT_SAVE_ROOT_DIR_DEFAULT, 
                         szOrigRootDir, MAX_PATH);
    CreateDeepDir(szOrigRootDir);

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
    cfgIniFile.GetString(szAPSection, CFG_AP_NAME, szAPSection, 
                         szAPName, 255);
    m_szDatePath = STRING(szOrigRootDir) + "/" + szAPName;

	
	//Added by maimaoshi at 2002-09-23
	SNPRINTF(m_szSrcBasePath,MAX_PATH,"%s",
        MakePath(STRING(szOrigRootDir), szAPName).c_str());
    m_szSrcBasePath[MAX_PATH - 1] = 0;

	//�������ļ��е�DiskFile�У���ȡ�����ļ��ı�����������ʼ����Ա����
    //m_uBillSavedDays;
    m_uBillSavedDays = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,             //DiskFile
                                      CFG_SAVE_DEADLINE_DEL,         //DeadLineOfAutoDel
                                      CFG_SAVE_DEADLINE_DEL_DEFAULT);
    m_uBillSavedDays = cfgIniFile.GetInt(szAPSection,             //AccessPoint%d
                                      CFG_SAVE_DEADLINE_DEL,         //DeadLineOfAutoDel
                                      m_uBillSavedDays);                                      

    //�����ļ��ı�������������Ҫ7��,�����45��
    if(m_uBillSavedDays < 7)
    {
        m_uBillSavedDays = 7;
		//��ʾ���õ�ֵС����Сֵ7������Ϊ��Сֵ7
		TRACE(m_szModuName, S_FS_TRC_ERR_CFG_AUTODEL_MINDAY, m_uBillSavedDays);
    }else if (m_uBillSavedDays > 180) //Modified by ZhengYuqun SWPD04451 2004-03-27
	{
		m_uBillSavedDays = 180;
		//��ʾ���õ�ֵ�������ֵ90������Ϊ���ֵ90
		TRACE(m_szModuName, S_FS_TRC_ERR_CFG_AUTODEL_MAXDAY, m_uBillSavedDays);
	}
	//end

    char szMainStatusPath[MAX_PATH], szBakStatusPath[MAX_PATH];
    ACE_OS::mkdir(m_szDatePath.c_str());
    SNPRINTF(szMainStatusPath, MAX_PATH, "%s/%s", 
             szMainStatusRootDir, szAPName);
    szMainStatusPath[MAX_PATH - 1] = 0;
    ACE_OS::mkdir(szMainStatusPath);

    SNPRINTF(szBakStatusPath, MAX_PATH, "%s/%s", 
             szBakStatusRootDir, szAPName);
    szBakStatusPath[MAX_PATH - 1] = 0;
    ACE_OS::mkdir(szBakStatusPath);

    //��������״̬�ļ�Ŀ¼
    SNPRINTF(szMainStatusPath, MAX_PATH, "%s/%s/%s", 
             szMainStatusRootDir, szAPName, "save");
    szMainStatusPath[MAX_PATH - 1] = 0;
    CreateDeepDir(szMainStatusPath);
    SNPRINTF(szBakStatusPath, MAX_PATH, "%s/%s/%s", 
             szBakStatusRootDir, szAPName, "save");
    szBakStatusPath[MAX_PATH - 1] = 0;
    CreateDeepDir(szBakStatusPath);

    //��������״̬�ļ�����ز���
    char szMainStatusName[MAX_PATH], szBakStatusName[MAX_PATH];
    SNPRINTF(szMainStatusName, MAX_PATH, "%s_FS_MAIN.stf", szAPName);
    szMainStatusName[MAX_PATH - 1] = 0;

    SNPRINTF(szBakStatusName, MAX_PATH, "%s_FS_BAK.stf", szAPName);
    szBakStatusName[MAX_PATH - 1] = 0;
    char szBackupPath[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_BACKUP_STATUS_SRC_DIR,
                         CFG_BACKUP_STATUS_SRC_DIR_DEFAULT,
                         szBackupPath, MAX_PATH);
    szBackupPath[MAX_PATH - 1] = 0;
    CreateDeepDir(szBackupPath);
    m_InfoMain.SetInfoFilePath(szMainStatusName, szMainStatusPath);
    m_InfoMain.SetBackupFilePath(szMainStatusName, szBackupPath);
    m_InfoMain.SetFileStruct(sizeof(FS_STATUS_FILE_HEAD),
                             sizeof(FS_STATUS_FILE_ELEMENT),
                             MAX_UNIT_NUM);
    m_InfoBak.SetInfoFilePath(szBakStatusName, szBakStatusPath);
    m_InfoBak.SetBackupFilePath(szBakStatusName, szBackupPath);
    m_InfoBak.SetFileStruct(sizeof(FS_STATUS_FILE_HEAD),
                            sizeof(FS_STATUS_FILE_ELEMENT),
                            MAX_UNIT_NUM);
    //��״̬ͷ�������������ȱʡֵ�����״̬�ļ������ڣ���ȱʡֵд��״̬�ļ�
    FS_STATUS_FILE_HEAD* pHead = (FS_STATUS_FILE_HEAD* )m_pHead;

	//����Զ2002.05.18�޸ģ����ⵥ��D13767
    pHead->uFileCsn = m_uFileCsnStart - m_uFileCsnStep;
	//����Զ2002.05.18�޸Ľ���

    pHead->uLastPID = 0;
    pHead->uStatusFileWritePos = m_uHeadLen;
    m_InfoMain.CalcCheckSum(m_pHead);

    bSucc = CSaveTransaction::Init();
    if (!bSucc)
    {
        return FALSE;
    }

    return TRUE;
}


/**********************************************************************
��������    ��ȡָ���Ļ�������Χ�еĵ�һ���ܹ������Ļ���
�������    uFirstPID----��Ҫ��ȡ�Ļ�������Χ�е���ʼ���� 
            uLastPID----��Ҫ��ȡ�Ļ�������Χ�еĽ�������
�������    uPID----��������Ӧ��ID�� 
            pPacket----����������ָ�룬����޷���ȡ�����������򷵻�ΪNULL��
                �ں����ڲ�����ռ䣬�ں������ͷ� 
            uPacketLen----���������ݳ��� 
            szFileFullPath----���������ڵĻ����ļ���ȫ·��  
            uStartPos----���������ļ��е���ʼλ��
����ֵ  ����ֵ˵�����£� 
            0----��ȡ�ɹ� 
            1----��ȡʧ�� 
            <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ�� 
                ERR_PID_TOO_LARGE----��ȡ״̬��¼ʱҪ��İ���̫�� 
                ERR_READ_STATUS_FAIL----��״̬�ļ�ʧ�� 
                ERR_READ_BILLFILE_FAIL----�������ļ�ʧ�� 
                ERR_PIDS_NOT_EXIST----Ҫ��İ��ŷ�Χ��״̬�ļ��ж�������
�׳��쳣    ��
**********************************************************************/
int CFrontSave::GetFirstBillPacket(const UINT4 uFirstPID, const UINT4 uLastPID,
                             UINT4& uPID, BYTE*& pPacket, UINT4&  uPacketLen, 
                             STRING& szFileFullPath, UINT4& uStartPos)
{
    FS_STATUS_FILE_ELEMENT CurInfoUnit;
    pPacket = NULL;
    uPacketLen = 0;
    m_uFirstPID = uFirstPID;
    m_uLastPID = uLastPID;
    m_bReadComplete = FALSE;

    if(uFirstPID > uLastPID)
    {
        return ERR_ALREADY_FINISH;
    }
    ASSERT(uLastPID <= ((FS_STATUS_FILE_HEAD* )m_pHead)->uLastPID);

    //��ȡ��һ��״̬��¼�����ݴ˶Ը����������и�ֵ
    int nResult = GetInfoUnit(uFirstPID, &CurInfoUnit, m_nCurIndex);
    if ((ERR_READ_STATUS_FAIL == nResult) || (ERR_PID_TOO_LARGE == nResult))
    {
        return nResult;
    }

    //���ݷ��ص�״ֵ̬������ȷ�Ե��ж��Լ��Ƿ�������ж�
    if (CurInfoUnit.uPID > uLastPID)
    {
        m_bReadComplete = TRUE;
        return ERR_PIDS_NOT_EXIST;
    }
    else if (CurInfoUnit.uPID == uLastPID)
    {
        m_bReadComplete = TRUE;
    }

    //���ݷ��ص�״̬��¼�Ը�������������и�ֵ
    uPID = CurInfoUnit.uPID;
    uPacketLen = CurInfoUnit.uPacketLen;

    //���Ϊ�հ�����ֱ�ӷ��أ��������״̬��¼��ȡ��Ӧ�Ļ�������
    if (0 != uPacketLen)
    {
        pPacket = new BYTE[uPacketLen];
        szFileFullPath = GetFileFullPath(&CurInfoUnit);
        uStartPos = CurInfoUnit.uStartPos;

        //�ӻ����ļ��ж�ȡ����������
        BOOL bSucc = ReadPacket(szFileFullPath.c_str(), pPacket, 
                                   uPacketLen, uStartPos);
        if (!bSucc)
        {
            delete[] pPacket;
            pPacket = NULL;
            return ERR_READ_BILLFILE_FAIL;
        }
    }

    return ERR_SUCCESS;
}

/**********************************************************************
��������    ��ȡָ���Ļ�������Χ�е���һ���ܹ������Ļ���
�������    uLastPID--��һ�ΰ���
�������    uPID----��������Ӧ��ID�� 
            pPacket----����������ָ�룬����޷���ȡ�����������Ѿ�ȡ������
                Ҫ��Χ�ڵ����л��������򷵻�ΪNULL���ں����ڲ�����ռ䣬
                �ں������ͷ� 
            uPacketLen----���������ݳ��� 
            szFileFullPath----���������ڵĻ����ļ���ȫ·��  
            uStartPos----���������ļ��е���ʼλ��
����ֵ      ����ֵ˵�����£� 
                0----��ȡ�ɹ� 
                1----��ȡʧ�� 
                <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ�� 
                    ERR_READ_STATUS_FAIL----��״̬�ļ�ʧ�� 
                    ERR_READ_BILLFILE_FAIL----�������ļ�ʧ��
                    ERR_ALREADY_FINISH----���ŷ�Χ�ڵĻ��������Ѿ���ȡ��
�׳��쳣    ��
**********************************************************************/
int CFrontSave::GetNextBillPacket(const UINT4 uLastPID,
                                  UINT4& uPID,
                                  BYTE*& pPacket, 
                                  UINT4&  uPacketLen, 
                                  STRING& szFileFullPath,
                                  UINT4& uStartPos)
{
    FS_STATUS_FILE_ELEMENT InfoUnit;
    pPacket = NULL;
    uPacketLen = 0;
    if (m_bReadComplete)
    {
        return ERR_ALREADY_FINISH;
    }

    //��ȡ��һ��״̬��¼��λ��
    m_nCurIndex += 1;
    if (m_nCurIndex > m_InfoMain.GetUnitNum())
    {
        m_nCurIndex = 1;
    }

    //��ȡ��һ��״̬��¼
    int nResult = m_InfoMain.ReadUnit(m_nCurIndex, (BYTE* )&InfoUnit); 
    if (0 != nResult)
    {
        return ERR_READ_STATUS_FAIL;
    }

    //�������ͻȻ��С������������
    int nCurPID = InfoUnit.uPID;
    if(uLastPID > InfoUnit.uPID)
    {
        nCurPID    = uLastPID + 1;
        uPID       = nCurPID;
        //ֻ�ܽ�������޸�Ϊ�հ�
        uPacketLen = 0;
        TRACE(MTS_FRONTSAVE, S_FS_TRC_MAKE_EMPTY_PACKAGE, nCurPID);
    }
    else
    {
        //���ݶ�������̬��¼�Ը��������и�ֵ
        uPID       = InfoUnit.uPID;
        uPacketLen = InfoUnit.uPacketLen;
    }

    //�ж��Ƿ���ŷ�Χ�ڵ����л��������Ѿ�����������
    if ((nCurPID >= m_uLastPID) 
        || (m_nCurIndex == m_InfoMain.GetWriteIndex()))
    {
        m_bReadComplete = TRUE;
    }
    if (nCurPID > m_uLastPID)
    {
        return ERR_ALREADY_FINISH;
    }


    //���Ϊ�հ�����ֱ�ӷ��أ��������״̬��¼��ȡ��Ӧ�Ļ�������
    if (0 != uPacketLen)
    {
        szFileFullPath = GetFileFullPath(&InfoUnit);
        uStartPos = InfoUnit.uStartPos;
        pPacket = new BYTE[uPacketLen];

        //�ӻ����ļ��ж�ȡ����������
        BOOL bSucc = ReadPacket(szFileFullPath.c_str(), pPacket, 
                           uPacketLen, uStartPos);
        if (!bSucc)
        {
            delete[] pPacket;
            pPacket = NULL;
            return ERR_READ_BILLFILE_FAIL;
        }
    }

    return ERR_SUCCESS;
}

//����״̬�ļ��õ����һ���������İ���
UINT4 CFrontSave::GetLastPID()
{
    return ((FS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
}

/**********************************************************************
��������    �õ���ǰ����������ز�������Щ���������򻺳������ͻ������������Ϣ
�������    szFileFullPath----��ǰ���������ڵĻ����ļ�ȫ·�� 
            uStartPos----��ǰ�������ڻ����ļ��е�ƫ��ֵ
**********************************************************************/
void CFrontSave::GetCurPacketParam(STRING& szFileFullPath, 
                                          UINT4& uStartPos)
{
    FS_STATUS_FILE_ELEMENT* pCurInfoUnit
                = (FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit;
    szFileFullPath = GetFileFullPath(pCurInfoUnit);
    uStartPos = pCurInfoUnit->uStartPos;
}

/**********************************************************************
��������    �õ���һ�������ļ����Ŀ¼�����Ŀ¼�����ڣ��򴴽�֮
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CFrontSave::GetNextFilePath()
{
    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCurTime = localtime(&tCurrent);

    //�����ǰ�������ϸ��ļ������ڲ�һ��������봴���µ�����Ŀ¼
    if ((ptmCurTime->tm_year != m_tmCreateTime.tm_year)
            || (ptmCurTime->tm_mon != m_tmCreateTime.tm_mon)
            || (ptmCurTime->tm_mday != m_tmCreateTime.tm_mday))
    {
        char szToday[20];
        strftime(szToday, 20, "%Y%m%d", ptmCurTime);
        m_szFilePath = m_szDatePath + "/" + szToday;

		//by yangshan 2004-04-20 ��Ӧ���ⵥ SWPD04771
		ACE_DIR *pDir = NULL;
		pDir = ACE_OS::opendir(m_szFilePath.c_str());
		if(pDir == NULL)
		{
			ACE_OS::mkdir(m_szFilePath.c_str());
			char szLogInfo[255];
			SNPRINTF(szLogInfo, 255, S_FS_LOG_CREATE_FS_DATE, szToday);
			szLogInfo[254] = 0;
			CMsgProcessor::WriteLog(MTS_FRONTSAVE, szLogInfo);
			MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_IMPORTANT, szLogInfo);
		}
		else
		{
			ACE_OS_Dirent::closedir(pDir);	
		}
		//end
		
		//added by maimaoshi at 2002-09-23
		//ɾ��������ļ�
		DelMoreFiles();
		//end
    }
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
BOOL CFrontSave::DeleteTempFile()
{
    //��ȡ��״̬�ļ������һ��״̬��¼
    FS_STATUS_FILE_ELEMENT LastUnit;
    int nResult = m_InfoMain.ReadLastUnit((BYTE* )&LastUnit); 
    if ((ERR_INDEX_INVALID == nResult) || (ERR_FAIL_READ == nResult))
    {
        return FALSE;
    }
    else if (ERR_NO_UNIT_EXIST == nResult)
    {
        return TRUE;
    }

    //�������һ��״̬��¼�õ���ʱ�ļ���ȫ·��������ɾ��
    STRING szTempFileFullPath = GetFileFullPath(&LastUnit);
    BOOL bSucc = FileDel(szTempFileFullPath.c_str());
    if (bSucc)
    {
        TRACE(MTS_FRONTSAVE, S_FS_TRC_SUCC_DEL_FILE, LastUnit.uFileCsn);
        return TRUE;
    }
    else
    {
        TRACE(MTS_FRONTSAVE, S_FS_TRC_FAIL_DEL_FILE, LastUnit.uFileCsn);
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
void CFrontSave::RefreshStatus(BYTE* pPacketInfo)
{
    //�Դ������Ϣ��������ת��
    SFrontPacketInfo* pFrontPacketInfo = (SFrontPacketInfo* )pPacketInfo; 
    m_uPID = pFrontPacketInfo->uPID;

    FS_STATUS_FILE_HEAD* pHead = (FS_STATUS_FILE_HEAD* )m_pHead;
    FS_STATUS_FILE_ELEMENT* pCurInfoUnit 
                = (FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit;

    //���ݴ������Ϣ���µ�ǰ״̬��¼
    pCurInfoUnit->uPID = pFrontPacketInfo->uPID;
    pCurInfoUnit->uFileCsn = m_uCurCsn;
    pCurInfoUnit->uStartPos = pHead->uFileWritePos;
    pCurInfoUnit->uPacketLen = pFrontPacketInfo->uPacketLen;
    pCurInfoUnit->nYear = m_tmCreateTime.tm_year;
    pCurInfoUnit->yMonth = m_tmCreateTime.tm_mon;
    pCurInfoUnit->yDay = m_tmCreateTime.tm_mday;

    //���ݴ������Ϣ�����ļ�ͷ
    m_InfoMain.IncreaseIndex(m_pHead);

    pHead->uLastPID = pFrontPacketInfo->uPID;
    pHead->uFileCsn = m_uCurCsn;
    pHead->uFileWritePos += pFrontPacketInfo->uPacketLen;
    m_InfoMain.CalcCheckSum(m_pHead);

}

/**********************************************************************
��������    �����µĻ����ļ��Լ���ص�״̬�ļ�ˢ�²���
�������    ��
�������    ��
����ֵ      �������ļ��Ƿ�ɹ��� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CFrontSave::CreateNewFile()
{
    BOOL bSucc = CSaveTransaction::CreateNewFile();
    if (!bSucc)
    {
        return FALSE;
    }

    //ˢ��״̬�ļ�
    ((FS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn = m_uCurCsn;
    ((FS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos = 0;
    m_InfoMain.CalcCheckSum(m_pHead);

    //�����º���ļ�ͷд����״̬�ļ�
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

	//by ����    2004-4-24    ��Ӧ���ⵥSWPD04770
     // //�����º���ļ�ͷд�뱸״̬�ļ�
     //    bSucc = m_InfoBak.WriteHead(m_pHead, m_uHeadLen);
     //    if (!bSucc)
     //    {
     //		return FALSE;
     //    }
     //	bSucc = m_InfoBak.Flush();
     //	if (!bSucc)
     //    {
     //        return FALSE;
     //     }
     
    //The end  
        



    //TRACE(MTS_FRONTSAVE, S_FS_TRC_CREATE_NEW_FILE, m_szFileName.c_str());
    MSGOUT(MTS_FRONTSAVE, MSGOUT_LEVEL_IMPORTANT, 
        S_FS_TRC_CREATE_NEW_FILE, m_szFileName.c_str());

    { //Added by zhouyonghui.
        SLog log;
        
        log.LogType = SLog::LOG_SYS;
        log.OperResult = SLog::OPER_SUCCESS;
        log.uIPAddr = 0;
        
        strncpy(log.szUserName,MTS_FRONTSAVE,48);
        log.szUserName[47] = 0;
        
        strcpy(log.szOperCmd,"-");
        
        SNPRINTF(log.szCmdDesc,LOG_CMD_DESC_LEN,S_FS_TRC_CREATE_NEW_FILE,
                m_szFileName.c_str());
        log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;
        
        CMsgProcessor::WriteLog(&log);
    }

    return TRUE;
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
BOOL CFrontSave::DoWhenFileClose()
{
    ASSERT(NULL == m_pBillFile);

    //�ضϵ�ǰ�Ļ����ļ�
    UINT4 uWriteFilePos = ((FS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos;
    STRING szFileFullPath = m_szFilePath + "/" + m_szFileName;
    int nResult = ACE_OS::truncate(szFileFullPath.c_str(), uWriteFilePos);

    return (0 == nResult);
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
��ע        ǰ���̵�״̬�ļ��У�һ��״̬��¼��Ӧһ����������
            ����ÿ����һ��ʱ���������״̬ͷ��״̬��¼��������Flush����
**********************************************************************/
BOOL CFrontSave::WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush)
{
    ASSERT(bFlush);

    //д��ǰ״̬��¼
    BOOL bSucc = pInfoFile->WriteUnit(m_pLastInfoUnit, m_uUnitLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //д״̬ͷ
    bSucc = pInfoFile->WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //����Flush����
    bSucc = pInfoFile->Flush();
    if (!bSucc)
    {
        return FALSE;
    }

    return TRUE;
}

/**********************************************************************
��������    �ڱ��滰���������е�д�����ļ�����
�������    pPacketInfo----�������������Ϣ��������������������Խ�
                �ͣ��ں����⸺��������ͷ�
            bFlush----�Ƿ����Flush����
�������    ��
����ֵ      ����ֵ˵�����£�
            0----��ȡ�ɹ�
            1----��ȡʧ��
            <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ��
                ERR_WRITE_BILLFILE_FAIL----д�����ļ�ʧ��
�׳��쳣    ��
**********************************************************************/
int CFrontSave::WriteBillFile(BYTE* pPacketInfo, BOOL bFlush)
{
    BOOL bSucc;
    //�Դ������Ϣ��������ת��
    SFrontPacketInfo* pFrontPacketInfo = (SFrontPacketInfo* )pPacketInfo; 

    //ֻ���ڻ������ǿյ�����£�����ʵ�ʵ�д�̲�����
    //�������κβ���
    if (pFrontPacketInfo->uPacketLen > 0)
    {
        //д�����ļ�
        bSucc = WritePacketData(pFrontPacketInfo->pPacket, 
                              pFrontPacketInfo->uPacketLen);
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
BOOL CFrontSave::DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead, 
								   BYTE* pMainLastUnit, BYTE* pBakLastUnit)
{
    BOOL bSucc;

	//����Զ2002.05.18�޸ģ����ⵥ��D13767
    //������״̬�ļ����ļ����кŲ���ʱ��ɾ�����һ�������ļ�
	//����״̬ͷ�е��ļ����кſ��ܻᱻ˫��ģ���޸ģ�
	//���Ա��������һ��״̬��¼�е��ļ����к���Ϊ�жϱ�׼
    if (((FS_STATUS_FILE_ELEMENT* )pMainLastUnit)->uFileCsn 
             > ((FS_STATUS_FILE_ELEMENT* )pBakLastUnit)->uFileCsn)
    {
        TRACE(MTS_FRONTSAVE, S_FS_TRC_DEL_LAST_FILE,
              ((FS_STATUS_FILE_ELEMENT* )pMainLastUnit)->uFileCsn);
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
��������    ����״̬�ļ������Ϣ����صĳ�Ա�������г�ʼ����
�������    ��
�������    ��
����ֵ      �����Ƿ�ɹ�
                TRUE----�ɹ�
                FALSE----ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CFrontSave::InitByStatusFile()
{
    //��ȡ״̬�ļ�ͷ�����һ��״̬��¼
    BOOL bSucc = m_InfoMain.ReadHead(m_pHead);
    if (!bSucc)
    {
        return FALSE;
    }

	//����Զ2002.05.18�޸ģ����ⵥ��D13767
    m_uCurCsn = ((FS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn;
    m_uPID = ((FS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
	//����Զ2002.05.18�޸Ľ���

    int nResult = m_InfoMain.ReadLastUnit(m_pLastInfoUnit);
    if ((ERR_FAIL_READ == nResult) || (ERR_INDEX_INVALID == nResult))
    {
        TraceModuleErr(MTS_FRONTSAVE, nResult);
        return FALSE;
    }
    else if (ERR_SUCCESS == nResult)
    //��ȡ״̬�ļ��ɹ��������״̬�ļ����ݳ�ʼ����س�Ա����
    {
        //�õ����һ���ļ���ȫ·�������ݴ˳�ʼ��m_pBillFile
        GetFilePathAndName((FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit, 
                           m_szFilePath, m_szFileName);
        STRING szFileFullPath 
            = GetFileFullPath((FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit);
        m_pBillFile = fopen(szFileFullPath.c_str(), "rb+");
    
        //��ʼ��m_tmCreateTime
        FS_STATUS_FILE_ELEMENT* pLastInfoUnit 
            = (FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit;
        m_tmCreateTime.tm_year = pLastInfoUnit->nYear;
        m_tmCreateTime.tm_mon = pLastInfoUnit->yMonth;
        m_tmCreateTime.tm_mday = pLastInfoUnit->yDay;

        //Ϊ�˷�ֹ�ϴ�����ʱ���ɵ����һ������Ŀ¼��ɾ����
        //���Գ�ʼ��ʱ������һ�¸�����Ŀ¼
        if (m_tmCreateTime.tm_year > 10)
        {
            char szLastDate[20];
            strftime(szLastDate, 20, "%Y%m%d", &m_tmCreateTime);
            m_szFilePath = m_szDatePath + "/" + szLastDate;
            ACE_OS::mkdir(m_szFilePath.c_str());
        }
    }

    return TRUE;
}

/**********************************************************************
��������    ����ָ����״̬��¼��û����ļ���ȫ·��
�������    pInfoUnit----״̬��¼
�������    ��
����ֵ      �����ļ���ȫ·��
�׳��쳣    ��
**********************************************************************/
STRING CFrontSave::GetFileFullPath(FS_STATUS_FILE_ELEMENT* pInfoUnit)
{
    STRING szFilePath, szFileName, szFileFullPath;

    GetFilePathAndName(pInfoUnit, szFilePath, szFileName);
    szFileFullPath = szFilePath + "/" + szFileName;

    return szFileFullPath;
}

/**********************************************************************
��������    ����ָ����״̬��¼��û����ļ���·�����ļ���
�������    pInfoUnit----״̬��¼
�������    szFilePath----�����ļ�·��
            szFileName----�����ļ���
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CFrontSave::GetFilePathAndName(FS_STATUS_FILE_ELEMENT* pInfoUnit,
                                    STRING& szFilePath, STRING& szFileName)
{
    //�õ������ļ���
    STRING szFileCsn;
    szFileCsn = GetFileCsnStr(pInfoUnit->uFileCsn);
    szFileName = m_szPrefix + szFileCsn + "." + m_szPostfix;

    //�õ������ļ���·��
    char szDateName[30];
    SNPRINTF(szDateName, 30, "%04d%02d%02d",
              pInfoUnit->nYear + 1900, pInfoUnit->yMonth + 1, pInfoUnit->yDay);
    szDateName[29] = 0;
    szFilePath = m_szDatePath + "/" + szDateName;
}

/**********************************************************************
��������    ���ݻ������ŵõ�״̬�ļ�����Ӧ��״̬��¼������Ҳ�����صļ�
            ¼����õ��������������İ��Ŷ�Ӧ��״̬��¼----��״̬�ļ��д�
            �ڣ���Ϊ���д���uPID�İ�������С�İ���
�������    uPID----��Ҫ��ȡ�Ļ������İ���
�������    pInfoUnit----uPID��������Ӧ��״̬��¼���ռ��ں�����������ͷ� 
            nIndex----pInfoUnit��״̬�ļ��ж�Ӧ�������ţ���1��ʼ��š�
                �����ȡpInfoUnitʧ�ܣ���Ϊ-1
����ֵ      ����ֵ˵�����£� 
                0----��ȡ�ɹ� 
                1----��ȡʧ�� 
                <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ�� 
                    ERR_PID_TOO_LARGE----��ȡ״̬��¼ʱҪ��İ���̫�� 
                    ERR_READ_STATUS_FAIL----��״̬�ļ�ʧ�� 
                    ERR_PID_NOT_FOUND----��ȡ״̬��¼ʱҪ��İ���û���ҵ�
�׳��쳣    ��
**********************************************************************/
int CFrontSave::GetInfoUnit(const UINT4 uPID, 
                     FS_STATUS_FILE_ELEMENT* pInfoUnit, INT4& nIndex)
{
    UINT4 uValidUnitNum;  //״̬�ļ�������������Ч״̬��¼�ĸ���
    int nResult;
    nIndex = -1;
    FS_STATUS_FILE_HEAD* pHead = (FS_STATUS_FILE_HEAD* )m_pHead;

    //������Ч��״̬��¼�ĸ���
    if (pHead->uRotateCount > 0)
    {
        uValidUnitNum = m_InfoMain.GetUnitNum();
    }
    else
    {
        uValidUnitNum = m_InfoMain.GetWriteIndex() - 1;
    }

    //����Ҫ��İ��������һ�����ŵĲ�
    int nPIDSpan = pHead->uLastPID - uPID;

    if ((nPIDSpan < 0))  //uPID��״̬�ļ��е����һ��״̬��¼��Ҫ��
    {
        return ERR_PID_TOO_LARGE;
    }
    //���Ų��״̬�ļ��е���Ч���������������С����Ч��¼��ʼ
    if (nPIDSpan > uValidUnitNum - 1)  
    {
        if (pHead->uRotateCount > 0)
        {
            nIndex = m_InfoMain.GetWriteIndex();
        }
        else
        {
            nIndex = 1;
        }
    }
    else
    {
        //���ݰ��Ų����Ҫ���״̬��¼��״̬�ļ��е�λ��
        nIndex = m_InfoMain.GetWriteIndex() - nPIDSpan - 1;
        if (nIndex <= 0)
        {
            nIndex += m_InfoMain.GetUnitNum();
        }
    }

    //���ݼ������״̬��¼��λ�ö�ȡ״̬��¼��
    //���ڶ�����״̬��¼�еİ��ű�uPIDС��ʱ��˳�����¶�ȡ״̬��¼ֱ��״̬
    //��¼�İ��Ŵ��ڻ����uPIDΪֹ
    nResult = m_InfoMain.ReadUnit(nIndex, (BYTE* )pInfoUnit);
    while ((0 == nResult) && (uPID > pInfoUnit->uPID))
    {
        nIndex = nIndex % m_InfoMain.GetUnitNum() + 1;
        nResult = m_InfoMain.ReadUnit(nIndex, (BYTE* )pInfoUnit);
    }

    if (0 != nResult)
    {
        return ERR_READ_STATUS_FAIL;
    }
    if (uPID != pInfoUnit->uPID)
    {
        return ERR_PID_NOT_FOUND;
    }

    return ERR_SUCCESS;
}

/**********************************************************************
��������    ��ָ�����ļ��ж�ȡһ����������
�������    szFileFullPath----���������ڵĻ����ļ�
            uLen----�������ĳ���
            uStartPos----�������ڻ����ļ��е���ʼλ��
�������    pPacket----�����Ļ��������ݣ��ڴ��ں�����������ͷ�
����ֵ      ��ȡ�������Ƿ�ɹ�
                TRUE----���ɹ�
                FALSE----��ʧ��
�׳��쳣    ��
**********************************************************************/
//��ָ�����ļ��ж�ȡһ����������
BOOL CFrontSave::ReadPacket(const char* szFileFullPath, const BYTE* pPacket,
                            const UINT4 uLen, const UINT4 uStartPos)
{
    //���ļ�
    FILE* pBillFile = fopen(szFileFullPath, "rb");
    if (NULL != pBillFile)
    {
        //�����ļ��Ķ�дָ��
        int nResult = fseek(pBillFile, uStartPos, SEEK_SET);
        if (0 != nResult)
        {
            fclose(pBillFile);
            return FALSE;
        }

        //��ȡ��������
        size_t uLenRead = fread((void* )pPacket, uLen, 1, pBillFile);
        if (1 != uLenRead)
        {
            fclose(pBillFile);
            return FALSE;
        }

        //�رջ����ļ�
        fclose(pBillFile);
        return TRUE;
    }

    return FALSE;
}

//�����յ��İ���Ϣ�ж��Ƿ�Ϊ�հ�
BOOL CFrontSave::IsEmptyPacket(BYTE* pInfo)
{
    SFrontPacketInfo* pFrontPacketInfo = (SFrontPacketInfo* )pInfo;

    return (0 == pFrontPacketInfo->uPacketLen);
}

/*****************************************************************************
����ԭ��    public: virtual void DelMoreFiles()
��������    ɾ�������ļ���������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
******************************************************************************/
void CFrontSave::DelMoreFiles()
{
    //��*.*��Ϊ������m_szSrcBasePathָ���Ŀ¼�г���һ���ļ�
    ACE_DIR *pDir = NULL;
    int nRet;
    dirent *pFileDirent =  NULL;    
    time_t curTime,DirTime;
	ACE_stat sDirStat;
    
    //���ļ�Ŀ¼
    pDir = ACE_OS::opendir(m_szSrcBasePath);
    if(pDir == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_OPEN_DIR_FAILED;
        sTmpInfo += m_szSrcBasePath;
        CWorkModule::MessageOut(MTS_FRONTSAVE, MSGOUT_LEVEL_IMPORTANT, sTmpInfo.c_str());
        //by ldf 2004-01-28 ��core�Ķ�λ�޸�
        sTmpInfo =m_szSrcBasePath;
        sTmpInfo += ',';
        //end
        sTmpInfo += szTmpBuf;
        TRACE(MTS_FRONTSAVE, sTmpInfo.c_str());

        return;
    }

    char szSrcFilePath[MAX_PATH];
    STRING sSrcFileName;
    while((pFileDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        sSrcFileName = MakePath(STRING(m_szSrcBasePath), pFileDirent->d_name);
        ACE_OS::stat(sSrcFileName.c_str(), &sDirStat);
        nRet = IsDigitValue(pFileDirent->d_name);
		if(S_IFDIR == (sDirStat.st_mode & S_IFDIR) && TRUE == nRet)
        {
            SNPRINTF(szSrcFilePath,MAX_PATH,"%s", sSrcFileName.c_str());
            szSrcFilePath[MAX_PATH - 1] = 0;
           
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
			//	TRACE(MTS_FRONTSAVE,S_BACKUP_TIMEERROR,DirTime,DirTime2);
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
                //������һ��dir;
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

            //ɾ�������ļ��б�,�����ڲ��ṹ��ָ��
            LISTFILELISTNODE::iterator iter = m_FileList.begin();
            while(iter != m_FileList.end())
            {
                delete (*iter);
                iter ++;
            }
            m_FileList.clear(); 
            //������һ���ļ�.
        }
    }

    //ɾ�������ļ��б�,�����ڲ��ṹ��ָ��;
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
    ACE_OS::closedir(pDir);
}
