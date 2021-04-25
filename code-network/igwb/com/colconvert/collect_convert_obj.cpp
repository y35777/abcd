#include "../include/base_type.h"
#include "collect_convert_obj.h"
#include "include/collect_convert_creator.h"

CCollectConvertObj::CCollectConvertObj(int nConvertID, const char* const szCfgPath)
{
    m_pCollectConvert  = NULL;
    m_szCfgPath[0]     = '\0';
    m_nConvertID       = nConvertID;

    if(szCfgPath != NULL)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }
}

CCollectConvertObj::~CCollectConvertObj()
{
    if(NULL != m_pCollectConvert)
    {
        delete m_pCollectConvert;
        m_pCollectConvert = NULL;
    }
}

//³õÊ¼»¯
int CCollectConvertObj::Init()
{
    if((NULL == m_pCollectConvert) && (m_szCfgPath[0] != '\0'))
    {
        m_pCollectConvert = CCollectConvertFactory::CreateCollectConvert(m_nConvertID, m_szCfgPath);
        if(NULL != m_pCollectConvert)
        {
            return m_pCollectConvert->Init();
        }
    }

    return ERR_FAIL;
}

int CCollectConvertObj::Process(const char* szBillFile, const int& nConvertInfo)
{
    if (NULL != m_pCollectConvert)
    {
        return m_pCollectConvert->Process(szBillFile, nConvertInfo);
    }

    return ERR_FAIL;
}
