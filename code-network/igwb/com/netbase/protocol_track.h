#ifndef  __PROTOCOL_TRACK_H__
#define  __PROTOCOL_TRACK_H__

#include "../include/frame.h"
#include "../comm/comm_struct.h"

//CProtocolTrack抽象了对协议帧的分析
#define SPACE_NUM_BEFORE 16

class CMMLReport;
class CProtocolTrack
{
public:

    //构造函数
    CProtocolTrack();

    //析构函数
    virtual ~CProtocolTrack();

    //实施对协议帧的分析,由函数内部分配内存，调用者不需要释放
    virtual const char* Analysis(void* const pFrame, int nProcRst = 0) = 0;

protected:

    //获得处理结果的解释
    const char* GetProcRstExplain(int nProcRst);

    //MML报文封装工具
    CMMLReport* m_pMmlReport;

    //收集协议内容的缓冲区
    STRING m_strTrackMsg;
};


#endif //__PROTOCOL_TRACK_H__
