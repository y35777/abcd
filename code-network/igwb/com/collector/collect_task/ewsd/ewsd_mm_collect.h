// ewsd_mm_collect.h: interface for the CEWSD_MMCollect class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _EWSD_MM_COLLECT_H_
#define _EWSD_MM_COLLECT_H_

#include "../collect_base.h"

class CEWSD_MMCollect : public CCollectBase
{
public:
    CEWSD_MMCollect(int nSwitchIndex, CLinkGroup* pLinkGroup);
    virtual ~CEWSD_MMCollect();
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

    int GetFileListIndex(const char* const szSrcFile);

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
    BOOL GetSubName(const char *sInfoLine,
                    SFileInfo& szFileInfo );

    STRING GetDstFileName(const char* const szSrcFile);

    //话单存放的根目录
    STRING m_strLocalRootPath;
    //话单存放的当前目录
    STRING m_strLocalPath;

    //指示文件存放的根目录
    STRING m_strFNARootPath;
    //指示文件存放的当前目录
    STRING m_strFNAPath;
    STRING m_strFnaFile;
    STRING m_strFnaIndex;
    STRING m_strBillIndex;

    //临时话单文件存放的目录
    STRING m_strTmpPath;
    //远端5ESS交换机的源目录
    STRING m_strRemotePath;

    //错误信息
    STRING m_strError;

    //当前时间
    STRING m_strCurDate;

    char m_szAreaCode[50]; //区号
    char m_szBillType[50]; //话单类型

    VECTOR <SFileInfo> m_FileList;

    ACE_Mutex          m_MutexForList;
};

#endif // _EWSD_MM_COLLECT_H_
