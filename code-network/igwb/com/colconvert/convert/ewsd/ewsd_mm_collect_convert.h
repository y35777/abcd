#ifndef __EWSD_MM_COLLECT_CONVERT_H__
#define __EWSD_MM_COLLECT_CONVERT_H__

/*
CEWSDAMASCollectConvert是ewsdama采集操作类, 它定义了采集操作相关的基本属性和基本方法
*/
#include "../../include/collect_convert_include.h"
#include "../include/collect_convert_struct.h"

#include "../collect_convert_base.h"

//以下是采集操作的类CEWSDAMASCollectConvert的定义
class  CEWSDMMCollectConvert : public CCollectConvertBase
{

public:
    CEWSDMMCollectConvert(int nSwitchIndex); //构造函数传入协议对象指针
    virtual ~CEWSDMMCollectConvert();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Init() ;

    virtual  int  Convert(SConvertFileReq& stReq);


    //动态创建子目录, 是虚函数, 由派生类具体实现
    virtual  int CreateLocSubDir();

    //清除过期文件, 是虚函数, 由派生类具体实现
    virtual  int ClearOldFile();

protected:
    virtual  STRING GetDstFileName(const char* szSrcName);

    virtual  STRING GetErrFileName(const char* szSrcName);

    CONVERTFILEFUN  m_pConvertFun;  //转换函数指针


    char m_szFinalRootPath[MAX_PATH];      //最终话单根目录
    char m_szFinalPath[MAX_PATH];          //最终话单目录
    char m_szSecondFinalPath[MAX_PATH];    //最终话单第二份的目录


    char m_szBillType[CFG_BUF_SIZE];       //话单类型

    int   m_nFileSaveDays;        //话单文件保存天数


    int  m_bFileLevelVerify;       //是否进行文件级校验
    int  m_bFileLevelFailReCol;    //文件级校验失败是否重采
    int  m_bBillLevelVerify;       //是否进行话单级校验
    int  m_bBillLevelFailReCol;    //话单级校验失败是否重采

};

#endif //__EWSD_MM_COLLECT_CONVERT_H__

