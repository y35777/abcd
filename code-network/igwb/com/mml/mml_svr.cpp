/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:mml_svr.cpp
    ������Ϣ:

    ����CMMLServer���ʵ��.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/

#include "mml_svr.h"
#include "../include/toolbox.h"
#include "../comm/sockets.h"
#include "mml_lex.h"
#include "resource.h"
#include "../toolbase/ftp_client.h"
//by ldf 2004-01-05 OM����
#include "../utility/encrypt.h"
#include "FastDes.h"
#include "MD5.h"
//end

int yyparse(void);
extern char* g_pbuf;
extern CParseResult* g_pParse;
extern CParameter* g_pPara;
extern CParaValue* g_pValue;
extern CPersistCmd* g_pPersistCmd;
extern CPersistPara* g_pPersistPara;

/*
    ����Ĵ�����ڲ��Ե�ʱ��ʹ��,��ӡYACC�����Ľ��.
*/
#ifdef _ONLY_FOR_TEST_

void PrintContents(CParseResult* pParse,int nErrCode)
{
    FILE* f = fopen("parse.txt","a");
    if(f == NULL) return;

    if(nErrCode != 0)
    {
        fprintf(f,"Error: %s\n",pParse->GetErrMsg());
        fclose(f);
        return;
    }

    UINT2 nParaNum = 0,nChldParaNum = 0;

    nParaNum = pParse->GetParaNum();
    CParameter* pPara = NULL;
    CParaValue* pVal = NULL;

    char szCmdName[256];

    fprintf(f,"Parameter num:%d\n",nParaNum);
    fprintf(f,"Command code:%d\n",pParse->GetCmdCode());
    fprintf(f,"Command level:%d\n",pParse->GetCmdLevel());
    fprintf(f,"Command name:%s\n",pParse->GetCmdName());
    fprintf(f,"Command dscription:%s\n",pParse->GetCmdDspName());

    for(UINT2 i = 1,j = 1;i <= nParaNum;i++)
    {
        fprintf(f,"\n");

        pPara = pParse->GetPara(i);
        if(pPara == NULL)
        {
            fprintf(f,"\t*** Para %d not repesent. ***\n",i);
            continue;
        }

        fprintf(f,"\t[Para %d]:\n",i);
        fprintf(f,"\tPara name:%s\n",pPara->GetParaName());
        fprintf(f,"\tPara code:%d\n",pPara->GetParaCode());
        fprintf(f,"\tPara value num:%d\n",pPara->GetValueNum());
        fprintf(f,"\tPara data type:%d\n",pPara->GetDataType());
        fprintf(f,"\tPara oper type:%d\n",pPara->GetOperType());

        nChldParaNum = pPara->GetValueNum();

        for(j = 1;j <= nChldParaNum;j++)
        {
            fprintf(f,"\n");

            pVal = pPara->GetValue(j);

            fprintf(f,"\t\t[Value %d]:\n",j);
            fprintf(f,"\t\tValue format:%d\n",pVal->GetValueFormat());
            fprintf(f,"\t\tStep:%d\n",pVal->GetStep());
            fprintf(f,"\t\tMax value:%s\n",pVal->GetMaxValue());
            fprintf(f,"\t\tMin value:%s\n",pVal->GetMinValue());
        }
        fprintf(f,"\n");
    }

    fclose(f);
}

#endif


CMMLServer::CMMLServer()
{
    m_pRightMan = NULL;
    m_lTimerIDBase = 0;

    //by ldf 2004-01-05 OM����
    m_szRandomNum = "";
    m_nM2000LoginTimes = 0;
    //��ȡ����IP
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();
	
    char szLocalIp[MAX_PATH];
    IniFile.GetString(CFG_SEC_MML, CFG_MML_KEY_LOCAL_IP,
	    CFG_MML_KEY_LOCAL_IP_DEFAULT,
	    szLocalIp, MAX_PATH);
    szLocalIp[MAX_PATH - 1] = '\0';
    m_strLocalLMTIP = STRING(szLocalIp);

    IniFile.Close();
    //end


}

CMMLServer::~CMMLServer()
{
}

/*
    ��������:�̳߳�ʼ������,������¹���:

    1.���ø����InitInstance()����.
    2.ʵ����CPersistCmdMan��m_pRightMan����.
    3.ע��һ��1���ӻ�׼��ʱ��.
    4.��ʼ������·�ɱ�.

    ����:
    ��ʼ���ɹ�����ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
int CMMLServer::InitInstance()
{
    CWorkModule::InitInstance();

    CINIFile iniFile(GetCfgFilePath());
    BOOL nRet = iniFile.Open();
    
    //��ȡFTPserverIP��userName,Password��������Ϣ
    iniFile.GetString(CFG_SEC_MML,CFG_MML_KEY_LOCAL_IP,  "127.0.0.1",  m_szLocalIP, MAX_HOST_LEN);
    iniFile.GetString(CFG_SEC_MML,CFG_MML_FTP_SERVER_IP,  "127.0.0.1",  m_szFTPServerIP, MAX_HOST_LEN);
    iniFile.GetString(CFG_SEC_MML,CFG_MML_FTP_USER_NAME,  "",  m_szFTPUserName, USER_NAME_LEN);
    iniFile.GetString(CFG_SEC_MML,CFG_MML_FTP_USER_PASSWORD,  "",  m_szFTPPassword, PASSWD_LEN);
    
    //��ȡ����������ı��ش��Ŀ¼
    char szLocalPath[MAX_PATH];
    iniFile.GetString(CFG_SEC_MML,CFG_MML_DLD_SOFT,  S_DLD_SOFTWARE,  szLocalPath, MAX_PATH);        
    
    INT2 nEnd = strlen(szLocalPath);
    if( szLocalPath[nEnd-1] != DELIMIT_CHAR )
    {
        sprintf(m_szLocalPath,"%s%c", szLocalPath, DELIMIT_CHAR);
    }
    else
    {
        sprintf(m_szLocalPath, "%s", szLocalPath); 
    }  
    CreateDeepDir(m_szLocalPath);
    

    CPersistCmdMan* pCmdMan = CPersistCmdMan::Instance();
    if(pCmdMan == NULL)
    {
        TRACE(MTS_MMLSERVER,S_MML_INIT_MACRO_FAIL);
        return ERR_FAIL;
    }

    m_pRightMan = new CRightMan();
    if(!m_pRightMan->Init())
    {
        TRACE(MTS_MMLSERVER,S_MML_INIT_USER_FAIL);
        return ERR_FAIL;
    }

    m_lTimerIDBase = SetTimer(ONE_SECOND_IN_MILISEC);

    /* ��ʼ��m_CmdRouterArray. */
    AddCmdRouter();

    return ERR_SUCCESS;
}


void CMMLServer::AddCmdRouter()
{
    /* 
        ��ѯ���ģ��,��ģ����������,���Է���ͷ��. 
    */
    CmdRouter* p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_BROWSER;
    p->uMinCmdCode = MML_MIN_CMDCODE_BROWSER;
    p->yReceiverMid = MT_BILLBROWSER;
    p->yReceiverPid = GetProcessType();

    m_CmdRouterArray.push_back(p);

    /* ����ģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_NETWORK;
    p->uMinCmdCode = MML_MIN_CMDCODE_NETWORK;
    p->yReceiverMid = MT_MMLSERVER;
    p->yReceiverPid = GetProcessType();
    
    m_CmdRouterArray.push_back(p);

    /* MMLServerģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_MMLSERVER;
    p->uMinCmdCode = MML_MIN_CMDCODE_MMLSERVER;
    p->yReceiverMid = MT_MMLSERVER;
    p->yReceiverPid = GetProcessType();

    m_CmdRouterArray.push_back(p);

    /* ��־����ģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_LOG;
    p->uMinCmdCode = MML_MIN_CMDCODE_LOG;
    p->yReceiverMid = MT_LOG;
    p->yReceiverPid = GetProcessType();

    m_CmdRouterArray.push_back(p);

    /* �Զ�����ģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_BACKUP;
    p->uMinCmdCode = MML_MIN_CMDCODE_BACKUP;
    p->yReceiverMid = MT_BACKUP;
    p->yReceiverPid = GetProcessType();

    m_CmdRouterArray.push_back(p);

    /* �����Ϣ,��MMLSERVER����������ϵͳ�㲥 */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_FRAME;
    p->uMinCmdCode = MML_MIN_CMDCODE_FRAME;
    p->yReceiverMid = MT_MMLSERVER;
    p->yReceiverPid = GetProcessType();;

    m_CmdRouterArray.push_back(p);

    /* ����ģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_PERF;
    p->uMinCmdCode = MML_MIN_CMDCODE_PERF;
    p->yReceiverMid = MT_PERF;
    p->yReceiverPid = GetProcessType();

    m_CmdRouterArray.push_back(p);

    /* ���ģ��,KERNEL���� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_MONITOR;
    p->uMinCmdCode = MML_MIN_CMDCODE_MONITOR;
    p->yReceiverMid = MT_MONITOR;
    p->yReceiverPid = PT_KERNEL;

    m_CmdRouterArray.push_back(p);

    /* ˫��ģ��,Agent���� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_CLSTR;
    p->uMinCmdCode = MML_MIN_CMDCODE_CLSTR;
    p->yReceiverMid = MT_CLSTR;
    p->yReceiverPid = PT_CLSTR;

    m_CmdRouterArray.push_back(p);

    /* �澯ģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_ALARM;
    p->uMinCmdCode = MML_MIN_CMDCODE_ALARM;
    p->yReceiverMid = MT_ALARM;
    p->yReceiverPid = GetProcessType();
   
    m_CmdRouterArray.push_back(p); 
    
    /* �ɼ�����ģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_COLLECTOR;
    p->uMinCmdCode = MML_MIN_CMDCODE_COLLECTOR;
    p->yReceiverMid = MT_COLAGENT;
    p->yReceiverPid = GetProcessType();
    m_CmdRouterArray.push_back(p);

    /* ͳ��ģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_CDRSTAT;
    p->uMinCmdCode = MML_MIN_CMDCODE_CDRSTAT;
    p->yReceiverMid = MT_CDRSTAT;
    p->yReceiverPid = GetProcessType();

    m_CmdRouterArray.push_back(p);

    //������ M2000���ܽӿ�
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_M2000;
    p->uMinCmdCode = MML_MIN_CMDCODE_M2000;
    p->yReceiverMid = MT_PERF;
    p->yReceiverPid = GetProcessType();
    m_CmdRouterArray.push_back(p);

    /* Զ������ģ�� */
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_CFGSVR;
    p->uMinCmdCode = MML_MIN_CMDCODE_CFGSVR;
    p->yReceiverMid = MT_CFGSVR;
    p->yReceiverPid = GetProcessType();   
    m_CmdRouterArray.push_back(p);  

    /*LICENSEģ�� zkq 2004-06-23*/
    p = new CmdRouter;
    p->uMaxCmdCode = MML_MAX_CMDCODE_LICENSE_MANAGER;
    p->uMinCmdCode = MML_MIN_CMDCODE_LICENSE_MANAGER;
    p->yReceiverMid = MT_LICENSE_MANAGER;
    p->yReceiverPid = PT_KERNEL;   
    m_CmdRouterArray.push_back(p);     
}

