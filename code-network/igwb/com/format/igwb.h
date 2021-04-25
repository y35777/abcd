#if !defined(_FORMAT_IGWB_H_)
#define _FORMAT_IGWB_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CIGWB
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormatLib;
class IFormatCfgImp;
class IBillProcessorImp;
class IQueryBrowseImp;
class CAccessPoint;

/****************** The CIGWB class Defining ************************/
// 功能描述：格式转换类
//
/********************************************************************/
class IGWB_Svc_Export CIGWB
{
public:

    //构造函数
    CIGWB(const STRING& sCfgRoot);

    //析构函数
    ~CIGWB();

    //初始化函数
    int Init(const UINT4 nAPID = 0, const int bCfg = -1);

    //获得相应的接口指针
    void* QueryInterface(const IID iid);

    //有元类
    friend class IBillProcessorImp;
    friend class IQueryBrowseImp;
    friend class IFormatCfgImp;

protected:

    IFormatCfgImp*        m_pIFormatCfgImp;      //格式配置接口指针
    IBillProcessorImp*    m_pIBillProcessorImp;  //话单处理接口指针
    IQueryBrowseImp*      m_pIQueryBrowseImp;    //浏览查询接口指针
    CFormatLib*           m_pFmtLib;             //格式库对象指针
    LIST<CAccessPoint*>*  m_pAPs;                //接入点对象列表
    STRING                m_sFmtRootDir;         //格式库存放的根路径

};
/****************** End of The CIGWB class Defining *****************/

#endif  //!defined(_FORMAT_IGWB_H_)
