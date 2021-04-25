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
// Update: 01-6-18 ���� 02:35:46
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
	//������Ӳ��״ֵ̬����
	memset(m_HDStatus, 0, sizeof(m_HDStatus));

	//Added by wf 2001.8.9
	m_nController = nController;
	//Added by wf 2001.8.9

}

CIBMRaidHDCheck::~CIBMRaidHDCheck()
{

}


/******************************************************
// ������: 	CIBMRaidHDCheck::SearchHDStatusInString
// ����:    Wangfeng
// ʱ��:    01-6-18
// ����:    
// ����:
//       ����1 :char * pInBuf	����ѯ���ַ���������
// ����ֵ: HDSTATUS				����Ӳ��״̬ 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-6-18       Wangfeng		��������ַ�����������
								���ҹؼ��ַ�����������
								��Ӧ��Ӳ��״ֵ̬��
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
// ������: 	CIBMRaidHDCheck::GetIBMRaidOutput
// ����:    Wangfeng
// ʱ��:    01-7-23
// ����:    ȡָ���������ŵ�Ӳ��״̬���
// ����:
// ����ֵ: BOOL					������ı�������
//								���ں����ڷ��䣬�ں�����ɾ������
//								���ʧ�ܣ��򷵻�NULL
// ע��:	����m_nController��������ѯ״̬������ı�
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-7-23       Wangfeng			����
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

	//�����������ַ�����
	GetSystemDirectory(strWorkDirectory, MAX_PATH);
	sprintf(strCmdLine, "%s\\ipssend.exe GETCONFIG %d PD", 
			strWorkDirectory, 
			FIRST_SERVERAID);

	//���������ܵ��İ�ȫ����
	memset(&sa,0,sizeof(sa));
	sa.nLength=sizeof(sa);
	sa.bInheritHandle = TRUE;

	//����һ�������ܵ� 
	CreatePipe(&hPipeRead,&hPipeWrite,&sa,0);      

	memset(&StartupInfo,0,sizeof(StartupInfo)); 
	StartupInfo.dwFlags = STARTF_USESTDHANDLES;
	//ȱʡ����
	StartupInfo.lpDesktop = "WinSta0\\Default";     
	//���ó����д���Ϊ�ܵ���д���
	StartupInfo.hStdOutput = hPipeWrite;   
	//���ó����д���Ϊ��׼�����
	StartupInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE); 
	//���ó���Ĵ�������Ϊ��׼��������
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
	{//���ִ������ʧ��
		CloseHandle(hPipeRead);
		return NULL;
	}
	else
	{//���ִ������ɹ�
		CloseHandle(ProcessInformation.hThread);
		#define FIVE_SECOND 5000
		if(WaitForSingleObject(ProcessInformation.hProcess, FIVE_SECOND) == WAIT_TIMEOUT)
		{
			TerminateProcess(ProcessInformation.hProcess, 1);
		}
		CloseHandle(ProcessInformation.hProcess);
	}

	//ȡ�ļ�����
	lFileLength = GetFileSize(hPipeRead, NULL);
	if(!lFileLength)
	{
		CloseHandle(hPipeRead);
		return NULL;
	}

	//���ļ�����ȫ������ָ��������,��ɾ����ʱ�ļ�
	pstrContent = new char[lFileLength + 1];
	ReadFile(hPipeRead, pstrContent, lFileLength, (LPDWORD) &lReadFileLength, NULL);
	CloseHandle(hPipeRead);

	//��֤��ȡ�Ƿ�ɹ�������ɹ�������ַ�����β��
	if(lFileLength != lReadFileLength)
	{
		delete pstrContent;
		return NULL;
	}
	*(pstrContent + lReadFileLength) = 0;

	return pstrContent;
}


/******************************************************
// ������: 	CIBMRaidHDCheck::CheckIBMRaidHD
// ����:    Wangfeng
// ʱ��:    01-6-18
// ����:    
// ����:
// ����ֵ: int					�������Ϣ���򷵻�0�����򷵻�1
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-6-18       Wangfeng		��ѯ����Ӳ�̵�״̬
								������Щ״̬���ص���
								Ա����m_HDStatus��
******************************************************/
int CIBMRaidHDCheck::CheckRaidHD()
{
	BOOL nReturn = 0;
	char *pstrContent;
	char strtmp[50];

	//ȡ�ò�ѯָ��������������Ӳ��״̬������ı�
	pstrContent = GetIBMRaidOutput();

	if(pstrContent)
	{//�����ѯ�������ı����
		for(int i = 0; i<= 15; i++)
		{
			char *pTemp1 = NULL;	//ָ���i��Ӳ��״̬�ı������ָ��
			char *pTemp2 = NULL;	//ָ����һ��Ӳ��״̬�ı������ָ��
			char *pIDContent = NULL;//���뿽������i��Ӳ��״̬�ı�

			//���ָ��RAID��������i��Ӳ�̵�״̬
			sprintf(strtmp, "      Target on SCSI ID %d\r\n", i);
			pTemp1 = strstr(pstrContent, strtmp);

			if (pTemp1) 
			{//����ҵ���i��Ӳ��״̬������

				//�����Ƿ�����һ��Ӳ��״̬������
				pTemp2 = strstr(pTemp1 + 1, "      Target on SCSI ID ");

				if (pTemp2)
				{//����ҵ���һ��Ӳ��״̬������
					int nSize = pTemp2 - pTemp1;
					pIDContent = new char[nSize + 1];
					strncpy(pIDContent, pTemp1, nSize);
					pIDContent[nSize] = 0;

					//���ظ�Ӳ�̵�״̬
					m_HDStatus[i] = SearchHDStatusInString(pIDContent);
					delete pIDContent;
				}
				else
				{//����Ҳ�����һ��Ӳ��״̬������

					//���ظ�Ӳ�̵�״̬
					m_HDStatus[i] = SearchHDStatusInString(pTemp1);
				}
			}
			else
			{//����Ҳ�����i��Ӳ��״̬������
				m_HDStatus[i] = HD_NULL;
			}

			//�������SCSI ID��Ӳ��״̬����ⲻ�����ͷ���0
			if (m_HDStatus[i] != HD_NULL) nReturn = 1;
		}
		delete pstrContent;
	}//�����ѯ�������ı����

	return nReturn;
}



