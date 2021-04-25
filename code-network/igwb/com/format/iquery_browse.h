#if !defined(_FORMAT_IQUERY_BROWSE_H_)
#define _FORMAT_IQUERY_BROWSE_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IQueryBrowse
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormat;

/****************** The IQueryBrowse Interface Defining *****************/
// 功能描述：话单查询浏览接口
//
/************************************************************************/
class IGWB_Svc_Export IQueryBrowse
{
public:

    //纯虚析构函数
    virtual ~IQueryBrowse(){};

    //话单浏览查询接口初始化
    virtual int Init(const UINT4 nAPID = 0) = 0;

    //获得格式库配置的所有接入点名称
    virtual UINT4 GetAPNames(LIST<STRING>& APNames) const = 0;

    //获得名为sAPName的接入点的所有通道名称
    virtual UINT4 GetChlNames(const STRING& sAPName, LIST<STRING>& ChlNames) const = 0;

    //获得名称为sAPName的接入点的所有原始话单浏览格式名及对应的格式描述
    virtual UINT4 GetOrigBrowseNames(const STRING& sAPName, LIST<STRING>& Names,
                                     LIST<STRING>& Descs) const = 0;

    //输入一个接入点名称，获得某个通道下所有最终话单浏览格式名及对应的格式描述
    virtual UINT4 GetFinalBrowseNames(const STRING& sAPName, const STRING& sChlName,
                                      LIST<STRING>&  Names, LIST<STRING>& Descs) const = 0;

    //获得名称为sBrowseName的浏览格式的格式处理对象
    virtual CFormat* GetBrowseFmt(const STRING& sBrowseName) = 0;

};
/****************** End of The IQueryBrowse Interface Defining **********/

#endif  //!defined(_FORMAT_IQUERY_BROWSE_H_)
