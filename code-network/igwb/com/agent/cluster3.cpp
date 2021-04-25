/***************************************************************************
 *产品名      ：iGateway Bill V200
 *模块名      ：agent
 *文件名      ：Cluster.cpp
 *描述信息    ：
对于SC3.0
可以从scstat一次性输出到 agent.tmp文件中，然后再读取各种静态参数。
包括节点名以及数量，磁盘阵列名以及数量，心跳名以及数量，用LIST存放，数量可以查
该LIST中元素的个数得知。
  
对于集群的名字采用其他方法得到。
公网数量用  publicnet list 得到，想要监测 可以用  publicnet check qfe0...
判断是否在SC中采用 scstat -n
判断活动私有网络采用 scstat -W

 *版本说明    ：V200R002i00M00B00D1130A
 *版权信息    ：版权所有（C）2001-2002 华为技术有限公司
 *作者        ：李杜芳
 *创建时间    ：2002-10-25
 *修改记录    ：
*****************************************************************************/
#include "cluster3.h"
#include "../include/frame.h"
#ifdef _PLATFORM_SOLARIS 
#include "../include/system1.h"
#endif

/******************************************************************
函数原型    public:  CCluster(const char *szConfigFile);
功能描述    CCluster类的构造函数，初始化有关集群的所有配置信息。
输入参数    szConfigFile：配置文件名，如果为空，则使用缺省配置文件。
输出参数    无
返回值     无
抛出异常    无
*******************************************************************/
CCluster3:: CCluster3(const char* pszConfigFile) : CCluster(pszConfigFile)
{
    char szCmd[255];
    SNPRINTF(szCmd, 255, "scstat > agent.tmp");
    szCmd[254] = '\0';
#ifdef _PLATFORM_SOLARIS 
    system1(szCmd);
#else
    system(szCmd);
#endif
    
    char szMatch_nodes[MAX_PATH];
    char szMatch_heartbeats[MAX_PATH];
    char szMatch_diskgroup[MAX_PATH];
    
    SNPRINTF(szMatch_nodes, MAX_PATH, "Cluster node:");
    SNPRINTF(szMatch_diskgroup, MAX_PATH, "Device group servers:");
    SNPRINTF(szMatch_heartbeats, MAX_PATH, "Transport path:");
    
    GetNodes(szMatch_nodes,m_LogicalHost.Cluster_Nodes) ;    
    GetDiskGroups (szMatch_diskgroup,m_LogicalHost.Cluster_Diskgroups);
    GetHeartBeats (szMatch_heartbeats,m_LogicalHost.Cluster_Heartbeats);
    m_nrOfPrvNet = m_LogicalHost.Cluster_Heartbeats.size();
        
    //获取当前主机名
    char szHostnameFile[MAX_PATH];
    SNPRINTF(szHostnameFile, MAX_PATH, "/etc/nodename");
    GetHostname(szHostnameFile,m_hostname);
    
    //此处ReadLogIP主要是为了获得 nafe0，nafe1，nafe2，nafe3，在SC3.0 中，
    //我们将不是nafo，采用返回qfe0，qfe1，qfe2，qfe3 （publicnet）
    //对于NAFO的LIST采用命令方式得到并保存qfe0 －－qfe3
    GetNafos (m_LogicalHost.Cluster_NAFO);
    
    remove("agent.tmp");    //注意：删除临时文件

}

/******************************************************************
函数原型   CCluster3:: GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
功能描述    获得节点个数以及节点名
输入参数    szMatch_nodes :文件中需要用来查找的字符串
          Cluster_Nodes :用来存放结果
输出参数    无
返回值     无
抛出异常    无
*******************************************************************/ 
BOOL CCluster3:: GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
{
    FILE* fp = fopen("agent.tmp", "r");
    if (fp == NULL)
    {
       return FALSE;
    }
    
    char  szOutput[MAX_PATH];
    char  nodename[100];
    char  temp [100];
    
    const char* szMatch_nodes = szNodesInfo;
    while (!feof(fp))
    {
       GetSCConfig(fp, szMatch_nodes, szOutput);
       sscanf(szOutput,"%s%s",nodename,temp);
       if (strlen(nodename) != 0)
       {
          Cluster_Nodes.push_back(nodename);
       }
       strcpy (nodename ,"");
    }
    fclose(fp);

    return TRUE ;
}

