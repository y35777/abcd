/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CConvert
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
#include "postfix_exp.h"
#include "../include/toolbox.h"
#include "utility_ex.h"
#include "convert.h"


/************ The CConvert Defining ****************************/
// �������������캯��
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CConvert::CConvert()
{
    //��ʼ����Ա����
    m_pDestFmt = NULL;
    m_pSrcFmt = NULL;
    m_pConvertArray = NULL;
    m_pConvertMap = NULL;

    for(int i=0; i<MAX_SRC_NUM; i++)
    {
        m_pSrcValue[i] = new BYTE[FMT_FIELD_MAX_LEN];
    }
}
/************ End of The CConvert Defining *********************/


/************ The ~CConvert Defining ***************************/
// ������������������
//
// �����������
//
// �����������
//
//   ����ֵ����
//
/***************************************************************/
CConvert::~CConvert()
{
    CONVERT_MAP::iterator item;  //�������������ת����ϵ������


    //������ת����ϵ����
    if(m_pConvertMap != NULL)
    {
        //ɾ��ת����ϵ�е�����SFieldConvertMap����
        item = m_pConvertMap->begin();
        while(item != m_pConvertMap->end())
        {
            delete item->second;
            item++;
        }

        //ɾ���������������ת����ϵ����
        delete m_pConvertMap;
        m_pConvertMap = NULL;

        //ɾ����������������ת����ϵ�������
        delete[] m_pConvertArray;
        m_pConvertArray = NULL;
    }

    for(int i=0; i<MAX_SRC_NUM; i++)
    {
        if(m_pSrcValue[i] != NULL)
        {
            delete[] m_pSrcValue[i];
        }
    }
}
/************ End of The ~CConvert Defining ********************/


extern char szFormatCfgErr[2048];


