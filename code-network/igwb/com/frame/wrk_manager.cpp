/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:wrk_manager.cpp
    ������Ϣ:
    
    ����CModuleManager���ʵ��,�������������������н�����Ϣ.
    CMsgProcessor���������Ķ���,��֧����Ϣ����.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-16,����.
*/

#include "wrk_manager.h"
#include "msg_router.h"
#include "wrk_module.h"

/* ����CCommlayer���ͷ�ļ� */
#include "../include/comm.h"

/* ����CperfIterm���ͷ�ļ� */
#include "../include/perf_item.h"
//������ 2002-04-25 ���ⵥ D12515
#include "../toolbase/ini_file.h"
#include "../utility/config.h"
CModuleManager::CModuleManager()
{
    m_lTimerIDBase = -1;
    m_lTimerCounterPerf = 0;
    m_nModuleReady = 0;

    m_nStatus = WKMGR_INIT_PENDING;
}

CModuleManager::~CModuleManager()
{
}

/*
    ��������:��ʼ������,����Ϣѭ�������е���.

    1.������Ϣ·�ɶ���.
    2.��������ͨ�Ų����.
    3.��·�ɱ���ע���Լ�.
    4.����һ����׼��ʱ��.
    5.��������ҵ��ģ��.
    6.������ʱ��.

    ����:���ش������,������붨����basetype.h�ļ���.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
int CModuleManager::InitInstance(void)
{
    ASSERT(GetProcessType() > 0);
    int nRet = ERR_SUCCESS;

    if(m_ptheRouter == NULL)
    {
        m_ptheRouter = new CMsgRouter(GetProcessType());
    }

    if(m_ptheCommLayer == NULL)
    {
        m_ptheCommLayer = CCommLayer::Instance(GetProcessType());
        if(m_ptheCommLayer == NULL)
        {
            TRACE(MTS_MANAGER,S_FRAME_CREATE_COM_OBJ_FAIL);
            return ERR_FAIL;
        }
    }

    //���û����InitInstance����,��·�ɱ���ע���Լ�.
    nRet = CMsgProcessor::InitInstance();
    if (nRet != ERR_SUCCESS)
    {
        return nRet;
    }
	
    m_lTimerIDBase = SetTimer(ONE_SECOND_IN_MILISEC);

    //��������ҵ��ģ�����.
    nRet = CreateWorkModules();
    if(nRet != ERR_SUCCESS)
    {
        return nRet;
    }

    //ʱ�ӱ����ڱ�����������.
    m_theTimerMgr.Run();

    //��״̬ΪREADY,�����ʼ���ɹ�,�ȴ�Kernel�ļ�������.
    m_nStatus = WKMGR_READY;
    //������ 2002-04-25 ���ⵥD12515
    //������ 2002-03-13 ���ⵥD11728
	CINIFile iniFile(GetCfgFilePath());
	BOOL bRet = iniFile.Open(); 
	if(bRet != TRUE)
	{
		char szTmpBuf[256];
		sprintf(szTmpBuf, "\r\n\t\t\t\t     Errno: %d (%s)", errno, strerror(errno));
		STRING sTmpInfo = S_FRAME_OPEN_INI_FAIL;//"�������ļ�ʧ��"
		sTmpInfo += GetCfgFilePath();
		sTmpInfo += szTmpBuf;
		TRACE(THISMODULE,sTmpInfo.c_str());
		return ERR_FAIL;
	}
	CMsgProcessor::g_nMsgOutLevel = iniFile.GetInt(CFG_SEC_COMMON,
	                                               CFG_COMMON_KEY_MSGOUTLEVEL,
	                                               MSGOUT_LEVEL_IMPORTANT|MSGOUT_LEVEL_URGENT);
	iniFile.Close();
	//�޸Ľ���
    return ERR_SUCCESS;
}

/*
    ��������:��Ϣ������.

    ����:
    pMsgParsed,��Ϣָ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CModuleManager::ParseMsg(MSG_INTRA* pMsgParsed)
{
    static BOOL bRecvActiveCmd = FALSE;

    switch (pMsgParsed->CmdCode)
    {
    case CMD_ACTIVE:
        /*
            �յ�Kernel���͵ļ�������,�������ҵ��ģ��.
            ֻ��Ӧһ�μ�������.
        */
        if((bRecvActiveCmd == TRUE) || (m_nStatus != WKMGR_READY))
        {
            return;
        }

        bRecvActiveCmd = TRUE;

        if(ActiveWorkModules() == FALSE) 
        {
            m_nStatus = WKMGR_FAILURE;
            TRACE(MTS_MANAGER,S_FRAME_ACTIVE_MODULE_FAIL);
            SendQuitToMe();
        }

        break;

    case CMD_READY:
        /*
            �յ�����ҵ��ģ�鷢�͵ľ�����Ϣ��,
            �����ҵ��ģ�鷢�ͼ�������,
            ����Kernelģ�鷢�ͼ���Ӧ����Ϣ.
        */
        m_nModuleReady++;    //�ñ�����������ģ���Լ�.
        if(m_nModuleReady == m_WorkModules.size() + 1)
        {
            LIST_WORK_MODULE::iterator i;
            for (i = m_WorkModules.begin();
                 i != m_WorkModules.end(); i++)
            {
                MSG_INTRA *pMsg = new MSG_INTRA;
                pMsg->SenderPid = PT_SELF; 
                pMsg->SenderMid = GetModuleType();
                pMsg->ReceiverPid = PT_SELF;
                pMsg->ReceiverMid = (*i)->GetModuleType();
                pMsg->CmdCode = CMD_MODULE_ACTIVE;
                SendMsg(pMsg);
            }

            m_nStatus = WKMGR_ACTIVE;

            //��Kernel�����еļ��ģ�鷢�ͼ���Ӧ����Ϣ.
            MSG_INTRA *pMsg = new MSG_INTRA;
            pMsg->SenderPid = GetProcessType();
            pMsg->SenderMid = GetModuleType();
            pMsg->ReceiverPid = PT_KERNEL;
            pMsg->ReceiverMid = MT_MONITOR;
            pMsg->CmdCode = CMD_ANS_ACTIVE;

            SendMsg(pMsg);

            MSGOUT(THISMODULE,MSGOUT_LEVEL_IMPORTANT,
                S_FRAME_ACTIVE_MODULE_SUCCEED);
        }
        
        break;
        
    default:
        //������ 2002-04-25 ���ⵥ D12535
		//TRACE(MTS_MANAGER,S_FRAME_UNPROCESSED_MSG,pMsgParsed->SenderPid,
        //                                          pMsgParsed->SenderMid,
        //                                          pMsgParsed->CmdCode);
        break;
    }
}

