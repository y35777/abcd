#include "../include/toolbox.h"
#include "alarm_bin_util.h"
#include "mml_alarm_handler.h"

CAlarmBinUtil::CAlarmBinUtil()
{
    m_pAlarmFile  = NULL;
    m_strFileName = "";
}

CAlarmBinUtil::~CAlarmBinUtil()
{
    CloseFile();
}

//打开历史告警文档，文件不存在则生成指定文件
int CAlarmBinUtil::OpenFile(const char * const szFilePath)
{
    CloseFile();
    
    if(szFilePath != NULL)
    {
        m_strFileName = szFilePath;

        //以读写方式打开告警文件
        m_pAlarmFile = fopen(szFilePath,"rb+");
    
        if(m_pAlarmFile != NULL)    //文件存在
        {
            return ERR_SUCCESS;
        }
        else//文件不存在时创建告警文件
        {
            m_pAlarmFile = fopen(szFilePath,"wb+");          
            if(NULL != m_pAlarmFile)
            {
                fwrite("VER:", sizeof(char), 4, m_pAlarmFile);//填写"VER:"
                UINT4 uTmp = LATEST_ALARM_HIS_VERSION;
                fwrite(&uTmp, sizeof(UINT4), 1, m_pAlarmFile);//填写版本号 
                return ERR_SUCCESS;
            }
             
        }
    }

    return ERR_OPEN_HISTORY_ALARM;
}

//关闭文件
int CAlarmBinUtil::CloseFile(void)
{
    if(NULL != m_pAlarmFile)
    {
        fclose(m_pAlarmFile);
        m_pAlarmFile = NULL;
    }
    
    return ERR_SUCCESS;
}

//获得剩余的告警数
int CAlarmBinUtil::GetCount(void)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }
    
    if(NULL == m_pAlarmFile)
    {
        return 0;
    }
    
    //当文件打开成功时,获得当前告警数据库文件中告警记录数
    fseek(m_pAlarmFile, 0L, SEEK_END);

/*#ifdef _OM_MODIFY
    int nCount = (ftell(m_pAlarmFile) - SIZEOF_ALARMHEAD*3) / SIZEOF_HISTORYALARM;
#else
    int nCount = (ftell(m_pAlarmFile) - SIZEOF_ALARMHEAD) / SIZEOF_HISTORYALARM;
#endif
*/
    int nCount = (ftell(m_pAlarmFile) - SIZEOF_ALARMHEAD*5) / SIZEOF_HISTORYALARM;    

    return nCount;
}

//保存文件
int CAlarmBinUtil::Flush(void)
{
    //写文件
    if(NULL == m_pAlarmFile)
    {
        return -1;
    }
    
    if (fflush(m_pAlarmFile) != 0)
    {
        return -1;
    }

    return 0;
}

//删除某条告警记录
int CAlarmBinUtil::Delete(int nIndex)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }

    int nPos      = ALARM_OFFSET(nIndex);
    int nFileSize = FileSize(m_pAlarmFile);
    int nCount    = (nFileSize - nPos) / (SIZEOF_HISTORYALARM) - 1;
    char szBuf[SIZEOF_HISTORYALARM];
    int nRet = 0;

    for(int i = 0; i < nCount; i++)
    {
        nRet = fseek(m_pAlarmFile, nPos + SIZEOF_HISTORYALARM, SEEK_SET);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
        nRet = fread(szBuf, SIZEOF_HISTORYALARM, 1, m_pAlarmFile);
        if(nRet != 1)
        {
            CloseFile();
            return ERR_FAIL;
        }

        nRet = fseek(m_pAlarmFile, nPos, SEEK_SET);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
        nRet = fwrite(szBuf, SIZEOF_HISTORYALARM, 1, m_pAlarmFile);
        if(nRet != 1)
        {
            CloseFile();
            return ERR_FAIL;
        }
        
#ifdef _OM_MODIFY
        //by ldf 2004-01-31 这里是否应该增加一句
		nPos +=SIZEOF_HISTORYALARM;
#endif

    }

    return ERR_SUCCESS;
}

//获得文件头记录
int CAlarmBinUtil::ReadHeadElement(UINT4& uCSN)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*2, SEEK_SET);    
    //读取告警流水号信息,若读取错误则返回失败
    int nRes = fread(&uCSN,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if( nRes != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

int CAlarmBinUtil::ReadHeadElementSyn(UINT4& uSYN)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*3, SEEK_SET);    
    //读取告警同步号信息,若读取错误则返回失败
    int nRes = fread(&uSYN,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if( nRes != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
int CAlarmBinUtil::ReadHeadElementMMLBaseID(UINT4& uMMLBaseID)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*4, SEEK_SET);    
    //读取告警流水号信息,若读取错误则返回失败
    int nRes = fread(&uMMLBaseID,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if( nRes != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

//添加/写文件头记录
int CAlarmBinUtil::WriteHeadElement(UINT4 uCSN)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*2, SEEK_SET);    
    //读取告警流水号信息,若读取错误则返回失败
    int nRes = fwrite(&uCSN,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if(nRes != 1 || Flush() != 0)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

int CAlarmBinUtil::WriteHeadElementSyn(UINT4 uSYN)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*3, SEEK_SET);    
    //读取告警流水号信息,若读取错误则返回失败
    int nRes = fwrite(&uSYN,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if(nRes != 1 || Flush() != 0)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
int CAlarmBinUtil::WriteHeadElementMMLBaseID(UINT4 uBaseID)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*4, SEEK_SET);    
    //读取告警流水号信息,若读取错误则返回失败
    int nRes = fwrite(&uBaseID,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if(nRes != 1 || Flush() != 0)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
//写一条日志信息
int CAlarmBinUtil::SaveHisAlarm(int            nIndex,
                                SHistoryAlarm& hisAlarm)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }

    int nPos      = ALARM_OFFSET(nIndex);
    int nFileSize = FileSize(m_pAlarmFile);
    int nRet      = 0;
    
#ifdef _OM_MODIFY
    if(nPos  > nFileSize)
    {
            CloseFile();
            return ERR_FAIL;
    }
#else
    if(nPos >= nFileSize)
    {
        nRet = fseek(m_pAlarmFile, 0, SEEK_END);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
    }
#endif
    else
    {
        nRet = fseek(m_pAlarmFile, nPos, SEEK_SET);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
    }

    nRet = fwrite(&hisAlarm, SIZEOF_HISTORYALARM, 1, m_pAlarmFile);
    if(nRet != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

//读一条告警信息
int CAlarmBinUtil::ReadOneAlarm(int            nIndex,
                                SHistoryAlarm& hisAlarm)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }

    int nPos      = ALARM_OFFSET(nIndex);
    int nFileSize = FileSize(m_pAlarmFile);
    int nRet      = 0;
    if(nPos >= nFileSize)
    {
        return ERR_FAIL;
    }
    else
    {
        nRet = fseek(m_pAlarmFile, nPos, SEEK_SET);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
    }

    nRet = fread(&hisAlarm, SIZEOF_HISTORYALARM, 1, m_pAlarmFile);
    if(nRet != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

