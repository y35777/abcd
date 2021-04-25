#ifndef __LINK_GROUP_H__
#define __LINK_GROUP_H__

#include "link_base.h"
#include "collect_struct.h"

class CLinkGroup
{
public:
    CLinkGroup()
    {
    }
    ~CLinkGroup()
    {
        while(m_LinkMap.size() > 0)
        {
            delete m_LinkMap.begin()->second;
            m_LinkMap.erase(m_LinkMap.begin());
        }
    }
    inline void RegisterLink(const char* const szLinkName, CLinkBase* pLink)
    {
        STRING strLinkName = szLinkName;
        if(m_LinkMap.find(strLinkName) == m_LinkMap.end())
        {
            m_LinkMap[strLinkName] = pLink;
        }
    }

    inline CLinkBase* GetLink(const STRING& strLinkName)
    {
        if(m_LinkMap.find(strLinkName) != m_LinkMap.end())
        {
            return m_LinkMap[strLinkName];
        }
        return NULL;
    }

    inline int GetLinkCount(void)
    {
        return m_LinkMap.size();
    }

    //added by chenliangwei, 2003-08-26
    //得到链路的信息，并将链路状态赋值为失败
    int GetLinkInfo(MAP<STRING, SLinkStatusMsg>& LinkStatus)
    {
        MAP<STRING, CLinkBase*>::iterator i = m_LinkMap.begin();

        for(; i != m_LinkMap.end(); i++)
        {
            CLinkBase* pLink = i->second;
            SLinkStatusMsg link_msg;
            memset(&link_msg, 0, sizeof(SLinkStatusMsg));
            if(NULL != pLink)
            {
                link_msg.nAlarmPara = pLink->GetAlarmPara();
                if(pLink->GetLocalHost()[0] != '\0')
                {
                    strncpy(link_msg.szLocalAddr,
                            pLink->GetLocalHost(),
                            sizeof(link_msg.szLocalAddr));
                    link_msg.szLocalAddr[sizeof(link_msg.szLocalAddr) - 1] = '\0';
                }

                if(pLink->GetRemoteHost()[0] != '\0')
                {
                    strncpy(link_msg.szRemoteAddr,
                            pLink->GetRemoteHost(),
                            sizeof(link_msg.szRemoteAddr));
                    link_msg.szRemoteAddr[sizeof(link_msg.szRemoteAddr) - 1] = '\0';
                }
                
            }
            link_msg.nStatus  = 0;
            LinkStatus[i->first] = link_msg;
        }

        return ERR_SUCCESS;
    }
    //added end

    //测试连接服务端
    int  TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus)
    {
        MAP<STRING, CLinkBase*>::iterator i = m_LinkMap.begin();

        for(; i != m_LinkMap.end(); i++)
        {
            CLinkBase* pLink = i->second;
            int nRet = ERR_FAIL;
            SLinkStatusMsg link_msg;
            memset(&link_msg, 0, sizeof(SLinkStatusMsg));
            if(NULL != pLink)
            {
                nRet = pLink->TestLink();
                link_msg.nAlarmPara = pLink->GetAlarmPara();
                if(pLink->GetLocalHost()[0] != '\0')
                {
                    strncpy(link_msg.szLocalAddr,
                            pLink->GetLocalHost(),
                            sizeof(link_msg.szLocalAddr));
                    link_msg.szLocalAddr[sizeof(link_msg.szLocalAddr) - 1] = '\0';
                }

                if(pLink->GetRemoteHost()[0] != '\0')
                {
                    strncpy(link_msg.szRemoteAddr,
                            pLink->GetRemoteHost(),
                            sizeof(link_msg.szRemoteAddr));
                    link_msg.szRemoteAddr[sizeof(link_msg.szRemoteAddr) - 1] = '\0';
                }
            }
            link_msg.nStatus    = nRet; 
            LinkStatus[i->first] = link_msg;
        }

        return ERR_SUCCESS;
    }

protected:
    MAP<STRING, CLinkBase*> m_LinkMap;
};

#endif //__LINK_GROUP_H__
