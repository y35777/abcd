///////////////////////////////////////////////////////////////////////////////////////
// RAIDHLP.h -- 冗余磁盘阵列帮助库
// chenghui, 2000.8
// 该库提供以下帮助函数集:
//     * PercRAID磁盘阵列控制器(SC/DC), 包括:
//          * 标准的Open/Close函数;
//			* 磁盘状态查询函数;
// 注意: 
//    每个函数集由一个Open函数，一个Close函数和若干个Get函数组成。Open函数必须首先被
//    调用以返回一个句柄，所有Get函数和Close函数的第一个参数都是该句柄。

#ifndef _RAIDHLP_H
#define _RAIDHLP_H

#ifdef  __cplusplus
extern "C" {
#endif

////////////////////////////////// PercRAID辅助函数 //////////////////////////////////
HANDLE __stdcall PercRAIDOpen(LPVOID pOpenParam);	// 打开一个PercRAID句柄
void __stdcall PercRAIDClose(HANDLE hSession);		// 关闭一个PercRAID句柄

#define PERCRAID_PDS_ERROR		-1	//状态查询错误

// 以下函数由PercRAID磁盘阵列控制卡的制造商American Megatrends Inc定义，以代表物理
// 磁盘的状态
#define PERCRAID_PDS_READY		1	//磁盘正常，但未初始化
#define PRECRAID_PDS_ONLINE		3	//磁盘正常，已经初始化
#define PRECRAID_PDS_FAILED		4	//磁盘失效
#define PERCRAID_PDS_REBUILD	5	//磁盘正在重建
#define PRECRAID_PDS_HOTSPARE	6	//表示热备份磁盘
#define PRECRAID_PDS_NONDISK	20	//非磁盘

// PercRAIDPhysicalDriveQueryState -- 获取物理磁盘状态
// 参数:
//     hSession -- 由PercRAIDOpen返回的Session句柄
//     nChannel -- 通道号。对于单通道卡应为零。多余多通道卡应为零或一。
//     nDrive   -- 磁盘号。零代表第一个磁盘。
// 返回值:
//     若函数发生错误，则返回PERCRAID_PDS_ERROR。否则返回上述状态值。
int __stdcall PercRAIDGetPhysicalDriveState(HANDLE hSession, int nChannel, int nDrive);

#ifdef  __cplusplus
}
#endif

#endif