#include "../../include/toolbox.h"
#include "x25_link_check.h"

CX25LinkCheck* CX25LinkCheck::g_X25LinkCheck = NULL;
ACE_Mutex CX25LinkCheck::g_Mutex;

CX25LinkCheck::CX25LinkCheck()
{
    m_nLastCheckTime = 0;
}

CX25LinkCheck::~CX25LinkCheck()
{

}

CX25LinkCheck* CX25LinkCheck::Instance()
{
    g_Mutex.acquire();
    if(g_X25LinkCheck == NULL)
    {
        g_X25LinkCheck = new CX25LinkCheck;
    }
    g_Mutex.release();

    return g_X25LinkCheck;
}

void CX25LinkCheck::Destroy()
{
    g_Mutex.acquire();
    if(g_X25LinkCheck != NULL)
    {
        delete g_X25LinkCheck;
        g_X25LinkCheck = NULL;
    }
    g_Mutex.release();
}

#define S_CHECK_X25_OUTPUT_FILE "./output/x25stat.output"

int CX25LinkCheck::GetLinkState(const char* szDevName)
{
    if((NULL == szDevName) || (szDevName[0] == '\0'))
    {
        return ERR_FAIL;
    }

    g_Mutex.acquire();

#ifdef _PLATFORM_HPUX
    if(m_X25LinkStatus.find(szDevName) == m_X25LinkStatus.end())
    {
        m_X25LinkStatus[szDevName] = 0;
    }

    if(time(NULL) - m_nLastCheckTime >= 60)
    {
        m_nLastCheckTime = time(NULL);
        MAP<STRING, int>::iterator i;

        //如果检测文件不存在
        if(ACE_OS::access("/usr/sbin/x25stat", F_OK) != 0)
        {
            for(i = m_X25LinkStatus.begin(); i != m_X25LinkStatus.end(); i++)
            {
                i->second = ERR_FAIL;
            }
        }
        else
        {
            CreateDeepDir("./output");
            for(i = m_X25LinkStatus.begin(); i != m_X25LinkStatus.end(); i++)
            {
                const STRING& strDevName = i->first;

                char szCmdLine[1024];
                remove(S_CHECK_X25_OUTPUT_FILE);
                sprintf(szCmdLine,
                        "/usr/sbin/x25stat -d %s > %s",
                        strDevName.c_str(),
                        S_CHECK_X25_OUTPUT_FILE);
                system(szCmdLine);

                i->second = ERR_SUCCESS;
                FILE* f = fopen(S_CHECK_X25_OUTPUT_FILE, "r");
                if(f != NULL)
                {
                    char szLine[1024];
                    for(int i = 1; i <= 14; i++)
                    {
                        if(NULL == fgets(szLine, sizeof(szLine), f))
                        {
                            break;
                        }
                    }

                    if((strstr(szLine, "State          :") == NULL)
                       || ((strstr(szLine, "State          :") != NULL)
                           && (strstr(szLine, "Connected") == NULL)))
                    {
                        i->second = ERR_FAIL;
                    }
                    fclose(f);
                }
                else
                {
                    i->second = ERR_FAIL;
                }
                remove(S_CHECK_X25_OUTPUT_FILE);
            }
        }
    }
#endif //_PLATFORM_HPUX

    int nStatus = m_X25LinkStatus[szDevName];
    g_Mutex.release();

    return nStatus;
}

void CX25LinkCheck::RegisterLink(const char* szDevName)
{
    if((NULL == szDevName) || (szDevName[0] == '\0'))
    {
        return;
    }

    g_Mutex.acquire();

    if(m_X25LinkStatus.find(szDevName) == m_X25LinkStatus.end())
    {
        m_X25LinkStatus[szDevName] = 0;
    }

    g_Mutex.release();
}
