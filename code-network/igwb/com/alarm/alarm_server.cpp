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
//功能描述  重载CWorkModule类的
//      <<protected virtual>>int InitInstance(void)成员函数
//      完成类的初始化工作
//输入参数  无
//输出参数  无
//返回值    成功返回0 ,失败返回负值
//////////////////////////////////////////////////////////////////////
int CAlarmServer::InitInstance()
{
    //调用父类的初始化函数
    int nRet = CWorkModule::InitInstance();
    if(nRet != ERR_SUCCESS)
    {
        TRACE(THISMODULE,S_ALARM_BASE_INIT_FAIL);
        return nRet;
    }

    //创建读取INI文件的对象
    CINIFile iniFile(GetCfgFilePath());
        nRet = iniFile.Open();
    if(nRet != TRUE)
    {
        TRACE(THISMODULE,S_ALARM_INI_OPEN_FAIL);
        return ERR_FAIL;
    }

    //ini文件中AccessPointX的名字
    char szApName[15];

    //读入接入点个数,默认是1
    int APCount  = iniFile.GetInt(CFG_SEC_COMMON,
                                  CFG_COMMON_KEY_APCOUNT,
                                  ALM_DEFAPCOUNT);
    int ConnectedMode = iniFile.GetInt(CFG_SEC_ALARM, CFG_ALARM_BOX_USING_MODE, BOX_USING_MODE);    
    //读入连接告警箱端口的名字
    char szPortName[20];
    iniFile.GetString(CFG_SEC_ALARM, CFG_ALARM_BOX_PORT_NAME, S_ALARM_BOX_PORT_NAME_DEFAULT,
        szPortName, sizeof(szPortName));
    szPortName[sizeof(szPortName) - 1] = '\0';        

    //读入主备信息,默认是主机
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
        szMMLAlarmShieldTmp,128); //此时已经去掉空格了
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
    else//非h结尾表示为以前的非16进制版本
    {
        memcpy(szMMLAlarmShield, szMMLAlarmShieldTmp, strlen(szMMLAlarmShieldTmp));
    }       

    //创建CMMLHandler对象,并调用其Init(void)函数,如果初始化失败则返回
    //否则将该对象加入m_AlarmHandlers;
    CAlarmHandler *pHandler = new CMMLAlarmHandler(BillsvrNo, this,szMMLAlarmShield,szTmpPath);    

    //配置文件的路径名写死2002-07-30
    sprintf(szTmpPath,"%s/%s", GetAppPath(), S_ALARM_CONFIG_PATH);

    nRet = pHandler->Init(szTmpPath, ConnectedMode, szPortName);     
    if(ERR_SUCCESS == nRet)
    {
        m_AlarmHandlers.push_back(pHandler);

        //如果MML告警对象创建成功,则通过CIniFile对象读取的接入点个数,
        //开始一个循环,依次创建CAlarmHandler对象
        for(int i = 1 ; i<= APCount ; i++)
        {
            //形成了AccessPointX名字
            sprintf(szApName,"%s%d", CFG_SEC_AP, i);

            //读入接入点类型和是否发送二进制告警
            int ProductType = iniFile.GetInt(szApName,
                                             CFG_AP_KEY_APTYPE,
                                             ALM_DEFAPTYPE);

            int nBinAlarmSend = iniFile.GetInt(szApName,
                                               CFG_ALARM_BINALMSEND,
                                               ALM_DEFBINSEND);

            //判断是否要添加二进制告警处理对象
            if((ProductType == AP_128
                            || ProductType == AP_GSM
                            || ProductType == AP_STP
                || ProductType == AP_SOFTX) //zhangyuxin modi fy
               && (nBinAlarmSend != ALM_DEFBINSEND))
            {

                //如果发送二进制告警,读出当前每个接入点对应的接入点进程ID,告警ID基数,
                //告警屏蔽码,创建一个CBinAlarmHandler对象
                BYTE  yAccessPointID = PT_AP_BASE + i -1;

                UINT2 uBaseAlarmID = iniFile.GetInt(szApName,
                                                    CFG_ALARM_BASEID,
                                                    ALM_BASEID);

                char szAlarmShieldTmp[128];
                char szAlarmShield[128];
                memcpy(szAlarmShield, ALM_DEFMMLALARMSHIELD, 128);
                iniFile.GetString(CFG_SEC_ALARM,CFG_MML_ALARM_ALMSHIELD,ALM_DEFMMLALARMSHIELD,
                    szAlarmShieldTmp,128); //此时已经去掉空格了
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
                else//非h结尾表示为以前的非16进制版本
                {
                    memcpy(szAlarmShield, szAlarmShieldTmp, strlen(szAlarmShieldTmp));
                } 
                
                //根据以上参数初始化二进制告警处理对象
                //张玉新联调2002-07-19
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
                
                //调用CBinAlarmHandler类的Init()函数,函数成功,
                //则将CBinAlarmHandler指针对象加入m_AlarmHandlers队列,
                //否则CAlarmServer初始化失败退出,并清空队列
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
    //如果MML告警处理对象失败的话,打印消息并退出
    else
    {
        TRACE(THISMODULE,S_ALARM_MML_INIT_FAIL);
        delete pHandler;
        return ERR_FAIL;
    }

    //获取硬件检测参数
    int nPort = iniFile.GetInt(CFG_SEC_ALARM, CFG_ALARM_HD_MONITOR_PORT,
                               CFG_ALARM_HD_MONITOR_PORT_DEFAULT);

    SNPRINTF(szTmpPath, sizeof(szTmpPath),
            "%s/config/alarm/", GetAppPath());
    szTmpPath[sizeof(szTmpPath) - 1] = '\0';


    //初始化硬件告警对象
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

    //函数注册一个1秒钟的定时器
    m_nTimer1SecID = CWorkModule::SetTimer(ONE_SECOND);

    //返回成功(ERR_SUCCESS)
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
    UINT2 nLen = strlen(pHexString) - 1;//去掉结尾处的h
    int pointer = 1;//告警的mask是从数组的第一位开始
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
    //如果字符串长度等于零
    int nLen = strlen(szData);
    if(nLen == 0)
    {
        return FALSE;
    }    
    
    //去掉前面的空格
    int i = 0;
    while(szData[i] == ' ')
    {
        i++;
    }
    
    //去掉后面的空格
    while(nLen > 0 && szData[nLen - 1] == ' ')
    {
        nLen--;
    }
    
    //判断是否还有剩余的字符
    if(nLen - i - 1 <= 0)
    {
        return FALSE;
    }
    
    if(szData[nLen - i - 1] != 'H' && szData[nLen - i - 1] != 'h')
    {
        return FALSE;
    }
    
    nLen --;
    //判断第一个字符是否是'+'或'-'符号
    if(((szData[i] == '+') || (szData[i] == '-')) && (nLen > 1))
    {
        i++;
    }
    
    //判断字符串中的每一个字符
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
    
    //如果字符串所有字符都是数字
    return TRUE;
}

//////////////////////////////////////////////////////////////
//功能描述  该函数完成m_ActiveAlarm队列成员指针的释放,清除操作
//          最后调用CWorkModule::ExistInstance(void)
//////////////////////////////////////////////////////////////
void CAlarmServer::ExitInstance()
{
    //m_pAlarmHandlers队列成员指针的释放、清除操作，

    ALARM_HANDLER_LIST::iterator i;
    for(i = m_AlarmHandlers.begin(); i != m_AlarmHandlers.end(); i++)
    {
        delete (*i);
    }
    m_AlarmHandlers.clear();

    //清除时钟
    CWorkModule::KillTimer(m_nTimer1SecID);

    //退出硬件检测器
    if(m_pHDMonitor != NULL)
    {
        m_pHDMonitor->Exit();
        m_pHDMonitor = NULL;
    }

    //父类的退出清理函数接口
    CWorkModule::ExitInstance();
}

////////////////////////////////////////////////////////////////////////
//功能描述  该函数重载CWorkModule的ParseMsg(MSG_INTRA*  pFrame) 成员函数
//      该函数处理模块间的消息
//输入参数  MSG_INTRA *pFrame——内部消息帧
////////////////////////////////////////////////////////////////////////
void CAlarmServer::ParseMsg(MSG_INTRA *pFrame)
{
    //修改 by ldf 增加对pFrame的判断 2002.1.8
	//by ldf 2004-06-03 对应问题单SWPD05582
    if ( pFrame->CmdCode == CMD_INNER_ALARM
         || pFrame->CmdCode == CMD_MML
         || pFrame->CmdCode == CMD_ALARM_RESPONSE
         || pFrame->CmdCode == 0xAA
		 || pFrame->CmdCode == CMD_MODULE_ACTIVE) //ALARM_CMD_BAU_TO_CONVERT
                                     //zhangyuxin modify BB is
                                     //not valid any more

    {
        //定义遍历的叠代器
        ALARM_HANDLER_LIST::iterator i;

        //遍历m_AlarmHandlers的成员,并调用其DispatchFrame依次处理该消息
        for(i = m_AlarmHandlers.begin(); i != m_AlarmHandlers.end(); i++)
        {
            ((CAlarmHandler*)(*i))->DispatchFrame(pFrame);
        }
    }
}
////////////////////////////////////////////////////////////////////////
//功能描述  该函数调用m_AlarmHandlers各对象的Timer1Sec接口
//输入参数  定时器ID
///////////////////////////////////////////////////////////////////////
void CAlarmServer::OnTimer(const long lTimerID)
{
    //如果为秒事件处理时钟,则依次调用各CAlarmHandler对象的Timer1Sec接口
    if(lTimerID == m_nTimer1SecID)
    {
        ALARM_HANDLER_LIST::iterator iter;
        for(iter = m_AlarmHandlers.begin(); iter != m_AlarmHandlers.end(); iter++)
        {
            (*iter)->Timer1Sec();
        }
    }

    //否则调用基类OnTimer接口
    CWorkModule::OnTimer(lTimerID);
}
////////////////////////////////////////////////////////////////////////
//功能描述  该函数返回告警模块的模块类型MT_ALARM
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






















