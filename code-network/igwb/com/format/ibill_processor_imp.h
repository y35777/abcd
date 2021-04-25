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
// 功能描述：话单处理接口实现
//
/************************************************************************/
class IGWB_Svc_Export IBillProcessorImp: public IBillProcessor
{
public:

    //构造函数
    IBillProcessorImp(CIGWB* pIGWB);

    //析构函数
    virtual ~IBillProcessorImp();

    //根据配置文件初始化
    virtual int Init(const UINT4 nAPID = 0);

    //输入一张话单及其长度，输出其对应的最终话单通道ID及其对应的格式处理对象
    virtual void Distribute(const void* pABill, const UINT4 uLen, 
                             UINT4& uChlID, CFormat*& pDestFmt);
	virtual void SpecDistribute(const void* pABill, const UINT4 uLen, 
							VECTOR<UINT4>& uChlID, VECTOR<CFormat*>& pDestFmt);

    //获得当前接入点的原始话单长度
    virtual UINT4 GetOrigLen(void) const;

    //获得当前接入点的指定通道的最终话单长度
    virtual UINT4 GetFinalLen(UINT4 uChlID) const;

    //获得当前接入点所有通道名称
    virtual UINT4 GetChannelNames(LIST<STRING>& ChlNames) const;

protected:

    CIGWB*   m_pIGWB;     //CIGWB对象指针
    UINT4    m_uAPID;     //本话单处理接口对应的接入点ID
    CAccessPoint* m_pAP;  //本话单处理接口对应的接入点对象指针

};
/*************** End of The IBillProcessorImp Interface Defining ********/

#endif  //!defined(_FORMAT_IBILL_PROCESSOR_IMPLEMENT_H_)
