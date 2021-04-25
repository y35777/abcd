#include "perf_task_global.h"

void SendMMLReport(CMMLReport *pReport,UINT2 uAppType,UINT4 uIPAddr)
{
    if (pReport == NULL)
    {
        return;
    }
	//���㱨�ĳ��ȣ������ȷ���ռ�
	int nLength       = strlen(pReport->GetReport());
	MSG_INTRA *pFrame = new(nLength + 1)MSG_INTRA;
	memcpy(pFrame->Data, pReport->GetReport(), nLength);
	
    //���һ���ַ�Ϊ'\0 '����ʾ�ַ�������
    pFrame->Data[nLength] = '\0';
	
    //��д֡ͷ����
	//by ldf 2003-12
    pFrame->IPAddr      = uIPAddr;
    pFrame->AppType     = uAppType;
    pFrame->SenderPid   = PT_OM;
    pFrame->SenderMid   = MT_ALARM;
    pFrame->ReceiverPid = PT_OUTSIDE;
    pFrame->ReceiverMid = MT_MMLDEBUG; //????
	
	//���ÿ�ܵķ��ͺ�������Ϣ֡���͵�����ģ��ת��
#ifdef UNIT_TEST
    printf("Send Report = \n%s\n",pReport->GetReport());
#else
    CWorkModule::SendMsg(pFrame);
#endif
}

