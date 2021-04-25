#include "../../../include/base_type.h"
#include "../../include/i_ftam.h"
#include "../vertel_ftam.h"
#include "../link_mgr.h"
#include "s1240_cmise_mgr.h"
#include "s1240_collect.h"
#include "s1240_tp_collect.h"
#include "../bk_link_mgr.h"

#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include <assert.h>

//构造函数
CS1240Collect::CS1240Collect(int nSwitchIndex, 
                             CLinkGroup* pLinkGroup,
                             CS1240CmiseGroup* pCmiseGroup)
                             :CCollectBase(nSwitchIndex, 
                                           pLinkGroup) 
{
    assert(NULL != pCmiseGroup);
    m_pCmiseGroup = pCmiseGroup;
}

//析构函数
CS1240Collect::~CS1240Collect()
{
    MAP<int, CS1240TPCollect*>::iterator i;
    for(i = m_TPMap.begin(); i != m_TPMap.end(); i++)
    {
        delete i->second;
    }

    m_TPMap.clear();

    if(NULL != m_pCmiseGroup)
    {
        delete m_pCmiseGroup;
        m_pCmiseGroup = NULL;
    }
}

//初始化采集服务, 是虚函数, 由派生类具体实现
int  CS1240Collect::Col_Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("采集任务(ID=%d)打开配置文件(%s)失败", 
              m_nSwitchIndex, 
              m_szCfgPath);
        return ERR_FAIL;
    }

    m_SwitchInfo.szOfficeNum[0]  = '\0';
    m_SwitchInfo.szSwitchCode[0] = '\0';
    m_SwitchInfo.szSwitchName[0] = '\0';

    char szSecName[S1240_CFG_LEN];
    SNPRINTF(szSecName, 
             sizeof(szSecName), 
             "%s%d", 
             CFG_CO_SEC_SWITCH, 
             m_nSwitchIndex);
    szSecName[S1240_CFG_LEN - 1] = '\0';

    //获取采集任务名
    INIFile.GetString(szSecName,
                      CFG_CO_NAME, 
                      CFG_CO_NAME_DEFAULT, 
                      m_SwitchInfo.szSwitchName, 
                      sizeof(m_SwitchInfo.szSwitchName));
    strncpy(m_szSwitchName, 
            m_SwitchInfo.szSwitchName,
            sizeof(m_szSwitchName));
    m_szSwitchName[sizeof(m_szSwitchName) - 1] = 0;

    //获取交换机局号
    INIFile.GetString(szSecName,
                      CFG_CO_SWITCH_OFFICE_NUM, 
                      CFG_CO_SWITCH_OFFICE_NUM_DEFAULT, 
                      m_SwitchInfo.szOfficeNum, 
                      sizeof(m_SwitchInfo.szOfficeNum));

    //获取交换机代码
    INIFile.GetString(szSecName,
                      CFG_CO_SWITCH_CODE, 
                      CFG_CO_SWITCH_CODE_DEFAULT, 
                      m_SwitchInfo.szSwitchCode, 
                      sizeof(m_SwitchInfo.szSwitchCode));

    //获取交换机的根目录名
    INIFile.GetString(szSecName, 
                      CFG_CO_REMOTE_DIR, 
                      CFG_CO_REMOTE_DIR_DEFAULT, 
                      m_SwitchInfo.szRemotePath, 
                      sizeof(m_SwitchInfo.szRemotePath));
    int nLen = strlen(m_SwitchInfo.szRemotePath) - 1;
    if(nLen >= 0)
    {
        if((m_SwitchInfo.szRemotePath[nLen] == '\\')
           || (m_SwitchInfo.szRemotePath[nLen] == '/'))
        {
            m_SwitchInfo.szRemotePath[nLen] = '\0';
        }
    }

    //获取交换机对应的本地根目录名
    INIFile.GetString(szSecName,
                      CFG_CO_LOCAL_DIR, 
                      CCollectorAdapter::GetFrontSavePath(), 
                      m_SwitchInfo.szLocalPath, 
                      sizeof(m_SwitchInfo.szLocalPath));
    nLen = strlen(m_SwitchInfo.szLocalPath) - 1;
    if(nLen > 0)
    {
        if((m_SwitchInfo.szLocalPath[nLen] == '\\')
           || (m_SwitchInfo.szLocalPath[nLen] == '/'))
        {
            m_SwitchInfo.szLocalPath[nLen] = '\0';
        }
    }

    //创建本地话单存放主目录
    if (!CreateDeepDir(m_SwitchInfo.szLocalPath))
    {
        COTRACE("采集对象(%s)原始话单目录(%s)无法创建成功",
            m_szSwitchName,
            m_SwitchInfo.szLocalPath);

        return ERR_FAIL;
    }


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

    int nDumped = INIFile.GetInt(szSecName, 
                                 CFG_CO_DUMPED,
                                 CFG_CO_DUMPED_DEFAULT);

    //获取目标状态
    char  szFinalStatus[S1240_CFG_LEN];        //目标状态(null|dumped|release)
    INIFile.GetString(szSecName, 
                      CFG_CO_FINAL_STATUS, 
                      CFG_CO_FINAL_STATUS_DEFAULT, 
                      szFinalStatus, 
                      sizeof(szFinalStatus));

    CLinkMgr *pPriLinkMgr = NULL;
    CLinkMgr *pBakLinkMgr = NULL; 
    CS1240CmiseMgr* pCmiseMgr = NULL;
    CS1240TPCollect* pTPCollect = NULL;
    char szPriLink[50];
    char szBakLink[50];

	int nTPCount = INIFile.GetInt(szSecName, 
                                  CFG_CO_TP_COUNT,
                                  CFG_CO_TP_COUNT_DEFAULT);
    for(int i = 1; i <= nTPCount; i++)
    {
        /*
            取主TP的链路信息
        */
        SNPRINTF(szSecName,
                 sizeof(szSecName),
                 "%s%d_tp%d_a",
                 CFG_CO_SEC_SWITCH,
                 m_nSwitchIndex,
                 i);
        INIFile.GetString(szSecName,
                          CFG_CO_PRI_LINK, 
                          CFG_CO_PRI_LINK_DEFAULT, 
                          szPriLink, 
                          sizeof(szPriLink));
        if(szPriLink[0] == '\0' || m_pLinkGroup->GetLink(szPriLink) == NULL)
        {
            COTRACE("采集对象(%s,TPid=%d)主侧FTAM主链路没有配置,或配置错误",
                  m_szSwitchName,
                  i);
            return ERR_FAIL;
        }
        INIFile.GetString(szSecName,
                          CFG_CO_BAK_LINK, 
                          CFG_CO_BAK_LINK_DEFAULT, 
                          szBakLink, 
                          sizeof(szBakLink));
        pPriLinkMgr = new CBKLinkMgr(m_pLinkGroup);
        pPriLinkMgr->RegisterLink(szPriLink);
        if(szBakLink[0] != '\0' && m_pLinkGroup->GetLink(szBakLink) != NULL)
        {
            pPriLinkMgr->RegisterLink(szBakLink);
        }

        /*
            取备TP的链路信息
        */
        SNPRINTF(szSecName,
                 sizeof(szSecName),
                 "%s%d_tp%d_b",
                 CFG_CO_SEC_SWITCH,
                 m_nSwitchIndex,
                 i);
        INIFile.GetString(szSecName,
                          CFG_CO_PRI_LINK, 
                          CFG_CO_PRI_LINK_DEFAULT, 
                          szPriLink, 
                          sizeof(szPriLink));
        if(szPriLink[0] == '\0' || m_pLinkGroup->GetLink(szPriLink) == NULL)
        {
            COTRACE("采集对象(%s,TPid=%d)备侧FTAM主链路没有配置,或配置错误",
                  m_szSwitchName,
                  i);
            return ERR_FAIL;
        }
        INIFile.GetString(szSecName,
                          CFG_CO_BAK_LINK, 
                          CFG_CO_BAK_LINK_DEFAULT, 
                          szBakLink, 
                          sizeof(szBakLink));
        pBakLinkMgr = new CBKLinkMgr(m_pLinkGroup);
        pBakLinkMgr->RegisterLink(szPriLink);
        if(szBakLink[0] != '\0' && m_pLinkGroup->GetLink(szBakLink) != NULL)
        {
            pBakLinkMgr->RegisterLink(szBakLink);
        }

        /*
            取CMISE信息
        */
        SNPRINTF(szSecName,
                 sizeof(szSecName),
                 "%s%d_tp%d_cmise",
                 CFG_CO_SEC_SWITCH,
                 m_nSwitchIndex,
                 i);
        INIFile.GetString(szSecName,
                          CFG_CO_PRI_LINK, 
                          CFG_CO_PRI_LINK_DEFAULT, 
                          szPriLink, 
                          sizeof(szPriLink));
        if(szPriLink[0] == '\0' || m_pCmiseGroup->GetCmise(szPriLink) == NULL)
        {
            COTRACE("采集对象(%s,TPid=%d)CMISE主链路没有配置,或配置错误",
                  m_szSwitchName,
                  i);
            return ERR_FAIL;
        }
        INIFile.GetString(szSecName,
                          CFG_CO_BAK_LINK, 
                          CFG_CO_BAK_LINK_DEFAULT, 
                          szBakLink, 
                          sizeof(szBakLink));
        pCmiseMgr = new CS1240CmiseMgr(m_pCmiseGroup);
        pCmiseMgr->RegisterCmise(szPriLink);
        if(szBakLink[0] != '\0' && m_pCmiseGroup->GetCmise(szBakLink) != NULL)
        {
            pCmiseMgr->RegisterCmise(szBakLink);
        }

        pTPCollect = new CS1240TPCollect(pPriLinkMgr,
                                         pBakLinkMgr,
                                         pCmiseMgr,
                                         m_nSwitchIndex,
                                         i);
        pTPCollect->SetCfgPath(m_szCfgPath);
        pTPCollect->SetFileSaveDays(m_nFileSaveDays);
        pTPCollect->SetSwitchName(m_szSwitchName);
        if((NULL == pTPCollect) || (pTPCollect->Init() != ERR_SUCCESS))
        {
            COTRACE("采集任务(%s)创建TP子对象(ID=%d)失败", 
                  m_szSwitchName,
                  i);
            delete pTPCollect;
            continue;
        }
        m_SwitchInfo.S1240TPMap[i] = pTPCollect->GetTPInfo();
        m_TPMap[i] = pTPCollect;
    }

    return ERR_SUCCESS;    
}

//拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现;
int  CS1240Collect::Col_GetFile(SColFileReq& stReq)
{
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        if(i->second->FindFileInList(stReq.szSrcName) == ERR_SUCCESS)
        {
            return i->second->GetFile(stReq);
        }
    }

    return ERR_FAIL;
}

//获得文件列表, 是虚函数, 由派生类具体实现
int  CS1240Collect::Col_GetList(SListOut& pOut)
{
    pOut.FileList.clear();
    SFileInfo file_info;

    VECTOR<SS1240FileMsg> file_msg_list;
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        i->second->GetList(file_msg_list);
    }

    VECTOR<SS1240FileMsg>::iterator iter;
    for(iter = file_msg_list.begin(); iter != file_msg_list.end(); iter++)
    {
        SS1240FileMsg& file_msg = *iter;
        strncpy(file_info.szFileName, 
                file_msg.szFileId,
                sizeof(file_info.szFileName));
        file_info.szFileName[sizeof(file_info.szFileName) - 1] = '\0';
        
        file_info.nAttr = ATTR_FILE;

        file_info.nFileSize      = file_msg.uFileSize;
        file_info.nCollectStatus = CS_NOT_COLLECTED;

        pOut.FileList.push_back(file_info);
    }

    return ERR_SUCCESS;
}

