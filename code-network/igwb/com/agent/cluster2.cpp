/***************************************************************************
 *产品名      ：iGateway Bill V200
 *模块名      ：agent
 *文件名      ：Cluster.cpp
 *描述信息    ：
 对于SC2.2
 类CCluster的实现，该类封装了获得SUN SC集群系统的配置以及运行时信息的功能。
 SC配置文件很多，这里主要使用的有：
 1. /etc/opt/SUNWcluster/conf/clustername.cdb   集群的静态配置文件(只读)
    ，其中clustername为相应的集群名。从CDB(Cluster Database)文件可得到集
    群的名字、节点数、各个节点的名称、私网数、IP、卷管理软件类型、Quorum
    等静态的配置信息，文件格式为名/值对的形式;
 2. /etc/opt/SUNWcluster/conf/ccd.database   集群的动态配置数据库(只读)，
    CCD(Cluster Configuration Database)文件包含了逻辑主机配置、数据服务状
    态和数据服务实例的管理信息等。PNM状态改变、逻辑主机倒换、逻辑主机的状态
    改变等导致集群重配置的情况都会立即反映到CCD文件中;
 3. /var/opt/SUNWcluster/clustername/cluster_is_running 此文件是否存在来表示
    集群是否运行，其中clustername为相应集群名;
 4. /var/opt/SUNWcluster/clustername/ccm.active_nets  当前活动的私有网络;
 5. /var/opt/SUNWcluster/clustername/ccm.selected    当前选择的私有网络;

 *版本说明    ：V200R002i00M00B00D1130A
 *版权信息    ：版权所有（C）2001-2002 华为技术有限公司
 *作者        ：李杜芳
 *创建时间    ：2002-10-25
 *修改记录    ：
*****************************************************************************/
#include "cluster2.h"
#include "../include/frame.h"
#ifdef _PLATFORM_SOLARIS 
#include "../include/system1.h"
#endif

/******************************************************************
函数原型    public:  CCluster(const char *pszConfigFile);
功能描述    CCluster类的构造函数，初始化有关集群的所有配置信息。
输入参数    pszConfigFile：配置文件名，如果为空，则使用缺省配置文件。
输出参数    无
返回值     无
抛出异常    无
*******************************************************************/
CCluster2:: CCluster2(const char* pszConfigFile) : CCluster(pszConfigFile)
{
    char szFile[MAX_PATH];

    CINIFile* pINIFile = new CINIFile((char *)pszConfigFile);
    pINIFile->Open();     //打开参数配置文件billsvr.ini
    
    char szSCDir[MAX_PATH];       //SC软件安装路径 
    //读SC安装点
    pINIFile->GetString(CFG_SEC_CLUSTER, CFG_CLUSTER_PATH, 
        DEFAULT_CLUSTER_PATH, szSCDir, MAX_PATH);
    szSCDir[MAX_PATH - 1] = '\0';
    m_sSCDir = szSCDir;
    
    //读逻辑主机名  
    char szLogHost[255];
    pINIFile->GetString(CFG_SEC_CLUSTER, CFG_LOG_HOST, 
        DEFAULT_LOG_HOST, szLogHost, 254);
    
    m_LogicalHost.sLogHostName = szLogHost;

    //读高可用数据服务名 
    char szDataService[255];
    pINIFile->GetString(CFG_SEC_CLUSTER, CFG_DATA_SERVICE, 
        DEFAULT_DATA_SERVICE, szDataService, 254);

    m_LogicalHost.sDataServiceName = szDataService;
    
    //读SC节点个数，该参数位于/etc/opt/SUNWcluster/conf/clustername.cdb
    //文件的"cluster.number.node"中 
    SNPRINTF(szFile, MAX_PATH, "%s/conf/%s.cdb", szSCDir, m_sCluster.c_str()); 
    szFile[MAX_PATH - 1] = '\0';
    
    GetNodes(szFile,m_LogicalHost.Cluster_Nodes) ;    
    
    //获得SC私网个数(一般为两条私网)  
    //该参数也位于"clustername.cdb"文件中
    m_nrOfPrvNet = GetPrvNetNum(szFile);
    
    //通过SC的数据库配置文件ccd.database获得共享磁盘组
    SNPRINTF(szFile, MAX_PATH, "%s/conf/ccd.database", szSCDir);
    szFile[MAX_PATH - 1] = '\0';
    STRING sLogHost;
    
    ReadLOGHOST(szFile, sLogHost, m_LogicalHost.Cluster_Nodes, m_LogicalHost.Cluster_Diskgroups);
    
    //通过SC的数据库配置文件ccd.database获得NAFO组
    ReadLogIP(szFile, m_LogicalHost.Cluster_NAFO);
    
    delete pINIFile;  //CINIFile类在析构函数中会关闭INI配置文件句柄

}

