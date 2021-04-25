/******************************************************************************

  版权所有 (C), 2001-2011, 华为技术有限公司
  
    ******************************************************************************
    文 件 名   : snmp_trap.h
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
#ifndef _SNMP_TRAP_H
#define _SNMP_TRAP_H

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4786)
#endif

#include "agent_define.h"
#include "snmp_pp.h"
#include <mib.h>

typedef MAP<Oidx, STRING> OIDXMap;

#define oidHwEnterprise      "1.1.1.1.1.1.2011"
#define DEFAULT_COMMUNITY    "public"
#define DEFAULT_PORT         162       
     
class CSNMPTrap  
{
public:
    void UnrigisterServer(const char *szIpAddr,int nPort = 162);
    int RegisterServer(const char *szIpAddr,int nPort = 162);
    int CleanOid();
    Oid m_company_id;      
    OctetStr m_community;     // community name    

    virtual int Init(const char *szCommunity = DEFAULT_COMMUNITY);
    CSNMPTrap(const char *szIPAddr = "0.0.0.0");
    virtual ~CSNMPTrap();
    int SetUpOid(const char *szOid, const char *szVal);
    int SetUpOid(const char *szOid, int nVal);
    int SetUpPdu();
    int SendTrap(const char *szTrapId, const char *szDest, int nPeerPort = 162,const char *EnterpriseId = oidHwEnterprise);
    int CleanPdu();
    const char *GetLastError()
    {
        return m_strError.c_str();
    }
protected:
    //don't change the order of variable, 
    //it must be consistent with member construction list in construct function  
    UdpAddress m_LocalIP;
    int m_status;             
    Snmp m_snmp;              
    snmp_version m_version;   // default is v1
    STRING m_strError; 
    Pdu m_pdu;
    Vb m_vb;
    OIDXMap m_OidToValMap; 
    LIST<UdpAddress> m_address_list;     // make a SNMP++ Generic address
};

#endif //_SNMP_TRAP_H
