#ifndef __HEART_BEAT_CREATOR_H__
#define __HEART_BEAT_CREATOR_H__

class CHeartBeatLink;
class CClusterNetwork;


class CHeartBeatCreator
{
public:

    CHeartBeatCreator();
    ~CHeartBeatCreator();

#define DEFAULT_UDP_LINK_PORT   11228
#define DEFAULT_COM_LINK_PORT   1

    //创建心跳对象队列
    static LIST<CHeartBeatLink *> *CreateHeartBeatList(
                                   CClusterNetwork *pNetwork);
};

#endif //__HEART_BEAT_CREATOR_H__
