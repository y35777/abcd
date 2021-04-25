/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200        
 *ģ����    ����־ģ��(Log)
 *�ļ���    : log.h
 *������Ϣ  �����ļ�����CLog��ʵ��
 *�汾˵��  ��V200R001i00M00B00D1130A   
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ��������
 *����ʱ��  ��2001-10-30
 *�޸ļ�¼  ��
 **************************************************************************/

#ifndef __LOG_H__
#define __LOG_H__

#include "../include/frame.h"
#include "../include/toolbox.h"
#include "resource.h"

#include "../include/i_log_util.h"
#include "../utility/mml_para.h"
#include "../utility/mml_report.h"
#include "log_encap.h"
#include "log_bin_util.h"


// һ����־ת����MML���ĸ�ʽ�ַ��������󳤶�
#define MAX_LOG_LEN  640
#define LOG_REP_MORE 0
#define LOG_REP_NONE 1
#define LOG_REP_ERR  -1
// MML������󳤶�
#define MAX_MML_LEN  4096   //???����ȷ���Ƕ���
#define MAX_HOST_LEN 24

class CLog : public CWorkModule
{
public:
    // ���캯��
    CLog ( void );

    // ��������
    virtual ~CLog ();

	static STRING MakePath(const STRING& path,const char *filename);

private:
	void InitArchiveTime(const char* szTime);
	void AutoArchiveLog();
protected:
	void GetLogArchiveTime(MSG_INTRA *pMsg);
    // �����־�ļ��ĺ�׺��
    inline const char* GetLogFileExt ( void )
    {
        if(m_bUseXml)
        {
            return ".xml";
        }

        return ".log";
    }

    // �����־�ļ��Ķ�д��װ����
    inline ILogUtil* CreateLogUtilObj ( void )
    {
        if(m_bUseXml)
        {
            return m_logUtilEncap.CreateLogUtil();
        }
        
        return new CLogBinUtil();
    }
    
    // ����־�ļ�
    BOOL OpenWriteFile ( void );

    // �ر���־�ļ�
    void CloseWriteFile ( void );
    
    // ������־
    BOOL WriteLog ( SLog *pBuf );
    
    // ������ѯҪ��ʵʩ��ѯ����
    void QueryLog ( MSG_INTRA* pMsg );
    
    // ɾ��ָ������ǰ����־
    BOOL DeleteOutdatedLog ( void );
    
	// ������־�Զ�ɾ������
	void SetLogSavingDays(MSG_INTRA* pMsg);
    void GetLogSavingDays(MSG_INTRA* pMsg);
	// �ֹ�ɾ����־
	void RemoveLogByClient(MSG_INTRA* pMsg);

    // �ֹ��鵵��־
	void ArchiveLog(MSG_INTRA* pMsg);

	// ������־�Զ��鵵ʱ��
	void SetLogArchiveTime(MSG_INTRA* pMsg);

    //������־
    void ExportLog(MSG_INTRA* pMsg);
    
    //������־
    void UploadLog(MSG_INTRA* pMsg);

    // ��ȡ��ǰ�ļ����к�
    UINT4 GetCurrentSN ( void );
        
    // ��һ����־�ļ����ȡ��־���к�
    BOOL GetSNFromFile ( const char * szPathFile, UINT4& uSN );
    
    // ������־��ѯ������������MML���ĸ���
    UINT4 QueryOperLog(tm *pStartTime, 
                       tm *pEndTime,
                       const char *pUserName, 
                       LIST <STRING>& qResult );
    
    // ϵͳ��־��ѯ������������MML���ĸ���
    UINT4 QuerySysLog(tm *pStartTime, 
                      tm *pEndTime, 
                      LIST <STRING>& qResult );
    //Query Log implement
    UINT4 QueryLog_impl(tm *pStartTime, 
                       tm *pEndTime, 
                       LIST <STRING>& qResult,
                       const char *pUserName = NULL);
    // ���������ַ�����
    virtual void ParseMsg ( MSG_INTRA* pMsgParsed );  
    
    // ��ʼ������
    virtual int InitInstance (void );
    
    // ��������������
    virtual void ExitInstance ( void );
    
    // ��ȡ��ģ�����ͺ���
    virtual MODULE_TYPE GetModuleType ( void );
    
    // ��ȡ��־��Ŀ¼��������־�ļ�
    int GetLogFileList ( LIST<STRING>& LogFileList );
    
    // ����������־��Ϣת��ΪMML���ĸ�ʽ
    int PrintLog(char *buf, SLog *pLog);

	void GetTimeValue(CParseResult *pParaResult, 
                      int paraID, 
                      tm *pDate, 
                      tm *pDefault);

	void GetDateValue(CParseResult *pParaResult, 
                      int paraID, 
                      tm *pDate, 
                      tm *pDefault);

	int GetIntValue(CParseResult *pParaResult, 
                    int paraID, 
                    int nDefault);

	void MakeTitle();
    void SendErrorReport(const char *szErrInfo,MSG_INTRA *pMsg,int nErrno = 1);
    void AssembleReport(int nMode,LIST<STRING> &retMML,MSG_INTRA *pMsg);
    //����MML����
	int SendMsgToMML(const char *pszMMLReport, MSG_INTRA *pFrame);
    void SendSimpleReport(const char* szCmd, int nRetCode, const char* szInfo,MSG_INTRA *pMsg);

	virtual void OnTimer(const long nTimerID);
	CMMLReport*     m_pReport;
    time_t          m_tLastDay;     //�ϴ�д��־��ʱ��
    UINT4           m_uCurrentSN;   //��ǰ���ù�����־���кţ������޷��ų������ޣ��Զ�����
	ILogUtil*       m_pLogUtil;     //������־�ļ�(.xml)�����Ķ���
    UINT2           m_uSavedDays;   //��־��������
    STRING          m_szBasePath;   //��־�ļ���ŵĸ�·��
    CLogUtilEncap   m_logUtilEncap;
    BOOL            m_bUseXml;      //�Ƿ���XML��ʽ
	ILogUtil*       m_pQueryLogUtil;//���ڲ�ѯ��־�ļ������Ķ���

    STRING          m_szXmlLogName; //��������־����
    
	STRING          m_szArchDir;		// ��־�鵵·��
    STRING          m_strError;      // ��־������Ϣ
	//��־�Զ��鵵ʱ��
	long            m_lTimerID5Sec;	//��־�鵵��ʱ��
    long            m_lTimerID1Day;    //��־ɾ����ʱ��
	tm	            m_tArchive;	
    int             m_nPageLimit;
    // ��ȡ��־��Ŀ¼����־�ļ����б�
    LIST<STRING>    m_LogFileList;
    //��ʼ����־
    bool            m_bFirst;
    bool            m_bSameFile;
    //��ѯ���Cnt
    int             m_nQueryCount;
    //��ǰ����״̬
    int             m_nNetStatus;
};

#endif /* __LOG_H__ */


