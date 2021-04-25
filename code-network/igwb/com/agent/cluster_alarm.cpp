/***************************************************************************
*产品名        ：iGateway Bill V200
*模块名        ：agent
*文件名        ：cluster_alarm.cpp
*描述信息      ：类CClusterAlarm的实现，该类封装对SUN SC(SUN CLUSTER)故障检
测以及告警功能
*版本说明      ：V200R002i00M00B00D1130A
*版权信息      ：版权所有（C）2001-2002 华为技术有限公司
*作者          ：邱意平
*创建时间      ：2001-10-30
*修改记录      ：
*****************************************************************************/

#include "cluster_alarm.h"
#include "cluster_agent.h"
#include "../include/frame.h"
#ifdef _PLATFORM_SOLARIS 
#include "../include/system1.h"
#endif

/********************************************************************
函数原型    public: CClusterAlarm(CCluster *ptheCluster);
功能描述    构造函数
输入参数    ptheCluster：CCluster对象指针，CClusterAlarm通过该指针
来获取SC的静态和动态信息
输出参数    无
返回值          无
抛出异常    无
*********************************************************************/
CClusterAlarm:: CClusterAlarm(CCluster* ptheCluster)
{
    //在单机环境下，构造函数中传入的ptheCluster指针必须为空
    m_ptheCluster   = ptheCluster;
    m_bAlarmRestore = FALSE;
    
    char szFile[MAX_PATH]; 
    
    //读配置文件
    CINIFile* pINIFile = NULL;
    pINIFile = new CINIFile((char *)::GetCfgFilePath());
    pINIFile->Open();
    
    //读服务资源类型(FTP或者FTAM)
    char szResType[64];
    
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_SERVICE_RES, 
        DEFAULT_SERVICE_RES, szResType, 64);
    StringToUpper(szResType);
    m_sResourceType = szResType;

    if (m_ptheCluster == NULL) //单机环境，从配置文件中读 
    {
        m_uVMType = (VM_TYPE)pINIFile->GetInt(CFG_SEC_ALM_AGENT, 
            CFG_VM_TYPE, VT_VXVM);
    } 
    else  //双机环境
    {
        m_uVMType = m_ptheCluster->GetVMType();
    }
    

    
    //读前后存盘所在的磁盘组名和卷名
    char szOutput[MAX_PATH];
    
    //读前存盘所在的逻辑卷名
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_FRONT_VOL, 
        DEFAULT_FRONT_VOL, szOutput, 254);
    m_sFrontVolume = szOutput;
    
    
    //读前存盘所在的磁盘组名
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_FRONT_DG, 
        DEFAULT_FRONT_DG, szOutput, 254);
    m_sFrontDG = szOutput;
    
    //读后存盘所在的逻辑卷名 
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_BACK_VOL, 
        DEFAULT_BACK_VOL, szOutput, 254);
    m_sBackVolume = szOutput;
    
    //读后存盘所在的磁盘组名
    pINIFile->GetString(CFG_SEC_ALM_AGENT, CFG_BACK_DG, 
        DEFAULT_BACK_DG, szOutput, 254);
    m_sBackDG = szOutput;
    
    //创建告警列表
    
    m_pAlarmList = new LIST<SInnerAlarm*>;
    
    //将心跳告警加入告警检测列表
    BYTE yAlarmPara[MAX_ALARM_PARA];
	memset(yAlarmPara, 0xFF, MAX_ALARM_PARA);

    GetAlarmItem(ALARM_ID_HEARTBEAT, yAlarmPara);
    
    //将倒换告警加入告警检测列表
    GetAlarmItem(ALARM_ID_FAILOVER, yAlarmPara);
    
    //在双机环境下将私网告警加入告警检测列表
    if (m_ptheCluster)
    {
        int nPrvNetNum = m_ptheCluster->GetNumOfPrvNet();
        for (int i = 0; i < nPrvNetNum; i++)
        {
			yAlarmPara[0] = i;
            GetAlarmItem(ALARM_ID_PRVNET, yAlarmPara);
        }
    }
    
    //将服务失效告警加入告警检测列表 
    if (m_sResourceType.find("FTP"))
        yAlarmPara[0] = (BYTE)0;                //FTP服务
    else
        yAlarmPara[0] = (BYTE)1;                //FTAM服务
    GetAlarmItem(ALARM_ID_SVCRES, yAlarmPara);
    
    //将前存盘卷失效告警加入告警检测列表
	yAlarmPara[0] = FRONT_VOLUME;
    GetAlarmItem(ALARM_ID_VOLUME, yAlarmPara);
    
    //将后存盘卷失效告警加入告警检测列表
	yAlarmPara[0] = BACK_VOLUME;
    GetAlarmItem(ALARM_ID_VOLUME, yAlarmPara);
    
    //将硬盘故障告警加入告警检测列表 
	yAlarmPara[0] = ALARM_PARA_NONE;
    GetAlarmItem(ALARM_ID_DISK, yAlarmPara);
    
    //将IP资源失效告警加入告警检测列表
	yAlarmPara[0] = ALARM_PARA_NONE;
    GetAlarmItem(ALARM_ID_IPRES, yAlarmPara);
    
    delete pINIFile;
}

