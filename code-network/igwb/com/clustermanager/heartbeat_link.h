#ifndef __HEART_BEAT_LINK_H__
#define __HEART_BEAT_LINK_H__

#include "cluster_struct.h"

enum LINK_TYPE
{
    LT_COM,     //串口心跳类型
    LT_UDP      //UDP心跳类型
};

class CClusterNetwork;

class CHeartBeatLink
{
/*
    成员函数
*/
public:
    CHeartBeatLink(CClusterNetwork* pNetwork,
                   int nLinkIndex,
                   const char* const szLinkName = 0);
    virtual ~CHeartBeatLink();

    //创建心跳端口
    virtual int Create() = 0;
    //向对端发送消息
    virtual int Send(SClusterFrame* pFrame) = 0;
    //处理对端发来的消息
    virtual void OnReceive(SClusterFrame* pFrame);
    //获取心跳链路类型
    LINK_TYPE GetLinkType();
    //获取心跳链路名称
    const char* GetLinkName();
    //获取心跳链路序列号
    int GetLinkIndex();
    //获取心跳是否可用
    BOOL GetbIsAvailable();
    //置本心跳对象不可用
    void Disable();
    //秒定时器
    virtual void Timer1Sec();

/*
    成员变量
*/
protected:
   //双机通讯对象的指针
   CClusterNetwork *    m_pNetwork;

   //心跳链路类型
   LINK_TYPE            m_nLinkType;

   //心跳链路是否可用
   BOOL                 m_bIsAvailable;

   //心跳链路中断检测计时器
   int                  m_nLinkBrokenCounter;
#define LINKBROKEN_TIMEOUT  15

   //心跳名
   STRING               m_strLinkName;

   //心跳序列号
   int                  m_nLinkIndex;

};

#endif //__HEART_BEAT_LINK_H__