/*
    ��������:�߳��������,������¹���:

    1.ɾ��CPersistCmdMan����.
    2.ɾ��m_pRightMan����.
    3.���ٻ�׼��ʱ��.
    4.���ø����ExitInstance()����.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ExitInstance()
{
    if(m_pRightMan != NULL) delete m_pRightMan;

    CPersistCmdMan::Destroy();
    KillTimer(m_lTimerIDBase);

    VECTOR<CmdRouter*>::iterator i = m_CmdRouterArray.begin(),
                                 e = m_CmdRouterArray.end();
    while(i != e)
    {
        delete *i;
        i++;
    }

    CWorkModule::ExitInstance();
}

MODULE_TYPE CMMLServer::GetModuleType()
{
    return MT_MMLSERVER;
}

/*
    ��������:�����������ȡ����Ĵ���ģ��.

    ����:
    uCmdCode,������.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
CmdRouter* CMMLServer::GetMsgTarget(UINT2 uCmdCode)
{
    VECTOR<CmdRouter*>::iterator i = m_CmdRouterArray.begin(),
                                 e = m_CmdRouterArray.end();
    while(i != e)
    {
        if(((*i)->uMaxCmdCode >= uCmdCode)
            && ((*i)->uMinCmdCode <= uCmdCode))
        {
            return *i;
        }
        i++;
    }

    return NULL;
}

void CMMLServer::ParseMsg(MSG_INTRA* pMsg)
{
    switch(pMsg->CmdCode)
    {
    case CMD_MML:
        {
            /*
                ����MML�ͻ��˵�����.

                1.��YACC�﷨������CParseResult������������Ա�����:
                  m_u2CmdCode,m_szCmdName,m_yCmdLevel,m_u2ParaNum,
                  m_ParaVector.

                2.��CRightMan::CheckRight()������CParseResult��
                  m_szUserName,m_yUserLevel����.

                3.��������m_u2WsNo,m_u2WsType,m_u4IP����.
            */
            
            #ifdef _ONLY_FOR_TEST_
                printf("MML command received : %s\n",(char*)(pMsg->Data));
            #endif

            g_pPara = NULL;
            g_pValue = NULL;
            g_pPersistCmd = NULL;
            g_pPersistPara = NULL;

            if(g_pParse != NULL) delete g_pParse;

            g_pParse = new CParseResult(pMsg);

            /*
                �﷨����,������մʷ����������ڲ�������.
            */
            YY_FLUSH_BUFFER;
            g_pbuf = (char*)(pMsg->Data);
            int nErrCode = yyparse();

            /*
                ����Ĵ�����ڲ��Ե�ʱ��ʹ��,��ӡYACC�����Ľ��.
            */
            #ifdef _ONLY_FOR_TEST_
                PrintContents(g_pParse,nErrCode);
            #endif

            if(nErrCode == 0)
            {
                /* 
                    ����MML �����������,
                    ��m_CmdRouterArray�в�������Ľ���ģ��.
                */
                CmdRouter* pRoutor = GetMsgTarget(g_pParse->GetCmdCode());
                if(pRoutor == NULL)
                {
                    /* ����û�н���ģ��,���ɳ����Ļ��Ϳͻ���. */
                    SendSimpleReport(g_pParse,1,S_MML_NO_CMD_TARGET,pMsg);
                    TRACE(MTS_MMLSERVER,S_MML_NO_TARGET_RECV,
                                        g_pParse->GetCmdCode());

                    break;
                }

				//by ldf 2004-08-17 ��Ӧ���ⵥSWPD07814
				if((g_pParse->GetCmdCode() != MML_SHK_HAND)&&(g_pParse->GetCmdCode() != MML_SHK))
				{
                    nErrCode = m_pRightMan->CheckRight(g_pParse);
				}
                strncpy(pMsg->UserName,g_pParse->GetUserName(),48);
                pMsg->UserName[47] = 0;

                if(nErrCode != ERR_SUCCESS)
                {
                    //��Ч��LGO
                    if(nErrCode == ERR_USER_LGO_AGAIN)
                        break;
                    /* û��Ȩ��,���ɳ����Ļ��Ϳͻ���. */
                    //SendSimpleReport(g_pParse,1,S_MML_NO_RIGHT,pMsg);

                    char buf[256];
                    sprintf(buf,S_MML_OPERATION_DENIED,
                                g_pParse->GetCmdCode());

                    WriteLog(g_pParse,1,buf,pMsg);
                    TRACE(MTS_MMLSERVER,buf);

                    break;
                }

                if(GetProcessType() != pRoutor->yReceiverPid)
                {
                    /* ����Ϣ���͸������,�������л�g_pParse����. */

                    UINT4 nSerialLen = g_pParse->GetParseLength();
                    MSG_INTRA* p = new(nSerialLen) MSG_INTRA;

                    g_pParse->Serialize((char*)p->Data,nSerialLen);

                    p->SenderPid = GetProcessType();
                    p->SenderMid = GetModuleType();
                    p->ReceiverPid = pRoutor->yReceiverPid;
                    p->ReceiverMid = pRoutor->yReceiverMid;
                    p->CmdCode = CMD_MML;

                    p->AppType = pMsg->AppType;
                    p->ClientNo = pMsg->ClientNo;
                    p->IPAddr = pMsg->IPAddr;
                    strncpy(p->UserName,g_pParse->GetUserName(),48);
                    p->UserName[47] = 0;

                    SendMsg(p);
                }
                else if(GetModuleType() == pRoutor->yReceiverMid)
                {
                    /* ��ģ�鴦�����Ϣ. */

                    ProcessMsg(g_pParse,pMsg);
                }
                else
                {
                    /* ������������ģ�鴦�����Ϣ. */

                    MSG_INTRA* p = new(sizeof(CParseResult*)) MSG_INTRA;
                    *((CParseResult**)p->Data) = g_pParse;

                    p->SenderPid = PT_SELF;
                    p->SenderMid = GetModuleType();
                    p->ReceiverPid = PT_SELF;
                    p->ReceiverMid = pRoutor->yReceiverMid;
                    p->CmdCode = CMD_MML;

                    p->AppType = pMsg->AppType;
                    p->ClientNo = pMsg->ClientNo;
                    p->IPAddr = pMsg->IPAddr;
                    strncpy(p->UserName,g_pParse->GetUserName(),48);
                    p->UserName[47] = 0;

                    if(SendMsg(p) == ERR_SUCCESS)
                    {
                        /* g_pParse�ɽ����ڵ�����ģ��ɾ��. */
                        g_pParse = NULL;
                    }
                }
            }
            else
            {
                /* ���ɳ����Ļ��Ϳͻ���. */
                SendSimpleReport(g_pParse,1,g_pParse->GetErrMsg(),pMsg);
                TRACE(MTS_MMLSERVER,S_MML_PARSE_FAIL,g_pParse->GetCmdLine());
            }
        }
        break;

    default:
        /* δ���������,дTRACE. */
		//������ 2002-04-25 ���ⵥ 12535
        //TRACE(MTS_MMLSERVER,S_FRAME_UNPROCESSED_MSG,pMsg->SenderPid,
        //                                   pMsg->SenderMid,
        //                                   pMsg->CmdCode);
        break;
    }
}

