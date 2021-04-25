#ifndef __COLLECT_CONVERT_SUBOBJ_H__
#define __COLLECT_CONVERT_SUBOBJ_H__

/*
CCollectConvertSubObj是采集操作子对象的基类, 它定义了采集操作相关的基本属性和基本方法
*/
#include "../include/collect_convert_include.h"
#include "include/collect_convert_struct.h"
#include "include/collect_convert_define.h"

class IBillProcessor;

//以下是采集操作的基类CCollectConvertBase的定义
class  CCollectConvertSubObj 
{

public:
    CCollectConvertSubObj(int nSwitchIndex, 
                          int nSubObjIndex,
                          IBillProcessor* pIBillProcessor = NULL); //构造函数传入协议对象指针

    virtual ~CCollectConvertSubObj();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;


    virtual  int  Convert(SConvertFileReq& stReq);

    //动态创建子目录, 是虚函数, 由派生类具体实现
    virtual  int CreateLocSubDir();


    //清除过期文件, 是虚函数, 由派生类具体实现
    virtual  int ClearOldFile();


    //构建目标文件的路径
    virtual void ConstuctDestFileLst(SConvertFileReq& stReq);

    //把临时目录下的文件移动到正式目录下
    virtual int RenameDestFile();

    //拷贝第二份文件
    virtual int CopySecondFile();

    virtual int ConvertFile(SConvertFileReq& stReq);

    virtual void OnFileConvertSucc(int nChannel);

    inline void SetCfgPath(const char* const szCfgPath)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }


    inline void SetSwitchName(const char* const szSwitchName)
    {
        strncpy(m_szSwitchName, szSwitchName, sizeof(m_szSwitchName));
        m_szSwitchName[sizeof(m_szSwitchName) - 1] = '\0';
    }


protected:

    virtual  STRING GetDstFileName(const char* szSrcName);

    virtual  STRING GetErrFileName(const char* szSrcName);

    CONVERTFILEFUN  m_pConvertFun;  //转换函数指针



    char  m_szSwitchName[40];       //交换机名
    char  m_szCollectId[60];        //指定具体的采集对象，如"5ESS"，用于创建C5ESSCollect对象
    int   m_nSwitchIndex;           //交换机索引
    int   m_nSubObjIndex;           //子对象索引
    char  m_szSubObjName[40];       //子对象名称

    char  m_szCfgPath[MAX_PATH];    //当前配置文件名，含路径信息

    
    char m_szFinalRootPath[MAX_PATH];      //最终话单根目录
    char m_szFinalPath[MAX_PATH];          //最终话单目录
    char m_szSecondFinalPath[MAX_PATH];    //最终话单第二份的目录


    char m_szBillType[CFG_BUF_SIZE];       //话单类型

    int   m_nFileSaveDays;        //话单文件保存天数


    int  m_bFileLevelVerify;       //是否进行文件级校验
    int  m_bFileLevelFailReCol;    //文件级校验失败是否重采
    int  m_bBillLevelVerify;       //是否进行话单级校验
    int  m_bBillLevelFailReCol;    //话单级校验失败是否重采

    int  m_bCrtZeroFnlFile;        //是否生成0字节的最终话单文件
    int  m_nOrigBillLen;           //原始话单的长度

    char m_szCurDate[24];
    IBillProcessor* m_pIBillProcessor;
    LIST<STRING> m_ChlNames;

    VECTOR<STRING> m_FinalDirList;
    VECTOR<STRING> m_SecondDirList;

    //是否需要复制第二份最终话单文件
    BOOL m_bSecondCopy;

    //是否需要第二份话单适配
    BOOL m_bNeedSecondAdapter;

    VECTOR<STRING> m_FinalFileList;
    VECTOR<int>    m_CvtResultList;

};

#endif //__S1240_TP_COLLECT_CONVERT_H__

