#include "protocol_track.h"
#include "resource.h"


/******************************************************
// ������:  CProtocolTrack::CProtocolTrack
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ���캯��
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CProtocolTrack::CProtocolTrack()
{
    m_pMmlReport = 0;
}


/******************************************************
// ������:  CProtocolTrack::~CProtocolTrack
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ��������
// ����:
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CProtocolTrack::~CProtocolTrack()
{
    //
}

//��ô������Ľ���
const char* CProtocolTrack::GetProcRstExplain(int nProcRst)
{
    switch(nProcRst) 
    {
    case ERR_SUCCESS:
        return "";

    case ERR_LICENSE_EXPIRED:
        return S_ERR_LICENSE_EXPIRED;

    case ERR_APP_NOT_ACTIVED:
        return S_ERR_APP_NOT_ACTIVED;

    case ERR_SOCKET_SEND_MSG:
        return S_ERR_SOCKET_SEND_MSG;

    case ERR_FAIL:
        return S_ERR_INNER_ERROR;

    default:
        return S_ERR_UNKNOWN_ERROR;
    }
}