/************ The Init Defining ********************************/
// ������������ʼ������
//
// ���������
//   pDestFmt��ת����Ӧ��Ŀ���ʽ����
//    pSrcFmt��ת����Ӧ��Դ��ʽ����
//
// �����������
//
//   ����ֵ�������������-1�����򷵻�0
//
/***************************************************************/
int CConvert::Init(CFormat* pDestFmt, CFormat* pSrcFmt)
{
    int           nFieldNum    = 0;     //Ŀ���ʽ�������
    FIELD_MAP*    pSrcFmtMap   = NULL;  //Դ��ʽ������ӳ��ָ��
    SFieldInfo*   pFI          = NULL;  //Ŀ���ʽ��ǰ������ָ��
    SFieldInfo*   pScrFI       = NULL;  //ԭ��ʽ����ָ��
    SFieldInfo**  pFieldsArray = NULL;  //Ŀ���ʽ������ָ��

    SFieldConvertMap*   pFieldConMap = NULL;  //Ŀ���ʽ��ǰ��ת������ӳ��ָ��
    FIELD_MAP::iterator iFMPItem;   //��ǰ��ת�����������б���ĳ��������Ӧ��ԭ��ʽӳ��λ��

    STRING        sLib;      //��ǰ��ת�������Ŀ���
    STRING        sFun;      //��ǰ��ת����������
    STRING        sDefault;  //��ǰ��ת�������е�ȱʡֵ
    LIST<STRING>  ParaList;  //��ǰ��ת�������Ĳ����б�
    LIST<STRING>::iterator iPLItem;  //��ǰ��ת�����������б������


    //���ݴ���Ĳ���ֵ��ʼ��ԭ��ʽ�����Ŀ���ʽ����
    m_pDestFmt = pDestFmt;
    m_pSrcFmt = pSrcFmt;
    if((m_pDestFmt == NULL) || (m_pSrcFmt == 0))
    {
        return -1;
    }

    //��ȡĿ���ʽ����������������
    pFieldsArray = (SFieldInfo**)pDestFmt->GetFieldsArray();
    nFieldNum = pDestFmt->GetFieldNum();

    //��ȡԭ��ʽ����������������ӳ��
    pSrcFmtMap = (FIELD_MAP*)pSrcFmt->GetFieldsMap();

    //������������������ת����ϵ���������������ת����ϵ
    m_pConvertArray = new SFieldConvertMap*[nFieldNum];
    m_pConvertMap = new CONVERT_MAP;

    //��ʼ��ÿһ������ʽ������ת������ָ��Ķ�Ӧ��ϵ
    for(int i = 0; i< nFieldNum; i++)
    {
        //����һ������ʽ������ת������ָ��Ķ�Ӧ��ϵ
        //��������������������ת����ϵ��������
        pFieldConMap = new SFieldConvertMap;
        m_pConvertArray[i] = pFieldConMap;

        //��ʼ������ʽ���Ӧ��ϵ�еĸ�ʽ����ָ��
        pFieldConMap->pFieldInfo = (SFieldInfo*)pFieldsArray[i];

        //��ȡ��ǰ��ĸ�ʽ����ָ��
        pFI = pFieldConMap->pFieldInfo;

        //�ѵ�ǰĿ���ʽ��ϵ���뵽�������������ת����ϵ��
        m_pConvertMap->insert(CONVERT_MAP::value_type(pFI->szFieldName, pFieldConMap));

        //�����ǰ���ת�������ַ���Ϊ�գ���Ŀ���ʽ��ֻ��Ӧԭ��ʽ��һ������������ͬ
        if(strlen(pFI->szConvertFun) == 0)
        {
            pFieldConMap->ySrcNum = 1;
            pFieldConMap->ConvertFun = NULL;

            iFMPItem = pSrcFmtMap->find(pFI->szFieldName);
            if(iFMPItem != pSrcFmtMap->end())
            {
                //����ҵ������ȡ��ԭ��ʽ������������Ÿ�������ʽ��ϵ��
                //���㱾��ʽ����Ҫ�õ���Դ��ʽ��Ӧ�������
                pScrFI = iFMPItem->second;
                pFieldConMap->Index[0] = pScrFI->nFieldIndex;
            }
            else
            {
                SNPRINTF(szFormatCfgErr,
                         sizeof(szFormatCfgErr),
                         S_FMT_CONFUN_FIELD_PARA_NOT_FOUND,
                         pFI->szFieldName, 
                         m_pDestFmt->GetFmtName(), 
                         pFI->szConvertFun, 
                         pFI->szFieldName, 
                         m_pSrcFmt->GetFmtName());
                TRACE(MTS_FORMAT, szFormatCfgErr);

                //ɾ�����д�����SFieldConvertMap����
                while(i >= 0)
                {
                    delete m_pConvertArray[i];
                    m_pConvertArray[i--] = NULL;
                }

                //ɾ����������������ת����ϵ�������
                delete[] m_pConvertArray;
                m_pConvertArray = NULL;

                //ɾ���������������ת����ϵ����
                delete m_pConvertMap;
                m_pConvertMap = NULL;

                return -1;
            }

            continue;
        }

        //������ʽ��ǰ���ת�������ַ��������ɿ������������������б���ȱʡֵ
        if(CConvertFun::ConvertAnalysis(pFI->szConvertFun, sLib, sFun,
                                        sDefault, ParaList) != 0)
        {
            SNPRINTF(szFormatCfgErr,
                     sizeof(szFormatCfgErr),
                     S_FMT_CONFUN_CAN_NOT_ALALYSIS,
                     pFI->szFieldName, 
                     m_pDestFmt->GetFmtName(), 
                     pFI->szConvertFun);
            TRACE(MTS_FORMAT, szFormatCfgErr);
                     
            //���ת�������ַ���ʧ�ܣ���ɾ�����д�����SFieldConvertMap����
            while(i >= 0)
            {
                delete m_pConvertArray[i];
                m_pConvertArray[i--] = NULL;
            }

            //ɾ����������������ת����ϵ�������
            delete[] m_pConvertArray;
            m_pConvertArray = NULL;

            //ɾ���������������ת����ϵ����
            delete m_pConvertMap;
            m_pConvertMap = NULL;

            return -1;
        }

        //��ʼ������ʽ���Ӧ��ϵ�е�ǰ���Ӧ��ԭ��ʽ�����
        //pFieldConMap->ySrcNum = ParaList.size();
		pFieldConMap->ySrcNum = 0;
        if(GetParaValue(sDefault, pFieldConMap->sDefault,
            CPostfixExp::GetDataType(sDefault.c_str())) != 0)
        {

            SNPRINTF(szFormatCfgErr,
                     sizeof(szFormatCfgErr),
                     S_FMT_CONFUN_DEFAULT_CAN_NOT_ALALYSIS,
                     pFI->szFieldName, 
                     m_pDestFmt->GetFmtName(), 
                     pFI->szConvertFun);
            TRACE(MTS_FORMAT, szFormatCfgErr);

            return -1;
        }

        int j = 0;  //���㱾��ʽ����Ҫ�õ���Դ��ʽ�����������������

        //��Ա���ʽ���ת��������ÿһ������
        iPLItem = ParaList.begin();
        while(iPLItem != ParaList.end())
        {
            //Modified by XDB on 2003.04.01
            //���ӳ��������б��ȱʡֵ����
            //�����жϲ��������ͣ�������Ǳ������;���Ϊ�ǳ�������
            int nParaType = CPostfixExp::GetDataType((*iPLItem).c_str());
            if(nParaType != ET_VARIANT)
            {
                //������������
                STRING sTmp;
                if(GetParaValue(*iPLItem, sTmp, nParaType) != 0)
                {
                    SNPRINTF(szFormatCfgErr,
                             sizeof(szFormatCfgErr),
                             S_FMT_CONFUN_INVALID_PARA_TYPE,
                             pFI->szFieldName, 
                             m_pDestFmt->GetFmtName(), 
                             pFI->szConvertFun, 
                             (*iPLItem).c_str());
                    TRACE(MTS_FORMAT, szFormatCfgErr);

                    return -1;
                }

                pFieldConMap->ConstValues.push_back(sTmp);
                ++iPLItem;
                continue;
            }

            //���ҵ�ǰ������Ӧ��ԭ��ʽӳ���ϵλ��
			pFieldConMap->ySrcNum++;
            iFMPItem = pSrcFmtMap->find(*iPLItem);
            if(iFMPItem != pSrcFmtMap->end())
            {
                //����ҵ������ȡ��ԭ��ʽ������������Ÿ�������ʽ��ϵ��
                //���㱾��ʽ����Ҫ�õ���Դ��ʽ��Ӧ�������
                pScrFI = iFMPItem->second;
                pFieldConMap->Index[j++] = pScrFI->nFieldIndex;
            }

            //�����ǰ������ԭ��ʽ��û���ҵ�
            else
            {
                SNPRINTF(szFormatCfgErr,
                         sizeof(szFormatCfgErr),
                         S_FMT_CONFUN_FIELD_PARA_NOT_FOUND,
                         pFI->szFieldName, 
                         m_pDestFmt->GetFmtName(), 
                         pFI->szConvertFun, 
                         (*iPLItem).c_str(), 
                         m_pSrcFmt->GetFmtName());
                TRACE(MTS_FORMAT, szFormatCfgErr);
                
                //ɾ�����д�����SFieldConvertMap����
                while(i >= 0)
                {
                    delete m_pConvertArray[i];
                    m_pConvertArray[i--] = NULL;
                }

                //ɾ����������������ת����ϵ�������
                delete[] m_pConvertArray;
                m_pConvertArray = NULL;

                //ɾ���������������ת����ϵ����
                delete m_pConvertMap;
                m_pConvertMap = NULL;

                return -1;
            }

            iPLItem++;
        }

        //�������ʽ����ת������
        if(sFun.length() != 0)
        {
            //������Ӧ��ת������ָ�룬����������ʽ��Ӧ��ϵ�е�ת������ָ��
            pFieldConMap->ConvertFun = CConvertFun::FindFun(pFI->szConvertFun, m_pSrcFmt);
            if(NULL == pFieldConMap->ConvertFun)
            {
                SNPRINTF(szFormatCfgErr,
                         sizeof(szFormatCfgErr),
                         S_FMT_CONFUN_NOT_FOUND,
                         pFI->szFieldName, 
                         m_pDestFmt->GetFmtName(), 
                         pFI->szConvertFun);
                TRACE(MTS_FORMAT, szFormatCfgErr);

                return -1;
            }
        }

        //�������ʽû������ת������
        else
        {
            pFieldConMap->ConvertFun = NULL;
        }
        //Modified by XDB on 2003.04.01
        //���Ӳ����������жϣ��Ա��ڱ�֤��������������
        if((pFieldConMap->ySrcNum + pFieldConMap->ConstValues.size()) > MAX_SRC_NUM)
        {

            SNPRINTF(szFormatCfgErr,
                     sizeof(szFormatCfgErr),
                     S_FMT_CONFUN_PARA_NUM_EXCEED_LIMIT,
                     pFI->szFieldName, 
                     m_pDestFmt->GetFmtName(), 
                     pFI->szConvertFun);
            TRACE(MTS_FORMAT, szFormatCfgErr);            

            return -1;
        }
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The ConvertABill Defining ************************/
// ������������һ��Դ��ʽ����ת����Ŀ���ʽ����
//
// ���������
//    pSrcABill��Դ��ʽ����ָ��
//  uSrcBillLen��pSrcABillָ��Ļ����ĳ���
//
// ���������
//
//   pDestABill��ת�����Ŀ���ʽ����ָ�룬�ڴ��ڱ������ڷ��䣬�ɵ����߸����ͷţ�
// uDestBillLen��pDestABillָ��Ļ�������
//
//   ����ֵ�������������-1�����򷵻�0
//
/***************************************************************/
int CConvert::ConvertABill(const void*  pSrcABill, const UINT4 uSrcBillLen, 
                           void*& pDestABill, UINT4& uDestBillLen)
{
    SFieldConvertMap* pFieldConMap = NULL;  //Ŀ���ʽ���ϵ�е�һ���ڵ�

    UINT4  uByteOffset  = 0;      //���ֽ�Ϊ��λ��Ŀ���ʽ��ƫ����
    UINT4  uFieldLen    = 0;      //ת������Ŀ���ʽ�򳤶�
    UINT4  uBitLen      = 0;      //λ�����͵���ת������Ŀ���ʽ�򳤶�
    UINT4  uBitOffset   = 0;      //λ�����͵���ת��ʱ��λƫ����

    UINT4  uTmpVal1     = 0;      //λ�����͵���ת��ʱ����ʱ����1
    UINT4  uTmpVal2     = 0;      //λ�����͵���ת������ʱ����2
    BYTE*  pTmp         = NULL;   //Ŀ���ʽ����ʱָ��

	UINT4  pSrcLen[MAX_SRC_NUM];       //Ŀ���ʽ���Ӧ��ԭ��ʽ����ĳ���
	UINT4  uSrcNum  = 0;               //Ŀ���ʽ��Ӧ��ԭ��ʽ�����
    BYTE*  pValue = m_szFieldBuffer;   //ת������Ŀ���ʽ��ֵ

    //��ʼ���������
    uDestBillLen = 0;
    pDestABill = NULL;

    //ȷ������Ļ������ݲ�Ϊ��
    if((pSrcABill == NULL) || (uSrcBillLen == 0))
    {
        return -1;
    }

    //����Ŀ���ʽ���ȣ�����Ŀ���ʽ��Ҫ�Ŀռ�
    uDestBillLen = m_pDestFmt->GetFmtLen();
    pDestABill = (void*)new BYTE[uDestBillLen];
    pTmp = (BYTE*)pDestABill;

    //��Ŀ���ʽ��ÿһ�������ת��
    int i = 0;
    UINT4 uFieldNum = m_pDestFmt->GetFieldNum();
    while(i < uFieldNum)
    {
        //ȡ��Ŀ���ʽ��һ�����ϵ����
        pFieldConMap = m_pConvertArray[i++];

        //ת����ǰ����Ҫ�ú���
        if(pFieldConMap->ConvertFun == NULL)
        {
            //ȷ��Ŀ���ʽ�ĵ�ǰ��ֻ��Ӧԭ��ʽ��һ����
            if(pFieldConMap->ySrcNum != 1)
            {
                delete[] (BYTE*)pDestABill;
                pDestABill = NULL;
                uDestBillLen = 0;

                return -1;
            }

            //��ǰ����λ����
            if(pFieldConMap->pFieldInfo->flgBit == 0)
            {
                //���ݵ�ǰ���Ӧԭ��ʽ���������ת������ǰ���ֵ
                //SWPD03631 �������һ������ʵ�ֶ�����ȡö�� 2004-02-18 ZhengYuqun
                if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0 , pFieldConMap->Index[0],
                                                    pValue, uFieldLen, pFieldConMap->pFieldInfo)) == 0)
                {
                    //������ֵ����Ҫ�ǶԸ��ֳ��ȵ���������ֵ�ĵ���
                    uFieldLen = AdjustValue(pValue, uFieldLen, pFieldConMap->pFieldInfo);

					//��Ҫ���϶��ֽ���Ļָ����ӱ����ֽ���intel���ߵ�����
					ConvertToDestOrder(pFieldConMap->pFieldInfo, pValue);

                    //����ǰ��ת���Ľ������Ŀ���ʽ����������ȥ
                    memcpy((void*)(pTmp + uByteOffset), (void*)pValue, uFieldLen);
                    uByteOffset += uFieldLen;  //����Ŀ���ʽ��һ�������ʼλ��
                }
                //�����ǰ��ת������
                else
                {
                    TRACE(MTS_FORMAT, S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL, 
                          m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                          i - 1, pFieldConMap->Index[0]);
                    delete[] (BYTE*)pDestABill;
                    pDestABill = NULL;
                    uDestBillLen = 0;

                    return -1;
                }
            }
            //��ǰ����λ����
            else
            {
                //����Ŀ��ʽ��һ��λ���������ʼλ��
                uBitOffset = pFieldConMap->pFieldInfo->nFieldLength;
                uFieldLen = CFormat::GetFieldTypeLen(pFieldConMap->pFieldInfo);
                if(uFieldLen <= 0)
                {
                    TRACE(MTS_FORMAT, S_FMT_CONVERT_FIELD_LEN_EMPTY, 
                          m_pDestFmt->GetFmtName(), i - 1);
                    delete[] (BYTE*)pDestABill;
                    pDestABill = NULL;
                    uDestBillLen = 0;

                    return -1;
                }

                //�����ֽ�������
                int nLShift = 0;  //����λ��
                if(pFieldConMap->pFieldInfo->nOrderMode == INTEL_ORDER)
                {
                    nLShift = 0;
                }
                else
                {
                    nLShift = (uFieldLen * 8) - pFieldConMap->pFieldInfo->nFieldLength;
                }

                //��õ�һ��λ������ֵ
                //SWPD03631 �������һ������ʵ�ֶ�����ȡö�� 2004-02-18 ZhengYuqun
                if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[0], 
                                                    pValue, uBitLen, pFieldConMap->pFieldInfo)) == 0)
                {
                    //��ȡ��ǰλ��ת������ֵ
                    if(uBitLen == LEN_BYTE)
                    {
                        BYTE nTmp = 0;
                        memcpy(&nTmp, pValue, LEN_BYTE);
                        uTmpVal1 = nTmp;
                        ///uTmpVal1 = *(BYTE*)pValue;
                    }
                    else if(uBitLen == LEN_UINT2)
                    {
                        UINT2 nTmp = 0;
                        memcpy(&nTmp, pValue, LEN_UINT2);
                        uTmpVal1 = nTmp;
                        //uTmpVal1 = *(UINT2*)pValue;
                    }
                    else if(uBitLen == LEN_UINT4)
                    {
                        UINT4 nTmp = 0;
                        memcpy(&nTmp, pValue, LEN_UINT4);
                        uTmpVal1 = nTmp;
                        //uTmpVal1 = *(UINT4*)pValue;
                    }
                    else
                    {
                        TRACE(MTS_FORMAT, S_FMT_CONVERT_FIELD_LEN_EXCEED_BITFLD, 
                              m_pDestFmt->GetFmtName(), i - 1);
                        delete[] (BYTE*)pDestABill;
                        pDestABill = NULL;
                        uDestBillLen = 0;

                        return -1;
                    }

                    //Added by ZhengYuqun SWPD07486 2004-08-09
                    UINT4 uTmpMask = 0xFFFFFFFF;
                    uTmpMask >>= 32 - pFieldConMap->pFieldInfo->nFieldLength;
                    uTmpVal1 &= uTmpMask;   
                    //End SWPD07486
                }

                //���ת����ǰ�����
                else
                {
                    TRACE(MTS_FORMAT, S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL, 
                          m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                          i - 1, pFieldConMap->Index[0]);
                    delete[] (BYTE*)pDestABill;
                    pDestABill = NULL;
                    uDestBillLen = 0;

                    return -1;
                }

                uTmpVal1 <<= nLShift;  //Added by XDB on 2002.10.28
                //����ת��Ŀ���ʽ����һ��������
                //ֱ��������λ���ͻ��� (nBitOffset >= uFieldLen * 8)
                while((uBitOffset < (uFieldLen * 8)) && (i < uFieldNum))
                {
                    //ȡ��Ŀ���ʽ����һ�����ϵ����
                    pFieldConMap = m_pConvertArray[i++];

                    //�����ǰ����λ����
                    if(pFieldConMap->pFieldInfo->flgBit != 0)
                    {
                        //Added by XDB on 2002.10.28
                        //�����ֽ�������
                        if(pFieldConMap->pFieldInfo->nOrderMode == INTEL_ORDER)
                        {
                            nLShift = uBitOffset;
                        }
                        else
                        {
                            nLShift = (uFieldLen * 8) - uBitOffset
                                      - pFieldConMap->pFieldInfo->nFieldLength;
                        }
                        //End of Add by XDB
                        //��õ�ǰλ������ֵ
                        //SWPD03631 �������һ������ʵ�ֶ�����ȡö�� 2004-02-18 ZhengYuqun
                        if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[0], 
                                                            pValue, uBitLen, pFieldConMap->pFieldInfo)) == 0)
                        {
                            //��ȡ��ǰλ��ת������ֵ
                            if(uBitLen == LEN_BYTE)
                            {
                                BYTE nTmp = 0;
                                memcpy(&nTmp, pValue, LEN_BYTE);
                                uTmpVal2 = nTmp;
                            }
                            else if(uBitLen == LEN_UINT2)
                            {
                                UINT2 nTmp = 0;
                                memcpy(&nTmp, pValue, LEN_UINT2);
                                uTmpVal2 = nTmp;
                            }
                            else if(uBitLen == LEN_UINT4)
                            {
                                UINT4 nTmp = 0;
                                memcpy(&nTmp, pValue, LEN_UINT4);
                                uTmpVal2 = nTmp;
                            }
                            else
                            {
                                TRACE(MTS_FORMAT, S_FMT_CONVERT_FIELD_LEN_EXCEED_BITFLD, 
                                      m_pDestFmt->GetFmtName(), i - 1);
                                delete[] (BYTE*)pDestABill;
                                pDestABill = NULL;
                                uDestBillLen = 0;

                                return -1;
                            }

                            //Added by ZhengYuqun SWPD07486 2004-08-09
                            UINT4 uTmpMask = 0xFFFFFFFF;
                            uTmpMask >>= 32 - pFieldConMap->pFieldInfo->nFieldLength;
                            uTmpVal2 &= uTmpMask;                            
                            //End SWPD07486

                            //�ѵ�ǰλ��ֵ����ǰһ��λ����
                            //Modified by XDB on 2002.10.28
                            uTmpVal1 |= (uTmpVal2 << nLShift);

                            //����Ŀ��ʽ��һ��λ���������ʼλ��
                            uBitOffset += pFieldConMap->pFieldInfo->nFieldLength;
                        }

                        //���ת����ǰ�����
                        else
                        {
                            TRACE(MTS_FORMAT, S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL, 
                                  m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                                  i - 1, pFieldConMap->Index[0]);
                            delete[] (BYTE*)pDestABill;
                            pDestABill = NULL;
                            uDestBillLen = 0;

                            return -1;
                        }                 
                    }

                    //�����ǰ����λ������
                    else
                    {
                        i--;  //�˻ص�ǰ��
                        break;
                    }                    
                }

                //��ת������λ��ֵ���뵽Ŀ���ʽ����������ȥ
                if(uFieldLen == LEN_BYTE)
                {
                    BYTE yTmp = (BYTE)uTmpVal1;
                    memcpy((pTmp + uByteOffset), &yTmp, LEN_BYTE);
                }
                else if(uFieldLen == LEN_UINT2)
                {
                    UINT2 uTmp = (UINT2)uTmpVal1;
                    ConvertToDestOrder(pFieldConMap->pFieldInfo, &uTmp);
                    memcpy((pTmp + uByteOffset), &uTmp, LEN_UINT2);
                }
                else if(uFieldLen == LEN_UINT4)
                {
                    ConvertToDestOrder(pFieldConMap->pFieldInfo, &uTmpVal1);

                    memcpy((pTmp + uByteOffset), &uTmpVal1, LEN_UINT4);
                }

                //����Ŀ���ʽ��һ�������ʼλ��
                uByteOffset += uFieldLen;
            }
        }

        //��ǰ����Ҫ����ת��
        else
        {
            //��ȡ��ǰ���Ӧ��ԭ��ʽ�����
            uSrcNum = pFieldConMap->ySrcNum;

            //��ȡ��ǰ���Ӧ��ԭ��ʽ�������ֵ���䳤��
            for(int j = 0; j < uSrcNum; j++)
            {
                if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[j], 
                                                   m_pSrcValue[j], pSrcLen[j])) != 0)
                {
                    TRACE(MTS_FORMAT, S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL, 
                          m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                          i - 1, pFieldConMap->Index[j]);

                    delete[] (BYTE*)pDestABill;
                    pDestABill = NULL;
                    uDestBillLen = 0;

                    return -1;
                }
            }

            //Modified by XDB on 2003.04.01
            //���ӳ��������б��ȱʡֵ����
            //�������г�������
            LIST<STRING>::iterator it = pFieldConMap->ConstValues.begin();
            while(it != pFieldConMap->ConstValues.end())
            {
                memcpy(m_pSrcValue[uSrcNum], it->c_str(), (it->length() + 1));
                pSrcLen[uSrcNum++] = it->length() + 1;
                ++it;
            }
            uFieldLen = CFormat::GetFieldTypeLen(pFieldConMap->pFieldInfo);
            uFieldLen *= pFieldConMap->pFieldInfo->nFieldLength;

            //���õ�ǰ���ת�������ó���ǰ���ֵ
            int nRet = pFieldConMap->ConvertFun((void*)pValue, uSrcNum, (const void **)m_pSrcValue,
                                                pSrcLen, pFieldConMap->pFieldInfo,
                                                pFieldConMap->sDefault.c_str(), 
                                                pFieldConMap->sDefault.length() + 1, 0);

            //���ת�������������
            if(nRet != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_CONVERT_FAIL, 
                      m_pSrcFmt->GetFmtName(), 
                      m_pDestFmt->GetFmtName(), 
                      i - 1);
                delete[] (BYTE*)pDestABill;
                pDestABill = NULL;
                uDestBillLen = 0;

                return -1;
            }

			//��Ҫ���϶��ֽ���Ļָ����ӱ����ֽ���intel���ߵ�����
			ConvertToDestOrder(pFieldConMap->pFieldInfo, pValue);
            //����ǰ��ת���Ľ������Ŀ���ʽ����������ȥ
            memcpy((void*)(pTmp + uByteOffset), (void*)pValue, uFieldLen);
            uByteOffset += uFieldLen;
        }
    }

    return 0;
}
/************ End of The ConvertABill Defining *****************/


