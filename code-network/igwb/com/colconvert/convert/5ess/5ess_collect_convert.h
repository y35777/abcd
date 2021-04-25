#ifndef __5ESS_COLLECT_CONVERT_H__
#define __5ESS_COLLECT_CONVERT_H__

/*
CCollectConvertBase是采集操作的基类, 它定义了采集操作相关的基本属性和基本方法
*/
#include "../../include/collect_convert_include.h"
#include "../include/collect_convert_struct.h"

#include "../collect_convert_base.h"

//以下是采集操作的基类CCollectConvertBase的定义
class  C5essCollectConvert : public CCollectConvertBase
{

public:
    C5essCollectConvert(int nSwitchIndex); //构造函数传入协议对象指针
    virtual ~C5essCollectConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;
};

#endif //__5ESS_COLLECT_CONVERT_H__