/******************************************************************
函数原型  CCluster2::  GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
功能描述    获得节点个数以及节点名
输入参数    szFilePath :文件的路径
          Cluster_Nodes :用来存放结果
输出参数    无
返回值     无
抛出异常    无
*******************************************************************/ 
BOOL CCluster2:: GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
{
    char szMatch[255] = "cluster.number.nodes";
    char szOutput[255];
    
    const char* szFilePath = szNodesInfo;
    FILE* fp  = fopen(szFilePath, "r");
    if (fp == NULL)
    {
       return FALSE;
    }
    
    BOOL bFound = GetSCConfig(fp, szMatch, szOutput);
    
    if (bFound == FALSE)  
    {
        fclose(fp);
        return FALSE; //默认情况下为两个节点
    }
    
    int nNodeNum = (atoi(szOutput));
    
    char  SZNodeName[MAX_PATH];
    
    int nNode = 0;
    for (nNode ; nNode <= nNodeNum ; nNode++ )
    {
        SNPRINTF(szMatch, 255, "cluster.node.%d.hostname", nNode);
        szMatch[254] = '\0';
        
        GetSCConfig(fp, szMatch, SZNodeName);
        
        if (strlen(SZNodeName) != 0)
        {
            Cluster_Nodes.push_back(SZNodeName);
        }
        strcpy (SZNodeName ,"");
    }
    
    fclose(fp);

    return TRUE ;
}

/******************************************************************
函数原型    public: const CCluster2:: STRING  GetSCDir(void);
功能描述    返回集群软件的安装路径
输入参数    无
输出参数    无
返回值       返回指向集群安装路径。
抛出异常    无
*******************************************************************/
const STRING CCluster2:: GetSCDir(void)
{
    return m_sSCDir;
}

/******************************************************************
函数原型    public: CCluster2:: const STRING  GetLogicalHost(void);
功能描述    获得逻辑主机名
输入参数    无
输出参数    无
返回值       返回逻辑主机名。
抛出异常    无
*******************************************************************/
const STRING CCluster2:: GetLogicalHost(void)
{
    return m_LogicalHost.sLogHostName;
}

/******************************************************************
函数原型    public: CCluster2:: const STRING GetDataService(void);
功能描述    获得高可用数据服务名
输入参数    无
输出参数    无
返回值       返回数据服务名。
抛出异常    无
*******************************************************************/
const STRING CCluster2:: GetDataService(void)
{
    return m_LogicalHost.sDataServiceName;
}

/******************************************************************
函数原型    public:  CCluster2:: BOOL  IsClusterRunning(void);
功能描述    检查当前本节点上集群系统是否正在运行
输入参数    无
输出参数    无
返回值       若本节点上集群系统还在运行则返回TRUE
抛出异常    无
*******************************************************************/
BOOL CCluster2:: IsClusterRunning(void)
{
    char szFile[MAX_PATH]; 
    
    SNPRINTF(szFile, MAX_PATH, "/var/opt/SUNWcluster/%s/cluster_is_running", 
        m_sCluster.c_str());
    szFile[MAX_PATH - 1] = '\0';

    //检测cluster_is_running文件是否存在
    if (access(szFile, F_OK) < 0)
    {
      return FALSE;
    }
    else
    {
      return TRUE;
    }
}

/**************************************************************************
函数原型    public: CCluster2:: int  GetCurrMemNum(void);
功能描述    获得当前逻辑主机中活动成员数量
输入参数    无
输出参数    无
返回值       返回逻辑主机中活动成员数量，返回值为0表示不确定。
抛出异常    无
***************************************************************************/
int CCluster2:: GetCurrMemNum(void)
{
    //如果检测到SC没有运行，则无法确定当前活动的成员个数
    //函数返回0表示不确定
    if (IsClusterRunning() == FALSE)
    {
       return 0;
    }

    char szFile[MAX_PATH];
    SNPRINTF(szFile, MAX_PATH, "%s/conf/ccd.database", m_sSCDir.c_str());
    szFile[MAX_PATH - 1] = '\0';
    
    int nMem = GetSCCurrMemNum(szFile);

    return nMem;
}

