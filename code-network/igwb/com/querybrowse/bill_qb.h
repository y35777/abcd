#ifndef __BILLQBSERVER_H__
#define __BILLQBSERVER_H__

#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/qb_report.h"
#include "../utility/mml_para.h"
#include "qb_task.h"

#define MAX_REPORT_SIZE (56 * 1024)

//���½ṹ��������һ�������������Ϣ
struct SAPInfo
{
    char szAPName[255];
    int  nAPType;
    int  nAPID;
    BOOL bMustMerge;
    BOOL bFinalSPFileFmt;     //���ջ����Ƿ���������ļ���ʽ
};


//Added by XDB on 2002.11.11
//�ļ�״̬�ṹ
struct SFILE_STATE
{
    STRING sFileName;
    int    nType;
    long   nSize;
    time_t mtime;
    time_t ctime;
};


class CBillQBServer : public CWorkModule
{
public:
    CBillQBServer();   //���캯��
    virtual ~CBillQBServer();  //��������

protected:
    int InitInstance();                     //��ʼ������
    void ExitInstance();                    //��������������
    MODULE_TYPE GetModuleType();            //��ȡ��ģ�����ͺ���
    void OnTimer(const long nTimerID);      //��ʱ���¼�������
    void ParseMsg(MSG_INTRA* pMsgParsed);   //���������ַ�����

    //����������Ϣ������ں���
    void ReqAccessPointProc(MSG_INTRA* pMsgParsed);
    //���󻰵����ʹ�����ں���
    void ReqBillTypeProc(MSG_INTRA* pMsgParsed);
    //����ͨ����Ϣ������ں���
    void ReqChannelProc(MSG_INTRA* pMsgParsed);
    //����ԭʼ��������Ŀ¼������ں���
    void ReqOriginalDateProc(MSG_INTRA* pMsgParsed);
    //�������ջ�������Ŀ¼������ں���
    void ReqFinalDateProc(MSG_INTRA* pMsgParsed);
    //���󻰵��ļ��б�����ں���
    void ReqBillFilesProc(MSG_INTRA* pMsgParsed);
    //���󻰵���ʽ��Ϣ������ں���
    void ReqBillFormatProc(MSG_INTRA* pMsgParsed);
    //�����ѯ����������ں���
    void ReqQueryCondProc(MSG_INTRA* pMsgParsed);
    //���󻰵����������ں���
    void ReqBrowseProc(MSG_INTRA* pMsgParsed);
    //��������������������ں���
    void ReqQueryProc(MSG_INTRA* pMsgParsed);
    //�������������ں���
    void ReqTaskProgressProc(MSG_INTRA* pMsgParsed);
    //��������ҳ��ں���
    void ReqGetPageDataProc(MSG_INTRA* pMsgParsed);
    //���������������ں���
    void ReqStopTaskProc(MSG_INTRA* pMsgParsed);
    //����رմ��ڴ�����ں���
    void ReqCloseTaskProc(MSG_INTRA* pMsgParsed);
    //���󴰿����ִ�����
    void ReqWndShakeHandsProc(MSG_INTRA* pMsgParsed);

    //Added by XDB on 2002.11.11
    //���ӶԲɼ��������������
    void ReqFileRoot(MSG_INTRA* pMsgParsed);

    void ReqBillFiles(MSG_INTRA* pMsgParsed);

    int GetFiles(const char* szPath, LIST<SFILE_STATE>& qResult);
    //End of Add by XDB


protected:
    //����ָ���Ĳ�ѯ�������
    void DestroyTask(CBillQBTask* pTask);
    //ָֹͣ���Ĳ�ѯ�������
    void StopTask(CBillQBTask* pTask);
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
    //����û�л������ݵ�MML����
    void SendEmptyBillDataReport(CParseResult* pParseResult, CBillQBTask* pTask,
                            const char* szNextPageExist,
                            const char* szPageNo, const char* szPercent);

    //��MML����ת����MSG_INTRA����ʽ���ͳ�ȥ
    void SendMMLReport(const char* szReport);
    int GetAPType(const char* szAPName);       //�õ����������Ӧ�Ľ���������
    int GetAPID(const char* szAPName);         //�õ������Ľ�����
    int GetAPMustMerge(const char* szAPName);  //�õ���������Ƿ���Ҫ�ϲ�
    int GetAPFinalSPFileFmt(const char* szAPName);  //�õ������ĺ�����Ƿ��������ʽ
    //�Ի�����ѯ�����������⴦��
    void TransQueryCond(const char* szCond, const char* szBeginDate,
                        const char* szEndDate, char* szExCond);

protected:
    //���ⵥ SWPD05145 YanYong 2004-05-25
	void SendSimpleInfoReport(const char* szCmd, int nRetCode, const char* szInfo, int clientNo);
	void SendMMLReport(const char* szReport, int clientNo);
    //</SWPD05145>
    char  m_szOrigBasePath[MAX_PATH];   //ԭʼ������Ŀ¼
    char  m_szFinalBasePath[MAX_PATH];  //���ջ�����Ŀ¼
    char  m_szQBTmpResultPath[MAX_PATH];//��ѯ��������Ŀ¼
    
    LIST<CBillQBTask* > m_qBrowseTask;  //�����������б�
    LIST<CBillQBTask* > m_qQueryTask;   //��ѯ��������б�
    CIGWB*          m_pFormatObj;       //��ʽ�����ָ��
    IQueryBrowse*   m_pIQueryBrowse;    //��ʽ���ѯ����ӿڶ���ָ��
    UINT4           m_uTimeOutLimit;    //��ѯ�������ʱ����
    long            m_lBaseTimer;       //��׼ʱ��,ʱ�Ӽ��Ϊ1��
    CQBMMLReport*   m_pMMLReport;       //�������MML����Ķ���
    int             m_nCurClientNo;     //��ǰ���ڴ���Ļ���̨��̨��
    LIST<SAPInfo* > m_APInfoList;       //���н����������Ϣ�б�

    UINT4           m_uPageSize;        //ҳ��С
    //���Ӳɼ������ĸ�·��
    char m_szCollectRootPath[MAX_PATH];
    //�Ƿ���ʾ���󻰵�
    BOOL            m_bDisplayErrBill;
    //���ⵥ SWPD05145 YanYong 2004-05-25
    LIST<STRING>    m_QueryFileList;    //��ѯ����ļ���������ʱ���б�
    MAP<int, LIST<STRING> > m_ClientFileListMap; //ÿ���ͻ��˶�Ӧ������
    //</SWPD05145>
};

#endif
