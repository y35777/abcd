#ifndef __COLLECT_CONVERT_STRUCT_H__
#define __COLLECT_CONVERT_STRUCT_H__


#include "../../../include/base_type.h"
//公用数据结构定义

//数据结构定义
//结构类型SColFileReq用于定义采集单个文件时的输入参数，定义如下：
struct SConvertFileReq
{
    char  szSrcName[MAX_PATH];//源文件名
    char  szFinalName[MAX_PATH];//目的文件名
    char  szSecondFinalName[MAX_PATH];
    char  szErrName[MAX_PATH];
    int   nConvertInfo;//文件类型
};


#endif //__COLLECT_CONVERT_STRUCT_H__
