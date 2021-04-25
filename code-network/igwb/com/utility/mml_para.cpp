/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:mml_para.cpp
    ������Ϣ:
    
    ����CParameter��CParaValue��CParseResult���ʵ��.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/
#include "mml_para.h"

/*
    ������CParaValue���ʵ��.
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
    ������CParameter���ʵ��.
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
    ��������:��������,�ͷ������Ӳ���ֵ.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ������CParseResult���ʵ��.
*/

CParseResult::CParseResult(MSG_INTRA* pMsg)
{
    m_uWsNo     = pMsg->ClientNo;   //����̨��
    m_uWsType   = pMsg->AppType;    //����̨����
    m_uIP       = pMsg->IPAddr;     //����̨IP��ַ

    /* ԭʼ��MML���� */
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
    ��������:��������,�ͷ����в���ֵ.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ������ȡ��������ָ��.

    ����:
    uParaNo,������.
    һ�������������мȿ��Բ���������,�ֿ��Բ��������.

    ����:
    �ڲ������в��Ҳ�����ƥ��Ĳ���,������ƥ��Ĳ�������,
    ���ز��������ָ��,���򷵻�NULL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ�������ȡ��������ָ��.

    ����:
    pParaName,������.
    һ�������������мȿ��Բ���������,�ֿ��Բ��������.
    �����������ִ�Сд.

    ����:
    �ڲ������в��Ҳ�����ƥ��Ĳ���,������ƥ��Ĳ�������,
    ���ز��������ָ��,���򷵻�NULL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���л�һ��CParseResult����,�Ա����ڽ��̼䴫��.
    �ڵ��øú���֮ǰӦ��ʹ��GetParseLength()�����жϻ�����
    �ĳ���,�Ա�֤�������.

    ����:
    pData,������л�����Ļ�����.
    ���л��Ľ��Ϊ���½ṹ:

    {
        _ParseResult ParseResult
        {   //��һ������
            _Parameter Parameter
            _ParaValue  paravalue[Parameter.uValueNum]
        }
        ...
        {   //��n������
            _Parameter Parameter
            _ParaValue  paravalue[Parameter.uValueNum]
        }
    }

    uLength,�������ĳ���.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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

    /* ���л��������� */
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

        /* ���л���������ֵ(�Ӳ���),�Ӳ�������Ŵ�1��ʼ. */
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
    ��������:����һ��MSG_INTRAЯ�������ݴ���CParseResult����.
    ����pMsg->Data�����ݺ����л��Ľṹ,������Ա������ֵ.

    ����:pMsg,��Ϣָ��.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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

    /* ��������������� */
    for(int i = 0,j = 0;i < m_uParaNum;i++)
    {
        pPara = (_Parameter*)p;
        CParameter* q = new CParameter;

        q->SetParaName(pPara->szParaName);
        q->SetParaCode(pPara->uParaCode);
        q->SetDataType(pPara->yDataType);
        q->SetOperType(pPara->yOperType);

        p += sizeof(_Parameter);

        /* ���������������ֵ */
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
    ��������:���ݲ�������ȡֵ���,�õ�����ֵ������.

    ����:
    pParaName,������.
    uValSeq,ȡֵ���.

    ����:
    �ɹ����ز���ֵ������.
    ʧ�ܷ���VF_ERROR.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ������ȡֵ���,�õ�����ֵ������.

    ����:
    uParaNo,������.
    uValSeq,ȡֵ���.

    ����:
    �ɹ����ز���ֵ������.
    ʧ�ܷ���VF_ERROR.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ������ȡֵ���,�õ��ַ�������ֵ.

    1.ֻ��ȡ��ֵ������ֵ,�����������͵Ĳ���,����NULL.
    2.��������������ֵ��,����NULL.

    ����:
    uParaNo,������.
    uValSeq,ȡֵ���.

    ����:
    �ɹ����ز���ֵ��ָ��(�ַ�������).
    ʧ�ܷ���NULL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ�������ȡֵ���,�õ��ַ�������ֵ.

    1.ֻ��ȡ��ֵ������ֵ,�����������͵Ĳ���,����NULL.
    2.��������������ֵ��,����NULL.

    ����:
    pParaName,������.
    uValSeq,ȡֵ���.

    ����:
    �ɹ����ز���ֵ��ָ��(�ַ�������).
    ʧ�ܷ���NULL.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ������ȡֵ���,�õ���ֵ����ֵ.

    1.ֻ��ȡ��ֵ������ֵ,�����������͵Ĳ���,����FALSE.
    2.���������Ͳ�����ֵ��,����FALSE.

    ����:
    pOut,������ֵ�Ͳ���.
    uParaNo,������.
    uValSeq,ȡֵ���.

    ����:
    �ɹ�����TRUE,ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ�������ȡֵ���,�õ���ֵ����ֵ.

    1.ֻ��ȡ��ֵ������ֵ,�����������͵Ĳ���,����FALSE.
    2.���������Ͳ�����ֵ��,����FALSE.

    ����:
    pOut,������ֵ�Ͳ���.
    pParaName,������.
    uValSeq,ȡֵ���.

    ����:
    �ɹ�����TRUE,ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ�����õ�ĳ������������ȡֵ,����ַ���ֵ.

    ����:
    uParaNo,������.
    pMinVal,������������.
    pMaxVal,������������.
    puStep,�������Ĳ���.
    uValSeq,ȡֵ���.

    ����:
    �ɹ�����TRUE,ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ������õ�ĳ������������ȡֵ,����ַ���ֵ.

    ����:
    pParaName,������.
    pMinVal,������������.
    pMaxVal,������������.
    puStep,�������Ĳ���.
    uValSeq,ȡֵ���.

    ����:
    �ɹ�����TRUE,ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ�����õ�ĳ������������ȡֵ,�����ֵ�Ͳ���ֵ.

    ����:
    uParaNo,������.
    pMinVal,������������.
    pMaxVal,������������.
    puStep,�������Ĳ���.
    uValSeq,ȡֵ���.

    ����:
    �ɹ�����TRUE,ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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
    ��������:���ݲ������õ�ĳ������������ȡֵ,����ַ���ֵ.

    ����:
    pParaName,������.
    pMinVal,������������.
    pMaxVal,������������.
    puStep,�������Ĳ���.
    uValSeq,ȡֵ���.

    ����:
    �ɹ�����TRUE,ʧ�ܷ���FALSE.

    �޸ļ�¼:
    zhouyonghui,2001-10-24,����.
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

