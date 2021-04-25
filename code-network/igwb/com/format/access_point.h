#if !defined(_FORMAT_ACCESS_POINT_H_)
#define _FORMAT_ACCESS_POINT_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CAccessPoint
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "channel.h"

class CFormatLib;
class CFormat;


//以通道ID为索引的通道MAP类型定义
typedef MAP<UINT4, CChannel*> CHL_MAP;

typedef LIST<STRING> ORG_FMT_NAME_LIST;  //Added by ZhengYuqun 2004-01-18 SWPD03392

/****************** The CAccessPoint class Defining *****************/
// 功能描述：格式转换类
//
/********************************************************************/
class CAccessPoint
{
public:

    //构造函数
    CAccessPoint(const STRING& sFmtRootDir, const UINT4 uAPID, 
                 CFormatLib* pFmtLib = NULL);

    //析构函数
    ~CAccessPoint(void);

    //初始化接入点
    int Init(CFormatLib* pFmtLib = NULL);

    //获得接入点ID
    UINT4 GetAPID(void) const;

    //获得接入点名称
    void GetAPName(STRING& sAPName) const;

    //载入接入点配置文件此函数对外封装了接入点配置文件结构
    int LoadConfig(void);

    //added by lucy 2004-06-29
    int LoadBinaryConfig(const STRING& sAPFile);
    int LoadAsciiConfig(const STRING& sAPFile);
    int VerifyFileFmt(const STRING& sAPFile);
    //end add

    
    //获得原始话单格式长度
    UINT4 GetOrigLen(void) const;

    //获得当前接入点的指定通道的最终话单长度
    UINT4 GetFinalLen(UINT4 uChlID) const;
    
    //获得本接入点下所有通道名称，不包含缺省的0通道
    UINT4 GetChannelNames(LIST<STRING>& ChlNames) const;

    //获得本接入点的通道名为sChlName的通道的所有最终话单浏览格式名
    UINT4 GetFinalBrowseNames(const STRING& sChlName, LIST<STRING>& Names, 
                              LIST<STRING>& Descs) const;

    //获得本接入点下所有原始话单浏览格式名
    UINT4 GetOrigBrowseNames(LIST<STRING>& Names, LIST<STRING>& Descs) const;    

    //分拣一张话单，获得该话单所属的通道和格式处理对象
    void Distribute(const void* pABill, const UINT4 uLen, UINT4& uChlID, 
                    CFormat*& pDestFmt);

    void SpecDistribute(const void* pABill, const UINT4 uLen, VECTOR<UINT4>& uChlID, 
					VECTOR<CFormat*>& pDestFmt);

    //设置接入点名称
    void SetAPName(const STRING& sAPName);

    //创建一种接入点
    int Create(void);

    //追加本接入点的原始话单浏览格式名
    int AddBrowseNames(UINT4 nCount, const char* szFormats[]);

    //增加指定的通道
    int AddChannel(UINT4 nChlID, const STRING& sChlName);

    //设置指定通道的针对一种最终话单源格式的分拣条件
    int SetDistingCondition(UINT4 nChlID, const STRING& sDisFormat,
                            const STRING& sDisCond);

    //以追加的方式设置指定通道的最终话单格式名和最终话单浏览格式名
    int SetChlFormats(UINT4 nChlID, UINT4 nFinalFmtCount, const char* szFinalFormats[],
                      UINT4 nFinalBrowseFmtCount, const char* szFinalBrowseFmts[]);

    //指定通道编号，获得该通道所有的最终话单格式名和最终话单浏览格式名
    int GetChlFormats(const UINT4 nChlID, LIST<STRING>& FinalFmts,
                      LIST<STRING>& FinalBrowseFormats) const;

    //删除指定的通道
    int DelChannel(UINT4 nChlID);

    //追加本接入点的原始话单格式名
    int AddOrgFmtNames(UINT4 nCount, const char* szFormats[]);

    //获得本接入点下所有原始话单格式名
    UINT4 GetOrgFmtNames(LIST<STRING>& Names, LIST<STRING>& Descs) const;

    //使上一次Commit到本次Commit调用之间所修改的配置信息保存到磁盘中
    int Commit(void);

    //删除接入点
    int Delete(void);

protected:

    UINT4        m_uAPID;               //接入点ID
    STRING       m_sAPName;             //接入点名称
    STRING       m_sFmtRootDir;         //格式库配置根目录
    CHL_MAP*     m_pChannels;           //以通道ID为索引的MAP指针
    FMT_MAP*     m_pOrigFmtMaps;        //以格式名为索引的原始话单格式列表
    FMT_MAP*     m_pOrigBrowseFmtMaps;  //以格式名为索引键值的原始话单浏览格式列表
    int          m_nAPType;             //接入点类型
    CFormatLib*  m_pFmtLib;             //格式库对象，不得释放该指针
    ORG_FMT_NAME_LIST* m_pOrgFmtNameList; //存放浏览格式名的数组 Added by ZhengYuqun 2004-01-18 SWPD03392
};
/****************** End of The CAccessPoint class Defining **********/


#endif  //!defined(_FORMAT_ACCESS_POINT_H_)