/************ The ConvertAField Defining ***********************/
// �������������һ��Դ��ʽ�������Ŀ���ʽ��һ�����ȡֵ
//
// ���������
//  pSrcABill������Դ��ʽ��һ�Ż���
// sFieldName��Ŀ���ʽ��һ����ʽ����
//   nIsQuery���Ƿ��ѯ��־��1��ʾ�ú���Ϊ��ѯ����ת����
//             0��ʾ����Ϊ��ѯ����ת��
//
// ���������
// pDestAField��szFieldName��ȡֵ��
//        uLen��pDestAFieldָ�������֡���ȣ��ڴ��ڱ����������룬�ɵ����߸����ͷ�
//
//   ����ֵ�������������-1�����򷵻�0
//
/***************************************************************/
int CConvert::ConvertAField(const void* pSrcABill, const STRING& sFieldName, 
                            void*& pDestAField, UINT4& uDestFieldLen,
                            int nIsQuery)
{
    SFieldConvertMap* pFieldConMap = NULL;  //Ŀ���ʽ���ϵ�е�һ���ڵ�

    //void*  pSrcValue[MAX_SRC_NUM];   //Ŀ���ʽ���Ӧ��ԭ��ʽ������
	UINT4  pSrcLen[MAX_SRC_NUM];       //Ŀ���ʽ���Ӧ��ԭ��ʽ����ĳ���
	UINT4  uSrcNum  = 0;               //Ŀ���ʽ��Ӧ��ԭ��ʽ�����
    CONVERT_MAP::iterator item;        //Ŀ���ʽ��ת����ϵ������
    BYTE*  pValue = m_szFieldBuffer;   //ת������Ŀ���ʽ��ֵ


    //��ʼ���������
    pDestAField = NULL;
    uDestFieldLen = 0;

    //ȷ������Ĳ�����ȷ
    if((pSrcABill == NULL) || (sFieldName.length() == 0))
    {
        return -1;
    }

    //��Ŀ���ʽ�в���ָ������
    item = m_pConvertMap->find(sFieldName);
    if(item != m_pConvertMap->end())
    {
        pFieldConMap = item->second;
    }
    //���Ŀ���ʽ�в�����ָ������
    else
    {
        TRACE(MTS_FORMAT, S_FMT_FLDNAME_NOT_FONND_CONVERT_A_FLD, 
              m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
              sFieldName.c_str());
        return -1;
    }

    //ָ������Ҫת������
    if(pFieldConMap->ConvertFun == NULL)
    {
        //ȷ��ָ����ֻ��Ӧԭ��ʽ��һ����
        if(pFieldConMap->ySrcNum != 1)
        {
            return -1;
        }

        //����ָ�����Ӧԭ��ʽ���������ת����ָ�����ֵ
        if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0 , pFieldConMap->Index[0],
                                            pValue, uDestFieldLen)) == 0)
        {
            //������ֵ����Ҫ�ǶԸ��ֳ��ȵ���������ֵ�ĵ���
            uDestFieldLen = AdjustValue(pValue, uDestFieldLen, pFieldConMap->pFieldInfo, nIsQuery);

			//��Ҫ���϶��ֽ���Ļָ����ӱ����ֽ���intel���ߵ�����
			ConvertToDestOrder(pFieldConMap->pFieldInfo, pValue);

            //����Ŀ���ʽ�������ȣ�Ϊ�����ռ�
            pDestAField = (void*)new BYTE[uDestFieldLen];
            memcpy(pDestAField, pValue, uDestFieldLen);
        }
        //�����ǰ��ת������
        else
        {
            TRACE(MTS_FORMAT, S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD, 
                  m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                  pFieldConMap->pFieldInfo->nFieldIndex, pFieldConMap->Index[0]);
            return -1;
        }
    }

    //ָ������Ҫ����ת��
    else
    {
        //��ȡָ�����Ӧ��ԭ��ʽ�����
        uSrcNum = pFieldConMap->ySrcNum;

        //��ȡָ�����Ӧ��ԭ��ʽ�������ֵ���䳤��
        for(int j = 0; j < uSrcNum; j++)
        {
            if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[j], 
                                                m_pSrcValue[j], pSrcLen[j])) != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD, 
                      m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                      pFieldConMap->pFieldInfo->nFieldIndex, pFieldConMap->Index[j]);

                return -1;
            }
        }

        //Modified by XDB on 2003.04.01
        //���ӳ��������б��ȱʡֵ����
        //�������г�������
        LIST<STRING>::iterator it = pFieldConMap->ConstValues.begin();
        while(it != pFieldConMap->ConstValues.end())
        {
            memcpy(m_pSrcValue[uSrcNum], it->c_str(), (it->length() + 1));
            pSrcLen[uSrcNum++] = it->length() + 1;
            ++it;
        }
        //����Ŀ���ʽ��ĳ��ȣ�Ϊ�����ռ�
        uDestFieldLen = CFormat::GetFieldTypeLen(pFieldConMap->pFieldInfo);
        uDestFieldLen *= pFieldConMap->pFieldInfo->nFieldLength;
        pDestAField = (void*)new BYTE[uDestFieldLen];

        //�������ת�������ó����ֵ
        int nRet = pFieldConMap->ConvertFun((void*)pDestAField, uSrcNum, (const void **)m_pSrcValue,
                                            pSrcLen, pFieldConMap->pFieldInfo,
                                            pFieldConMap->sDefault.c_str(), 
                                            pFieldConMap->sDefault.length() + 1, nIsQuery);

        //���ת�������������
        if(nRet != 0)
        {
            TRACE(MTS_FORMAT, S_FMT_CONVERT_FAIL_CONVERT_A_FLD, 
                  m_pSrcFmt->GetFmtName(), 
                  m_pDestFmt->GetFmtName(), 
                  pFieldConMap->pFieldInfo->nFieldIndex);
            delete[] (BYTE*)pDestAField;
            pDestAField = NULL;
            uDestFieldLen = 0;

            return -1;
        }
    }

    return 0;
}
/************ End of The ConvertAField Defining ****************/

