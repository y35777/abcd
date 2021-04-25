// 5ess_collect.h: interface for the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _5ESS_COLLECT_H_
#define _5ESS_COLLECT_H_

#include "../collect_base.h"

//added by chenliangwei
#define PREFIX_AMA_FILE     "stream"
#define LEN_OF_AMA_FILE_SN  4

class CFileSNMgr;
//added end

class C5ESSCollect : public CCollectBase
{
public:
    C5ESSCollect(int nSwitchIndex, CLinkGroup* pLinkGroup);
    virtual ~C5ESSCollect();
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

    //added by chenliangwei
    //检查文件序列号是否连续，并处理漏采文件，是虚函数, 由派生类具体实现
    virtual  BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);
    //added end

    //指定的文件是否可采
    int CanGetSpecFile(const char* const szModule,
                       const char* const szSrcFile);
    //采集指定的文件
    virtual  int  GetSpecFile(const char* const szModule,
                              const char* const szSrcFile,
                              const char* const szDstFile);
    //获取交换机信息
    virtual  int  GetSwitchInfo(LIST<STRING>& label,
                                LIST<STRING>& content);

    //获取模块信息
    virtual  int  GetModuleInfo(const char* const szModule,
                                LIST<STRING>& label,
                                LIST<STRING>& content);

    //获取文件长度
    virtual int  GetFileLen(const char* const szModule,
                            const char* const szSrcFile);

    int MakeFileNameMsg(const char* const szSrcFile,
                        SGetFileReq& req);

    int GetFile(SGetFileReq& stReq);

    //文件管理器的回调函数，通过文件的序列号计算漏采文件名
    static STRING GetFileNameBySN(const int   nFileSN,
                                  const char* szReferName,
                                  const int   nSNLen,
                                  const int   nPostfixLen);


protected:

    BOOL GetSubName(const char *sInfoLine,
                    SFileInfo& szFileInfo );

    int GetPrefixIndex(const char* const szSrcFile);
    int GetFileListIndex(const char* const szSrcFile);

    STRING GetDstFileName(const char* const szSrcFile);

    //多种话单存放的详细目录名
    VECTOR<STRING> m_vBillSavePath;
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

    //临时话单文件存放的目录
    STRING m_strTmpPath;
    //远端5ESS交换机的源目录
    STRING m_strRemotePath;

    //文件前缀的数组
    VECTOR<STRING> m_vFilePrefix;

    //错误信息
    STRING m_strError;

    //当前时间
    STRING m_strCurDate;

    VECTOR<STRING> m_vAreaCode; //区号
    VECTOR<STRING> m_vBillType; //话单类型
    VECTOR<STRING> m_vLabel;    //标识

    //added by chenliangwei
    int m_bCheckSN;

    CFileSNMgr* m_pFileSNMgr;

    //比较函数类，用于对fna中的文件信息进行排序
    class Compare
    {
    public:
        BOOL operator ()(const SFileInfo &lfileinfo, const SFileInfo &rfileinfo);
    };//定义结束

    //added end

    VECTOR <SFileInfo> m_FileList;

    ACE_Mutex          m_MutexForList;
};

#endif // _5ESS_COLLECT_H_
