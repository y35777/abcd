#ifndef __RESOURCE_USER_DEFINED_H__
#define __RESOURCE_USER_DEFINED_H__

#include "res_object.h"

class CResUserDefined : public CResObject
{
public:

    CResUserDefined(const char* const szCreateName,
                    const char* const szActiveName,
                    const char* const szDeActiveName,
                    const char* const szCheckStatus,
                    const char* const szResName,
                    ERES_TYPE nResType,
                    int nMaxLocalRecovery);
    virtual ~CResUserDefined();

    virtual int Create();       //��Դ��������
    virtual int CheckStatus();  //��Դ��״̬��麯��
    virtual int Active();       //��Դ�ļ����
    virtual int DeActive();     //��Դ��ȥ�����

protected:

    //ִ�м�������ĳ���
    char m_szActiveName[MAX_PATH];

    //ִ��ȥ��������ĳ���
    char m_szDeActiveName[MAX_PATH];

    //ִ��״̬�������ĳ���
    char m_szCheckName[MAX_PATH];

    //ִ�д��������ĳ���
    char m_szCreateName[MAX_PATH];
};


#endif //__RESOURCE_USER_DEFINED_H__
