/******************************************************************************
Copyright (C), 2003-2011, Huawei Tech. Co., Ltd.

  /****************************************************************************
  File Name       : cfg_svr.cpp
  Version         : 1.0
  Author          : dengwanqiu
  Created         : 2003-07-31
  Last Modified   : 
  Description     : implementation of the CCfgSvr class.
  Function List   : 
  History         : 
  1 Date          : 2003-07-31
  Author        : dengwanqiu
  Modification  : Created file
******************************************************************************/
#include "para_file.h"
#include "cfg_svr.h"
#include "file_mgr.h"
#include "resource.h"
#include "../include/system1.h"

#ifdef _PLATFORM_WIN32
#define CFG_SVR_MTIME   st_mtime
#else
#define CFG_SVR_MTIME   st_ctime
#endif

//��¼�������ò���ʧ��ԭ�򣬵�ʧ��ʱ��������Ϣ�������ͻ���
char g_szErrorReason[20*1024] = "";   

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*******************************************************************
�������ƣ�CCfgSvr()
�������������캯��,��ʼ����Ա����
���������
�����������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CCfgSvr::CCfgSvr() : m_pParaFile(NULL), m_pCfgFileMgr(NULL), 
                     m_pMMLReport(NULL), m_nCurClientNo(1)
{    
}

