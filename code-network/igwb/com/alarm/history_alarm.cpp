    // HistoryAlarm.cpp: implementation of the CHistoryAlarm class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/frame.h"
#include "../include/toolbox.h"
#include "../utility/config.h"
#include "history_alarm.h"
#include "resource.h"
#include "ace/ACE.h"
#include "ace/OS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//功能描述  CHistoryAlarm类的构造函数中传入的szAlarmFile与成员变量
//          m_szAlarmFile对应表示历史告警的存储路径
//输入参数
//*szAlarmFile      －存放告警文件名
// nAlarmUpperLimit －告警条目上限
// nAlarmDelNum     －告警条目超过上限时,一次性删除的告警个数
//张玉新2001-10-20
////////////////////////////////////////////////////////////////////////
CHistoryAlarm::CHistoryAlarm(const char* szAlarmFile,int nAlarmDelNum,int nAlarmUpperLimit)
{
    //拷贝文件名（无后缀信息）
    sprintf(m_szAlarmFile,"%s", szAlarmFile);

    //初始化各成员变量
    m_nAlarmUpperLimit  = nAlarmUpperLimit;
    m_nAlarmDelNum      = nAlarmDelNum;
    m_nRecordNum = 0;
    //by ldf 2002.1.21因为起始的流水号为100
    //by ldf 2004-01-31 OM整改，这里的99，第一次需要从CHK ALM中获得
    m_uCsn = 99;
    m_nReadIndex        = 0;
    m_pAlarmUtil        = NULL;
    m_uSyn = 99;
    m_uMMLAlmBaseID = 0;
}

CHistoryAlarm::~CHistoryAlarm()
{
    Close();
    if(NULL != m_pAlarmUtil)
    {
        delete m_pAlarmUtil;
        m_pAlarmUtil = NULL;
    #ifdef _OM_MODIFY
        //由于可能删除告警信息时造成文件有冗余的告警数据
        //所以应根据m_nRecordNum长度截短告警文件
        //by ldf 2004-01-31 OM整改
        ACE_OS::truncate(m_szAlarmFile,SIZEOF_ALARMHEAD*5+SIZEOF_HISTORYALARM*m_nRecordNum);	
        m_nRecordNum = 0;//by lx SWPD04555
    #endif

    }
}

