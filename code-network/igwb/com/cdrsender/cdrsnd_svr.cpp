 /**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : cdrsnd_svr.cpp
 *������Ϣ  �����ļ�����CCDRSndSvr��ʵ��
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/
#include "cdrsnd_svr.h"
#include "../include/frame.h"
#include "../include/msg_def.h"
#include "../frame/resource.h"
#include "gtp_client.h"

//////////////////////////////////////////////////////////////////////////
/**************************************************************************
 *����ԭ��  : CCDRSndSvr::CCDRSndSvr()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
CCDRSndSvr::CCDRSndSvr()
{
    m_pPrdCDRThrd = NULL;
    m_pGtpClient = NULL;
}
/**************************************************************************
 *����ԭ��  : CCDRSndSvr::~CCDRSndSvr()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
CCDRSndSvr::~CCDRSndSvr()
{
}
/**************************************************************************
 *����ԭ��  : int CCDRSndSvr::InitInstance()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
int CCDRSndSvr::InitInstance()
{
    int nRet = CWorkModule::InitInstance();
    if (nRet != ERR_SUCCESS) 
    {
        return nRet;
    }
    
    
    CINIFile IniFile(GetCfgFilePath());
    if (IniFile.Open() != TRUE)
    {
        TRACE(MTS_CDRSENDER,S_PROCESS_OPEN_INIFAIL);
        return ERR_FAIL;
    }
    
    //��ȡ״̬�ļ�·��
    char szFileName[MAX_PATH];
	IniFile.GetString(CFG_SEC_BS, 
                      CFG_BS_KEY_STATUS_FILE_DIR,
                      CFG_MAIN_STATUS_ROOT_DIR_DEFAULT,
                      szFileName,
                      MAX_PATH
                      );
	
    STRNCAT(szFileName, "/bs", MAX_PATH);
    CreateDeepDir(szFileName);

    STRNCAT(szFileName, "/sender.sf", MAX_PATH);

    IniFile.Close();

    if(!m_StatusFile.Open(szFileName))
    {
        return ERR_FAIL;
    }

    m_pGtpClient = new CGtpClient(&m_CDRBuffer,    /* ������ */
                                  &m_StatusFile    /*״̬�ļ�*/
                                 ); 

    if (!m_pGtpClient->GetGtpInfo())
    {
        return ERR_FAIL;
    }

    m_pPrdCDRThrd = new CCDRProducer(&m_CDRBuffer, /* ������ */
                                     &m_StatusFile /*״̬�ļ�*/
                                     );

    if (m_pPrdCDRThrd != NULL)
    {
        if (m_pPrdCDRThrd->open() != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }
    }

    //�������¼���ʱ��
    m_Timer1SecId = SetTimer(1000);

    return ERR_SUCCESS;

}
/**************************************************************************
 *����ԭ��  : void CCDRSndSvr::ExitInstance()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
void CCDRSndSvr::ExitInstance()
{
    m_pPrdCDRThrd->stop();
    delete m_pPrdCDRThrd;
    m_pPrdCDRThrd = NULL;
    
    KillTimer(m_Timer1SecId);
    
    delete m_pGtpClient;
    m_pGtpClient = NULL;

    CWorkModule::ExitInstance();
}
/**************************************************************************
 *����ԭ��  : void CCDRSndSvr::OnTimer(const long nTimerID)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
void CCDRSndSvr::OnTimer(const long nTimerID)
{
    if (nTimerID == m_Timer1SecId)
    {
        Timer1Sec();
    }
}
/**************************************************************************
 *����ԭ��  : void CCDRSndSvr::Timer1Sec()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
void CCDRSndSvr::Timer1Sec()
{
    m_pGtpClient->Timer1Sec();
}
/**************************************************************************
 *����ԭ��  : MODULE_TYPE CCDRSndSvr::GetModuleType()
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
MODULE_TYPE CCDRSndSvr::GetModuleType()
{
    return MT_CDRSENDER;
}
/**************************************************************************
 *����ԭ��  : void CCDRSndSvr::ParseMsg(MSG_INTRA* pMsgParsed)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
void CCDRSndSvr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    switch (pMsgParsed->CmdCode)
    {
    case CMD_BS_DATA:
        m_pGtpClient->OnReceive(pMsgParsed);
        break;
    default:
        break;
        
    }
    CWorkModule::ParseMsg(pMsgParsed);
}
/**************************************************************************
 *����ԭ��  : int CCDRSndSvr::SendMsgtoBS(MSG_INTRA* pMsg)
 *�������  : 
 *�������  : 
 *��������  : 
 *����      : Z30971
 *�޸ļ�¼  : 
***************************************************************************/
int CCDRSndSvr::SendMsgtoBS(MSG_INTRA* pMsg)
{
    return SendMsg(pMsg);
}

