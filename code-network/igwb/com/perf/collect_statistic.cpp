// collect_statistic.cpp: implementation of the CCollectStatistic class.
//
//////////////////////////////////////////////////////////////////////
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "collect_statistic.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCollectStatistic::CCollectStatistic():m_nPageLimit(64)
{
}

CCollectStatistic::~CCollectStatistic()
{

}

int CCollectStatistic::SetFrontsaveRoot(const char *szRoot)
{
    m_strFrontsaveRoot = szRoot;
    return 0;
}

int CCollectStatistic::Init(const char *szRoot)
{
    m_strFrontsaveRoot = szRoot;
    return 0;
}

int CCollectStatistic::ProcStatisticCollectFiles(CParseResult *pParse)
{
    m_ColPointList.clear();
    //第一个是可选参数
    m_strColName = pParse->GetStringVal(1);
    //第2、3个是必选参数
    m_strStartDate = pParse->GetStringVal(2);
    if(!IsDigitValue(m_strStartDate.c_str()))
    {
        m_strError = "Failed to get the second parameter";
        //发送失败的报文
        SendCommonReport(pParse,m_strError.c_str(),-1);
        return -1;
    }
    m_strEndDate = pParse->GetStringVal(3);
    if(!IsDigitValue(m_strEndDate.c_str()))
    {
        m_strError = "Failed to get the third parameter";
        //发送失败的报文
        SendCommonReport(pParse,m_strError.c_str(),-1);
        return -1;
    }
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;
    
    //打开文件目录,打开szPath目录
    pDir = ACE_OS::opendir(m_strFrontsaveRoot.c_str());
    if(pDir == NULL)
    {
        m_strError = S_BACKUP_OPEN_DIR_FAIL;
        m_strError += m_strFrontsaveRoot.c_str();
        ErrorOutput();
        //发送失败的报文
        SendCommonReport(pParse,m_strError.c_str(),-1);
        return ERR_FAIL;
    }
    char szSrcFilePath[MAX_PATH];
    //搜索接入点
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        if(strcmp(pDirent->d_name,".") == 0
          || strcmp(pDirent->d_name,"..") == 0)
          continue;
        //形成源文件路径
        SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",
                 m_strFrontsaveRoot.c_str(),
                 pDirent->d_name);
        szSrcFilePath[MAX_PATH - 1] = 0;
        
        ACE_OS::stat(szSrcFilePath, &sDirStat);
        if(!S_ISDIR(sDirStat.st_mode))
        {
            continue;
        }
        else if(StringiCmp(m_strColName.c_str(),pDirent->d_name) 
               || m_strColName.compare("") == 0)
        {
            m_ColPointList.push_back(pDirent->d_name);
        }
    }
    if(m_ColPointList.empty())
    {
        //返回查询无结果的报文
        SendCommonReport(pParse,S_MML_QUERY_NO_RESULT,0);
        return ERR_SUCCESS;
    }
    //根据查询接入点的个数走不同的业务逻辑
    if(m_ColPointList.size() == 1 && m_strColName.compare("") != 0)
    {
        //针对查询单个接入点的报文
        QuerySpecialPoint(pParse);
    }
    else
    {
         //针对查询所有接入点的报文
        QueryAllPoint(pParse);
    }
    return 0;
}

void CCollectStatistic::ErrorOutput()
{
    char szTmpBuf[500];
    sprintf(szTmpBuf, "\nErrno: %d (%s)", errno, strerror(errno));
    m_strError += szTmpBuf;
    MSGOUT(MTS_PERF, MSGOUT_LEVEL_IMPORTANT, m_strError.c_str());
    TRACE(MTS_PERF,m_strError.c_str());
}

