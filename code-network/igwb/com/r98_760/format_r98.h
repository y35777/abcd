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
// 功能描述：Gprs话单格式类
//
/********************************************************************/
class CFormatR98_760: public CFormat
{
public:

    //构造函数
    CFormatR98_760(const STRING& sRootDir, const STRING& sFmtName);

    //析构函数
    virtual ~CFormatR98_760();

    //初始化函数
    virtual int Init(void);

    //计算一个格式中所有域的字节偏移、位偏移以及字节总长度函数
    virtual int InitFormatInfo(void);

    //判断传入的一张话单是否符合本格式
    virtual int IsTheFormat(const void* pABill,  const UINT4 uLen);

    //将传入的一张话单转换成目标格式
    virtual int ConvertABill(const void* pASrcBill,
                             const UINT4 uSrcBillLen, 
                             void*& pADestBill,
                             UINT4& uDestBillLen);

    //判断一张话单是否符合指定的查询条件
    virtual int IsTheQueryCondition(const void* pASrcBill, const STRING& sQueryCond = "");

    //根据话单域名从一张话单中获得该域的值
    virtual int GetFieldValueByName(const void* pABill,
                                    const int bThisFmt,
                                    const STRING& sFieldName,
                                    void*& pFieldValue,
                                    UINT4& uLen,
                                    int nIsQuery = 0);

    //根据话单域索引号从一张话单中获得该域的值(由外部分配内存)
    virtual int GetFieldContentByIndex(const void* pABill,
                                       const int bThisFmt,
                                       UINT2 nFieldIndex,  
                                       void* pFieldValue,
                                       UINT4& uLen,
                                       int nIsQuery);
    
protected:
	//根据话单域名从一张话单中获得该域的值(用于r98分拣)
	int GetFieldValueByNameEx(const void* pABill,
                              UINT4& uLen,  
		                      const STRING& sFieldName,
                              void*& pFieldValue);

	//根据话单域名从一张话单中获得该域的值(由外部分配内存, 用于r98分拣)
	int GetFieldContentByNameEx(const void* pABill,
                                UINT4& uLen,  
		                        const STRING& sFieldName,
                                void* pFieldValue);

    //获取查询条件域值
    int GetQueryCondFieldsValue(const void* pABill);

protected:

	BOOL m_bIsDisFormat;                               //是否属于Gprs分拣格式
    int  m_nGprsFormatType;                            //Gprs话单格式类型
    QUERY_COND_FIELDS_VALUE m_QueryCondFieldsValue;    //查询条件的域名与域值对应关系

};
/****************** End of The CFormat3G class Defining *************/


#endif  //!defined(_FORMAT_FORMAT_GPRS_H_)
