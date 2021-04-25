#ifndef _C_PERF_XML_UTIL_H_
#define _C_PERF_XML_UTIL_H_

#include "../include/i_perf_util.h"
#include "xml_file_opr.h"

class CPerfXmlUtil : public IPerfUtil
{
public:
    CPerfXmlUtil();
    virtual ~CPerfXmlUtil();

    //剖析并生成DOM文档树，文件不存在则生成指定文件
    virtual int OpenFile(const char * const szFilePath);

    //关闭文件
    virtual int CloseFile(void);

    //保存文件
    virtual int Flush(void);

    //追加一条性能信息
    virtual int AppendPerfItem(SPerfItem& perfItem);

protected:
    
    CXmlFileOpr m_xmlFilePerf; //用于告警文件(.xml)操作的对象
};

#endif //_C_PERF_XML_UTIL_H_