/*******************************************************************
�������ƣ�~CCfgSvr()
������������������,ɾ��ָ���Ա����
���������
�����������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
CCfgSvr::~CCfgSvr()
{
	if (m_pParaFile != NULL)
    {
        delete m_pParaFile;
        m_pParaFile = NULL;
    }
    if (m_pCfgFileMgr != NULL)
    {
        delete m_pCfgFileMgr;
        m_pCfgFileMgr = NULL;
    }
    if (m_pMMLReport != NULL)
    {
        delete m_pMMLReport;
        m_pMMLReport = NULL;
    }
    FILERELATION_MAP::iterator it;
    for(it=m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
    {
        delete (*it).second;
    }
	m_mapFileRelation.clear();
}

/*******************************************************************
�������ƣ�InitInstance()
������������ʼ������
���������
�����������
�������ݣ����ش�����(ERR_SUCCESS��ʾ�ɹ�,ERR_FAIL��ʾʧ��)
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
int CCfgSvr::InitInstance()
{
    //���ó�ʼ�����ຯ����
    int nResult = CWorkModule::InitInstance();
    if (ERR_SUCCESS != nResult)
    {
        TRACE(MTS_CFGSVR,S_CFG_MODULE_STARTUP_FAIL);
        return ERR_FAIL;
    }

    //�������ü���Ϣ�ļ����·��
    char szDirPath[MAX_PATH];
	int nLen = SNPRINTF(szDirPath, MAX_PATH - 1, "%s", GetCfgFilePath());
	szDirPath[nLen] = '\0';

    //ȥ�����ò����ļ����󣬼�Ϊ���ò����ļ�ȫ·��
	char *szDest = strrchr(szDirPath, '/');
#ifdef _PLATFORM_WIN32
    char *pPos2 = strrchr(szDirPath, '\\');
#else //_PLATFORM_WIN32
    char *pPos2 = NULL;
#endif //_PLATFORM_WIN32
    szDest = max(szDest, pPos2);
    if (szDest != NULL) 
    { 
        szDest[1] = '\0'; 
    }

    //������Ϣ�ļ�(allpara)�������ļ�λ����ͬ��Ŀ¼��; ȡallpara�ļ���ȫ·����
	char szParaFileFullPath[MAX_PATH];
	nLen = SNPRINTF(szParaFileFullPath, MAX_PATH - 1, "%sallpara", szDirPath);
	szParaFileFullPath[nLen] = '\0';

    //��m_pParaFile�Ѵ��ڣ����ȹر�
    if (m_pParaFile != NULL)
    {
        delete m_pParaFile;
    }
    //��ʼ�����в�������m_pParaFile
    m_pParaFile = new  CParaFile();
    if ( !m_pParaFile->Init(szParaFileFullPath) )
    {
        TRACE(MTS_CFGSVR,S_CFG_MODULE_STARTUP_FAIL);
        return ERR_FAIL;
    }
    //��ʼ�������ļ�������m_pCfgFileMgr��
    if (m_pCfgFileMgr != NULL)
    {
        delete m_pCfgFileMgr;
    }
    m_pCfgFileMgr = new  CCfgFileMgr();  
    
    //��������ini�ļ������������ļ��б���¼ÿ���ļ����޸�ʱ�䣻
    int nGrpNo = 0;
    ACE_Dirent FindDir((const ACE_TCHAR* )szDirPath);

    //����ָ����Ŀ¼���ҳ����е���iniΪ��׺�������ļ�
    dirent* pDirent;
    ACE_stat FileInfo;
    char szFileFullPath[MAX_PATH], szFileName[MAX_PATH];
    pDirent = FindDir.read();
    while (pDirent)
    {
        strcpy(szFileName, pDirent->d_name);
        sprintf(szFileFullPath, "%s%s", szDirPath, szFileName);
        ACE_OS::stat(szFileFullPath, &FileInfo);

        //����ҵ�����һ���ļ�
        if ((S_IFDIR != (FileInfo.st_mode & S_IFMT)))
        {			
			char* szDest = strstr(szFileName, ".ini");
            if (szDest != NULL)
            {
			    //�ж��Ƿ���iniΪ��׺			
			    if (strcmp(szDest, ".ini") == 0)
			    {   
				    SGroupAndFile* pTmp = new  SGroupAndFile;
                 
				    pTmp->nGrpNo  = nGrpNo;
				    pTmp->nFileNo = FILE_TYPE_CUR;    //��ǰ�����ļ�
				    strcpy(pTmp->szFileName, szFileName);
				    pTmp->mTime = FileInfo.CFG_SVR_MTIME;
				    
                    m_mapFileRelation.insert( PAIR< STRING, SGroupAndFile * >(STRING(szFileFullPath),pTmp) );

				    //ȡ��ȥ����׺��ȫ·����
				    char *pdest = strstr(szFileFullPath,".ini");
                    if (pdest != NULL)
                    {
                        pdest[0] = '\0';
                    }
				    if ( !m_pCfgFileMgr->AddCfgFileGrp(nGrpNo, szFileFullPath) )
                    {
                        TRACE(MTS_CFGSVR,S_CFG_MODULE_STARTUP_FAIL);
                        return ERR_FAIL;
                    }

				    nGrpNo++;
			    }
            }
        }
        //������ȡ��һ���ļ�
        pDirent = FindDir.read();
    }; 
    
    //��ʼ��m_pMMLReport����
    m_pMMLReport = new CQBMMLReport();
    
    //ע�ᶨʱ��
    m_lTimerID = SetTimer(ONE_SECOND_IN_MILISEC);
 
    TRACE(MTS_CFGSVR,S_CFG_MODULE_STARTUP_SUCCESS);
    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�ExitInstance()
�����������˳�������
���������
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/15
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ExitInstance()
{	
    if (m_pParaFile != NULL)
    {
        delete m_pParaFile;
        m_pParaFile = NULL;
    }
    if (m_pCfgFileMgr != NULL)
    {
        delete m_pCfgFileMgr;
        m_pCfgFileMgr = NULL;
    }
    if (m_pMMLReport != NULL)
    {
        delete m_pMMLReport;
        m_pMMLReport = NULL;
    }
    //���ӳ���
    FILERELATION_MAP::iterator it;
    for(it=m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
    {
        delete (*it).second;
    }
	m_mapFileRelation.clear();

    //ע����ʱ��
    KillTimer(m_lTimerID);
    
    CWorkModule::ExitInstance();	
}

/*******************************************************************
�������ƣ�GetModuleType()
������������ȡ��ģ������
���������
�����������
�������ݣ���ģ�����ͺ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
MODULE_TYPE CCfgSvr::GetModuleType()
{
	return MT_CFGSVR;
}

/*******************************************************************
�������ƣ�ParseMsg(MSG_INTRA* pMsgParsed)
�������������������ַ�
���������MSG_INTRA* pMsgParsed,����֡
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    ASSERT(pMsgParsed != NULL);

    //����CMD_MML����
    CParseResult* pParseResult = NULL;

    switch (pMsgParsed->CmdCode)
    {
    case CMD_MML:
		pParseResult = *((CParseResult**)(pMsgParsed->Data));
        m_nCurClientNo = pMsgParsed->ClientNo;

		switch (pParseResult->GetCmdCode())
		{
		case MML_LST_FILECLASS:
            //��������̨���������ļ�����
			ProcGetCfgFile(pMsgParsed);
			break; 
		case MML_LST_PARASECT:
            //��������̨���������Ϣ�ļ������ж���
			ProcGetAllParaSect(pMsgParsed);
			break; 
		case MML_LST_PARAINFO:
            //��������̨���������Ϣ�ļ�ָ�����������в�����Ϣ
			ProcGetParaItemFromSect(pMsgParsed);
			break; 
		case MML_LST_CFGSECT:
            //��������̨����ָ�������ļ������ж���
			ProcGetAllCfgSect(pMsgParsed);
			break; 
		case MML_LST_CFGINFO:
            //��������̨����ָ�������ļ���ָ������������������Ϣ
			ProcGetCfgItemFromSect(pMsgParsed);
			break; 
		case MML_SET_CFGINFO:
            //��������̨����ָ�������ļ���������Ϣ
			ProcSetCfgInfo(pMsgParsed);
			break; 
        case MML_OPR_SYS:
            //��������̨��������/�ر�IGWB����
            ProcSwitch(pMsgParsed);
            break;
		default:
			break;
		}
        break;
        
	default:
		break;
	}
}


/*******************************************************************
�������ƣ�OnTimer(const long nTimerID)
������������ʱ���¼�������
���������const long nTimerID,
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::OnTimer(const long nTimerID)
{   
    if (nTimerID == m_lTimerID)
    {
        Timer1Sec();		    
    }
    else
	{
		CWorkModule::OnTimer(nTimerID);
	}
}

/*******************************************************************
�������ƣ�ProcGetCfgFile(MSG_INTRA* pMsgParsed)
���������������������ļ�����
���������MSG_INTRA* pMsgParsed,����֡
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ProcGetCfgFile(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);
    
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_CFGGRP_NO);
	TitleList.push_back(S_CFG_TT_CFGGRP_NAME);
    TitleList.push_back(S_CFG_TT_CFGGRP_TIME);

    //�����ź�����
    LIST<STRING> ContentList;
	FILERELATION_MAP::iterator it;
	char szTmp[128];
	
	for(it = m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
	{
		sprintf(szTmp, "%d", (*it).second->nGrpNo);	
		ContentList.push_back(szTmp);

		ContentList.push_back((*it).second->szFileName);
        
        time_t tFile = (*it).second->mTime;
        struct tm tmLocal = *ACE_OS::localtime(&tFile); 
        sprintf(szTmp,"%d-%02d-%02d %02d:%02d:%02d",
                tmLocal.tm_year+1900,
                tmLocal.tm_mon+1,
                tmLocal.tm_mday,
                tmLocal.tm_hour,
                tmLocal.tm_min,
                tmLocal.tm_sec);
        ContentList.push_back(szTmp);
	}
   
	SendHoriTableReport(pParseResult, TitleList, ContentList, S_CFG_TH_CFGFILE);

}

/*******************************************************************
�������ƣ�ProcGetAllParaSect(MSG_INTRA* pMsgParsed)
�������������������в�������
���������MSG_INTRA* pMsgParsed,����֡
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ProcGetAllParaSect(MSG_INTRA* pMsgParsed)
{
    ASSERT(pMsgParsed != NULL);

	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_PARA_SECTNAME);

    //��ò����ļ����ж���
    LIST<STRING> ContentList;
	m_pParaFile->GetAllSectName(ContentList);
   
	SendHoriTableReport(pParseResult, TitleList, ContentList, S_CFG_TH_PARA_SECTNAME);
}


/*******************************************************************
�������ƣ�������ĳ�ε����в�����
����������ProcGetParaItemFromSect(MSG_INTRA* pMsgParsed)
���������MSG_INTRA* pMsgParsed,����֡
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ProcGetParaItemFromSect(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);
    
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//��ö���
    char* szSectName = (char* )pParseResult->GetStringVal(1);

	//��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_PARA_NAME);
	TitleList.push_back(S_CFG_TT_PARA_DESC);
	TitleList.push_back(S_CFG_TT_PARA_TYPE);
	TitleList.push_back(S_CFG_TT_PARA_DAFAULT);
	TitleList.push_back(S_CFG_TT_PARA_RELASECT);
	TitleList.push_back(S_CFG_TT_PARA_MUSTFLAG);
	TitleList.push_back(S_CFG_TT_PARA_CHNFLAG);

    //���ݶ�����ò�������
    LIST<STRING> ContentList;
	PARAITEM_LIST* pParaList = m_pParaFile->GetParaListFromSectName(szSectName);

	if (pParaList == NULL)
	{
		//��ʾ�޷���ô˶����Ĳ�����Ϣ
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, S_CFG_TT_PARA_NOITEM);		
	}
	else
	{
		char szTmp[5];
		PARAITEM_LIST::iterator it;

		for(it = pParaList->begin(); it != pParaList->end(); it++)
		{
			ContentList.push_back((*it)->szItemName);
            if ((*it)->szItemDesc[0] == '\0')
            {
                ContentList.push_back("-");
            }
            else
            {
			    ContentList.push_back((*it)->szItemDesc);
            }
            //ContentList.push_back((*it)->szItemDesc);
			
            sprintf(szTmp, "%d", (*it)->nItemType);
			ContentList.push_back(szTmp);

            if ((*it)->szDefaultValue[0] == '\0')
            {
                ContentList.push_back("-");
            }
            else
            {
			    ContentList.push_back((*it)->szDefaultValue);
            }
			//ContentList.push_back((*it)->szDefaultValue);

            if ((*it)->szParaRelaSect[0] == '\0')
            {
                ContentList.push_back("-");
            }
            else
            {
			    ContentList.push_back((*it)->szParaRelaSect);
            }
            //ContentList.push_back((*it)->szParaRelaSect);
			sprintf(szTmp, "%d", (*it)->bIsMustCfg);
			ContentList.push_back(szTmp);

			sprintf(szTmp, "%d", (*it)->bIsCanChg);
			ContentList.push_back(szTmp);
		}
		
		SendHoriTableReport(pParseResult, TitleList, ContentList, S_CFG_TH_PARA_PARAITEM);
	}
}

/*******************************************************************
�������ƣ�ProcSwitch(MSG_INTRA* pMsgParsed)
��������������رջ�������ϵͳ
���������MSG_INTRA* pMsgParsed,����֡
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ProcSwitch(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);

	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//��ö�����־
    int nFlag;
	pParseResult->GetDigitVal(&nFlag, 1);

	if (nFlag == 0)
	{
		//�ر�igwb
		KillIGWB();
	}
	else
	{
		//����igwb
		StartIGWB();
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, "");
	}
}
/*******************************************************************
�������ƣ�ProcGetAllCfgSect(MSG_INTRA* pMsgParsed)
����������������ĳһ�����ļ������ж���
���������MSG_INTRA* pMsgParsed,����֡
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ProcGetAllCfgSect(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);

    int nGrpNo,nFileNo;
	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//������
	pParseResult->GetDigitVal(&nGrpNo, 1);
	//����ļ���
	pParseResult->GetDigitVal(&nFileNo, 2);

	//��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_CFG_SECTNAME);

    //��������ļ����ж���
    LIST<STRING> ContentList;
	m_pCfgFileMgr->GetAllSectName(ContentList, nGrpNo, nFileNo);
   
	SendHoriTableReport(pParseResult, TitleList, ContentList, S_CFG_TH_CFG_SECTNAME);
}

/*******************************************************************
�������ƣ�ProcGetCfgItemFromSect(MSG_INTRA* pMsgParsed)
����������������ĳһ�����ļ���ĳ�ε�����������
���������MSG_INTRA* pMsgParsed,����֡
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ProcGetCfgItemFromSect(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);

    int nGrpNo,nFileNo;
	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	
	//������
	pParseResult->GetDigitVal(&nGrpNo, 1);
	//����ļ���
	pParseResult->GetDigitVal(&nFileNo, 2);
    //��ö���
    char* szSectName = (char* )pParseResult->GetStringVal(3);

	//��ȡ�����͵ı��ı��������
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_CFG_NAME);
	TitleList.push_back(S_CFG_TT_CFG_VALUE);
	
    //���ݶ�����ò�������
    LIST<STRING> ContentList;
	CFGITEM_LIST* pCfgList = m_pCfgFileMgr->GetCfgListFromSectName(szSectName, 
        nGrpNo, nFileNo);

	if (pCfgList == NULL)
	{
		//��ʾ�޷���ô˶�����������Ϣ
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, S_CFG_TT_CFG_NOITEM);		
	}
	else
	{
		CFGITEM_LIST::iterator it;

		for(it = pCfgList->begin(); it != pCfgList->end(); it++)
		{
            if((*it)->szItemValue[0] != '\0')
            {
				ContentList.push_back((*it)->szItemName);

				//�õ��ֶ����ݵĳ���
				int nLength = strlen((*it)->szItemValue);

				//����һ����ʱ���飬���ڴ洢��Ӧת������ַ���
				char szTempValue[MAX_ITEMVALUE_LEN];  

				//���ԭ�ַ����д����»��ߣ������»���ǰ����һ���ַ���־'\'���洢���µ��ַ�������
				int nTempIndex = 0;
				int nItemIndex = 0;	
				while(nLength > 0)
				{
					//���ԭ�ַ�����Ϊ�»��ߣ������»���ǰ����һ����־'\'���洢���µ��ַ�������
					if ((*it)->szItemValue[nItemIndex] == '_') 
					{
						szTempValue[nTempIndex++] = '\\';
						szTempValue[nTempIndex++] = (*it)->szItemValue[nItemIndex++];
					}
					else
					{
						szTempValue[nTempIndex++] = (*it)->szItemValue[nItemIndex++];
					}
					nLength--;
				}
				szTempValue[nTempIndex] = '\0';

				//����ַ����д��ڿո��򽫿ո�ת�����»���'_'
				for(int nIndex = 0; nIndex<nTempIndex; nIndex++)
				{
					//�����'\'+' '����ϣ��򲻽�����Ŀո�ת����'_'
					if (szTempValue[nIndex] == ' ' && szTempValue[nIndex - 1] == '\\')
					{
						continue;
					}
					if(szTempValue[nIndex] == ' ')
					{
						szTempValue[nIndex] = '_';
					}
				}

			    ContentList.push_back(szTempValue);
            }
/*
            else
            {
			    ContentList.push_back("-");
            }
*/
		}
		
		SendHoriTableReport(pParseResult, TitleList, ContentList, S_CFG_TH_CFG_CFGITEM);
	}
}

