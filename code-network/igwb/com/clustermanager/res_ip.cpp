#include "../include/frame.h"
#include "res_ip.h"
#include "Iphlpapi.h"
#include "ping.h"
#include "resource.h"
#include "BtwIpHelper.h"
#include "cluster_toolbox.h"

#define INVALID_NTE_CONTEXT   0xFFFFFFFF

/********************************************************
* 名称: CResIp::CResIp
* 功能: 构造函数
* 输入: const char* const szSourceIPAddr: 原IP地址
*       const char* const szVirtualIP   : 虚拟IP，即附加IP
*       const char* const szVirtualMask : 虚拟IP地址的掩码
*       const char* const szResName     : 资源名
*       ERES_TYPE nResType              : 资源类型
*       int nMaxLocalRecovery           : 最大本地恢复次数
* 输出:
* 返回:
********************************************************/
CResIp::CResIp(const char* const szSourceIPAddr,
               const char* const szVirtualIP,
               const char* const szVirtualMask,
               BOOL              bCheckDisConn,
               const char* const szResName,
               ERES_TYPE nResType,
               int nMaxLocalRecovery):
CResObject(szResName, nResType, nMaxLocalRecovery)
{
    strncpy(m_szSourceIPAddr, szSourceIPAddr, IP_LENGTH);
    m_szSourceIPAddr[IP_LENGTH - 1] = '\0';

    m_uSourceIPMask = 0;

    strncpy(m_szVirtualIPAddr, szVirtualIP, IP_LENGTH);
    m_szVirtualIPAddr[IP_LENGTH - 1] = '\0';

    m_uVirtualIPMask = inet_addr(szVirtualMask);

    m_uIfIndex       = INVALID_ADAPTER_INDEX;
    m_uNTEContext    = INVALID_NTE_CONTEXT;
    m_bCheckDisConn  = bCheckDisConn;
}

/********************************************************
* 名称: CResIp::~CResIp
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResIp::~CResIp()
{
    DeActive();
}

/********************************************************
* 名称: CResIp::Create
* 功能: 资源创建函数
* 输入:
* 输出:
* 返回: int 资源创建是否成功，0表示成功，非零值表示失败
********************************************************/
int CResIp::Create()
{
    //检查原始IP是否存在
    BOOL bRet = GetMsgOfIp(m_szSourceIPAddr, m_uIfIndex, m_uSourceIPMask);
    if((!bRet) || (INVALID_ADAPTER_INDEX == m_uIfIndex))
    {
        TRACE(MTS_CLSTR, S_GET_IP_ADAPTER_FAIL, m_szSourceIPAddr);
        return ERR_FAIL;
    }

    //检查虚拟IP是否存在
    UINT4 uAdapterIndex = INVALID_ADAPTER_INDEX;
    UINT4 uMask;
    bRet = GetMsgOfIp(m_szVirtualIPAddr, uAdapterIndex, uMask);
    if((bRet) && (INVALID_ADAPTER_INDEX != uAdapterIndex))
    {
        RestoreIPAddress();
        GetMsgOfIp(m_szVirtualIPAddr, uAdapterIndex, uMask);
        if((bRet) && (INVALID_ADAPTER_INDEX != uAdapterIndex))
        {
            TRACE(MTS_CLSTR, S_VIRTUAL_IP_EXIST_IN_HOST, m_szVirtualIPAddr);
            return ERR_FAIL;
        }
    }

    return CResObject::Create();
}

