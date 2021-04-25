#if !defined(_FORMAT_COMMON_H_)
#define _FORMAT_COMMON_H_
/**************** The Head Comment of This File *********************/
// Description:  The Common Head File of the Format Module
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "ace/OS.h"
#include "../include/base_type.h"
#include "format_def.h"


/*
#ifndef ASSERT
#define  ASSERT assert
#endif  //#ifndef ASSERT
*/


/****************** The Align Format Defining ***********************/
#define LEFT_ALIGN    0x00    //左对齐
#define MID_ALIGN     0x01    //居中对齐
#define RIGHT_ALIGN   0x02    //右对齐
/****************** The Align Format Defining ***********************/


/****************** The Data Type Length Defining *******************/
//定义数据类型的长度
#define LEN_INT2      (sizeof(INT2))
#define LEN_INT4      (sizeof(INT4))
#define LEN_INT8      (sizeof(INT8))

#define LEN_BYTE      (sizeof(BYTE))
#define LEN_UINT2     (sizeof(UINT2))
#define LEN_UINT4     (sizeof(UINT4))
#define LEN_BOOL      (sizeof(BOOL))

#define LEN_CHAR      (sizeof(char))
#define LEN_SHORT      (sizeof(short))
#define LEN_INT       (sizeof(int))
#define LEN_LONG      (sizeof(long))

#define LEN_UCHAR     (sizeof(unsigned char))
#define LEN_USHORT    (sizeof(unsigned short))
#define LEN_UINT      (sizeof(unsigned int))
#define LEN_ULONG     (sizeof(unsigned long))
/****************** End of The Data Type Length Defining ************/


/****************** The Data Type Defining **************************/

//表达式元素类型定义
enum EPostfixEleType
{
    ET_VARIANT,                     //变量，如话单域名 
    ET_INT_CONST,                   //整型常量
    ET_STRING_CONST,                //字符串常量
    ET_UNSIGNED_CHAR_CONST,         //字符常量
    ET_ARITH_OPERATOR,              //算术算子
    ET_LOGIC_OPERATOR,              //逻辑算子
    ET_FUN,                         //函数
    ET_HEX_CONST,                   //16进制类型
    ET_NOTDEFINE                    //未定义类型
};

//操作符的类型定义
enum EOperatorType
{
    OT_UNARY_OPERATOR,
    OT_BINARY_OPERATOR,
    OT_NOTDEFINE                    //未定义类型
};

//话单域或表达式计算类型定义
enum EFIELD_TYPE
{
    FT_CHAR   =     0,              //字符型，1字节
    FT_BYTE   =     1,              //无符号字符型，1字节
    FT_SHORT  =     2,              //短整型，2字节
    FT_USHORT =     3,              //无符号短整型，2字节
    FT_LONG   =     4,              //长整型，4字节
    FT_ULONG  =     5,              //无符号长整型，4字节
    FT_INT8   =     6,              //64位整型，8字节
    FT_BCD    =     7,              //BCD码，1字节
    FT_RBCD   =     8,              //反向BCD码，1字节
    FT_STRING =     9,              //字符串类型
    FT_HEX    =     10              //十六进制类型
};

//话单域作为查询条件的数据类型
enum EQUERYCOND_FIELD_TYPE
{
    QT_CHAR     =    0,             //字符类型
    QT_NUMBER   =    1,             //数值类型
    QT_STRING   =    2,             //字符串类型
    QT_ENUM     =    3,             //枚举类型
    QT_TIME     =    4,             //时间类型
    QT_DATE     =    5,             //日期类型
    QT_HEX      =    6              //十六进制类型
};


//字节序转换标志枚举定义
enum EByteOrderFlg
{
    INTEL_ORDER =    0,    //表示格式为INTEL字节序
    NET_ORDER   =    1    //表示格式为网络字节序
};


//表达式元素结构定义
typedef struct _PostfixExpEle
{
        BYTE   yEleType;            //表达式元素的类型，为EPostfixEleType枚举型
                                    //如果为ET_NOTDEFINE，则表示该元素无效
        char   szEleContent[63];    //表达式元素，为字符串型
} SPostfixExpEle, *PPostfixExpEle;

