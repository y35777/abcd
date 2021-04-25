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

//记录设置配置参数失败原因，当失败时，将此信息反馈给客户端
char g_szErrorReason[20*1024] = "";   

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*******************************************************************
函数名称：CCfgSvr()
功能描述：构造函数,初始化成员变量
输入参数：
输出参数：无
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
CCfgSvr::CCfgSvr() : m_pParaFile(NULL), m_pCfgFileMgr(NULL), 
                     m_pMMLReport(NULL), m_nCurClientNo(1)
{    
}

/*******************************************************************
函数名称：~CCfgSvr()
功能描述：析构函数,删除指针成员变量
输入参数：
输出参数：无
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：InitInstance()
功能描述：初始化函数
输入参数：
输出参数：无
返回内容：返回错误码(ERR_SUCCESS表示成功,ERR_FAIL表示失败)
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
int CCfgSvr::InitInstance()
{
    //调用初始化基类函数；
    int nResult = CWorkModule::InitInstance();
    if (ERR_SUCCESS != nResult)
    {
        TRACE(MTS_CFGSVR,S_CFG_MODULE_STARTUP_FAIL);
        return ERR_FAIL;
    }

    //参数配置及信息文件存放路径
    char szDirPath[MAX_PATH];
	int nLen = SNPRINTF(szDirPath, MAX_PATH - 1, "%s", GetCfgFilePath());
	szDirPath[nLen] = '\0';

    //去掉配置参数文件名后，即为配置参数文件全路径
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

    //参数信息文件(allpara)与配置文件位于相同的目录下; 取allpara文件的全路径名
	char szParaFileFullPath[MAX_PATH];
	nLen = SNPRINTF(szParaFileFullPath, MAX_PATH - 1, "%sallpara", szDirPath);
	szParaFileFullPath[nLen] = '\0';

    //如m_pParaFile已存在，则先关闭
    if (m_pParaFile != NULL)
    {
        delete m_pParaFile;
    }
    //初始化所有参数对象m_pParaFile
    m_pParaFile = new  CParaFile();
    if ( !m_pParaFile->Init(szParaFileFullPath) )
    {
        TRACE(MTS_CFGSVR,S_CFG_MODULE_STARTUP_FAIL);
        return ERR_FAIL;
    }
    //初始化配置文件管理器m_pCfgFileMgr；
    if (m_pCfgFileMgr != NULL)
    {
        delete m_pCfgFileMgr;
    }
    m_pCfgFileMgr = new  CCfgFileMgr();  
    
    //查找所有ini文件，建立监视文件列表，记录每个文件的修改时间；
    int nGrpNo = 0;
    ACE_Dirent FindDir((const ACE_TCHAR* )szDirPath);

    //搜索指定的目录，找出所有的以ini为后缀的配置文件
    dirent* pDirent;
    ACE_stat FileInfo;
    char szFileFullPath[MAX_PATH], szFileName[MAX_PATH];
    pDirent = FindDir.read();
    while (pDirent)
    {
        strcpy(szFileName, pDirent->d_name);
        sprintf(szFileFullPath, "%s%s", szDirPath, szFileName);
        ACE_OS::stat(szFileFullPath, &FileInfo);

        //如果找到的是一个文件
        if ((S_IFDIR != (FileInfo.st_mode & S_IFMT)))
        {			
			char* szDest = strstr(szFileName, ".ini");
            if (szDest != NULL)
            {
			    //判断是否以ini为后缀			
			    if (strcmp(szDest, ".ini") == 0)
			    {   
				    SGroupAndFile* pTmp = new  SGroupAndFile;
                 
				    pTmp->nGrpNo  = nGrpNo;
				    pTmp->nFileNo = FILE_TYPE_CUR;    //当前配置文件
				    strcpy(pTmp->szFileName, szFileName);
				    pTmp->mTime = FileInfo.CFG_SVR_MTIME;
				    
                    m_mapFileRelation.insert( PAIR< STRING, SGroupAndFile * >(STRING(szFileFullPath),pTmp) );

				    //取得去掉后缀的全路径名
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
        //继续读取下一个文件
        pDirent = FindDir.read();
    }; 
    
    //初始化m_pMMLReport对象；
    m_pMMLReport = new CQBMMLReport();
    
    //注册定时器
    m_lTimerID = SetTimer(ONE_SECOND_IN_MILISEC);
 
    TRACE(MTS_CFGSVR,S_CFG_MODULE_STARTUP_SUCCESS);
    return ERR_SUCCESS;
}


/*******************************************************************
函数名称：ExitInstance()
功能描述：退出处理函数
输入参数：
输出参数：无
返回内容：成功返回TRUE,失败返回FALSE
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/15
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
    //清除映射表
    FILERELATION_MAP::iterator it;
    for(it=m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
    {
        delete (*it).second;
    }
	m_mapFileRelation.clear();

    //注销定时器
    KillTimer(m_lTimerID);
    
    CWorkModule::ExitInstance();	
}

/*******************************************************************
函数名称：GetModuleType()
功能描述：获取本模块类型
输入参数：
输出参数：无
返回内容：本模块类型号
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
MODULE_TYPE CCfgSvr::GetModuleType()
{
	return MT_CFGSVR;
}

/*******************************************************************
函数名称：ParseMsg(MSG_INTRA* pMsgParsed)
功能描述：命令解析与分发
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::ParseMsg(MSG_INTRA* pMsgParsed)
{
    ASSERT(pMsgParsed != NULL);

    //分析CMD_MML命令
    CParseResult* pParseResult = NULL;

    switch (pMsgParsed->CmdCode)
    {
    case CMD_MML:
		pParseResult = *((CParseResult**)(pMsgParsed->Data));
        m_nCurClientNo = pMsgParsed->ClientNo;

		switch (pParseResult->GetCmdCode())
		{
		case MML_LST_FILECLASS:
            //处理配置台请求配置文件种类
			ProcGetCfgFile(pMsgParsed);
			break; 
		case MML_LST_PARASECT:
            //处理配置台请求参数信息文件的所有段名
			ProcGetAllParaSect(pMsgParsed);
			break; 
		case MML_LST_PARAINFO:
            //处理配置台请求参数信息文件指定段名的所有参数信息
			ProcGetParaItemFromSect(pMsgParsed);
			break; 
		case MML_LST_CFGSECT:
            //处理配置台请求指定配置文件的所有段名
			ProcGetAllCfgSect(pMsgParsed);
			break; 
		case MML_LST_CFGINFO:
            //处理配置台请求指定配置文件的指定段名的所有配置信息
			ProcGetCfgItemFromSect(pMsgParsed);
			break; 
		case MML_SET_CFGINFO:
            //处理配置台设置指定配置文件的配置信息
			ProcSetCfgInfo(pMsgParsed);
			break; 
        case MML_OPR_SYS:
            //处理配置台请求启动/关闭IGWB程序
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
函数名称：OnTimer(const long nTimerID)
功能描述：定时器事件处理函数
输入参数：const long nTimerID,
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：ProcGetCfgFile(MSG_INTRA* pMsgParsed)
功能描述：处理获得配置文件种类
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::ProcGetCfgFile(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);
    
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//获取待发送的表格的标题和内容
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_CFGGRP_NO);
	TitleList.push_back(S_CFG_TT_CFGGRP_NAME);
    TitleList.push_back(S_CFG_TT_CFGGRP_TIME);

    //获得组号和组名
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
函数名称：ProcGetAllParaSect(MSG_INTRA* pMsgParsed)
功能描述：处理获得所有参数段名
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::ProcGetAllParaSect(MSG_INTRA* pMsgParsed)
{
    ASSERT(pMsgParsed != NULL);

	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//获取待发送的表格的标题和内容
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_PARA_SECTNAME);

    //获得参数文件所有段名
    LIST<STRING> ContentList;
	m_pParaFile->GetAllSectName(ContentList);
   
	SendHoriTableReport(pParseResult, TitleList, ContentList, S_CFG_TH_PARA_SECTNAME);
}


/*******************************************************************
函数名称：处理获得某段的所有参数项
功能描述：ProcGetParaItemFromSect(MSG_INTRA* pMsgParsed)
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::ProcGetParaItemFromSect(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);
    
    CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//获得段名
    char* szSectName = (char* )pParseResult->GetStringVal(1);

	//获取待发送的表格的标题和内容
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_PARA_NAME);
	TitleList.push_back(S_CFG_TT_PARA_DESC);
	TitleList.push_back(S_CFG_TT_PARA_TYPE);
	TitleList.push_back(S_CFG_TT_PARA_DAFAULT);
	TitleList.push_back(S_CFG_TT_PARA_RELASECT);
	TitleList.push_back(S_CFG_TT_PARA_MUSTFLAG);
	TitleList.push_back(S_CFG_TT_PARA_CHNFLAG);

    //根据段名获得参数项名
    LIST<STRING> ContentList;
	PARAITEM_LIST* pParaList = m_pParaFile->GetParaListFromSectName(szSectName);

	if (pParaList == NULL)
	{
		//提示无法获得此段名的参数信息
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
函数名称：ProcSwitch(MSG_INTRA* pMsgParsed)
功能描述：处理关闭或者启动系统
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::ProcSwitch(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);

	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//获得动作标志
    int nFlag;
	pParseResult->GetDigitVal(&nFlag, 1);

	if (nFlag == 0)
	{
		//关闭igwb
		KillIGWB();
	}
	else
	{
		//启动igwb
		StartIGWB();
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, "");
	}
}
/*******************************************************************
函数名称：ProcGetAllCfgSect(MSG_INTRA* pMsgParsed)
功能描述：处理获得某一配置文件的所有段名
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::ProcGetAllCfgSect(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);

    int nGrpNo,nFileNo;
	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//获得组号
	pParseResult->GetDigitVal(&nGrpNo, 1);
	//获得文件号
	pParseResult->GetDigitVal(&nFileNo, 2);

	//获取待发送的表格的标题和内容
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_CFG_SECTNAME);

    //获得配制文件所有段名
    LIST<STRING> ContentList;
	m_pCfgFileMgr->GetAllSectName(ContentList, nGrpNo, nFileNo);
   
	SendHoriTableReport(pParseResult, TitleList, ContentList, S_CFG_TH_CFG_SECTNAME);
}

/*******************************************************************
函数名称：ProcGetCfgItemFromSect(MSG_INTRA* pMsgParsed)
功能描述：处理获得某一配置文件的某段的所有配置项
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::ProcGetCfgItemFromSect(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);

    int nGrpNo,nFileNo;
	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	
	//获得组号
	pParseResult->GetDigitVal(&nGrpNo, 1);
	//获得文件号
	pParseResult->GetDigitVal(&nFileNo, 2);
    //获得段名
    char* szSectName = (char* )pParseResult->GetStringVal(3);

	//获取待发送的表格的标题和内容
    LIST<STRING> TitleList;
    TitleList.push_back(S_CFG_TT_CFG_NAME);
	TitleList.push_back(S_CFG_TT_CFG_VALUE);
	
    //根据段名获得参数项名
    LIST<STRING> ContentList;
	CFGITEM_LIST* pCfgList = m_pCfgFileMgr->GetCfgListFromSectName(szSectName, 
        nGrpNo, nFileNo);

	if (pCfgList == NULL)
	{
		//提示无法获得此段名的配置信息
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

				//得到字段内容的长度
				int nLength = strlen((*it)->szItemValue);

				//声明一个临时数组，用于存储相应转换后的字符串
				char szTempValue[MAX_ITEMVALUE_LEN];  

				//如果原字符串中存在下划线，则在下划线前加上一个字符标志'\'，存储到新的字符数组中
				int nTempIndex = 0;
				int nItemIndex = 0;	
				while(nLength > 0)
				{
					//如果原字符串中为下划线，则在下划线前面作一个标志'\'，存储到新的字符数组中
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

				//如果字符串中存在空格，则将空格转换成下划线'_'
				for(int nIndex = 0; nIndex<nTempIndex; nIndex++)
				{
					//如果是'\'+' '的组合，则不将后面的空格转换成'_'
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
函数名称：ProcSetCfgInfo(MSG_INTRA* pMsgParsed)
功能描述：处理设置参数配置信息
输入参数：MSG_INTRA* pMsgParsed,命令帧
输出参数：
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::ProcSetCfgInfo(MSG_INTRA* pMsgParsed)
{
	ASSERT(pMsgParsed != NULL);

    int nGrpNo;
    //只有当前配置文件能修改
    int nFileNo = FILE_TYPE_CUR;   

	CParseResult* pParseResult = *((CParseResult** )(pMsgParsed->Data));
	//获得组号
	pParseResult->GetDigitVal(&nGrpNo, 1);

	//获得参数起始标志
    int nParaFlag = -1;
    pParseResult->GetDigitVal(&nParaFlag, 2);

	//获得设置的内容
	const char* szContent = pParseResult->GetStringVal(3);
    
    switch(nParaFlag) {
    case 0:
    //开始
        {
            if(m_mapGrpNoCfgSet.find(nGrpNo) == m_mapGrpNoCfgSet.end())
            {
                STRING strCfgSet = "";
                m_mapGrpNoCfgSet[nGrpNo] = strCfgSet;
            }

            STRING& strCfgSet = m_mapGrpNoCfgSet[nGrpNo];
            strCfgSet = szContent;
        }
        //提示配置台设置配置文件成功
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, S_CFG_TT_SETCFGSUCCESS);
    	break;
    case 1:
    //中间
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
        //提示配置台设置配置文件成功
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, S_CFG_TT_SETCFGSUCCESS);
    	break;
    case 2:
    //结束
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

    //设置当前配置文件前先保存当前配置文件的副本，后缀名为".tmp",设置成功
    //后，则用副本覆盖上次配置文件，这样原当前配置文件就是上次配置文件
    
    //当前配置文件全路径名
    STRING strCurFilePath = GetFullPathByGrpNo(nGrpNo);
    char szCurFile[MAX_PATH];
    strcpy(szCurFile,strCurFilePath.c_str());
    //去掉".ini"后缀名
    int nPosition = strCurFilePath.find(".ini");
    if (nPosition != -1)
    {
        strCurFilePath[nPosition] = '\0';
    }
    //临时文件全路径名
    char szTmpFile[MAX_PATH];
    sprintf(szTmpFile,"%s%s",strCurFilePath.c_str(),".tmp");
    //上次配置文件全路径名
    char szLstFile[MAX_PATH];
    sprintf(szLstFile,"%s%s",strCurFilePath.c_str(),".lst");
    //将当前配置文件复制一份以".tmp"为后缀，以备成功设置当前配置文件后更新上次配置文件
    BOOL bRet = FileCopy(szCurFile, szTmpFile);

    if(m_mapGrpNoCfgSet.find(nGrpNo) == m_mapGrpNoCfgSet.end())
    {
        STRING strCfgSet = "";
        m_mapGrpNoCfgSet[nGrpNo] = strCfgSet;
    }

    szContent = m_mapGrpNoCfgSet[nGrpNo].c_str();

    //设置当前配置文件
	if (!m_pCfgFileMgr->SetAllCfgItem(szContent, nGrpNo, nFileNo))
	{
        if(g_szErrorReason[0] == '\0')
        {
            strncpy(g_szErrorReason, S_CFG_TT_SETCFGFAIL, sizeof(g_szErrorReason));
            g_szErrorReason[sizeof(g_szErrorReason) - 1] = '\0';
        }
        //提示配置台设置配置文件失败
		SendSimpleInfoReport(pParseResult->GetCmdLine(), 1, g_szErrorReason);
        //清空错误字符串
        g_szErrorReason[0] = '\0';     
	}
    else
    {
        //提示配置台设置配置文件成功
        SendSimpleInfoReport(pParseResult->GetCmdLine(), 0, S_CFG_TT_SETCFGSUCCESS);
        //修正监控文件的最后修改时间
        FILERELATION_MAP::iterator it;	
	    for (it = m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
	    {
            if (it->second->nGrpNo == nGrpNo)
		    {
                //重新获取配置文件的最后修改时间，避免Timer1Sec()函数刷新
				ACE_stat FileStat;
		        ACE_OS::stat(szCurFile, &FileStat);
                it->second->mTime = FileStat.CFG_SVR_MTIME;
		    }
        }
        //更新上次配置文件
        if (bRet)
        {
            //用当前配置文件副本覆盖上次配置文件
            FileCopy(szTmpFile, szLstFile);
            //刷新上次配置文件映射表
            m_pCfgFileMgr->UpdateLastCfg(nGrpNo,szLstFile);
        }
    }

    //删除临时文件
    if (bRet)
    {
        FileDel(szTmpFile);
    }
}


/*******************************************************************
函数名称：Timer1Sec()
功能描述：处理1秒事件：握手，确认，检测文件状态
输入参数：无
输出参数：无
返回内容：无
调用函数：
   函数1：SendTriggerInfo，发送触发方式的信息
被调函数：
   函数1：OnTimer()，定时器处理
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::Timer1Sec()
{
	//定时检测文件
	ACE_stat FileStat;

	//顺序获取找到的文件
	FILERELATION_MAP::iterator it;	
	for (it = m_mapFileRelation.begin(); it != m_mapFileRelation.end(); it++)
	{
		//获得修改时间
		ACE_OS::stat(it->first.c_str(), &FileStat);

		//如果修改时间发生改变
		if (it->second->mTime != FileStat.CFG_SVR_MTIME)
		{
			//更新某组的当前配置文件
			if (!m_pCfgFileMgr->UpdateCurrentCfg(it->second->nGrpNo, 
                it->first.c_str()))
            {
                //如刷新不成功，因覆盖了配置文件，则需重新获取监控文件的最后修改时间
				ACE_OS::stat(it->first.c_str(), &FileStat);
            }
            it->second->mTime = FileStat.CFG_SVR_MTIME;
		}
	}
}

/*******************************************************************
函数名称：StartIGWB()
功能描述：启动igwb
输入参数：无
输出参数：无
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：KillIGWB()
功能描述：杀掉igwb
输入参数：无
输出参数：无
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：SendSimpleInfoReport(const char* szCmd, int nRetCode, 
                               const char* szInfo)
功能描述：发送报告体中只有简单的消息的MML报告
输入参数：
   参数1：const char* szCmd,MML报告对应的命令
   参数2：int nRetCode,报告的错误码
   参数3：const char* szInfo,报告体中的简单消息
输出参数：无
返回内容：
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName)
功能描述：发送报告体为一个横向表格的MML报告
输入参数：
   参数1：CParseResult* pParseResult,MML报告解析后的对象
   参数2：LIST<STRING>& TitleList,表格的标题
   参数3：LIST<STRING>& ContentList,表格的内容
   参数4：const char* szTableHeadName,整个表格的名字
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName)
{
    //由于表格的数据可能会很大，所以必须考虑以分页的方式发送MML报告
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
函数名称：SendVertTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        LIST<STRING>& RecHeadList
功能描述：发送报告体为一个纵向表格的MML报告
输入参数：
   参数1：CParseResult* pParseResult,MML报告解析后的对象
   参数2：LIST<STRING>& TitleList,表格的标题
   参数3：LIST<STRING>& ContentList,表格的内容
   参数4：LIST<STRING>& RecHeadList,头列表
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



********************************************************************/
void CCfgSvr::SendVertTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        LIST<STRING>& RecHeadList)
{
    //由于表格的数据可能会很大，所以必须考虑以分页的方式发送MML报告
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
函数名称：SendMMLReport(const char* szReport)
功能描述：将MML报告转换成MSG_INTRA的形式发送出去
输入参数：const char* szReport,待发送的MML报告
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
函数名称：GetFullPathByGrpNo(int nGrpNo)
功能描述：根据文件组号取得配置文件的全路径名
输入参数：int nGrpNo, 文件组号
输出参数：无
返回内容：配置文件的全路径名
调用函数：
被调函数：
作者时间：dengwanqiu,2003/07/31
------------------------------修改记录------------------------------
修改人          修改时间           修改内容      



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
