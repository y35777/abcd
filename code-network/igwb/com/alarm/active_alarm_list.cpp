// ActiveAlarmList.cpp: implementation of the CActiveAlarmList class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/frame.h"
#include "active_alarm_list.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//函数原型  Public CActiveAlarmList(SActiveAlarm& sActiveAlarm)
//功能描述  构造函数
//输入参数  sActiveAlarm——活动告警帧模板,因为告警帧中的许多域是写死的
//          所以用一个模板来初始化他.
////////////////////////////////////////////////////////////////////////

CActiveAlarmList::CActiveAlarmList(SActiveAlarm& sActiveAlarm):m_pAlmCfg(NULL)
{

    memcpy(&m_ActiveAlarmTmpl,&sActiveAlarm,sizeof(SActiveAlarm));
}


CActiveAlarmList::CActiveAlarmList():m_pAlmCfg(NULL)
{
    memset(&m_ActiveAlarmTmpl,0,sizeof(SActiveAlarm));
}

////////////////////////////////////////////////////////////////////////
//功能描述  析构函数
////////////////////////////////////////////////////////////////////////
CActiveAlarmList::~CActiveAlarmList()
{
    //A、遍历链表，释放链表中各成员占用的内容空间。
    iterator Iter;
    Iter = begin();
    while(Iter != end())
    {
        delete (*Iter);
        Iter++;
    }
    clear();
    m_pAlmCfg = NULL;

}

//////////////////////////////////////////////////////////////////////////////
//功能描述  向队列中增加一个告警帧
//输入参数  pInnerAlarm - 内容告警数据帧
//输出参数  pActive     - 活动告警结构,用于对历史库的操作和添加流水号
//返回值    ALARM_ADD_RESULT
//          {
//           ADD_OK,      //告警添加正常
//           ADD_REPEATED,    //告警重复
//           ADD_NOT_MATCH,   //告警不匹配
//           INVALID_ALARM    //告警非法
//          }
//////////////////////////////////////////////////////////////////////////////
int CActiveAlarmList::AddAlarm(SInnerAlarm *pInnerAlarm, SActiveAlarm **ppActive, int nRecoverType)
{
    if(pInnerAlarm == NULL)
    {
        return INVALID_ALARM;
    }
    //判断告警类型是否正确,若告警类型不为事件告警,故障告警或
    //恢复告警中的一种则返回INVALID_ALARM
    if(pInnerAlarm->yAlarmType != AT_EVENT
        && pInnerAlarm->yAlarmType != AT_FAULT
        && pInnerAlarm->yAlarmType != AT_RESTORE)
    {
        *ppActive = NULL;
        return INVALID_ALARM;
    }

    //查找是否重复告警,若为重复告警,则返回ADD_REPEATED
    if(Find(pInnerAlarm) != NULL)
    {
        *ppActive = NULL;
        return ADD_REPEATED;
    }

    //将告警加入告警队列,不同告警类型加入告警队列的方式不同.
    switch(pInnerAlarm->yAlarmType)
    {
    case AT_RESTORE:
        if((*ppActive = AddRestoreAlarm(pInnerAlarm->yAlarmType,
            pInnerAlarm->uAlarmID, pInnerAlarm->yAlarmPara,nRecoverType)) == NULL)
        {
            return ADD_NOT_MATCH;
        }
        break;
    case AT_FAULT:
        if((*ppActive = AddFatalAlarm(pInnerAlarm->yAlarmType,
            pInnerAlarm->uAlarmID, pInnerAlarm->yAlarmPara)) ==NULL)
        {
            return INVALID_ALARM;
        }
        break;
    case AT_EVENT:
        if((*ppActive = AddEventAlarm(pInnerAlarm->yAlarmType,
            pInnerAlarm->uAlarmID, pInnerAlarm->yAlarmPara)) == NULL)
        {
            return INVALID_ALARM;
        }
        break;
    //张玉新 2002-04-12 问题单D12566
    default:
        {
            TRACE(MTS_ALARM,S_ALARM_RECV_INVALIDALARM,pInnerAlarm->uAlarmID,
                  pInnerAlarm->yAlarmPara, pInnerAlarm->yAlarmType);
            return INVALID_ALARM;
        }
    }
    //结束
    //返回成功.
    return ADD_OK;

}

////////////////////////////////////////////////////////////////////
//功能描述  在告警队列中查找指定告警帧
//输入参数  pInnerAlarm - 包含要查找的告警信息
//输出参数  无
//返回值    如果找到则返回告警帧的指针,否则为空
////////////////////////////////////////////////////////////////////
SActiveAlarm *CActiveAlarmList::Find(SInnerAlarm *pInnerAlarm)
{
    return Find(pInnerAlarm->yAlarmType,
                pInnerAlarm->uAlarmID,
                pInnerAlarm->yAlarmPara);

}