//表达式链表结构类型定义
typedef  LIST<SPostfixExpEle> PostfixExpEleList;

//表示计算单元类型定义
typedef struct _Variant
{
    BYTE yVariantType;              //操作数取值类型的标识
    union 
    {
        BYTE     yValue;            //无符号字符型，FT_BYTE或FT_CHAR                
        INT2     n2Value;           //整型，FT_SHORT
        INT4     n4Value;           //整型，FT_LONG
        INT8     n8Value;           //整型，FT_INT8
        UINT2    u2Value;           //整型，FT_USHORT
        UINT4    u4Value;           //整型，FT_ULONG
        char     szValue[63];       //字符串，FT_BCD或FT_RBCD
    };
} SVariant, *PVariant;

//操作符优先级结构定义
typedef struct _OPERATOR_PRIORITY
{
    unsigned char  Left;            //左边的操作符
    unsigned char  Right;           //右边的操作符
    int            nPriority;       //操作符优先级
} SOperatorPriority, *POperatorPriority;

//操作符结构定义
typedef struct _OPERATOR_SYMBOL
{
    unsigned char Left;             //左边的操作符
    unsigned char Right;            //右边的操作符
} SOperatorSymbol, *POperatorSymbol;

//话单域枚举类型定义
typedef struct _EnumInfo
{
	UINT2  nEnumValue;			  //枚举值
	char   szEnumValue[30];		  //与枚举值对应的字符串 
} SEnumInfo, *PEnumInfo;

//按1字节对齐的SEnumInfo结构的长度
//邱意平 2002年4月4日增加
#define SIZEOF_ENUMINFO  32
//添加完毕

//按1字节对齐SFieldInfo结构的长度
//邱意平 2002年4月4日增加
#define SIZEOF_FIELDINFO    287
//添加完毕

//话单域枚举值链表类型定义
typedef LIST<SEnumInfo> ENUMINFO_LIST;

//格式域信息描述
typedef struct   _FieldInfo		
{
	UINT2           nFieldIndex;		  //域序号，从零开始依次递增编号
	char            szFieldName[64];	  //域名
	char            szFieldDescribe[64];  //域描述，域名的详细解释
	BYTE            nDataType;		      //域数据类型，为EFIELD_TYPE枚举型
    BYTE            nQueryCondDataType;   //查询条件数据类型，为EQUERYCOND_FIELD_TYPE枚举型
	BYTE            flgBit;			      //是否是位域型，非零表示该域是位域型
	UINT2           nFieldLength;		  //域长度，对位域型，它指示位域的位数；对非位域型，
                                          //它指示字段的维数，例如int a，长度为1；int a[4]长度为4；                                                                       //int C: 3，则长度为3。
	BYTE            flgEnum;		      //枚举标志，非零表示枚举型
	ENUMINFO_LIST*  pEnumInfos;           //枚举结构链表，当一个域为枚举型时，该域存放枚举信息
	char            szConvertFun[128];	  //转换函数，即从源格式到目标格式需经过的变换
	BYTE            nAlignMode;		      //对齐方式，在文本方式的最终目标格式中有意义，
                                          //0x01为左对齐，0x02为居中对齐，0x03为右对齐
	char            cFillChar;			  //填充符，在文本方式的最终目标格式中有意义
	char            cSeparator; 		  //该域与下一个域之间的分割符，在文本方式的最终目标格式中有意义
    INT4            nBitOffset;		      //该域在整个格式中按位计算的偏移(由配置计算得出)
	UINT4           nByteOffset;		  //该域在整个格式中按字节计算的偏移(由配置计算得出)
	BYTE            flgQuery;			  //该域是否用于查询
	BYTE            nOperator;		      //用于查询的逻辑算子，0x01表示‘==’，0x02表示‘<’，
                                          //0x04表示‘>’，0x08表示‘<=’，0x10表示‘>=’，0x20表示‘!=’
	char            szUnit[5]; 			  //查询时，该域的取值单位，例如“秒”、“个” 
	BYTE            nOrderMode;           //字节序转换模式
										  //INTEL_ORDER: INTEL字节序
										  //  NET_ORDER: 网络字节序
} SFieldInfo, *PFieldInfo;


