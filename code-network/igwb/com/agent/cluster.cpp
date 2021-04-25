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
#include "../include/frame.h"
#include "cluster.h"
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
CCluster:: CCluster(const char* pszConfigFile)
{
    char szFile[MAX_PATH];
    
    CINIFile* pINIFile = new CINIFile((char *)pszConfigFile);
    pINIFile->Open();     //�򿪲��������ļ�billsvr.ini
    
    //����Ⱥ�������û�����ã���ȡĬ��ֵ:"sc-igwb"
    char szCluster[255] = "";
    pINIFile->GetString(CFG_SEC_CLUSTER, CFG_CLUSTER, DEFAULT_CLUSTER_NAME, szCluster, 254);          
    m_sCluster = szCluster;
    
    //��ȡ��ǰ������
    char szHostnameFile[MAX_PATH];
    SNPRINTF(szHostnameFile, MAX_PATH, "/etc/nodename");
    GetHostname(szHostnameFile,m_hostname);
    
    m_VMType = (VM_TYPE)pINIFile->GetInt(CFG_SEC_ALM_AGENT, CFG_VM_TYPE, VT_VXVM);
    
    
    //�������ж��Ƿ����������ļ�master д�ڹ���������� ��/var/otherĿ¼��
    char szMaster[MAX_PATH];
    SNPRINTF(szMaster, MAX_PATH, "/var/other/master");
    szMaster[254] = '\0';
    
    if (access(szMaster, F_OK) < 0)
    {
        FILE* pMaster = fopen(szMaster, "wb");
        if ( NULL != pMaster )
        {
            //�����ڵ���д��
            fwrite(m_hostname.c_str(), m_hostname.length(), 1, pMaster);
            fclose(pMaster);
        }
    }    

    delete pINIFile;  //CINIFile�������������л�ر�INI�����ļ����    
}

/******************************************************************
����ԭ��    public: ~CCluster(void);
��������    CCluster��������������ͷ��ڹ��캯���з����������
           ��ռ䡣
�������    ��
�������    ��
����ֵ       ��
�׳��쳣    ��
*******************************************************************/ 
CCluster:: ~CCluster(void) 
{
    m_LogicalHost.Cluster_Nodes.clear();
    
    m_LogicalHost.Cluster_Heartbeats.clear();
    
    m_LogicalHost.Cluster_Diskgroups.clear();
    
    m_LogicalHost.Cluster_NAFO.clear();
}

/******************************************************************
����ԭ��    public: const STRING  GetCluster(void);
��������    ��ü�Ⱥϵͳ����
�������    ��
�������    ��
����ֵ       ���ؼ�Ⱥϵͳ����
�׳��쳣    ��
*******************************************************************/
const STRING CCluster:: GetCluster(void)
{
    return m_sCluster;
}

/******************************************************************
����ԭ��    public: const STRING  GetLocalNodename(void);
��������    ��ñ��ڵ�����
�������    ��
�������    ��
����ֵ       ���ر��ڵ�����
�׳��쳣    ��
*******************************************************************/
const STRING CCluster:: GetLocalNodename(void)
{
    return m_hostname;
}

/******************************************************************
����ԭ��    public:    GetDiskGroup(void);
��������    ��ô�������
�������    ��
�������    ��
����ֵ       ����ָ�������ṹ��ָ��
�׳��쳣    ��
*******************************************************************/
LIST<STRING>& CCluster:: GetDiskGroup(void)
{
    return (m_LogicalHost.Cluster_Diskgroups);
}

/*******************************************************************
����ԭ��    LIST<STRING>& CCluster:: GetNAFO(void)
��������    ���NAFO����
�������    ��
�������    ��
����ֵ       ����ָ��NAFO��ṹ��ָ��
�׳��쳣    ��
********************************************************************/
LIST<STRING>& CCluster:: GetNAFO(void)
{
    return (m_LogicalHost.Cluster_NAFO);
}

/******************************************************************
����ԭ��    public: GetHostname(const char* szFilePath,STRING& sHostname)
��������    ��ýڵ�����Լ��ڵ���
�������    File *fp
�������    ��
����ֵ     ��
�׳��쳣    ��
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
����ԭ��    public:  int GetNumOfPrvNet(void);
��������    ��ü�Ⱥ��˽������
�������    ��
�������    ��
����ֵ       ���ؼ�Ⱥ��˽������
�׳��쳣    ��
*******************************************************************/
int CCluster:: GetNumOfPrvNet(void)
{
    return m_nrOfPrvNet;
}

/************************************************************************
����ԭ��    public: MV_TYPE  GetVMType(const char* pszFilePath);
��������    ����Ĭ��Ϊ��VXVM��
�������    ��
�������    ��
����ֵ     VXVM����
�׳��쳣    ��
*************************************************************************/
VM_TYPE CCluster:: GetVMType(void)
{
    return m_VMType;   //Ĭ����VT_VXVM����
}

