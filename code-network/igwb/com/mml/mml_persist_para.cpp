/*
    ��Ʒ��:iGateway Bill V200
    ģ����:MML Server
    �ļ���:mml_persist_para.cpp
    ������Ϣ:
    
    ����CPersistPara���ʵ��,����ʵ����������Ĺ���,
    �����Ӵ����ļ��ж�ȡ��������,��֤�����в����ĺϷ��Ե�.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
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
    ��������:��������,�����Ӳ�����λ��ö��ֵ���ڴ�ռ�.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:��ʼ������/�Ӳ���.

    ����:
    1.��ʼ���ɹ�,����ERR_SUCCESS.
    2.ʧ�ܷ��ؾ��������.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistPara::Init()
{
    int nResult = 0;
    if(m_uChildParaCode == MAX_UINT2)
    {
        /* ʼ���������� */
        if((nResult = InitPara()) != ERR_SUCCESS) 
            return nResult;
    }
    else
    {
        /*
            ��ʼ���Ӳ�������,��ʱCPersistPara������һ���Ӳ�������.
        */
        if((nResult = InitChildPara()) != ERR_SUCCESS)
            return nResult;
    }

    CPersistCmdMan* pCmdMan = CPersistCmdMan::Instance();

    if(m_yRangeType == ENUM_ZONE)
    {
        /*
            ������ֵ������������,���²���������������.
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
                /* �Ӳ��� */
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
            ������ֵ����ö��/λ������,���²�����ö��ֵ.
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
                /* �Ӳ��� */
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
            �����Ӳ���,��ʼ���Ӳ���,�Ӳ������1��ʼ.
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
    ��������:��ʼ����������.

    ����:
    1.���ļ�paradef.dat�в������ڶ��������ƥ���������Ϣ,
    ����ERR_PARA_NOT_EXIST(����������).
    2.������������Ϣ����,����ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistPara::InitPara()
{
    UINT4 uParaNum = 0;
    _ParaDef* pParaDef = NULL;

    CPersistCmdMan* pCmdMan = CPersistCmdMan::Instance();

    /* ��ò���ָ��ͼ�¼�� */
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
    ��������:��ʼ���Ӳ�������.

    ����:
    1.childparadef.dat�в������ڶ��������ƥ���������Ϣ,
    ����ERR_CHILD_PARA_NOT_EXIST(�Ӳ���������).
    2.������������Ϣ����,����ERR_SUCCESS.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
int CPersistPara::InitChildPara()
{
    UINT4  uChildParaNum = 0;
    _ChildParaDef* pChildParaDef = NULL;

    CPersistCmdMan* pCmdMan = CPersistCmdMan::Instance();

    /* ��ò���ָ��ͼ�¼�� */
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
    ��������:�Բ���ֵ���Ϸ��Լ��.

    1.��m_uChildParaCode != MAX_UINT2,��ʾ�ò����������Ӳ���,
    ������Ӳ�����ֵ��������ĺϷ���.

    2.��m_uChildParaCode == MAX_UINT2,���ݲ��������ͺ�ֵ����
    ����ĺϷ���.

    3.��������ֵ��������ö����,�����ö��ֵ,��ֵ������������
    ���������Ƿ������䷶Χ��.������û��ֵ��������ֱ���������ֵ.

    ����:
    1.pInputValue,�����Ĳ���.
    2.pOutputValue,�������,��pInputValue��������ǺϷ���,
    3.��pOutputValue���������.
    4.nValType,�����Ĳ���ֵ������.
    0 -> ����,1 -> �ַ���,2 -> ��ʶ��,3 -> ������,4 -> λ������.

    ����:
    1.������ֵ�Ϸ�,����TRUE,���򷵻�FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
BOOL CPersistPara::CheckParaValue(const char* pInputValue,
                                  char* pOutputValue,int nValType)
{
    /* ��������Ϊλ������,��ʱֵ��Ϊö������. */
    if(m_yDataType == PT_BITFIELD)
    {
        if(nValType != 4) return FALSE;

        return CheckBitFieldValue(pInputValue,pOutputValue);
    }

    /* ֵ��Ϊ��������,��������һ����ֵ. */
    if(m_yRangeType == ENUM_ZONE)
    {
        if(nValType != 0) return FALSE;

        if(!CheckRangeValue(atoi(pInputValue),atoi(pInputValue),0))
            return FALSE;

        strcpy(pOutputValue,pInputValue);
        return TRUE;
    }

    /*
        ֵ��Ϊö������(������������ֵ��),ö�����������������ֺ��ַ���:
        1.����������ֵ,����m_EnumValueMap�в���,�������򷵻�.
        2.���������ַ�,�򷵻ض�Ӧö��ֵ,ö���������ִ�Сд.
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
                ʹ�ô�д��ĸ��Ϊ����,��Ϊmacro.ini�ļ�ת���Ľ��
                ֻ������д��ĸ.
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
        ��ֵ����Ҫ��,�ж����������Ƿ�һ��.
    */
    if(m_yRangeType == ENUM_NONE)
    {
        if((nValType != 0) && (nValType != 3)
            && (m_yDataType == PT_DIGIT))
        {
            /* ��������ֵ��,��������ֵ������ֵ. */
            return FALSE;
        }

        if(((m_yDataType == PT_STRING) || (m_yDataType == PT_BCD)
            || (m_yDataType == PT_ANYTYPE)) && (nValType != 1))
        {
            /*
                �����������ַ�����BCD�����������������,
                ��������ֵ�����ַ���.
            */
            return FALSE;
        }

        strcpy(pOutputValue,pInputValue);
        return TRUE;
    }

    return FALSE;
}


/*
    ��������:����������Ͳ���ֵ�ĺϷ���.

    ����:
    nMin,���������.
    nMax,���������.
    uStep,����Ĳ���.

    ����:
    1.��������������������,����TRUE.
    2.��ֵ��������ENUM_NONE,����TRUE.
    3.����������������Χ��,����TRUE.
    4.�����������FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���λ���������ݵĺϷ���.

    ����:
    pInputValue,����Ĳ���ֵ,��ʽ:λ����-1/0.

    pOutputValue,����Ĳ���ֵ,��ʽ:λ���-1/0.

    ����:
    1.��m_EnumValueMap�д���pInputValue��Ӧ��λ���¼,����TRUE.
    2.���򷵻�FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
        ʹ�ô�д��ĸ��Ϊ����,��Ϊmacro.ini�ļ�ת���Ľ��
        ֻ������д��ĸ.
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
    ��������:�����Ӳ��������Ӳ���ָ��.

    ����:
    uChildParaNo,�Ӳ�����.
    �Ӳ���û������,ֻ�б�ʾλ��˳��Ĵ���.

    ����:
    1.��uChildParaNo==MAX_UINT2,���ǺϷ����Ӳ�����,����NULL.
    2.���Ӳ������в����Ӳ�������uChildParaNo��ͬ���Ӳ���,
      ���ҵ��򷵻��Ӳ�����ָ��,���򷵻�NULL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���Ӳ���ֵ���Ϸ��Լ��.

    ����:
    uChildParaCode,�Ӳ�����.
    pIn,�����Ĳ���.
    pOut,�������,��pIn��������ǺϷ���,
    ��pOut���������.
    nValType,�����Ĳ���ֵ������.0 -> ����,1 -> �ַ���,2 -> ������.

    ����:
    1.������ֵ�Ϸ�,����TRUE,���򷵻�FALSE.
    2.���Ӳ���������,����FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
*/
BOOL CPersistPara::CheckChildParaValue(UINT2 uChildParaCode,char *pIn,
                                       char *pOut,int nValType)
{
    CPersistPara* p = GetChildPara(uChildParaCode);
    if(p == NULL) return FALSE;

    return p->CheckParaValue(pIn,pOut,nValType);
}

/*
    ��������:����������Ͳ���ֵ�ĺϷ���.

    ����:
    uChildParaCode,�Ӳ�����.
    nMin,���������.
    nMax,���������.
    uStep,����Ĳ���.

    ����:
    1.��uChildParaCode��Ӧ���Ӳ���������,����FALSE.
    2.�����Ӳ����ļ����.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
