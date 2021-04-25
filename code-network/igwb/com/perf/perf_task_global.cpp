#include "perf_task_global.h"

void SendMMLReport(CMMLReport *pReport,UINT2 uAppType,UINT4 uIPAddr)
{
    if (pReport == NULL)
    {
        return;
    }
	//计算报文长度，按长度分配空间
	int nLength       = strlen(pReport->GetReport());
	MSG_INTRA *pFrame = new(nLength + 1)MSG_INTRA;
	memcpy(pFrame->Data, pReport->GetReport(), nLength);
	
    //最后一个字符为'\0 '，表示字符串结束
    pFrame->Data[nLength] = '\0';
	
    //填写帧头内容
	//by ldf 2003-12
    pFrame->IPAddr      = uIPAddr;
    pFrame->AppType     = uAppType;
    pFrame->SenderPid   = PT_OM;
    pFrame->SenderMid   = MT_ALARM;
    pFrame->ReceiverPid = PT_OUTSIDE;
    pFrame->ReceiverMid = MT_MMLDEBUG; //????
	
	//调用框架的发送函数将消息帧发送到网络模块转发
#ifdef UNIT_TEST
    printf("Send Report = \n%s\n",pReport->GetReport());
#else
    CWorkModule::SendMsg(pFrame);
#endif
}

