#ifndef _I_PERF_UTIL_H_
#define _I_PERF_UTIL_H_

#include "../include/base_type.h"
#include "../include/perf_item.h"

class IPerfUtil
{
public:
    IPerfUtil(){};
    virtual ~IPerfUtil(){};

    //剖析并生成DOM文档树，文件不存在则生成指定文件
    virtual int OpenFile(const char * const szFilePath) = 0;

    //关闭文件
    virtual int CloseFile(void) = 0;

    //保存文件
    virtual int Flush(void) = 0;

    //追加一条性能信息
    virtual int AppendPerfItem(SPerfItem& perfItem) = 0;

};

#endif //_I_PERF_UTIL_H_

