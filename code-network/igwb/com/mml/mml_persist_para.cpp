/*
    产品名:iGateway Bill V200
    模块名:MML Server
    文件名:mml_persist_para.cpp
    描述信息:
    
    包含CPersistPara类的实现,该类实现命令参数的管理,
    包括从磁盘文件中读取参数配置,验证命令行参数的合法性等.

    版本说明:V200R001i00M00B00D1130A
    版权信息:版权所有(C)2001-2002 华为技术有限公司

    修改记录:
    周拥辉,2001-10-24,创建.
*/

#include "mml_persist_para.h"
#include "mml_persist_cmd.h"
#include "../include/toolbox.h"

CPersistPara::CPersistPara(UINT2 uCmdCode,UINT2 uParaCode,
                           UINT2 uChildParaCode)
{
    m_uCmdCode = uCmdCode;
    m_uParaCode = uParaCode;
    m_uChildParaCode = uChildParaCode;
    m_uChildParaNum = 0;
    m_nMin = 0;
    m_nMax = 0;

    m_yDataType = PT_DIGIT;
    m_yRangeType = ENUM_NONE;
    m_yMustGive = 0;
}

/*
    函数功能:析构函数,回收子参数和位域、枚举值的内存空间.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
CPersistPara::~CPersistPara()
{
    VECTOR<CPersistPara*>::iterator i = m_ChildPara.begin(),
                                    e = m_ChildPara.end();
    while(i != e)
    {
        delete *i;
        i++;
    }
}

/*
    函数功能:初始化参数/子参数.

    返回:
    1.初始化成功,返回ERR_SUCCESS.
    2.失败返回具体错误码.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
int CPersistPara::Init()
{
    int nResult = 0;
    if(m_uChildParaCode == MAX_UINT2)
    {
        /* 始化参数对象 */
        if((nResult = InitPara()) != ERR_SUCCESS) 
            return nResult;
    }
    else
    {
        /*
            初始化子参数对象,此时CPersistPara对象是一个子参数对象.
        */
        if((nResult = InitChildPara()) != ERR_SUCCESS)
            return nResult;
    }

    CPersistCmdMan* pCmdMan = CPersistCmdMan::Instance();

    if(m_yRangeType == ENUM_ZONE)
    {
        /*
            参数的值域是区间类型,更新参数的区间上下限.
        */
        UINT4 uParaNum = 0;
        _ParaRangeDef* pPara = NULL;

        pCmdMan->GetParaRangeDef(pPara,uParaNum);

        for(int i = 0;i < uParaNum;i++)
        {
            if(m_uChildParaCode == MAX_UINT2)
            {
                if((pPara[i].uCmdCode == m_uCmdCode)
                    && (pPara[i].uParaCode == m_uParaCode))
                {
                    m_nMin = pPara[i].nMinValue;
                    m_nMax = pPara[i].nMaxValue;
                    break;
                }
            }
            else
            {
                /* 子参数 */
                if((pPara[i].uCmdCode == m_uCmdCode)
                    && (pPara[i].uParaCode == m_uParaCode)
                    && (pPara[i].uChildParaCode == m_uChildParaCode))
                {
                    m_nMin = pPara[i].nMinValue;
                    m_nMax = pPara[i].nMaxValue;
                    break;
                }
            }
        }
    }
    else if(m_yRangeType == ENUM_ARATE || m_yDataType == PT_BITFIELD)
    {
        /*
            参数的值域是枚举/位域类型,更新参数的枚举值.
        */
        UINT4 uParaNum = 0;
        _ParaEnumDef* pPara = NULL;

        pCmdMan->GetParaEnumDef(pPara,uParaNum);

        STRING strEnumName;
        for(int i = 0;i < uParaNum;i++)
        {
            if(m_uChildParaCode == MAX_UINT2)
            {
                if((pPara[i].uCmdCode == m_uCmdCode)
                    && (pPara[i].uParaCode == m_uParaCode))
                {
                    strEnumName = pPara[i].szInput;
                    m_EnumValueMap[strEnumName] = pPara[i].uValue;
                }
            }
            else
            {
                /* 子参数 */
                if((pPara[i].uCmdCode == m_uCmdCode)
                    && (pPara[i].uParaCode == m_uParaCode)
                    && (pPara[i].uChildParaCode == m_uChildParaCode))
                {
                    strEnumName = pPara[i].szInput;
                    m_EnumValueMap[strEnumName] = pPara[i].uValue;
                }
            }
        }
    }
    else if(m_yDataType == PT_COMPLEX)
    {
        /*
            存在子参数,初始化子参数,子参数码从1开始.
        */
        CPersistPara* pPara = NULL;
        for(int i = 1;i <= m_uChildParaNum;i++)
        {
            pPara = new CPersistPara(m_uCmdCode,m_uParaCode,i);
            if((nResult = pPara->Init()) != ERR_SUCCESS)
            {
                return nResult;
            }

            m_ChildPara.push_back(pPara);
        }
    }

    return ERR_SUCCESS;
}

