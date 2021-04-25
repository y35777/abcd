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
#ifndef _CLUSTER3_H_
#define _CLUSTER3_H_

#include "agent_hdr.h"
#include "cluster.h"

class CCluster3 : public CCluster
{
public:
    CCluster3(const char* pszConfigFile);    //���캯��
    virtual ~CCluster3(){};                            //��������

    BOOL          IsClusterRunning(void);    //��鼯Ⱥ�Ƿ�������
    int           GetSCCurrMemNum(const char* szFilePath);//���SC��ǰ���Ա��
    BOOL          GetPrvNetStatus(int PrvNetID);  //���ָ��ID��˽���ĵ�ǰ״̬
    BOOL          GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets); //���SC��ǰ���˽���б�
    int           GetCurrMemNum(void);       //��õ�ǰ�߼������л��Ա����    
    BOOL          GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes);

private:
    BOOL          GetHeartBeats(const char* szMatch_heartbeats, LIST <STRING>& Cluster_Heartbeats);//���SC����������
    BOOL          GetDiskGroups(const char* szMatch_diskgroup, LIST <STRING>& Cluster_Diskgroups);
    BOOL          GetNafos(LIST <STRING>& Cluster_NAFO);  

};

#endif   //_CLUSTER3_H_
