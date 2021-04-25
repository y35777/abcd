/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : cdrsnd_svr.h
 *描述信息  ：本文件是类CCDRSndSvr的定义
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#ifndef __CDR_SENDER_H__
#define __CDR_SENDER_H__

#include "../include/precom.h"
#include "../frame/wrk_module.h"

#include "sender_status.h"
#include "cdr_buffer.h"
#include "cdr_producer.h"

class CGtpClient;

class CCDRSndSvr : public CWorkModule  
{
public:
    virtual ~CCDRSndSvr();                //析构函数
    CCDRSndSvr();                         //构造函数
    
    int SendMsgtoBS(MSG_INTRA* pMsg);

protected:
    //继承的接口函数
    int InitInstance();                   //初始化
    void ExitInstance();                  //退出清理
    void OnTimer(const long nTimerID);    //定时器
    
    MODULE_TYPE GetModuleType();          //模块类型
    void ParseMsg(MSG_INTRA* pMsgParsed); //消息分发
    
    virtual void Timer1Sec();         

protected:
    CSenderStatusFile m_StatusFile;       //发送状态
    CCDRBuffer        m_CDRBuffer;        //缓冲区
    CCDRProducer*     m_pPrdCDRThrd;      //产生话单线程
    CGtpClient*       m_pGtpClient;       //GTP' Client
    long              m_Timer1SecId;       //秒定时器
    
};
#endif //__CDR_SENDER_H__
