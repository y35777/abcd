/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:mml_right_man.cpp
    ������Ϣ:

    ����CRightMan���ʵ��,����ʵ���û���¼��ע����Ȩ�޹���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/

#include "mml_right_man.h"
#include "../include/toolbox.h"
#include "../include/resource.h"
#include "resource.h"
#include "../frame/msg_proc.h"  //������ 2002-09-18
#include "ace/INET_Addr.h"
#include <iostream>
#include <iomanip>
#include <sstream>
/*
    ������CLoginUser���ʵ��,�����װ���Ѿ���¼���û���Ϣ.
*/
CLoginUser::CLoginUser(const SUserInfo *pUser,BYTE yLevel,UINT2 uWS,UINT2 uType,UINT4 uIP,
                       time_t uLoginTime, UINT4 uCount)                       
{
    strncpy(m_szUserName,pUser->szName,USER_NAME_LEN);
    m_szUserName[USER_NAME_LEN - 1] = 0;

    strncpy(m_szUserFullName,pUser->szFullName,USER_FULL_NAEM_LEN);
    m_szUserFullName[USER_FULL_NAEM_LEN - 1] = 0;

    strncpy(m_szArea,pUser->szArea,AREA_DESCRIPTION_LEN);
    m_szArea[AREA_DESCRIPTION_LEN-1] = 0;

    m_yUserLevel = yLevel;
    m_uStationNo = uWS;
    m_uStationType = uType;
    m_uIP = uIP;
    m_uLoginTime = uLoginTime;
    m_uPwdTimeLimit = pUser->uPwdTimeLimit; 
	m_uCount = pUser->uCount;
}
CLoginUser::~CLoginUser()
{
}
const char* CLoginUser::GetArea()
{
    if(strcmp(m_szArea,"") == 0)
        strcpy(m_szArea,"-");
    return m_szArea;
}
const char* CLoginUser::GetUserName()
{
    return m_szUserName;
}

BYTE CLoginUser::GetUserLevel()
{
    return m_yUserLevel;
}

UINT2 CLoginUser::GetStationNo()
{
    return m_uStationNo;
}

UINT2 CLoginUser::GetStationType()
{
    return m_uStationType;
}

UINT4 CLoginUser::GetIP()
{
    return m_uIP;
}

char* CLoginUser::GetUserFullName()
{
    return m_szUserFullName;
}
time_t CLoginUser::GetLoginTime()
{
    return m_uLoginTime;
}
int CLoginUser::GetPwdTimeLimit()
{
    return m_uPwdTimeLimit;
}
UINT4  CLoginUser::GetCount()
{
	return m_uCount;
}
/*
    ������CRightMan���ʵ��.
*/

CRightMan::CRightMan()
{
    m_pPersistUser = NULL;
    m_uMaxUserNum = 0;
}

/*
    ��������:��������,ɾ����¼�û���¼.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
CRightMan::~CRightMan()
{
    CPersistUser::Destroy();
    m_pPersistUser = NULL;

    MAP<UINT4,CLoginUser*>::iterator i = m_LoginUsers.begin(),
                                     e = m_LoginUsers.end();

    while(i != e)
    {
        delete (*i).second;
        i++;
    }

    m_LoginUsers.clear();
}

/*
    ��������:�������û�.

    1.��pParse������ȡ���û���,�û�����,����,ȷ������.
    2.����m_pPersistUser�����AddUser()����.
    3.ϵͳ��ֻ����һ������Ա�ʻ�,����������ͨ�����û�.

    MML����ṹ:

    [ADD OP]
    macro_code= 101
    macro_name= ���Ӳ���Ա�ʺ�
    para_num=4
    must_give=1
    1= op(�ʺ�)
    2= desc(����)
    3= new(������)
    4= cfm(ȷ������)

    ����:
    pParse,MML��������Ľ��.

    ����:
    1.��ȷ�����벻ƥ��,����ERR_PASSWORD_ERR.
    2.���������m_pPersistUser�����AddUser()��������.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CRightMan::AddUser(CParseResult* pParse)
{
    if(strlen(pParse->GetStringVal(1)) == 0)
    {
        return ERR_ACCOUNT_IS_EMPTY;
    }

    if(strcmp(pParse->GetStringVal(3),pParse->GetStringVal(4)))
    {
        return ERR_PASSWORD_ERR;
    }

    SUserInfo usr;
    memset(&usr,0,sizeof(SUserInfo));
    usr.yLevel = UL_OPER;

    strncpy(usr.szName,pParse->GetStringVal(1),USER_NAME_LEN);
    strncpy(usr.szFullName,pParse->GetStringVal(2),USER_FULL_NAEM_LEN);
    strncpy(usr.szPassword,pParse->GetStringVal(3),PASSWD_LEN * 2);
    strncpy(usr.szArea,pParse->GetStringVal(5),AREA_DESCRIPTION_LEN);
    usr.szName[USER_NAME_LEN - 1] = 0;
    usr.uPwdTimeLimit = atoi(pParse->GetStringVal(6));
	pParse->GetDigitVal((int*)&(usr.uCount), 7);
    if(usr.uPwdTimeLimit > PWD_TIME_LIMIT_MAX || usr.uPwdTimeLimit < 0)
        usr.uPwdTimeLimit = PWD_TIME_LIMIT_MAX;
    
    //by ldf 2004-01-30 OM ����
    if(ACE_OS::strcasecmp(usr.szName,cc08) == 0 )
    {
        //��������cc08�û� �� M2000��ͷ���û�
        return ERR_NOT_ENOUGHT_RIGHT;
    }
    if( ACE_OS::strncasecmp(usr.szName, M2000, 4) == 0 )
    {
        //��������cc08�û� �� M2000��ͷ���û�
        return ERR_ADD_M2000_ACCOUNT;
    }
    //end

    usr.szFullName[USER_FULL_NAEM_LEN - 1] = 0;
    usr.szPassword[(PASSWD_LEN * 2) - 1] = 0;
    usr.szArea[AREA_DESCRIPTION_LEN-1] = 0;
    StringLRTrim(usr.szName);
    StringLRTrim(usr.szFullName);
    StringLRTrim(usr.szArea);
    if(usr.szName[0] == 0)
    {
        strcpy(usr.szName,"-");
    }
    if(usr.szFullName[0] == 0)
    {
        strcpy(usr.szFullName,"-");
    }
    if(usr.szArea[0] == 0)
    {
        strcpy(usr.szArea,"-");
    }    
    return m_pPersistUser->AddUser(&usr);
}

/*
    ��������:ɾ���û�.

    1.��pParse������ȡ���û���.
    2.����m_pPersistUser�����DelUser()����.

    MML����ṹ:
    
    [RMV OP]
    macro_code=102
    macro_name=ɾ������Ա�ʺ�
    para_num=1
    must_give=1
    1=op(�ʺ�)
      
    ����:
    pParse,MML��������Ľ��.

    ����:m_pPersistUser�����DelUser()�����ķ���ֵ.
*/
int CRightMan::DelUser(CParseResult* pParse)
{
    //�������ⵥ20370�޸�Ϊ�����û�����ɾ��
    STRING strOpName = pParse->GetStringVal(1);
    MAP<UINT4,CLoginUser*>::iterator it;
    MAP<UINT4,CLoginUser*>::iterator et = m_LoginUsers.end();
    CLoginUser *pUser = NULL;
    for(it = m_LoginUsers.begin(); it != et; it++)
    {
        pUser = it->second;
        if(ACE_OS::strcasecmp(pUser->GetUserName(),strOpName.c_str()) == 0)
        {
            return ERR_USER_IN_LINE;
        }
    }
    //�޸Ľ���
    int nRet = m_pPersistUser->DelUser(strOpName.c_str());
    if(nRet == ERR_SUCCESS)
    {
        //ͬ��������������е���Ӧ�û���Ȩ����Ϣ
        //û��ʧ�ܵ����
        m_CmdAdmin.DeleteUserInfo(strOpName.c_str());
    }
    return nRet;
}

