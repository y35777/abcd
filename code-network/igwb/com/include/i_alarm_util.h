#ifndef _I_ALARM_UTIL_H_
#define _I_ALARM_UTIL_H_

#include "../include/base_type.h"
#include "../alarm_util/alarm_config.h"
#include "../alarm_util/alarm_struct.h"

class IAlarmUtil
{
public:
    IAlarmUtil(){};
    virtual ~IAlarmUtil(){};

    //剖析并生成DOM文档树，文件不存在则生成指定文件
    virtual int OpenFile(const char * const szFilePath) = 0;

    //关闭文件
    virtual int CloseFile(void) = 0;

    //保存文件
    virtual int Flush(void) = 0;

    //删除某条告警记录
    virtual int Delete(int nIndex) = 0;

    //获得剩余的告警数
    virtual int GetCount(void) = 0;

    //获得文件头记录
    virtual int ReadHeadElement(UINT4& uCSN) = 0;
    virtual int ReadHeadElementSyn(UINT4& uSYN) = 0;
	virtual int ReadHeadElementMMLBaseID(UINT4& uMMLBaseID) = 0;

    //添加/写文件头记录
    virtual int WriteHeadElement(UINT4 uCSN) = 0;
    virtual int WriteHeadElementSyn(UINT4 uSYN) = 0;
    virtual int WriteHeadElementMMLBaseID(UINT4 uBaseID) = 0;

    //写一条告警信息
    virtual int SaveHisAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm) = 0;

    //读一条告警信息
    virtual int ReadOneAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm) = 0;

};

#endif //_I_ALARM_UTIL_H_