/**************************************************************************
函数原型    public: CCluster2:: BOOL   GetPrvNetStatus(int nPrvNetID);
功能描述    获取指定私网ID的当前状态，通过读SC的动态配置文件ccm.active_nets
           可以获得SC当前活动私网ID列表，例如"0  1"表示当前0号私网和1号
           私网处于活动状态。
输入参数    nPrvNetID －－ 私网编号
输出参数    无
返回值       如果指定私网状态正常则返回TRUE
抛出异常    无
***************************************************************************/
BOOL CCluster2:: GetPrvNetStatus(int nPrvNetID)
{
    char szFile[MAX_PATH];

    SNPRINTF(szFile, MAX_PATH, "/var/opt/SUNWcluster/%s/ccm.active_nets", 
         m_sCluster.c_str());
    szFile[MAX_PATH - 1] = '\0';
    
    //从配置文件中获得私网ID列表
    LIST<int> PrvNets;
    GetActivePrvNets(szFile, PrvNets);
    
    LIST<int>::iterator it;
    for (it = PrvNets.begin(); it != PrvNets.end(); it++)
    {
       if (nPrvNetID == (*it))
       {
         return TRUE;    //该私网当前处于激活状态
       }
    }
   
    return FALSE;
}

/************************************************************************
函数原型    private: CCluster2:: int GetSCCurrMemNum(const char* szFilePath);
功能描述      读SC的数据库文件ccd.database文件中的"CLUSTM:"字段，例如：
           "CLUSTM:cluster1,cluster2"，则表示SC当前活动成员有cluster1
           和cluster2，则返回2
输入参数      szFilePath: ccd.database文件的全路径名。    
输出参数      无。    
返回值        SC当前活动成员数。  
抛出异常    无
*************************************************************************/
int CCluster2:: GetSCCurrMemNum(const char* szFilePath)
{
    char  szMatch[255] = "CLUSTM:";
    char  szOutput[MAX_PATH];
    
    FILE* fp  = fopen(szFilePath, "r");
    if (fp == NULL)
    {
       return 0;
    }

    BOOL bFound = GetSCConfig(fp, szMatch, szOutput);
    fclose(fp);
    
    if (bFound == FALSE)   return 0;

    int nLen = strlen(szOutput);
    if (nLen == 0)   return 0;

    int nIndex = 0;
    int nMem = 0;

    while (nIndex < nLen)
    {
       char ch = szOutput[nIndex];
       nIndex++;
       
       // 成员之间以','分隔
       if (ch == ',') nMem++;
    }

    return (nMem + 1);
}

/************************************************************************
函数原型    private: CCluster2:: BOOL GetActivePrvNets(const char* szFilePath,
                                LIST<int>& PrvNets);
功能描述      通过读ccm.active_nets文件获得SC当前活动的私网列表。
输入参数      szFilePath: ccm.active_nets文件的全路径名。 
输出参数      PrvNets   :活动私网列表。   
返回值       执行成功返回TRUE，否则返回FALSE。   
抛出异常    无
*************************************************************************/
BOOL CCluster2:: GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets)
{
    FILE* fp = fopen(szFilePath, "r");
    if (fp == NULL)
    {
      return FALSE;
    }
    
    PrvNets.clear();       //首先清空列表    

    char szLine[1024] = "";
    fgets(szLine, 1023, fp);
    StringLRTrim(szLine);    //去掉字符串左右的空格符号

    int nLen = strlen(szLine);
    if (nLen == 0)   
    {
       fclose(fp);
       return FALSE;
    }

    int nIndex = 0;
    STRING sNetID = "";
    //私网ID之间用空格分隔
    while (nIndex < nLen)
    {
       char ch = szLine[nIndex];
       if (ch == ' ')
       {
         if (sNetID.length() != 0)
         {
            PrvNets.push_back(atoi(sNetID.c_str()));
         }

         sNetID = "";  //重新清空字符串
       }
       else
       {
         sNetID += ch;
       }

       nIndex++;    //字符串索引增1
    }

    //注意处理最后一个ID
    if (sNetID.length() != 0)
    {
       PrvNets.push_back(atoi(sNetID.c_str()));
    }
    
    fclose(fp);
    
    return TRUE;
    
}