/*
    ��������:�û���¼.

    ������������:
    1.��ͬ���͵��ն�ֻ������ͬһ��IP�ϵ�¼һ��.
    2.��¼�û������ó��������.

    MML����ṹ:
    
    [LGI]
    macro_code=106
    IsNotShow=1
    macro_name=����Ա��¼
    para_num=2
    must_give=1
    1=op (�ʺ�)
    2=pwd (����)

    ����:
    pParse,MML��������Ľ��.

    ����:
    1.����ͬ���͵��ն��Ѿ���ͬһ��IP�ϵ�¼,����ERR_MULTI_LOGIN.
    2.����¼�û������������,����ERR_TOO_MUCH_USER.
    3.����m_pPersistUser�����Login()�����ķ���ֵ.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CRightMan::Login(CParseResult* pParse)
{
    /*
        ����㱣֤MML�ͻ��˵�̨��ʼ�շ�����С���õ�.
        �����ڵ�¼��¼�в����Ƿ����ظ���¼,������,���ʾ
        ��¼��¼��������һ����¼����Ч��,�Ӷ����Լ�����¼.
    */
    UINT4 nIndex = MAKEUINT4(pParse->GetWorkStationNo(),
                             pParse->GetWorkStationType());

    BYTE yLevel = 0;
    int nRet = m_pPersistUser->Login(pParse->GetStringVal(1), pParse->GetStringVal(2), yLevel);
    if(nRet == ERR_SUCCESS || nRet == ERR_PWD_OUTDATED)
    {
		SUserInfo* pUser = m_pPersistUser->FindUser(pParse->GetStringVal(1));
		UINT4 nMaxNum = 0; 

        //�����¼��Ϊ��ͨ�û��ҵ�¼���û��������ֵ
        if(m_LoginUsers.size() >= m_uMaxUserNum && yLevel == UL_OPER)
        {
            return ERR_TOO_MUCH_USER;        
        }

		//��ʼ���û�����Ȩ�ޱ�
		SPrivilegeInfo PI;
		CMmlIniFile mmlINI;
		char szPath[MAX_PATH];
		char szLimitType[256];
		char szBegin[32];
		char szEnd[32];
		char szCurrent[32];

		//��igwb.ini�ļ�
		mmlINI.Open(GetCfgFilePath());			

		//�õ�igwb.ini�ļ���[MML]����PersistUser����ֵ������ע���û������ļ�userinfo.dat�Ĵ��·�������浽szPath��
		mmlINI.GetString(CFG_SEC_MML,               
			CFG_MML_PERSIST_USER,     
			CFG_MML_PERSIST_USER_DEF, 
			szPath,                   
			MAX_PATH);   
		
		//�õ�user.ini�ļ���·��
		CreateDeepDir(szPath);
		strcat(szPath,"/user.ini");
		mmlINI.Close();

		//����򲻿�user.ini�ļ����򷵻�ʧ��
		if(!mmlINI.Open(szPath))
		{
			FILE *pFile = fopen(szPath,"wb+");
			if(pFile == NULL)
			{
				TRACE(MTS_MMLSERVER,S_MML_CMD_ADM_CRE_USER_FILE,strerror(errno));
				return ERR_FAIL;
			}
			fclose(pFile);
		}
		mmlINI.Open(szPath);

		//�õ�user.ini�ļ����û���ΪpParse->GetStringVal(1)��pLimitType������ֵ
		mmlINI.GetString(pParse->GetStringVal(1),
						CFG_MML_LIMIT_TYPE,
						"-1",
						szLimitType,
						256);

		int nLimitType = atoi(szLimitType);
		
		//���ʱ�����ʹ��ڣ������ʱ��У��
		if (nLimitType >= 0)
		{
			time_t tCurrent = 0;
            time(&tCurrent);
            tm tmNow = *localtime(&tCurrent);

			//����û�ʱ��Ϊ�������ͣ���õ���ʼ���ں���ֹ����ֵ
			if (nLimitType == 0)
			{
				mmlINI.GetString(pParse->GetStringVal(1),
					CFG_MML_BEGIN_DATE,
					"0000-00-00",
					szBegin,
					32);
				mmlINI.GetString(pParse->GetStringVal(1),
					CFG_MML_END_DATE,
					"9999-12-31",
					szEnd,
					32);
                strftime(szCurrent,32,"%Y-%m-%d",&tmNow);
			}
			//����û�ʱ��Ϊʱ�����ͣ���õ���ʼʱ�̺���ֹʱ��ֵ
			else if(nLimitType > 0)
			{
				mmlINI.GetString(pParse->GetStringVal(1),
					CFG_MML_BEGIN_TIME,
					"00:00:00",
					szBegin,
					32);
				mmlINI.GetString(pParse->GetStringVal(1),
					CFG_MML_END_TIME,
					"23:59:59",
					szEnd,
					32);
				strftime(szCurrent,32,"%H:%M:%S",&tmNow);
			}
			
			//���鵱ǰʱ���Ƿ񳬳��û�ʱ��
			if(strcmp(szBegin,szCurrent) > 0 || strcmp(szEnd,szCurrent) < 0) 
			{
				return ERR_EXPIRE_USER;
			}
		}
		
		CLoginUser *pLoginUser = NULL;
		MAP<UINT4,CLoginUser*>::iterator it1 = m_LoginUsers.begin();
		MAP<UINT4,CLoginUser*>::iterator et1 = m_LoginUsers.end();

		for (; it1 != et1; ++ it1)
		{
			pLoginUser = it1->second;
			if (strncmp(pUser->szName, pLoginUser->GetUserName(), strlen(pLoginUser->GetUserName()) ) == 0)
			{
				nMaxNum++;
			}
		}

        if(nMaxNum >= pUser->uCount && yLevel == UL_OPER)
        {
            return ERR_TOO_MUCH_USER;        
        }

        //�����¼��Ϊ�����û��ҵ�¼���û��������ֵ
        if(m_LoginUsers.size() >= m_uMaxUserNum && yLevel == UL_ADMIN)
        {
            CLoginUser *pLoginUser = NULL;
            MAP<UINT4,CLoginUser*>::iterator it = m_LoginUsers.begin();
            MAP<UINT4,CLoginUser*>::iterator et = m_LoginUsers.end();
            time_t uTmpOldTime = -1;
            UINT4 nIndex = -1;
            int nTmp = 1;
            //�ҳ������¼��UL_OPER�û�
            for(it; it != et; it ++)
            {
                pLoginUser = it->second;
                if(pLoginUser->GetUserLevel() != UL_ADMIN)
                {
                    if(nTmp == 1) //��¼�µ�һ��UL_OPER�û��ĵ�¼ʱ��
                    {
                        uTmpOldTime = pLoginUser->GetLoginTime();
                        nIndex = MAKEUINT4(pLoginUser->GetStationNo(),
                        pLoginUser->GetStationType()); 
                        nTmp ++;
                    }
                    else   //�ҳ������¼��UL_OPER�û�
                    {
                        if(uTmpOldTime > pLoginUser->GetLoginTime())
                        {
                            nIndex = MAKEUINT4(pLoginUser->GetStationNo(),
                            pLoginUser->GetStationType()); 
                            uTmpOldTime = pLoginUser->GetLoginTime();
                        }
                    }
                }                
            }
            if(nIndex == -1) //���û����ͨ�û������ص�¼�û�����Ĵ���
            {
                return ERR_TOO_MUCH_USER; 
            }
            else   //���򣬼��������¼���û�
            {
                //���ͶϿ���ʾ��Ϣ
                CMMLReport Report(MTS_MMLSERVER);
                Report.MakeReportHead();                
                
                Report.MakeCmdDisplay(pParse->GetCmdLine(),0, 0, MTS_MMLSERVER);
                Report.AddString(S_MML_SUCCEED);
                Report.AddString(STR_NEWLINE);
                Report.MakeReportTail();
                
                const char* pChar = Report.GetReport();
                
                MSG_INTRA* pMsg = new(strlen(pChar) + 1) MSG_INTRA; 
                strcpy((char *)(pMsg->Data), pChar);
                
                pMsg->SenderPid   = PT_OM;
                pMsg->SenderMid   = MT_MMLSERVER;
                pMsg->ReceiverPid = PT_OUTSIDE;
                pMsg->ReceiverMid = MT_MMLDEBUG;
                pMsg->AppType     = APP_MML_NORM;
                pMsg->ClientNo    = m_LoginUsers[nIndex]->GetStationNo();
                pMsg->IPAddr      = m_LoginUsers[nIndex]->GetIP();
                pMsg->CmdCode     = CMD_MML;                
                CMsgProcessor::SendMsg(pMsg);
                
                //�Ͽ����û�������
                MSG_INTRA* pAns = new MSG_INTRA;
                pAns->SenderPid   = PT_OM;
                pAns->SenderMid   = MT_MMLSERVER;
                pAns->ReceiverPid = PT_OUTSIDE;
                pAns->ReceiverMid = MT_MMLDEBUG;
                pAns->AppType     = APP_MML_NORM;
                pAns->ClientNo    = m_LoginUsers[nIndex]->GetStationNo();
                pAns->IPAddr      = m_LoginUsers[nIndex]->GetIP();
                pAns->CmdCode     = CMD_BREAK_OPERATOR;
                strncpy(pAns->UserName, m_LoginUsers[nIndex]->GetUserName(), USER_NAME_LEN);
                CMsgProcessor::SendMsg(pAns);
                
                delete m_LoginUsers[nIndex];
                m_LoginUsers.erase(nIndex);
            }
        }
        
        /*
            ��¼�ɹ�,����CLoginUser����,��׷�ӵ�m_LoginUsers��.
            ���Ѿ�����<����̨��>+<����̨����>��ͬ�ļ�¼,����
            ԭ�ȵļ�¼.
        */
        SUserInfo usr;
        m_pPersistUser->GetUserInfo(pParse->GetStringVal(1),&usr);
        time_t tNow;
        tNow = time(NULL);
        CLoginUser* p = NULL;
        p = new CLoginUser(&usr,
            yLevel,
            pParse->GetWorkStationNo(),
            pParse->GetWorkStationType(),
            pParse->GetIP(),
            tNow, usr.uCount);
        if(m_LoginUsers.find(nIndex) != m_LoginUsers.end())
            delete m_LoginUsers[nIndex];
		
        m_LoginUsers[nIndex] = p;
		//������ 2002-12-07 ����ɹ���½��ɾ���������е���Ӧ��
		BLACKMAP::iterator it = m_BlackUser.find(pParse->GetIP());
		if(it != m_BlackUser.end())
		{
			m_BlackUser.erase(it);
		}
		//������ ���ӽ��� 2002-12-07
    }
	//������ 2002-12-07 ���ʧ�ܾͼӵ���������
	else
	{
		BLACKMAP::iterator it = m_BlackUser.find(pParse->GetIP());
		if(it != m_BlackUser.end())
		{
			it->second ++;
			if(it->second >= m_nFailMaxTimes)
			{
                //���͸澯
                SInnerAlarm sAlarm;
                sAlarm.uAlarmID = ALARM_ID_MML_LOGIN_FAIL_MUCH;
                sAlarm.yAlarmType = AT_EVENT;
                sAlarm.yAlarmPara[0] = ALARM_PARA_LOGIN_FAIL_TOO_MUCH;
                
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
			}
		}
		else
		{
			m_BlackUser[pParse->GetIP()] = 1;
		}
	}
	//���ӽ���
    return nRet;
}

/*
    ��������:ע����¼���û�.

    ���û��Ѿ���¼,ɾ���û���¼��¼.
    ���û�δ��¼,�����κβ���.

    MML����ṹ:
    
    [LGO]
    macro_code = 107
    IsNotShow = 1
    macro_name=����Աע��

    ����:
    pParse,MML��������Ľ��.

    ����:ֻ����ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CRightMan::Logout(CParseResult* pParse)
{
    UINT4 nIndex = MAKEUINT4(pParse->GetWorkStationNo(),
                             pParse->GetWorkStationType());
    if(m_LoginUsers.find(nIndex) != m_LoginUsers.end())
    {
        CLoginUser *pUser = m_LoginUsers[nIndex];
        pParse->SetUserName(const_cast<char *>(pUser->GetUserName()));
        delete m_LoginUsers[nIndex];
        m_LoginUsers.erase(nIndex);
    }

    //by ldfldf 2004-01-30 OM����
    //�����M2000�û��˳���ɾ��M2000�û�
    if(ACE_OS::strncasecmp(pParse->GetUserName(),M2000, 4) == 0)
    {
        m_pPersistUser->DelUser(pParse->GetUserName());
    }
    //end

    return ERR_SUCCESS;
}
/*
    ��������:�޸��û�����.

    MML����ṹ:

    [MOD PWD]
    macro_code=104
    IsNotShow=1
    macro_name=�޸Ĳ���Ա�Ŀ���
    para_num=4
    must_give=1
    1= op(�ʺ�)
    2= old(������)
    3= new(������)
    4= cfm(ȷ������)

    ����:
    pParse,MML��������Ľ��.

    ����:
    1.��ȷ�����벻ƥ��,����ERR_PASSWORD_ERR

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CRightMan::Password(CParseResult* pParse)
{
    if(strcmp(pParse->GetStringVal(3),pParse->GetStringVal(4)))
        return ERR_PWD_UNMATCH;

    return m_pPersistUser->Password(pParse->GetStringVal(1),
                                    pParse->GetStringVal(2),
                                    pParse->GetStringVal(3));
}


/*
    ��������:����û�Ȩ��.

    ����:
    pParse,MML��������Ľ��,�ڸò����з����û������û�����.

    ����:
    1.������ͨ�û�ִ�г����û�����,����ERR_NOT_ENOUGHT_RIGHT.
    2.�����ǵ�¼�������û�δ��¼,����ERR_NOT_LOGIN.
    3.�����������ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CRightMan::CheckRight(CParseResult* pParse)
{
    return m_CmdAdmin.CheckPrivilege(pParse);
}

/*
    ��������:��ѯ�û���Ϣ,�����û���,�û�����,�Ƿ�����,

    ����:
    UserInfo,�����û���¼.

    ����:
    1.ֻ����ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CRightMan::QueryUserInfo(LIST<STRING>& UserInfo, CParseResult *pParse)
{
    //������ 2003-03-14
    MAP<UINT4,CLoginUser*>::iterator it = m_LoginUsers.begin();
    char szStatus[40] = {0};
    STRING strUserName;
    char szPI[1024];
    CLoginUser *pUser = NULL;
    UINT4 uIP = 0;
    //bM2000 ���ΪTRUE,�����Ѿ���ʾ��һ��M2000���û���.
    BOOL bM2000 = FALSE;

    for(; it != m_LoginUsers.end(); it++)
    {
        pUser = it->second;

        //by ldf 2004-01-30 OM����
        //���˵�һ���û�
        if(ACE_OS::strcasecmp(pUser->GetUserName(),cc08) == 0)
		{
     	    continue;
		}

        //by ldf 2004-02-05 ��Ϊһ��M2000��¼������,��ͨ��¼��proxy��¼,����Ӧ��ʾΪ1��
        if( ACE_OS::strncasecmp( pUser->GetUserName(),M2000, 4) == 0 )
		{
			if( !bM2000 )
			{
				bM2000 = TRUE;
			}
			else
			{
				continue;
			}
		}

		//by yangguang ��ͨ�û�ֻ������Լ�,����Ȩ��ʱ���������.
		if (pParse->GetUserLevel() == UL_OPER)
		{
			SET<BYTE>::iterator iter;
			iter = m_CmdAdmin.GetUserPrivilegeMap()[pParse->GetUserName()].CmdSet.find(1);
			if(iter == m_CmdAdmin.GetUserPrivilegeMap()[pParse->GetUserName()].CmdSet.end())
			{
				if (strcmp(pParse->GetUserName(), pUser->GetUserName()) !=0 || pUser->GetIP() != pParse->GetIP())
				{
					continue;	
				}
			}
		}

        //�û���
        if(strcmp(pUser->GetUserName(),"") == 0)
		{
            UserInfo.push_back("-");
		}
        else
		{
            UserInfo.push_back(pUser->GetUserName());
		}
        //�û�ȫ��
        if(strcmp(pUser->GetUserFullName(),"") == 0)
		{
            UserInfo.push_back("-");
		}
        else
		{
            UserInfo.push_back(pUser->GetUserFullName());
		}
        //�û�����
        if(pUser->GetUserLevel() == UL_ADMIN)
		{
            UserInfo.push_back(S_MML_ADMIN);
		}
        else
		{
            UserInfo.push_back(S_MML_NORMAL);
		}
        //����״̬
        uIP = pUser->GetIP();
        //֮����������ת������ΪCMmlCommRouter�µ�OnReceiveEx��
        //IPת��Ϊ����������Ȼ��HTONL�꣬��������ACE��INET_Addr
        //��get_ip_address����ת��Ϊ�����ֽ�������ʵ����HTONL
        //�ֽ����Ϊ�����ֽ���,����
        uIP = HTONL(uIP);
        ACE_INET_Addr ip_addr((unsigned short)1,uIP);
        SNPRINTF(szStatus,40,"%s(CN:%d)",
                 ip_addr.get_host_addr(),
                 pUser->GetStationNo());
        UserInfo.push_back(szStatus);
        //��������
        UserInfo.push_back(pUser->GetArea());
        //�û�Ȩ��
        strUserName = pUser->GetUserName();
        m_CmdAdmin.QueryUserPrivilege(strUserName,&szPI[0],1024);
        UserInfo.push_back(szPI);
        //��������
        char szPwdTimeLimit[6];
        SNPRINTF(szPwdTimeLimit, sizeof(szPwdTimeLimit), "%d", pUser->GetPwdTimeLimit());
        szPwdTimeLimit[5] = '\0';
        UserInfo.push_back(szPwdTimeLimit);  
		//��½����
		char szSession[10];
		sprintf(szSession, "%d", pUser->GetCount());
		UserInfo.push_back(szSession);		
    }

// add  by yangguang ��ͨ�û�ֻ������Լ�,����Ȩ��ʱ���������.
	if (pParse->GetUserLevel() == UL_OPER)
	{
		SET<BYTE>::iterator iter;
		iter = m_CmdAdmin.GetUserPrivilegeMap()[pParse->GetUserName()].CmdSet.find(1);
		if(iter == m_CmdAdmin.GetUserPrivilegeMap()[pParse->GetUserName()].CmdSet.end())
		{
			return ERR_SUCCESS;
		}
	}

    //��ʾ�����ߵ��û�
    UINT2 uUserNum = m_pPersistUser->GetUserNum();
    SUserInfo* pUsr = (SUserInfo*)(m_pPersistUser->QueryUserInfo());

    STRING strTemp;
    MAP<UINT4,CLoginUser*>::iterator i = m_LoginUsers.begin(),
                                     e = m_LoginUsers.end();
    for(UINT2 k = 0;k < uUserNum;k++)
    {
		/*
            �����û��������û��Ƿ�����,�û����ıȽϲ����ִ�Сд.
        */
        i = m_LoginUsers.begin();
        while(i != e)
        {
            if(ACE_OS::strcasecmp((*i).second->GetUserName(),
                                    pUsr[k].szName) == 0)
            {
                break;
            }
            i++;
        }
        if(i != e)
        {
            //������߾Ͳ�����һ��
            continue;
        }
        //cc08����ʾ���ͻ�
		//by ldf 2004-01-30 OM����
		if(ACE_OS::strcasecmp(pUsr[k].szName,cc08) == 0)
			continue;
		
		//�����M2000�û���������offline�������CG�쳣�˳���mmlû�и���
		//��ʱ��Ҫ����mml�����Ҳ���ʾ��û�н������ӵ�M2000�û�
		if( ACE_OS::strncasecmp( pUsr[k].szName,M2000, 4) == 0 )
		{
			m_pPersistUser->DelUser(pUsr[k].szName);				
			continue;
		}
		
        if(pUsr[k].szName[0] == 0)
            UserInfo.push_back("-");
        else
            UserInfo.push_back(pUsr[k].szName);

        if(pUsr[k].szFullName[0] == 0)
            UserInfo.push_back("-");
        else
            UserInfo.push_back(pUsr[k].szFullName);

        if(pUsr[k].yLevel == UL_ADMIN)
            UserInfo.push_back(S_MML_ADMIN);
        else
            UserInfo.push_back(S_MML_NORMAL);
        //����״̬
        UserInfo.push_back(S_MML_OFFLINE);
        //��������
        if(strcmp(pUsr[k].szArea,"") == 0)
            UserInfo.push_back("-");
        else
            UserInfo.push_back(pUsr[k].szArea);
        //�û�Ȩ��
        strUserName = pUsr[k].szName;
        m_CmdAdmin.QueryUserPrivilege(strUserName,&szPI[0],1024);
        UserInfo.push_back(szPI);
        //��������
        char szPwdTimeLimit[6];
        SNPRINTF(szPwdTimeLimit, sizeof(szPwdTimeLimit), "%d", pUsr[k].uPwdTimeLimit);
        szPwdTimeLimit[5] = '\0';
        UserInfo.push_back(szPwdTimeLimit);  
		//��½����
		char szSession[10];
		sprintf(szSession, "%d", pUsr[k].uCount);
		UserInfo.push_back(szSession);
    }
    return ERR_SUCCESS;
}

