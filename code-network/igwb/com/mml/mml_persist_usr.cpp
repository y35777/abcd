/*
    产品名:iGateway Bill V200
    模块名:MML Server
    文件名:mml_persist_usr.h
    描述信息:
    
    包含CPersistUser类的实现,该类封装了用户数据和操作.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
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
    函数功能:初始化函数,把用户数据记载到内存中.

    1.从配置文件中读取userinfo.dat的路径,
      若读取配置文件失败就使用缺省路径.
    2.从userinfo.dat中读取用户记录.
    3.若有必要,调整userinfo.dat文件的长度.

    返回:
    成功从userinfo.dat文件中读取用户记录,返回TRUE.
    否则返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
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
	** 读取密码更换期限，配置单位为天,为0时密码永不需要更换	
	*/
	/*m_uPwdTimeLimit = pIni->GetInt(CFG_SEC_MML, 
								   CFG_MML_PWD_TIME_LIMIT,
								   CFG_MML_PWD_TIME_LIMIT_DEFAULT);

	/* 配置密码期限不得超过最大值 */
	/*if(m_uPwdTimeLimit > PWD_TIME_LIMIT_MAX)
		m_uPwdTimeLimit = PWD_TIME_LIMIT_MAX;
	
	m_uPwdTimeLimit *= ONE_DAY_IN_SECOND;*/

    delete pIni;

    if(!CreateDeepDir(szOut))
        return FALSE;

    if(m_uMaxUserNum <= 0)
        m_uMaxUserNum = MAX_USER_NUM;

    /* 配置参数不得超过最大允许值 */
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
        打开userinfo.dat文件,并读取用户记录.
    */
    m_bFlag = FALSE;
    f = fopen(m_szUserFile,"rb");

    if(f == NULL)
    {
        /* 用户文件不存在,创建新文件 */
        m_bFlag = TRUE;
        //by ldf 2004-01-30 OM整改
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
		//SCP用户,xujian,2004-09-06
        SUserInfo usrSCP;

       	usrSCP.yLevel = UL_OPER;
		strcpy(usrSCP.szName, cc08);
		strcpy(usrSCP.szFullName, cc08);
	
		if(encode(cc08Pwd, usrSCP.szPassword) != 0)
		{
			return ERR_FAIL;
		}

		memcpy(m_pUserInfos + sizeof(SUserInfo), &usrSCP, sizeof(SUserInfo));

		//by ldf 2004-01-30 OM整改
		/*
		//M2000默认用户的添加，D19629,ZYS修改
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
		//ZYS 修改结束
		*/
    }
    else
    {
		//取出Userinfo.dat文件的头四个字符
		char m_szVer[5];
		memset(m_szVer,0 ,4);
		fread(m_szVer,sizeof( char ) , 4, f);
		m_szVer[4] = '\0';

		fclose(f);
		f = fopen(m_szUserFile,"rb");
		//判断Userinfo.dat文件的头四个字符是否为"VER:"
		if(ACE_OS::strncasecmp(m_szVer, VER, 4) != 0)
		{
			//如果Userinfo.dat文件的头四个字符不是VER:，则说明SUserInfoHead结构为老版本
			SUserInfoOldHead hdr;
			if(fread(&hdr,sizeof(SUserInfoOldHead),1,f) != 1)
			{
				fclose(f);
				return FALSE;
			}
			m_uVer = 0;
			m_uUserNum = hdr.uUserNum;		        // 取出用户数 
		}
		else
		{
			//读取userinfo.dat文件中文件头的信息
			SUserInfoHead hdr;
			if(fread(&hdr,sizeof(SUserInfoHead),1,f) != 1)
			{
				fclose(f);
				return FALSE;
			}
			m_uVer = hdr.uVer;						// 取出当前SUserInfo结构的版本号
			m_uUserNum = hdr.uUserNum;		        // 取出用户数 
		}

        /* 用户文件需要扩大 */
        if(m_uMaxUserNum < m_uUserNum)
            m_bFlag = TRUE;

        if(m_uMaxUserNum < m_uUserNum)
            m_uMaxUserNum = m_uUserNum;

      	/* 读取用户记录 */
 		m_pUserInfos = new char[max(m_uMaxUserNum, m_uUserNum) * sizeof(SUserInfo)];

		m_nFileSize = FileSize(f);

		//对文件结构进行格式转换
		if(!FormatChange(m_pUserInfos))
		{
			//如果格式转换失败，则返回FALSE
			TRACE(S_APPNAME_OM, STR_USERFILE_UPDATE_FAILED);
			return FALSE;
		}
		
        fclose(f);
    }

	//by ldf 2004-01-30 OM整改，如果刚开始启动，用户数据中有M2000和emscomm用户，则删除该用户
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

    /* 刷新userinfo.dat文件. */
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
    函数功能:根据用户名获得用户记录.

    从m_pUserInfos缓冲区中找出指定用户的记录.
    用户名大小写不敏感.

    参数:
    pUserName,输入用户名.
    pUser,输出用户记录.
    调用者负责申请和释放参数占用的内存.

    返回:
    若找到用户记录,返回ERR_SUCCESS.
    否则返回ERR_NO_CRU_USER.

    修改记录:
    zhouyonghui,2001-10-24,创建.
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
    函数功能:刷新用户文件.

    把m_pUserInfos和m_uUserNum都写入用户文件中.
    若写文件失败则用户文件无变化.

    返回:成功返回TRUE,否则返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
    zhouyonghui,2001-12-12,增加如下语句:
    若用户数据文件不存在则创建.
