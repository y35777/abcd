/**************************************************************************
 *产品名    : iGateway Bill V200        
 *模块名    : 性能模块(Perf)
 *文件名    : unix_perf_db.cpp
 *描述信息  : 本文件是类CUNIXPerfDB的实现
 *版本说明  : V200R001i00M00B00D1130A   
 *版权信息  : 版权所有(c) 2001-2002 华为技术有限公司
 *作者      : 方卫华
 *创建时间  : 2001-11-05
 *修改记录  : 
 **************************************************************************/
 
#include "unix_perf_db.h"
#include <sys/utsname.h>
#include <unistd.h>
#include "../../utility/mml_report.h"
#include "../resource.h"
#ifdef _PLATFORM_HPUX
#include <sys/pstat.h>
#else
#include "../../include/system1.h"
#endif
/**************************************************************************
函数原型    public: void GetOSStatus (char*& szAllPerf);
功能描述    获得操作系统相关性能数据项的描述
输入参数    pbPara  －参数信息
            nNum    －参数个数
            szAllPerf - 输出MML报文
输出参数    包含操作系统相关性能数据的MML报文字符串
返回值      无
抛出异常    无
***************************************************************************/
void CUNIXPerfDB::GetOSStatus(BYTE *pbPara, int nNum, char*& szAllPerf)
{
    int nIdle1 = 0,nUser1 = 0,nKernel1 = 0,nWait1 = 0;
    int nIdle2 = 0,nUser2 = 0,nKernel2 = 0,nWait2 = 0;
    int i = 0;

    long nTotal = 0,nFree = 0;

    CMMLReport MMLReport(S_PERF_TITLE);
    MMLReport.MakeReportHead();
#ifdef _PRODUCT_MOTO
    MMLReport.MakeCmdDisplay("LST PERF:",908,0,"O&M");
#else
    MMLReport.MakeCmdDisplay("LST PERF:",908,0,"TASKCMD");
#endif
    MMLReport.AddString(S_PERF_BILLSVRNO,8,ALIGN_LEFT);
	//张玉新 2002-05-25 问题单D13961
    MMLReport.AddString(S_PERF_HEARTBEAT,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_CPU_PER,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_CPU_PER,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_FRONTSAVE,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_BACKSAVE,16,ALIGN_LEFT);
    MMLReport.AddString(S_PERF_MEM,16,ALIGN_LEFT);
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.AddString(STR_NEWLINE);

    char szTmp[512];
    char *pTmp = &szTmp[0];

    //主备信息
    sprintf(szTmp,"%d",pbPara[0]);
    MMLReport.AddString(szTmp,8,ALIGN_LEFT);

    //心跳信息
    sprintf(szTmp,"%d/%d",pbPara[2],pbPara[1]);
    MMLReport.AddString(szTmp,8,ALIGN_LEFT);

    //CPU使用信息
    if(GetCPUTime(nIdle1,nUser1,nKernel1,nWait1,
               nIdle2,nUser2,nKernel2,nWait2) == 0)
    {
        if(m_nCPUNumber == 2)
        {
            sprintf(szTmp,"%d",nUser1 + nKernel1);
            MMLReport.AddString(szTmp,16,ALIGN_LEFT);

            sprintf(szTmp,"%d",nUser2 + nKernel2);
            MMLReport.AddString(szTmp,16,ALIGN_LEFT);
        }
        else
        {
            sprintf(szTmp,"%d",nUser1 + nKernel1);
            MMLReport.AddString(szTmp,16,ALIGN_LEFT);

            MMLReport.AddString("-",16,ALIGN_LEFT);
        }
    }
    else
    {
        MMLReport.AddString("-",16,ALIGN_LEFT);
        MMLReport.AddString("-",16,ALIGN_LEFT);
    }

    //前后存盘磁盘空间
    char szFrontSave[256],szBackSave[256];
    szFrontSave[0] = 0;
    szBackSave[0] = 0;

    if(GetDiskStatistic(m_szFrontSavePath,&nTotal,&nFree) == 0)
    {
        sprintf(szFrontSave,"%d/%d",nFree / 1024,nTotal / 1024);
        MMLReport.AddString(szFrontSave,16,ALIGN_LEFT);
    }
    else
    {
        MMLReport.AddString("-",16,ALIGN_LEFT);
    }

    if(GetDiskStatistic(m_szBackSavePath,&nTotal,&nFree) == 0)
    {
        sprintf(szBackSave,"%d/%d",nFree / 1024,nTotal / 1024);
        MMLReport.AddString(szBackSave,16,ALIGN_LEFT);
    }
    else
    {
        MMLReport.AddString("-",16,ALIGN_LEFT);
    }

    //内存使用信息
    long nVirTotal = 0,nVirAvail = 0;

    GetMemStatistic(&nTotal,&nFree,&nVirTotal,&nVirAvail);

    sprintf(szTmp,"%d/%d",nFree,nTotal);
    MMLReport.AddString(szTmp,16,ALIGN_LEFT);
    MMLReport.AddString(STR_NEWLINE);
    MMLReport.MakeReportTail();

    szAllPerf = new char[strlen(MMLReport.GetReport()) + 1];
    sprintf(szAllPerf,"%s",MMLReport.GetReport());
}

