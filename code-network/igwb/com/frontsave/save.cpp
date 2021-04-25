#include "save.h"
#include "../include/toolbox.h"
#include <time.h>
#include <stdio.h>
#include "resource.h"

CSaveTransaction::CSaveTransaction()
{
    m_pHead = NULL;
    m_uHeadLen = 0;
    m_pLastInfoUnit = NULL;
    m_uUnitLen = 0;
    m_pBillFile = NULL;
/* Begin: 	Adder: Gan Zhiliang   Date: 2003-4-23*/
	m_uResFileProductTime = 0;
/* End  : Adder: Gan Zhiliang	   Date: 2003-4-23*/

    //备注：因为在一次运行中，每次localtime调用返回的tm*都指向同一块内存，
    //如果进行了两次localtime调用，则后一次的调用将会冲掉前一次的执行结果,
    //所以如果要保存返回值，必须将localtime的返回值另外拷贝到其它的内存中去
    //为了保证在程序第一次运行没有状态文件的情况下，能够正常创建日期目录，
    //所以必须将m_tmLastFileTime和m_tmCreateTime的年分别赋值为0和1，
    //以保证两者的时间不一致，且和当前年份不一致，从而会创建新的日期目录
    m_tmToday.tm_year = 0;
    m_tmCreateTime.tm_year = 1;
    m_tmLastFileTime.tm_year = 0;

	m_sCurFileFullPath = "";
    m_bAlwaysGenFile = FALSE;  //Added by ZhengYuqun 2004-01-17 SWPD03357
}

CSaveTransaction::~CSaveTransaction()
{
    if (NULL != m_pHead)
    {
        delete[] m_pHead;
    }
    if (NULL != m_pLastInfoUnit)
    {
        delete[] m_pLastInfoUnit;
    }

	if(m_FileList.empty() == true)
    {
        return;
    }

    LISTFILELISTNODE::iterator iter = m_FileList.begin();
    while(iter != m_FileList.end())
    { 
        if(*iter != NULL)
        {
            delete *iter;
        }
        iter++;
    }
    m_FileList.clear();
}

UINT4 CSaveTransaction::GetCurCsn(BOOL bDisplay)
{
	//如果用于向用户显示，则如果当前的文件序列号不合法的话，一律显示为0
	if (bDisplay 
		  && ((m_uCurCsn > m_uFileCsnEnd) || (m_uCurCsn < m_uFileCsnStart)))
	{
		return 0;
	}

    return m_uCurCsn;
}

/**********************************************************************
功能描述    处理初始化时的工作
输入参数    无
输出参数    无
返回值      初始化是否成功 
                TRUE----初始化成功 
                FALSE----初始化失败
抛出异常    无
**********************************************************************/
BOOL CSaveTransaction::Init()
{
    //获取当前时间，并据此初始化m_tmToday
    //备注：因为在一次运行中，每次localtime调用返回的tm*都指向同一块内存，
    //如果进行了两次localtime调用，则后一次的调用将会冲掉前一次的执行结果,
    //所以必须将localtime的返回值另外拷贝到其它的内存中去
    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCur = localtime(&tCurrent);
    m_tmToday = *ptmCur;

    //主备状态文件校验并根据状态文件进行变量的初始化
    BOOL bSucc = StartCheck();
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_CHECK_STATUS);
        return FALSE;
    }
    //打开主状态文件和备状态文件
    bSucc = m_InfoMain.OpenInfoFile(FALSE);
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_OPEN_MAIN);
        return FALSE;
    }
    bSucc = m_InfoBak.OpenInfoFile(FALSE);
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_OPEN_BAK);
        return FALSE;
    }

    bSucc = InitByStatusFile();
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_INITBYSTATUS);
        return FALSE;
    }
/* Begin: 	Adder: Gan Zhiliang   Date: 2003-4-23*/
    if(0 == m_uResFileProductTime)
	{
		bSucc = CloseCurFile();
	}
/* End  : Adder: Gan Zhiliang	   Date: 2003-4-23*/
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_CLOSE_BILL);
        return FALSE;
    }
    
    m_uLastFlushPID = m_uPID;

    return TRUE;
}

