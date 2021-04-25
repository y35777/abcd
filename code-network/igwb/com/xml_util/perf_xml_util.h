#ifndef _C_PERF_XML_UTIL_H_
#define _C_PERF_XML_UTIL_H_

#include "../include/i_perf_util.h"
#include "xml_file_opr.h"

class CPerfXmlUtil : public IPerfUtil
{
public:
    CPerfXmlUtil();
    virtual ~CPerfXmlUtil();

    //����������DOM�ĵ������ļ�������������ָ���ļ�
    virtual int OpenFile(const char * const szFilePath);

    //�ر��ļ�
    virtual int CloseFile(void);

    //�����ļ�
    virtual int Flush(void);

    //׷��һ��������Ϣ
    virtual int AppendPerfItem(SPerfItem& perfItem);

protected:
    
    CXmlFileOpr m_xmlFilePerf; //���ڸ澯�ļ�(.xml)�����Ķ���
};

#endif //_C_PERF_XML_UTIL_H_

