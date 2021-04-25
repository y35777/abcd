#if !defined(_FORMAT_IFORMAT_CONFIG_IMPLEMENT_H_)
#define _FORMAT_IFORMAT_CONFIG_IMPLEMENT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the IFormatCfgImp
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "iformat_cfg.h"

class CIGWB;

/****************** The IFormatCfgImp Interface Defining ************/
// 功能描述：格式配置接口实现
//
/********************************************************************/
class IGWB_Svc_Export IFormatCfgImp: public IFormatCfg
{
public:

    //构造函数
    IFormatCfgImp(CIGWB  *pIGWB);

    //析构函数
    virtual ~IFormatCfgImp();

    //根据格式库配置文件初始化格式配置接口
    virtual int Init(const UINT4 nAPID = 0);

    //在格式库中增加一种名字为sFmtName的话单格式
    virtual int AddFormat(const STRING& sFmtName);

    //指定一种格式名sFmtName，设置或修改其格式描述sFmtDes
    //和对应的源格式名sSrcFmtName
    virtual int SetFormatGen(const STRING& sFmtName, const STRING& sFmtDes, 
                             const STRING& sSrcFmtName);

    //获得指定格式名sFmtName的所有的话单域信息MAP指针
    virtual const FIELD_MAP* GetFieldsMap(const STRING& sFmtName);

    //获得指定格式名sFmtName的所有的话单域信息SFieldInfo*数组指针
    virtual const SFieldInfo** GetFieldsArray(const STRING& sFmtName);

    //设置指定格式名为sFmtName的格式库的格式识别条件sDistingCondition
    virtual void SetDistingCondition(const STRING& sFmtName, 
                                     const STRING& sDistingCondition);

    //获得指定格式名为sFmtName的格式库的格式识别条件，
    //并将识别条件存入sDistingCondition
    virtual void GetDistingCondition(const STRING& sFmtName, 
                                     STRING& sDistingCondition);

    //删除指定格式名为sFmtName的格式库
    virtual int DelFormat(const STRING& sFmtName);

    //获得格式库中所有格式名
    virtual UINT4 GetFormats(LIST<STRING>& Formats);

    //获得指定格式名的格式库的一般信息，即获得该格式的描述和源格式名
    virtual void GetFormatGen(const STRING& sFmtName, STRING& sFmtDes, 
                              STRING& sSrcFmtName);

    //指定接入点的ID、接入点名称，增加一个接入点的格式配置信息
    virtual int AddAccessPoint(const UINT4 nAPID, const STRING& sAPName);

    //指定接入点的ID，追加该接入点的原始话单浏览格式名
    virtual int AddOrgBrowseNames(const UINT4 nAPID, const UINT4 nCount, 
                                  const char* szFormats[]);

    //指定接入点的ID，获得该接入点所有原始话单浏览格式名
    virtual UINT4 GetOrgBrowseNames(const UINT4 nAPID, LIST<STRING>& Formats);

    //指定接入点的ID，通道ID和名称，增加指定的通道
    virtual int AddChannel(const UINT4 nAPID, const UINT4 nChlID , 
                           const STRING& sChlName);

    //指定接入点的ID、通道编号，设置该通道的针对一种最终话单源格式的分拣条件
    virtual int SetDisCondition(const UINT4 nAPID, const UINT4 nChlID , 
                                const STRING& sDisFormat,
                                const STRING& sDisCondition);

    //指定接入点ID、通道编号，以追加的方式设置该通道的最终话单格式名
    //和最终话单浏览格式名
    virtual int SetChlFormats(const UINT4 nAPID, const UINT4 nChlID , 
                              const UINT4 nFinalFmtCount, 
                              const char* szFinalFormats[], 
                              const UINT4 nFinalBrowseFmtCount, 
                              const char* szFinalBrowseFmts[]);

    //指定接入点ID、通道编号，获得该通道所有的最终话单格式名和最终话单浏览格式名
    virtual int GetChlFormats(const UINT4 nAPID, const UINT4 nChlID,
                              LIST<STRING>& FinalFmts,
                              LIST<STRING>& FinalBrowseFormats);

    //指定接入点ID、通道编号，删除该通道的配置信息
    virtual int DelChannel(const UINT4 nAPID, const UINT4 nChlID);

    //指定接入点ID，获得该接入点的所有通道名称
    virtual UINT4 GetChannels(const UINT4 nAPID, LIST<STRING>& ChlNames);

    //获得格式库所有接入点名称
    virtual UINT4 GetAPNames(LIST<STRING>& APNames);

    //使上一次Commit到本次Commit调用之间所修改的配置信息保存到磁盘中
    virtual int Commit(void);

    //指定接入点的ID，追加该接入点的原始话单格式名
    virtual int AddOrgFmtNames(const UINT4 nAPID, const UINT4 nCount, 
                               const char* szFormats[]);

    //指定接入点的ID，获得该接入点所有原始话单格式名
    virtual UINT4 GetOrgFmtNames(const UINT4 nAPID, LIST<STRING>& Formats);

    //删除指定接入点的格式配置信息
    virtual int DelAccessPoint(UINT4 nAPID);

protected:

    CIGWB*  m_pIGWB;		//CIGWB对象指针

};
/*************** End of The IFormatCfgImp Interface Defining ********/

#endif  //!defined(_FORMAT_IFORMAT_CONFIG_IMPLEMENT_H_)
