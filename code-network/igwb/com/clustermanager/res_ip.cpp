#include "../include/frame.h"
#include "res_ip.h"
#include "Iphlpapi.h"
#include "ping.h"
#include "resource.h"
#include "BtwIpHelper.h"
#include "cluster_toolbox.h"

#define INVALID_NTE_CONTEXT   0xFFFFFFFF

/********************************************************
* ����: CResIp::CResIp
* ����: ���캯��
* ����: const char* const szSourceIPAddr: ԭIP��ַ
*       const char* const szVirtualIP   : ����IP��������IP
*       const char* const szVirtualMask : ����IP��ַ������
*       const char* const szResName     : ��Դ��
*       ERES_TYPE nResType              : ��Դ����
*       int nMaxLocalRecovery           : ��󱾵ػָ�����
* ���:
* ����:
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
* ����: CResIp::~CResIp
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResIp::~CResIp()
{
    DeActive();
}

/********************************************************
* ����: CResIp::Create
* ����: ��Դ��������
* ����:
* ���:
* ����: int ��Դ�����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CResIp::Create()
{
    //���ԭʼIP�Ƿ����
    BOOL bRet = GetMsgOfIp(m_szSourceIPAddr, m_uIfIndex, m_uSourceIPMask);
    if((!bRet) || (INVALID_ADAPTER_INDEX == m_uIfIndex))
    {
        TRACE(MTS_CLSTR, S_GET_IP_ADAPTER_FAIL, m_szSourceIPAddr);
        return ERR_FAIL;
    }

    //�������IP�Ƿ����
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
* ����: CResIp::CheckStatus
* ����: ��Դ��״̬��麯��
* ����:
* ���:
* ����: int �Ƿ���ڲ��ɻָ��Ĺ��ϣ�0��ʾ�޹��ϣ�����ֵ��ʾ��⵽����
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

    //��������Ƿ�������
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
* ����: CResIp::Active
* ����: ��Դ�ļ����
* ����:
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ��
********************************************************/
int CResIp::Active()
{
    //���ԭʼIP�Ƿ���ڣ�ͬʱҲҪ������µ�m_uIfIndex��������Ϊ���
    //���������������ú�m_uIfIndexֵ��ı�
    BOOL bRet = GetMsgOfIp(m_szSourceIPAddr, m_uIfIndex, m_uSourceIPMask);
    if((!bRet) || (INVALID_ADAPTER_INDEX == m_uIfIndex))
    {
        TRACE(MTS_CLSTR, S_GET_IP_ADAPTER_FAIL, m_szSourceIPAddr);
        return ERR_FAIL;
    }

    DeActive();
    
    //�������IP�Ƿ����
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
* ����: CResIp::DeActive
* ����: ��Դ��ȥ�����
* ����:
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾȥ����ʧ��
********************************************************/
int CResIp::DeActive()
{
    //��WIN2000��ǿ��ɾ������IP
    if(g_nOSVersion >= 5)
    {
	    if (INVALID_NTE_CONTEXT != m_uNTEContext)
	    {
            if(NO_ERROR == DeleteIPAddress(m_uNTEContext))
                m_uNTEContext = INVALID_NTE_CONTEXT;
        }
    }

    //�������IP�Ƿ����
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
* ����: CResIp::GetMsgOfIp
* ����: ����ָ��IP��ַ���������������ż�����
* ����: szAddr        : IP��ַ
* ���: uAdapterIndex : ָ��IP���ڵ�������������
*       uMask         : ����
* ����: BOOL �Ƿ�ɹ� 
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