/*
    ��������:����MML�ͻ��˷��͵�MML����.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessMsg(CParseResult* pParse,MSG_INTRA* pMsg)
{
    switch(pParse->GetCmdCode())
    {
    //by ldf ��Ӧ���ⵥD20443 2003-03-06
    case MML_SHK_HAND:      //MML�ͻ�������
    case MML_SHK:           //MML�ͻ�������(����֧��SHK:;����)
        ProcessShakeHands(pParse,pMsg);
        break;
    case MML_ADD_OP:        //���Ӳ���Ա�ʺ�
        ProcessAddUser(pParse,pMsg);
        break;

    case MML_RMV_OP:        //ɾ������Ա�ʺ�
        ProcessDelUser(pParse,pMsg);
        break;
    
    case MML_MOD_OP:        //�޸��û�����
        ProcessModifyUsrInfo(pParse,pMsg);
        break;
    
    case MML_MOD_PWD:       //�޸Ĳ���Ա�Ŀ���
        ProcessModifyPwd(pParse,pMsg);
        break;

    case MML_LST_OP:        //��ѯ����Ա�ĸ�����Ϣ
        ProcessQueryUsrInfo(pParse,pMsg);
        break;

    case MML_LGI:           //����Ա��¼
        ProcessLogon(pParse,pMsg);
        break;

    case MML_DLD_SOFTWARE:
        ProcessDldSoftware(pParse, pMsg); 
        break;       
     //by zkq 2004-07-31,������������̨�ĵ�½����
    case MML_LGI_CFG:       //����̨����Ա��½
        ProcessCfgLogon(pParse,pMsg);
        break;
    //end
    case MML_LGO:           //����Աע��,û�г�����.
        ProcessLogout(pParse,pMsg);
        break;
    case MML_QUERY_OP:
        ProcessQueryOp(pParse,pMsg);
        break;
    case MML_BRK_OP:
        ProcessBreakOp(pParse,pMsg);
        break;

    //by ldf 2003-01-30 OM����
    case MML_RVI_OP:           //M2000������Ԫ��֤����Ա���,��Ԫ���������ȨID��M2000
        ProcessReqID(pParse,pMsg);
        break;

    case MML_SND_CRG:           //M2000�������ĵ���Ԫ����Ԫ��֤��ȨID
        ProcessCheckID(pParse,pMsg);
        break;
    //end

    case MML_SET_PTRACK:    //����Э����ٿ���
    case MML_SET_STRACK:    //����ҵ�����̸��ٿ���
    case MML_SET_DBG:       //���ò�ѯ�����������
    case MML_SET_PERF:      //��������ͳ�ƿ���
        ProcessSetFlag(pParse,pMsg);
        break;

    case MML_LST_DBG:       //��ѯ�����������
        ProcessQueryDebugLvl(pParse,pMsg);
        break;
    //by ldf ��Ӧ���ⵥD20443 2003-03-06
    case MML_LST_VERSION:       //��ѯϵͳ�汾��Ϣ(���̰汾��)
    case MML_LST_VER:           //��ѯϵͳ�汾��Ϣ(�̰汾��)
    case MML_LST_INTVER:        //��ѯϵͳ�汾��Ϣ(���汾��)
        ProcessQueryVer(pParse,pMsg);
        break;
    case MML_ADD_IP:
        ProcessIPFilter(pParse,pMsg);
        break;
    case MML_SET_OPCG:      //���ò���Ա��Ȩ��������
    case MML_SET_OPTM:      //���ò���Ա�Ĳ���ʱ��
    case MML_LST_OPTM:      //��ѯ����Ա����ʱ��
    case MML_LST_CMDS:      //��ѯ��ǰ��ִ�е�����
    case MML_ADD_CCG:       //����������������������
    case MML_RMV_CCG:       //ɾ��������������������
    case MML_LST_CCG:       //��ѯ������������������
        m_pRightMan->ProcessPrivilegeCommand(pParse,pMsg);
    default:
        break;
    }
}

/*
    ��������:����"��������Ӧ��".

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessShakeHands(CParseResult* pParse,
                                   MSG_INTRA* pMsg)
{
    SendSimpleReport(pParse,0,S_MML_SHK,pMsg);
}

/*
    ��������:��ϵͳ������һ���û�.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessAddUser(CParseResult *pParse,MSG_INTRA* pMsg)
{
    char buf[256];

    int nRet = m_pRightMan->AddUser(pParse);
    if(nRet == ERR_SUCCESS)
    {
        //���͸澯
        SInnerAlarm sAlarm;
        sAlarm.uAlarmID = ALARM_ID_USEREINFO_CHANGED;
        sAlarm.yAlarmType = AT_EVENT;
        sAlarm.yAlarmPara[0] = ALARM_PARA_ADD_USERINFO;
        
        //��������дIP
        ACE_INET_Addr AddrClient;
        AddrClient.set(0, NTOHL(pParse->GetIP()), 1);
        int nIPLen = strlen(AddrClient.get_host_addr());
        memcpy(&sAlarm.yAlarmPara[1], AddrClient.get_host_addr(), nIPLen);
        
        sAlarm.yAlarmPara[nIPLen + 1] = ',';//�Ӷ��ŷָ���
        
        //��������д�û���
        strncpy((char *)&sAlarm.yAlarmPara[nIPLen + 2], pParse->GetStringVal(1), MAX_ALARM_PARA - nIPLen - 3);
        sAlarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
        CMsgProcessor::SendAlarm(&sAlarm);        
        
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);

        SendSimpleReport(pParse,0,S_MML_ADD_OP_OK,pMsg);
        return;
    }

    switch(nRet)
    {
    case ERR_PASSWORD_ERR:          //�������
        SendSimpleReport(pParse,1,S_MML_PWD_INCORRECT,pMsg);
        break;
    case ERR_USER_ALREADY_EXIST:    //�û��Ѿ�����
        SendSimpleReport(pParse,1,S_MML_ACC_EXIST,pMsg);
        break;
    case ERR_FULL_USER:             //�û����Ѿ��ﵽ���
        SendSimpleReport(pParse,1,S_MML_REACH_ACC_LIMIT,pMsg);
        break;
    case ERR_PASSWD_LEN_EXCCED:     //���볤��̫��
        SendSimpleReport(pParse,1,S_MML_REACH_PWD_LIMIT,pMsg);
        break;
    case ERR_ACCOUNT_IS_EMPTY:      //�ʺ���Ϊ��
        SendSimpleReport(pParse,1,S_MML_ACCOUNT_IS_EMPTY,pMsg);
        break;
    //by ldf 2004-01-30 OM����
    case ERR_ADD_M2000_ACCOUNT:      //�ʺ�����M2000��ͷ
        SendSimpleReport(pParse,1,S_MML_ADD_M2000_ACCOUNT,pMsg);
        break;
    //end
    case ERR_FAIL:
    default:                        //����ʧ��
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        break;
    }

    sprintf(buf,S_MML_OPERATION_FAIL,pParse->GetCmdCode(),nRet);
    WriteLog(pParse,1,buf,pMsg);
}

/*
    ��������:��ϵͳ��ɾ��һ���û�.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessDelUser(CParseResult *pParse,MSG_INTRA* pMsg)
{
    char buf[256];

    int nRet = m_pRightMan->DelUser(pParse);
    if(nRet == ERR_SUCCESS)
    {
        //���͸澯
        SInnerAlarm sAlarm;
        sAlarm.uAlarmID = ALARM_ID_USEREINFO_CHANGED;
        sAlarm.yAlarmType = AT_EVENT;
        sAlarm.yAlarmPara[0] = ALARM_PARA_DEL_USERINFO;
        
        //��������дIP
        ACE_INET_Addr AddrClient;
        AddrClient.set(0, NTOHL(pParse->GetIP()), 1);
        int nIPLen = strlen(AddrClient.get_host_addr());
        memcpy(&sAlarm.yAlarmPara[1], AddrClient.get_host_addr(), nIPLen);
        
        sAlarm.yAlarmPara[nIPLen + 1] = ',';//�Ӷ��ŷָ���
        
        //��������д�û���
        strncpy((char *)&sAlarm.yAlarmPara[nIPLen + 2], pParse->GetStringVal(1), MAX_ALARM_PARA - nIPLen - 3);
        sAlarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
        CMsgProcessor::SendAlarm(&sAlarm);        
        
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);

        SendSimpleReport(pParse,0,S_MML_SUCCEED,pMsg);
        return;
    }

    switch(nRet)
    {
    case ERR_NOT_ENOUGHT_RIGHT: //Ȩ�޲���
        SendSimpleReport(pParse,1,S_MML_NO_RIGHT,pMsg);
        break;
    case ERR_NO_CRU_USER:       //�û�������
        SendSimpleReport(pParse,1,S_MML_ACC_NOT_EXIST,pMsg);
        break;
    case ERR_USER_IN_LINE:      //�û������޷�ɾ��
        SendSimpleReport(pParse,1,S_MML_USER_LOGIN_DEL_FAIL,pMsg);
        break;
    case ERR_FAIL:
    default:
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        break;
    }

    sprintf(buf,S_MML_OPERATION_FAIL,pParse->GetCmdCode(),nRet);
    WriteLog(pParse,1,buf,pMsg);
}

/*
    ��������:�޸��û���Ϣ.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessModifyUsrInfo(CParseResult *pParse,
                                      MSG_INTRA* pMsg)
{
    char buf[256];

    int nRet = m_pRightMan->ModifyUserInfo(pParse);
    if(nRet == ERR_SUCCESS)
    {
        //���͸澯
        SInnerAlarm sAlarm;
        sAlarm.uAlarmID = ALARM_ID_USEREINFO_CHANGED;
        sAlarm.yAlarmType = AT_EVENT;
        sAlarm.yAlarmPara[0] = ALARM_PARA_MOD_USERINFO;
        
        //��������дIP
        ACE_INET_Addr AddrClient;
        AddrClient.set(0, NTOHL(pParse->GetIP()), 1);
        int nIPLen = strlen(AddrClient.get_host_addr());
        memcpy(&sAlarm.yAlarmPara[1], AddrClient.get_host_addr(), nIPLen);
        
        sAlarm.yAlarmPara[nIPLen + 1] = ',';//�Ӷ��ŷָ���
        
        //��������д�û���
        strncpy((char *)&sAlarm.yAlarmPara[nIPLen + 2], pParse->GetStringVal(1), MAX_ALARM_PARA - nIPLen - 3);
        sAlarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
        CMsgProcessor::SendAlarm(&sAlarm);        
        
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);

        SendSimpleReport(pParse,0,S_MML_SUCCEED,pMsg);
        return;
    }

    switch(nRet)
    {
    case ERR_PASSWORD_ERR:  //ȷ�����벻ƥ��
        SendSimpleReport(pParse,1,S_MML_PWD_INCORRECT,pMsg);
        break;
    case ERR_NO_CRU_USER:   //�û�������
        SendSimpleReport(pParse,1,S_MML_ACC_NOT_EXIST,pMsg);
        break;
    case ERR_PASSWD_LEN_EXCCED: //���볤��̫��
        SendSimpleReport(pParse,1,S_MML_REACH_PWD_LIMIT,pMsg);
        break;
    case ERR_FAIL:
    default:
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        break;
    }

    sprintf(buf,S_MML_OPERATION_FAIL,pParse->GetCmdCode(),nRet);
    WriteLog(pParse,1,buf,pMsg);
}

/*
    ��������:�޸��û�����.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessModifyPwd(CParseResult *pParse,MSG_INTRA* pMsg)
{
    char buf[256];

    int nRet = m_pRightMan->Password(pParse);
    if(nRet == ERR_SUCCESS)
    {
        //���͸澯
        SInnerAlarm sAlarm;
        sAlarm.uAlarmID = ALARM_ID_USEREINFO_CHANGED;
        sAlarm.yAlarmType = AT_EVENT;
        sAlarm.yAlarmPara[0] = ALARM_PARA_MOD_USERINFO;
        
        //��������дIP
        ACE_INET_Addr AddrClient;
        AddrClient.set(0, NTOHL(pParse->GetIP()), 1);
        int nIPLen = strlen(AddrClient.get_host_addr());
        memcpy(&sAlarm.yAlarmPara[1], AddrClient.get_host_addr(), nIPLen);
        
        sAlarm.yAlarmPara[nIPLen + 1] = ',';//�Ӷ��ŷָ���
        
        //��������д�û���
        strncpy((char *)&sAlarm.yAlarmPara[nIPLen + 2], pParse->GetStringVal(1), MAX_ALARM_PARA - nIPLen - 3);
        sAlarm.yAlarmPara[MAX_ALARM_PARA - 1] = '\0';
        CMsgProcessor::SendAlarm(&sAlarm);        
        
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);

        SendSimpleReport(pParse,0,S_MML_SUCCEED,pMsg);
        return;
    }
    
    switch(nRet)
    {
    case ERR_PWD_UNMATCH:       //ȷ�����벻ƥ��
        SendSimpleReport(pParse,1,S_MML_PWD_UNMATCH,pMsg);
        break;
    case ERR_PASSWORD_ERR:      //�������
        SendSimpleReport(pParse,1,S_MML_PWD_INCORRECT,pMsg);
        break;
    case ERR_NO_CRU_USER:       //�û�������
        SendSimpleReport(pParse,1,S_MML_ACC_NOT_EXIST,pMsg);
        break;
    case ERR_PASSWD_LEN_EXCCED: //����̫��
        SendSimpleReport(pParse,1,S_MML_REACH_PWD_LIMIT,pMsg);
        break;
    case ERR_FAIL:
    default:
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        break;
    }

    sprintf(buf,S_MML_OPERATION_FAIL,pParse->GetCmdCode(),nRet);
    WriteLog(pParse,1,buf,pMsg);
}

/*
    ��������:��ѯ�����û���Ϣ.

    ��������:�������¸�ʽ���MML����.

    ----------------------------------------------------------
    ����Ա����        ����Ա����                Ȩ��      ������Ϣ

    Admin             iGateway bill��������Ա   ����Ա    ����
    CYAN              ������ά����Ա            ��ͨ�û�  δ����     

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessQueryUsrInfo(CParseResult *pParse,
                                     MSG_INTRA* pMsg)
{
    char buf[256];
    sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
    WriteLog(pParse,0,buf,pMsg);

    LIST<STRING> title,content;

    /*
        �����title���ֵ�˳��,
        �����CRightMan::QueryUserInfo()�����һ��.
    */
    title.push_back(S_MML_LST_OP_NAME);
    title.push_back(S_MML_LST_OP_DESP);
    title.push_back(S_MML_LST_OP_RIGHT);
    title.push_back(S_MML_LST_OP_ONLINE);
    title.push_back(S_MML_AREA);
    title.push_back(S_MML_PRIVILEGE_INFO);
    title.push_back(S_MML_PWD_LIMIT_TIME); 
	title.push_back(S_MML_SESSION);
    m_pRightMan->QueryUserInfo(content, pParse);

    /*
        ���ɲ�����MML����.
    */
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();

    Report.MakeCmdDisplay(pParse->GetCmdLine(),
                          0, 0, THISMODULE);

    Report.AddHoriTable(title,content);
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();

    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);

    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;

    SendMsg(pAns); 
}

