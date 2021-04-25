#ifndef __COM_DIR_COLLECT_H__
#define __COM_DIR_COLLECT_H__

#include "../collect_base.h"

class CFileSNMgr;

class CComDirCollect
{
public:
    CComDirCollect(CLinkMgr *pLinkMgr,
                   int nSwitchIndex,
                   int nDirIndex);
    virtual ~CComDirCollect();

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

    inline const char* GetModuleLabel()
    {
        return m_strModuleLabel.c_str();
    }

    //指定的文件是否可采
    virtual int  CanGetSpecFile(const char* const szSrcFile);

    //采集指定的文件
    virtual  int  GetSpecFile(const char* const szModule,
                              const char* const szSrcFile,
                              const char* const szDstFile);

    //获取模块信息
    virtual int  GetModuleInfo(LIST<STRING>& label,
                               LIST<STRING>& content);

    int MakeFileNameMsg(const char* const szSrcFile,
                        SGetFileReq& req);

    //获取文件长度
    virtual int  GetFileLen(const char* const szSrcFile);

    int DoGetAFile(SGetFileReq& stReq);
protected:


    int LoadStatus();

    int SaveStatus();

    STRING GetDstFileName(const char* const szSrcFile);

    int                   m_nSwitchIndex;
    int                   m_nDirIndex;

    char m_szCollectId[40];

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
    
    int 	m_nForceColAllFile; //add by yangguang 采集同名的所有文件

    int    m_nMode;

    CLinkMgr*               m_pLinkMgr;


    VECTOR<SFileInfo>       m_FileList;

    char  m_szCfgPath[MAX_PATH];    //当前配置文件名，含路径信息

    char  m_szSwitchName[40];       //交换机名

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;


    int m_bCheckSN;

    CFileSNMgr* m_pFileSNMgr;

    int  m_nFileSNLen;

    int  m_nFilePostfixLen;

    int  m_nFileSNDownLimit;
    int  m_nFileSNLimit;
    //是否进行文件长度校验
    BOOL m_bFileLenVerify;

    //是否在采集后删除交换机文件
    BOOL m_bDelRemoteFile;

    //是否采集交换机上的最后一个文件
    BOOL m_bCollectLastFile;

    //交换机上文件后缀名
    LIST<STRING> m_FilePostfixList;

    //后缀名过滤模式
    int m_nPostfixFltMode;

    //交换机上文件前缀名
    LIST<STRING> m_FilePrefixList;

    //前缀名过滤模式
    int m_nPrefixFltMode;

    //模块标识名
    STRING  m_strModuleLabel;

    ACE_Mutex          m_MutexForList;

	class Compare
	{
	public:
        BOOL operator ()(const SFileInfo &lhs, const SFileInfo &rhs)
        {
            return strcmp(lhs.szFileName,rhs.szFileName) < 0;
            /*
            if (strcmp(lhs.szModTime,rhs.szModTime) < 0)
            {
                return TRUE;
            }
            else if(strcmp(lhs.szModTime,rhs.szModTime) == 0 
                    && strcmp(lhs.szFileName,rhs.szFileName) < 0 )
            {
                return TRUE;
            }

            return FALSE;*/
		}
	};//定义结束	

};


#endif //__COM_DIR_COLLECT_H__