/************ The ConvertAFieldByIndex Defining ***********************/
// �������������һ��Դ��ʽ�������Ŀ���ʽ��һ�����ȡֵ
//
// ���������
//  pSrcABill������Դ��ʽ��һ�Ż���
// nFieldIndex��Ŀ���ʽ��һ�������
//   nIsQuery���Ƿ��ѯ��־��1��ʾ�ú���Ϊ��ѯ����ת����
//             0��ʾ����Ϊ��ѯ����ת��
//
// ���������
// pDestAField��szFieldName��ȡֵ��
//        uLen��pDestAFieldָ�������֡���ȣ��ڴ��ڱ����������룬�ɵ����߸����ͷ�
//
//   ����ֵ�������������-1�����򷵻�0
//
/***************************************************************/
int CConvert::ConvertAFieldByIndex(const void* pSrcABill, UINT2 nFieldIndex, 
                                   void* pDestAField, UINT4& uDestFieldLen,
                                   int nIsQuery)
{
    //ȷ������Ĳ�����ȷ
    if(pSrcABill == NULL)
    {
        return -1;
    }

	SFieldConvertMap* pFieldConMap = NULL;  //Ŀ���ʽ���ϵ�е�һ���ڵ�

	UINT4  pSrcLen[MAX_SRC_NUM];       //Ŀ���ʽ���Ӧ��ԭ��ʽ����ĳ���
	UINT4  uSrcNum  = 0;               //Ŀ���ʽ��Ӧ��ԭ��ʽ�����
    CONVERT_MAP::iterator item;        //Ŀ���ʽ��ת����ϵ������

    //��Ŀ���ʽ�в���ָ������
	pFieldConMap = m_pConvertArray[nFieldIndex];
    if(NULL == pFieldConMap)
    {
        return -1;
    }

    
    //ָ������Ҫת������
    if(pFieldConMap->ConvertFun == NULL)
    {
        //ȷ��ָ����ֻ��Ӧԭ��ʽ��һ����
        if(pFieldConMap->ySrcNum != 1)
        {
            return -1;
        }

        //����ָ�����Ӧԭ��ʽ���������ת����ָ�����ֵ
        if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[0], 
                                            pDestAField, uDestFieldLen)) == 0)
        {
            //������ֵ����Ҫ�ǶԸ��ֳ��ȵ���������ֵ�ĵ���
            uDestFieldLen = AdjustValue((BYTE*)pDestAField, uDestFieldLen, 
                                        pFieldConMap->pFieldInfo, nIsQuery);

			//��Ҫ���϶��ֽ���Ļָ����ӱ����ֽ���intel���ߵ�����
			ConvertToDestOrder(pFieldConMap->pFieldInfo, pDestAField);
        }
        //�����ǰ��ת������
        else
        {
            TRACE(MTS_FORMAT, S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD, 
                  m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                  nFieldIndex, pFieldConMap->Index[0]);
            return -1;
        }
    }

    //ָ������Ҫ����ת��
    else
    {
        //��ȡָ�����Ӧ��ԭ��ʽ�����
        uSrcNum = pFieldConMap->ySrcNum;

        //��ȡָ�����Ӧ��ԭ��ʽ�������ֵ���䳤��
        for(int j = 0; j < uSrcNum; j++)
        {
            if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[j], 
                                                m_pSrcValue[j], pSrcLen[j])) != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD, 
                      m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                      nFieldIndex, pFieldConMap->Index[j]);
                return -1;
            }
        }

        //Modified by XDB on 2003.04.01
        //���ӳ��������б��ȱʡֵ����
        //�������г�������
        LIST<STRING>::iterator it = pFieldConMap->ConstValues.begin();
        while(it != pFieldConMap->ConstValues.end())
        {
            memcpy(m_pSrcValue[uSrcNum], it->c_str(), (it->length() + 1));
            pSrcLen[uSrcNum++] = it->length() + 1;
            ++it;
        }
        //����Ŀ���ʽ��ĳ��ȣ�Ϊ�����ռ�
        uDestFieldLen = CFormat::GetFieldTypeLen(pFieldConMap->pFieldInfo);
        uDestFieldLen *= pFieldConMap->pFieldInfo->nFieldLength;

		
        //�������ת�������ó����ֵ
        int nRet = pFieldConMap->ConvertFun((void*)pDestAField, uSrcNum, (const void **)m_pSrcValue,
                                            pSrcLen, pFieldConMap->pFieldInfo,
                                            pFieldConMap->sDefault.c_str(), 
                                            pFieldConMap->sDefault.length() + 1, nIsQuery);

        //���ת�������������
        if(nRet != 0)
        {
            TRACE(MTS_FORMAT, S_FMT_CONVERT_FAIL_CONVERT_A_FLD, 
                  m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                  nFieldIndex);
            uDestFieldLen = 0;
            return -1;
        }
        
    }

    return 0;
}
/************ End of The ConvertAField Defining ****************/
//Added by XDB on 2003.04.01
//��ȡ�����ַ���ֵ
int CConvert::GetParaValue(const STRING& sParaStr, STRING& sParaValue, int nParaType)
{
	//�����ж�������ַ����Ƿ�Ϊ��
	if(sParaStr.size() == 0)
	{
		sParaValue = "";
		return 0;
	}

    if(nParaType == ET_INT_CONST)
    {
        //��������
        sParaValue = sParaStr;
    }
    else if((nParaType == ET_STRING_CONST) || (nParaType == ET_UNSIGNED_CHAR_CONST))
    {
        //�ַ����������ַ�������
        sParaValue = sParaStr.substr(1, sParaStr.length() - 2);
    }
    else //��������Ϊ�Ƿ�
    {
        //��ʱ��������������
        return -1;
    }

    return 0;
}

