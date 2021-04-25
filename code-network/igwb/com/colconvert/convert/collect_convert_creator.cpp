#include "../include/collect_convert_include.h"
#include "../include/i_collect_convert.h"
#include "../include/collect_convert_creator.h"
#include "s1240/s1240_collect_convert.h"
#include "5ess/5ess_collect_convert.h"
#include "com/com_collect_convert.h"
#include "ewsd/ewsd_amas_collect_convert.h"
#include "ewsd/ewsd_mm_collect_convert.h"

ICollectConvert* CCollectConvertFactory::CreateCollectConvert(int nSwitchIndex, const char* const szCfgPath)
{

    int  nRet = 0;
    char szSecName[CFG_BUF_SIZE] = "";

    CINIFile INIFile(szCfgPath);
    INIFile.Open();
    SNPRINTF(szSecName,CFG_BUF_SIZE, "switch%d", nSwitchIndex);

    char szSwitchName[CFG_BUF_SIZE] = "";
    INIFile.GetString(szSecName,
                      CFG_CO_NAME, 
                      CFG_CO_NAME_DEFAULT, 
                      szSwitchName, 
                      sizeof(szSwitchName));

    char szCollectId[CFG_BUF_SIZE] = "";
    INIFile.GetString(szSecName, CFG_CO_TYPE,
                      CFG_CO_TYPE_DEFAULT,
                      szCollectId,
                      sizeof(szCollectId));

    ICollectConvert* pConvert = NULL;


    //根据标识串创建不同的采集类对象
    if (StringiCmp(szCollectId, CO_TYPE_S1240))
    {
        pConvert = new CS1240CollectConvert(nSwitchIndex);
    }
    else if (StringiCmp(szCollectId, CO_TYPE_5ESS))
    {
        pConvert = new C5essCollectConvert(nSwitchIndex);
    }
    else if (StringiCmp(szCollectId, CO_TYPE_EWSD_MM))
    {
        pConvert = new CEWSDMMCollectConvert(nSwitchIndex);
    }
    else if (StringiCmp(szCollectId, CO_TYPE_EWSD_AMA))
    {
        pConvert = new CEWSDAMASCollectConvert(nSwitchIndex);
    }
    else if (StringiCmp(szCollectId, CO_TYPE_COM))
    {
        pConvert = new CComCollectConvert(nSwitchIndex);
    }
    else if (StringiCmp(szCollectId, CO_TYPE_CC08))
    {
        pConvert = new CComCollectConvert(nSwitchIndex);
    }
    else if (StringiCmp(szCollectId, CO_TYPE_TELLIN))    //added by chenliangwei, 2003-08-14
    {
        pConvert = new CComCollectConvert(nSwitchIndex);
    }           
	else if (StringiCmp(szCollectId, CO_TYPE_SJZ_TELLIN))    //added by chenliangwei, 2003-08-14
    {
        pConvert = new CComCollectConvert(nSwitchIndex);
    }   //added end
    else if (StringiCmp(szCollectId, CO_TYPE_ZXJ))
    {
        pConvert = new CComCollectConvert(nSwitchIndex);
    }
    else if (StringiCmp(szCollectId, CO_TYPE_METER_CC08))
    {
        pConvert = new CComCollectConvert(nSwitchIndex); //add by yangguang
    }
    else
    {
        COTRACE("采集对象(%s)检测到无效的采集类型(%s)",
              szSwitchName, 
              szCollectId);
    }

    if(NULL != pConvert)
    {
        pConvert->SetCfgPath(szCfgPath);
    }

    return pConvert;
}
