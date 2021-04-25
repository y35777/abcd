#include "../include/frame.h"
#include "res_manager.h"
#include "res_object_creator.h"
#include "RaidHDChkCreator.h"
#include "resource.h"

/********************************************************
* ����: CResManager::CResManager
* ����: ���캯��
* ����:
* ���:
* ����:
********************************************************/
CResManager::CResManager()
{
    m_pDynamResMap   = NULL;
    m_pStaticResMap  = NULL;
    m_pRaidHDChecker = NULL;
}

/********************************************************
* ����: CResManager::~CResManager
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResManager::~CResManager()
{
    ExitInstance();
}

/********************************************************
* ����: CResManager::InitInstance
* ����: ��ʼ������
* ����:
* ���:
* ����: int ��ʼ���Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CResManager::InitInstance()
{
    //��CResObjectCreator�ഴ����̬ͬ����Դ�ĺ��������ض�̬ͬ����ԴMap��ָ��
    m_pDynamResMap = CResObjectCreator::CreateResObject(RT_DYNAMIC);
    if((NULL == m_pDynamResMap) && (ERR_CREATE_RES_OBJECT == errno))
    {
        return ERR_CREATE_RES_OBJECT;
    }

    //��CResObjectCreator�ഴ����̬ͬ����Դ�ĺ��������ؾ�̬ͬ����ԴMap��ָ��
    m_pStaticResMap = CResObjectCreator::CreateResObject(RT_STATIC);
    if(NULL != m_pStaticResMap)
    {
        Active(RT_STATIC);
    }
    else if(ERR_CREATE_RES_OBJECT == errno)
    {
        return ERR_CREATE_RES_OBJECT;
    }

    //����CRaidHDChkCreator::CreatRaidHDCheck������
    //����CRaidHDCheck���󣬳�ʼ����Ա����m_pRaidHDChecker
    m_pRaidHDChecker = CRaidHDChkCreator::CreatRaidHDCheck();

    return ERR_SUCCESS;
}

/********************************************************
* ����: CResManager::ExitInstance
* ����: �˳�ǰ�Ľ���������
* ����:
* ���:
* ����: void
********************************************************/
void CResManager::ExitInstance()
{
    //ֹͣ���ͷŶ�̬ͬ����ԴMap����Դ����
    DeActive(RT_DYNAMIC);
    if(NULL != m_pDynamResMap)
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            (*i).second->Remove();
            delete (*i).second;
        }
        m_pDynamResMap->clear();
        delete m_pDynamResMap;
        m_pDynamResMap = NULL; 
    }

    //ֹͣ���ͷž�̬ͬ����ԴMap����Դ����
    DeActive(RT_STATIC);
    if(NULL != m_pStaticResMap)
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            (*i).second->Remove();
            delete (*i).second;
        }
        m_pStaticResMap->clear();
        delete m_pStaticResMap;
        m_pStaticResMap = NULL; 
    }

    //�ͷ�Ӳ�̼�����
    if(NULL != m_pRaidHDChecker)
    {
        delete m_pRaidHDChecker;
        m_pRaidHDChecker = NULL;
    }
}