//以话单域名为键值，索引该话单域的结构类型定义
typedef MAP<STRING, SFieldInfo*> FIELD_MAP;

/****************** The CONVERTFUN Defining *************************/
// 功能描述：转换函数类型定义
//
// 输入参数：
//   nSrcFields：计算当前域所需的源格式话单中相关域的个数
//   pSrcFields：计算当前域所需的源格式话单中的相关域值的指针数组，
//               数组长度由nSrcFields指示
// nSrcFieldLen：计算当前域所需的源格式话单中的相关域的长度的数组，
//               数组长度由nSrcFields指示，该数组与pSrcFields存在一一对应关系
//   pFieldInfo：当前域的描述，在目标格式为文本格式的时候，每转换一个域，
//               需要为目标格式域增加填充符和分隔符，为了在给各个转换函数提供方便，
//               在调用实际的转换函数时，pSrcFields中最后一个元素固定填写目标格式域
//               的描述SFieldInfo
//     pDefault：当前域的缺省取值
//  nDefaultLen：当前域的缺省取值的长度
//
// 输出参数：
//   pCurtField：当前域值的指针，转换函数的调用者负责初始化该指针；
//               转换函数负责将当前域的取值写入该指针指向的内存区；
//               转换函数不能修改该指针的取值
//     nIsQuery：是否查询标志，1表示该函数为查询条件转换，
//               0表示不是为查询条件转换
//
//   返回值：无
//
//   关键点：
//         
/********************************************************************/
typedef int (*CONVERTFUN)(void* pCurField,
                          const UINT4 nSrcFields,
                          const void* pSrcFields[],
			              const UINT4 nSrcFieldLen[],
			              const SFieldInfo* pFieldInfo,
			              const void* pDefault,
			              const UINT4 nDefaultLen,
                          int nIsQuery);
/****************** End of The CONVERTFUN Defining ******************/

/****************** The FINDFUN Defining ****************************/
// 功能描述：查找函数指针的函数类型定义
//
// 输入参数：
//   nSrcFields：计算当前域所需的源格式话单中相关域的个数
//nSrcFieldType：计算当前域所需的源格式话单中的相关域的类型，
//               该数组与CONVERTFUN的输入参数pSrcFields存在一一对应关系，
//               各种类型的取值可参见FIELD_TYPE的定义
//
// 输出参数：无
//
//   返回值：CONVERTFUN类型的函数指针
//
//   关键点：
//         
/********************************************************************/
typedef CONVERTFUN (* FINDFUN)(const UINT4 nSrcFields, 
                               const BYTE nSrcFieldType[]);
/****************** End of The FINDFUN Defining *********************/

#define  MAX_SRC_NUM  8	        //计算目标格式最多需要用到的源格式域个数


/****************** The CONDITIONFUN Defining ***********************/
// 功能描述：条件函数类型定义
//
// 输入参数：
//    nParasNum：参数个数
//       pParas：参数值指针数组，数组长度由nParasNum指示
//    nParasLen：各参数的长度的数组，数组长度由nParasNum指示，该数组与pParas存在一一对应关系
//     pDefault：函数的缺省取值
//  nDefaultLen：函数的缺省取值的长度
//
// 输出参数：无
//
//   返回值：如果计算正确，则返回SVariant*类型的计算结果，如果出错返回NULL
//
//   关键点：
//         
/********************************************************************/
typedef int (*CONDITIONFUN)(const UINT4 nParasNum,
							const void* pParas[],
							const UINT4 nParasLen[],
							const STRING& pDefault,
							SVariant& stVar);
/****************** End of The CONDITIONFUN Defining ****************/

#define  MAX_PARA_NUM  8	        //条件函数的最大参数个数


