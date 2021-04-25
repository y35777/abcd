/*
    ��Ʒ��:iGateway Bill V200
    ģ����:MML Server
    �ļ���:mml_persist_usr.h
    ������Ϣ:
    
    ����CPersistUser���ʵ��,�����װ���û����ݺͲ���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/

#include "mml_persist_usr.h"
#include "../include/toolbox.h"
#include "../include/frame.h"
#include <assert.h>

CPersistUser* CPersistUser::g_pPersistUser = NULL;

CPersistUser::CPersistUser()
{
    m_uUserNum = 0;
    m_uMaxUserNum = 0;
    m_szUserFile[0] = 0;
    m_pUserInfos = NULL;
	m_nFileSize = 0;
}

CPersistUser::~CPersistUser()
{
    delete m_pUserInfos;
}

/*
    ��������:��ʼ������,���û����ݼ��ص��ڴ���.

    1.�������ļ��ж�ȡuserinfo.dat��·��,
      ����ȡ�����ļ�ʧ�ܾ�ʹ��ȱʡ·��.
    2.��userinfo.dat�ж�ȡ�û���¼.
    3.���б�Ҫ,����userinfo.dat�ļ��ĳ���.

    ����:
    �ɹ���userinfo.dat�ļ��ж�ȡ�û���¼,����TRUE.
    ���򷵻�FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
BOOL CPersistUser::Init()
{
    char szOut[MAX_PATH + 1];
    CINIFile* pIni = new CINIFile(GetCfgFilePath());

    pIni->Open();
    pIni->GetString(CFG_SEC_MML,              /* Section       */
                    CFG_MML_PERSIST_USER,     /* Key           */
                    CFG_MML_PERSIST_USER_DEF, /* Default value */
                    szOut,                    /* Buffer out    */
                    MAX_PATH);                /* Buffer len    */

    m_uMaxUserNum = pIni->GetInt(CFG_SEC_MML,CFG_MML_MAX_USER_NUMBER,20);


	/*
	** ��ȡ����������ޣ����õ�λΪ��,Ϊ0ʱ����������Ҫ����	
	*/
	/*m_uPwdTimeLimit = pIni->GetInt(CFG_SEC_MML, 
								   CFG_MML_PWD_TIME_LIMIT,
								   CFG_MML_PWD_TIME_LIMIT_DEFAULT);

	/* �����������޲��ó������ֵ */
	/*if(m_uPwdTimeLimit > PWD_TIME_LIMIT_MAX)
		m_uPwdTimeLimit = PWD_TIME_LIMIT_MAX;
	
	m_uPwdTimeLimit *= ONE_DAY_IN_SECOND;*/

    delete pIni;

    if(!CreateDeepDir(szOut))
        return FALSE;

    if(m_uMaxUserNum <= 0)
        m_uMaxUserNum = MAX_USER_NUM;

    /* ���ò������ó����������ֵ */
    if(m_uMaxUserNum > MAX_USER_NUM)
       m_uMaxUserNum = MAX_USER_NUM;

	m_uMaxUserNum += INNER_USER_NUM;

    if((szOut[strlen(szOut) - 1] == '/')
        || (szOut[strlen(szOut) - 1] == '\\'))
    {
        sprintf(m_szUserFile,"%suserinfo.dat",szOut);
    }
    else
    {
        sprintf(m_szUserFile,"%s/userinfo.dat",szOut);
    }

    /*
        ��userinfo.dat�ļ�,����ȡ�û���¼.
    */
    m_bFlag = FALSE;
    f = fopen(m_szUserFile,"rb");

    if(f == NULL)
    {
        /* �û��ļ�������,�������ļ� */
        m_bFlag = TRUE;
        //by ldf 2004-01-30 OM����
        m_uUserNum = 2;
        //end

        m_pUserInfos = new char[m_uMaxUserNum * sizeof(SUserInfo)];

        SUserInfo usrAdmin;

        strcpy(usrAdmin.szName,"admin");
        strcpy(usrAdmin.szFullName,"Administrator");
        usrAdmin.yLevel = UL_ADMIN;

        if(encode("", usrAdmin.szPassword) != 0)
		{
			return ERR_FAIL;
		}
		
        memcpy(m_pUserInfos,&usrAdmin,sizeof(SUserInfo));
		//SCP�û�,xujian,2004-09-06
        SUserInfo usrSCP;

       	usrSCP.yLevel = UL_OPER;
		strcpy(usrSCP.szName, cc08);
		strcpy(usrSCP.szFullName, cc08);
	
		if(encode(cc08Pwd, usrSCP.szPassword) != 0)
		{
			return ERR_FAIL;
		}

		memcpy(m_pUserInfos + sizeof(SUserInfo), &usrSCP, sizeof(SUserInfo));

		//by ldf 2004-01-30 OM����
		/*
		//M2000Ĭ���û�����ӣ�D19629,ZYS�޸�
		memset(&usr, 0, sizeof(SUserInfo));
       	usr.yLevel = UL_OPER;
		strcpy(usr.szName, emscomm);
		strcpy(usr.szFullName, emscomm);
	
		if(encode(emscomm, usr.szPassword) != 0)
		{
			return ERR_FAIL;
		}

		usr.tPwdLastUpdate = time(NULL);
		memcpy(m_pUserInfos + 2 * sizeof(SUserInfo), &usr, sizeof(SUserInfo));
		//ZYS �޸Ľ���
		*/
    }
    else
    {
		//ȡ��Userinfo.dat�ļ���ͷ�ĸ��ַ�
		char m_szVer[5];
		memset(m_szVer,0 ,4);
		fread(m_szVer,sizeof( char ) , 4, f);
		m_szVer[4] = '\0';

		fclose(f);
		f = fopen(m_szUserFile,"rb");
		//�ж�Userinfo.dat�ļ���ͷ�ĸ��ַ��Ƿ�Ϊ"VER:"
		if(ACE_OS::strncasecmp(m_szVer, VER, 4) != 0)
		{
			//���Userinfo.dat�ļ���ͷ�ĸ��ַ�����VER:����˵��SUserInfoHead�ṹΪ�ϰ汾
			SUserInfoOldHead hdr;
			if(fread(&hdr,sizeof(SUserInfoOldHead),1,f) != 1)
			{
				fclose(f);
				return FALSE;
			}
			m_uVer = 0;
			m_uUserNum = hdr.uUserNum;		        // ȡ���û��� 
		}
		else
		{
			//��ȡuserinfo.dat�ļ����ļ�ͷ����Ϣ
			SUserInfoHead hdr;
			if(fread(&hdr,sizeof(SUserInfoHead),1,f) != 1)
			{
				fclose(f);
				return FALSE;
			}
			m_uVer = hdr.uVer;						// ȡ����ǰSUserInfo�ṹ�İ汾��
			m_uUserNum = hdr.uUserNum;		        // ȡ���û��� 
		}

        /* �û��ļ���Ҫ���� */
        if(m_uMaxUserNum < m_uUserNum)
            m_bFlag = TRUE;

        if(m_uMaxUserNum < m_uUserNum)
            m_uMaxUserNum = m_uUserNum;

      	/* ��ȡ�û���¼ */
 		m_pUserInfos = new char[max(m_uMaxUserNum, m_uUserNum) * sizeof(SUserInfo)];

		m_nFileSize = FileSize(f);

		//���ļ��ṹ���и�ʽת��
		if(!FormatChange(m_pUserInfos))
		{
			//�����ʽת��ʧ�ܣ��򷵻�FALSE
			TRACE(S_APPNAME_OM, STR_USERFILE_UPDATE_FAILED);
			return FALSE;
		}
		
        fclose(f);
    }

	//by ldf 2004-01-30 OM���ģ�����տ�ʼ�������û���������M2000��emscomm�û�����ɾ�����û�
	SUserInfo* pUsr = (SUserInfo*)(m_pUserInfos);
	for(UINT2 k = 0;k < m_uUserNum;k++)
	{
		if( ACE_OS::strncasecmp( pUsr[k].szName,M2000, 4) == 0 
			|| ACE_OS::strncasecmp( pUsr[k].szName,emscomm, 4) == 0)
		{
			int nRet = DelUser(pUsr[k].szName);
			if(nRet == ERR_SUCCESS)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}

		}	
	}
	//end

    /* ˢ��userinfo.dat�ļ�. */
    if(m_bFlag == TRUE)
        return RefreshUserFile();

    return TRUE;
}

