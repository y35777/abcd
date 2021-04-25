#ifndef _C_PERF_BIN_UTIL_H_
#define _C_PERF_BIN_UTIL_H_

#include "../include/i_perf_util.h"

class CPerfBinUtil : public IPerfUtil
{
public:
    CPerfBinUtil();
    virtual ~CPerfBinUtil();

    //����������DOM�ĵ������ļ�������������ָ���ļ�
    virtual int OpenFile(const char * const szFilePath);

    //�ر��ļ�
    virtual int CloseFile(void);

    //�����ļ�
    virtual int Flush(void);

    //׷��һ��������Ϣ
    virtual int AppendPerfItem(SPerfItem& perfItem);

protected:
    
    FILE*       m_pPerfFile;
    STRING      m_strFileName;
};

#endif //_C_PERF_BIN_UTIL_H_

