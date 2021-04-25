#include <assert.h>
#include "../include/frame.h"
#include "../include/toolbox.h"
#include "cluster_struct.h"
#include "heartbeat_link.h"
#include "cluster_network.h"
#include "heartbeat_creator.h"
#include "comm_link.h"
#include "udp_link.h"
#include "resource.h"


/********************************************************
* 名称: CHeartBeatCreator::CHeartBeatCreator
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CHeartBeatCreator::CHeartBeatCreator()
{
    //
}

/********************************************************
* 名称: CHeartBeatCreator::~CHeartBeatCreator
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CHeartBeatCreator::~CHeartBeatCreator()
{
    //
}

#define LOCAL_BUFFER_SIZE  20
#define MAX_HEARTBEAT_LINK 5
/********************************************************
* 名称: CHeartBeatCreator::CreateHeartBeatList
* 功能: 创建心跳对象队列
* 输入: CClusterNetwork *pNetwork : CClusterNetwork对象的指针
* 输出:
* 返回: LIST<CHeartBeatLink*>* 返回心跳对象队列的指针
********************************************************/
LIST<CHeartBeatLink *>* CHeartBeatCreator::CreateHeartBeatList(
                                           CClusterNetwork *pNetwork)
{
    assert(NULL != pNetwork);

    LIST<CHeartBeatLink *> *pLinkList = new LIST<CHeartBeatLink *>;
    CHeartBeatLink* pLink = NULL;

    if(NULL != pLinkList)
    {

        CINIFile *pIniFile;
        pIniFile = new CINIFile(GetCfgFilePath());
        pIniFile->Open();

        //读心跳总数
        int nHBCount = pIniFile->GetInt(CFG_SEC_CLUSTER,
                                        CFG_CLSTR_HEART_BEAT_COUNT,
                                        0);

        nHBCount = nHBCount > MAX_HEARTBEAT_LINK ? MAX_HEARTBEAT_LINK : nHBCount;

        int i;
        char szSection[LOCAL_BUFFER_SIZE];
        char szLinkType[LOCAL_BUFFER_SIZE];
        UINT uPort = 0;
        for (i = 1; i <= nHBCount; i++)
        {
            SNPRINTF(szSection, LOCAL_BUFFER_SIZE, "Link%d", i);
            szSection[LOCAL_BUFFER_SIZE - 1] = '\0';

            //读出心跳名
            char szName[LOCAL_BUFFER_SIZE];
            pIniFile->GetString(szSection,
                                CFG_CLSTR_HB_NAME,
                                "",
                                szName,
                                LOCAL_BUFFER_SIZE);
            szName[LOCAL_BUFFER_SIZE - 1] = '\0';

            //读取心跳类型
            pIniFile->GetString(szSection,
                                CFG_CLSTR_HEART_BEAT_TYPE,
                                "",
                                szLinkType,
                                LOCAL_BUFFER_SIZE);
            szLinkType[LOCAL_BUFFER_SIZE - 1] = '\0';

            if (ACE_OS::strcasecmp(szLinkType, CFG_CLSTR_HB_TYPE_UDP) == 0)
            //如果是UDP类型的心跳
            {
                //读取本端心跳地址
                char szLocalIP[LOCAL_BUFFER_SIZE];
                pIniFile->GetString(szSection,
                                    CFG_CLSTR_HB_LOCAL_IP,
                                    "",
                                    szLocalIP,
                                    LOCAL_BUFFER_SIZE);
                szLocalIP[LOCAL_BUFFER_SIZE - 1] = '\0';

                //读取对端心跳地址
                char szPeerIP[LOCAL_BUFFER_SIZE];
                pIniFile->GetString(szSection,
                                    CFG_CLSTR_HB_PEER_IP,
                                    "",
                                    szPeerIP,
                                    LOCAL_BUFFER_SIZE);
                szPeerIP[LOCAL_BUFFER_SIZE - 1] = '\0';

                //读取心跳端口
                uPort = pIniFile->GetInt(szSection,
                                         CFG_CLSTR_HB_PORT,
                                         DEFAULT_UDP_LINK_PORT);

                pLink = new CUdpLink(szLocalIP,
                                     szPeerIP,
                                     uPort,
                                     pNetwork,
                                     i - 1,
                                     szName);
            }
            else if(ACE_OS::strcasecmp(szLinkType, CFG_CLSTR_HB_TYPE_COM) == 0)
            {
                //读取心跳端口
                uPort = pIniFile->GetInt(szSection,
                                         CFG_CLSTR_HB_PORT,
                                         DEFAULT_COM_LINK_PORT);

                pLink = new CCommLink(uPort,
                                      pNetwork,
                                      i - 1,
                                      szName);
            }
            else
            {
                TRACE(MTS_CLSTR, S_UNKNOWN_HEART_BEAT_TYPE, szLinkType);
            }

            if(NULL != pLink)
            {
                if(pLink->Create() != ERR_SUCCESS)
                {
                    TRACE(MTS_CLSTR, S_CREATE_HEART_BEAT_LINK,
                          pLink->GetLinkIndex(),
                          pLink->GetLinkName(),
                          pLink->GetLinkType());
                    delete pLink;
                }
                else
                {
                    pLinkList->push_back(pLink);
                }
                pLink = NULL;
            }
        }
        pIniFile->Close();
        delete pIniFile;

        if(pLinkList->size() == 0)
        {
            delete pLinkList;
            pLinkList = NULL;
        }
    }

    return pLinkList;
}
