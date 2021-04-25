/*     
    产品名:iGateway Bill V200
    模块名:告警
    文件名:alarm_snmp_agent.cpp
    描述信息:
    
    包含CSnmpAgent类的定义,该类Snmp告警代理的功能，
    用于NMS进行告警查询，以及调用模块进行告警上报。

    包含CErrorTrap，实现错误信息Trap上报功能。

    包含CAlarmTrap，实现告警上报及告警结果上报功能。


    修改记录:
    张玉新,  2003-08 ,   创建.
    陈凉威， 2003-11 ，  将之封装为动态库，并优化代码，增加注释.
*/

#include "../include/toolbox.h"
#include "../include/frame.h"
#include "agent_define.h"
#include "../include/export_dll.h"
#include "alarm_snmp_agent.h"

IAlarmSnmpAgent* GetSnmpAgent()
{
    return (IAlarmSnmpAgent*)CAlarmSnmpAgent::Instance();
}

void Destruct()
{
    if (NULL != CAlarmSnmpAgent::Instance())
    {
        CAlarmSnmpAgent::Instance()->Destroy();
    }
}

CAlarmSnmpAgent *CAlarmSnmpAgent::s_pInstance = NULL;

/*
 * 接口函数实现部分。
 */
//创建Agent线程
int CAlarmSnmpAgent::StartThread(const char *szUdp, const char* szFilePathRoot)
{
   int nRet = Init(szUdp, szFilePathRoot);
   if (nRet != ERR_SUCCESS)
       return nRet;

   return open();
}

//退出Agent线程
int CAlarmSnmpAgent::ExitThread()
{
    m_bRun = false;
    return wait();
}

//注册函数
int CAlarmSnmpAgent::RegisterFunc(QUERYFUNC p)
{
    if (p == NULL)
        return ERR_FAIL;

    m_pQueryAlarm = p;
    return ERR_SUCCESS;
}


//发送告警到指定NMS
//当ip_addr为“”时表示向所有的NMS发送
int CAlarmSnmpAgent::ReportAlarm(SHistoryAlarm  *pAlarm,const char *szIpAddr, int nPort)
{
    return m_pAlarmTrap->ReportAlarm(pAlarm, szIpAddr, nPort);
}

//上报一个错误信息
int CAlarmSnmpAgent::SendErrorInfo(const char *szError, const char *szIpAddr, int nPort)
{
    return m_pErrorTrap->SendError(szError,szIpAddr,nPort); 
}

/*
 * 内部函数实现部分。
 */
//构造函数
CAlarmSnmpAgent::CAlarmSnmpAgent()

{
    m_bRun = true;              //监听循环的条件变量

    m_nNMSCnt  = 0;             //NMS计数
    m_nQueryCondCnt = 0;        //查询条件计数

    m_pErrorTrap = NULL;        //错误Trap
    m_pAlarmTrap = NULL;        //告警Trap
    m_strError = "";            //错误

    m_nSnmpStatus = 0;
    m_pSnmp = NULL;
    
}

//析构函数
CAlarmSnmpAgent::~CAlarmSnmpAgent()
{
    //删除SNMP对象
    if(m_pSnmp != NULL)
    {
        delete m_pSnmp;
        m_pSnmp = NULL;
    }

    //删除错误Trap
    if(m_pErrorTrap != NULL)
    {
        delete m_pErrorTrap;
        m_pErrorTrap = NULL;
    }

    //删除告警Trap
    if(m_pAlarmTrap != NULL)
    {
        delete m_pAlarmTrap;
        m_pAlarmTrap = NULL;
    }
}


//得到函数的实例
CAlarmSnmpAgent* CAlarmSnmpAgent::Instance()
{
    if(s_pInstance == NULL)
    {
        s_pInstance = new CAlarmSnmpAgent();
    }

    return s_pInstance;
}