/**********************************************************************
功能描述    秒事件处理函数。
输入参数    无
输出参数    无
返回值      操作是否成功
抛出异常    无
**********************************************************************/
BOOL CSaveTransaction::Timer1Sec()
{
    //获取当前时间
    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCurTime = localtime(&tCurrent);
    
    //如果发生了跨天，则关闭当前的话单文件，并备份状态文件
    if ((ptmCurTime->tm_year != m_tmToday.tm_year)
        || (ptmCurTime->tm_mon != m_tmToday.tm_mon)
		|| (ptmCurTime->tm_mday != m_tmToday.tm_mday))
    {
        /* Begin: 	Adder: Gan Zhiliang   Date: 2003-4-23*/
		if(0 == m_uResFileProductTime)
		{
			BOOL bSucc = CloseCurFile();
			if (!bSucc)
			{				
				return FALSE;
			}
            //Added by ZhengYuqun 持续生成文件的时候也需要跨天关闭 SWPD04389 2004-03-26
            else if(m_bAlwaysGenFile)
            {
                CreateNewFile();
            }			
		}
       /* End  : Adder: Gan Zhiliang	   Date: 2003-4-23*/
		

        m_InfoMain.CopyFileForBackup();
        m_tmToday = *ptmCurTime;
    }

	return TRUE;
}

/**********************************************************************
功能描述    实现话单包保存的处理逻辑。 
            该处理流程为： 
                1）将状态信息写入主状态文件； 
                2） 将话单包写入话单文件中； 
                3）将状态信息写入备状态文件； 
            这三步处理构成一次事务操作，在中间某一步出错则该处理均为失败.
            为了安全性考虑，进行了写状态文件操作后，一定要进行状态文件的
            Flush操作，同时话单文件也必须进行Flush操作；但是为了效率考虑，
            并非每保存一包话单都要进行状态文件的刷新，而是在保存了指定包
            数的话单后进行一次状态更新（前存盘必须每保存一包话单进行一次
            状态文件更新，所以指定的更新间隔包数必须为1）
输入参数    pPacketInfo----话单包的相关信息，具体内容在子类里各自解释，
                在函数外负责申请和释放
输出参数    uWriteTime----写话单文件时间，单位ms，用于计费网关的统计功能
返回值      返回值说明如下： 
            0----读取成功 
            1----读取失败 
            <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值： 
                ERR_WRITE_MAIN_STATUS_FAIL----写主状态文件失败 
                ERR_WRITE_BAK_STATUS_FAIL----写备状态文件失败 
                ERR_WRITE_BILLFILE_FAIL----写话单文件失败 
                ERR_CREATE_BILLFILE_FAIL----创建新话单文件失败
                ERR_CLOSE_BILLFILE_FAIL----关闭当前话单文件失败
抛出异常    无
**********************************************************************/
int CSaveTransaction::WritePacket(BYTE* pPacketInfo, UINT4& uWriteTime)
{
    ACE_Profile_Timer timer;   //用于进行性能统计，精确到毫秒
    timer.start();

    BOOL bSucc = FALSE;
	BOOL bCreateNewFile = FALSE;  //是否新创建了话单文件

    //如果当前没有话单文件被打开，且话单包非空，则创建新的话单文件
    if ((NULL == m_pBillFile) && !IsEmptyPacket(pPacketInfo))
    {
		bCreateNewFile = TRUE;
        bSucc = CreateNewFile(); //创建新的话单文件
        if (!bSucc)
        {			
			m_pBillFile = NULL;
            return ERR_CREATE_BILLFILE_FAIL;
        }
    }

    RefreshStatus(pPacketInfo);

    //判断是否需要进行Flush操作，在写了指定数目的话单包后，必须进行Flush操作，
	//另外，在创建新文件后的第一包也必须进行Flush操作，
	//否则在退出时可能形成空话单文件
    BOOL bFlush = FALSE;
    if (bCreateNewFile || ((m_uPID - m_uLastFlushPID) >= m_uMaxFlushSpan))
    {
        bFlush = TRUE;
        m_uLastFlushPID = m_uPID;
    }
    else
    {
        bFlush = FALSE;
    }


    //写主状态文件
    bSucc = WriteStatusFile(&m_InfoMain, bFlush);
    if (!bSucc)
    {
		return ERR_WRITE_MAIN_STATUS_FAIL;
    }

    //写话单文件
    int nResult = WriteBillFile(pPacketInfo, bFlush);
    if (0 != nResult)
    {		
		return nResult;
    }

    //写备状态文件
    bSucc = WriteStatusFile(&m_InfoBak, bFlush);
    if (!bSucc)
    {
		return ERR_WRITE_BAK_STATUS_FAIL;
    }

    //如果当前话单文件已经超长，则必须关闭当前话单文件
/* Begin: 	Modifier: Gan Zhiliang   Date: 2003-4-23*/
    //if ((NULL != m_pBillFile) && (ftell(m_pBillFile) > m_uFileLen))
	if ((NULL != m_pBillFile) && (ftell(m_pBillFile) > m_uFileLen) 
        && (0 == m_uResFileProductTime) && !m_bAlwaysGenFile)  //增加最后一个条件 ZhengYuqun 2004-01-17 SWPD03357
/* End  : Modifier: Gan Zhiliang		   Date: 2003-4-23*/
    {
        bSucc = CloseCurFile();
        if (!bSucc)
        {			
			return ERR_CLOSE_BILLFILE_FAIL;
        }
    }

    //计算保存话单包所花费的时间
    timer.stop();
    ACE_Profile_Timer::ACE_Elapsed_Time et;
    timer.elapsed_time(et);
    uWriteTime = et.real_time;
    
    return ERR_SUCCESS;
}

