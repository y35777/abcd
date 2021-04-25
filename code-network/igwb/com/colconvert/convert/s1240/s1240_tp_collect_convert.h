#ifndef __S1240_TP_COLLECT_CONVERT_H__
#define __S1240_TP_COLLECT_CONVERT_H__

/*
CS1240TPCollectConvert定义了S1240采集操作相关的基本属性和基本方法
*/
#include "../collect_convert_subobj.h"

//以下是采集操作的基类CS1240TPCollectConvert的定义
class  CS1240TPCollectConvert : public CCollectConvertSubObj 
{

public:
    CS1240TPCollectConvert(int nSwitchIndex, 
                           int nTPIndex, 
                           IBillProcessor* pIBillProcessor = NULL); //构造函数传入协议对象指针
    virtual ~CS1240TPCollectConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;

};

#endif //__S1240_TP_COLLECT_CONVERT_H__

