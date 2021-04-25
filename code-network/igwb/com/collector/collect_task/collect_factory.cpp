#include "../../include/toolbox.h"
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "collect_factory.h"
#include "file_protocol_creator.h"
#include "collect_base.h"
#include "../include/i_cmise_fun.h"
#include "../include/res_config.h"
#include "vertel_ftam.h"
#include "5ess/5ess_collect.h"
#include "ewsd/ewsd_mm_collect.h"
#include "ewsd/ewsd_amas_collect.h"
#include "s1240/s1240_vertel_cmise.h"
#include "s1240/s1240_cmise_group.h"
#include "s1240/s1240_collect.h"
#include "link_group.h"
#include <assert.h>
#include "../include/adapter_include.h"

//added by chenliangwei
#include "ftp_protocol.h"
#include "com/com_collect.h"
//add end

//以下是CCollectFactory文件采集类工厂的实现
CCollectFactory::CCollectFactory()
{
    //NULL
}

//析构函数
CCollectFactory::~CCollectFactory()
{
    //  NULL
}

/********************************************************************
Function:        CCollectBase* CreateCollectObject(const SCreateCollect& stObj);
Description:     本方法用来根据不同标识串来动态创建CCollectBase的派生类对象
                 以及创建文件协议对象，是静态成员函数
Calls:           无
Called By:       类的使用者
Table Accessed:  无
Table Updated:   无
Input:           stObj, SCreateCollect的常量引用，该结构类型的定义见上；
Output:          无
Return:          CCollectBase类对象指针，它实际指向不同的采集类对象
Others:          无
*********************************************************************/
CCollectBase* CCollectFactory::CreateCollectObject(const char* const szCfgPath,
                                                   const SCreateCollect& stObj)
{
    //根据标识串创建不同的采集类对象
    if (StringiCmp(stObj.szCollectId, CO_TYPE_S1240))
    {
        return CreateS1240Object(szCfgPath, stObj);
    }
    if (StringiCmp(stObj.szCollectId, CO_TYPE_5ESS))
    {
        return Create5ESSObject(szCfgPath, stObj);
    }

    //added by chenliangwei
    if (StringiCmp(stObj.szCollectId, CO_TYPE_EWSD_MM))
    {
        return CreateEWSD_MMObject(szCfgPath, stObj);
    }
    if (StringiCmp(stObj.szCollectId, CO_TYPE_EWSD_AMA))
    {
        return CreateEWSD_AMAObject(szCfgPath, stObj);
    }

    if (StringiCmp(stObj.szCollectId, CO_TYPE_CC08))
    {
        return CreateComObject(szCfgPath, stObj);
    }

    if (StringiCmp(stObj.szCollectId, CO_TYPE_METER_CC08)) //add by yangguang
    {
        return CreateComObject(szCfgPath, stObj);
    }

    //added by chenliangwei, 2003-08-14
    if (StringiCmp(stObj.szCollectId, CO_TYPE_TELLIN))
    {
        return CreateComObject(szCfgPath, stObj);
    }
    //added end
    //added by chenliangwei, 2003-08-14
    if (StringiCmp(stObj.szCollectId, CO_TYPE_SJZ_TELLIN))
    {
        return CreateComObject(szCfgPath, stObj);
    }
    //added end
    if (StringiCmp(stObj.szCollectId, CO_TYPE_ZXJ))
    {
        return CreateComObject(szCfgPath, stObj);
    }

    if (StringiCmp(stObj.szCollectId, CO_TYPE_COM))
    {
        return CreateComObject(szCfgPath, stObj);
    }
    //added

    COTRACE("采集对象(%s)检测到无效的采集类型(%s)",
          stObj.szSwitchName,
          stObj.szCollectId);

    return  NULL;
}