/*******************************************************************
�������ƣ�ProcSetCfgInfo(MSG_INTRA* pMsgParsed)
�����������������ò���������Ϣ
���������MSG_INTRA* pMsgParsed,����֡
���������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::ProcSetCfgInfo(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);

    int nGrpNo;
    //ֻ�е�ǰ�����ļ����޸�
    int nFileNo = FILE_TYPE_CUR;   

	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//������
	pParseResult->GetDigitVal(&nGrpNo, 1);

	//��ò�����ʼ��־
    int nParaFlag = -1;
    pParseResult->GetDigitVal(&nParaFlag, 2);

	//������õ�����
	const char* szContent = pParseResult->GetStringVal(3);
    
    switch(nParaFlag) {
    case 0:
    //��ʼ
        {
            if(m_mapGrpNoCfgSet.find(nGrpNo) == m_mapGrpNoCfgSet.end())
            {
                STRING strCfgSet = "";
                m_mapGrpNoCfgSet[nGrpNo] = strCfgSet;
            }

            STRING& strCfgSet = m_mapGrpNoCfgSet[nGrpNo];
            strCfgSet = szContent;
        }
        //��ʾ����̨���������ļ��ɹ�
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, S_CFG_TT_SETCFGSUCCESS);
    	break;
    case 1:
    //�м�
        {
            if(m_mapGrpNoCfgSet.find(nGrpNo) == m_mapGrpNoCfgSet.end())
            {
                STRING strCfgSet = "";
                m_mapGrpNoCfgSet[nGrpNo] = strCfgSet;
            }

            STRING& strCfgSet = m_mapGrpNoCfgSet[nGrpNo];
            if(strCfgSet != "")
            {
                strCfgSet += "|";
            }
            strCfgSet += szContent;
        }
        //��ʾ����̨���������ļ��ɹ�
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, S_CFG_TT_SETCFGSUCCESS);
    	break;
    case 2:
    //����
        {
            if(m_mapGrpNoCfgSet.find(nGrpNo) == m_mapGrpNoCfgSet.end())
            {
                STRING strCfgSet = "";
                m_mapGrpNoCfgSet[nGrpNo] = strCfgSet;
            }

            STRING& strCfgSet = m_mapGrpNoCfgSet[nGrpNo];
            if(strCfgSet != "")
            {
                strCfgSet += "|";
            }
            strCfgSet += szContent;
        }
    	break;
    default:
        break;
    }

    if(nParaFlag != 2)
    {
        return;
    }

    //���õ�ǰ�����ļ�ǰ�ȱ��浱ǰ�����ļ��ĸ�������׺��Ϊ".tmp",���óɹ�
    //�����ø��������ϴ������ļ�������ԭ��ǰ�����ļ������ϴ������ļ�
    
    //��ǰ�����ļ�ȫ·����
    STRING strCurFilePath = GetFullPathByGrpNo(nGrpNo);
    char szCurFile[MAX_PATH];
    strcpy(szCurFile,strCurFilePath.c_str());
    //ȥ��".ini"��׺��
    int nPosition = strCurFilePath.find(".ini");
    if (nPosition != -1)
    {
        strCurFilePath[nPosition] = '\0';
    }
    //��ʱ�ļ�ȫ·����
    char szTmpFile[MAX_PATH];
    sprintf(szTmpFile,"%s%s",strCurFilePath.c_str(),".tmp");
    //�ϴ������ļ�ȫ·����
    char szLstFile[MAX_PATH];
    sprintf(szLstFile,"%s%s",strCurFilePath.c_str(),".lst");
    //����ǰ�����ļ�����һ����".tmp"Ϊ��׺���Ա��ɹ����õ�ǰ�����ļ�������ϴ������ļ�
    BOOL bRet = FileCopy(szCurFile, szTmpFile);

    if(m_mapGrpNoCfgSet.find(nGrpNo) == m_mapGrpNoCfgSet.end())
    {
        STRING strCfgSet = "";
        m_mapGrpNoCfgSet[nGrpNo] = strCfgSet;
    }

    szContent = m_mapGrpNoCfgSet[nGrpNo].c_str();

    //���õ�ǰ�����ļ�
	if (!m_pCfgFileMgr->SetAllCfgItem(szContent, nGrpNo, nFileNo))
	{
        if(g_szErrorReason[0] == '\0')
        {
            strncpy(g_szErrorReason, S_CFG_TT_SETCFGFAIL, sizeof(g_szErrorReason));
            g_szErrorReason[sizeof(g_szErrorReason) - 1] = '\0';
        }
        //��ʾ����̨���������ļ�ʧ��
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, g_szErrorReason);
        //��մ����ַ���
        g_szErrorReason[0] = '\0';     
	}
    else
    {
        //��ʾ����̨���������ļ��ɹ�
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, S_CFG_TT_SETCFGSUCCESS);
        //��������ļ�������޸�ʱ��
        FILERELATION_MAP::iterator it;	
	    for (it = m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
	    {
            if (it->second->nGrpNo == nGrpNo)
		    {
                //���»�ȡ�����ļ�������޸�ʱ�䣬����Timer1Sec()����ˢ��
				ACE_stat FileStat;
		        ACE_OS::stat(szCurFile, &FileStat);
                it->second->mTime = FileStat.CFG_SVR_MTIME;
		    }
        }
        //�����ϴ������ļ�
        if (bRet)
        {
            //�õ�ǰ�����ļ����������ϴ������ļ�
            FileCopy(szTmpFile, szLstFile);
            //ˢ���ϴ������ļ�ӳ���
            m_pCfgFileMgr->UpdateLastCfg(nGrpNo,szLstFile);
        }
    }

    //ɾ����ʱ�ļ�
    if (bRet)
    {
        FileDel(szTmpFile);
    }
}


/*******************************************************************
�������ƣ�Timer1Sec()
��������������1���¼������֣�ȷ�ϣ�����ļ�״̬
�����������
�����������
�������ݣ���
���ú�����
   ����1��SendTriggerInfo�����ʹ�����ʽ����Ϣ
����������
   ����1��OnTimer()����ʱ������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::Timer1Sec()
{
	//��ʱ����ļ�
	ACE_stat FileStat;

	//˳���ȡ�ҵ����ļ�
	FILERELATION_MAP::iterator it;	
	for (it = m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
	{
		//����޸�ʱ��
		ACE_OS::stat(it->first.c_str(), &FileStat);

		//����޸�ʱ�䷢���ı�
		if (it->second->mTime != FileStat.CFG_SVR_MTIME)
		{
			//����ĳ��ĵ�ǰ�����ļ�
			if (!m_pCfgFileMgr->UpdateCurrentCfg(it->second->nGrpNo, 
                it->first.c_str()))
            {
                //��ˢ�²��ɹ����򸲸��������ļ����������»�ȡ����ļ�������޸�ʱ��
				ACE_OS::stat(it->first.c_str(), &FileStat);
            }
            it->second->mTime = FileStat.CFG_SVR_MTIME;
		}
	}
}

/*******************************************************************
�������ƣ�StartIGWB()
��������������igwb
�����������
�����������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgSvr::StartIGWB()
{
    char szAppPath[MAX_PATH];
    char szFix[MAX_PATH];
    sprintf(szAppPath, S_APPNAME_AGENT);
    sprintf(szFix, "./");
    szAppPath[MAX_PATH - 1] = '\0';
    szFix[MAX_PATH - 1] = '\0';

#ifdef _PLATFORM_WIN32
    strcat(szAppPath, ".exe");
#else
    strcat(szAppPath, " &");    
     strcat(szFix, szAppPath);   
#endif      
   
    int m_PID = system1(szAppPath);
    if (-1 == m_PID) 
    {
        return false;
    }      
    return TRUE;

}


/*******************************************************************
�������ƣ�KillIGWB()
����������ɱ��igwb
�����������
�����������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
BOOL CCfgSvr::KillIGWB()
{
	MSG_INTRA *pMsg = new MSG_INTRA;
    
    pMsg->SenderPid   = PT_SELF; 
    pMsg->SenderMid   = MT_CFGSVR;
    pMsg->ReceiverPid = PT_KERNEL;
    pMsg->ReceiverMid = MT_MONITOR;
    pMsg->CmdCode     = CMD_KILL_PROCESS;

    CWorkModule::SendMsg(pMsg);

	return TRUE;
}


/*******************************************************************
�������ƣ�SendSimpleInfoReport(const char* szCmd, int nRetCode, 
                               const char* szInfo)
�������������ͱ�������ֻ�м򵥵���Ϣ��MML����
���������
   ����1��const char* szCmd,MML�����Ӧ������
   ����2��int nRetCode,����Ĵ�����
   ����3��const char* szInfo,�������еļ���Ϣ
�����������
�������ݣ�
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::SendSimpleInfoReport(const char* szCmd, int nRetCode, 
                                         const char* szInfo) 
{
    m_pMMLReport->MakeQBReportHead(szCmd, nRetCode);
    m_pMMLReport->AddString(szInfo);
	m_pMMLReport->AddString("\r\n");
    m_pMMLReport->MakeReportTail();
    const char* szReport = m_pMMLReport->GetReport();

    SendMMLReport(szReport);
}


/*******************************************************************
�������ƣ�SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName)
�������������ͱ�����Ϊһ���������MML����
���������
   ����1��CParseResult* pParseResult,MML���������Ķ���
   ����2��LIST<STRING>& TitleList,���ı���
   ����3��LIST<STRING>& ContentList,��������
   ����4��const char* szTableHeadName,������������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName)
{
    //���ڱ������ݿ��ܻ�ܴ����Ա��뿼���Է�ҳ�ķ�ʽ����MML����
    int nSubPageNo = 1;
    const char* szReport;
    while (!ContentList.empty() || (1 == nSubPageNo))
    {
        m_pMMLReport->MakeQBReportHead(pParseResult->GetCmdLine());
        m_pMMLReport->AddQBHoriTable(TitleList, ContentList, 
                                szTableHeadName, nSubPageNo);
        m_pMMLReport->MakeReportTail();

        szReport = m_pMMLReport->GetReport();
        SendMMLReport(szReport);
    }
}

/*******************************************************************
�������ƣ�SendVertTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        LIST<STRING>& RecHeadList
�������������ͱ�����Ϊһ���������MML����
���������
   ����1��CParseResult* pParseResult,MML���������Ķ���
   ����2��LIST<STRING>& TitleList,���ı���
   ����3��LIST<STRING>& ContentList,��������
   ����4��LIST<STRING>& RecHeadList,ͷ�б�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::SendVertTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        LIST<STRING>& RecHeadList)
{
    //���ڱ������ݿ��ܻ�ܴ����Ա��뿼���Է�ҳ�ķ�ʽ����MML����
    const char* szReport;
	int nSubPageNo = 1;
    while (!ContentList.empty() || (1 == nSubPageNo))
    {
        m_pMMLReport->MakeQBReportHead(pParseResult->GetCmdLine());
        m_pMMLReport->AddQBVertTable(TitleList, ContentList, 
                                    RecHeadList);
        m_pMMLReport->MakeReportTail();

        szReport = m_pMMLReport->GetReport();
        SendMMLReport(szReport);

		nSubPageNo++;
    }
}

/*******************************************************************
�������ƣ�SendMMLReport(const char* szReport)
������������MML����ת����MSG_INTRA����ʽ���ͳ�ȥ
���������const char* szReport,�����͵�MML����
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
void CCfgSvr::SendMMLReport(const char* szReport)
{
    int nStrLen = strlen(szReport);

    MSG_INTRA* pMsg = new(nStrLen + 1) MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_CFGSVR;
	pMsg->ReceiverPid = PT_OUTSIDE;
	pMsg->ReceiverMid = PT_SELF;
    pMsg->AppType = APP_MML_NORM;
    pMsg->ClientNo = m_nCurClientNo;

    memcpy(pMsg->Data, szReport, nStrLen + 1);

    CWorkModule::SendMsg(pMsg);
}

/*******************************************************************
�������ƣ�GetFullPathByGrpNo(int nGrpNo)
���������������ļ����ȡ�������ļ���ȫ·����
���������int nGrpNo, �ļ����
�����������
�������ݣ������ļ���ȫ·����
���ú�����
����������
����ʱ�䣺dengwanqiu,2003/07/31
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����      



********************************************************************/
STRING CCfgSvr::GetFullPathByGrpNo(int nGrpNo)
{
    FILERELATION_MAP::iterator it;	
    for (it = m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
    {
        if (it->second->nGrpNo == nGrpNo)
        {
            return it->first;
        }
    }

    return STRING("");
}
