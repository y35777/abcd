#ifndef __S1240_CMISE_H__
#define __S1240_CMISE_H__

#include "../collect_include.h"
#include "../collect_define.h"
#include "../collect_struct.h"
#include <assert.h>

//以下是文件传输基类的定义
class CS1240Cmise
{
public:

    CS1240Cmise()
    {
        m_szAgent[0] = '\0';
        m_szMgr[0]   = '\0';
        m_szAssPwd[0] = '\0';
        m_szActPwd[0]   = '\0';
        m_szNEName[0] = '\0';
        m_szTPID[0]   = '\0';
    }
    virtual  ~CS1240Cmise(){}

    //初始化本地服务, 是虚函数, 由派生类具体实现该方法
    virtual int  Init() = 0;

    //测试能否正常连接对端服务器
    virtual int  TestLink(){return ERR_SUCCESS;}

    virtual int  SetTPMsg(const char* const szAgent,
                          const char* const szMgr,
                          const char* const szAssPwd,
                          const char* const szActPwd,
                          const char* const szNename,
                          const char* const szTpid)
    {
        assert(szAgent != NULL);
        assert(szMgr != NULL);
        assert(szNename != NULL);
        assert(szTpid != NULL);

        if(szAssPwd != NULL)
        {
            strncpy(m_szAssPwd, szAssPwd, sizeof(m_szAssPwd));
            m_szAssPwd[sizeof(m_szAssPwd) - 1] = '\0';
        }
    
        if(szActPwd != NULL)
        {
            strncpy(m_szActPwd, szActPwd, sizeof(m_szActPwd));
            m_szActPwd[sizeof(m_szActPwd) - 1] = '\0';
        }

        strncpy(m_szAgent, szAgent, sizeof(m_szAgent));
        m_szAgent[sizeof(m_szAgent) - 1] = '\0';

        strncpy(m_szMgr, szMgr, sizeof(m_szMgr));
        m_szMgr[sizeof(m_szMgr) - 1] = '\0';

        strncpy(m_szNEName, szNename, sizeof(m_szNEName));
        m_szNEName[sizeof(m_szNEName) - 1] = '\0';

        strncpy(m_szTPID, szTpid, sizeof(m_szTPID));
        m_szTPID[sizeof(m_szTPID) - 1] = '\0';

        return ERR_SUCCESS;
    }

    virtual int  GetFilePool(SS1240FilePoolMsg& pool_msg) = 0;

    virtual int  SetFilePool(int nAlarmLimit) = 0;

    virtual int  GetFile(const char* const szFileId,
                         SS1240FileMsg&    file_msg) = 0;

    virtual int  SetFile(const char* const szFileId,
                         int               nFilestatus) = 0;

    //获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
    virtual int  GetFileList(int nFileStatus,
                             VECTOR<SS1240FileMsg>  &Out) = 0;

protected:

    char m_szAgent[100];
    char m_szMgr[100];
    char m_szAssPwd[100];
    char m_szActPwd[100];
    char m_szNEName[100];
    char m_szTPID[100];
};

#endif //__S1240_CMISE_H__
