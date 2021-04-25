#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "res_object.h"
#include "RaidHDCheck.h"

enum RESOURCE_TYPE
{
    RT_DYNAMIC      = 1,    //��̬��Դ
    RT_STATIC       = 2,    //��̬��Դ
    RT_HARDDISK     = 4     //����
};

class CResManager
{
public:
    CResManager();
    virtual ~CResManager();

    //��ʼ������
    int InitInstance();
    //��Դ��״̬��麯��
    UINT4 CheckStatus(int nCheckResMask);
    //��Դ�ļ����
    int Active(int nCheckResMask);
    //��Դ��ȥ�����
    int DeActive(int nCheckResMask);
    //��������Ӳ�̵Ĺ��ϼ�麯��
    int CheckRaidHD();
    //����ָ��CResObject��Դ�����ָ��
    CResObject* GetObject(int nIndex);
    //���ָ������ԴID���ڵ���Դ���Ƿ���ʧЧ
    BOOL VerifyGroupFailed(int nIndex);

protected:

    //�˳�ǰ�Ľ���������
    void ExitInstance();

protected:

    //��̬ͬ����Դ��Mapָ��
    MAP<int, CResObject*>      *m_pDynamResMap;

    //��̬ͬ����Դ��Mapָ��
    MAP<int, CResObject*>      *m_pStaticResMap;

    //��������Ӳ��״̬�����ָ��
    CRaidHDCheck*               m_pRaidHDChecker;

};

#endif //__RESOURCE_MANAGER_H__
