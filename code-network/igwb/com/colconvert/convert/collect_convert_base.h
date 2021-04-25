#ifndef __COLLECT_CONVERT_BASE_H__
#define __COLLECT_CONVERT_BASE_H__

#include "include/collect_convert_struct.h"
#include "../include/i_collect_convert.h"
#include "../include/collect_convert_include.h"
#include "collect_convert_subobj.h"

/*
CCollectConvertBase是采集操作的基类, 它定义了采集操作相关的基本属性和基本方法
*/
class CIGWB;
class IBillProcessor;

//以下是采集操作的基类CCollectConvertBase的定义
class  CCollectConvertBase : public ICollectConvert
{

public:
    CCollectConvertBase(int nSwitchIndex); //构造函数传入协议对象指针
    virtual ~CCollectConvertBase();

    //初始化, 是虚函数, 由派生类具体实现
    virtual  int  Init();

    virtual  int  Process(const char* szBillFile, 
                          const int& nConvertInfo);

    virtual  int  Convert(SConvertFileReq& stReq);


    //动态创建子目录, 是虚函数, 由派生类具体实现
    virtual  int CreateLocSubDir();

    //added by chenliangwei
    //清除指定目录下的过期子目录，供派生类使用的工具函数
    static int ClearOldDateSubDir(const char* szRootDir, const int nSaveDays);

    //得到本地指定目录下的文件列表
    static int GetLocalFileList(const char* szSrcDir, SListOut &pOut);
    //added end

    //清除过期文件, 是虚函数, 由派生类具体实现
    virtual  int ClearOldFile();

    inline void SetCfgPath(const char* const szCfgPath)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }

protected:

    MAP<int, CCollectConvertSubObj*>  m_SubObjMap;


    int   m_nSwitchIndex;
    char  m_szSwitchName[40];       //交换机名
    char  m_szCollectId[40];        //指定具体的采集对象，如"5ESS"，用于创建C5ESSCollect对象

    char  m_szCfgPath[MAX_PATH];    //当前配置文件名，含路径信息

    UINT4  m_nAPID;

    CIGWB* m_pIGWB;
    IBillProcessor* m_pIBillProcessor ;

};

#endif //__COLLECT_CONVERT_BASE_H__

