#ifndef __COLLECT_MESSAGEDEF_H__
#define __COLLECT_MESSAGEDEF_H__

#include "../include/precom.h"

struct SSwitchFileInfo
{
    int   nSwitchIndex;               //交换机的索引号
    char  szModuleLabel[50];          //模块信息
    char  szRemoteFile[MAX_PATH];     //交换机上的原始文件名
    char  szLocalFile[MAX_PATH];      //要转换的话单文件路径
    int   nConvertInfo;               //话单文件的类型
};

//采集模块发往采集转换模块"转换话单文件"消息所带的内容
struct SCollectConvertFileInfo
{
    int   nFileNum;             
    SSwitchFileInfo info;
};
  
#define INFO_CONVERT_FILE_LENGTH  (sizeof(SCollectConvertFileInfo))

//采集转换模块发往采集模块"转换话单文件应答"消息所带的内容
struct SCollectConvertResult
{
    int   nFileNum;
    int   nConvertResult;                    //转换结果
};

#define COL_RECOL_BILL      40001
#define COL_WRITE_BILL_FAIL 40002



#endif //__COLLECT_MESSAGEDEF_H__