//////////////////////////////////////////////////////
//功能描述  函数Open(void)打开历史告警库文件m_szAlarmFile
//输入参数  无
//输出参数  无
//返回值    返回ERR_SUCCESS(0)表示文件打开成功,返回非0表示打开失败
//张玉新 2001-10-20
/////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::Open()
{
    // 从配置库里获取是否用XML
    CINIFile ConfigFile(GetCfgFilePath());
    ConfigFile.Open();
    m_bUseXml = ConfigFile.GetInt(CFG_SEC_ALARM, 
                                  CFG_USE_XML, 
                                  CFG_USE_XML_DEFAULT);

    m_pAlarmUtil = CreateAlarmUtilObj();
    if(NULL == m_pAlarmUtil)
    {
        return ERR_FAIL;
    }

    STRING szFilePath = m_szAlarmFile;
    szFilePath       += "/";
    szFilePath       += GetAlarmFile();

    //此后m_szAlarmFile中带了文件名后缀信息
    SNPRINTF(m_szAlarmFile, sizeof(m_szAlarmFile), "%s", szFilePath.c_str());
    m_szAlarmFile[sizeof(m_szAlarmFile) - 1] = '\0';

    //如果文件已打开则关闭
    Close();

    //判断文件是否存在
    BOOL bFileExist = (access(m_szAlarmFile, 0) == 0);
    if (!bFileExist)
    {
        int nPos  = szFilePath.find_last_of('/');
#ifdef _PLATFORM_WIN32
        int nPos2 = szFilePath.find_last_of('\\');
#else //_PLATFORM_WIN32
        int nPos2 = -1;
#endif //_PLATFORM_WIN32
        nPos      = max(nPos, nPos2);
        if (nPos <= 0)
        {
            return ERR_FAIL;
        }

        STRING szDirPath = szFilePath.substr(0,nPos);

        //创建文件所在目录
        if (!CreateDeepDir(szDirPath.c_str()))
        {
            return ERR_FAIL;
        }
    }

    //初始化XMLFile,如指定的XML文件不存在，则新建一个仅包含指定根结点的XML文件
    if (ERR_SUCCESS != m_pAlarmUtil->OpenFile(m_szAlarmFile))
    {
        return ERR_OPEN_HISTORY_ALARM;
    }

    if (bFileExist) //文件原本就存在
    {
        //读取告警流水号信息,若读取错误则返回失败
        m_pAlarmUtil->ReadHeadElement(m_uCsn);
        if(ERR_SUCCESS != m_pAlarmUtil->ReadHeadElement(m_uCsn))
        {
            Close();
            TRACE(MTS_ALARM,S_ALARM_OPEN_HISTORY_FAIL);
            return ERR_OPEN_HISTORY_ALARM;
        }
#ifdef _OM_MODIFY
        //by ldf 2004-01-31 OM整改
        //读取告警同步号信息,若读取错误则返回失败
        m_pAlarmUtil->ReadHeadElementSyn(m_uSyn);
        if(ERR_SUCCESS != m_pAlarmUtil->ReadHeadElementSyn(m_uSyn))
        {
            Close();
            TRACE(MTS_ALARM,S_ALARM_OPEN_HISTORY_FAIL);
            return ERR_OPEN_HISTORY_ALARM;
        }


        //读取告警同步基准号信息,若读取错误则返回失败
        m_pAlarmUtil->ReadHeadElementMMLBaseID(m_uMMLAlmBaseID);
        if(ERR_SUCCESS != m_pAlarmUtil->ReadHeadElementMMLBaseID(m_uMMLAlmBaseID))
        {
            Close();
            TRACE(MTS_ALARM,S_ALARM_OPEN_HISTORY_FAIL);
            return ERR_OPEN_HISTORY_ALARM;
        }
        //end
#endif



        //当文件打开成功时,获得当前告警数据库文件中告警记录数
        m_nRecordNum = m_pAlarmUtil->GetCount();
        
        return ERR_SUCCESS;
    }
    else//文件不存在时创建告警文件
    {
        //写入告警流水号
        //by ldf 2002.1.21,因为起始流水号为100
        m_uCsn = 99;
        if(WriteCsn(m_uCsn) != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }
//#ifdef _OM_MODIFY
        //by ldf 2004-01-31 OM整改
        m_uSyn = 99;

        if(WriteSyn(m_uSyn) != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }

        m_uMMLAlmBaseID = 0;
        if(WriteMMLBaseID(m_uMMLAlmBaseID) != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }
        //end
//#endif
        return ERR_SUCCESS;
    }
}

int CHistoryAlarm::Close()
{
    //关闭告警文件
    if(m_pAlarmUtil != NULL)
    {
        m_pAlarmUtil->CloseFile();
    }
    
    //by ldf 2002.1.21
    m_uCsn = 99;
    m_uSyn = 99;

    return ERR_SUCCESS;//self check add
}

UINT4 CHistoryAlarm::CreateCsn()
{
    //生成告警流水号,并更新告警数据库文件
    ++m_uCsn;
    WriteCsn(m_uCsn);

    //返回告警流水号
    return m_uCsn;
}

//by ldf 2004-01-31 OM整改
UINT4 CHistoryAlarm::CreateSyn()
{
    //生成告警流水号,并更新告警数据库文件
    ++m_uSyn;
    WriteSyn(m_uSyn);

    //返回告警流水号
    return m_uSyn;
}

//////////////////////////////////////////////////
//功能描述  回退告警流水号
//输入参数  无
//输出参数  无
//返回值    有效的Csn值
//张玉新2001-10-20
///////////////////////////////////////////////
UINT4 CHistoryAlarm::RollBackCsn()
{
    //如果m_nCsn为100; 因为告警的起始流水号是100
    //by ldf 2002.1.21
    if(100 > m_uCsn)
    {
        return 99;
    }

    //若m_nCsn不为1,则返回--m_nCsn;
    --m_uCsn;

    //更新告警数据库
    WriteCsn (m_uCsn);

    return m_uCsn;
}

//by ldf 2004-01-31 OM整改
UINT4 CHistoryAlarm::RollBackSyn()
{
    //如果m_nSyn为100; 因为告警的起始流水号是100
    if(100 > m_uSyn)
    {
        return 99;
    }
    
    //若m_nSyn不为1,则返回--m_nSyn;
    --m_uSyn;
    
    //更新告警数据库
    WriteSyn(m_uSyn);

    return m_uSyn;
}

