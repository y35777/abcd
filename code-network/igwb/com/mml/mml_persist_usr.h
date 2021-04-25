 /*
    ��Ʒ��:iGateway Bill V200
    ģ����:MML Server
    �ļ���:mml_persist_usr.h
    ������Ϣ:
    
    ����CPersistUser��Ķ���,�����װ���û����ݺͲ���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/

#ifndef _MML_PERSIST_USER_H
#define _MML_PERSIST_USER_H

#include "../include/precom.h"
#include "../utility/mml_para.h"
#include "../utility/encrypt.h"

#define cc08     "cc08"
#define cc08Pwd  "cc08"
//by ldf 2004-01-30 OM����
#define M2000  "M2000"  
#define VER    "VER:"
#define emscomm  "emscomm"
//end  

#define INNER_USER_NUM         2 
#define PWD_TIME_LIMIT_MAX     365

//SUserInfo�ṹ�İ汾��, ���SUserInfo�ṹ���˸Ķ�����VEROFSUSERINFO++
#define VEROFSUSERINFO		1

//zyx modify PASSWD_LEN for ajimide
#define PASSWD_LEN              128         //�û�������󳤶�
#define USER_NAME_LEN           48          //�û�����󳤶�
#define USER_FULL_NAEM_LEN      100         //�û�ȫ����󳤶�
#define AREA_DESCRIPTION_LEN    128         //��������������󳤶�
#define MAX_USER_NUM    30                  //ϵͳ������������û���

/* userinfo.dat�ļ�ͷ�Ͻṹ */
typedef struct  _UserInfoOldHead
{
	
#define MAX_USER_NUM    30                  //ϵͳ������������û���
    UINT2 uUserNum;                         //�ļ����û���¼��
	
}SUserInfoOldHead;

/* userinfo.dat�ļ�ͷ�ṹ */
typedef struct  _UserInfoHead
{
	char  szVer[4];							//ע��"VER:"
    UINT2 uUserNum;                         //�ļ����û���¼��
	UINT2 uVer;								//SUserInfo�ṹ�İ汾��

}SUserInfoHead;

//B01�汾userinfo.dat�ļ���¼�ṹ
typedef struct  _SUserB01Info
{
    char szName[USER_NAME_LEN];             //MML�ͻ����û���
    char szFullName[USER_FULL_NAEM_LEN];    //MML�ͻ����û�ȫ��
    char szPassword[PASSWD_LEN * 2];        //�û�����
	
    BYTE yLevel;                            //�û���������,
	//ȡֵΪUSER_LEVELö��
} SUserB01Info;

/* C32B001�汾userinfo.dat�ļ���¼�ṹ */
typedef struct  _UserInfo1
{
    char szName[USER_NAME_LEN];             //MML�ͻ����û���
    char szFullName[USER_FULL_NAEM_LEN];    //MML�ͻ����û�ȫ��
    char szPassword[PASSWD_LEN * 2];        //�û�����
	
    char szArea[AREA_DESCRIPTION_LEN];      //�û���������
    BYTE yLevel;                            //�û���������, ȡֵΪUSER_LEVELö��
	time_t tPwdLastUpdate;                  //�������ʱ��
    UINT4   uPwdTimeLimit;                  //�������ʱ��
	
} SUserInfo1;

/* C32B002/C32B003�汾userinfo.dat�ļ���¼�ṹ����Ϊ��C32B004�汾��ʼ��MMLƽ���������д���
   SUserInfo�ṹ�����ı仯ֱ����SUserInfo�����Ķ�����Ҳ����C32B002/C32B003�汾userinfo.dat
   �ļ���¼�ṹ�����ڶ�ȡC32B002/C32B003�汾��userinfo.dat�ļ�ʱ���ܰ��մ˽ṹ��ȷ�ض���
   �ļ�
*/
typedef struct  _UserInfo2
{
    char szName[USER_NAME_LEN];             //MML�ͻ����û���
    char szFullName[USER_FULL_NAEM_LEN];    //MML�ͻ����û�ȫ��
    char szPassword[PASSWD_LEN * 2];        //�û�����
	
    char szArea[AREA_DESCRIPTION_LEN];      //�û���������
    BYTE yLevel;                            //�û���������, ȡֵΪUSER_LEVELö��
	time_t tPwdLastUpdate;                  //�������ʱ��
    UINT4   uPwdTimeLimit;                  //�������ʱ��
	UINT4   uCount;							//session Restricts Number

} SUserInfo2;

