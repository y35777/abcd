/***************************************************************************
 *��Ʒ��        ��iGateway Bill V200
 *ģ����        ��monitor
 *�ļ���        ��monitor_hdr.h
 *������Ϣ      �����ļ��������ģ����Ҫʹ�õ�ͷ�ļ������ݽṹ��
 *�汾˵��      ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ      ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����          ������ƽ
 *����ʱ��      ��2001-10-30
 *�޸ļ�¼      ��
****************************************************************************/

#ifndef _MONITOR_HEADER_H_
#define _MONITOR_HEADER_H_

#include "../include/precom.h"
#include "../include/resource.h"
#include "../include/toolbox.h"
#include "../include/frame.h"

extern long g_lDebugVersion;

enum EAPPSTATUS
{
    APP_STOPPED,                //ֹͣ״̬
    APP_READY,                  //���յ�ҵ����̹���ģ��ĳ�ʼ��Ӧ��
    APP_SCHEDULED_START,        //��������ǰ�ĵȴ�״̬
    APP_START_PENDING,          //��������״̬
    APP_STOP_PENDING,           //����ֹͣ״̬
    APP_ACTIVING,               //���ڼ���״̬
    APP_ACTIVE                  //���в�����״̬
};

enum EAUXUPGSTATUS
{
    AUXUPG_STOPPED,             //δ������������״̬
    AUXUPG_START,               //����ִ�и�������״̬
    AUXUPG_COMPLETED            //�������������״̬
};

struct SMappedAppInfo
{
    PROCESS_TYPE nPType;        //ҵ����̱��
    BOOL bStart;                //�Ƿ�������ģ��
    BOOL bCanRestart;           //�ý������쳣�˳�ʱ�Ƿ���������ý���
    char szAppName[100];        //��ҵ����̵�Name
    char szPathName[MAX_PATH];  //��ҵ����̵ĳ���·����ȫ·����
};

enum EPROCMGRSTATUS
{
    PROCMGR_STOPPED,            //ֹͣ״̬
    PROCMGR_READY,              //���յ�ҵ����̹���ģ��ĳ�ʼ��Ӧ��
    PROCMGR_START_PENDING,      //��������״̬
    PROCMGR_STOP_PENDING,       //����ֹͣ״̬
    PROCMGR_ACTIVING,           //���ڼ���״̬
    PROCMGR_ACTIVE,             //���в�����״̬
    PROCMGR_FAILURE             //����״̬
};

enum EKERNELSTATUS
{
    KERNEL_INIT_PENDING,        //Kernel���ڳ�ʼ��״̬���������ȴ�ҵ����̷���
                                //��ʼ���ɹ�����Ϣ��
    KERNEL_READY,               //Kernel��ʼ���ɹ������ڵȴ��������
    KERNEL_ACTIVING,            //Kernel����ִ�м����״̬��
    KERNEL_ACTIVE,              //Kernel���ڼ���״̬��
    KERNEL_DEACTIVING,          //Kernel����ִ��ȥ�����״̬��
    KERNEL_DEACTIVE,            //Kernel����ֹͣ״̬
    KERNEL_FAILURE,             //Kernel���ڹ��ϵ�״̬��
    KERNEL_FAILOVER,            //Kernel��������������ϵ�����״̬
    KERNEL_SHUTDOWNING          //Kernel���������˳���״̬
};

#endif //_MONITOR_HEADER_H_