CPersistUser* CPersistUser::Instance()
{
    if(g_pPersistUser == NULL)
    {
        g_pPersistUser = new CPersistUser();
        if(!g_pPersistUser->Init())
        {
            delete g_pPersistUser;
            g_pPersistUser = NULL;
        }
    }

    return g_pPersistUser;
}

void CPersistUser::Destroy()
{
    if(g_pPersistUser != NULL)
    {
        delete g_pPersistUser;
        g_pPersistUser = NULL;
    }
}

/*
    ��������:�����û�������û���¼.

    ��m_pUserInfos���������ҳ�ָ���û��ļ�¼.
    �û�����Сд������.

    ����:
    pUserName,�����û���.
    pUser,����û���¼.
    �����߸���������ͷŲ���ռ�õ��ڴ�.

    ����:
    ���ҵ��û���¼,����ERR_SUCCESS.
    ���򷵻�ERR_NO_CRU_USER.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistUser::GetUserInfo(const char* pUserName,
                                SUserInfo* pUser)
{
    SUserInfo* p = FindUser(pUserName);

    if(p == NULL) return ERR_NO_CRU_USER;

    memcpy(pUser,p,sizeof(SUserInfo));
    return ERR_SUCCESS;
}


/*
    ��������:ˢ���û��ļ�.

    ��m_pUserInfos��m_uUserNum��д���û��ļ���.
    ��д�ļ�ʧ�����û��ļ��ޱ仯.

    ����:�ɹ�����TRUE,���򷵻�FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
    zhouyonghui,2001-12-12,�����������:
    ���û������ļ��������򴴽�.
*/
BOOL CPersistUser::RefreshUserFile()
{
    /* ��д�û��ļ�֮ǰ���ȱ��� */
    char szBakFile[MAX_PATH],tmp[MAX_PATH];

    char* p = strstr(m_szUserFile,"userinfo.dat");
    if(p == NULL) return FALSE;

    memcpy(tmp,m_szUserFile,p - m_szUserFile);
    tmp[p - m_szUserFile] = 0;
    sprintf(szBakFile,"%suserinfo.bak",tmp);

    if(!FileCopy(m_szUserFile,szBakFile))
    {
        if(!ACE_OS::access(m_szUserFile,F_OK))
            return FALSE;
    }

    FILE* f = fopen(m_szUserFile,"wb");
    if(f == NULL) return FALSE;

    SUserInfoHead hdr;
	memset(&hdr,0,sizeof(SUserInfoHead));
    hdr.uUserNum = m_uUserNum;
	hdr.uVer = VEROFSUSERINFO;		
    strncpy(hdr.szVer,VER, 4);
    if(fwrite(&hdr,sizeof(SUserInfoHead),1,f) == 1)
    {
        if(fwrite(m_pUserInfos,m_uUserNum * sizeof(SUserInfo),1,f)
            == 1)
        {
            fclose(f);
            return TRUE;
        }
    }

    fclose(f);
    /* д�ļ�ʧ��,�ָ��û��ļ� */
    FileCopy(szBakFile,m_szUserFile);

    return FALSE;
}

