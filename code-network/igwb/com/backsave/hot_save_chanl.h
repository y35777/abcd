/*
    ��Ʒ��:iGateway Bill V200
    ģ����:�����
    �ļ���:hot_save_chanl.h
    ������Ϣ:
    
    ����CHotSaveChannel��Ķ���. 
    ��Ϊʵʱ�Ʒѵ�����Ҫ�������Ʒ����ĵĵڶ��ݻ����ļ��Ի������Ĵ洢��ʵʱ�ģ�
    �������Ӵ��࣬��ɶԵ�һ����ʱ�ļ��͵ڶ��ݻ����ļ�ͬʱд���������Դ���

    �汾˵��:V200R001i00M00B00D1130A
    ��Ȩ��Ϣ:��Ȩ����(C)2001-2004 ��Ϊ�������޹�˾

    �޸ļ�¼:
    ������,2004-05-14,����.
*/



#ifndef __HOTSAVECHANNEL_H__
#define __HOTSAVECHANNEL_H__

#include "save_chanl.h"

class CHotSaveChannel : public CSaveChannel  
{
public:
    //���졢��������
    CHotSaveChannel(CPatchEncap* pPatchEncap);
    virtual ~CHotSaveChannel();

    //��ʼ������
    BOOL Init(UINT4 uAPID, UINT4 uChannelID, STRING& szChannelName, int nBillLen = 0);

protected:

    //�رջ����ļ��ĺ�������
    BOOL DoWhenFileClose(); 
    
    //�ڱ��滰���������е�д�����ļ�����
    int WriteBillFile(BYTE* pPacketInfo, BOOL bFlush); 

    //�����µĻ����ļ��Լ���ص�״̬�ļ�ˢ�²���
    BOOL CreateNewFile();

    //����״̬�ļ������Ϣ����صĳ�Ա�������г�ʼ��
    BOOL InitByStatusFile();

    //ɾ�����һ����ʱ�ļ����ڽ���״̬�ļ�У��ʱ����
    BOOL DeleteTempFile();

protected:
    FILE*     m_pSeccondFile;        //��ǰ���ڴ���ĵڶ��ݻ����ļ�����ָ��
    STRING    m_szSecondFileName;    //��ǰ���ڴ���ĵڶ��ݻ����ļ���

};

#endif //__HOTSAVECHANNEL_H__
