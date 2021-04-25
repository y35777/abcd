#include "../include/frame.h"
#include "../include/toolbox.h"
#include "cluster_creator.h"


/********************************************************
* 名称: CClusterCreator::CClusterCreator
* 功能: 构造函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CClusterCreator::CClusterCreator()
{
    //
}

/********************************************************
* 名称: CClusterCreator::~CClusterCreator
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CClusterCreator::~CClusterCreator()
{
    //
}

/********************************************************
* 名称: CClusterCreator::CreateComCluster
* 功能: CComCluster的创建函数
* 输入: 
* 输出: 
* 返回: CComCluster* CComCluster对象的指针,调用者释放
********************************************************/
CComCluster* CClusterCreator::CreateComCluster()
{
    CComCluster* pComCluster = new CComCluster;
    if(NULL != pComCluster)
    {
        CINIFile *pIniFile;
        pIniFile = new CINIFile(GetCfgFilePath());
        pIniFile->Open();

        //读节点号
        pComCluster->m_LocalNodeMsg.nNodeID = pIniFile->GetInt(
                                              CFG_SEC_COMMON,
                                              CFG_SERVERNO,
                                              INVALID_NODE_ID);

        //读两次激活的最小时间差
        pComCluster->m_lActiveInterval = pIniFile->GetInt(
                                         CFG_SEC_CLUSTER,
                                         CFG_CLSTR_MIN_ACTIVE_INV,
                                         ACTIVE_INTERVAL_TIME_DEFAULT);

        //读出切换策略
        pComCluster->m_SwitchBackMode = (ESWITCH_MODE) pIniFile->GetInt(
                                         CFG_SEC_CLUSTER,
                                         CFG_CLSTR_SWITCH_MODE,
                                         SM_AUTO_BACK);

        //读出最大激活失败次数
        pComCluster->m_lMaxActiveFailTimes = pIniFile->GetInt(
                                             CFG_SEC_CLUSTER,
                                             CFG_CLSTR_MAX_ACTIVE_FAIL_TIMES,
                                             MAX_ACTIVE_FAIL_TIMES_DEFAULT);

        //读取判断心跳中断地时间阀值
        pComCluster->m_nMaxHBBrokenTime = pIniFile->GetInt(
                                          CFG_SEC_CLUSTER,
                                          CFG_CLSTR_HEART_BEAT_BROKEN,
                                          MAX_HB_BROKEN_TIME_DEFAULT);

        //读取共享磁盘阵列设置
        pComCluster->m_bInstallArray = pIniFile->GetInt(
                                          CFG_SEC_CLUSTER,
                                          CFG_CLSTR_INSTALL_DISK_ARRAY,
                                          TRUE);

        //读取是否单机运行
        int nNoCluster = pIniFile->GetInt(
                              CFG_SEC_COMMON,
                              CFG_COMMON_KEY_NO_CLUSTER,
                              CFG_COMMON_KEY_NO_CLUSTER_DEFAULT);

        int nClusterMode = pIniFile->GetInt(
                              CFG_SEC_COMMON,
                              CFG_COMMON_KEY_CLUSTER_MODE,
                              CFG_COMMON_KEY_CLUSTER_MODE_TEST);
        if(nClusterMode == CFG_COMMON_KEY_CLUSTER_MODE_TEST)
        //ClusterMode未配置
        {
            if(nNoCluster == CFG_COMMON_KEY_NO_CLUSTER_DEFAULT)
            //NoCluster未配置
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