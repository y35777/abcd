// AlarmMMLReport.cpp: implementation of the CAlarmMMLReport class.
//
//////////////////////////////////////////////////////////////////////

#include "alarm_mml_report.h"

#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlarmMMLReport::CAlarmMMLReport(const char* szName):CMMLReport(szName)
{
    //by ldf 2004-01-12 OM整改，初始化告警同步号为100
    m_uMMLBaseAlarmID = 100;
}   

CAlarmMMLReport::~CAlarmMMLReport()
{
}
////////////////////////////////////////////////////////
//功能描述    构造告警报告片段头
//        格式为:
//        <告警标志 ALARM><2SP><告警流水号'L10><2SP><告警类别'L8>
//        <2SP><告警级别'L12><4SP><告警来源'L8><2SP><告警编号'L5><4SP>
//        <告警网管分类'L16>\r\n
//输入参数    SAlarmDetail *pAlarm
//李杜芳 2002-9-17
////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeAlarmHead(SAlarmDetail *pAlarm)
{
    if(pAlarm == NULL)
    {
        return;
    }
    //添加告警标志
    AddString("ALARM");
    FillSpace(2);

    //添加告警流水号
    char tmp[16];
    sprintf(tmp,"%d",pAlarm->uCsn);
    AddString(tmp,10,ALIGN_LEFT);
    FillSpace(2);
    STRING szTmp = pAlarm->GetAlarmType();
    //添加告警类别
    AddString(szTmp.c_str(), 8, ALIGN_LEFT);
    FillSpace(2);

    //添加告警级别
    szTmp = pAlarm->GetAlarmLevel();
    AddString(szTmp.c_str(), 12, ALIGN_LEFT);
    FillSpace(4);

    //添加告警来源
    szTmp = pAlarm->GetAlarmSource();
    AddString(szTmp.c_str(), 8, ALIGN_LEFT);
    FillSpace(2);

    //添加告警编号
    szTmp = pAlarm->GetAlarmID();
    AddString(szTmp.c_str(), 5, ALIGN_LEFT);
    FillSpace(4);

    //添加告警网管分类
    szTmp = pAlarm->GetAlarmSubSysID();
    AddString(szTmp.c_str(), 16, ALIGN_LEFT);
    //FillSpace(2);
}

