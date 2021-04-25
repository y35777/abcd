#ifndef __EWSD_AMA_COLLECT_CONVERT_H__
#define __EWSD_AMA_COLLECT_CONVERT_H__

/*
CEWSDAMACollectConvert定义了EWSDAMA采集操作相关的基本属性和基本方法
*/
#include "../collect_convert_subobj.h"

//以下是采集操作的基类CEWSDAMACollectConvert的定义
class  CEWSDAMACollectConvert : public CCollectConvertSubObj 
{

public:
    CEWSDAMACollectConvert(int nSwitchIndex, 
                           int nAMAIndex,
                           IBillProcessor* pIBillProcessor = NULL); //构造函数传入协议对象指针
    virtual ~CEWSDAMACollectConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;
};

#endif //__EWSD_AMA_COLLECT_CONVERT_H__

