#include "../../include/base_type.h"
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "bk_link_mgr.h"
#include "link_base.h"

#include "../../include/toolbox.h"
#include "../include/adapter_include.h"
#include "../include/res_config.h"
#include <assert.h>


//实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
int CBKLinkMgr::GetFile(const SGetFileReq &pReq)
{
    int nRet = ERR_FAIL;
    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        nRet = pLinkBase->GetFile(pReq);
        if(ERR_SUCCESS != nRet && GetLinkCount() > 1)
        {
            pLinkBase->Disconnect();
            LetNextLinkActive();
            pLinkBase = GetActiveLink();
            if(NULL != pLinkBase)
            {
                pLinkBase->Connect();
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pLinkBase->GetFile(pReq);
                }
            }
        }
    }

    return nRet;
}

//获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
int CBKLinkMgr::GetFileList(const  char* szSrcDir, SListOut &pOut)
{
    int nRet = ERR_FAIL;
    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        nRet = pLinkBase->GetFileList(szSrcDir, pOut);
        if(ERR_SUCCESS != nRet && GetLinkCount() > 1)
        {
            pLinkBase->Disconnect();
            LetNextLinkActive();
            pLinkBase = GetActiveLink();
            if(NULL != pLinkBase)
            {
                pLinkBase->Connect();
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pLinkBase->GetFileList(szSrcDir, pOut);
                }
            }
        }
    }

    return nRet;
}

//删除文件服务端的指定文件
int CBKLinkMgr::DelFile(const char* szFileName)
{
    int nRet = ERR_FAIL;
    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        nRet = pLinkBase->DelFile(szFileName);
        if(ERR_SUCCESS != nRet && GetLinkCount() > 1)
        {
            pLinkBase->Disconnect();
            LetNextLinkActive();
            pLinkBase = GetActiveLink();
            if(NULL != pLinkBase)
            {
                pLinkBase->Connect();
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pLinkBase->DelFile(szFileName);
                }
            }
        }
    }

    return nRet;
}
//取文件服务端的指定文件的长度
int CBKLinkMgr::GetFileLen(const char* szFileName,
                           unsigned long& uFileLen)
{
    int nRet = ERR_FAIL;
    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        nRet = pLinkBase->GetFileLen(szFileName, uFileLen);
        if(ERR_SUCCESS != nRet && GetLinkCount() > 1)
        {
            pLinkBase->Disconnect();
            LetNextLinkActive();
            pLinkBase = GetActiveLink();
            if(NULL != pLinkBase)
            {
                pLinkBase->Connect();
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pLinkBase->GetFileLen(szFileName, uFileLen);
                }
            }
        }
    }

    return nRet;
}

//重命名指定文件
int CBKLinkMgr::RenameFile(const char *szOld, const char *szNew)
{
    int nRet = ERR_FAIL;
    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        nRet = pLinkBase->RenameFile(szOld, szNew);
        if(ERR_SUCCESS != nRet && GetLinkCount() > 1)
        {
            pLinkBase->Disconnect();
            LetNextLinkActive();
            pLinkBase = GetActiveLink();
            if(NULL != pLinkBase)
            {
                pLinkBase->Connect();
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pLinkBase->RenameFile(szOld, szNew);
                }
            }
        }
    }

    return nRet;
}

//连接文件服务端
int CBKLinkMgr::Connect()
{
    int nRet = ERR_FAIL;

    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        nRet = pLinkBase->Connect();
    }
    if(ERR_SUCCESS != nRet && GetLinkCount() > 1)
    {
        LetNextLinkActive();
        pLinkBase = GetActiveLink();
        if(NULL != pLinkBase)
        {
            nRet = pLinkBase->Connect();
        }
    }

    return nRet;
}

//断开连接
int CBKLinkMgr::Disconnect()
{
    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        return pLinkBase->Disconnect();
    }

    return ERR_FAIL;
}

//设置文件传输模式
int CBKLinkMgr::SetMode(int nMode)
{
    int nRet = ERR_FAIL;
    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        nRet = pLinkBase->SetMode(nMode);
        if(ERR_SUCCESS != nRet && GetLinkCount() > 1)
        {
            pLinkBase->Disconnect();
            LetNextLinkActive();
            pLinkBase = GetActiveLink();
            if(NULL != pLinkBase)
            {
                pLinkBase->Connect();
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pLinkBase->SetMode(nMode);
                }
            }
        }
    }

    return nRet;
}