/********************************************************************
函数原型    public: ~CClusterAgent(void);
功能描述    CClusterAgent类的析构函数
输入参数    无
输出参数    无
返回值          无
抛出异常    无
*********************************************************************/
CClusterAlarm:: ~CClusterAlarm(void)
{
    LIST<SInnerAlarm*>::iterator it;
    
    if (m_pAlarmList)
    {
        //依次删除告警列表中的告警元素
        for (it = m_pAlarmList->begin(); it != m_pAlarmList->end();
        it++)
        {
            delete (*it);
        }
        
        m_pAlarmList->clear();
        delete m_pAlarmList;
    }
}

/********************************************************************
函数原型    private: PInnerAlarm GetAlarmItem(UINT2 uAlarmID, 
BYTE yAlarmPara);
功能描述    根据告警号、告警参数在告警列表中查找指定的告警项，如果
没有则创建指定告警项并插入告警列表。
输入参数    uAlarmID  ：告警号 
yAlarmPara：告警参数（0xFF表示无告警参数）
输出参数    无
返回值          指向指定告警项的指针
抛出异常    无
*********************************************************************/
PInnerAlarm CClusterAlarm:: GetAlarmItem(UINT2 uAlarmID, BYTE* yAlarmPara)
{
    LIST<SInnerAlarm*>::iterator it;
    SInnerAlarm* pAlarm = NULL;
    
    //遍历告警列表，查找ID和参数均匹配的告警项
    for (it = m_pAlarmList->begin(); it != m_pAlarmList->end();
    it++)
    {
        pAlarm = (*it);
        //告警ID、告警参数全匹配
        if ((pAlarm->uAlarmID == uAlarmID) && 
             memcmp(pAlarm->yAlarmPara, yAlarmPara, MAX_ALARM_PARA) == 0 )

        {
            return pAlarm;
        } 
    }
    
    //如果没有查找到，则创建一个对应的告警项 
    pAlarm = new SInnerAlarm;
    pAlarm->uAlarmID   = uAlarmID;
    memcpy(pAlarm->yAlarmPara, yAlarmPara, MAX_ALARM_PARA);
    pAlarm->yAlarmType = AT_NOT_DEFINE;
    
    m_pAlarmList->push_back(pAlarm);
    
    return pAlarm;
}

/*********************************************************************
函数原型    private: void SetAlarmItem(PInnerAlarm pAlarmItem, 
BOOL bStatus);
功能描述    设置指定告警项的告警类型。
输入参数    bStatus：告警项状态，FALSE表示产生了告警，TRUE表示告警
恢复。
输出参数    无
返回值  无
抛出异常    无
**********************************************************************/
void CClusterAlarm:: SetAlarmItem(PInnerAlarm pAlarmItem,
                                  BOOL bStatus)
{
    if (bStatus == FALSE)
    {
        //产生故障告警
        pAlarmItem->yAlarmType = AT_FAULT;
    }
    else
    {
        //产生恢复告警
        switch(pAlarmItem->yAlarmType)
        {
        case AT_FAULT:
            pAlarmItem->yAlarmType = AT_RESTORE;
            break;
            
        case AT_RESTORE:
            pAlarmItem->yAlarmType = AT_NOT_DEFINE;
            break;
        }
    }
}

