/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:mml_right_man.h
    描述信息:
    
    包含CRightMan类的定义,该类实现用户登录、注销和权限管理.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
*/
#ifndef _MML_RIGHT_MAN_H
#define _MML_RIGHT_MAN_H

#include "../include/precom.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "mml_persist_usr.h"
#include "mml_ini_file.h"
//张玉新 2003-03-16 add for CCS
typedef struct _PrivilegeInfo
{
    SET<BYTE> CmdSet;
    tm tmStart;
    tm tmEnd;
    int nLimitType;
} SPrivilegeInfo;

typedef struct _CMDDescription
{
    STRING strCmdName;            //命令名
    STRING strCmdDes;             //命令描述
    BYTE yCmdSetId;               //所属功能集
    int nCmd;                     //命令码
} SCMDDescription;

class CLoginUser
{
    friend class CRightMan;
protected:
    char m_szUserName[USER_NAME_LEN];           //用户名
    char m_szUserFullName[USER_FULL_NAEM_LEN];  //用户描述
    char m_szArea[AREA_DESCRIPTION_LEN];        //地理区域
    BYTE m_yUserLevel;                          //用户级别
    UINT2 m_uStationNo;                         //登录台号
    UINT2 m_uStationType;                       //工作台类型
    UINT4 m_uIP;                                //登录终端IP地址
    time_t m_uLoginTime;                        //用户登录时间
    int   m_uPwdTimeLimit;                      //密码使用期限 
	UINT4 m_uCount;								//用户限制得次数.

public:
    CLoginUser(const SUserInfo *pUser,BYTE yLevel,UINT2 uWS,UINT2 uType,UINT4 uIP,
               time_t uLoginTime, UINT4 uCount);    
    virtual ~CLoginUser();
    const char* GetUserName();
    char* GetUserFullName();
    const char* GetArea();
    BYTE GetUserLevel();
    UINT2 GetStationNo();
    UINT2 GetStationType();
    UINT4 GetIP();
    time_t GetLoginTime();
    int    GetPwdTimeLimit();
	UINT4  GetCount();
};
#define INTMAP MAP<int,int,LESS<int> >

class CRightMan;
class CCommandAdministrator  
{
public:
	int DeleteUserInfo(const char *szUserName);
	int QueryOpTm(CParseResult *pParse);
	const SET<BYTE> *QueryUserPrivilegeSet(STRING &strUser);
	CCommandAdministrator();
	virtual ~CCommandAdministrator();
    int Init(CRightMan *pRightMan);
    int QueryRunCmd(CParseResult *pParse);
	int QueryCmdOfGroup(CParseResult *pParse);
	int DelCmdFromGroup(CParseResult *pParse);
	int AddCmdToGroup(CParseResult *pParse);
	int SetTimeLimitForOp(CParseResult *pParse);
	int SetCommandForOp(CParseResult *pParse);
    int CheckPrivilege(CParseResult *pParse);
	int PrintSetToStr(char *pValue,int nLen, SET<BYTE> PrintedSet, const char *pSec = "&");
    int QueryUserPrivilege(STRING &szUserName,char *strPI, int nLen);
	MAP<STRING,SPrivilegeInfo>& GetUserPrivilegeMap();
	
protected:
	void WriteLog(CParseResult *pParse, int nErr, char *pTxt);
	int m_nPageLimit;
	void SendToClient(CParseResult *pParse);
	int AddCmdToIni(CMmlIniFile &ini,const char *szSection,int nCmdCnt,SCMDDescription &CmdDes);
	int AnalysisDatePara(STRING &sDate, tm &tmDate);
    int AnalysisTime(const char *pszTime, tm& Time);
	int AnalysisDate(const char *pszDate, tm &Date);
	int AnalysisCommandGroup(const char *pCmdGrp,SET<BYTE> &CmdGrpSet);
	int GetEnumValue(CParseResult *pParse, int nID, int nMaxLen, INTMAP &EnumMap);
    int MoveCmdBetweenSet(int nOutSet, int nInSet, const char *szCmdArray,LIST<STRING> &NoteList);
    int StatisticCmdCntOfSet(int nSetID);
    int WriteSetToStr(int nSetId, char *Buffer,int nLen);
    int GenerateRunCmdReportBody(SPrivilegeInfo &PI,int &nRecCnt);
    void GenerateCommonReport(CParseResult *pParse, const char *pszTxt, int nRes);
    void SendReportToClient(LIST<STRING> &NoteList,CParseResult *pParse);
	
    MAP<STRING,SPrivilegeInfo> m_UserPrivilegeMap;
    MAP<UINT4,SCMDDescription> m_CmdMap;
    MAP<UINT4,STRING> m_DefaultCmdMap;
    MAP<UINT4,SCMDDescription> m_CmdMap0;
    MAP<STRING, UINT4> m_NameToIdCmdMap;
    MAP<BYTE,STRING> m_CmdGrpDes;

    CMMLReport m_Report;
    char m_szCatch[128];
    STRING m_strCmdPath;
    STRING m_strUserPath;
    STRING m_strError; 
    CRightMan *m_pRight;
    int m_nSetNum; 
};

class CRightMan
{
    friend class CCommandAdministrator;
protected:
	BYTE PrivilegeEqual(const char *szPI,const SUserInfo &usr);
	BYTE AreaEqual(const STRING &strArea, const SUserInfo &usr);

    /* 访问持久层的用户信息 */
    CPersistUser* m_pPersistUser;
    /*
        登录用户列表.
        模板类的第一个参数UINT4为通信层分配的
        <工作台号>+<工作台类型>的组合.
    */
    MAP<UINT4,CLoginUser*> m_LoginUsers;
	//张玉新 2002-12-07 MML 登陆失败次数太多的告警
	typedef MAP<UINT4,int,LESS<UINT4> > BLACKMAP;
	BLACKMAP m_BlackUser;
	int m_nFailMaxTimes;
    //张玉新 2002-12-07
    /* 最大允许登录的用户数 */
    UINT2 m_uMaxUserNum;
    CCommandAdministrator m_CmdAdmin;

public:
	int BreakOperator(CParseResult *pParse);
	int QueryOp(CParseResult *pParse, LIST<STRING> &ResultList);
	void ProcessPrivilegeCommand(CParseResult *pParse, MSG_INTRA *pMsg);
    CRightMan();
    virtual ~CRightMan();

    BOOL Init();
    BOOL UserIsExistent(const char *szUser);
    /* 增加、删除用户 */
    int AddUser(CParseResult* pParse);
    int DelUser(CParseResult* pParse);

    /* 用户登录、注销 */
    int Login(CParseResult* pParse);
    int Logout(CParseResult* pParse);

    /* 修改用户密码 */
    int Password(CParseResult* pParse);

    /* 检查用户是否有执行命令的权限 */
    int CheckRight(CParseResult* pParse);

    int QueryUserInfo(LIST<STRING>& UserInfo, CParseResult *pParse);

    int ModifyUserInfo(CParseResult* pParse);

    //by ldf 2004-01-30 OM整改
    void M2000Login(CParseResult* pParse,BOOL bM2000Login);
    //end

    //by zkq 2004-07-31 SWPD07057
    int CfgLogin(CParseResult* pParse);
    //end
};

#endif
