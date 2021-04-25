#ifndef _I_LOG_UTIL_H_
#define _I_LOG_UTIL_H_

#include "../include/base_type.h"

enum E_LOG_WHENCE
{
    EL_FIRST = 0,   //取第一条日志信息
    EL_NEXT,        //取下一条日志信息
    EL_FINAL        //取最后一条日志信息
};

class ILogUtil
{
public:
    ILogUtil(){};
    virtual ~ILogUtil(){};

	//剖析并生成DOM文档树，文件不存在则生成指定文件
	virtual int OpenFile(const char * const szFilePath) = 0;

    //关闭文件
	virtual int CloseFile(void) = 0;

    //获得剩余的日志数
	virtual int GetRemainCount(void) = 0;

	//写一条日志信息
	virtual int AppendLog(UINT4       uSerialNo,
                          BYTE        LogType,
                          const char* szUserName,
                          UINT4       uIPAddr,
                          time_t      tLogTime,
                          BYTE        OperResult,
                          const char* szOperCmd,
                          const char* szCmdDesc) = 0;

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
                        int     nCmdDescSize) = 0;
};

#endif //_I_LOG_UTIL_H_