*/
BOOL CPersistUser::RefreshUserFile()
{
    /* 在写用户文件之前首先备份 */
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
    /* 写文件失败,恢复用户文件 */
    FileCopy(szBakFile,m_szUserFile);

    return FALSE;
}

/*
    函数功能:在m_pUserInfos缓冲区中查找指定的用户记录.

    参数:
    pUserName,用户名,进行大小写不敏感比较.

    返回:成功返回用户记录指针,否则返回NULL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
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
    函数功能:增加一条用户记录.

    参数:
    pUser,用户记录.

    返回:
    1.若用户记录达到最大数,返回ERR_FULL_USER.
    2.若用户记录已经存在,返回ERR_USER_ALREADY_EXIST.
    3.若用户密码长度大于规定长度,返回ERR_PASSWD_LEN_EXCCED.
    4.成功增加记录,返回ERR_SUCCESS.
    5.其他错误返回ERR_FAIL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
int CPersistUser::AddUser(const SUserInfo *pUser)
{
    //by ldf 2004-01-30 OM整改
    /*
	if(ACE_OS::strcasecmp(pUser->szName,cc08) == 0 
		|| ACE_OS::strcasecmp(pUser->szName, emscomm) == 0)
	{
		//不能增加cc08用户和emscomm用户 
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
    //zyx 采用阿基米德算法，送过来的就是密文
	strcpy(usr.szPassword,pUser->szPassword);
	usr.tPwdLastUpdate = time(NULL);
    //地理区域
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
    函数功能:删除一条用户记录.

    1.从m_pUserInfos缓冲区中找出指定用户的记录.
    2.把待删除记录后面的所有记录都向前移一条记录.

    参数:
    pUserName,输入用户名.

    返回:
    1.若待删除的用户记录是管理员,返回ERR_NOT_ENOUGHT_RIGHT.
    2.若用户记录不存在,返回ERR_NO_CRU_USER.
    3.成功增加记录,返回ERR_SUCCESS.
    4.其他错误返回ERR_FAIL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
int CPersistUser::DelUser(const char* pUserName)
{
    //by ldf 2004-01-30 OM整改
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
    /* 刷新用户文件 */
    //张玉新 注：这里没有考虑到内存的RollBack
    if(!RefreshUserFile()) return ERR_FAIL;

    return ERR_SUCCESS;
}