/*
    获取系统性能数据.
    OSPerfStat,仅返回磁盘分区的容量,
    CPU和内存占用率不在这里返回.
*/
void CUNIXPerfDB::GetPerfStatistic(SOSPerfStat& OSPerfStat)
{
    int nCount = 0;

    OSPerfStat.nCPUs = 0;
    OSPerfStat.CPUPercent = NULL;

    OSPerfStat.uPhyMemTot = 0;
    OSPerfStat.uPhyMemAva = 0;
    OSPerfStat.uVirMemTot = 0;
    OSPerfStat.uVirMemAva = 0;

    OSPerfStat.nLogDisk = 0;
    OSPerfStat.Disks = NULL;

    nCount = GetDiskStatistic(&(OSPerfStat.Disks));
    if(nCount > 0)
    {
        OSPerfStat.nLogDisk = nCount;
    }
	//获得CPU占用率 张玉新 2002-12-21
	int nIdle[2] = {0,0};
	int nUser[2] = {0,0};
	int nKernel[2] = {0,0};
	int nWait[2] = {0,0};
	if(GetCPUTime(nIdle[0],nUser[0],nKernel[0],nWait[0],
               nIdle[1],nUser[1],nKernel[1],nWait[1]) == 0)
    {
        if(nIdle[1] == 0 && nUser[1] == 0 
			&& nKernel[1] == 0 && nWait[1] == 0)
		{
			OSPerfStat.CPUPercent = new BYTE[1];
			OSPerfStat.nCPUs = 1;
			OSPerfStat.CPUPercent[0] = nUser[0] + nKernel[0];
		}
		else
		{
			OSPerfStat.nCPUs = 2;
			OSPerfStat.CPUPercent = new BYTE[2];
			for(int i = 0; i < OSPerfStat.nCPUs; i++)
			{
				OSPerfStat.CPUPercent[i] = nUser[i] + nKernel[i];
			}
		}
	}
    //内存使用率
	//内存使用信息
    GetMemStatistic((long *)&(OSPerfStat.uPhyMemTot),
		            (long *)&(OSPerfStat.uPhyMemAva),
                    (long *)&(OSPerfStat.uVirMemTot),
					(long *)&(OSPerfStat.uVirMemAva));
}

