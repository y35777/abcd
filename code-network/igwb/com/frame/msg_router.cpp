/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:msg_router.cpp
    ������Ϣ:
    
    ����CMsgRouter���ʵ��,�������������������н�����Ϣ.
    CMsgProcessor���������Ķ���,��֧����Ϣ����.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/

#include "msg_router.h"
#include "msg_proc.h"

/* CTimerManager��Ķ���ͷ�ļ�. */
#include "../include/comm.h"
#include "../include/toolbox.h"
#include "../utility/mml_report.h"

/* ����CperfIterm���ͷ�ļ� */
#include "../include/perf_item.h"

CMsgRouter::CMsgRouter(const PROCESS_TYPE pt)
{
    m_pt = pt;
    m_bTrack = FALSE;
}

CMsgRouter::~CMsgRouter()
{
    m_RouteTab.clear();
}

/*
    ��������:��·�ɱ���ע����Ϣ�������.
    һ�����͵Ķ���ֻ����·�ɱ���ע��һ������.

    1.��·�ɱ��д��ڶ�Ӧ���͵Ķ���,����FALSE.
    2.��·�ɱ��в����ڶ�Ӧ���͵Ķ���,����Ϣ�������׷�ӵ�����.

    ����:
    mt,ģ������.
    pModule,��Ϣ�������ָ��.

    ����:ע��ɹ�����TRUE,ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
BOOL CMsgRouter::Register(const MODULE_TYPE mt, CMsgProcessor* pModule)
{
    m_RouterRWMutex.acquire_write();

    MAPROUTE::iterator i = m_RouteTab.find(mt);
    if(i != m_RouteTab.end())
    {
        m_RouterRWMutex.release();
        return FALSE;
    }

    //ע��ָ��ҵ��ģ�鵽·�ɱ�
    m_RouteTab[mt] = pModule;

    m_RouterRWMutex.release();
    return TRUE;
}

/*
    ��������:��·�ɱ���ɾ����Ϣ�������.
    һ�����͵Ķ�����·�ɱ�ֻ��һ������.

    1.��·�ɱ��д��ڶ�Ӧ���͵Ķ���,��ɾ��.
    2.��·�ɱ��в����ڶ�Ӧ���͵Ķ���,�����κδ���.

    ����:
    mt,ģ������.

    ����:�ú���ֻ����TRUE.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/

BOOL CMsgRouter::UnRegister(const MODULE_TYPE mt)
{
    m_RouterRWMutex.acquire_write();

    MAPROUTE::iterator i = m_RouteTab.find(mt);
    if(i != m_RouteTab.end())
    {
        m_RouteTab.erase(i);
    }

    m_RouterRWMutex.release();

    return TRUE;
}

/*
    ��������:���͸�����Ϣ.
    ������Ϣ������MML Server,�������͵�MML�ͻ���.

    ����:pMsgTracked��Ϣָ��.

    ����:
*/
BOOL CMsgRouter::SendTrackedMsg(MSG_INTRA* pMsgTracked)
{
    char szBuf[256];

    /* ����MML����ͷ. */

    CMMLReport Report(MTS_MSGDIST);
    Report.MakeReportHead();

    /* ����ͷ�ڶ��� */
    Report.AddString(S_FRAME_MML_REPORT_HEADER_PART0);
    //by ldf 2002.6.10 ��Ӧ���ⵥD14393
    Report.FillSpace(3);
    //�޸����

    Report.AddString(STR_NEWLINE);

    /* ���� */
    Report.MakeReportItem2(S_FRAME_SENDER_PID,
        CMsgProcessor::GetProcessDesp((PROCESS_TYPE)
        (pMsgTracked->SenderPid)));

    Report.MakeReportItem2(S_FRAME_SENDER_MID,
        CMsgProcessor::GetModuleDesp((MODULE_TYPE)
        (pMsgTracked->SenderMid)));

    Report.MakeReportItem2(S_FRAME_RECEIVER_PID,
        CMsgProcessor::GetProcessDesp((PROCESS_TYPE)
        (pMsgTracked->ReceiverPid)));

    Report.MakeReportItem2(S_FRAME_RECEIVER_MID,
        CMsgProcessor::GetModuleDesp((MODULE_TYPE)
        (pMsgTracked->ReceiverMid)));

    sprintf(szBuf,"%d",pMsgTracked->AppType);
    Report.MakeReportItem2(S_FRAME_APP_TYPE,szBuf);

    sprintf(szBuf,"%d",pMsgTracked->ClientNo);
    Report.MakeReportItem2(S_FRAME_CLIENT_NO,szBuf);

    Report.MakeReportItem2(S_FRAME_USER_NAME,pMsgTracked->UserName);

    struct in_addr addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
    addr.s_addr = pMsgTracked->IPAddr;
#else //_PLATFORM_HPUX
    addr.S_un.S_addr = pMsgTracked->IPAddr;
#endif//_PLATFORM_HPUX
    Report.MakeReportItem2(S_FRAME_CLIENT_IP,ACE_OS::inet_ntoa(addr));

    sprintf(szBuf,"%d",pMsgTracked->CmdCode);
    Report.MakeReportItem2(S_FRAME_CMD_CODE,szBuf);

    sprintf(szBuf,"%d",pMsgTracked->Len);
    Report.MakeReportItem2(S_FRAME_DATA_LENGTH,szBuf);

    /* ���챨��β */
    Report.MakeReportTail();

    const char* p = Report.GetReport();
    MSG_INTRA *pMsg = new(strlen(p) + 1) MSG_INTRA;
    strcpy((char*)(pMsg->Data),p);

    pMsg->SenderPid = CMsgProcessor::GetProcessType(); 
    pMsg->SenderMid = MT_UNKNOWN;
    pMsg->ReceiverPid = PT_OM;
    pMsg->ReceiverMid = MT_MMLDEBUG;
    pMsg->AppType = APP_MML_DEBUG;
    pMsg->CmdCode = CMD_TRACKMSG;

    return (CMsgProcessor::SendMsg(pMsg) == ERR_SUCCESS);
}

