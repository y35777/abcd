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
#include "../include/frame.h"
#include "cluster.h"
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
CCluster:: CCluster(const char* pszConfigFile)
{
    char szFile[MAX_PATH];
    
    CINIFile* pINIFile = new CINIFile((char *)pszConfigFile);
    pINIFile->Open();     //打开参数配置文件billsvr.ini
    
    //读集群名，如果没有配置，则取默认值:"sc-igwb"
    char szCluster[255] = "";
    pINIFile->GetString(CFG_SEC_CLUSTER, CFG_CLUSTER, DEFAULT_CLUSTER_NAME, szCluster, 254);          
    m_sCluster = szCluster;
    
    //获取当前主机名
    char szHostnameFile[MAX_PATH];
    SNPRINTF(szHostnameFile, MAX_PATH, "/etc/nodename");
    GetHostname(szHostnameFile,m_hostname);
    
    m_VMType = (VM_TYPE)pINIFile->GetInt(CFG_SEC_ALM_AGENT, CFG_VM_TYPE, VT_VXVM);
    
    
    //把用来判断是否发生倒换的文件master 写在共享磁盘阵列 的/var/other目录下
    char szMaster[MAX_PATH];
    SNPRINTF(szMaster, MAX_PATH, "/var/other/master");
    szMaster[254] = '\0';
    
    if (access(szMaster, F_OK) < 0)
    {
        FILE* pMaster = fopen(szMaster, "wb");
        if ( NULL != pMaster )
        {
            //将本节点名写入
            fwrite(m_hostname.c_str(), m_hostname.length(), 1, pMaster);
            fclose(pMaster);
        }
    }    

    delete pINIFile;  //CINIFile类在析构函数中会关闭INI配置文件句柄    
}

/******************************************************************
函数原型    public: ~CCluster(void);
功能描述    CCluster类的析构函数，释放在构造函数中分配的所有内
           存空间。
输入参数    无
输出参数    无
返回值       无
抛出异常    无
*******************************************************************/ 
CCluster:: ~CCluster(void) 
{
    m_LogicalHost.Cluster_Nodes.clear();
    
    m_LogicalHost.Cluster_Heartbeats.clear();
    
    m_LogicalHost.Cluster_Diskgroups.clear();
    
    m_LogicalHost.Cluster_NAFO.clear();
}

/******************************************************************
函数原型    public: const STRING  GetCluster(void);
功能描述    获得集群系统名字
输入参数    无
输出参数    无
返回值       返回集群系统名。
抛出异常    无
*******************************************************************/
const STRING CCluster:: GetCluster(void)
{
    return m_sCluster;
}

/******************************************************************
函数原型    public: const STRING  GetLocalNodename(void);
功能描述    获得本节点名字
输入参数    无
输出参数    无
返回值       返回本节点名。
抛出异常    无
*******************************************************************/
const STRING CCluster:: GetLocalNodename(void)
{
    return m_hostname;
}

/******************************************************************
函数原型    public:    GetDiskGroup(void);
功能描述    获得磁盘组名
输入参数    无
输出参数    无
返回值       返回指向磁盘组结构的指针
抛出异常    无
*******************************************************************/
LIST<STRING>& CCluster:: GetDiskGroup(void)
{
    return (m_LogicalHost.Cluster_Diskgroups);
}

/*******************************************************************
函数原型    LIST<STRING>& CCluster:: GetNAFO(void)
功能描述    获得NAFO组名
输入参数    无
输出参数    无
返回值       返回指向NAFO组结构的指针
抛出异常    无
********************************************************************/
LIST<STRING>& CCluster:: GetNAFO(void)
{
    return (m_LogicalHost.Cluster_NAFO);
}

/******************************************************************
函数原型    public: GetHostname(const char* szFilePath,STRING& sHostname)
功能描述    获得节点个数以及节点名
输入参数    File *fp
输出参数    无
返回值     无
抛出异常    无
*******************************************************************/ 
BOOL CCluster:: GetHostname(const char* szFilePath,STRING& sHostname)
{
    FILE* fp = fopen(szFilePath, "r");
    if (fp == NULL)
    {
       return FALSE;
    }
    
    char  nodename[100];
    
    if(fgets(nodename, 1023, fp) != NULL)
    {
       StringLRTrim(nodename); 
       sHostname = nodename;
    }
    
    fclose(fp);
    
    return TRUE;
}

/******************************************************************
函数原型    public:  int GetNumOfPrvNet(void);
功能描述    获得集群中私网数量
输入参数    无
输出参数    无
返回值       返回集群中私网数量
抛出异常    无
*******************************************************************/
int CCluster:: GetNumOfPrvNet(void)
{
    return m_nrOfPrvNet;
}

/************************************************************************
函数原型    public: MV_TYPE  GetVMType(const char* pszFilePath);
功能描述    程序默认为是VXVM。
输入参数    无
输出参数    无
返回值     VXVM类型
抛出异常    无
*************************************************************************/
VM_TYPE CCluster:: GetVMType(void)
{
    return m_VMType;   //默认是VT_VXVM类型
}