//测试连接服务端
int CS1240Collect::TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus)
{
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        i->second->TestLink(LinkStatus);
    }

    return 0;
}


//动态创建子目录, 是虚函数
BOOL CS1240Collect::CreateLocSubDir()
{
    BOOL bRet = TRUE;
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        if(!i->second->CreateLocSubDir())
        {
            bRet = FALSE;
        }
    }

    return bRet;
}

//清除过期文件, 是虚函数
BOOL CS1240Collect::ClearOldFile()
{
    BOOL bRet = TRUE;
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        if(!i->second->ClearOldFile())
        {
            bRet = FALSE;
        }
    }

    return bRet;
}

//检查文件序列号是否连续，是虚函数, 由派生类具体实现
BOOL CS1240Collect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        if(i->second->FindFileInList(szFileName) == ERR_SUCCESS)
        {
            return i->second->CheckFileSN(szFileName, szPreviousFile);
        }
    }

    return TRUE;

}

int CS1240Collect::SetAlarmLimit(const char* const szTP, int nAlarmLimit)
{
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        const SS1240TPInfo& TPInfo = i->second->GetTPInfo();
        if(StringiCmp(TPInfo.szTPID, szTP))
        {
            return i->second->SetAlarmLimit(nAlarmLimit);
        }
    }

    return ERR_FAIL;
}

