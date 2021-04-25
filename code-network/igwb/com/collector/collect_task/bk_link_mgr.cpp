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


//ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
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

//����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
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

//ɾ���ļ�����˵�ָ���ļ�
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
//ȡ�ļ�����˵�ָ���ļ��ĳ���
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

//������ָ���ļ�
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

//�����ļ������
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

//�Ͽ�����
int CBKLinkMgr::Disconnect()
{
    CLinkBase* pLinkBase = GetActiveLink();
    if(NULL != pLinkBase)
    {
        return pLinkBase->Disconnect();
    }

    return ERR_FAIL;
}

//�����ļ�����ģʽ
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
