#ifndef _EWSD_AMA_COLLECT_H_
#define _EWSD_AMA_COLLECT_H_

#include "../collect_base.h"


class CEWSD_AMACollect
{
public:
    CEWSD_AMACollect(CLinkMgr *pLinkMgr,
                   int nSwitchIndex,
                   int nAMAIndex);
    virtual ~CEWSD_AMACollect();

    //初始化采集服务
    int  Init();

    //拷贝文件服务端的单个文件到本地;
    int  GetFile(SColFileReq& stReq);


    //在文件列表中查找指定文件
    int  FindFileInList(const char* const szFileName);

    //获得文件列表
    int  GetList(SListOut& pOut);


    //动态创建子目录
    BOOL CreateLocSubDir();

    //清除过期文件
    BOOL ClearOldFile();

    //检查文件序列号是否连续
    BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);


    inline void SetCfgPath(const char* const szCfgPath)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }

    inline void SetFileSaveDays(const int nFileSaveDays)
    {
        m_nFileSaveDays = nFileSaveDays;
    }

    inline void SetSwitchName(const char* const szSwitchName)
    {
        strncpy(m_szSwitchName, szSwitchName, sizeof(m_szSwitchName));
        m_szSwitchName[sizeof(m_szSwitchName) - 1] = '\0';
    }

    //指定的文件是否可采
    virtual int  CanGetSpecFile(const char* const szSrcFile);
    //采集指定的文件
    virtual int  GetSpecFile(const char* const szSrcFile,
                             const char* const szDstFile);
    //获取模块信息
    virtual int  GetModuleInfo(LIST<STRING>& label,
                               LIST<STRING>& content);

    //获取文件长度
    virtual int  GetFileLen(const char* const szSrcFile);

    inline const char* GetModuleLabel()
    {
        return m_strModuleLabel.c_str();
    }

    int MakeFileNameMsg(const char* const szSrcFile,
                        SGetFileReq& req);

    int DoGetAFile(SGetFileReq& stReq);
protected:

    int LoadStatus();

    int SaveStatus();

    int IncreaseFileSN();

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;


    STRING GetDstFileName(const char* const szSrcFile);

    //要采集的EWSD交换机的AMA文件名
    STRING m_strAMAFile;

    //AMA文件的话单类型
    STRING m_strBillType;

    //AMA文件的文件序列号
    UINT4  m_nFileSN;

    int    m_nFileSNLen;
    UINT4  m_nFileSNLimit;

    char m_szAreaCode[50]; //区号
    char m_szBillType[50]; //话单类型


    int                   m_nSwitchIndex;
    int                   m_nAMAIndex;

    //话单存放的根目录
    STRING m_strLocalRootPath;
    //话单存放的当前目录
    STRING m_strLocalPath;

    //临时话单文件存放的目录
    STRING m_strTmpPath;

    //远端交换机的源目录
    STRING m_strRemotePath;

    //话单文件保存天数
    int    m_nFileSaveDays;   //added by chenliangwei


    CLinkMgr*               m_pLinkMgr;


    VECTOR<SFileInfo>       m_FileList;

    char  m_szCfgPath[MAX_PATH];    //当前配置文件名，含路径信息

    char  m_szSwitchName[40];       //交换机名

    //是否进行文件长度校验
    BOOL m_bFileLenVerify;

    //模块标识名
    STRING      m_strModuleLabel;

    ACE_Mutex   m_MutexForList;
};


#endif //_EWSD_AMA_COLLECT_H_


