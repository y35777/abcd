/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ��GTP'�Ʒѽӿ�
 *�ļ���    : cdrsnd_svr.h
 *������Ϣ  �����ļ�����CCDRSndSvr�Ķ���
 *�汾˵��  ��V200R002B02   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2004-2005 ��Ϊ�������޹�˾
 *����      ��Z30971
 *����ʱ��  ��2004-06-19
 *�޸ļ�¼  ��
 **************************************************************************/
#ifndef __CDR_SENDER_H__
#define __CDR_SENDER_H__

#include "../include/precom.h"
#include "../frame/wrk_module.h"

#include "sender_status.h"
#include "cdr_buffer.h"
#include "cdr_producer.h"

class CGtpClient;

class CCDRSndSvr : public CWorkModule  
{
public:
    virtual ~CCDRSndSvr();                //��������
    CCDRSndSvr();                         //���캯��
    
    int SendMsgtoBS(MSG_INTRA* pMsg);

protected:
    //�̳еĽӿں���
    int InitInstance();                   //��ʼ��
    void ExitInstance();                  //�˳�����
    void OnTimer(const long nTimerID);    //��ʱ��
    
    MODULE_TYPE GetModuleType();          //ģ������
    void ParseMsg(MSG_INTRA* pMsgParsed); //��Ϣ�ַ�
    
    virtual void Timer1Sec();         

protected:
    CSenderStatusFile m_StatusFile;       //����״̬
    CCDRBuffer        m_CDRBuffer;        //������
    CCDRProducer*     m_pPrdCDRThrd;      //���������߳�
    CGtpClient*       m_pGtpClient;       //GTP' Client
    long              m_Timer1SecId;       //�붨ʱ��
    
};
#endif //__CDR_SENDER_H__