int CCollectStatistic::QuerySpecialPoint(CParseResult *pParse)
{
    int nFileSize;
    tm tmFileTime;
    CMMLReport report("PERF");
    char szTmp[64];
    FILE *pFile = NULL;
    LIST<STRING> TitleList;
    LIST<STRING> ContentList;
    char szSrcDirPath[MAX_PATH];
    char szSrcDateDirPath[MAX_PATH];
    char szSrcFilePath[MAX_PATH];
    /**************************************/
    TitleList.push_back(S_PERF_FILE_NAME);
    TitleList.push_back(S_PERF_CRE_DATE);
    TitleList.push_back(S_PERF_FILE_SIZE);
    /**************************************/
    SNPRINTF(szSrcDirPath,MAX_PATH,"%s/%s",
             m_strFrontsaveRoot.c_str(),
             m_ColPointList.front().c_str());
    szSrcDirPath[MAX_PATH - 1] = 0;

    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;
    ACE_DIR *pDateDir = NULL;
    dirent *pDateDirent = NULL;
    //打开文件目录,打开szPath目录
    pDir = ACE_OS::opendir(szSrcDirPath);
    if(pDir == NULL)
    {
        m_strError = S_BACKUP_OPEN_DIR_FAIL;
        m_strError += szSrcDirPath;
        ErrorOutput();
        //发送失败的报文
        SendCommonReport(pParse,m_strError.c_str(),-1);
        return ERR_FAIL;
    }
    //遍历指定接入点目录
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        m_strCurrentDate = pDirent->d_name;   
        if(strcmp(pDirent->d_name,".") == 0
            || strcmp(pDirent->d_name,"..") == 0
            || !IsDigitValue(m_strCurrentDate.c_str()))
            continue;
        //形成源文件路径/frontsave/3g/20030411
        SNPRINTF(szSrcDateDirPath,MAX_PATH,"%s/%s",
            szSrcDirPath,pDirent->d_name);
        szSrcDateDirPath[MAX_PATH - 1] = 0;
        
        ACE_OS::stat(szSrcDateDirPath, &sDirStat);
        if(!S_ISDIR(sDirStat.st_mode))
            continue;
        
        //到这里就一定是数字目录名了
        if(m_strStartDate.compare(m_strCurrentDate) <= 0 
            && m_strEndDate.compare(m_strCurrentDate) >= 0)
        {
            pDateDir = ACE_OS::opendir(szSrcDateDirPath);
            if(pDir == NULL)
            {
                m_strError = S_BACKUP_OPEN_DIR_FAIL;
                m_strError += szSrcDirPath;
                ErrorOutput();
                //发送失败的报文
                SendCommonReport(pParse,m_strError.c_str(),-1);
                return ERR_FAIL;
            }
            pDateDirent = ACE_OS::readdir(pDateDir);
            while(pDateDirent != NULL)
            {
                if(strcmp(pDateDirent->d_name,".") == 0
                    || strcmp(pDateDirent->d_name,"..") == 0)
                {
                    pDateDirent = ACE_OS::readdir(pDateDir);
                    continue;
                }
                //形成源文件路径/frontsave/3g/20030411
                SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",
                    szSrcDateDirPath,pDateDirent->d_name);
                szSrcFilePath[MAX_PATH - 1] = 0;        
                ACE_OS::stat(szSrcFilePath, &sDirStat);
                if(S_ISDIR(sDirStat.st_mode))
                {
                    pDateDirent = ACE_OS::readdir(pDateDir);
                    continue;
                }
                pFile = fopen(szSrcFilePath,"rb");
                if(!pFile)
                {
                    m_strError = szSrcFilePath;
                    ErrorOutput();
                    pDateDirent = ACE_OS::readdir(pDateDir);
                    continue;
                }
                fseek(pFile,0,SEEK_END);
                /*****************特化代码*********************/
                nFileSize = ftell(pFile) / 1024;
                fclose(pFile);
                pFile = NULL;
                if(ContentList.size() >= m_nPageLimit * 3)
                {
                    //防止上一次搜索上一个目录时有残留，生成中间报文
                    report.MakeReportHead();
                    report.MakeCmdDisplay(pParse->GetCmdLine(),
                        0,0,"TASKCMD");
                    report.AddHoriTable(TitleList,ContentList,
                        m_strColName.c_str());
                    report.AddString(STR_NEWLINE);
                    report.AddString(S_ALARM_TOBECONITUE);
                    report.AddString(STR_NEWLINE);
                    report.MakeReportTail();
                    SendMMLReport(report.GetReport(),pParse);
                    ContentList.clear();
                }
                ContentList.push_back(pDateDirent->d_name);
                tmFileTime = *localtime(&(sDirStat.st_ctime));
                strftime(szTmp,64,"%Y-%m-%d",&tmFileTime);
                ContentList.push_back(szTmp);
                SNPRINTF(szTmp,64,"%d",nFileSize);
                ContentList.push_back(szTmp);
                if(ContentList.size() >= m_nPageLimit * 3)
                {
                    //看看能否找到下一个文件
                    while((pDateDirent = ACE_OS::readdir(pDateDir)) != NULL)
                    {
                        SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",
                                 szSrcDateDirPath,pDateDirent->d_name);
                        szSrcFilePath[MAX_PATH - 1] = 0;
                        ACE_OS::stat(szSrcFilePath, &sDirStat);
                        if(S_ISDIR(sDirStat.st_mode))
                            continue;
                        else
                            break;
                    }
                    if(pDateDirent == NULL)
                        break;   
                    //生成中间报文
                    report.MakeReportHead();
                    report.MakeCmdDisplay(pParse->GetCmdLine(),
                             0,0,"TASKCMD");
                    report.AddHoriTable(TitleList,ContentList,
                             m_strColName.c_str());
                    report.AddString(STR_NEWLINE);
                    report.AddString(S_ALARM_TOBECONITUE);
                    report.AddString(STR_NEWLINE);
                    report.MakeReportTail();
                    SendMMLReport(report.GetReport(),pParse);
                    ContentList.clear();
                }
                else
                    pDateDirent = ACE_OS::readdir(pDateDir);
               /****************************************/
            }//while
            ACE_OS::closedir(pDateDir);
        }//if
        
    }//while
    ACE_OS::closedir(pDir);
    //生成结尾的报文
    report.MakeReportHead();
    report.MakeCmdDisplay(pParse->GetCmdLine(),
             0,0,"TASKCMD");
    report.AddHoriTable(TitleList,
                          ContentList,
                          m_strColName.c_str());
    report.MakeReportTail();
    SendMMLReport(report.GetReport(),pParse);
    ContentList.clear();
    return ERR_SUCCESS;
}

