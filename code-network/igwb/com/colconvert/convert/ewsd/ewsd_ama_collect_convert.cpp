#include "ewsd_ama_collect_convert.h"
#include "../comm_convert_fun.h"
#include "ewsd_convert_fun.h"
#include "../../../include/format_out.h"

//以下是采集操作的基类CCollectConvertBase的定义
//构造函数
CEWSDAMACollectConvert::CEWSDAMACollectConvert(int nSwitchIndex, int nAMAIndex, IBillProcessor* pIBillProcessor)
                       : CCollectConvertSubObj(nSwitchIndex, nAMAIndex, pIBillProcessor)
{
    sprintf(m_szSubObjName, "%s%d", CFG_CO_SEC_AMA, nAMAIndex); 
}

//析构函数
CEWSDAMACollectConvert::~CEWSDAMACollectConvert()
{
    //
}


int CEWSDAMACollectConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //根据根据不同的话单类型指定不同转换函数
/*    if (StringiCmp(m_szBillType, CO_BILL_TYPE_AMA))
    {
        m_pConvertFun = ProcessEWSDAMAFile;
    }
    else if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "采集对象(%s, AMA=%d)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        COTRACE("采集对象(%s, AMA=%d)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        return ERR_FAIL;

    }
*/
    return ERR_SUCCESS;
}

