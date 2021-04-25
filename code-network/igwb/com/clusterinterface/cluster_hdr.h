/*************************************************************************************
 *��Ʒ��        ��iGateway Bill V200
 *ģ����        ��˫���ӿ�(ClusterInterface)
 *�ļ���        ��cluster_hdr.h
 *������Ϣ      �����ļ�����˫���ӿ�ģ����Ҫʹ�õ�ͷ�ļ������ݽṹ��
 *�汾˵��      ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ      ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����          ������ƽ
 *����ʱ��      ��2001-10-30
 *�޸ļ�¼      ��
**************************************************************************************/
#ifndef _CLUSTER_HEADER_H_
#define _CLUSTER_HEADER_H_

#include "../include/resource.h"
#include "../include/frame.h"
#include "../include/precom.h"
#include "../include/toolbox.h"

//˫���ӿ�ģ���ʼ��״̬ö��ֵ
enum ECLSTRINTSTATUS
{
    CLSTRINT_INIT,              //��ʼ��״̬
    CLSTRINT_ACTIVING,	        //����ִ�м����״̬��
    CLSTRINT_ACTIVE,	        //����״̬��
    CLSTRINT_DEACTIVING,        //����ִ��ȥ�����״̬��
    CLSTRINT_DEACTIVE,	        //ȥ����״̬��
    CLSTRINT_FAILURE,	        //����״̬��
    CLSTRINT_FAILOVER           //����ִ�й��ϵ�����״̬��
};

#endif  //_CLUSTER_HEADER_H_