/*
    函数功能:初始化参数对象.

    返回:
    1.若文件paradef.dat中不存在于对象参数码匹配的配置信息,
    返回ERR_PARA_NOT_EXIST(参数不存在).
    2.若参数配置信息存在,返回ERR_SUCCESS.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
int CPersistPara::InitPara()
{
    UINT4 uParaNum = 0;
    _ParaDef* pParaDef = NULL;

    CPersistCmdMan* pCmdMan = CPersistCmdMan::Instance();

    /* 获得参数指针和记录数 */
    pCmdMan->GetParaDef(pParaDef,uParaNum);

    for(UINT4 i = 0;i < uParaNum;i++)
    {
        if((pParaDef[i].uCmdCode == m_uCmdCode)
            && (pParaDef[i].uParaCode == m_uParaCode))
        {
            strcpy(m_szParaName,pParaDef[i].szParaName);
            strcpy(m_szDefValue,pParaDef[i].szDefault);

            m_yRangeType = pParaDef[i].yRangeType;
            m_yDataType = pParaDef[i].yDataType;
            m_yMustGive = pParaDef[i].yMustGive;
            m_uChildParaNum = pParaDef[i].yByteNum;

            return ERR_SUCCESS;
        }
    }

    return ERR_PARA_NOT_EXIST;
}

/*
    函数功能:初始化子参数对象.

    返回:
    1.childparadef.dat中不存在于对象参数码匹配的配置信息,
    返回ERR_CHILD_PARA_NOT_EXIST(子参数不存在).
    2.若参数配置信息存在,返回ERR_SUCCESS.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
int CPersistPara::InitChildPara()
{
    UINT4  uChildParaNum = 0;
    _ChildParaDef* pChildParaDef = NULL;

    CPersistCmdMan* pCmdMan = CPersistCmdMan::Instance();

    /* 获得参数指针和记录数 */
    pCmdMan->GetChildParaDef(pChildParaDef,uChildParaNum);

    for(UINT4 i = 0;i < uChildParaNum;i++)
    {
        if((pChildParaDef[i].uCmdCode == m_uCmdCode)
            && (pChildParaDef[i].uParaCode == m_uParaCode)
            && (pChildParaDef[i].uChildParaCode == m_uChildParaCode))
        {
            strcpy(m_szParaName,pChildParaDef[i].szParaName);
            strcpy(m_szDefValue,pChildParaDef[i].szDefValue);
            m_yRangeType = pChildParaDef[i].yRangeType;

            return ERR_SUCCESS;
        }
    }

    return ERR_CHILD_PARA_NOT_EXIST;
}