/*
    获取CPU占用率,最大支持2个CPU.
    nIdle1, 第一个CPU的空闲
    nUser1,
    nKernel1,
    nWait1

    nIdle1,
    nUser1,
    nKernel1,
    nWait1

    返回:成功返回0,失败返回-1.
*/
int CUNIXPerfDB::GetCPUTime(int& nIdle1, int& nUser1,
                            int& nKernel1, int& nWait1,
                            int& nIdle2, int& nUser2,
                            int& nKernel2, int& nWait2)
{
#ifdef _PLATFORM_HPUX 
    nIdle1 = 0;
    nUser1 = 0;
    nKernel1 = 0;
    nWait1 = 0;

    nIdle2 = 0;
    nUser2 = 0;
    nKernel2 = 0;
    nWait2 = 0;

    double dRatio = 0;
    double dTotal = 0;
    long   lTmp   = 0;

    int nCount = 0,nLoop = 0;
    struct pst_dynamic    psd;
    if(pstat_getdynamic(&psd, sizeof(psd), 1, 0) == -1)
    {
        return 0;
    }

    struct pst_processor *psp = new pst_processor[psd.psd_proc_cnt];
    nCount = pstat_getprocessor(psp, sizeof(pst_processor), psd.psd_proc_cnt, 0);
    if(nCount <= 0)
    {
        delete psp;
        return 0;
    }

    m_nCPUNumber = GetCPUNumber();
    for(int i = 0;i < m_nCPUNumber;i++)
    {
        for(nLoop = 0;nLoop < 5;nLoop++)
        {
            lTmp = psp[i].psp_cpu_time[nLoop];

            psp[i].psp_cpu_time[nLoop] 
                   -= m_OldCPUStat[i][nLoop];

            m_OldCPUStat[i][nLoop] = lTmp;
        }
    }

    //计算第1个CPU的占用率.
    dTotal = 0;
    for(nLoop = 0; nLoop < 5; nLoop++)
    {
        dTotal += psp[0].psp_cpu_time[nLoop];
    }

    if (dTotal)
    {
        dRatio = 100.0 / dTotal;
    }
    else
    {
        dRatio = 0;
    }

    //常量CPU_IDLE(0)、CPU_USER(1)、CPU_KERNEL(2)、
    //CPU_WAIT(3)、CPU_STATES(4)定义在"sys/sysinfo.h"中

    nIdle1   = (int)((psp[0].psp_cpu_time[CPU_WAIT]   * dRatio) + 0.5);
    nUser1   = (int)((psp[0].psp_cpu_time[CPU_IDLE]   * dRatio) + 0.5);
    nKernel1 = (int)((psp[0].psp_cpu_time[CPU_WAIT] * dRatio) + 0.5);
    nWait1   = (int)((psp[0].psp_cpu_time[CPU_USER]   * dRatio) + 0.5);

    //by ldf 2004-04-27 把CPU测量方法放宽松，对应网上问题200312230035
    nKernel1 = 100 - nIdle1 - nUser1 - nWait1;
    //end

    if(m_nCPUNumber == 1)
    {
        nIdle2 = nUser2 = nKernel2 = nWait2 = 0;
        delete psp;
        return 0;
    }

    //计算第2个CPU的占用率.
    dTotal = 0;
    for (nLoop = 0; nLoop < 5; nLoop++)
    {
        dTotal += psp[0].psp_cpu_time[nLoop];
    }

    if (dTotal)
    {
        dRatio = 100.0 / dTotal;
    }
    else
    {
        dRatio = 0;
    }

    nIdle2   = (int)((psp[1].psp_cpu_time[CPU_IDLE]   * dRatio) + 0.5);
    nUser2   = (int)((psp[1].psp_cpu_time[CPU_USER]   * dRatio) + 0.5);
    nKernel2 = (int)((psp[1].psp_cpu_time[CPU_KERNEL] * dRatio) + 0.5);
    nWait2   = (int)((psp[1].psp_cpu_time[CPU_WAIT]   * dRatio) + 0.5);

    //by ldf 2004-04-27 把CPU测量方法放宽松，对应网上问题200312230035
    nKernel2 = 100 - nIdle2 - nUser2 - nWait2;
    //end

    delete psp;
    return 0;
#else
    int nCount = 0,nLoop = 0;

    //CPU当前状态数据,最大允许2个CPU.
    cpu_stat_t stCurStat[2];

    double dRatio = 0;
    double dTotal = 0;
    long   lTmp   = 0;

    kstat_ctl_t   *kc = NULL;
    kstat_t       *ks = NULL;

    //打开内核数据结构
    kc = kstat_open();
    if(kc == NULL)
        return -1;

    //读核心数据
    for(ks = kc->kc_chain;ks != NULL;ks = ks->ks_next)
    {
        if(strncmp(ks->ks_name,"cpu_stat",8) == 0)
        {
            if(kstat_read(kc,ks,&stCurStat[nCount]) < 0)
            {
                kstat_close(kc);
                return -1;
            }
            nCount++;
        }

        if(nCount >= m_nCPUNumber)
            break;
    }

    for(int i = 0;i < m_nCPUNumber;i++)
    {
        for(nLoop = 0;nLoop < CPU_STATES;nLoop++)
        {
            lTmp = stCurStat[i].cpu_sysinfo.cpu[nLoop];

            stCurStat[i].cpu_sysinfo.cpu[nLoop] 
                -= m_OldCPUStat[i].cpu_sysinfo.cpu[nLoop];

            m_OldCPUStat[i].cpu_sysinfo.cpu[nLoop] = lTmp;
        }
    }

    //计算第1个CPU的占用率.
    dTotal = 0;
    for(nLoop = 0; nLoop < CPU_STATES; nLoop++)
    {
        dTotal += stCurStat[0].cpu_sysinfo.cpu[nLoop];
    }

    if (dTotal)
    {
        dRatio = 100.0 / dTotal;
    }
    else
    {
        dRatio = 0;
    }

    //常量CPU_IDLE(0)、CPU_USER(1)、CPU_KERNEL(2)、
    //CPU_WAIT(3)、CPU_STATES(4)定义在"sys/sysinfo.h"中

    nIdle1   = (int)((stCurStat[0].cpu_sysinfo.cpu[CPU_IDLE]   * dRatio) + 0.5);
    nUser1   = (int)((stCurStat[0].cpu_sysinfo.cpu[CPU_USER]   * dRatio) + 0.5);
    nKernel1 = (int)((stCurStat[0].cpu_sysinfo.cpu[CPU_KERNEL] * dRatio) + 0.5);
    nWait1   = (int)((stCurStat[0].cpu_sysinfo.cpu[CPU_WAIT]   * dRatio) + 0.5);

    //by ldf 2004-04-27 把CPU测量方法放宽松，对应网上问题200312230035
    nKernel1 = 100 - nIdle1 - nUser1 - nWait1;
    //end

    if(m_nCPUNumber == 1)
    {
        nIdle2 = nUser2 = nKernel2 = nWait2 = 0;
        kstat_close(kc);
        return 0;
    }

    //计算第2个CPU的占用率.
    dTotal = 0;
    for (nLoop = 0; nLoop < CPU_STATES; nLoop++)
    {
        dTotal += stCurStat[1].cpu_sysinfo.cpu[nLoop];
    }

    if (dTotal)
    {
        dRatio = 100.0 / dTotal;
    }
    else
    {
        dRatio = 0;
    }

    nIdle2   = (int)((stCurStat[1].cpu_sysinfo.cpu[CPU_IDLE]   * dRatio) + 0.5);
    nUser2   = (int)((stCurStat[1].cpu_sysinfo.cpu[CPU_USER]   * dRatio) + 0.5);
    nKernel2 = (int)((stCurStat[1].cpu_sysinfo.cpu[CPU_KERNEL] * dRatio) + 0.5);
    nWait2   = (int)((stCurStat[1].cpu_sysinfo.cpu[CPU_WAIT]   * dRatio) + 0.5);

    //by ldf 2004-04-27 把CPU测量方法放宽松，对应网上问题200312230035
    nKernel2 = 100 - nIdle2 - nUser2 - nWait2;
    //end

    kstat_close(kc);
    return 0;
#endif
}

