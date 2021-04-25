#if !defined(_FORMAT_CONVERT_H_)
#define _FORMAT_CONVERT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CConvert
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormat;

/****************** The CConvert class Defining *********************/
// ������������ʽת����
//
/********************************************************************/
class CConvert
{
public:

    CConvert();         //���캯��
    ~CConvert();        //��������

    //��ʼ������
    int Init(CFormat* pDestFmt, CFormat* pSrcFmt);

    //��һ��Դ��ʽ����ת����Ŀ���ʽ����
    int ConvertABill(const void*  pSrcABill, const UINT4 uSrcBillLen, 
                     void*& pDestABill, UINT4& uDestBillLen);

    //���һ��Դ��ʽ�������Ŀ���ʽ��һ�����ȡֵ
    int ConvertAField(const void* pSrcABill, const STRING& sFieldName, 
                      void*& pDestAField, UINT4& uDestFieldLen,
                      int nIsQuery = 0);

	//���һ��Դ��ʽ�������Ŀ���ʽ��һ�����ȡֵ
	int ConvertAFieldByIndex(const void* pSrcABill, UINT2 nFieldIndex, 
		                     void* pDestAField, UINT4& uDestFieldLen,
		                     int nIsQuery);

    //Added by XDB on 2003.04.01
    //��ȡ�����ַ���ֵ
    static int GetParaValue(const STRING& sParaStr, STRING& sParaValue,
                            int nParaType);
    //�ѱ����ֽ���ת��ΪINTEL��Net�ֽ���
    //Modified by XDB on 2002.10.28
	static int ConvertToDestOrder(const SFieldInfo* pFI, void* pFieldValue);

    //��ʽ��������ת��
    UINT4 AdjustValue(BYTE* pValue, UINT4 nOrigLen, const SFieldInfo* pFI, int nIsQuery = 0);

protected:

    CFormat*            m_pDestFmt;		       //��ʽת����Ӧ��Ŀ���ʽ����
    CFormat*            m_pSrcFmt;		       //��ʽת����Ӧ��Դ��ʽ����
    SFieldConvertMap**  m_pConvertArray;       //��������������ת����ϵ
    CONVERT_MAP*        m_pConvertMap;         //�������������ת����ϵ

    BYTE m_szFieldBuffer[FMT_FIELD_MAX_LEN];   //����ת��ʱ���һ����Ļ�����
    BYTE* m_pSrcValue[MAX_SRC_NUM];            //Ŀ���ʽ���Ӧ��ԭ��ʽ������

};
/****************** End of The CConvert class Defining **************/

#endif  //!defined(_FORMAT_CONVERT_H_)