//初始化函数
int CAlarmSnmpAgent::Init(const char *szUdp, const char *szFilePathRoot)
{
    m_UdpAddr = szUdp;
    m_pSnmp = new Snmpx(m_nSnmpStatus, m_UdpAddr);

    CreateDeepDir(szFilePathRoot);

    m_NMSFilePath = szFilePathRoot;
    m_NMSFilePath += NMS_STATUS_FILE;
    m_QueryStatusFilePath = szFilePathRoot;
    m_QueryStatusFilePath += QUERY_STATUS_FILE;

    if(m_nSnmpStatus != SNMP_CLASS_SUCCESS)
    {
        m_strError = S_ALARM_SNMP_SESSION_CRT_FAIL;
        m_strError += m_pSnmp->error_msg(m_nSnmpStatus);
        return -1;
    }

    //初始化RequestList对象
    m_Mib.set_request_list(&m_ReqList);

    //初始化Mib对象
    OctetStr sysDescr(S_ALARM_COMPANY_NAME);
    sysDescr += S_IGATEWAY_BILL_VER;
    m_Mib.add(new sysGroup(sysDescr.get_printable(), oidHuawei, 10));
    m_Mib.add(new snmpGroup());
    m_Mib.add(new snmp_target_mib());
    //Not added by default:
    //mib.add(new snmp_community_mib());
    m_Mib.add(new snmp_notification_mib());
    m_Mib.add(new huawei_snmpalarm_mib());
    m_Mib.init();
    m_ReqList.set_snmp(m_pSnmp);

    m_pErrorTrap = new CErrorTrap();
    m_pErrorTrap->Init();

    m_pAlarmTrap = new CAlarmTrap();
    m_pAlarmTrap->Init();

    int  i = 0;
    char szKey[VALLEN] = "";
    char szNMSName[VALLEN] = "";
    char szValue[VALLEN] = "";
    int  nVal = 0;

    //打开保存NMS信息的INI文件，将NMS信息加入到NmsTable中，
    CINIFileEx  iniNMS(m_NMSFilePath.c_str());
    if(iniNMS.Open())
    {
        m_nNMSCnt = iniNMS.GetInt(HEAD, NUMBER, 0);
        for(i = 1; i <= m_nNMSCnt; i++)
        {
            SNPRINTF(szKey, sizeof(szKey), "%d", i);
            iniNMS.GetString(HEAD, szKey, "", 
                             szNMSName, sizeof(szNMSName));

            if(szNMSName[0] == '\0')
            {
                m_strError = S_ALARM_FIND_ERR_NMS_FILE;
                return -1;
            }
            
            //将NMS信息加入到NmsTable中
            Oidx oidRowIndex = Oidx::from_string(szNMSName);
            MibTableRow *pRow = nmsEntry::s_pInstance->add_row(oidRowIndex);

            iniNMS.GetString(szNMSName, NMS_IP, "", 
                             szValue, sizeof(szValue));
            pRow->get_nth(0)->set_value(OctetStr(szValue));

            nVal = iniNMS.GetInt(szNMSName, NMS_PORT, 0);
            pRow->get_nth(1)->set_value(nVal);

            pRow->get_nth(2)->set_value(rowActive); //rowStatus
        }
    }

    //打开保存查询条件的INI文件，将查询条件加入到查询条件表中
    CINIFileEx iniCond(m_QueryStatusFilePath.c_str());
    if(iniCond.Open())
    {
        m_nQueryCondCnt = iniCond.GetInt(HEAD, NUMBER, 0);
        for(i = 1; i <= m_nQueryCondCnt; i++)
        {
            SNPRINTF(szKey, sizeof(szKey), "%d", i);
            iniCond.GetString(HEAD, szKey, "", 
                             szNMSName, sizeof(szNMSName));

            if(szNMSName[0] == '\0')
            {
                m_strError = S_ALARM_FIND_ERR_QUERY_COND_FILE;
                return -1;
            }

            //将查询条件加入到查询条件表中
            Oidx oidRowIndex = Oidx::from_string(szNMSName);
            MibTableRow *pRow = alarmQueryEntry::s_pInstance->add_row(oidRowIndex);

            iniCond.GetString(szNMSName, QUERY_TYPE, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(0)->set_value(OctetStr(szValue));

            iniCond.GetString(szNMSName, QUERY_RES_FLAG, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(1)->set_value(OctetStr(szValue));

            iniCond.GetString(szNMSName, QUERY_ACK_FLAG, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(2)->set_value(OctetStr(szValue));
            
            nVal = iniCond.GetInt(szNMSName, QUERY_MODULE_NUM, 0);
            pRow->get_nth(3)->set_value(nVal);

            iniCond.GetString(szNMSName, QUERY_CSN_SCOPE, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(4)->set_value(OctetStr(szValue));

            iniCond.GetString(szNMSName, QUERY_ID_SCOPE, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(5)->set_value(OctetStr(szValue));

            iniCond.GetString(szNMSName, QUERY_LEVEL, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(6)->set_value(OctetStr(szValue));

            iniCond.GetString(szNMSName, QUERY_SUBSYS_ID, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(7)->set_value(OctetStr(szValue));
            
            iniCond.GetString(szNMSName, QUERY_DATE_SCOPE, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(8)->set_value(OctetStr(szValue));
            
            iniCond.GetString(szNMSName, QUERY_TIME_SCOPE, "", 
                              szValue, sizeof(szValue));
            pRow->get_nth(9)->set_value(OctetStr(szValue));

            nVal = iniCond.GetInt(szNMSName, QUERY_RETURN_CNT, 0);
            pRow->get_nth(10)->set_value(nVal);
            
            pRow->get_nth(11)->set_value(rowActive); //rowStatus
        }
    }
    
    return ERR_SUCCESS;
}

//清除函数
void CAlarmSnmpAgent::Destroy()
{
    delete this;
    s_pInstance = NULL;
}

//打开Agent线程
int CAlarmSnmpAgent::open(void* args)
{
    //调用Activate
    if (this->activate())
    {
        m_strError = S_ALARM_START_SNMPAGENTTHD_FAIL;
        m_strError += ACE_OS::strerror(ACE_OS::last_error());
        return ERR_FAIL;
    }
    
    return ERR_SUCCESS;
}

//Agent线程主函数
int CAlarmSnmpAgent::svc()
{
    Request* req;
    while (m_bRun)
    {
        req = m_ReqList.receive(5);     
        if (req)
        {
            m_Mib.process_request(req);
        }
        else
        {
            m_Mib.cleanup();
        }
    }
    
    return 0;
}


//增加网管中心
int CAlarmSnmpAgent::AddNmsInfo(SNMSInfo &nms)
{
    /*
     * 更新NMS的INI文件
     */
    CINIFileEx iniNMS(m_NMSFilePath.c_str());
    if(!iniNMS.Open())
    {
        m_strError = S_ALARM_OPEN_NMS_STATUS_FILE_FAIL;
        m_strError += strerror(errno);
        return -1;
    }

    m_nNMSCnt++;

    char szNMSName[32] = "";
    strncpy(szNMSName, nms.strName.c_str(), sizeof(szNMSName));
    szNMSName[sizeof(szNMSName) - 1] = '\0';

    //更新INI文件中的[head]段
    char szKey[VALLEN] = "";
    SNPRINTF(szKey, sizeof(szKey), "%d", m_nNMSCnt);
    szKey[sizeof(szKey) - 1] = '\0';

    iniNMS.WriteInt(HEAD, NUMBER, m_nNMSCnt);
    iniNMS.WriteString(HEAD, szKey, szNMSName);

    //在INI文件中增加一个NMS
    iniNMS.WriteString(szNMSName, NMS_IP, (char*)nms.strNmsIP.c_str());
    iniNMS.WriteInt(szNMSName, NMS_PORT, nms.nNmsPort);

    //在AlarmTrap中注册NMS对应的IP和端口
    m_pAlarmTrap->RegisterServer(nms.strNmsIP.c_str(),nms.nNmsPort);

    /*
     * 更新QueryCond的INI文件
     */
    CINIFileEx iniCond(m_QueryStatusFilePath.c_str());
    if(iniCond.Open())
    {
        //更新INI文件中的[head]段
        iniCond.WriteInt(HEAD, NUMBER, m_nNMSCnt);
        iniCond.WriteString(HEAD, szKey, szNMSName);

        //在INI文件中增加一个NMS
        iniCond.WriteString(szNMSName, QUERY_TYPE, "");
        iniCond.WriteString(szNMSName, QUERY_RES_FLAG, "");
        iniCond.WriteString(szNMSName, QUERY_ACK_FLAG, "");
        iniCond.WriteString(szNMSName, QUERY_MODULE_NUM, "");
        iniCond.WriteString(szNMSName, QUERY_CSN_SCOPE, "");
        iniCond.WriteString(szNMSName, QUERY_ID_SCOPE, "");
        iniCond.WriteString(szNMSName, QUERY_LEVEL, "");
        iniCond.WriteString(szNMSName, QUERY_SUBSYS_ID, "");
        iniCond.WriteString(szNMSName, QUERY_DATE_SCOPE, "");
        iniCond.WriteString(szNMSName, QUERY_TIME_SCOPE, "");
        iniCond.WriteString(szNMSName, QUERY_RETURN_CNT, "");
    }

    return ERR_SUCCESS;
}

//删除网管中心
int CAlarmSnmpAgent::DeleteNMS(const char *szNMSName)
{
    //获得NMS列表
    LIST<STRING> nms_list;
    if(ERR_SUCCESS != nmsEntry::s_pInstance->get_nms_list(nms_list))
    {
        m_strError = S_ALARM_GET_NMS_LIST_FAIL;
        m_strError += strerror(errno);
        return errno;
    }

    //删除列表中的NMS
    LIST<STRING>::iterator it;
    it = std::find(nms_list.begin(), 
                   nms_list.end(), 
                   STRING(szNMSName));
    if(it != nms_list.end())
    {
        nms_list.erase(it);
    }
    else
    {
        m_strError = S_ALARM_NMS_NOT_EXIST;
        return -1;
    }

    /*
     * 更新INI文件
     */
    
    //构造ini文件中的[head]段
    std::stringstream ss;
    ss << '[' << HEAD << ']' << std::endl;
    ss << NUMBER << '=' << nms_list.size() << std::endl;
    int i = 1;
    for(it = nms_list.begin(); it != nms_list.end(); i++, it++)
    {
        ss << i << '=' << (*it) << std::endl;
    }

    CINIFileEx iniNMS(m_NMSFilePath.c_str());
    if(iniNMS.Open())
    {
        //注销AlarmTrap中NMS对应的IP和端口
        char szIp[32] = "";
        int nPort = 0;
        iniNMS.GetString(szNMSName, NMS_IP, 
                         "", szIp, 32);
        nPort = iniNMS.GetInt(szNMSName, NMS_PORT, 0);
        m_pAlarmTrap->UnrigisterServer(szIp, nPort);

        //更新NMS的INI文件
        char szSection[VALLEN] = "";
        SNPRINTF(szSection, sizeof(szSection), 
                 "[%s]", HEAD);
        szSection[sizeof(szSection) - 1] = '\0';
        iniNMS.ReplaceSecWithStr(szSection, ss.str().c_str());

        SNPRINTF(szSection, sizeof(szSection), 
                 "[%s]", szNMSName);
        szSection[sizeof(szSection) - 1] = '\0';
        iniNMS.DelSection(szSection);
        m_nNMSCnt = nms_list.size();

        //更新QueryCond的INI文件
        CINIFileEx iniCond(m_QueryStatusFilePath.c_str());
        if(iniCond.Open())
        {
            SNPRINTF(szSection, sizeof(szSection),
                     "[%s]",HEAD);
            szSection[sizeof(szSection) - 1] = '\0';
            iniCond.ReplaceSecWithStr(szSection, ss.str().c_str());

            SNPRINTF(szSection, sizeof(szSection), 
                     "[%s]", szNMSName);
            szSection[sizeof(szSection) - 1] = '\0';
            iniCond.DelSection(szSection);
            m_nQueryCondCnt = m_nNMSCnt;
        }
    }

    return ERR_SUCCESS;
}

//更改网管中心信息
int CAlarmSnmpAgent::ModifyNMSInfo(const char *szNMSName, const char *szKeyName, const char *szValue)
{
    CINIFileEx iniNMS(m_NMSFilePath.c_str());
    if(!iniNMS.Open())
    {
        m_strError = S_ALARM_OPEN_NMS_STATUS_FILE_FAIL;
        m_strError += strerror(errno);
        return -1;
    }

    //注销AlarmTrap中老的NMS
    char szIP[32] = "";
    int  nPort = 0;
    iniNMS.GetString(szNMSName, NMS_IP, "", 
                     szIP, sizeof(szIP));
    nPort = iniNMS.GetInt(szNMSName, NMS_PORT, 0);
    m_pAlarmTrap->UnrigisterServer(szIP, nPort);

    //更新INI文件
    iniNMS.WriteString(szNMSName, szKeyName, (char*)szValue);

    //注册AlarmTrap中新的NMS
    iniNMS.GetString(szNMSName, NMS_IP, "", 
                     szIP, sizeof(szIP));
    nPort = iniNMS.GetInt(szNMSName, NMS_PORT, 0);
    m_pAlarmTrap->RegisterServer(szIP, nPort);        

    return ERR_SUCCESS;
}

//更改网管中心信息
int CAlarmSnmpAgent::ModifyNMSInfo(const char *szNMSName, const char *szKeyName, int nVal)
{
    std::stringstream ss;
    ss << nVal;

    return ModifyNMSInfo(szNMSName, szKeyName, ss.str().c_str());
}

//增加查询条件
int CAlarmSnmpAgent::AddQueryCondition(SQueryStatus &condition, const char *szNMSName)
{
    CINIFileEx ini(m_QueryStatusFilePath.c_str());
    if(ini.Open())
    {
        m_nQueryCondCnt++;
        char szKey[VALLEN] = "";
        char szVal[VALLEN] = "";
        char szSection[VALLEN] = "";

        strncpy(szSection, szNMSName, sizeof(szSection));
        szSection[sizeof(szSection) - 1] = '\0';
        ini.WriteInt(HEAD, NUMBER, m_nQueryCondCnt);

        SNPRINTF(szKey, sizeof(szKey), 
                 "%d", m_nQueryCondCnt);
        strncpy(szVal, szNMSName, sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(HEAD, szKey, &szVal[0]);
        
        strncpy(szVal, condition.strQueryType.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0], QUERY_TYPE, &szVal[0]);

        strncpy(szVal, condition.strQueryRestoreFlag.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0], QUERY_RES_FLAG, &szVal[0]);

        strncpy(szVal, condition.strQueryAckFlag.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0], QUERY_ACK_FLAG, &szVal[0]);
        ini.WriteInt(&szSection[0], QUERY_MODULE_NUM, condition.nQueryModuleNumber);
        
        strncpy(szVal, condition.strQueryCsnScope.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0], QUERY_CSN_SCOPE, &szVal[0]);

        strncpy(szVal, condition.strQueryIDScope.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0],QUERY_ID_SCOPE,&szVal[0]);

        strncpy(szVal, condition.strQueryLevel.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0],QUERY_LEVEL,&szVal[0]);

        strncpy(szVal, condition.strQuerySubsystemID.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0],QUERY_SUBSYS_ID,&szVal[0]);

        strncpy(szVal, condition.strQueryDateScope.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0],QUERY_DATE_SCOPE,&szVal[0]);

        strncpy(szVal, condition.strQueryTimeScope.c_str(), sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';
        ini.WriteString(&szSection[0],QUERY_TIME_SCOPE,&szVal[0]);

        ini.WriteInt(&szSection[0], QUERY_RETURN_CNT, condition.nReturnCsn);
    }
    else
    {
        m_strError = S_ALARM_OPEN_QUERY_STATUS_FILE_FAIL;
        m_strError += strerror(errno);
        return -1;
    }
    return 0;
}

//删除查询条件
int CAlarmSnmpAgent::DelQueryCondition(const char *szNMSName)
{ 
    LIST<STRING> nms_list;
    if(!nmsEntry::s_pInstance->get_nms_list(nms_list))
    {
        LIST<STRING>::iterator it,et;
        et = nms_list.end();
        it = std::find(nms_list.begin(),et,STRING(szNMSName));
        if(it == et)
        {
            m_strError = S_ALARM_NMS_NOT_EXIST;
            return -1;
        }

        std::stringstream ss;
        ss << '[' << HEAD << ']' << std::endl;
        ss << NUMBER << '=' << nms_list.size() << std::endl;
        int i = 1;
        for(it = nms_list.begin(); it != et; i++, it++)
        {
            ss << i << '=' << (*it) << std::endl;
        }
        
        char szSection[VALLEN];
        CINIFileEx ini(m_QueryStatusFilePath.c_str());
        if(ini.Open())
        {
            SNPRINTF(szSection, sizeof(szSection), "[%s]", HEAD);
            szSection[sizeof(szSection) - 1] = '\0';
            ini.ReplaceSecWithStr(szSection,ss.str().c_str());
            SNPRINTF(szSection, sizeof(szSection), "[%s]",szNMSName);
            ini.DelSection(szSection);
            m_nQueryCondCnt = nms_list.size();
        }
        return 0;
    }
    
    m_strError = S_ALARM_GET_NMS_LIST_FAIL;
    m_strError += strerror(errno);
    return -1;
}

//更改查询条件
int CAlarmSnmpAgent::ModifyQueryCondition(const char *strName,const char *strKeyName, const char *strValue)
{
    CINIFileEx ini(m_QueryStatusFilePath.c_str());
    if(ini.Open())
    {
        char szKey[VALLEN] = "";
        char szVal[VALLEN] = "";
        char szSection[VALLEN] = "";

        strncpy(szSection, strName, sizeof(szSection));
        szSection[sizeof(szSection) - 1] = '\0';

        strncpy(szKey, strKeyName, sizeof(szKey));
        szKey[sizeof(szKey) - 1] = '\0';

        strncpy(szVal, strValue, sizeof(szVal));
        szVal[sizeof(szVal) - 1] = '\0';

        ini.WriteString(szSection, szKey, szVal);
    }
    else
    {
        m_strError = S_ALARM_OPEN_NMS_STATUS_FILE_FAIL;
        m_strError += strerror(errno);
        return -1;
    }
    return 0;
}

//更改查询条件
int CAlarmSnmpAgent::ModifyQueryCondition(const char *strName,const char *strKeyName, int nValue)
{
    CINIFileEx ini(m_QueryStatusFilePath.c_str());
    if(ini.Open())
    {
        char szKey[VALLEN] = "";
        char szVal[VALLEN] = "";
        char szSection[VALLEN] = "";

        strncpy(szSection, strName, sizeof(szSection));
        szSection[sizeof(szSection) - 1] = '\0';

        strncpy(szKey, strKeyName, sizeof(szKey));
        szKey[sizeof(szKey) - 1] = '\0';

        ini.WriteInt(szSection, szKey, nValue);
        ini.Close();
    }
    else
    {
        m_strError = S_ALARM_OPEN_NMS_STATUS_FILE_FAIL;
        m_strError += strerror(errno);
        return -1;
    }
    return 0;
}


//开始查询告警
int CAlarmSnmpAgent::QueryAlarm(SAlarmQueryCond &condition, const char *szIpAddr, int nPort)
{
    LIST<SHistoryAlarm> ResultList;
    m_pQueryAlarm(condition, ResultList);
    
    if(!ResultList.empty())
    {
        LIST<SHistoryAlarm>::iterator it,et;
        et = ResultList.end();
        for(it = ResultList.begin(); it != et; it++)
        {
            ReportAlarm(&(*it),szIpAddr,nPort);
        }
    }
    else
    {
        SendErrorInfo("No result",szIpAddr,nPort);
    }
    return 0;
}



/*
 * 以下为CErrorTrap和CAlarmTrap的实现
 */

CErrorTrap::CErrorTrap(const char *szIPAddr):CSNMPTrap(szIPAddr)
{
}

//发送故障Trap
int CErrorTrap::SendError(const char *szError, const char *szIpAdder, int nPort)
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex,guard,m_mutex,-1);
    SetUpOid(oidErrnoInformation,szError);
    return SendTrap(oidErrorTrap,szIpAdder,nPort);
}

