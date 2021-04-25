// AlarmServer.cpp: implementation of the CAlarmServer class.
//
//////////////////////////////////////////////////////////////////////

/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "alarm_server.h"
#include "resource.h"
#include "alarm_handler.h"
#include "mml_alarm_handler.h"
#include "bin_alarm_handler.h"
#include "../toolbase/ini_file.h"
#include "../utility/config.h"
#include "hd_monitor/v2_hd_monitor_proxy.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlarmServer::CAlarmServer()
{
    m_pHDMonitor = NULL;
}

CAlarmServer::~CAlarmServer()
{
    ALARM_HANDLER_LIST::iterator i;
    CAlarmHandler *pHandler;

    for(i = m_AlarmHandlers.begin(); i != m_AlarmHandlers.end(); i++)
    {
        pHandler = (*i);
        delete pHandler;
        pHandler = NULL;
    }

    m_AlarmHandlers.clear();

}
/////////////////////////////////////////////////////////////////////
//��������  ����CWorkModule���
//      <<protected virtual>>int InitInstance(void)��Ա����
//      �����ĳ�ʼ������
//�������  ��
//�������  ��
//����ֵ    �ɹ�����0 ,ʧ�ܷ��ظ�ֵ
//////////////////////////////////////////////////////////////////////
int CAlarmServer::InitInstance()
{
    //���ø���ĳ�ʼ������
    int nRet = CWorkModule::InitInstance();
    if(nRet != ERR_SUCCESS)
    {
        TRACE(THISMODULE,S_ALARM_BASE_INIT_FAIL);
        return nRet;
    }

    //������ȡINI�ļ��Ķ���
    CINIFile iniFile(GetCfgFilePath());
        nRet = iniFile.Open();
    if(nRet != TRUE)
    {
        TRACE(THISMODULE,S_ALARM_INI_OPEN_FAIL);
        return ERR_FAIL;
    }

    //ini�ļ���AccessPointX������
    char szApName[15];

    //�����������,Ĭ����1
    int APCount  = iniFile.GetInt(CFG_SEC_COMMON,
                                  CFG_COMMON_KEY_APCOUNT,
                                  ALM_DEFAPCOUNT);
    int ConnectedMode = iniFile.GetInt(CFG_SEC_ALARM, CFG_ALARM_BOX_USING_MODE, BOX_USING_MODE);    
    //�������Ӹ澯��˿ڵ�����
    char szPortName[20];
    iniFile.GetString(CFG_SEC_ALARM, CFG_ALARM_BOX_PORT_NAME, S_ALARM_BOX_PORT_NAME_DEFAULT,
        szPortName, sizeof(szPortName));
    szPortName[sizeof(szPortName) - 1] = '\0';        

    //����������Ϣ,Ĭ��������
    int BillsvrNo = iniFile.GetInt(CFG_SEC_COMMON,
                                   CFG_SERVERNO,
                                   ALM_DEFSERVER);
    char szTmpPath[MAX_PATH];
    iniFile.GetString(CFG_SEC_ALARM, CFG_ALARM_HISTORY, S_ALARM_ALMHISTORY,
                      szTmpPath, MAX_PATH);

    char szMMLAlarmShieldTmp[128];
    char szMMLAlarmShield[128];
    memcpy(szMMLAlarmShield, ALM_DEFMMLALARMSHIELD, 128);
    iniFile.GetString(CFG_SEC_ALARM,CFG_MML_ALARM_ALMSHIELD,ALM_DEFMMLALARMSHIELD,
        szMMLAlarmShieldTmp,128); //��ʱ�Ѿ�ȥ���ո���
    char cTmp = szMMLAlarmShieldTmp[strlen(szMMLAlarmShieldTmp)-1];
    if(cTmp =='H' ||cTmp == 'h')
    {
        if(strlen(szMMLAlarmShieldTmp) == 127)                 
        {
            memcpy(szMMLAlarmShield, szMMLAlarmShieldTmp, strlen(szMMLAlarmShieldTmp));
        }
        else
        {
            HexStringToBinString(szMMLAlarmShieldTmp, szMMLAlarmShield);
        }
    }
    else//��h��β��ʾΪ��ǰ�ķ�16���ư汾
    {
        memcpy(szMMLAlarmShield, szMMLAlarmShieldTmp, strlen(szMMLAlarmShieldTmp));
    }       

    //����CMMLHandler����,��������Init(void)����,�����ʼ��ʧ���򷵻�
    //���򽫸ö������m_AlarmHandlers;
    CAlarmHandler *pHandler = new CMMLAlarmHandler(BillsvrNo, this,szMMLAlarmShield,szTmpPath);    

    //�����ļ���·����д��2002-07-30
    sprintf(szTmpPath,"%s/%s", GetAppPath(), S_ALARM_CONFIG_PATH);

    nRet = pHandler->Init(szTmpPath, ConnectedMode, szPortName);     
    if(ERR_SUCCESS == nRet)
    {
        m_AlarmHandlers.push_back(pHandler);

        //���MML�澯���󴴽��ɹ�,��ͨ��CIniFile�����ȡ�Ľ�������,
        //��ʼһ��ѭ��,���δ���CAlarmHandler����
        for(int i = 1 ; i<= APCount ; i++)
        {
            //�γ���AccessPointX����
            sprintf(szApName,"%s%d", CFG_SEC_AP, i);

            //�����������ͺ��Ƿ��Ͷ����Ƹ澯
            int ProductType = iniFile.GetInt(szApName,
                                             CFG_AP_KEY_APTYPE,
                                             ALM_DEFAPTYPE);

            int nBinAlarmSend = iniFile.GetInt(szApName,
                                               CFG_ALARM_BINALMSEND,
                                               ALM_DEFBINSEND);

            //�ж��Ƿ�Ҫ��Ӷ����Ƹ澯�������
            if((ProductType == AP_128
                            || ProductType == AP_GSM
                            || ProductType == AP_STP
                || ProductType == AP_SOFTX) //zhangyuxin modi fy
               && (nBinAlarmSend != ALM_DEFBINSEND))
            {

                //������Ͷ����Ƹ澯,������ǰÿ��������Ӧ�Ľ�������ID,�澯ID����,
                //�澯������,����һ��CBinAlarmHandler����
                BYTE  yAccessPointID = PT_AP_BASE + i -1;

                UINT2 uBaseAlarmID = iniFile.GetInt(szApName,
                                                    CFG_ALARM_BASEID,
                                                    ALM_BASEID);

                char szAlarmShieldTmp[128];
                char szAlarmShield[128];
                memcpy(szAlarmShield, ALM_DEFMMLALARMSHIELD, 128);
                iniFile.GetString(CFG_SEC_ALARM,CFG_MML_ALARM_ALMSHIELD,ALM_DEFMMLALARMSHIELD,
                    szAlarmShieldTmp,128); //��ʱ�Ѿ�ȥ���ո���
                char cTmp = szAlarmShieldTmp[strlen(szAlarmShieldTmp)-1];
                if(cTmp =='H' ||cTmp == 'h')
                {
                    if(strlen(szAlarmShieldTmp) == 127)                 
                    {
                        memcpy(szAlarmShield, szAlarmShieldTmp, strlen(szAlarmShieldTmp));
                    }
                    else
                    {
                        HexStringToBinString(szAlarmShieldTmp, szAlarmShield);
                    }
                }
                else//��h��β��ʾΪ��ǰ�ķ�16���ư汾
                {
                    memcpy(szAlarmShield, szAlarmShieldTmp, strlen(szAlarmShieldTmp));
                } 
                
                //�������ϲ�����ʼ�������Ƹ澯�������
                //����������2002-07-19
                if(ProductType == AP_SOFTX)
                {
                    pHandler = new CSoftXBinAlarmHandler(yAccessPointID,
                        uBaseAlarmID,
                        szAlarmShield,
                        ProductType,
                        BillsvrNo,
                        this,
                        szApName);
                }
                else
                {
                    pHandler = new CMSCBinAlarmHandler(yAccessPointID,
                        uBaseAlarmID,
                        szAlarmShield,
                        ProductType,
                        BillsvrNo,
                        this,
                        szApName);
                }
                
                //����CBinAlarmHandler���Init()����,�����ɹ�,
                //��CBinAlarmHandlerָ��������m_AlarmHandlers����,
                //����CAlarmServer��ʼ��ʧ���˳�,����ն���
                nRet = pHandler->Init(szTmpPath);
                if(ERR_SUCCESS == nRet)
                {
                    m_AlarmHandlers.push_back(pHandler);
                }
                else
                {   //self change 2 pop_front() return void!
                    ALARM_HANDLER_LIST::iterator iter = m_AlarmHandlers.begin();

                    while(iter != m_AlarmHandlers.end())
                    {
                        delete (*iter);
                        iter++;
                    }
                    m_AlarmHandlers.clear();

                    TRACE(THISMODULE,S_ALARM_BIN_INIT_FAIL,szApName);
                    return ERR_FAIL;
                }
            }
        }//for is over self change 2 {} not Match
    }
    //���MML�澯�������ʧ�ܵĻ�,��ӡ��Ϣ���˳�
    else
    {
        TRACE(THISMODULE,S_ALARM_MML_INIT_FAIL);
        delete pHandler;
        return ERR_FAIL;
    }

    //��ȡӲ��������
    int nPort = iniFile.GetInt(CFG_SEC_ALARM, CFG_ALARM_HD_MONITOR_PORT,
                               CFG_ALARM_HD_MONITOR_PORT_DEFAULT);

    SNPRINTF(szTmpPath, sizeof(szTmpPath),
            "%s/config/alarm/", GetAppPath());
    szTmpPath[sizeof(szTmpPath) - 1] = '\0';


    //��ʼ��Ӳ���澯����
    m_pHDMonitor = CHDMonitorProxy::Instance((CV2HDMonitorProxy *)0);
    if(m_pHDMonitor != NULL)
    {
        if(ERR_SUCCESS != m_pHDMonitor->Init(szTmpPath, nPort))
        {
            TRACE(THISMODULE,
                  S_ALARM_HDMONITOR_INIT_FAIL,
                  m_pHDMonitor->GetLastError());

            m_pHDMonitor->Exit();
            m_pHDMonitor = NULL;
        }
    }
    else
    {
        TRACE(THISMODULE, S_ALARM_HDMONITOR_CRT_FAIL);
    }

    //����ע��һ��1���ӵĶ�ʱ��
    m_nTimer1SecID = CWorkModule::SetTimer(ONE_SECOND);

    //���سɹ�(ERR_SUCCESS)
    return ERR_SUCCESS;
}
void CAlarmServer::HexStringToBinString(char* pHexString, char* pBinString)
{
    if(!IsHex(pHexString))
    {        
        TRACE(THISMODULE, S_ALARM_SET_ALARMSHEILD_FAIL);
        memcpy(pBinString, ALM_DEFALARMSHIELD, 128);
        return ;
    }
    UINT2 szMask[4]={8, 4, 2, 1};
    UINT2 nLen = strlen(pHexString) - 1;//ȥ����β����h
    int pointer = 1;//�澯��mask�Ǵ�����ĵ�һλ��ʼ
    for (int i =0; i< nLen; i++)
    {        
        UINT2 nTmp = 0;
        if('0' <= pHexString[i] && pHexString[i] <= '9')
        {
            nTmp = pHexString[i] - '0';
        }
        if('A' <= pHexString[i] && pHexString[i] <= 'F')
        {
            nTmp = pHexString[i] - 'A' + 10;
        }
        if('a' <= pHexString[i] && pHexString[i] <= 'f')
        {
            nTmp = pHexString[i] - 'a' + 10;
        }        
        for(int j=0; j<4; j++)
        {
            UINT2 nResult = nTmp&szMask[j];
            if(nResult == 0)
            {
                *(pBinString + pointer) = '0';
            }
            else
            {
                *(pBinString + pointer) = '1';
            }
            if(pointer+2<128)
            {
                pointer=pointer + 2;
            }            
        }        
    }
}
BOOL CAlarmServer::IsHex(const char* szData)
{
    //����ַ������ȵ�����
    int nLen = strlen(szData);
    if(nLen == 0)
    {
        return FALSE;
    }    
    
    //ȥ��ǰ��Ŀո�
    int i = 0;
    while(szData[i] == ' ')
    {
        i++;
    }
    
    //ȥ������Ŀո�
    while(nLen > 0 && szData[nLen - 1] == ' ')
    {
        nLen--;
    }
    
    //�ж��Ƿ���ʣ����ַ�
    if(nLen - i - 1 <= 0)
    {
        return FALSE;
    }
    
    if(szData[nLen - i - 1] != 'H' && szData[nLen - i - 1] != 'h')
    {
        return FALSE;
    }
    
    nLen --;
    //�жϵ�һ���ַ��Ƿ���'+'��'-'����
    if(((szData[i] == '+') || (szData[i] == '-')) && (nLen > 1))
    {
        i++;
    }
    
    //�ж��ַ����е�ÿһ���ַ�
    for(; i < nLen; i++)
    {         
        if((szData[i] >= '0') && (szData[i] <= '9'))
        {
            continue;
        }
        
        if((szData[i] >= 'a') && (szData[i] <= 'f'))
        {
            continue;
        }
        
        if((szData[i] >= 'A') && (szData[i] <= 'F'))
        {
            continue;
        }
        return FALSE;
        
    }
    
    //����ַ��������ַ���������
    return TRUE;
}

