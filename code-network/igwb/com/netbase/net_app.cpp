#include "net_app.h"
#include "protocol_stack.h"
#include "network.h"
#include "resource.h"


/******************************************************
// ������:  CNetApp::CNetApp
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���캯��
// ����:
//       ����1 :CNetWork* const pNetWork
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CNetApp::CNetApp(CNetWork* const pNetWork)
{
    m_pNetWork = pNetWork;
    m_bActive = FALSE;
    m_nStackStatusInfoType = 0;
    m_pReceiveBillCnt = 0;
}


/******************************************************
// ������:  CNetApp::~CNetApp
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CNetApp::~CNetApp()
{
    m_pNetWork = 0;
    MAP_ID_TO_PS::iterator i;
    for(i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        delete (*i).second;
    }
    m_StackMan.clear();
}


/******************************************************
// ������:  CNetApp::Init
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��ʼ������
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::Init(void)
{
    //������ 2003-05-16 ͳ��ƽ�����ջ�������
    BYTE yAPType = (BYTE)CMsgProcessor::GetProcessType() - PT_AP_BASE + 1;
    m_pReceiveBillCnt = CPerfItem::GetItem(yAPType,
                                           MT_NETWORK,
                                           PERF_RECEIVED_CDR,
                                           PERF_STATS_TOTAL_VALUE);
    return ERR_SUCCESS;
}


/******************************************************
// ������:  CNetApp::Active
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��ǰ����ģ��֪ͨ����ģ�������ָ��Ѿ���ɺ�
//          ������������
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::Active(void)
{
    m_bActive = TRUE;

    MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, S_NET_NETAPP_ACTIVE);
}


/******************************************************
// ������:  CNetApp::DeActive
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ������ģ���ڽ��ո���������ȥ��������ʱ��
//          ������������
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::DeActive(void)
{
    m_bActive = FALSE;

    MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, S_NET_NETAPP_DEACTIVE);
}


/******************************************************
// ������:  CNetApp::LoadStatus
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����״̬�ļ��ж�ȡ����Э��ջ��״̬��Ϣ
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::LoadStatus(void)
{
    return ERR_SUCCESS;
}


/******************************************************
// ������:  CNetApp::SaveStatus
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����״̬�ļ���д������Э��ջ��״̬��Ϣ
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::SaveStatus(void)
{
    return ERR_SUCCESS;
}



/******************************************************
// ������:  CNetApp::SendFrame2Ex
// ����:    Wangfeng
// ʱ��:    01-11-6
// ����:    ���Ӧ������ģ���GSN����Э��֡
// ����:
//       ����1 :MSG_INTRA* pMSG
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-11-6       Wangfeng
******************************************************/
int CNetApp::SendFrame2Ex(MSG_INTRA* pMsg, CProtocolStack* pStack)
{
    if((m_Stack_To_RecvInv.size() < m_pNetWork->GetLicenseCount())
        || (pStack == NULL) //��ʾΪNetApp���ĸ澯��Ϣ������Ҫ����ȥ
        || (m_Stack_To_RecvInv.find((UINT4)pStack) 
            != m_Stack_To_RecvInv.end()))
    {
        MSG_INTRA* p = new(pMsg->Len) MSG_INTRA;
        memcpy(p, pMsg, pMsg->Len + MSG_INTRA_HEADLENGTH);
        int nRet = m_pNetWork->SendMsgtoHost(pMsg);
        if(nRet == ERR_SUCCESS)
            m_pNetWork->TrackProtocol(p);
        else
            m_pNetWork->TrackProtocol(p, ERR_SOCKET_SEND_MSG);
        delete p;
        return nRet;
    }
    else
    {
        //ģ����������Ȩ��
        m_pNetWork->TrackProtocol(pMsg, ERR_LICENSE_EXPIRED);
    }

    return ERR_SUCCESS;
}


