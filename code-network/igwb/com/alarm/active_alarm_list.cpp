// ActiveAlarmList.cpp: implementation of the CActiveAlarmList class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/frame.h"
#include "active_alarm_list.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//����ԭ��  Public CActiveAlarmList(SActiveAlarm& sActiveAlarm)
//��������  ���캯��
//�������  sActiveAlarm������澯֡ģ��,��Ϊ�澯֡�е��������д����
//          ������һ��ģ������ʼ����.
////////////////////////////////////////////////////////////////////////

CActiveAlarmList::CActiveAlarmList(SActiveAlarm& sActiveAlarm):m_pAlmCfg(NULL)
{

    memcpy(&m_ActiveAlarmTmpl,&sActiveAlarm,sizeof(SActiveAlarm));
}


CActiveAlarmList::CActiveAlarmList():m_pAlmCfg(NULL)
{
    memset(&m_ActiveAlarmTmpl,0,sizeof(SActiveAlarm));
}

////////////////////////////////////////////////////////////////////////
//��������  ��������
////////////////////////////////////////////////////////////////////////
CActiveAlarmList::~CActiveAlarmList()
{
    //A�����������ͷ������и���Առ�õ����ݿռ䡣
    iterator Iter;
    Iter = begin();
    while(Iter != end())
    {
        delete (*Iter);
        Iter++;
    }
    clear();
    m_pAlmCfg = NULL;

}

//////////////////////////////////////////////////////////////////////////////
//��������  �����������һ���澯֡
//�������  pInnerAlarm - ���ݸ澯����֡
//�������  pActive     - ��澯�ṹ,���ڶ���ʷ��Ĳ����������ˮ��
//����ֵ    ALARM_ADD_RESULT
//          {
//           ADD_OK,      //�澯�������
//           ADD_REPEATED,    //�澯�ظ�
//           ADD_NOT_MATCH,   //�澯��ƥ��
//           INVALID_ALARM    //�澯�Ƿ�
//          }
//////////////////////////////////////////////////////////////////////////////
int CActiveAlarmList::AddAlarm(SInnerAlarm *pInnerAlarm, SActiveAlarm **ppActive, int nRecoverType)
{
    if(pInnerAlarm == NULL)
    {
        return INVALID_ALARM;
    }
    //�жϸ澯�����Ƿ���ȷ,���澯���Ͳ�Ϊ�¼��澯,���ϸ澯��
    //�ָ��澯�е�һ���򷵻�INVALID_ALARM
    if(pInnerAlarm->yAlarmType != AT_EVENT
        && pInnerAlarm->yAlarmType != AT_FAULT
        && pInnerAlarm->yAlarmType != AT_RESTORE)
    {
        *ppActive = NULL;
        return INVALID_ALARM;
    }

    //�����Ƿ��ظ��澯,��Ϊ�ظ��澯,�򷵻�ADD_REPEATED
    if(Find(pInnerAlarm) != NULL)
    {
        *ppActive = NULL;
        return ADD_REPEATED;
    }

    //���澯����澯����,��ͬ�澯���ͼ���澯���еķ�ʽ��ͬ.
    switch(pInnerAlarm->yAlarmType)
    {
    case AT_RESTORE:
        if((*ppActive = AddRestoreAlarm(pInnerAlarm->yAlarmType,
            pInnerAlarm->uAlarmID, pInnerAlarm->yAlarmPara,nRecoverType)) == NULL)
        {
            return ADD_NOT_MATCH;
        }
        break;
    case AT_FAULT:
        if((*ppActive = AddFatalAlarm(pInnerAlarm->yAlarmType,
            pInnerAlarm->uAlarmID, pInnerAlarm->yAlarmPara)) ==NULL)
        {
            return INVALID_ALARM;
        }
        break;
    case AT_EVENT:
        if((*ppActive = AddEventAlarm(pInnerAlarm->yAlarmType,
            pInnerAlarm->uAlarmID, pInnerAlarm->yAlarmPara)) == NULL)
        {
            return INVALID_ALARM;
        }
        break;
    //������ 2002-04-12 ���ⵥD12566
    default:
        {
            TRACE(MTS_ALARM,S_ALARM_RECV_INVALIDALARM,pInnerAlarm->uAlarmID,
                  pInnerAlarm->yAlarmPara, pInnerAlarm->yAlarmType);
            return INVALID_ALARM;
        }
    }
    //����
    //���سɹ�.
    return ADD_OK;

}

////////////////////////////////////////////////////////////////////
//��������  �ڸ澯�����в���ָ���澯֡
//�������  pInnerAlarm - ����Ҫ���ҵĸ澯��Ϣ
//�������  ��
//����ֵ    ����ҵ��򷵻ظ澯֡��ָ��,����Ϊ��
////////////////////////////////////////////////////////////////////
SActiveAlarm *CActiveAlarmList::Find(SInnerAlarm *pInnerAlarm)
{
    return Find(pInnerAlarm->yAlarmType,
                pInnerAlarm->uAlarmID,
                pInnerAlarm->yAlarmPara);

}

