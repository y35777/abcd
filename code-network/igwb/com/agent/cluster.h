/***************************************************************************
 *��Ʒ��        ��iGateway Bill V200        
 *ģ����    ��agent
 *�ļ���    ��Cluster.h
 *������Ϣ      ����CCluster�Ķ����ļ��������װ�˻��SUN SC��Ⱥϵͳ������
                  �Լ�����ʱ��Ϣ�Ĺ��ܡ� 
 *�汾˵��  ��V200R002i00M00B00D1130A   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ������ƽ
 *����ʱ��  ��2001-10-30
 *�޸ļ�¼  ��
****************************************************************************/
#ifndef _CLUSTER_H_
#define _CLUSTER_H_

#include "agent_hdr.h"

//��CCluster
class CCluster
{
public:
    CCluster(const char* pszConfigFile);            //���캯��
    virtual ~CCluster();                            //��������

public:
    const STRING   GetCluster(void);                //��ü�Ⱥ����
    const STRING   GetLocalNodename(void);          //��ñ������ڵ���    
    LIST<STRING>&  GetDiskGroup(void);              //��ô�����
    LIST<STRING>&  GetNAFO(void);                   //���NAFO����
    BOOL           GetHostname(const char* szFilePath,STRING& sHostname);  //��ñ���������
    int            GetNumOfPrvNet(void);            //���˽������
    VM_TYPE        GetVMType(void);                 //��þ�����������
    BOOL           GetSCConfig(FILE* fp, const char* szMatch, char* szOutput);
    int            GetSCNodeNum(void);              //���SC�нڵ����

public:
    virtual  BOOL  IsClusterRunning(void);          //��鼯Ⱥ�Ƿ�������
    virtual  int   GetCurrMemNum(void);             //��õ�ǰ�߼������л��Ա����    
    virtual  BOOL  GetPrvNetStatus(int PrvNetID);   //���ָ��ID��˽���ĵ�ǰ״̬
    virtual  BOOL  GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets); //���SC��ǰ���˽���б�
    virtual  int   GetSCCurrMemNum(const char* szFilePath);//���SC��ǰ���Ա��
    virtual  BOOL  GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes); //���SC�����нڵ�

    virtual  const STRING  GetLogicalHost(void);    //����߼�������
    virtual  const STRING  GetDataService(void);    //��ø߿������ݷ�������


public:
    STRING         m_sCluster;                      //��Ⱥ���֣���װSCʱ��ָ��������
    SLogicalHost   m_LogicalHost;                   //�߼�������Ϣ
    int            m_nrOfPrvNet;                    //˽������
    STRING         m_hostname;                      //��ǰ������
    VM_TYPE        m_VMType;                        //������������
};

#endif   //_CLUSTER_H_
