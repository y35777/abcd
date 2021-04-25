#if !defined(_FORMAT_IBILL_PROCESSOR_IMPLEMENT_H_)
#define _FORMAT_IBILL_PROCESSOR_IMPLEMENT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IBillProcessorImp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "ibill_processor.h"

class CFormat;
class CIGWB;
class CAccessPoint;

/****************** The IBillProcessorImp Interface Defining ************/
// ������������������ӿ�ʵ��
//
/************************************************************************/
class IGWB_Svc_Export IBillProcessorImp: public IBillProcessor
{
public:

    //���캯��
    IBillProcessorImp(CIGWB* pIGWB);

    //��������
    virtual ~IBillProcessorImp();

    //���������ļ���ʼ��
    virtual int Init(const UINT4 nAPID = 0);

    //����һ�Ż������䳤�ȣ�������Ӧ�����ջ���ͨ��ID�����Ӧ�ĸ�ʽ�������
    virtual void Distribute(const void* pABill, const UINT4 uLen, 
                             UINT4& uChlID, CFormat*& pDestFmt);
	virtual void SpecDistribute(const void* pABill, const UINT4 uLen, 
							VECTOR<UINT4>& uChlID, VECTOR<CFormat*>& pDestFmt);

    //��õ�ǰ������ԭʼ��������
    virtual UINT4 GetOrigLen(void) const;

    //��õ�ǰ������ָ��ͨ�������ջ�������
    virtual UINT4 GetFinalLen(UINT4 uChlID) const;

    //��õ�ǰ���������ͨ������
    virtual UINT4 GetChannelNames(LIST<STRING>& ChlNames) const;

protected:

    CIGWB*   m_pIGWB;     //CIGWB����ָ��
    UINT4    m_uAPID;     //����������ӿڶ�Ӧ�Ľ����ID
    CAccessPoint* m_pAP;  //����������ӿڶ�Ӧ�Ľ�������ָ��

};
/*************** End of The IBillProcessorImp Interface Defining ********/

#endif  //!defined(_FORMAT_IBILL_PROCESSOR_IMPLEMENT_H_)
