#ifndef __LINK_MGR_H__
#define __LINK_MGR_H__

#include "link_group.h"
#include <assert.h>

//以下是文件传输基类的定义
class  CLinkMgr
{

public:

    CLinkMgr(CLinkGroup* pLinkGroup)
    {
        m_szErrInfo[0] = '\0';
        m_nActiveIndex = 0;

        assert(NULL != pLinkGroup);
        m_pLinkGroup = pLinkGroup;
    }

    virtual  ~CLinkMgr()
    {
        m_pLinkGroup = NULL;
    }

    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual int  Init(void *pPara = NULL){return ERR_SUCCESS;}

    //实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
    virtual int  GetFile(const SGetFileReq &pReq) = 0;

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual int  GetFileList(const  char* szSrcDir, SListOut &pOut) = 0;

    //删除文件服务端的指定文件
    virtual int  DelFile(const char* szFileName) = 0;

    //取文件服务端的指定文件的长度
    virtual int  GetFileLen(const char* szFileName,
                            unsigned long& uFileLen) = 0;

    //重命名指定文件
    virtual int  RenameFile(const char *szOld, const char *szNew) = 0;

    //连接文件服务端
    virtual int  Connect() = 0;

    //断开连接
    virtual int  Disconnect() = 0;

    //测试连接服务端
    virtual int  TestLink(MAP<STRING, SLinkStatusMsg>& LinkStatus)
    {
        VECTOR<STRING>::iterator i = m_LinkList.begin();
        m_LinkStatusList.clear();

        for(; i != m_LinkList.end(); i++)
        {
            CLinkBase* pLink = m_pLinkGroup->GetLink(*i);
            int nRet = ERR_FAIL;
            SLinkStatusMsg link_msg;
            memset(&link_msg, 0, sizeof(SLinkStatusMsg));
            if(NULL != pLink)
            {
                nRet = pLink->TestLink();
                link_msg.nAlarmPara = pLink->GetAlarmPara();
                if(pLink->GetLocalHost() != '\0')
                {
                    strncpy(link_msg.szLocalAddr,
                            pLink->GetLocalHost(),
                            sizeof(link_msg.szLocalAddr));
                }
            }
            m_LinkStatusList.push_back(nRet);

            if(LinkStatus.find(*i) == LinkStatus.end())
            {
                link_msg.nStatus = nRet; 
                LinkStatus[*i]   = link_msg;
            }
        }

        return ERR_SUCCESS;
    }

    //设置文件传输模式
    virtual int  SetMode(int nMode) = 0;

    //得到最后一次文件传输操作的错误信息
    inline const char*  GetLastErr()
    {
        return m_szErrInfo;
    }

    //注册链路
    inline int RegisterLink(const char* const szLinkName)
    {
        STRING strLinkName = szLinkName;
        //m_LinkList[m_LinkList.size()] = strLinkName;
        m_LinkList.push_back(strLinkName);

        return ERR_SUCCESS;
    }

    //获取pthis对应的当前可用的一条链路
    inline CLinkBase* GetActiveLink()
    {
        if (m_LinkList.size() > m_nActiveIndex)
        {
            return m_pLinkGroup->GetLink(m_LinkList[m_nActiveIndex]);
        }

        return NULL;
    }

    //置下一条链路为激活链路
    inline int LetNextLinkActive()
    {
        m_nActiveIndex++;
        if (m_nActiveIndex >= m_LinkList.size())
        {
            m_nActiveIndex = 0;
        }

        return ERR_SUCCESS;
    }
    inline int GetLinkCount()
    {
        return m_LinkList.size();
    }

public:
    char  m_szErrInfo[MAX_ERROR_LEN]; //保存最后一次文件传输操作的错误信息

protected:
    //到业务类型到链路列表的MAP
    VECTOR<STRING> m_LinkList;
    VECTOR<int>    m_LinkStatusList;
    int            m_nActiveIndex;
    CLinkGroup*    m_pLinkGroup;
}; //end of CLinkMgr

#endif //__LINK_MGR_H__
