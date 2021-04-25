#ifndef  __PROTOCOL_TRACK_H__
#define  __PROTOCOL_TRACK_H__

#include "../include/frame.h"
#include "../comm/comm_struct.h"

//CProtocolTrack�����˶�Э��֡�ķ���
#define SPACE_NUM_BEFORE 16

class CMMLReport;
class CProtocolTrack
{
public:

    //���캯��
    CProtocolTrack();

    //��������
    virtual ~CProtocolTrack();

    //ʵʩ��Э��֡�ķ���,�ɺ����ڲ������ڴ棬�����߲���Ҫ�ͷ�
    virtual const char* Analysis(void* const pFrame, int nProcRst = 0) = 0;

protected:

    //��ô������Ľ���
    const char* GetProcRstExplain(int nProcRst);

    //MML���ķ�װ����
    CMMLReport* m_pMmlReport;

    //�ռ�Э�����ݵĻ�����
    STRING m_strTrackMsg;
};


#endif //__PROTOCOL_TRACK_H__