/*********************************************************************
函数原型    public: void AlarmPrvNet(void);
功能描述    检查每条私网的状态，并产生相应故障告警或恢复告警；如果
全部私网中断，则产生心跳全部中断告警及恢复告警。
输入参数    无
输出参数    无
返回值          无
抛出异常    无
**********************************************************************/
void CClusterAlarm:: AlarmPrvNet(void)
{
    int nrOfAvailPrvNet = 0;    //记录可用私网数量
    SInnerAlarm* pAlarm;
    
    //检查双机环境下本节点上集群系统是否在运行
    //by ldf 2002.4.22 对应问题单 D13065
    if ( (m_ptheCluster == NULL) || (!m_ptheCluster->IsClusterRunning()) )
    //修改完毕
    {
        return;
    }
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, 0xff, MAX_ALARM_PARA);

    //获得集群当前成员数，如果只有一个节点激活，则nrOfAvailPrvNet=0，
    //则应当发送心跳中断告警
    if (m_ptheCluster->GetCurrMemNum() >= 2)
    {
    	

        int nNumOfPrvNet = m_ptheCluster->GetNumOfPrvNet();
        
        //检查每条私网的状态
        for (int i = 0; i < nNumOfPrvNet; i++)
        {
            yAlarmPara[0] = (BYTE)i;
        	pAlarm = GetAlarmItem(ALARM_ID_PRVNET, yAlarmPara);
            
            //获得私网i的当前状态
            if (m_ptheCluster->GetPrvNetStatus(i))
            {
                SetAlarmItem(pAlarm, TRUE);
                nrOfAvailPrvNet++;
            }
            else //私网故障
            {
                SetAlarmItem(pAlarm, FALSE);
            } 
        }
    }
    
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
    pAlarm = GetAlarmItem(ALARM_ID_HEARTBEAT, yAlarmPara);  //心跳中断

    if (nrOfAvailPrvNet == 0)
    {
        SetAlarmItem(pAlarm, FALSE);   //发送心跳中断告警
    }
    else
    {
        SetAlarmItem(pAlarm, TRUE);    //心跳正常
    }
}

/*********************************************************************
函数原型    public: void AlarmSwitch(void);
功能描述    发送双机倒换告警(注：在单机环境下不调用此函数)。
双机代理在检测双机是否倒换时采用如下方法：在SC的管理文
件系统根目录下存在一个master文件，文件中保存激活的SC主
机名，任何一端的主机激活后将自己的主机名替代原文件中的
内容，这样，当一台主机激活时，如果发现mater文件中的主机
名不是自身的主机名，即可判断发生了倒换。
输入参数    无
输出参数    无
返回值          无
抛出异常    无
**********************************************************************/
void CClusterAlarm:: AlarmSwitch(void)
{
    //在单机环境下不作倒换检测
    if (m_ptheCluster == NULL)   return;
    
    char szMaster[MAX_PATH];
    SNPRINTF(szMaster, MAX_PATH, "/var/other/master");
    szMaster[254] = '\0';
    
    const STRING sLocalNodename = m_ptheCluster->GetLocalNodename();   //获得本机主机名
    
    //打开master文件
    FILE* pMaster = fopen(szMaster, "r");
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
    
    if (pMaster != NULL)
    {
        char szLine[1024];
        fgets(szLine, 1023, pMaster);
        fclose(pMaster);   //关闭文件
        
        StringLRTrim(szLine);
        //比较master文件中的节点名是否等于本节点名
        if (strcmp(sLocalNodename.c_str(), szLine) == 0)
        {
            return;    //相等，没有倒换
        }
        else           //不相等，发送倒换告警
        {
            SInnerAlarm* pAlarm;
            
            //获得倒换告警
            pAlarm = GetAlarmItem(ALARM_ID_FAILOVER, yAlarmPara);
            SetAlarmItem(pAlarm, FALSE);

            //by ldf 2003-08-07,对应问题单SWPD00823
            CMsgProcessor::SendAlarm(pAlarm);
            //end			
        }
    }
    
    //打开master文件并删除文件中的内容
    pMaster = fopen(szMaster, "wb");
    if ( NULL == pMaster )
    {
        return;
    }

    //将本节点名写入
    fwrite(sLocalNodename.c_str(), sLocalNodename.length(), 1, pMaster);
    fclose(pMaster);
}

