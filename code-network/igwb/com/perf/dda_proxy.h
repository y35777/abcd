// dda_proxy.h: interface for the CCreateProxy class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _DDA_PROXY_H_
#define _DDA_PROXY_H_

#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "resource.h"
#include "ddaset.h"
#include "ddaquery.h"
class CPerfTask;
struct SInformation;

typedef MAP<STRING,CPerfTask *,LESS<STRING> > TaskMap;

#ifdef _PLATFORM_HPUX
#define CounterMap MAP<int, float>
#define OIVMap MAP<int, int> 
#else
#define CounterMap MAP<int, float,LESS<int> >
#define OIVMap MAP<int, int,LESS<int> > 
#endif

#define IntVector VECTOR<int> 
const char *StrFormat(int nLen, const char *pszEnd);

class CCreateProxy  
{
public:
    CDdaSet* Init(STRING &szDdaFilePath, TaskMap *pmTaskMap);
	CPerfTask *CreateTask(SInformation *pInfo);
	int DeleteTask(STRING szTaskName);
	CCreateProxy();
	virtual ~CCreateProxy();
    const char *GetLastError()
    {
        return m_szError.c_str();
    }

    char *PrintMapKey(char *pOut, CounterMap &Map, int nLen, char cSep = '&')
    {
        char szTmp[256];
        char *pBuf = &szTmp[0];
        CounterMap::iterator it;
        for(it = Map.begin(); it != Map.end(); it++)
        {
            SNPRINTF(pBuf,5,"%-d%-c",it->first,cSep);
            if(it->first >= 10)
                pBuf += 3;
            else
                pBuf += 2;
            
        }
        pBuf --; //��Ϊ���һ��'&'������Ҫ�����һ��'&'��Ϊ' '
        //*pBuf = ' ';
        memset(pBuf,' ',&szTmp[nLen] - pBuf);
        memcpy(pOut,szTmp,nLen);
        return pOut + nLen;
    }
    char *PrintIntVector(char *pOut, VECTOR<int> &IntVec,int nLen, char cSep = '&');
    char *PrintTimeSpan(char *pOut,SInformation *pInfo);
	void AnalysisMU(SInformation *pInfo, const char *pMU);
	void AnalysisOIV(SInformation *pInfo, const char *pOIV);
	void AnalysisITM(SInformation *pInfo,const char *pITM);
	void AnalysisTimeSet(SInformation *pInfo, const char *pTimeSet);
	CDdaSet *m_pDdaSet;	  //DBF�ļ����ϲ�����
    STRING	m_szFilePath; //״̬�ļ��ĸ�Ŀ¼
    STRING  m_szError;    //ʧ����Ϣ
};

class CSaveProxy  
{
public:
	int Save(const time_t &tStart, const time_t &tEnd);
	int Init(STRING &szTaskName, STRING &szRootPath, CounterMap *pmCounterMap);
    const char *GetLastError(){
        return m_szError.c_str();
    }
	CSaveProxy();
	virtual ~CSaveProxy();
protected:
	STRING GenerateFileName(time_t tTime);
	int CreatePerfFile(STRING &szFileName);
protected:
    CDdaSet *m_pDdaSet;	           //����д����������
    STRING	m_szTaskName;          //��������
    CounterMap *m_pCounterMap;     //����ָ���ϣ���ӳ��
    STRING m_szFileName;	       //���ܴ����ļ����ļ���
    STRING m_szFilePath;           //���ܴ����ļ���Ŀ¼��
    STRING m_szError;              //ʧ����Ϣ
    tm m_tLastSaveTime;            //���һ�δ���ʱ��
    int m_nRecordLen;              //һ����¼�Ĵ�С
};


class CQueryProxy : public CDdaQuery 
{
public:
	CQueryProxy();
	virtual ~CQueryProxy();
    const char *GetLastError(){
        return m_szError.c_str();
    }
protected:
    STRING  m_szError;    //ʧ����Ϣ
};

#endif // _DDA_PROXY_H





















