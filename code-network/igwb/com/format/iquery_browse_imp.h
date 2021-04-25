#if !defined(_FORMAT_IQUERY_BROWSE_IMPLEMENT_H_)
#define _FORMAT_IQUERY_BROWSE_IMPLEMENT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IQueryBrowseImp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "iquery_browse.h"

class CFormat;
class CIGWB;

/****************** The IQueryBrowseImp Interface Defining **************/
// 功能描述：话单查询浏览接口实现
//
/************************************************************************/
class IGWB_Svc_Export IQueryBrowseImp: public IQueryBrowse
{
public:

    //构造函数
    IQueryBrowseImp(CIGWB* pIGWB);

    //析构函数
    virtual ~IQueryBrowseImp();

    //话单浏览查询接口初始化
    virtual int Init(const UINT4 nAPID = 0);

    //获得格式库配置的所有接入点名称
    virtual UINT4 GetAPNames(LIST<STRING>& APNames) const;

    //获得名为sAPName的接入点的所有通道名称
    virtual UINT4 GetChlNames(const STRING& sAPName, LIST<STRING>& ChlNames) const;

    //获得名称为sAPName的接入点的所有原始话单浏览格式名及对应的格式描述
    virtual UINT4 GetOrigBrowseNames(const STRING& sAPName, LIST<STRING>& Names,
                                     LIST<STRING>& Descs) const;

    //输入一个接入点名称，获得某个通道下所有最终话单浏览格式名及对应的格式描述
    virtual UINT4 GetFinalBrowseNames(const STRING& sAPName, const STRING& sChlName,
                                      LIST<STRING>&  Names, LIST<STRING>& Descs) const;

    //获得名称为sBrowseName的浏览格式的格式处理对象
    virtual CFormat* GetBrowseFmt(const STRING& sBrowseName);

protected:

    CIGWB*   m_pIGWB;    //CIGWB对象指针

};
/*************** End of The IQueryBrowseImp Interface Defining **********/

#endif  //!defined(_FORMAT_IQUERY_BROWSE_IMPLEMENT_H_)