///////////////////////////////////////////////////////////////
//功能描述    构造"主动上报告警报告". 报告的格式如下:
//<起始符标识 +++><4SP><源标识符'L32><8SP><告警产生日期'L10><1SP>
//<告警产生时间'L8>\r\n
//ALARM  告警流水号   告警类别   告警级别   告警来源  告警编号 告警网管分类
//       模块号    =  (模块号)
//       告警名称  =  (告警名称)
//       定位信息  =  告警参数 ＝参数字  告警详细解释 ＝ 详细解释
//                      告警参数详细解释 ＝ 参数解释值
//<结束符标识 ---><4SP><END>
//输入参数
//活动告警队列中存储的告警结构 -SActiveAlarm
//告警配置类指针               -pConfig
//张玉新2001-10-17
/////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeAlarmBroad(SActiveAlarm *pAlarm, CAlarmConfig *pConfig,int nRecoverType)
{
    //A 构造报文头
    MakeReportHead2(pAlarm);
    //B 获得CAlarmDetail结构指针,构造"告警报告片段头"
    SAlarmDetail* pAlarmDetail = pAlarm->GetAlarmDetail();
    MakeAlarmHead(pAlarmDetail);
    //C 构造<模块号'L20A1><2SP><=><2SP><模块号'L3A0>\r\n
    AddString(STR_NEWLINE);
	
    STRING szTmp;
#ifdef _OM_MODIFY
    //by ldf 2004-01-31 OM整改
    UINT4 uSynInLog = pAlarmDetail->GetSYNNO();

    //对报文的同步号，进行M2000同步基准值处理
    char tmp[8];
    sprintf(tmp,"%d",uSynInLog + (m_uMMLBaseAlarmID -100) );
    MakeReportItem(S_ALARM_SYN_NO, tmp, 20,ALIGN_RIGHT);
	
    szTmp = pAlarmDetail->GetAlarmTime();
    if(pAlarm->yAlarmType != AT_RESTORE)
    {
        MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
    }
    //end
#endif


    szTmp = pAlarmDetail->GetModule();
    MakeReportItem(S_ALARM_MODULENO, szTmp.c_str(),20,ALIGN_RIGHT);
    //D 构造<告警名称'L20A1<2SP><=><2SP><告警名称'L55A0>\r\n
    szTmp = pAlarmDetail->GetAlarmName(pConfig);
    MakeReportItem(S_ALARM_ALARMNAME, szTmp.c_str(),20,ALIGN_RIGHT);
    //E 构造<定位信息'L20A1><2SP><=><2SP><告警参数=><参数值'M3A0><1SP>
    //                                     <告警详细解释=><解释值'M6A0><1SP>
    //                                   <告警参数详细解释=><告警参数解释值'M3>\r\n
    char szContent[1024];
    STRING szTmpPara = pAlarmDetail->GetAlarmParas();
    STRING szTmpExp = pAlarmDetail->GetExplain(pConfig);

    #ifdef _OM_MODIFY
        SNPRINTF(szContent,1024,"%s=%s",S_ALARM_ALARMPARAMETER , szTmpPara.c_str()); 
    #else 
        STRING szTmpParaExp = pAlarmDetail->GetParaExplain(pConfig);
        //为联调去掉空格
        SNPRINTF(szContent,1024,"%s=%s %s=%s %s=%s",
            S_ALARM_ALARMPARAMETER , szTmpPara.c_str() ,
            S_ALARM_ALARMEXPLAIN ,    szTmpExp.c_str() ,
            S_ALARM_ALARMPARAEXPLAIN, szTmpParaExp.c_str());
    #endif


    MakeReportItem(S_ALARM_ALARMPARA,szContent,20,ALIGN_RIGHT);

    #ifdef _OM_MODIFY
        //by ldf 2004-01-31 OM整改
        if(pAlarm->yAlarmType == AT_RESTORE)
        {
            switch(nRecoverType)
            {
                //AUTO_RECOVER,		//1:正常恢复
            case 1:
                MakeReportItem(S_ALARM_RECOVERSTATUS, S_ALARM_AUTO_RECOVER,20,ALIGN_RIGHT);
                break;
                //RESET_RECOVER,		//2:复位恢复
            case 2:
                MakeReportItem(S_ALARM_RECOVERSTATUS, S_ALARM_RESET_RECOVER,20,ALIGN_RIGHT);
                break;
                //MANNUAL_RECOVER     //3:手工恢复
            case 3:
                MakeReportItem(S_ALARM_RECOVERSTATUS, S_ALARM_MANNUAL_RECOVER,20,ALIGN_RIGHT);
                break;
            default:
                MakeReportItem(S_ALARM_RECOVERSTATUS, S_ALARM_AUTO_RECOVER,20,ALIGN_RIGHT);
            }

            //得到恢复告警的恢复时间，其实和头部时间相同，这里有些重复
            char tmp[20];
            sprintf(tmp,"%04d-%02d-%02d %02d:%02d:%02d",pAlarm->uYear, pAlarm->yMonth,pAlarm->yDay,
                pAlarm->yHour,pAlarm->yMin,pAlarm->ySec);
            STRING szRecoverTime =tmp;		
            MakeReportItem(S_ALARM_ALARMRECOVERACKDATE, szRecoverTime.c_str(),20,ALIGN_RIGHT);
        }
        //end
    #endif


    //F 构造报文尾
    MakeReportTail();
}
/////////////////////////////////////////////////////////////////////////
//功能描述    构造"事件告警确认上报报告",
//格式如下:
//<告警标志 ALARM     0          确认     消息      告警来源>\r\n
//<确认操作员'L0A1<2SP><=><2SP><确认操作员'L0A0>\r\n
//<告警类型'L0A1<2SP><=><2SP><告警类型'L0A0>\r\n
//<告警流水号'L0A1<2SP><=><2SP><流水号'L0A0>\r\n
//输入参数:
//szConfirmOpr -确认操作员
//szAlarmType  -告警类型
//szBeginCsn   -起始流水号
//szEndCsn     -结束流水号
//张玉新2001-10-17
////////////////////////////////////////////////////////////////////////////

//by ldf 2002.6.7 对应问题单D14393
void CAlarmMMLReport::MakeAlarmEventAckBody(const char *szConfirmOpr, const char *szAlarmType, const char *szCsn)
//修改完毕
{
    //A 构造ALARM     0          确认     消息      告警来源
    AddString(S_ALARM_ALARMACKHEAD);
    AddString(STR_NEWLINE); //解决\r\n问题
    //D 构造<告警流水号'L0A1<2SP><=><2SP><流水号'L0A0>\r\n
    MakeReportItem(S_ALARM_ALARMCSN, szCsn,20,ALIGN_RIGHT);
    //B 构造<确认操作员'L0A1<2SP><=><2SP><确认操作员'L0A0>\r\n
    MakeReportItem(S_ALARM_CONFIRMOPR, szConfirmOpr,20,ALIGN_RIGHT);
    //C 构造<告警类型'L0A1<2SP><=><2SP><告警类型'L0A0>\r\n
    //MakeReportItem(S_ALARM_ALARMTYPE, szAlarmType,16,ALIGN_RIGHT);
    //E 构造<结束流水号'L0A1<2SP><=><2SP><流水号'L0A0>\r\n
    //MakeReportItem(S_ALARM_ENDCSN, szEndCsn,16,ALIGN_RIGHT);

}

