#ifndef __BILLQBTASK_H__
#define __BILLQBTASK_H__

#include "../include/precom.h"
#include "../include/frame.h"
#include "../include/format_out.h"
#include "file_group.h"
#include "qb_result.h"

class CBillQBTask
{
public:
    //构造函数
    CBillQBTask(UINT2 uDeskNo, int nWndNo, UINT4 uPageNo,
                char* szBillFormat, UINT4 uBillType, char* szResultPath);
    virtual ~CBillQBTask ();     //析构函数

    static void SetPageSize(UINT4 uPageSize);
    BOOL IsRunning();            //线程是否正在运行
    const char* GetErrInfo();    //得到本线程的错误信息

    //初始化函数，该函数针对浏览任务对象
    BOOL Init(char* szPathFile, UINT4 uAPType, int nAPID);
    //初始化函数，该函数针对查询任务对象
    BOOL Init(char* szPath, char* szStartRange, char* szEndRange,
              UINT4 uFlg, char* szQueryCond, UINT4 uAPType, int nAPID);
    
    //问题单 SWPD05145 YanYong 2004-05-25 添加该方法
    BOOL Init(char* szPath, LIST<STRING> & queryList,
              UINT4 uFlg, char* szQueryCond, UINT4 uAPType, int nAPID);
    
    BOOL Run();      //准备话单数据页线程启动函数
    //获得话单表格的标题的列表
    void GetBillTitleList(LIST<STRING>& TitleList);
    //获取指定页的话单记录数据
    int GetPageData(int nPageNo, LIST<STRING>& qResult);

    BOOL IsValidPage(int nPageNo);    //判断一个请求页号是否合法
    //该对象是否匹配指定的查询浏览任务
    BOOL IsMatched(UINT2 uDeskNo, int nWndNo);
    int WaitThreadExit();       //等待准备话单线程结束函数

    void Stop();                //设置停止标志为停止状态函数
    void IncTimeOutFlg();       //超时标志增加一函数
    void RsetTimeOutFlg();      //超时标志置零函数
    BOOL IsTimeOut(UINT4 uTimeOutLimit);  //是否超时函数
    void SetMustMerge(BOOL bMustMerge);
    void SetFinalSPFileFmt(BOOL bFinalSPFileFmt);
    //获得查询或浏览的进度
    BOOL GetProcess(UINT4& nRecordCount, int& nPercent, int& nCurPage);
    void SetDisplayErrBill(BOOL bDisplayErrBill);
protected:
    //准备话单数据页线程入口函数
    static void ThreadProc(CBillQBTask *pBillQBTask);
    //准备话单数据页线程函数
    void PrepareBillPages();

protected:
    UINT2               m_uDeskNo;            //话单业务台号
    int                 m_nWndNo;             //查询浏览任务窗口号
    UINT4               m_uBillType;          //话单文件类型
    UINT4               m_uAPType;

    CFileGroup*         m_pFileGroup;         //话单文件组对象指针
    CIGWB*              m_pFormatObj;         //格式库对象指针
    IQueryBrowse*       m_pIQueryBrowse;      //格式库对象的查询浏览接口指针
    CFormat*            m_pFormat;            //浏览格式对象指针
    char                m_szFormatName[100];  //浏览格式的名字
    UINT4               m_uTimeOut;           //任务未收到消息的时间，初始化为0

    static UINT4        m_uPageSize;     //一页包含的话单记录数据条数，默认为50

    ACE_Thread_Manager  m_tThreadMgr;    //线程管理成员变量
    BOOL                m_bRunning;      //本线程是否正在运行标志
    char                m_szErrInfo[2000];//本线程的异常信息
                                         //在线程异常退出时有效
    UINT4               m_uObjType;      //任务对象类型，类型定义在TaskType中

    CBillQBResult*      m_pResult;     //查询或浏览结果对象指针

    ACE_Mutex           m_Mutex;       //格式库对象指针

    char                m_pResultPath[MAX_PATH];
    //是否显示错误话单
    BOOL                m_bDisplayErrBill;
};

#endif
