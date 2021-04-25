/*****************************************************
//		RaidHDCheck.cpp
//*****************************************************
//Copyright 2001 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       Wangfeng   wangzhaoyang
// Date:         01-6-18
// Description:  
// 
// Side Effects: NONE
// Functions:
// Notes:    
// 
// Update: 01-6-18 下午 02:35:46
// Date              Name              Description 
============== ================ ======================
## 01-06-18       Wangfeng              create
## 02-03-02       wangzhaoyang          update  
*****************************************************/
// RaidHDCheck.cpp: implementation of the CRaidHDCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "../include/frame.h"
#include "RaidHDCheck.h"
#include "RAIDHLP.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaidHDCheck::CRaidHDCheck(int nController)
{
	m_nController = nController;
}

CRaidHDCheck::~CRaidHDCheck()
{

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBMRaidHDCheck::CIBMRaidHDCheck(int nController)
{
	//将所有硬盘状态值赋空
	memset(m_HDStatus, 0, sizeof(m_HDStatus));

	//Added by wf 2001.8.9
	m_nController = nController;
	//Added by wf 2001.8.9

}

CIBMRaidHDCheck::~CIBMRaidHDCheck()
{

}


/******************************************************
// 函数名: 	CIBMRaidHDCheck::SearchHDStatusInString
// 作者:    Wangfeng
// 时期:    01-6-18
// 描述:    
// 输入:
//       参数1 :char * pInBuf	待查询的字符串缓冲区
// 返回值: HDSTATUS				返回硬盘状态 
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-6-18       Wangfeng		在输入的字符串缓冲区内
								查找关键字符串，并返回
								对应的硬盘状态值。
******************************************************/
HDSTATUS CIBMRaidHDCheck::SearchHDStatusInString(char * pInBuf)
{

	if(strstr(pInBuf, "State                    : Empty (EMP)"))
	{
		return HD_EMP;
	}
	else if(strstr(pInBuf, "State                    : Ready (RDY)"))
	{
		return HD_RDY;
	}
	else if(strstr(pInBuf, "State                    : Hot Spare (HSP)"))
	{
		return HD_HSP;
	}
	else if(strstr(pInBuf, "State                    : Standby Hot Spare (SHS)"))
	{
		return HD_SHS;
	}
	else if(strstr(pInBuf, "State                    : Defunct Disk Drive (DDD)"))
	{
		return HD_DDD;
	}
	else if(strstr(pInBuf, "State                    : Defunct Hot Spare (DHS)"))
	{
		return HD_DHS;
	}
	else if(strstr(pInBuf, "State                    : Rebuild (RBL)"))
	{
		return HD_RBL;
	}
	else if(strstr(pInBuf, "State                    : Standby (SBY)"))
	{
		return HD_SBY;
	}
	else if(strstr(pInBuf, "State                    : Online (ONL)"))
	{
		return HD_ONL;
	}
	else 
	{
		return HD_NULL;
	}

}


/******************************************************
// 函数名: 	CIBMRaidHDCheck::GetIBMRaidOutput
// 作者:    Wangfeng
// 时期:    01-7-23
// 描述:    取指定控制器号的硬盘状态输出
// 输入:
// 返回值: BOOL					输出的文本缓冲区
//								（在函数内分配，在函数外删除），
//								如果失败，则返回NULL
// 注释:	返回m_nController控制器查询状态的输出文本
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-7-23       Wangfeng			新增
******************************************************/
char * CIBMRaidHDCheck::GetIBMRaidOutput()
{
	char strCmdLine[MAX_PATH];
	char strWorkDirectory[MAX_PATH];
	char* pstrContent;

	unsigned long lFileLength = 0;
	unsigned long lReadFileLength = 0;

	STARTUPINFO         StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	HANDLE hPipeRead;
	HANDLE hPipeWrite;
	SECURITY_ATTRIBUTES sa;

	//构建命令行字符串。
	GetSystemDirectory(strWorkDirectory, MAX_PATH);
	sprintf(strCmdLine, "%s\\ipssend.exe GETCONFIG %d PD", 
			strWorkDirectory, 
			FIRST_SERVERAID);

	//设置匿名管道的安全属性
	memset(&sa,0,sizeof(sa));
	sa.nLength=sizeof(sa);
	sa.bInheritHandle = TRUE;

	//创建一个匿名管道 
	CreatePipe(&hPipeRead,&hPipeWrite,&sa,0);      

	memset(&StartupInfo,0,sizeof(StartupInfo)); 
	StartupInfo.dwFlags = STARTF_USESTDHANDLES;
	//缺省桌面
	StartupInfo.lpDesktop = "WinSta0\\Default";     
	//设置程序的写句柄为管道的写句柄
	StartupInfo.hStdOutput = hPipeWrite;   
	//设置程序的写句柄为标准读句柄
	StartupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE); 
	//设置程序的错误处理句柄为标准错误处理句柄
	StartupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE); 
	StartupInfo.cb = sizeof(StartupInfo);

	int nRet = CreateProcess(NULL			
			,(LPSTR)strCmdLine
			,NULL				
			,NULL						
			,TRUE						
			,CREATE_NO_WINDOW | NORMAL_PRIORITY_CLASS		
			,NULL					
			,NULL						
			,&StartupInfo			
			,&ProcessInformation	
			);
	CloseHandle(hPipeWrite);

	if (!nRet)
	{//如果执行命令失败
		CloseHandle(hPipeRead);
		return NULL;
	}
	else
	{//如果执行命令成功
		CloseHandle(ProcessInformation.hThread);
		#define FIVE_SECOND 5000
		if(WaitForSingleObject(ProcessInformation.hProcess, FIVE_SECOND) == WAIT_TIMEOUT)
		{
			TerminateProcess(ProcessInformation.hProcess, 1);
		}
		CloseHandle(ProcessInformation.hProcess);
	}

	//取文件长度
	lFileLength = GetFileSize(hPipeRead, NULL);
	if(!lFileLength)
	{
		CloseHandle(hPipeRead);
		return NULL;
	}

	//将文件内容全部读到指定缓冲区,并删除临时文件
	pstrContent = new char[lFileLength + 1];
	ReadFile(hPipeRead, pstrContent, lFileLength, (LPDWORD) &lReadFileLength, NULL);
	CloseHandle(hPipeRead);

	//验证读取是否成功，如果成功，则给字符串加尾零
	if(lFileLength != lReadFileLength)
	{
		delete pstrContent;
		return NULL;
	}
	*(pstrContent + lReadFileLength) = 0;

	return pstrContent;
}