/******************************************************************
函数原型   CCluster3:: GetHeartBeats(const char* szMatch_heartbeats, LIST <STRING>& Cluster_Heartbeats)
功能描述    获得心跳个数以及心跳名
输入参数    szMatch_heartbeats :文件中需要用来查找的字符串
            Cluster_Heartbeats :用来存放结果
输出参数    无
返回值      无
抛出异常    无
*******************************************************************/ 
BOOL CCluster3:: GetHeartBeats(const char* szMatch_heartbeats, LIST <STRING>& Cluster_Heartbeats)
{
    FILE* fp = fopen("agent.tmp", "r");
    if (fp == NULL)
    {
       return FALSE;
    }
    
    STRING   sFound;
    char  szOutput[MAX_PATH];
    char  heartbeats[100];
    char  temp [100];
    
    while (!feof(fp))
    {
       GetSCConfig(fp, szMatch_heartbeats, szOutput);
       sscanf(szOutput,"%s%s",heartbeats,temp);
       STRING sLine = heartbeats;
       int nColon = sLine.find(':');
       if (nColon != -1)
       {
          sFound = sLine.substr(nColon + 1);
          strcpy(heartbeats, sFound.c_str());
          StringLRTrim(heartbeats);
       }    
       
       if (strlen(heartbeats) != 0)
       {
          Cluster_Heartbeats.push_back(heartbeats);
       }
       strcpy (heartbeats ,"");
    }
    fclose(fp);

    return TRUE ;
}

/******************************************************************
函数原型  CCluster3::  GetDiskGroups(const char* szMatch_diskgroup, LIST <STRING>& Cluster_Diskgroups)
功能描述    获得磁盘组个数以及磁盘组名，一般就是一个 （igwbdg）
输入参数    szMatch_diskgroup :文件中需要用来查找的字符串
            Cluster_Diskgroups :用来存放结果
输出参数    无
返回值      无
抛出异常    无
*******************************************************************/ 
BOOL CCluster3:: GetDiskGroups(const char* szMatch_diskgroup, LIST <STRING>& Cluster_Diskgroups)
{
    FILE* fp = fopen("agent.tmp", "r");
    if (fp == NULL)
    {
       return FALSE;
    }
    
    char  szOutput[MAX_PATH];
    char  diskgroup[100];
    char  temp [100];
    
    while (!feof(fp))
    {
       GetSCConfig(fp, szMatch_diskgroup, szOutput);
       sscanf(szOutput,"%s%s",diskgroup,temp);
       if (strlen(diskgroup) != 0)
       {
          Cluster_Diskgroups.push_back(diskgroup);
       }
       strcpy (diskgroup ,"");
    }
    fclose(fp);

    return TRUE ;
}

/******************************************************************
函数原型  CCluster3::  GetNafos(LIST <STRING>& Cluster_NAFO)
功能描述    获得Nafo个数以及Nafo名
输入参数    Cluster_NAFO :用于存放结果
输出参数    无
返回值      无
抛出异常    无
*******************************************************************/ 
BOOL CCluster3:: GetNafos(LIST <STRING>& Cluster_NAFO)
{
    char szCmd[255];
    SNPRINTF(szCmd, MAX_PATH, "/usr/cluster/lib/sc/publicnet list > pubnet.tmp");
    szCmd[254] = '\0';
#ifdef _PLATFORM_SOLARIS 
    system1(szCmd);
#else
    system(szCmd);
#endif

    FILE* fp = fopen("pubnet.tmp", "r");
    if (fp == NULL)
    {
       remove("pubnet.tmp");    //注意：删除临时文件
       return FALSE;
    }
    
    char  szLine[MAX_PATH];
    char  nofo[100];
    char  temp [100];
    int   nComma = TRUE;
    int   nLen;

    if(fgets(szLine, 1023, fp) != NULL) 
    {
       StringLRTrim(szLine);
       STRING sLine = szLine;
       nLen = sLine.length();
       if (nLen == 0)    //如果读出的是一个空行，退出
       {
          fclose(fp);
          remove("pubnet.tmp");    //注意：删除临时文件
          return FALSE;
       }
    }
    
    int nIndex = 0;
    STRING sNafo = "";
    
    while (nIndex < nLen)
    {
       char ch = szLine[nIndex];
       if (ch == ' ')
       {
          if (sNafo.length() != 0)
          {
             Cluster_NAFO.push_back(sNafo.c_str());
          }
          
          sNafo = "";  //重新清空字符串
       }
       else
       {
          sNafo += ch;
       }
       
       nIndex++;     //字符串索引增1
    }
    
    //注意处理最后一个ID
    if (sNafo.length() != 0)
    {
       Cluster_NAFO.push_back(sNafo.c_str());
    }
    
    fclose(fp);
    
    remove("pubnet.tmp");    //注意：删除临时文件
    return TRUE ;
}

