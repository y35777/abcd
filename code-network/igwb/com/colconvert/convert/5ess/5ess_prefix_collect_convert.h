#ifndef __5ESS_PREFIX_COLLECT_CONVERT_H__
#define __5ESS_PREFIX_COLLECT_CONVERT_H__

/*
C5essPrefixCollectConvert定义了5ess采集操作相关的基本属性和基本方法
*/
#include "../collect_convert_subobj.h"

//以下是采集操作的类C5essPrefixCollectConvert的定义
class  C5essPrefixCollectConvert : public CCollectConvertSubObj
{

public:
    C5essPrefixCollectConvert(int nSwitchIndex, 
                              int nPrefixIndex,
                              IBillProcessor* pIBillProcessor = NULL); //构造函数传入协议对象指针
    virtual ~C5essPrefixCollectConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;

};

#endif //__5ESS_PREFIX_COLLECT_CONVERT_H__

