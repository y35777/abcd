/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.
  
    ******************************************************************************
    File Name     : cfg_svr.h
    Version       : 1.0
    Author        : dengwanqiu
    Created       : 2003/07/31
    Last Modified :
    Description   : interface for the CCfgSvr class.
    Function List :
    History       :
    1.Date        : 2003-07-31
    Author      : dengwanqiu
    Modification: Created file
    
******************************************************************************/
#ifndef __CFG_SVR_H__
#define __CFG_SVR_H__

#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/qb_report.h"
#include "../utility/mml_para.h"

#include "cfg_def.h"

//监视文件项信息: 用于记录配置文件的状态，以监视其是否修改
struct SGroupAndFile
{
	char   szFileName[MAX_PATH]; //监视文件名
	BYTE   nGrpNo;               //监视文件组号
	BYTE   nFileNo;              //监视文件号
	time_t mTime;                //监视文件的最后修改时间
};

//监视文件映射表: 用于秒事件中判断配置文件是否手动或被其它
//进程修改。其中关键字为配置文件的全路径名，值为监视文件项信息
typedef MAP< STRING, SGroupAndFile* > FILERELATION_MAP;

class CCfgFileMgr;
class CParaFile;

class CCfgSvr : public CWorkModule
{
public:
	//构造函数
    CCfgSvr(); 
	
	//析构函数
    virtual ~CCfgSvr();  

protected:
	//初始化函数
    int InitInstance();  

	//结束工作处理函数
    void ExitInstance();   
	
	//获取本模块类型函数
    MODULE_TYPE GetModuleType();    
	
	//定时器事件处理函数
    void OnTimer(const long nTimerID); 
	
	//命令解析与分发函数
    void ParseMsg(MSG_INTRA* pMsgParsed);   	
	
protected:
	//发送报告体中只有简单的消息的MML报告
    void SendSimpleInfoReport(const char* szCmd, int nRetCode, 
                              const char* szInfo);

    //发送报告体为一个横向表格的MML报告
    void SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName);

    //发送报告体为一个纵向表格的MML报告
    void SendVertTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        LIST<STRING>& RecTitleList);

	//发送MML报文
	void SendMMLReport(const char* szReport);

	//处理获得配置文件种类
	void ProcGetCfgFile(MSG_INTRA* pMsgParsed);

	//处理获得所有参数段名
	void ProcGetAllParaSect(MSG_INTRA* pMsgParsed);

	//处理获得某段的所有参数项
	void ProcGetParaItemFromSect(MSG_INTRA* pMsgParsed);

	//处理获得某一配置文件的所有段名
	void ProcGetAllCfgSect(MSG_INTRA* pMsgParsed);

	//处理获得某一配置文件的某段的所有配置项
	void ProcGetCfgItemFromSect(MSG_INTRA* pMsgParsed);

	//处理设置某一配置文件的配置信息
	void ProcSetCfgInfo(MSG_INTRA* pMsgParsed);

	//处理关闭或者启动系统
	void ProcSwitch(MSG_INTRA* pMsgParsed);

	//1秒事件
	void Timer1Sec();

	//启动igwb
	BOOL StartIGWB();

	//杀掉igwb
	BOOL KillIGWB();
			
protected:
	CParaFile*         m_pParaFile;             //参数文件对象
    CCfgFileMgr*       m_pCfgFileMgr;			//配置文件管理器对象
    int                m_nCurClientNo;			//当前正在处理的话单台的台号
    CQBMMLReport*      m_pMMLReport;			//MML报文对象
    long               m_lTimerID;				//定时器ID
	FILERELATION_MAP   m_mapFileRelation;       //监视文件映射表

private:
    //根据文件组号取得配置文件的全路径名
    STRING GetFullPathByGrpNo(int nGrpNo);

    MAP<int, STRING>   m_mapGrpNoCfgSet;
};

#endif //__CFG_SVR_H__
