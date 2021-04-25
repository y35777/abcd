/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:mml_svr.h
    ������Ϣ:

    ����CMMLServer��Ķ���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/

#ifndef _MML_SERVER_H
#define _MML_SERVER_H

#include "../include/precom.h"
#include "../include/frame.h"

#include "mml_right_man.h"
#include "mml_persist_cmd.h"

#ifdef _PLATFORM_WIN32
#define S_DLD_SOFTWARE              "d:\\other\\dld_soft"
#else
#define S_DLD_SOFTWARE              "/var/other/dld_soft"
#endif

#ifdef _PLATFORM_WIN32
#define DELIMIT_CHAR    '\\'
#else
#define DELIMIT_CHAR    '/'
#endif

#define MAX_HOST_LEN 24

struct CmdRouter
{
    BYTE    yReceiverPid;   //����ģ�����ڵĽ��̺�
    BYTE    yReceiverMid;   //����ģ���ģ���
    UINT4   uMinCmdCode;    //��С������
    UINT4   uMaxCmdCode;    //���������
};

class CMMLServer:public CWorkModule
{
protected:
    //by zkq 2004-08-02 SWPD07057
	void ProcessCfgLogon(CParseResult *pParse,MSG_INTRA* pMsg);
    //end
	void ProcessBreakOp(CParseResult *pParse, MSG_INTRA *pMsg);
	void ProcessIPFilter(CParseResult *pParse, MSG_INTRA *pMsg);
	void ProcessQueryOp(CParseResult *pParse, MSG_INTRA *pMsg);
    CRightMan*  m_pRightMan;
    VECTOR<CmdRouter*> m_CmdRouterArray;
    //������ 2003-02-15 ���ⵥ20356
	SET<UINT2> m_ProtocolSet;
	SET<UINT2> m_WorkflowSet;
	//���Ӽ����������
	

    /* ��׼��ʱ��ID */
    long m_lTimerIDBase;

    /* ���ػ�����ķ��� */
    int InitInstance(void);
    void ExitInstance(void);
    virtual void ParseMsg(MSG_INTRA* pMsg);

    /* ��Ϣ������ */
    void ProcessQueryVer(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessQueryDebugLvl(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessSetFlag(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessSetFlag_impl(int nCmd,int nOpt,MSG_INTRA *pMsg);
    void ProcessLogout(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessLogon(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessQueryUsrInfo(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessModifyPwd(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessModifyUsrInfo(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessDelUser(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessAddUser(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessShakeHands(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessMsg(CParseResult* pParse,MSG_INTRA* pMsg);
    void ProcessDldSoftware(CParseResult* pParse,MSG_INTRA* pMsg); 

    //by ldf 2004-01-05 OM����
    void ProcessReqID(CParseResult *pParse,MSG_INTRA* pMsg);
    void ProcessCheckID(CParseResult *pParse,MSG_INTRA* pMsg);
    STRING MakeOriIDCipherOne(STRING sRandomNum);
    STRING GetRevertID(STRING sCipherTwo);
    BOOL DownLoadSoft(CParseResult* pParse);    

    STRING m_szRandomNum;
    STRING m_strLocalLMTIP;
    //BOOL   m_bM2000Login;
    int m_nM2000LoginTimes; //M2000��¼����
    //end
    char m_szLocalIP[MAX_HOST_LEN];
    char m_szFTPServerIP[MAX_HOST_LEN];
    char m_szFTPUserName[USER_NAME_LEN];
    char m_szFTPPassword[PASSWD_LEN];
    char m_szLocalPath[MAX_PATH];    

public:
    void WriteLog(CParseResult* pParse,int nErrCode,char* pTxt,MSG_INTRA* pMsg);
    void AddCmdRouter();
    /* ʹ��MML���ķ��ʹ�����Ϣ */
    void SendSimpleReport(CParseResult *pParse,int nErrCode,
            const char* pTxt,MSG_INTRA* pMsg);
    void SendMMLReprot(CParseResult *pParse, int nErrCode, const char* pTxt, MSG_INTRA *pMsg);
    void SendSynConfReport(CParseResult *pParse, const char* pOPName,
        const char* pLMTInfo);        

    CMMLServer();
    virtual ~CMMLServer();

    /* �����������ȡ�������ģ�� */
    CmdRouter* GetMsgTarget(UINT2 uCmdCode);

    /* ���ػ�����ķ��� */
    virtual MODULE_TYPE GetModuleType();
};

/*
    ���涨���ȫ�ֱ������ڽ����﷨����.
*/

#endif
