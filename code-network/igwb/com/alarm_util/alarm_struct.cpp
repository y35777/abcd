#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "../include/resource.h"
#include "alarm_config.h"
#include "../include/export_dll.h"
#include "alarm_struct.h"


STRING SAlarmDetail::GetAlarmFuncID()
{
    switch(yFuncID)
    {
    case AI_COMM_IN_MODULE:
        return STRING(S_ALARM_AI_COMM_IN_MODULE);
    case AI_CONTROL_SYSTEM:
        return STRING(S_ALARM_AI_CONTROL_SYSTEM);
    case AI_NETWORK_SYSTEM:
        return STRING(S_ALARM_AI_NETWORK_SYSTEM);
    case AI_COMM_IN_MAIN_NODE:
        return STRING(S_ALARM_AI_COMM_IN_MAIN_NODE);
    case AI_SIGNALING_SYSTEM:
        return STRING(S_ALARM_AI_SIGNALING_SYSTEM);
    case AI_TIME_SYSTEM:
        return STRING(S_ALARM_AI_TIME_SYSTEM);
    case AI_USER_SYSTEM:
        return STRING(S_ALARM_AI_USER_SYSTEM);
    case AI_TRUNK_SYSTEM:
        return STRING(S_ALARM_AI_TRUNK_SYSTEM);
    case AI_TEST_SYSTEM:
        return STRING(S_ALARM_AI_TEST_SYSTEM);
    case AI_POWER_SYSTEM:
        return STRING(S_ALARM_AI_POWER_SYSTEM);
    case AI_ENVIROMENT_SYSTEM:
        return STRING(S_ALARM_AI_ENVIROMENT_SYSTEM);
    case AI_TRANSFER_SYSTEM:
        return STRING(S_ALARM_AI_TRANSFER_SYSTEM);
    case AI_DATABASE_SYSTEM:
        return STRING(S_ALARM_AI_DATABASE_SYSTEM);
    case AI_BILLING_SYSTEM:
        return STRING(S_ALARM_AI_BILLING_SYSTEM);
    case AI_CALL_IDENTITY:
        return STRING(S_ALARM_AI_CALL_IDENTITY);
    case AI_SOFTWARE_RUNNING:
        return STRING(S_ALARM_AI_SOFTWARE_RUNNING);
    case AI_COMM_FROM_TO:
        return STRING(S_ALARM_AI_COMM_FROM_TO);
    default:
        return STRING(S_ALARM_NOUSE);
    }
}

STRING SAlarmDetail::GetModule()
{
    char tmp[8];
    sprintf(tmp,"%d",yModuleNO);
    return STRING(tmp);
}

STRING SAlarmDetail::GetAlarmName(CAlarmConfig *pConfig)
{
    PAlarmExplain ptmpAlarmExplain;
    if(pConfig == NULL)
    {
        return STRING(S_ALARM_CFG_NULL);
    }
    if(this->yAlarmType == AT_RESTORE)
    {
         ptmpAlarmExplain = pConfig->GetAlarmExplain(uAlarmID);
    }
    else
    {
        ptmpAlarmExplain = pConfig->GetAlarmExplain(uAlarmID);
    }

    if(ptmpAlarmExplain != NULL)
    {
        return STRING(ptmpAlarmExplain->szAlarmName);
    }
    else
    {
        return STRING(S_ALARM_NOUSE);
    }
}
STRING SAlarmDetail::GetAlarmParas()
{
    char tmp[MAX_ALARM_PARA];
    if(yPara[0] == 0xff)//没有参数
    {
        tmp[0] = '-';
        tmp[1] = 0;
    }
    else 
    {
        if(yPara[0] != 0xff)//有一个参数
        {
            sprintf(tmp,"%d",yPara[0]);
        }
        if(yPara[1] != 0xff)//有两个参数
        {
            int nLen = strlen(tmp);
            tmp[nLen] = ';';            
            strncpy(&tmp[nLen + 1], (char *)&yPara[1], MAX_ALARM_PARA - 1 - nLen);
        }
    }
    return STRING(tmp);
}