//Modified by XDB on 2003.04.01
//增加常量参数列表和缺省值参数
//目标格式域与其转换函数指针的对应关系结构定义
typedef struct FIELD_CONVERT_MAP
{
	SFieldInfo*    pFieldInfo;			 //目标格式域描述
	BYTE	       ySrcNum;			     //计算本格式域需要用到的源格式域的个数
	UINT2	       Index[MAX_SRC_NUM];   //计算本格式域需要用到的源格式各个域的索引  将BYTE改为UINT2 ZhengYuqun SWPD03695 2004-02-20
	CONVERTFUN     ConvertFun;		     //对应的转换函数指针
    LIST<STRING>   ConstValues;          //常量参数列表
    STRING         sDefault;             //缺省值
} SFieldConvertMap, *PFieldConvertMap;

//以格式域名为索引的格式域转换函数的映射类型定义
typedef MAP<STRING, SFieldConvertMap*> CONVERT_MAP;

//格式配置接口标识枚举定义
enum IID
{
	IID_FP_IFORMATCFG,        //格式配置接口标识
	IID_FP_IBILLPROCESSOR,    //话单处理接口标识
	IID_FP_IQUERYBROWSE       //查询浏览接口标识
};


//3G话单查询条件域名与域值对应关系类型定义
typedef struct _QUERY_COND_FIELDS_VALUE
{
    UINT2  nFieldIndex;		  //域序号
    STRING sFieldName;
    STRING sFieldValue;
} SQUERY_COND_FIELDS_VALUE, *PQUERY_COND_FIELDS_VALUE;
typedef VECTOR<SQUERY_COND_FIELDS_VALUE> QUERY_COND_FIELDS_VALUE;

/****************** End of The Data Type Defining *******************/

//邱意平 2002年4月4日增加如下处理字节序的宏定义
# define SWAP_LONG(L) ((SWAP_WORD ((L) & 0xFFFF) << 16) \
        | SWAP_WORD(((L) >> 16) & 0xFFFF))
# define SWAP_WORD(L) ((((L) & 0x00FF) << 8) | (((L) & 0xFF00) >> 8))

# define SWAP_LLONG(L)((SWAP_LONG ((L) & 0xFFFFFFFF) << 32) \
        | SWAP_LONG(((L) >> 32) & 0xFFFFFFFF))
#if defined (_PLATFORM_WIN32) || defined (_PLATFORM_LINUX) 

    # define ITOHS(S) (S)
    # define ITOHL(L) (L)
    #define  ITOHLL(L) (L)

    # define HTOIS(S) (S)
    # define HTOIL(L) (L)

    # define NTOHS(S) SWAP_WORD(S)
    # define NTOHL(L) SWAP_LONG(L)

    # define HTONS(S) SWAP_WORD(S)
    # define HTONL(L) SWAP_LONG(L)
	//added by Gan Zhiliang 处理字节序的问题INT8
	#define HTONLL(L)  SWAP_LLONG(L)
	#define NTOHLL(L)  SWAP_LLONG(L)

#else

    # define ITOHS(S) SWAP_WORD(S)
    # define ITOHL(L) SWAP_LONG(L)
    #define  ITOHLL(L) SWAP_LLONG(L)

    # define HTOIS(S) SWAP_WORD(S)
    # define HTOIL(L) SWAP_LONG(L)

    # define NTOHS(S) (S)
    # define NTOHL(L) (L)

    # define HTONS(S) (S)
    # define HTONL(L) (L)

	#define HTONLL(L)  (L)
	#define NTOHLL(L)  (L)
#endif //_PLATFORM_WIN32
//添加完毕 邱意平

//邱意平 2004-06-24 添加内容计费格式标识
//因为内容计费的客户化程度很高，不同运营商定义的内容计费字段的格式可能完全不同
//CBB - Content Based Billing
enum CBB_OPERATOR_ID
{
	CBB_OPERATOR_ID_DEFAULT	= 0,
	CBB_OPERATOR_ID_CMCC	= 1	
};


enum FILE_FORMAT
{
    FILE_FMT_UNKNOWN = 0,
    FILE_FMT_BINARY  = 1,
    FILE_FMT_ASCII	 = 2
};

#endif  //!defined(_FORMAT_COMMON_H_)