CAlarmTrap::CAlarmTrap(const char *szIPAddr):CSNMPTrap(szIPAddr)
{
}

//发送告警Trap
int CAlarmTrap::ReportAlarm(SHistoryAlarm  *pAlarm, const char *szIpAdder, int nPort)
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, m_mutex, -1);

    //设置Trap中叶子节点的值
    SetUpOid(oidAlarmCsn, pAlarm->uCsn);    
    SetUpOid(oidAlarmID, (int)(pAlarm->uAlarmID)); 
    SetUpOid(oidAlarmType, pAlarm->GetAlarmDetail()->GetAlarmType().c_str());
    SetUpOid(oidAlarmLevel,pAlarm->GetAlarmDetail()->GetAlarmLevel().c_str());
    SetUpOid(oidAlarmModuleNumber,(int)(pAlarm->yModuleNO));
    SetUpOid(oidFunctionID,pAlarm->GetAlarmDetail()->GetAlarmFuncID().c_str());
    SetUpOid(oidAlarmSubsystemID,pAlarm->GetAlarmDetail()->GetAlarmSubSysID().c_str());
    SetUpOid(oidAlarmDate,pAlarm->GetAlarmDetail()->GetAlarmTime().c_str());
    SetUpOid(oidCpuPosition,(int)(pAlarm->yCPUPos));  
    SetUpOid(oidProcesID,(int)(pAlarm->yPID));  
    SetUpOid(oidAlarmParameter,(int)(pAlarm->yPara));  
    SetUpOid(oidAlarmRestoreType,pAlarm->GetRecoverAckStatus().c_str()); 
    SetUpOid(oidRestoreDate,pAlarm->GetRevTime().c_str());
    SetUpOid(oidAcknowledgementOperator,pAlarm->GetFAckOptr().c_str()); 
    SetUpOid(oidAcknowledgementDate,pAlarm->GetFAckTime().c_str());
    return SendTrap(oidAlarmTrap,szIpAdder,nPort);
}