/**********************************************************************
函数原型    public:  void AlarmSwitchRestore(void);
功能描述    发送双机倒换恢复告警。
输入参数    无
输出参数    无
返回值          无
抛出异常    无
***********************************************************************/
void CClusterAlarm:: AlarmSwitchRestore(void)
{
    if (m_bAlarmRestore)   // 倒换已经恢复
    {
        return;
    }
    
    //获得当前集群成员数
    if (m_ptheCluster->GetCurrMemNum() < 2)
    {
        return;    //只有一台在运行
    }
    
    m_bAlarmRestore = TRUE;
    SInnerAlarm* pAlarm;
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);

    
    //发送倒换恢复告警
    pAlarm = GetAlarmItem(ALARM_ID_FAILOVER, yAlarmPara);
    SetAlarmItem(pAlarm, TRUE);
}

/***********************************************************************
函数原型    public:  void AlarmVolume(UINT2 uFlag);
功能描述    检查前后存盘逻辑卷状态，并产生相应的故障告警和恢复告警。
输入参数    uFlag：指定前存盘或后存盘逻辑卷，可以取以下值： 
            #define FRONT_VOLUME  0 
            #define BACK_VOLUME   1
输出参数    无
返回值      无
抛出异常    无
************************************************************************/
void CClusterAlarm:: AlarmVolume(UINT2 uFlag)
{
    PInnerAlarm pAlarm = NULL;
    BOOL bStatus = TRUE;       //记录逻辑卷的当前状态
    
    //检查双机环境下本节点上集群系统是否在运行
    //by ldf 2002.4.22 对应问题单 D13065
    if ( (m_ptheCluster == NULL) || (!m_ptheCluster->IsClusterRunning()) )
    //修改完毕
    {
        return;
    }    
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, 0xff, MAX_ALARM_PARA);

    STRING sDiskGroup;
    STRING sVolume;
    
    switch (uFlag)
    {
        //检测前存盘所在的逻辑卷
    case FRONT_VOLUME:
        yAlarmPara[0] = (BYTE)1;
        sDiskGroup = m_sFrontDG;
        sVolume    = m_sFrontVolume;
        break;
        
        //检测后存盘所在的逻辑卷
    case BACK_VOLUME:
        yAlarmPara[0] = (BYTE)1;
        sDiskGroup = m_sBackDG;
        sVolume    = m_sBackVolume;
        break;
        
    default:
        return;
    }

    
    char szCmdLine[MAX_PATH];
    FILE* fp = NULL;
    char szLine[1024];
    STRING sLine;
    
    switch (m_uVMType)
    {
    case VT_SDS:
        sprintf(szCmdLine, "metastat %s/%s > agent.tmp", 
            sDiskGroup.c_str(), sVolume.c_str());
        
#ifdef _PLATFORM_SOLARIS 
        system1(szCmdLine);     //调用system()获得命令的输出
#else
        system(szCmdLine);     //调用system()获得命令的输出
#endif
        
        if ((fp = fopen("agent.tmp", "r")) == NULL)
        {
            return; 
        }
        
        //检查所有逻辑卷的状态是否为"Okay"
        while (!feof(fp))
        {
            if(fgets(szLine, 1023, fp) != NULL)
            {
                sLine = szLine;
                if ((sLine.find("State:") != -1) 
                    && (sLine.find("Okay") == -1))
                {
                    bStatus = FALSE;
                    fclose(fp);
                    break;
                }
            }
            strcpy (szLine,"");
        }
        
        fclose(fp); 
        break;
        
    case VT_VXVM:
        sprintf(szCmdLine, "vxinfo -g %s %s > vxinfo.tmp", 
            sDiskGroup.c_str(), sVolume.c_str());
#ifdef _PLATFORM_SOLARIS 
        system1(szCmdLine);     //调用system()获得命令的输出
#else
        system(szCmdLine);     //调用system()获得命令的输出
#endif
        
        if ((fp = fopen("vxinfo.tmp", "r")) == NULL)
        {
            return;
        }
        
        //检查所有逻辑卷是否都"Started"
        while (!feof(fp))
        {
            if(fgets(szLine, 1023, fp) !=NULL)
            {
                sLine = szLine;
                
                //李杜芳 这里做了修改，因为产生的agent.tmp文件没有State:字样，而且只有一行
                //如：front fsgen Started
                if (sLine.find("Started") == -1)
                {
                    bStatus = FALSE;
                    fclose(fp);
                    break;
                }
            }
            strcpy(szLine,"");
        }
        
        fclose(fp);
        remove("vxinfo.tmp");
        break;
        
    default:
        return;
    }
    
    //发送告警或者恢复告警
    pAlarm = GetAlarmItem(ALARM_ID_VOLUME, yAlarmPara);
    SetAlarmItem(pAlarm, bStatus);
}