/*
    ��������:�޸��û���Ϣ,�����û���,�û�����,�û�����.

    [MOD OP]
    macro_code=103
    macro_name=�޸��û�����
    IsNotShow = 1
    para_num= 4
    must_give= 1
    1= op (�ʺ�)
    2= desc (�û�����)
    3= new (������)
    4= cfm (ȷ������)

    ����:
    pParse,MML��������Ľ��.

    ����:
    1.��ȷ�����벻ƥ��,����ERR_PASSWORD_ERR.
    2.�����������m_pPersistUser->ModifyUserInfo().

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CRightMan::ModifyUserInfo(CParseResult *pParse)
{
    if(strcmp(pParse->GetStringVal(4),pParse->GetStringVal(5)))        
        return ERR_PASSWORD_ERR;
    SUserInfo usr;
    strncpy(usr.szName,pParse->GetStringVal(1),USER_NAME_LEN);
    strncpy(usr.szFullName,pParse->GetStringVal(2),USER_FULL_NAEM_LEN);
    strncpy(usr.szArea,pParse->GetStringVal(6),AREA_DESCRIPTION_LEN);    
    usr.szName[USER_NAME_LEN - 1] = 0;
    usr.szFullName[USER_FULL_NAEM_LEN - 1] = 0;
    usr.szArea[AREA_DESCRIPTION_LEN-1] = 0;
    StringLRTrim(usr.szName);
    StringLRTrim(usr.szFullName);
    StringLRTrim(usr.szArea);
    usr.uPwdTimeLimit = atoi(pParse->GetStringVal(7));
	pParse->GetDigitVal((int*)&usr.uCount, 8);
    if(usr.uPwdTimeLimit > PWD_TIME_LIMIT_MAX || usr.uPwdTimeLimit < 0)
        usr.uPwdTimeLimit = PWD_TIME_LIMIT_MAX;
    
    if(usr.szName[0] == 0)
    {
        strcpy(usr.szName,"-");
    }
    if(usr.szFullName[0] == 0)
    {
        strcpy(usr.szFullName,"-");
    }
    if(usr.szArea[0] == 0)
    {
        strcpy(usr.szArea,"-");
    }
    UINT4 nIndex = MAKEUINT4(pParse->GetWorkStationNo(),
                             pParse->GetWorkStationType());
    int nRet = ERR_SUCCESS;
    MAP<UINT4,CLoginUser *>::const_iterator it;
    CLoginUser *pLogin = NULL;
    MAP<UINT4,CLoginUser *>::const_iterator et= m_LoginUsers.end();
    if( m_LoginUsers.find(nIndex) != m_LoginUsers.end())
    {
        nRet = m_pPersistUser->ModifyUserInfo(&usr, pParse->GetStringVal(3), pParse->GetStringVal(4));
        if(nRet != ERR_SUCCESS)
            return ERR_FAIL;
        //����޸��������û���Ҫ�޸��ڴ�
        for(it = m_LoginUsers.begin(); it != et; it++)
        {
            pLogin = it->second;
            if(strcmp(pLogin->GetUserName(),usr.szName) == 0)
            {
                strncpy(pLogin->m_szUserFullName,usr.szFullName,USER_NAME_LEN);
                strncpy(pLogin->m_szArea,usr.szArea,AREA_DESCRIPTION_LEN);
                pLogin->m_uPwdTimeLimit = usr.uPwdTimeLimit; 
				pLogin->m_uCount = usr.uCount;
            }
        }
    }
    return ERR_SUCCESS;
}
BOOL CRightMan::UserIsExistent(const char *szUser)
{
    if(ACE_OS::strcasecmp("admin",szUser) == 0)
        return TRUE;
    UINT2 uUserNum = m_pPersistUser->GetUserNum();
    SUserInfo* pUsr = (SUserInfo*)(m_pPersistUser->QueryUserInfo());
    
    for(UINT2 k = 0;k < uUserNum;k++)
    {
		//by ldf 2004-01-30 OM����
        //cc08 must be shielded
        if(ACE_OS::strcasecmp(pUsr[k].szName,cc08) == 0)
            continue;
        //�û����Ƿ����
        if(ACE_OS::strcasecmp(pUsr[k].szName,szUser) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CRightMan::Init()
{
    m_pPersistUser = CPersistUser::Instance();
    if(m_pPersistUser == NULL)
        return FALSE;
	//������ 2002-09-18 �������û�����Ϊ������
	CINIFile ini(GetCfgFilePath());
	ini.Open();
	m_uMaxUserNum = 5;
	m_uMaxUserNum = ini.GetInt(CFG_SEC_MML,CFG_MML_MAX_CONNECT_USER,5);
	if(m_uMaxUserNum > MAX_USER_NUM)
	{
		m_uMaxUserNum = MAX_USER_NUM;
	}
	//������ 2002-12-07
	m_nFailMaxTimes = ini.GetInt(CFG_SEC_MML,CFG_MML_MAX_FAIL_COUNT,CFG_MML_MAX_FAIL_COUNT_DEFAULT);
	if(m_nFailMaxTimes > 6 || m_nFailMaxTimes < 3)
	{
		m_nFailMaxTimes = CFG_MML_MAX_FAIL_COUNT_DEFAULT;
	}
	//���ӽ���
    return (m_CmdAdmin.Init(this) == ERR_SUCCESS);
}

//////////////////////////////////////////////////////////////////////
// CCommandAdministrator Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandAdministrator::CCommandAdministrator():m_Report("MML_SERVER"),m_pRight(NULL)
{
    m_nPageLimit = MAX_PAGE_LIMIT;
}
CCommandAdministrator::~CCommandAdministrator()
{
}
//���ò���Ա��Ȩ�������� Set Command Groups of Operator
int CCommandAdministrator::SetCommandForOp(CParseResult *pParse)
{
    STRING strOpName = pParse->GetStringVal(1);
    if(ACE_OS::strcasecmp(strOpName.c_str(),"admin") == 0)
    {
        m_strError = S_MML_NOT_ADD_CMD_FOR_ADM;
        GenerateCommonReport(pParse,m_strError.c_str(),-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;
    }
    if(m_pRight->UserIsExistent(strOpName.c_str()) != TRUE)
    {
        m_strError = S_MML_USER_NAME_NONEXIST;
        GenerateCommonReport(pParse,m_strError.c_str(),-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;
    }
    INTMAP GrpMap;
    int nRet = 0;
    //char szValue[256];
    GetEnumValue(pParse,2,60,GrpMap);
    SPrivilegeInfo PITemp;
    
    memset(&PITemp.tmStart,0,sizeof(tm));
    memset(&PITemp.tmEnd,0,sizeof(tm));
    CMmlIniFile ini(m_strUserPath.c_str());
    if(!ini.Open())
    {
        m_strError = S_MML_OPEN_INI_FAIL;
        m_strError += strerror(errno);
        GenerateCommonReport(pParse,m_strError.c_str(),-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;
    }
    MAP<STRING,SPrivilegeInfo>::iterator it = m_UserPrivilegeMap.find(strOpName);
    if(it == m_UserPrivilegeMap.end())
    {
        //����û���Ϣ
        char szKey[8];
        PITemp.nLimitType = -1;
        int nNumber = m_UserPrivilegeMap.size() + 1;
        nRet = ini.WriteInt(S_MML_CMD_ADM_HEAD,S_MML_CMD_ADM_NUM,m_UserPrivilegeMap.size() +1);
        sprintf(szKey,"%d",nNumber);
        nRet = ini.WriteString(S_MML_CMD_ADM_HEAD,szKey,const_cast<char *>(strOpName.c_str()));
        if(!nRet)
        {
            m_strError = S_MML_WRITE_INI_ERR;
            m_strError += strerror(errno);
            GenerateCommonReport(pParse,m_strError.c_str(),-1);
            SendToClient(pParse);
            WriteLog(pParse,-1,NULL);
            return ERR_FAIL;
        }
        //�����û�Ȩ�ޱ�
        m_UserPrivilegeMap[strOpName] = PITemp;
    }
    SPrivilegeInfo &PI = m_UserPrivilegeMap[strOpName];
    //�޸��û�Ȩ��
    PI.CmdSet.clear();
    INTMAP::iterator iter = GrpMap.begin();
    for(; iter != GrpMap.end(); iter ++)
    {
        nRet = iter->first;
        PI.CmdSet.insert(nRet);
    }
    char szTmp[256];
    memset(szTmp,0,256);
    PrintSetToStr(&szTmp[0],256,PI.CmdSet);
    //����Ӧ������д�̣��ٸ����ڴ档����Խ������������
    char *pTmp = const_cast<char *>(strOpName.c_str());
    ini.WriteString(pTmp,CFG_MML_CMD_GRP,szTmp);
    ini.TruncateFile();
    GenerateCommonReport(pParse,S_MML_SUCCEED,0);
    SendToClient(pParse);
    WriteLog(pParse,0,NULL);
    return ERR_SUCCESS;
}
//���ò���Ա�Ĳ���ʱ�� Set Time Limit of Operator
int CCommandAdministrator::SetTimeLimitForOp(CParseResult *pParse)
{
    //���������������Ǳظ�
    STRING strOpName = pParse->GetStringVal(1);
    STRING strBegin  = pParse->GetStringVal(2);
    STRING strEnd    = pParse->GetStringVal(3);
    tm tmStartDate, tmEndDate, tmZero;
    memset(&tmStartDate,0,sizeof(tm));
    memset(&tmEndDate,0,sizeof(tm));
    memset(&tmZero,0,sizeof(tm));
    int nRet = AnalysisDatePara(strBegin,tmStartDate);
    if(nRet == ERR_FAIL)
    {
        GenerateCommonReport(pParse,m_strError.c_str(),nRet);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return nRet;
    }
    nRet = AnalysisDatePara(strEnd,tmEndDate);
    if(nRet == ERR_FAIL)
    {
        GenerateCommonReport(pParse,m_strError.c_str(),nRet);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return nRet;
    }
    bool bRes = (tmStartDate.tm_year == 0);
    bool bRes2 = (tmEndDate.tm_year == 0);
    if(bRes != bRes2)
    {
        m_strError = S_MML_2_TIME_FORMAT_ERR;
        GenerateCommonReport(pParse,S_MML_2_TIME_FORMAT_ERR,-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;
    }
    //������ 2003-05-10
    if(ACE_OS::strcasecmp(strBegin.c_str(),strEnd.c_str()) >= 0)
    {
        m_strError = S_CMD_TIME_WRONG;
        GenerateCommonReport(pParse,S_CMD_TIME_WRONG,-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;
    }
    //����л��Ҫ�����ӽ���ʱ���뵱ǰʱ����ж�
    if((strEnd[0] == 'Y' || strEnd[0] == 'y')
        &&(strEnd[5] == 'M' || strEnd[5] == 'm')
        &&(strEnd[8] == 'D' || strEnd[8] == 'd'))
    {
        time_t tCurrent = 0;
        time(&tCurrent);
        tm tmCurrent = *localtime(&tCurrent);
        char szCurrent[16];
        strftime(szCurrent,16,"Y%YM%mD%d",&tmCurrent);
        if(ACE_OS::strcasecmp(szCurrent,strEnd.c_str()) > 0)
        {
            m_strError = S_CMD_END_TIME_TOO_SMALL;
            GenerateCommonReport(pParse,S_CMD_END_TIME_TOO_SMALL,-1);
            SendToClient(pParse);
            WriteLog(pParse,-1,NULL);
            return ERR_FAIL;
        }
    }
    //�޸Ľ���
    //д��
    CMmlIniFile ini(m_strUserPath.c_str());
    if(!ini.Open())
    {
        m_strError = S_MML_OPEN_USER_INI_ERR;
        GenerateCommonReport(pParse,S_MML_OPEN_USER_INI_ERR,-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;
    }
    MAP<STRING, SPrivilegeInfo>::iterator it = m_UserPrivilegeMap.find(strOpName);
    if(it == m_UserPrivilegeMap.end()
    && m_pRight->UserIsExistent(strOpName.c_str()) != TRUE)
    {        
        m_strError = S_MML_USER_NAME_NONEXIST;
        GenerateCommonReport(pParse,S_MML_USER_NAME_NONEXIST,-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;
    }
    char szTmp[32];
    int nLimitType = 0;
    //����ʡ���˴�����
    char *pSection = const_cast<char *>(strOpName.c_str());
    if(it == m_UserPrivilegeMap.end())
    {
        int nNumber = m_UserPrivilegeMap.size() + 1;
        ini.WriteInt(S_MML_CMD_ADM_HEAD,S_MML_CMD_ADM_NUM,nNumber);
        sprintf(szTmp,"%d",nNumber);
        ini.WriteString(S_MML_CMD_ADM_HEAD,szTmp,pSection);
    }
    if(memcmp(&tmStartDate,&tmZero,sizeof(tm)) == 0
      && memcmp(&tmEndDate,&tmZero,sizeof(tm)) == 0)
      nLimitType = -1;
    else if(tmStartDate.tm_year == 0 
          && tmStartDate.tm_mon == 0 
          && tmStartDate.tm_mday == 0)
    {
        nLimitType = 1; 
        strftime(&szTmp[0],32,"%H:%M:%S",&tmStartDate);
        ini.WriteString(pSection,CFG_MML_BEGIN_TIME,szTmp);
        strftime(&szTmp[0],32,"%H:%M:%S",&tmEndDate);
        ini.WriteString(pSection,CFG_MML_END_TIME,szTmp);    
    }
    else 
    {
        nLimitType = 0;
        strftime(&szTmp[0],32,"%Y-%m-%d",&tmStartDate);
        ini.WriteString(pSection,CFG_MML_BEGIN_DATE,szTmp);
        strftime(&szTmp[0],32,"%Y-%m-%d",&tmEndDate);
        ini.WriteString(pSection,CFG_MML_END_DATE,szTmp);
    }
    ini.WriteInt(pSection,CFG_MML_LIMIT_TYPE,nLimitType);
    ini.TruncateFile();
    //�����ڴ�
    SPrivilegeInfo &PI = m_UserPrivilegeMap[strOpName];
    PI.tmStart = tmStartDate;
    PI.tmEnd = tmEndDate;
    PI.nLimitType = nLimitType;
    
    GenerateCommonReport(pParse,S_MML_SUCCEED,0);
    SendToClient(pParse);
    WriteLog(pParse,0,NULL);
    return ERR_SUCCESS;
}
//��ѯ��ǰ��ִ�е����� Query Current Authorized Commands
int CCommandAdministrator::QueryRunCmd(CParseResult *pParse)
{
    STRING strOpName = pParse->GetUserName();
    if(ACE_OS::strcasecmp(pParse->GetUserName(),"admin") == 0)
    {
        GenerateCommonReport(pParse,S_MML_ADMIN_USER_RUN_ALL,0);
        SendToClient(pParse);
        WriteLog(pParse,0,NULL);
        return ERR_SUCCESS;
    }
    MAP<STRING,SPrivilegeInfo>::iterator it;
    it = m_UserPrivilegeMap.find(strOpName);
    if(it == m_UserPrivilegeMap.end()
    && m_pRight->UserIsExistent(strOpName.c_str()) != TRUE)
    {
        m_strError = S_MML_USER_NAME_NONEXIST;
        GenerateCommonReport(pParse,S_MML_USER_NAME_NONEXIST,-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;
    }
    SPrivilegeInfo sPi;
    SPrivilegeInfo &PI = sPi;
    if(it == m_UserPrivilegeMap.end())
    {
        sPi.nLimitType = -1;
        memset(&(sPi.tmStart),0,sizeof(tm));
        memset(&(sPi.tmEnd),0,sizeof(tm));
    }
    else
        PI = m_UserPrivilegeMap[strOpName];
    int nCnt = 0;
    int nTmpCnt = 0;
    int nRet = REPORT_MORE;
    while(nRet >= REPORT_MORE)
    {
        m_Report.MakeReportHead();
        m_Report.MakeCmdDisplay(pParse->GetCmdLine(),0,0,"MML_SERVER");
        nRet = GenerateRunCmdReportBody(PI,nTmpCnt);
        nCnt += nTmpCnt;
        m_Report.AddString(STR_NEWLINE);
        m_Report.AddString(STR_NEWLINE);
        if(nRet > REPORT_NONE)
        {
            m_Report.AddString(S_ALARM_TOBECONITUE);
        }
        else
        {
            char szTmp[128];
            SNPRINTF(szTmp,128,S_PERF_FMT_STR2,S_PERF_IP_RESULT,nCnt);
            m_Report.AddString(szTmp);
        }
        m_Report.AddString(STR_NEWLINE);
        m_Report.MakeReportTail();
        SendToClient(pParse);
    }
    WriteLog(pParse,0,NULL);
    return ERR_SUCCESS;
}
//���������������������� Add Commands to Command Group
int CCommandAdministrator::AddCmdToGroup(CParseResult *pParse)
{
    int nSetId = 0;
    pParse->GetDigitVal(&nSetId,1);
    STRING strCmdArray = pParse->GetStringVal(2);
    //����NoteList�������
    LIST<STRING> NoteList;
    int nRet = MoveCmdBetweenSet(0,nSetId,strCmdArray.c_str(),NoteList);
    SendReportToClient(NoteList,pParse);
    WriteLog(pParse,nRet,NULL);
    return ERR_SUCCESS;
}
//ɾ�������������������� Remove Commands from Command Group
int CCommandAdministrator::DelCmdFromGroup(CParseResult *pParse)
{
    int nSetId = 0;
    pParse->GetDigitVal(&nSetId,1);
    STRING strCmdArray = pParse->GetStringVal(2);
    //����NoteList�������
    LIST<STRING> NoteList;
    int nRet = MoveCmdBetweenSet(nSetId,0,strCmdArray.c_str(),NoteList);
    SendReportToClient(NoteList,pParse);
    WriteLog(pParse,nRet,NULL);
    return ERR_SUCCESS;
}
//��ѯ������������������ Query Commands of Command Group
int CCommandAdministrator::QueryCmdOfGroup(CParseResult *pParse)
{
    int nSetId = 0;
    LIST<STRING> ResiltList;
    LIST<STRING> TitleList;
    m_Report.MakeReportHead();
    m_Report.MakeCmdDisplay(pParse->GetCmdLine(),0,0,"MML_SERVER");
    if(!pParse->GetDigitVal(&nSetId,1))
    {
        //��ʾ�г������Ŀ
        TitleList.push_back(S_MML_COMMAND_COL1);
        MAP<BYTE,STRING>::iterator it;
        MAP<BYTE,STRING>::iterator e = m_CmdGrpDes.end();
        for(it = m_CmdGrpDes.begin(); it != e; it++)
        {
            if(it->first == 0)
                continue;
            STRING &strCommand = it->second;
            ResiltList.push_back(strCommand);
        }
        m_Report.AddHoriTable(TitleList,ResiltList,S_PERF_IP_RESULT);
    }
    else
    {
        MAP<UINT4,SCMDDescription> *pMap = NULL;
        if(nSetId == 0)
            pMap = &m_CmdMap0;
        else
            pMap = &m_CmdMap;
        //����ÿ�������û�п��Ƿ�ҳ�����
        MAP<UINT4,SCMDDescription>::iterator it = pMap->begin();
        for(;it != pMap->end(); it++)
        {
            SCMDDescription &Des = it->second;
            if(Des.yCmdSetId == nSetId)
            {
                ResiltList.push_back(Des.strCmdName);
            }
        }
        TitleList.push_back(S_MML_COMMAND_COL1);
        //TitleList.push_back(S_MML_COMMAND_COL2);
        m_Report.AddHoriTable(TitleList,ResiltList,S_MML_CMD_LIST);
    }
    m_Report.MakeReportTail();
    SendToClient(pParse); 
    WriteLog(pParse,0,NULL);
    return ERR_SUCCESS;
}
//����û��Ƿ���ִ���ض������Ȩ��
int CCommandAdministrator::CheckPrivilege(CParseResult *pParse)
{

    //by ldf 2004-01-30 OM���ģ�����M2000 ��������ȷ��ʽ�������¼��ʽ
    if(pParse->GetCmdCode() == MML_RVI_OP || pParse->GetCmdCode() == MML_SND_CRG)
    {
        char szUserNameTmp[64];

        UINT4 nM2000IP = pParse->GetIP();

        struct in_addr Addr;
#if defined(_PLATFORM_HPUX) || defined(_PLATFORM_LINUX)
        Addr.s_addr = nM2000IP;
#else //_PLATFORM_HPUX
        Addr.S_un.S_addr = nM2000IP;
#endif//_PLATFORM_HPUX
        char szIPAddress[32];
        if (ACE_OS::inet_ntoa(Addr))
        {
            SNPRINTF(szIPAddress, 32, "%s", ACE_OS::inet_ntoa(Addr));
        }
        else
        {
            return ERR_INVALID_IP_ADDR;
        }

        sprintf(szUserNameTmp,"M2000(%s)",szIPAddress);
        pParse->SetUserName(szUserNameTmp);
        return ERR_SUCCESS;
    }
    //end

    //��¼����ֱ�ӷ��سɹ�.
    int nErrCode = ERR_SUCCESS;
    int nSetId = 0;
    int nCmd = pParse->GetCmdCode();
    
    UINT4 nIndex = MAKEUINT4(pParse->GetWorkStationNo(),
                             pParse->GetWorkStationType());
    if(pParse->GetCmdCode() == MML_LGI || pParse->GetCmdCode() == MML_LGI_CFG)//��������̨��½
    {
        //�Ȳ����û�
        STRING strUserName = pParse->GetStringVal(1);
        MAP<STRING,SPrivilegeInfo>::iterator UserIt = m_UserPrivilegeMap.find(strUserName);
        if(UserIt == m_UserPrivilegeMap.end())
        {
            return ERR_SUCCESS;
        }
        SPrivilegeInfo &PI = UserIt->second;
        
        //���û��ʱ������
        if(PI.nLimitType < 0)
        {
            return ERR_SUCCESS;
        }
        else
        {
            time_t tCurrent = 0;
            time(&tCurrent);
            tm tmNow = *localtime(&tCurrent);
            char szCurrent[32];
            char szBegin[32];
            char szEnd[32];
            if(PI.nLimitType > 0)
            {
                strftime(szCurrent,32,"%H%M%S",&tmNow);
                strftime(szBegin,32,  "%H%M%S",&(PI.tmStart));
                strftime(szEnd,32,    "%H%M%S",&(PI.tmEnd));
            }
            else
            {
                strftime(szCurrent,32,"%Y%m%d",&tmNow);
                strftime(szBegin,32,  "%Y%m%d",&(PI.tmStart));
                strftime(szEnd,32,    "%Y%m%d",&(PI.tmEnd));
            }
            if(strcmp(szBegin,szCurrent) > 0 || strcmp(szEnd,szCurrent) < 0) 
            {
                //���͸澯
                SInnerAlarm sAlarm;
                sAlarm.uAlarmID = ALARM_ID_MML_LOGIN_FAIL_MUCH;
                sAlarm.yAlarmType = AT_EVENT;
                sAlarm.yAlarmPara[0] = ALARM_PARA_UNAUTHORIZED_LOGIN;
                
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
            }                
        }        
        
        pParse->SetUserName((char*)pParse->GetStringVal(1));
        return ERR_SUCCESS;
    }
    //δ��¼����ʧ��.
    if(m_pRight->m_LoginUsers.find(nIndex) == m_pRight->m_LoginUsers.end())
    {
        if(pParse->GetCmdCode() == MML_LGO)
            return ERR_USER_LGO_AGAIN;
        return ERR_NOT_LOGIN;
    }
    pParse->SetUserName((char*)(m_pRight->m_LoginUsers[nIndex]->GetUserName()));
    pParse->SetUserLevel(m_pRight->m_LoginUsers[nIndex]->GetUserLevel());    
    //�����û����سɹ�.
    if(ACE_OS::strcasecmp(pParse->GetUserName(),"admin") == 0)
        return ERR_SUCCESS;
    
    //��Ĭ������е�����ֱ�ӷ���(���ѵ�½�������)
    MAP<UINT4,STRING>::iterator i = m_DefaultCmdMap.find(nCmd);
    if(i != m_DefaultCmdMap.end())
    {
        return ERR_SUCCESS;
    }
    

    //��������ID�����������ܼ�
    MAP<UINT4,SCMDDescription>::iterator it = m_CmdMap.find(nCmd);
    if(it != m_CmdMap.end())
    {
        SCMDDescription &cmd_des = it->second;
        nSetId = cmd_des.yCmdSetId;
        STRING strUserName = pParse->GetUserName();
	    //by ldf 2004-02-02
        if(ACE_OS::strncasecmp(pParse->GetUserName(),M2000,4) == 0)
        {
			if(nSetId == 4 || nSetId == 8)
            {
                return ERR_SUCCESS;
            }
        }
    	//end

        //Ҫ�Ȳ����û�
        MAP<STRING,SPrivilegeInfo>::iterator UserIt = m_UserPrivilegeMap.find(strUserName);
        if(UserIt == m_UserPrivilegeMap.end())
        {
            GenerateCommonReport(pParse,S_MML_NEW_USER_NO_ADD_CMD,-1);
            SendToClient(pParse);
            return ERR_NO_PRIVILEGE;
        }
        SPrivilegeInfo &PI = UserIt->second;
        SET<BYTE>::iterator SetIt = PI.CmdSet.find(nSetId);
        if(SetIt != PI.CmdSet.end())
        {
            //���û��ʱ������
            bool nTimeLimitPass = false;
            if(PI.nLimitType < 0)
              nTimeLimitPass = true;
            else
            {
                time_t tCurrent = 0;
                time(&tCurrent);
                tm tmNow = *localtime(&tCurrent);
                char szCurrent[32];
                char szBegin[32];
                char szEnd[32];
                if(PI.nLimitType > 0)
                {
                    strftime(szCurrent,32,"%H%M%S",&tmNow);
                    strftime(szBegin,32,  "%H%M%S",&(PI.tmStart));
                    strftime(szEnd,32,    "%H%M%S",&(PI.tmEnd));
                }
                else
                {
                    strftime(szCurrent,32,"%Y%m%d",&tmNow);
                    strftime(szBegin,32,  "%Y%m%d",&(PI.tmStart));
                    strftime(szEnd,32,    "%Y%m%d",&(PI.tmEnd));
                }
                if(strcmp(szBegin,szCurrent) <= 0 && strcmp(szEnd,szCurrent) >= 0) 
                {
                    nTimeLimitPass = true;
                }
            }
            if(nTimeLimitPass)
            {   
                //�����û��޸��Լ������Ժ�����.
                //����������Ҫ���⴦��
                if((pParse->GetCmdCode() == MML_MOD_OP) || 
                    (pParse->GetCmdCode() == MML_MOD_PWD))
                {
                    if(ACE_OS::strcasecmp(pParse->GetUserName(),
                        pParse->GetStringVal(1)) == 0)
                    { 
                        return ERR_SUCCESS;
                    }
                    //�����û������޸�
                    else if(pParse->GetUserLevel() == CL_ADMIN)
                        return ERR_SUCCESS;
                    else
                        m_strError = S_MML_NO_RIGHT;
                }
                //ͨ��Ȩ�޼��
                else 
                    return ERR_SUCCESS;
            }
            else
                m_strError = S_MML_USER_EXPIRE;
        }
        else 
            m_strError = S_MML_NO_RIGHT;
        GenerateCommonReport(pParse,m_strError.c_str(),-1);
        SendToClient(pParse);
        return ERR_NO_PRIVILEGE;
    }
    GenerateCommonReport(pParse,S_MML_NO_CMD_IN_MAP,-1);
    SendToClient(pParse);
    return ERR_NO_COMMAND;
}
//��ʼ��
int CCommandAdministrator::Init(CRightMan *pRightMan)
{
    //��ʼ��
    char szPath[MAX_PATH];
    char szSection[32];
    char szKey[16];
    char szValue[256];
    SCMDDescription sCmd;
    int i = 0;
    int j = 0;
    int nCmdNum = 0;
    m_pRight = pRightMan;
    int nRet = 0;
    SNPRINTF(szPath,MAX_PATH,"%s/config/mml/command_group.ini",
             GetAppPath());
    m_strCmdPath = szPath;
    CMmlIniFile mmlINI(szPath);
    if(!mmlINI.Open())
    {
        TRACE(MTS_MMLSERVER,S_MML_CMD_ADM_INIT_FAIL);
        return ERR_FAIL;
    }
    int nNum = mmlINI.GetInt(S_MML_CMD_ADM_HEAD,S_MML_CMD_ADM_NUM,15);
    //��ʼ������������
    for(i = 1; i <= nNum; i++)
    {
        SNPRINTF(szSection,32,"%s%d",S_MML_CMD_ADM_SEC,i);
        nCmdNum = mmlINI.GetInt(szSection,S_MML_CMD_ADM_NUM,0);
        sCmd.yCmdSetId = i;
        mmlINI.GetString(szSection,CFG_MML_CMD_GRP_DES,
                         szSection,szValue,256);
        m_CmdGrpDes[i] = szValue;
        for(j = 1; j <= nCmdNum; j++)
        {
            SNPRINTF(szKey,16,"%d",j);
            mmlINI.GetString(szSection,szKey,"",szValue,256);
            sCmd.strCmdName = szValue;
            SNPRINTF(szKey,16,"%d.1",j);
            sCmd.nCmd = mmlINI.GetInt(szSection,szKey,0);
            SNPRINTF(szKey,16,"%d.2",j);
            mmlINI.GetString(szSection,szKey,"",szValue,256);
            sCmd.strCmdDes = szValue;
            m_CmdMap[sCmd.nCmd] = sCmd;
            m_NameToIdCmdMap[sCmd.strCmdName] = sCmd.nCmd;
        }
    }
    m_nSetNum = i-1;
    //��ʼ��Ĭ�����
    strncpy(szSection,"DefaultSet",32);
    int nCmd = 0;
    nCmdNum = mmlINI.GetInt(szSection,S_MML_CMD_ADM_NUM,0);
    for(j = 1; j <= nCmdNum; j++)
    {
        SNPRINTF(szKey,16,"%d.1",j);
        nCmd = mmlINI.GetInt(szSection,szKey,0);
        SNPRINTF(szKey,16,"%d",j);
        mmlINI.GetString(szSection,szKey,"",szValue,256);
        m_DefaultCmdMap[nCmd] = szValue;
        m_NameToIdCmdMap[szValue] = nCmd;
    }
    //�����Set0,���ڴ�������ӵ�����
    SNPRINTF(szSection,32,"%s0",S_MML_CMD_ADM_SEC);
    nCmdNum = mmlINI.GetInt(szSection,S_MML_CMD_ADM_NUM,0);
    m_CmdGrpDes[0] = CFG_MML_CMD_GRP0_DES;
    sCmd.yCmdSetId = 0;
    for(j = 1; j <= nCmdNum; j++)
    {
        SNPRINTF(szKey,16,"%d",j);
        mmlINI.GetString(szSection,szKey,"",szValue,256);
        sCmd.strCmdName = szValue;
        SNPRINTF(szKey,16,"%d.1",j);
        sCmd.nCmd = mmlINI.GetInt(szSection,szKey,0);
        SNPRINTF(szKey,16,"%d.2",j);
        mmlINI.GetString(szSection,szKey,"",szValue,256);
        sCmd.strCmdDes = szValue;
        m_CmdMap0[sCmd.nCmd] = sCmd;
        m_NameToIdCmdMap[sCmd.strCmdName] = sCmd.nCmd;
    }
    mmlINI.Close();
    //��ʼ���û�����Ȩ�ޱ�
    SPrivilegeInfo PI;
    mmlINI.Open(GetCfgFilePath());
    mmlINI.GetString(CFG_SEC_MML,            /* Section       */
                    CFG_MML_PERSIST_USER,     /* Key           */
                    CFG_MML_PERSIST_USER_DEF, /* Default value */
                    szPath,                   /* Buffer out    */
                    MAX_PATH);                /* Buffer len    */
    CreateDeepDir(szPath);
    strcat(szPath,"/user.ini");
    mmlINI.Close();
    m_strUserPath = szPath;
    if(!mmlINI.Open(szPath))
    {
        FILE *pFile = fopen(szPath,"wb+");
        if(pFile == NULL)
        {
            TRACE(MTS_MMLSERVER,S_MML_CMD_ADM_CRE_USER_FILE,strerror(errno));
            return ERR_FAIL;
        }
        fclose(pFile);
    }
    mmlINI.Open(szPath);
    nNum = mmlINI.GetInt(S_MML_CMD_ADM_HEAD,S_MML_CMD_ADM_NUM,0);
    for(i = 1; i <= nNum; i++)
    {
        SNPRINTF(szKey,16,"%d",i);
        mmlINI.GetString(S_MML_CMD_ADM_HEAD,szKey,"",szSection,32);
        if(strcmp(szSection,"") == 0)
        {
            TRACE(MTS_MMLSERVER,S_MML_CMD_ADM_USER_NMAE_ERR,i);
            continue;
        }
        mmlINI.GetString(szSection,CFG_MML_CMD_GRP,"-1&-1&-1",szValue,256);
        AnalysisCommandGroup(szValue,PI.CmdSet);
        PI.nLimitType = mmlINI.GetInt(szSection,CFG_MML_LIMIT_TYPE,-1);
        memset(&(PI.tmStart),0,sizeof(tm));
        memset(&(PI.tmEnd),0,sizeof(tm));
        if(PI.nLimitType == 0)
        {
            mmlINI.GetString(szSection,CFG_MML_BEGIN_DATE,"0000-00-00",szValue,256);
            AnalysisDate(szValue,PI.tmStart);
            mmlINI.GetString(szSection,CFG_MML_END_DATE,"0000-00-00",szValue,256);
            AnalysisDate(szValue,PI.tmEnd);
        }
        else if(PI.nLimitType > 0)
        {
            mmlINI.GetString(szSection,CFG_MML_BEGIN_TIME,"00:00:00",szValue,256);
            AnalysisTime(szValue,PI.tmStart);
            mmlINI.GetString(szSection,CFG_MML_END_TIME,"00:00:00",szValue,256);
            AnalysisTime(szValue,PI.tmEnd);     
        }
        m_UserPrivilegeMap[szSection] = PI;
    } // for
    return ERR_SUCCESS;
}

