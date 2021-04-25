#ifndef  __X25_LINK_CHECK_H__
#define  __X25_LINK_CHECK_H__

class CX25LinkCheck
{
    
public:
    CX25LinkCheck();
    ~CX25LinkCheck();
    
    static CX25LinkCheck* Instance();
    static void Destroy();
    
    int    GetLinkState(const char* szDevName);
    void   RegisterLink(const char* szDevName);
    
private:

    static CX25LinkCheck* g_X25LinkCheck;
    static ACE_Mutex      g_Mutex;
    MAP<STRING, int>      m_X25LinkStatus;
    time_t                m_nLastCheckTime;
};

#endif //__X25_LINK_CHECK_H__