/***********************************************************************
函数原型    public:  void AlarmResource(void);
功能描述    检查FTP或FTAM的状态，并产生相应的故障告警和恢复告警
输入参数    无
输出参数    无
返回值          无
抛出异常    无
************************************************************************/
void CClusterAlarm:: AlarmResource(void)
{
}

/***********************************************************************
函数原型    public:  void AlarmIPs(void);
功能描述    检查所有动态IP地址资源的状态，并产生故障告警和恢复告警
利用SC提供的命令"pnmstat -l"可以检查SC双机虚拟IP的状态
例如执行"pnmstat -l"命令输出：
bkggrp  r_adp     status  fo_time live_adp
nafo0   hme0:hme3   OK      NEVER   hme0
该信息表明虚拟IP nafo0当前处于"OK"状态。

  输入参数  无
  输出参数  无
  返回值            无
  抛出异常  无
***********************************************************************/
void CClusterAlarm:: AlarmIPs(void)
{
    PInnerAlarm  pAlarm = NULL;
    
    //检测双机环境下本节点上集群系统是否在运行
    //by ldf 2002.4.22 对应问题单 D13065
    if ( (m_ptheCluster == NULL) || (!m_ptheCluster->IsClusterRunning()) )
    //修改完毕
    {
        return;
    }

    //*sc3.0* 此处的GetNAFO，返回结果将变为qfe0，qfe1，qfe2，qfe3，然后利用这些来作判断，而不是 nafo，nafo1...
    //*sc3.0* 这里的NAFO其实是做了计数用，SC3.0我们可以直接使用返回值，qfe0,qfe1,qfe2,qfe3...,
    //*sc3.0* 或许顺序不一样，我们也可以用来计数

    LIST<STRING> pNAFO = m_ptheCluster->GetNAFO();
    
    //注意下面命令行中的"-l"是字母l而非数字1。
    char szCmdLine[MAX_PATH] = "pnmstat -l > pnmstat.tmp";
    
#ifdef _PLATFORM_SOLARIS 
    system1(szCmdLine);     //调用system()获得命令的输出
#else
    system(szCmdLine);     //调用system()获得命令的输出
#endif
    FILE* fp = NULL;
    
    if ((fp = fopen("pnmstat.tmp", "r")) == NULL)
    {
        return;
    } 
    
    int i = 0;
    LIST<STRING>::iterator it;
    char szLine[1024];
    STRING sLine;
    BOOL bStatus = TRUE;
    char szchar = '0' ;
    STRING sNAFO;
    char szbuf[10];
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);
    
    for (it = pNAFO.begin(); it != pNAFO.end(); it++)
    {
       //root@igwb2 # pnmstat -l
       //group   adapters        status  fo_time act_adp
       //nafo0   qfe0            OK      NEVER   qfe0
       //nafo1   qfe3            DOWN    NEVER   qfe3
       //nafo2   qfe1            OK      NEVER   qfe1
       //nafo3   qfe2            OK      NEVER   qfe2

        //所生成的文件agent.tmp中为nafe0,nafe1,nafe2,nafe3...

        sprintf(szbuf, "nafo%c",szchar);
        sNAFO = szbuf;
        rewind(fp);   //回转文件指针
        yAlarmPara[0] = (BYTE)i;
        pAlarm = GetAlarmItem(ALARM_ID_IPRES, yAlarmPara);
        
        while (!feof(fp))
        {
            if(fgets(szLine, 1023, fp) !=NULL)
            {
                sLine = szLine;
                //检查虚拟IP的状态
                if ((sLine.find(sNAFO) != -1)
                    && (sLine.find("OK")) == -1)
                {
                    bStatus = FALSE;
                    break;
                }
            }
            strcpy (szLine,"");
        }
        
        szchar++;
        SetAlarmItem(pAlarm, bStatus);
        bStatus = TRUE;  //检查下一个
        i++;             //IP资源参数
    } 
    fclose(fp);
    remove("pnmstat.tmp");
}