int CCommandAdministrator::AnalysisCommandGroup(const char *pCmdGrp, SET<BYTE> &CmdGrpSet)
{
    strncpy(m_szCatch,pCmdGrp,128);
    CmdGrpSet.clear();
    int nToken = 0;
    char *pToken = strtok(m_szCatch,"&");
    while(pToken)
    {
        nToken = atoi(pToken);
        CmdGrpSet.insert(nToken);
        pToken = strtok(NULL,"&");
    }
    return CmdGrpSet.size();
}

int CCommandAdministrator::AnalysisDate(const char *pszDate, tm &Date)
{
    strncpy(m_szCatch,pszDate,128);
    int nYear = 0, nMonth = 0, nDay = 0;
    char *pToken = NULL;
    pToken = strtok(&m_szCatch[0],"-");
    if(pToken)
        nYear = atoi(pToken);
    pToken = strtok(NULL,"-");
    if(pToken)
        nMonth = atoi(pToken);
    pToken = strtok(NULL,"-");
    if(pToken)
        nDay = atoi(pToken);
    if(nYear >= 1980 && nYear <= 3000)
    {
        Date.tm_year = nYear - 1900;
        Date.tm_mon = nMonth - 1;
        Date.tm_mday = nDay;
    }
    else
    {
        Date.tm_year = 0;
        Date.tm_mon = 0;
        Date.tm_mday = 0;
    }
    return 0;
}

