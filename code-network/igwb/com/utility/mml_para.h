/*
    ��Ʒ��:iGateway Bill V200
    ģ����:���
    �ļ���:mml_para.h
    ������Ϣ:
    
    ����CParameter��CParaValue��CParseResult��Ķ���.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/

#ifndef _MML_PARAMETER_H
#define _MML_PARAMETER_H

#include "../include/precom.h"
#include "../include/mmlcmd_def.h"

class CParaValue
{
protected:
    BYTE m_yValueFormat;        //����ֵ������
    STRING m_strMinValue;       //���������
    STRING m_strMaxValue;       //���������
    UINT4 m_uStep;             //����ֵ�Ĳ���

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

    char        m_szParaName[PARA_NAME_LEN];    //������
    UINT2       m_uParaCode;                    //������

    UINT2       m_uValueNum;                    //����ֵ����
    PARA_TYPE   m_yDataType;                    //������������
    OPERATOR_TYPE   m_yOperType;                //��������

    VECTOR<CParaValue*> m_ValueVector;          //����ֵ�б�
};

/*
    _ParaValue��_Parameter��_ParseResult�����ṹ�������л�
    CParseResult����.
*/
struct _ParaValue
{
    BYTE    yValueFormat;				//ֵ�ĸ�ʽ,ȡֵΪVALUE_FORMAT
    char    szMinValue[256];			//����ֵ����Сֵ��ֵ
    char    szMaxValue[256];			//����ֵ�����ֵ,��ֵʱ��ʹ��
    UINT4   uStep;						//����ֵ�Ĳ���,��ֵʱ����
};

struct _Parameter
{
    char    szParaName[PARA_NAME_LEN];  //������
    UINT2   uParaCode;					//������
    BYTE    yOperType;					//��������,ȡֵOPERATOR_TYPE
    UINT2   uValueNum;					//����ֵ����
    BYTE    yDataType;					//������������,ȡֵPARA_TYPE
};

struct _ParseResult
{
    UINT2   uCmdCode;					//������
    char    szCmdName[CMD_NAME_LEN];	//��������
	char    szCmdDspName[MAX_CMD_DSP_LEN];

    BYTE    yCmdLevel;					//�����,����Ȩ����֤

    UINT2   uParaNumber;				//��������

    UINT2   uWsNo;						//��������Ĺ���̨��
    UINT2   uWsType;					//��������Ĺ���̨����

    /* ��������ĵ�¼�û��� */
    char    szUserName[USER_NAME_LEN];

    BYTE    yUserLevel;         //�û�����,ȡֵȡֵUSER_LEVEL
    UINT4   uIP;                //����������ն˵�IP��ַ
    char    szErrorMsg[256];    //����������Ϣ
    char    szCmdLine[512];     //��Ӧ��ԭʼMML����
};

class CParseResult
{
protected:
    UINT2 m_uCmdCode;                   //������
    char m_szCmdName[CMD_NAME_LEN];     //��������
    char m_szCmdDspName[MAX_CMD_DSP_LEN];

    BYTE m_yCmdLevel;                   //�����

    UINT2 m_uParaNum;                   //��������
    VECTOR<CParameter*> m_ParaVector;   //�����б�

    UINT2 m_uWsNo;                      //��������Ĺ���̨��
    UINT2 m_uWsType;                    //��������Ĺ���̨����

    char m_szUserName[USER_NAME_LEN];   //��������ĵ�¼�û���
    BYTE m_yUserLevel;                  //�û�����,ȡֵȡֵUSER_LEVEL
    UINT4 m_uIP;                        //����������ն˵�IP��ַ

    STRING m_strErrorMsg;               //����������Ϣ
    STRING m_strCmdLine;                //��Ӧ��ԭʼMML����

public:
    CParseResult(MSG_INTRA* pMsg);
    ~CParseResult();

    /* ���û�ȡm_uCmdCode */
    UINT2 GetCmdCode(char* pCmdName);
    UINT2 GetCmdCode();
    void  SetCmdCode(UINT2 uCmdCode,char* pCmdName);

    /* ���û�ȡm_yCmdLevel */
    BYTE  GetCmdLevel();
    void  SetCmdLevel(BYTE yCmdLevel);

    /* ���û�ȡm_uWsNo */
    void SetWorkStationNo(UINT2 n2WsNo);
    UINT2 GetWorkStationNo();

    /* ���û�ȡm_uWsType */
    void SetWorkStationType(UINT2 uWsType);
    UINT2 GetWorkStationType();

    /* ���û�ȡm_szUserName */
    const char* GetUserName();
    void  SetUserName(char* pUseName);

