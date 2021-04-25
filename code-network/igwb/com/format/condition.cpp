/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CCondition
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/


#include "convert_fun.h"
#include "format.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "condition.h"

/************ The CCondition Defining **************************/
// �������������캯��
//
// ���������
// pFmtOwning��������ʽ
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CCondition::CCondition(CFormat* pFmtOwning): m_pFmtOwning(pFmtOwning)
{
    //��ʼ����Ա����
    //m_pFmtOwning = (CFormat*)pFmtOwning;
    m_bThisFmt = 0;
    m_nIsQuery = 1;
}
/************ End of The CCondition Defining *******************/


/************ The CCondition Defining **************************/
// �������������캯��
//
// ���������
//    pFmtOwning��������ʽ
// sDisCondition��ʶ����������׺���ʽ
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CCondition::CCondition(CFormat* pFmtOwning, const STRING& sDisCondition):
                       m_pFmtOwning(pFmtOwning)
{
    //��ʼ����Ա����
    //m_pFmtOwning = (CFormat*)pFmtOwning;
    m_bThisFmt = 0;
    m_nIsQuery = 1;

    //����׺���ʽת���ɺ�׺���ʽ
    if(Infix2Postfix(sDisCondition.c_str()) != 0)
    {
        assert(0);
    }
}
/************ End of The CCondition Defining *******************/


/************ The ~CCondition Defining *************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CCondition::~CCondition()
{
}
/************ End of The ~CCondition Defining ******************/


/************ The SetFlag Defining *****************************/
// �������������ø�ʽ��־����
//
// ���������
//   bThisFmt����ʽ��־��Ϊ0��ʾ����ʽ��Ϊ-1��ʾԴ��ʽ
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CCondition::SetFlag(const int bThisFmt)
{
    m_bThisFmt = bThisFmt;
}
/************ End of The SetFlag Defining **********************/


/************ The SetQueryFlag Defining ************************/
// �������������ò�ѯ������־����
//
// ���������
//   nIsQuery����ѯ������־��Ϊ1��ʾ��ǰ�����ǲ�ѯ��������
//             Ϊ0��ʾ��ǰ�����ǲ�ѯ��������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
void CCondition::SetQueryFlag(int nIsQuery)
{
    m_nIsQuery = nIsQuery;
}
/************ End of The SetQueryFlag Defining *****************/


