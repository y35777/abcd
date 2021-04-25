#ifndef __CDR_STAT_TSK_H__
#define __CDR_STAT_TSK_H__

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

#include "ace/Task.h"
#include "stat_cfg_info.h"
#include "../include/format_out.h"

class CCDRStatRstMgr;
class CCDRStatFileGroup;
typedef LIST<CFormat*>   FORMAT_LIST;
//一个接入点每个格式对应的表达式列表
typedef MAP<CFormat*, INFO_LIST>  FORMAT_MAP_INFOLIST;

//接入点映射到“格式-表达式”列表
typedef MAP<STRING, FORMAT_MAP_INFOLIST> AP_MAP_FORMATINFO;


class CCDRStatTask: public ACE_Task_Base
{
public:
    CCDRStatTask();
    virtual ~CCDRStatTask();

public:

    //进行初始化操作
    int Init(UINT4 nApCount);

    //设置退出标志，并等待退出
    void WaitForExit(void);

    //激活下一次统计操作
    void ActivateNextStatOper(void);

    //线程处理函数
    virtual int svc();

    //启动统计操作
    int DoStatOpr(void);

    //构建接入点对应“格式-表达式列表”的映射
    void BuildMap_AP_FMT_EXP(const char* const szAPName);

    //分析一个域是否存在于可选域列表中
    BOOL IsExist(STRING sField, LIST<STRING> ParaInfoList);

    //根据表达式名称查找和名称对应的索引号
    int FindIndexFromName(const char* const szExpress, SStatExpIndexList* pIndexList);

    //获得下一天的日期
    int GetNextDate(char* szDate, time_t& lastdate);

    //在可选域列表查找可选域
    BOOL FindField(const char* const szFieldName, LIST<STRING> FieldList);

    CCDRStatRstMgr* GetCCDRStatRstMgr();

    int StatCDR(SCdrStatTaskInfo* pInfo,
                char* szPath,
                char* szLastDate,
                SStatExpIndexList* pIndexList,
                SAllStatExpRst*&  pAllRst);

protected:
    //原始话单根目录
    char  m_szOrigBasePath[MAX_PATH];

    //最终话单根目录
    char  m_szFinalBasePath[MAX_PATH];

    char  m_szStatPath[MAX_PATH];

    //格式库对象指针
    CIGWB*            m_pIGWB;

    //格式库查询浏览接口对象指针
    IQueryBrowse*     m_pIQueryBrowse;

    //统计结果文件管理者对象指针
    CCDRStatRstMgr*          m_pCDRStatRstMgr;

    //退出标志
    BOOL                     m_bExit;

    //任务消息列表
    TASKINFO_LIST            m_TaskInfoList;

    //每个接入点对应的表达式列表
    MAP<STRING, INFO_LIST>   m_mapAPToExpress;

    //接入点对应"格式-表达式"的映射
    AP_MAP_FORMATINFO        m_mapAPToFormatExpress;

    //接入点对应"格式-可选域"的映射
    AP_MAP_FORMATINFO        m_mapAPToFormatParaInfo;

    //接入点对应原始浏览格式映射
    MAP<STRING,FORMAT_LIST>  m_mapAPToOrigFormat;

    //每个接入点对应的通道列表
    MAP<STRING, INFO_LIST>   m_mapAPToChlName;

    MAP<STRING, int>         m_mapChlNameToLen;
};
#endif  //__CDR_STAT_TSK_H__
