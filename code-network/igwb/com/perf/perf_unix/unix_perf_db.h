
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
    //保存上次查询的CPU状态,最大允许2个CPU.
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
    
    // 获得操作系统相关性能数据项的描述
    virtual void GetOSStatus(BYTE *pbPara, int nNum, char*& szAllPerf);
    
    /*
        参数：
        nIdle1，第1个CPU空闲率
        nUser1，用户代码CPU占用率
        nKernel1，内核代码CPU占用率

        nIdle2，第2个CPU空闲率
        nUser2，用户代码CPU占用率
        nKernel2，内核代码CPU占用率

        返回：成功返回0，失败返回非0。
    */
    int GetCPUTime(int& nIdle1, int& nUser1,int& nKernel1, int& nWait1,
                   int& nIdle2, int& nUser2,int& nKernel2, int& nWait2);

    /*
        参数：
        pStat,输出分区使用数据.

        返回：
        成功返回分区数，失败返回非-1.
    */
    int GetDiskStatistic(PLogicalDisk* pStat);

    /*
        参数：
        pMountName,分区名
        nTotal,分区总容量
        nAvail,分区可用空间

        返回：
        成功返回0，失败返回-1.
    */
    int GetDiskStatistic(char* pMountName,long* pTotal,long* pAvail);

    /*
        参数：
        nTotal,物理内存总量.
        nAvai,空闲物理内存.

        返回：
        成功返回0，失败返回非0.
    */
    int GetMemStatistic(long* pTotal,long* pAvail,
                        long* pVirTotal,long* pVirAvail);
    
    //获取系统配置的CPU数量.
    int GetCPUNumber();
};

#endif /* __UNIX_PERF_DB__ */

