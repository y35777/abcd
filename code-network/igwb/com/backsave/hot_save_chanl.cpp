#include "hot_save_chanl.h"
#include "../include/msg_def.h"

CHotSaveChannel::CHotSaveChannel(CPatchEncap* pPatchEncap) : CSaveChannel(pPatchEncap)
{
    m_pSeccondFile = NULL;
}

CHotSaveChannel::~CHotSaveChannel()
{
    if (NULL != m_pSeccondFile)
    {
        fclose(m_pSeccondFile);
        m_pSeccondFile = NULL;
    }
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
更新记录: 
日期            姓名                  描述 
========== ================== ======================
04-05-14        chenliangwei     创建。
**********************************************************************/
BOOL CHotSaveChannel::Init(UINT4 uAPID, UINT4 uChannelID, STRING& szChannelName, int nBillLen)
{
    //如果配置为HotSave，则m_bSecondCopy无条件生效
    m_bSecondCopy = TRUE;

    return CSaveChannel::Init(uAPID, uChannelID, szChannelName, nBillLen);
    
}

/**********************************************************************
功能描述    创建第二份话单文件，并调用父类的同名函数。
输入参数    无
输出参数    无
返回值      创建新文件是否成功：
                TRUE----创建成功
                FALSE----创建失败
抛出异常    无
备注        先创建第二份话单文件，再调用父类的同名函数进行临时文件的创建
            和状态文件的更新。
更新记录: 
日期            姓名                  描述 
========== ================== ======================
04-05-14        chenliangwei     创建。
**********************************************************************/
BOOL CHotSaveChannel::CreateNewFile()
{
    //构造前缀
    BOOL bSucc = CSaveTransaction::CreateNewFile();
    if (!bSucc)
    {
        return FALSE;
    }

    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCurTime = localtime(&tCurrent);
    m_tmCreateTime = *ptmCurTime;

    STRING szPrefix = FormatCfgStr(m_szFormalPrefix, 
                                   m_tmCreateTime, 
                                   m_uChannelID);

    //构造序列号
    STRING szFileCsn = GetFileCsnStr(m_uCurCsn);

    STRING szMidfix = FormatCfgStr(m_szFormalMidfix, 
                                   m_tmCreateTime, 
                                   m_uChannelID);

	//by	杨善	2004-5-29	对应问题单SWPD05385	
    //构造第二份话单文件文件名
    if (0 == strlen(m_szFormalPostfix))
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        m_szSecondFileName = szPrefix + szFileCsn + szMidfix;
    }
    else
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        m_szSecondFileName
             = szPrefix + szFileCsn + szMidfix + "." + m_szFormalPostfix;
    }
	//the end	2004-5-29		对应问题单SWPD05385
	

    STRING szSecondFullPath = m_szSecondPath + "/" + m_szSecondFileName;

    //创建第二份话单文件
    m_pSeccondFile = fopen(szSecondFullPath.c_str(), "wb+");
    if (NULL == m_pSeccondFile)
    {
        FileDel(m_sCurFileFullPath.c_str());
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
                = tCurrent;

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
    if ((m_uCurCsn > m_uFileCsnEnd) || (m_uCurCsn < m_uFileCsnStart - m_uFileCsnStep))
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

    return TRUE;

}
/**********************************************************************
功能描述    关闭话单文件的后续处理，关闭并截断第二份话单文件。
输入参数    无
输出参数    无
返回值      处理是否成功
                TRUE----处理成功
                FALSE----处理失败
抛出异常    无
更新记录: 
日期            姓名                  描述 
========== ================== ======================
04-05-14        chenliangwei     创建。
**********************************************************************/
BOOL CHotSaveChannel::DoWhenFileClose()
{
    //关闭第二份话单文件
    if (NULL != m_pSeccondFile)
    {
        int nResult = fclose(m_pSeccondFile);
        if (0 != nResult)
        {
            return FALSE;
        }
        
        m_pSeccondFile = NULL;
        
        //截断第二份话单文件
        UINT4 uWriteFilePos = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos;
        STRING szFileFullPath = m_szSecondPath + "/" + m_szSecondFileName;
        nResult = ACE_OS::truncate(szFileFullPath.c_str(), uWriteFilePos);
        if (0 != nResult)
        {
            return FALSE;
        }
    }
    
    //为防止重复做复制第二份文件的操作，
    //这里将m_bSecondCopy先禁用，再启用
    m_bSecondCopy = FALSE;
    BOOL bSucc = CSaveChannel::DoWhenFileClose();
    m_bSecondCopy = TRUE;

    return bSucc;
}