CCollectBase* CCollectFactory::CreateS1240Object(const char* const szCfgPath,
                                                 const SCreateCollect& stObj)
{
    CLinkGroup* pLinkGroup = CreateLinkGroup(szCfgPath, stObj);

    if (pLinkGroup == NULL)
    {
        return NULL;
    }

    CS1240CmiseGroup*   pCmiseGroup = new CS1240CmiseGroup();

    CINIFile ini(szCfgPath);
    ini.Open();

    //初始化链路组
    char szTmpKey[50];
    SNPRINTF(szTmpKey,
             sizeof(szTmpKey),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             stObj.nSwitchIndex);

    char szAssPwd[S1240_CFG_LEN];
    ini.GetString(szTmpKey,
                  CFG_CO_ASS_PWD,
                  CFG_CO_ASS_PWD_DEFAULT,
                  szAssPwd,
                  sizeof(szAssPwd));

    char szActPwd[S1240_CFG_LEN];
    ini.GetString(szTmpKey,
                  CFG_CO_ACT_PWD,
                  CFG_CO_ACT_PWD_DEFAULT,
                  szActPwd,
                  sizeof(szActPwd));

    //初始化CMISE组
    char szNename1[S1240_CFG_LEN];
    ini.GetString(szTmpKey,
                  CFG_CO_TP_NENAME,
                  CFG_CO_TP_NENAME_DEFAULT,
                  szNename1,
                  sizeof(szNename1));

    int nTPCount = ini.GetInt(szTmpKey,
                              CFG_CO_TP_COUNT,
                              CFG_CO_TP_COUNT_DEFAULT);
    char szPriLink[50];
    char szBakLink[50];
    char szAgent[50];
    char szManager[50];
    char szNename[S1240_CFG_LEN];
    char szTPID[S1240_CFG_LEN];
    for(int i = 1; i <= nTPCount; i++)
    {
        SNPRINTF(szTmpKey,
                 sizeof(szTmpKey),
                 "%s%d_tp%d",
                 CFG_CO_SEC_SWITCH,
                 stObj.nSwitchIndex,
                 i);
        ini.GetString(szTmpKey,
                      CFG_CO_TP_NENAME,
                      szNename1,
                      szNename,
                      sizeof(szNename));
        if(szNename[0] == '\0')
        {
            delete pLinkGroup;
            delete pCmiseGroup;
            COTRACE("采集对象(%s,TP=%d)的NENAME未配置",
                  stObj.szSwitchName,
                  i);
            return NULL;
        }

        ini.GetString(szTmpKey,
                      CFG_CO_TP_ID,
                      CFG_CO_TP_ID_DEFAULT,
                      szTPID,
                      sizeof(szTPID));
        if(szTPID[0] == '\0')
        {
            delete pLinkGroup;
            delete pCmiseGroup;
            COTRACE("采集对象(%s,TP=%d)的TPID未配置",
                  stObj.szSwitchName,
                  i);
            return NULL;
        }

        SNPRINTF(szTmpKey,
                 sizeof(szTmpKey),
                 "%s%d_tp%d_cmise",
                 CFG_CO_SEC_SWITCH,
                 stObj.nSwitchIndex,
                 i);

        ini.GetString(szTmpKey,
                      CFG_CO_PRI_LINK,
                      CFG_CO_PRI_LINK_DEFAULT,
                      szPriLink,
                      sizeof(szPriLink));
        ini.GetString(szTmpKey,
                      CFG_CO_BAK_LINK,
                      CFG_CO_BAK_LINK_DEFAULT,
                      szBakLink,
                      sizeof(szBakLink));

        //创建CMISE的主连接
        if(szPriLink[0] != '\0')
        {
            ini.GetString(szPriLink,
                          CFG_CO_AGENT_ADDR,
                          CFG_CO_AGENT_ADDR_DEFAULT,
                          szAgent,
                          sizeof(szAgent));
            if(szAgent[0] == '\0')
            {
                delete pLinkGroup;
                delete pCmiseGroup;
                COTRACE("采集对象(%s,TP=%d)的CMISE的主连接的agent未配置",
                      stObj.szSwitchName,
                      i);
                return NULL;
            }

            ini.GetString(szPriLink,
                          CFG_CO_MANAGER_ADDR,
                          CFG_CO_MANAGER_ADDR_DEFAULT,
                          szManager,
                          sizeof(szManager));
            if(szManager[0] == '\0')
            {
                delete pLinkGroup;
                delete pCmiseGroup;
                COTRACE("采集对象(%s,TP=%d)的CMISE的主连接的manager未配置",
                      stObj.szSwitchName,
                      i);
                return NULL;
            }
            CS1240Cmise* pCmise = NULL;
            if (StringiCmp(stObj.szProtocolId, PL_TYPE_VERTEL))
            {
                pCmise = new CS1240VertelCmise();
            }
            else
            {
                COTRACE("采集对象(%s)检测到无效的协议类型(%s)",
                      stObj.szSwitchName,
                      stObj.szProtocolId);
                delete pLinkGroup;
                delete pCmiseGroup;
                return NULL;
            }
            pCmise->SetTPMsg(szAgent,
                             szManager,
                             szAssPwd,
                             szActPwd,
                             szNename,
                             szTPID);
            int nRet = pCmise->Init();
            if(nRet != ERR_SUCCESS)
            {
                delete pLinkGroup;
                delete pCmiseGroup;
                COTRACE("采集对象(%s,TP=%d)的CMISE的主连接初始化失败(错误码=%d)",
                      stObj.szSwitchName,
                      i,
                      nRet);
                return NULL;
            }
            pCmiseGroup->RegisterCmise(szPriLink, pCmise);
        }
        else
        {
            delete pLinkGroup;
            delete pCmiseGroup;
            COTRACE("采集对象(%s,TP=%d)的CMISE的主连接未配置",
                  stObj.szSwitchName,
                  i);
            return NULL;
        }

        //创建CMISE的备连接
        if(szBakLink[0] != '\0')
        {
            ini.GetString(szBakLink,
                          CFG_CO_AGENT_ADDR,
                          CFG_CO_AGENT_ADDR_DEFAULT,
                          szAgent,
                          sizeof(szAgent));
            if(szAgent[0] == '\0')
            {
                delete pLinkGroup;
                delete pCmiseGroup;
                COTRACE("采集对象(%s,TP=%d)的CMISE的备连接的agent未配置",
                      stObj.szSwitchName,
                      i);
                return NULL;
            }

            ini.GetString(szBakLink,
                          CFG_CO_MANAGER_ADDR,
                          CFG_CO_MANAGER_ADDR_DEFAULT,
                          szManager,
                          sizeof(szManager));
            if(szManager[0] == '\0')
            {
                delete pLinkGroup;
                delete pCmiseGroup;
                COTRACE("采集对象(%s,TP=%d)的CMISE的备连接的manager未配置",
                      stObj.szSwitchName,
                      i);
                return NULL;
            }
            CS1240Cmise* pCmise = NULL;
            if (StringiCmp(stObj.szProtocolId, PL_TYPE_VERTEL))
            {
                pCmise = new CS1240VertelCmise;
            }
            else
            {
                COTRACE("采集对象(%s)检测到无效的协议类型(%s)",
                      stObj.szSwitchName,
                      stObj.szProtocolId);
                delete pLinkGroup;
                delete pCmiseGroup;
                return NULL;
            }
            pCmise->SetTPMsg(szAgent,
                             szManager,
                             szAssPwd,
                             szActPwd,
                             szNename,
                             szTPID);
            int nRet = pCmise->Init();
            if(nRet != ERR_SUCCESS)
            {
                delete pLinkGroup;
                delete pCmiseGroup;
                COTRACE("采集对象(%s,TP=%d)的CMISE的首连接初始化失败(错误码=%d)",
                      stObj.szSwitchName,
                      i,
                      nRet);
                return NULL;
            }
            pCmiseGroup->RegisterCmise(szBakLink, pCmise);
        }
    }

    if(pCmiseGroup->GetCmiseCount() == 0)
    {
        delete pLinkGroup;
        delete pCmiseGroup;
        COTRACE("采集对象(%s)CMISE链接数不能为零",
              stObj.szSwitchName);
        return NULL;
    }

    //创建CS1240Collect对象
    CCollectBase *pCollect = new CS1240Collect(stObj.nSwitchIndex,
                                               pLinkGroup,
                                               pCmiseGroup);
    if(pCollect == NULL)
    {
        delete pLinkGroup;
        delete pCmiseGroup;
        COTRACE("采集对象(%s)创建S1240采集子对象失败",
              stObj.szSwitchName);
        return NULL;
    }

    pCollect->SetCfgPath(szCfgPath);

    return  pCollect;
}