/////////////////////////////////////////////////////////////////////////
//功能描述    构造"故障告警确认上报报告",
//格式如下:
//<告警标志 ALARM     0          确认     消息      告警来源>\r\n
//<告警流水号'L0A1<2SP><=><2SP><流水号'L10A0><1SP><流水号'L10A0><1SP><流水号'L10A0>\r\n
//<确认操作员'L0A1<2SP><=><2SP><确认操作员'L0A0>\r\n
//输入参数:
//szConfirmOpr -确认操作员
//szAlarmType  -告警类型
//szBeginCsn   -起始流水号
//szEndCsn     -结束流水号
//张玉新2001-10-17
////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeAlarmFaultAckBody (const char* szConfirmOpr,
                                             const char* szAlarmType,
                                             LIST <STRING>& CsnAcked)
{
    //A 构造ALARM     0          确认     消息      告警来源
    AddString(S_ALARM_ALARMACKHEAD);
    AddString(STR_NEWLINE); //解决\r\n问题
    //B 构造<告警流水号'L0A1<2SP><=><2SP><流水号'L0A0>\r\n
    //取出确认成功的告警流水号
    int uCsnNum = CsnAcked.size();
    STRING szCsn;
    const char* szSpace = "          " ;

    for (int nlist_count=0 ; nlist_count < uCsnNum ; nlist_count++)
    {
        STRING szCsnTmp = CsnAcked.front();
        CsnAcked.pop_front();
        int szCsnLen = szCsnTmp.length();
        szCsnTmp = szCsnTmp.append(szSpace,10-szCsnLen);
        szCsn = szCsn + szCsnTmp;
        szCsn = szCsn + " ";
    }

    MakeReportItem(S_ALARM_ALARMCSN, szCsn.c_str(),20,ALIGN_RIGHT);
    //C 构造<确认操作员'L0A1<2SP><=><2SP><确认操作员'L0A0>\r\n
    MakeReportItem(S_ALARM_CONFIRMOPR, szConfirmOpr,20,ALIGN_RIGHT);
}