int CCommandAdministrator::AnalysisTime(const char *pszTime, tm &Time)
{
    strncpy(m_szCatch,pszTime,128);
    int nHour = 0, nMinute = 0, nSecond = 0;
    char *pToken = NULL;
    pToken = strtok(&m_szCatch[0],":");
    if(pToken)
        nHour = atoi(pToken);
    pToken = strtok(NULL,":");
    if(pToken)
        nMinute = atoi(pToken);
    pToken = strtok(NULL,":");
    if(pToken)
        nSecond = atoi(pToken);
    if(nHour >= 0 && nHour < 24)
    {
        Time.tm_hour = nHour;
        Time.tm_min = nMinute;
        Time.tm_sec = nSecond;
    }
    return 0;
}
int CCommandAdministrator::GetEnumValue(CParseResult *pParse, int nID, int nMaxLen, INTMAP &EnumMap)
{
    STRING szParameter;
    int nPos = 0;
    int nValue = 0;
    int nKeyPos = 0;
    for (int i = 1; i <= nMaxLen; i ++)
    {
        szParameter = pParse->GetStringVal(nID,i);
        if (szParameter.compare("") != 0)
        {
            nPos = szParameter.find("-");
            nKeyPos = atoi((szParameter.substr(0, nPos)).c_str());
            nValue = atoi((szParameter.substr(nPos + 1)).c_str());
            EnumMap[nKeyPos] = nValue;
        }
        else
            break;
    }
    return EnumMap.size();
}

