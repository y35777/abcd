#ifndef __FRONTSTORAGE_H__
#define __FRONTSTORAGE_H__

#include "../include/precom.h"
#include "front_save.h"

class CPacketCache;

class CFrontStorage : public CWorkModule  
{
public:
    //���졢��������
    virtual ~CFrontStorage();
    CFrontStorage(UINT4 uAPID, CPacketCache* pPacketCache);

protected:
    //�̳еĽӿں���
    int InitInstance();     //��ʼ������
    void ExitInstance();    //ģ���˳�ʱ��������
    void OnTimer(const long nTimerID);  //��ʱ���¼�������
    //��ȡģ������
    MODULE_TYPE GetModuleType();
    void ParseMsg(MSG_INTRA* pMsgParsed); //ʵ����Ϣ�ַ�����ش���
    
    //�ڲ�����
    //����"���󱣴滰����"����֡
    void ProcSavePacketMsg(MSG_INTRA* pMessage);
    //����"���������ָ�"����֡
    void ProcResumeMsg(MSG_INTRA* pMessage);
    //����"֪ͨ״̬��Ϣ"����֡
    void ProcInformStatusMsg(MSG_INTRA* pMsgParsed);
    void ProcessPacket(BYTE* pPacketInfo);  //���������̴���
protected:
    CFrontSave      m_Save;         //���о���Ĵ���������Ķ���
    CPacketCache*   m_pPacketCache; //������������ָ��
    BOOL            m_bError;       //��ʾ�Ƿ���ڴ���
    long            m_lSecondTimerID;   //���¼�ʱ��
    UINT4           m_uLastPID;         //���һ���İ���
    BYTE*           m_pPacket;          //���һ��������ָ��
    UINT4           m_uPacketLen;       //���һ�������ݳ���
    UINT4           m_uAPID;            //������
};

#endif 