//////////////////////////////////////////////////////////////
//��������  �ú������m_ActiveAlarm���г�Աָ����ͷ�,�������
//          ������CWorkModule::ExistInstance(void)
//////////////////////////////////////////////////////////////
void CAlarmServer::ExitInstance()
{
    //m_pAlarmHandlers���г�Աָ����ͷš����������

    ALARM_HANDLER_LIST::iterator i;
    for(i = m_AlarmHandlers.begin(); i != m_AlarmHandlers.end(); i++)
    {
        delete (*i);
    }
    m_AlarmHandlers.clear();

    //���ʱ��
    CWorkModule::KillTimer(m_nTimer1SecID);

    //�˳�Ӳ�������
    if(m_pHDMonitor != NULL)
    {
        m_pHDMonitor->Exit();
        m_pHDMonitor = NULL;
    }

    //������˳��������ӿ�
    CWorkModule::ExitInstance();
}

////////////////////////////////////////////////////////////////////////
//��������  �ú�������CWorkModule��ParseMsg(MSG_INTRA*  pFrame) ��Ա����
//      �ú�������ģ������Ϣ
//�������  MSG_INTRA *pFrame�����ڲ���Ϣ֡
////////////////////////////////////////////////////////////////////////
void CAlarmServer::ParseMsg(MSG_INTRA *pFrame)
{
    //�޸� by ldf ���Ӷ�pFrame���ж� 2002.1.8
	//by ldf 2004-06-03 ��Ӧ���ⵥSWPD05582
    if ( pFrame->CmdCode == CMD_INNER_ALARM
         || pFrame->CmdCode == CMD_MML
         || pFrame->CmdCode == CMD_ALARM_RESPONSE
         || pFrame->CmdCode == 0xAA
		 || pFrame->CmdCode == CMD_MODULE_ACTIVE) //ALARM_CMD_BAU_TO_CONVERT
                                     //zhangyuxin modify BB is
                                     //not valid any more

    {
        //��������ĵ�����
        ALARM_HANDLER_LIST::iterator i;

        //����m_AlarmHandlers�ĳ�Ա,��������DispatchFrame���δ������Ϣ
        for(i = m_AlarmHandlers.begin(); i != m_AlarmHandlers.end(); i++)
        {
            ((CAlarmHandler*)(*i))->DispatchFrame(pFrame);
        }
    }
}
////////////////////////////////////////////////////////////////////////
//��������  �ú�������m_AlarmHandlers�������Timer1Sec�ӿ�
//�������  ��ʱ��ID
///////////////////////////////////////////////////////////////////////
void CAlarmServer::OnTimer(const long lTimerID)
{
    //���Ϊ���¼�����ʱ��,�����ε��ø�CAlarmHandler�����Timer1Sec�ӿ�
    if(lTimerID == m_nTimer1SecID)
    {
        ALARM_HANDLER_LIST::iterator iter;
        for(iter = m_AlarmHandlers.begin(); iter != m_AlarmHandlers.end(); iter++)
        {
            (*iter)->Timer1Sec();
        }
    }

    //������û���OnTimer�ӿ�
    CWorkModule::OnTimer(lTimerID);
}
////////////////////////////////////////////////////////////////////////
//��������  �ú������ظ澯ģ���ģ������MT_ALARM
////////////////////////////////////////////////////////////////////////
MODULE_TYPE CAlarmServer::GetModuleType()
{
    return MT_ALARM;
}

void CAlarmServer::AddBinAlarm(SInnerAlarm InnerAlarm)
{
    ALARM_HANDLER_LIST::iterator iter = m_AlarmHandlers.begin();  
    iter++;
    
    SActiveAlarm *pActiveAlarm   = NULL;
    
    while(iter != m_AlarmHandlers.end())
    {
        CActiveAlarmList* ActiveAlarmListTmp;
        ActiveAlarmListTmp = (*iter)->GetActiveAlarmList();
        ActiveAlarmListTmp->AddAlarm(&InnerAlarm, &pActiveAlarm, RESET_RECOVER);  
        iter++;
    }
}






