/************************************************************************
函数原型    private: BOOL  GetSCConfig(FILE* fp, const char* szMatch, char* szOutput);
功能描述      按照SC配置文件的规则，读出匹配szMatch的配置，例如：SC配置
           文件中包含"cluster.number.nets  :  2",则当输入的szMatch为
           "cluster.number.nets"时，则szOutput="2";
输入参数      fp    : 配置文件句柄;
            szMatch : 需要匹配的字符串;
输出参数      szOutput: 查找到的字符串。  
返回值       如果找到匹配的字符串则返回TRUE，否则返回FALSE。 
抛出异常    无
*************************************************************************/
BOOL CCluster:: GetSCConfig(FILE* fp, const char* szMatch, char* szOutput)
{
    char    szLine[2048]; //配置文件一行最大长度
    STRING   sLine;
    STRING   sFound;

    if (fp == NULL)   
    {
      return FALSE;
    }

    szOutput[0] = '\0';

    while (!feof(fp))
    {
      //从SC配置文件中读出一行
      if(fgets(szLine, 2047, fp) != NULL)
      {
        sLine = szLine;
        
        int nLen = sLine.length();
        if (nLen == 0)  continue;  //如果读出的是一个空行，继续
        
        int nColon = sLine.find(':');
        int nMatch = sLine.find(szMatch);
        int nComment = sLine.find('#');   //注释
        
        if ((nColon != -1) && (nMatch != -1)
           && (nComment == -1))
        {
           sFound = sLine.substr(nColon + 1);
           strcpy(szOutput, sFound.c_str());
           StringLRTrim(szOutput);  //去掉字符串左右的空格
           
           return TRUE;
        }
        
        sLine = "";
      }
    }

    return FALSE;
}

/************************************************************************
函数原型    private:  int GetSCNodeNum(void);
功能描述    获得SC的节点数。
输入参数   
输出参数    无。    
返回值       SC节点成员数量
抛出异常    无
*************************************************************************/
int CCluster:: GetSCNodeNum(void)
{
    return (m_LogicalHost.Cluster_Nodes.size());
}



/******************************************************************
函数原型    public:  BOOL  IsClusterRunning(void);
功能描述    检查当前本节点上集群系统是否正在运行
            从子类中重载
输入参数    无
输出参数    无
返回值       若本节点上集群系统还在运行则返回TRUE
抛出异常    无
*******************************************************************/
BOOL CCluster:: IsClusterRunning(void)
{
    return TRUE;;
}

/**************************************************************************
函数原型    public: int  GetCurrMemNum(void);
功能描述    获得当前逻辑主机中活动成员数量
             从子类中重载
输入参数    无
输出参数    无
返回值       返回逻辑主机中活动成员数量，返回值为0表示不确定。
抛出异常    无
***************************************************************************/
int CCluster:: GetCurrMemNum(void)
{
    return 0;
}

/**************************************************************************
函数原型    public: BOOL   GetPrvNetStatus(int nPrvNetID);
功能描述    获取指定私网ID的当前状态。
            从子类中重载
输入参数    nPrvNetID －－ 私网编号
输出参数    无
返回值       如果指定私网状态正常则返回TRUE
抛出异常    无
***************************************************************************/
BOOL CCluster:: GetPrvNetStatus(int nPrvNetID)
{
    return TRUE;
}

/************************************************************************
函数原型    private: int GetSCCurrMemNum(const char* szFilePath);
功能描述    获取SC当前活动成员数。  
             从子类中重载
输入参数      szFilePath: ccd.database文件的全路径名。    
输出参数      无。    
返回值        SC当前活动成员数。  
抛出异常    无
*************************************************************************/
int CCluster:: GetSCCurrMemNum(const char* szFilePath)
{
    return 0;
}
    

/************************************************************************
函数原型    private: BOOL GetActivePrvNets(const char* szFilePath,
                                LIST<int>& PrvNets);
功能描述      获得SC当前活动的私网列表。
              在子类重重载
输入参数      szFilePath: ccm.active_nets文件的全路径名。 
输出参数      PrvNets   :活动私网列表。   
返回值       执行成功返回TRUE，否则返回FALSE。   
抛出异常    无
*************************************************************************/
BOOL CCluster:: GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets)
{
    return TRUE;
}

/******************************************************************
函数原型  GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
功能描述    获得节点个数以及节点名
            在子类中重载
输入参数    szFilePath :文件的路径
          Cluster_Nodes :用来存放结果
输出参数    无
返回值     无
抛出异常    无
*******************************************************************/ 
BOOL CCluster::GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
{
    return TRUE ;
}


/******************************************************************
函数原型    public const STRING  GetLogicalHost(void);
功能描述    获得逻辑主机名
            在CCluster2中重载
输入参数    无
输出参数    无
返回值       返回逻辑主机名。
抛出异常    无
*******************************************************************/
const STRING CCluster:: GetLogicalHost(void)
{
    return m_LogicalHost.sLogHostName;
}

/******************************************************************
函数原型    public: const STRING GetDataService(void);
功能描述    获得高可用数据服务名
            在CCluster2中重载
输入参数    无
输出参数    无
返回值       返回数据服务名。
抛出异常    无
*******************************************************************/
const STRING CCluster:: GetDataService(void)
{
    return m_LogicalHost.sDataServiceName;
}









