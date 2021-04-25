/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:mml_para.h
    描述信息:
    
    包含CParameter、CParaValue、CParseResult类的定义.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
*/

#ifndef _MML_PARAMETER_H
#define _MML_PARAMETER_H

#include "../include/precom.h"
#include "../include/mmlcmd_def.h"

class CParaValue
{
protected:
    BYTE m_yValueFormat;        //参数值得类型
    STRING m_strMinValue;       //区间的下限
    STRING m_strMaxValue;       //区间的上限
    UINT4 m_uStep;             //区间值的步长

public:
    CParaValue();
    ~CParaValue();

    BYTE GetValueFormat();
    void SetValueFormat(BYTE yValueFormat);
    const char* GetMinValue();
    const char* GetMaxValue();
    void SetMinValue(const char* pValue);
    void SetMaxValue(const char* pValue);
    UINT4 GetStep();
    void SetStep(UINT4 uStep);
};


class CParameter
{
public:
    CParameter();
    ~CParameter();

    const char* GetParaName();
    UINT2 GetParaCode();
    PARA_TYPE GetDataType();
    OPERATOR_TYPE GetOperType();

    CParaValue* GetValue(UINT2 uValSeq = 1);

    void SetValueNum(UINT2 uVal);
    void SetParaName(char* pParaName);
    void SetDataType(BYTE yDataType);
    void SetOperType(BYTE yOperType);

    void AddValue(CParaValue* pValue);
    void SetParaCode(UINT2 uParaCode);
    UINT2 GetValueNum();

protected:

    char        m_szParaName[PARA_NAME_LEN];    //参数名
    UINT2       m_uParaCode;                    //参数码

    UINT2       m_uValueNum;                    //参数值个数
    PARA_TYPE   m_yDataType;                    //参数数据类型
    OPERATOR_TYPE   m_yOperType;                //操作类型

    VECTOR<CParaValue*> m_ValueVector;          //参数值列表
};

/*
    _ParaValue、_Parameter、_ParseResult三个结构用于序列化
    CParseResult对象.
*/
struct _ParaValue
{
    BYTE    yValueFormat;				//值的格式,取值为VALUE_FORMAT
    char    szMinValue[256];			//区间值的最小值或单值
    char    szMaxValue[256];			//区间值的最大值,单值时不使用
    UINT4   uStep;						//区间值的步长,单值时不用
};

struct _Parameter
{
    char    szParaName[PARA_NAME_LEN];  //参数名
    UINT2   uParaCode;					//参数码
    BYTE    yOperType;					//操作类型,取值OPERATOR_TYPE
    UINT2   uValueNum;					//参数值个数
    BYTE    yDataType;					//参数数据类型,取值PARA_TYPE
};

struct _ParseResult
{
    UINT2   uCmdCode;					//命令码
    char    szCmdName[CMD_NAME_LEN];	//命令名称
	char    szCmdDspName[MAX_CMD_DSP_LEN];

    BYTE    yCmdLevel;					//命令级别,用于权限验证

    UINT2   uParaNumber;				//参数个数

    UINT2   uWsNo;						//发起命令的工作台号
    UINT2   uWsType;					//发起命令的工作台类型

    /* 发起命令的登录用户名 */
    char    szUserName[USER_NAME_LEN];

    BYTE    yUserLevel;         //用户级别,取值取值USER_LEVEL
    UINT4   uIP;                //发起命令的终端的IP地址
    char    szErrorMsg[256];    //解析出错信息
    char    szCmdLine[512];     //对应的原始MML命令
};

class CParseResult
{
protected:
    UINT2 m_uCmdCode;                   //命令码
    char m_szCmdName[CMD_NAME_LEN];     //命令名称
    char m_szCmdDspName[MAX_CMD_DSP_LEN];

    BYTE m_yCmdLevel;                   //命令级别

    UINT2 m_uParaNum;                   //参数个数
    VECTOR<CParameter*> m_ParaVector;   //参数列表

    UINT2 m_uWsNo;                      //发起命令的工作台号
    UINT2 m_uWsType;                    //发起命令的工作台类型

    char m_szUserName[USER_NAME_LEN];   //发起命令的登录用户名
    BYTE m_yUserLevel;                  //用户级别,取值取值USER_LEVEL
    UINT4 m_uIP;                        //发起命令的终端的IP地址

    STRING m_strErrorMsg;               //解析出错信息
    STRING m_strCmdLine;                //对应的原始MML命令

public:
    CParseResult(MSG_INTRA* pMsg);
    ~CParseResult();

    /* 设置获取m_uCmdCode */
    UINT2 GetCmdCode(char* pCmdName);
    UINT2 GetCmdCode();
    void  SetCmdCode(UINT2 uCmdCode,char* pCmdName);

    /* 设置获取m_yCmdLevel */
    BYTE  GetCmdLevel();
    void  SetCmdLevel(BYTE yCmdLevel);

