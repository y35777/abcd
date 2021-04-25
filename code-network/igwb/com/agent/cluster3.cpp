/***************************************************************************
 *��Ʒ��      ��iGateway Bill V200
 *ģ����      ��agent
 *�ļ���      ��Cluster.cpp
 *������Ϣ    ��
����SC3.0
���Դ�scstatһ��������� agent.tmp�ļ��У�Ȼ���ٶ�ȡ���־�̬������
�����ڵ����Լ������������������Լ��������������Լ���������LIST��ţ��������Բ�
��LIST��Ԫ�صĸ�����֪��
  
���ڼ�Ⱥ�����ֲ������������õ���
����������  publicnet list �õ�����Ҫ��� ������  publicnet check qfe0...
�ж��Ƿ���SC�в��� scstat -n
�жϻ˽��������� scstat -W

 *�汾˵��    ��V200R002i00M00B00D1130A
 *��Ȩ��Ϣ    ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����        ����ŷ�
 *����ʱ��    ��2002-10-25
 *�޸ļ�¼    ��
*****************************************************************************/
#include "cluster3.h"
#include "../include/frame.h"
#ifdef _PLATFORM_SOLARIS 
#include "../include/system1.h"
#endif

/******************************************************************
����ԭ��    public:  CCluster(const char *szConfigFile);
��������    CCluster��Ĺ��캯������ʼ���йؼ�Ⱥ������������Ϣ��
�������    szConfigFile�������ļ��������Ϊ�գ���ʹ��ȱʡ�����ļ���
�������    ��
����ֵ     ��
�׳��쳣    ��
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
        
    //��ȡ��ǰ������
    char szHostnameFile[MAX_PATH];
    SNPRINTF(szHostnameFile, MAX_PATH, "/etc/nodename");
    GetHostname(szHostnameFile,m_hostname);
    
    //�˴�ReadLogIP��Ҫ��Ϊ�˻�� nafe0��nafe1��nafe2��nafe3����SC3.0 �У�
    //���ǽ�����nafo�����÷���qfe0��qfe1��qfe2��qfe3 ��publicnet��
    //����NAFO��LIST�������ʽ�õ�������qfe0 ����qfe3
    GetNafos (m_LogicalHost.Cluster_NAFO);
    
    remove("agent.tmp");    //ע�⣺ɾ����ʱ�ļ�

}

/******************************************************************
����ԭ��   CCluster3:: GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
��������    ��ýڵ�����Լ��ڵ���
�������    szMatch_nodes :�ļ�����Ҫ�������ҵ��ַ���
          Cluster_Nodes :������Ž��
�������    ��
����ֵ     ��
�׳��쳣    ��
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
����ԭ��   CCluster3:: GetHeartBeats(const char* szMatch_heartbeats, LIST <STRING>& Cluster_Heartbeats)
��������    ������������Լ�������
�������    szMatch_heartbeats :�ļ�����Ҫ�������ҵ��ַ���
            Cluster_Heartbeats :������Ž��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��  CCluster3::  GetDiskGroups(const char* szMatch_diskgroup, LIST <STRING>& Cluster_Diskgroups)
��������    ��ô���������Լ�����������һ�����һ�� ��igwbdg��
�������    szMatch_diskgroup :�ļ�����Ҫ�������ҵ��ַ���
            Cluster_Diskgroups :������Ž��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��  CCluster3::  GetNafos(LIST <STRING>& Cluster_NAFO)
��������    ���Nafo�����Լ�Nafo��
�������    Cluster_NAFO :���ڴ�Ž��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
       remove("pubnet.tmp");    //ע�⣺ɾ����ʱ�ļ�
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
       if (nLen == 0)    //�����������һ�����У��˳�
       {
          fclose(fp);
          remove("pubnet.tmp");    //ע�⣺ɾ����ʱ�ļ�
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
          
          sNafo = "";  //��������ַ���
       }
       else
       {
          sNafo += ch;
       }
       
       nIndex++;     //�ַ���������1
    }
    
    //ע�⴦�����һ��ID
    if (sNafo.length() != 0)
    {
       Cluster_NAFO.push_back(sNafo.c_str());
    }
    
    fclose(fp);
    
    remove("pubnet.tmp");    //ע�⣺ɾ����ʱ�ļ�
    return TRUE ;
}

/******************************************************************
����ԭ��    public: CCluster3:: BOOL  IsClusterRunning(void);
��������    ��鵱ǰ���ڵ��ϼ�Ⱥϵͳ�Ƿ���������
�������    ��
�������    ��
����ֵ      �����ڵ��ϼ�Ⱥϵͳ���������򷵻�TRUE
�׳��쳣    ��
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
          if (nLen == 0)  continue;  //�����������һ�����У�����
          
          int nMatch = sLine.find(m_hostname);
          
          if (nMatch != 0)    //�����������һ�����У��˳�
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
    remove("node.tmp");    //ע�⣺ɾ����ʱ�ļ�
    
    return bState ;
}

/**************************************************************************
����ԭ��    public: CCluster3:: int  GetCurrMemNum(void);
��������    ��õ�ǰ�߼������л��Ա����
�������    ��
�������    ��
����ֵ      �����߼������л��Ա����������ֵΪ0��ʾ��ȷ����
�׳��쳣    ��
***************************************************************************/
int CCluster3:: GetCurrMemNum(void)
{
    //�����⵽SCû�����У����޷�ȷ����ǰ��ĳ�Ա����
    //��������0��ʾ��ȷ��
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
    
    remove("nodenum.tmp");    //ע�⣺ɾ����ʱ�ļ�
    
    return nMem;
}


