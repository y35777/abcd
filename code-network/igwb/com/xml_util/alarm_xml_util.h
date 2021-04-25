#ifndef _C_ALARM_XML_UTIL_H_
#define _C_ALARM_XML_UTIL_H_

#include "../include/i_alarm_util.h"
#include "xml_file_opr.h"

class CAlarmXmlUtil : public IAlarmUtil
{
public:
    CAlarmXmlUtil();
    virtual ~CAlarmXmlUtil();

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

    //���/д�ļ�ͷ��¼
    virtual int WriteHeadElement(UINT4 uCSN);

    //дһ����־��Ϣ
    virtual int SaveHisAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm);

    //��һ���澯��Ϣ
    virtual int ReadOneAlarm(int            nIndex,
                             SHistoryAlarm& hisAlarm);

    virtual int ReadHeadElementSyn(UINT4& uSYN);
	virtual int ReadHeadElementMMLBaseID(UINT4& uMMLBaseID);
    virtual int WriteHeadElementSyn(UINT4 uSYN);
    virtual int WriteHeadElementMMLBaseID(UINT4 uBaseID);

protected:
    //ת��һ��ʮ�����Ƶ��ַ���Ϊ��ֵ�����"AC"ת����172
    BYTE GetHexCharValue(char chr[2]);

    //����������ʱ�����ʽ
    void ProcessDate(const XMLCh* pChr,
                     UINT2& uYear,
                     BYTE& yMonth,
                     BYTE& yDay,
                     BYTE& yHour,
                     BYTE& yMin,
                     BYTE& ySec);
    
    CXmlFileOpr m_xmlFileAlarm; //���ڸ澯�ļ�(.xml)�����Ķ���
};

#endif //_C_ALARM_XML_UTIL_H_

