#ifndef _C_PERF_BIN_UTIL_H_
#define _C_PERF_BIN_UTIL_H_

#include "../include/i_perf_util.h"

class CPerfBinUtil : public IPerfUtil
{
public:
    CPerfBinUtil();
    virtual ~CPerfBinUtil();

    //剖析并生成DOM文档树，文件不存在则生成指定文件
    virtual int OpenFile(const char * const szFilePath);

    //关闭文件
    virtual int CloseFile(void);

    //保存文件
    virtual int Flush(void);

    //追加一条性能信息
    virtual int AppendPerfItem(SPerfItem& perfItem);

protected:
    
    FILE*       m_pPerfFile;
    STRING      m_strFileName;
};

#endif //_C_PERF_BIN_UTIL_H_