int CUNIXPerfDB::GetDiskStatistic(PLogicalDisk* pStat)
{
#define TEMP_FILE_NAME "./DiskSpace.out"

    char szCmdLine[255] = "";
    sprintf(szCmdLine, "df -k | grep / > %s",TEMP_FILE_NAME);

#ifdef _PLATFORM_SOLARIS 
    if(system1(szCmdLine) < 0)
#else
    if(system(szCmdLine) < 0)
#endif
    {
        return -1;
    }

    //打开输出文件
    FILE* fp = NULL;
    if ((fp = fopen(TEMP_FILE_NAME, "rb")) == NULL)
    {
        return -1;
    }

    //如果文件长度为0，则表示出错
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
    {
        remove(TEMP_FILE_NAME);
        fclose(fp);
        return -1;
    }

    //再将指针指向文件头
    fseek(fp, 0, SEEK_SET);

    long nTotal = 0,nUsed = 0,nFree = 0,nTemp = 0;
    char szTemp[256] = "";
    char szLinkDir[255] = "";

    SLogicalDisk* p = NULL;
    LIST<SLogicalDisk*> lstTemp;

#ifdef _PLATFORM_HPUX
    char szBuf[1024];
    while(NULL != fgets(szBuf, sizeof(szBuf), fp))
    {
        if(sscanf(szBuf, "%s",szLinkDir) == 1)
        {
            p = new SLogicalDisk;

            strncpy(p->szName,szLinkDir,MAX_PATH);
            p->szName[MAX_PATH - 1] = 0;

            GetDiskStatistic(p->szName, (long*)&p->uTot, (long*)&p->uAva);

            lstTemp.push_back(p);
        }
    }
#else
    while(fscanf(fp, "%s %d %d %d %s %s",szTemp,&nTotal,
            &nUsed,&nFree,szTemp,szLinkDir) == 6)
    {
        p = new SLogicalDisk;

        strncpy(p->szName,szLinkDir,MAX_PATH);
        p->szName[MAX_PATH - 1] = 0;

        p->uAva = nFree;
        p->uTot = nTotal;

        lstTemp.push_back(p);
    }
#endif
    fclose(fp);

    remove(TEMP_FILE_NAME);

    int nCount = lstTemp.size(),k = 0;
    if(nCount > 0)
    {
        p = new SLogicalDisk[nCount];

        LIST<SLogicalDisk*>::iterator i = lstTemp.begin(),
                                      e = lstTemp.end();
        while(i != e)
        {
            memcpy(&(p[k++]),(*i),sizeof(SLogicalDisk));
            delete (*i);
            i++;
        }

        *pStat = p;
    }

    return nCount;
}

