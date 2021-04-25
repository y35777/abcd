/***************************************************************************
 *��Ʒ��      ��iGateway Bill V200
 *ģ����      ��agent
 *�ļ���      ��Cluster.cpp
 *������Ϣ    ��
 ����SC2.2
 ��CCluster��ʵ�֣������װ�˻��SUN SC��Ⱥϵͳ�������Լ�����ʱ��Ϣ�Ĺ��ܡ�
 SC�����ļ��ܶ࣬������Ҫʹ�õ��У�
 1. /etc/opt/SUNWcluster/conf/clustername.cdb   ��Ⱥ�ľ�̬�����ļ�(ֻ��)
    ������clusternameΪ��Ӧ�ļ�Ⱥ������CDB(Cluster Database)�ļ��ɵõ���
    Ⱥ�����֡��ڵ����������ڵ�����ơ�˽������IP�������������͡�Quorum
    �Ⱦ�̬��������Ϣ���ļ���ʽΪ��/ֵ�Ե���ʽ;
 2. /etc/opt/SUNWcluster/conf/ccd.database   ��Ⱥ�Ķ�̬�������ݿ�(ֻ��)��
    CCD(Cluster Configuration Database)�ļ��������߼��������á����ݷ���״
    ̬�����ݷ���ʵ���Ĺ�����Ϣ�ȡ�PNM״̬�ı䡢�߼������������߼�������״̬
    �ı�ȵ��¼�Ⱥ�����õ��������������ӳ��CCD�ļ���;
 3. /var/opt/SUNWcluster/clustername/cluster_is_running ���ļ��Ƿ��������ʾ
    ��Ⱥ�Ƿ����У�����clusternameΪ��Ӧ��Ⱥ��;
 4. /var/opt/SUNWcluster/clustername/ccm.active_nets  ��ǰ���˽������;
 5. /var/opt/SUNWcluster/clustername/ccm.selected    ��ǰѡ���˽������;

 *�汾˵��    ��V200R002i00M00B00D1130A
 *��Ȩ��Ϣ    ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����        ����ŷ�
 *����ʱ��    ��2002-10-25
 *�޸ļ�¼    ��
*****************************************************************************/
#include "cluster2.h"
#include "../include/frame.h"
#ifdef _PLATFORM_SOLARIS 
#include "../include/system1.h"
#endif

/******************************************************************
����ԭ��    public:  CCluster(const char *pszConfigFile);
��������    CCluster��Ĺ��캯������ʼ���йؼ�Ⱥ������������Ϣ��
�������    pszConfigFile�������ļ��������Ϊ�գ���ʹ��ȱʡ�����ļ���
�������    ��
����ֵ     ��
�׳��쳣    ��
*******************************************************************/
CCluster2:: CCluster2(const char* pszConfigFile) : CCluster(pszConfigFile)
{
    char szFile[MAX_PATH];

    CINIFile* pINIFile = new CINIFile((char *)pszConfigFile);
    pINIFile->Open();     //�򿪲��������ļ�billsvr.ini
    
    char szSCDir[MAX_PATH];       //SC�����װ·�� 
    //��SC��װ��
    pINIFile->GetString(CFG_SEC_CLUSTER, CFG_CLUSTER_PATH, 
        DEFAULT_CLUSTER_PATH, szSCDir, MAX_PATH);
    szSCDir[MAX_PATH - 1] = '\0';
    m_sSCDir = szSCDir;
    
    //���߼�������  
    char szLogHost[255];
    pINIFile->GetString(CFG_SEC_CLUSTER, CFG_LOG_HOST, 
        DEFAULT_LOG_HOST, szLogHost, 254);
    
    m_LogicalHost.sLogHostName = szLogHost;

    //���߿������ݷ����� 
    char szDataService[255];
    pINIFile->GetString(CFG_SEC_CLUSTER, CFG_DATA_SERVICE, 
        DEFAULT_DATA_SERVICE, szDataService, 254);

    m_LogicalHost.sDataServiceName = szDataService;
    
    //��SC�ڵ�������ò���λ��/etc/opt/SUNWcluster/conf/clustername.cdb
    //�ļ���"cluster.number.node"�� 
    SNPRINTF(szFile, MAX_PATH, "%s/conf/%s.cdb", szSCDir, m_sCluster.c_str()); 
    szFile[MAX_PATH - 1] = '\0';
    
    GetNodes(szFile,m_LogicalHost.Cluster_Nodes) ;    
    
    //���SC˽������(һ��Ϊ����˽��)  
    //�ò���Ҳλ��"clustername.cdb"�ļ���
    m_nrOfPrvNet = GetPrvNetNum(szFile);
    
    //ͨ��SC�����ݿ������ļ�ccd.database��ù��������
    SNPRINTF(szFile, MAX_PATH, "%s/conf/ccd.database", szSCDir);
    szFile[MAX_PATH - 1] = '\0';
    STRING sLogHost;
    
    ReadLOGHOST(szFile, sLogHost, m_LogicalHost.Cluster_Nodes, m_LogicalHost.Cluster_Diskgroups);
    
    //ͨ��SC�����ݿ������ļ�ccd.database���NAFO��
    ReadLogIP(szFile, m_LogicalHost.Cluster_NAFO);
    
    delete pINIFile;  //CINIFile�������������л�ر�INI�����ļ����

}