/******************************************************************
函数原型    public: CCluster3:: BOOL  IsClusterRunning(void);
功能描述    检查当前本节点上集群系统是否正在运行
输入参数    无
输出参数    无
返回值      若本节点上集群系统还在运行则返回TRUE
抛出异常    无
*******************************************************************/
BOOL CCluster3:: IsClusterRunning(void)
{
    
    char szCmd[MAX_PATH];
    SNPRINTF(szCmd, MAX_PATH, "scstat -n > node.tmp");
    szCmd[254] = '\0';
#ifdef _PLATFORM_SOLARIS 
    system1(szCmd);
#else
    system(szCmd);
#endif
    
    FILE* fp = fopen("node.tmp", "r");
    if (fp == NULL)
    {
       return FALSE;
    }
    
    BOOL bState = FALSE;
    
    int nMem = 0;
    char  szLine[MAX_PATH];    
    STRING sLine;
    
    while (!feof(fp))
    {
       if(fgets(szLine, 1023, fp) != NULL) 
       {
          sLine = szLine;
          
          int nLen = sLine.length();
          if (nLen == 0)  continue;  //如果读出的是一个空行，继续
          
          int nMatch = sLine.find(m_hostname);
          
          if (nMatch != 0)    //如果读出的是一个空行，退出
          {
             if (sLine.find("Online") != 0)
             {
                bState = TRUE;
             }
          }
       }
       strcpy (szLine,"");
       
    }
    
    fclose(fp);
    remove("node.tmp");    //注意：删除临时文件
    
    return bState ;
}

/**************************************************************************
函数原型    public: CCluster3:: int  GetCurrMemNum(void);
功能描述    获得当前逻辑主机中活动成员数量
输入参数    无
输出参数    无
返回值      返回逻辑主机中活动成员数量，返回值为0表示不确定。
抛出异常    无
***************************************************************************/
int CCluster3:: GetCurrMemNum(void)
{
    //如果检测到SC没有运行，则无法确定当前活动的成员个数
    //函数返回0表示不确定
    if (IsClusterRunning() == FALSE)
    {
       return 0;
    }
    
    char szCmd[255];
    SNPRINTF(szCmd, 255, "scstat -n > nodenum.tmp");
    szCmd[254] = '\0';
#ifdef _PLATFORM_SOLARIS 
    system1(szCmd);
#else
    system(szCmd);
#endif
    
    char szFile[MAX_PATH];
    SNPRINTF(szFile, MAX_PATH, "nodenum.tmp");
    szFile[MAX_PATH - 1] = '\0';
    
    int nMem = GetSCCurrMemNum(szFile);
    
    remove("nodenum.tmp");    //注意：删除临时文件
    
    return nMem;
}