    /* 设置获取m_uWsNo */
    void SetWorkStationNo(UINT2 n2WsNo);
    UINT2 GetWorkStationNo();

    /* 设置获取m_uWsType */
    void SetWorkStationType(UINT2 uWsType);
    UINT2 GetWorkStationType();

    /* 设置获取m_szUserName */
    const char* GetUserName();
    void  SetUserName(char* pUseName);

    /* 设置获取m_yUserLevel */
    BYTE  GetUserLevel();
    void  SetUserLevel(BYTE yUserLevel);

    /* 设置获取m_IP */
    UINT4 GetIP();
    void  SetIP(UINT4 uIP);

    /* 设置获取m_uParaNum */
    void  SetParaNum(UINT2 uParaNum);
    UINT2 GetParaNum();

    /* 在m_ParaVector中增加一个参数对象 */
    void AddPara(CParameter* pPara);

    /* 根据参数索引获得参数对象 */
    CParameter* GetPara(UINT2 uParaNo);

    /* 根据参数名获得参数对象 */
    CParameter* GetPara(const char* pParaName);

    void SetErrMsg(const char* pErrMsg);
    const char* GetErrMsg();

    void SetCmdLine(char* pCmdLine);
    const char* GetCmdLine();

    /*
        把一个CParseResult对象序列化,以便在进程间传送.
    */
    void Serialize(char* pData,const UINT4 uLength);

    /*
        获得CParseResult对象序列化后的长度.
    */
    UINT4 GetParseLength();

    /*
        根据消息中序列化的数据构造CParseResult对象.
    */
    void CreateParseResult(MSG_INTRA* pMsg);

public:
    const char* GetCmdName();

    /* 设置/获取命令显示 */
    void SetCmdDspName(const char* p);
    const char* GetCmdDspName();

    /* 根据参数码和取值序号(缺省为0),得到参数值的类型 */
    BYTE GetParaValueFormat(UINT2 uParaNo,UINT2 uValSeq = 1);

    /* 根据参数名和取值序号(缺省为0),得到参数值的类型 */
    BYTE GetParaValueFormat(char* pParaName,UINT2 uValSeq = 1);

    /*
        根据参数码和取值序号,得到字符串参数值.
        该函数只获取单值参数的值,若是区间类型的参数,返回NULL.
    */
    const char* GetStringVal(UINT2 uParaNo,UINT2 uValSeq = 1);

    /*
        根据参数码和取值序号,得到字符串参数值.
        该函数只获取单值参数的值,若是区间类型的参数,返回NULL.
    */
    const char* GetStringVal(const char* pParaName,
                                UINT2 uValSeq = 1);

    /*
        根据参数码和取值序号,得到数值参数值.
        只获取单值参数的值,若是区间类型的参数,返回FALSE.
        若参数类型不是数值型,返回FALSE.
    */
    BOOL GetDigitVal(INT* pOut,UINT2 uParaNo,UINT2 uValSeq = 1);

    /*
        根据参数名和取值序号,得到数值参数值.
        只获取单值参数的值,若是区间类型的参数,返回FALSE.
        若参数类型不是数值型,返回FALSE.
    */
    BOOL GetDigitVal(INT* pOut,char* pParaName,UINT2 uValSeq = 1);

    /*
        根据参数码得到某个参数的区间取值,输出字符串值.
    */
    BOOL GetRegionStringVal(UINT2 uParaNo,
                            char** pMinVal,   /* out,输出区间下限 */
                            char** pMaxVal,   /* out,输出区间上限 */
                            UINT2* puStep,    /* out,输出区间步长 */
                            UINT2 uValSeq = 1);

    /*
        根据参数名得到某个参数的区间取值,输出字符串值.
    */
    BOOL GetRegionStringVal(char* pParaName,
                            char** pMinVal,   /* out,输出区间下限 */
                            char** pMaxVal,   /* out,输出区间上限 */
                            UINT2* puStep,    /* out,输出区间步长 */
                            UINT2 uValSeq = 1);

    /*
        根据参数码得到某个参数的区间取值,输出数值型参数值.
    */
    BOOL GetRegionDigitVal(UINT2 uParaNo,
                           INT* pMinVal,    /* out,输出区间下限 */
                           INT* pMaxVal,    /* out,输出区间上限 */
                           UINT2* puStep,   /* out,输出区间步长 */
                           UINT2 uValSeq = 1);

    /*
        根据参数名得到某个参数的区间取值,输出数值型参数值.
    */
    BOOL GetRegionDigitVal(char* pParaName,
                           INT* pMinVal,    /* out,输出区间下限 */
                           INT* pMaxVal,    /* out,输出区间上限 */
                           UINT2* puStep,   /* out,输出区间步长 */
                           UINT2 uValSeq = 1);
};

#endif