/******************************************************************
����ԭ��  CCluster2::  GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
��������    ��ýڵ�����Լ��ڵ���
�������    szFilePath :�ļ���·��
          Cluster_Nodes :������Ž��
�������    ��
����ֵ     ��
�׳��쳣    ��
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
        return FALSE; //Ĭ�������Ϊ�����ڵ�
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
����ԭ��    public: const CCluster2:: STRING  GetSCDir(void);
��������    ���ؼ�Ⱥ����İ�װ·��
�������    ��
�������    ��
����ֵ       ����ָ��Ⱥ��װ·����
�׳��쳣    ��
*******************************************************************/
const STRING CCluster2:: GetSCDir(void)
{
    return m_sSCDir;
}

/******************************************************************
����ԭ��    public: CCluster2:: const STRING  GetLogicalHost(void);
��������    ����߼�������
�������    ��
�������    ��
����ֵ       �����߼���������
�׳��쳣    ��
*******************************************************************/
const STRING CCluster2:: GetLogicalHost(void)
{
    return m_LogicalHost.sLogHostName;
}

/******************************************************************
����ԭ��    public: CCluster2:: const STRING GetDataService(void);
��������    ��ø߿������ݷ�����
�������    ��
�������    ��
����ֵ       �������ݷ�������
�׳��쳣    ��
*******************************************************************/
const STRING CCluster2:: GetDataService(void)
{
    return m_LogicalHost.sDataServiceName;
}