/*
    函数功能:对参数值作合法性检查.

    1.若m_uChildParaCode != MAX_UINT2,表示该参数对象是子参数,
    则根据子参数的值域检查输入的合法性.

    2.若m_uChildParaCode == MAX_UINT2,根据参数的类型和值域检查
    输入的合法性.

    3.若参数的值域类型是枚举型,则输出枚举值,若值域类型是区间
    则检查输入是否在区间范围内.若参数没有值域限制则直接输出输入值.

    参数:
    1.pInputValue,待检查的参数.
    2.pOutputValue,输出参数,若pInputValue经过检查是合法的,
    3.则pOutputValue存放输出结果.
    4.nValType,待检查的参数值得类型.
    0 -> 整数,1 -> 字符串,2 -> 标识符,3 -> 浮点数,4 -> 位域类型.

    返回:
    1.若输入值合法,返回TRUE,否则返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CPersistPara::CheckParaValue(const char* pInputValue,
                                  char* pOutputValue,int nValType)
{
    /* 参数类型为位域类型,此时值域为枚举类型. */
    if(m_yDataType == PT_BITFIELD)
    {
        if(nValType != 4) return FALSE;

        return CheckBitFieldValue(pInputValue,pOutputValue);
    }

    /* 值域为区间类型,且输入是一个单值. */
    if(m_yRangeType == ENUM_ZONE)
    {
        if(nValType != 0) return FALSE;

        if(!CheckRangeValue(atoi(pInputValue),atoi(pInputValue),0))
            return FALSE;

        strcpy(pOutputValue,pInputValue);
        return TRUE;
    }

    /*
        值域为枚举类型(参数类型是数值型),枚举类型允许输入数字和字符串:
        1.若输入是数值,则在m_EnumValueMap中查找,若存在则返回.
        2.在输入是字符,则返回对应枚举值,枚举名不区分大小写.
    */
    if(m_yRangeType == ENUM_ARATE)
    {
        if(nValType == 0)
        {
            UINT4 u4Val = (UINT4)atoi(pInputValue);

            MAP<STRING,UINT4>::iterator i = m_EnumValueMap.begin(),
                                        e = m_EnumValueMap.end();
            while(i != e)
            {
                if(u4Val == (*i).second)
                {
                    sprintf(pOutputValue,"%d",u4Val);
                    return TRUE;
                }
                i++;
            }
            return FALSE;
        }
        else if(nValType == 2)
        {
            /*
                使用大写字母作为索引,因为macro.ini文件转换的结果
                只包含大写字母.
            */
            char buf[1024];
            strncpy(buf,pInputValue,1024);
			buf[1023] = 0;

            StringToUpper(buf);
            STRING strName = buf;

            if(m_EnumValueMap.find(strName) == m_EnumValueMap.end())
                return FALSE;

            sprintf(pOutputValue,"%d",m_EnumValueMap[strName]);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    /*
        无值类型要求,判断数据类型是否一致.
    */
    if(m_yRangeType == ENUM_NONE)
    {
        if((nValType != 0) && (nValType != 3)
            && (m_yDataType == PT_DIGIT))
        {
            /* 参数是数值型,但是输入值不是数值. */
            return FALSE;
        }

        if(((m_yDataType == PT_STRING) || (m_yDataType == PT_BCD)
            || (m_yDataType == PT_ANYTYPE)) && (nValType != 1))
        {
            /*
                参数类型是字符串、BCD码或者其他任意类型,
                但是输入值不是字符串.
            */
            return FALSE;
        }

        strcpy(pOutputValue,pInputValue);
        return TRUE;
    }

    return FALSE;
}


/*
    函数功能:检查区间类型参数值的合法性.

    参数:
    nMin,区间的上限.
    nMax,区间的下限.
    uStep,区间的步长.

    返回:
    1.若参数类型是任意类型,返回TRUE.
    2.若值域类型是ENUM_NONE,返回TRUE.
    3.若输入区间在允许范围内,返回TRUE.
    4.其他情况返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CPersistPara::CheckRangeValue(INT4 nMin,INT4 nMax,UINT4 uStep)
{
    if(m_yDataType == PT_ANYTYPE)
        return TRUE;

    if(m_yRangeType == ENUM_NONE)
        return TRUE;

    if(nMin >= m_nMin && nMax <= m_nMax)
        return TRUE;

    return FALSE;
}

/*
    函数功能:检查位域类型数据的合法性.

    参数:
    pInputValue,输入的参数值,格式:位域名-1/0.

    pOutputValue,输出的参数值,格式:位域号-1/0.

    返回:
    1.若m_EnumValueMap中存在pInputValue相应的位域记录,返回TRUE.
    2.否则返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CPersistPara::CheckBitFieldValue(const char* pInputValue,
                                      char* pOutputValue)
{
    const char* p = strstr(pInputValue,"-");
    if(p == NULL) return FALSE;

    char buf[1024];
    memcpy(buf,pInputValue,p - pInputValue);
    buf[p - pInputValue] = 0;

    /*
        使用大写字母作为索引,因为macro.ini文件转换的结果
        只包含大写字母.
    */
    StringToUpper(buf);
    STRING strName = buf;

    if(m_EnumValueMap.find(strName) == m_EnumValueMap.end())
        return FALSE;

    sprintf(pOutputValue,"%d-%s",m_EnumValueMap[strName],(p + 1));

    return TRUE;

}