/************************************************************************
函数原型    private: CCluster2:: int   GetPrvNetNum(const char* szFilePath);
功能描述      通过读clustername.cdb文件中的cluster.number.nets段获得
           SC拥有的私网数量。例如"cluster.number.nets:2"表示SC配置
           了2条私网。
输入参数      szFilePath: clustername.cdb文件的全路径名。 
输出参数      无。    
返回值        SC拥有的私网数量。  
抛出异常    无
*************************************************************************/
int CCluster2:: GetPrvNetNum(const char* szFilePath)
{
    char szMatch[255] = "cluster.number.nets";
    char szOutput[255];
    FILE* fp = NULL;
    
    fp = fopen(szFilePath, "r");
    if (fp == NULL) 
    {
       return 2;
    } 

    BOOL bFound = GetSCConfig(fp, szMatch, szOutput);
    fclose(fp);

    if (bFound == FALSE)
    {
       return 2;   //默认为有两个私网
    }
       
    return atoi(szOutput);
}


/************************************************************************
函数原型    private: CCluster2:: BOOL   GetMaster(const char* szLogHostName,
                            char* szMaster);
功能描述      获得SC主机节点名，通过haget命令获得。   
           例如：在SC上执行命令"haget -f master -h hasyb"，命令
           返回cluster1，则cluster1就是SC当前的节点名。
输入参数      szLogHostName:逻辑主机名。    
输出参数      szMaster    :主机节点名。  
返回值       如果成功获得SC管理软件的安装点，则返回TRUE，否则返回FALSE。 
抛出异常    无
*************************************************************************/
BOOL CCluster2:: GetMaster(const char* szLogHostName, char* szMaster)
{
    if (szMaster == NULL)   return FALSE;

    char szCmd[255];
    SNPRINTF(szCmd, 255, "haget -f master -h %s > agent.tmp", szLogHostName);
    szCmd[254] = '\0';
#ifdef _PLATFORM_SOLARIS 
    system1(szCmd);
#else
    system(szCmd);
#endif

    FILE* fp = fopen("agent.tmp", "r");
    if (fp == NULL)
    {
       return FALSE;
    }

    char szLine[1024];
    fgets(szLine, 1023, fp);
    fclose(fp);

    StringLRTrim(szLine);    //去掉字符串左右的空格
    if (strlen(szLine) == 0)   
    {
       return FALSE;
    } 

    strcpy(szMaster, szLine);
    remove("agent.tmp");    //注意：删除临时文件

    return TRUE;
}

/************************************************************************
函数原型    private: CCluster2:: BOOL   ReadLOGHOST(const char* pszFilePath,
                              STRING& sLogHost,
                              LIST<STRING>& NodeName,
                              LIST<STRING>& DiskGroup);
功能描述      通过读SC的数据库配置文件ccd.database获得逻辑主机名、节点名
           共享磁盘组。    
输入参数      pszFilePath: ccd.database文件的全路径名。   
输出参数    无
返回值       如果成功获得配置则返回TRUE，否则返回FALSE。 
抛出异常    无
*************************************************************************/
BOOL CCluster2:: ReadLOGHOST(const char* pszFilePath, STRING& sLogHost,
                   LIST<STRING>& NodeName, LIST<STRING>& DiskGroup)
{
    char szMatch[255] = "LOGHOST:";
    char szOutput[1024];
    FILE* fp;
    
    fp = fopen(pszFilePath, "r");
    if (fp == NULL)
    {
       return FALSE;
    } 
    
    //读出LOGHOST:，例如"LOGHOST:hasyb:cluster1,cluster2:dg01:1:1"
    BOOL bRet = GetSCConfig(fp, szMatch, szOutput);
    fclose(fp);
    
    if (bRet == FALSE)
    {
        return FALSE;
    }
    
    StringLRTrim(szOutput);
    if (strlen(szOutput) == 0)  
    {
       return FALSE;
    }
 
    int nIndex;
    int nColon;
    STRING sLine;
    STRING sNode = "";
    char ch;
    sLine = szOutput;
    nColon = sLine.find(':');
    if (nColon == -1)   
    {
       return FALSE;
    }

    //获得逻辑主机名
    sLogHost = sLine.substr(0, nColon);
    if (sLogHost.length() == 0)  
    {
       return FALSE;
    }

    //获得节点名
    sLine = sLine.substr(nColon+1);
    const char* pChar = sLine.c_str();
    nIndex = 0;
    ch = pChar[nIndex];
    sNode = "";
    while (ch != ':')
    {
        if (ch == ',')   //多个成员之间用','分隔
       {
          if (sNode.length() != 0)
          {
             NodeName.push_back(sNode);
          }
          sNode = "";
       }
        else
       {
          sNode += ch;
       }

        nIndex++;
        ch = pChar[nIndex];
    }

    //处理最后一个节点名
    if (sNode.length() != 0)
    {
       NodeName.push_back(sNode);
    }
    sNode = "";

    nColon = sLine.find(':');
    if (nColon == -1)   return FALSE;

    sLine = sLine.substr(nColon + 1);
    const char* pDisk = sLine.c_str();
    nIndex = 0;
    ch = pDisk[nIndex];
    sNode = "";
    while (ch != ':')
    {
       if (ch == ',')   //多个成员之间用','分隔
       {
         if (sNode.length() != 0)
         {
            DiskGroup.push_back(sNode);
         }
         sNode = "";
       }
       else
       {
         sNode += ch;
       }

       nIndex++;
       ch = pDisk[nIndex];
    }
    
    //处理最后一个成员
    if (sNode.length() != 0)
    {
       DiskGroup.push_back(sNode);
    }

    return TRUE;
}

