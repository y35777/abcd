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

    //����������DOM�ĵ������ļ�������������ָ���ļ�
    virtual int OpenFile(const char * const szFilePath) = 0;

    //�ر��ļ�
    virtual int CloseFile(void) = 0;

    //�����ļ�
    virtual int Flush(void) = 0;

    //ɾ��ĳ���澯��¼
    virtual int Delete(int nIndex) = 0;

    //���ʣ��ĸ澯��
    virtual int GetCount(void) = 0;

    //����ļ�ͷ��¼
    virtual int ReadHeadElement(UINT4& uCSN) = 0;
    virtual int ReadHeadElementSyn(UINT4& uSYN) = 0;
	virtual int ReadHeadElementMMLBaseID(UINT4& uMMLBaseID) = 0;

    //���/д�ļ�ͷ��¼
    virtual int WriteHeadElement(UINT4 uCSN) = 0;
    virtual int WriteHeadElementSyn(UINT4 uSYN) = 0;
    virtual int WriteHeadElementMMLBaseID(UINT4 uBaseID) = 0;

    //дһ���澯��Ϣ
    virtual int SaveHisAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm) = 0;

    //��һ���澯��Ϣ
    virtual int ReadOneAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm) = 0;

};

#endif //_I_ALARM_UTIL_H_

