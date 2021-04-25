/**************************************************************************
 *产品名    : iGateway Bill V200        
 *模块名    : 性能模块(Perf)
 *文件名    : os_perf_db.h
 *描述信息  : 本文件是类COSPerfDB的定义；
              该类仅提供了获取操作系统状态的虚函数接口
 *版本说明  : V200R001i00M00B00D1130A   
 *版权信息  : 版权所有(c) 2001-2002 华为技术有限公司
 *作者      : 方卫华
 *创建时间  : 2001-11-05
 *修改记录  : 
 **************************************************************************/

#ifndef __OS_PERF_DB__
#define __OS_PERF_DB__

#include "../include/base_type.h"

//by ldf 2002.6.7 对应问题单D14393
#include "../include/toolbox.h"
//修改完毕

// 逻辑分区信息
typedef struct _LogicalDisk{
    char szName[MAX_PATH];  // 逻辑分区名
    UINT4 uTot;         // 逻辑分区总空间
    UINT4 uAva;         // 逻辑分区可用空间
} SLogicalDisk, *PLogicalDisk;

// 定义如下数据结构来描述需要获得的操作系统性能数据
typedef struct _OSPerfStat{
    BYTE nCPUs;         // CPU个数
    BYTE nLogDisk;      // 逻辑分区个数
    BYTE* CPUPercent;   // 每个CPU的占用率
    UINT4 uPhyMemTot;   // 物理内存总空间
    UINT4 uPhyMemAva;   // 物理内存可用空间
    UINT4 uVirMemTot;   // 虚拟内存总空间
    UINT4 uVirMemAva;   // 虚拟内存可用空间
    PLogicalDisk Disks;  // 各个逻辑分区总空间与可用空间

	~_OSPerfStat()
	{
		delete CPUPercent;
		delete Disks;
	}

} SOSPerfStat, *POSPerfStat;

class COSPerfDB {
public:
    COSPerfDB()
	{
		m_szFrontSavePath[0] = 0;
		m_szBackSavePath[0] = 0;
	}

    COSPerfDB(const char *szFront,const char *szBack)
    {
        strncpy(m_szFrontSavePath,szFront,MAX_PATH);
		m_szFrontSavePath[MAX_PATH - 1] = 0;

        strncpy(m_szBackSavePath,szBack,MAX_PATH);
		m_szBackSavePath[MAX_PATH - 1] = 0;
    }

    virtual ~COSPerfDB(){};

    // 获得操作系统相关性能数据项的描述，输出的操作系统相关性能数据字符串格式
    // 为：
    // "性能数据项1名++性能数据项1值++......++性能数据项N名++性能数据项N值"，
    // 其中+号表示一个空格字符
    virtual void GetOSStatus(BYTE *pbPara,int nNum,char*& szAllPerf) = 0;

    // 获得所需性能数据，包括：
    //      每个CPU的占用率
    //      物理内存的可用空间
    //      虚拟内存的可用空间
    //      各逻辑分区的可用空间
    virtual void GetPerfStatistic(SOSPerfStat& OSPerfStat) = 0;

protected:
    char m_szFrontSavePath[MAX_PATH];
    char m_szBackSavePath[MAX_PATH];
};

#endif /* __OS_PERF_DB__ */