int CCommandAdministrator::PrintSetToStr(char *pValue, int nLen,SET<BYTE> PrintedSet, const char *pSec)
{
    char *pBuf = pValue;
    int nLeft = nLen;
    SET<BYTE>::const_iterator it = PrintedSet.begin();
    SET<BYTE>::const_iterator et = PrintedSet.end();
    for(; it != et; it++)
    {
        SNPRINTF(pBuf,nLeft,"%-d%-s",(*it),pSec);
        if(*it >= 10)
        {
            nLeft -= 3;
            pBuf += 3;
        }
        else
        {
            nLeft -= 2;
            pBuf += 2;
        }
    }
    pBuf --; //��Ϊ���һ��'&'������Ҫ�����һ��'&'��Ϊ' '
    *pBuf = '\0';
    return pValue - pBuf;
}

void CCommandAdministrator::GenerateCommonReport(CParseResult *pParse,const char *pszTxt, int nRes)
{
    m_Report.Clear("MML_SERVER");
    m_Report.MakeReportHead();
    
    nRes = (nRes > 0)? (nRes):(-nRes);
    m_Report.MakeCmdDisplay(pParse->GetCmdLine(),
                          0, nRes, "MML_SERVER");

    m_Report.AddString(pszTxt);
    m_Report.AddString(STR_NEWLINE);
    m_Report.MakeReportTail();
}

int CCommandAdministrator::AnalysisDatePara(STRING &sDate, tm &tmDate)
{
    //��������Y2002M01D01 or H08N30S59���ַ���
    try{
        
        if(sDate.length() > strlen("Y2002M01D01"))
        {
            throw S_MML_DATE_LEN_ERR;
        }
        int nPara1 = 0, nPara2 = 0,nPara3 = 0;
        memset(&tmDate,0,sizeof(tm));
        if((sDate[0] == 'Y' || sDate[0] == 'y')
            &&(sDate[5] == 'M' || sDate[5] == 'm')
            &&(sDate[8] == 'D' || sDate[8] == 'd'))
        {
            nPara1 = atoi(sDate.substr(1,4).c_str());
            nPara2 = atoi(sDate.substr(6,2).c_str());
            nPara3 = atoi(sDate.substr(9,2).c_str());
            
            if(nPara1 == 0)
                tmDate.tm_year = 0;
            else
                tmDate.tm_year = nPara1 - 1900;
            
            if(nPara2 == 0)
                tmDate.tm_mon = 0;
            else
                tmDate.tm_mon = nPara2 - 1;
            
            tmDate.tm_mday = nPara3;
        }
        else if((sDate[0] == 'H' || sDate[0] == 'h')
            &&(sDate[3] == 'N' || sDate[3] == 'n')
            &&(sDate[6] == 'S' || sDate[6] == 's'))
        {
            nPara1 = atoi(sDate.substr(1,2).c_str());
            nPara2 = atoi(sDate.substr(4,2).c_str());
            nPara3 = atoi(sDate.substr(7,2).c_str());
            tmDate.tm_hour = nPara1;
            tmDate.tm_min = nPara2;
            tmDate.tm_sec = nPara3;
        }
        else
        {
            throw S_MML_DATE_FORMAT_ERR;
        }
    }
    catch(const char *pError)
    {
        m_strError = pError;
        return ERR_FAIL;
    }
    return ERR_SUCCESS;
}

int CCommandAdministrator::GenerateRunCmdReportBody(SPrivilegeInfo &PI, int &nRecCnt)
{
    static MAP<UINT4,SCMDDescription>::iterator it = m_CmdMap.begin();
    LIST<STRING> TitleList;
    LIST<STRING> ContentList;
    if(it == m_CmdMap.end())
    {
        //���¿�ʼ��ѯ
        it = m_CmdMap.begin();
    }
    TitleList.push_back(S_MML_COMMAND_COL1);
    //TitleList.push_back(S_MML_COMMAND_COL2);
    SET<BYTE>::iterator iter;
    int i = 0;
    int nSetId = 0;
    SCMDDescription &cmdDes = it->second;
    for(;i < MAX_REC_NUM && it != m_CmdMap.end(); it++)
    {
       cmdDes = it->second;
       nSetId = cmdDes.yCmdSetId;
       iter = PI.CmdSet.find(nSetId);
       if(iter != PI.CmdSet.end())
       {
           ContentList.push_back(cmdDes.strCmdName);
           i++;
       }
    }
    m_Report.AddHoriTable(TitleList,ContentList,S_MML_CMD_LIST_TITLE);
    nRecCnt = i; 
    if(it == m_CmdMap.end())
        return REPORT_NONE;
    return i;
}

int CCommandAdministrator::StatisticCmdCntOfSet(int nSetID)
{
    if(nSetID < 0)
    {
        return m_DefaultCmdMap.size();
    }
    else if(nSetID == 0)
    {
        return m_CmdMap0.size();
    }
    int nCnt = 0;
    MAP<UINT4,SCMDDescription>::iterator it;
    for(it = m_CmdMap.begin(); it != m_CmdMap.end(); it++)
    {
        SCMDDescription &des = it->second;
        if(des.yCmdSetId == nSetID)
        {
            nCnt ++;
        }
    }
    return nCnt;
}

int CCommandAdministrator::WriteSetToStr(int nSetId, char *Buffer, int nLen)
{
    int nLeft = nLen - 32; //Ϊnumber=1�����ռ�
    char szTemp[64];
    char szTemp2[64];
    char szTemp3[64];
    int nCnt = 0;
    SNPRINTF(szTemp,64,"[%s%d]\n",S_MML_CMD_ADM_SEC,nSetId);
    strncpy(Buffer,szTemp,nLeft);
    STRING &strCmdDescription = m_CmdGrpDes[nSetId];
    std::stringstream st;
    st << CFG_MML_CMD_GRP_DES << "=" << strCmdDescription << std::endl;
    strcat(Buffer,st.str().c_str());
    nLeft -= strCmdDescription.length();
    MAP<UINT4,SCMDDescription>::iterator it;
    MAP<UINT4,SCMDDescription> *pCmdMap = NULL;
    if(nSetId == 0)
    {
       pCmdMap = &m_CmdMap0;
    }
    else
    {
        pCmdMap = &m_CmdMap;
    }
    for(it = pCmdMap->begin(); it != pCmdMap->end(); it++)
    {
        SCMDDescription &Des = it->second;
        if(Des.yCmdSetId == nSetId)
        {
            nCnt++;
            SNPRINTF(szTemp,64,"%d=%s\n",nCnt,Des.strCmdName.c_str());
            SNPRINTF(szTemp2,64,"%d.1=%d\n",nCnt,Des.nCmd);
            SNPRINTF(szTemp3,64,"%d.2=%s\n\n",nCnt,Des.strCmdDes.c_str());
            nLeft -= strlen(szTemp);
            nLeft -= strlen(szTemp2);
            nLeft -= strlen(szTemp3);
            if(nLeft <= 0)
                return ERR_FAIL;
            strcat(Buffer,szTemp);
            strcat(Buffer,szTemp2);
            strcat(Buffer,szTemp3);
        }
    }
    SNPRINTF(szTemp,64,"%s=%d\n",S_MML_CMD_ADM_NUM,nCnt);
    strcat(Buffer,szTemp);
    return ERR_SUCCESS;
}

int CCommandAdministrator::AddCmdToIni(CMmlIniFile &ini, const char *szSection, int nCmdCnt,SCMDDescription &CmdDes)
{
    if(!ini.IsOpen())
    {    
        m_strError = "ini is not open";
        return ERR_FAIL;
    }
    char szKey[32];
    char *pSection = const_cast<char *>(szSection);
    char *pValue = NULL;
    ini.WriteInt(pSection,S_MML_CMD_ADM_NUM,nCmdCnt);    
    SNPRINTF(szKey,32,"%d",nCmdCnt);
    pValue = const_cast<char *>(CmdDes.strCmdName.c_str());
    ini.WriteString(pSection,szKey,pValue);
    SNPRINTF(szKey,32,"%d.1",nCmdCnt);
    ini.WriteInt(pSection,szKey,CmdDes.nCmd);
    SNPRINTF(szKey,32,"%d.2",nCmdCnt);
    pValue = const_cast<char *>(CmdDes.strCmdDes.c_str());
    ini.WriteString(pSection,szKey,pValue);
    return ERR_SUCCESS;
}

void CCommandAdministrator::SendReportToClient(LIST<STRING> &NoteList,CParseResult *pParse)
{
    int nRes = NoteList.size();
    if(nRes > 0)
    {
        //ʧ�ܱ���
        while (NoteList.size())
        {
            m_Report.MakeReportHead();
            m_Report.MakeCmdDisplay(pParse->GetCmdLine(),0,-nRes,"MML_SERVER");
            m_Report.AddString(S_MML_NOTE_INFO);
            m_Report.AddString(STR_NEWLINE);
            m_Report.AddString(STR_TITLE_SPIN);
            m_Report.AddString(STR_NEWLINE);
            while(NoteList.size())
            {
                m_Report.AddString(NoteList.front().c_str());
                m_Report.AddString(STR_NEWLINE);
                NoteList.pop_front();
                if(m_Report.GetReportLen() > m_nPageLimit)
                {
                    if(NoteList.size())
                    {
                        m_Report.AddString(S_ALARM_TOBECONITUE);
                        m_Report.AddString(STR_NEWLINE);
                        m_Report.MakeReportTail();
                        SendToClient(pParse);
                    }
                    break;
                }
            }// while
        }//while
        m_Report.AddString(S_MML_CMD_EXE_PARTIALLY);
        m_Report.AddString(STR_NEWLINE);
    }
    else
    {
        //�ɹ�����
        m_Report.MakeReportHead();
        m_Report.MakeCmdDisplay(pParse->GetCmdLine(),0,-nRes,"MML_SERVER");
        m_Report.AddString(S_MML_SUCCEED);
    }
    m_Report.AddString(STR_NEWLINE);
    m_Report.MakeReportTail();
    SendToClient(pParse);
}

