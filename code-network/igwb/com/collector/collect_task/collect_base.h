#ifndef __COLLECT_BASE_H__
#define __COLLECT_BASE_H__

/*
CCollectBase是采集操作的基类, 它定义了采集操作相关的基本属性和基本方法
*/
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "file_protocol.h"
#include "link_mgr.h"


//以下是采集操作的基类CCollectBase的定义
class  CCollectBase
{

public:
    CCollectBase(int nSwitchIndex, CLinkGroup* pLinkGroup); //构造函数传入协议对象指针
    virtual ~CCollectBase();

    //初始化采集服务, 是虚函数, 由派生类具体实现
    virtual  int  Col_Init() = 0;

    //拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现;
    virtual  int  Col_GetFile(SColFileReq& stReq) = 0;

    //获得文件列表, 是虚函数, 由派生类具体实现
    virtual  int  Col_GetList(SListOut& pOut) = 0;

    //动态创建子目录, 是虚函数, 由派生类具体实现
    virtual  BOOL CreateLocSubDir() = 0;

    //added by chenliangwei
    //清除指定目录下的过期子目录，供派生类使用的工具函数
    static int ClearOldDateSubDir(const char* szRootDir, const int nSaveDays);

    //得到本地指定目录下的文件列表
    static int GetLocalFileList(const char* szSrcDir, SListOut &pOut);
    //added end

    //清除过期文件, 是虚函数, 由派生类具体实现
    virtual  BOOL ClearOldFile()
    {
        return TRUE;
    }

    //added by chenliangwei
    //检查文件序列号是否连续，是虚函数, 由派生类具体实现
    virtual  BOOL CheckFileSN(const char* szFileName, char* szPreviousFile)
    {
        return TRUE;
    }
    //added end

    //added by chenliangwei, 2003-08-26
    //得到链路的信息
    int GetLinkInfo(MAP<STRING, SLinkStatusMsg>& LinkStatus)
    {
        if(m_pLinkGroup != NULL)
        {
            return m_pLinkGroup->GetLinkInfo(LinkStatus);
        }

        return ERR_SUCCESS;
    }
    //added end

    //测试连接服务端
    virtual  int  TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus)
    {
        if(m_pLinkGroup != NULL)
        {
            return m_pLinkGroup->TestLink(LinkStatus);
        }

        return ERR_SUCCESS;
    }

    //连接采集服务器
    virtual  int  Connect()
    {
        return  ERR_SUCCESS;
    }

    //断开连接
    virtual  int  Disconnect()
    {
        return  ERR_SUCCESS;
    }

    inline void SetCfgPath(const char* const szCfgPath)
    {
        strncpy(m_szCfgPath, szCfgPath, MAX_PATH);
        m_szCfgPath[MAX_PATH - 1] = '\0';
    }
    inline void RegisterLinkMgr(const char* const szType, CLinkMgr* pLinkMgr)
    {
        STRING strType = szType;
        if(m_LinkMgrMap.find(strType) == m_LinkMgrMap.end())
        {
            m_LinkMgrMap[strType] = pLinkMgr;
        }
    }
    inline CLinkMgr* GetLinkMgr(const STRING& strType)
    {
        if(m_LinkMgrMap.find(strType) != m_LinkMgrMap.end())
        {
            return m_LinkMgrMap[strType];
        }
        return NULL;
    }

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

    inline void EnableFileLenVerify()
    {
        m_bFileLenVerify = TRUE;
    }

    inline void DisableFileLenVerify()
    {
        m_bFileLenVerify = FALSE;
    }

protected:

    int   m_nSwitchIndex;
    char  m_szSwitchName[40];       //交换机名
    char  m_szCollectId[40];        //指定具体的采集对象，如"5ESS"，用于创建C5ESSCollect对象
    char  m_szProtocolId[40];       //指定具体的文件采集协议，如"Vertel NT"，用于创建CVertelForNT对象

    char  m_szCfgPath[MAX_PATH];    //当前配置文件名，含路径信息
    CLinkGroup* m_pLinkGroup;
    CLinkMgr* m_pLinkMgr;           //文件传输基类指针，它指向文件传输类CFileProtocol的派生类的对象
    MAP<STRING, CLinkMgr*>  m_LinkMgrMap;

    //话单文件保存天数
    int   m_nFileSaveDays;          //added by chenliangwei
    BOOL  m_bFileLenVerify;         //是否进行文件长度校验
};

#endif //__COLLECT_BASE_H__
