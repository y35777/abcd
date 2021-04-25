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

//�����ļ�����Ϣ: ���ڼ�¼�����ļ���״̬���Լ������Ƿ��޸�
struct SGroupAndFile
{
	char   szFileName[MAX_PATH]; //�����ļ���
	BYTE   nGrpNo;               //�����ļ����
	BYTE   nFileNo;              //�����ļ���
	time_t mTime;                //�����ļ�������޸�ʱ��
};

//�����ļ�ӳ���: �������¼����ж������ļ��Ƿ��ֶ�������
//�����޸ġ����йؼ���Ϊ�����ļ���ȫ·������ֵΪ�����ļ�����Ϣ
typedef MAP< STRING, SGroupAndFile* > FILERELATION_MAP;

class CCfgFileMgr;
class CParaFile;

class CCfgSvr : public CWorkModule
{
public:
	//���캯��
    CCfgSvr(); 
	
	//��������
    virtual ~CCfgSvr();  

protected:
	//��ʼ������
    int InitInstance();  

	//��������������
    void ExitInstance();   
	
	//��ȡ��ģ�����ͺ���
    MODULE_TYPE GetModuleType();    
	
	//��ʱ���¼�������
    void OnTimer(const long nTimerID); 
	
	//���������ַ�����
    void ParseMsg(MSG_INTRA* pMsgParsed);   	
	
protected:
	//���ͱ�������ֻ�м򵥵���Ϣ��MML����
    void SendSimpleInfoReport(const char* szCmd, int nRetCode, 
                              const char* szInfo);

    //���ͱ�����Ϊһ���������MML����
    void SendHoriTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        const char* szTableHeadName);

    //���ͱ�����Ϊһ���������MML����
    void SendVertTableReport(CParseResult* pParseResult, 
                        LIST<STRING>& TitleList, LIST<STRING>& ContentList, 
                        LIST<STRING>& RecTitleList);

	//����MML����
	void SendMMLReport(const char* szReport);

	//�����������ļ�����
	void ProcGetCfgFile(MSG_INTRA* pMsgParsed);

	//���������в�������
	void ProcGetAllParaSect(MSG_INTRA* pMsgParsed);

	//������ĳ�ε����в�����
	void ProcGetParaItemFromSect(MSG_INTRA* pMsgParsed);

	//������ĳһ�����ļ������ж���
	void ProcGetAllCfgSect(MSG_INTRA* pMsgParsed);

	//������ĳһ�����ļ���ĳ�ε�����������
	void ProcGetCfgItemFromSect(MSG_INTRA* pMsgParsed);

	//��������ĳһ�����ļ���������Ϣ
	void ProcSetCfgInfo(MSG_INTRA* pMsgParsed);

	//����رջ�������ϵͳ
	void ProcSwitch(MSG_INTRA* pMsgParsed);

	//1���¼�
	void Timer1Sec();

	//����igwb
	BOOL StartIGWB();

	//ɱ��igwb
	BOOL KillIGWB();
			
protected:
	CParaFile*         m_pParaFile;             //�����ļ�����
    CCfgFileMgr*       m_pCfgFileMgr;			//�����ļ�����������
    int                m_nCurClientNo;			//��ǰ���ڴ���Ļ���̨��̨��
    CQBMMLReport*      m_pMMLReport;			//MML���Ķ���
    long               m_lTimerID;				//��ʱ��ID
	FILERELATION_MAP   m_mapFileRelation;       //�����ļ�ӳ���

private:
    //�����ļ����ȡ�������ļ���ȫ·����
    STRING GetFullPathByGrpNo(int nGrpNo);

    MAP<int, STRING>   m_mapGrpNoCfgSet;
};

#endif //__CFG_SVR_H__
