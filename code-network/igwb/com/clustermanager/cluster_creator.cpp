#include "../include/frame.h"
#include "../include/toolbox.h"
#include "cluster_creator.h"


/********************************************************
* ����: CClusterCreator::CClusterCreator
* ����: ���캯��
* ����: 
* ���: 
* ����: 
********************************************************/
CClusterCreator::CClusterCreator()
{
    //
}

/********************************************************
* ����: CClusterCreator::~CClusterCreator
* ����: ��������
* ����: 
* ���: 
* ����: 
********************************************************/
CClusterCreator::~CClusterCreator()
{
    //
}

/********************************************************
* ����: CClusterCreator::CreateComCluster
* ����: CComCluster�Ĵ�������
* ����: 
* ���: 
* ����: CComCluster* CComCluster�����ָ��,�������ͷ�
********************************************************/
CComCluster* CClusterCreator::CreateComCluster()
{
    CComCluster* pComCluster = new CComCluster;
    if(NULL != pComCluster)
    {
        CINIFile *pIniFile;
        pIniFile = new CINIFile(GetCfgFilePath());
        pIniFile->Open();

        //���ڵ��
        pComCluster->m_LocalNodeMsg.nNodeID = pIniFile->GetInt(
                                              CFG_SEC_COMMON,
                                              CFG_SERVERNO,
                                              INVALID_NODE_ID);

        //�����μ������Сʱ���
        pComCluster->m_lActiveInterval = pIniFile->GetInt(
                                         CFG_SEC_CLUSTER,
                                         CFG_CLSTR_MIN_ACTIVE_INV,
                                         ACTIVE_INTERVAL_TIME_DEFAULT);

        //�����л�����
        pComCluster->m_SwitchBackMode = (ESWITCH_MODE) pIniFile->GetInt(
                                         CFG_SEC_CLUSTER,
                                         CFG_CLSTR_SWITCH_MODE,
                                         SM_AUTO_BACK);

        //������󼤻�ʧ�ܴ���
        pComCluster->m_lMaxActiveFailTimes = pIniFile->GetInt(
                                             CFG_SEC_CLUSTER,
                                             CFG_CLSTR_MAX_ACTIVE_FAIL_TIMES,
                                             MAX_ACTIVE_FAIL_TIMES_DEFAULT);

        //��ȡ�ж������жϵ�ʱ�䷧ֵ
        pComCluster->m_nMaxHBBrokenTime = pIniFile->GetInt(
                                          CFG_SEC_CLUSTER,
                                          CFG_CLSTR_HEART_BEAT_BROKEN,
                                          MAX_HB_BROKEN_TIME_DEFAULT);

        //��ȡ���������������
        pComCluster->m_bInstallArray = pIniFile->GetInt(
                                          CFG_SEC_CLUSTER,
                                          CFG_CLSTR_INSTALL_DISK_ARRAY,
                                          TRUE);

        //��ȡ�Ƿ񵥻�����
        int nNoCluster = pIniFile->GetInt(
                              CFG_SEC_COMMON,
                              CFG_COMMON_KEY_NO_CLUSTER,
                              CFG_COMMON_KEY_NO_CLUSTER_DEFAULT);

        int nClusterMode = pIniFile->GetInt(
                              CFG_SEC_COMMON,
                              CFG_COMMON_KEY_CLUSTER_MODE,
                              CFG_COMMON_KEY_CLUSTER_MODE_TEST);
        if(nClusterMode == CFG_COMMON_KEY_CLUSTER_MODE_TEST)
        //ClusterModeδ����
        {
            if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
            //NoClusterδ����
            {
                pComCluster->m_bSingleServer = !CFG_COMMON_KEY_CLUSTER_MODE_DEFAULT;
            }
            else
            {
                pComCluster->m_bSingleServer = nNoCluster;
            }
        }
        else
        {
            pComCluster->m_bSingleServer = !nClusterMode;
        }

        if(!pComCluster->m_bSingleServer
            && !pComCluster->m_bInstallArray
            && pComCluster->m_LocalNodeMsg.nNodeID != 0)
        {
            STRING strStatusFile = GetAppPath();
            strStatusFile       += "/config/cluster";
            CreateDeepDir(strStatusFile.c_str());
            strStatusFile       += "/cls_status.dat";
            pComCluster->SetStatusFile(strStatusFile.c_str());
        }
        pIniFile->Close();
        delete pIniFile;
    }

    return pComCluster;
}