/*
    ��������:��m_pUserInfos�������в���ָ�����û���¼.

    ����:
    pUserName,�û���,���д�Сд�����бȽ�.

    ����:�ɹ������û���¼ָ��,���򷵻�NULL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
SUserInfo* CPersistUser::FindUser(const char *pUserName)
{
    SUserInfo* p = (SUserInfo*)m_pUserInfos;

    for(int i = 0;i < m_uUserNum;i++)
    {
        if(ACE_OS::strcasecmp(p[i].szName,pUserName) == 0)
            return &p[i];
    }

    return NULL;
}

/*
    ��������:����һ���û���¼.

    ����:
    pUser,�û���¼.

    ����:
    1.���û���¼�ﵽ�����,����ERR_FULL_USER.
    2.���û���¼�Ѿ�����,����ERR_USER_ALREADY_EXIST.
    3.���û����볤�ȴ��ڹ涨����,����ERR_PASSWD_LEN_EXCCED.
    4.�ɹ����Ӽ�¼,����ERR_SUCCESS.
    5.�������󷵻�ERR_FAIL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistUser::AddUser(const SUserInfo *pUser)
{
    //by ldf 2004-01-30 OM����
    /*
	if(ACE_OS::strcasecmp(pUser->szName,cc08) == 0 
		|| ACE_OS::strcasecmp(pUser->szName, emscomm) == 0)
	{
		//��������cc08�û���emscomm�û� 
		return ERR_NOT_ENOUGHT_RIGHT;
	}
    */

    if(FindUser(pUser->szName))
        return ERR_USER_ALREADY_EXIST;

    if(m_uUserNum >= m_uMaxUserNum)
        return ERR_FULL_USER;

    if(strlen(pUser->szPassword) > PASSWD_LEN)
        return ERR_PASSWD_LEN_EXCCED;

    SUserInfo usr;

    memset(&usr,0,sizeof(SUserInfo));

    strcpy(usr.szName,pUser->szName);
    strcpy(usr.szFullName,pUser->szFullName);
    usr.yLevel = pUser->yLevel;
    //zyx ���ð����׵��㷨���͹����ľ�������
	strcpy(usr.szPassword,pUser->szPassword);
	usr.tPwdLastUpdate = time(NULL);
    //��������
    strncpy(usr.szArea,pUser->szArea,AREA_DESCRIPTION_LEN);
    usr.uPwdTimeLimit = pUser->uPwdTimeLimit;   
	usr.uCount = pUser->uCount;
    SUserInfo* pInfoBuf = (SUserInfo*)m_pUserInfos;

    memcpy(&pInfoBuf[m_uUserNum],&usr,sizeof(SUserInfo));
    m_uUserNum++;

    if(!RefreshUserFile())
    {
        m_uUserNum--;
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

/*
    ��������:ɾ��һ���û���¼.

    1.��m_pUserInfos���������ҳ�ָ���û��ļ�¼.
    2.�Ѵ�ɾ����¼��������м�¼����ǰ��һ����¼.

    ����:
    pUserName,�����û���.

    ����:
    1.����ɾ�����û���¼�ǹ���Ա,����ERR_NOT_ENOUGHT_RIGHT.
    2.���û���¼������,����ERR_NO_CRU_USER.
    3.�ɹ����Ӽ�¼,����ERR_SUCCESS.
    4.�������󷵻�ERR_FAIL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistUser::DelUser(const char* pUserName)
{
    //by ldf 2004-01-30 OM����
    if(ACE_OS::strcasecmp(pUserName,"admin") == 0 
	   || ACE_OS::strcasecmp(pUserName,cc08) == 0)
        return ERR_NOT_ENOUGHT_RIGHT;
    //end

    SUserInfo* p = FindUser(pUserName);

    if(p == NULL) return ERR_NO_CRU_USER;

    UINT2 nIndex = 
        (unsigned int)((char*)p - m_pUserInfos) / sizeof(SUserInfo);
    assert(nIndex < m_uUserNum);

    SUserInfo* pBuf = (SUserInfo*)m_pUserInfos;
    for(int i = nIndex;i < m_uUserNum - 1;i++)
    {
        memcpy(&pBuf[i],&pBuf[i + 1],sizeof(SUserInfo));
    }
    m_uUserNum--;
    /* ˢ���û��ļ� */
    //������ ע������û�п��ǵ��ڴ��RollBack
    if(!RefreshUserFile()) return ERR_FAIL;

    return ERR_SUCCESS;
}