/*
    ��������:��һ����Ϣ�ַ���·�ɱ��е�ĳ����Ϣ�������.

    1.����Ϣ���ն������,�����Ϣ������ն������Ϣ������.
    2.����Ϣ���ն��󲻴���,������Ϣ.
    3.����Ϣ����ʧ��,������Ϣ��дTrace.

    ����:
    pMsgRouted,���ַ�����Ϣָ��.

    ����:���ش������,������붨����basetype.h�ļ���.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
int CMsgRouter::Route(MSG_INTRA* pMsgRouted)
{
    ASSERT(pMsgRouted != NULL);

    /*
        m_bTrack = TRUE,���͸�����Ϣ.
        ������Ϣͨ��MML Server��������̨.
    */
    if(m_bTrack && (pMsgRouted->CmdCode != CMD_TRACKMSG))
    {
        SendTrackedMsg(pMsgRouted);
    }

    /*
        ����:����������OMM����,����Ϣ�Ľ���ģ����MT_MML_DEBUGģ��,
        �����Ϣ�Ľ��ս��̸���ΪPT_OUTSIDE,�Ա����Ϣ���͸������,
        ���շ��͵�MML�ͻ���.
    */
    if((m_pt == PT_OM) && (pMsgRouted->ReceiverMid == MT_MMLDEBUG))
    {
        pMsgRouted->ReceiverPid = PT_OUTSIDE;
    }

    /*
        ��Ϣ�Ľ������ǽ�����ģ��,����Ϣͨ��ͨ�Ų㷢�ͳ�ȥ.
    */
    if ((pMsgRouted->ReceiverPid != m_pt)
        && (pMsgRouted->ReceiverPid != PT_SELF))
    {
        int nRet = CMsgProcessor::m_ptheCommLayer->Send(pMsgRouted);
        //���ⵥ 2002-03-13 delete old code for D11689
		//���ⵥ 2002-04-26 ���ⵥD13074
		delete pMsgRouted;
        return nRet;
    }
    else
    {
        /* ��Ϣ���ն�����·�ɶ��� */
        if(pMsgRouted->ReceiverMid == MT_MSGDIST)
        {
            ProcessOwnMsg(pMsgRouted);          
            return ERR_SUCCESS;
        }

        /*
            ��·�ɱ��в�����Ϣ���ն���.

            1.����Ϣ���ն������,�����Ϣ����������Ϣ������.
            2.����Ϣ���ն��󲻴���,������Ϣ.
            3.�ڱ���·�ɱ�֮ǰ��Ҫ������.
        */
        m_RouterRWMutex.acquire_read();

        MAPROUTE::iterator i = m_RouteTab.find(pMsgRouted->ReceiverMid);
        if(i != m_RouteTab.end())
        {
            int nRet = (*i).second->Enqueue(pMsgRouted);

            m_RouterRWMutex.release();
            if(nRet != ERR_SUCCESS)
            {
                //������ 2002-03-13 delete old code for ���ⵥ D11689
                /* ��Ϣ���ʧ��,������Ϣ */
                delete pMsgRouted;
            }

            return nRet;
        }

        m_RouterRWMutex.release();

        //������ 2002-03-13 delete old code for ���ⵥD11689
        delete pMsgRouted;

        return ERR_FAIL;
    }
}