/*
    函数功能:修改用户密码.

    1.从m_pUserInfos缓冲区中找出指定用户的记录.
    2.把用户记录中的密码域更新.

    参数:
    pUserName,输入用户名.
    pOldPasswd,旧密码(明文).
    pNewPasswd,新密码(明文).

    返回:
    1.若用户记录不存在,返回ERR_NO_CRU_USER.
    2.若旧密码和用户密码不一致,返回ERR_PASSWORD_ERR.
    3.若新密码长度超过规定最大长度,返回ERR_PASSWD_LEN_EXCCED
    4.其他错误返回ERR_FAIL.
    5.修改成功返回ERR_SUCCESS.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
int CPersistUser::Password(const char* pUserName,
                           const char* pOldPasswd,
                           const char* pNewPasswd)
{
    //by ldf 2004-01-30 OM整改
	if(ACE_OS::strcasecmp(pUserName,cc08) == 0
		||ACE_OS::strncasecmp(pUserName, M2000, 4) ==0)
	{
		//cc08和M2000 不能改密码
		return ERR_FAIL;
	}
    SUserInfo* p = FindUser(pUserName);

    if(p == NULL) return ERR_NO_CRU_USER;

    if(strlen(pNewPasswd) > PASSWD_LEN)
        return ERR_PASSWD_LEN_EXCCED;

    /*
        解密密码:
        int decode(char* pcaCode,char* pcaPass);
        pcaCode:密文.
        pcaPass:明文.

        加密密码:
        int encode(char* pcaPass,char* pcaCode);
        pcaPass:明文.
        pcaCode:密文.
    */
    char pwd[PASSWD_LEN + 1];
	//zyx 现在修改后送过来的新旧密码都是密文
    char pwd2[PASSWD_LEN + 1];
    memset(pwd,0,PASSWD_LEN + 1);
    memset(pwd2,0,PASSWD_LEN + 1);
	//将用户密码解密为明文
	if(decode(p->szPassword,pwd2) != 0)
		return ERR_FAIL;
	//将旧密码解密为明文
	if(decode((char *)pOldPasswd,pwd) != 0)
		return ERR_FAIL;
	//比较两个明文
	if(strcmp(pwd2,pwd) != 0)
		return ERR_PASSWORD_ERR;
	memset(p->szPassword,0,PASSWD_LEN * 2);
	//直接将新密码的密文写到用户数据中去
	strcpy(p->szPassword,pNewPasswd);
	
	//更新密码修改时间
	p->tPwdLastUpdate = time(NULL);
    
    if(!RefreshUserFile()) return ERR_FAIL;

    return ERR_SUCCESS;
}

/*
    函数功能:用户登录.
    在m_pUserInfos中查找指定用户,并验证密码.

    参数:
    pUserName,输入用户名.
    pPasswd,密码(明文).

    返回:
    1.若用户记录不存在,返回ERR_NO_CRU_USER.
    2.若密码不符,返回ERR_PASSWORD_ERR.
    3.其他错误返回ERR_FAIL.
    4.登录成功返回ERR_SUCCESS.

    修改记录:
    zhouyonghui,2001-10-24,创建.
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
	//将本地文件中的用户密码解密成明文
    if(decode(p->szPassword,pwd) != 0) return ERR_FAIL;
	//cc08 特殊处理
	if(ACE_OS::strcasecmp(pUserName,cc08) == 0)
    {
		memcpy(pwd2,pPasswd,PASSWD_LEN + 1);
		pwd2[PASSWD_LEN] = 0;
    }
	else
	{
		//将客户端送过来的密码解密成明文
		if(decode((char *)pPasswd,pwd2) !=0) return ERR_FAIL;
	}
    //比较两个明文
    if(strcmp(pwd,pwd2) != 0) return ERR_PASSWORD_ERR;

    UserLevel = p->yLevel;

	time_t tNow;
	tNow = time(NULL);

    //密码过期
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
    函数功能:修改一条用户记录.

    1.从m_pUserInfos缓冲区中找出指定用户的记录.
    2.把用户记录中的密码域和用户描述域更新.

    参数:
    pUserName,输入用户名.
    pUserDes,用户描述.
    pNewPasswd,新密码(明文).

    返回:
    1.若用户记录不存在,返回ERR_NO_CRU_USER.
    2.若新密码长度超过规定最大长度,返回ERR_PASSWD_LEN_EXCCED
    3.其他错误返回ERR_FAIL.
    4.修改成功返回ERR_SUCCESS.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
int CPersistUser::ModifyUserInfo(SUserInfo *pUser,const char* pPWDChangeFlag, const char *pNewPwd)
{
    //by ldf 2004-01-30 OM整改
	if(ACE_OS::strcasecmp(pUser->szName,cc08) == 0
		|| ACE_OS::strncasecmp(pUser->szName, M2000, 4) == 0 )
	{
		//cc08 和 M2000不能改用户信息
		return ERR_FAIL;
	}
    SUserInfo* p = FindUser(pUser->szName);

    if(p == NULL) return ERR_NO_CRU_USER;

    if(strlen(pNewPwd) > PASSWD_LEN)
        return ERR_PASSWD_LEN_EXCCED;

    pUser->yLevel = p->yLevel;

    //如果密码改变的标志为Y，则修改密码、密码更改时间以及密码保持期限。
    if(ACE_OS::strcasecmp(pPWDChangeFlag, "Y") == 0)
    {
        memset((void *)&(pUser->szPassword[0]),0,PASSWD_LEN * 2);
        //zyx 阿基米德算法加密
        strcpy(&(pUser->szPassword[0]),pNewPwd);
        /*if(encode((char*)pNewPwd,usr.szPassword) != 0)
        return ERR_FAIL;*/
        pUser->tPwdLastUpdate = time(NULL); 
    }//否则不做改动
    else
    {          
        pUser->tPwdLastUpdate = p->tPwdLastUpdate;        
        memcpy(pUser->szPassword, p->szPassword, PASSWD_LEN * 2);
    }
    /* 修改用户记录并保存 */
    memcpy(p,pUser,sizeof(SUserInfo));
    if(!RefreshUserFile()) return ERR_FAIL;

    return ERR_SUCCESS;
}