/************ The HomeToIntelOrNet Defining ***********************/
// �����������ѱ����ֽ���ת��ΪINTEL��Net�ֽ���
//
// ���������
//  SFieldInfo* pFI����ǰ��ʽ�Ľṹ
//  void* pFieldValue��ת��ǰ��ֵ
//
// ���������
// void* pFieldValue��ת�����ֵ��
// 
//added by maimaoshi,2002-10-24
//Modified by XDB on 2002.10.28
/***************************************************************/
int CConvert::ConvertToDestOrder(const SFieldInfo* pFI, void* pFieldValue)
{
    //INTEL_ORDER	0;��ʾ��ʽΪINTEL�ֽ���
    //NET_ORDER 	1;��ʾ��ʽΪ�����ֽ���
    if((pFI == NULL) || (pFieldValue == NULL))
    {
        return -1;
    }  
    
    INT2  nShortValue = 0;
	INT4  nIntValue = 0;
    if ((pFI->nDataType == FT_SHORT) || (pFI->nDataType == FT_USHORT))
    {
        //����2�ֽڵ����ʹ���
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //ԭ��ʽΪ�����ֽ�����Ҫ�ѱ����ֽ���תΪINTEL�ֽ���
            memcpy(&nShortValue, pFieldValue, LEN_INT2);
            nShortValue = HTOIS(nShortValue);
            memcpy(pFieldValue, &nShortValue, LEN_INT2);
        }
        else
        {
            //ԭ��ʽΪ�����ֽ�����Ҫ�ѱ����ֽ���תΪ�����ֽ���
            memcpy(&nShortValue, pFieldValue, LEN_INT2);
            nShortValue = HTONS(nShortValue);
            memcpy(pFieldValue, &nShortValue, LEN_INT2);			
        }
        
    }
    else if ((pFI->nDataType == FT_LONG) || (pFI->nDataType == FT_ULONG))
    {
        //����4�ֽڵ����ʹ���
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //ԭ��ʽΪ�����ֽ�����Ҫ�ѱ����ֽ���תΪINTEL�ֽ���
            memcpy(&nIntValue, pFieldValue, LEN_INT4);
            nIntValue = HTOIL(nIntValue);
            memcpy(pFieldValue, &nIntValue, LEN_INT4);
        }
        else
        {
            //ԭ��ʽΪ�����ֽ�����Ҫ�ѱ����ֽ���תΪ�����ֽ���
            memcpy(&nIntValue, pFieldValue, LEN_INT4);
            nIntValue = HTONL(nIntValue);
            memcpy(pFieldValue, &nIntValue, LEN_INT4);
        }        
    }

    return 0;   
}
/************ End of The HomeToIntelOrNet Defining ****************/

