#include "s1240_tp_collect_convert.h"
#include "../comm_convert_fun.h"
#include "s1240_convert_fun.h"
#include "s1240_gb_convert_fun.h"
#include "../../../include/format_out.h"

//以下是采集操作的基类CCollectConvertBase的定义
//构造函数
CS1240TPCollectConvert::CS1240TPCollectConvert(int nSwitchIndex, int nTPIndex, IBillProcessor* pIBillProcessor)
                       : CCollectConvertSubObj(nSwitchIndex, nTPIndex, pIBillProcessor)
{
    sprintf(m_szSubObjName, "%s%d", CFG_CO_SEC_TP, nTPIndex); 
}

//析构函数
CS1240TPCollectConvert::~CS1240TPCollectConvert()
{
    //
}


int CS1240TPCollectConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //根据根据不同的话单类型指定不同转换函数
    if (StringiCmp(m_szBillType, CO_BILL_TYPE_GB))
    {
        m_pConvertFun = ProcessS1240GBFile;
    }
/*    else if (StringiCmp(m_szBillType, CO_BILL_TYPE_DETAIL))
    {
        m_pConvertFun = ProcessS1240DetailFile;
    }
    else if (StringiCmp(m_szBillType, CO_BILL_TYPE_METER))
    {
        m_pConvertFun = ProcessS1240MeterFile;
    }
*/
    else if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "采集对象(%s, TP=%d)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        COTRACE("采集对象(%s, TP=%d)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        return ERR_FAIL;

    }

    return ERR_SUCCESS;
}

