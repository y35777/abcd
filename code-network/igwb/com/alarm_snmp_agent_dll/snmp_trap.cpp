/******************************************************************************

  版权所有 (C), 2001-2011, 华为技术有限公司
  
    ******************************************************************************
    文 件 名   : snmp_trap.cpp
    版 本 号   : 初稿
    作    者   : 张玉新
    生成日期   : 2003年4月12日
    最近修改   :
    功能描述   : 发送SNMP V1 TRAP
    函数列表   :
    修改历史   :
    1.日    期   : 2003年4月12日
    作    者   : 张玉新
    修改内容   : 创建文件
    
******************************************************************************/
#include "snmp_trap.h"
#include  <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSNMPTrap::CSNMPTrap(const char *szIPAddr):
                      m_LocalIP(szIPAddr),
                      m_snmp(m_status,m_LocalIP),
                      m_version(version1),
                      m_company_id(oidHwEnterprise),
                      m_community(DEFAULT_COMMUNITY)
{
}

CSNMPTrap::~CSNMPTrap()
{
}

int CSNMPTrap::Init(const char *szCommunity)
{
    m_address_list.clear();
    if (m_status != SNMP_CLASS_SUCCESS) {
        m_strError = S_ALARM_SNMP_SESSION_CRT_FAIL;
        m_strError += m_snmp.error_msg(m_status);
        return -1;
    }
    m_community = szCommunity;
    return 0;
}
int CSNMPTrap::SetUpOid(const char *szOid, const char *szVal)
{
    if(szOid == NULL || szVal == NULL)
    {
        m_strError = S_ALARM_OID_VALUE_IS_NULL;
        return -1;
    }

    if(m_OidToValMap.size() > 50)
    {
        m_strError = S_ALARM_OID_NUMBER_TOO_MANY;
        return -1;
    }

    m_OidToValMap[Oidx(szOid)] = szVal;
    return 0;
}
int CSNMPTrap::SetUpOid(const char *szOid, int nVal)
{
    std::stringstream ss;
    ss << nVal;
    return SetUpOid(szOid,ss.str().c_str());
}

int CSNMPTrap::SetUpPdu()
{
    OIDXMap::iterator it = m_OidToValMap.begin();
    OIDXMap::iterator et = m_OidToValMap.end();
    CleanPdu();
    for(; it != et; it++)
    {
        m_vb.set_oid(it->first);
        m_vb.set_value(it->second.c_str());
        if(!m_vb.valid())
        {
            char szErrInfo[100];
            SNPRINTF(szErrInfo, sizeof(szErrInfo), 
                     S_ALARM_SET_VB_ERR,
                     it->first.as_string().get_printable(),
                     it->second.c_str()); 
            szErrInfo[sizeof(szErrInfo) - 1] = '\0';

            m_strError = szErrInfo;
            return -1;
        }
        m_pdu += m_vb;
        if(!m_pdu.valid())
        {
            m_strError = S_ALARM_SET_PDU_ERR;
            return -1;
        }
    }
    return 0;
}
int CSNMPTrap::CleanPdu()
{
    return m_pdu.set_vblist(&m_vb,0);
}
int CSNMPTrap::SendTrap(const char *szTrapId, const char *szDest, int nPeerPort,const char *EnterpriseId)
{
    SetUpPdu();
    int nRet = 0;
    m_pdu.set_notify_id(szTrapId);               // set the id of the trap
    m_pdu.set_notify_enterprise(EnterpriseId);   // set up the enterprise of the trap
    UdpAddress address(szDest);
    address.set_port(nPeerPort);
    SnmpTarget *target = NULL;
    CTarget ctarget;             // make a target using the address
    ctarget.set_version(m_version);         // set the SNMP version SNMPV1 or V2
    ctarget.set_readcommunity(m_community); // set the read community name
    if(address.valid())
    {
        ctarget.set_address(address);
        //-------[ Send the trap  ]-------------------------------------------    
        target = &ctarget;
        m_status = m_snmp.trap(m_pdu,*target);
        if (m_status != SNMP_CLASS_SUCCESS) 
        {
            char szErrInfo[100];
            SNPRINTF(szErrInfo, sizeof(szErrInfo), 
                     S_ALARM_SEND_TRAP_FAIL,
                     address.get_printable(),
                     m_snmp.error_msg(m_status)); 
            szErrInfo[sizeof(szErrInfo) - 1] = '\0';
            
            m_strError = szErrInfo;
            nRet = -1;
        }
    }
    else
    {
        
        LIST<UdpAddress>::iterator it,et;
        et = m_address_list.end();
        for(it = m_address_list.begin(); it != et; it++)
        {
            if(!(it->valid()))
            {   // check validity of address
                char szErrInfo[100];
                SNPRINTF(szErrInfo, sizeof(szErrInfo), 
                         S_ALARM_INVALID_ADRR_INFO,
                         it->get_printable());
                szErrInfo[sizeof(szErrInfo) - 1] = '\0';

                m_strError = szErrInfo;
                nRet = -1;
                continue;
            }
            ctarget.set_address(*it);
            //-------[ Send the trap  ]-------------------------------------------    
            target = &ctarget;
            m_status = m_snmp.trap(m_pdu,*target);
            if (m_status != SNMP_CLASS_SUCCESS) 
            {
                char szErrInfo[100];
                SNPRINTF(szErrInfo, sizeof(szErrInfo), 
                         S_ALARM_SEND_TRAP_FAIL,
                         it->get_printable(),
                         m_snmp.error_msg(m_status)); 
                szErrInfo[sizeof(szErrInfo) - 1] = '\0';

                m_strError = szErrInfo;
                nRet = -1;
                continue;
            }
        }
    }
    return nRet;
}
    

int CSNMPTrap::CleanOid()
{
    m_OidToValMap.clear();
    return 0;
}

int CSNMPTrap::RegisterServer(const char *szIpAddr,int nPort)
{
    UdpAddress udp_addr(szIpAddr);
    udp_addr.set_port(nPort);
    if(m_address_list.size() < 50)
    {
        m_address_list.push_back(udp_addr);
        return 0;
    }
    else
        m_strError = S_ALARM_REG_SVR_TOO_MANY;
    return -1;
}


void CSNMPTrap::UnrigisterServer(const char *szIpAddr,int nPort)
{
    UdpAddress udp_addr(szIpAddr);
    udp_addr.set_port(nPort);
    m_address_list.remove(udp_addr);
}