/*
    ��������:�û���¼.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessLogon(CParseResult *pParse,MSG_INTRA* pMsg)
{
    char buf[256];

    /*
        �ڴ����¼����֮ǰ,����������ʽ�ı��ĸ��ͻ���:
    
      ++++++++++++++++++++++++++++
      S_IGATEWAY_BILL_VER
      ++++++++++++++++++++++++++++
    */
    STRING strReport = S_MML_SEPARATOR;
    strReport += "\r\n";
    strReport += S_IGATEWAY_BILL_VER;
    strReport += "\r\n";
    strReport += S_MML_SEPARATOR;
    strReport += "\r\n";
    
    MSG_INTRA* pAns = new(strReport.size() + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), strReport.c_str());
    
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;
    
    SendMsg(pAns); 
    
    int nRet = m_pRightMan->Login(pParse);
    if(nRet == ERR_SUCCESS)
    {
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);

        SendSimpleReport(pParse,0,S_MML_LOGIN_SUCCEED,pMsg);

#ifdef _OM_MODIFY
        //��6099�˿��ϱ�
        SendSynConfReport(pParse, pParse->GetStringVal(1),  pParse->GetStringVal(3));       
#endif              
        
        return;
    }
    
    switch(nRet)
    {
    case ERR_TOO_MUCH_USER: //��¼�û���̫��
        SendSimpleReport(pParse,1,S_MML_TOO_MANY_USER,pMsg);
        break;
    case ERR_MULTI_LOGIN:   //�ظ���¼
        SendSimpleReport(pParse,1,S_MML_ALREADY_LGGIN,pMsg);
        break;
    case ERR_NO_CRU_USER:   //�ʻ�������
        SendSimpleReport(pParse,1,S_MML_ACC_NOT_EXIST,pMsg);
        break;
    case ERR_PASSWORD_ERR:  //�������
        SendSimpleReport(pParse,1,S_MML_PWD_INCORRECT,pMsg);
        break;
	case ERR_PWD_OUTDATED:  //�������
		SendSimpleReport(pParse,0,S_MML_PWD_OUTDATED,pMsg);
		break;
	case ERR_EXPIRE_USER:	//�û���ʱ�����½	
        SendSimpleReport(pParse,1,S_MML_EXPIRE_USER,pMsg);
        break;
    case ERR_FAIL:
    default:
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        break;
    }

    sprintf(buf,S_MML_OPERATION_FAIL,pParse->GetCmdCode(),nRet);
    WriteLog(pParse,1,buf,pMsg);
}