CCollectBase* CCollectFactory::Create5ESSObject(const char* const szCfgPath,
                                                const SCreateCollect& stObj)
{
    CLinkGroup* pLinkGroup = CreateLinkGroup(szCfgPath, stObj);

    if (pLinkGroup == NULL)
    {
        return NULL;
    }

    //创建C5ESSCollect对象
    CCollectBase *pCollect = new C5ESSCollect(stObj.nSwitchIndex, pLinkGroup);
    if(pCollect == NULL)
    {
        delete pLinkGroup;
        COTRACE("采集对象(%s)创建5ESS采集子对象失败",
              stObj.szSwitchName);
        return NULL;
    }

    pCollect->SetCfgPath(szCfgPath);
    return  pCollect;
}

CCollectBase* CCollectFactory::CreateEWSD_MMObject(const char* const szCfgPath,
                                                   const SCreateCollect& stObj)
{
    CLinkGroup* pLinkGroup = CreateLinkGroup(szCfgPath, stObj);

    if (pLinkGroup == NULL)
    {
        return NULL;
    }

    //创建C5EWSD_MMCollect对象
    CCollectBase *pCollect = new CEWSD_MMCollect(stObj.nSwitchIndex, pLinkGroup);
    if(pCollect == NULL)
    {
        delete pLinkGroup;
        COTRACE("采集对象(%s)创建EWSD(MM)采集子对象失败",
              stObj.szSwitchName);
        return NULL;
    }

    pCollect->SetCfgPath(szCfgPath);
    return  pCollect;
}