/**************************************************************************
����ԭ��    public: CCluster3:: BOOL   GetPrvNetStatus(int nPrvNetID);
��������    ��ȡָ��˽��ID�ĵ�ǰ״̬��ͨ����SC�Ķ�̬�����ļ�ccm.active_nets
           ���Ի��SC��ǰ�˽��ID�б�����"0  1"��ʾ��ǰ0��˽����1��
           ˽�����ڻ״̬��
�������    nPrvNetID ���� ˽�����
�������    ��
����ֵ       ���ָ��˽��״̬�����򷵻�TRUE
�׳��쳣    ��
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
    
    //�������ļ��л��˽��ID�б�
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
             remove("prvnet.tmp");    //ע�⣺ɾ����ʱ�ļ�    
             return TRUE;    //��˽����ǰ���ڼ���״̬
          }
          else 
          {
             remove("prvnet.tmp");    //ע�⣺ɾ����ʱ�ļ�    
             return FALSE;
          }
       }
       PrvNetNo++;
    }
    
    remove("prvnet.tmp");    //ע�⣺ɾ����ʱ�ļ�    
    return FALSE;
}

/************************************************************************
����ԭ��    private: CCluster3:: int GetSCCurrMemNum(const char* szFilePath);
��������       ��SC�����ݿ��ļ�ccd.database�ļ��е�"CLUSTM:"�ֶΣ����磺
             "CLUSTM:cluster1,cluster2"�����ʾSC��ǰ���Ա��cluster1
             ��cluster2���򷵻�2
�������       szFilePath: ccd.database�ļ���ȫ·������    
�������       �ޡ�    
����ֵ        SC��ǰ���Ա����  
�׳��쳣    ��
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
          if (nLen == 0)  continue;  //�����������һ�����У�����
          
          int nMatch = sLine.find(szMatch);
          
          if (nMatch != -1)    //�����������һ�����У��˳�
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
����ԭ��    private: CCluster3:: BOOL GetActivePrvNets(const char* szFilePath,
                                LIST<int>& PrvNets);
��������      ͨ����ccm.active_nets�ļ����SC��ǰ���˽���б�
�������      szFilePath: ccm.active_nets�ļ���ȫ·������ 
�������      PrvNets   :�˽���б�   
����ֵ       ִ�гɹ�����TRUE�����򷵻�FALSE��   
�׳��쳣    ��
*************************************************************************/
BOOL CCluster3:: GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets)
{
    FILE* fp = fopen(szFilePath, "r");
    if (fp == NULL)
    {
      return FALSE;
    }
    
    PrvNets.clear();       //��������б�    

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

