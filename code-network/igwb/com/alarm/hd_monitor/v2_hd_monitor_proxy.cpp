/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:v2_hd_monitor_proxy.h
    描述信息:
    
    包含CV2HDMonitorProxy类的实现,该类作为iGateway Bill V200硬件检测功能的代理，
    用于将硬件告警转发给iGateway Bill V200告警模块处理.

    修改记录:
    张玉新,　2003-05 ,    创建.
    陈凉威,  2003-11 ,    优化代码，增加注释.   
*/


#include "v2_hd_monitor_proxy.h"
#include "../resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CV2HDMonitorProxy::CV2HDMonitorProxy()
{

}

CV2HDMonitorProxy::~CV2HDMonitorProxy()
{

}

//初始化
int CV2HDMonitorProxy::Init(const char *szCfgPath, int nPort)
{
    if ((szCfgPath == NULL) || (szCfgPath[0] == '\0'))
    {
        return false;
    }

    //打开配置文件
    char szCvtFilePath[MAX_PATH] = "";
    strcpy(szCvtFilePath, szCfgPath);
    int nLen = strlen(szCvtFilePath);
    if((szCvtFilePath[nLen - 1] != '\\') 
       && (szCvtFilePath[nLen - 1] != '/'))
    {
        szCvtFilePath[nLen] = '/';
        szCvtFilePath[nLen + 1] = '\0';
    }    

    SNPRINTF(szCvtFilePath, sizeof(szCvtFilePath), 
             "%s%s",
             szCvtFilePath, S_ALARM_CONVERT_CFG_FILE);
    szCvtFilePath[sizeof(szCvtFilePath) - 1] = '\0';

    CINIFile ini(szCvtFilePath);
    if(!ini.Open())
    {
        char szErrInfo[100];
        SNPRINTF(szErrInfo, sizeof(szErrInfo), 
                 S_ALARM_HDMONI_OPEN_CVT_FILE_FAIL,
                 szCvtFilePath); 
        szErrInfo[sizeof(szErrInfo) - 1] = '\0';
        
        m_strError = szErrInfo;
        m_nErrno = ERR_FAIL;
        return ERR_FAIL;
    }

    //获取告警转换配置参数
    char szSection[64] = "";
    char szKey[64] = "";
    char szVal[64] = "";

    //从每个告警组中获取告警参数
    int nAlmGrpCnt = ini.GetInt(S_CFG_ALARM_ID_LIST, S_CFG_NUMBER, 1);
    for(int nAlmGrp = 1; nAlmGrp <= nAlmGrpCnt; nAlmGrp++)
    {
        SNPRINTF(szSection, sizeof(szSection), 
                 "%s%d", S_CFG_ALARM_GROUP, nAlmGrp);
        szSection[sizeof(szSection) - 1] = '\0';
        
        //内部告警ID
        int nAlarmID = ini.GetInt(szSection, S_CFG_ALARM_ID, 0);
        
        //建立此告警所有参数与外部告警的对应关系
        char szParaName[64] = "";
        int nParaNum = ini.GetInt(szSection, S_CFG_ALARM_PARA_NUM, 0);
        for(int nPara = 1; nPara <= nParaNum; nPara++)
        {
            SNPRINTF(szKey, sizeof(szKey), 
                     "%d", nPara);
            szKey[sizeof(szKey) - 1] = '\0';

            ini.GetString(szSection, szKey, "", 
                          szParaName, sizeof(szParaName));

            //将属于此参数类型的所有外部告警与内部告警及参数建立对应关系
            int nExAlmNum = ini.GetInt(szParaName, S_CFG_NUMBER, 0);
            for(int nExAlm = 1; nExAlm <= nExAlmNum; nExAlm++)
            {
                //读入外部告警ID
                SNPRINTF(szKey, sizeof(szKey), 
                         "%d", nExAlm);
                szKey[sizeof(szKey) - 1] = '\0';

                int nOriginID = ini.GetInt(szParaName,
                                           szKey, 0);
                m_AlarmOriginIDToFinalMap[nOriginID] = nAlarmID;
                m_AlarmOriginIDToParaMap[nOriginID] = nPara;

                //读入告警解释
                SNPRINTF(szKey, sizeof(szKey), 
                         "%d.1", nExAlm);
                szKey[sizeof(szKey) - 1] = '\0';
                ini.GetString(szParaName, szKey, "", 
                              szVal, sizeof(szVal));
                m_AlarmIDToExplainMap[nOriginID] = szVal;
            }
        }
    }
    
    //外部告警类型到内部告警类型的MAP
    m_WarnTypeToAlarmTypeMap[WARN_RESTORE] = AT_RESTORE;
    m_WarnTypeToAlarmTypeMap[WARN_FAULT] = AT_FAULT;
    m_WarnTypeToAlarmTypeMap[WARN_EVENT] = AT_EVENT;
    
    return CHDMonitorProxy::Init(szCfgPath, nPort);
}

//告警处理函数
int CV2HDMonitorProxy::ProcessAlarm(SWarnData &Alarm)
{
    
    //将硬件外部告警转换为V200的内部告警，并进行转发
    MAP<int,int>::iterator it = m_AlarmOriginIDToFinalMap.find(Alarm.AlarmId);
    if(it != m_AlarmOriginIDToFinalMap.end())
    {
        SInnerAlarm sAlarm;
        sAlarm.uAlarmID = it->second;
        sAlarm.yAlarmType = m_WarnTypeToAlarmTypeMap[Alarm.AlarmType];
        sAlarm.yAlarmPara[0] = m_AlarmOriginIDToParaMap[Alarm.AlarmId];

        CMsgProcessor::SendAlarm(&sAlarm);
    }
    
    return 0;
}


