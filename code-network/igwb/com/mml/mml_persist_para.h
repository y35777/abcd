/*
    ��Ʒ��:iGateway Bill V200
    ģ����:MML Server
    �ļ���:mml_persist_para.h
    ������Ϣ:
    
    ����CPersistPara��Ķ���,����ʵ����������Ĺ���,
    �����Ӵ����ļ��ж�ȡ��������,��֤�����в����ĺϷ��Ե�.

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2002 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ��ӵ��,2001-10-24,����.
*/

#ifndef _MML_PERSIST_PARAMETER_H
#define _MML_PERSIST_PARAMETER_H

#include "../include/precom.h"
#include "../utility/mml_para.h"
#include "../include/mmlcmd_def.h"

/*
    �����װ�˲���/�Ӳ����Ľṹ.
*/
class CPersistPara
{
protected:
    char m_szParaName[PARA_NAME_LEN];   /* ������       */
    UINT2 m_uParaCode;                  /* ������       */
    UINT2 m_uChildParaCode;             /* �Ӳ�����     */
    UINT2 m_uCmdCode;                   /* ������       */
    BYTE m_yDataType;                   /* ��������     */
    BYTE m_yRangeType;                  /* ֵ������     */
    BYTE m_yMustGive;                   /* ���������� */
    INT4 m_nMin;                        /* ��������     */
    INT4 m_nMax;                        /* ��������     */
    char m_szDefValue[VALUE_LEN];       /* ȱʡֵ       */
    UINT2 m_uChildParaNum;              /* �Ӳθ���     */

    /*
        ����ö�ٻ�λ���������ֵ�Ķ�Ӧ��ϵ,
        ��һ������Ϊö�ٻ�λ�������,�ڶ�������Ϊö��ֵ,
        λ���Ӧ����λ���.
        ������ֻ��ֵ������Ϊö�ٻ��������Ϊλ��ʱ��Ч.
    */
    MAP<STRING,UINT4> m_EnumValueMap;

    /*
        �ò������Ӳ��б�,��û���Ӳ�ʱ���鳤��Ϊ0.
    */
    VECTOR<CPersistPara*> m_ChildPara;

public:
    CPersistPara(UINT2 uCmdCode,UINT2 uParaCode,
                 UINT2 uChildParaCode = MAX_UINT2);
    virtual ~CPersistPara();

    /*
        ��ʼ������/�Ӳ���.
    */
    int Init();
    int InitPara();
    int InitChildPara();

    /*
        ������ֵ/�Ӳ���ֵ����Ч��.
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
        ��ȡ������Ա����.
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
