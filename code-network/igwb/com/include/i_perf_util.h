#ifndef _I_PERF_UTIL_H_
#define _I_PERF_UTIL_H_

#include "../include/base_type.h"
#include "../include/perf_item.h"

class IPerfUtil
{
public:
    IPerfUtil(){};
    virtual ~IPerfUtil(){};

    //����������DOM�ĵ������ļ�������������ָ���ļ�
    virtual int OpenFile(const char * const szFilePath) = 0;

    //�ر��ļ�
    virtual int CloseFile(void) = 0;

    //�����ļ�
    virtual int Flush(void) = 0;

    //׷��һ��������Ϣ
    virtual int AppendPerfItem(SPerfItem& perfItem) = 0;

};

#endif //_I_PERF_UTIL_H_

