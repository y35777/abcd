#include "net_app.h"
#include "net_status.h"
#include "scan_app.h"
#include "protocol_stack.h"
#include "scan_stack.h"
#include "network.h"
#include "resource.h"
#include "../include/toolbox.h"


CScanApp::CScanApp(CNetWork* const pNetWork)
                  :CNetApp(pNetWork)
{
    m_nStackStatusInfoType = STACK_SCAN;

    //�������ɨ���Ŀ¼
    m_szPathName[0] = '\0';

    //ɨ��������λ����
    m_nScanInv      = 100;

    //ɨ����������
    m_nScanCounter  = -1;
}

CScanApp::~CScanApp()
{
    //
}

int CScanApp::Init(void)
{
    CINIFile IniFile((char *)GetCfgFilePath());
    IniFile.Open();

    char strSec[20];
    sprintf(strSec,
            "%s%d",
            CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);
    IniFile.GetString(strSec,
                      CFG_SCAN_STACK_ROOT_DIR,
                      CFG_SCAN_STACK_ROOT_DIR_DEFAULT,
                      (char*)m_szPathName,
                      sizeof(m_szPathName));
    m_szPathName[sizeof(m_szPathName) - 1] = '\0';

    if(m_szPathName[0] == '\0')
    {
        return ERR_FAIL;
    }

    CreateDeepDir(m_szPathName);

    return ERR_SUCCESS;
}

int CScanApp::LoadStatus(void)
{
    //����CNetStatusInfo::EnumStackId�������������Э��ջ�ı��

    //Ϊÿ����Ŵ���һ��CGtpStack����
    //��ӳ���ϵ<���, ��Ӧ��CGtpStack�����ָ��>���뵽m_StackMan�У�
    //���ε���ÿ��CGtpStack����LoadStatus����
    int nNum = 0;
    UINT4 uStackId;
    while((uStackId = CNetStatusInfo::EnumStackId(nNum++, STACK_SCAN)) 
          != INVALID_STACK_ID)
    {
        if(m_StackMan.find(uStackId) == m_StackMan.end())
        {
            //����һ��ScanЭ��ջ����
            CScanStack* pScanStack = new CScanStack(NULL, this, uStackId);
            if(!pScanStack) return ERR_FAIL;

            m_StackMan[uStackId] = pScanStack;

            //װ���Э��ջ��״̬�ļ�
            if(pScanStack->LoadStatus() != ERR_SUCCESS)
            {
                TRACE(MTS_NETWORK, S_NET_LOAD_GTP_STACK, uStackId);
                return ERR_FAIL;
            }
        }
        else
        //˵��״̬�ļ��д�������ID��ͬ��Э��ջ�����Ǳ�̴���
        {
            ASSERT(0);
        }
    }

    return ERR_SUCCESS;
}

int CScanApp::SaveStatus(void)
{
    int nRet = ERR_SUCCESS;
    
    //���ε���ÿ��CScanStack�����SaveStatus������
    MAP_ID_TO_PS::iterator i;
    for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
    {
        nRet = (*i).second->SaveStatus();
        if (nRet != ERR_SUCCESS)
        {
            TRACE(MTS_NETWORK, S_NET_SAVE_GTP_STATUS);
            return nRet;
        }
    }

    return nRet;
}

void CScanApp::Timer1Sec(void)
{
    if (m_bActive)
    {
        if((m_nScanCounter == -1) || (m_nScanCounter++ >= m_nScanInv))
        {
            ScanStack(m_szPathName);
            m_nScanCounter = 0;
        }

        //���ø�CProtocolStack�����Timer1Sec����
        MAP_ID_TO_PS::iterator i;
        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            (*i).second->Timer1Sec();
        }

        for (i = m_StackMan.begin(); i != m_StackMan.end(); i++)
        {
            if(((CScanStack*)(*i).second)->GetFailureStatus())
            {
                char szMsg[100];
                sprintf(szMsg, 
                        S_REMOVE_SCAN_STACK,
                        (*i).second->GetStackId());
                MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT, szMsg);
                TRACE(MTS_NETWORK, szMsg);
                CMsgProcessor::WriteLog(MTS_NETWORK, szMsg);

                delete (*i).second;
                m_StackMan.erase(i);
                i = m_StackMan.begin();
            }
        }
    }
}

//����szPathĿ¼�������ļ�
int CScanApp::ScanStack(const char* const szPath)
{
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;
    
    if(m_StackMan.find(0) == m_StackMan.end())
    {
        //0��Э��ջ��ʾ�������̵ĸ�Ŀ¼
        m_StackMan[0] = new CScanStack(szPath, this, 0);
    }

    //���ļ�Ŀ¼,��szPathĿ¼
    pDir = ACE_OS::opendir(szPath);
    if(pDir == NULL)
    {
        return ERR_FAIL;
    }

    char szFilePath[MAX_PATH];
	while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        SNPRINTF(szFilePath,MAX_PATH,"%s/%s",
                szPath, pDirent->d_name);
        szFilePath[MAX_PATH - 1] = 0;

        ACE_OS::stat(szFilePath, &sDirStat);
        if(S_IFDIR != (sDirStat.st_mode & S_IFMT)
		   || strcmp(pDirent->d_name,".") == 0
		   || strcmp(pDirent->d_name,"..") == 0
           || !IsDigitValue(pDirent->d_name))
        {
            continue;
        }

        int nStackID = atoi(pDirent->d_name);
        if(m_StackMan.find(nStackID) == m_StackMan.end())
        {
            char szSubPath[MAX_PATH];
            SNPRINTF(szSubPath, MAX_PATH, "%s/%s", szPath, pDirent->d_name);
            szSubPath[MAX_PATH - 1] = '\0';
            m_StackMan[nStackID] = new CScanStack(szSubPath, this, nStackID);
        }
    }
	ACE_OS::closedir(pDir);

	return ERR_SUCCESS;
}