//////////////////////////////////////////////////////////////////////////
//功能描述    构造“字段查询返回报告”报告体.格式如下:
//ALARM  告警流水号   告警类别   告警级别    告警来源  告警编号  告警网管分类
//告警发生时间  =  (告警发生时间)
//模块号        =  (模块号)
//告警名称        =  (告警名称)
//定位信息        =  告警参数 = 参数值  告警详细解释  = 解释信息  告警参数详细解释
//                =  参数解释信息  修复建议 ＝ 修复建议信息      恢复状态  = (恢复状态)
//确认状态        =  (确认状态)
//告警确认操作员=  (操作员: 机器名|IP地址|用户名)
//告警确认时间    =  (确认时间)
//告警恢复确认操作员 = (操作员: 机器名|IP地址|用户名)
//告警恢复确认时间   = (确认时间)
//输入参数:
//pAlarm  - 历史告警结构指针
//pConfig - 告警配置类的指针
//张玉新 2001-10-17
///////////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeDetailAlarmBody(SHistoryAlarm *pAlarm, CAlarmConfig *pConfig)
{
    //A、获得CAlarmDetail结构指针,构造"告警报告片段头"
    SAlarmDetail* pAlarmDetail = pAlarm->GetAlarmDetail();
    MakeAlarmHead(pAlarmDetail);
    AddString(STR_NEWLINE);
    //联调去掉空行
    //AddString(STR_NEWLINE);
    STRING szTmp = pAlarmDetail->GetAlarmTime();
    //B、构造<告警发生时间'L20A1><2SP><=><2SP><告警发生时间'L3A0>\r\n
    MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
    //C、构造<模块号'L20A1><2SP><=><2SP><模块号'L3A0>\r\n
    szTmp = pAlarmDetail->GetModule();
    MakeReportItem(S_ALARM_MODULENO,  szTmp.c_str(),20,ALIGN_RIGHT);
    //D、构造<告警名称'L20A1<2SP><=><2SP><告警名称'L55A0>\r\n
    szTmp = pAlarmDetail->GetAlarmName(pConfig);
    MakeReportItem(S_ALARM_ALARMNAME, szTmp.c_str(),20,ALIGN_RIGHT);
    //E、构造<定位信息'L16A1><2SP><=><2SP><定位信息1=><定位信息1值'M3A0><1SP>
    //                                    <定位信息2=><定位信息2值'M3A0><1SP>
    //                                       ...
    //                                      <定位信息n=><定位信息n值'M3>\r\n
    char szContent[1024];
    szTmp = pAlarmDetail->GetFixExplain(pConfig);
    STRING szTmpExp = pAlarmDetail->GetExplain(pConfig);

    STRING szTmpPara = pAlarmDetail->GetAlarmParas();
#ifdef _OM_MODIFY
    SNPRINTF(szContent,1024,"%s=%s",S_ALARM_ALARMPARAMETER , szTmpPara.c_str());
#else    
    STRING szTmpParaExp = pAlarmDetail->GetParaExplain(pConfig);
    //联调去掉空格
    SNPRINTF(szContent,1024,"%s=%s %s=%s %s=%s",
        S_ALARM_ALARMPARAMETER , szTmpPara.c_str() ,
        S_ALARM_ALARMEXPLAIN , szTmpExp.c_str() ,
        S_ALARM_ALARMPARAEXPLAIN , szTmpParaExp.c_str());
#endif
    MakeReportItem(S_ALARM_ALARMPARA, szContent,20,ALIGN_RIGHT);
    
#ifndef _OM_MODIFY
    MakeReportItem(S_ALARM_ALARMFIX , szTmp.c_str(),20,ALIGN_RIGHT,201,ALIGN_LEFT);
    //F 构造<确认状态'L20A1<2SP><=><2SP><确认状态'L16A0>\r\n
    szTmp = pAlarm->GetAckStatus();
    MakeReportItem(S_ALARM_ACKSTATUS , szTmp.c_str(),20,ALIGN_RIGHT,16,ALIGN_LEFT);
#endif

    //G 构造<恢复状态'L20A1<2SP><=><2SP><恢复状态'L16A0>\r\n
    if(pAlarm->yAlarmType == AT_FAULT)
    {
        szTmp = pAlarm->GetRecoverAckStatus();
        MakeReportItem(S_ALARM_RECOVERSTATUS,szTmp.c_str(),20,ALIGN_RIGHT,16,ALIGN_RIGHT);
    }
    
#ifndef _OM_MODIFY
    //H、构造
    //        告警确认操作员  =  (操作员:机器名|IP地址|用户名)
    //        告警确认时间   =   (确认时间)
    //        告警恢复确认操作员  =  (操作员:机器名|IP地址|用户名)
    //        告警恢复确认时间  =  (确认时间)
    szTmp = pAlarm->GetFAckOptr();
    MakeReportItem(S_ALARM_ALARMACKOPR , szTmp.c_str(),20,ALIGN_RIGHT);
    szTmp = pAlarm->GetFAckTime();
    MakeReportItem(S_ALARM_ALARMACKDATE , szTmp.c_str(),20,ALIGN_RIGHT);
    if(pAlarm->yAlarmType == AT_FAULT)
    {
        szTmp = pAlarm->GetRAckOptr();
        MakeReportItem(S_ALARM_ALARMRECOVERACKOPR, szTmp.c_str(),20,ALIGN_RIGHT);
        szTmp = pAlarm->GetRecoverAckTime();
        MakeReportItem(S_ALARM_ALARMRECOVERACKDATE, szTmp.c_str(),20,ALIGN_RIGHT);
    }
#endif

}
///////////////////////////////////////////////////////////////////////////////////
//功能描述:
//            ALARM  告警流水号   告警类别   告警级别    告警来源  告警编号  告警网管分类
//            告警发生时间  =  (告警发生时间)
//            模块号  =  (模块号)
//            告警名称  =  (告警名称)
//            定位信息  =  (定位信息)告警参数 = 参数值  告警详细解释  = 解释信息
//                          告警参数详细解释 = 参数解释信息  修复建议 = (修复建议)
//          恢复状态  =  (恢复状态)
//          确认状态  =  (确认状态)
//输入参数
//pAlarm    -历史告警结构指针
//pConfig   -告警配置类的指针
//张玉新 2001-10-17
/////////////////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeQueryAlarmBody(SHistoryAlarm *pAlarm, CAlarmConfig *pConfig)
{
    //A 获得CAlarmDetail结构指针,构造"告警报告片段头"
    SAlarmDetail* pAlarmDetail = pAlarm->GetAlarmDetail();
    MakeAlarmHead(pAlarmDetail);
    AddString(STR_NEWLINE);
    STRING szTmp = pAlarmDetail->GetAlarmTime();
    
#ifdef _OM_MODIFY
    //by ldf 2004-01-31 OM整改
    UINT4 uSynInLog = pAlarmDetail->GetSYNNO();
    //对报文的同步号，进行M2000同步基准值处理
    char tmp[8];
    sprintf(tmp,"%d",uSynInLog + (m_uMMLBaseAlarmID - 100) );
    MakeReportItem(S_ALARM_SYN_NO, tmp, 20,ALIGN_RIGHT);

    if(pAlarm->yResType == NOT_RECOVER)
    {
        MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
    }
#else
    MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
#endif

    //C 构造<模块号'L20A1><2SP><=><2SP><模块号'L3A0>\r\n
    szTmp =  pAlarmDetail->GetModule();
    MakeReportItem(S_ALARM_MODULENO, szTmp.c_str(),20,ALIGN_RIGHT);
    //D 构造<告警名称'L20A1<2SP><=><2SP><告警名称'L50A0>\r\n
    szTmp = pAlarmDetail->GetAlarmName(pConfig);
    MakeReportItem(S_ALARM_ALARMNAME, szTmp.c_str(),20,ALIGN_RIGHT);
    //E 构造<定位信息'L20A1><2SP><=><2SP><定位信息1=><定位信息1值'M3A0><1SP>
    //                                   <定位信息2=><定位信息2值'M6A0><1SP>
    //                                     ...
    //                                         <定位信息n=><定位信息n值'M3>\r\n
    
    char szContent[1024];
    STRING szTmpPara = pAlarmDetail->GetAlarmParas();
#ifdef _OM_MODIFY
    SNPRINTF(szContent,1024,"%s=%s",S_ALARM_ALARMPARAMETER ,szTmpPara.c_str());
#else
    STRING szTmpExp = pAlarmDetail->GetExplain(pConfig);
    STRING szTmpParaExp = pAlarmDetail->GetParaExplain(pConfig);
    szTmp = pAlarmDetail->GetFixExplain(pConfig);
    //联调去掉空格
    SNPRINTF(szContent,1024,"%s=%s %s=%s %s=%s",
        S_ALARM_ALARMPARAMETER ,szTmpPara.c_str() ,
        S_ALARM_ALARMEXPLAIN , szTmpExp.c_str() ,
        S_ALARM_ALARMPARAEXPLAIN , szTmpParaExp.c_str());
#endif
    
    MakeReportItem(S_ALARM_ALARMPARA,szContent,20,ALIGN_RIGHT);
#ifndef _OM_MODIFY
    MakeReportItem(S_ALARM_ALARMFIX , szTmp.c_str(),20,ALIGN_RIGHT,201,ALIGN_LEFT);
    //F 构造<确认状态'L20A1<2SP><=><2SP><确认状态'L0A0>\r\n
    szTmp = pAlarm->GetAckStatus();
    MakeReportItem(S_ALARM_ACKSTATUS, szTmp.c_str(),20,ALIGN_LEFT);
#endif
    //G 构造<恢复状态'L20A1<2SP><=><2SP><恢复状态'L50A0>\r\n
    szTmp = pAlarm->GetRecoverAckStatus();
    MakeReportItem(S_ALARM_RECOVERSTATUS, szTmp.c_str(),20,ALIGN_RIGHT);

    //H、构造
    //        告警确认操作员  =  (操作员:机器名|IP地址|用户名)
    //        告警确认时间   =   (确认时间)
    //        告警恢复确认操作员  =  (操作员:机器名|IP地址|用户名)
    //        告警恢复确认时间  =  (确认时间)
#ifdef _OM_MODIFY
    if(pAlarm->yResType != NOT_RECOVER)
    {
        szTmp = pAlarm->GetRevTime();
        MakeReportItem(S_ALARM_ALARMREVTIME , szTmp.c_str(),20,ALIGN_RIGHT);
    }
#else
    if(pAlarm->yAlarmType == AT_FAULT && pAlarm->yResType != NOT_RECOVER)
    {
        szTmp = pAlarm->GetRevTime();
        MakeReportItem(S_ALARM_ALARMREVTIME , szTmp.c_str(),20,ALIGN_RIGHT);
    }
    szTmp = pAlarm->GetFAckOptr();
    MakeReportItem(S_ALARM_ALARMACKOPR , szTmp.c_str(),20,ALIGN_RIGHT);
    szTmp = pAlarm->GetFAckTime();
    MakeReportItem(S_ALARM_ALARMACKDATE , szTmp.c_str(),20,ALIGN_RIGHT);
    
    szTmp = pAlarm->GetRAckOptr();
    MakeReportItem(S_ALARM_ALARMRECOVERACKOPR, szTmp.c_str(),20,ALIGN_RIGHT);
    szTmp = pAlarm->GetRecoverAckTime();
    MakeReportItem(S_ALARM_ALARMRECOVERACKDATE, szTmp.c_str(),20,ALIGN_RIGHT);
#endif

}