///////////////////////////////////////////////////////////////
//功能描述  改写文件头的告警流水号
//输入参数  nCsn－要写入文件的告警流水号
//输出参数  无
//返回值    ERR_SUCCESS(0)表示成功，否则为失败码
///////////////////////////////////////////////////////////////
int CHistoryAlarm::WriteCsn(UINT4 csn)
{
    
    //写告警流水号
    if(ERR_SUCCESS != m_pAlarmUtil->WriteHeadElement(csn))
    {
        TRACE(MTS_ALARM,S_ALARM_FILE_ERITECSN_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FAIL_FLUSH_CSN);
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

//by ldf 2004-01-31 OM整改
int CHistoryAlarm::WriteSyn(UINT4 syn)
{
//#ifdef _OM_MODIFY
    //写告警同步号
    if(ERR_SUCCESS != m_pAlarmUtil->WriteHeadElementSyn(syn))
    {
        TRACE(MTS_ALARM,S_ALARM_FILE_ERITESYN_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FAIL_FLUSH_SYN);
        return ERR_FAIL;
    }
//#endif
    return ERR_SUCCESS;

}

//by ldf 2004-01-31 OM整改
int CHistoryAlarm::WriteMMLBaseID(UINT4 uMMLBaseID )
{
//#ifdef _OM_MODIFY
    //写告警MML基准号
    if(ERR_SUCCESS != m_pAlarmUtil->WriteHeadElementMMLBaseID(uMMLBaseID))
    {
        TRACE(MTS_ALARM,S_ALARM_FILE_ERITEMMLBASEID_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FAIL_FLUSH_MMLBASEID);
        return ERR_FAIL;
    }
//#endif

    return ERR_SUCCESS;

}
//end


UINT4 CHistoryAlarm::GetCsn()
{
    return m_uCsn;
}

//by ldf 2004-01-31 OM整改
UINT4 CHistoryAlarm::GetSyn()
{
    return m_uSyn;
}

UINT4 CHistoryAlarm::GetMMLBaseID()
{
    return m_uMMLAlmBaseID;
}
//end
////////////////////////////////////////////////////////////////////////
//功能描述  函数将一条告警记录pAlarm加入历史告警库的尾部,返回操作结果,
//          pAlarm指向一条告警信息结构.
//输入参数  pAlarm - 历史告警结构
//输出参数  无
//返回值    执行成功均返回0,执行失败均返回not 0值
//////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::AddRecord(SHistoryAlarm *pAlarm)
{
    //将告警记录添加到第m_nRecordNum条告警记录后
    if(ERR_SUCCESS != m_pAlarmUtil->SaveHisAlarm(m_nRecordNum, *pAlarm))
    {
        TRACE(MTS_ALARM,S_ALARM_ADD_HIS_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    m_nRecordNum++;
    return ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////
//功能描述  添加数条告警记录到告警数据库文件中
//参数      uNum－预追加到历史告警库尾部的告警记录数
//      pAlarm－指向uNum个告警信息结构
//输出参数  无
//返回值    成功添加到告警文件中的告警记录数
////////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::AddRecord(SHistoryAlarm pAlarm[], UINT4 uNum)
{
    //添加告警到告警文件中,并返回成功添加的告警记录数
    int nWriteNum = 0;

    for(UINT4 i = 0; i<uNum; i++)
    {

        if(AddRecord(&pAlarm[i]) == ERR_SUCCESS)
        {
            nWriteNum++;
        }
        else
        {
            break;
        }

    }

    return nWriteNum;
}

//////////////////////////////////////////////////////////////////////
//功能描述  根据告警流水号查找告警记录
//输入参数  uCsn - 预追加到历史告警库尾部的告警记录数
//输出参数  无
//返回值    该告警在告警文件中的位置,以告警记录为单位,为-1时表示没有找到
////////////////////////////////////////////////////////////////////////
//!!!!
//因OM整改，uCsn在历史告警库中不是唯一的，所以请尽量不要使用这个函数
//如果是查找历史库已经有的告警,请考虑使用该告警的同步号,利用FindRecordBySyn，
//uSyn在告警库中是唯一的
int CHistoryAlarm::FindRecord(UINT4 uCsn)
{
    //从文件中读取所有告警记录信息,如果读取错误则返回-1;
    //由于不考虑告警序列号回转的情况,历史告警库中告警记录的流水号是非递增排列的,
    //所以采用逐个查找法 从最开始查找,找到第一个流水号为uCsn的历史告警记录
    int nRetPos = -1;
    int nLeft   = 0;
#ifdef _OM_MODIFY     //对于OM整改后，由于csn不连续递增，则用逐个匹配的方法进行查找
    int nRight  = m_nRecordNum - 1;
    SHistoryAlarm hisAlarm; 
    
    while(nLeft <= nRight)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nLeft, hisAlarm))
        {
            break;
        }
        
        if(uCsn == hisAlarm.uCsn)
        {
            nRetPos = nLeft;
            break;
        }
        
        nLeft++;
    }
#else         //对于非OM整改的版本，仍采用二份查找以提高效率

    int nRight  = m_nRecordNum - 1;
    int nMid    = (nRight-nLeft) / 2;

    UINT4 nLeftCsn,nMidCsn,nRightCsn;
    SHistoryAlarm hisAlarm;
    
    while(1)        
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nLeft, hisAlarm))
        {
            break;
        }
        nLeftCsn = hisAlarm.uCsn;
        
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nMid, hisAlarm))
        {
            break;
        }
        nMidCsn = hisAlarm.uCsn;
        
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nRight, hisAlarm))
        {
            break;
        }
        nRightCsn = hisAlarm.uCsn;
        
        if(nLeft == nMid)
        {
            if(uCsn == nLeftCsn)
                nRetPos = nLeft;
            else
                if(uCsn == nRightCsn)
                    nRetPos = nRight;
                break;
        }
        if(uCsn < nLeftCsn || uCsn > nRightCsn)
            break;
        if(uCsn == nMidCsn)
        {
            nRetPos = nMid;
            break;
        }
        else
        {
            if(uCsn < nMidCsn)
            {
                nRight = nMid;
            }
            else
            {
                nLeft = nMid;
            }
            //by lx 2003-09-28 SWPD01792
            nMid = nLeft + (nRight-nLeft)/2;
        }
    }
