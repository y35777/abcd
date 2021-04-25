#ifndef _MML_COMMAND_DEFINE_
#define _MML_COMMAND_DEFINE_

#include "precom.h"

#define MAX_COMMAND_CODE      30000                  //MML命令码的最大值 
#define MAX_CMD_NAME_LEN      32                     //MML命令名的最大长度
#define MAX_MACRO_NAME_LEN    64                     //宏定义最大长度
#define MAX_CMD_DES_LEN	      64                     //命令描述的最大长度
#define MAX_BUFFER_SIZE       (16 * 1024)            //缓冲区最大长度 


//这是MML SERVER定义的

#define USER_NAME_LEN   48      //用户名的最大长度
#define PARA_NAME_LEN   32      //MML参数名的最大长度
#define CMD_NAME_LEN    32      //MML命令名的最大长度
//by ldf 2004-04-07 对应问题单 SWPD04414
#define MAX_CMD_DSP_LEN 128      //命令显示名称的最大长度
//end
#define VALUE_LEN       64      //MML参数值的最大长度
#define ENUM_LEN        20      //枚举值名称长度

#define MAX_UINT2       0xFFFF  //2位无符号整数的最大值

#define SZ_MACRO_CODE        "macro_code"          //命令ID在MACRO.INI中得关键字名
#define SZ_PARA_NUM          "para_num"	           //命令行参数个数在MACRO.INI中的关键字名
#define SZ_MUST_GIVE         "must_give"           //命令行中必须给出的参数
#define SZ_MUST_CONFIRM      "MUST BE CONFIRM"	   //必须确认
#define SZ_ALL_MACROS        "all_macros"          //所有MML命令名称定义	
#define SZ_NODE_NUM          "node_num"            //MML命令节点数
#define SZ_MACRO_NAME        "macro_name"          //MML命令显示名称
#define SZ_COMPOUND_STRING   "fieldarray~1"
#define	SZ_COMPLEX_STRING    "complex~1"
#define SZ_CHILDPART	     "childpart"
#define SZ_BYTENUM           "bytenumber"
#define SZ_DEFAULT           "default("
#define SZ_TABLE             "table("
#define SZ_FIELD             "field("
#define SZ_QUERY_CMD	     "query_cmd"
#define SZ_PARA_MAP	         "para_map"
#define SZ_PARA_VALUE        "para_value"
#define SZ_CMD_LEVEL	     "cmd_level"
#define SZ_QUERY_CMD	     "query_cmd"
#define SZ_PARA_MAP	         "para_map"
#define SZ_CONFIRM_ASK	     "是否确认"
#define SZ_EMPTY_STR	     ""			//空字符串指针

//MML命令定义级别
//(MML SERVER使用枚举)

#define CMD_LEVEL_ADMIN       0			//只有管理员有权使用
#define CMD_LEVEL_OPER	      1			//普通操作员可以使用


//参数数据类型
typedef enum       
{
    TYPE_CHAR          = 0,        /* 单字节 */
    TYPE_SHORT         = 1,        /* 双字节 */
    TYPE_LONG          = 2,        /* 双字 */
    TYPE_TELE          = 3,        /* 电话号码类型 */
    TYPE_STRING        = 4,        /* 字符串 */
    TYPE_DATE          = 5,        /* 日期型 */
    TYPE_TIME          = 6,        /* 时间类型 */
    TYPE_IP            = 7,        /* IP地址 */
    TYPE_SPC           = 8,        /* 信令点编码 */
    TYPE_CELLID        = 9,        /* 小区编码 */
    TYPE_RAI           = 10,       /* 路由区编码 */
    TYPE_BYTE          = 11,       /* 位域类型 */
    TYPE_COMPLEX       = 12,       /* 父参类型 */
    TYPE_BCD           = 13,       /* BCD码类型 */
    TYPE_IMSI          = 14,       /* IMSI号类型 */
    TYPE_DATETIME      = 15,       /* 日期时间型 */
    TYPE_HEXADECIMAL   = 16,       /* 十六进制型 */
    TYPE_SUBCOMPLEX    = 17,       /* 子参类型 */
    TYPE_BUTT
}DATA_TYPE;

#define  PARA_ENABLE        1     //命令允许不带参数
#define  PARA_DISABLE       0     //命令不允许不带参数


//参数可选择性
typedef enum  
{
   OPT_ENABLE       = 0,        /* 可选 */        
   OPT_MUST         = 1         /* 必选 */
}MML_PARA_OPTIONAL;


/* 定义命令表记录结构 (MML SERVER)*/
typedef struct _CmdDef
{
    UINT2   uCmdCode;                       //命令码
    char    szCmdName[CMD_NAME_LEN];        //命令名称
    UINT2   uParaNum;                       //命令参数个数
    BYTE    yLevel;                         //命令级别
    char    szDspName[MAX_CMD_DSP_LEN];     //命令的显示名称

}SCmdDef,*PCmdDef;


//MML命令参数描述

/* 定义参数允许的运算操作 (MML SERVER)*/
enum OPERATOR_TYPE
{
    OT_ASSIGN           = 0,    //赋值
    OT_ASCEND           = 1,    //升序排列
    OT_DESCEND          = 2,    //降序排列

