
#ifndef __UNIX_PERF_DB__
#define __UNIX_PERF_DB__

#include <sys/types.h>
#include <sys/sysinfo.h>

#ifndef _PLATFORM_HPUX
#include  <sys/kstat.h> 
#include  <kstat.h> 
#endif

#define CPU_STAT "cpu_stat"    

#include "../os_perf_db.h"

class CUNIXPerfDB : public COSPerfDB
{
protected:
    //�����ϴβ�ѯ��CPU״̬,�������2��CPU.
#ifdef _PLATFORM_HPUX
    int        m_OldCPUStat[2][5];
#elif defined (_PLATFORM_SOLARIS)
    cpu_stat_t m_OldCPUStat[2];
#endif

    int m_nCPUNumber;

public:
    CUNIXPerfDB(void)
    {
        m_nCPUNumber = GetCPUNumber();
        if(m_nCPUNumber > 2) m_nCPUNumber = 2;

        memset(m_OldCPUStat,0,sizeof(m_OldCPUStat));
    }

    CUNIXPerfDB(const char *szFront,const char *szBack):
                COSPerfDB(szFront,szBack)
    {
        m_nCPUNumber = GetCPUNumber();
        if(m_nCPUNumber > 2) m_nCPUNumber = 2;

        memset(m_OldCPUStat,0,sizeof(m_OldCPUStat));
    }


    ~CUNIXPerfDB(void) 
    {
    }

    virtual void GetPerfStatistic(SOSPerfStat& OSPerfStat);
    
    // ��ò���ϵͳ������������������
    virtual void GetOSStatus(BYTE *pbPara, int nNum, char*& szAllPerf);
    
    /*
        ������
        nIdle1����1��CPU������
        nUser1���û�����CPUռ����
        nKernel1���ں˴���CPUռ����

        nIdle2����2��CPU������
        nUser2���û�����CPUռ����
        nKernel2���ں˴���CPUռ����

        ���أ��ɹ�����0��ʧ�ܷ��ط�0��
    */
    int GetCPUTime(int& nIdle1, int& nUser1,int& nKernel1, int& nWait1,
                   int& nIdle2, int& nUser2,int& nKernel2, int& nWait2);

    /*
        ������
        pStat,�������ʹ������.

        ���أ�
        �ɹ����ط�������ʧ�ܷ��ط�-1.
    */
    int GetDiskStatistic(PLogicalDisk* pStat);

    /*
        ������
        pMountName,������
        nTotal,����������
        nAvail,�������ÿռ�

        ���أ�
        �ɹ�����0��ʧ�ܷ���-1.
    */
    int GetDiskStatistic(char* pMountName,long* pTotal,long* pAvail);

    /*
        ������
        nTotal,�����ڴ�����.
        nAvai,���������ڴ�.

        ���أ�
        �ɹ�����0��ʧ�ܷ��ط�0.
    */
    int GetMemStatistic(long* pTotal,long* pAvail,
                        long* pVirTotal,long* pVirAvail);
    
    //��ȡϵͳ���õ�CPU����.
    int GetCPUNumber();
};

#endif /* __UNIX_PERF_DB__ */

