#include "../../../include/base_type.h"
#include "s1240_cmise_mgr.h"

#include "../../include/adapter_include.h"
#include <assert.h>

//测试能否正常连接对端服务器
int CS1240CmiseMgr::TestLink()
{
    VECTOR<STRING>::iterator i = m_CmiseList.begin();
    m_CmiseStatusList.clear();
    for(; i != m_CmiseList.end(); i++)
    {
        CS1240Cmise* pCmise = m_pCmiseGroup->GetCmise(*i);
        int nRet = ERR_FAIL;
        if(NULL != pCmise)
        {
            nRet = pCmise->TestLink();
        }
        m_CmiseStatusList.push_back(nRet);
    }

    return ERR_SUCCESS;
}

int CS1240CmiseMgr::GetFilePool(SS1240FilePoolMsg& pool_msg)
{
    int nRet = ERR_FAIL;
    CS1240Cmise* pCmise = (CS1240Cmise*)GetActiveCmise();
    if(NULL != pCmise)
    {
        nRet = pCmise->GetFilePool(pool_msg);
        if(ERR_SUCCESS != nRet)
        {
            LetNextCmiseActive();
            pCmise = (CS1240Cmise*)GetActiveCmise();
            if(NULL != pCmise)
            {
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pCmise->GetFilePool(pool_msg);
                }
            }
        }
    }

    return nRet;
}

int CS1240CmiseMgr::SetFilePool(int nAlarmLimit)
{
    int nRet = ERR_FAIL;
    CS1240Cmise* pCmise = (CS1240Cmise*)GetActiveCmise();
    if(NULL != pCmise)
    {
        nRet = pCmise->SetFilePool(nAlarmLimit);
        if(ERR_SUCCESS != nRet)
        {
            LetNextCmiseActive();
            pCmise = (CS1240Cmise*)GetActiveCmise();
            if(NULL != pCmise)
            {
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pCmise->SetFilePool(nAlarmLimit);
                }
            }
        }
    }

    return nRet;
}

int CS1240CmiseMgr::GetCmiseFile(const char* const szFileId,
                                 SS1240FileMsg&    file_msg)
{
    if((szFileId == NULL) || (szFileId[0] == '\0'))
    {
        return ERR_FAIL;
    }

    int nRet = ERR_FAIL;
    CS1240Cmise* pCmise = (CS1240Cmise*)GetActiveCmise();
    if(NULL != pCmise)
    {
        nRet = pCmise->GetFile(szFileId, file_msg);
        if(ERR_SUCCESS != nRet)
        {
            LetNextCmiseActive();
            pCmise = (CS1240Cmise*)GetActiveCmise();
            if(NULL != pCmise)
            {
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pCmise->GetFile(szFileId, file_msg);
                }
            }
        }
    }

    return nRet;
}

int CS1240CmiseMgr::SetCmiseFile(const char* const szFileId,
                                 int               nFilestatus)
{
    if((szFileId == NULL) || (szFileId[0] == '\0'))
    {
        return ERR_FAIL;
    }

    int nRet = ERR_FAIL;
    CS1240Cmise* pCmise = (CS1240Cmise*)GetActiveCmise();
    if(NULL != pCmise)
    {
        nRet = pCmise->SetFile(szFileId, nFilestatus);
        if(ERR_SUCCESS != nRet)
        {
            LetNextCmiseActive();
            pCmise = (CS1240Cmise*)GetActiveCmise();
            if(NULL != pCmise)
            {
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pCmise->SetFile(szFileId, nFilestatus);
                }
            }
        }
    }

    return nRet;
}

int CS1240CmiseMgr::GetCmiseFileList(int               nFileStatus,
                                     VECTOR<SS1240FileMsg>  &Out)
{
    int nRet = ERR_FAIL;
    CS1240Cmise* pCmise = (CS1240Cmise*)GetActiveCmise();
    if(NULL != pCmise)
    {
        nRet = pCmise->GetFileList(nFileStatus, Out);
        if(ERR_SUCCESS != nRet)
        {
            LetNextCmiseActive();
            pCmise = (CS1240Cmise*)GetActiveCmise();
            if(NULL != pCmise)
            {
                if(ERR_SUCCESS == nRet)
                {
                    nRet = pCmise->GetFileList(nFileStatus, Out);
                }
            }
        }
    }

    return nRet;
}

