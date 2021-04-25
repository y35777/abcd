#include "5ess_collect_convert.h"
#include "5ess_prefix_collect_convert.h"
#include "../../../include/format_out.h"


//以下是采集操作的基类CCollectConvertBase的定义
//构造函数
C5essCollectConvert::C5essCollectConvert(int nSwitchIndex)
                        :CCollectConvertBase(nSwitchIndex) 
{
    //
}

//析构函数
C5essCollectConvert::~C5essCollectConvert()
{
    //
}


int C5essCollectConvert::Init()
{
    
    int nRet = CCollectConvertBase::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //读入S1240的配置信息
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("采集任务(%s)转换模块打开配置文件(%s)失败", 
              m_szSwitchName, 
              m_szCfgPath);
        return ERR_FAIL;
    }

    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName, 
             sizeof(szSecName),
             "%s%d", //"switch*"
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);


    C5essPrefixCollectConvert* pConvert = NULL;

	int nPrefixCount = INIFile.GetInt(szSecName, 
                                  CFG_5ESS_PREFIX_NUM,
                                  CFG_5ESS_PREFIX_NUM_DEFAULT);
    for(int i = 1; i <= nPrefixCount; i++)
    {
        pConvert = new C5essPrefixCollectConvert(m_nSwitchIndex,
                                                i,
                                                m_pIBillProcessor);
        pConvert->SetCfgPath(m_szCfgPath);
        pConvert->SetSwitchName(m_szSwitchName);

        if((NULL == pConvert) || (pConvert->Init() != ERR_SUCCESS))
        {
            COTRACE("采集对象(%s)创建Prefix转换子对象(ID=%d)失败", 
                  m_szSwitchName,
                  i);
            delete pConvert;
            return ERR_FAIL;
        }
        m_SubObjMap[i] = pConvert;
    }

    return ERR_SUCCESS;
}