/////////////////////////////////////////////////////////////////////////////////
//功能描述:
//            ALARM  告警流水号   告警类别   告警级别    告警来源  告警编号  告警网管分类
//            告警发生时间  =  (告警发生时间)
//          模块号  =  (模块号)
//          告警名称  =  (告警名称)
//          定位信息  =     告警参数  =  参数值
//                         告警详细解释  =  解释信息
//                         告警参数详细解释  =  参数解释信息
//                         修复建议  =  (修复建议)
//输入参数:
//pAlarm  -告警结构指针
//pConfig -告警配置类的指针
//张玉新2001-10-17
//////////////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeQueryAlarmBody(SActiveAlarm *pAlarm, CAlarmConfig *pConfig)
{
    //A 获得CAlarmDetail结构指针,构造"告警报告片段头"
    SAlarmDetail *pAlarmDetail = pAlarm->GetAlarmDetail();
    MakeAlarmHead(pAlarmDetail);
    //B 构造<告警发生时间'L20A1><2SP><=><2SP><告警发生时间'L3A0>\r\n
    STRING szTmp = pAlarmDetail->GetAlarmTime();
    MakeReportItem(S_ALARM_ALARMTIME, szTmp.c_str(),20,ALIGN_RIGHT);
    //C 构造<模块号'L20A1><2SP><=><2SP><模块号'L3A0>\r\n
    szTmp = pAlarmDetail->GetModule();
    MakeReportItem(S_ALARM_MODULENO, szTmp.c_str(),20,ALIGN_RIGHT);
    //D 构造<告警名称'L20A1<2SP><=><2SP><告警名称'L50A0>\r\n
    szTmp = pAlarmDetail->GetAlarmName(pConfig);
    MakeReportItem(S_ALARM_ALARMNAME, szTmp.c_str(),20,ALIGN_RIGHT);
    //E 构造<定位信息'L20A1><2SP><=><2SP><定位信息1=><定位信息1值'M3A0><1SP>
    //                                     <定位信息2=><定位信息2值'M6A0><1SP>
    //                                     ...
    //                                     <定位信息n=><定位信息n值'M3>\r\n
    szTmp = pAlarmDetail->GetAlarmParas();
    MakeReportItem(S_ALARM_ALARMPARA, szTmp.c_str(),20,ALIGN_RIGHT);
    
    char szContent[1024];
    STRING szTmpPara = pAlarmDetail->GetAlarmParas();
#ifdef _OM_MODIFY
    SNPRINTF(szContent,1024,"%s=%s",S_ALARM_ALARMPARAMETER ,szTmpPara.c_str());
#else
    STRING szTmpExp = pAlarmDetail->GetExplain(pConfig);
    STRING szTmpParaExp = pAlarmDetail->GetParaExplain(pConfig);
    szTmp = pAlarmDetail->GetFixExplain(pConfig);
    ////联调去掉空格
    SNPRINTF(szContent,1024,"%s=%s %s=%s %s=%s %s=%s",
        S_ALARM_ALARMPARAMETER ,szTmpPara.c_str(),
        S_ALARM_ALARMEXPLAIN , szTmpExp.c_str(),
        S_ALARM_ALARMPARAEXPLAIN , szTmpParaExp.c_str(),
        S_ALARM_ALARMFIX ,szTmp.c_str());
#endif
    MakeReportItem(S_ALARM_ALARMPARA, szContent,20,ALIGN_RIGHT);
}
///////////////////////////////////////////////////////////////////////////////
//功能描述    构造类似:
//< ALARM><2SP>\r\n
//<模块号'L0A1<2SP><=><2SP><模块号'L0A0>\r\n
//<起始流水号'L0A1<2SP><=><2SP><流水号'L0A0>\r\n
//<结束流水号'L0A1<2SP><=><2SP><流水号'L0A0>\r\n
//的报文
//输入参数
//start_csn    -起始流水号
//end_csn    -结束流水号
//MH        -模块号
//张玉新2001-10-17
///////////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeRecoverAlarmBody(int start_csn, int end_csn, int MH)
{
    //A 构造<模块号'L0A1<2SP><=><2SP><模块号'L0A0>\r\n
    char tmp[32];
#ifndef _OM_MODIFY
    sprintf(tmp,"%d",MH);
    MakeReportItem(S_ALARM_MODULENO,tmp,20,ALIGN_RIGHT);
#endif

    //B 构造<起始流水号'L0A1<2SP><=><2SP><流水号'L0A0>\r\n
    sprintf(tmp,"%d",start_csn);
    MakeReportItem(S_ALARM_BEGINCSN,tmp,20,ALIGN_RIGHT);
    //C 构造<结束流水号'L0A1<2SP><=><2SP><流水号'L0A0>\r\n
    sprintf(tmp,"%d",end_csn);
    MakeReportItem(S_ALARM_ENDCSN, tmp,20,ALIGN_RIGHT);
}

