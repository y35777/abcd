#if !defined(_FORMAT_CHANNEL_H_)
#define _FORMAT_CHANNEL_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CChannel
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"

class CFormat;
class CDisCondition;
class CFormatLib;

//格式名与格式对象对应关系结构定义
typedef struct _FmtMap
{
	STRING      sFmtName;	 //格式名
	CFormat*    pFmt;		 //格式对象
} SFmtMap, *PFmtMap;

//格式名与格式对象对应关系类型定义
typedef MAP<STRING, SFmtMap*> FMT_MAP;

typedef LIST<STRING> FINAL_FMT_NAME_LIST; //Added by ZhengYuqun 2004-01-18 SWPD03392

/****************** The CChannel class Defining *********************/
// 功能描述：通道类
//
/********************************************************************/
class CChannel
{
public:

    //构造函数
    CChannel(const STRING& sFmtRootDir, const UINT4 uAPID, const UINT4 uChlID,
             CFormatLib *pFmtLib = NULL);

    //析构函数
    ~CChannel();

    //初始化
    int Init(CFormatLib *pFmtLib = NULL);

    //将本通道的配置载入内存
    int LoadConfig(void);

    //获得当前通道的最终话单长度
    UINT4 GetFinalLen() const;

	int LoadBinaryConfig(const STRING& sChlFile);
	int LoadAsciiConfig(const STRING& sChlFile);
	int VerifyFileFmt(const STRING& sChlFile);

    //获得通道的名称
    void GetChlName(STRING&  sChlName) const;

    //获得通道的ID
    UINT4 GetChlID(void) const;

    //获得本通道所有最终话单浏览格式名
    UINT4 GetFinalBrowseNames(LIST<STRING>& Names, LIST<STRING>& Descs) const;

    //判断传入的一张话单pABill数否属于本通道
    int IsTheChannel(const void* pABill, const UINT4 uLen, CFormat*& pDestFmt);

    //设置通道名称
    void SetChlName(const STRING& sChlName);

    //创建一个新的通道
    int Create(void);

    //设置本通道的针对一种最终话单源格式的分拣条件
    int SetDistingCondition(const STRING& sDisFormat, const STRING& sDisCond);

    //以追加的方式设置本通道的最终话单格式名和最终话单浏览格式名
    int AddChlFormats(UINT4 nFinalFmtCount, const char* szFinalFormats[],
                      UINT4 nFinalBrowseFmtCount, const char* szFinalBrowseFmts[]);

    //获得本通道所有的最终话单格式名和最终话单浏览格式名
    int GetChlFormats(LIST<STRING>& FinalFmts, LIST<STRING>& FinalBrowseFormats) const;

    //删除通道
    int Delete(void);

    //使上一次Commit到本次Commit调用之间所修改的配置信息保存到磁盘中
    int Commit(void);


protected:

    UINT4           m_uChlID;		        //通道ID
    UINT4           m_uAPID;		        //本通道所属的接入点ID
    STRING          m_sChlName;		        //通道名称
    STRING          m_sFmtRootDir;          //格式库根目录路径
    CDisCondition*  m_pDisCondition;        //本通道对应的分拣条件对象
    CFormatLib*     m_pFmtLib;              //格式库对象
    FMT_MAP*        m_pFinalFmtMaps;        //本通道对应的最终话单格式对象列表
    FMT_MAP*        m_pFinalBrowseFmtMaps;  //本通道对应的最终话单浏览格式列表
    
    FINAL_FMT_NAME_LIST* m_pFinalFmtNameList; //存放浏览格式名的数组 Added by ZhengYuqun 2004-01-18 SWPD03392    
};

/****************** End of The CChannel class Defining **************/


#endif  //!defined(_FORMAT_CHANNEL_H_)
