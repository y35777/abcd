/*
    产品名:iGateway Bill V200
    模块名:框架
    文件名:mml_para.cpp
    描述信息:
    
    包含CParameter、CParaValue、CParseResult类的实现.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
*/
#include "mml_para.h"

/*
    下面是CParaValue类的实现.
*/

CParaValue::CParaValue()
{
    m_yValueFormat = VF_SINGLE;
    m_strMinValue = "";
    m_strMinValue = "";
    m_uStep = 0;
}

CParaValue::~CParaValue()
{
}

BYTE CParaValue::GetValueFormat()
{
    return m_yValueFormat;
}

void CParaValue::SetValueFormat(BYTE yValueFormat)
{
    m_yValueFormat = yValueFormat;
}

const char* CParaValue::GetMinValue()
{
    return m_strMinValue.c_str();
}

const char* CParaValue::GetMaxValue()
{
    return m_strMaxValue.c_str();
}

void CParaValue::SetMinValue(const char* pValue)
{
    m_strMinValue = pValue;
}

void CParaValue::SetMaxValue(const char* pValue)
{
    m_strMaxValue = pValue;
}

UINT4 CParaValue::GetStep()
{
    return m_uStep;
}

void CParaValue::SetStep(UINT4 uStep)
{
    m_uStep = uStep;
}


/*
    下面是CParameter类的实现.
*/

CParameter::CParameter()
{
    m_uValueNum = 0;
    m_uParaCode = 0;
    m_szParaName[0] = 0;
    m_yDataType = PT_DIGIT;
    m_yOperType = OT_ASSIGN;
}

/*
    函数功能:析构函数,释放所有子参数值.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
CParameter::~CParameter()
{
    VECTOR<CParaValue*>::iterator i = m_ValueVector.begin(),
                                  e = m_ValueVector.end();
    
    while(i != e)
    {
        delete *i;
        i++;
    }

    m_ValueVector.clear();
}

const char* CParameter::GetParaName()
{
    return m_szParaName;
}

UINT2 CParameter::GetParaCode()
{
    return m_uParaCode;
}

PARA_TYPE CParameter::GetDataType()
{
    return m_yDataType;
}

OPERATOR_TYPE CParameter::GetOperType()
{
    return m_yOperType;
}

void CParameter::AddValue(CParaValue* pValue)
{
    m_ValueVector.push_back(pValue);
    m_uValueNum++;
}

void CParameter::SetParaCode(UINT2 uParaCode)
{
    m_uParaCode = uParaCode;
}

void CParameter::SetParaName(char* pParaName)
{
    strncpy(m_szParaName,pParaName,PARA_NAME_LEN);
    m_szParaName[PARA_NAME_LEN - 1] = 0;
}

void CParameter::SetValueNum(UINT2 uVal)
{
    m_uValueNum = uVal;
}

void CParameter::SetDataType(BYTE yDataType)
{
    m_yDataType = (enum PARA_TYPE)yDataType;
}

void CParameter::SetOperType(BYTE yOperType)
{
    m_yOperType = (enum OPERATOR_TYPE)yOperType;
}

UINT2 CParameter::GetValueNum()
{
    return m_uValueNum;
}

CParaValue* CParameter::GetValue(UINT2 uValSeq)
{
    if(uValSeq > m_ValueVector.size()) return NULL;

    return m_ValueVector[uValSeq - 1];
}

/*
    下面是CParseResult类的实现.
*/

CParseResult::CParseResult(MSG_INTRA* pMsg)
{
    m_uWsNo     = pMsg->ClientNo;   //工作台号
    m_uWsType   = pMsg->AppType;    //工作台类型
    m_uIP       = pMsg->IPAddr;     //工作台IP地址

    /* 原始的MML命令 */
    m_strCmdLine = (char*)(pMsg->Data);

    strncpy(m_szUserName,pMsg->UserName,USER_NAME_LEN);
    m_szUserName[USER_NAME_LEN - 1] = 0;

    m_uCmdCode = pMsg->CmdCode;
    m_szCmdName[0] = 0;
    m_yUserLevel = UL_OPER;
    m_yCmdLevel = CL_ADMIN;
    m_uParaNum = 0;
}

