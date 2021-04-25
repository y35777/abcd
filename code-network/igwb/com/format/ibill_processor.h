  #if !defined(_FORMAT_IBILL_PROCESSOR_H_)
#define _FORMAT_IBILL_PROCESSOR_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IBillProcessor
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormat;

/****************** The IBillProcessor Interface Defining ***************/
// ������������������ӿ�
//
/************************************************************************/
class IGWB_Svc_Export IBillProcessor
{
public:

    //������������
    virtual ~IBillProcessor(){};

    //���������ļ���ʼ��
    virtual int Init(const UINT4 nAPID = 0) = 0;

    //����һ�Ż������䳤�ȣ�������Ӧ�����ջ���ͨ��ID�����Ӧ�ĸ�ʽ�������
    virtual void Distribute(const void* pABill, const UINT4 uLen, 
                            UINT4& uChlID, CFormat*& pDestFmt) = 0;

	virtual void SpecDistribute(const void* pABill, const UINT4 uLen, 
							VECTOR<UINT4>& uChlID, VECTOR<CFormat*>& pDestFmt) = 0;

    //��õ�ǰ������ԭʼ��������
    virtual UINT4 GetOrigLen(void) const = 0;

    //��õ�ǰ������ָ��ͨ�������ջ�������
    virtual UINT4 GetFinalLen(UINT4 uChlID) const = 0;

    //��õ�ǰ���������ͨ������
    virtual UINT4 GetChannelNames(LIST<STRING>& ChlNames) const = 0;

};
/****************** End of The IBillProcessor Interface Defining ********/

#endif  //!defined(_FORMAT_IBILL_PROCESSOR_H_)