/************************************************************************
函数原型    public:  void AlarmDisks(void);
功能描述    检查共享磁盘状态，并产生相应故障告警和恢复告警
输入参数    无
输出参数    无
返回值      无
抛出异常    无
*************************************************************************/
void CClusterAlarm:: AlarmDisks(void)
{
    PInnerAlarm pAlarm = NULL;
    BOOL bStatus = TRUE;   //记录硬盘状态，TRUE表示硬盘无故障
    
    //by ldf 2002.4.22 对应问题单 D13065
    if ( (m_ptheCluster == NULL) || (!m_ptheCluster->IsClusterRunning()) )
    //修改完毕
    {
        return;
    }
    
    LIST<STRING> DGList;
    LIST<STRING>::iterator it;
    
    if (m_ptheCluster == NULL) 
    {
        DGList.push_back(m_sFrontDG);
        DGList.push_back(m_sBackDG);
    }
    else
    {
        LIST<STRING> pDG = m_ptheCluster->GetDiskGroup();
        
        for (it = pDG.begin(); it != pDG.end(); it++)
        {
            DGList.push_back(*it);
        } 
    }
    
    FILE* fp = NULL;
    char szCmdLine[MAX_PATH];
    char szLine[1024];
    STRING sLine;
    
    
    LIST<STRING>::iterator itCheck;
    LIST<STRING> DGListCheck;
    char temp[100];
    char buf[100];
    STRING sLineCheck;
    
    
    switch (m_uVMType)
    {
    case VT_SDS:
        for (it = DGList.begin(); it != DGList.end(); it++)
        {
            sprintf(szCmdLine, "metastat -s %s > agent.tmp", (*it).c_str());
#ifdef _PLATFORM_SOLARIS 
            system1(szCmdLine);     //调用system()获得命令的输出
#else
            system(szCmdLine);     //调用system()获得命令的输出
#endif
            
            fp = fopen("agent.tmp", "r");
            if (fp == NULL)
            {
                return;
            }
            
            while (!feof(fp))
            {
                if(fgets(szLine, 1023, fp) != NULL)
                {
                    sLine = szLine;
                    
                    //如果输出的状态部是Okay，则表明硬盘出现问题
                    if ((sLine.find("State:") != -1)
                        && (sLine.find("Okay") == -1))
                    {
                        bStatus = FALSE;
                        
                        fclose(fp);
                        fp = NULL;
                        
                        remove("agent.tmp");
                        
                        break;
                    }
                }
                strcpy (szLine,"");
            }            
            
            if (bStatus == FALSE)
            {
                break;
            }
            
            if (fp != NULL)
            {
                fclose(fp);
                fp = NULL;
                
                remove("agent.tmp");
            }
        } 
        
        break;
        
    case VT_VXVM:
        //李杜芳 这里根据实际情况进行了修改
        for (it = DGList.begin(); it != DGList.end(); it++)
        {
            sprintf(szCmdLine, "vxdisk -g %s list > vxdisk.tmp", (*it).c_str());
#ifdef _PLATFORM_SOLARIS 
            system1(szCmdLine);     //调用system()获得命令的输出
#else
            system(szCmdLine);     //调用system()获得命令的输出
#endif
            
            fp = fopen("vxdisk.tmp", "r");
            if (fp == NULL)
            {
                return;
            }
            //略过第一行DEVICE       TYPE      DISK         GROUP        STATUS
            fgets(szLine, 1023, fp);
            
            //把各个DISK放入DGListCheck列表中，DISK在第三个位置(如 igwbdg01)。
            //行的格式如：c1t0d0s2     sliced    igwbdg01     igwbdg       online
            while (!feof(fp))
            {
                if(fgets(szLine, 1023, fp) != NULL)
                {
                    sscanf(szLine,"%s%s%s",temp,temp,buf);
                    sLineCheck = buf;
                    DGListCheck.push_back(sLineCheck);
                }
            }
            
            
            if (fp != NULL)
            {
                fclose(fp);
                fp = NULL;
                
                remove("vxdisk.tmp");
            }
            
            
            //判断磁盘的状态，如果是Okay则表明工作正常
            for (itCheck = DGListCheck.begin(); itCheck != DGListCheck.end(); itCheck++)
            {
                sprintf(szCmdLine, "vxdisk check %s > vxdisk.tmp", (*itCheck).c_str());
#ifdef _PLATFORM_SOLARIS 
                system1(szCmdLine);     //调用system()获得命令的输出
#else
                system(szCmdLine);     //调用system()获得命令的输出
#endif
                
                fp = fopen("vxdisk.tmp", "r");
                if (fp == NULL)
                {
                    return;
                }
                
                
                while (!feof(fp))
                {
                    if(fgets(szLine, 1023, fp) !=NULL)
                    {
                        sLine = szLine;
                        
                        //如果输出的状态不是Okay，则表明硬盘出现问题
                        if (sLine.find("Okay") == -1)
                        {
                            bStatus = FALSE;
                            
                            fclose(fp);
                            fp = NULL;
                            
                            remove("vxdisk.tmp");
                            
                            break;
                        }
                    }
                }            
                
                if (bStatus == FALSE)
                {
                    break;
                }
                
                if (fp != NULL)
                {
                    fclose(fp);
                    fp = NULL;
                    
                    remove("vxdisk.tmp");
                }
            } 
            if (bStatus == FALSE)
            {
                break;
            }
            
        }   
        break; 
        
        
    default:
        break;
    }    
    
    BYTE yAlarmPara[MAX_ALARM_PARA];
    memset(yAlarmPara, ALARM_PARA_NONE, MAX_ALARM_PARA);    
    pAlarm = GetAlarmItem(ALARM_ID_DISK, yAlarmPara);

    SetAlarmItem(pAlarm, bStatus); 
}