#endif
    return nRetPos;
}

////////////////////////////////////////////////////////////////////////////////////////
//功能描述  函数用pAlarm覆盖历史告警库中与pAlarm拥有相同告警流水号(CSN)的历史告警记录
//输入参数  pAlarm指向一条历史告警信息
//输出参数  无
//返回值    成功返回0,失败返回错误码
//////////////////////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::RewriteRecord(SHistoryAlarm *pAlarm)
{
    if(NULL == pAlarm)
    {
        return ERR_POINTER_NULL;
    }

    //根据告警流水号查找告警库获得对应告警记录在文件中的位置；
    int noffset;

    if((noffset = FindRecordBySyn(pAlarm->uSyn)) != -1)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->SaveHisAlarm(noffset, *pAlarm))
        {
            TRACE(MTS_ALARM,S_ALARM_ADD_HIS_FAIL);
            return ERR_WRITE_HISTORY_ALARM;
        }

        if(ERR_SUCCESS != m_pAlarmUtil->Flush())
        {
            TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
            return ERR_WRITE_HISTORY_ALARM;
        }

        return ERR_SUCCESS;
    }

    return ERR_WRITE_HISTORYALARM;
}

//////////////////////////////////////////////////////////////////////
//功能描述  函数将pAlarm指针所指的uNum条告警记录写到历史告警库的头部
//      pAlarm指向uNum个告警信息结构
//输入参数  无
//输出参数  无
//返回值    执行成功均返回0,执行失败均返回负值
//////////////////////////////////////////////////////////////////////
int CHistoryAlarm::WriteRecord(SHistoryAlarm *pAlarm, UINT4 uNum)
{
    for(int i = 0; i < uNum; i++)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->SaveHisAlarm(i, *(pAlarm + i)))
        {
            TRACE(MTS_ALARM,S_ALARM_ADD_HIS_FAIL);
            return ERR_WRITE_HISTORY_ALARM;
        }
    }

    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    return ERR_SUCCESS;
}

