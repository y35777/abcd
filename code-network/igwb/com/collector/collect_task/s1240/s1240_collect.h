#ifndef __S1240_COLLECT_H__
#define __S1240_COLLECT_H__

#include "../collect_base.h"
#include "../../include/i_cmise_fun.h"

class CS1240TPCollect;

class CS1240Collect : public CCollectBase
{
public:
    CS1240Collect(int nSwitchIndex, 
                  CLinkGroup* pLinkGroup,
                  CS1240CmiseGroup* pCmiseGroup); //构造函数传入协议对象指针
    virtual ~CS1240Collect();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Col_Init();

    //拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现;
    virtual  int  Col_GetFile(SColFileReq& stReq);

    //获得文件列表, 是虚函数, 由派生类具体实现
    virtual  int  Col_GetList(SListOut& pOut);

    //动态创建子目录, 是虚函数, 由派生类具体实现
    virtual  BOOL CreateLocSubDir();

    //清除过期文件, 是虚函数, 由派生类具体实现
    virtual  BOOL ClearOldFile();

    //检查文件序列号是否连续，是虚函数, 由派生类具体实现
    virtual  BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);


    //测试连接服务端
    virtual  int  TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus);

    //设置告警门限
    virtual int  SetAlarmLimit(const char* const szTP, int nAlarmLimit);

    //指定的文件是否可采
    virtual int  CanGetSpecFile(const char* const szModule,
                                const char* const szSrcFile);

    //采集指定的文件
    virtual int  GetSpecFile(const char* const szModule,
                             const char* const szSrcFile, 
                             const char* const szDstFile);
    //获取交换机信息
    virtual int  GetSwitchInfo(LIST<STRING>& label,
                               LIST<STRING>& content);

    //获取模块信息
    virtual int  GetModuleInfo(const char* const szModule,
                               LIST<STRING>& label,
                               LIST<STRING>& content);
    //获取文件长度
    virtual int  GetFileLen(const char* const szModule,
                            const char* const szSrcFile);

protected:
    MAP<int, CS1240TPCollect*>  m_TPMap;
    CS1240CmiseGroup*           m_pCmiseGroup;
    SS1240SwitchInfo            m_SwitchInfo;          //服务端列表
};


#endif //__S1240_COLLECT_H__
