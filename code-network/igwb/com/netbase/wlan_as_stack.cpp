#include "../include/frame.h"
#include "net_status.h"
#include "protocol_stack.h"
#include "data_buffer.h"
#include "net_status.h"
#include "wlan_as_stack.h"
#include "resource.h"

extern IWlanAsOci* CreateWlanAsOci(int nRecordSize,
                                   int nLowAtSize,
                                   int nMaxPackageRecord);

CWlanAsStack::CWlanAsStack(CNetApp* const pNetApp, 
                           UINT4    const uStackId)
                          :CProtocolStack(pNetApp, uStackId)
{
    //读取本进程对应的相关配置参数
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);
    int nBillRecordSize = IniFile.GetInt(strSec,
                                         CFG_AP_KEY_BILL_REC_SIZE, 
                                         CFG_AP_KEY_BILL_REC_SIZE_DEFAULT);
    //获得低潮包大小
    int uLowAtSize = IniFile.GetInt(strSec,
                                    CFG_AP_KEY_LOW_AT_SIZE,
                                    CFG_AP_KEY_LOW_AT_SIZE_WLAN_DEFAULT);
    int nMaxPackageRecord = IniFile.GetInt(strSec,
                                           CFG_AP_KEY_MAX_PACKAGE_SIZE,
                                           CFG_AP_KEY_MAX_PACKAGE_SIZE_WLAN_DEFAULT);
    m_pStatusInfo = NULL;

    char szFmtDll[MAX_PATH];
    IniFile.GetString(strSec,
                      CFG_AP_KEY_WLAN_AS_DLL,
                      CFG_AP_KEY_WLAN_AS_DLL_DEFAULT,
                      szFmtDll,
                      sizeof(szFmtDll));
    if(szFmtDll[0] != '\0')
    {
        m_pWlanAsOci  = m_WlanUtilEncap.CreateWlanAsOci(szFmtDll);
        if(NULL != m_pWlanAsOci)
        {
            int nRet = m_pWlanAsOci->Init(nBillRecordSize,
                                          uLowAtSize,
                                          nMaxPackageRecord);
            if(0 != nRet)
            {
                delete m_pWlanAsOci;
                m_pWlanAsOci = NULL;
            }
        }
    }
}

CWlanAsStack::~CWlanAsStack()
{
    if(NULL != m_pWlanAsOci)
    {
        delete m_pWlanAsOci;
        m_pWlanAsOci = NULL;
    }
}

int CWlanAsStack::LoadStatus(void)
{
    return ERR_SUCCESS;
}

int CWlanAsStack::SaveStatus(void)
{
    return ERR_SUCCESS;
}

//返回本对象中当前已接收的且可以提交给前存盘模块
//保存的话单数据的字节数
int CWlanAsStack::GetReadyDataSize(void)
{
    if(NULL != m_pWlanAsOci)
    {
        return m_pWlanAsOci->GetReadyDataSize();
    }

    return 0;
}

//返回本对象中当前已接收的且可以提交给前存盘模块
//保存的话单数据的记录数
int CWlanAsStack::GetReadyRecordCount(void)
{
    if(NULL != m_pWlanAsOci)
    {
        return m_pWlanAsOci->GetReadyRecordCount();
    }

    return 0;
}


//将本协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
void CWlanAsStack::GetReadyData(CDataBuffer* const pDataBuf)
{
    char* pTempBuf = NULL;
    int   nLen     = 0;

    if(NULL != m_pWlanAsOci)
    {
        int nRet = m_pWlanAsOci->GetReadyData(pTempBuf, nLen);
        if(0 == nRet && pTempBuf != NULL && nLen > 0)
        {
            pDataBuf->Put(pTempBuf, nLen);
        }
    }
}

void CWlanAsStack::OnResponse(BOOL const bSaved)
{
    if(NULL != m_pWlanAsOci)
    {
        m_pWlanAsOci->OnResponse(bSaved);
    }
}


//秒事件处理函数
void CWlanAsStack::Timer1Sec(void)
{
    if(NULL != m_pWlanAsOci)
    {
        m_pWlanAsOci->Timer1Sec();
    }
}

//向对应的主机模块或GSN发送协议帧
int CWlanAsStack::SendFrame2Ex(void* pFrame)
{
    return 0;
}