/************************************************************************
函数原型    public:  void Alarm(void);
功能描述    在单、双机不同环境下分别调用相应函数产生故障告警和恢复告警
输入参数    无
输出参数    无
返回值          无
抛出异常    无
*************************************************************************/
void CClusterAlarm:: Alarm(void)
{
    //以下告警只在双机环境下检测
    if (m_ptheCluster)
    {
        AlarmPrvNet();              //私网中断告警
        AlarmSwitchRestore();       //倒换恢复告警
        AlarmIPs();                 //IP资源失效告警
    }
    
    AlarmResource();                //资源失效告警
    AlarmVolume(FRONT_VOLUME);      //卷失效告警(前存盘所在的逻辑卷)
    AlarmVolume(BACK_VOLUME);       //卷失效告警(后存盘所在的逻辑卷)
    AlarmDisks();                   //硬盘故障告警
    
    //定义：
    LIST<SInnerAlarm*>::iterator it;
    SInnerAlarm* pAlarm = NULL;
    
    //遍历告警项，发送告警/恢复告警
    for (it = m_pAlarmList->begin(); it != m_pAlarmList->end();
    it++)
    {
        pAlarm = (*it);
        if (pAlarm->yAlarmType != AT_NOT_DEFINE)
        {
            CMsgProcessor::SendAlarm(pAlarm);
        }
    } 
}