/*
    函数功能:析构函数,释放所有参数值.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
CParseResult::~CParseResult()
{
    VECTOR<CParameter*>::iterator i = m_ParaVector.begin(),
                                  e = m_ParaVector.end();
    while(i != e)
    {
        delete *i;
        i++;
    }

    m_ParaVector.clear();
}

UINT2 CParseResult::GetCmdCode(char* pCmdName)
{
    strcpy(pCmdName,m_szCmdName);
    return m_uCmdCode;
}

UINT2 CParseResult::GetCmdCode()
{
    return m_uCmdCode;
}

void CParseResult::SetCmdCode(UINT2 uCmdCode,char* pCmdName)
{
    m_uCmdCode = uCmdCode;
    strncpy(m_szCmdName,pCmdName,CMD_NAME_LEN);
    m_szCmdName[CMD_NAME_LEN - 1] = 0;
}

BYTE CParseResult::GetCmdLevel()
{
    return m_yCmdLevel;
}

void CParseResult::SetCmdLevel(BYTE yCmdLevel)
{
    m_yCmdLevel = yCmdLevel;
}

void CParseResult::SetWorkStationNo(UINT2 uWsNo)
{
    m_uWsNo = uWsNo;
}

UINT2 CParseResult::GetWorkStationNo()
{
    return m_uWsNo;
}

void CParseResult::SetWorkStationType(UINT2 uWsType)
{
    m_uWsType = uWsType;
}

UINT2 CParseResult::GetWorkStationType()
{
    return m_uWsType;
}

const char* CParseResult::GetUserName()
{
    return m_szUserName;
}

void CParseResult::SetUserName(char* pUseName)
{
    strncpy(m_szUserName,pUseName,USER_NAME_LEN);
    m_szUserName[USER_NAME_LEN - 1] = 0;
}

BYTE CParseResult::GetUserLevel()
{
    return m_yUserLevel;
}

void CParseResult::SetUserLevel(BYTE yUserLevel)
{
    m_yUserLevel = yUserLevel;
}

UINT4 CParseResult::GetIP()
{
    return m_uIP;
}

void CParseResult::SetIP(UINT4 uIP)
{
    m_uIP = uIP;
}

void CParseResult::SetParaNum(UINT2 uParaNum)
{
    m_uParaNum = uParaNum;
}

UINT2 CParseResult::GetParaNum()
{
    return m_uParaNum;
}

void CParseResult::AddPara(CParameter* pPara)
{
    m_ParaVector.push_back(pPara);
    m_uParaNum++;
}

/*
    函数功能:根据参数码获取参数对象指针.

    参数:
    uParaNo,参数码.
    一个参数在命令中既可以参数名出现,又可以参数码出现.

    返回:
    在参数表中查找参数码匹配的参数,若存在匹配的参数对象,
    返回参数对象的指针,否则返回NULL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
CParameter* CParseResult::GetPara(UINT2 uParaNo)
{
    for(unsigned int i = 0;i < m_ParaVector.size();i++)
    {
        if((m_ParaVector[i]->GetParaCode()) == uParaNo)
        {
            return m_ParaVector[i];
        }
    }

    return NULL;
}

/*
    函数功能:根据参数名获取参数对象指针.

    参数:
    pParaName,参数名.
    一个参数在命令中既可以参数名出现,又可以参数码出现.
    命令名不区分大小写.

    返回:
    在参数表中查找参数名匹配的参数,若存在匹配的参数对象,
    返回参数对象的指针,否则返回NULL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
CParameter* CParseResult::GetPara(const char* pParaName)
{
    const char* p = NULL;
    for(unsigned int i = 0;i < m_ParaVector.size();i++)
    {
        p = m_ParaVector[i]->GetParaName();
        if(ACE_OS::strcasecmp(p,pParaName) == 0)
        {
            return m_ParaVector[i];
        }
    }

    return NULL;
}

void CParseResult::SetErrMsg(const char* pErrMsg)
{
    m_strErrorMsg = pErrMsg;
}

const char* CParseResult::GetErrMsg()
{
    return m_strErrorMsg.c_str();
}

void CParseResult::SetCmdLine(char* pCmdLine)
{
    m_strCmdLine = pCmdLine;
}

const char* CParseResult::GetCmdLine()
{
    return m_strCmdLine.c_str();
}


/*
    函数功能:序列化一个CParseResult对象,以便于在进程间传送.
    在调用该函数之前应该使用GetParseLength()函数判断缓冲区
    的长度,以保证不会溢出.

    参数:
    pData,输出序列化结果的缓冲区.
    序列化的结果为如下结构:

    {
        _ParseResult ParseResult
        {   //第一个参数
            _Parameter Parameter
            _ParaValue  paravalue[Parameter.uValueNum]
        }
        ...
        {   //第n个参数
            _Parameter Parameter
            _ParaValue  paravalue[Parameter.uValueNum]
        }
    }

    uLength,缓冲区的长度.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
void CParseResult::Serialize(char* pData,const UINT4 uLength)
{
    assert(uLength >= GetParseLength());
    char* p = pData;

    _ParseResult parse_result;

    memcpy(parse_result.szCmdName,m_szCmdName,CMD_NAME_LEN);
    memcpy(parse_result.szUserName,m_szUserName,USER_NAME_LEN);
	memcpy(parse_result.szCmdDspName,m_szCmdDspName,MAX_CMD_DSP_LEN);

    strncpy(parse_result.szErrorMsg,m_strErrorMsg.c_str(),255);
    strncpy(parse_result.szCmdLine,m_strCmdLine.c_str(),511);
    parse_result.szErrorMsg[255] = 0;
    parse_result.szCmdLine[511] = 0;

    parse_result.yCmdLevel = m_yCmdLevel;
    parse_result.yUserLevel = m_yUserLevel;

    parse_result.uCmdCode = m_uCmdCode;
    parse_result.uParaNumber = m_uParaNum;
    parse_result.uWsNo = m_uWsNo;
    parse_result.uWsType = m_uWsType;
    parse_result.uIP = m_uIP;

    memcpy(p,&parse_result,sizeof(_ParseResult));
    p += sizeof(_ParseResult);

    /* 序列化各个参数 */
    _Parameter para;
    _ParaValue val;
    CParameter* pPara = NULL;
    CParaValue* pVal = NULL;

    for(unsigned int i = 0,j = 0;i < m_ParaVector.size();i++)
    {
        pPara = m_ParaVector[i];

        memcpy(para.szParaName,pPara->GetParaName(),PARA_NAME_LEN);
        para.uParaCode = pPara->GetParaCode();
        para.uValueNum = pPara->GetValueNum();
        para.yDataType = pPara->GetDataType();
        para.yOperType = pPara->GetOperType();

        memcpy(p,&para,sizeof(_Parameter));
        p += sizeof(_Parameter);

        /* 序列化各个参数值(子参数),子参数的序号从1开始. */
        for(j = 1;j <= para.uValueNum;j++)
        {
            pVal = pPara->GetValue(j);

            strncpy(val.szMaxValue,pVal->GetMaxValue(),255);
            strncpy(val.szMinValue,pVal->GetMinValue(),255);
            val.szMaxValue[255] = 0;
            val.szMinValue[255] = 0;

            val.uStep = pVal->GetStep();
            val.yValueFormat = pVal->GetValueFormat();

            memcpy(p,&val,sizeof(_ParaValue));
            p += sizeof(_ParaValue);
        }
    }
}