void CAlarmMMLReport::MakeDeleteAlarmBody(char *start_date, char *end_date, int MH)
{
    //A、构造<ALARM><2SP>\r\n
    AddString("ALARM");
    FillSpace(2);
    AddString(STR_NEWLINE);
    //B 构造<模块号'L0A1<2SP><=><2SP><模块号'L0A0>\r\n
    char tmp[8];
    sprintf(tmp,"%d",MH);
    MakeReportItem(S_ALARM_MODULENO,tmp,20,ALIGN_RIGHT);
    //C 构造<起始日期'L0A1<2SP><=><2SP><起始日期'L0A0>\r\n
    MakeReportItem(S_ALARM_BEGINDATE,start_date,20,ALIGN_RIGHT);
    //D 构造<结束日期'L0A1<2SP><=><2SP><结束日期'L0A0>\r\n
    MakeReportItem(S_ALARM_ENDDATE, end_date,20,ALIGN_RIGHT);

}
/////////////////////////////////////////////////////
//功能描述    根据报告头格式组装报告头,格式为
//<起始符标识 +++><4SP><源标识符'L32><8SP><告警产生日期'L10><1SP><告警产生时间'L8>\r\n
//张玉新 2001-10-13
/////////////////////////////////////////////////////
void CAlarmMMLReport::MakeReportHead2(SActiveAlarm *pActive)
{
    //A 置m_nReportLen = m_nHeadPosition,表示开始从m_nHeadPosition重新拼接报文数据,并将从m_nHeadPosition开始的数据清0
    m_nReportLen = m_nHeadPosition;
    memset(m_pszBuf + m_nHeadPosition, 0, m_nBufLen - m_nHeadPosition);
    //B 通过GetStrDate获得当前日期字符串,并添加到报文中
    char szTime[20];
    sprintf(szTime, "%04d-%02d-%02d",pActive->uYear, pActive->yMonth, pActive->yDay);
    AddString(szTime, 10, ALIGN_LEFT);
    FillSpace(1);
    //C 通过GetStrTime获得当前时间字符串,并添加到报文中
    sprintf(szTime, "%02d:%02d:%02d", pActive->yHour,pActive->yMin,pActive->ySec);
    AddString(szTime, 8, ALIGN_LEFT);
    AddString(STR_NEWLINE);
}