/***********************************************************************
函数原型    private: CCluster2:: BOOL   ReadLogIP(const char* pszFilePath,
LIST<STRING>& LogIP);
功能描述      通过读SC的数据库配置文件ccd.database获得SC虚拟IP配置,其中LOGIP的字段如下格式：
#
# Logical IP address
#
LOGIP_fmt:nodelist:iflist:ipaddr:logif
LOGIP_idx:0
LOGIP_sync:ccd_nofreeze
LOGIP:igwb1,igwb2:qfe3,qfe3:ha-igwb3:4
LOGIP:igwb1,igwb2:qfe2,qfe2:ha-igwb2:3
LOGIP:igwb1,igwb2:qfe1,qfe1:ha-igwb1:2
LOGIP:igwb1,igwb2:qfe0,qfe0:ha-igwb:1   
输入参数      pszFilePath: ccd.database文件的全路径名。   
输出参数    无
返回值       如果成功获得虚拟IP配置，则返回TRUE，否则返回FALSE。 
抛出异常    无

**************************************************************************/

BOOL CCluster2:: ReadLogIP(const char* pszFilePath, 
                 LIST<STRING>& LogIP)
{
    char szMatch[255] = "LOGIP:";
    char szOutput[1024];
    FILE* fp = NULL;
    
    
    fp = fopen(pszFilePath, "r");
    if (fp == NULL)  
    {
      return FALSE;
    }
    //如果没有找到匹配的字符串行，则返回FALSE
    
    BOOL bRetok = FALSE ;
    
    
    while (!feof(fp) && !ferror(fp))
    {
      BOOL bRet = GetSCConfig(fp, szMatch, szOutput);
      
      if (bRet == FALSE)
      {
        continue;
      }
      
      StringLRTrim(szOutput);
      if (strlen(szOutput) == 0)
      {
        fclose(fp);
        return FALSE;
      }
      
      char* pPos = szOutput;
      
      //修改记录： 把下面的for语句中 由 "i < 3" 改为了 "i < 2 "，原因见下行注释括号中
      //单行的格式为：(LOGIP:已经在szOutput中去掉了)igwb1,igwb2:qfe3,qfe3:ha-igwb3:4 
      //需要的字段为ha-igwb3,下面取第2个":"后的字段
      
      for(int i = 0; i < 2 ; i++)
      {
        pPos = strchr(pPos,':');
        
        if (pPos == NULL)
        {
           fclose(fp);
           return FALSE;
        }
        pPos = pPos + 1 ;    
      }   
      
      char* p = strchr(pPos,':');  
      if (p == NULL)   
      {   
        fclose(fp);
        return FALSE;
      }
      
      *p = '\0';
      
      //获得逻辑IP
      STRING sLogIP = pPos;
      
      if (sLogIP.length() == 0)  
      {
        fclose(fp);
        return FALSE;
      }
      
      LogIP.push_back(sLogIP);

      bRetok = TRUE;
    }
    
    fclose(fp);

    return  bRetok;
}


