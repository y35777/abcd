// com_collect.cpp: implementation of the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/base_type.h"
#include "../bk_link_mgr.h"
#include "ewsd_amas_collect.h"
#include "ewsd_ama_collect.h"

#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include <assert.h>

CEWSD_AMASCollect::CEWSD_AMASCollect(int nSwitchIndex, CLinkGroup* pLinkGroup)
                        :CCollectBase(nSwitchIndex, pLinkGroup)
{
    m_pLinkMgr        = NULL;
    m_szSwitchName[0] = '\0';
}


CEWSD_AMASCollect::~CEWSD_AMASCollect()
{
    VECTOR<CEWSD_AMACollect*>::iterator i;
    for(i = m_AMAColList.begin(); i != m_AMAColList.end(); i++)
    {
        delete *i;
    }

    m_AMAColList.clear();

    if (m_pLinkMgr != NULL)
    {
        delete m_pLinkMgr;
        m_pLinkMgr = NULL;
    }

}


//初始化采集服务, 是虚函数, 由派生类具体实现
int  CEWSD_AMASCollect::Col_Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("采集对象(ID=%d)打开配置文件(%s)失败",
              m_nSwitchIndex,
              m_szCfgPath);
        return ERR_FAIL;
    }


    char szSecName[COM_CFG_LEN];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[COM_CFG_LEN - 1] = '\0';

    //获取采集任务名
    INIFile.GetString(szSecName,
                      CFG_CO_NAME,
                      CFG_CO_NAME_DEFAULT,
                      m_szSwitchName,
                      sizeof(m_szSwitchName));

    //读话单文件的保存天数
    m_nFileSaveDays = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT);
    m_nFileSaveDays = INIFile.GetInt(szSecName,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);
    if(m_nFileSaveDays < 0)
    {
        m_nFileSaveDays = CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT;
    }

    //创建LinkMgr对象
    char szPriLink[50];
    char szBakLink[50];

    INIFile.GetString(szSecName,
                      CFG_CO_PRI_LINK,
                      CFG_CO_PRI_LINK_DEFAULT,
                      szPriLink,
                      sizeof(szPriLink));
    if (szPriLink[0] == '\0' || m_pLinkGroup->GetLink(szPriLink) == NULL)
    {
        COTRACE("采集对象(%s)主链路没有配置,或配置错误",
              m_szSwitchName);
        return ERR_FAIL;
    }

    INIFile.GetString(szSecName,
                      CFG_CO_BAK_LINK,
                      CFG_CO_BAK_LINK_DEFAULT,
                      szBakLink,
                      sizeof(szBakLink));
    m_pLinkMgr = new CBKLinkMgr(m_pLinkGroup);
    m_pLinkMgr->RegisterLink(szPriLink);
    if(szBakLink[0] != '\0' && m_pLinkGroup->GetLink(szBakLink) != NULL)
    {
        m_pLinkMgr->RegisterLink(szBakLink);
    }

    //创建目录采集对象
    CEWSD_AMACollect* pCollect = NULL;

    int nAMACount = INIFile.GetInt(szSecName,
                                  CFG_CO_COL_AMA_COUNT,
                                  CFG_CO_COL_AMA_COUNT_DEFAULT);
    for(int i = 1; i <= nAMACount; i++)
    {
        /*
            取采集目录的链路信息
        */
        SNPRINTF(szSecName,
                 sizeof(szSecName),
                 "%s%d_ama%d",
                 CFG_CO_SEC_SWITCH,
                 m_nSwitchIndex,
                 i);

        pCollect = new CEWSD_AMACollect(m_pLinkMgr, m_nSwitchIndex, i);
        if (pCollect ==  NULL)
        {
            COTRACE("采集对象(%s)创建AMA子对象(ID=%d)失败",
                  m_szSwitchName, i);
            continue;
        }

        pCollect->SetCfgPath(m_szCfgPath);
        pCollect->SetFileSaveDays(m_nFileSaveDays);
        pCollect->SetSwitchName(m_szSwitchName);

        if(pCollect->Init() != ERR_SUCCESS)
        {
            COTRACE("采集对象(%s)创建AMA子对象(ID=%d)失败",
                  m_szSwitchName, i);
            delete pCollect;
            continue;
        }

        m_AMAColList.push_back(pCollect);
    }

    return ERR_SUCCESS;
}


//拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现;
int  CEWSD_AMASCollect::Col_GetFile(SColFileReq& stReq)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if((*i)->FindFileInList(stReq.szSrcName) == ERR_SUCCESS)
        {
            return (*i)->GetFile(stReq);
        }
    }

    return ERR_FAIL;
}

//获得文件列表, 是虚函数, 由派生类具体实现
int  CEWSD_AMASCollect::Col_GetList(SListOut& pOut)
{
    pOut.FileList.clear();

    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        (*i)->GetList(pOut);
    }

    return ERR_SUCCESS;
}

//动态创建子目录, 是虚函数
BOOL CEWSD_AMASCollect::CreateLocSubDir()
{
    BOOL bRet = TRUE;
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(!((*i)->CreateLocSubDir()))
        {
            bRet = FALSE;
        }
    }

    return bRet;
}

//清除过期文件, 是虚函数
BOOL CEWSD_AMASCollect::ClearOldFile()
{
    BOOL bRet = TRUE;
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(!((*i)->ClearOldFile()))
        {
            bRet = FALSE;
        }
    }

    return bRet;
}

//检查文件序列号是否连续，并处理漏采文件，是虚函数, 由派生类具体实现
BOOL CEWSD_AMASCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if((*i)->FindFileInList(szFileName) == ERR_SUCCESS)
        {
            return (*i)->CheckFileSN(szFileName, szPreviousFile);
        }
    }

    return TRUE;

}

//指定的文件是否可采
int CEWSD_AMASCollect::CanGetSpecFile(const char* const szModule,
                                      const char* const szSrcFile)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->CanGetSpecFile(szSrcFile);
        }
    }

    return ERR_FAIL;
}

int CEWSD_AMASCollect::GetSpecFile(const char* const szModule,
                                   const char* const szSrcFile,
                                   const char* const szDstFile)
{
    return ERR_FAIL;
}

//获取交换机信息
int CEWSD_AMASCollect::GetSwitchInfo(LIST<STRING>& label,
                                     LIST<STRING>& content)
{
    label.push_back("module");

    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        content.push_back((*i)->GetModuleLabel());
    }

    return ERR_SUCCESS;
}

//获取模块信息
int CEWSD_AMASCollect::GetModuleInfo(const char* const szModule,
                                     LIST<STRING>& label,
                                     LIST<STRING>& content)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->GetModuleInfo(label, content);
        }
    }

    return ERR_FAIL;
}

//获取文件长度
int CEWSD_AMASCollect::GetFileLen(const char* const szModule,
                                  const char* const szSrcFile)
{
    VECTOR<CEWSD_AMACollect*>::iterator i = m_AMAColList.begin();
    for(; i != m_AMAColList.end(); i++)
    {
        if(StringiCmp((*i)->GetModuleLabel(), szModule))
        {
            return (*i)->GetFileLen(szSrcFile);
        }
    }

    return 0;
}