/******************************************************************
����ԭ��    public:  CCluster2:: BOOL  IsClusterRunning(void);
��������    ��鵱ǰ���ڵ��ϼ�Ⱥϵͳ�Ƿ���������
�������    ��
�������    ��
����ֵ       �����ڵ��ϼ�Ⱥϵͳ���������򷵻�TRUE
�׳��쳣    ��
*******************************************************************/
BOOL CCluster2:: IsClusterRunning(void)
{
    char szFile[MAX_PATH]; 
    
    SNPRINTF(szFile, MAX_PATH, "/var/opt/SUNWcluster/%s/cluster_is_running", 
        m_sCluster.c_str());
    szFile[MAX_PATH - 1] = '\0';

    //���cluster_is_running�ļ��Ƿ����
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
����ԭ��    public: CCluster2:: int  GetCurrMemNum(void);
��������    ��õ�ǰ�߼������л��Ա����
�������    ��
�������    ��
����ֵ       �����߼������л��Ա����������ֵΪ0��ʾ��ȷ����
�׳��쳣    ��
***************************************************************************/
int CCluster2:: GetCurrMemNum(void)
{
    //�����⵽SCû�����У����޷�ȷ����ǰ��ĳ�Ա����
    //��������0��ʾ��ȷ��
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
����ԭ��    public: CCluster2:: BOOL   GetPrvNetStatus(int nPrvNetID);
��������    ��ȡָ��˽��ID�ĵ�ǰ״̬��ͨ����SC�Ķ�̬�����ļ�ccm.active_nets
           ���Ի��SC��ǰ�˽��ID�б�����"0  1"��ʾ��ǰ0��˽����1��
           ˽�����ڻ״̬��
�������    nPrvNetID ���� ˽�����
�������    ��
����ֵ       ���ָ��˽��״̬�����򷵻�TRUE
�׳��쳣    ��
***************************************************************************/
BOOL CCluster2:: GetPrvNetStatus(int nPrvNetID)
{
    char szFile[MAX_PATH];

    SNPRINTF(szFile, MAX_PATH, "/var/opt/SUNWcluster/%s/ccm.active_nets", 
         m_sCluster.c_str());
    szFile[MAX_PATH - 1] = '\0';
    
    //�������ļ��л��˽��ID�б�
    LIST<int> PrvNets;
    GetActivePrvNets(szFile, PrvNets);
    
    LIST<int>::iterator it;
    for (it = PrvNets.begin(); it != PrvNets.end(); it++)
    {
       if (nPrvNetID == (*it))
       {
         return TRUE;    //��˽����ǰ���ڼ���״̬
       }
    }
   
    return FALSE;
}

/************************************************************************
����ԭ��    private: CCluster2:: int GetSCCurrMemNum(const char* szFilePath);
��������      ��SC�����ݿ��ļ�ccd.database�ļ��е�"CLUSTM:"�ֶΣ����磺
           "CLUSTM:cluster1,cluster2"�����ʾSC��ǰ���Ա��cluster1
           ��cluster2���򷵻�2
�������      szFilePath: ccd.database�ļ���ȫ·������    
�������      �ޡ�    
����ֵ        SC��ǰ���Ա����  
�׳��쳣    ��
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
       
       // ��Ա֮����','�ָ�
       if (ch == ',') nMem++;
    }

    return (nMem + 1);
}

/************************************************************************
����ԭ��    private: CCluster2:: BOOL GetActivePrvNets(const char* szFilePath,
                                LIST<int>& PrvNets);
��������      ͨ����ccm.active_nets�ļ����SC��ǰ���˽���б�
�������      szFilePath: ccm.active_nets�ļ���ȫ·������ 
�������      PrvNets   :�˽���б�   
����ֵ       ִ�гɹ�����TRUE�����򷵻�FALSE��   
�׳��쳣    ��
*************************************************************************/
BOOL CCluster2:: GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets)
{
    FILE* fp = fopen(szFilePath, "r");
    if (fp == NULL)
    {
      return FALSE;
    }
    
    PrvNets.clear();       //��������б�    

    char szLine[1024] = "";
    fgets(szLine, 1023, fp);
    StringLRTrim(szLine);    //ȥ���ַ������ҵĿո����

    int nLen = strlen(szLine);
    if (nLen == 0)   
    {
       fclose(fp);
       return FALSE;
    }

    int nIndex = 0;
    STRING sNetID = "";
    //˽��ID֮���ÿո�ָ�
    while (nIndex < nLen)
    {
       char ch = szLine[nIndex];
       if (ch == ' ')
       {
         if (sNetID.length() != 0)
         {
            PrvNets.push_back(atoi(sNetID.c_str()));
         }

         sNetID = "";  //��������ַ���
       }
       else
       {
         sNetID += ch;
       }

       nIndex++;    //�ַ���������1
    }

    //ע�⴦�����һ��ID
    if (sNetID.length() != 0)
    {
       PrvNets.push_back(atoi(sNetID.c_str()));
    }
    
    fclose(fp);
    
    return TRUE;
    
}

/************************************************************************
����ԭ��    private: CCluster2:: int   GetPrvNetNum(const char* szFilePath);
��������      ͨ����clustername.cdb�ļ��е�cluster.number.nets�λ��
           SCӵ�е�˽������������"cluster.number.nets:2"��ʾSC����
           ��2��˽����
�������      szFilePath: clustername.cdb�ļ���ȫ·������ 
�������      �ޡ�    
����ֵ        SCӵ�е�˽��������  
�׳��쳣    ��
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
       return 2;   //Ĭ��Ϊ������˽��
    }
       
    return atoi(szOutput);
}


/************************************************************************
����ԭ��    private: CCluster2:: BOOL   GetMaster(const char* szLogHostName,
                            char* szMaster);
��������      ���SC�����ڵ�����ͨ��haget�����á�   
           ���磺��SC��ִ������"haget -f master -h hasyb"������
           ����cluster1����cluster1����SC��ǰ�Ľڵ�����
�������      szLogHostName:�߼���������    
�������      szMaster    :�����ڵ�����  
����ֵ       ����ɹ����SC��������İ�װ�㣬�򷵻�TRUE�����򷵻�FALSE�� 
�׳��쳣    ��
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

    StringLRTrim(szLine);    //ȥ���ַ������ҵĿո�
    if (strlen(szLine) == 0)   
    {
       return FALSE;
    } 

    strcpy(szMaster, szLine);
    remove("agent.tmp");    //ע�⣺ɾ����ʱ�ļ�

    return TRUE;
}

/************************************************************************
����ԭ��    private: CCluster2:: BOOL   ReadLOGHOST(const char* pszFilePath,
                              STRING& sLogHost,
                              LIST<STRING>& NodeName,
                              LIST<STRING>& DiskGroup);
��������      ͨ����SC�����ݿ������ļ�ccd.database����߼����������ڵ���
           ��������顣    
�������      pszFilePath: ccd.database�ļ���ȫ·������   
�������    ��
����ֵ       ����ɹ���������򷵻�TRUE�����򷵻�FALSE�� 
�׳��쳣    ��
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
    
    //����LOGHOST:������"LOGHOST:hasyb:cluster1,cluster2:dg01:1:1"
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

    //����߼�������
    sLogHost = sLine.substr(0, nColon);
    if (sLogHost.length() == 0)  
    {
       return FALSE;
    }

    //��ýڵ���
    sLine = sLine.substr(nColon+1);
    const char* pChar = sLine.c_str();
    nIndex = 0;
    ch = pChar[nIndex];
    sNode = "";
    while (ch != ':')
    {
        if (ch == ',')   //�����Ա֮����','�ָ�
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

    //�������һ���ڵ���
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
       if (ch == ',')   //�����Ա֮����','�ָ�
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
    
    //�������һ����Ա
    if (sNode.length() != 0)
    {
       DiskGroup.push_back(sNode);
    }

    return TRUE;
}

/***********************************************************************
����ԭ��    private: CCluster2:: BOOL   ReadLogIP(const char* pszFilePath,
LIST<STRING>& LogIP);
��������      ͨ����SC�����ݿ������ļ�ccd.database���SC����IP����,����LOGIP���ֶ����¸�ʽ��
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
�������      pszFilePath: ccd.database�ļ���ȫ·������   
�������    ��
����ֵ       ����ɹ��������IP���ã��򷵻�TRUE�����򷵻�FALSE�� 
�׳��쳣    ��

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
    //���û���ҵ�ƥ����ַ����У��򷵻�FALSE
    
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
      
      //�޸ļ�¼�� �������for����� �� "i < 3" ��Ϊ�� "i < 2 "��ԭ�������ע��������
      //���еĸ�ʽΪ��(LOGIP:�Ѿ���szOutput��ȥ����)igwb1,igwb2:qfe3,qfe3:ha-igwb3:4 
      //��Ҫ���ֶ�Ϊha-igwb3,����ȡ��2��":"����ֶ�
      
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
      
      //����߼�IP
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