int CUNIXPerfDB::GetDiskStatistic(char* pMountName,long* pTotal,long* pAvail)
{
    char szCmdLine[255] = "";

#ifdef _PLATFORM_SOLARIS 
    sprintf(szCmdLine, "df -k | grep / > %s",TEMP_FILE_NAME);
    if(system1(szCmdLine) < 0)
#elif defined(_PLATFORM_HPUX)
    sprintf(szCmdLine, "df -k %s > %s", pMountName, TEMP_FILE_NAME);
    if(system(szCmdLine) < 0)
#endif
    {
        return -1;
    }

    //打开输出文件
    FILE* fp = NULL;
    if ((fp = fopen(TEMP_FILE_NAME, "rb")) == NULL)
    {
        return -1;
    }

    //如果文件长度为0，则表示出错
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
    {
        fclose(fp);
        remove(TEMP_FILE_NAME);
        return -1;
    }

    //再将指针指向文件头
    fseek(fp, 0, SEEK_SET);
#ifdef _PLATFORM_HPUX
    long nTotal = 0,nFree = 0;
    char szTemp1[256] = "";
    char szTemp2[256] = "";
    char szTemp3[256] = "";
    char szTemp4[256] = "";

    char szBuf[1024];
    if(NULL == fgets(szBuf, sizeof(szBuf), fp))
    {
         fclose(fp);
         remove(TEMP_FILE_NAME);
         return -1;
    }

    if(sscanf(szBuf, "%s %s %s %s %d",szTemp1,szTemp2,
            szTemp3,szTemp4,&nTotal) != 5)
    {
         fclose(fp);
         remove(TEMP_FILE_NAME);
         return -1;
    }

    if(NULL == fgets(szBuf, sizeof(szBuf), fp))
    {
         fclose(fp);
         remove(TEMP_FILE_NAME);
         return -1;
    }
    if(sscanf(szBuf, "%d", &nFree) != 1)
    {
         fclose(fp);
         remove(TEMP_FILE_NAME);
         return -1;
    }

    *pTotal = nTotal;
    *pAvail = nFree;

    fclose(fp);
    remove(TEMP_FILE_NAME);
    return 0;
#elif defined (_PLATFORM_SOLARIS)
    long nTotal = 0,nUsed = 0,nFree = 0,nTemp = 0;
    char szTemp[256] = "";
    char szLinkDir[MAX_PATH] = "";

    STRING strSub,strMountName = pMountName;
    int nSubLen = 0,nMountLen = 0;

    while(fscanf(fp, "%s %d %d %d %s %s",szTemp,&nTotal,
            &nUsed,&nFree,szTemp,szLinkDir) == 6)
    {
        nMountLen = strlen(szLinkDir);
        strSub = strMountName.substr(0,nMountLen);
        if(strSub.compare(szLinkDir) == 0)
        {
            if(nSubLen < nMountLen)
            {
                nSubLen = nMountLen;
                *pTotal = nTotal;
                *pAvail = nFree;
            }
        }
    }

    fclose(fp);
    return 0;
#endif
}

