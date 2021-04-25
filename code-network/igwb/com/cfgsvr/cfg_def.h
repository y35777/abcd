#ifndef __CFG_DEF_H__
#define __CFG_DEF_H__

#include "../include/precom.h"
#include "../include/toolbox.h"
#include "../include/frame.h"

//MAX_LINE为读取一行的最大长度
const int MAX_LINE = 1024;
//段名最大长度
const int MAX_SECTNAME_LEN = 64;
//参数项名最大长度
const int MAX_ITEMNAME_LEN = 32;
//参数值最大长度
const int MAX_ITEMVALUE_LEN = 256;
//参数项信息描述的最大长度
const int MAX_ITEMDESC_LEN = 256;

//配置文件类型定义：0--出厂配置(.bas)，1--上次配置(.lst), 2--当前配置(.ini)
#define FILE_TYPE_FAC 0
#define FILE_TYPE_SUC 1
#define FILE_TYPE_CUR 2

#define SECTLIST   "SectList"	
#define RELATION   "Relation"
#define TYPE       "Type"
#define DESC       "Desc"
#define MAX        "Max"
#define MIN        "Min"
#define DEFAULT    "Default"
#define MUSTFLAG   "MustFlag"
#define UPDATEFLAG "UpdateFlag"

#endif //__CFG_DEF_H__
