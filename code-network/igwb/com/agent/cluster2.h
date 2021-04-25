/***************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��agent
 *�ļ���    ��Cluster.h
 *������Ϣ  ����CCluster�Ķ����ļ��������װ�˻��SUN SC��Ⱥϵͳ������
                  �Լ�����ʱ��Ϣ�Ĺ��ܡ� 
 *�汾˵��  ��V200R002i00M00B00D1130A   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ������ƽ
 *����ʱ��  ��2002-11-01
 *�޸ļ�¼  ��
****************************************************************************/
#ifndef _CLUSTER_2_H_
#define _CLUSTER_2_H_

#include "agent_hdr.h"
#include "cluster.h"

class CCluster2 : public CCluster
{
public:
    CCluster2(const char* pszConfigFile);    //���캯��
    virtual ~CCluster2() {};                            //��������

    BOOL          IsClusterRunning(void);    //��鼯Ⱥ�Ƿ�������
    int           GetCurrMemNum(void);       //��õ�ǰ�߼������л��Ա����    
    BOOL          GetPrvNetStatus(int PrvNetID);  //���ָ��ID��˽���ĵ�ǰ״̬
    BOOL          GetActivePrvNets(const char* szFilePath, LIST<int>& PrvNets); //���SC��ǰ���˽���б�
    int           GetSCCurrMemNum(const char* szFilePath);//���SC��ǰ���Ա��
    BOOL          GetNodes(const char* szNodesInfo, LIST <STRING>& Cluster_Nodes); //���SC�����нڵ�

private:
    const STRING  GetSCDir(void);            //��ü�Ⱥ����İ�װ·��    
    const STRING  GetLogicalHost(void);      //����߼�������
    const STRING  GetDataService(void);      //��ø߿������ݷ�������

    BOOL  ReadLogIP(const char* pszFilePath, LIST<STRING>& LogIP);
    int   GetPrvNetNum(const char* szFilePath);
    BOOL  ReadLOGHOST(const char* pszFilePath, STRING& sLogHost,
                   LIST<STRING>& NodeName, LIST<STRING>& DiskGroup);
    BOOL  GetMaster(const char* szLogHostName, char* szMaster);

private:
    STRING         m_sSCDir;           //SC�����װ·��

};

#endif   //_CLUSTER2_H_