int CCollectStatistic::QueryAllPoint(CParseResult *pParse)
{
    int nFileSize = 0;
    time_t tFileTime = 0;
    int nFileCnt = 0;
    CMMLReport report("PERF");
    char szTmp[64];
    FILE *pFile = NULL;
    LIST<STRING> TitleList;
    LIST<STRING> ContentList;
    char szSrcPointPath[MAX_PATH];
    char szSrcDateDirPath[MAX_PATH];
    char szSrcFilePath[MAX_PATH];
    /***********************************************/
    STRING strDate;
    TitleList.push_back(S_PERF_COL_POINT);
    TitleList.push_back(S_PERF_COL_DATE);
    TitleList.push_back(S_PERF_COL_FILE_NUM);
    TitleList.push_back(S_PERF_ALL_FILE_SIZE);
    /***********************************************/
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;
    ACE_DIR *pDateDir = NULL;
    dirent *pDateDirent = NULL;
    for(; m_ColPointList.size(); m_ColPointList.pop_front())
    {
        m_strColName = m_ColPointList.front();
        //形成接入点路径
        SNPRINTF(szSrcPointPath,MAX_PATH,"%s/%s",
                 m_strFrontsaveRoot.c_str(),
                 m_strColName.c_str());
        szSrcPointPath[MAX_PATH - 1] = 0;
        //打开文件目录,打开szPath目录
        pDir = ACE_OS::opendir(szSrcPointPath);
        if(pDir == NULL)
        {
            m_strError = S_BACKUP_OPEN_DIR_FAIL;
            m_strError += szSrcPointPath;
            ErrorOutput();
            //发送失败的报文
            SendCommonReport(pParse,
                             m_strError.c_str(),
                             -1);
            return ERR_FAIL;
        }
        pDirent = ACE_OS::readdir(pDir);
        while(pDirent != NULL)
        {
            m_strCurrentDate = pDirent->d_name;
            if(strcmp(pDirent->d_name,".") == 0
                || strcmp(pDirent->d_name,"..") == 0
                || !IsDigitValue(m_strCurrentDate.c_str()))
            {
                pDirent = ACE_OS::readdir(pDir);
                continue;
            }
            //形成源文件路径/frontsave/3g/20030411
            SNPRINTF(szSrcDateDirPath,MAX_PATH,"%s/%s",
                     szSrcPointPath,pDirent->d_name);
            szSrcDateDirPath[MAX_PATH - 1] = 0;
            
            ACE_OS::stat(szSrcDateDirPath, &sDirStat);
            if(!S_ISDIR(sDirStat.st_mode))
            {
                pDirent = ACE_OS::readdir(pDir);
                continue;
            }
            //到这里就一定是数字目录名了
            if(m_strStartDate.compare(m_strCurrentDate) <= 0 
                && m_strEndDate.compare(m_strCurrentDate) >= 0)
            {
                pDateDir = ACE_OS::opendir(szSrcDateDirPath);
                if(pDir == NULL)
                {
                    m_strError = S_BACKUP_OPEN_DIR_FAIL;
                    m_strError += szSrcDateDirPath;
                    ErrorOutput();
                    //发送失败的报文
                    SendCommonReport(pParse,m_strError.c_str(),-1);
                    return ERR_FAIL;
                }
                //遍历日期目录累加所有文件的长度
                nFileSize = 0;
                nFileCnt = 0;
                while((pDateDirent = ACE_OS::readdir(pDateDir)) != NULL)
                {
                    if(strcmp(pDateDirent->d_name,".") == 0
                        || strcmp(pDateDirent->d_name,"..") == 0)
                        continue;
                    //形成源文件路径/frontsave/3g/20030411
                    SNPRINTF(szSrcFilePath,MAX_PATH,"%s/%s",
                             szSrcDateDirPath,pDateDirent->d_name);
                    szSrcFilePath[MAX_PATH - 1] = 0;
                    
                    ACE_OS::stat(szSrcFilePath, &sDirStat);
                    if(S_ISDIR(sDirStat.st_mode))
                        continue;
                    pFile = fopen(szSrcFilePath,"rb");
                    if(!pFile)
                    {
                        m_strError = szSrcFilePath;
                        ErrorOutput();
                        continue;
                    }
                    fseek(pFile,0,SEEK_END);
                    /****************************/
                    nFileSize += (ftell(pFile) / 1024);
                    fclose(pFile);
                    pFile = NULL;
                    nFileCnt ++;
                    /****************************/
                }//while
                ACE_OS::closedir(pDateDir);
                /************************************************/
                //如果Content不为空表示有上一次查询的结果必须先送出
                if(ContentList.size() >= m_nPageLimit * 4)
                {
                    report.MakeReportHead();
                    report.MakeCmdDisplay(pParse->GetCmdLine(),
                        0,0,"TASKCMD");
                    report.AddHoriTable(TitleList,ContentList,
                        S_PERF_COL_STATISTIC);
                    report.AddString(STR_NEWLINE);
                    report.AddString(S_ALARM_TOBECONITUE);
                    report.AddString(STR_NEWLINE);
                    report.MakeReportTail();
                    SendMMLReport(report.GetReport(),pParse);
                    ContentList.clear();
                }
                //然后生成本次报文
                ContentList.push_back(m_strColName);
                strcpy(szTmp,m_strCurrentDate.substr(0,4).c_str());
                strcat(szTmp,"-");
                strcat(szTmp,m_strCurrentDate.substr(4,2).c_str());
                strcat(szTmp,"-");
                strcat(szTmp,m_strCurrentDate.substr(6,2).c_str());
                ContentList.push_back(szTmp);
                SNPRINTF(szTmp,64,"%d",nFileCnt);
                ContentList.push_back(szTmp);
                SNPRINTF(szTmp,64,"%d",nFileSize);
                ContentList.push_back(szTmp);
                /*************************************************/
                if(ContentList.size() >= m_nPageLimit * 4)
                {
                    //需要找到下一个符合条件的目录
                    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
                    {
                        m_strCurrentDate = pDirent->d_name;
                        if(!IsDigitValue(m_strCurrentDate.c_str()))
                            continue;
                        if(m_strStartDate.compare(m_strCurrentDate) > 0 
                        || m_strEndDate.compare(m_strCurrentDate) < 0)
                            continue;
                        else
                            break;
                    }
                    if(pDirent == NULL) //没有找到
                        break;
                    //生成中间报文
                    report.MakeReportHead();
                    report.MakeCmdDisplay(pParse->GetCmdLine(),
                                            0,0,"TASKCMD");
                    report.AddHoriTable(TitleList,ContentList,
                                          S_PERF_COL_STATISTIC);
                    report.AddString(STR_NEWLINE);
                    report.AddString(S_ALARM_TOBECONITUE);
                    report.AddString(STR_NEWLINE);
                    report.MakeReportTail();
                    SendMMLReport(report.GetReport(),pParse);
                    ContentList.clear();
                }
                else
                    pDirent = ACE_OS::readdir(pDir);
            }//if
            else
                pDirent = ACE_OS::readdir(pDir);
        }//while
        ACE_OS::closedir(pDir);
    }//for
    //生成结尾的报文
    report.MakeReportHead();
    report.MakeCmdDisplay(pParse->GetCmdLine(),
        0,0,"TASKCMD");
    report.AddHoriTable(TitleList,
        ContentList,
        S_PERF_COL_STATISTIC);
    report.MakeReportTail();
    SendMMLReport(report.GetReport(),pParse);
    ContentList.clear();
    return ERR_SUCCESS;
}

void CCollectStatistic::SendMMLReport(const char *szReport,CParseResult *pParse)
{
    int nStrLen = strlen(szReport);
    
    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_PERF;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_NORM;
    pMsg->ClientNo = pParse->GetWorkStationNo();
    
    memcpy(pMsg->Data, szReport, nStrLen + 1);
    
    CMsgProcessor::SendMsg(pMsg);
}

void CCollectStatistic::SendCommonReport(CParseResult *pParse, const char *szContent, int nRes)
{
    CMMLReport MMLReport("PERF");
    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay(pParse->GetCmdLine(),0,nRes,"TASKCMD");
    MMLReport.AddString(szContent);
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportTail();
    const char* szReport = MMLReport.GetReport();
    SendMMLReport(szReport,pParse);
}
