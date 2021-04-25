#include "5ess_prefix_collect_convert.h"
#include "../comm_convert_fun.h"
#include "5ess_convert_fun.h"
#include "../../../include/format_out.h"
#include "../collect_convert_base.h"

//以下是采集操作的基类CCollectConvertBase的定义
//构造函数
C5essPrefixCollectConvert::C5essPrefixCollectConvert(int nSwitchIndex, int nPrefixIndex, IBillProcessor* pIBillProcessor)
                       : CCollectConvertSubObj(nSwitchIndex, nPrefixIndex, pIBillProcessor)
{
    sprintf(m_szSubObjName, "%s%d", CFG_CO_SEC_PREFIX, nPrefixIndex); 
}

//析构函数
C5essPrefixCollectConvert::~C5essPrefixCollectConvert()
{
    //
}


int C5essPrefixCollectConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //根据根据不同的话单类型指定不同转换函数
    if (StringiCmp(m_szBillType, CO_BILL_TYPE_AMA))
    {
        m_pConvertFun = Process5essAMAFile;
    }
    else if (StringiCmp(m_szBillType, CO_BILL_TYPE_METER))
    {
        m_pConvertFun = Process5essMMFile;
    }
    else if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "采集对象(%s, Prefix=%d)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        COTRACE("采集对象(%s, Prefix=%d)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        return ERR_FAIL;

    }

    return ERR_SUCCESS;
}



