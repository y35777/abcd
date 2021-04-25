
#include "co_manager.h"
#include "../../com/utility/frame_adapter.h"
#include "../../com/toolbase/frame_tool.h"

#include "../../com/include/toolbox.h"
#include "../../com/include/signal_tab.h"
#include "../../com/collector/collector.h"
#include "../../com/colconvert/collect_convertor.h"
#include "resource.h"

/****************************************************************************
����ԭ��    public:CCollectorMgr::CCollectorMgr()
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
CCollectorMgr::CCollectorMgr()
{
    //by ldf �������ⵥ D11610  2002.3.14�޸�
    m_nRecvSignal   = 0;
    m_pFrameAdapter = NULL;
}


/****************************************************************************
����ԭ��    public:CCollectorMgr::~CCollectorMgr()
��������    ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
CCollectorMgr::~CCollectorMgr()
{
    if(m_pFrameAdapter)
    {
        delete m_pFrameAdapter;
        m_pFrameAdapter = NULL;
    }
}


/****************************************************************************
����ԭ��    protected:int CCollectorMgr::InitInstance(void)
��������    ��ʼ������
�������    ��
�������    ��
����ֵ      ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CCollectorMgr::InitInstance(void)
{
    int nRet = CModuleManager::InitInstance();
    if (nRet != ERR_SUCCESS) return nRet;

    ACE_Sig_Set sigset;
    sigset.fill_set();
    reactor()->register_handler(sigset, this, 0);

    m_pFrameAdapter = new CFrameAdapter;
    CFrameTools::SetInstance(m_pFrameAdapter);

    //��KERNEL�����еļ��ģ�鷢��ʼ��Ӧ��
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = GetProcessType();
    pMsg->SenderMid     = GetModuleType();
    pMsg->ReceiverPid   = PT_KERNEL;
    pMsg->ReceiverMid   = MT_MONITOR;
    pMsg->CmdCode       = CMD_ANS_INIT;
    SendMsg(pMsg);
    
    return ERR_SUCCESS;
}

/****************************************************************************
����ԭ��    protected:int CCollectorMgr::ExitInstance(void)
��������    ������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CCollectorMgr::ExitInstance(void)
{
    CFrameTools::SetInstance(NULL);
    CModuleManager::ExitInstance();
}

/****************************************************************************
����ԭ��    protected:int CCollectorMgr::CreateWorkModules(void)
��������    ������ģ���߳�
�������    ��
�������    ��
����ֵ      ERR_SUCCESS---�ɹ�
            ERR_FAIL   ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CCollectorMgr::CreateWorkModules(void)
{
    //�ɼ�ģ��
    m_WorkModules.push_back(new CCollectorModule());

    //added by chenliangwei
    //�ɼ�ת��ģ��
    m_WorkModules.push_back(new CCollectConvertorModule());
    //added end

    return ERR_SUCCESS;
}

/****************************************************************************
����ԭ��    protected:void CCollectorMgr::ParseMsg(MSG_INTRA* pMsgParsed)
��������    ������Ϣ֡
�������    pMsgParsed--��Ϣָ֡��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CCollectorMgr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch(pMsgParsed->CmdCode)
    {
    case CMD_AUXUPGRADE:
        BeginAuxUpgrade();
        break;

    case CMD_ANS_AUXUPGRADE:
        EndAuxUpgrade();
        break;

    default:
        CModuleManager::ParseMsg(pMsgParsed);
        break;
    }
}


/****************************************************************************
����ԭ��    protected:void CCollectorMgr::BeginAuxUpgrade(void)
��������    ��ʼ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CCollectorMgr::BeginAuxUpgrade(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = PT_SELF; 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_SELF;
    pMsg->ReceiverMid   = MT_COLLECTOR;
    pMsg->CmdCode       = CMD_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_BEGIN_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_BEGIN_AUXUPGRADE);
}


/****************************************************************************
����ԭ��    protected:void CCollectorMgr::EndAuxUpgrade(void)
��������    ������������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CCollectorMgr::EndAuxUpgrade(void)
{
    MSG_INTRA *pMsg     = new MSG_INTRA;
    pMsg->SenderPid     = GetProcessType(); 
    pMsg->SenderMid     = MT_MANAGER;
    pMsg->ReceiverPid   = PT_KERNEL;
    pMsg->ReceiverMid   = MT_MONITOR;
    pMsg->CmdCode       = CMD_ANS_AUXUPGRADE;
    SendMsg(pMsg);

    TRACE(THISMODULE, S_END_AUXUPGRADE);
    MSGOUT(THISMODULE, MSGOUT_LEVEL_IMPORTANT, S_END_AUXUPGRADE);
}


/****************************************************************************
����ԭ��    protected:int CCollectorMgr::handle_signal(int signum,
                                               siginfo_t* st,
                                               ucontext_t* ut)
��������    �����ź�
�������    signum---�ź���
�������    ��
����ֵ      0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣    ��
*****************************************************************************/
int CCollectorMgr::handle_signal(int signum,
                          siginfo_t* st,
                          ucontext_t* ut)
{
    //by ldf �������ⵥ D11610  2002.3.14�޸�
    if(( signum < 0) || ( signum > 36))
    {
        TRACE(THISMODULE, S_RECV_UNKNOW_SIGNAL, signum);
        return 0;
    }

    m_nRecvSignal = signum;

    //�������Ҫ�˳����źţ���ע���Ը��źŵĲ���
    if (g_nSignalToExit[m_nRecvSignal])
    {
        return -1;
    }
    return 0;
}

/****************************************************************************
����ԭ��    protected:void CCollectorMgr::OnTimer(const long nTimerID)
��������    ��ʱ������
�������    nTimerID---��ʱ��ID
�������    ��
����ֵ      ��
�׳��쳣    ��
*****************************************************************************/
void CCollectorMgr::OnTimer(const long nTimerID)
{
    if(nTimerID == m_lTimerIDBase)
    {
        //by ldf �������ⵥ D11610  2002.3.14�޸�
        if ((m_nRecvSignal > 0) && (m_nRecvSignal <= 36))
        {
            if (g_nSignalToExit[m_nRecvSignal])
            //��Ҫ�˳����ź�
            {
                TRACE(THISMODULE, S_RECV_SIGNAL_EXIT,
                    g_szSignalDesp[m_nRecvSignal]);

                SendQuitToMe();
            }
            else
            //���Ժ��Ե��ź�
            {
                TRACE(THISMODULE, S_RECV_SIGNAL_IGNORED,
                    g_szSignalDesp[m_nRecvSignal]);
            }
        }
        else if (m_nRecvSignal != 0)
        {
            TRACE(THISMODULE, S_RECV_SIGNAL_DEFAULT, m_nRecvSignal);
        }
        m_nRecvSignal = 0;
    }

    CModuleManager::OnTimer(nTimerID);
}