BOOL CMMLServer::DownLoadSoft(CParseResult* pParse)
{
    /*�������Ϊ��1=IP (FTP server IP)
    2=USR (FTP SERVER USERNAME)
        3=PWD (FTP SERVER PASSWORD)
        4=DIR (FTP SERVER DIRECTORY)
        5=FT (�����ļ�����) 
        6= SV (����汾)
        7= DF (�����ļ�ѡ��)
        8=GA (�Ƿ��ϱ����ȱ�־)*/
        
    //����IP
    STRING strFTPServerIP = pParse->GetStringVal(1);
    if(strFTPServerIP == "")
    {
        strFTPServerIP = m_szFTPServerIP;
    }

    //userName��ѡ
    STRING strUserName = pParse->GetStringVal(2);
    if(strUserName == "")
    {
        strUserName = m_szFTPUserName;
    }

    //password��ѡ
    STRING strPassword = pParse->GetStringVal(3);
    if(strPassword == "")
    {
        strPassword = m_szFTPPassword;
    }   
    
    //�����ļ��б��ļ�
    CFtpClient FtpClient;
    if (0 != FtpClient.Connect(strFTPServerIP.c_str(), strUserName.c_str(), strPassword.c_str(), m_szLocalIP))
    {
        return FALSE;
    }
    
    //��ż��ذ汾��Ŀ¼
    STRING strDldPath = pParse->GetStringVal(4);
    INT2 nSize = strDldPath.length();
    if(strDldPath.find_last_of(DELIMIT_CHAR) != nSize)
    {
        strDldPath = strDldPath + DELIMIT_CHAR;
    } 
    if (0 != FtpClient.SetDirectory(strDldPath.c_str()) )
    {
        return FALSE;
    }
    
    STRING strLocalPath = m_szLocalPath;
    STRING strLocalFile = strLocalPath + "igwbpkg_list.txt";
    STRING strDesFile = strDldPath + "igwbpkg_list.txt";
    if (0 != FtpClient.GetFile(strLocalFile.c_str(), strDesFile.c_str()) )
    {
        return FALSE;   
    }
    
    //��igwbpkg_list.txt�ļ�
    FILE* fFileList = NULL; 
    fFileList = fopen(strLocalFile.c_str(), "rb");
    if(fFileList == NULL)
    {
        return FALSE;
    }

    //����Ҫ���ص��ļ��б������ļ�
    char buf[MAX_LINE_LEN]; 
    while(fgets(buf, MAX_LINE_LEN, fFileList) != NULL)
    {
        strLocalFile = "";
        int nLen = strlen(buf);
        if(buf[nLen - 2] == '\r')    //�н���Ϊ\r\n
        {
            buf[nLen - 2] = '\0';
        }
        else if(buf[nLen -1] == '\n') //�н���Ϊ\n
        {
            buf[nLen - 1] ='\0';
        }
        strLocalFile = strLocalPath + buf;
        strDesFile = "";
        strDesFile = strDldPath + buf;
        if(0 != FtpClient.GetFile(strLocalFile.c_str(), strDesFile.c_str()) )
        {
            return FALSE;
        }
    }    
    
    FtpClient.Disconnect();
    return TRUE;
}

void CMMLServer::ProcessDldSoftware(CParseResult* pParse,MSG_INTRA* pMsg)
{
    char buf[256];
    BOOL bRet = DownLoadSoft(pParse);
    if(bRet)
    {
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);

        char buf[256];
        int nLen = strlen(GetStringFromID(5001));
        strcpy(buf, GetStringFromID(5001));
        buf[nLen] = '=';
        buf[nLen + 1] = '1';
        buf[nLen + 2] = '\0';
        SendMMLReprot(pParse, 0, buf, pMsg);
        
        //SendSimpleReport(pParse,0,S_MML_SUCCEED, pMsg);
    }
    else
    {
        SendMMLReprot(pParse, 1, NULL, pMsg);
        //SendSimpleReport(pParse,1,S_MML_OPERATION_FAIL,pMsg);
    }

    //by ldf 2004-08-17 OM-M2000����
    CMMLReport MMLReport("HUAWEI");
    MMLReport.MakeReportHead();
    MMLReport.MakeCmdDisplay("DLD SOFTWARE:",0,0,"O&M");
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportItem("Report Type","Download",24,ALIGN_RIGHT,8,ALIGN_LEFT,1);
    if (bRet)
    {
        MMLReport.MakeReportItem("Status","Success",24,ALIGN_RIGHT,7,ALIGN_LEFT,1); 
    }
    else
    {
        MMLReport.MakeReportItem("Status","Failure",24,ALIGN_RIGHT,7,ALIGN_LEFT,1); 
    }
    
    MMLReport.MakeReportItem("Session ID","1",24,ALIGN_RIGHT,1,ALIGN_LEFT,1); 

    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportTail();    
	
	char* szReport;
    szReport = new char[strlen(MMLReport.GetReport()) + 1];
    sprintf(szReport,"%s",MMLReport.GetReport());
	
	//���͵��ͻ���
	int nStrLen = strlen(szReport);
    MSG_INTRA* pMsgProcess = new(nStrLen + 1) MSG_INTRA;
    pMsgProcess->SenderPid   = GetProcessType();
    pMsgProcess->SenderMid   = GetModuleType();
    pMsgProcess->ReceiverPid = PT_OUTSIDE;
    pMsgProcess->ReceiverMid = MT_MMLDEBUG;
	
    strcpy(pMsgProcess->UserName,pMsg->UserName);
	
    pMsgProcess->AppType = APP_MML_NORM;
    pMsgProcess->ClientNo =pMsg->ClientNo;
    pMsgProcess->IPAddr      =pMsg ->IPAddr;
    pMsgProcess->CmdCode     = CMD_MML;
	
    memcpy(pMsgProcess->Data, szReport, nStrLen + 1);
	delete[] szReport;
	
    SendMsg(pMsgProcess);
	//end
}

/*
    ��������:�û�ע��.

    ���û��Ѿ���¼,��ɾ��MML SERVERά�����û���¼,
    �������κδ���.

    �������ʧ��,����ֻ��MML�ͻ��˷��سɹ�����.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessLogout(CParseResult *pParse,MSG_INTRA* pMsg)
{
    char buf[256];
    sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
    WriteLog(pParse,0,buf,pMsg);
    ProcessSetFlag_impl(MML_SET_STRACK,1,pMsg);
    ProcessSetFlag_impl(MML_SET_PTRACK,1,pMsg);
    
    m_pRightMan->Logout(pParse);
    SendSimpleReport(pParse,0,S_MML_SUCCEED,pMsg);

#ifdef _OM_MODIFY 
    //��6099�˿��ϱ�
    SendSynConfReport(pParse, pParse->GetUserName(),  pParse->GetStringVal(1));
#endif    
    
    //by ldf 2004-01-30 OM����
    //�����M2000�û��˳������У����
    if(ACE_OS::strncasecmp(pParse->GetUserName(),M2000,4) == 0)
    {
        m_szRandomNum.erase();
    }
    //end

}

/*
    ��������:����ϵͳ�ĸ��ֿ���,����:
    ��Ϣ�������,Э����ٿ���,����ͳ�ƿ��غ���Ϣ������.

    ��Щ��Ϣ��MML SERVER��������,������ϵͳ����Ϣ��תģ��ת��.
    ��Щ�����Ҫ����MML����Ŀͻ��˷�����Ϣ.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessSetFlag(CParseResult *pParse,MSG_INTRA* pMsg)
{
    char buf[256];
    
    INT nOpt = 0;
    if(!pParse->GetDigitVal(&nOpt,1))
    {
        /* ���ɴ�����,�Ϳͻ���. */
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        WriteLog(pParse,-1,NULL,pMsg);
        return;
    }
    
    sprintf(buf,S_MML_SET_FLAG,pParse->GetCmdCode(),nOpt);
    WriteLog(pParse,0,buf,pMsg);
    ProcessSetFlag_impl(pParse->GetCmdCode(),nOpt,pMsg);
}
void CMMLServer::ProcessSetFlag_impl(int nCmd,int nOpt,MSG_INTRA *pMsg)
{
    INT uCmdCode = 0;
    if(nCmd == MML_SET_STRACK)
    {
        if(nOpt == 0)
        {
            //������ 2003-02-15 ���ⵥ20356
            m_WorkflowSet.insert(pMsg->ClientNo);
            //����
            uCmdCode = CMD_TRACKMSG_ENABLE;
        }
        else if(nOpt == 1)
        {
            //������ 2003-02-15 ���ⵥ20356
            m_WorkflowSet.erase(pMsg->ClientNo);
            if(m_WorkflowSet.size() == 0)
                uCmdCode = CMD_TRACKMSG_DISABLE;
            //����
        }
        else
            return;
    }
    else if(nCmd == MML_SET_PTRACK)
    {
        if(nOpt == 0)
        {
            //������ 2003-02-15 ���ⵥ20356
            m_ProtocolSet.insert(pMsg->ClientNo);
            //����
            uCmdCode = CMD_TRACKPROTOCOL_ENABLE;
        }
        else if(nOpt == 1)
        {
            //������ 2003-02-15 ���ⵥ20356
            m_ProtocolSet.erase(pMsg->ClientNo);
            if(m_ProtocolSet.size() == 0)
                uCmdCode = CMD_TRACKPROTOCOL_DISABLE;
            //����
        }
        else
            return;
    }
    else if(nCmd == MML_SET_DBG)
    {
        //������ 2002-03-13 ��������������𱣴� ���ⵥD11728
        int nMsgOutLevel = 0;
        
		//���ⵥ��SWPD00562��2003��7��23�գ�Zou Yongsheng�޸�
		uCmdCode = CMD_SET_MSGOUT_LEVEL;
		nMsgOutLevel = nOpt;
		//�޸Ľ���

        CINIFile iniFile(GetCfgFilePath());
        int nRet = iniFile.Open();
        if(nRet != TRUE)
        {
            char szTmpBuf[256];
            sprintf(szTmpBuf, "\r\n\t\t\t\t     Errno: %d (%s)", errno, strerror(errno));
            STRING sTmpInfo = S_MML_OPEN_INI_FAIL;//"�������ļ�ʧ��"
            sTmpInfo += GetCfgFilePath();
            sTmpInfo += szTmpBuf;
            TRACE(MTS_MMLSERVER,sTmpInfo.c_str());
            return;
        }
        iniFile.WriteInt(CFG_SEC_COMMON,CFG_COMMON_KEY_MSGOUTLEVEL,nMsgOutLevel);
        iniFile.Close();
        //�޸Ľ���
    }
    else if(nCmd == MML_SET_PERF)
    {
        if(nOpt == 0)
            uCmdCode = CMD_PERFITERM_ENABLE;
        else if(nOpt == 1)
            uCmdCode = CMD_PERFITERM_DISABLE;
        else
            return;
    }
    else
        return;

    /* ������ϵͳ����Ϣ��תģ��㲥. */
    MSG_INTRA* p = new MSG_INTRA;
    p->SenderPid = GetProcessType();
    p->SenderMid = GetModuleType();
    p->ReceiverPid = PT_BROADCAST;
    p->ReceiverMid = MT_MSGDIST;
    p->AppType = pMsg->AppType;
    p->CmdCode = uCmdCode;
	p->Data[0] =(BYTE)nOpt;

    SendMsg(p);
}