////////////////////////////////////////////////////////////////////
//��������  ���ݸ澯����,�澯ID�͸澯��������ָ���澯֡
//�������  byAlarmType - �澯����
//      wAlarmID    - �澯ID
//      byAlarmPara - �澯����
//�������  ��
//����ֵ    �澯֡��ָ��,���û����Ϊ��
////////////////////////////////////////////////////////////////////
SActiveAlarm *CActiveAlarmList::Find(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara)
{
    //�����澯��������,���ƥ����Ҹ澯����,�澯ID�͸澯�����������ͬ�Ļ�澯.
    iterator theIterator;
    SActiveAlarm *pActiveAlarm;

    for(theIterator = begin(); theIterator != end(); theIterator++)
    {
        pActiveAlarm = (*theIterator);

        if(pActiveAlarm == NULL)
        {
            continue;
        }

        if(pActiveAlarm->yAlarmType == yAlarmType
            && pActiveAlarm->uAlarmID == uAlarmID
            && memcmp(pActiveAlarm->yPara, yAlarmPara, MAX_ALARM_PARA) == 0)//self change 2 from = to ==
        {
            return pActiveAlarm;
        }
    }
    //��û�в��ҵ�ƥ��Ļ�澯,�򷵻ؿ�ָ��
    return NULL;

}

//////////////////////////////////////////////////////////////////////
//��������  ���Ҹ澯֡
//�������  SActiveAlarm *pActiveAlarm
//�������  ��
//����ֵ    �澯֡��ָ�룬���û���ҵ���Ϊ��
//////////////////////////////////////////////////////////////////////
SActiveAlarm * CActiveAlarmList::Find(SActiveAlarm *pActiveAlarm)
{
    return Find(pActiveAlarm->yAlarmType,
                pActiveAlarm->uAlarmID,
                pActiveAlarm->yPara);

}

/////////////////////////////////////////////////////////////////////
//��������  ɾ���澯֡����
//�������  SActiveAlarm *pActiveAlarm
//�������  ��
//����ֵ    ��
//�׳��쳣  ��
////////////////////////////////////////////////////////////////////
void CActiveAlarmList::RemoveAlarm(SActiveAlarm *pActiveAlarm)
{
    RemoveAlarm(pActiveAlarm->yAlarmType,
                pActiveAlarm->uAlarmID,
                pActiveAlarm->yPara);
}

////////////////////////////////////////////////////////////////////
//��������  ɾ���澯֡����
//�������  byAlarmType - �澯����
//          wAlarmID    - �澯ID
//          byAlarmPara - �澯����
/////////////////////////////////////////////////////////////////////
void CActiveAlarmList::RemoveAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara)
{
    //����Ҫɾ���Ļ�澯,��������ɾ��.
    SActiveAlarm *pAlarm = NULL;
    pAlarm = Find(yAlarmType, uAlarmID, yAlarmPara);

    if(pAlarm != NULL)
    {
        remove(pAlarm);
        delete pAlarm;
        pAlarm = NULL;
    }
}

///////////////////////////////////////////////////////////////////////
//��������  ���ݲ�������һ���澯֡
//�������  byAlarmType - �澯����
//          wAlarmID    - �澯ID
//          byAlarmPara - �澯����
//�������  ��
//����ֵ    �澯֡��ָ��
///////////////////////////////////////////////////////////////////////
SActiveAlarm *CActiveAlarmList::CreateActiveAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara,int nRecoverType)
{
    SAlarmExplain *pExplain = NULL;
    pExplain = m_pAlmCfg->GetAlarmExplain(uAlarmID);

    if(pExplain == NULL)
    {
        return NULL;
    }

    SActiveAlarm *pActiveAlarm = new SActiveAlarm;
    //��д��澯����֡�Ĳ�������ΪiGateway bill���е�����ֵ
    //unit test change ������ֵ���ڴ濽��
    memcpy(pActiveAlarm,&m_ActiveAlarmTmpl,sizeof(SActiveAlarm));
    //������澯����֡,������Ĳ�����ֵ��������֡

    pActiveAlarm->yAlarmType = yAlarmType;
    pActiveAlarm->uAlarmID = uAlarmID;
    memcpy(pActiveAlarm->yPara, yAlarmPara, MAX_ALARM_PARA);

    pActiveAlarm->yRecoverType = nRecoverType;

    //���������ļ�����д�澯�����ӹ��ܺŵ�
    pActiveAlarm->yAlarmLevel = pExplain->yAlarmLevel;
    pActiveAlarm->yFuncID = pExplain->yFuncID;
    pActiveAlarm->ySubSysID = pExplain->ySubSysID;
    //��д��ǰʱ�䵽��澯����֡��
    time_t ltime;
    time(&ltime);
    tm *ptoday = localtime(&ltime);
    pActiveAlarm->uYear = ptoday->tm_year + 1900;
    pActiveAlarm->yMonth = ptoday->tm_mon + 1;
    pActiveAlarm->yDay = ptoday->tm_mday;
    pActiveAlarm->yHour = ptoday->tm_hour;
    pActiveAlarm->yMin = ptoday->tm_min;
    pActiveAlarm->ySec = ptoday->tm_sec;

    return pActiveAlarm;
}

