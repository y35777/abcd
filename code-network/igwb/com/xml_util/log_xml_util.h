#ifndef _C_LOG_XML_UTIL_H_
#define _C_LOG_XML_UTIL_H_

#include "../include/i_log_util.h"
#include "xml_file_opr.h"

class CLogXmlUtil : public ILogUtil
{
public:
    CLogXmlUtil();
    virtual ~CLogXmlUtil();

	//剖析并生成DOM文档树，文件不存在则生成指定文件
	virtual int OpenFile(const char * const szFilePath);

    //关闭文件
	virtual int CloseFile(void);

    //获得剩余的日志数
    virtual int GetRemainCount(void);

	//写一条日志信息
	virtual int AppendLog(UINT4       uSerialNo,
                          BYTE        LogType,
                          const char* szUserName,
                          UINT4       uIPAddr,
                          time_t      tLogTime,
                          BYTE        OperResult,
                          const char* szOperCmd,
                          const char* szCmdDesc);

    //读一条日志信息
	virtual int ReadLog(int     whence,
                        UINT4&  uSerialNo,
                        BYTE&   LogType,
                        char*   szUserName,
                        int     nUserNameSize,
                        UINT4&  uIPAddr,
                        time_t& tLogTime,
                        BYTE&   OperResult,
                        char*   szOperCmd,
                        int     nOperCmdSize,
                        char*   szCmdDesc,
                        int     nCmdDescSize);
protected:
    //读一条日志信息
	virtual int ReadOneLog(int     nIndex,
                           UINT4&  uSerialNo,
                           BYTE&   LogType,
                           char*   szUserName,
                           int     nUserNameSize,
                           UINT4&  uIPAddr,
                           time_t& tLogTime,
                           BYTE&   OperResult,
                           char*   szOperCmd,
                           int     nOperCmdSize,
                           char*   szCmdDesc,
                           int     nCmdDescSize);

    CXmlFileOpr m_xmlFileLog; //用于日志文件(.xml)操作的对象
    int         m_nReadIndex;
};

#endif //_C_LOG_XML_UTIL_H_