//指定的文件是否可采
int CS1240Collect::CanGetSpecFile(const char* const szModule,
                                  const char* const szSrcFile)
{
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        const SS1240TPInfo& TPInfo = i->second->GetTPInfo();
        if(StringiCmp(TPInfo.szTPID, szModule))
        {
            return i->second->CanGetSpecFile(szSrcFile);
        }
    }

    return ERR_FILE_CANNOT_BE_FOUND;
}

int CS1240Collect::GetSpecFile(const char* const szModule,
                               const char* const szSrcFile, 
                               const char* const szDstFile)
{
    /*
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        const SS1240TPInfo& TPInfo = i->second->GetTPInfo();
        if(StringiCmp(TPInfo.szTPID, szModule))
        {
            return i->second->GetSpecFile(szSrcFile, szDstFile);
        }
    }*/

    return ERR_FAIL;
}

//获取交换机信息
int CS1240Collect::GetSwitchInfo(LIST<STRING>& label,
                                 LIST<STRING>& content)
{
    label.push_back("module");
    label.push_back("alarmLimit");

    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        const SS1240TPInfo& TPInfo = i->second->GetTPInfo();
        content.push_back(TPInfo.szTPID);
        char szBuf[10];
        SNPRINTF(szBuf, sizeof(szBuf), "%d", i->second->GetAlarmLimit());
        content.push_back(szBuf);
    }

    return ERR_SUCCESS;
}

//获取模块信息
int CS1240Collect::GetModuleInfo(const char* const szModule,
                                 LIST<STRING>& label, 
                                 LIST<STRING>& content)
{
    label.push_back("主文件名");
    label.push_back("副文件名");
    label.push_back("MODE");
    label.push_back("长度");
    label.push_back("状态");
    label.push_back("DUMPED");
    label.push_back("写计数器");
    label.push_back("采集状态");

    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        const SS1240TPInfo& TPInfo = i->second->GetTPInfo();
        if(StringiCmp(TPInfo.szTPID, szModule))
        {
            MAP<STRING, SS1240FileStatus> file_msg_list;
            i->second->GetAllList(file_msg_list);

            MAP<STRING, SS1240FileStatus>::iterator iter = file_msg_list.begin();
            for(; iter != file_msg_list.end(); iter++)
            {
                char szBuf[20];
                SS1240FileStatus& file_status = iter->second;
                content.push_back(file_status.file_msg.szFileId);
                content.push_back(file_status.file_msg.szFileIdDup);

                SNPRINTF(szBuf, sizeof(szBuf), "%d", file_status.file_msg.nFileMode);
                content.push_back(szBuf);

                SNPRINTF(szBuf, sizeof(szBuf), "%d", file_status.file_msg.uFileSize);
                content.push_back(szBuf);

                SNPRINTF(szBuf, sizeof(szBuf), "%d", file_status.file_msg.nFileStatus);
                switch(file_status.file_msg.nFileStatus)
                {
                case CMISE_STATUS_SELECTED:
                    content.push_back("SELECTED");
                    break;
                case CMISE_STATUS_RECORDING:
                    content.push_back("RECORDING");
                    break;
                case CMISE_STATUS_READYTOBETRANSFERRED:
                    content.push_back("READY");
                    break;
                case CMISE_STATUS_TRANSFERRING:
                    content.push_back("TRANSFERRING");
                    break;
                default:
                    content.push_back("FREE");
                    break;
                }

                SNPRINTF(szBuf, sizeof(szBuf), "%d", file_status.file_msg.nFileLocalDumped);
                content.push_back(szBuf);

                SNPRINTF(szBuf, sizeof(szBuf), "%d", file_status.file_msg.nFileCounter);
                content.push_back(szBuf);

                if(file_status.file_msg.nFileCounter == file_status.nLastFileCounter)
                {
                    content.push_back("已采集");
                }
                else if(CS_NOT_COLLECTED == file_status.nStatus)
                {
                    content.push_back("未采集");
                }
                else
                {
                    content.push_back("采集过程中");
                }
            }
        }
    }
    if(content.size() == 0)
    {
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
    }

    return ERR_SUCCESS;
}

//获取文件长度
int CS1240Collect::GetFileLen(const char* const szModule,
                              const char* const szSrcFile)
{
    MAP<int, CS1240TPCollect*>::iterator i = m_TPMap.begin();
    for(; i != m_TPMap.end(); i++)
    {
        const SS1240TPInfo& TPInfo = i->second->GetTPInfo();
        if(StringiCmp(TPInfo.szTPID, szModule))
        {
            return i->second->GetFileLen(szSrcFile);
        }
    }

    return 0;
}


