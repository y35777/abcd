// com_collect.h: interface for the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __EWSD_AMAS_COLLECT_H_
#define __EWSD_AMAS_COLLECT_H_

#include "../collect_base.h"

class CEWSD_AMACollect;
class CLinkMgr;

//中兴交换机的采集类
class CEWSD_AMASCollect : public CCollectBase
{
public:
    CEWSD_AMASCollect(int nSwitchIndex, CLinkGroup* pLinkGroup);
    virtual ~CEWSD_AMASCollect();

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

    //检查文件序列号是否连续，并处理漏采文件，是虚函数, 由派生类具体实现
    virtual  BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);

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

    CLinkMgr* m_pLinkMgr;

    //错误信息
    STRING m_strError;

    VECTOR<CEWSD_AMACollect*>   m_AMAColList;
};

#endif // __EWSD_AMAS_COLLECT_H_
