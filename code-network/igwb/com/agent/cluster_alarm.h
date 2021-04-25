/**********************************************************************
 *��Ʒ����   iGateway Bill V200
 *ģ������   agent
 *�ļ�����   cluster_alarm.h
 *������Ϣ�� ���ļ�����CClusterAlarm�Ķ����ļ���
             CClusterAlarm���װ�˼��iGateway Bill V200��������SC����
             ��صĹ����Լ�һЩ��Դ�����ϵĹ��ϡ�
 *�汾˵���� V200R002i00M00B00D1130A
 *��Ȩ��Ϣ�� ��Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *���ߣ�     ����ƽ
 *����ʱ�䣺 2001-10-30
 *�޸ļ�¼��
 **********************************************************************/
#ifndef _CLUSTER_ALARM_H_
#define _CLUSTER_ALARM_H_

#include "agent_hdr.h"
#include "../include/frame.h"
#include "cluster.h"
#include "cluster2.h"
#include "cluster3.h"

class CCluster;
class CCluster2;
class CCluster3;

class CClusterAlarm
{
public:
    CClusterAlarm(CCluster *ptheCluster);
    ~CClusterAlarm();

    void Alarm(void);                //�澯���

private:
    PInnerAlarm GetAlarmItem(UINT2 uAlarmID, BYTE*  yAlarmPara);
    void SetAlarmItem(PInnerAlarm pAlarmItem, BOOL bStatus);
    
public:
    void AlarmPrvNet(void);          //���˽���澯
    void AlarmSwitch(void);          //��⵹���澯
    void AlarmSwitchRestore(void);   //��⵹���ָ��澯
    void AlarmVolume(UINT2 uFlag);   //����ʧЧ�澯
    void AlarmResource(void);        //�����ԴʧЧ�澯
    void AlarmIPs(void);             //���IP��ԴʧЧ�澯
    void AlarmDisks(void);           //���Ӳ�̹��ϸ澯

private:
    LIST<SInnerAlarm*>*      m_pAlarmList;    //�����澯���б�
    CCluster*                m_ptheCluster;   //SC�������
    BOOL                     m_bAlarmRestore; //�����ָ���־
    STRING                   m_sResourceType; //������Դ����(FTAM/FTP)
    VM_TYPE                  m_uVMType;       //������������
    STRING                   m_sFrontVolume;  //ǰ�����߼�����
    STRING                   m_sFrontDG;      //ǰ�������ڵĴ�������
    STRING                   m_sBackVolume;   //������߼�����
    STRING                   m_sBackDG;       //��������ڵĴ�������
};

#endif   //_CLUSTER_ALARM_H_