/////////////////////////////////////////////////////////////////
//功能描述  根据告警流水号查找历史告警库删除对应的告警信息
//输入参数  uCsn－要删除告警对应的告警流水号
//输出参数  无
//返回值    函数执行成功返回0,执行错误码
///////////////////////////////////////////////////////////////////
int CHistoryAlarm::DeleteRecord(const UINT4 uSyn)
{
    //查找告警流水号uSyn对应告警在历史告警库中的位置；
    int noffset;
    SHistoryAlarm *pBuf = NULL;

#ifdef _OM_MODIFY
    noffset = FindRecordBySyn(uSyn);
#else
    noffset = FindRecord(uSyn);
#endif
    if(noffset  != -1)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->Delete(noffset))
        {
            return ERR_FAIL;
        }
        m_nRecordNum--;

        if(ERR_SUCCESS != m_pAlarmUtil->Flush())
        {
            TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
            return ERR_WRITE_HISTORY_ALARM;
        }

        return ERR_SUCCESS;
    }

    TRACE(MTS_ALARM,S_ALARM_WRITE_HIS_ERR);
    return ERR_WRITE_HISTORYALARM;
}

///////////////////////////////////////////////////////////
//功能描述  函数删除历史告警库最早的uRecordNum条告警记录
//输入参数  无
//输出参数  无
//返回值    返回0表示删除成功，负值表示删除失败
///////////////////////////////////////////////////////////
int CHistoryAlarm::DeleteRecords(const UINT2 uRecordNum)
{
    if(0 == uRecordNum)
    {
        return ERR_SUCCESS;
    }

    int i = min(uRecordNum, m_nRecordNum) - 1;
    SHistoryAlarm hisAlarm;
    while(i >= 0)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(i, hisAlarm))
        {
            break;
        }

    #ifdef _OM_MODIFY
       if(ERR_SUCCESS == m_pAlarmUtil->Delete(i))
       {
            m_nRecordNum--;
            i--;
       }
    #else
       //非紧急事件告警直接标记为无效
       if(hisAlarm.yAlarmType == AT_EVENT && hisAlarm.yAlarmLevel != AL_EMERGENCY)
       {
           //by ldf 2002.1.21
           if(ERR_SUCCESS == m_pAlarmUtil->Delete(i))
               m_nRecordNum--;
           i--;
           continue;
       }
       //确认的紧急事件告警直接标记为无效
       if(hisAlarm.yAlarmType == AT_EVENT 
           && hisAlarm.yAlarmLevel == AL_EMERGENCY 
           && hisAlarm.szFAckOptr[0] != '\0')
       {
           if(ERR_SUCCESS == m_pAlarmUtil->Delete(i))
               m_nRecordNum--;
           i--;
           continue;
       }
       //是否为故障告警，并且已经恢复和确认，将此记录标记为无效
       if(hisAlarm.yAlarmType == AT_FAULT
           && hisAlarm.yResType != NOT_RECOVER 
           && hisAlarm.szFAckOptr[0] != '\0')
       {
           //by ldf 2002.1.21
           if(ERR_SUCCESS == m_pAlarmUtil->Delete(i))
               m_nRecordNum--;
       }
       i--;
    #endif
    }
    i = 0;
    
    if(ERR_SUCCESS != m_pAlarmUtil->Flush())
    {
        TRACE(MTS_ALARM,S_ALARM_FLUSH_HIS_FAIL);
        return ERR_WRITE_HISTORY_ALARM;
    }

    return ERR_SUCCESS;
}

UINT4 CHistoryAlarm::GetRecordNum()
{
    return m_nRecordNum;
}

/////////////////////////////////////////////////////////////////////
//功能描述  读取历史告警库文件指针当前所指位置后的uNum个历史告警记录
//输入参数  uNum - 需要读入的告警记录数
//输出参数  uNum - 实际读入的告警记录数
//          pData - 返回的记录头指针
//返回值    函数执行成功返回0,执行失败时返回错误码
//////////////////////////////////////////////////////////////////////
int CHistoryAlarm::GetRecord(SHistoryAlarm *pData, UINT4 &uNum)
{
    //读取告警数据记录
    if(0 == uNum || NULL == pData)
    {
        return ERR_SUCCESS;
    }

    //读出的记录数
    int nMaxOffset = min(uNum + m_nReadIndex, m_nRecordNum);
    uNum = 0;
    while(m_nReadIndex < nMaxOffset)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(m_nReadIndex, *(pData + uNum)))
        {
            uNum = 0;
            return ERR_READ_HISTORY_ALARM;
        }
        m_nReadIndex++;
        uNum++;
    }

    return ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////