int CUNIXPerfDB::GetMemStatistic(long* pTotal,long* pAvail,
                                 long* pVirTotal,long* pVirAvail)
{
	//张玉新 2002-04-27 问题单 D12712
    long long nPhyMem = 0,nAvailMem = 0,nPageSize = 0;
    //修改结束
#ifdef _PLATFORM_HPUX
    struct pst_static  pst1;
    struct pst_dynamic pst2;
    if(pstat_getstatic(&pst1, sizeof(pst1), 1, 0) != -1)
    {
        if(pstat_getdynamic(&pst2, sizeof(pst2), 1, 0) != -1)
        {
            nPhyMem = pst1.physical_memory;
            nAvailMem = nPhyMem - pst2.psd_rm;
            nPageSize = sysconf(_SC_PAGE_SIZE);
        }
    }
#elif defined (_PLATFORM_SOLARIS)
    nPhyMem = sysconf(_SC_PHYS_PAGES);
    nAvailMem = sysconf(_SC_AVPHYS_PAGES);
    nPageSize = sysconf(_SC_PAGE_SIZE);
#endif

    *pTotal = (long)((nPhyMem * nPageSize) / (1024 * 1024));
    *pAvail = (long)((nAvailMem * nPageSize) / (1024 * 1024));

    *pVirTotal = 0;
    *pVirAvail = 0;

    return 0;
}


int CUNIXPerfDB::GetCPUNumber()
{
#ifdef _PLATFORM_HPUX
    int ncpus = -1;
    struct pst_dynamic psd;
    if(pstat_getdynamic(&psd, sizeof(psd), 1, 0) != -1)
    {
        ncpus = psd.psd_proc_cnt;
    }
    return ncpus;
#elif defined (_PLATFORM_SOLARIS)
    int ncpus = -1;

    kstat_ctl_t*   kc = NULL;
    kstat_t*       ks = NULL;
    kstat_named_t* kn = NULL;

    kc = kstat_open();

    ks = kstat_lookup(kc,"unix",0,"system_misc");
    if(kstat_read(kc,ks,NULL) == -1) {
        kstat_close(kc);
        return -1;
    }

    kn = (kstat_named_t*)kstat_data_lookup(ks, "ncpus");
    if (kn !=NULL) {
        ncpus = kn->value.ui32;
    }

    kstat_close(kc);

    return ncpus;
#endif
}

