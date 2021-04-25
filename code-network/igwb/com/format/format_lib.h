#if !defined(_FORMAT_FORMAT_LIB_H_)
#define _FORMAT_FORMAT_LIB_H_
/**************** The Head Comment of This File *********************/
// Description:  The Head File of the CFormatLib
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "format_comm.h"
#include "ace/DLL.h"

class CFormat;

//格式名为与格式库映射关系类型定义
typedef  MAP<STRING,  CFormat*> FORMAT_MAP;

/****************** The CFormatLib class Defining *******************/
// 功能描述：格式库类
//
/********************************************************************/
class CFormatLib
{
public:

    //构造函数
    CFormatLib(const STRING& sRootDir);

    //析构函数
    ~CFormatLib();

    //初始化函数
    int Init(int bCfg = -1);

    //获得当前格式库中所有格式名
    UINT4 GetFormats(LIST<STRING>& FmtNamesList) const;

    //获得指定格式名的格式对象
    CFormat* GetFormat(const STRING& sFmtName, int nAP = -1);

    //增加一种新话单格式配置
    int AddFormat(const STRING& sFmtName);

    //删除一种话单格式配置
    int DelFormat(const STRING& sFmtName);

    //使上一次Commit到本次Commit调用之间所修改的配置信息保存到磁盘中
    int Commit(void);
    
    void SetMustMerge(BOOL bMustMerge);

protected:

    CFormat* CreateFormatObject(const char* szDllName,
                                const char* szRootDir, 
                                const char* szFmtName,
                                int nAP);

protected:

    STRING              m_sFmtRootDir;      //格式配置库的根目录
    FORMAT_MAP*         m_pFmtsMap;         //以格式名为索引的MAP指针
    
    BOOL                m_bIsMustMerge;

    MAP<int,ACE_DLL*>   m_mapAPToFmtSpDll;

    //接入点号到格式库特性动态库名的MAP
    MAP<int,STRING>     m_mapAPToFmtSpName;
};
/****************** End of The CFormatLib class Defining ************/


#endif  //!defined(_FORMAT_FORMAT_LIB_H_)