/******************************************************
// ������: 	CIBMRaidHDCheck::GetStatusDesp
// ����:    Wangfeng
// ʱ��:    01-6-18
// ����:    
// ����:
//       ����1 :HDSTATUS nStatus	�����Ӳ��״ֵ̬
// ����ֵ: LPCSTR					�����Ӳ��״̬��
//									���ĳ����ַ���ָ��
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-6-18       Wangfeng		Ӳ��״ֵ̬ת����
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
// ������: 	CIBMRaidHDCheck::GetFailedHDCount
// ����:    Wangfeng
// ʱ��:    01-6-18
// ����:    
// ����:
// ����ֵ: int			����Ӳ�̹��ϵ���Ŀ
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-6-18       Wangfeng		����Ӳ�̹��ϵ���Ŀ
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
// ������: 	CIBMRaidHDCheck::GetFailedHDID
// ����:    Wangfeng
// ʱ��:    01-6-18
// ����:    
// ����:
// ����ֵ: int					���ع���Ӳ�̵�����
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-6-18       Wangfeng		���ع���Ӳ�̵�����
******************************************************/
int CIBMRaidHDCheck::GetFailedHDID()
{
	int nFailedHDID = 0;
	//���0�͵�9��Ӳ�̻��ˣ�nFailedHDID = ������(0000 0010 0000 0001) = 0x0201

	//���ع���Ӳ�̵�����
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
// ������: 	CIBMRaidHDCheck::IsIBMRaidController
// ����:    Wangfeng
// ʱ��:    01-7-23
// ����:    ����Ƿ����IBM��RAID��
// ����:
// ����ֵ: BOOL		����и�RAID�����򷵻�TRUE
// ע��: 
// ����: 
// ����            ����                  ����     
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
		//�������п��Ĺؼ���
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
// ������: 	CIBMRaidHDCheck::GetIBMHDStatus
// ����:    Wangfeng
// ʱ��:    01-8-9
// ����:    
// ����:
//       ����1 :int nIndex			����ѯ��Ӳ�̺�
// ����ֵ: HDSTATUS 
// ע��:	����ָ��Ӳ�̺ŵ�״̬
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-8-9        Wangfeng			������
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
// ������: 	CheckRaidHD
// ����:    wangzhaoyang
// ʱ��:    02-2-28
// ����:    ���Dell��������
// ����:
// ����ֵ: int	������ɹ�����1�����򷵻�0
// ע��: 
// ����:    
// ����            ����                  ����     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          ����
******************************************************/
int CDellRaidHDCheck::CheckRaidHD()
{
	if(hPerc)
	{
		int k = 0, nDiskNo = 0;
		static int nStartFlg = 1;  //����ʱ����������е�״̬
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
// ������: 	GetFailedHDCount
// ����:    wangzhaoyang
// ʱ��:    02-2-28
// ����:    ���ʧЧ��dell���̸���
// ����:
// ����ֵ: int	����ʧЧ���̸���
// ע��: 
// ����:    
// ����            ����                  ����     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          ����
******************************************************/
int CDellRaidHDCheck::GetFailedHDCount()
{
	int nFailedHDCount = 0;

	//Dell�ļ�ⷽ����IBM�����𣬵�ĳ�����̱��γ�ʱdell
	//���̼�ⷽ������ֵΪ0��û�д��̣���������PRECRAID_PDS_FAILED
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
// ������: 	GetFailedHDCount
// ����:    wangzhaoyang
// ʱ��:    02-2-28
// ����:    ���ʧЧdell���̵�����
// ����:
// ����ֵ: int	����ʧЧdell���̵�����
// ע��: 
// ����:    
// ����            ����                  ����     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          ����
******************************************************/
int CDellRaidHDCheck::GetFailedHDID()
{
	int nFailedHDID = 0;
	//���һ��ͨ���ĵ�0�͵�9��Ӳ�̻��ˣ�nFailedHDID = 
	//������(0000 0000 0000 0000 0000 0010 0000 0001) = 0x0201

	//���ع���Ӳ�̵�����
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
// ������: 	IsDellRaidController
// ����:    wangzhaoyang
// ʱ��:    02-2-28
// ����:    ����Ƿ�Ϊdell���̿�����
// ����:
// ����ֵ:  BOOL �����dell���̿�����������TRUE������ΪFALSE
// ע��: 
// ����:    
// ����            ����                  ����     
   ========== ================== ======================
//02-2-28       Wangzhaoyang          ����
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
				//��û�а�װSNMP����ʱ������ʧ��
				if(nRet == PERCRAID_PDS_ERROR)
				{
					PercRAIDClose(hPerc);
					return FALSE;
				}
				//����Ӧ����һ��Ӳ�̣�������ʧȥ����
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