/*
    ��������:��ѯϵͳ��ǰ����Ϣ�������.

    ����"level=X"�ĸ�ʽ����������.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::ProcessQueryDebugLvl(CParseResult *pParse,
                                      MSG_INTRA* pMsg)
{
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();

    Report.MakeCmdDisplay(pParse->GetCmdLine(),
                          0, 0, THISMODULE);

    char szBuf[256];
 	sprintf(szBuf, "%d", g_nMsgOutLevel);
    
	Report.MakeReportItem(S_MML_DEBUG_TITLE, szBuf);
    Report.MakeReportTail();

    /* ����MML����. */
    const char* pChar = Report.GetReport();

    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);

    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;

    SendMsg(pAns); 
    WriteLog(pParse,0,NULL,pMsg);
}

/*
    ��������:��MML�ͻ��˷���ϵͳ�İ汾��.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/

void CMMLServer::ProcessQueryVer(CParseResult *pParse,MSG_INTRA* pMsg)
{
    char szContent[512];
	CMMLReport Report(THISMODULE);
    Report.MakeReportHead();

    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, 0, THISMODULE);
    Report.AddString(STR_NEWLINE);

    Report.AddString(S_MML_IGWB_VER_TITLE); //S_MML_IGWB_VER_TITLE
	Report.AddString(STR_NEWLINE);
	int nStrLen = strlen(S_MML_IGWB_VER_TITLE);
	for(int j = 0; j < nStrLen; j++)
	{
		Report.AddString("-");
	}
	Report.AddString(STR_NEWLINE);

    //by ldf ��Ӧ���ⵥD20443 2003-03-06
	STRING sVer = S_IGATEWAY_BILL_VER;
	int nCompart = sVer.find('|');
	STRING sShortVer =  sVer.substr(0,nCompart);
	STRING sLongVer = sVer.substr(nCompart+1);
		
	if ( (pParse->GetCmdCode()) == MML_LST_VERSION )
		SNPRINTF(szContent,512,"%s = %s",S_MML_IGWB_VER,S_IGATEWAY_BILL_VER);
	if ( (pParse->GetCmdCode()) == MML_LST_VER )
		SNPRINTF(szContent,512,"%s = %s",S_MML_IGWB_VER,sShortVer.c_str());
	if (  (pParse->GetCmdCode()) == MML_LST_INTVER )
		SNPRINTF(szContent,512,"%s = %s",S_MML_IGWB_VER,sLongVer.c_str());
    //over
	
	szContent[511] = 0;
	Report.AddString(szContent);
	Report.AddString(STR_NEWLINE);
	Report.AddString(STR_NEWLINE);

    Report.MakeReportTail();

    const char* pChar = Report.GetReport();

    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);

    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;

    SendMsg(pAns); 
    WriteLog(pParse,0,NULL,pMsg);
}

/*
    ��������:ʹ��MML������MML�ͻ��˷���һ�м��ı�.

    ����:
    pTxt,�����͵��ı�.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zhouyonghui,2001-11-13,����.
*/
void CMMLServer::SendSimpleReport(CParseResult *pParse,int nErrCode,
                                  const char* pTxt,
                                  MSG_INTRA *pMsg)
{
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();
    
    nErrCode = (nErrCode > 0)? (nErrCode):(-nErrCode);
    Report.MakeCmdDisplay(pParse->GetCmdLine(),
                          0, nErrCode, THISMODULE);

    Report.AddString(pTxt);
    Report.AddString(STR_NEWLINE);
    Report.MakeReportTail();

    const char* pChar = Report.GetReport();

    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);

    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = pMsg->SenderMid;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;

    SendMsg(pAns);
}
void CMMLServer::SendSynConfReport(CParseResult *pParse, const char* pOPName,
                                   const char* pLMTInfo )
{
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();

    //by ldf 2004-09-06 OM�������� b02ͬ��
	Report.AddString("SYNC    #1");
    Report.AddString("\r\n");
    
    Report.AddString("CMD  =  ");
    //end	
    
    Report.AddString(pParse->GetCmdName());
    Report.AddString(":");
    
    char *szCmdName = new char[strlen(pOPName) + 3];
    strcpy(szCmdName, "\"");
    strncat(szCmdName, pOPName, strlen(pOPName));
    strcat(szCmdName, "\"");
    
    Report.AddString("OP");
    Report.AddString("=");
    Report.AddString(szCmdName);
    Report.AddString(",");
    delete[] szCmdName;
    
    //IP
    ACE_INET_Addr AddrClient;
    AddrClient.set(0, NTOHL(pParse->GetIP()), 1);
    int nIPLen = strlen(AddrClient.get_host_addr());
    char* szAddr = new char [nIPLen + 3];
    strcpy(szAddr, "\"");
    strncat(szAddr, AddrClient.get_host_addr(), nIPLen);
    strcat(szAddr, "\"");
    
    Report.AddString("IP");
    Report.AddString("=");
    Report.AddString(szAddr);
    Report.AddString(",");
    delete szAddr;
    
    
    //time
    time_t ltime;
    time(&ltime);
    tm *ptoday = localtime(&ltime);
    char tmp[20];
    //tmp[0] = '\"';
    sprintf(tmp, "\"%04d-%02d-%02d %02d:%02d:%02d\"",
        ptoday->tm_year + 1900, ptoday->tm_mon + 1,ptoday->tm_mday, 
        ptoday->tm_hour,ptoday->tm_min, ptoday->tm_sec);
    
    Report.AddString("Time");
    Report.AddString("=");
    Report.AddString(tmp);
    Report.AddString(",");
    
    //�ͻ�������
    //by ldf 2004-09-06 OM�������� b02ͬ��
    char *szLMTInfo = new char[60];
    sprintf(szLMTInfo,"%s---%s", m_strLocalLMTIP.c_str(),"O&M System"); 
    
    Report.AddString("Service");
    Report.AddString("=");
    Report.AddString("\"");
    Report.AddString(szLMTInfo);
    Report.AddString("\"");

    Report.AddString(";");
    Report.AddString("\r\n");
    delete szLMTInfo;
    //end    


    Report.MakeReportTail();    
    const char* pChar = Report.GetReport();
    
    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);
    
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = APP_SYN_CONFIG;
    //pAns->ClientNo    = pMsg->ClientNo;
    //pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;
    
    SendMsg(pAns);
}

void CMMLServer::SendMMLReprot(CParseResult *pParse, int nErrCode, const char* pTxt, MSG_INTRA *pMsg)
{
    CMMLReport Report(GetStringFromID(5000));
    Report.MakeReportHead();
    Report.MakeCmdDisplay(pParse->GetCmdLine(),  10640, nErrCode, "O&M");
    
    if(nErrCode == 0)
    {
        Report.AddString(STR_NEWLINE);
        Report.AddString(pTxt);
        Report.AddString(STR_NEWLINE);
        Report.AddString(STR_NEWLINE);
    }    
    
    Report.MakeReportTail();
    const char* pChar = Report.GetReport();
    
    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);
    
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = pMsg->SenderMid;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;
    
    SendMsg(pAns);
}

