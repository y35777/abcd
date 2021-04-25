/**************************************************************************
 *产品名    ：iGateway Bill V200
 *模块名    ：采集模块(collector)
 *文件名    : colloector.h
 *描述信息  ：本文件是类CCollectorModule的实现
 *版本说明  ：V200R001i00M00B00D1130A
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：王锋
 *创建时间  ：2002-10-26
 *修改记录  ：
 **************************************************************************/

#ifndef __COLLECTOR_MODULE_H__
#define __COLLECTOR_MODULE_H__

#include "../include/frame.h"
#include "../include/toolbox.h"
#include "collect_task_obj.h"
#include "collect_task_obj_factory.h"
#include "collector_file_pool.h"
#include "resource.h"


class CCollectorModule : public CWorkModule
{
public:
    // 构造函数
    CCollectorModule(void);

    // 析构函数
    virtual ~CCollectorModule();

    //采集进程名
    static char g_szColAppName[MAX_PATH];

protected:

    // 命令解析与分发函数
    virtual void ParseMsg (MSG_INTRA* pMsgParsed);

    // 初始化函数
    virtual int InitInstance(void);

    // 结束工作处理函数
    virtual void ExitInstance(void);

    // 获取本模块类型函数
    virtual MODULE_TYPE GetModuleType(void);

    virtual void OnTimer(const long nTimerID);

    void Timer5Sec();

    //查询指定采集任务的信息
    void ProcListColInfo(MSG_INTRA* pMsgParsed);

    //设置采集任务的状态
    void ProcSetColStatus(MSG_INTRA* pMsgParsed);

    //设置告警门限
    void ProcSetALarmLimit(MSG_INTRA* pMsgParsed);

    //查询指定模块的计费信息
    void ProcListModuleInfo(MSG_INTRA* pMsgParsed);

    //查询采集计划
    void ProcListColSch(MSG_INTRA* pMsgParsed);

    //设置采集计划
    void ProcSetColSch(MSG_INTRA* pMsgParsed);

    //查询单次采集计划
    void ProcListOnceColSch(MSG_INTRA* pMsgParsed);

    //设置单次采集计划
    void ProcSetOnceColSch(MSG_INTRA* pMsgParsed);

    //手动采集文件
    void ProcGetFile(MSG_INTRA* pMsgParsed);

    void SendMMLReport(const char* szReport,
                       int nAppType,
                       int nClientNo);

    //added by chenliangwei
    void OnResumeOver(SCollectConvertResult* pResult);

    void OnResponse(int nConvertResult);

    void SendFileToConvert();

    void Timer1Sec();

    int LoadStatus();

    int SaveStatus();

    //added end


    VECTOR<CCollectTaskObj*>* m_pTaskList;

    long m_lTimerID5Sec;

    enum CO_MD_STATUS
    {
        CM_FAILURE           = -1,
        CM_DEACTIVE          = 0,
        CM_ACTIVE            = 1,
        CM_AUXUPGRADE        = 2
    };

    CO_MD_STATUS  m_nStatus;

    BOOL          m_bDiskFull;

    //added by chenliangwei
    long m_lTimerID1Sec;

    enum CO_CVT_STATUS
    {
        CO_CVT_STATUS_INIT       = 0,
        CO_CVT_STATUS_RESUME     = 1,
        CO_CVT_STATUS_WAIT       = 2,
        CO_CVT_STATUS_CONVERT    = 3
    };

    CO_CVT_STATUS  m_nConvertStatus;

    int    m_nFileNum;

    BOOL   m_bFileIsSended;

    BOOL   m_bFileNeedReSend;      //文件是否需要重新发送
    int    m_nFileReSendTime;      //重送时间计数
    int    m_nFileReSendInterval;  //重送时间间隔

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;


    static CCollectorFilePool* g_pFilePool;
    friend CCollectorFilePool* GetFilePool();
    //added end

};

#endif //__COLLECTOR_MODULE_H__