//////////////////////////////////////////////////////////////////////////
//重载utility模块中的mml_report函数中的函数
//功能描述  根据报告体标题和内容组装类似
//          <告警名称'L16A1><2SP><=><2SP><告警名称'L50A0>\r\n的报告体项
//输入参数  STRING szTitle  -等号左边的字符
//      STRING szContent    -等号右边的字符
//      int nFirstFormatLen -第一个参数的字符宽度
//      int nFirstAign      -第一个参数的对齐方式
//      int nSecondFormatLen-第二个参数的字符宽度
//      int nSecondAlign    -第二个参数的对齐方式
//李杜芳 2002.1.15
/////////////////////////////////////////////////////////////////////////
void CAlarmMMLReport::MakeReportItem(const char* szTitle,
                                     const char* szContent,
                                     int nFirstFormatLen,
                                     int nFirstAign,
                                     int nSecondFormatLen,
                                     int nSecondAlign)
{
    int nTitleLen = strlen(szTitle);
    int nConLen = strlen(szContent);
    int nContentLen = nConLen + nTitleLen + strlen(MML_FILL1);

    //如果长度太长,则要多行显示
    if(nContentLen >= nSecondFormatLen || nSecondFormatLen > 55)
    {
        char tempContent[1024];
        char tempTitle[512];
        BYTE nSecondSpace = 0;

        //将整个显示给客户的信息组成一个整体,然后组成类似如下的报文:
        //      定位信息 ＝  定为信息1 = 定位信息1的值  定位信息2 = 定位信息2
        //                   值定位信息  定位信息3 = 定位信息3的值

        if(nTitleLen >= nFirstFormatLen)
        {
            memcpy(tempTitle, szTitle, nFirstFormatLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else if(nFirstAign == ALIGN_LEFT)//左对齐方式
        {
            memcpy(tempTitle, szTitle, nTitleLen);
            memset(tempTitle + nTitleLen, CHAR_SPACE,nFirstFormatLen-nTitleLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else if(nFirstAign == ALIGN_RIGHT)
        {
            memset(tempTitle,CHAR_SPACE,nFirstFormatLen-nTitleLen);
            memcpy(tempTitle + nFirstFormatLen-nTitleLen, szTitle, nTitleLen);
            tempTitle[nFirstFormatLen] = '\0';
        }
        else
        {
            strcpy(tempTitle, szTitle);
        }
        if(nSecondAlign == ALIGN_LEFT)
            sprintf(tempContent,"%s%s%s",tempTitle,MML_FILL1,szContent);
        else
        {
            char temp[128];
            memset(temp,CHAR_SPACE,nSecondFormatLen);
            temp[nSecondFormatLen] = '\0';
            nSecondSpace = nSecondFormatLen;
            sprintf(tempContent,"%s%s%s%s",tempTitle,MML_FILL1,temp,szContent);
        }
        STRING sOutput;
        MultiLine(tempContent,sOutput,strlen(tempTitle)+strlen(MML_FILL1)+nSecondSpace);
        AddString(sOutput.c_str());
    }
    //长度足够的话,简单拼接
    else
    {
    //拼接szTitle中数据
        AddString(szTitle,  nFirstFormatLen,nFirstAign);
    //添加<2SP><=><2SP>报文数据中
        AddString(MML_FILL1);
    //拼接szContent中数据,可能多行,续行后第一个等号右边左对齐
        //by ldf 2003-03-06 对应问题单 D20433
        if ( 0 == nConLen)
        {
            AddString(MML_NULL,nSecondFormatLen,nSecondAlign);
        }
        else
        {
            AddString(szContent,nSecondFormatLen,nSecondAlign);
        }
        //over

    }

    //添加回车换行
    AddString(STR_NEWLINE);
}
////////////////////////////////////////////////////
//重载utility模块中的mml_report函数中的函数
//功能描述  将输入的一行报文裁成多行例如:
//    定位信息 =  定为信息1 = 定位信息1的值  定位信息2 = 定位信息2
//             =  值定位信息  定位信息3 = 定位信息3的值
//输入参数
//      sInput      -输入的报文
//      LEFT_MARGIN -左边留下的空格数
//      ROW_LENGTH  -每一行的长度
//输出参数
//      sOutPut     -输出的报文
//李杜芳 2002.1.15
////////////////////////////////////////////////////////
void CAlarmMMLReport::MultiLine(const char* szInput,
                                STRING &sOutput,
                                int LEFT_MARGIN,
                                int ROW_LENGTH)
{
    int nLen = strlen(szInput);
    unsigned char szTmp[1024];
    unsigned char* pszOutput = szTmp;
    int nLineLen = 0;
    int nIndex = 0;
    int nCharNum;
    unsigned char* pszLast;

    unsigned char* pszInput = (unsigned char *)szInput;
    nLineLen = ROW_LENGTH;

    //是否剩下的字符 ＋ 左边要求的空格 > 规定的一行的长度
    while ((nLen + nIndex) > ROW_LENGTH)
    {
        pszLast = (unsigned char *)(pszInput + nLineLen);
        nCharNum = 0;
        if ((*pszLast) > 0xA0)   //最后一个字符是汉字
        {
            do
            {
                pszLast--;
                if (*pszLast > 0xA0)
                    nCharNum++;
                else
                    break;
            } while (pszLast > pszInput);

            if ((nCharNum % 2) != 0)
                nLineLen --;
        }
        else if (isalpha(*pszLast) || isdigit(*pszLast))
        {
            do
            {
                pszLast--;
                if (isalpha(*pszLast) || isdigit(*pszLast))
                    nCharNum++;
                else
                    break;
            } while (pszLast > pszInput);
            nLineLen -= nCharNum;
        }

        if (nLineLen == 0)
        {
            nLineLen = ROW_LENGTH - LEFT_MARGIN;
        }

        memcpy(pszOutput, pszInput, nLineLen);
        pszOutput += nLineLen;
        pszInput  += nLineLen;
        nLen      -= nLineLen;

        while (*pszInput == ' ')
        {
            pszInput++;
            nLen--;
        }

        if (nLen == 0)
            break;

        pszOutput += sprintf((char *)pszOutput, STR_NEWLINE);
        memset(pszOutput, ' ', LEFT_MARGIN);
        pszOutput += LEFT_MARGIN ;
        nIndex = LEFT_MARGIN;
        nLineLen = ROW_LENGTH - LEFT_MARGIN;
    }

    memcpy(pszOutput, pszInput, nLen);
    *(pszOutput + nLen) = '\0';
    sOutput = (char *)szTmp;
}

//by ldf 2004-01-31 OM整改
void CAlarmMMLReport::SetMMLBaseAlarmID(UINT4 uMMLBaseID)
{
    m_uMMLBaseAlarmID = uMMLBaseID;
    return;
}
