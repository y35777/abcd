/***************************************************************************
 *��Ʒ��        ��iGateway Bill V200
 *ģ����        ��agent
 *�ļ���        ��agent_hdr.h
 *������Ϣ      �����ļ�����˫������ģ��agent��Ҫʹ�õ�ͷ�ļ������ݽṹ��
 *�汾˵��      ��V200R002i00M00B00D1130A
 *��Ȩ��Ϣ      ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����          ������ƽ
 *����ʱ��      ��2001-10-30
 *�޸ļ�¼      ��
****************************************************************************/

#ifndef _AGENT_HEADER_H_
#define _AGENT_HEADER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include "../include/precom.h"
#include "../include/toolbox.h"

//������ɼ�Ⱥ���߼�������һЩ��������Ϣ���������������������顢
//NAFO��ȣ�ͨ����Щ�����ж��ʵ����ÿ��ʵ��ʹ��һ������������

#define  MAX_SC_NODES    4      //SUN SC֧�ֵ����ڵ�����ĿǰΪ4����

//�����߼���������Ϣ�������߼����������֡��߿������ݷ���������Լ�
//�߼������������Ϣ
typedef struct _LogicalHost
{
    STRING   sLogHostName;      //�߼�������
    STRING   sDataServiceName;  //�߿������ݷ�����

    LIST <STRING> Cluster_Nodes;
    LIST <STRING> Cluster_Heartbeats; //just used in the SC3.0 now
    //�߼���������Ĺ�������飬�����ж����������飬ÿ��������ͨ��
    //������Ψһȷ��
    LIST <STRING> Cluster_Diskgroups;  

    //SCͨ��NAFO(Network Adapter Failover)����������IP��ַ��ÿ��
    //NAFO��ͨ��������Ψһȷ��
    LIST <STRING> Cluster_NAFO;          //NAFO��

    //SResGroup  ResGroup;     //��Դ��
    //STRING       sResGroupName;    //��Դ����
    //LIST<STRING> Resources;   //��Դ��

}SLogicalHost, *PLogicalHost;

//˫��������·����
enum HBLINKTYPE
{
    HL_UDP = 0,      //"UDP"
    HL_COM = 1       //"COM"
};

//��������״̬
struct THBState
{
    HBLINKTYPE  LinkType;       //������·����
    BOOL        bIsAlive;       //�����Ƿ����
};

//SC֧�ֵľ�������(Volume Manager)����
//ĿǰSC2.2֧�����־����������ͣ�
//1. Solstice DiskSuite           SDS
//2. Sun StorEdge Volume Manager  SSVM
//3. Cluster Volume Manager       CVM
enum VM_TYPE
{
    VT_VXVM,            //Veritas Volume Manager
    VT_SDS,             //Solstice DiskSuite
    VT_CVM              //Cluster Volume Manager(�����õ�)
};

#define FRONT_VOLUME      0
#define BACK_VOLUME       1

#endif  //_AGENT_HEADER_H_

