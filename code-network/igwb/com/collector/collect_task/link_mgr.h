#ifndef __LINK_MGR_H__
#define __LINK_MGR_H__

#include "link_group.h"
#include <assert.h>

//�������ļ��������Ķ���
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

    //��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
    virtual int  Init(void *pPara = NULL){return ERR_SUCCESS;}

    //ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual int  GetFile(const SGetFileReq &pReq) = 0;

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual int  GetFileList(const  char* szSrcDir, SListOut &pOut) = 0;

    //ɾ���ļ�����˵�ָ���ļ�
    virtual int  DelFile(const char* szFileName) = 0;

    //ȡ�ļ�����˵�ָ���ļ��ĳ���
    virtual int  GetFileLen(const char* szFileName,
                            unsigned long& uFileLen) = 0;

    //������ָ���ļ�
    virtual int  RenameFile(const char *szOld, const char *szNew) = 0;

    //�����ļ������
    virtual int  Connect() = 0;

    //�Ͽ�����
    virtual int  Disconnect() = 0;

    //�������ӷ����
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

    //�����ļ�����ģʽ
    virtual int  SetMode(int nMode) = 0;

    //�õ����һ���ļ���������Ĵ�����Ϣ
    inline const char*  GetLastErr()
    {
        return m_szErrInfo;
    }

    //ע����·
    inline int RegisterLink(const char* const szLinkName)
    {
        STRING strLinkName = szLinkName;
        //m_LinkList[m_LinkList.size()] = strLinkName;
        m_LinkList.push_back(strLinkName);

        return ERR_SUCCESS;
    }

    //��ȡpthis��Ӧ�ĵ�ǰ���õ�һ����·
    inline CLinkBase* GetActiveLink()
    {
        if (m_LinkList.size() > m_nActiveIndex)
        {
            return m_pLinkGroup->GetLink(m_LinkList[m_nActiveIndex]);
        }

        return NULL;
    }

    //����һ����·Ϊ������·
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
    char  m_szErrInfo[MAX_ERROR_LEN]; //�������һ���ļ���������Ĵ�����Ϣ

protected:
    //��ҵ�����͵���·�б��MAP
    VECTOR<STRING> m_LinkList;
    VECTOR<int>    m_LinkStatusList;
    int            m_nActiveIndex;
    CLinkGroup*    m_pLinkGroup;
}; //end of CLinkMgr

#endif //__LINK_MGR_H__