UINT4 CParseResult::GetParseLength()
{
    UINT4 nLen = sizeof(_ParseResult) + m_uParaNum * 
                                        sizeof(_Parameter);
    for(int i = 0;i < m_uParaNum;i++)
    {
        nLen += m_ParaVector[i]->GetValueNum() * sizeof(_ParaValue);
    }

    return nLen;
}

/*
    函数功能:根据一个MSG_INTRA携带的数据创建CParseResult对象.
    根据pMsg->Data的数据和序列化的结构,将各成员变量赋值.

    参数:pMsg,消息指针.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
void CParseResult::CreateParseResult(MSG_INTRA* pMsg)
{
    assert(pMsg != NULL);

    _ParseResult* pResult = NULL;
    _Parameter* pPara = NULL;
    _ParaValue* pVal = NULL;

    char* p = (char*)(pMsg->Data);
    pResult = (_ParseResult*)p;

    m_strCmdLine = pResult->szCmdLine;
    m_strErrorMsg = pResult->szErrorMsg;
    memcpy(m_szCmdName,pResult->szCmdName,CMD_NAME_LEN);
    memcpy(m_szUserName,pResult->szUserName,USER_NAME_LEN);
	memcpy(m_szCmdDspName,pResult->szCmdDspName,MAX_CMD_DSP_LEN);

    m_yCmdLevel = pResult->yCmdLevel;
    m_yUserLevel = pResult->yUserLevel;

    m_uCmdCode = pResult->uCmdCode;
    m_uWsNo = pResult->uWsNo;
    m_uWsType = pResult->uWsType;
    m_uIP = pResult->uIP;
    m_uParaNum = pResult->uParaNumber;

    p += sizeof(_ParseResult);

    /* 下面解析各个参数 */
    for(int i = 0,j = 0;i < m_uParaNum;i++)
    {
        pPara = (_Parameter*)p;
        CParameter* q = new CParameter;

        q->SetParaName(pPara->szParaName);
        q->SetParaCode(pPara->uParaCode);
        q->SetDataType(pPara->yDataType);
        q->SetOperType(pPara->yOperType);

        p += sizeof(_Parameter);

        /* 下面解析各个参数值 */
        for(j = 0;j < pPara->uValueNum;j++)
        {
            pVal = (_ParaValue*)p;
            CParaValue* v = new CParaValue;

            v->SetMaxValue(pVal->szMaxValue);
            v->SetMinValue(pVal->szMinValue);
            v->SetStep(pVal->uStep);
            v->SetValueFormat(pVal->yValueFormat);

            p += sizeof(_ParaValue);

            q->AddValue(v);
        }

        m_ParaVector.push_back(q);
    }
}