/**********************************************************************
功能描述    在保存话单包事务中的写话单文件操作
            再写完第一份临时文件后，再写第二份话单文件
输入参数    pPacketInfo----话单包的相关信息，在函数外负责申请和释放
            bFlush----是否进行Flush操作
输出参数    无
返回值      返回值说明如下：
                0----读取成功
                1----读取失败
                <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值：
                    ERR_WRITE_BILLFILE_FAIL----写话单文件失败
抛出异常    无
更新记录: 
日期            姓名                  描述 
========== ================== ======================
04-05-14        chenliangwei     创建。
**********************************************************************/
int CHotSaveChannel::WriteBillFile(BYTE* pPacketInfo, BOOL bFlush)
{
    int nRet = CSaveChannel::WriteBillFile(pPacketInfo, bFlush);

    if ( ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //对传入的信息进行类型转换
    SBackPacketInfo* pBackPacketInfo = (SBackPacketInfo* )pPacketInfo;

    //写入第二份话单文件
    //只有在话单包非空的情况下，才作实际的创建新文件、写盘等操作，
    //否则不作任何操作
    if (pBackPacketInfo->uLen >0)
    {
        if(m_pSeccondFile != NULL)
        {
            //写话单包数据
            size_t uLenWritten = fwrite(pBackPacketInfo->pPacket, 
                                        pBackPacketInfo->uLen, 1, m_pSeccondFile);

            if (1 != uLenWritten)
            {
                return ERR_WRITE_BILLFILE_FAIL;
            }

            if (bFlush)
            {
                //进行Flush操作
                int nResult = fflush(m_pSeccondFile);
                if (0 != nResult)
                {
                    return ERR_WRITE_BILLFILE_FAIL;
                }
            }
        }
    }

    return ERR_SUCCESS;
}

BOOL CHotSaveChannel::DeleteTempFile()
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
    
    time_t tFileCreateTime = LastUnitInfo.uReserved;
    tm* ptmCreateTime = localtime(&tFileCreateTime);

    //得到第二份文件的路径和文件名
    STRING szPrefix = FormatCfgStr(m_szFormalPrefix, 
                                   *ptmCreateTime, 
                                   m_uChannelID);
    
    //构造序列号
    STRING szFileCsn = GetFileCsnStr(LastUnitInfo.uFileCsn);

    STRING szMidfix = FormatCfgStr(m_szFormalMidfix, 
                                   m_tmCreateTime, 
                                   m_uChannelID);

	//by	杨善	2004-5-29	对应问题单SWPD05385
    //构造第二份话单文件文件名
    STRING szSecondFileName;
    if (0 == strlen(m_szFormalPostfix))
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        szSecondFileName = szPrefix + szFileCsn + szMidfix;
    }
    else
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        szSecondFileName
             = szPrefix + szFileCsn + szMidfix + "." + m_szFormalPostfix;
    }
	//the end	2004-5-29		对应问题单SWPD05385

    STRING szSecondFullPath = m_szSecondPath + "/" + szSecondFileName;

    BOOL bSucc = FileDel(szSecondFullPath.c_str());
    if (bSucc)
    {
        TRACE(m_szModuName, S_BS_TRC_SUCC_DEL_FILE, LastUnitInfo.uFileCsn);
    }
    else
    {
        TRACE(m_szModuName, S_BS_TRC_FAIL_DEL_FILE, LastUnitInfo.uFileCsn);
    }    

    return CSaveChannel::DeleteTempFile();    
    
}

/**********************************************************************
功能描述    根据状态文件里的信息对相关的成员变量进行初始化
            增加了对第二份文件的打开操作。
输入参数    无
输出参数    无
返回值      操作是否成功
                TRUE----操作成功
                FALSE----操作失败
抛出异常    无
更新记录: 
日期            姓名                  描述 
========== ================== ======================
04-05-14        chenliangwei     创建。
**********************************************************************/
BOOL CHotSaveChannel::InitByStatusFile()
{
    BOOL bSucc = m_InfoMain.ReadHead(m_pHead);
    if (!bSucc)
    {
        return FALSE;
    }

    m_uCurCsn = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn;
    m_uPID = ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;

    int nResult = m_InfoMain.ReadLastUnit(m_pLastInfoUnit);
    if ((ERR_FAIL_READ == nResult) || (ERR_INDEX_INVALID == nResult))
    {
        TraceModuleErr(MTS_BACKSAVE, nResult);
        return FALSE;
    }
    else if (ERR_SUCCESS == nResult)
    {
        //读取状态文件成功，则根据状态文件内容初始化相关成员变量

        //得到最后一个文件的全路径，并据此初始化m_pBillFile
        //在双机对状态文件进行同步的情况下，m_pHead以及m_pLastInfoUnit
        //中的文件序列号可能不一致，必须以m_pLastInfoUnit中的为准打开
        //上次运行时产生的最后文件
        m_szFileName = m_szPrefix
            + GetFileCsnStr(((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFileCsn)
            + "." + m_szPostfix;

        STRING szFileFullPath = m_szFilePath + "/" + m_szFileName;
        m_pBillFile = fopen(szFileFullPath.c_str(), "rb+");

        //初始化m_tmCreateTime
        if (NULL != m_pBillFile)
        {
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

            //得到第二份文件的路径和文件名
            STRING szPrefix = FormatCfgStr(m_szFormalPrefix, 
                                           m_tmCreateTime, 
                                           m_uChannelID);

            //构造序列号
            STRING szFileCsn = 
                GetFileCsnStr(((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFileCsn);

            STRING szMidfix = FormatCfgStr(m_szFormalMidfix, 
                                           m_tmCreateTime, 
                                           m_uChannelID);
			
			//by	杨善	2004-5-29	对应问题单SWPD05385
            //构造第二份话单文件文件名
            if (0 == strlen(m_szFormalPostfix))
            {
				if (0 == m_nCsnLen)
				{
					szFileCsn = "";
				}
                m_szSecondFileName = szPrefix + szFileCsn + szMidfix;
            }
            else
            {
				if (0 == m_nCsnLen)
				{
					szFileCsn = "";
				}
                m_szSecondFileName
                     = szPrefix + szFileCsn + szMidfix + "." + m_szFormalPostfix;
            }
			//the end	2004-5-29		对应问题单SWPD05385
			

            STRING szSecondFullPath = m_szSecondPath + "/" + m_szSecondFileName;
            m_pSeccondFile = fopen(szSecondFullPath.c_str(), "rb+");          
       }
    }

    return TRUE;    
}