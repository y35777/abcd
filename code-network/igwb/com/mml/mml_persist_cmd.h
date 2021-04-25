/*
    产品名:iGateway Bill V200
    模块名:MML Server
    文件名:mml_persist_cmd.h
    描述信息:
    
    包含CPersistCmd类的定义,该类封装了一条命令的结构.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
*/

#ifndef _MML_PERSIST_CMD_H
#define _MML_PERSIST_CMD_H

#include "../include/precom.h"
#include "mml_persist_para.h"

class CPersistCmd
{
protected:
    UINT2 m_uCmdCode;
    UINT2 m_uParaNum;
    BYTE m_yCmdLevel;
    char m_szCmdName[CMD_NAME_LEN];
    char m_szCmdDspName[MAX_CMD_DSP_LEN];

    VECTOR<CPersistPara*> m_ParaVector;

public:
    const char* GetCmdDspName();
    CPersistCmd(_CmdDef* pData);
    virtual ~CPersistCmd();

    int Init();
    char* GetCmdName();
    UINT2 GetCmdCode();
    BYTE GetCmdLevel();
    UINT2 GetParaNum();

    /* 获取参数指针 */
    CPersistPara* GetPara(UINT2 uParaNo);
    CPersistPara* GetPara(const char* pParaName);
};

class CPersistCmdMan
{
protected:
    MAP<STRING,CPersistCmd*> m_CmdMap;
    static CPersistCmdMan* g_pCmdMan;
    void* m_pParaDef;
    UINT4 m_uParaNum;
    void* m_pChildParaDef;
    UINT4 m_uChildParaNum;
    void* m_pParaEnumDef;
    UINT4 m_uParaEnumNum;
    void* m_pParaRangeDef;
    UINT4 m_uParaRangeNum;

public:
    CPersistCmdMan();
    virtual ~CPersistCmdMan();

    int Init();
    CPersistCmd* GetCmd(const STRING strCmdName);
    static CPersistCmdMan* Instance();
    static void Destroy();

    void GetParaDef(_ParaDef*& pParaDef,UINT4& uParaNum);
    void GetChildParaDef(_ChildParaDef*& pParaDef,
                         UINT4& uChildParaNum);
    void GetParaEnumDef(_ParaEnumDef*& pParaEnumDef,
                         UINT4& uParaEnumNum);
    void GetParaRangeDef(_ParaRangeDef*& pParaRangeDef,
                         UINT4& uParaRangeNum);
};

#endif