/********************************************************
* 名称: CResIp::CheckStatus
* 功能: 资源的状态检查函数
* 输入:
* 输出:
* 返回: int 是否存在不可恢复的故障，0表示无故障，非零值表示检测到故障
********************************************************/
int CResIp::CheckStatus()
{
    UINT4 uAdapterIndex = INVALID_ADAPTER_INDEX;
    UINT4 uMask;
    BOOL bRet = GetMsgOfIp(m_szVirtualIPAddr, uAdapterIndex, uMask);
    if((!bRet) || (INVALID_ADAPTER_INDEX == uAdapterIndex))
    {
        DeActive();
        
        m_nStatus = ERES_STATUS_FAILURE;
        return ERR_FAIL;
    }

    //检测网线是否已连接
    if(g_nOSVersion >= 5 && m_bCheckDisConn)
    {
        MIB_IFROW mid_ifrow;
        memset(&mid_ifrow, 0, sizeof(mid_ifrow));
        mid_ifrow.dwIndex = uAdapterIndex;
        int nRet = GetIfEntry(&mid_ifrow);
        if(0 == nRet)
        {
            if(MIB_IF_OPER_STATUS_CONNECTING > mid_ifrow.dwOperStatus
               || MIB_IF_ADMIN_STATUS_DOWN == mid_ifrow.dwAdminStatus)
            {
                TRACE(MTS_CLSTR, "ResID=%d, dwOperStatus=%d, dwAdminStatus=%d", 
                      m_uResIndex,
                      mid_ifrow.dwOperStatus,
                      mid_ifrow.dwAdminStatus);
                m_nStatus = ERES_STATUS_FAILURE;
                return ERR_FAIL;
            }
        }
    }

    m_nStatus = ERES_STATUS_ACTIVE;
    
    return ERR_SUCCESS;
}

/********************************************************
* 名称: CResIp::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CResIp::Active()
{
    //检查原始IP是否存在，同时也要获得最新的m_uIfIndex，这是因为如果
    //网卡被禁用再启用后，m_uIfIndex值会改变
    BOOL bRet = GetMsgOfIp(m_szSourceIPAddr, m_uIfIndex, m_uSourceIPMask);
    if((!bRet) || (INVALID_ADAPTER_INDEX == m_uIfIndex))
    {
        TRACE(MTS_CLSTR, S_GET_IP_ADAPTER_FAIL, m_szSourceIPAddr);
        return ERR_FAIL;
    }

    DeActive();
    
    //检查虚拟IP是否存在
    UINT4 uAdapterIndex = INVALID_ADAPTER_INDEX;
    UINT4 uMask;
    bRet = GetMsgOfIp(m_szVirtualIPAddr, uAdapterIndex, uMask);
    if((!bRet) || (INVALID_ADAPTER_INDEX == uAdapterIndex))
    {
        if(Ping(m_szVirtualIPAddr)) 
        {
            TRACE(MTS_CLSTR, S_VIRTUAL_IP_EXIST_IN_LAN, m_szVirtualIPAddr);
            return ERR_FAIL;
        }

        IPAddr Address = inet_addr(m_szVirtualIPAddr);

        UINT4 uNTEInstance;
        int nRet = ERR_FAIL;

#ifdef _PLATFORM_WIN32
        if(g_nOSVersion >= 5)
        {
            nRet = AddIPAddress(Address,
                        m_uVirtualIPMask,
                        m_uIfIndex,
                        &m_uNTEContext,
                        &uNTEInstance);
        }
        else
        {
            nRet = ChangeIp(Address, m_uVirtualIPMask,
                            inet_addr(m_szSourceIPAddr))
                            ? 0 : -1;
        }
#endif //_PLATFORM_WIN32

        if(ERR_SUCCESS != nRet)
        {
            TRACE(MTS_CLSTR, S_ADD_VIRTUAL_IP_FAILED, m_szVirtualIPAddr, nRet);
            m_uNTEContext = INVALID_NTE_CONTEXT;
            return nRet;
        }
    }

    return CResObject::Active();
}

/********************************************************
* 名称: CResIp::DeActive
* 功能: 资源的去激活函数
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CResIp::DeActive()
{
    //在WIN2000下强制删除虚拟IP
    if(g_nOSVersion >= 5)
    {
	    if (INVALID_NTE_CONTEXT != m_uNTEContext)
	    {
            if(NO_ERROR == DeleteIPAddress(m_uNTEContext))
                m_uNTEContext = INVALID_NTE_CONTEXT;
        }
    }

    //检查虚拟IP是否存在
    UINT4 uAdapterIndex = INVALID_ADAPTER_INDEX;
    UINT4 uMask;
    BOOL bRet = GetMsgOfIp(m_szVirtualIPAddr, uAdapterIndex, uMask);
    if((bRet) && (INVALID_ADAPTER_INDEX != uAdapterIndex))
    {
        int nRet = RestoreIPAddress();
        if (ERR_SUCCESS != nRet)
        {
            return nRet;
        }
        m_uNTEContext = INVALID_NTE_CONTEXT;
    }

    return CResObject::DeActive();
}

/********************************************************
* 名称: CResIp::GetMsgOfIp
* 功能: 查找指定IP地址所在适配器索引号及掩码
* 输入: szAddr        : IP地址
* 输出: uAdapterIndex : 指定IP所在的适配器索引号
*       uMask         : 掩码
* 返回: BOOL 是否成功 
********************************************************/
BOOL CResIp::GetMsgOfIp(const char* const szAddr, UINT4& uAdapterIndex, UINT4& uMask)
{
    UINT4 uAddr  = inet_addr(szAddr);
    UINT4 uIndex = INVALID_ADAPTER_INDEX;
    UINT4 uSize  = 0;
    PMIB_IPADDRTABLE pIpAddrTable = NULL;

    uAdapterIndex = INVALID_ADAPTER_INDEX;
    uMask = 0;
    UINT4 uRet = GetIpAddrTable(pIpAddrTable, &uSize, FALSE);
    if((ERROR_INSUFFICIENT_BUFFER == uRet) && (uSize > 0))
    {
        pIpAddrTable = (PMIB_IPADDRTABLE) new BYTE[uSize];
        uRet = GetIpAddrTable(pIpAddrTable, &uSize, FALSE);
    }

    if((NO_ERROR == uRet) && (NULL != pIpAddrTable))
    {
        for (int i = 0; i < int(pIpAddrTable->dwNumEntries); i++)
        {
            if(pIpAddrTable->table[i].dwAddr == uAddr)
            {
                uAdapterIndex = pIpAddrTable->table[i].dwIndex;
                uMask = pIpAddrTable->table[i].dwMask;
                delete[] pIpAddrTable;
                return TRUE;
            }
        }
    }

    if(NULL != pIpAddrTable)
    {
        delete[] pIpAddrTable;
    }
    return FALSE;
}