int CCommandAdministrator::MoveCmdBetweenSet(int nOutSet, int nInSet, const char *szCmdArray, LIST<STRING> &NoteList)
{
    if(nOutSet != 0 && nInSet != 0)
    {
        NoteList.push_back("Only operate command between Set0 and other Set");
        return ERR_FAIL;
    }
    MAP<STRING,UINT4>::iterator sToiIt;
    MAP<UINT4,SCMDDescription>::iterator iToCmdIt;
    MAP<UINT4,STRING>::iterator itosIt;
    int nAllocateLen = strlen(szCmdArray) + 1;
    char *pVal = new char[nAllocateLen];
    strncpy(pVal,szCmdArray,nAllocateLen);
    char *pToken = NULL;
    STRING strCmd;
    int nCmd = 0, nRet = 0, nSyncToDisk = 0;
    char szTmp[32],szSection[32];
    for(pToken = strtok(pVal,"&"); pToken; pToken = strtok(NULL,"&"))
    {
        strcpy(szTmp,pToken);
        StringLRTrim(szTmp);
        strCmd = szTmp;
        //�鿴�Ƿ��Ǽ����е�����
        sToiIt = m_NameToIdCmdMap.find(strCmd);
        if(sToiIt != m_NameToIdCmdMap.end())
        {
            nCmd = sToiIt->second;
            //�Ƿ���Ĭ�ϼ����е���������,����ʧ��
            itosIt = m_DefaultCmdMap.find(nCmd);
            if(itosIt != m_DefaultCmdMap.end())
            {
                SNPRINTF(szTmp,64,S_MML_CMD_IN_DEFAULT_SET,pToken);
                NoteList.push_back(szTmp);
                continue;
            }
            //�鿴�Ƿ��Set0�Ƴ�
            if(nOutSet == 0)
            {
                iToCmdIt = m_CmdMap0.find(nCmd);
                if(iToCmdIt == m_CmdMap0.end())
                {
                    SNPRINTF(szTmp,64,S_MML_CMD_IN_OTHER_SET,pToken);
                    NoteList.push_back(szTmp);
                    continue;
                }
            }
            else //�����������Ƴ�
            {
                iToCmdIt = m_CmdMap.find(nCmd);
                if(iToCmdIt == m_CmdMap.end())
                {
                    SNPRINTF(szTmp,64,S_MML_CMD_FIND_ERR,pToken);
                    NoteList.push_back(szTmp);
                    continue;
                }
            }
            SCMDDescription &Des = iToCmdIt->second;
            if(Des.yCmdSetId != nOutSet)
            {
                SNPRINTF(szTmp,64,S_MML_CMD_NO_IN_SET,nOutSet,pToken);
                NoteList.push_back(szTmp);
                continue;
            }
            //�����ڴ�
            Des.yCmdSetId = nInSet;
            if(nOutSet == 0)  //�����Set0���Ƴ�
            {   
                m_CmdMap[Des.nCmd] = Des;
                //ɾ�������ļ�Set0�е���Ӧ��
                m_CmdMap0.erase(Des.nCmd);
            }
            else //����Set0
            {
                m_CmdMap0[Des.nCmd] = Des;
                m_CmdMap.erase(Des.nCmd);    
            }
            nSyncToDisk = 1;
        }
        else
        {
            //���ڼ����е�����
            SNPRINTF(szTmp,64,S_MML_CMD_NONE_EXIST,pToken);
            NoteList.push_back(szTmp);
            continue;
        }    
    }
    if(nSyncToDisk)
    {
        //���Խ��������Դ���������ʱ�ļ����ڴ��ͬ��
        nAllocateLen = StatisticCmdCntOfSet(nInSet);
        int nAllocateLen2 = StatisticCmdCntOfSet(nOutSet);
        if(nAllocateLen2 > nAllocateLen)
            nAllocateLen = nAllocateLen2;
        nAllocateLen *= 256;
        nAllocateLen += 48;
        CMmlIniFile mml_ini(m_strCmdPath.c_str());
        if(!mml_ini.Open())
        {
            NoteList.push_back(S_MML_CMD_ADM_INIT_FAIL);
            delete[] pVal;
            //���Խ�������������û���ڴ��RollBack����
            return ERR_SUCCESS;
        }
        char *pSubstitution = new char[nAllocateLen];
        //��SetX����������
        //��д���뼯�ϵ������ļ�
        WriteSetToStr(nInSet,pSubstitution,nAllocateLen);
        SNPRINTF(szSection,64,"[%s%d]",S_MML_CMD_ADM_SEC,nInSet);
        nRet = mml_ini.ReplaceSecWithStr(szSection,pSubstitution);
        //���´����ļ����Ƴ������SetX
        WriteSetToStr(nOutSet,pSubstitution,nAllocateLen);
        SNPRINTF(szSection,64,"[%s%d]",S_MML_CMD_ADM_SEC,nOutSet);
        nRet = mml_ini.ReplaceSecWithStr(szSection,pSubstitution);
        if(nRet != TRUE)
        {
            m_strError = S_MML_WRITE_INI_ERR;
            m_strError += strerror(errno);
            NoteList.push_back(m_strError);
        }
        delete[] pSubstitution;
    }
    delete[] pVal;
    return ERR_SUCCESS;
}

void CCommandAdministrator::SendToClient(CParseResult *pParse)
{   
    int nLen = m_Report.GetReportLen();
    MSG_INTRA* pAns = new(nLen + 1) MSG_INTRA; 
    strcpy((char *)(pAns->Data), m_Report.GetReport());
    pAns->SenderPid   = PT_OM;
    pAns->SenderMid   = MT_MMLSERVER;
    pAns->ReceiverPid = PT_OUTSIDE;
    pAns->ReceiverMid = MT_MMLDEBUG;
    pAns->AppType     = APP_MML_NORM;
    pAns->ClientNo    = pParse->GetWorkStationNo();
    pAns->IPAddr      = pParse->GetIP();
    pAns->CmdCode     = CMD_MML;

    CMsgProcessor::SendMsg(pAns);
}
int CCommandAdministrator::QueryUserPrivilege(STRING &szUserName,char *strPI, int nLen)
{
    memset(strPI,0,nLen);
    MAP<STRING,SPrivilegeInfo>::iterator i = m_UserPrivilegeMap.find(szUserName);
    if(i != m_UserPrivilegeMap.end())
    {
        SPrivilegeInfo &PI = i->second;
        if(PI.CmdSet.empty())
        {
            strncpy(strPI,"-",nLen);
            return ERR_SUCCESS;
        }
        SET<BYTE>::iterator it = PI.CmdSet.begin();
        SET<BYTE>::iterator et = PI.CmdSet.end();
        int nLeft = nLen;
        for(it = PI.CmdSet.begin(); it != et; it++)
        {
            STRING &strCommand = m_CmdGrpDes[*it];
            if(strCommand.length() > nLeft)
            {
                strcat(strPI,"---");
                return ERR_SUCCESS;
            }
            strcat(strPI,strCommand.c_str());
            //by ldf 2004-04-07 ��Ӧ���ⵥSWPD04412

			if(strPI != "")
			{
				strcat(strPI,"^");
			}
			else
			{
				strncpy(strPI,"-",nLen);
				return ERR_SUCCESS;
			}
            
            //end
            nLeft -= strCommand.length();
            nLeft --;
        }
        strPI[nLen -1] = 0;
    }
    else
    {
        strncpy(strPI,"-",nLen);
    }
    return ERR_SUCCESS;
}
void CRightMan::ProcessPrivilegeCommand(CParseResult *pParse, MSG_INTRA *pMsg)
{
    switch(pParse->GetCmdCode())
    {
    case MML_SET_OPCG:      //���ò���Ա��Ȩ��������
        m_CmdAdmin.SetCommandForOp(pParse);
        break;
    case MML_SET_OPTM:      //���ò���Ա�Ĳ���ʱ��
        m_CmdAdmin.SetTimeLimitForOp(pParse);
        break;
    case MML_LST_OPTM:
        m_CmdAdmin.QueryOpTm(pParse);
        break;
    case MML_LST_CMDS:      //��ѯ��ǰ��ִ�е�����
        m_CmdAdmin.QueryRunCmd(pParse);
        break;
    case MML_ADD_CCG:       //����������������������
        m_CmdAdmin.AddCmdToGroup(pParse);
        break;
    case MML_RMV_CCG:       //ɾ��������������������
        m_CmdAdmin.DelCmdFromGroup(pParse);
        break;
    case MML_LST_CCG:       //��ѯ������������������
        m_CmdAdmin.QueryCmdOfGroup(pParse);
        break;
    default:
        break;
        
    }
}

int CRightMan::QueryOp(CParseResult *pParse, LIST<STRING> &ResultList)
{
    int nUserLevel = 0;
    int nState = 0;
    char szPI[80] = {0};
    char szPrintTmp[80] = {0};
    STRING strOpName = pParse->GetStringVal(1);
    STRING strOpDes  = pParse->GetStringVal(2);
    pParse->GetDigitVal(&nUserLevel,3);
    pParse->GetDigitVal(&nState,4);
    STRING strArea = pParse->GetStringVal(5);
    strncpy(szPI,pParse->GetStringVal(6),80);
    //������ 2003-03-20
    struct Condition
    {
        BYTE opEqual;
        BYTE opDesEqual;
        BYTE privilegeEqual;
        BYTE statEqual;
        BYTE areaEqual;
        BYTE setEqual;
    };
    Condition PassCond = {1,1,1,1,1,1};
    Condition CondResult;
    UINT2 uUserNum = m_pPersistUser->GetUserNum();
    SUserInfo* pUsr = (SUserInfo*)(m_pPersistUser->QueryUserInfo());

    MAP<UINT4,CLoginUser*>::const_iterator i = m_LoginUsers.begin();
    MAP<UINT4,CLoginUser*>::const_iterator e = m_LoginUsers.end();
    for(UINT2 k = 0;k < uUserNum;k++)
    {
        memset(&CondResult,0,sizeof(Condition));
        //by ldf 2004-01-30 OM����
		//cc08����ʾ���ͻ�
	   if(ACE_OS::strcasecmp(pUsr[k].szName,cc08) == 0)
           continue;
       //�ж�7������
       //1.�û����Ƿ����
       if(ACE_OS::strcasecmp(pUsr[k].szName,strOpName.c_str()) == 0
           || strOpName.compare("") == 0)
           CondResult.opEqual = 1;
       else
           CondResult.opEqual = 0;
       //2.�û�����
       if(ACE_OS::strcasecmp(pUsr[k].szFullName,strOpDes.c_str()) == 0
         || strOpDes.compare("") == 0)
           CondResult.opDesEqual = 1;
       else
           CondResult.opDesEqual = 0;
       //3.�û�����
       if(nUserLevel == 0)
           CondResult.privilegeEqual = 1;
       else
       {
           if((nUserLevel == 1 && pUsr[k].yLevel == UL_ADMIN)
             || (nUserLevel == 2 && pUsr[k].yLevel == UL_OPER))
               CondResult.privilegeEqual = 1;
           else
               CondResult.privilegeEqual = 0;
       }
       //4.����״̬
       if(nState == 0)
           CondResult.statEqual = 1;
       else
       {
           i = m_LoginUsers.begin();
           while(i != e)
           {
               if(ACE_OS::strcasecmp((*i).second->GetUserName(),
                   pUsr[k].szName) == 0)
               {
                   break;
               }
               i++;
           }
           if(i != e)
           {
               //������߾Ͳ�����һ��
               if(nState == 1)
                   CondResult.statEqual = 1;
               else
                   CondResult.statEqual = 0;
           }
           else
           {

			   //�����M2000�û���������offline�������CG�쳣�˳���mmlû�и���
			   //��ʱ��Ҫ����mml�����Ҳ���ʾ��û�н������ӵ�M2000�û�
			   if( ACE_OS::strncasecmp( pUsr[k].szName,M2000, 4) == 0 )
			   {
				   m_pPersistUser->DelUser(pUsr[k].szName);				
				   continue;
			   }

               //���������
               if(nState == 1)
                   CondResult.statEqual = 0;
               else
                   CondResult.statEqual = 1;
           }
       }
       //��������
       CondResult.areaEqual = AreaEqual(strArea,pUsr[k]);
       //Ȩ����Ϣ
       CondResult.setEqual = PrivilegeEqual(szPI,pUsr[k]);
       if(memcmp(&CondResult,&PassCond,sizeof(Condition)) == 0)
       {
           ResultList.push_back(pUsr[k].szName);
           ResultList.push_back(pUsr[k].szFullName);
           switch(nUserLevel)
           {
           case 0:
               if(pUsr[k].yLevel == UL_ADMIN)
                   ResultList.push_back(S_MML_ADMIN);
               else
                   ResultList.push_back(S_MML_NORMAL);
               break;
           case 1:
               ResultList.push_back(S_MML_ADMIN);
               break;
           default:
               ResultList.push_back(S_MML_NORMAL);
               break;
           }
           switch(nState)
           {
           case 0:
               {
                   i = m_LoginUsers.begin();
                   while(i != e)
                   {
                       if(ACE_OS::strcasecmp((*i).second->GetUserName(),
                           pUsr[k].szName) == 0)
                       {
                           break;
                       }
                       i++;
                   }
                   if(i != e)
                       ResultList.push_back(S_MML_ONLINE);
                   else
                       ResultList.push_back(S_MML_OFFLINE);
                   break;
               }
           case 1:
               ResultList.push_back(S_MML_ONLINE);
               break;
           default:
               ResultList.push_back(S_MML_OFFLINE);
           }
           //��������
           StringLRTrim(pUsr[k].szArea);
           if(strcmp(pUsr[k].szArea,"") == 0)
               ResultList.push_back("-");
           else
               ResultList.push_back(pUsr[k].szArea);
           STRING strUsrName = pUsr[k].szName;
           m_CmdAdmin.QueryUserPrivilege(strUsrName,&szPrintTmp[0],80);
           StringLRTrim(&szPrintTmp[0]);
           if(strcmp(szPrintTmp,"") == 0)
               ResultList.push_back("-");
           else
               ResultList.push_back(szPrintTmp);
		   //��������
		   char szPwdTimeLimit[6];
		   SNPRINTF(szPwdTimeLimit, sizeof(szPwdTimeLimit), "%d", pUsr[k].uPwdTimeLimit);
		   szPwdTimeLimit[5] = '\0';
		   ResultList.push_back(szPwdTimeLimit);  
		   //��½����
		   char szSession[10];
		   sprintf(szSession, "%d", pUsr[k].uCount);
		   szSession[9] = '\0';
		   ResultList.push_back(szSession);
       }
    } //for
    return ERR_SUCCESS;
}

BYTE CRightMan::AreaEqual(const STRING &strArea, const SUserInfo &usr)
{
    if(strArea.compare("") == 0 || strcmp(strArea.c_str(),usr.szArea) == 0)
        return 1;
    else
        return 0;
}

