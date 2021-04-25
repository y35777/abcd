/***************************************************************************
 *��Ʒ��        ��iGateway Bill V200
 *ģ����        ��˫���ӿ�ģ��(ClusterInterface)
 *�ļ���        ��cluster_interface.h
 *������Ϣ      �������װ��˫���ӿ�ģ�����˫������Ľӿڡ�
 *�汾˵��      ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ      ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����          ������ƽ
 *����ʱ��      ��2001-10-30
 *�޸ļ�¼      ��
****************************************************************************/

#ifndef _CLUSTER_INTERFACE_H_
#define _CLUSTER_INTERFACE_H_

#include "cluster_hdr.h"

class CClusterInterface : public CWorkModule
{
public:
    CClusterInterface();                 //���캯��
    virtual ~CClusterInterface();        //��������
    virtual int InitInstance();          //��ʼ������(�̳и���)
    virtual void ExitInstance();         //�˳�������(�̳и���)

protected:
    void ParseMsg(MSG_INTRA* pMsg);      //��Ϣ������(�̳и���)
    virtual MODULE_TYPE GetModuleType(); //��ñ�ģ�������(�̳и���)
    void OnTimer(const long nTimerID);   //��ʱ����Ϣ(�̳и���) 

    void SendActiveAnsToCluster(BOOL bResult);    //���ͼ���Ӧ��
    void ProcInitAns(BOOL bResult);               //��ʼ��Ӧ��
    void ProcActiveAns(BOOL bResult);             //������Ӧ��
    void Active(void);                            //�������
    void DeActive(void);                          //ȥ�������
    void Shutdown(void);                          //�رղ���

    void ProcAskedFailOver(const PROCESS_TYPE pt,
                           const MODULE_TYPE  mt,
                           const BYTE nCause, 
                           const void* pData = 0,
                           const UINT4 nLen = 0); //����������

    void ProcManualOverCmd(void);                 //�����ֹ���������
    void ProcDiskSpaceFull(void);                 //������̿ռ��� 
    void ProcDiskSpaceRecovery(void);             //������̿ռ�ָ�
    
    void Timer5Sec(void);                         //5���Ӷ�ʱ������
    
    
//��Ա����
private:
    ECLSTRINTSTATUS   m_nStatus;         //˫���ӿ�ģ�鵱ǰ״̬
    SFailOverCause*   m_pFailOverCause;  //��ǰ���ڽ��е�����ԭ��
    long              m_lTimerID5Sec;    //5���Ӷ�ʱ��ID
    BOOL              m_bNetworkPaused;  //����ģ����ֹͣ���ջ����ı�־
    BYTE              m_yDiskFullMask;   //���̿ռ�����־ 
    UINT2             m_nAPCount;        //��������
    
    BOOL              m_bSingleServer;  //added by chenliangwei, 2003-04-23

};


#endif //_CLUSTER_INTERFACE_H_
