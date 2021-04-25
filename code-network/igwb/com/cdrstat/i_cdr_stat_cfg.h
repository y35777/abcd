#ifndef __I_CDR_STAT_CFG_H__
#define __I_CDR_STAT_CFG_H__

#include "stat_cfg_info.h"

class ICDRStatCfg
{
public:
    ICDRStatCfg(){};
    virtual ~ICDRStatCfg(){};

public:
    //设置参数，进行初始化操作
    virtual int Init(UINT4 uAccessPoint) = 0;

     //获得所有统计项的项目名
    virtual void GetStatItemNameList(LIST<STRING>& listStatItemName) = 0;

    //获得指定统计项目名的统计配置信息
    virtual SStatItemCfg* GetStatItem(const char* const szStatItemName) = 0;

     //获得指定统计项目名的统计配置信息
    virtual void GetStatItem(const char* const szStatItemName, LIST<SOneStatSubItemCfg*>& subItemList) = 0;

    //设置所有统计子项的配置信息
    virtual int SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList) = 0;

    //获得接入点号
    virtual UINT4 GetAccessPoint() = 0;

    //获得接入点名称
    virtual const char* GetAccessPointName() = 0;

};

#endif  //__I_CDR_STAT_CFG_H__