/* ��ǰ�汾��userinfo.dat�ļ���¼�ṹ 
   ���SUserInfo�ṹ�����˱仯������Ҫ�������޸ģ�
   1)��ԭSUserInfo�ṹ�Ķ�����ֱ���и��ġ����ڹ��캯�������ȱʡֵ��
   2)����ԭSUserInfo�ṹ�Ķ��壬��Ҫ��������SUserInfoV01��
   3)�޸�mml_persist_usr.cpp�ļ��е�FormatChange��������switch(m_uVer)��֧����Ӷ�ǰһ����
     ����SUserInfo���ɵ�Userinfo.dat�ļ��Ĵ�������һ���汾ΪSUserInfoV01���͵Ľṹ�������
   case 1:{���и�ʽת��}���޸ķ����ɲο�����case��֧��
   4)#define VEROFSUserInfo		1��䡣��VEROFSUserInfo�Լ�һ�Ρ����Ϊ
     #define VEROFSUserInfo	    2
*/
typedef struct  _UserInfo
{
    char szName[USER_NAME_LEN];             //MML�ͻ����û���
    char szFullName[USER_FULL_NAEM_LEN];    //MML�ͻ����û�ȫ��
    char szPassword[PASSWD_LEN * 2];        //�û�����

    char szArea[AREA_DESCRIPTION_LEN];      //�û���������
    BYTE yLevel;                            //�û���������, ȡֵΪUSER_LEVELö��
	time_t tPwdLastUpdate;                  //�������ʱ��
    UINT4   uPwdTimeLimit;                  //�������ʱ��
	UINT4   uCount;							//session Restricts Number
    
	//ÿ��SUserInfo�汾�Ķ������޸Ĵ˹��캯����Ϊ�������ֶθ�ȱʡֵ��
	_UserInfo()
	{
		memset(szName, 0, USER_NAME_LEN);
		memset(szFullName, 0, USER_FULL_NAEM_LEN);
		memset(szPassword, 0, PASSWD_LEN * 2);
		memset(szArea, 0, AREA_DESCRIPTION_LEN);
//		memcpy
		yLevel = 0;
		tPwdLastUpdate = time(NULL);
		uPwdTimeLimit = 30;
		uCount = 30;
	}

} SUserInfo;

class CPersistUser
{
protected:
    /*
        �û���Ϣ�������ļ�ȫ·����,
        ����·���ڳ�ʼ��ʱ�����ò�������,
        �ļ���Ϊuserinfo.dat��
    */
    char m_szUserFile[MAX_PATH];

    /*
        ϵͳĿǰ���õ��û���,���û������ļ���ͷ�����ֽڶ���.
    */
    UINT2 m_uUserNum;

    /*
        ϵͳ����������õ��û���,�ڳ�ʼ��ʱ�����ò�������,
        Ĭ��ֵΪ20.
    */
    UINT2 m_uMaxUserNum;

    /*
        �û���Ϣ������ָ��,�ڹ��캯���з���ռ�,����ʱ�ͷ�,
        �ռ��СΪ:
        m_uMaxUserNum * sizeof(SUserInfo) + sizeof(SUserInfoHead)
    */
    char* m_pUserInfos;

	/*
	    ����������ޣ�Ĭ����һ���£����������޸ģ����Ϊһ��
		Ϊ0ʱ��������
	*/
	UINT4 m_uPwdTimeLimit;


    static CPersistUser* g_pPersistUser;

	//�ļ��Ĵ�С
	long m_nFileSize;

	//�Ƿ���Ҫˢ��userinfo.dat�ļ�
	BOOL m_bFlag;

	FILE* f;

	//SUserInfo�ṹ�İ汾��
	UINT2 m_uVer;

protected:
    CPersistUser();
    virtual ~CPersistUser();

public:
    UINT2 GetUserNum();
    char* QueryUserInfo();
    int ModifyUserInfo(SUserInfo *pUser,const char* pPWDChangeFlag, const char *pNewPwd);    
    UINT2 GetMaxUserNum();
    static CPersistUser* Instance();
    static void Destroy();

    /* ��ʼ������,���û���Ϣ���ص��ڴ���. */
    BOOL Init();

	//by zkq 2004-08-02 SWPD07057    
    BOOL ReInit();/*���°��û���Ϣ���ص��ڴ���*/
    /* ����һ���û��Ƿ����. */
    SUserInfo* FindUser(const char* pUserName);

    /* ���ڴ��е��û���������ˢ�µ������ļ���. */
    BOOL RefreshUserFile();

    /* ����û���ϸ��Ϣ. */
    int GetUserInfo(const char* pUserName, SUserInfo *pUser);

    /* ����/ɾ���û�. */
    int AddUser(const SUserInfo *pUser);
    int DelUser(const char* pUserName);

    /* �޸��û�����. */
    int Password(const char* pUserName,const char* pOldPasswd,
                 const char* pNewPasswd);

    /* �û���¼. */
    int Login(const char* pUserName,const char* pPasswd,
              BYTE& UserLevel);

	BOOL FormatChange(char * pUserInfo);
};

#endif
