#ifndef __S1240_TP_COLLECT_H__
#define __S1240_TP_COLLECT_H__

#include "../collect_base.h"
#include "../../include/i_cmise_fun.h"

#define S1240_DS_DUMPED         "dumped"
#define S1240_DS_RELEASED       "released"

#define S1240_FILE_SN_LIMIT      200
#define S1240_FILE_SN_LEN        3
#define BUF_SIZE                 256

#define S1240_FILE_PREFIX        "TP-FILE-"

class CFileSNMgr;


//SS1240FileStatus保存每个S1240交换机的文件信息
struct SS1240FileStatus
{
    SS1240FileMsg file_msg;     //文件目前的状态信息
    int  nLastFileCounter;      //上次采集时的文件计数器号
    int  nLastSetStatus;        //本程序上次设置的状态
    int  nFinalStatus;          //目标状态(-1:null|dumped|release)
    int  nStatus;               //是否正在采集的状态 E_COLLECT_STATUS
    BOOL bAdditional;           //是否是附加要求采集的文件
    char szDstFile[MAX_PATH];   //目标文件名
};


class CS1240TPCollect
{
public:
    CS1240TPCollect(CLinkMgr *pPriLinkMgr,
                    CLinkMgr *pBakLinkMgr,
                    CS1240CmiseMgr* pCmiseMgr,
                    int nSwitchIndex,
                    int nTPIndex);
    virtual ~CS1240TPCollect();

    //初始化采集服务
    int  Init();

    const SS1240TPInfo& GetTPInfo()
    {
        return m_TPInfo;
    }

    int MakeFileNameMsg(const char* const szSrcFile,
                        SGetFileReq& req);

    //取可采集文件列表
    int GetList(VECTOR<SS1240FileMsg>& pOut);

    //拷贝文件服务端的单个文件到本地，包括文件状态迁移;
    int  GetFile(SColFileReq& stReq);

    //拷贝文件服务端的单个文件到本地，只是取文件;
    int  DoGetAFile(SColFileReq& stReq);

    //获得文件列表
    int  GetAllFileList();

    //在文件列表中查找指定文件
    int  FindFileInList(const char* const szFileName);

    //获得所有文件列表
    int  GetAllList(MAP<STRING, SS1240FileStatus>& Out);

    //获得alarmlimit
    int  GetFilePool(SS1240FilePoolMsg& pool_msg);

    //获得alarmlimit
    int  GetAlarmLimit();

    //设置alarmlimit
    int  SetAlarmLimit(int nAlarmLimit);

    //动态创建子目录
    BOOL CreateLocSubDir();

    //清除过期文件
    BOOL ClearOldFile();

    //检查文件序列号是否连续
    BOOL CheckFileSN(const char* szFileName, char* szPreviousFile);


    void SetCfgPath(const char* const szCfgPath);

    void SetFileSaveDays(int nFileSaveDays);

    inline void SetSwitchName(const char* const szSwitchName)
    {
        strncpy(m_szSwitchName, szSwitchName, sizeof(m_szSwitchName));
        m_szSwitchName[sizeof(m_szSwitchName) - 1] = '\0';
    }

    //测试连接服务端
    virtual int  TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus);

    //指定的文件是否可进行手动采集
    virtual int  CanGetSpecFile(const char* const szSrcFile);

    //指定的文件是否可进行自动采集
    virtual int  CanAutoGet(const char* const szFileName);

    //获取文件长度
    virtual int  GetFileLen(const char* const szSrcFile);

    //文件管理器的回调函数，通过文件的序列号计算漏采文件名
    static STRING GetFileNameBySN(const int   nFileSN,
                                  const char* szReferName,
                                  const int   nSNLen,
                                  const int   nPostfixLen);

protected:

    int LoadStatus();

    int SaveStatus();

    STRING GetDstFileName(const char* const szSrcFile);

    int RestoreOperatingFile(const char* const szFileName);

    CLinkMgr*               m_pPriLinkMgr;
    CLinkMgr*               m_pBakLinkMgr;
    CS1240CmiseMgr*         m_pCmiseMgr;
    int                     m_nSwitchIndex;
    int                     m_nTPIndex;
    int                     m_nCoFailAlarmLimit;

    SS1240TPInfo            m_TPInfo;

    MAP<STRING, SS1240FileStatus>   m_FileList;
    ACE_Recursive_Thread_Mutex      m_MutexForMap;
    int                             m_nCollectingFile;

    char  m_szCfgPath[MAX_PATH];    //当前配置文件名，含路径信息

    //话单存放的当前目录
    char  m_szLocalPath[MAX_PATH];

    //临时话单文件存放的目录
    STRING m_strTmpPath;

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;

    //调试用的远端目录
    char  m_szRemotePath[MAX_PATH];

    //话单文件保存天数
    int   m_nFileSaveDays;   //added by chenliangwei
    char  m_szSwitchName[40];       //交换机名

    int   m_bCheckSN;

    CFileSNMgr* m_pFileSNMgr;

    int   m_nAlarmLimit;

    class CTPFileCompare
    {
    public:
        BOOL operator ()(const SS1240FileMsg &lfileinfo, const SS1240FileMsg &rfileinfo)
        {
            //如果不是AMA文件，则放到列表的尾部
            if (strcmp(rfileinfo.szFileId, lfileinfo.szFileId) > 0)
            {
                return TRUE;
            }

            return FALSE;
        }
    };

    //是否进行文件长度校验
    BOOL m_bFileLenVerify;
};


#endif //__S1240_TP_COLLECT_H__


