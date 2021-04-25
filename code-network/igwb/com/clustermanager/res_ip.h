#ifndef __RESOURCE_IP_H__
#define __RESOURCE_IP_H__

#include "res_object.h"

class CResIp : public CResObject
{
public:

    CResIp(const char* const szSourceIPAddr,
           const char* const szVirtualIP,
           const char* const szVirtualMask,
           BOOL              bCheckDisConn,
           const char* const szResName,
           ERES_TYPE nResType,
           int nMaxLocalRecovery);
    virtual ~CResIp();

    virtual int Create();       //资源创建函数
    virtual int CheckStatus();  //资源的状态检查函数
    virtual int Active();       //资源的激活函数
    virtual int DeActive();     //资源的去激活函数

protected:

    //查找指定IP地址所在适配器索引号
#define INVALID_ADAPTER_INDEX   0xFFFFFFFF
    BOOL GetMsgOfIp(const char* const szAddr, UINT4& uAdapterIndex, UINT4& uMask);

    int RestoreIPAddress();
    UINT4 GetNTEContextOfIp(const char* const szAddr);
   //虚拟IP地址
#define IP_LENGTH    20
   char     m_szVirtualIPAddr[IP_LENGTH];

   //虚拟IP地址的掩码
   UINT4    m_uVirtualIPMask;

   //产生虚拟IP的适配器号
   UINT4    m_uIfIndex;

   //Net Table Entry (NTE)地址的指针
   UINT4    m_uNTEContext;

   //原始IP地址
   char     m_szSourceIPAddr[IP_LENGTH];

   //原始IP地址的掩码
   UINT4    m_uSourceIPMask;

   //是否检测网线
   BOOL     m_bCheckDisConn;
};


#endif //__RESOURCE_IP_H__
