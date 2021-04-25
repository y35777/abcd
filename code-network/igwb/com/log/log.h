/**************************************************************************
 *产品名    ：iGateway Bill V200        
 *模块名    ：日志模块(Log)
 *文件名    : log.h
 *描述信息  ：本文件是类CLog的实现
 *版本说明  ：V200R001i00M00B00D1130A   
 *版权信息  ：版权所有（C）2001-2002 华为技术有限公司
 *作者      ：方卫华
 *创建时间  ：2001-10-30
 *修改记录  ：
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


// 一条日志转换成MML报文格式字符串后的最大长度
#define MAX_LOG_LEN  640
#define LOG_REP_MORE 0
#define LOG_REP_NONE 1
#define LOG_REP_ERR  -1
// MML报文最大长度
#define MAX_MML_LEN  4096   //???还不确定是多少
#define MAX_HOST_LEN 24

class CLog : public CWorkModule
{
public:
    // 构造函数
    CLog ( void );

    // 析构函数
    virtual ~CLog ();

	static STRING MakePath(const STRING& path,const char *filename);

private:
	void InitArchiveTime(const char* szTime);
	void AutoArchiveLog();
protected:
	void GetLogArchiveTime(MSG_INTRA *pMsg);
    // 获得日志文件的后缀名
    inline const char* GetLogFileExt ( void )
    {
        if(m_bUseXml)
        {
            return ".xml";
        }

        return ".log";
    }

    // 获得日志文件的读写封装对象
    inline ILogUtil* CreateLogUtilObj ( void )
    {
        if(m_bUseXml)
        {
            return m_logUtilEncap.CreateLogUtil();
        }
        
        return new CLogBinUtil();
    }
    
    // 打开日志文件
    BOOL OpenWriteFile ( void );

    // 关闭日志文件
    void CloseWriteFile ( void );
    
    // 保存日志
    BOOL WriteLog ( SLog *pBuf );
    
    // 分析查询要求，实施查询操作
    void QueryLog ( MSG_INTRA* pMsg );
    
    // 删除指定天数前的日志
    BOOL DeleteOutdatedLog ( void );
    
	// 设置日志自动删除期限
	void SetLogSavingDays(MSG_INTRA* pMsg);
    void GetLogSavingDays(MSG_INTRA* pMsg);
	// 手工删除日志
	void RemoveLogByClient(MSG_INTRA* pMsg);

    // 手工归档日志
	void ArchiveLog(MSG_INTRA* pMsg);

	// 设置日志自动归档时间
	void SetLogArchiveTime(MSG_INTRA* pMsg);

    //导出日志
    void ExportLog(MSG_INTRA* pMsg);
    
    //上载日志
    void UploadLog(MSG_INTRA* pMsg);

    // 获取当前文件序列号
    UINT4 GetCurrentSN ( void );
        
    // 从一个日志文件里获取日志序列号
    BOOL GetSNFromFile ( const char * szPathFile, UINT4& uSN );
    
    // 操作日志查询处理函数，返回MML报文个数
    UINT4 QueryOperLog(tm *pStartTime, 
                       tm *pEndTime,
                       const char *pUserName, 
                       LIST <STRING>& qResult );
    
    // 系统日志查询处理函数，返回MML报文个数
    UINT4 QuerySysLog(tm *pStartTime, 
                      tm *pEndTime, 
                      LIST <STRING>& qResult );
    //Query Log implement
    UINT4 QueryLog_impl(tm *pStartTime, 
                       tm *pEndTime, 
                       LIST <STRING>& qResult,
                       const char *pUserName = NULL);
    // 命令解析与分发函数
    virtual void ParseMsg ( MSG_INTRA* pMsgParsed );  
    
    // 初始化函数
    virtual int InitInstance (void );
    
    // 结束工作处理函数
    virtual void ExitInstance ( void );
    
    // 获取本模块类型函数
    virtual MODULE_TYPE GetModuleType ( void );
    
    // 获取日志根目录下所有日志文件
    int GetLogFileList ( LIST<STRING>& LogFileList );
    
    // 将二进制日志信息转换为MML报文格式
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
    //发送MML报文
	int SendMsgToMML(const char *pszMMLReport, MSG_INTRA *pFrame);
    void SendSimpleReport(const char* szCmd, int nRetCode, const char* szInfo,MSG_INTRA *pMsg);

	virtual void OnTimer(const long nTimerID);
	CMMLReport*     m_pReport;
    time_t          m_tLastDay;     //上次写日志的时间
    UINT4           m_uCurrentSN;   //当前已用过的日志序列号，超过无符号长整上限，自动回绕
	ILogUtil*       m_pLogUtil;     //用于日志文件(.xml)操作的对象
    UINT2           m_uSavedDays;   //日志保留天数
    STRING          m_szBasePath;   //日志文件存放的根路径
    CLogUtilEncap   m_logUtilEncap;
    BOOL            m_bUseXml;      //是否用XML格式
	ILogUtil*       m_pQueryLogUtil;//用于查询日志文件操作的对象

    STRING          m_szXmlLogName; //打包后的日志名称
    
	STRING          m_szArchDir;		// 日志归档路径
    STRING          m_strError;      // 日志错误信息
	//日志自动归档时间
	long            m_lTimerID5Sec;	//日志归档定时器
    long            m_lTimerID1Day;    //日志删除定时器
	tm	            m_tArchive;	
    int             m_nPageLimit;
    // 获取日志根目录下日志文件名列表
    LIST<STRING>    m_LogFileList;
    //初始化标志
    bool            m_bFirst;
    bool            m_bSameFile;
    //查询结果Cnt
    int             m_nQueryCount;
    //当前错误状态
    int             m_nNetStatus;
};

#endif /* __LOG_H__ */