STRING SAlarmDetail::GetExplain(CAlarmConfig *pConfig)
{
    PAlarmExplain ptmpExplain;
    if(pConfig == NULL)
    {
        return STRING(S_ALARM_CFG_NULL);
    }
    if(this->yAlarmType == AT_RESTORE)
    {
        ptmpExplain = pConfig->GetAlarmExplain(uAlarmID);
    }
    else
    {
        ptmpExplain = pConfig->GetAlarmExplain(uAlarmID);

    }

    if(ptmpExplain != NULL)
    {
        return STRING(ptmpExplain->szDetail);
    }
    else
    {
        return STRING(S_ALARM_NOUSE);
    }

}


STRING SAlarmDetail::GetParaExplain(CAlarmConfig *pConfig)
{
    PAlarmExplain pTmpExplain;
    if(pConfig == NULL)
    {
        return STRING(S_ALARM_CFG_NULL);
    }
    if(this->yAlarmType == AT_RESTORE)
    {
        pTmpExplain = pConfig->GetAlarmExplain(uAlarmID);
    }
    else
    {
        pTmpExplain = pConfig->GetAlarmExplain(uAlarmID);

    }
    if(pTmpExplain != NULL)
    {
        return STRING(pTmpExplain->szParaExp);
    }
    else
    {
        return STRING(S_ALARM_NOUSE);
    }

}

STRING SAlarmDetail::GetFixExplain(CAlarmConfig *pConfig)
{

    PAlarmExplain pTmpExplain;
    if(pConfig == NULL)
    {
        return STRING(S_ALARM_CFG_NULL);
    }
    pTmpExplain = pConfig->GetAlarmExplain(uAlarmID);
    if(pTmpExplain != NULL)
    {
        return STRING(pTmpExplain->szFixAdvice);
    }
    else
    {
        return STRING(S_ALARM_NOUSE);
    }
}

STRING SAlarmDetail::GetAlarmType()
{
    switch(yAlarmType)
    {
    case AT_EVENT:
        return STRING(S_ALARM_AT_EVENT);
    case AT_FAULT:
        return STRING(S_ALARM_AT_FAULT);
    //by ldf 2004-01-09 OM整改
    case AT_RESTORE:
#ifdef _OM_MODIFY
        return STRING(S_ALARM_AT_FAULT);
#else
        return STRING(S_ALARM_AT_RESTORE);
#endif

    case AT_HISTORY:
        return STRING(S_ALARM_AT_HISTORY);
    default:
        return STRING(S_ALARM_AT_NOT_DEFINE);
    }
}

STRING SAlarmDetail::GetAlarmLevel()
{
    switch(yAlarmLevel)
    {
    case AL_EMERGENCY:
        return STRING(S_ALARM_AL_EMERGENCY);
    case AL_IMPORTANT:
        return STRING(S_ALARM_AL_IMPORTANT);
    case AL_MINOR:
        return STRING(S_ALARM_AL_MINOR);
    case AL_WARNING:
        return STRING(S_ALARM_AL_WARNING);
    default:
        return STRING(S_ALARM_NOUSE);
    }
}

STRING SAlarmDetail::GetAlarmSource()
{
    return STRING("iGWB");
}

STRING SAlarmDetail::GetAlarmID()
{
    char tmp[8];
    sprintf(tmp,"%d",uAlarmID);
    return STRING(tmp);
}

//by ldf 2004-01-31 OM整改
UINT4 SAlarmDetail::GetSYNNO()
{
    return uSyn;
}