/******************************************************
// 函数名: 	CIBMRaidHDCheck::CheckIBMRaidHD
// 作者:    Wangfeng
// 时期:    01-6-18
// 描述:    
// 输入:
// 返回值: int					如果无信息，则返回0，否则返回1
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-6-18       Wangfeng		查询所有硬盘的状态
								，将这些状态返回到成
								员变量m_HDStatus。
******************************************************/
int CIBMRaidHDCheck::CheckRaidHD()
{
	BOOL nReturn = 0;
	char *pstrContent;
	char strtmp[50];

	//取得查询指定控制器下所有硬盘状态的输出文本
	pstrContent = GetIBMRaidOutput();

	if(pstrContent)
	{//如果查询操作有文本输出
		for(int i = 0; i<= 15; i++)
		{
			char *pTemp1 = NULL;	//指向第i个硬盘状态文本的起点指针
			char *pTemp2 = NULL;	//指向下一个硬盘状态文本的起点指针
			char *pIDContent = NULL;//分离拷贝出第i个硬盘状态文本

			//检测指定RAID控制器第i个硬盘的状态
			sprintf(strtmp, "      Target on SCSI ID %d\r\n", i);
			pTemp1 = strstr(pstrContent, strtmp);

			if (pTemp1) 
			{//如果找到第i个硬盘状态的描述

				//查找是否有下一个硬盘状态的描述
				pTemp2 = strstr(pTemp1 + 1, "      Target on SCSI ID ");

				if (pTemp2)
				{//如果找到下一个硬盘状态的描述
					int nSize = pTemp2 - pTemp1;
					pIDContent = new char[nSize + 1];
					strncpy(pIDContent, pTemp1, nSize);
					pIDContent[nSize] = 0;

					//返回该硬盘的状态
					m_HDStatus[i] = SearchHDStatusInString(pIDContent);
					delete pIDContent;
				}
				else
				{//如果找不到下一个硬盘状态的描述

					//返回该硬盘的状态
					m_HDStatus[i] = SearchHDStatusInString(pTemp1);
				}
			}
			else
			{//如果找不到第i个硬盘状态的描述
				m_HDStatus[i] = HD_NULL;
			}

			//如果所有SCSI ID的硬盘状态都检测不到，就返回0
			if (m_HDStatus[i] != HD_NULL) nReturn = 1;
		}
		delete pstrContent;
	}//如果查询操作有文本输出

	return nReturn;
}