CCollectBase* CCollectFactory::CreateEWSD_AMAObject(const char* const szCfgPath,
                                                   const SCreateCollect& stObj)
{
    CLinkGroup* pLinkGroup = CreateLinkGroup(szCfgPath, stObj);

    if (pLinkGroup == NULL)
    {
        return NULL;
    }

    //创建C5EWSD_AMACollect对象
    CCollectBase *pCollect = new CEWSD_AMASCollect(stObj.nSwitchIndex, pLinkGroup);
    if(pCollect == NULL)
    {
        delete pLinkGroup;
        COTRACE("采集对象(%s)创建EWSD(AMA)采集子对象失败",
              stObj.szSwitchName);
        return NULL;
    }

    pCollect->SetCfgPath(szCfgPath);
    return  pCollect;

}
//added by chenliangwei

CCollectBase* CCollectFactory::CreateComObject(const char* const szCfgPath,
                                                const SCreateCollect& stObj)
{
    CLinkGroup* pLinkGroup = CreateLinkGroup(szCfgPath, stObj);

    if (pLinkGroup == NULL)
    {
        return NULL;
    }


    //创建CComCollect对象
    CCollectBase *pCollect = new CComCollect(stObj.nSwitchIndex, pLinkGroup);
    if(pCollect == NULL)
    {
        delete pLinkGroup;
        COTRACE("采集对象(%s)创建失败",
              stObj.szSwitchName);
        return NULL;
    }

    pCollect->SetCfgPath(szCfgPath);
    return  pCollect;
}

