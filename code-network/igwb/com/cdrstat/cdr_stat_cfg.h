#ifndef __CDR_STAT_CFG_H__
#define __CDR_STAT_CFG_H__

#include "stat_cfg_info.h"
#include "i_cdr_stat_cfg.h"


class CCDRStatCfg: public ICDRStatCfg
{
public:
    CCDRStatCfg();
    virtual ~CCDRStatCfg();

public:
     //设置参数，进行初始化操作
    virtual int Init(UINT4 uAccessPoint);

    //获得所有统计项的项目名
    virtual void GetStatItemNameList(LIST<STRING>& listStatItemName);

    //获得指定统计项目名的统计配置信息
    virtual SStatItemCfg* GetStatItem(const char* const szStatItemName);

    //获得指定统计项目名的统计配置信息
    virtual void GetStatItem(const char* const szStatItemName,
                             LIST<SOneStatSubItemCfg*>& subItemList);

    //设置所有统计子项的配置信息
    virtual int SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList);

    //获得接入点号
    virtual UINT4 GetAccessPoint();

    //获得接入点名称
    virtual const char* GetAccessPointName();

protected:
    //接入点号
    UINT4    m_uAccessPoint;
    //接入点名称
    STRING   m_sAPName;
    //统计配置文件绝对路径名
    char     m_szFileName[MAX_PATH];
    //统计配置信息缓冲区指针
    BYTE*    m_pMemData;
    //统计配置信息文件长度
    UINT4    m_nMemDataLen;
};
#endif  //__CDR_STAT_FILE_H__

