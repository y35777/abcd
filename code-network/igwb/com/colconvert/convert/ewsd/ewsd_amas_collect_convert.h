#ifndef __EWSD_AMAS_COLLECT_CONVERT_H__
#define __EWSD_AMAS_COLLECT_CONVERT_H__

/*
CEWSDAMASCollectConvert是ewsdama采集操作类, 它定义了采集操作相关的基本属性和基本方法
*/
#include "../../include/collect_convert_include.h"
#include "../include/collect_convert_struct.h"

#include "../collect_convert_base.h"

//以下是采集操作的类CEWSDAMASCollectConvert的定义
class  CEWSDAMASCollectConvert : public CCollectConvertBase
{

public:
    CEWSDAMASCollectConvert(int nSwitchIndex); //构造函数传入协议对象指针
    virtual ~CEWSDAMASCollectConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;

};

#endif //__EWSD_AMAS_COLLECT_CONVERT_H__