/*
    函数功能:查询所有用户记录.

    返回:
    1.返回用户信息的缓冲区m_pUserInfos.

    修改记录:
    zhouyonghui,2001-10-24,创建.
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
		//如果读取的文件类型为B01版本中SUserInfo类型，即SUserB01Info类型，则将其转换为SUserInfo类型
		if((m_nFileSize - sizeof(SUserInfoOldHead)) % sizeof(SUserB01Info) == 0 
			&& (m_nFileSize - sizeof(SUserInfoOldHead)) / sizeof(SUserB01Info) == m_uUserNum)
		{
			char* pOldInfos = new char[m_uUserNum * sizeof(SUserB01Info)];
			char* pTemp = pOldInfos;
			//旧用户数据结构，结构中无更新时间
			if(fread(pTemp, m_uUserNum * sizeof(SUserB01Info), 1, f) != 1)
			{
				fclose(f);
				delete [] pOldInfos;
				return FALSE;
			}
			
			//转换数据结构
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
		//如果读取的userinfo.dat文件结构是SUserInfo1类型的，则将其转成SUserInfo类型
		else if((m_nFileSize - sizeof(SUserInfoOldHead)) % sizeof(SUserInfo1) == 0 
			&& (m_nFileSize - sizeof(SUserInfoOldHead)) / sizeof(SUserInfo1) == m_uUserNum)
		{
			char* pOldInfos = new char[m_uUserNum * sizeof(SUserInfo1)];
			char* pTemp = pOldInfos;
			
			//旧用户数据结构，结构中无session Restricts Number
			if(fread(pTemp, m_uUserNum * sizeof(SUserInfo1), 1, f) != 1)
			{
				fclose(f);
				delete [] pOldInfos;
				return FALSE;
			}
			
			//转换数据结构
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
			
			//旧用户数据结构，结构中无session Restricts Number
			if(fread(pTemp, m_uUserNum * sizeof(SUserInfo2), 1, f) != 1)
			{
				fclose(f);
				delete [] pOldInfos;
				return FALSE;
			}
			
			//转换数据结构
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
/*	case 1:								//如果SUserInfo结构以后再更改，则在此进行对应的转换
	break;	
	case 2:
	break;
	……
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
    
    //读用户数据前清空内存中的用户数据 //by zkq 2004-08-01
    if( m_pUserInfos != NULL)
    {
		delete [] m_pUserInfos;
		m_pUserInfos = NULL;
    }   
    return Init();
}
