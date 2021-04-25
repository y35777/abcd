#ifndef __QB_COMMON_H__
#define __QB_COMMON_H__

#include "../include/precom.h"
#include "resource.h"

#define MAX_COND_LEN 2048             //查询条件的最大长度

enum STYLE
{
    STYLE_HORI, //横向
    STYLE_VERI  //纵向
};

//任务类型的枚举定义
enum TaskType
{
    BROWSE_TASK = 0,    //浏览任务
    QUERY_TASK = 1      //查询任务
};

//查询的文件范围类型的枚举定义
enum QueryRangeType
{
    DATE_RANGE = 0,     //查询某日期范围内的话单
    FILE_RANGE = 1      //查询某文件范围内的话单
};

//话单页状态的枚举
enum PageState
{
    PREPARING,         //正在准备
    READY,             //已经准备好
    DELETED            //已经删除
};

//查询结果偏移量，记录符合查询条件的话单记录在查询文件中的位置
struct ResultOffset
{
    INT2 nFileNo; //查询文件在查询文件列表的编号
    //INT2 nBillLen;//话单长度
    INT4 nOffset; //查询结果在查询文件中的偏移量
};

#endif