//����Զ2002.04.16���
/**********************************************************************
��������    ת�����Ը澯ģ��ĸ澯֡
�������    pMsg�����Ӹ澯ģ�鷢�͹����ĸ澯��Ϣ
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CNetApp::DispatchAlarm(MSG_INTRA* pMsg)
{
	//�����ʵ�ֱ�����������ʵ��
	ASSERT(FALSE);
}
//����Զ2002.04.16��ӽ���


/******************************************************
// ������:  CNetApp::OnReceive
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������������Э��֡
// ����:
//       ����1 :SNetFrame* pFrame
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::OnReceive(SNetFrame* pFrame)
{
    //
}



/******************************************************
// ������:  CNetApp::Timer1Sec
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���¼�������
// ����:
//       ����1 :void
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::Timer1Sec(void)
{
    if (m_bActive)
    {
        //���ø�CProtocolStack�����Timer1Sec����
        MAP_ID_TO_PS::iterator i;
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            (*i).second->Timer1Sec();
        }

        //ɾ���ںܳ�ʱ����(15��)��δ�յ�������Ϣ��CProtocolStack����
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            if((*i).second->GetInertiaPeriod() > 15L * ONE_DAY_IN_SECOND)
            {
                char szMsg[100];
                sprintf(szMsg, 
                        S_NET_REMOVE_GTP_STACK,
                        (*i).second->GetStackId());
                MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
                TRACE(MTS_NETWORK, szMsg);
                CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

                delete (*i).second;
                m_StackMan.erase(i);
                i = m_StackMan.begin();
            }
        }

        //��������Э��ջ��һ���յ�Э��֡��ʱ���
        m_Stack_To_RecvInv.clear();
        time_t tNow;
        time(&tNow);
        int nCount = 0;
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            if(tNow >= i->second->GetLastRecvTime() 
               && tNow - i->second->GetLastRecvTime() < 1800 //��ʮ����
               && nCount < m_pNetWork->GetLicenseCount())
            {
                m_Stack_To_RecvInv[(UINT4)i->second] = tNow - i->second->GetLastRecvTime();
                nCount++;
            }
        }
    }
}


/******************************************************
// ������:  CNetApp::GetNetWork
// ����:    Wangfeng
// ʱ��:    01-10-26
// ����:    ����CNetWork*�����ָ��
// ����:
// ����ֵ: CNetWork* 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-26      Wangfeng
******************************************************/
CNetWork* CNetApp::GetNetWork()
{
    return m_pNetWork;
}

/******************************************************
// ������:  CNetApp::GetReadyDataSize
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������Э��ջ�е�ǰ�ѽ��յ��ҿ����ύ��ǰ��
//          ��ģ�鱣��Ļ������ݵ��ֽ���
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::GetReadyDataSize(void)
{
    int nSizeTotal = 0;
    //��m_StackMan�е�Ԫ�ؽ��б���������ÿ��CProtocolStack
    //�����GetReadyDataSize����
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        nSizeTotal += (*i).second->GetReadyDataSize();
    }
    return nSizeTotal; 
}


/******************************************************
// ������:  CNetApp::GetReadyRecordCount
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������Э��ջ�е�ǰ�ѽ��յ��ҿ����ύ��ǰ��
//          ��ģ�鱣��Ļ������ݵļ�¼��
// ����:
//       ����1 :void
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
int CNetApp::GetReadyRecordCount(void)
{
    int nCountTotal = 0;
    //��m_StackMan�е�Ԫ�ؽ��б���������ÿ��CProtocolStack
    //�����GetReadyRecordCount����
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        nCountTotal += (*i).second->GetReadyRecordCount();
    }
    return nCountTotal;
}



/******************************************************
// ������:  CNetApp::GetReadyData
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ������Э��ջ���ύ��ǰ����ģ�鱣��Ļ�������
//          ����*pDataBuf������
// ����:
//       ����1 :CDataBuffer* const pDataBuf
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::GetReadyData(CDataBuffer* const pDataBuf)
{
    //��m_StackMan�е�Ԫ�ؽ��б���������ÿ��CProtocolStack
    //�����GetReadyData����
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        (*i).second->GetReadyData(pDataBuf);
    }
}



/******************************************************
// ������:  CNetApp::OnResponse
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���ϴ��ṩ��ǰ����ģ�鱣��Ļ������ݽ��к����Ĵ���
// ����:
//       ����1 :bool const bSaved
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CNetApp::OnResponse(BOOL const bSaved)
{
    //��m_StackMan�е�Ԫ�ؽ��б���������ÿ��CProtocolStack
    //�����OnResponse����
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        (*i).second->OnResponse(bSaved);
    }
}

