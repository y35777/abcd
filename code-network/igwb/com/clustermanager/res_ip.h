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

    virtual int Create();       //��Դ��������
    virtual int CheckStatus();  //��Դ��״̬��麯��
    virtual int Active();       //��Դ�ļ����
    virtual int DeActive();     //��Դ��ȥ�����

protected:

    //����ָ��IP��ַ����������������
#define INVALID_ADAPTER_INDEX   0xFFFFFFFF
    BOOL GetMsgOfIp(const char* const szAddr, UINT4& uAdapterIndex, UINT4& uMask);

    int RestoreIPAddress();
    UINT4 GetNTEContextOfIp(const char* const szAddr);
   //����IP��ַ
#define IP_LENGTH    20
   char     m_szVirtualIPAddr[IP_LENGTH];

   //����IP��ַ������
   UINT4    m_uVirtualIPMask;

   //��������IP����������
   UINT4    m_uIfIndex;

   //Net Table Entry (NTE)��ַ��ָ��
   UINT4    m_uNTEContext;

   //ԭʼIP��ַ
   char     m_szSourceIPAddr[IP_LENGTH];

   //ԭʼIP��ַ������
   UINT4    m_uSourceIPMask;

   //�Ƿ�������
   BOOL     m_bCheckDisConn;
};


#endif //__RESOURCE_IP_H__