//Added by XDB on 2003.04.04 and 2003.04.07
//������ֵ��������Ҫ�ǽ�ԭֵ��Ŀ��ֵ�ĳ��Ȳ�һ�µ����
//���ص��������ֵ����
UINT4 CConvert::AdjustValue(BYTE* pValue, UINT4 nOrigLen, const SFieldInfo* pFI, int nIsQuery)
{
    const FIELD_MAP* pFieldMap = m_pSrcFmt->GetFieldsMap();
    FIELD_MAP::const_iterator iter = pFieldMap->find(pFI->szFieldName);
    if(iter == pFieldMap->end())
    {
        return 0;
    }

    //���ԭֵ�ĳ��ȵ���Ŀ��ֵ�ĳ��ȣ����õ���
    UINT4 nDestLen = CFormat::GetFieldTypeLen(pFI) * pFI->nFieldLength;
    const SFieldInfo* pSrcFI = iter->second;

    if(pFI->nFieldLength == pSrcFI->nFieldLength && pSrcFI->nDataType == pFI->nDataType)
    {
        //memmove(pValue, pValue, nDestLen);
        return nDestLen;
    }
    //ע�⣬ֻ�ж���ĳ��������ֵ���ܵ���������������ֱ�ӿ���Ŀ���ֵ����
    switch(pFI->nDataType)
    {
    case FT_BYTE:
    case FT_CHAR:
        //���Ŀ��ֵ��һ�ֽ���������
        if(nDestLen == LEN_BYTE)
        {
            if(nOrigLen == LEN_UINT2)
            {
                UINT2 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT2);
                BYTE nValue = nTmp;
                memcpy(pValue, &nValue, LEN_BYTE);
                return LEN_BYTE;
            }
            else if(nOrigLen == LEN_UINT4)
            {
                UINT4 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT4);
                BYTE nValue = nTmp;
                memcpy(pValue, &nValue, LEN_BYTE);
                return LEN_BYTE;
            }
        }
        break;

    case FT_SHORT:
    case FT_USHORT:
        //���Ŀ��ֵ�Ƕ��ֽ���������
        if(nDestLen == LEN_UINT2)
        {
            if(nOrigLen == LEN_BYTE)
            {
                BYTE nTmp = 0;
                memcpy(&nTmp, pValue, LEN_BYTE);
                UINT2 nValue = nTmp;
                memcpy(pValue, &nValue, LEN_UINT2);
                return LEN_UINT2;
            }
            else if(nOrigLen == LEN_UINT4)
            {
                UINT4 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT4);
                UINT2 nValue = nTmp;
                memcpy(pValue, &nValue, LEN_UINT2);
                return LEN_UINT2;
            }
        }
        break;

    case FT_LONG:
    case FT_ULONG:
        //���Ŀ��ֵ�����ֽ���������
        if(nDestLen == LEN_UINT4)
        {
            if(nOrigLen == LEN_BYTE)
            {
                BYTE nTmp = 0;
                memcpy(&nTmp, pValue, LEN_BYTE);
                UINT4 nValue = nTmp;
                memcpy(pValue, &nValue, LEN_UINT4);
                return LEN_UINT4;
            }
            else if(nOrigLen == LEN_UINT2)
            {
                UINT2 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT2);
                UINT4 nValue = nTmp;
                memcpy(pValue, &nValue, LEN_UINT4);
                return LEN_UINT4;
            }
        }
        break;

    case FT_STRING:
        switch(pSrcFI->nDataType) {
        case FT_STRING:
            pValue[nOrigLen] = '\0';
        	break;

        case FT_CHAR:
            {
                char nTmp = 0;
                memcpy(&nTmp, pValue, LEN_CHAR);
                SNPRINTF((char*)pValue, nDestLen,
                         "%d",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_BYTE:
            {
                BYTE nTmp = 0;
                memcpy(&nTmp, pValue, LEN_BYTE);
                SNPRINTF((char*)pValue, nDestLen,
                         "%u",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_SHORT:
            {
                INT2 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_INT2);
                SNPRINTF((char*)pValue, nDestLen,
                         "%d",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_USHORT:
            {
                UINT2 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_INT2);
                SNPRINTF((char*)pValue, nDestLen,
                         "%u",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_LONG:
            {
                INT4 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT4);
                SNPRINTF((char*)pValue, nDestLen,
                         "%d",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_ULONG:
            {
                UINT4 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT4);
                SNPRINTF((char*)pValue, nDestLen,
                         "%u",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_BCD:
            {
                char szTemp[FMT_FIELD_MAX_LEN] = "";
                UINT4 nLen = FMT_FIELD_MAX_LEN;
                BCDtoString(pValue, szTemp,
                            nOrigLen,
                            nLen);

                //ȥ���ս��֮����ַ���
                char* p1 = szTemp;
                while((*p1 >= '0')
                      && (*p1 <= '9'))
                {
                    p1++;
                }
                *p1 = '\0';

                strncpy((char*)pValue, szTemp, nDestLen);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        default:
            break;
        }

        if(!nIsQuery)
        {
            CConvertFun::AddFillerSeparator((char*)pValue, pFI);
        }
        break;

    default:
        break;
    }
    
    return nDestLen;
}