/******************************************************
// 函数名: 	CIBMRaidHDCheck::GetStatusDesp
// 作者:    Wangfeng
// 时期:    01-6-18
// 描述:    
// 输入:
//       参数1 :HDSTATUS nStatus	输入的硬盘状态值
// 返回值: LPCSTR					输出的硬盘状态描
//									述的常量字符串指针
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-6-18       Wangfeng		硬盘状态值转描述
******************************************************/
LPCSTR CIBMRaidHDCheck::GetStatusDesp(HDSTATUS nStatus)
{
	LPCSTR lpcsDesp;

	switch(nStatus)
	{
	case HD_EMP: 
		lpcsDesp = "Empty (EMP)";
		break;
	case HD_RDY:
		lpcsDesp = "Ready (RDY)";
		break;
	case HD_HSP:
		lpcsDesp = "Hot Spare (HSP)";
		break;
	case HD_SHS:
		lpcsDesp = "Standby Hot Spare (SHS)";
		break;
	case HD_DDD:
		lpcsDesp = "Defunct Disk Drive (DDD)";
		break;
	case HD_DHS:
		lpcsDesp = "Defunct Hot Spare (DHS)";
		break;
	case HD_RBL:
		lpcsDesp = "Rebuild (RBL)";
		break;
	case HD_SBY:
		lpcsDesp = "Standby (SBY)";
		break;
	case HD_ONL:
		lpcsDesp = "Online (ONL)";
		break;
	default:
		lpcsDesp = NULL;
	}
	return lpcsDesp;
}


/******************************************************
// 函数名: 	CIBMRaidHDCheck::GetFailedHDCount
// 作者:    Wangfeng
// 时期:    01-6-18
// 描述:    
// 输入:
// 返回值: int			返回硬盘故障的数目
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-6-18       Wangfeng		返回硬盘故障的数目
******************************************************/
int CIBMRaidHDCheck::GetFailedHDCount()
{
	int nFailedHDCount = 0;

	for (int i = 0; i <= 15; i++)
	{
		if((m_HDStatus[i] == HD_DDD) || (m_HDStatus[i] == HD_DHS)) 
			nFailedHDCount++;
	}
	return nFailedHDCount;
}


/******************************************************
// 函数名: 	CIBMRaidHDCheck::GetFailedHDID
// 作者:    Wangfeng
// 时期:    01-6-18
// 描述:    
// 输入:
// 返回值: int					返回故障硬盘的掩码
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-6-18       Wangfeng		返回故障硬盘的掩码
******************************************************/
int CIBMRaidHDCheck::GetFailedHDID()
{
	int nFailedHDID = 0;
	//如第0和第9个硬盘坏了，nFailedHDID = 二进制(0000 0010 0000 0001) = 0x0201

	//返回故障硬盘的掩码
	for (int i = 0; i <= 15; i++)
	{
		if((m_HDStatus[i] == HD_DDD) || (m_HDStatus[i] == HD_DHS)) 
		{
			nFailedHDID = nFailedHDID | (0x01 << i);
		}
	}
	return nFailedHDID;
}


/******************************************************
// 函数名: 	CIBMRaidHDCheck::IsIBMRaidController
// 作者:    Wangfeng
// 时期:    01-7-23
// 描述:    检查是否存在IBM的RAID卡
// 输入:
// 返回值: BOOL		如果有该RAID卡，则返回TRUE
// 注释: 
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-7-23       Wangfeng
******************************************************/
BOOL CIBMRaidHDCheck::IsIBMRaidController()
{
	char* pstrContent;
	BOOL bReturn = FALSE;

	pstrContent = GetIBMRaidOutput();
	if(pstrContent)
	{
		//检索阵列卡的关键字
		if(!strstr(pstrContent, "Controller number is invalid")
			&& !strstr(pstrContent, "Found 0 IBM ServeRAID Controller(s).")
			&& strstr(pstrContent, "IBM ServeRAID Controller(s)."))
		{
			bReturn = TRUE;
		}
		delete pstrContent;
	}
	return bReturn;

}


