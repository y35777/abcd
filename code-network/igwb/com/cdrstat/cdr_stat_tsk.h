#ifndef __CDR_STAT_TSK_H__
#define __CDR_STAT_TSK_H__

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

#include "ace/Task.h"
#include "stat_cfg_info.h"
#include "../include/format_out.h"

class CCDRStatRstMgr;
class CCDRStatFileGroup;
typedef LIST<CFormat*>   FORMAT_LIST;
//һ�������ÿ����ʽ��Ӧ�ı��ʽ�б�
typedef MAP<CFormat*, INFO_LIST>  FORMAT_MAP_INFOLIST;

//�����ӳ�䵽����ʽ-���ʽ���б�
typedef MAP<STRING, FORMAT_MAP_INFOLIST> AP_MAP_FORMATINFO;


class CCDRStatTask: public ACE_Task_Base
{
public:
    CCDRStatTask();
    virtual ~CCDRStatTask();

public:

    //���г�ʼ������
    int Init(UINT4 nApCount);

    //�����˳���־�����ȴ��˳�
    void WaitForExit(void);

    //������һ��ͳ�Ʋ���
    void ActivateNextStatOper(void);

    //�̴߳�����
    virtual int svc();

    //����ͳ�Ʋ���
    int DoStatOpr(void);

    //����������Ӧ����ʽ-���ʽ�б���ӳ��
    void BuildMap_AP_FMT_EXP(const char* const szAPName);

    //����һ�����Ƿ�����ڿ�ѡ���б���
    BOOL IsExist(STRING sField, LIST<STRING> ParaInfoList);

    //���ݱ��ʽ���Ʋ��Һ����ƶ�Ӧ��������
    int FindIndexFromName(const char* const szExpress, SStatExpIndexList* pIndexList);

    //�����һ�������
    int GetNextDate(char* szDate, time_t& lastdate);

    //�ڿ�ѡ���б���ҿ�ѡ��
    BOOL FindField(const char* const szFieldName, LIST<STRING> FieldList);

    CCDRStatRstMgr* GetCCDRStatRstMgr();

    int StatCDR(SCdrStatTaskInfo* pInfo,
                char* szPath,
                char* szLastDate,
                SStatExpIndexList* pIndexList,
                SAllStatExpRst*&  pAllRst);

protected:
    //ԭʼ������Ŀ¼
    char  m_szOrigBasePath[MAX_PATH];

    //���ջ�����Ŀ¼
    char  m_szFinalBasePath[MAX_PATH];

    char  m_szStatPath[MAX_PATH];

    //��ʽ�����ָ��
    CIGWB*            m_pIGWB;

    //��ʽ���ѯ����ӿڶ���ָ��
    IQueryBrowse*     m_pIQueryBrowse;

    //ͳ�ƽ���ļ������߶���ָ��
    CCDRStatRstMgr*          m_pCDRStatRstMgr;

    //�˳���־
    BOOL                     m_bExit;

    //������Ϣ�б�
    TASKINFO_LIST            m_TaskInfoList;

    //ÿ��������Ӧ�ı��ʽ�б�
    MAP<STRING, INFO_LIST>   m_mapAPToExpress;

    //������Ӧ"��ʽ-���ʽ"��ӳ��
    AP_MAP_FORMATINFO        m_mapAPToFormatExpress;

    //������Ӧ"��ʽ-��ѡ��"��ӳ��
    AP_MAP_FORMATINFO        m_mapAPToFormatParaInfo;

    //������Ӧԭʼ�����ʽӳ��
    MAP<STRING,FORMAT_LIST>  m_mapAPToOrigFormat;

    //ÿ��������Ӧ��ͨ���б�
    MAP<STRING, INFO_LIST>   m_mapAPToChlName;

    MAP<STRING, int>         m_mapChlNameToLen;
};
#endif  //__CDR_STAT_TSK_H__
