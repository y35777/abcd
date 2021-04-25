 /**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：GTP'计费接口
 *文件名    : cdrsnd_svr.cpp
 *描述信息  ：本文件是类CCDRSndSvr的实现
 *版本说明  ：V200R002B02   
 *版权信息  ：版权所有（C）2004-2005 华为技术有限公司
 *作者      ：Z30971
 *创建时间  ：2004-06-19
 *修改记录  ：
 **************************************************************************/
#include "cdrsnd_svr.h"
#include "../include/frame.h"
#include "../include/msg_def.h"
#include "../frame/resource.h"
#include "gtp_client.h"

//////////////////////////////////////////////////////////////////////////
/**************************************************************************
 *函数原型  : CCDRSndSvr::CCDRSndSvr()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
CCDRSndSvr::CCDRSndSvr()
{
    m_pPrdCDRThrd = NULL;
    m_pGtpClient = NULL;
}
/**************************************************************************
 *函数原型  : CCDRSndSvr::~CCDRSndSvr()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
CCDRSndSvr::~CCDRSndSvr()
{
}
/**************************************************************************
 *函数原型  : int CCDRSndSvr::InitInstance()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
    
    //读取状态文件路径
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

    m_pGtpClient = new CGtpClient(&m_CDRBuffer,    /* 缓存区 */
                                  &m_StatusFile    /*状态文件*/
                                 ); 

    if (!m_pGtpClient->GetGtpInfo())
    {
        return ERR_FAIL;
    }

    m_pPrdCDRThrd = new CCDRProducer(&m_CDRBuffer, /* 缓存区 */
                                     &m_StatusFile /*状态文件*/
                                     );

    if (m_pPrdCDRThrd != NULL)
    {
        if (m_pPrdCDRThrd->open() != ERR_SUCCESS)
        {
            return ERR_FAIL;
        }
    }

    //设置秒事件定时器
    m_Timer1SecId = SetTimer(1000);

    return ERR_SUCCESS;

}
/**************************************************************************
 *函数原型  : void CCDRSndSvr::ExitInstance()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : void CCDRSndSvr::OnTimer(const long nTimerID)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
void CCDRSndSvr::OnTimer(const long nTimerID)
{
    if (nTimerID == m_Timer1SecId)
    {
        Timer1Sec();
    }
}
/**************************************************************************
 *函数原型  : void CCDRSndSvr::Timer1Sec()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
void CCDRSndSvr::Timer1Sec()
{
    m_pGtpClient->Timer1Sec();
}
/**************************************************************************
 *函数原型  : MODULE_TYPE CCDRSndSvr::GetModuleType()
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
MODULE_TYPE CCDRSndSvr::GetModuleType()
{
    return MT_CDRSENDER;
}
/**************************************************************************
 *函数原型  : void CCDRSndSvr::ParseMsg(MSG_INTRA* pMsgParsed)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
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
 *函数原型  : int CCDRSndSvr::SendMsgtoBS(MSG_INTRA* pMsg)
 *输入参数  : 
 *输出参数  : 
 *功能描述  : 
 *作者      : Z30971
 *修改记录  : 
***************************************************************************/
int CCDRSndSvr::SendMsgtoBS(MSG_INTRA* pMsg)
{
    return SendMsg(pMsg);
}