/*
    ��������:�޸��û�����.

    1.��m_pUserInfos���������ҳ�ָ���û��ļ�¼.
    2.���û���¼�е����������.

    ����:
    pUserName,�����û���.
    pOldPasswd,������(����).
    pNewPasswd,������(����).

    ����:
    1.���û���¼������,����ERR_NO_CRU_USER.
    2.����������û����벻һ��,����ERR_PASSWORD_ERR.
    3.�������볤�ȳ����涨��󳤶�,����ERR_PASSWD_LEN_EXCCED
    4.�������󷵻�ERR_FAIL.
    5.�޸ĳɹ�����ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistUser::Password(const char* pUserName,
                           const char* pOldPasswd,
                           const char* pNewPasswd)
{
    //by ldf 2004-01-30 OM����
	if(ACE_OS::strcasecmp(pUserName,cc08) == 0
		||ACE_OS::strncasecmp(pUserName, M2000, 4) ==0)
	{
		//cc08��M2000 ���ܸ�����
		return ERR_FAIL;
	}
    SUserInfo* p = FindUser(pUserName);

    if(p == NULL) return ERR_NO_CRU_USER;

    if(strlen(pNewPasswd) > PASSWD_LEN)
        return ERR_PASSWD_LEN_EXCCED;

    /*
        ��������:
        int decode(char* pcaCode,char* pcaPass);
        pcaCode:����.
        pcaPass:����.

        ��������:
        int encode(char* pcaPass,char* pcaCode);
        pcaPass:����.
        pcaCode:����.
    */
    char pwd[PASSWD_LEN + 1];
	//zyx �����޸ĺ��͹������¾����붼������
    char pwd2[PASSWD_LEN + 1];
    memset(pwd,0,PASSWD_LEN + 1);
    memset(pwd2,0,PASSWD_LEN + 1);
	//���û��������Ϊ����
	if(decode(p->szPassword,pwd2) != 0)
		return ERR_FAIL;
	//�����������Ϊ����
	if(decode((char *)pOldPasswd,pwd) != 0)
		return ERR_FAIL;
	//�Ƚ���������
	if(strcmp(pwd2,pwd) != 0)
		return ERR_PASSWORD_ERR;
	memset(p->szPassword,0,PASSWD_LEN * 2);
	//ֱ�ӽ������������д���û�������ȥ
	strcpy(p->szPassword,pNewPasswd);
	
	//���������޸�ʱ��
	p->tPwdLastUpdate = time(NULL);
    
    if(!RefreshUserFile()) return ERR_FAIL;

    return ERR_SUCCESS;
}

