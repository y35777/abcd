#ifndef __PROCCHANNEL_H__
#define __PROCCHANNEL_H__

#include "../include/precom.h"
#include "convert_comm.h"

struct SBill;
class CFormat;

class CBillBuffer
{
public:
    CBillBuffer();
    virtual ~CBillBuffer();

    UINT4 GetBillDataLength();     //��ȡ�������еĻ��������ܳ���
    void AddABill(SBill* pABill);  //�򻺳��������һ�Ż���
    //�ӻ�������ȡ����һ�Ż����������û�����������ɾ��
    SBill* GetABill();
protected:
    UINT4 m_uBillDataLen;       //�������еĻ��������ܳ���
    LIST<SBill*> m_BillList;    //������б�������л���������
};

class CProcChannel
{
public:
    //���졢��������
    CProcChannel();
    virtual ~CProcChannel();

    //�������úͻ�ȡ����
    void SetLastPID(UINT4 uLastPID);

    //����ͨ�������ò��������г�ʼ��
    virtual BOOL Init(UINT4 uAPID, INT4 nChannelID, BOOL bNeedConvert);
    //ת��һ�Ż���
    virtual BOOL ConvertABill(SBill* pABill, SBill* pConvertedBill, 
                              CFormat* pDestFmt);
    //����һ�Ż���
    void ProcessABill(SBill* pABill, CFormat* pDestFmt);
    void GetBillPacket(BYTE* &pPacket, UINT4 &uPacketLen);  //���������ݴ��
    //����Ӻ����ģ�鷢�͹�����ͬ����Ϣ
    void SynChannelInfo(const SChannelSynInfo* pInfo);
    SChannelSynInfo* GetSynInfo();  //��ȡͬ����Ϣ
    virtual BOOL AddABill(SBill& ABill);

protected:
    //����ת����Ļ���У���
    BOOL CorrectChkSum(BYTE* pABill, UINT4 uLen, 
                       INT4 nCheckSumPos, 
                       INT4 nCheckPosLow, 
                       INT4 nCheckPosHigh);

protected:
    CBillBuffer     m_BillBuffer;       //��������������
    SChannelSynInfo m_ChannelSynInfo;   //ͬ��״̬��Ϣ
    BOOL            m_bNeedConvert;     //�Ƿ���и�ʽת����־
    UINT4           m_uAPID;            //��ǰ������ID��
    INT4            m_nChannelID;       //��ǰ��ͨ����

    BOOL            m_bCorrectChkSum;   //�Ƿ�Ҫ����ת����Ļ���У���
    INT4            m_nCheckSumPos;     //У��������ջ����е�λ��
    INT4            m_nCheckPosLow;     //ҪУ��ĵ��ֽ�λ��  
    INT4            m_nCheckPosHigh;    //ҪУ��ĸ��ֽ�λ��
};

#endif