void CMMLServer::WriteLog(CParseResult *pParse, int nErrCode,
                          char *pTxt, MSG_INTRA *pMsg)
{
    SLog log;
    //������ 2003-02-18
    char szTmpUser[USER_NAME_LEN];
    strncpy(szTmpUser,pParse->GetUserName(),48);
    StringLRTrim(szTmpUser);
    if(strcmp(szTmpUser,"") == 0)
    {
        strcpy(szTmpUser,"system");
    }
    strncpy(log.szUserName,szTmpUser,LOG_USR_NAME_LEN);
    //����
    log.szUserName[LOG_USR_NAME_LEN-1] = 0;

    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pMsg->IPAddr;

    if(nErrCode == 0)
        log.OperResult = SLog::OPER_SUCCESS;
    else
        log.OperResult = SLog::OPER_FAIL;

    strncpy(log.szOperCmd,pParse->GetCmdName(),LOG_OPER_CMD_LEN);
    log.szOperCmd[LOG_OPER_CMD_LEN-1] = 0;

    strncpy(log.szCmdDesc,pParse->GetCmdDspName(),LOG_CMD_DESC_LEN);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

    CMsgProcessor::WriteLog(&log);
}


void CMMLServer::ProcessQueryOp(CParseResult *pParse, MSG_INTRA *pMsg)
{
    LIST<STRING> TitleList;
    LIST<STRING> ContentList;
    TitleList.push_back(S_MML_LST_OP_NAME);
    TitleList.push_back(S_MML_LST_OP_DESP);
    TitleList.push_back(S_MML_LST_OP_RIGHT);
    TitleList.push_back(S_MML_LST_OP_ONLINE);
    TitleList.push_back(S_MML_AREA);
    TitleList.push_back(S_MML_PRIVILEGE_INFO);
    TitleList.push_back(S_MML_PWD_LIMIT_TIME); 
	TitleList.push_back(S_MML_SESSION);
    int nRet = m_pRightMan->QueryOp(pParse,ContentList);
    if(nRet == ERR_SUCCESS)
    {
        if(ContentList.size())
        {
            CMMLReport Report(THISMODULE);
            Report.MakeReportHead();

            Report.MakeCmdDisplay(pParse->GetCmdLine(),
                0, 0, THISMODULE);
            Report.AddHoriTable(TitleList,ContentList);
            Report.MakeReportTail();

            const char* pChar = Report.GetReport();

            MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA;
            strcpy((char *)(pAns->Data), pChar);

            pAns->SenderPid   = GetProcessType();
            pAns->SenderMid   = GetModuleType();
            pAns->ReceiverPid = PT_OUTSIDE;
            pAns->ReceiverMid = pMsg->SenderMid;
            pAns->AppType     = pMsg->AppType;
            pAns->ClientNo    = pMsg->ClientNo;
            pAns->IPAddr      = pMsg->IPAddr;
            pAns->CmdCode     = CMD_MML;

            SendMsg(pAns);
        }
        else
        {
            SendSimpleReport(pParse,0,S_MML_QUERY_NO_RESULT,pMsg);
        }
        WriteLog(pParse,0,NULL,pMsg);
    }
    else
    {
        switch(nRet)
        {
        case ERR_NO_CRU_USER:   //�û�������
            SendSimpleReport(pParse,1,S_MML_ACC_NOT_EXIST,pMsg);
            break;
        case ERR_FAIL:
        default:
            SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
            break;
        }
        WriteLog(pParse,-1,NULL,pMsg);
    }
    return;
}

void CMMLServer::ProcessIPFilter(CParseResult *pParse, MSG_INTRA *pMsg)
{
    int nIPMode = 0;
    int nOldIpLen = 0,nNewIpLen = 0;
    LIST<UINT4> IpList;
    pParse->GetDigitVal(&nIPMode,1);
    char szIpList[172];
    ACE_INET_Addr addr;
    strncpy(szIpList,pParse->GetStringVal(2),172);
    char *pToken = NULL;
    for(pToken = strtok(szIpList,"&"); pToken != NULL; pToken = strtok(NULL,"&"))
    {
        addr.set((u_short)0,pToken);
        IpList.push_back((UINT4)addr.get_ip_address());
    }
    CMmlIniFile mml_ini(GetCfgFilePath());
    if(!mml_ini.Open())
    {
        SendSimpleReport(pParse,-1,S_MML_OPEN_INI_FAIL,pMsg);
        WriteLog(pParse,-1,NULL,pMsg);
        return;
    }
    CCommMutex::Instance()->Acquire();
    CIPFilter *pFilter = ACE_Singleton<CIPFilter,ACE_Null_Mutex>::instance();
    //���ģʽ���䣬Ҫ����ǰ��IP��ɾ��
    nOldIpLen = pFilter->m_IPList.size();
    if(nIPMode < 2)
    {
        if(pFilter->m_nMode != nIPMode)
        {
            pFilter->m_IPList.clear();
        }
        pFilter->m_IPList.splice(pFilter->m_IPList.end(),IpList);
    }
    else
    {
        pFilter->m_IPList.clear();
    }
    pFilter->m_nMode = nIPMode;
    nNewIpLen = pFilter->m_IPList.size();
    CCommMutex::Instance()->Release();
    //���´����ļ�
    printf("IpList size = %d\n",IpList.size());

    mml_ini.WriteInt(CFG_SEC_MML,CFG_AP_KEY_IP_MODE,nIPMode);
    mml_ini.WriteInt(CFG_SEC_MML,CFG_AP_IP_LIST_NUM,nNewIpLen);
    char szKey[16];
    char szSection[16];
    int i = 0;
    //ɾ��ԭ������ļ�ֵ,���ԭ�����٣�����ִ��ѭ����
    for(i = nNewIpLen + 1; i <= nOldIpLen; i++)
    {
        SNPRINTF(szKey,16,"%s%d",CFG_AP_IP_LIST_PREFIX,i);
        SNPRINTF(szSection,16,"[%s]",CFG_SEC_MML);
        mml_ini.DeleteKey(szSection,szKey);
    }
    LIST<UINT4>::const_iterator it = pFilter->m_IPList.begin();
    LIST<UINT4>::const_iterator end = pFilter->m_IPList.end();
    for(i = 1;it != end; ++it,i++)
    {
        addr.set((u_short)0,(ACE_UINT32)(*it));
        SNPRINTF(szKey,172,"%s%d",CFG_AP_IP_LIST_PREFIX,i);
        mml_ini.WriteString(CFG_SEC_MML,szKey,(char *)addr.get_host_addr());
    }
    mml_ini.Close();
    SendSimpleReport(pParse,0,S_MML_SUCCEED,pMsg);
    WriteLog(pParse,0,NULL,pMsg);
}

void CMMLServer::ProcessBreakOp(CParseResult *pParse, MSG_INTRA *pMsg)
{
    char buf[256];
    int nRet = m_pRightMan->BreakOperator(pParse);
    if(nRet == ERR_SUCCESS)
    {
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);

        SendSimpleReport(pParse,0,S_MML_SUCCEED,pMsg);
        return;
    }

    switch(nRet)
    {
    case ERR_NO_CRU_USER:   //�ʻ�������
        SendSimpleReport(pParse,1,S_MML_ACC_NOT_EXIST,pMsg);
        break;
    case ERR_FAIL:
    default:
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        break;
    }
    sprintf(buf,S_MML_OPERATION_FAIL,pParse->GetCmdCode(),nRet);
    WriteLog(pParse,1,buf,pMsg);
}

//by ldf 2004-01-30 OM����

/*
    ��������:��M2000����������1 �ļ�Ȩ�����

    �������ʧ��,����ֻ��MML�ͻ��˷��سɹ�����.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    Li Dufang,2004-01-05,����.
*/
void CMMLServer::ProcessReqID(CParseResult *pParse,MSG_INTRA* pMsg)
{

    //ͬʱֻ����һ��M2000��¼
    //����Ѿ�������M2000��¼����szRandomNum��Ϊ�գ�
    //���������M2000���ڵ�¼����ȴ�5�룬�����δ��¼��ɣ�����������M2000��¼
    //��M2000�����޸ģ�һ��M2000��Ҫ��¼���Σ�����proxy��

    static unsigned int nM2000IP = 0;

    //��һ�ε�¼���M2000 IP
    if(m_nM2000LoginTimes == 0)
    {
        nM2000IP = pParse->GetIP();
    }

    //�ڶ����յ���¼��Ϣ(����ΪM2000 Proxy)���Ƚ��Ƿ���ͬһ��M2000
    if(m_nM2000LoginTimes == 1)
    {
        if( nM2000IP != pParse->GetIP() )
        {
            //���������һ��M2000�����5����ж��Ƿ��Ѿ���M2000��¼��
            //���û������������M2000��¼����������
            ACE_OS::sleep(5);
            if(m_nM2000LoginTimes == 2)
            {
                TRACE(MTS_MMLSERVER,"There has another M2000 login.");
                SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
                return;
            }
            else
            {
                m_nM2000LoginTimes = 0;
            }
        }
    }


    char szContent[128];
    CMMLReport Report(THISMODULE);
    Report.MakeReportHead();

    Report.MakeCmdDisplay(pParse->GetCmdLine(), 0, 0, THISMODULE);
    Report.AddString(STR_NEWLINE);

    Report.AddString(S_MML_TOKEN); //S_MML_TOKEN
    Report.AddString(STR_NEWLINE);
    int nStrLen = strlen(S_MML_TOKEN);
    for(int j = 0; j < nStrLen; j++)
    {
        Report.AddString("-");
    }
    Report.AddString(STR_NEWLINE);
    //S_MML_CIPHERTXT == "%s = %s"

    //��֤�������������
    unsigned int nRandSeed = time(NULL);
    srand(nRandSeed);
    unsigned int nRandom = (rand()*9301 + 49297)%(233280+1);

    char szTmp[128];
    memset(szTmp,0,128);
    sprintf(szTmp,"%d",nRandom);

    //���������
    m_szRandomNum = szTmp;

    //sClpherTxt Ϊ �������������������1�㷨���� �Ľ��
    STRING sCipherTxt1 = MakeOriIDCipherOne(m_szRandomNum);

    char szTmp2[128];
    memset(szTmp2,0,128);
    sprintf(szTmp2,"%s",sCipherTxt1.c_str());

    //�Ѽ��ܺ���ַ���ת����16���ƣ����� A��ASCΪ65��ת��Ϊ 41H
    char * szCipherHexTmp = szTmp2;
    
    char szCipherHexTxt[128];
    memset(szCipherHexTxt,0,128);

    int nCipherLen = sCipherTxt1.length();
    for ( int i=0; i<nCipherLen; i++ )
    {
        char szTmpHex = szCipherHexTmp[i];
        sprintf(szCipherHexTxt,"%s%02X",szCipherHexTxt,szTmpHex);
    }

    //by ldf 2004-07-05 M2000�µĶԽ�
    Report.MakeReportItem(S_MML_CIPHERTXT,szCipherHexTxt,24,ALIGN_RIGHT);
    Report.MakeReportItem("Key",m_strLocalLMTIP.c_str(),24,ALIGN_RIGHT,m_strLocalLMTIP.length());
    //end
    Report.AddString(STR_NEWLINE);
    Report.AddString(STR_NEWLINE);

    Report.MakeReportTail();

    const char* pChar = Report.GetReport();

    MSG_INTRA* pAns = new(strlen(pChar) + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), pChar);

    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;

    SendMsg(pAns); 
}