/************ The GetVarValue Defining *************************/
// �������������һ��������ʵ��ȡֵ�����ػ�����麯��
//
// ���������
//    szInput���������ݣ�����һ�Ż���
//  szVarName��������
//
// �����������
//
//   ����ֵ�����ؼ�����ı���ֵ���ڴ��ڱ������ڷ��䣬�ɱ������ĵ����߸����ͷ�
//
/***************************************************************/
//��Ӧ���ⵥD17165 2002.9.26
#define STARTTIME_LENGTH  19
//�޸�
int CCondition::GetVarValue(const void* szInput, const char* szVarName, SVariant& stVar)
{
    //ȷ������Ĳ�����ȷ
    if(szInput == NULL)
    {
        return -1;
    }
	
    if(szVarName == NULL)
	{
		return -1;
	}
	
    UINT2 nFieldIndex = 0;
    memcpy(&nFieldIndex, szVarName, sizeof(UINT2));
    UINT4        uLen = 0;      //������ֵʱ�Ľ������
    SFieldInfo*  pFIF = NULL;   //��ǰ������Ӧ��������ָ��
	SFieldInfo**   pFieldsArray;

    //��û������ֵ������
    m_pFmtOwning->GetFieldContentByIndex(szInput, m_bThisFmt, nFieldIndex, 
                                         (void *)(stVar.szValue), uLen, m_nIsQuery);
    if(uLen == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_GET_VALUE_BY_IDX_FAIL, 
              nFieldIndex, m_pFmtOwning->GetFmtName());
        return -1;
    }

    if (uLen > 63)
    {
        TRACE(MTS_FORMAT, S_FMT_VAL_LEN_EXCEED_LIMIT_GET_BY_IDX, 
              nFieldIndex, m_pFmtOwning->GetFmtName(), 
              uLen, 63);
        return -1;  //�����ȡ��ֵʧ��
    }

    //by lx SWPD04662
    //2002.9.26 by ldf ��Ӧ���ⵥD17165
    ////2002.11.19 by ldf ��Ӧ���ⵥ D18928 
    /*if(uLen > STARTTIME_LENGTH)
    {
        (stVar.szValue)[STARTTIME_LENGTH] = 0;
    }*/
    //change end

    //��öԸ�ʽ���������ã�����ȡ��ǰ������Ӧ�ĸ�ʽ����ӳ���еĽڵ�    
	pFieldsArray = (SFieldInfo**)m_pFmtOwning->GetFieldsArray();
	pFIF = pFieldsArray[nFieldIndex];

    //�����ǰ�����ǻ�����ʽ�е�һ����
    if(NULL != pFIF)
    {
        //�����ǰ���������ַ���
        if(pFIF->nDataType == FT_CHAR)
        {
            stVar.yVariantType = FT_CHAR;
			BYTE yValue = stVar.szValue[0];
            memcpy(&stVar.yValue, (void *)&yValue, 1);
        }

        //�����ǰ����BCD�롢����BCD�롢�ַ������ͣ�
        //Ϊ�˱��ʽ���㷽�㣬BCD�롢����BCD�롢�ַ������Ͷ����ַ������ͱ�ʾ
        else if((pFIF->nDataType == FT_BCD) || (pFIF->nDataType == FT_RBCD)
                || (pFIF->nDataType == FT_STRING)) 
        {
            stVar.yVariantType = FT_STRING;
            stVar.szValue[uLen] = '\0';
        }

        //�����ǰ�������ͣ���ʱ������INT8���ͣ���
        //Ϊ�˱��ʽ���㷽�㣬�������Ͷ���INT4�ͱ�ʾ
        else if((pFIF->nDataType == FT_BYTE) || (pFIF->nDataType == FT_SHORT)
                || (pFIF->nDataType == FT_USHORT) || (pFIF->nDataType == FT_LONG)
                || (pFIF->nDataType == FT_ULONG))
        {
            
            stVar.yVariantType = FT_ULONG;        
            if(uLen == LEN_BYTE)
            {
                BYTE nTmp = 0;
                memcpy(&nTmp, stVar.szValue, LEN_BYTE);
                stVar.u4Value = nTmp;
            }
            else if(uLen == LEN_INT2)
            {
                INT2 nTmp = 0;
                memcpy(&nTmp, stVar.szValue, LEN_INT2);
                stVar.u4Value = nTmp;
            }
            else if(uLen == LEN_INT4)
            {
                INT4 nTmp = 0;
                memcpy(&nTmp, stVar.szValue, LEN_INT4);
                stVar.u4Value = nTmp;
            }            
            else
            {
                TRACE(MTS_FORMAT, S_FMT_VAL_LEN_EXCEED_INT_GET_BY_IDX, 
                      nFieldIndex, m_pFmtOwning->GetFmtName(), uLen);
                return -1;
            }
        }

        //�Ƿ�����
        else
        {
            TRACE(MTS_FORMAT, S_FMT_UNKONWN_FIELD_TYPE, 
                  m_pFmtOwning->GetFmtName(), nFieldIndex);
            return -1;
        }
    }

    //���������ʽ�в����ڵ�ǰ������
    else
    {
        return -1;
    }

    //���ؼ�����
    return 0;
}

/************ End of The GetVarValue Defining ******************/

