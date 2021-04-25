#include "s1240_collect_convert.h"
#include "s1240_tp_collect_convert.h"

//以下是采集操作的基类CCollectConvertBase的定义
//构造函数
CS1240CollectConvert::CS1240CollectConvert(int nSwitchIndex)
                        :CCollectConvertBase(nSwitchIndex) 
{
    //
}

//析构函数
CS1240CollectConvert::~CS1240CollectConvert()
{
    //
}


int CS1240CollectConvert::Init()
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

    CS1240TPCollectConvert* pConvert = NULL;

	int nTPCount = INIFile.GetInt(szSecName, 
                                  CFG_CO_TP_COUNT,
                                  CFG_CO_TP_COUNT_DEFAULT);
    for(int i = 1; i <= nTPCount; i++)
    {
        /*
            取主TP的链路信息
        */
        pConvert = new CS1240TPCollectConvert(m_nSwitchIndex,
                                                i,
                                                m_pIBillProcessor);
        pConvert->SetCfgPath(m_szCfgPath);
        pConvert->SetSwitchName(m_szSwitchName);

        if((NULL == pConvert) || (pConvert->Init() != ERR_SUCCESS))
        {
            COTRACE("采集对象(%s)创建TP转换子对象(ID=%d)失败", 
                  m_szSwitchName,
                  i);
            delete pConvert;
            return ERR_FAIL;
        }
        m_SubObjMap[i] = pConvert;
    }

    return ERR_SUCCESS;
}

