#ifndef _C_ALARM_XML_UTIL_H_
#define _C_ALARM_XML_UTIL_H_

#include "../include/i_alarm_util.h"
#include "xml_file_opr.h"

class CAlarmXmlUtil : public IAlarmUtil
{
public:
    CAlarmXmlUtil();
    virtual ~CAlarmXmlUtil();

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

    //添加/写文件头记录
    virtual int WriteHeadElement(UINT4 uCSN);

    //写一条日志信息
    virtual int SaveHisAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm);

    //读一条告警信息
    virtual int ReadOneAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm);

    virtual int ReadHeadElementSyn(UINT4& uSYN);
	virtual int ReadHeadElementMMLBaseID(UINT4& uMMLBaseID);
    virtual int WriteHeadElementSyn(UINT4 uSYN);
    virtual int WriteHeadElementMMLBaseID(UINT4 uBaseID);

protected:
    //转换一个十六进制的字符串为数值，如把"AC"转换成172
    BYTE GetHexCharValue(char chr[2]);

    //处理年月日时分秒格式
    void ProcessDate(const XMLCh* pChr,
                     UINT2& uYear,
                     BYTE& yMonth,
                     BYTE& yDay,
                     BYTE& yHour,
                     BYTE& yMin,
                     BYTE& ySec);
    
    CXmlFileOpr m_xmlFileAlarm; //用于告警文件(.xml)操作的对象
};

#endif //_C_ALARM_XML_UTIL_H_