//功能描述  将文件读写指针根据origin移到第offset个告警记录处
//输入参数  offset－要定位的告警记录偏移
//          origin－初始位置,同fseek,包含以下值:
//                  SEEK_CUR－Current position of file pointer;
//                  SEEK_END－End of file;
//                  SEEK_SET－Beginning of file.
//                  默认情况下为SEEK_SET
//输出参数  无
//返回值    ERR_SUCCESS表示成功,其它为错误码；
///////////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::Seek(long offset, int origin)
{
    //根据offset和origin计算应移动的偏移
    switch(origin)
    {
    //当origin为SEEK_SET时,不考虑offset>m_nRecordNum的情况
    case SEEK_SET:
        m_nReadIndex = offset;
        break;

    //当origin为SEEK_CUR时,计算相对于文件头的偏移量
    case SEEK_CUR:
        //计算当前文件指针所在位置,并调整以保证tmpoffset肯定指向一条告警记录的头位置.
        m_nReadIndex += offset;
    break;

    //当origin为SEEK_END时,为了避免不完整记录对移动指针的影响
    //根据offset计算从文件头开始的偏移值
    case SEEK_END:
        if(offset > m_nRecordNum)
            offset = m_nRecordNum;
        m_nReadIndex = m_nRecordNum - offset;
        break;

    //默认情况下,移动到第0个记录处
    default:
        m_nReadIndex = 0;
        break;
    }

    return 0;
}

/////////////////////////////////////////////////////////
//功能描述  清理历史告警库,删除超过指定条目的历史告警
//输入参数  无
//输出参数  无
//返回值    返回0表示删除成功,非0表示删除失败
/////////////////////////////////////////////////////////
int CHistoryAlarm::Sweep()
{
    //看是否告警库达到了上限
    if(m_nRecordNum > m_nAlarmUpperLimit)
    //删除告警纪录
    {
        DeleteRecords(m_nAlarmDelNum);
    }
    return 0;
}

//by ldf 2004-01-31 OM整改
//////////////////////////////////////////////////////////////////////
//功能描述    根据告警同步号查找告警记录
//输入参数    uSyn - 预追加到历史告警库尾部的告警记录数
//输出参数    无
//返回值    该告警在告警文件中的位置,以告警记录为单位,为-1时表示没有找到
////////////////////////////////////////////////////////////////////////
int CHistoryAlarm::FindRecordBySyn(UINT4 uSyn)
{

    //从文件中读取所有告警记录信息,如果读取错误则返回-1;
    //由于不考虑告警序列号回转的情况,历史告警库中告警记录的流水号是递增排列的,
    //所以采用二分法查找算法找到流水号为uSyn的历史告警记录

    int nLeft   = 0;
    int nRight  = m_nRecordNum - 1;
    int nMid    = (nRight-nLeft) / 2;
    int nRetPos = -1;
    UINT4 nLeftSyn,nMidSyn,nRightSyn;
    SHistoryAlarm hisAlarm;

    while(1)
    {
        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nLeft, hisAlarm))
        {
            break;
        }
        nLeftSyn = hisAlarm.uSyn;

        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nMid, hisAlarm))
        {
            break;
        }
        nMidSyn = hisAlarm.uSyn;

        if(ERR_SUCCESS != m_pAlarmUtil->ReadOneAlarm(nRight, hisAlarm))
        {
            break;
        }
        nRightSyn = hisAlarm.uSyn;

        if(nLeft == nMid)
        {
            if(uSyn == nLeftSyn)
                nRetPos = nLeft;
            else
                if(uSyn == nRightSyn)
                    nRetPos = nRight;
            break;
        }
        if(uSyn < nLeftSyn || uSyn > nRightSyn)
            break;
        if(uSyn == nMidSyn)
        {
            nRetPos = nMid;
            break;
        }
        else
        {
            if(uSyn < nMidSyn)
            {
                nRight = nMid;
            }
            else
            {
                nLeft = nMid;
            }
            //by lx 2003-09-28 SWPD01792
            nMid = nLeft + (nRight-nLeft)/2;
        }
    }

    return nRetPos;
}



