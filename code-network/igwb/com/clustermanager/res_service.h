#ifndef __RESOURCE_SERVICE_H__
#define __RESOURCE_SERVICE_H__

#include "res_object.h"

class CResService : public CResObject
{
public:

    CResService(const char* const szServiceName,
                const char* const szResName,
                ERES_TYPE nResType,
                int nMaxLocalRecovery);
    virtual ~CResService();

    virtual int Create();       //��Դ��������
    virtual int CheckStatus();  //��Դ��״̬��麯��
    virtual int Active();       //��Դ�ļ����
    virtual int DeActive();     //��Դ��ȥ�����

protected:

    //������
#define MAX_SERVICE_NAME    50
    char    m_szServiceName[MAX_SERVICE_NAME];

};


#endif //__RESOURCE_SERVICE_H__