/*
    函数功能:根据参数名和取值序号,得到参数值的类型.

    参数:
    pParaName,参数名.
    uValSeq,取值序号.

    返回:
    成功返回参数值的类型.
    失败返回VF_ERROR.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BYTE CParseResult::GetParaValueFormat(char* pParaName,
                                      UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(pParaName);
    if(pParameter == NULL)
        return VF_ERROR;

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return VF_ERROR;

    return pParaValue->GetValueFormat();
}

/*
    函数功能:根据参数码和取值序号,得到参数值的类型.

    参数:
    uParaNo,参数码.
    uValSeq,取值序号.

    返回:
    成功返回参数值的类型.
    失败返回VF_ERROR.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BYTE CParseResult::GetParaValueFormat(UINT2 uParaNo,UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(uParaNo);
    if(pParameter == NULL)
        return VF_ERROR;

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return VF_ERROR;

    return pParaValue->GetValueFormat();
}

/*
    函数功能:根据参数码和取值序号,得到字符串参数值.

    1.只获取单值参数的值,若是区间类型的参数,返回NULL.
    2.若参数类型是数值型,返回NULL.

    参数:
    uParaNo,参数码.
    uValSeq,取值序号.

    返回:
    成功返回参数值的指针(字符串参数).
    失败返回NULL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
const char* CParseResult::GetStringVal(UINT2 uParaNo,
                                       UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(uParaNo);
    if(pParameter == NULL)
        return "";

    if((pParameter->GetDataType() != PT_STRING)
        && (pParameter->GetDataType() != PT_BITFIELD)
        && (pParameter->GetDataType() != PT_BCD)
        && (pParameter->GetDataType() != PT_COMPLEX)
        && (pParameter->GetDataType() != PT_ANYTYPE))
    {
        return "";
    }

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return "";

    if(pParaValue->GetValueFormat() != VF_SINGLE)
        return "";

    return pParaValue->GetMinValue();
}

/*
    函数功能:根据参数名和取值序号,得到字符串参数值.

    1.只获取单值参数的值,若是区间类型的参数,返回NULL.
    2.若参数类型是数值型,返回NULL.

    参数:
    pParaName,参数名.
    uValSeq,取值序号.

    返回:
    成功返回参数值的指针(字符串参数).
    失败返回NULL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
const char* CParseResult::GetStringVal(const char* pParaName,
                                       UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(pParaName);
    if(pParameter == NULL)
        return "";

    if((pParameter->GetDataType() != PT_STRING)
        && (pParameter->GetDataType() != PT_BITFIELD)
        && (pParameter->GetDataType() != PT_BCD)
        && (pParameter->GetDataType() != PT_COMPLEX)
        && (pParameter->GetDataType() != PT_ANYTYPE))
    {
        return "";
    }

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return "";

    if(pParaValue->GetValueFormat() != VF_SINGLE)
        return "";

    return pParaValue->GetMinValue();
}


/*
    函数功能:根据参数码和取值序号,得到数值参数值.

    1.只获取单值参数的值,若是区间类型的参数,返回FALSE.
    2.若参数类型不是数值型,返回FALSE.

    参数:
    pOut,返回数值型参数.
    uParaNo,参数码.
    uValSeq,取值序号.

    返回:
    成功返回TRUE,失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CParseResult::GetDigitVal(INT* pOut,UINT2 uParaNo,
                               UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(uParaNo);
    if(pParameter == NULL)
        return FALSE;

    if((pParameter->GetDataType() != PT_DIGIT)
        && (pParameter->GetDataType() != PT_COMPLEX))
        return FALSE;

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return FALSE;

    if(pParaValue->GetValueFormat() != VF_SINGLE)
        return  FALSE;

    *pOut = atoi(pParaValue->GetMinValue());
    return TRUE;
}

/*
    函数功能:根据参数名和取值序号,得到数值参数值.

    1.只获取单值参数的值,若是区间类型的参数,返回FALSE.
    2.若参数类型不是数值型,返回FALSE.

    参数:
    pOut,返回数值型参数.
    pParaName,参数名.
    uValSeq,取值序号.

    返回:
    成功返回TRUE,失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CParseResult::GetDigitVal(INT* pOut,char* pParaName,
                               UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(pParaName);
    if(pParameter == NULL) return FALSE;

    if((pParameter->GetDataType() != PT_DIGIT)
        && (pParameter->GetDataType() != PT_COMPLEX))
        return  FALSE;

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return FALSE;

    if(pParaValue->GetValueFormat() != VF_SINGLE)
        return  FALSE;

    *pOut = atoi(pParaValue->GetMinValue());
    return TRUE;
}

/*
    函数功能:根据参数码得到某个参数的区间取值,输出字符串值.

    参数:
    uParaNo,参数码.
    pMinVal,输出区间的下限.
    pMaxVal,输出区间的上限.
    puStep,输出区间的步长.
    uValSeq,取值序号.

    返回:
    成功返回TRUE,失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CParseResult::GetRegionStringVal(UINT2 uParaNo,
                                      char** pMinVal, 
                                      char** pMaxVal,
                                      UINT2* puStep,
                                      UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(uParaNo);
    if(pParameter == NULL)
        return  FALSE;

    if((pParameter->GetDataType() != PT_BCD )
        && (pParameter->GetDataType() != PT_COMPLEX))
        return FALSE;

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return FALSE;

    if(pParaValue->GetValueFormat() == VF_REGION)
    {
        *pMinVal = (char*)pParaValue->GetMinValue();
        *pMaxVal = (char*)pParaValue->GetMaxValue();
        *puStep = pParaValue->GetStep(); 
    }
    else if(pParaValue->GetValueFormat() == VF_SINGLE)
    {
        *pMinVal = (char*)pParaValue->GetMinValue();
        *pMaxVal = (char*)pParaValue->GetMinValue();
        *puStep = 0;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*
    函数功能:根据参数名得到某个参数的区间取值,输出字符串值.

    参数:
    pParaName,参数名.
    pMinVal,输出区间的下限.
    pMaxVal,输出区间的上限.
    puStep,输出区间的步长.
    uValSeq,取值序号.

    返回:
    成功返回TRUE,失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CParseResult::GetRegionStringVal(char* pParaName,
                                      char** pMinVal, 
                                      char** pMaxVal,
                                      UINT2* puStep,
                                      UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(pParaName);
    if(pParameter == NULL)
        return  FALSE;

    if((pParameter->GetDataType() != PT_BCD)
        && (pParameter->GetDataType() != PT_COMPLEX))
        return  FALSE;

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return FALSE;

    if(pParaValue->GetValueFormat() == VF_REGION)
    {
        *pMinVal = (char*)pParaValue->GetMinValue();
        *pMaxVal = (char*)pParaValue->GetMaxValue();
        *puStep = pParaValue->GetStep(); 
    }
    else if(pParaValue->GetValueFormat() == VF_SINGLE)
    {
        *pMinVal = (char*)pParaValue->GetMinValue();
        *pMaxVal = (char*)pParaValue->GetMinValue();
        *puStep = 0;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*
    函数功能:根据参数码得到某个参数的区间取值,输出数值型参数值.

    参数:
    uParaNo,参数码.
    pMinVal,输出区间的下限.
    pMaxVal,输出区间的上限.
    puStep,输出区间的步长.
    uValSeq,取值序号.

    返回:
    成功返回TRUE,失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CParseResult::GetRegionDigitVal(UINT2 uParaNo,
                                     INT* pMinVal,
                                     INT* pMaxVal,
                                     UINT2* puStep,
                                     UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(uParaNo);
    if(pParameter == NULL)
        return  FALSE;

    if((pParameter->GetDataType() != PT_DIGIT)
        && (pParameter->GetDataType() != PT_COMPLEX))
        return  FALSE;

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return FALSE;

    if(pParaValue->GetValueFormat() == VF_REGION)
    {
        *pMinVal = atoi(pParaValue->GetMinValue());
        *pMaxVal = atoi(pParaValue->GetMaxValue());
        *puStep = pParaValue->GetStep();
    }
    else if(pParaValue->GetValueFormat() == VF_SINGLE)
    {
        *pMinVal = atoi(pParaValue->GetMinValue());
        *pMaxVal = atoi(pParaValue->GetMinValue());
        *puStep = 0;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*
    函数功能:根据参数名得到某个参数的区间取值,输出字符串值.

    参数:
    pParaName,参数名.
    pMinVal,输出区间的下限.
    pMaxVal,输出区间的上限.
    puStep,输出区间的步长.
    uValSeq,取值序号.

    返回:
    成功返回TRUE,失败返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CParseResult::GetRegionDigitVal(char* pParaName,
                                     INT* pMinVal,
                                     INT* pMaxVal,
                                     UINT2* puStep,
                                     UINT2 uValSeq)
{
    CParameter* pParameter = NULL;
    CParaValue* pParaValue = NULL;

    pParameter = GetPara(pParaName);
    if(pParameter == NULL)
        return  FALSE;

    if((pParameter->GetDataType() != PT_DIGIT)
        && (pParameter->GetDataType() != PT_COMPLEX))
        return  FALSE;

    pParaValue = pParameter->GetValue(uValSeq);
    if(pParaValue == NULL)
        return FALSE;

    if(pParaValue->GetValueFormat() == VF_REGION)
    {
        *pMinVal = atoi(pParaValue->GetMinValue());
        *pMaxVal = atoi(pParaValue->GetMaxValue());
        *puStep = pParaValue->GetStep();
    }
    else if(pParaValue->GetValueFormat() == VF_SINGLE)
    {
        *pMinVal = atoi(pParaValue->GetMinValue());
        *pMaxVal = atoi(pParaValue->GetMinValue());
        *puStep = 0;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

const char* CParseResult::GetCmdDspName()
{
    return m_szCmdDspName;
}

void CParseResult::SetCmdDspName(const char *p)
{
    strncpy(m_szCmdDspName,p,MAX_CMD_DSP_LEN);
    m_szCmdDspName[MAX_CMD_DSP_LEN - 1] = 0;
}


const char* CParseResult::GetCmdName()
{
    return m_szCmdName;
}