////////////////////////////////////////////////////////////////////
//功能描述  根据告警类型,告警ID和告警参数查找指定告警帧
//输入参数  byAlarmType - 告警类型
//      wAlarmID    - 告警ID
//      byAlarmPara - 告警参数
//输出参数  无
//返回值    告警帧的指针,如果没有则为空
////////////////////////////////////////////////////////////////////
SActiveAlarm *CActiveAlarmList::Find(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara)
{
    //遍历告警整个队列,逐个匹配查找告警类型,告警ID和告警参数与参数相同的活动告警.
    iterator theIterator;
    SActiveAlarm *pActiveAlarm;

    for(theIterator = begin(); theIterator != end(); theIterator++)
    {
        pActiveAlarm = (*theIterator);

        if(pActiveAlarm == NULL)
        {
            continue;
        }

        if(pActiveAlarm->yAlarmType == yAlarmType
            && pActiveAlarm->uAlarmID == uAlarmID
            && memcmp(pActiveAlarm->yPara, yAlarmPara, MAX_ALARM_PARA) == 0)//self change 2 from = to ==
        {
            return pActiveAlarm;
        }
    }
    //若没有查找到匹配的活动告警,则返回空指针
    return NULL;

}

//////////////////////////////////////////////////////////////////////
//功能描述  查找告警帧
//输入参数  SActiveAlarm *pActiveAlarm
//输出参数  无
//返回值    告警帧的指针，如果没有找到则为空
//////////////////////////////////////////////////////////////////////
SActiveAlarm * CActiveAlarmList::Find(SActiveAlarm *pActiveAlarm)
{
    return Find(pActiveAlarm->yAlarmType,
                pActiveAlarm->uAlarmID,
                pActiveAlarm->yPara);

}

/////////////////////////////////////////////////////////////////////
//功能描述  删除告警帧函数
//输入参数  SActiveAlarm *pActiveAlarm
//输出参数  无
//返回值    无
//抛出异常  无
////////////////////////////////////////////////////////////////////
void CActiveAlarmList::RemoveAlarm(SActiveAlarm *pActiveAlarm)
{
    RemoveAlarm(pActiveAlarm->yAlarmType,
                pActiveAlarm->uAlarmID,
                pActiveAlarm->yPara);
}

////////////////////////////////////////////////////////////////////
//功能描述  删除告警帧函数
//输入参数  byAlarmType - 告警类型
//          wAlarmID    - 告警ID
//          byAlarmPara - 告警参数
/////////////////////////////////////////////////////////////////////
void CActiveAlarmList::RemoveAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara)
{
    //查找要删除的活动告警,若存在则删除.
    SActiveAlarm *pAlarm = NULL;
    pAlarm = Find(yAlarmType, uAlarmID, yAlarmPara);

    if(pAlarm != NULL)
    {
        remove(pAlarm);
        delete pAlarm;
        pAlarm = NULL;
    }
}

///////////////////////////////////////////////////////////////////////
//功能描述  根据参数创建一个告警帧
//输入参数  byAlarmType - 告警类型
//          wAlarmID    - 告警ID
//          byAlarmPara - 告警参数
//输出参数  无
//返回值    告警帧的指针
///////////////////////////////////////////////////////////////////////
SActiveAlarm *CActiveAlarmList::CreateActiveAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara,int nRecoverType)
{
    SAlarmExplain *pExplain = NULL;
    pExplain = m_pAlmCfg->GetAlarmExplain(uAlarmID);

    if(pExplain == NULL)
    {
        return NULL;
    }

    SActiveAlarm *pActiveAlarm = new SActiveAlarm;
    //填写活动告警数据帧的部分数据为iGateway bill特有的数据值
    //unit test change 单个赋值到内存拷贝
    memcpy(pActiveAlarm,&m_ActiveAlarmTmpl,sizeof(SActiveAlarm));
    //创建活动告警数据帧,将传入的参数赋值给该数据帧

    pActiveAlarm->yAlarmType = yAlarmType;
    pActiveAlarm->uAlarmID = uAlarmID;
    memcpy(pActiveAlarm->yPara, yAlarmPara, MAX_ALARM_PARA);

    pActiveAlarm->yRecoverType = nRecoverType;

    //根据配置文件来填写告警级别、子功能号等
    pActiveAlarm->yAlarmLevel = pExplain->yAlarmLevel;
    pActiveAlarm->yFuncID = pExplain->yFuncID;
    pActiveAlarm->ySubSysID = pExplain->ySubSysID;
    //填写当前时间到活动告警数据帧中
    time_t ltime;
    time(&ltime);
    tm *ptoday = localtime(&ltime);
    pActiveAlarm->uYear = ptoday->tm_year + 1900;
    pActiveAlarm->yMonth = ptoday->tm_mon + 1;
    pActiveAlarm->yDay = ptoday->tm_mday;
    pActiveAlarm->yHour = ptoday->tm_hour;
    pActiveAlarm->yMin = ptoday->tm_min;
    pActiveAlarm->ySec = ptoday->tm_sec;

    return pActiveAlarm;
}