/*
    ��������:��������ҵ��ģ���߳�.

    1.���ø���ҵ��ģ���Run()�������������߳�.

    ����:�ɹ���������ҵ��ģ�鷵��TRUE,ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
BOOL CModuleManager::ActiveWorkModules()
{
    LIST_WORK_MODULE::iterator i;
    for (i = m_WorkModules.begin(); i != m_WorkModules.end(); i++)
    {
        if(((*i)->Run()) != ERR_SUCCESS)
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*
    ��������:��ֹ����ҵ��ģ���߳�.

    1.ͨ��������ҵ��ģ�鷢���˳���Ϣ����ֹҵ��ģ��.
    ����������Ϣʧ�ܵ����.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CModuleManager::DeActiveWorkModules()
{
    //ֹͣ��ʱ��
    m_theTimerMgr.Stop();

    LIST_WORK_MODULE::iterator i;
    for (i = m_WorkModules.begin(); i != m_WorkModules.end(); i++)
    {
        MSG_INTRA *pMsg = new MSG_INTRA;
        pMsg->SenderPid = PT_SELF; 
        pMsg->SenderMid = GetModuleType();
        pMsg->ReceiverPid = PT_SELF;
        pMsg->CmdCode = CMD_QUIT;
        pMsg->ReceiverMid = (*i)->GetModuleType();

        SendMsg(pMsg);
    }
}

/*
    ��������:�������.

    1.��ֹ���ȴ�����ҵ��ģ�����.
    2.ע����ʱ��.
    3.��ֹ����ͳ��.
    4.ɾ��·�ɱ��ͨ�Ų�.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CModuleManager::ExitInstance(void)
{
    /*
        ��ֹ���ȴ�����ҵ��ģ���˳�.
    */
    DeActiveWorkModules();

    LIST_WORK_MODULE::iterator i;
    for (i = m_WorkModules.begin(); i != m_WorkModules.end(); i++)
    {
        (*i)->Wait();
        delete (*i);
    }

    m_WorkModules.clear();

    KillTimer(m_lTimerIDBase);
    CPerfItem::ReleaseAllItems();
	CLinkStatItem::ReleaseAllItems();
    CMsgProcessor::ExitInstance();

    if(m_ptheCommLayer) 
    {
        m_ptheCommLayer->Destroy();
        m_ptheCommLayer = NULL;
    }

    /*
        ɾ����Ϣ��ת����,������ɾ��ͨ�Ų����֮�����,
        ��Ϊ��Ϣ��ת����ɾ��֮��,����ͨ�Ų������Ȼ�ڷ�����Ϣ.
    */
    if(m_ptheRouter)  
    {
        delete m_ptheRouter;
        m_ptheRouter = NULL;
    }
}

/*
    ��������:����ʱ����Ϣ.

    ����:nTimerIDʱ��ID.

    �޸ļ�¼:
    zhouyonghui,2001-10-16,����.
*/
void CModuleManager::OnTimer(const long nTimerID)
{
    if(nTimerID == m_lTimerIDBase)
    {
        /*
            ͬ������ͳ�ƿ��ص�״̬.
        */
        CPerfItem::SetPerfOn(g_bPerfIterm);

        if((m_lTimerCounterPerf++ > 10) && (g_bPerfIterm == TRUE))
        {
            /*
                ��nTimerIDΪ��׼��ʱ��ID,��ִ������ͳ��.
                1.��ֹ��ǰͳ������.
                2.��ʼһ���µ�ͳ������.
            */

            SPerfItem* pItems = NULL;
            UINT2 nItemCount = 0;

            CPerfItem::EndStatistic(pItems,nItemCount);
            
            m_lTimerCounterPerf = 0;

            if(nItemCount == 0) return;

            MSG_INTRA *pMsg = new(sizeof(SPerfItem) * nItemCount) MSG_INTRA;
            memcpy(pMsg->Data,pItems,sizeof(SPerfItem) * nItemCount);
            delete pItems;

            pMsg->SenderPid = GetProcessType(); 
            pMsg->SenderMid = GetModuleType();
            pMsg->ReceiverPid = PT_OM;
            pMsg->ReceiverMid = MT_PERF;
            pMsg->CmdCode =  CMD_SAVE_PERF;
            
            SendMsg(pMsg);
            CPerfItem::BeginStatistic();            
        }
    }

    /* ʱ����Ϣ�ͻ��� */
    CMsgProcessor::OnTimer(nTimerID);
}
