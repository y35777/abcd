#include <assert.h>
#include "../include/frame.h"
#include "../include/toolbox.h"
#include "res_manager.h"
#include "RaidHDCheck.h"
#include "res_object_creator.h"
#include "res_service.h"
#include "res_volume.h"
#include "res_ibm_volume.h"
#include "res_program.h"
#include "res_ip.h"
#include "res_user_defined.h"
#include "resource.h"


/********************************************************
* 名称: CResObjectCreator::CResObjectCreator
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResObjectCreator::CResObjectCreator()
{
    //
}

/********************************************************
* 名称: CResObjectCreator::~CResObjectCreator
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CResObjectCreator::~CResObjectCreator()
{
    //
}

/********************************************************
* 名称: CResObjectCreator::CreateResObject
* 功能: 创建同步资源对象的MAP
* 输入: RESOURCE_TYPE nStaticRes : RESOURCE_TYPE资源类型，
*       1表示动态资源，2表示静态资源
* 输出:
* 返回: MAP<int, CResObject*> 同步资源的Map指针，不存在对
*       应的资源则为NULL
********************************************************/
MAP<int, CResObject*>* CResObjectCreator::CreateResObject(
                                          RESOURCE_TYPE nStaticRes)
{
    CResObject* pResObject = NULL;
    errno = 0;
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    //读取是否是双机
    BOOL bNoCluster;
    int nNoCluster = IniFile.GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_NO_CLUSTER,
                          CFG_COMMON_KEY_NO_CLUSTER_DEFAULT);

    int nClusterMode = IniFile.GetInt(
                          CFG_SEC_COMMON,
                          CFG_COMMON_KEY_CLUSTER_MODE,
                          CFG_COMMON_KEY_CLUSTER_MODE_TEST);
    if(nClusterMode == CFG_COMMON_KEY_CLUSTER_MODE_TEST)
    //ClusterMode未配置
    {
        if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
        //NoCluster未配置
        {
            bNoCluster = !CFG_COMMON_KEY_CLUSTER_MODE_DEFAULT;
        }
        else
        {
            bNoCluster = nNoCluster;
        }
    }
    else
    {
        bNoCluster = !nClusterMode;
    }

	if(bNoCluster)
	//单机模式下不加载资源
	{
		return NULL;
	}

    //从配置文件中读出各个资源对象配置信息。
    MAP<int, CResObject*>* pResObjMap = new MAP<int, CResObject*>;

    //读资源数
    int nResCount = IniFile.GetInt(CFG_SEC_CLUSTER,
                                   CFG_CLSTR_RES_COUNT,
                                   0);
    if(nResCount > MAX_RES_NUM)
    {
        TRACE(MTS_CLSTR, S_RES_NUM_EXCEED, nResCount, MAX_RES_NUM);
        errno = ERR_CREATE_RES_OBJECT;
		return NULL;
    }

    char szResType[20];
    char szResName[MAX_RES_NAME];
    char szSecClstr[20];
    int nSwitchGroup         = 0;
    int nLocalRecovery       = 0;
    BOOL bStaticRes          = FALSE;
    UINT4 uIPResIndex        = 1;
    UINT4 uVolumeResIndex    = 1;
    UINT4 uServiceResIndex   = 1;
    UINT4 uProgramResIndex   = 1;
    UINT4 uUserDefResIndex   = 1;
    for(int i = 1; i <= nResCount; i++)
    {
        pResObject = NULL;
        SNPRINTF(szSecClstr, 20, "Resource%d", i);
        szSecClstr[19] = '\0';

        //读是否是静态资源
        bStaticRes = IniFile.GetInt(szSecClstr,
                                    CFG_CLSTR_STATIC_RES,
                                    0);

        //读资源组号
        nSwitchGroup = IniFile.GetInt(szSecClstr,
                                      CFG_CLSTR_SWITCH_GROUP,
                                      0);

        //读资源类型
        IniFile.GetString(szSecClstr,
                          CFG_CLSTR_RES_TYPE,
                          "",
                          szResType,
                          20);
        szResType[19] = '\0';

        //读资源名
        IniFile.GetString(szSecClstr,
                          CFG_CLSTR_RES_NAME,
                          "",
                          szResName,
                          MAX_RES_NAME);

        szResName[MAX_RES_NAME - 1] = '\0';

        //读资源的本地恢复次数
        nLocalRecovery = IniFile.GetInt(szSecClstr,
                                        CFG_CLSTR_RES_LOCAL_REC,
                                        CFG_CLSTR_RES_LOCAL_REC_DEFAULT);

        if(ACE_OS::strcasecmp(szResType, S_RES_VIRTUAL_IP) == 0)
        {
            //读原始IP
            char szOrginalIP[IP_LENGTH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_ORGINAL_IP,
                              "",
                              szOrginalIP,
                              IP_LENGTH);
            szOrginalIP[IP_LENGTH - 1] = '\0';

            //读虚拟IP
            char szVirtualIP[IP_LENGTH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_VIRTUAL_IP,
                              "",
                              szVirtualIP,
                              IP_LENGTH);
            szVirtualIP[IP_LENGTH - 1] = '\0';

            //读虚拟IP掩码
            char szIPMask[IP_LENGTH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_VIRTUAL_MASK,
                              "",
                              szIPMask,
                              IP_LENGTH);
            szIPMask[IP_LENGTH - 1] = '\0';

            BOOL bCheckDisConn = IniFile.GetInt(szSecClstr,
                                                CFG_CHECK_DISCONN,
                                                CFG_CHECK_DISCONN_DEFUALT);

            pResObject = new CResIp(szOrginalIP, szVirtualIP,
                                    szIPMask, bCheckDisConn,
                                    szResName, 
                                    ERES_VIRTUAL_IP,
                                    nLocalRecovery);

            //配置告警信息
            pResObject->m_uAlarmID = IniFile.GetInt(szSecClstr,
                                                    CFG_CLSTR_ALARM_ID,
                                                    ALARM_ID_IPRES);
            pResObject->m_uAlarmPara = IniFile.GetInt(szSecClstr,
                                                      CFG_CLSTR_ALARM_PARA,
                                                      uIPResIndex);
            pResObject->m_nSwitchGroup = nSwitchGroup;
            uIPResIndex++;
        }
        else if(ACE_OS::strcasecmp(szResType, S_RES_SERVICE) == 0)
        {
            //读服务名
            char szService[MAX_SERVICE_NAME];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_SERVICE,
                              "",
                              szService,
                              MAX_SERVICE_NAME);
            szService[MAX_SERVICE_NAME - 1] = '\0';

            pResObject = new CResService(szService, szResName,
                                         ERES_SERVICE,
                                         nLocalRecovery);

            //配置告警信息
            pResObject->m_uAlarmID = IniFile.GetInt(szSecClstr,
                                                    CFG_CLSTR_ALARM_ID,
                                                    ALARM_ID_SVCRES);
            pResObject->m_uAlarmPara = IniFile.GetInt(szSecClstr,
                                                      CFG_CLSTR_ALARM_PARA,
                                                      uServiceResIndex);
            pResObject->m_nSwitchGroup = nSwitchGroup;
            uServiceResIndex++;
        }
        else if(ACE_OS::strcasecmp(szResType, S_RES_PROGRAM) == 0)
        {
            //读应用程序全路径名
            char szAppName[MAX_PATH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_APP_PATH,
                              "",
                              szAppName,
                              MAX_PATH);
            szAppName[MAX_PATH - 1] = '\0';

            pResObject = new CResProgram(szAppName, szResName,
                                         ERES_PROGRAM,
                                         nLocalRecovery);

            //配置告警信息
            pResObject->m_uAlarmID = IniFile.GetInt(szSecClstr,
                                                    CFG_CLSTR_ALARM_ID,
                                                    -1);
            pResObject->m_uAlarmPara = IniFile.GetInt(szSecClstr,
                                                      CFG_CLSTR_ALARM_PARA,
                                                      uProgramResIndex);
            pResObject->m_nSwitchGroup = nSwitchGroup;
            uProgramResIndex++;
        }
        else if(ACE_OS::strcasecmp(szResType, S_RES_VOLUME) == 0)
        {
            //读Volume卷名
            char szVolume[DRIVE_NAME_LENGTH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_VOLUME,
                              "",
                              szVolume,
                              DRIVE_NAME_LENGTH);
            szVolume[DRIVE_NAME_LENGTH - 1] = '\0';

            pResObject = new CResVolume(szVolume, szResName,
                                        ERES_VOLUME,
                                        nLocalRecovery);

            //配置告警信息
            pResObject->m_uAlarmID = IniFile.GetInt(szSecClstr,
                                                    CFG_CLSTR_ALARM_ID,
                                                    ALARM_ID_VOLUME);
            pResObject->m_uAlarmPara = IniFile.GetInt(szSecClstr,
                                                      CFG_CLSTR_ALARM_PARA,
                                                      uVolumeResIndex);
            uVolumeResIndex++;
        }
        else if(ACE_OS::strcasecmp(szResType, S_RES_IBM_VOLUME) == 0)
        {
            //读Volume卷名
            char szVolume[DRIVE_NAME_LENGTH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_VOLUME,
                              "",
                              szVolume,
                              DRIVE_NAME_LENGTH);
            szVolume[DRIVE_NAME_LENGTH - 1] = '\0';

            pResObject = new CResIBMVolume(szVolume, szResName,
                                           ERES_IBM_VOLUME,
                                           nLocalRecovery);

            //配置告警信息
            pResObject->m_uAlarmID = IniFile.GetInt(szSecClstr,
                                                    CFG_CLSTR_ALARM_ID,
                                                    ALARM_ID_VOLUME);
            pResObject->m_uAlarmPara = IniFile.GetInt(szSecClstr,
                                                      CFG_CLSTR_ALARM_PARA,
                                                      uVolumeResIndex);
            uVolumeResIndex++;
        }
        else if(ACE_OS::strcasecmp(szResType, S_RES_USERDEFINED) == 0)
        {
            //读创建方法
            char szCreate[MAX_PATH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_USER_CREATE,
                              "",
                              szCreate,
                              MAX_PATH);
            szCreate[MAX_PATH - 1] = '\0';

            //读激活方法
            char szActive[MAX_PATH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_USER_ACTIVE,
                              "",
                              szActive,
                              MAX_PATH);
            szActive[MAX_PATH - 1] = '\0';

            //读去激活方法
            char szDeActive[MAX_PATH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_USER_DEACTIVE,
                              "",
                              szDeActive,
                              MAX_PATH);
            szDeActive[MAX_PATH - 1] = '\0';

            //读检查方法
            char szCheckStatus[MAX_PATH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_USER_CHECK,
                              "",
                              szCheckStatus,
                              MAX_PATH);
            szCheckStatus[MAX_PATH - 1] = '\0';

            pResObject = new CResUserDefined(szCreate, szActive,
                                             szDeActive, szCheckStatus,
                                             szResName,
                                             ERES_USERDEFINED,
                                             nLocalRecovery);

            //配置告警信息
            pResObject->m_uAlarmID = IniFile.GetInt(szSecClstr,
                                                    CFG_CLSTR_ALARM_ID,
                                                    -1);
            pResObject->m_uAlarmPara = IniFile.GetInt(szSecClstr,
                                                      CFG_CLSTR_ALARM_PARA,
                                                      uUserDefResIndex);
            pResObject->m_nSwitchGroup = nSwitchGroup;
            uUserDefResIndex++;
        }

        //判断创建的动/静态资源类型是否与输入一致
        if(((!bStaticRes) && (nStaticRes - RT_DYNAMIC)) ||
           (bStaticRes && ! (nStaticRes - RT_DYNAMIC)))
        {
            if(NULL != pResObject)
            {
                pResObject->Remove();
                delete pResObject;
                pResObject = NULL;
            }
            continue;
        }

        if(NULL == pResObject)
        {
            goto CREATE_RES_ERROR;
        }
        else
        {
            pResObject->m_uResIndex = i;
            //在pResObjMap中建立资源号到pResObject的映射
            (*pResObjMap)[i] = pResObject;
        }
    }

    if (pResObjMap->size() == 0)
    {
        delete pResObjMap;
        pResObjMap= NULL;
    }
    else
    {
        MAP<int, CResObject*>::iterator i;
        for (i = pResObjMap->begin(); i != pResObjMap->end(); i++)
        {
            pResObject = (*i).second;
            if(pResObject->Create() != ERR_SUCCESS)
            {
                int nSwitchGroup = pResObject->GetSwitchGroup();
                if(nSwitchGroup == 0)
                //如果倒换组号为0，则返回当前失效资源的ID掩码
                {
                    goto CREATE_RES_ERROR;
                }
                else if(nSwitchGroup > 0)
                //如果倒换组号大于0，则要检查其它同组号的资源是否也初始化失败
                {
                    //寻找后绪资源中是否存在同组号的资源
                    MAP<int, CResObject*>::iterator j = i;
                    BOOL bExistSameGroupRes = FALSE;
                    while(++j != pResObjMap->end())
                    {
                        if((*j).second->GetSwitchGroup() == nSwitchGroup)
                        {
                            bExistSameGroupRes = TRUE;
                            break;
                        }
                    }

                    //当后绪资源中存在同组号的资源，则继续激活下一个资源
                    //如果不存在，则检查前面的同组资源中是否存在已初始化成功的资源
                    if(!bExistSameGroupRes)
                    {
                        BOOL bActiveOneRes = FALSE;
                        j = pResObjMap->begin();
                        while (j != i)
                        {
                            if((*j).second->GetStatus() == ERES_STATUS_DEACTIVE)
                            {
                                if((*j).second->GetSwitchGroup() == nSwitchGroup)
                                {
                                    bActiveOneRes = TRUE;
                                    break;
                                }
                            }
                            j++;
                        }

                        //如果前面的同组资源中不存在已激活的资源，则返回失效资源的ID掩码
                        if(!bActiveOneRes)
                        {
                            goto CREATE_RES_ERROR;
                        }
                    }
                }    
            }
        }
    }

    return pResObjMap;

CREATE_RES_ERROR:

    //删除pResObjMap的各个节点
    if(pResObjMap != NULL)
    {
        MAP<int, CResObject*>::iterator iter;
        for (iter = pResObjMap->begin(); iter != pResObjMap->end(); iter++)
        {
            (*iter).second->Remove();
            delete (*iter).second;
        }
        pResObjMap->clear();
        delete pResObjMap;
    }

    errno = ERR_CREATE_RES_OBJECT;
    return NULL;
}
