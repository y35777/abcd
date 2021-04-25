#ifndef _C_ALARM_BIN_UTIL_H_
#define _C_ALARM_BIN_UTIL_H_

#include "../include/i_alarm_util.h"

class CAlarmBinUtil : public IAlarmUtil
{
public:
    CAlarmBinUtil();
    virtual ~CAlarmBinUtil();

    //剖析并生成DOM文档树，文件不存在则生成指定文件
    virtual int OpenFile(const char * const szFilePath);

    //关闭文件
    virtual int CloseFile(void);

    //保存文件
    virtual int Flush(void);

    //删除某条告警记录
    virtual int Delete(int nIndex);

    //获得剩余的告警数
    virtual int GetCount(void);

    //获得文件头记录
    virtual int ReadHeadElement(UINT4& uCSN);
    virtual int ReadHeadElementSyn(UINT4& uSYN);
	virtual int ReadHeadElementMMLBaseID(UINT4& uMMLBaseID);

    //添加/写文件头记录
    virtual int WriteHeadElement(UINT4 uCSN);
    virtual int WriteHeadElementSyn(UINT4 uSYN);
    virtual int WriteHeadElementMMLBaseID(UINT4 uBaseID);

    //写一条日志信息
    virtual int SaveHisAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm);

    //读一条告警信息
    virtual int ReadOneAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm);

protected:
    FILE*       m_pAlarmFile;
    STRING      m_strFileName;
};

#endif //_C_ALARM_BIN_UTIL_H_

