/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:mml_right_man.h
    ������Ϣ:
    
    ����CRightMan��Ķ���,����ʵ���û���¼��ע����Ȩ�޹���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/
#ifndef _MML_RIGHT_MAN_H
#define _MML_RIGHT_MAN_H

#include "../include/precom.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "mml_persist_usr.h"
#include "mml_ini_file.h"
//������ 2003-03-16 add for CCS
typedef struct _PrivilegeInfo
{
    SET<BYTE> CmdSet;
    tm tmStart;
    tm tmEnd;
    int nLimitType;
} SPrivilegeInfo;

typedef struct _CMDDescription
{
    STRING strCmdName;            //������
    STRING strCmdDes;             //��������
    BYTE yCmdSetId;               //�������ܼ�
    int nCmd;                     //������
} SCMDDescription;

class CLoginUser
{
    friend class CRightMan;
protected:
    char m_szUserName[USER_NAME_LEN];           //�û���
    char m_szUserFullName[USER_FULL_NAEM_LEN];  //�û�����
    char m_szArea[AREA_DESCRIPTION_LEN];        //��������
    BYTE m_yUserLevel;                          //�û�����
    UINT2 m_uStationNo;                         //��¼̨��
    UINT2 m_uStationType;                       //����̨����
    UINT4 m_uIP;                                //��¼�ն�IP��ַ
    time_t m_uLoginTime;                        //�û���¼ʱ��
    int   m_uPwdTimeLimit;                      //����ʹ������ 
	UINT4 m_uCount;								//�û����Ƶô���.

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

    /* ���ʳ־ò���û���Ϣ */
    CPersistUser* m_pPersistUser;
    /*
        ��¼�û��б�.
        ģ����ĵ�һ������UINT4Ϊͨ�Ų�����
        <����̨��>+<����̨����>�����.
    */
    MAP<UINT4,CLoginUser*> m_LoginUsers;
	//������ 2002-12-07 MML ��½ʧ�ܴ���̫��ĸ澯
	typedef MAP<UINT4,int,LESS<UINT4> > BLACKMAP;
	BLACKMAP m_BlackUser;
	int m_nFailMaxTimes;
    //������ 2002-12-07
    /* ��������¼���û��� */
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
    /* ���ӡ�ɾ���û� */
    int AddUser(CParseResult* pParse);
    int DelUser(CParseResult* pParse);

    /* �û���¼��ע�� */
    int Login(CParseResult* pParse);
    int Logout(CParseResult* pParse);

    /* �޸��û����� */
    int Password(CParseResult* pParse);

    /* ����û��Ƿ���ִ�������Ȩ�� */
    int CheckRight(CParseResult* pParse);

    int QueryUserInfo(LIST<STRING>& UserInfo, CParseResult *pParse);

    int ModifyUserInfo(CParseResult* pParse);

    //by ldf 2004-01-30 OM����
    void M2000Login(CParseResult* pParse,BOOL bM2000Login);
    //end

    //by zkq 2004-07-31 SWPD07057
    int CfgLogin(CParseResult* pParse);
    //end
};

#endif