UINT2 CPersistPara::GetParaCode()
{
    return m_uParaCode;
}

UINT2 CPersistPara::GetChildParaCode()
{
    return m_uChildParaCode;
}

char* CPersistPara::GetParaName()
{
    return m_szParaName;
}

/*
    函数功能:根据子参数码获得子参数指针.

    参数:
    uChildParaNo,子参数码.
    子参数没有名字,只有表示位置顺序的代码.

    返回:
    1.若uChildParaNo==MAX_UINT2,不是合法的子参数码,返回NULL.
    2.在子参数表中查找子参数码与uChildParaNo相同的子参数,
      若找到则返回子参数的指针,否则返回NULL.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
CPersistPara* CPersistPara::GetChildPara(UINT2 uChildParaNo)
{
    if(uChildParaNo == MAX_UINT2) return NULL;

    VECTOR<CPersistPara*>::iterator i = m_ChildPara.begin(),
                                    e = m_ChildPara.end();
    while(i != e)
    {
        if((*i)->GetChildParaCode() == uChildParaNo)
            return (*i);
        i++;
    }
    return NULL;
}

char* CPersistPara::GetDefaultValue()
{
    return m_szDefValue;
}

BYTE CPersistPara::GetDataType()
{
    return m_yDataType;
}

BOOL CPersistPara::IsMustGive()
{
    return (m_yMustGive == 1);
}

/*
    函数功能:对子参数值作合法性检查.

    参数:
    uChildParaCode,子参数码.
    pIn,待检查的参数.
    pOut,输出参数,若pIn经过检查是合法的,
    则pOut存放输出结果.
    nValType,待检查的参数值得类型.0 -> 整数,1 -> 字符串,2 -> 浮点数.

    返回:
    1.若输入值合法,返回TRUE,否则返回FALSE.
    2.若子参数不存在,返回FALSE.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CPersistPara::CheckChildParaValue(UINT2 uChildParaCode,char *pIn,
                                       char *pOut,int nValType)
{
    CPersistPara* p = GetChildPara(uChildParaCode);
    if(p == NULL) return FALSE;

    return p->CheckParaValue(pIn,pOut,nValType);
}

/*
    函数功能:检查区间类型参数值的合法性.

    参数:
    uChildParaCode,子参数码.
    nMin,区间的上限.
    nMax,区间的下限.
    uStep,区间的步长.

    返回:
    1.若uChildParaCode对应的子参数不存在,返回FALSE.
    2.返回子参数的检查结果.

    修改记录:
    zhouyonghui,2001-10-24,创建.
*/
BOOL CPersistPara::CheckChildRangeValue(UINT2 uChildParaCode,INT4 nMin,
                                        INT4 nMax,UINT4 uStep)
{
    CPersistPara* p = GetChildPara(uChildParaCode);
    if(p == NULL) return FALSE;

    return p->CheckRangeValue(nMin,nMax,uStep);
}

UINT2 CPersistPara::GetChildParaNum()
{
    return m_uChildParaNum;
}
