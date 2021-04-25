#if !defined(_FORMAT_FORMAT_GPRS_H_)
#define _FORMAT_FORMAT_GPRS_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CFormatR98_760
//      Author:  maimaoshi
//        Date:  2002.04.03
/**************** End of This File's Head Comment *******************/


#include "../format/format_comm.h"
#include "../format/format.h"
#include "ace/Synch.h"


/****************** The CFormatR98_760 class Defining ********************/
// ����������Gprs������ʽ��
//
/********************************************************************/
class CFormatR98_760: public CFormat
{
public:

    //���캯��
    CFormatR98_760(const STRING& sRootDir, const STRING& sFmtName);

    //��������
    virtual ~CFormatR98_760();

    //��ʼ������
    virtual int Init(void);

    //����һ����ʽ����������ֽ�ƫ�ơ�λƫ���Լ��ֽ��ܳ��Ⱥ���
    virtual int InitFormatInfo(void);

    //�жϴ����һ�Ż����Ƿ���ϱ���ʽ
    virtual int IsTheFormat(const void* pABill,  const UINT4 uLen);

    //�������һ�Ż���ת����Ŀ���ʽ
    virtual int ConvertABill(const void* pASrcBill,
                             const UINT4 uSrcBillLen, 
                             void*& pADestBill,
                             UINT4& uDestBillLen);

    //�ж�һ�Ż����Ƿ����ָ���Ĳ�ѯ����
    virtual int IsTheQueryCondition(const void* pASrcBill, const STRING& sQueryCond = "");

    //���ݻ���������һ�Ż����л�ø����ֵ
    virtual int GetFieldValueByName(const void* pABill,
                                    const int bThisFmt,
                                    const STRING& sFieldName,
                                    void*& pFieldValue,
                                    UINT4& uLen,
                                    int nIsQuery = 0);

    //���ݻ����������Ŵ�һ�Ż����л�ø����ֵ(���ⲿ�����ڴ�)
    virtual int GetFieldContentByIndex(const void* pABill,
                                       const int bThisFmt,
                                       UINT2 nFieldIndex,  
                                       void* pFieldValue,
                                       UINT4& uLen,
                                       int nIsQuery);
    
protected:
	//���ݻ���������һ�Ż����л�ø����ֵ(����r98�ּ�)
	int GetFieldValueByNameEx(const void* pABill,
                              UINT4& uLen,  
		                      const STRING& sFieldName,
                              void*& pFieldValue);

	//���ݻ���������һ�Ż����л�ø����ֵ(���ⲿ�����ڴ�, ����r98�ּ�)
	int GetFieldContentByNameEx(const void* pABill,
                                UINT4& uLen,  
		                        const STRING& sFieldName,
                                void* pFieldValue);

    //��ȡ��ѯ������ֵ
    int GetQueryCondFieldsValue(const void* pABill);

protected:

	BOOL m_bIsDisFormat;                               //�Ƿ�����Gprs�ּ��ʽ
    int  m_nGprsFormatType;                            //Gprs������ʽ����
    QUERY_COND_FIELDS_VALUE m_QueryCondFieldsValue;    //��ѯ��������������ֵ��Ӧ��ϵ

};
/****************** End of The CFormat3G class Defining *************/


#endif  //!defined(_FORMAT_FORMAT_GPRS_H_)