/*
    ��������:�û���¼.
    ��m_pUserInfos�в���ָ���û�,����֤����.

    ����:
    pUserName,�����û���.
    pPasswd,����(����).

    ����:
    1.���û���¼������,����ERR_NO_CRU_USER.
    2.�����벻��,����ERR_PASSWORD_ERR.
    3.�������󷵻�ERR_FAIL.
    4.��¼�ɹ�����ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistUser::Login(const char* pUserName,
                        const char* pPasswd,
                        BYTE& UserLevel)
{
    SUserInfo* p = FindUser(pUserName);

    if(p == NULL) return ERR_NO_CRU_USER;

    char pwd[PASSWD_LEN + 1];
    char pwd2[PASSWD_LEN + 1];
    memset(pwd, 0,PASSWD_LEN + 1);
    memset(pwd2,0,PASSWD_LEN + 1);
	//zyx 
	//�������ļ��е��û�������ܳ�����
    if(decode(p->szPassword,pwd) != 0) return ERR_FAIL;
	//cc08 ���⴦��
	if(ACE_OS::strcasecmp(pUserName,cc08) == 0)
    {
		memcpy(pwd2,pPasswd,PASSWD_LEN + 1);
		pwd2[PASSWD_LEN] = 0;
    }
	else
	{
		//���ͻ����͹�����������ܳ�����
		if(decode((char *)pPasswd,pwd2) !=0) return ERR_FAIL;
	}
    //�Ƚ���������
    if(strcmp(pwd,pwd2) != 0) return ERR_PASSWORD_ERR;

    UserLevel = p->yLevel;

	time_t tNow;
	tNow = time(NULL);

    //�������
    if((tNow - p->tPwdLastUpdate) > (p->uPwdTimeLimit) * ONE_DAY_IN_SECOND)
    {
        return ERR_PWD_OUTDATED;
    }
    return ERR_SUCCESS;
}


UINT2 CPersistUser::GetMaxUserNum()
{
    return m_uMaxUserNum;
}

/*
    ��������:�޸�һ���û���¼.

    1.��m_pUserInfos���������ҳ�ָ���û��ļ�¼.
    2.���û���¼�е���������û����������.

    ����:
    pUserName,�����û���.
    pUserDes,�û�����.
    pNewPasswd,������(����).

    ����:
    1.���û���¼������,����ERR_NO_CRU_USER.
    2.�������볤�ȳ����涨��󳤶�,����ERR_PASSWD_LEN_EXCCED
    3.�������󷵻�ERR_FAIL.
    4.�޸ĳɹ�����ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistUser::ModifyUserInfo(SUserInfo *pUser,const char* pPWDChangeFlag, const char *pNewPwd)
{
    //by ldf 2004-01-30 OM����
	if(ACE_OS::strcasecmp(pUser->szName,cc08) == 0
		|| ACE_OS::strncasecmp(pUser->szName, M2000, 4) == 0 )
	{
		//cc08 �� M2000���ܸ��û���Ϣ
		return ERR_FAIL;
	}
    SUserInfo* p = FindUser(pUser->szName);

    if(p == NULL) return ERR_NO_CRU_USER;

    if(strlen(pNewPwd) > PASSWD_LEN)
        return ERR_PASSWD_LEN_EXCCED;

    pUser->yLevel = p->yLevel;

    //�������ı�ı�־ΪY�����޸����롢�������ʱ���Լ����뱣�����ޡ�
    if(ACE_OS::strcasecmp(pPWDChangeFlag, "Y") == 0)
    {
        memset((void *)&(pUser->szPassword[0]),0,PASSWD_LEN * 2);
        //zyx �����׵��㷨����
        strcpy(&(pUser->szPassword[0]),pNewPwd);
        /*if(encode((char*)pNewPwd,usr.szPassword) != 0)
        return ERR_FAIL;*/
        pUser->tPwdLastUpdate = time(NULL); 
    }//�������Ķ�
    else
    {          
        pUser->tPwdLastUpdate = p->tPwdLastUpdate;        
        memcpy(pUser->szPassword, p->szPassword, PASSWD_LEN * 2);
    }
    /* �޸��û���¼������ */
    memcpy(p,pUser,sizeof(SUserInfo));
    if(!RefreshUserFile()) return ERR_FAIL;

    return ERR_SUCCESS;
}