BYTE CRightMan::PrivilegeEqual(const char *szPI,const SUserInfo &usr)
{
    if(strcmp(szPI,"") == 0)
        return 1;
    else
    {
        int nComp = strcmp(szPI,"-");
        if(ACE_OS::strcasecmp(usr.szName,"admin") == 0)
        {
            if(nComp == 0)
                return 0;
            else
                return 1;
        }
        STRING strUser = usr.szName;
        SET<BYTE> CondSet;
        int nVal = 0;
        const SET<BYTE> *pSet = m_CmdAdmin.QueryUserPrivilegeSet(strUser);
        if(pSet == NULL)
        {
            //�����ѯ��Ȩ��Ϊ"-"��ʾ��ѯû��Ȩ�޵��û�
            if(nComp == 0)
                return 1;
            else
                return 0;
        }
        else
        {
            if(pSet->empty())
            {
                if(nComp == 0)
                    return 1;
                else
                    return 0;
            }
            if(nComp == 0)
                return 0;
            
            int nFlag = TRUE;
            char *pTmp = new char[strlen(szPI) + 1];
            strcpy(pTmp,szPI);
            char *pToken = NULL;
            CondSet.clear();
            for(pToken = strtok(pTmp,"&"); pToken != NULL; pToken = strtok(NULL,"&"))
            {
                nVal = atoi(pToken);
                if(nVal != 0)
                    CondSet.insert(nVal);
            }
            delete[] pTmp;
            SET<BYTE>::const_iterator it = CondSet.begin();
            SET<BYTE>::const_iterator et = CondSet.end();
            SET<BYTE>::const_iterator rt;
            SET<BYTE>::const_iterator EndIter = pSet->end();
            for(; it != et; it ++)
            {
                nVal = *it;
                rt = pSet->find(nVal);
                if(rt == EndIter)
                {
                    //ֻҪ��һ�����Ⱦ�Ҫ�˳�
                    nFlag = FALSE;
                    break;
                }
            }
            if(nFlag)
                return 1;
            else
                return 0;
        }
    }
}

const SET<BYTE> *CCommandAdministrator::QueryUserPrivilegeSet(STRING &strUser)
{
    MAP<STRING,SPrivilegeInfo>::iterator it = m_UserPrivilegeMap.find(strUser);
    if(it != m_UserPrivilegeMap.end())
    {
        SPrivilegeInfo &PI = it->second;
        return &(PI.CmdSet);
    }
    return NULL;
}

void CCommandAdministrator::WriteLog(CParseResult *pParse, int nErr, char *pTxt)
{
    SLog log;
    //������ 2003-02-18
    char szTmpUser[USER_NAME_LEN];
    strncpy(szTmpUser,pParse->GetUserName(),48);
    StringLRTrim(szTmpUser);
    if(strcmp(szTmpUser,"") == 0)
    {
        strcpy(szTmpUser,"-");
    }
    int nUserLen = strlen(szTmpUser);
    for(int i = 0; i<nUserLen; i++)
    {
        if(szTmpUser[i] == ' ')
            szTmpUser[i] = '-';
    }
    strncpy(log.szUserName,szTmpUser,48);
    //����
    log.szUserName[47] = 0;

    log.LogType = SLog::LOG_OPER;
    log.uIPAddr = pParse->GetIP();

    if(nErr == 0)
        log.OperResult = SLog::OPER_SUCCESS;
    else
        log.OperResult = SLog::OPER_FAIL;

    strncpy(log.szOperCmd,pParse->GetCmdName(),20);
    log.szOperCmd[19] = 0;

    strncpy(log.szCmdDesc,pParse->GetCmdDspName(),LOG_CMD_DESC_LEN);
    log.szCmdDesc[LOG_CMD_DESC_LEN-1] = 0;

    CMsgProcessor::WriteLog(&log);
}

int CRightMan::BreakOperator(CParseResult *pParse)
{
    //��ѡ����
    STRING strOpName = pParse->GetStringVal(1);
    int nFind = 0;
    CLoginUser *pLoginUser = NULL;
    MAP<UINT4,CLoginUser*>::iterator it;
    MAP<UINT4,CLoginUser*>::iterator et = m_LoginUsers.end();
    for(it = m_LoginUsers.begin(); it != et; it ++)
    {
        pLoginUser = it->second;
        if(StringiCmp(pLoginUser->GetUserName(),strOpName.c_str()))
        {
            //������
            nFind = 1;
            MSG_INTRA* pAns = new MSG_INTRA;
            pAns->SenderPid   = PT_OM;
            pAns->SenderMid   = MT_MMLSERVER;
            pAns->ReceiverPid = PT_OUTSIDE;
            pAns->ReceiverMid = MT_MMLDEBUG;
            pAns->AppType     = APP_MML_NORM;
            pAns->ClientNo    = pLoginUser->GetStationNo();
            pAns->IPAddr      = pLoginUser->GetIP();
            pAns->CmdCode     = CMD_BREAK_OPERATOR;
            strncpy(pAns->UserName,
                    pLoginUser->GetUserName(),
                    USER_NAME_LEN);
            CMsgProcessor::SendMsg(pAns);
        }
    }
    if(!nFind)
        return ERR_NO_CRU_USER;
    return ERR_SUCCESS;
}

MAP<STRING,SPrivilegeInfo>& CCommandAdministrator::GetUserPrivilegeMap()
{
	return m_UserPrivilegeMap;
}

int CCommandAdministrator::QueryOpTm(CParseResult *pParse)
{
    STRING strOpName = pParse->GetStringVal(1);
    if(strOpName.compare("") == 0)
    {       
        m_strError = S_MML_ERR_NAME_NULL;
        GenerateCommonReport(pParse,m_strError.c_str(),-1);
        SendToClient(pParse);
        WriteLog(pParse,-1,NULL);
        return ERR_FAIL;   
    }
    //�γɱ���
    LIST<STRING> TitleList,ContentList;
    TitleList.push_back(S_MML_BEGIN_TIME);
    TitleList.push_back(S_MML_END_TIME);
    char szTime[16];
    MAP<STRING, SPrivilegeInfo>::iterator it = m_UserPrivilegeMap.find(strOpName);
    if(it == m_UserPrivilegeMap.end())
    {
        if(m_pRight->UserIsExistent(strOpName.c_str()) != TRUE)
        {  
            m_strError = S_MML_USER_NAME_NONEXIST;
            GenerateCommonReport(pParse,S_MML_USER_NAME_NONEXIST,-1);
            SendToClient(pParse);
            WriteLog(pParse,-1,NULL);
            return ERR_FAIL;
        }
    }
    else
    {
        SPrivilegeInfo &PI = it->second;
        if(PI.nLimitType == 0) //��������ʽ
        {
			tm tmTempStart;
			tm tmTempEnd;
			tmTempStart = PI.tmStart;
			tmTempEnd = PI.tmEnd;
			
			//�ж������Ƿ�Ϸ�
			if(tmTempStart.tm_year >= 0 
				&& (tmTempStart.tm_mon >= 0 && tmTempStart.tm_mon <= 11)
				&& (tmTempStart.tm_mday >= 1 && tmTempStart.tm_mday <= 31)
				&& tmTempEnd.tm_year >= 0 
				&& (tmTempEnd.tm_mon >= 0 && tmTempEnd.tm_mon <= 11)
				&& (tmTempEnd.tm_mday >= 1 && tmTempEnd.tm_mday <= 31))
			{
				//���ںϷ�����ʱ�ް���ʽ���뵽�����У������ظ��ͻ���
				strftime(szTime,16,"%Y:%m:%d",&(PI.tmStart));
				ContentList.push_back(szTime);
				strftime(szTime,16,"%Y:%m:%d",&(PI.tmEnd));
				ContentList.push_back(szTime);
			}
			
        }
        else if(PI.nLimitType == 1)//ʱ�����ʽ
        {
			tm tmTempStart;
			tm tmTempEnd;
			tmTempStart = PI.tmStart;
			tmTempEnd = PI.tmEnd;

			//�ж�ʱ���Ƿ�Ϸ�		
			if(tmTempStart.tm_hour >= 0 && tmTempStart.tm_hour <= 23
				&& (tmTempStart.tm_min >= 0 && tmTempStart.tm_min <= 59)
				&& (tmTempStart.tm_sec >= 0 && tmTempStart.tm_sec <= 59)
				&& tmTempEnd.tm_hour >= 0 && tmTempEnd.tm_hour <= 23
				&& (tmTempEnd.tm_min >= 0 && tmTempEnd.tm_min <= 59)
				&& (tmTempEnd.tm_sec >= 0 && tmTempEnd.tm_sec <= 59))
			{
				//ʱ��Ϸ�����ʱ�ް���ʽ���뵽�����У������ظ��ͻ���
				strftime(szTime,16,"%H:%M:%S",&(PI.tmStart));
				ContentList.push_back(szTime);
				strftime(szTime,16,"%H:%M:%S",&(PI.tmEnd));
				ContentList.push_back(szTime);
			}
        }
    }
    m_Report.MakeReportHead();
    m_Report.MakeCmdDisplay(pParse->GetCmdLine(),0,0, "MML_SERVER");
    m_Report.AddHoriTable(TitleList,ContentList);
    m_Report.MakeReportTail();
    SendToClient(pParse);
    WriteLog(pParse,0,NULL);
    return ERR_SUCCESS;
}

int CCommandAdministrator::DeleteUserInfo(const char *szUserName)
{
    if(m_UserPrivilegeMap.find(szUserName) == m_UserPrivilegeMap.end())
        return ERR_SUCCESS;
    m_UserPrivilegeMap.erase(szUserName);
    //delete user info in disk
    CMmlIniFile user(m_strUserPath.c_str());
    if(user.Open() != TRUE)
    {
        TRACE(MTS_MMLSERVER,S_MML_OPEN_USER_INI_FAIL,m_strUserPath.c_str());
        return ERR_FAIL;
    }
    //����дuser.ini�ļ�
    char szSection[64];
    SNPRINTF(szSection,64,"[%s]",S_MML_CMD_ADM_HEAD);
    szSection[63] = 0;
    user.DelSection(szSection);
    SNPRINTF(szSection,64,"[%s]",szUserName);
    szSection[63] = 0;
    user.DelSection(szSection);

    user.WriteInt(S_MML_CMD_ADM_HEAD,S_MML_CMD_ADM_NUM,m_UserPrivilegeMap.size());
    MAP<STRING,SPrivilegeInfo>::iterator it = m_UserPrivilegeMap.begin();
    char szKey[16];
    char *pUserName;
    MAP<STRING,SPrivilegeInfo>::iterator et = m_UserPrivilegeMap.end();
    for(int i = 0; i < m_UserPrivilegeMap.size() && it != et; i++,it++)
    {
        pUserName = const_cast<char *>((it->first).c_str());
        SNPRINTF(szKey,16,"%d",i+1);
        user.WriteString(S_MML_CMD_ADM_HEAD,szKey,pUserName);
    }
    return ERR_SUCCESS;
}

//by ldf 2004-01-30 OM����
void CRightMan::M2000Login(CParseResult* pParse,BOOL bM2000Login)
{
    if(bM2000Login)
    {
        /*
	    ��¼�ɹ�,����CLoginUser����,��׷�ӵ�m_LoginUsers��.
	    ���Ѿ�����<����̨��>+<����̨����>��ͬ�ļ�¼,����
	    ԭ�ȵļ�¼.
        */
        UINT4 nIndex = MAKEUINT4(pParse->GetWorkStationNo(),
            pParse->GetWorkStationType());

        CLoginUser* p = NULL;
        SUserInfo usr;
        //M2000Ϊ��ͨ�û�Ȩ��
        BYTE yLevel = UL_OPER; 
        //M2000�û���Ϊ M2000(M2000IP)
        sprintf(usr.szName,"%s",pParse->GetUserName());
        strcpy(usr.szFullName,usr.szName);
        strcpy(usr.szPassword,usr.szName);
        strncpy(usr.szArea,"-",AREA_DESCRIPTION_LEN);
		usr.szArea[AREA_DESCRIPTION_LEN-1] = 0;

        time_t tNow = time(NULL);
        p = new CLoginUser(&usr,
            yLevel,
            pParse->GetWorkStationNo(),
            pParse->GetWorkStationType(),
            pParse->GetIP(),
            tNow, 0);        

        if(m_LoginUsers.find(nIndex) != m_LoginUsers.end())
        delete m_LoginUsers[nIndex];

        m_LoginUsers[nIndex] = p;

        //�����ʾM2000�û�
        m_pPersistUser->AddUser(&usr);

        //����ɹ���½��ɾ���������е���Ӧ��
        BLACKMAP::iterator it = m_BlackUser.find(pParse->GetIP());
        if(it != m_BlackUser.end())
        {
            m_BlackUser.erase(it);
        }
    }
    else
    {
        //���ʧ�ܾͼӵ���������
        BLACKMAP::iterator it = m_BlackUser.find(pParse->GetIP());
        if(it != m_BlackUser.end())
        {
            it->second ++;
            if(it->second >= m_nFailMaxTimes)
            {
                //���͸澯
                SInnerAlarm sAlarm;
                sAlarm.uAlarmID = ALARM_ID_MML_LOGIN_FAIL_MUCH;
                sAlarm.yAlarmType = AT_EVENT;
                sAlarm.yAlarmPara[0] = ALARM_PARA_LOGIN_FAIL_TOO_MUCH;
                
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
            }
        }
        else
        {
            m_BlackUser[pParse->GetIP()] = 1;
        }
    }	
}

//add by zkq 2004-07-31  ����̨��½
int CRightMan::CfgLogin(CParseResult* pParse)
{
    BOOL bSec = m_pPersistUser->ReInit(); //�û�����ˢ��
    if(!bSec)return FALSE;
    int nRet = Login(pParse);
    return nRet;
}