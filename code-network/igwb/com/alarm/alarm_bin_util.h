#ifndef _C_ALARM_BIN_UTIL_H_
#define _C_ALARM_BIN_UTIL_H_

#include "../include/i_alarm_util.h"

class CAlarmBinUtil : public IAlarmUtil
{
public:
    CAlarmBinUtil();
    virtual ~CAlarmBinUtil();

    //����������DOM�ĵ������ļ�������������ָ���ļ�
    virtual int OpenFile(const char * const szFilePath);

    //�ر��ļ�
    virtual int CloseFile(void);

    //�����ļ�
    virtual int Flush(void);

    //ɾ��ĳ���澯��¼
    virtual int Delete(int nIndex);

    //���ʣ��ĸ澯��
    virtual int GetCount(void);

    //����ļ�ͷ��¼
    virtual int ReadHeadElement(UINT4& uCSN);
    virtual int ReadHeadElementSyn(UINT4& uSYN);
	virtual int ReadHeadElementMMLBaseID(UINT4& uMMLBaseID);

    //���/д�ļ�ͷ��¼
    virtual int WriteHeadElement(UINT4 uCSN);
    virtual int WriteHeadElementSyn(UINT4 uSYN);
    virtual int WriteHeadElementMMLBaseID(UINT4 uBaseID);

    //дһ����־��Ϣ
    virtual int SaveHisAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm);

    //��һ���澯��Ϣ
    virtual int ReadOneAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm);

protected:
    FILE*       m_pAlarmFile;
    STRING      m_strFileName;
};

#endif //_C_ALARM_BIN_UTIL_H_

