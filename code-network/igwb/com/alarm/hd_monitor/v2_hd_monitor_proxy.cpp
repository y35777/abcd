/*     
    ��Ʒ��:iGateway Bill V200
    ģ����:�澯
    �ļ���:v2_hd_monitor_proxy.h
    ������Ϣ:
    
    ����CV2HDMonitorProxy���ʵ��,������ΪiGateway Bill V200Ӳ����⹦�ܵĴ���
    ���ڽ�Ӳ���澯ת����iGateway Bill V200�澯ģ�鴦��.

    �޸ļ�¼:
    ������,��2003-05 ,    ����.
    ������,  2003-11 ,    �Ż����룬����ע��.   
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

//��ʼ��
int CV2HDMonitorProxy::Init(const char *szCfgPath, int nPort)
{
    if ((szCfgPath == NULL) || (szCfgPath[0] == '\0'))
    {
        return false;
    }

    //�������ļ�
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

    //��ȡ�澯ת�����ò���
    char szSection[64] = "";
    char szKey[64] = "";
    char szVal[64] = "";

    //��ÿ���澯���л�ȡ�澯����
    int nAlmGrpCnt = ini.GetInt(S_CFG_ALARM_ID_LIST, S_CFG_NUMBER, 1);
    for(int nAlmGrp = 1; nAlmGrp <= nAlmGrpCnt; nAlmGrp++)
    {
        SNPRINTF(szSection, sizeof(szSection), 
                 "%s%d", S_CFG_ALARM_GROUP, nAlmGrp);
        szSection[sizeof(szSection) - 1] = '\0';
        
        //�ڲ��澯ID
        int nAlarmID = ini.GetInt(szSection, S_CFG_ALARM_ID, 0);
        
        //�����˸澯���в������ⲿ�澯�Ķ�Ӧ��ϵ
        char szParaName[64] = "";
        int nParaNum = ini.GetInt(szSection, S_CFG_ALARM_PARA_NUM, 0);
        for(int nPara = 1; nPara <= nParaNum; nPara++)
        {
            SNPRINTF(szKey, sizeof(szKey), 
                     "%d", nPara);
            szKey[sizeof(szKey) - 1] = '\0';

            ini.GetString(szSection, szKey, "", 
                          szParaName, sizeof(szParaName));

            //�����ڴ˲������͵������ⲿ�澯���ڲ��澯������������Ӧ��ϵ
            int nExAlmNum = ini.GetInt(szParaName, S_CFG_NUMBER, 0);
            for(int nExAlm = 1; nExAlm <= nExAlmNum; nExAlm++)
            {
                //�����ⲿ�澯ID
                SNPRINTF(szKey, sizeof(szKey), 
                         "%d", nExAlm);
                szKey[sizeof(szKey) - 1] = '\0';

                int nOriginID = ini.GetInt(szParaName,
                                           szKey, 0);
                m_AlarmOriginIDToFinalMap[nOriginID] = nAlarmID;
                m_AlarmOriginIDToParaMap[nOriginID] = nPara;

                //����澯����
                SNPRINTF(szKey, sizeof(szKey), 
                         "%d.1", nExAlm);
                szKey[sizeof(szKey) - 1] = '\0';
                ini.GetString(szParaName, szKey, "", 
                              szVal, sizeof(szVal));
                m_AlarmIDToExplainMap[nOriginID] = szVal;
            }
        }
    }
    
    //�ⲿ�澯���͵��ڲ��澯���͵�MAP
    m_WarnTypeToAlarmTypeMap[WARN_RESTORE] = AT_RESTORE;
    m_WarnTypeToAlarmTypeMap[WARN_FAULT] = AT_FAULT;
    m_WarnTypeToAlarmTypeMap[WARN_EVENT] = AT_EVENT;
    
    return CHDMonitorProxy::Init(szCfgPath, nPort);
}

//�澯������
int CV2HDMonitorProxy::ProcessAlarm(SWarnData &Alarm)
{
    
    //��Ӳ���ⲿ�澯ת��ΪV200���ڲ��澯��������ת��
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


