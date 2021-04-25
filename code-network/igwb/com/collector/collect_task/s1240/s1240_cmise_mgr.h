#ifndef __S1240_LINK_MGR_H__
#define __S1240_LINK_MGR_H__

#include "s1240_cmise_group.h"

//以下是文件传输基类的定义
class  CS1240CmiseMgr
{

public:
    CS1240CmiseMgr(CS1240CmiseGroup* pCmiseGroup)
    {
        m_nActiveIndex = 0;

        assert(NULL != pCmiseGroup);
        m_pCmiseGroup = pCmiseGroup;
    }

    virtual  ~CS1240CmiseMgr()
    {
        m_pCmiseGroup = NULL;
    }

    //测试能否正常连接对端服务器
    int TestLink();

    int GetFilePool(SS1240FilePoolMsg& pool_msg);

    int SetFilePool(int nAlarmLimit);

    int GetCmiseFile(const char* const szFileId,
                     SS1240FileMsg&    file_msg);

    int SetCmiseFile(const char* const szFileId,
                     int               nFilestatus);

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    int GetCmiseFileList(int  nFileStatus,
                         VECTOR<SS1240FileMsg>  &Out);

    //注册链路
    inline int RegisterCmise(const char* const szCmiseName)
    {
        m_CmiseList.push_back(szCmiseName);

        return ERR_SUCCESS;
    }

    //获取pthis对应的当前可用的一条链路
    inline CS1240Cmise* GetActiveCmise()
    {
        if (m_CmiseList.size() > m_nActiveIndex)
        {
            return m_pCmiseGroup->GetCmise(m_CmiseList[m_nActiveIndex]);
        }

        return NULL;
    }

    //置下一条链路为激活链路
    inline int LetNextCmiseActive()
    {
        m_nActiveIndex++;
        if (m_nActiveIndex >= m_CmiseList.size())
        {
            m_nActiveIndex = 0;
        }

        return ERR_SUCCESS;
    }
    inline int GetCmiseCount()
    {
        return m_CmiseList.size();
    }

protected:
    //到业务类型到链路列表的MAP
    VECTOR<STRING>       m_CmiseList;
    VECTOR<int>          m_CmiseStatusList;
    int                  m_nActiveIndex;
    CS1240CmiseGroup*    m_pCmiseGroup;

}; 

#endif //__S1240_LINK_MGR_H__

