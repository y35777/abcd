#ifndef __CDR_STAT_SVR_H__
#define __CDR_STAT_SVR_H__

#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/mml_para.h"

#include "stat_cfg_info.h"

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

class CCDRStatCfgMgr;
class CCDRStatRstMgr;
class CCDRStatTask;
class CQBMMLReport;

class CCDRStatSvr : public CWorkModule
{
public:
    //构造函数
    CCDRStatSvr();

    //析构函数
    virtual ~CCDRStatSvr();

    //获取指定目录下面的所有话单文件名称
    static int GetBillFiles(const char* szPath, LIST<STRING>& qResult);

    static void SetParaInfoContentList(STRING sAPName, INFO_LIST ContentList);

protected:
    //初始化函数
    int InitInstance();

    //结束工作处理函数
    void ExitInstance();

    //获取本模块类型函数
    MODULE_TYPE GetModuleType();

    //定时器事件处理函数
    void OnTimer(const long nTimerID);

    //命令解析与分发函数
    void ParseMsg(MSG_INTRA* pMsgParsed);

    //处理统计项目查询请求
    void ProcListStatItem(MSG_INTRA* pMsgParsed);

    //处理统计可选域信息查询请求
    void ProcListQueryPara(MSG_INTRA* pMsgParsed);

    //处理统计项目设置请求
    void ProcSetStatItem(MSG_INTRA* pMsgParsed);

    //处理统计数据浏览请求
    void ProcBrowseStatRst(MSG_INTRA* pMsgParsed);

protected:
    //发送报告体中只有简单的消息的MML报告
    void SendSimpleInfoReport(const char* szCmd, int nRetCode,
                              const char* szInfo);

    //发送报告体为一个横向表格的MML报告
    void SendHoriTableReport(CParseResult* pParseResult,
                             LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                             const char* szTableHeadName);

    //发送报告体为一个纵向表格的MML报告
    void SendVertTableReport(CParseResult* pParseResult,
                             LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                             LIST<STRING>& RecTitleList);

    void SendMMLReport(const char* szReport);

    //把字串转换为时间
    int StrToTm(const char* szSrc, tm &tmTar);

    //由年，月获得日的天数
    int GetDay(int nYear, int nMonth);

    //获得和周期相关信息
    int ConvertCycle(int nPeriod,
                     char* szStartTime,
                     char* szEndTime,
                     VECTOR<STRING>& CycleName,
                     VECTOR<time_t>& StartTime,
                     VECTOR<time_t>& EndTime);

    //分析配置信息
    void ParseCfgInfo(char* szCfgInfo,
                      MAP<STRING, ONESTATSUBITEMCFG_LIST>& mapAPNameStatCfg);

    //获得详细配置信息内容
    void GetCfgItem(char* szStatCfg,
                    char* szApName,
                    char* szItemName,
                    char* szSubItemName,
                    char* szExpress);

protected:
    //当前正在处理的话单台的台号
    int               m_nCurClientNo;
    //统计配置文件管理者对象指针
    CCDRStatCfgMgr*   m_pCDRStatCfgMgr;
    //统计结果文件管理者对象指针
    CCDRStatRstMgr*   m_pCDRStatRstMgr;
    //MML报文对象指针
    CQBMMLReport*     m_pMMLReport;
    //统计操作对象指针
    CCDRStatTask*     m_pCDRStatTask;

    char              m_szBeginTime[20];
    //统计起始时间
    time_t            m_tBeginTime;
    //统计截止时间
    time_t            m_tEndTime;
    //上次统计启动时间
    time_t            m_tLastStatTime;
    //定时器ID
    long              m_lTimerID;
    //接点数
    UINT4             m_uAPNum;

    //接入点列表
    LIST<STRING>      m_APNameList;
    //每个接入点对应的可选域信息
    MAP<STRING, INFO_LIST>  m_mapAPToParaField;
    //每个接入点对应的可选域信息
    static MAP<STRING, INFO_LIST>  m_mapAPToParaContent;

    //每个接入点对应的可选域信息查询MML报文的缓冲区
    MAP<STRING, char*>      m_mapAPToParaMem;
    //每个接入点对应的存储统计项目查询MML报文的缓冲区
    MAP<STRING, INFO_LIST>  m_mapAPToStatItemMem;

    STRING m_szInfo;
};
#endif //__CDR_STAT_SVR_H__

