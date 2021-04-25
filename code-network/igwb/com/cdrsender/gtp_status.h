/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : gtp_client.h
 *������Ϣ  �����ļ�����CGtpClient�Ķ���
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/
#ifndef __GTP_STATUS_H__
#define __GTP_STATUS_H__

#define PEER_IPCOUNT_MAX  10

//CDRSENDERģ��GTP'Э���״̬�ṹ
typedef struct _GtpInfo
{
    unsigned long  uAppType;             //Э������,ȡֵ��APP_TYPE
    unsigned long  uGtpVersion;          //��ǰGTP'�İ汾��
    unsigned long  nPeerActivedIP;       //�Զ˻IP��ַ
    unsigned long  uPeerIp[PEER_IPCOUNT_MAX];  //���Զ�Ԥ��10��IP��ַ
    unsigned long  uPeerPort;            //BS�Ķ˿ں�
    unsigned long  uLocalIp;             //����IP��ַ,ֻ��������õ��Ǹ�
    unsigned long  uLocalPort;           //���ض˿ں�,ֻ��������õ��Ǹ�
    unsigned long  uPeerRestartCount;    //�Զ˵���������
    unsigned long  uLocaRestartCount;    //��������Ĵ���
    unsigned short uSeqNum;              //�Ѿ��ɹ����͵�����֡�����к�            
    unsigned long uReserved[50];         //����һ���ֿռ�
} SGtpInfo;

#define GTP_NODE_INFO_LENGTH sizeof(SGtpInfo)


#endif //__GTP_STATUS_H__