void CMMLServer::ProcessCheckID(CParseResult *pParse,MSG_INTRA* pMsg)
{
    char buf[256];
    
    STRING sCipherTxt2 = pParse->GetStringVal(1);
    STRING sRandomCheckNum = GetRevertID(sCipherTxt2);
    
    int nRet = sRandomCheckNum.compare(m_szRandomNum);
    
    //����У��ɹ�����Ҫ���Ӵ��û�����Ϣ���Լ��ж��û������Ƿ��Ѿ��ﵽ���ֵ
    if(nRet == 0)
    {
        //�����û���Ϣ
        m_pRightMan->M2000Login(pParse,TRUE);

        m_nM2000LoginTimes ++;
        
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);
        
        SendSimpleReport(pParse,0,S_MML_SUCCEED,pMsg);
        return;
    }
    else
    {        
        //M2000У��ʧ�ܣ�����¼ʧ�ܺ�Ҫ��У����������
        //��¼������0
        m_szRandomNum.erase();
        m_nM2000LoginTimes = 0;

        m_pRightMan->M2000Login(pParse,FALSE);
        
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        
        sprintf(buf,S_MML_OPERATION_FAIL,pParse->GetCmdCode(),nRet);
        WriteLog(pParse,1,buf,pMsg);
    }
}

STRING CMMLServer::MakeOriIDCipherOne(STRING sRandomNum)
{
    STRING sCipherTxtTmp;

    //���ܷ���һ������sRandomNum, ��ֵ�� sCipherTxtTmp
    char szCode[128];
    memset(szCode,0,128);

    char szTmp[128];
    memset(szTmp,0,128);

    sprintf(szTmp,"%s",sRandomNum.c_str());
    
    encode(szTmp,szCode);
    sCipherTxtTmp = szCode;

    return sCipherTxtTmp;
}

STRING CMMLServer::GetRevertID( STRING sCipherTwo)
{
    STRING sRandomCheckNumTmp;
    //���ü��ܷ����������� sCipherTwo, ��ֵ�� sRandomCheckNumTmp
    //��MD5���ɱ�����IP������Ԫ��IP����ժҪ�������㷨2����Կ
 
    char szTmp[128];
    memset(szTmp,0,128);

    sprintf(szTmp,"%s",sCipherTwo.c_str());

    unsigned char szTmp2[128];
    memset(szTmp2,0,128);

    //��M2000���������ַ�������ת�������� 4B5E(4���ֽ�) ת��Ϊ 4B 5E(�����ֽ�)
    int nCipherLen = sCipherTwo.length();

    int nHexHigh = 0;
    int nHexLow = 0;

    if(nCipherLen%2 == 0)
    {
    for ( int i=0; i<nCipherLen; i++ )
    {
        int nHex = 0;

        if( (szTmp[i]>='A') && (szTmp[i]<='F') )
        {
            nHex = szTmp[i] - 'A' + 0xA ;
        }
        else 
        {
            if ('a' <= szTmp[i] && szTmp[i] <= 'f' )
            {
                nHex = szTmp[i] - 'a' + 0xA ;
            }
            else
            {
                if ('0' > szTmp[i] && szTmp[i] > '9') 
                {
                    TRACE(MTS_MMLSERVER,"The Cipher %s is not right.", sCipherTwo.c_str());
                    break;
                }
                else
                {
                    nHex = szTmp[i] - '0';
                }
            }
        }
    
        if(i%2 == 0)
        {
            nHexHigh = nHex;
        }
        else
        {
            nHexLow =nHex;
            nHex = ((nHexHigh<<4 ) & 0xf0) | (nHexLow&0xf);
            
            szTmp2[i/2] = nHex;
            //sprintf(szTmp2,"%s%02X",szTmp2,nHex);
        }

    }
    }
    else
    {
        TRACE(MTS_MMLSERVER,"The Cipher %s is not right.", sCipherTwo.c_str());
    }

    unsigned char szOut[128];
    memset(szOut,0,128);
    
    des_keys ks;

    MD5 oMd5( (unsigned char*)(m_strLocalLMTIP.c_str()) );
    unsigned char* pMd5Key = oMd5.raw_digest();
    set_key(pMd5Key, ks);
    delete[] pMd5Key;
    
    //�Կͻ��˷�������Ϣ���н���
    des_crypt((unsigned char*)szTmp2, (unsigned char*)szOut, ks, DES_DECRYPT);

    UINT4 uResult = atol( (char*)szOut);
    sprintf(szTmp,"%d",uResult);
    sRandomCheckNumTmp = szTmp;

    return sRandomCheckNumTmp;
}

/*
    ��������:�û���¼.

    ����:
    pParse,�ͻ��˲�ѯ����ķ������.
    pMsg,MML��Ӧ����Ϣ.

    �޸ļ�¼:
    zkq,2004-07-31,����.
*/

void CMMLServer::ProcessCfgLogon(CParseResult *pParse, MSG_INTRA *pMsg)
{
     char buf[256];

    /*
        �ڴ����¼����֮ǰ,����������ʽ�ı��ĸ��ͻ���:
    
      ++++++++++++++++++++++++++++
      S_IGATEWAY_BILL_VER
      ++++++++++++++++++++++++++++
    */
    STRING strReport = S_MML_SEPARATOR;
    strReport += "\r\n";
    strReport += S_IGATEWAY_BILL_VER;
    strReport += "\r\n";
    strReport += S_MML_SEPARATOR;
    strReport += "\r\n";
    
    MSG_INTRA* pAns = new(strReport.size() + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), strReport.c_str());
    
    pAns->SenderPid   = GetProcessType();
    pAns->SenderMid   = GetModuleType();
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = pMsg->AppType;
    pAns->ClientNo    = pMsg->ClientNo;
    pAns->IPAddr      = pMsg->IPAddr;
    pAns->CmdCode     = CMD_MML;
    
    SendMsg(pAns); 
    
    int nRet = m_pRightMan->CfgLogin(pParse);
    if(nRet == ERR_SUCCESS)
    {
        sprintf(buf,S_MML_OPERATION_SUCC,pParse->GetCmdCode());
        WriteLog(pParse,0,buf,pMsg);

        SendSimpleReport(pParse,0,S_MML_LOGIN_SUCCEED,pMsg);
        return;
    }
    
    switch(nRet)
    {
    case ERR_TOO_MUCH_USER: //��¼�û���̫��
        SendSimpleReport(pParse,1,S_MML_TOO_MANY_USER,pMsg);
        break;
    case ERR_MULTI_LOGIN:   //�ظ���¼
        SendSimpleReport(pParse,1,S_MML_ALREADY_LGGIN,pMsg);
        break;
    case ERR_NO_CRU_USER:   //�ʻ�������
        SendSimpleReport(pParse,1,S_MML_ACC_NOT_EXIST,pMsg);
        break;
    case ERR_PASSWORD_ERR:  //�������
        SendSimpleReport(pParse,1,S_MML_PWD_INCORRECT,pMsg);
        break;
	case ERR_PWD_OUTDATED:  //�������
		SendSimpleReport(pParse,0,S_MML_PWD_OUTDATED,pMsg);
		break;
    case ERR_FAIL:
    default:
        SendSimpleReport(pParse,1,S_MML_FAIL,pMsg);
        break;
    }

    sprintf(buf,S_MML_OPERATION_FAIL,pParse->GetCmdCode(),nRet);
    WriteLog(pParse,1,buf,pMsg);
}
