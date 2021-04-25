 /*
    产品名:iGateway Bill V200
    模块名:MML Server
    文件名:mml_persist_usr.h
    描述信息:
    
    包含CPersistUser类的定义,该类封装了用户数据和操作.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
*/

#ifndef _MML_PERSIST_USER_H
#define _MML_PERSIST_USER_H

#include "../include/precom.h"
#include "../utility/mml_para.h"
#include "../utility/encrypt.h"

#define cc08     "cc08"
#define cc08Pwd  "cc08"
//by ldf 2004-01-30 OM整改
#define M2000  "M2000"  
#define VER    "VER:"
#define emscomm  "emscomm"
//end  

#define INNER_USER_NUM         2 
#define PWD_TIME_LIMIT_MAX     365

//SUserInfo结构的版本号, 如果SUserInfo结构做了改动，则VEROFSUSERINFO++
#define VEROFSUSERINFO		1

//zyx modify PASSWD_LEN for ajimide
#define PASSWD_LEN              128         //用户密码最大长度
#define USER_NAME_LEN           48          //用户名最大长度
#define USER_FULL_NAEM_LEN      100         //用户全名最大长度
#define AREA_DESCRIPTION_LEN    128         //地理区域描述最大长度
#define MAX_USER_NUM    30                  //系统最大允许配置用户数

/* userinfo.dat文件头老结构 */
typedef struct  _UserInfoOldHead
{
	
#define MAX_USER_NUM    30                  //系统最大允许配置用户数
    UINT2 uUserNum;                         //文件中用户记录数
	
}SUserInfoOldHead;

/* userinfo.dat文件头结构 */
typedef struct  _UserInfoHead
{
	char  szVer[4];							//注名"VER:"
    UINT2 uUserNum;                         //文件中用户记录数
	UINT2 uVer;								//SUserInfo结构的版本号

}SUserInfoHead;

//B01版本userinfo.dat文件记录结构
typedef struct  _SUserB01Info
{
    char szName[USER_NAME_LEN];             //MML客户端用户名
    char szFullName[USER_FULL_NAEM_LEN];    //MML客户端用户全名
    char szPassword[PASSWD_LEN * 2];        //用户密码
	
    BYTE yLevel;                            //用户操作级别,
	//取值为USER_LEVEL枚举
} SUserB01Info;

/* C32B001版本userinfo.dat文件记录结构 */
typedef struct  _UserInfo1
{
    char szName[USER_NAME_LEN];             //MML客户端用户名
    char szFullName[USER_FULL_NAEM_LEN];    //MML客户端用户全名
    char szPassword[PASSWD_LEN * 2];        //用户密码
	
    char szArea[AREA_DESCRIPTION_LEN];      //用户区域描述
    BYTE yLevel;                            //用户操作级别, 取值为USER_LEVEL枚举
	time_t tPwdLastUpdate;                  //密码更新时间
    UINT4   uPwdTimeLimit;                  //密码过期时间
	
} SUserInfo1;

/* C32B002/C32B003版本userinfo.dat文件记录结构，因为从C32B004版本开始对MML平滑升级进行处理，
   SUserInfo结构发生的变化直接在SUserInfo里做改动，故也保留C32B002/C32B003版本userinfo.dat
   文件记录结构，以在读取C32B002/C32B003版本的userinfo.dat文件时，能按照此结构正确地读出
   文件
*/
typedef struct  _UserInfo2
{
    char szName[USER_NAME_LEN];             //MML客户端用户名
    char szFullName[USER_FULL_NAEM_LEN];    //MML客户端用户全名
    char szPassword[PASSWD_LEN * 2];        //用户密码
	
    char szArea[AREA_DESCRIPTION_LEN];      //用户区域描述
    BYTE yLevel;                            //用户操作级别, 取值为USER_LEVEL枚举
	time_t tPwdLastUpdate;                  //密码更新时间
    UINT4   uPwdTimeLimit;                  //密码过期时间
	UINT4   uCount;							//session Restricts Number

} SUserInfo2;

/* 当前版本的userinfo.dat文件记录结构 
   如果SUserInfo结构发生了变化，则需要作如下修改：
   1)在原SUserInfo结构的定义中直进行更改。并在构造函数中添加缺省值。
   2)保留原SUserInfo结构的定义，但要改名，如SUserInfoV01。
   3)修改mml_persist_usr.cpp文件中的FormatChange函数。在switch(m_uVer)分支上添加对前一个版
     本的SUserInfo生成的Userinfo.dat文件的处理，如上一个版本为SUserInfoV01类型的结构，则添加
   case 1:{进行格式转换}，修改方法可参考其他case分支。
   4)#define VEROFSUserInfo		1语句。宏VEROFSUserInfo自加一次。如改为
     #define VEROFSUserInfo	    2
*/
typedef struct  _UserInfo
{
    char szName[USER_NAME_LEN];             //MML客户端用户名
    char szFullName[USER_FULL_NAEM_LEN];    //MML客户端用户全名
    char szPassword[PASSWD_LEN * 2];        //用户密码

    char szArea[AREA_DESCRIPTION_LEN];      //用户区域描述
    BYTE yLevel;                            //用户操作级别, 取值为USER_LEVEL枚举
	time_t tPwdLastUpdate;                  //密码更新时间
    UINT4   uPwdTimeLimit;                  //密码过期时间
	UINT4   uCount;							//session Restricts Number
    
	//每次SUserInfo版本改动，则修改此构造函数。为新增的字段赋缺省值。
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
        用户信息的配置文件全路径名,
        其中路径在初始化时由配置参数读入,
        文件名为userinfo.dat；
    */
    char m_szUserFile[MAX_PATH];

    /*
        系统目前配置的用户数,由用户配置文件的头两个字节读出.
    */
    UINT2 m_uUserNum;

    /*
        系统最大允许配置的用户数,在初始化时由配置参数读入,
        默认值为20.
    */
    UINT2 m_uMaxUserNum;

    /*
        用户信息缓冲区指针,在构造函数中分配空间,析构时释放,
        空间大小为:
        m_uMaxUserNum * sizeof(SUserInfo) + sizeof(SUserInfoHead)
    */
    char* m_pUserInfos;

	/*
	    密码更换期限，默认是一个月，可以配置修改，最大为一年
		为0时永不过期
	*/
	UINT4 m_uPwdTimeLimit;


    static CPersistUser* g_pPersistUser;

	//文件的大小
	long m_nFileSize;

	//是否需要刷新userinfo.dat文件
	BOOL m_bFlag;

	FILE* f;

	//SUserInfo结构的版本号
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

    /* 初始化函数,把用户信息记载到内存中. */
    BOOL Init();

	//by zkq 2004-08-02 SWPD07057    
    BOOL ReInit();/*重新把用户信息记载到内存中*/
    /* 查找一个用户是否存在. */
    SUserInfo* FindUser(const char* pUserName);

    /* 把内存中的用户配置数据刷新到磁盘文件中. */
    BOOL RefreshUserFile();

    /* 获得用户详细信息. */
    int GetUserInfo(const char* pUserName, SUserInfo *pUser);

    /* 增加/删除用户. */
    int AddUser(const SUserInfo *pUser);
    int DelUser(const char* pUserName);

    /* 修改用户密码. */
    int Password(const char* pUserName,const char* pOldPasswd,
                 const char* pNewPasswd);

    /* 用户登录. */
    int Login(const char* pUserName,const char* pPasswd,
              BYTE& UserLevel);

	BOOL FormatChange(char * pUserInfo);
};

#endif