/********************************************************
* ����: CResManager::CheckStatus
* ����: ��Դ��״̬��麯��
* ����: nCheckResMask : nCheckResMask---�������Դ���͵����룬
*       1��ʾ��̬��Դ,2��ʾ��̬��Դ,3��ʾӲ����Դ
* ���:
* ����: UINT4 �Ƿ���ڲ��ɻָ��Ĺ��ϣ�0��ʾ�޹��ϣ�����ֵ
*       ��ʾ�������ϵ���ԴID,����������£�
*       ����Դ��������ʱ��������ԴID������
*       ���������з�������ʱ�����ع��ϴ���ID������
********************************************************/
UINT4 CResManager::CheckStatus(int nCheckResMask)
{
    CResObject* pResObject = NULL;

    //���nCheckResMask����RT_DYNAMIC��
    //�����m_pDynamResMap��������Դ�����CheckStatus������
    //�����Դ״̬��������ڲ��ɻָ��Ĺ��ϣ��򷵻ظ���Դ
    //��m_pDynamResMap�е�����
    if ((RT_DYNAMIC == nCheckResMask) && (NULL != m_pDynamResMap))
    {
        MAP<int, CResObject*>::iterator i;
        int nFailedResIDMask = 0;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            pResObject = (*i).second;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                   S_BEGIN_CHECK_RESOURCE, pResObject->GetResName());
            if(pResObject->CheckStatus() != ERR_SUCCESS)
            {
                MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                       S_RES_LOCAL_RECOVERY, 
                       pResObject->GetResName());
                if(pResObject->LocalRecovery() != ERR_SUCCESS)
                {
                    nFailedResIDMask |= 1 << ((*i).first - 1);
                }
                else
                {
                    MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                           S_RES_LOCAL_RECOVERY_SUCCESS, 
                           pResObject->GetResName());
                }
            }
            else
            {
                MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                       S_CHECK_RESOURCE_OK, pResObject->GetResName());
            }
        }

        return nFailedResIDMask;
    }

    //����������Ӳ��
    if(RT_HARDDISK == nCheckResMask)
    {
        int nHardDisk = CheckRaidHD();
        if(0 != nHardDisk)
        {
            return nHardDisk;
        }
    }

    //���nCheckResMask����RT_STATIC��
    //�����m_pStaticResMap��������Դ�����CheckStatus������
    //�����Դ״̬��������ڲ��ɻָ��Ĺ��ϣ��򷵻ظ���Դ��
    //m_pStaticResMap�е�����
    if ((RT_STATIC == nCheckResMask) && (NULL != m_pStaticResMap))
    {
        MAP<int, CResObject*>::iterator i;
        int nFailedResIDMask = 0;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            pResObject = (*i).second;
            MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                   S_BEGIN_CHECK_RESOURCE, pResObject->GetResName());
            if(pResObject->CheckStatus() != ERR_SUCCESS)
            {
                MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                       S_RES_LOCAL_RECOVERY, 
                       pResObject->GetResName());
                if(pResObject->LocalRecovery() != ERR_SUCCESS)
                {
                    nFailedResIDMask |= 1 << ((*i).first - 1);
                }
                else
                {
                    MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                           S_RES_LOCAL_RECOVERY_SUCCESS, 
                           pResObject->GetResName());
                }
            }
            else
            {
                MSGOUT(MTS_CLSTR, MSGOUT_LEVEL_IMPORTANT, 
                       S_CHECK_RESOURCE_OK, pResObject->GetResName());
            }
        }

        return nFailedResIDMask;
    }

    return 0;
}

