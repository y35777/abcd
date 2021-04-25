#ifndef __S1240_LINK_MGR_H__
#define __S1240_LINK_MGR_H__

#include "s1240_cmise_group.h"

//�������ļ��������Ķ���
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

    //�����ܷ��������ӶԶ˷�����
    int TestLink();

    int GetFilePool(SS1240FilePoolMsg& pool_msg);

    int SetFilePool(int nAlarmLimit);

    int GetCmiseFile(const char* const szFileId,
                     SS1240FileMsg&    file_msg);

    int SetCmiseFile(const char* const szFileId,
                     int               nFilestatus);

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    int GetCmiseFileList(int  nFileStatus,
                         VECTOR<SS1240FileMsg>  &Out);

    //ע����·
    inline int RegisterCmise(const char* const szCmiseName)
    {
        m_CmiseList.push_back(szCmiseName);

        return ERR_SUCCESS;
    }

    //��ȡpthis��Ӧ�ĵ�ǰ���õ�һ����·
    inline CS1240Cmise* GetActiveCmise()
    {
        if (m_CmiseList.size() > m_nActiveIndex)
        {
            return m_pCmiseGroup->GetCmise(m_CmiseList[m_nActiveIndex]);
        }

        return NULL;
    }

    //����һ����·Ϊ������·
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
    //��ҵ�����͵���·�б��MAP
    VECTOR<STRING>       m_CmiseList;
    VECTOR<int>          m_CmiseStatusList;
    int                  m_nActiveIndex;
    CS1240CmiseGroup*    m_pCmiseGroup;

}; 

#endif //__S1240_LINK_MGR_H__

