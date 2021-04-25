#ifndef __CDR_STAT_SVR_H__
#define __CDR_STAT_SVR_H__

#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/mml_para.h"

#include "stat_cfg_info.h"

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

class CCDRStatCfgMgr;
class CCDRStatRstMgr;
class CCDRStatTask;
class CQBMMLReport;

class CCDRStatSvr : public CWorkModule
{
public:
    //���캯��
    CCDRStatSvr();

    //��������
    virtual ~CCDRStatSvr();

    //��ȡָ��Ŀ¼��������л����ļ�����
    static int GetBillFiles(const char* szPath, LIST<STRING>& qResult);

    static void SetParaInfoContentList(STRING sAPName, INFO_LIST ContentList);

protected:
    //��ʼ������
    int InitInstance();

    //��������������
    void ExitInstance();

    //��ȡ��ģ�����ͺ���
    MODULE_TYPE GetModuleType();

    //��ʱ���¼�������
    void OnTimer(const long nTimerID);

    //���������ַ�����
    void ParseMsg(MSG_INTRA* pMsgParsed);

    //����ͳ����Ŀ��ѯ����
    void ProcListStatItem(MSG_INTRA* pMsgParsed);

    //����ͳ�ƿ�ѡ����Ϣ��ѯ����
    void ProcListQueryPara(MSG_INTRA* pMsgParsed);

    //����ͳ����Ŀ��������
    void ProcSetStatItem(MSG_INTRA* pMsgParsed);

    //����ͳ�������������
    void ProcBrowseStatRst(MSG_INTRA* pMsgParsed);

protected:
    //���ͱ�������ֻ�м򵥵���Ϣ��MML����
    void SendSimpleInfoReport(const char* szCmd, int nRetCode,
                              const char* szInfo);

    //���ͱ�����Ϊһ���������MML����
    void SendHoriTableReport(CParseResult* pParseResult,
                             LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                             const char* szTableHeadName);

    //���ͱ�����Ϊһ���������MML����
    void SendVertTableReport(CParseResult* pParseResult,
                             LIST<STRING>& TitleList, LIST<STRING>& ContentList,
                             LIST<STRING>& RecTitleList);

    void SendMMLReport(const char* szReport);

    //���ִ�ת��Ϊʱ��
    int StrToTm(const char* szSrc, tm &tmTar);

    //���꣬�»���յ�����
    int GetDay(int nYear, int nMonth);

    //��ú����������Ϣ
    int ConvertCycle(int nPeriod,
                     char* szStartTime,
                     char* szEndTime,
                     VECTOR<STRING>& CycleName,
                     VECTOR<time_t>& StartTime,
                     VECTOR<time_t>& EndTime);

    //����������Ϣ
    void ParseCfgInfo(char* szCfgInfo,
                      MAP<STRING, ONESTATSUBITEMCFG_LIST>& mapAPNameStatCfg);

    //�����ϸ������Ϣ����
    void GetCfgItem(char* szStatCfg,
                    char* szApName,
                    char* szItemName,
                    char* szSubItemName,
                    char* szExpress);

protected:
    //��ǰ���ڴ���Ļ���̨��̨��
    int               m_nCurClientNo;
    //ͳ�������ļ������߶���ָ��
    CCDRStatCfgMgr*   m_pCDRStatCfgMgr;
    //ͳ�ƽ���ļ������߶���ָ��
    CCDRStatRstMgr*   m_pCDRStatRstMgr;
    //MML���Ķ���ָ��
    CQBMMLReport*     m_pMMLReport;
    //ͳ�Ʋ�������ָ��
    CCDRStatTask*     m_pCDRStatTask;

    char              m_szBeginTime[20];
    //ͳ����ʼʱ��
    time_t            m_tBeginTime;
    //ͳ�ƽ�ֹʱ��
    time_t            m_tEndTime;
    //�ϴ�ͳ������ʱ��
    time_t            m_tLastStatTime;
    //��ʱ��ID
    long              m_lTimerID;
    //�ӵ���
    UINT4             m_uAPNum;

    //������б�
    LIST<STRING>      m_APNameList;
    //ÿ��������Ӧ�Ŀ�ѡ����Ϣ
    MAP<STRING, INFO_LIST>  m_mapAPToParaField;
    //ÿ��������Ӧ�Ŀ�ѡ����Ϣ
    static MAP<STRING, INFO_LIST>  m_mapAPToParaContent;

    //ÿ��������Ӧ�Ŀ�ѡ����Ϣ��ѯMML���ĵĻ�����
    MAP<STRING, char*>      m_mapAPToParaMem;
    //ÿ��������Ӧ�Ĵ洢ͳ����Ŀ��ѯMML���ĵĻ�����
    MAP<STRING, INFO_LIST>  m_mapAPToStatItemMem;

    STRING m_szInfo;
};
#endif //__CDR_STAT_SVR_H__

