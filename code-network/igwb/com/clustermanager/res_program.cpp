#include "../include/base_type.h"
#include "res_program.h"


/********************************************************
* 名称: CResProgram::CResProgram
* 功能: 构造函数
* 输入: const char* const szAppPath : 程序资源的全路径名
*       const char* const szResName : 资源名
*       ERES_TYPE nResType          : 资源类型
*       int nMaxLocalRecovery       : 最大本地恢复次数
* 输出:
* 返回:
********************************************************/
CResProgram::CResProgram(const char* const szAppPath,
                         const char* const szResName,
                         ERES_TYPE nResType,
                         int nMaxLocalRecovery):
CResObject(szResName, nResType, nMaxLocalRecovery)
{
    strncpy(m_szAppPath, szAppPath, MAX_PATH);
    m_szAppPath[MAX_PATH - 1] = '\0';
    m_nPid                    = ACE_INVALID_PID;
}

/********************************************************
* 名称: CResProgram::~CResProgram
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResProgram::~CResProgram()
{
    //
}

/********************************************************
* 名称: CResProgram::Create
* 功能: 资源创建函数
* 输入:
* 输出:
* 返回: int 资源创建是否成功，0表示成功，非零值表示失败
********************************************************/
int CResProgram::Create()
{
    int nRet = ACE_OS::access(m_szAppPath, F_OK);
    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    m_POptions.command_line("%s", m_szAppPath);

    return CResObject::Create();
}

/********************************************************
* 名称: CResProgram::CheckStatus
* 功能: 资源的状态检查函数
* 输入:
* 输出:
* 返回: int 是否存在不可恢复的故障，0表示无故障，非零值表示检测到故障
********************************************************/
int CResProgram::CheckStatus()
{
    if(ACE_INVALID_PID == m_nPid)
    {
        m_nStatus = ERES_STATUS_DEACTIVE;
        return ERR_FAIL;
    }

    int nRet = m_PMgr.wait(m_nPid, ACE_Time_Value(0));
    if(0 != nRet)
    //0表示超时，即进程还在运行，非零则表示进程已退出或错误
    {
        m_nPid = ACE_INVALID_PID;
        m_nStatus = ERES_STATUS_DEACTIVE;
        nRet = ERR_FAIL;
    }

    return nRet;
}

/********************************************************
* 名称: CResProgram::Active
* 功能: 资源的激活函数
* 输入:
* 输出:
* 返回: int 是否激活成功，0表示成功，非零值表示启动失败
********************************************************/
int CResProgram::Active()
{
    //如果已经激活,则直接返回
    if(CheckStatus() == ERR_SUCCESS)
    {
        m_nStatus = ERES_STATUS_ACTIVE;
        return ERR_SUCCESS;
    }
 
    m_nPid = m_PMgr.spawn(m_POptions);
    if (ACE_INVALID_PID == m_nPid)
    {
        return ERR_FAIL;
    }

    return CResObject::Active();
}

/********************************************************
* 名称: CResProgram::DeActive
* 功能: 资源的去激活函数
* 输入:
* 输出:
* 返回: int 是否去激活成功，0表示成功，非零值表示去激活失败
********************************************************/
int CResProgram::DeActive()
{
    if(CheckStatus() != ERR_SUCCESS)
    {
        return ERR_SUCCESS;
    }

    ACE::terminate_process(m_nPid);

    return CResObject::DeActive();
}