/********************************************************
* ����: CResManager::Active
* ����: ��Դ�ļ����
* ����: nCheckResMask : nCheckResMask---��������Դ���͵����룬
*       ��һλ�������λ����ʾ��̬��Դ���ڶ�λ��ʾ��̬��Դ
* ���:
* ����: int �Ƿ񼤻�ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ�ܵ�
*       ��ԴID������������£�
*       �����ص�int�����λΪ0��ʣ��λΪ��ԴID
********************************************************/
int CResManager::Active(int nCheckResMask)
{
    CResObject* pResObject = NULL;

    //���nCheckResMaskλ��RT_DYNAMIC��Ϊ�㣬�����
    //m_pDynamResMap��������Դ�����Active������������Щ��Դ��
    //�������ʧ�ܣ��򷵻ظ���Դ��m_pDynamResMap�е�����
    int nFailedResIDMask = 0;
    if ((nCheckResMask & RT_DYNAMIC) && (NULL != m_pDynamResMap))
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            pResObject = (*i).second;
            if(pResObject->Active() != ERR_SUCCESS)
            {
                nFailedResIDMask |= 1 << ((*i).first - 1);
                int nSwitchGroup = pResObject->GetSwitchGroup();
                if(nSwitchGroup == 0)
                //����������Ϊ0���򷵻ص�ǰʧЧ��Դ��ID����
                {
                    return nFailedResIDMask;
                }
                else if(nSwitchGroup > 0)
                //���������Ŵ���0����Ҫ�������ͬ��ŵ���Դ�Ƿ�ҲʧЧ
                {
                    //Ѱ�Һ�����Դ���Ƿ����ͬ��ŵ���Դ
                    MAP<int, CResObject*>::iterator j = i;
                    BOOL bExistSameGroupRes = FALSE;
                    while(++j != m_pDynamResMap->end())
                    {
                        if((*j).second->GetSwitchGroup() == nSwitchGroup)
                        {
                            bExistSameGroupRes = TRUE;
                            break;
                        }
                    }

                    //��������Դ�д���ͬ��ŵ���Դ�������������һ����Դ
                    //��������ڣ�����ǰ���ͬ����Դ���Ƿ�����Ѽ������Դ
                    if(!bExistSameGroupRes && VerifyGroupFailed((*i).first))
                    {
                        return nFailedResIDMask;
                    }
                }
            }
        }
    }

    //���nCheckResMaskλ��RT_STATIC��Ϊ�㣬�����
    //m_pStaticResMap��������Դ�����Active������������Щ��Դ��
    //�������ʧ�ܣ��򷵻ظ���Դ��m_pStaticResMap�е�����
    if ((nCheckResMask & RT_STATIC) && (NULL != m_pStaticResMap))
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            pResObject = (*i).second;
            if(pResObject->Active() != ERR_SUCCESS)
            {
                nFailedResIDMask |= 1 << ((*i).first - 1);
                int nSwitchGroup = pResObject->GetSwitchGroup();
                if(nSwitchGroup == 0)
                //����������Ϊ0���򷵻ص�ǰʧЧ��Դ��ID����
                {
                    return nFailedResIDMask;
                }
                else if(nSwitchGroup > 0)
                //���������Ŵ���0����Ҫ�������ͬ��ŵ���Դ�Ƿ�ҲʧЧ
                {
                    //Ѱ�Һ�����Դ���Ƿ����ͬ��ŵ���Դ
                    MAP<int, CResObject*>::iterator j = i;
                    BOOL bExistSameGroupRes = FALSE;
                    while(++j != m_pStaticResMap->end())
                    {
                        if((*j).second->GetSwitchGroup() == nSwitchGroup)
                        {
                            bExistSameGroupRes = TRUE;
                            break;
                        }
                    }

                    //��������Դ�д���ͬ��ŵ���Դ�������������һ����Դ
                    //��������ڣ�����ǰ���ͬ����Դ���Ƿ�����Ѽ������Դ
                    if(!bExistSameGroupRes && VerifyGroupFailed((*i).first))
                    {
                        return nFailedResIDMask;
                    }
                }
            }
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CResManager::DeActive
* ����: ��Դ��ȥ�����
* ����: nCheckResMask : nCheckResMask---��������Դ���͵����룬
*       ��һλ�������λ����ʾ��̬��Դ���ڶ�λ��ʾ��̬��Դ
* ���:
* ����: int �Ƿ�ȥ����ɹ���0��ʾ�ɹ�������ֵ��ʾ����ʧ�ܵ�
*       ��ԴID������������£�
*       �����ص�UINT4�����λΪ0��ʣ��λΪ��ԴID
********************************************************/
int CResManager::DeActive(int nCheckResMask)
{
    CResObject* pResObject = NULL;
    int nFailedResObject = 0;

    //���nCheckResMaskλ��RT_DYNAMIC��Ϊ�㣬�����
    //m_pDynamResMap��������Դ�����DeActive������ȥ������Щ��Դ��
    //�������ʧ�ܣ��򷵻ظ���Դ��m_pDynamResMap�е�����
    if ((nCheckResMask & RT_DYNAMIC) && (NULL != m_pDynamResMap))
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            pResObject = (*i).second;
            if((pResObject->DeActive() != ERR_SUCCESS) && (0 == nFailedResObject))
            {
                nFailedResObject = (*i).first;
            }
        }
    }

    //���nCheckResMaskλ��RT_STATIC��Ϊ�㣬�����
    //m_pStaticResMap��������Դ�����DeActive������ȥ������Щ��Դ��
    //�������ʧ�ܣ��򷵻ظ���Դ��m_pStaticResMap�е�����
    if ((nCheckResMask & RT_STATIC) && (NULL != m_pStaticResMap))
    {
        MAP<int, CResObject*>::iterator i;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            pResObject = (*i).second;
            if((pResObject->DeActive() != ERR_SUCCESS) && (0 == nFailedResObject))
            {
                nFailedResObject = (*i).first;
            }
        }
    }

    return nFailedResObject;
}