/******************************************************
// 函数名: 	CIBMRaidHDCheck::GetIBMHDStatus
// 作者:    Wangfeng
// 时期:    01-8-9
// 描述:    
// 输入:
//       参数1 :int nIndex			待查询的硬盘号
// 返回值: HDSTATUS 
// 注释:	返回指定硬盘号的状态
// 更新: 
// 日期            姓名                  描述     
   ========== ================== ======================
//01-8-9        Wangfeng			新增加
******************************************************/
HDSTATUS CIBMRaidHDCheck::GetIBMHDStatus(int nHDIndex)
{
	if (nHDIndex >= 0 && nHDIndex <= 15)
	{
		return m_HDStatus[nHDIndex];
	}
	else
	{
		return HD_NULL;
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDellRaidHDCheck::CDellRaidHDCheck(int nController)
{
	memset(m_nCurrentHDStatus, 0, sizeof(m_nCurrentHDStatus));
	memset(m_nHDStatus, 0, sizeof(m_nHDStatus));
	m_nController = nController;
	
	hPerc = PercRAIDOpen("127.0.0.1");
	if(!hPerc)
	{
		TRACE(MTS_CLSTR, S_OPEN_DELL_RAID_FAIL);
	}
}

CDellRaidHDCheck::~CDellRaidHDCheck()
{
	PercRAIDClose("127.0.0.1");
}

/******************************************************
// 函数名: 	CheckRaidHD
// 作者:    wangzhaoyang
// 时期:    02-2-28
// 描述:    检测Dell磁盘阵列
// 输入:
// 返回值: int	如果检测成功返回1，否则返回0
// 注释: 
// 更新:    
// 日期            姓名                  描述     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          创建
******************************************************/
int CDellRaidHDCheck::CheckRaidHD()
{
	if(hPerc)
	{
		int k = 0, nDiskNo = 0;
		static int nStartFlg = 1;  //启动时读入磁盘阵列的状态
		for (int i=0; i<2; i++)
		{
			for(int j=0; j<16; j++)
			{
				int nRet = PercRAIDGetPhysicalDriveState(hPerc, i, j);
				m_nCurrentHDStatus[k++] = nRet;
				if(nRet == PERCRAID_PDS_ERROR)
				{
					return 0;
				}
			}
		}
		if(nStartFlg)
		{
			nStartFlg = 0;
			memcpy((BYTE*)&m_nHDStatus, (BYTE*)&m_nCurrentHDStatus, sizeof(m_nHDStatus));
		}
	}
	else
	{
		return 0;
	}

	return 1;
}


/******************************************************
// 函数名: 	GetFailedHDCount
// 作者:    wangzhaoyang
// 时期:    02-2-28
// 描述:    获得失效的dell磁盘个数
// 输入:
// 返回值: int	返回失效磁盘个数
// 注释: 
// 更新:    
// 日期            姓名                  描述     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          创建
******************************************************/
int CDellRaidHDCheck::GetFailedHDCount()
{
	int nFailedHDCount = 0;

	//Dell的检测方法和IBM有区别，当某个磁盘被拔出时dell
	//磁盘检测方法返回值为0（没有磁盘），而不是PRECRAID_PDS_FAILED
	for (int i = 0; i < 32; i++)
	{
		if(((m_nCurrentHDStatus[i] != m_nHDStatus[i]) \
			&&(m_nCurrentHDStatus[i] == 0)) \
			||(m_nCurrentHDStatus[i] == PRECRAID_PDS_FAILED))
			nFailedHDCount++;
	}
	
	return nFailedHDCount;
}

/******************************************************
// 函数名: 	GetFailedHDCount
// 作者:    wangzhaoyang
// 时期:    02-2-28
// 描述:    获得失效dell磁盘的掩码
// 输入:
// 返回值: int	返回失效dell磁盘的掩码
// 注释: 
// 更新:    
// 日期            姓名                  描述     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          创建
******************************************************/
int CDellRaidHDCheck::GetFailedHDID()
{
	int nFailedHDID = 0;
	//如第一个通道的第0和第9个硬盘坏了，nFailedHDID = 
	//二进制(0000 0000 0000 0000 0000 0010 0000 0001) = 0x0201

	//返回故障硬盘的掩码
	for (int i = 0; i < 32; i++)
	{
		if(((m_nCurrentHDStatus[i] != m_nHDStatus[i]) \
			&&(m_nCurrentHDStatus[i] == 0)) \
			||(m_nCurrentHDStatus[i] == PRECRAID_PDS_FAILED))
		{
			nFailedHDID = nFailedHDID | (0x01 << i);
		}
	}
	return nFailedHDID;	
}

/******************************************************
// 函数名: 	IsDellRaidController
// 作者:    wangzhaoyang
// 时期:    02-2-28
// 描述:    检测是否为dell磁盘控制器
// 输入:
// 返回值:  BOOL 如果是dell磁盘控制器，返回TRUE，否则为FALSE
// 注释: 
// 更新:    
// 日期            姓名                  描述     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          创建
******************************************************/
BOOL CDellRaidHDCheck::IsDellRaidController()
{
	HANDLE hPerc;
	int nDiskNumber = 0;
	if(hPerc = PercRAIDOpen("127.0.0.1"))
	{
		for (int i=0; i<2; i++)
		{
			for(int j=0; j<16; j++)
			{
				int nRet = PercRAIDGetPhysicalDriveState(hPerc, i, j);
				//当没有安装SNMP服务时，会检测失败
				if(nRet == PERCRAID_PDS_ERROR)
				{
					PercRAIDClose(hPerc);
					return FALSE;
				}
				//至少应该有一个硬盘，否则检测失去意义
				else if(nRet != 0)
				{
					nDiskNumber++;
				}
			}
		}
		PercRAIDClose(hPerc);
	}
	
	return (nDiskNumber > 0);
}
