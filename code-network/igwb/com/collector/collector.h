/**************************************************************************
 *��Ʒ��    ��iGateway Bill V200
 *ģ����    ���ɼ�ģ��(collector)
 *�ļ���    : colloector.h
 *������Ϣ  �����ļ�����CCollectorModule��ʵ��
 *�汾˵��  ��V200R001i00M00B00D1130A
 *��Ȩ��Ϣ  ����Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      ������
 *����ʱ��  ��2002-10-26
 *�޸ļ�¼  ��
 **************************************************************************/

#ifndef __COLLECTOR_MODULE_H__
#define __COLLECTOR_MODULE_H__

#include "../include/frame.h"
#include "../include/toolbox.h"
#include "collect_task_obj.h"
#include "collect_task_obj_factory.h"
#include "collector_file_pool.h"
#include "resource.h"


class CCollectorModule : public CWorkModule
{
public:
    // ���캯��
    CCollectorModule(void);

    // ��������
    virtual ~CCollectorModule();

    //�ɼ�������
    static char g_szColAppName[MAX_PATH];

protected:

    // ���������ַ�����
    virtual void ParseMsg (MSG_INTRA* pMsgParsed);

    // ��ʼ������
    virtual int InitInstance(void);

    // ��������������
    virtual void ExitInstance(void);

    // ��ȡ��ģ�����ͺ���
    virtual MODULE_TYPE GetModuleType(void);

    virtual void OnTimer(const long nTimerID);

    void Timer5Sec();

    //��ѯָ���ɼ��������Ϣ
    void ProcListColInfo(MSG_INTRA* pMsgParsed);

    //���òɼ������״̬
    void ProcSetColStatus(MSG_INTRA* pMsgParsed);

    //���ø澯����
    void ProcSetALarmLimit(MSG_INTRA* pMsgParsed);

    //��ѯָ��ģ��ļƷ���Ϣ
    void ProcListModuleInfo(MSG_INTRA* pMsgParsed);

    //��ѯ�ɼ��ƻ�
    void ProcListColSch(MSG_INTRA* pMsgParsed);

    //���òɼ��ƻ�
    void ProcSetColSch(MSG_INTRA* pMsgParsed);

    //��ѯ���βɼ��ƻ�
    void ProcListOnceColSch(MSG_INTRA* pMsgParsed);

    //���õ��βɼ��ƻ�
    void ProcSetOnceColSch(MSG_INTRA* pMsgParsed);

    //�ֶ��ɼ��ļ�
    void ProcGetFile(MSG_INTRA* pMsgParsed);

    void SendMMLReport(const char* szReport,
                       int nAppType,
                       int nClientNo);

    //added by chenliangwei
    void OnResumeOver(SCollectConvertResult* pResult);

    void OnResponse(int nConvertResult);

    void SendFileToConvert();

    void Timer1Sec();

    int LoadStatus();

    int SaveStatus();

    //added end


    VECTOR<CCollectTaskObj*>* m_pTaskList;

    long m_lTimerID5Sec;

    enum CO_MD_STATUS
    {
        CM_FAILURE           = -1,
        CM_DEACTIVE          = 0,
        CM_ACTIVE            = 1,
        CM_AUXUPGRADE        = 2
    };

    CO_MD_STATUS  m_nStatus;

    BOOL          m_bDiskFull;

    //added by chenliangwei
    long m_lTimerID1Sec;

    enum CO_CVT_STATUS
    {
        CO_CVT_STATUS_INIT       = 0,
        CO_CVT_STATUS_RESUME     = 1,
        CO_CVT_STATUS_WAIT       = 2,
        CO_CVT_STATUS_CONVERT    = 3
    };

    CO_CVT_STATUS  m_nConvertStatus;

    int    m_nFileNum;

    BOOL   m_bFileIsSended;

    BOOL   m_bFileNeedReSend;      //�ļ��Ƿ���Ҫ���·���
    int    m_nFileReSendTime;      //����ʱ�����
    int    m_nFileReSendInterval;  //����ʱ����

    char  m_szStatusFile[MAX_PATH];
    FILE* m_pStatusFile;


    static CCollectorFilePool* g_pFilePool;
    friend CCollectorFilePool* GetFilePool();
    //added end

};

#endif //__COLLECTOR_MODULE_H__


