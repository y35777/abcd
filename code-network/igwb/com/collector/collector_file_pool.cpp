#include "collector_file_pool.h"
#include "include/adapter_include.h"
#include "../include/toolbox.h"
#include "resource.h"

#include <assert.h>

CCollectorFilePool::CCollectorFilePool(const char* szStatusFile)
{
    assert(szStatusFile != NULL);

    strncpy(m_szStatusFile, szStatusFile, sizeof(m_szStatusFile));

    m_nFileCount = 0;
}

CCollectorFilePool::~CCollectorFilePool()
{
    //
}

int CCollectorFilePool::Init()
{
    int nRet = ERR_SUCCESS;

    m_FilePoolMutex.acquire();

    nRet = LoadStatus();

    m_FilePoolMutex.release();

    return nRet;
}

int CCollectorFilePool::PushBack(SSwitchFileInfo& stFileInfo)
{
    int nRet = ERR_SUCCESS;

    m_FilePoolMutex.acquire();

    m_FileList.push_back(stFileInfo);
    m_nFileCount++;

    nRet = SaveStatus();

    m_FilePoolMutex.release();

    return nRet;
}

SSwitchFileInfo& CCollectorFilePool::Front()
{

    m_FilePoolMutex.acquire();

    SSwitchFileInfo& stFileInfo = m_FileList.front();

    m_FilePoolMutex.release();

    return stFileInfo;

}


int CCollectorFilePool::PopFront()
{
    int nRet = ERR_SUCCESS;

    m_FilePoolMutex.acquire();

    m_FileList.pop_front();
    m_nFileCount--;

    SaveStatus();

    m_FilePoolMutex.release();

    return nRet;
}

//加载状态文件
int CCollectorFilePool::LoadStatus()
{
    m_FileList.clear();

    FILE* pStatusFile = fopen(m_szStatusFile, "rb+");
    if(NULL == pStatusFile)
    {
        //创建新文件
        pStatusFile = fopen(m_szStatusFile, "wb+");

        if (NULL == pStatusFile)
        {
            COTRACE(S_CVT_FAILED_TO_OPEN_STATUS, m_szStatusFile);
            return ERR_FAIL;
        }
        else
        {
            m_nFileCount = 0;
            fclose(pStatusFile);

            return ERR_SUCCESS;
        }
    }

    int nFileLength = FileSize(pStatusFile);
    if(nFileLength < sizeof(int))
    {
        m_nFileCount = 0;
        fclose(pStatusFile);

        return ERR_SUCCESS;
    }

    BYTE *pReadBuffer = new BYTE[nFileLength];
    int nRet = fread(pReadBuffer,
                     nFileLength,
                     1,
                     pStatusFile);

    fclose(pStatusFile);
    if(1 != nRet)
    {
        delete []pReadBuffer;

        COTRACE(S_CVT_FAILED_TO_READ_STATUS, m_szStatusFile);
        return ERR_FAIL;
    }

    memcpy(&m_nFileCount, pReadBuffer, sizeof(int));

    SSwitchFileInfo* pInfo =
        (SSwitchFileInfo*)(pReadBuffer + sizeof(int));

    for (int i = 0; i < m_nFileCount; i++)
    {
        m_FileList.push_back(*pInfo);
        pInfo++;
    }

    delete []pReadBuffer;

    return ERR_SUCCESS;
}

//保存状态文件
int CCollectorFilePool::SaveStatus()
{
    FILE* pStatusFile = fopen(m_szStatusFile, "wb");
    if(NULL == pStatusFile)
    {
        COTRACE(S_CVT_FAILED_TO_OPEN_STATUS, m_szStatusFile);
        return ERR_FAIL;

    }

    int nRet = 0;
    int nFileLength = sizeof(int)
                + sizeof(SSwitchFileInfo) * m_nFileCount;

    BYTE *pWriteBuffer = new BYTE[nFileLength];

    memcpy(pWriteBuffer, &m_nFileCount, sizeof(int));

    SSwitchFileInfo* pInfo =
        (SSwitchFileInfo*)(pWriteBuffer + sizeof(int));

    LIST<SSwitchFileInfo>::iterator i = m_FileList.begin();

    for (; i != m_FileList.end(); i++)
    {
        memcpy(pInfo, &(*i), sizeof(SSwitchFileInfo));
        pInfo++;
    }


    nRet = fwrite(pWriteBuffer,
                  nFileLength,
                  1,
                  pStatusFile);

    if(1 != nRet)
    {
        fclose(pStatusFile);
        delete []pWriteBuffer;

        COTRACE(S_CVT_FAILED_TO_WRITE_STATUS, m_szStatusFile);
        return ERR_FAIL;
    }


    nRet = fflush(pStatusFile);
    fclose(pStatusFile);

    delete []pWriteBuffer;

    ACE_OS::truncate(m_szStatusFile, nFileLength);

    return nRet;
}
//added end