/////////////////////////////////////////////////////////////////////////////
//功能描述  向告警队列中增加一个故障告警帧
//输入参数  byAlarmType - 告警类型
//          wAlarmID    - 告警ID
//          byAlarmPara - 告警参数
//输出参数  无
//返回值    告警帧的指针
//////////////////////////////////////////////////////////////////////////////
SActiveAlarm * CActiveAlarmList::AddFatalAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara)
{
    //根据告警参数创建活动告警数据帧
    SActiveAlarm *pFatalAlarm = CreateActiveAlarm(yAlarmType, uAlarmID, yAlarmPara);

    //如果告警队列中存在对应的恢复告警则删除该恢复告警
    SActiveAlarm *pRestoreAlarm = Find(AT_RESTORE, uAlarmID, yAlarmPara);

    if(pRestoreAlarm !=  NULL)
    {
        delete pRestoreAlarm;
        remove(pRestoreAlarm);
        pRestoreAlarm = NULL;

    }

    //将该故障告警添加到告警队列中
    //self chang 2 push_back return void so old if() be delete!;
    if(pFatalAlarm != NULL)
    {
        push_back(pFatalAlarm);
    }

    //返回告警数据帧
    return pFatalAlarm;
}

////////////////////////////////////////////////////////////////////
//功能描述  向告警队列中增加事件告警
//输入参数  byAlarmType - 告警类型
//      wAlarmID    - 告警ID
//      byAlarmPara - 告警参数
//输出参数  无
//返回值    告警帧的指针
//////////////////////////////////////////////////////////////////////
SActiveAlarm *CActiveAlarmList::AddEventAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara)
{
    //根据告警参数创建活动告警数据帧
    SActiveAlarm *pEventAlarm = CreateActiveAlarm(yAlarmType, uAlarmID, yAlarmPara);
   
    //OM整改：事件告警不记入活动告警库，直接可以进入历史告警库（即告警日志）
#ifndef _OM_MODIFY
    //将告警数据帧添加到告警队列中并返回该告警数据帧
    if(pEventAlarm != NULL)
    {
        push_back(pEventAlarm);
    }
#endif

    return pEventAlarm;

}

//////////////////////////////////////////////////////////////////
//功能描述  向告警队列中增加恢复告警
//输入参数  byAlarmType - 告警类型
//      wAlarmID    - 告警ID
//      byAlarmPara - 告警参数
//输出参数  无
//返回值    告警帧的指针
//////////////////////////////////////////////////////////////////
SActiveAlarm * CActiveAlarmList::AddRestoreAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara, int nRecoverType)
{
    //如果告警队列中存在对应的故障告警则删除该故障告警
    //Unit test change uAlarmID to uAlarmID
    SActiveAlarm *pFatalAlarm = Find(AT_FAULT, uAlarmID, yAlarmPara);
    SActiveAlarm *pRestoreAlarm = CreateActiveAlarm(yAlarmType,
                                                        uAlarmID,
                                                        yAlarmPara, nRecoverType);

    if(pFatalAlarm != NULL && pRestoreAlarm != NULL)
    {
        //根据告警参数生成活动告警数据帧
        pRestoreAlarm->uCsn = pFatalAlarm->uCsn;
        remove(pFatalAlarm);
        delete pFatalAlarm;

        //添加该恢复告警到队列中,并返回该该告警
        push_back(pRestoreAlarm);
        return pRestoreAlarm;
    }
    //否则返回NULL
    else
    {        
        //OM整改:当没有找到活动告警，或者没有正确生成恢复告警，都直接返回NULL,不然会影响同步号的正确连续性
        if(pRestoreAlarm != NULL)
        {
            //如果是启动恢复告警，则需要添加
            if(RESET_RECOVER == nRecoverType)
            {
                push_back(pRestoreAlarm); 
            }
            else
            {
                delete pRestoreAlarm;
                pRestoreAlarm = NULL;
            }
        }
        
        return pRestoreAlarm;
    }
}

//////////////////////////////////////////////////////////////////
//功能描述  初始化告警队列中告警模板
//输入参数  sActiveAlarm - 告警模板参数
//输出参数  无
//返回值    失败为非零,0 为成功
//////////////////////////////////////////////////////////////////
int CActiveAlarmList::InitList(SActiveAlarm &sActiveAlarm,CAlarmConfig *pConfig)
{
    memcpy(&m_ActiveAlarmTmpl,&sActiveAlarm,sizeof(SActiveAlarm));
    m_pAlmCfg = pConfig;

    return 0;
}