/************ The GetFunReturn Defining ************************/
// ��������������һ�����������ֵ�����ػ�����麯��
//
// ���������
//    szInput���������ݣ�����һ�Ż���
//      szFun�������ַ���
//
// �����������
//
//   ����ֵ�����ؼ�����ı���ֵ���ڴ��ڱ������ڷ��䣬�ɱ������ĵ����߸����ͷ�
//
/***************************************************************/
int CCondition::GetFunReturn(const void* szInput, const char* szFun, SVariant& stVar)
{
    STRING  sLib;                 //��
    STRING  sFun;                 //������
    STRING  sDefault;             //ȱʡֵ
    LIST<STRING>  ParaList;       //�����б�
    LIST<STRING>::iterator item;  //�����б������

    void*  pParas[MAX_PARA_NUM];      //����������������
	UINT4  nParasLen[MAX_PARA_NUM];   //���������������ĳ���
    EPostfixEleType eParaType;        //�����������������͵���ʱ����

	UINT4  uParasNum  = 0;            //�������������ĸ���
    char*  pTmpStr   = NULL;          //�ַ�����ʱ���� 

    CONDITIONFUN pCondFun = NULL;     //��������ָ��
    

    //ȷ������Ĳ�����ȷ
    if((szInput == NULL) || (szFun == NULL))
    {
        return -1;
    }
    
    //����������������
    for(int n = 0; n < MAX_PARA_NUM; n++)
    {
        pParas[n] = NULL;
    }

    //���������ַ���Ϊ��������������ȱʡֵ�������б�
    if((CConvertFun::ConvertAnalysis(szFun, sLib, sFun, sDefault, ParaList)) != 0)
    {
        TRACE(MTS_FORMAT, S_FMT_FUN_CAN_NOT_ALALYSIS, 
              szFun, m_pFmtOwning->GetFmtName());
        return -1;    //���������ַ�������
    }

    //���������Ϊ��
    else if(sFun.length() == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_FUN_CAN_NOT_ALALYSIS, 
              szFun, m_pFmtOwning->GetFmtName());
        return -1;
    }
    uParasNum = ParaList.size();
    
    //��ȡ������������ָ��
    pCondFun = CConvertFun::FindConFun(sFun);
    if(pCondFun == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_FUN_NOT_FOUND, 
              szFun, m_pFmtOwning->GetFmtName());
        return -1;
    }

    //�ֱ��ȡÿһ����������������ֵ
    int i = 0;
    item = ParaList.begin();
    while(item != ParaList.end())
    {
        eParaType = GetDataType((*item).c_str());

        //�����ǰ�����Ǳ�������
        if(eParaType == ET_VARIANT)
        {
            //��ȡ��ǰ����������Ӧ���ֵ�ͳ���
            if((m_pFmtOwning->GetFieldValueByName(szInput, m_bThisFmt, *item,
               pParas[i], nParasLen[i], m_nIsQuery)) != 0)
            {
                //�ͷ��Ѿ�����Ĳ����Ŀռ�
                while(i >= 0)
                {
                    if(pParas[i] != NULL)
                    {
                        delete[] (BYTE*)pParas[i];
                        pParas[i] = NULL;
                    }

                    --i;
                }

                TRACE(MTS_FORMAT, S_FMT_FUN_GET_VALUE_BY_NAME_FAIL, 
                      szFun, (*item).c_str(), m_pFmtOwning->GetFmtName());
                return -1;  //�����ȡ��ֵ����
            }
        }

        //�����ǰ���������γ���
        else if(eParaType == ET_INT_CONST)
        {
            pParas[i] = new BYTE[LEN_INT4];            
            //*(UINT4*)pParas[i] = atol(item->c_str());
            INT4 nTmp = atol(item->c_str());
            memcpy(pParas[i], (void*)&nTmp, LEN_INT4);
            nParasLen[i] = LEN_INT4;
        }

        //�����ǰ�������ַ��ͳ���
        else if(eParaType == ET_UNSIGNED_CHAR_CONST)
        {
            pParas[i] = new BYTE[LEN_BYTE];

            //����ַ��ͳ����а��������ţ���'a'
            if(item->length() == 3)
            {
                *(char*)pParas[i] = (*item)[1];
            }

            //����ַ��ͳ����в�����������
            else
            {
                *(char*)pParas[i] = (*item)[0];
            }            
            nParasLen[i] = LEN_BYTE;
        }

        //�����ǰ�������ַ�������
        else if(eParaType == ET_STRING_CONST)
        {
            //����ַ��������а���'@'��˫���ţ�'@'�����ַ������ߵ�˫����
            if(((*item)[0] == '@') || ((*item)[0] == '\"'))
            {
                pParas[i] = new BYTE[item->length() - 1];
                nParasLen[i] = item->length() - 1;

                //pTmpStrָ�������ַ�����'@'��˫���ŵ���һ��λ��
                pTmpStr = (char*)item->c_str();
                pTmpStr++;
                memcpy(pParas[i], (void*)pTmpStr, item->length() - 2);

                //�������ַ��������ӽ�����
                pTmpStr = (char*)pParas[i];
                pTmpStr += item->length() - 2;
                *pTmpStr = '\0';
            }

            //����ַ��������в�����'@'��˫����
            else
            {
                pParas[i] = new BYTE[item->length() + 1];
                strcpy((char*)pParas[i], item->c_str());
                nParasLen[i] = item->length() + 1;
            }            
        }

        //�����ǷǷ�����
        else
        {
            //�ͷ��Ѿ�����Ĳ����Ŀռ�
            while(i >= 0)
            {
                if(pParas[i] != NULL)
                {
                    delete[] (BYTE*)pParas[i];
                    pParas[i] = NULL;
                }

                --i;
            }

            TRACE(MTS_FORMAT, S_FMT_INVALID_PARA_TYPE, 
                  szFun, (*item).c_str(), m_pFmtOwning->GetFmtName());
            return NULL;
        }

        i++;
        item++;
    }

    //���ݲ�����ȡ���������Ĵ�����
    int nRet = pCondFun(uParasNum, (const void**)pParas, nParasLen, sDefault, stVar);

    //�ͷ������������������Ŀռ�
    for(int k = 0; k < MAX_PARA_NUM; k++)
    {
        if(pParas[k] != NULL)
        {
            delete[] (BYTE*)pParas[k];
            pParas[k] = NULL;
        }
    }

    return nRet;
}
/************ End of The GetFunReturn Defining *****************/

