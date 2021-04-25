#ifndef __STAT_CFG_RST_H__
#define __STAT_CFG_RST_H__

#include "stat_cfg_info.h"
#include "i_cdr_stat_rst.h"

class CCDRStatRst: public ICDRStatRst
{
public:
    CCDRStatRst();
    virtual ~CCDRStatRst();

public:

     //���ò��������г�ʼ������
    virtual int Init(UINT4 uAccessPoint);    //������

    //���ͳ�ƽ���ļ�ͷ��Ϣ
    virtual void GetStatRstHead(SStatRstHead& statRstHead);

    //�������ͳ���������ʽ
    virtual SStatExpIndexList* GetStatExpList(void);

    //���ָ��ʱ�䷶Χ��ͳ���������ʽ�Ľ��
    virtual int GetStatExpRst(const char* const szStatExpress,
                              time_t tBeginTime,
                              time_t tEndTime,
                              UINT4& nCDRCount,
                              UINT4& nValueTotal);

    //���ͳ���������ʽ
    virtual int AddStatExpress(LIST<char* >& StatExpList);

    //���һ�������ͳ�Ʊ��ʽ��ͳ�ƽ��
    virtual int AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime);

    //����ͳ�ƽ���ļ��в���ʹ�õ��������ʽ
    virtual int CleanStatExp(void);

    //���ؽ�����
    virtual UINT4 GetAccessPoint();

    //]���ؽ��������
    virtual const char* GetAccessPointName();

    //���ݽ����ŷ������б��ʽ
    virtual void GetAllExpress(LIST<STRING>& ExpressList);

    //����ϴ�ͳ��ʱ��
    virtual time_t GetLastStatTime();

    //ˢ����ִ��ͳ�Ʋ�����ʱ��
    virtual int SetLastStatTime(time_t curtime);

protected:

    //���ݱ��ʽ�ڻ������в�����λ��
    BOOL FindStatExpress(char* szExpress, UINT4& nPosition);

    //��������г���Ҫ��ع�
    BOOL RollBack(void);

    //��һ���ͳ�ƽ������������ò���ĳͳ�Ʊ��ʽ��ͳ�ƽ��
    void FindStatRst(SAllStatExpRst* pAllRst, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal);

protected:

    //������
    UINT4    m_uAccessPoint;
    //���������
    STRING   m_sAPName;
    //ͳ�������ļ�����·����
    char     m_szFileName[MAX_PATH];
    //ͳ�ƽ����Ϣ������ָ��
    BYTE*    m_pMemData;
    //ͳ�ƽ����Ϣ����������
    UINT4    m_nMemDataLen;

    BYTE*    m_pHead;
};

#endif //__STAT_CFG_RST_H__