/*
    ��������:�����͸�CMsgRouter�������Ϣ.
    �����ֲ���ʶ���С��дTrace.

    ����:
    pMsgOwn,��Ϣָ��.

    ����:��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CMsgRouter::ProcessOwnMsg(MSG_INTRA* pMsgOwn)
{
    switch(pMsgOwn->CmdCode)
    {
    case CMD_TRACKMSG_ENABLE:       //����Ϣ����
        m_bTrack = TRUE;
        break;
    case CMD_TRACKMSG_DISABLE:      //ȡ����Ϣ����
        m_bTrack = FALSE;
        break;
    case CMD_TRACKPROTOCOL_ENABLE:  //��Э�����
        CMsgProcessor::g_bTrackProtocol = TRUE;
        break;
    case CMD_TRACKPROTOCOL_DISABLE: //ȡ��Э�����
        CMsgProcessor::g_bTrackProtocol = FALSE;
        break;
    
	//���ⵥ��SWPD00562��2003��7��23�գ�Zou Yongsheng�޸�
    case CMD_SET_MSGOUT_LEVEL:      //�����������
		CMsgProcessor::g_nMsgOutLevel = pMsgOwn->Data[0];
		break;
    //�޸Ľ���
    case CMD_PERFITERM_ENABLE:      //������ͳ��
        {
            if(CMsgProcessor::g_bPerfIterm != TRUE)
            {
                CMsgProcessor::g_bPerfIterm = TRUE;
                CPerfItem::BeginStatistic();
                CPerfItem::SetPerfOn(TRUE);
            }
        }
        break;
    case CMD_PERFITERM_DISABLE:     //ȡ������ͳ��
        {
            CMsgProcessor::g_bPerfIterm = FALSE;
            CPerfItem::SetPerfOn(FALSE);
        }
        break;
    default:
        {
            /* δ��ʶ�����Ϣ,дTrace */
			//������ 2002-04-25 ���ⵥ 12535
            //TRACE(CMsgProcessor::GetModuleDesp(MT_MSGDIST),
            //                        S_FRAME_UNPROCESSED_MSG,
            //                        pMsgOwn->SenderPid,
            //                        pMsgOwn->SenderMid,
            //                        pMsgOwn->CmdCode);
        }
        break;
    }

    delete pMsgOwn;
}
