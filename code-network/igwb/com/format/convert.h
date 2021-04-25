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
// 功能描述：格式转换类
//
/********************************************************************/
class CConvert
{
public:

    CConvert();         //构造函数
    ~CConvert();        //析构函数

    //初始化函数
    int Init(CFormat* pDestFmt, CFormat* pSrcFmt);

    //将一张源格式话单转换成目标格式函数
    int ConvertABill(const void*  pSrcABill, const UINT4 uSrcBillLen, 
                     void*& pDestABill, UINT4& uDestBillLen);

    //针对一张源格式话单获得目标格式中一个域的取值
    int ConvertAField(const void* pSrcABill, const STRING& sFieldName, 
                      void*& pDestAField, UINT4& uDestFieldLen,
                      int nIsQuery = 0);

	//针对一张源格式话单获得目标格式中一个域的取值
	int ConvertAFieldByIndex(const void* pSrcABill, UINT2 nFieldIndex, 
		                     void* pDestAField, UINT4& uDestFieldLen,
		                     int nIsQuery);

    //Added by XDB on 2003.04.01
    //获取参数字符串值
    static int GetParaValue(const STRING& sParaStr, STRING& sParaValue,
                            int nParaType);
    //把本机字节序转换为INTEL和Net字节序
    //Modified by XDB on 2002.10.28
	static int ConvertToDestOrder(const SFieldInfo* pFI, void* pFieldValue);

    //隐式数据类型转换
    UINT4 AdjustValue(BYTE* pValue, UINT4 nOrigLen, const SFieldInfo* pFI, int nIsQuery = 0);

protected:

    CFormat*            m_pDestFmt;		       //格式转换对应的目标格式对象
    CFormat*            m_pSrcFmt;		       //格式转换对应的源格式对象
    SFieldConvertMap**  m_pConvertArray;       //以域序号索引域的转换关系
    CONVERT_MAP*        m_pConvertMap;         //以域名索引域的转换关系

    BYTE m_szFieldBuffer[FMT_FIELD_MAX_LEN];   //话单转换时存放一个域的缓冲区
    BYTE* m_pSrcValue[MAX_SRC_NUM];            //目标格式域对应的原格式域数组

};
/****************** End of The CConvert class Defining **************/

#endif  //!defined(_FORMAT_CONVERT_H_)
