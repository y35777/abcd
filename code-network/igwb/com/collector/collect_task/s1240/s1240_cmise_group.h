#ifndef __CMISE_GROUP_H__
#define __CMISE_GROUP_H__

#include "s1240_cmise.h"

class CS1240CmiseGroup
{
public:
    CS1240CmiseGroup()
    {
    }
    ~CS1240CmiseGroup()
    {
        while(m_CmiseMap.size() > 0)
        {
            delete m_CmiseMap.begin()->second;
            m_CmiseMap.erase(m_CmiseMap.begin());
        }
    }
    inline void RegisterCmise(const char* const szCmiseName, CS1240Cmise* pCmise)
    {
        STRING strCmiseName = szCmiseName;
        if(m_CmiseMap.find(strCmiseName) == m_CmiseMap.end())
        {
            m_CmiseMap[strCmiseName] = pCmise;
        }
    }

    inline CS1240Cmise* GetCmise(const STRING& strCmiseName)
    {
        if(m_CmiseMap.find(strCmiseName) != m_CmiseMap.end())
        {
            return m_CmiseMap[strCmiseName];
        }
        return NULL;
    }

    inline int GetCmiseCount(void)
    {
        return m_CmiseMap.size();
    }
protected:
    MAP<STRING, CS1240Cmise*> m_CmiseMap;
};

#endif //__CMISE_GROUP_H__
