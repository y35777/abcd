#ifndef __STAT_CFG_INFO_H__
#define __STAT_CFG_INFO_H__

#include "../include/toolbox.h"

#pragma warning(disable :66)
#pragma warning(disable :21)
#pragma warning(disable :46)

#define MSG_BUFFER_MAXSIZE   500 * 1024

//统计配置文件头信息的长度
#define STAT_CFG_HEAD_LENGTH    20

#define INCMEMLENGTH         1024 * 400

//统计配置文件头信息
struct SStatCfgHead
{
    UINT4 uVersion;       //文件的版本号
    UINT4 uAccessPoint;   //接入点号
    UINT4 uStatItemCount; //统计项目个数
    BYTE  yReserved[8];   //保留字节
};


//统计子项配置信息的长度
#define SUBITEMCFGLENGTH     576

//统计子项配置信息
struct SStatSubItemCfg
{
    char  szStatSubItemName[64];  //统计子项名
    char  szStatSubItemCond[512]; //统计条件表达式
};

//统计项目头信息的头长度
#define STATITEMCFGLENGTH    68

//统计项目头信息
struct SStatItemCfg
{
    char  szStatItemName[64];     //统计项名
    UINT4 uStatItemCount;         //表示后续统计配置信息的个数
    SStatSubItemCfg subItemCfg[1];//统计子项配置信息数组

    void* operator new(size_t classSize, size_t nCount = 1)
    {
        void *ptr = NULL;

        if( nCount < 1)
        {
            nCount = 1;
        }

        UINT4 length = nCount * sizeof(SStatSubItemCfg) + 68;

        if (length > MSG_BUFFER_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new BYTE[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SStatItemCfg*)ptr)->uStatItemCount = nCount;
        }

        return ptr;
    };
};

//一条统计子项的配置信息
struct SOneStatSubItemCfg
{
    char szStatItemName[64];    //统计项目名
    char szStatSubItemName[64]; //统计子项名
    char szStatSubItemCond[512]; //统计条件表达式
};



//统计结果文件头信息的长度
#define STAT_RST_HEAD_LENGTH  20

//统计结果文件头信息
struct SStatRstHead
{
    UINT4  uVersion;        //文件的版本号
    UINT4  uAccessPoint;    //接入点号
    time_t tStatEndTime;    //统计截止时间
    BYTE   yReserved[8];    //保留字节
};


#define STATEXPINDEXMSGLENGTH 520

struct SStatExpIndexMsg
{
    char  szStatExp[512]; //统计表达式
    BOOL  bInUsed;        //是否在使用中
    UINT4 nStatExpIndex;  //统计表达式索引号
};


//统计表达式索引表信息
struct SStatExpIndexList
{
    UINT4 uStatExpCount;           //统计表达式个数
    SStatExpIndexMsg expIndex[1];  //统计表达式信息

    void* operator new(size_t classSize, size_t nCount = 1)
    {
        void *ptr = NULL;

        if( nCount < 1)
        {
            nCount = 1;
        }

        UINT4 length = nCount * sizeof(SStatExpIndexMsg) + sizeof(UINT4);

        if (length > MSG_BUFFER_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new BYTE[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SStatExpIndexList*)ptr)->uStatExpCount = nCount;
        }

        return ptr;
    };
};

//一个统计子项一天的结果信息的长度
#define ONESTATEXPRSTLENGTH 12

//一个统计子项一天的结果信息
struct SOneStatExpRst
{
    INT4  nStatExpIndex; //统计表达式索引号
    INT4  nCDRCount;     //话单张数
    INT4  nValueTotal;   //域的累计值
};


//所有统计子项一天的结果信息
struct SAllStatExpRst
{
    UINT4 uStatEndTime;         //统计截止时间
    UINT4 uStatSubItemCount;    //后续统计子项数
    SOneStatExpRst ItemRst[1];  //个数必须与统计子项数一致

    void* operator new(size_t classSize, size_t nCount = 1)
    {
        void *ptr = NULL;

        if( nCount < 1)
        {
            nCount = 1;
        }

        UINT4 length = nCount * sizeof(SOneStatExpRst) + 2 * sizeof(UINT4);

        if (length > MSG_BUFFER_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new BYTE[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SAllStatExpRst*)ptr)->uStatSubItemCount = nCount;
        }

        return ptr;
    };
};

typedef LIST<SOneStatSubItemCfg*> ONESTATSUBITEMCFG_LIST;
typedef LIST<STRING>     INFO_LIST;

struct SCdrStatTaskInfo
{
    char  szAPName[16];
    char  szRelease[16];
    UINT4 uAPNumber;
    UINT4 uAPType;
    UINT4 uBillType;
    UINT4 uBillLen;
    BOOL  bMustMerge;
    BOOL  bFinalSPFileFmt;
};

typedef LIST<SCdrStatTaskInfo*> TASKINFO_LIST;

#define MAX_COND_LEN 2048             //查询条件的最大长度

#endif //__STAT_CFG_INFO_H__
