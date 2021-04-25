#ifndef _BTWSERVICE_H
#define _BTWSERVICE_H
#include "winsvc.h"

//���º궨����CtrlService��nCtrlCode�������֡�
#define CMD_START_SERVICE 0 //�������������֡�
#define CMD_STOP_SERVICE 1  //ֹͣ���������֡�
#define CMD_QUERY_SERVICE 2 //��ѯ����״̬�����֡�
#define CMD_CHECK_SERVICE 3 //�������Ƿ���ϵͳ�д��ڡ�
//��szService�ַ���ָ���ķ�����в��������������ϵĺ궨�塣
BOOL CtrlService(LPCTSTR szService, int nCtrlCode, void* buf = NULL, int len = 0);

//����szService�ַ���ָ���ķ���
BOOL StartService(LPCTSTR szService);
//����schServiceָ���ķ���schServiceΪOpenService()�������صľ��
BOOL StartService(SC_HANDLE  schService);

//ֹͣszService�ַ���ָ���ķ���,���Ὣ�������ķ���ֹͣ��
BOOL StopService(LPCTSTR szService);
//ֹͣschServiceָ���ķ���schServiceΪOpenService()�������صľ��
BOOL StopService(SC_HANDLE  schService);
//ֹͣschService�����ķ���
BOOL StopDependentService(SC_HANDLE schService);

//��ѯszService�ַ���ָ���ķ���ĵ�ǰ״̬��
BOOL QueryServiceStatus(LPCTSTR szService, LPSERVICE_STATUS lpServiceStatus);

#endif