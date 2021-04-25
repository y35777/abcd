#ifndef _PATCH_ENCAP_H_
#define _PATCH_ENCAP_H_

#include "../include/base_type.h"
#include "ace/DLL.h"

extern "C" {

typedef bool (*PC_INIT)(const char* szConfigPath, BYTE nApID, UINT4 nChannelCount);

typedef BOOL (*PC_PROCESS)(const char* szFileName, char*& szDestFileName, UINT4 nChannelNum);

typedef BOOL (*PC_TERMINATE)();

//by 杨善	2004-6-1	对应问题单SWPD05564
typedef BOOL (*PC_CANCLOSE)(const char* szFileName);
//the end	对应问题单SWPD05564

//added by lucy 2004-6-11 wlan backsave patch
typedef void (*PC_RESETREMAINTICKET_WLAN)();
typedef BOOL (*PC_ISFILEOUTPUT_WLAN)(UINT4 nChannelID);
typedef void (*PC_GETAEMPTYFILEPARA)(char *szFileName, char *szFileSuffix, UINT4 nChannelID);
typedef BOOL (*PC_CREATEAEMPTYFILE)(char *strFileName, char *strFileSuffix);
//end add
}

class CPatchEncap
{
public:
    CPatchEncap();
    ~CPatchEncap();
    int Init(int nAPId);

    BOOL Init(const char* szConfigPath, BYTE nApID, UINT4 nChannelCount);
    BOOL Process(const char* szFileName, char*& szDestFileName, UINT4 nChannelNum);
    BOOL Terminate();

    //by 杨善	2004-6-1	对应问题单SWPD05564
	BOOL CanClose(const char* szFileName);
    //the end	对应问题单SWPD05564
	
    
    //added by lucy 2004-6-11 wlan backsave patch
    STRING GetDllName();
    void ResetRemainTicket_WLAN();
    BOOL IsFileOutPut_WLAN(UINT4 nChannelID);    
    void GetAEmptyFilePara(char *szFileName, char *szFileSuffix, UINT4 nChannelID); 
    BOOL CreateAEmptyFile(char *strFileName, char *strFileSuffix);
    //end add

    inline BOOL GetbAviable()
    {
        return m_bAviable;
    }

protected:
    PC_INIT      m_pInitFun;
    PC_PROCESS   m_pProcessFun;
    PC_TERMINATE m_pTerminateFun;

	PC_CANCLOSE  m_pCanCloseFun;

    //added by lucy 2004-6-11 wlan backsave patch
    char                       m_szDllName[100];
    PC_RESETREMAINTICKET_WLAN  m_pResetRemainTicket_WLAN;
    PC_ISFILEOUTPUT_WLAN       m_pIsFileOutPut_WLAN;
    PC_GETAEMPTYFILEPARA       m_pGetAEmptyFilePara;
    PC_CREATEAEMPTYFILE        m_pCreateAEmptyFile;
    //end add

    ACE_DLL*     m_pDll;

    BOOL         m_bAviable;
};

#endif //_PATCH_ENCAP_H_
