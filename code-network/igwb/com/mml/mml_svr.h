/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:mml_svr.h
    描述信息:

    包含CMMLServer类的定义.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
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
    BYTE    yReceiverPid;   //接收模块所在的进程号
    BYTE    yReceiverMid;   //接收模块的模块号
    UINT4   uMinCmdCode;    //最小命令码
    UINT4   uMaxCmdCode;    //最大命令码
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
    //张玉新 2003-02-15 问题单20356
	SET<UINT2> m_ProtocolSet;
	SET<UINT2> m_WorkflowSet;
	//增加计数集合完毕
	

    /* 基准定时器ID */
    long m_lTimerIDBase;

    /* 重载基础类的方法 */
    int InitInstance(void);
    void ExitInstance(void);
    virtual void ParseMsg(MSG_INTRA* pMsg);

    /* 消息处理函数 */
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

    //by ldf 2004-01-05 OM整改
    void ProcessReqID(CParseResult *pParse,MSG_INTRA* pMsg);
    void ProcessCheckID(CParseResult *pParse,MSG_INTRA* pMsg);
    STRING MakeOriIDCipherOne(STRING sRandomNum);
    STRING GetRevertID(STRING sCipherTwo);
    BOOL DownLoadSoft(CParseResult* pParse);    

    STRING m_szRandomNum;
    STRING m_strLocalLMTIP;
    //BOOL   m_bM2000Login;
    int m_nM2000LoginTimes; //M2000登录次数
    //end
    char m_szLocalIP[MAX_HOST_LEN];
    char m_szFTPServerIP[MAX_HOST_LEN];
    char m_szFTPUserName[USER_NAME_LEN];
    char m_szFTPPassword[PASSWD_LEN];
    char m_szLocalPath[MAX_PATH];    

public:
    void WriteLog(CParseResult* pParse,int nErrCode,char* pTxt,MSG_INTRA* pMsg);
    void AddCmdRouter();
    /* 使用MML报文发送错误消息 */
    void SendSimpleReport(CParseResult *pParse,int nErrCode,
            const char* pTxt,MSG_INTRA* pMsg);
    void SendMMLReprot(CParseResult *pParse, int nErrCode, const char* pTxt, MSG_INTRA *pMsg);
    void SendSynConfReport(CParseResult *pParse, const char* pOPName,
        const char* pLMTInfo);        

    CMMLServer();
    virtual ~CMMLServer();

    /* 根据命令码获取命令接收模块 */
    CmdRouter* GetMsgTarget(UINT2 uCmdCode);

    /* 重载基础类的方法 */
    virtual MODULE_TYPE GetModuleType();
};

/*
    下面定义的全局变量用于进行语法解析.
*/

#endif