int CResIp::RestoreIPAddress()
{
    int nRet = ERR_FAIL;
    if(g_nOSVersion >= 5)
    {
	    UINT4 uNTEContext = GetNTEContextOfIp(m_szVirtualIPAddr);
	    if (INVALID_NTE_CONTEXT != uNTEContext)
	    {
            nRet = DeleteIPAddress(uNTEContext);
        }
	}
    else
    {
        nRet = ChangeIp(inet_addr(m_szSourceIPAddr),
                        m_uSourceIPMask,
                        inet_addr(m_szVirtualIPAddr))
                        ? ERR_SUCCESS : ERR_FAIL;
    }
	
	return nRet;
}

UINT4 CResIp::GetNTEContextOfIp(const char* const szAddr)
{
	UINT4 uAddr = INADDR_NONE ;
	UINT4 uNTEContext = INVALID_NTE_CONTEXT ;
	
	uAddr = inet_addr(szAddr);
	if (uAddr == INADDR_NONE)
	{
		return INVALID_NTE_CONTEXT;
	}
	
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	ULONG  ulLen = 0;
	
	DWORD uRet = GetAdaptersInfo(NULL, &ulLen);
	
	if ( (uRet != ERROR_BUFFER_OVERFLOW) && ( uRet != NO_ERROR) )
	{
		return INVALID_NTE_CONTEXT;
	}
	
	if (ulLen > 0)
	{
		pAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[ulLen];
		uRet = GetAdaptersInfo(pAdapterInfo, &ulLen);
		
		if (NO_ERROR != uRet)
		{
			delete []pAdapterInfo;
			return INVALID_NTE_CONTEXT;
		}
		
		DWORD  uAdapterIpAddr = 0;
		PIP_ADAPTER_INFO pCurrentAdapterInfo = pAdapterInfo;
		while ( pCurrentAdapterInfo != NULL)
		{
			PIP_ADDR_STRING pCurrentIpAddrString = &pCurrentAdapterInfo->IpAddressList;
			while ( pCurrentIpAddrString != NULL )
			{
				uAdapterIpAddr = inet_addr(pCurrentIpAddrString->IpAddress.String);
				if (uAdapterIpAddr == uAddr)
				{
					uNTEContext = pCurrentIpAddrString->Context;
					delete []pAdapterInfo;
					return uNTEContext;
				}
				
				pCurrentIpAddrString = pCurrentIpAddrString->Next;
			}
			
			pCurrentAdapterInfo = pCurrentAdapterInfo->Next;
		}
		delete []pAdapterInfo;
	}	
	
	return INVALID_NTE_CONTEXT;
}
