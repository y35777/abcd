/*
    产品名:iGateway Bill V200
    模块名:MML Server
    文件名:mml_persist_para.h
    描述信息:
    
    包含CPersistPara类的定义,该类实现命令参数的管理,
    包括从磁盘文件中读取参数配置,验证命令行参数的合法性等.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
*/

#ifndef _MML_PERSIST_PARAMETER_H
#define _MML_PERSIST_PARAMETER_H

#include "../include/precom.h"
#include "../utility/mml_para.h"
#include "../include/mmlcmd_def.h"

/*
    该类封装了参数/子参数的结构.
*/
class CPersistPara
{
protected:
    char m_szParaName[PARA_NAME_LEN];   /* 参数名       */
    UINT2 m_uParaCode;                  /* 参数码       */
    UINT2 m_uChildParaCode;             /* 子参数码     */
    UINT2 m_uCmdCode;                   /* 命令码       */
    BYTE m_yDataType;                   /* 参数类型     */
    BYTE m_yRangeType;                  /* 值域类型     */
    BYTE m_yMustGive;                   /* 必须给出标记 */
    INT4 m_nMin;                        /* 区间下限     */
    INT4 m_nMax;                        /* 区间上限     */
    char m_szDefValue[VALUE_LEN];       /* 缺省值       */
    UINT2 m_uChildParaNum;              /* 子参个数     */

    /*
        保存枚举或位域的名字与值的对应关系,
        第一个参数为枚举或位域的名字,第二个参数为枚举值,
        位域对应的是位序号.
        该属性只有值域类型为枚举或参数类型为位域时有效.
    */
    MAP<STRING,UINT4> m_EnumValueMap;

    /*
        该参数的子参列表,若没有子参时数组长度为0.
    */
    VECTOR<CPersistPara*> m_ChildPara;

public:
    CPersistPara(UINT2 uCmdCode,UINT2 uParaCode,
                 UINT2 uChildParaCode = MAX_UINT2);
    virtual ~CPersistPara();

    /*
        初始化参数/子参数.
    */
    int Init();
    int InitPara();
    int InitChildPara();

    /*
        检查参数值/子参数值的有效性.
    */
    BOOL CheckParaValue(const char* pInputValue,char* pOutputValue,
                        int nValType);
    BOOL CheckRangeValue(INT4 nMin,INT4 nMax,UINT4 uStep);
    BOOL CheckBitFieldValue(const char* pInputValue,
                            char* pOutputValue);

    BOOL CheckChildParaValue(UINT2 uChildParaCode,char* pIn,
                            char* pOut,int nValType);
    BOOL CheckChildRangeValue(UINT2 uChildParaCode,INT4 nMin,
                            INT4 nMax,UINT4 uStep);

    /*
        获取各个成员变量.
    */
    UINT2 GetChildParaNum();
    UINT2 GetParaCode();
    UINT2 GetChildParaCode();
    char* GetParaName();
    CPersistPara* GetChildPara(UINT2 uChildParaNo);
    char* GetDefaultValue();
    BYTE GetDataType();
    BOOL IsMustGive();
};


#endif