    OT_EQUAL            = 3,    //相等关系
    OT_LARGER           = 4,    //大于关系
    OT_SMALLER          = 5,    //小于关系
    OT_LARGER_EQUAL     = 6,    //大于等于关系
    OT_SMALLER_EQUAL    = 7,    //小于等于关系
    OT_NOT_EQUAL        = 8     //不等于关系
};

/* 参数类型定义  (MML SERVER)*/
enum PARA_TYPE
{
    PT_DIGIT            = 0,    //数值型
    PT_BCD              = 1,    //BCD码
    PT_STRING           = 2,    //普通字符串
    PT_BITFIELD         = 3,    //位域/枚举参数
    PT_COMPLEX          = 4,    //复合参数
    PT_ANYTYPE          = 5     //任意参数,当作字符串处理
};

/* 值类型定义  (MML SERVER)*/
enum VALUE_FORMAT
{
    VF_SINGLE           = 0,    //值是单值
    VF_REGION           = 1,    //值是区间
    VF_ERROR            = 0xFF  //无效,认为是单值
};

/* 用户级别  (MML SERVER)*/
enum USER_LEVEL
{
    UL_ADMIN            = 0,    //管理员用户
    UL_OPER             = 1     //普通操作用户
};

/* 命令级别  (MML SERVER)*/
enum CMD_LEVEL
{
    CL_ADMIN            = 0,    //管理员级别
    CL_OPER             = 1     //普通操作员级别
};

/* 工作台类型  (MML SERVER)*/
enum  STATION_TYPE
{
    ST_COMMON           = 0,    //普通维护类
    ST_ALAEM            = 1,    //告警类
    ST_DEBUG            = 2     //调试信息类
};

/*
    MML参数值域类型定义  (MML SERVER).
    只有当参数类型是PT_DIGIT时,值域类型才能是ENUM_ZONE
    和ENUM_ARATE.
*/
typedef enum
{
    ENUM_NONE   = 0,            /* 无值类型要求     */
    ENUM_ZONE   = 1,            /* 值域为区间类型   */
    ENUM_ARATE  = 2             /* 值域为枚举类型   */

}RangeType;

/*
    MML命令参数表记录结构.  (MML SERVER)
	对应磁盘文件:paradef.dat
*/
typedef struct _ParaDef
{
    UINT2 uCmdCode;                  /* MML命令码        */
    UINT2 uParaCode;                 /* MML命令参数码    */
    char  szParaName[PARA_NAME_LEN]; /* MML命令参数名称  */
    char  szDspName[PARA_NAME_LEN];  /* 参数的显示名称，例如上面的"命令组"、"命令名" */
    BYTE  yDataType;                 /* 参数数据类型     */
    char  szDefault[VALUE_LEN];      /* 参数缺省值       */
    BYTE  yMustGive;                 /* 参数必须给出标记 */
    BYTE  yRangeType;                /* 该参数值域类型   */
    BYTE  yByteNum;                  /* 字节数/子参个数  */

}SParaDef,*PParaDef;


/*
    MML命令子参数表记录结构.  (MML SERVER)
	对应磁盘文件:childparadef.dat
*/
typedef struct _ChildParaDef
{
    UINT2 uCmdCode;                  /* MML命令码        */
    UINT2 uParaCode;                 /* MML命令参数码    */
    UINT2 uChildParaCode;            /* 子参数码         */
    char  szParaName[PARA_NAME_LEN]; /* 参数名           */
    char  szDefValue[VALUE_LEN];     /* 缺省值           */
    char  szDspName[PARA_NAME_LEN];	 /* 子参数显示名称    */

    /*
        子参数的值域类型,子参数的参数类型都是数值.
    */
    BYTE  yRangeType;

}SChildParaDef,*PChildParaDef;


/*
    MML命令参数枚举值表记录结构.  (MML SERVER)
	对应磁盘文件:paraenumdef.dat
*/
typedef struct _ParaEnumDef
{
    UINT2 uCmdCode;                     /* MML命令码     */
    UINT2 uParaCode;                    /* 参数码        */
    UINT2 uChildParaCode;               /* 子参数码      */
    UINT4 uValue;                       /* 枚举值/位域号 */
    char  szInput[ENUM_LEN];            /* 枚举名/位域名 */
    char  szDspName[MAX_CMD_DSP_LEN];   /* 枚举的显示值  */

}SParaEnumDef,*PParaEnumDef;


/*
    MML命令区间参数值表记录结构.  (MML SERVER)
	对应磁盘文件:pararangedef.dat
*/
typedef  struct  _ParaRangeDef
{
    UINT2 uCmdCode;             /* MML命令    */
    UINT2 uParaCode;            /* 参数码     */
    UINT2 uChildParaCode;       /* 子参数码   */
    INT4  nMinValue;            /* 最小值     */
    INT4  nMaxValue;            /* 最大值     */

}SParaRangeDef,*PParaRangeDef;


BOOL IsParaCode(UINT2 nCmdCode, UINT2 nParaCode, char *pszParaName);
BYTE GetParaDataType(UINT2 nCmdCode, UINT2 nParaNo);

#endif
