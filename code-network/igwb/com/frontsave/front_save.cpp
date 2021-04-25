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
功能描述    处理初始化时的工作
输入参数    uAPID----接入点号
输出参数    无
返回值      初始化是否成功 
                TRUE----初始化成功 
                FALSE----初始化失败
抛出异常    无
备注        为了能够简化实际开局中的配置工作同时达到灵活性的要求，前存盘
            中采用多级配置的方式。配置参数的优先级从高到低总共分为
            总体/接入点2个级别，如果高级别的参数没有配置，则取低级别的配置参数
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

    //从配置文件中读取原始话单文件长度，配置参数的读取原则见函数头中的备注
    m_uFileLen = cfgIniFile.GetInt(CFG_SEC_DISK_FILE, CFG_ORIG_FILE_MAX_SIZE, 
                                   CFG_ORIG_FILE_MAX_SIZE_DEFAULT);
    m_uFileLen = cfgIniFile.GetInt(szAPSection, CFG_ORIG_FILE_MAX_SIZE, 
                                   m_uFileLen);
	//added by maimaoshi,2002-10-29
    /*
	//m_uFileLen必须大于3M
    if ((m_uFileLen < 3 * 1024) || (m_uFileLen > 20 * 1024))
    {
        m_uFileLen = CFG_ORIG_FILE_MAX_SIZE_DEFAULT;
        TRACE(MTS_FRONTSAVE, S_FS_TRC_ERR_CFG_FILE_SIZE, m_uFileLen);
    }
	*/
	
	if (m_uFileLen < 3 * 1024)
    {
        m_uFileLen = 3 * 1024;
		//当配置值小于最小值3M,设置为最小值3M
        TRACE(MTS_FRONTSAVE, S_FS_TRC_ERR_CFG_FILE_MINSIZE, m_uFileLen * 1024);
    }else if  (m_uFileLen > 20 * 1024)
	{
		m_uFileLen = 20 * 1024;
		//当配置值大于最大值20M,设置为最大值20M
        TRACE(MTS_FRONTSAVE, S_FS_TRC_ERR_CFG_FILE_MAXSIZE, m_uFileLen * 1024);
	}
	//end
    m_uFileLen *= 1024;     //进行从K到字节的转换

    //原始话单的文件名固定，不可配置
    strcpy(m_szPrefix, "b");
    m_nCsnLen = 10;
    m_uFileCsnStart = 1;
    m_uFileCsnEnd = 999999999;  //10个9已经大于UINT4的范围，所以只用9个9
    m_uFileCsnStep = 1;
    strcpy(m_szPostfix, "bil");
    m_uMaxFlushSpan = 1;    //前存盘必须每保存一包都进行Flush操作

    //得到并创建原始话单存放的根目录
    char szOrigRootDir[MAX_PATH];
    cfgIniFile.GetString(CFG_SEC_DISK_FILE, CFG_FRONT_SAVE_ROOT_DIR, 
                         CFG_FRONT_SAVE_ROOT_DIR_DEFAULT, 
                         szOrigRootDir, MAX_PATH);
    CreateDeepDir(szOrigRootDir);

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
    cfgIniFile.GetString(szAPSection, CFG_AP_NAME, szAPSection, 
                         szAPName, 255);
    m_szDatePath = STRING(szOrigRootDir) + "/" + szAPName;

	
	//Added by maimaoshi at 2002-09-23
	SNPRINTF(m_szSrcBasePath,MAX_PATH,"%s",
        MakePath(STRING(szOrigRootDir), szAPName).c_str());
    m_szSrcBasePath[MAX_PATH - 1] = 0;

	//从配置文件中的DiskFile中，读取话单文件的保留天数，初始化成员变量
    //m_uBillSavedDays;
    m_uBillSavedDays = cfgIniFile.GetInt(CFG_SEC_DISK_FILE,             //DiskFile
                                      CFG_SAVE_DEADLINE_DEL,         //DeadLineOfAutoDel
                                      CFG_SAVE_DEADLINE_DEL_DEFAULT);
    m_uBillSavedDays = cfgIniFile.GetInt(szAPSection,             //AccessPoint%d
                                      CFG_SAVE_DEADLINE_DEL,         //DeadLineOfAutoDel
                                      m_uBillSavedDays);                                      

    //话单文件的保留天数至少需要7天,最大是45天
    if(m_uBillSavedDays < 7)
    {
        m_uBillSavedDays = 7;
		//提示配置的值小于最小值7，设置为最小值7
		TRACE(m_szModuName, S_FS_TRC_ERR_CFG_AUTODEL_MINDAY, m_uBillSavedDays);
    }else if (m_uBillSavedDays > 180) //Modified by ZhengYuqun SWPD04451 2004-03-27
	{
		m_uBillSavedDays = 180;
		//提示配置的值大于最大值90，设置为最大值90
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

    //创建主备状态文件目录
    SNPRINTF(szMainStatusPath, MAX_PATH, "%s/%s/%s", 
             szMainStatusRootDir, szAPName, "save");
    szMainStatusPath[MAX_PATH - 1] = 0;
    CreateDeepDir(szMainStatusPath);
    SNPRINTF(szBakStatusPath, MAX_PATH, "%s/%s/%s", 
             szBakStatusRootDir, szAPName, "save");
    szBakStatusPath[MAX_PATH - 1] = 0;
    CreateDeepDir(szBakStatusPath);

    //设置主备状态文件的相关参数
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
    //在状态头中填入各变量的缺省值，如果状态文件不存在，则将缺省值写入状态文件
    FS_STATUS_FILE_HEAD* pHead = (FS_STATUS_FILE_HEAD* )m_pHead;

	//颜文远2002.05.18修改，问题单号D13767
    pHead->uFileCsn = m_uFileCsnStart - m_uFileCsnStep;
	//颜文远2002.05.18修改结束

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
功能描述    读取指定的话单包范围中的第一包能够读到的话单
输入参数    uFirstPID----需要读取的话单包范围中的起始包号 
            uLastPID----需要读取的话单包范围中的结束包号
输出参数    uPID----话单包对应的ID号 
            pPacket----话单包数据指针，如果无法读取到话单包，则返回为NULL，
                在函数内部申请空间，在函数外释放 
            uPacketLen----话单包数据长度 
            szFileFullPath----话单包所在的话单文件的全路径  
            uStartPos----话单包在文件中的起始位置
返回值  返回值说明如下： 
            0----读取成功 
            1----读取失败 
            <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值： 
                ERR_PID_TOO_LARGE----读取状态记录时要求的包号太大 
                ERR_READ_STATUS_FAIL----读状态文件失败 
                ERR_READ_BILLFILE_FAIL----读话单文件失败 
                ERR_PIDS_NOT_EXIST----要求的包号范围在状态文件中都不存在
抛出异常    无
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

    //获取第一个状态记录，并据此对各个变量进行赋值
    int nResult = GetInfoUnit(uFirstPID, &CurInfoUnit, m_nCurIndex);
    if ((ERR_READ_STATUS_FAIL == nResult) || (ERR_PID_TOO_LARGE == nResult))
    {
        return nResult;
    }

    //根据返回的状态值进行正确性的判断以及是否结束的判断
    if (CurInfoUnit.uPID > uLastPID)
    {
        m_bReadComplete = TRUE;
        return ERR_PIDS_NOT_EXIST;
    }
    else if (CurInfoUnit.uPID == uLastPID)
    {
        m_bReadComplete = TRUE;
    }

    //根据返回的状态记录对各个输出函数进行赋值
    uPID = CurInfoUnit.uPID;
    uPacketLen = CurInfoUnit.uPacketLen;

    //如果为空包，则直接返回，否则根据状态记录读取相应的话单数据
    if (0 != uPacketLen)
    {
        pPacket = new BYTE[uPacketLen];
        szFileFullPath = GetFileFullPath(&CurInfoUnit);
        uStartPos = CurInfoUnit.uStartPos;

        //从话单文件中读取话单包数据
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
功能描述    读取指定的话单包范围中的下一包能够读到的话单
输入参数    uLastPID--上一次包号
输出参数    uPID----话单包对应的ID号 
            pPacket----话单包数据指针，如果无法读取到话单包或已经取完了所
                要求范围内的所有话单包，则返回为NULL，在函数内部申请空间，
                在函数外释放 
            uPacketLen----话单包数据长度 
            szFileFullPath----话单包所在的话单文件的全路径  
            uStartPos----话单包在文件中的起始位置
返回值      返回值说明如下： 
                0----读取成功 
                1----读取失败 
                <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值： 
                    ERR_READ_STATUS_FAIL----读状态文件失败 
                    ERR_READ_BILLFILE_FAIL----读话单文件失败
                    ERR_ALREADY_FINISH----包号范围内的话单包都已经读取了
抛出异常    无
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

    //获取下一个状态记录的位置
    m_nCurIndex += 1;
    if (m_nCurIndex > m_InfoMain.GetUnitNum())
    {
        m_nCurIndex = 1;
    }

    //读取下一个状态记录
    int nResult = m_InfoMain.ReadUnit(m_nCurIndex, (BYTE* )&InfoUnit); 
    if (0 != nResult)
    {
        return ERR_READ_STATUS_FAIL;
    }

    //如果包号突然变小，则修正包号
    int nCurPID = InfoUnit.uPID;
    if(uLastPID > InfoUnit.uPID)
    {
        nCurPID    = uLastPID + 1;
        uPID       = nCurPID;
        //只能将这个包修改为空包
        uPacketLen = 0;
        TRACE(MTS_FRONTSAVE, S_FS_TRC_MAKE_EMPTY_PACKAGE, nCurPID);
    }
    else
    {
        //根据读到的找态记录对各参数进行赋值
        uPID       = InfoUnit.uPID;
        uPacketLen = InfoUnit.uPacketLen;
    }

    //判断是否包号范围内的所有话单包都已经被读出来了
    if ((nCurPID >= m_uLastPID) 
        || (m_nCurIndex == m_InfoMain.GetWriteIndex()))
    {
        m_bReadComplete = TRUE;
    }
    if (nCurPID > m_uLastPID)
    {
        return ERR_ALREADY_FINISH;
    }


    //如果为空包，则直接返回，否则根据状态记录读取相应的话单数据
    if (0 != uPacketLen)
    {
        szFileFullPath = GetFileFullPath(&InfoUnit);
        uStartPos = InfoUnit.uStartPos;
        pPacket = new BYTE[uPacketLen];

        //从话单文件中读取话单包数据
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

//根据状态文件得到最后一个话单包的包号
UINT4 CFrontSave::GetLastPID()
{
    return ((FS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
}

/**********************************************************************
功能描述    得到当前话单包的相关参数，这些参数用于向缓冲区传送话单包的相关信息
输出参数    szFileFullPath----当前话单包所在的话单文件全路径 
            uStartPos----当前话单包在话单文件中的偏移值
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
功能描述    得到下一个话单文件存放目录，如果目录不存在，则创建之
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CFrontSave::GetNextFilePath()
{
    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCurTime = localtime(&tCurrent);

    //如果当前日期与上个文件的日期不一样，则必须创建新的日期目录
    if ((ptmCurTime->tm_year != m_tmCreateTime.tm_year)
            || (ptmCurTime->tm_mon != m_tmCreateTime.tm_mon)
            || (ptmCurTime->tm_mday != m_tmCreateTime.tm_mday))
    {
        char szToday[20];
        strftime(szToday, 20, "%Y%m%d", ptmCurTime);
        m_szFilePath = m_szDatePath + "/" + szToday;

		//by yangshan 2004-04-20 对应问题单 SWPD04771
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
		//删除多余的文件
		DelMoreFiles();
		//end
    }
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
BOOL CFrontSave::DeleteTempFile()
{
    //读取主状态文件的最后一个状态记录
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

    //根据最后一个状态记录得到临时文件的全路径并将其删除
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
功能描述    根据收到的话单包，更新状态文件的内容及各成员变量
输入参数    pPacketInfo----话单包的相关信息，具体内容在子类里各自解释，
                在函数外负责申请和释放
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CFrontSave::RefreshStatus(BYTE* pPacketInfo)
{
    //对传入的信息进行类型转换
    SFrontPacketInfo* pFrontPacketInfo = (SFrontPacketInfo* )pPacketInfo; 
    m_uPID = pFrontPacketInfo->uPID;

    FS_STATUS_FILE_HEAD* pHead = (FS_STATUS_FILE_HEAD* )m_pHead;
    FS_STATUS_FILE_ELEMENT* pCurInfoUnit 
                = (FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit;

    //根据传入的信息更新当前状态记录
    pCurInfoUnit->uPID = pFrontPacketInfo->uPID;
    pCurInfoUnit->uFileCsn = m_uCurCsn;
    pCurInfoUnit->uStartPos = pHead->uFileWritePos;
    pCurInfoUnit->uPacketLen = pFrontPacketInfo->uPacketLen;
    pCurInfoUnit->nYear = m_tmCreateTime.tm_year;
    pCurInfoUnit->yMonth = m_tmCreateTime.tm_mon;
    pCurInfoUnit->yDay = m_tmCreateTime.tm_mday;

    //根据传入的信息更新文件头
    m_InfoMain.IncreaseIndex(m_pHead);

    pHead->uLastPID = pFrontPacketInfo->uPID;
    pHead->uFileCsn = m_uCurCsn;
    pHead->uFileWritePos += pFrontPacketInfo->uPacketLen;
    m_InfoMain.CalcCheckSum(m_pHead);

}

/**********************************************************************
功能描述    创建新的话单文件以及相关的状态文件刷新操作
输入参数    无
输出参数    无
返回值      创建新文件是否成功： 
                TRUE----创建成功 
                FALSE----创建失败
抛出异常    无
**********************************************************************/
BOOL CFrontSave::CreateNewFile()
{
    BOOL bSucc = CSaveTransaction::CreateNewFile();
    if (!bSucc)
    {
        return FALSE;
    }

    //刷新状态文件
    ((FS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn = m_uCurCsn;
    ((FS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos = 0;
    m_InfoMain.CalcCheckSum(m_pHead);

    //将更新后的文件头写入主状态文件
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

	//by 杨善    2004-4-24    对应问题单SWPD04770
     // //将更新后的文件头写入备状态文件
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
功能描述    关闭话单文件的后续处理
输入参数    无
输出参数    无
返回值      处理是否成功 
                TRUE----处理成功 
                FALSE----处理失败
抛出异常    无
**********************************************************************/
BOOL CFrontSave::DoWhenFileClose()
{
    ASSERT(NULL == m_pBillFile);

    //截断当前的话单文件
    UINT4 uWriteFilePos = ((FS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos;
    STRING szFileFullPath = m_szFilePath + "/" + m_szFileName;
    int nResult = ACE_OS::truncate(szFileFullPath.c_str(), uWriteFilePos);

    return (0 == nResult);
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
备注        前存盘的状态文件中，一个状态记录对应一个话单包，
            所以每处理一包时，必须更新状态头和状态记录，并进行Flush操作
**********************************************************************/
BOOL CFrontSave::WriteStatusFile(CInfoFile* pInfoFile, BOOL bFlush)
{
    ASSERT(bFlush);

    //写当前状态记录
    BOOL bSucc = pInfoFile->WriteUnit(m_pLastInfoUnit, m_uUnitLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //写状态头
    bSucc = pInfoFile->WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //进行Flush操作
    bSucc = pInfoFile->Flush();
    if (!bSucc)
    {
        return FALSE;
    }

    return TRUE;
}

/**********************************************************************
功能描述    在保存话单包事务中的写话单文件操作
输入参数    pPacketInfo----话单包的相关信息，具体内容在子类里各自解
                释，在函数外负责申请和释放
            bFlush----是否进行Flush操作
输出参数    无
返回值      返回值说明如下：
            0----读取成功
            1----读取失败
            <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值：
                ERR_WRITE_BILLFILE_FAIL----写话单文件失败
抛出异常    无
**********************************************************************/
int CFrontSave::WriteBillFile(BYTE* pPacketInfo, BOOL bFlush)
{
    BOOL bSucc;
    //对传入的信息进行类型转换
    SFrontPacketInfo* pFrontPacketInfo = (SFrontPacketInfo* )pPacketInfo; 

    //只有在话单包非空的情况下，才作实际的写盘操作，
    //否则不作任何操作
    if (pFrontPacketInfo->uPacketLen > 0)
    {
        //写话单文件
        bSucc = WritePacketData(pFrontPacketInfo->pPacket, 
                              pFrontPacketInfo->uPacketLen);
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
BOOL CFrontSave::DoWithMainDiffBak(BYTE* pMainHead, BYTE* pBakHead, 
								   BYTE* pMainLastUnit, BYTE* pBakLastUnit)
{
    BOOL bSucc;

	//颜文远2002.05.18修改，问题单号D13767
    //当主备状态文件的文件序列号不等时，删除最后一个话单文件
	//由于状态头中的文件序列号可能会被双机模块修改，
	//所以必须以最后一个状态记录中的文件序列号作为判断标准
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
功能描述    根据状态文件里的信息对相关的成员变量进行初始化。
输入参数    无
输出参数    无
返回值      操作是否成功
                TRUE----成功
                FALSE----失败
抛出异常    无
**********************************************************************/
BOOL CFrontSave::InitByStatusFile()
{
    //读取状态文件头和最后一个状态记录
    BOOL bSucc = m_InfoMain.ReadHead(m_pHead);
    if (!bSucc)
    {
        return FALSE;
    }

	//颜文远2002.05.18修改，问题单号D13767
    m_uCurCsn = ((FS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn;
    m_uPID = ((FS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
	//颜文远2002.05.18修改结束

    int nResult = m_InfoMain.ReadLastUnit(m_pLastInfoUnit);
    if ((ERR_FAIL_READ == nResult) || (ERR_INDEX_INVALID == nResult))
    {
        TraceModuleErr(MTS_FRONTSAVE, nResult);
        return FALSE;
    }
    else if (ERR_SUCCESS == nResult)
    //读取状态文件成功，则根据状态文件内容初始化相关成员变量
    {
        //得到最后一个文件的全路径，并据此初始化m_pBillFile
        GetFilePathAndName((FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit, 
                           m_szFilePath, m_szFileName);
        STRING szFileFullPath 
            = GetFileFullPath((FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit);
        m_pBillFile = fopen(szFileFullPath.c_str(), "rb+");
    
        //初始化m_tmCreateTime
        FS_STATUS_FILE_ELEMENT* pLastInfoUnit 
            = (FS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit;
        m_tmCreateTime.tm_year = pLastInfoUnit->nYear;
        m_tmCreateTime.tm_mon = pLastInfoUnit->yMonth;
        m_tmCreateTime.tm_mday = pLastInfoUnit->yDay;

        //为了防止上次运行时生成的最后一个日期目录被删除，
        //所以初始化时，创建一下该日期目录
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
功能描述    根据指定的状态记录获得话单文件的全路径
输入参数    pInfoUnit----状态记录
输出参数    无
返回值      话单文件的全路径
抛出异常    无
**********************************************************************/
STRING CFrontSave::GetFileFullPath(FS_STATUS_FILE_ELEMENT* pInfoUnit)
{
    STRING szFilePath, szFileName, szFileFullPath;

    GetFilePathAndName(pInfoUnit, szFilePath, szFileName);
    szFileFullPath = szFilePath + "/" + szFileName;

    return szFileFullPath;
}

/**********************************************************************
功能描述    根据指定的状态记录获得话单文件的路径和文件名
输入参数    pInfoUnit----状态记录
输出参数    szFilePath----话单文件路径
            szFileName----话单文件名
返回值      无
抛出异常    无
**********************************************************************/
void CFrontSave::GetFilePathAndName(FS_STATUS_FILE_ELEMENT* pInfoUnit,
                                    STRING& szFilePath, STRING& szFileName)
{
    //得到话单文件名
    STRING szFileCsn;
    szFileCsn = GetFileCsnStr(pInfoUnit->uFileCsn);
    szFileName = m_szPrefix + szFileCsn + "." + m_szPostfix;

    //得到话单文件的路径
    char szDateName[30];
    SNPRINTF(szDateName, 30, "%04d%02d%02d",
              pInfoUnit->nYear + 1900, pInfoUnit->yMonth + 1, pInfoUnit->yDay);
    szDateName[29] = 0;
    szFilePath = m_szDatePath + "/" + szDateName;
}

/**********************************************************************
功能描述    根据话单包号得到状态文件中相应的状态记录，如果找不到相关的记
            录，则得到符合以下条件的包号对应的状态记录----在状态文件中存
            在，且为所有大于uPID的包号中最小的包号
输入参数    uPID----需要读取的话单包的包号
输出参数    pInfoUnit----uPID话单包对应的状态记录，空间在函数外申请和释放 
            nIndex----pInfoUnit在状态文件中对应的索引号，从1开始编号。
                如果获取pInfoUnit失败，则为-1
返回值      返回值说明如下： 
                0----读取成功 
                1----读取失败 
                <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值： 
                    ERR_PID_TOO_LARGE----读取状态记录时要求的包号太大 
                    ERR_READ_STATUS_FAIL----读状态文件失败 
                    ERR_PID_NOT_FOUND----读取状态记录时要求的包号没有找到
抛出异常    无
**********************************************************************/
int CFrontSave::GetInfoUnit(const UINT4 uPID, 
                     FS_STATUS_FILE_ELEMENT* pInfoUnit, INT4& nIndex)
{
    UINT4 uValidUnitNum;  //状态文件中所包含的有效状态记录的个数
    int nResult;
    nIndex = -1;
    FS_STATUS_FILE_HEAD* pHead = (FS_STATUS_FILE_HEAD* )m_pHead;

    //计算有效的状态记录的个数
    if (pHead->uRotateCount > 0)
    {
        uValidUnitNum = m_InfoMain.GetUnitNum();
    }
    else
    {
        uValidUnitNum = m_InfoMain.GetWriteIndex() - 1;
    }

    //计算要求的包号与最后一个包号的差
    int nPIDSpan = pHead->uLastPID - uPID;

    if ((nPIDSpan < 0))  //uPID比状态文件中的最后一个状态记录还要大
    {
        return ERR_PID_TOO_LARGE;
    }
    //包号差比状态文件中的有效包个数还大，则从最小的有效记录开始
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
        //根据包号差计算要求的状态记录在状态文件中的位置
        nIndex = m_InfoMain.GetWriteIndex() - nPIDSpan - 1;
        if (nIndex <= 0)
        {
            nIndex += m_InfoMain.GetUnitNum();
        }
    }

    //根据计算出的状态记录的位置读取状态记录，
    //并在读出的状态记录中的包号比uPID小的时候顺序往下读取状态记录直到状态
    //记录的包号大于或等于uPID为止
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
功能描述    从指定的文件中读取一包话单数据
输入参数    szFileFullPath----话单包所在的话单文件
            uLen----话单包的长度
            uStartPos----话单包在话单文件中的起始位置
输出参数    pPacket----读出的话单包内容，内存在函数外申请和释放
返回值      读取话单包是否成功
                TRUE----读成功
                FALSE----读失败
抛出异常    无
**********************************************************************/
//从指定的文件中读取一包话单数据
BOOL CFrontSave::ReadPacket(const char* szFileFullPath, const BYTE* pPacket,
                            const UINT4 uLen, const UINT4 uStartPos)
{
    //打开文件
    FILE* pBillFile = fopen(szFileFullPath, "rb");
    if (NULL != pBillFile)
    {
        //设置文件的读写指针
        int nResult = fseek(pBillFile, uStartPos, SEEK_SET);
        if (0 != nResult)
        {
            fclose(pBillFile);
            return FALSE;
        }

        //读取话单数据
        size_t uLenRead = fread((void* )pPacket, uLen, 1, pBillFile);
        if (1 != uLenRead)
        {
            fclose(pBillFile);
            return FALSE;
        }

        //关闭话单文件
        fclose(pBillFile);
        return TRUE;
    }

    return FALSE;
}

//根据收到的包信息判断是否为空包
BOOL CFrontSave::IsEmptyPacket(BYTE* pInfo)
{
    SFrontPacketInfo* pFrontPacketInfo = (SFrontPacketInfo* )pInfo;

    return (0 == pFrontPacketInfo->uPacketLen);
}

/*****************************************************************************
函数原型    public: virtual void DelMoreFiles()
功能描述    删除多余文件任务处理函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
******************************************************************************/
void CFrontSave::DelMoreFiles()
{
    //以*.*作为条件在m_szSrcBasePath指向的目录中出第一个文件
    ACE_DIR *pDir = NULL;
    int nRet;
    dirent *pFileDirent =  NULL;    
    time_t curTime,DirTime;
	ACE_stat sDirStat;
    
    //打开文件目录
    pDir = ACE_OS::opendir(m_szSrcBasePath);
    if(pDir == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_OPEN_DIR_FAILED;
        sTmpInfo += m_szSrcBasePath;
        CWorkModule::MessageOut(MTS_FRONTSAVE, MSGOUT_LEVEL_IMPORTANT, sTmpInfo.c_str());
        //by ldf 2004-01-28 对core的定位修改
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
			//	TRACE(MTS_FRONTSAVE,S_BACKUP_TIMEERROR,DirTime,DirTime2);
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
                //查找下一个dir;
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

            //删除话单文件列表,包括内部结构的指针
            LISTFILELISTNODE::iterator iter = m_FileList.begin();
            while(iter != m_FileList.end())
            {
                delete (*iter);
                iter ++;
            }
            m_FileList.clear(); 
            //查找下一个文件.
        }
    }

    //删除话单文件列表,包括内部结构的指针;
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
