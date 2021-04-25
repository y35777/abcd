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
* ����: CResObjectCreator::CResObjectCreator
* ����: ���캯��
* ����:
* ���:
* ����:
********************************************************/
CResObjectCreator::CResObjectCreator()
{
    //
}

/********************************************************
* ����: CResObjectCreator::~CResObjectCreator
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CResObjectCreator::~CResObjectCreator()
{
    //
}

/********************************************************
* ����: CResObjectCreator::CreateResObject
* ����: ����ͬ����Դ�����MAP
* ����: RESOURCE_TYPE nStaticRes : RESOURCE_TYPE��Դ���ͣ�
*       1��ʾ��̬��Դ��2��ʾ��̬��Դ
* ���:
* ����: MAP<int, CResObject*> ͬ����Դ��Mapָ�룬�����ڶ�
*       Ӧ����Դ��ΪNULL
********************************************************/
MAP<int, CResObject*>* CResObjectCreator::CreateResObject(
                                          RESOURCE_TYPE nStaticRes)
{
    CResObject* pResObject = NULL;
    errno = 0;
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    //��ȡ�Ƿ���˫��
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
    //ClusterModeδ����
    {
        if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
        //NoClusterδ����
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
	//����ģʽ�²�������Դ
	{
		return NULL;
	}

    //�������ļ��ж���������Դ����������Ϣ��
    MAP<int, CResObject*>* pResObjMap = new MAP<int, CResObject*>;

    //����Դ��
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

        //���Ƿ��Ǿ�̬��Դ
        bStaticRes = IniFile.GetInt(szSecClstr,
                                    CFG_CLSTR_STATIC_RES,
                                    0);

        //����Դ���
        nSwitchGroup = IniFile.GetInt(szSecClstr,
                                      CFG_CLSTR_SWITCH_GROUP,
                                      0);

        //����Դ����
        IniFile.GetString(szSecClstr,
                          CFG_CLSTR_RES_TYPE,
                          "",
                          szResType,
                          20);
        szResType[19] = '\0';

        //����Դ��
        IniFile.GetString(szSecClstr,
                          CFG_CLSTR_RES_NAME,
                          "",
                          szResName,
                          MAX_RES_NAME);

        szResName[MAX_RES_NAME - 1] = '\0';

        //����Դ�ı��ػָ�����
        nLocalRecovery = IniFile.GetInt(szSecClstr,
                                        CFG_CLSTR_RES_LOCAL_REC,
                                        CFG_CLSTR_RES_LOCAL_REC_DEFAULT);

        if(ACE_OS::strcasecmp(szResType, S_RES_VIRTUAL_IP) == 0)
        {
            //��ԭʼIP
            char szOrginalIP[IP_LENGTH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_ORGINAL_IP,
                              "",
                              szOrginalIP,
                              IP_LENGTH);
            szOrginalIP[IP_LENGTH - 1] = '\0';

            //������IP
            char szVirtualIP[IP_LENGTH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_VIRTUAL_IP,
                              "",
                              szVirtualIP,
                              IP_LENGTH);
            szVirtualIP[IP_LENGTH - 1] = '\0';

            //������IP����
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

            //���ø澯��Ϣ
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
            //��������
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

            //���ø澯��Ϣ
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
            //��Ӧ�ó���ȫ·����
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

            //���ø澯��Ϣ
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
            //��Volume����
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

            //���ø澯��Ϣ
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
            //��Volume����
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

            //���ø澯��Ϣ
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
            //����������
            char szCreate[MAX_PATH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_USER_CREATE,
                              "",
                              szCreate,
                              MAX_PATH);
            szCreate[MAX_PATH - 1] = '\0';

            //�������
            char szActive[MAX_PATH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_USER_ACTIVE,
                              "",
                              szActive,
                              MAX_PATH);
            szActive[MAX_PATH - 1] = '\0';

            //��ȥ�����
            char szDeActive[MAX_PATH];
            IniFile.GetString(szSecClstr,
                              CFG_CLSTR_RES_USER_DEACTIVE,
                              "",
                              szDeActive,
                              MAX_PATH);
            szDeActive[MAX_PATH - 1] = '\0';

            //����鷽��
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

            //���ø澯��Ϣ
            pResObject->m_uAlarmID = IniFile.GetInt(szSecClstr,
                                                    CFG_CLSTR_ALARM_ID,
                                                    -1);
            pResObject->m_uAlarmPara = IniFile.GetInt(szSecClstr,
                                                      CFG_CLSTR_ALARM_PARA,
                                                      uUserDefResIndex);
            pResObject->m_nSwitchGroup = nSwitchGroup;
            uUserDefResIndex++;
        }

        //�жϴ����Ķ�/��̬��Դ�����Ƿ�������һ��
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
            //��pResObjMap�н�����Դ�ŵ�pResObject��ӳ��
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
                //����������Ϊ0���򷵻ص�ǰʧЧ��Դ��ID����
                {
                    goto CREATE_RES_ERROR;
                }
                else if(nSwitchGroup > 0)
                //���������Ŵ���0����Ҫ�������ͬ��ŵ���Դ�Ƿ�Ҳ��ʼ��ʧ��
                {
                    //Ѱ�Һ�����Դ���Ƿ����ͬ��ŵ���Դ
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

                    //��������Դ�д���ͬ��ŵ���Դ�������������һ����Դ
                    //��������ڣ�����ǰ���ͬ����Դ���Ƿ�����ѳ�ʼ���ɹ�����Դ
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

                        //���ǰ���ͬ����Դ�в������Ѽ������Դ���򷵻�ʧЧ��Դ��ID����
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

    //ɾ��pResObjMap�ĸ����ڵ�
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