/**************************************************************************
函数原型    public: CCluster3:: BOOL   GetPrvNetStatus(int nPrvNetID);
功能描述    获取指定私网ID的当前状态，通过读SC的动态配置文件ccm.active_nets
           可以获得SC当前活动私网ID列表，例如"0  1"表示当前0号私网和1号
           私网处于活动状态。
输入参数    nPrvNetID －－ 私网编号
输出参数    无
返回值       如果指定私网状态正常则返回TRUE
抛出异常    无
***************************************************************************/
BOOL CCluster3:: GetPrvNetStatus(int nPrvNetID)
{
    char szCmd[255];
    SNPRINTF(szCmd, 255, "scstat -W > prvnet.tmp");
    szCmd[254] = '\0';
#ifdef _PLATFORM_SOLARIS 
    system1(szCmd);
#else
    system(szCmd);
#endif
    
    char szFile[MAX_PATH];
    SNPRINTF(szFile, MAX_PATH, "prvnet.tmp");
    szFile[MAX_PATH - 1] = '\0';
    
    //从配置文件中获得私网ID列表
    LIST<int> PrvNets;
    GetActivePrvNets(szFile, PrvNets);
    
    LIST<int>::iterator it;
    int PrvNetNo = 0 ;
    
    for (it = PrvNets.begin(); it != PrvNets.end(); it++)
    {
       if (nPrvNetID == PrvNetNo)
       {
          if ( 1 == (*it) )
          {
             remove("prvnet.tmp");    //注意：删除临时文件    
             return TRUE;    //该私网当前处于激活状态
          }
          else 
          {
             remove("prvnet.tmp");    //注意：删除临时文件    
             return FALSE;
          }
       }
       PrvNetNo++;
    }
    
    remove("prvnet.tmp");    //注意：删除临时文件    
    return FALSE;
}

/************************************************************************
函数原型    private: CCluster3:: int GetSCCurrMemNum(const char* szFilePath);
功能描述       读SC的数据库文件ccd.database文件中的"CLUSTM:"字段，例如：
             "CLUSTM:cluster1,cluster2"，则表示SC当前活动成员有cluster1
             和cluster2，则返回2
输入参数       szFilePath: ccd.database文件的全路径名。    
输出参数       无。    
返回值        SC当前活动成员数。  
抛出异常    无
*************************************************************************/
int CCluster3:: GetSCCurrMemNum(const char* szFilePath)
{
    char  szMatch[MAX_PATH] = "Online";
    char  szOutput[MAX_PATH];
    
    FILE* fp  = fopen(szFilePath, "r");
    if (fp == NULL)
    {
       return 0;
    }
    
    int nMem = 0;
    char  szLine[MAX_PATH];    
    STRING sLine;
    
    while (!feof(fp))
    {
       if(fgets(szLine, 1023, fp) != NULL) 
       {
          sLine = szLine;
          
          int nLen = sLine.length();
          if (nLen == 0)  continue;  //如果读出的是一个空行，继续
          
          int nMatch = sLine.find(szMatch);
          
          if (nMatch != -1)    //如果读出的是一个空行，退出
          {
             nMem = nMem + 1;
          }
       }
       strcpy (szLine,"");
    }
    
    fclose(fp);

    return nMem;
}

/************************************************************************
函数原型    private: CCluster3:: BOOL GetActivePrvNets(const char* szFilePath,
                                LIST<int>& PrvNets);
功能描述      通过读ccm.active_nets文件获得SC当前活动的私网列表。
输入参数      szFilePath: ccm.active_nets文件的全路径名。 
输出参数      PrvNets   :活动私网列表。   
返回值       执行成功返回TRUE，否则返回FALSE。   
抛出异常    无
*************************************************************************/
BOOL CCluster3:: GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets)
{
    FILE* fp = fopen(szFilePath, "r");
    if (fp == NULL)
    {
      return FALSE;
    }
    
    PrvNets.clear();       //首先清空列表    

    char szMatch_heartbeats[MAX_PATH];
    SNPRINTF(szMatch_heartbeats, MAX_PATH, "Transport path:");
    
    STRING   sFound;
    char  szOutput[MAX_PATH];
    char  heartbeats[100];
    
    while (!feof(fp))
    {
       GetSCConfig(fp, szMatch_heartbeats, szOutput);
       STRING sLine = szOutput;
       int nColon = sLine.find("faulted");
       if (nColon != -1)
       {
          PrvNets.push_back(0);
       }
       nColon = sLine.find("online");
       if (nColon != -1)
       {
          PrvNets.push_back(1);
       }
       strcpy (szOutput , "") ;
    }
    
    fclose(fp);
    
    return TRUE;
    
}