/*
    ��������:��ѯ�����û���¼.

    ����:
    1.�����û���Ϣ�Ļ�����m_pUserInfos.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
char* CPersistUser::QueryUserInfo()
{
    return m_pUserInfos;
}


UINT2 CPersistUser::GetUserNum()
{
    return m_uUserNum;
}

BOOL CPersistUser::FormatChange(char * pUserInfo)
{
	switch(m_uVer)
	{
	case 0:
		//�����ȡ���ļ�����ΪB01�汾��SUserInfo���ͣ���SUserB01Info���ͣ�����ת��ΪSUserInfo����
		if((m_nFileSize - sizeof(SUserInfoOldHead)) % sizeof(SUserB01Info) == 0 
			&& (m_nFileSize - sizeof(SUserInfoOldHead)) / sizeof(SUserB01Info) == m_uUserNum)
		{
			char* pOldInfos = new char[m_uUserNum * sizeof(SUserB01Info)];
			char* pTemp = pOldInfos;
			//���û����ݽṹ���ṹ���޸���ʱ��
			if(fread(pTemp, m_uUserNum * sizeof(SUserB01Info), 1, f) != 1)
			{
				fclose(f);
				delete [] pOldInfos;
				return FALSE;
			}
			
			//ת�����ݽṹ
			for(int i = 0; i < m_uUserNum; i++)
			{
				SUserInfo tmpUser;
				SUserB01Info tmpB01User;
				memcpy(&tmpUser, pTemp, sizeof(SUserB01Info));
				memcpy(&tmpB01User, pTemp, sizeof(SUserB01Info));
				tmpUser.yLevel = tmpB01User.yLevel;
				strncpy(tmpUser.szArea, "-", AREA_DESCRIPTION_LEN);
/*				memset((char*)&tmpUser + sizeof(SUserB01Info),
					0,
					sizeof(tmpUser) - sizeof(SUserInfo));*/
				pTemp += sizeof(SUserB01Info);
				memcpy(m_pUserInfos + i*sizeof(SUserInfo), &tmpUser, sizeof(SUserInfo));
			}
			
			delete [] pOldInfos;
			m_bFlag = TRUE;
		}
		//�����ȡ��userinfo.dat�ļ��ṹ��SUserInfo1���͵ģ�����ת��SUserInfo����
		else if((m_nFileSize - sizeof(SUserInfoOldHead)) % sizeof(SUserInfo1) == 0 
			&& (m_nFileSize - sizeof(SUserInfoOldHead)) / sizeof(SUserInfo1) == m_uUserNum)
		{
			char* pOldInfos = new char[m_uUserNum * sizeof(SUserInfo1)];
			char* pTemp = pOldInfos;
			
			//���û����ݽṹ���ṹ����session Restricts Number
			if(fread(pTemp, m_uUserNum * sizeof(SUserInfo1), 1, f) != 1)
			{
				fclose(f);
				delete [] pOldInfos;
				return FALSE;
			}
			
			//ת�����ݽṹ
			for(int i = 0; i < m_uUserNum; i++)
			{
				SUserInfo tmpUser;
				memcpy(&tmpUser, pTemp, sizeof(SUserInfo1));
				pTemp += sizeof(SUserInfo1);
				memcpy(m_pUserInfos + i*sizeof(SUserInfo), &tmpUser, sizeof(SUserInfo));
			}
			
			delete [] pOldInfos;
			m_bFlag = TRUE;
		}
		else
		{
			char* pOldInfos = new char[m_uUserNum * sizeof(SUserInfo2)];
			char* pTemp = pOldInfos;
			
			//���û����ݽṹ���ṹ����session Restricts Number
			if(fread(pTemp, m_uUserNum * sizeof(SUserInfo2), 1, f) != 1)
			{
				fclose(f);
				delete [] pOldInfos;
				return FALSE;
			}
			
			//ת�����ݽṹ
			for(int i = 0; i < m_uUserNum; i++)
			{
				SUserInfo tmpUser;
				memcpy(&tmpUser, pTemp, sizeof(SUserInfo2));
				pTemp += sizeof(SUserInfo2);
				memcpy(m_pUserInfos + i*sizeof(SUserInfo2), &tmpUser, sizeof(SUserInfo));
			}
			
			delete [] pOldInfos;
			m_bFlag = TRUE;
			
		}
		break;
/*	case 1:								//���SUserInfo�ṹ�Ժ��ٸ��ģ����ڴ˽��ж�Ӧ��ת��
	break;	
	case 2:
	break;
	����
    */
	case VEROFSUSERINFO:								
		if(fread(pUserInfo,m_uUserNum * sizeof(SUserInfo),1,f) != 1)
		{
			fclose(f);
			return FALSE;
		}
		break;
	default:
		break;
	}

	return TRUE;
}

//add by zkq 2004-08-01
BOOL CPersistUser::ReInit()
{
    
    //���û�����ǰ����ڴ��е��û����� //by zkq 2004-08-01
    if( m_pUserInfos != NULL)
    {
		delete [] m_pUserInfos;
		m_pUserInfos = NULL;
    }   
    return Init();
}
