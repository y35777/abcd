#ifndef __I_STAT_CFG_RST_H__
#define __I_STAT_CFG_RST_H__

#include "stat_cfg_info.h"

class ICDRStatRst
{
public:
    ICDRStatRst(){};
    virtual  ~ICDRStatRst(){};
public:
    //���ò��������г�ʼ������
    virtual int Init(UINT4 uAccessPoint) = 0;

    //���ͳ�ƽ���ļ�ͷ��Ϣ
    virtual void GetStatRstHead(SStatRstHead& statRstHead) = 0;

    //�������ͳ���������ʽ
    virtual SStatExpIndexList* GetStatExpList(void) = 0;

    //���ָ��ʱ�䷶Χ��ͳ���������ʽ�Ľ��
    virtual int GetStatExpRst(const char* const szStatExpress, time_t tBeginTime,
                            time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal) = 0;

    //���ͳ���������ʽ
    virtual int AddStatExpress(LIST<char* >& StatExpList) = 0;

    //���һ�������ͳ�Ʊ��ʽ��ͳ�ƽ��
    virtual int AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime) = 0;

    //���ؽ�����
    virtual UINT4 GetAccessPoint() = 0;

    //]���ؽ��������
    virtual const char* GetAccessPointName() = 0;

    //���ݽ����ŷ������б��ʽ
    virtual void  GetAllExpress(LIST<STRING>& ExpressList) = 0;

    //����ϴ�ͳ��ʱ��
    virtual time_t GetLastStatTime() = 0;
    
    //ˢ����ִ��ͳ�Ʋ�����ʱ��
    virtual int SetLastStatTime(time_t curtime) = 0;
};


#endif //__I_STAT_CFG_RST_H__
