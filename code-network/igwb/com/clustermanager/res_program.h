#ifndef __RESOURCE_PROGRAM_H__
#define __RESOURCE_PROGRAM_H__

#include "res_object.h"

class CResProgram : public CResObject
{
public:

    CResProgram(const char* const szAppPath,
                const char* const szResName,
                ERES_TYPE nResType,
                int nMaxLocalRecovery);
    virtual ~CResProgram();

    virtual int Create();       //��Դ��������
    virtual int CheckStatus();  //��Դ��״̬��麯��
    virtual int Active();       //��Դ�ļ����
    virtual int DeActive();     //��Դ��ȥ�����

protected:

    //����ȫ·����
    char                m_szAppPath[MAX_PATH];

    //����PID��
    pid_t               m_nPid;

    //ACE�Ľ��̹������
    ACE_Process_Manager m_PMgr;

    //ҵ����̵�ACE����
    ACE_Process_Options m_POptions;

};


#endif //__RESOURCE_PROGRAM_H__