/////////////////////////////////////////////////////////////////////////////
//��������  ��澯����������һ�����ϸ澯֡
//�������  byAlarmType - �澯����
//          wAlarmID    - �澯ID
//          byAlarmPara - �澯����
//�������  ��
//����ֵ    �澯֡��ָ��
//////////////////////////////////////////////////////////////////////////////
SActiveAlarm * CActiveAlarmList::AddFatalAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara)
{
    //���ݸ澯����������澯����֡
    SActiveAlarm *pFatalAlarm = CreateActiveAlarm(yAlarmType, uAlarmID, yAlarmPara);

    //����澯�����д��ڶ�Ӧ�Ļָ��澯��ɾ���ûָ��澯
    SActiveAlarm *pRestoreAlarm = Find(AT_RESTORE, uAlarmID, yAlarmPara);

    if(pRestoreAlarm !=  NULL)
    {
        delete pRestoreAlarm;
        remove(pRestoreAlarm);
        pRestoreAlarm = NULL;

    }

    //���ù��ϸ澯��ӵ��澯������
    //self chang 2 push_back return void so old if() be delete!;
    if(pFatalAlarm != NULL)
    {
        push_back(pFatalAlarm);
    }

    //���ظ澯����֡
    return pFatalAlarm;
}

////////////////////////////////////////////////////////////////////
//��������  ��澯�����������¼��澯
//�������  byAlarmType - �澯����
//      wAlarmID    - �澯ID
//      byAlarmPara - �澯����
//�������  ��
//����ֵ    �澯֡��ָ��
//////////////////////////////////////////////////////////////////////
SActiveAlarm *CActiveAlarmList::AddEventAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara)
{
    //���ݸ澯����������澯����֡
    SActiveAlarm *pEventAlarm = CreateActiveAlarm(yAlarmType, uAlarmID, yAlarmPara);
   
    //OM���ģ��¼��澯�������澯�⣬ֱ�ӿ��Խ�����ʷ�澯�⣨���澯��־��
#ifndef _OM_MODIFY
    //���澯����֡��ӵ��澯�����в����ظø澯����֡
    if(pEventAlarm != NULL)
    {
        push_back(pEventAlarm);
    }
#endif

    return pEventAlarm;

}

//////////////////////////////////////////////////////////////////
//��������  ��澯���������ӻָ��澯
//�������  byAlarmType - �澯����
//      wAlarmID    - �澯ID
//      byAlarmPara - �澯����
//�������  ��
//����ֵ    �澯֡��ָ��
//////////////////////////////////////////////////////////////////
SActiveAlarm * CActiveAlarmList::AddRestoreAlarm(BYTE yAlarmType, UINT2 uAlarmID, BYTE* yAlarmPara, int nRecoverType)
{
    //����澯�����д��ڶ�Ӧ�Ĺ��ϸ澯��ɾ���ù��ϸ澯
    //Unit test change uAlarmID to uAlarmID
    SActiveAlarm *pFatalAlarm = Find(AT_FAULT, uAlarmID, yAlarmPara);
    SActiveAlarm *pRestoreAlarm = CreateActiveAlarm(yAlarmType,
                                                        uAlarmID,
                                                        yAlarmPara, nRecoverType);

    if(pFatalAlarm != NULL && pRestoreAlarm != NULL)
    {
        //���ݸ澯�������ɻ�澯����֡
        pRestoreAlarm->uCsn = pFatalAlarm->uCsn;
        remove(pFatalAlarm);
        delete pFatalAlarm;

        //��Ӹûָ��澯��������,�����ظøø澯
        push_back(pRestoreAlarm);
        return pRestoreAlarm;
    }
    //���򷵻�NULL
    else
    {        
        //OM����:��û���ҵ���澯������û����ȷ���ɻָ��澯����ֱ�ӷ���NULL,��Ȼ��Ӱ��ͬ���ŵ���ȷ������
        if(pRestoreAlarm != NULL)
        {
            //����������ָ��澯������Ҫ���
            if(RESET_RECOVER == nRecoverType)
            {
                push_back(pRestoreAlarm); 
            }
            else
            {
                delete pRestoreAlarm;
                pRestoreAlarm = NULL;
            }
        }
        
        return pRestoreAlarm;
    }
}

//////////////////////////////////////////////////////////////////
//��������  ��ʼ���澯�����и澯ģ��
//�������  sActiveAlarm - �澯ģ�����
//�������  ��
//����ֵ    ʧ��Ϊ����,0 Ϊ�ɹ�
//////////////////////////////////////////////////////////////////
int CActiveAlarmList::InitList(SActiveAlarm &sActiveAlarm,CAlarmConfig *pConfig)
{
    memcpy(&m_ActiveAlarmTmpl,&sActiveAlarm,sizeof(SActiveAlarm));
    m_pAlmCfg = pConfig;

    return 0;
}