STRING SAlarmDetail::GetAlarmSubSysID()
{
    switch(ySubSysID)
    {
    case AS_POWER_SYSTEM:
        return S_ALARM_AS_POWER_SYSTEM;
    case AS_ENVIROMENT_SYSTEM:
        return S_ALARM_AS_ENVIROMENT_SYSTEM;
    case AS_SIGNALING_SYSTEM:
        return S_ALARM_AS_SIGNALING_SYSTEM;
    case AS_TRUNK_SYSTEM:
        return S_ALARM_AS_TRUNK_SYSTEM;
    case AS_HARDWARE_SYSTEM:
        return S_ALARM_AS_HARDWARE_SYSTEM;
    case AS_SOFTWARE_SYSTEM:
        return S_ALARM_AS_SOFTWARE_SYSTEM;
    case AS_RUNNING_SYSTEM:
        return S_ALARM_AS_RUNNING_SYSTEM;
    default:
        return STRING(S_ALARM_NOUSE);
    }
}

STRING SAlarmDetail::GetAlarmTime()
{
    char tmp[20];
    sprintf(tmp,"%04d-%02d-%02d %02d:%02d:%02d",uYear,yMonth,yDay,yHour,yMin,ySec);
    return STRING(tmp);
}

////////////////
SAlarmDetail * SHistoryAlarm::GetAlarmDetail()
{
    return (SAlarmDetail *)(&this->uCsn);
}

STRING SHistoryAlarm::GetFAckTime()
{
    int nFlg = uFAckYear+yFAckMonth+yFAckDay+yFAckHour+yFAckMin+yFAckSec;
    if(nFlg == 0)
    {
        return STRING("");
    }
    char sztmpTime[20];
    sprintf(sztmpTime, "%04d-%02d-%02d %02d:%02d:%02d",uFAckYear, yFAckMonth,
        yFAckDay,yFAckHour,yFAckMin,yFAckSec);

    return STRING(sztmpTime);
}

STRING SHistoryAlarm::GetFAckOptr()
{
    return STRING(szFAckOptr);
}

STRING SHistoryAlarm::GetRAckOptr()
{
    return STRING(szRAckOptr);
}

STRING SHistoryAlarm::GetRecoverAckTime()
{
    int nFlg = uRAckYear+yRAckMonth+yRAckDay+yRAckHour+yRAckMin+yRAckSec;
    if(nFlg == 0)
    {
        return STRING("");
    }
    char sztmpTime[20];
    sprintf(sztmpTime, "%04d-%02d-%02d %02d:%02d:%02d",uRAckYear, yRAckMonth,
        yRAckDay,yRAckHour,yRAckMin,yRAckSec);
    return STRING(sztmpTime);
}

STRING SHistoryAlarm::GetAckStatus()
{
    char temp[78];
    memset(&temp[0],0,78);
    if(memcmp(szFAckOptr,temp,78) == 0)
    {
        return STRING(S_ALARM_NOACKVALUE);
    }
    else
    {
        return STRING(S_ALARM_ACKVALUE);
    }
}

STRING SHistoryAlarm::GetRecoverAckStatus()
{
    //by ldf 2002-10-29 对应问题单 D18685
    switch(yResType)
    {
    case AUTO_RECOVER:
        return STRING(S_ALARM_AUTO_RECOVER);
    case RESET_RECOVER:
        return STRING(S_ALARM_RESET_RECOVER);
    case MANNUAL_RECOVER:
        return STRING(S_ALARM_MANNUAL_RECOVER);
    default:
        return STRING(S_ALARM_NOT_RECOVER);
    }
    //修改完毕
}




STRING SHistoryAlarm::GetRevTime()
{
    int nFlg = uRYear+yRMonth+yRDay+yRHour+yRMin+yRSec;
    if(nFlg == 0)
    {
        return STRING("");
    }
    char tmp[20];
    sprintf(tmp,"%04d-%02d-%02d %02d:%02d:%02d",uRYear,yRMonth,yRDay,yRHour,yRMin,yRSec);
    return STRING(tmp);
}