/**********************************************************************
功能描述    计算下一个新话单文件的文件序列号
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CSaveTransaction::CalcNextCsn()
{
    m_uCurCsn += m_uFileCsnStep;
    if ((m_uCurCsn > m_uFileCsnEnd) || (m_uCurCsn < m_uFileCsnStart))
    {
        m_uCurCsn = m_uFileCsnStart;
    }
}

/**********************************************************************
功能描述    得到下一个话单文件的文件名
输入参数    无
输出参数    无
返回值      无 
抛出异常    无
**********************************************************************/
void CSaveTransaction::GetNextFileName()
{
    //获得下一个话单文件的文件序列号的字符串
    CalcNextCsn();
    STRING szFileCsn = GetFileCsnStr(m_uCurCsn);

    if (strlen(m_szPostfix) > 0)
    {
        m_szFileName = m_szPrefix + szFileCsn + "." + m_szPostfix;
    }
    else
    {
        m_szFileName = m_szPrefix + szFileCsn;
    }
}

/**********************************************************************
功能描述    创建新的话单文件以及相关的状态文件刷新操作。
            在基类中实现创建新文件的操作，在子类中负责实现状态文件的刷新
输入参数    无
输出参数    无
返回值      创建新文件是否成功： 
                TRUE----创建成功 
                FALSE----创建失败
抛出异常    无
**********************************************************************/
BOOL CSaveTransaction::CreateNewFile()
{
    //创建新文件，创建时必须申请指定的空间
    GetNextFileName();
    GetNextFilePath();
    m_sCurFileFullPath = m_szFilePath + "/" + m_szFileName;
    m_pBillFile = fopen(m_sCurFileFullPath.c_str(), "wb+");
    if (NULL == m_pBillFile)
    {
        return FALSE;
    }
    BOOL bSucc = AskFileSpace(m_pBillFile, m_uFileLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //更新m_tmCreateTime和m_tmLastFileTime的值
    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCurTime = localtime(&tCurrent);
    m_tmLastFileTime = m_tmCreateTime;
    m_tmCreateTime = *ptmCurTime;

    return TRUE;
}

/**********************************************************************
功能描述    关闭当前正在操作的话单文件，在文件超长、时间超时、跨天、
            启动恢复等情况下被调用
输入参数    无
输出参数    无
返回值      关闭话单文件是否成功
                TRUE----写成功
                FALSE----写失败
抛出异常    无
**********************************************************************/
BOOL CSaveTransaction::CloseCurFile()
{
    if (NULL != m_pBillFile)
    {
        ASSERT(FileSize(m_pBillFile) >= 0);

        m_uLastFlushPID = m_uPID;
        int nResult = fclose(m_pBillFile);
        if (0 != nResult)
        {
            return FALSE;
        }
        m_pBillFile = NULL;

        BOOL bSucc = DoWhenFileClose();
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************
功能描述    将一包话单写入到当前的话单文件中
输入参数    pPacket----话单包指针，在函数外申请和释放 
            uPacketLen----话单包长度
输出参数    无
返回值      写话单文件是否成功 
                TRUE----写成功 
                FALSE----写失败
抛出异常    无
**********************************************************************/
BOOL CSaveTransaction::WritePacketData(BYTE* pPacket, UINT4 uPacketLen)
{
    if (0 != uPacketLen)  //不是空包
    {
        //写话单包数据
        size_t uLenWritten = fwrite(pPacket, uPacketLen, 1, m_pBillFile);
        return (1 == uLenWritten);
    }

    return TRUE;
}

/**********************************************************************
功能描述    对主备状态文件进行校验工作，相应调用其他处理函数根据状态处理
            话单文件。在模块启动时调用。
输入参数    无
输出参数    无
返回值      操作是否成功
                TRUE----操作成功
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CSaveTransaction::StartCheck()
{
    BOOL bSucc;
    BOOL bStatusValid = FALSE;  //状态文件是否有效的标志
    BOOL bMainExist = FALSE, bBakExist = FALSE; //主备状态文件是否存在

    bMainExist = m_InfoMain.OpenInfoFile(FALSE);    //打开主状态文件
    bBakExist = m_InfoBak.OpenInfoFile(FALSE);      //打开备状态文件

	//颜文远2002.05.18修改，问题单号D13767
    if (bMainExist && bBakExist)    //主备状态文件同时存在
    {
        //主备状态文件头都有效
        if (m_InfoMain.CheckHead() && m_InfoBak.CheckHead())
        {
            TRACE(m_szModuName, S_FS_TRC_BOTH_VALID);

			int nResult;
            BYTE* pMainHead = new BYTE[m_uHeadLen];
			memset(pMainHead, 0, m_uHeadLen);
            BYTE* pBakHead = new BYTE[m_uHeadLen];
			memset(pBakHead, 0, m_uHeadLen);
			BYTE* pMainLastUnit = new BYTE[m_uUnitLen];
			memset(pMainLastUnit, 0, m_uUnitLen);
			BYTE* pBakLastUnit = new BYTE[m_uUnitLen];
			memset(pBakLastUnit, 0, m_uUnitLen);
            bSucc = m_InfoMain.ReadHead(pMainHead);
            if (!bSucc)
            {
                delete[] pMainHead;
                delete[] pBakHead;
				delete[] pMainLastUnit;
				delete[] pBakLastUnit;
                return FALSE;
            }
            bSucc = m_InfoBak.ReadHead(pBakHead);
            if (!bSucc)
            {
                delete[] pMainHead;
                delete[] pBakHead;
				delete[] pMainLastUnit;
				delete[] pBakLastUnit;
                return FALSE;
            }
            nResult = m_InfoMain.ReadLastUnit(pMainLastUnit);
            if ((ERR_INDEX_INVALID == nResult) || (ERR_FAIL_READ == nResult))
            {
                delete[] pMainHead;
                delete[] pBakHead;
				delete[] pMainLastUnit;
				delete[] pBakLastUnit;
                return FALSE;
            }
            nResult = m_InfoBak.ReadLastUnit(pBakLastUnit);
            if ((ERR_INDEX_INVALID == nResult) || (ERR_FAIL_READ == nResult))
            {
                delete[] pMainHead;
                delete[] pBakHead;
				delete[] pMainLastUnit;
				delete[] pBakLastUnit;
                return FALSE;
            }

            //主备状态文件头都有效，但相互间不一致
            if ((0 != memcmp(pMainLastUnit, pBakLastUnit, m_uUnitLen)) 
				|| (0 != memcmp(pMainHead, pBakHead, m_uHeadLen)))
            {
                TRACE(m_szModuName, S_FS_TRC_MAIN_DIFF_BAK);

                bSucc = DoWithMainDiffBak(pMainHead, pBakHead, 
										  pMainLastUnit, pBakLastUnit);
                if (!bSucc)
                {
                    delete[] pMainHead;
                    delete[] pBakHead;
					delete[] pMainLastUnit;
					delete[] pBakLastUnit;
                    return FALSE;
                }
            }
            else
            {
                TRACE(m_szModuName, S_FS_TRC_MAIN_SAME_BAK);
            }

            delete[] pMainHead;
            delete[] pBakHead;
			delete[] pMainLastUnit;
			delete[] pBakLastUnit;
            bStatusValid = TRUE;
        }
        //主状态文件头有效，但备状态文件头无效
        else if (m_InfoMain.CheckHead() && !m_InfoBak.CheckHead())
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_V_BAK_INV);

			bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            STRING szSrcFileFullPath = m_InfoMain.GetFileFullPath(); 
            STRING szDstFileFullPath = m_InfoBak.GetFileFullPath(); 
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //备状态文件头有效，但主状态文件头无效
        else if (!m_InfoMain.CheckHead() && m_InfoBak.CheckHead())
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_INV_BAK_V);

			bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            STRING szSrcFileFullPath = m_InfoBak.GetFileFullPath(); 
            STRING szDstFileFullPath = m_InfoMain.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        else//主备状态文件头都无效
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_INV_BAK_INV);

            bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
	//颜文远2002.05.18修改结束

    //主状态文件存在但备状态文件不存在
    else if (bMainExist && !bBakExist)
    {
        //主状态文件头有效
        if (m_InfoMain.CheckHead())
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_V_BAK_INE);

            STRING szSrcFileFullPath = m_InfoMain.GetFileFullPath();
            STRING szDstFileFullPath = m_InfoBak.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //主状态文件头无效
        else
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_INV_BAK_INE);

            bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
    //主状态文件不存在但备状态文件存在
    else if (!bMainExist && bBakExist)
    {
        //备状态文件头有效
        if (m_InfoBak.CheckHead())
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_INE_BAK_V);

            STRING szSrcFileFullPath = m_InfoBak.GetFileFullPath();
            STRING szDstFileFullPath = m_InfoMain.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //备状态文件头无效
        else
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_INE_BAK_INV);

            bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
    //主备状态文件都不存在
    else
    {
        TRACE(m_szModuName, S_FS_TRC_MAIN_INE_BAK_INE);
        bStatusValid = FALSE;
    }

    //如果状态文件无效，则创建新的主备状态文件，缺省的文件头存放在m_pHead中
    if (!bStatusValid)
    {
        bSucc = m_InfoMain.OpenInfoFile(TRUE, m_pHead);
        if (!bSucc)
        {
            return FALSE;
        }
        bSucc = m_InfoBak.OpenInfoFile(TRUE, m_pHead);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//根据序列号得到文件序列号的字符串
STRING CSaveTransaction::GetFileCsnStr(UINT4 uCsn)
{
    char szFormat[100], szFileCsn[100];

    SNPRINTF(szFormat, 100, "%%0%dd", m_nCsnLen);
    szFormat[99] = 0;
    SNPRINTF(szFileCsn, 100, szFormat, uCsn);
    szFileCsn[99] = 0;

    return szFileCsn;
}

/**************************************************************************
函数原型    protected: int DelOneDirBillFiles(char* szPath);
功能描述    删除指定目录下过期话单文件处理函数
输入参数    szPath: 待删除的话单文件目录
输出参数    无
返回值      删除过期话单文件是否成功，如果成功返回ERR_SUCCESS，否则返回非零
抛出异常    无
**************************************************************************/
int CSaveTransaction::DelOneDirBillFiles(char* szPath)
{
    STRING sFileNameDeleted = "";
    STRING szLog;
    
    //转换成时间结构
    //获取当前时间
    time_t tCurDate,tDirDate;
    time(&tCurDate);
    
    //得到目录时间
    tm tmDirDate;
    memset(&tmDirDate,0,sizeof(tm));
    STRING szDirDate(szPath);
    int nLen = szDirDate.size();
	
	STRING sYear = szDirDate.substr(nLen-8,4);
    STRING sMon = szDirDate.substr(nLen-4,2);
    STRING sDay = szDirDate.substr(nLen-2,2);	

    tmDirDate.tm_year = atoi(sYear.c_str()) - 1900;
    tmDirDate.tm_mon = atoi(sMon.c_str()) - 1;
    tmDirDate.tm_mday = atoi(sDay.c_str());

    tDirDate = mktime(&tmDirDate);

    // 当前日期与日期目录的日期之差小于话单文件保留天数
    if(difftime(tCurDate,tDirDate) < (m_uBillSavedDays * 24 * 60 * 60))
    {
        return ERR_FAIL;
    }
    
    PFilesListNode pCurFile;
    STRING szCurFile;
    LISTFILELISTNODE::iterator iter = m_FileList.begin();
    while(iter != m_FileList.end())
    {
        pCurFile = *iter;
        if(pCurFile == NULL) 
        {
            iter = m_FileList.erase(iter);
            continue;
        }
        
        szCurFile = szPath;
        szCurFile = MakePath(szCurFile, pCurFile->szFileName);
        
        //删除当前文件
        if(FileDel(szCurFile.c_str()) != TRUE)
        {
            if(sFileNameDeleted.empty() != true)
            {
                szLog += sFileNameDeleted;
                CWorkModule::WriteLog(m_szModuNameEx, szLog.c_str());
                sFileNameDeleted = "";
            }

            char szTmpBuf[500];
            sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
            //by ldf 2004-01-29 这个的Mesgout没有什么意义，资源文件也未定义，去掉之
            //STRING sTmpInfo = S_DELETE_BILL_FILE_FAILED;
            //sTmpInfo += szCurFile;
            //CWorkModule::MessageOut(m_szModuNameEx, MSGOUT_LEVEL_SIDLY, sTmpInfo.c_str());
            //by ldf 2004-01-28 对core的定位修改
            STRING sTmpInfo =szCurFile;
            sTmpInfo += ',';
            //end
            sTmpInfo += szTmpBuf;
            TRACE(m_szModuNameEx, sTmpInfo.c_str());

            iter++;
            continue;
        }
        
        if(sFileNameDeleted.empty() == true)
        {
            szLog = m_szModuNameEx;
			szLog += " Deleted ";
            szLog += szCurFile;
            szLog += " ~ ";
        }
        else 
        {
            int nPos1;
            int nPos2;
            STRING sTmpCurFile = pCurFile->szFileName;
            nPos2 = sFileNameDeleted.find_last_of(CHAR_DIR);
            STRING sTmpFileDel = sFileNameDeleted.substr(nPos2+1);
            nPos1 = sTmpCurFile.find(".");
            if(nPos1 == -1)
            {
                nPos1 = sTmpCurFile.length();
            }
            nPos2 = sTmpFileDel.find(".");
            if(nPos2 == -1)
            {
                nPos2 = sTmpFileDel.length();
            }
            STRING sCurFileBack4Int = (sTmpCurFile.substr(nPos1 - 4, 4)).c_str();
            STRING sFileDeletedBack4Int = (sTmpFileDel.substr(nPos2-4, 4)).c_str();

            if(atoi(sCurFileBack4Int.c_str()) - atoi(sFileDeletedBack4Int.c_str()) != 1)
            {
                szLog += sFileNameDeleted;
                CWorkModule::WriteLog(m_szModuNameEx, szLog.c_str());

                szLog = m_szModuNameEx;
				szLog += " Deleted ";
                szLog += szCurFile;
                szLog += " ~ ";
            }
        }

        sFileNameDeleted = pCurFile->szFileName;
        iter++;
    }
    
    if(sFileNameDeleted.empty() != true)
    {
        szLog += sFileNameDeleted;
        CWorkModule::WriteLog(m_szModuNameEx, szLog.c_str());
        sFileNameDeleted = "";
    }
    
    SweepDirectory(szPath);    
    return ERR_SUCCESS;
}

STRING CSaveTransaction::MakePath(const STRING &path, const char *filename)
{
    if(filename == NULL)
    {
        return STRING("");
    }

    char szTmp[MAX_PATH];
    if( *((path).end() - 1) != CHAR_DIR )
    {
        SNPRINTF(szTmp,MAX_PATH,"%s/%s", path.c_str(),filename);
        szTmp[MAX_PATH - 1] = 0;
    }

    else
    {
        SNPRINTF(szTmp,MAX_PATH,"%s%s", path.c_str(),filename);
        szTmp[MAX_PATH - 1] = 0;
    }
    return STRING(szTmp);
}
/**************************************************************************
函数原型    protected: virtual int PrepareFiles(char* szPath, 
                                                time_t tFileTime);
功能描述    准备文件列表
输入参数    szPath: 话单文件的路径
            tFileTime: 上次已成功备份的话单文件的修改时间
输出参数    无
返回值      准备话单文件是否成功，如果成功返回ERR_SUCCESS，否则返回非零
抛出异常    无
**************************************************************************/
int CSaveTransaction::PrepareFiles(const char *szPath, time_t tFileTime)
{
    // 打开目录
    ACE_DIR *pBaseDIR = ACE_OS_Dirent::opendir(szPath);
    if( pBaseDIR == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_OPEN_DIR_FAILED;
        sTmpInfo += szPath;
        MSGOUT(m_szModuName, MSGOUT_LEVEL_IMPORTANT, sTmpInfo.c_str());
        //by ldf 2004-01-28 对core的定位发现此处存在问题
        sTmpInfo =szPath;
        sTmpInfo += ',';
        //end
        sTmpInfo += szTmpBuf;
        TRACE(m_szModuName, sTmpInfo.c_str());

        return ERR_FAIL;
    }

    int nRet = 0;
    dirent *pDirent;
    ACE_stat astat;
    STRING szCurFile;
    PFilesListNode pFileNode;
    
    // 查找目录下的所有文件
    while((pDirent = ACE_OS_Dirent::readdir(pBaseDIR)) != NULL)
    {
        // 忽略所有目录
        szCurFile = szPath;
        szCurFile = MakePath(szCurFile, pDirent->d_name);
        nRet = ACE_OS::stat(szCurFile.c_str(), &astat);
        if(nRet != 0)
        {
            continue;
        }
        
        #ifdef _PLATFORM_WIN32
        #define S_ISDIR(mode)   (((mode) & S_IFMT) == S_IFDIR)
        #endif
        nRet = S_ISDIR(astat.st_mode);
        if(nRet != 0 )
        {
            continue;
        }

        // 忽略所有在tFileTime之后没有修改过的文件
        if(astat.st_mtime < tFileTime)
        {
            continue;
        }
        
        pFileNode = new SFilesListNode;
        pFileNode->tFileTime = astat.st_mtime;
        ACE_OS::strcpy(pFileNode->szFileName, pDirent->d_name);
        m_FileList.push_back(pFileNode);
    }
    
    ACE_OS_Dirent::closedir(pBaseDIR);
    CSaveTransaction::Compare comp;
    std::sort(m_FileList.begin(), m_FileList.end(), comp);

    return ERR_SUCCESS;
}


bool CSaveTransaction::Compare::operator ()(SFilesListNode* x, SFilesListNode *y)
{
    if(x->tFileTime < y->tFileTime)
    {
        return true;
    }

    if(x->tFileTime > y->tFileTime)
    {
        return false;
    }

    //if(x->tFileTime == y->tFileTime)
    if(strcmp(x->szFileName, y->szFileName) < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CSaveTransaction::GetCurrentFileName(STRING& sFileName)
{
	sFileName = m_sCurFileFullPath;
}