/************ The Infix2Postfix Defining ************************/
// ��������������׺���ʽת��Ϊ��׺���ʽ�����ػ���ĺ���
//
// ���������
//   ��
//
// �����������
//
//   ����ֵ�����������-1�����򷵻�0
//
/***************************************************************/
int CCondition::Infix2Postfix(const char* szInfix)
{
	//���ɺ�׺���ʽ
    int nResult = CPostfixExp::Infix2Postfix(szInfix);
    if (ERR_SUCCESS != nResult)
    {
        return -1;
    }
    
    
    PostfixExpEleList::iterator itemPostExp;  
    FIELD_MAP::iterator itemFieldMap;
    SFieldInfo*  pFI   = NULL;
    FIELD_MAP* fieldmap = (FIELD_MAP*)m_pFmtOwning->GetFieldsMap();
    
    //���������͵ı��ʽԪ��ת��������Ŵ��
    itemPostExp = m_PostfixExp.begin();
    while(itemPostExp != m_PostfixExp.end())
    {
        if(itemPostExp->yEleType == ET_VARIANT)
        {
            itemFieldMap = fieldmap->find(itemPostExp->szEleContent);
            if(itemFieldMap == fieldmap->end())
            {
                TRACE(MTS_FORMAT, S_FMT_EXPRESS_FIELD_PARA_NOT_FOUND, 
                    szInfix, itemPostExp->szEleContent, m_pFmtOwning->GetFmtName());
                return -1;
            }
            
            pFI = itemFieldMap->second;
            itemPostExp->yEleType = 0;
            memcpy(itemPostExp->szEleContent, &(pFI->nFieldIndex), sizeof(UINT2));
        }		
        
        itemPostExp++;
    }
    
    return 0;
}
/************ End of The Infix2Postfix Defining *****************/


//��ÿ�������ͳ�Ƶ����б�
int CCondition::ParseExpress(const char* const  szExpress, LIST<STRING>& FieldList)
{
	if (Infix2Postfix(szExpress) != 0)
	{
		return -1;
	}

	PostfixExpEleList::iterator item;   //��׺���ʽ������
	//˳���ȡ��׺���ʽ��ÿһ��Ԫ��
    item = m_PostfixExp.begin();
    while(item != m_PostfixExp.end())
    {
        //�����ǰԪ���Ǳ�������
        if(item->yEleType == ET_VARIANT)
        {
            UINT2 nFieldIndex = 0;
            memcpy(&nFieldIndex, item->szEleContent,sizeof(UINT2));
	        const SFieldInfo** fields_array = m_pFmtOwning->GetFieldsArray();
            if(NULL != fields_array && nFieldIndex < m_pFmtOwning->GetFieldNum())
            {
	            const SFieldInfo* pFI = fields_array[nFieldIndex];
			    FieldList.push_back(pFI->szFieldName);
            }
		}
		item++;
	}
	return 0;
}
