#include "net_app.h"
#include "net_status.h"
#include "wlan_as_app.h"
#include "protocol_stack.h"
#include "wlan_as_stack.h"
#include "network.h"
#include "resource.h"
#include "../include/toolbox.h"


CWlanAsApp::CWlanAsApp(CNetWork* const pNetWork)
                      :CNetApp(pNetWork)
{
    m_nStackStatusInfoType = STACK_WLAN_AS;
}

CWlanAsApp::~CWlanAsApp()
{
    //
}

int CWlanAsApp::Init(void)
{
    if(m_StackMan.size() == 0)
    {
        //创建一个CWlanAsStack协议栈对象
        CWlanAsStack* pStack = new CWlanAsStack(this, 1);
        if(!pStack)
        {
            return ERR_FAIL;
        }

        m_StackMan[1] = pStack;
    }
    return ERR_SUCCESS;
}

int CWlanAsApp::LoadStatus(void)
{
    return ERR_SUCCESS;
}

int CWlanAsApp::SaveStatus(void)
{
    return ERR_SUCCESS;
}

void CWlanAsApp::Timer1Sec(void)
{
    if (m_bActive)
    {
        //调用各CProtocolStack对象的Timer1Sec函数
        MAP_ID_TO_PS::iterator i;
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            (*i).second->Timer1Sec();
        }
    }
}

