#ifndef __BILLQBSERVER_H__
#define __BILLQBSERVER_H__

#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/qb_report.h"
#include "../utility/mml_para.h"
#include "qb_task.h"

#define MAX_REPORT_SIZE (56 * 1024)

//以下结构用于描述一个接入点的相关信息
struct SAPInfo
{
    char szAPName[255];
    int  nAPType;
    int  nAPID;
    BOOL bMustMerge;
    BOOL bFinalSPFileFmt;     //最终话单是否是特殊的文件格式
};


//Added by XDB on 2002.11.11
//文件状态结构
struct SFILE_STATE
{
    STRING sFileName;
    int    nType;
    long   nSize;
    time_t mtime;
    time_t ctime;
};


class CBillQBServer : public CWorkModule
{
public:
    CBillQBServer();   //构造函数
    virtual ~CBillQBServer();  //析构函数

protected:
    int InitInstance();                     //初始化函数
    void ExitInstance();                    //结束工作处理函数
    MODULE_TYPE GetModuleType();            //获取本模块类型函数
    void OnTimer(const long nTimerID);      //定时器事件处理函数
    void ParseMsg(MSG_INTRA* pMsgParsed);   //命令解析与分发函数

    //请求接入点信息处理入口函数
    void ReqAccessPointProc(MSG_INTRA* pMsgParsed);
    //请求话单类型处理入口函数
    void ReqBillTypeProc(MSG_INTRA* pMsgParsed);
    //请求通道信息处理入口函数
    void ReqChannelProc(MSG_INTRA* pMsgParsed);
    //请求原始话单日期目录处理入口函数
    void ReqOriginalDateProc(MSG_INTRA* pMsgParsed);
    //请求最终话单日期目录处理入口函数
    void ReqFinalDateProc(MSG_INTRA* pMsgParsed);
    //请求话单文件列表处理入口函数
    void ReqBillFilesProc(MSG_INTRA* pMsgParsed);
    //请求话单格式信息处理入口函数
    void ReqBillFormatProc(MSG_INTRA* pMsgParsed);
    //请求查询条件处理入口函数
    void ReqQueryCondProc(MSG_INTRA* pMsgParsed);
    //请求话单浏览处理入口函数
    void ReqBrowseProc(MSG_INTRA* pMsgParsed);
    //请求结束话单浏览处理入口函数
    void ReqQueryProc(MSG_INTRA* pMsgParsed);
    //请求任务进度入口函数
    void ReqTaskProgressProc(MSG_INTRA* pMsgParsed);
    //请求数据页入口函数
    void ReqGetPageDataProc(MSG_INTRA* pMsgParsed);
    //请求结束任务处理入口函数
    void ReqStopTaskProc(MSG_INTRA* pMsgParsed);
    //请求关闭窗口处理入口函数
    void ReqCloseTaskProc(MSG_INTRA* pMsgParsed);
    //请求窗口握手处理函数
    void ReqWndShakeHandsProc(MSG_INTRA* pMsgParsed);

    //Added by XDB on 2002.11.11
    //增加对采集话单的浏览处理
    void ReqFileRoot(MSG_INTRA* pMsgParsed);

    void ReqBillFiles(MSG_INTRA* pMsgParsed);

    int GetFiles(const char* szPath, LIST<SFILE_STATE>& qResult);
    //End of Add by XDB


protected:
    //销毁指定的查询浏览任务
    void DestroyTask(CBillQBTask* pTask);
    //停止指定的查询浏览任务
    void StopTask(CBillQBTask* pTask);
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
    //发送没有话单内容的MML报告
    void SendEmptyBillDataReport(CParseResult* pParseResult, CBillQBTask* pTask,
                            const char* szNextPageExist,
                            const char* szPageNo, const char* szPercent);

    //将MML报告转换成MSG_INTRA的形式发送出去
    void SendMMLReport(const char* szReport);
    int GetAPType(const char* szAPName);       //得到接入点名对应的接入点的类型
    int GetAPID(const char* szAPName);         //得到接入点的接入点号
    int GetAPMustMerge(const char* szAPName);  //得到接入点名是否需要合并
    int GetAPFinalSPFileFmt(const char* szAPName);  //得到接入点的后存盘是否是特殊格式
    //对话单查询条件进行特殊处理
    void TransQueryCond(const char* szCond, const char* szBeginDate,
                        const char* szEndDate, char* szExCond);

protected:
    //问题单 SWPD05145 YanYong 2004-05-25
	void SendSimpleInfoReport(const char* szCmd, int nRetCode, const char* szInfo, int clientNo);
	void SendMMLReport(const char* szReport, int clientNo);
    //</SWPD05145>
    char  m_szOrigBasePath[MAX_PATH];   //原始话单根目录
    char  m_szFinalBasePath[MAX_PATH];  //最终话单根目录
    char  m_szQBTmpResultPath[MAX_PATH];//查询浏览结果集目录
    
    LIST<CBillQBTask* > m_qBrowseTask;  //浏览任务对象列表
    LIST<CBillQBTask* > m_qQueryTask;   //查询任务对象列表
    CIGWB*          m_pFormatObj;       //格式库对象指针
    IQueryBrowse*   m_pIQueryBrowse;    //格式库查询浏览接口对象指针
    UINT4           m_uTimeOutLimit;    //查询浏览任务超时期限
    long            m_lBaseTimer;       //基准时钟,时钟间隔为1秒
    CQBMMLReport*   m_pMMLReport;       //负责产生MML报告的对象
    int             m_nCurClientNo;     //当前正在处理的话单台的台号
    LIST<SAPInfo* > m_APInfoList;       //所有接入点的相关信息列表

    UINT4           m_uPageSize;        //页大小
    //增加采集话单的根路径
    char m_szCollectRootPath[MAX_PATH];
    //是否显示错误话单
    BOOL            m_bDisplayErrBill;
    //问题单 SWPD05145 YanYong 2004-05-25
    LIST<STRING>    m_QueryFileList;    //查询多个文件或者日期时的列表
    MAP<int, LIST<STRING> > m_ClientFileListMap; //每个客户端对应的链表
    //</SWPD05145>
};

#endif