/************************************************************************
����ԭ��    private: BOOL  GetSCConfig(FILE* fp, const char* szMatch, char* szOutput);
��������      ����SC�����ļ��Ĺ��򣬶���ƥ��szMatch�����ã����磺SC����
           �ļ��а���"cluster.number.nets  :  2",�������szMatchΪ
           "cluster.number.nets"ʱ����szOutput="2";
�������      fp    : �����ļ����;
            szMatch : ��Ҫƥ����ַ���;
�������      szOutput: ���ҵ����ַ�����  
����ֵ       ����ҵ�ƥ����ַ����򷵻�TRUE�����򷵻�FALSE�� 
�׳��쳣    ��
*************************************************************************/
BOOL CCluster:: GetSCConfig(FILE* fp, const char* szMatch, char* szOutput)
{
    char    szLine[2048]; //�����ļ�һ����󳤶�
    STRING   sLine;
    STRING   sFound;

    if (fp == NULL)   
    {
      return FALSE;
    }

    szOutput[0] = '\0';

    while (!feof(fp))
    {
      //��SC�����ļ��ж���һ��
      if(fgets(szLine, 2047, fp) != NULL)
      {
        sLine = szLine;
        
        int nLen = sLine.length();
        if (nLen == 0)  continue;  //�����������һ�����У�����
        
        int nColon = sLine.find(':');
        int nMatch = sLine.find(szMatch);
        int nComment = sLine.find('#');   //ע��
        
        if ((nColon != -1) && (nMatch != -1)
           && (nComment == -1))
        {
           sFound = sLine.substr(nColon + 1);
           strcpy(szOutput, sFound.c_str());
           StringLRTrim(szOutput);  //ȥ���ַ������ҵĿո�
           
           return TRUE;
        }
        
        sLine = "";
      }
    }

    return FALSE;
}

/************************************************************************
����ԭ��    private:  int GetSCNodeNum(void);
��������    ���SC�Ľڵ�����
�������   
�������    �ޡ�    
����ֵ       SC�ڵ��Ա����
�׳��쳣    ��
*************************************************************************/
int CCluster:: GetSCNodeNum(void)
{
    return (m_LogicalHost.Cluster_Nodes.size());
}



/******************************************************************
����ԭ��    public:  BOOL  IsClusterRunning(void);
��������    ��鵱ǰ���ڵ��ϼ�Ⱥϵͳ�Ƿ���������
            ������������
�������    ��
�������    ��
����ֵ       �����ڵ��ϼ�Ⱥϵͳ���������򷵻�TRUE
�׳��쳣    ��
*******************************************************************/
BOOL CCluster:: IsClusterRunning(void)
{
    return TRUE;;
}

/**************************************************************************
����ԭ��    public: int  GetCurrMemNum(void);
��������    ��õ�ǰ�߼������л��Ա����
             ������������
�������    ��
�������    ��
����ֵ       �����߼������л��Ա����������ֵΪ0��ʾ��ȷ����
�׳��쳣    ��
***************************************************************************/
int CCluster:: GetCurrMemNum(void)
{
    return 0;
}

/**************************************************************************
����ԭ��    public: BOOL   GetPrvNetStatus(int nPrvNetID);
��������    ��ȡָ��˽��ID�ĵ�ǰ״̬��
            ������������
�������    nPrvNetID ���� ˽�����
�������    ��
����ֵ       ���ָ��˽��״̬�����򷵻�TRUE
�׳��쳣    ��
***************************************************************************/
BOOL CCluster:: GetPrvNetStatus(int nPrvNetID)
{
    return TRUE;
}

/************************************************************************
����ԭ��    private: int GetSCCurrMemNum(const char* szFilePath);
��������    ��ȡSC��ǰ���Ա����  
             ������������
�������      szFilePath: ccd.database�ļ���ȫ·������    
�������      �ޡ�    
����ֵ        SC��ǰ���Ա����  
�׳��쳣    ��
*************************************************************************/
int CCluster:: GetSCCurrMemNum(const char* szFilePath)
{
    return 0;
}
    

/************************************************************************
����ԭ��    private: BOOL GetActivePrvNets(const char* szFilePath,
                                LIST<int>& PrvNets);
��������      ���SC��ǰ���˽���б�
              ������������
�������      szFilePath: ccm.active_nets�ļ���ȫ·������ 
�������      PrvNets   :�˽���б�   
����ֵ       ִ�гɹ�����TRUE�����򷵻�FALSE��   
�׳��쳣    ��
*************************************************************************/
BOOL CCluster:: GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets)
{
    return TRUE;
}

/******************************************************************
����ԭ��  GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
��������    ��ýڵ�����Լ��ڵ���
            ������������
�������    szFilePath :�ļ���·��
          Cluster_Nodes :������Ž��
�������    ��
����ֵ     ��
�׳��쳣    ��
*******************************************************************/ 
BOOL CCluster::GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes)
{
    return TRUE ;
}


/******************************************************************
����ԭ��    public const STRING  GetLogicalHost(void);
��������    ����߼�������
            ��CCluster2������
�������    ��
�������    ��
����ֵ       �����߼���������
�׳��쳣    ��
*******************************************************************/
const STRING CCluster:: GetLogicalHost(void)
{
    return m_LogicalHost.sLogHostName;
}

/******************************************************************
����ԭ��    public: const STRING GetDataService(void);
��������    ��ø߿������ݷ�����
            ��CCluster2������
�������    ��
�������    ��
����ֵ       �������ݷ�������
�׳��쳣    ��
*******************************************************************/
const STRING CCluster:: GetDataService(void)
{
    return m_LogicalHost.sDataServiceName;
}