/********************************************************
* ����: CResManager::CheckRaidHD
* ����: ��������Ӳ�̵Ĺ��ϼ�麯��
* ����:
* ���:
* ����: int �������ϵ�Ӳ��ID����
********************************************************/
int CResManager::CheckRaidHD()
{
    //����m_pRaidHDChecker����ļ��ӿڣ�ʵʩ�Դ�������
    //Ӳ�̵ļ�⣬���ط������ϵ�Ӳ������
    if(NULL != m_pRaidHDChecker)
    {
        m_pRaidHDChecker->CheckRaidHD();

        return m_pRaidHDChecker->GetFailedHDID();
    }

    return 0;
}

/********************************************************
* ����: CResManager::GetObject
* ����: ����ָ��CResObject��Դ�����ָ��
* ����: nIndex : ��Դ������
* ���:
* ����: CResObject* ��Ӧ��CResObject��Դ����ָ��
********************************************************/
CResObject* CResManager::GetObject(int nIndex)
{
    if((NULL != m_pDynamResMap) &&
       (m_pDynamResMap->find(nIndex) != m_pDynamResMap->end()))
    {
        return (*m_pDynamResMap)[nIndex];
    }

    if((NULL != m_pStaticResMap) &&
       (m_pStaticResMap->find(nIndex) != m_pStaticResMap->end()))
    {
        return (*m_pStaticResMap)[nIndex];
    }

    return NULL;
}

/********************************************************
* ����: CResManager::VerifyGroupFailed
* ����: ���ָ������ԴID���ڵ���Դ���Ƿ���ʧЧ
* ����: nIndex : ��Դ������
* ���:
* ����: CResObject* ��Ӧ��CResObject��Դ����ָ��
********************************************************/
BOOL CResManager::VerifyGroupFailed(int nIndex)
{
    if((NULL != m_pDynamResMap) &&
       (m_pDynamResMap->find(nIndex) != m_pDynamResMap->end()))
    {
        int nSwitchGroup = (*m_pDynamResMap)[nIndex]->GetSwitchGroup();
        if(nSwitchGroup < 0)
        {
            return FALSE;
        }
        else if(nSwitchGroup == 0)
        {
            return TRUE;
        }

        MAP<int, CResObject*>::iterator i;
        for (i = m_pDynamResMap->begin(); i != m_pDynamResMap->end(); i++)
        {
            if(((*i).second->GetStatus() == ERES_STATUS_ACTIVE) &&
               ((*i).second->GetSwitchGroup() == nSwitchGroup))
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    if((NULL != m_pStaticResMap) &&
       (m_pStaticResMap->find(nIndex) != m_pStaticResMap->end()))
    {
        int nSwitchGroup = (*m_pStaticResMap)[nIndex]->GetSwitchGroup();
        if(nSwitchGroup < 0)
        {
            return FALSE;
        }
        else if(nSwitchGroup == 0)
        {
            return TRUE;
        }

        MAP<int, CResObject*>::iterator i;
        for (i = m_pStaticResMap->begin(); i != m_pStaticResMap->end(); i++)
        {
            if(((*i).second->GetStatus() == ERES_STATUS_ACTIVE) &&
               ((*i).second->GetSwitchGroup() == nSwitchGroup))
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    return FALSE;
}