    /* ���û�ȡm_yUserLevel */
    BYTE  GetUserLevel();
    void  SetUserLevel(BYTE yUserLevel);

    /* ���û�ȡm_IP */
    UINT4 GetIP();
    void  SetIP(UINT4 uIP);

    /* ���û�ȡm_uParaNum */
    void  SetParaNum(UINT2 uParaNum);
    UINT2 GetParaNum();

    /* ��m_ParaVector������һ���������� */
    void AddPara(CParameter* pPara);

    /* ���ݲ���������ò������� */
    CParameter* GetPara(UINT2 uParaNo);

    /* ���ݲ�������ò������� */
    CParameter* GetPara(const char* pParaName);

    void SetErrMsg(const char* pErrMsg);
    const char* GetErrMsg();

    void SetCmdLine(char* pCmdLine);
    const char* GetCmdLine();

    /*
        ��һ��CParseResult�������л�,�Ա��ڽ��̼䴫��.
    */
    void Serialize(char* pData,const UINT4 uLength);

    /*
        ���CParseResult�������л���ĳ���.
    */
    UINT4 GetParseLength();

    /*
        ������Ϣ�����л������ݹ���CParseResult����.
    */
    void CreateParseResult(MSG_INTRA* pMsg);

public:
    const char* GetCmdName();

    /* ����/��ȡ������ʾ */
    void SetCmdDspName(const char* p);
    const char* GetCmdDspName();

    /* ���ݲ������ȡֵ���(ȱʡΪ0),�õ�����ֵ������ */
    BYTE GetParaValueFormat(UINT2 uParaNo,UINT2 uValSeq = 1);

    /* ���ݲ�������ȡֵ���(ȱʡΪ0),�õ�����ֵ������ */
    BYTE GetParaValueFormat(char* pParaName,UINT2 uValSeq = 1);

    /*
        ���ݲ������ȡֵ���,�õ��ַ�������ֵ.
        �ú���ֻ��ȡ��ֵ������ֵ,�����������͵Ĳ���,����NULL.
    */
    const char* GetStringVal(UINT2 uParaNo,UINT2 uValSeq = 1);

    /*
        ���ݲ������ȡֵ���,�õ��ַ�������ֵ.
        �ú���ֻ��ȡ��ֵ������ֵ,�����������͵Ĳ���,����NULL.
    */
    const char* GetStringVal(const char* pParaName,
                                UINT2 uValSeq = 1);

    /*
        ���ݲ������ȡֵ���,�õ���ֵ����ֵ.
        ֻ��ȡ��ֵ������ֵ,�����������͵Ĳ���,����FALSE.
        ���������Ͳ�����ֵ��,����FALSE.
    */
    BOOL GetDigitVal(INT* pOut,UINT2 uParaNo,UINT2 uValSeq = 1);

    /*
        ���ݲ�������ȡֵ���,�õ���ֵ����ֵ.
        ֻ��ȡ��ֵ������ֵ,�����������͵Ĳ���,����FALSE.
        ���������Ͳ�����ֵ��,����FALSE.
    */
    BOOL GetDigitVal(INT* pOut,char* pParaName,UINT2 uValSeq = 1);

    /*
        ���ݲ�����õ�ĳ������������ȡֵ,����ַ���ֵ.
    */
    BOOL GetRegionStringVal(UINT2 uParaNo,
                            char** pMinVal,   /* out,����������� */
                            char** pMaxVal,   /* out,����������� */
                            UINT2* puStep,    /* out,������䲽�� */
                            UINT2 uValSeq = 1);

    /*
        ���ݲ������õ�ĳ������������ȡֵ,����ַ���ֵ.
    */
    BOOL GetRegionStringVal(char* pParaName,
                            char** pMinVal,   /* out,����������� */
                            char** pMaxVal,   /* out,����������� */
                            UINT2* puStep,    /* out,������䲽�� */
                            UINT2 uValSeq = 1);

    /*
        ���ݲ�����õ�ĳ������������ȡֵ,�����ֵ�Ͳ���ֵ.
    */
    BOOL GetRegionDigitVal(UINT2 uParaNo,
                           INT* pMinVal,    /* out,����������� */
                           INT* pMaxVal,    /* out,����������� */
                           UINT2* puStep,   /* out,������䲽�� */
                           UINT2 uValSeq = 1);

    /*
        ���ݲ������õ�ĳ������������ȡֵ,�����ֵ�Ͳ���ֵ.
    */
    BOOL GetRegionDigitVal(char* pParaName,
                           INT* pMinVal,    /* out,����������� */
                           INT* pMaxVal,    /* out,����������� */
                           UINT2* puStep,   /* out,������䲽�� */
                           UINT2 uValSeq = 1);
};

#endif
