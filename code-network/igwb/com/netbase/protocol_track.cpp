#include "protocol_track.h"
#include "resource.h"


/******************************************************
// 函数名:  CProtocolTrack::CProtocolTrack
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    构造函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CProtocolTrack::CProtocolTrack()
{
    m_pMmlReport = 0;
}


/******************************************************
// 函数名:  CProtocolTrack::~CProtocolTrack
// 作者:    Wangfeng
// 时期:    01-10-19
// 描述:    析构函数
// 输入:
// 返回值: 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
CProtocolTrack::~CProtocolTrack()
{
    //
}

//获得处理结果的解释
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