CLinkGroup* CCollectFactory::CreateLinkGroup(const char* const szCfgPath,
                                       const SCreateCollect& stObj)
{
    CLinkGroup *pLinkGroup = new CLinkGroup();

    CINIFile ini(szCfgPath);
    ini.Open();

    //初始化链路组
    char szTmpKey[50];
    char szRemoteAddr[50];
    char szRemoteUser[50];
    char szRemotePwd[50];
    char szLocalAddr[50];
    char szLocalUser[50];
    char szLocalPwd[50];
    char szDevName[50];

    SNPRINTF(szTmpKey,
             sizeof(szTmpKey),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             stObj.nSwitchIndex);
    int nLinkNum = ini.GetInt(szTmpKey,
                              CFG_CO_LINK_COUNT,
                              CFG_CO_LINK_COUNT_DEFAULT);
    for(int i = 1; i <= nLinkNum; i++)
    {
        SNPRINTF(szTmpKey,
                 sizeof(szTmpKey),
                 "%s%d_link%d",
                 CFG_CO_SEC_SWITCH,
                 stObj.nSwitchIndex,
                 i);

        ini.GetString(szTmpKey,
                      CFG_CO_REMOTE_ADDR,
                      CFG_CO_REMOTE_ADDR_DEFAULT,
                      szRemoteAddr,
                      sizeof(szRemoteAddr));
        if(szRemoteAddr[0] == '\0')
        {
            continue;
        }

        ini.GetString(szTmpKey,
                      CFG_CO_REMOTE_USER,
                      CFG_CO_REMOTE_USER_DEFAULT,
                      szRemoteUser,
                      sizeof(szRemoteUser));

        ini.GetString(szTmpKey,
                      CFG_CO_REMOTE_PWD,
                      CFG_CO_REMOTE_PWD_DEFAULT,
                      szRemotePwd,
                      sizeof(szRemotePwd));

        ini.GetString(szTmpKey,
                      CFG_CO_LOCAL_ADDR,
                      CFG_CO_LOCAL_ADDR_DEFAULT,
                      szLocalAddr,
                      sizeof(szLocalAddr));

        ini.GetString(szTmpKey,
                      CFG_CO_LOCAL_USER,
                      CFG_CO_LOCAL_USER_DEFAULT,
                      szLocalUser,
                      sizeof(szLocalUser));

        ini.GetString(szTmpKey,
                      CFG_CO_LOCAL_PWD,
                      CFG_CO_LOCAL_PWD_DEFAULT,
                      szLocalPwd,
                      sizeof(szLocalPwd));

        ini.GetString(szTmpKey,
                      CFG_CO_DEV_NAME,
                      CFG_CO_DEV_NAME_DEFAULT,
                      szDevName,
                      sizeof(szDevName));

        int nTransSpeed = ini.GetInt(szTmpKey,
                                     CFG_CO_TRANS_SPEED,
                                     CFG_CO_TRANS_SPEED_DEFAULT);
        if((nTransSpeed <= 0) || (nTransSpeed > 8))
        {
            nTransSpeed = CFG_CO_TRANS_SPEED_DEFAULT;
        }

        int nAlarmPara = ini.GetInt(szTmpKey,
                                    CFG_CO_LINK_ALARM_PARA,
                                    CFG_CO_LINK_ALARM_PARA_DEFAULT);
        if(nAlarmPara < 0)
        {
            nAlarmPara = CFG_CO_LINK_ALARM_PARA_DEFAULT;
        }

        CFileProtocol* pFileProtocol = NULL;
        pFileProtocol = CFileProtocolFactory::CreateObject(stObj.szProtocolId,
                                                           nTransSpeed,
                                                           szDevName);

        if (pFileProtocol == NULL)
        {
            COTRACE("采集对象(%s)检测到无效的协议类型(%s)",
                  stObj.szSwitchName,
                  stObj.szProtocolId);
            delete pLinkGroup;
            return NULL;
        }

        CLinkBase* pLinkBase = new CLinkBase(pFileProtocol,
                                             szRemoteAddr,
                                             szRemoteUser,
                                             szRemotePwd,
                                             nAlarmPara);
        pLinkBase->SetLocalHostMsg(szLocalAddr,
                                   szLocalUser,
                                   szLocalPwd);
        if(pLinkBase->Init() != ERR_SUCCESS)
        {
            COTRACE("采集对象(%s)初始化链路(%s)失败",
                  stObj.szSwitchName,
                  szTmpKey);
            delete pLinkBase;
            delete pLinkGroup;
            return NULL;
        }
        pLinkGroup->RegisterLink(szTmpKey, pLinkBase);
    }

    if(pLinkGroup->GetLinkCount() == 0)
    {
        delete pLinkGroup;
        COTRACE("采集对象(%s)链接数不能为零",
              stObj.szSwitchName);
        return NULL;
    }

    return pLinkGroup;
}
