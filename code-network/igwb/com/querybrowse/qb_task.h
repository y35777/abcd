#ifndef __BILLQBTASK_H__
#define __BILLQBTASK_H__

#include "../include/precom.h"
#include "../include/frame.h"
#include "../include/format_out.h"
#include "file_group.h"
#include "qb_result.h"

class CBillQBTask
{
public:
    //���캯��
    CBillQBTask(UINT2 uDeskNo, int nWndNo, UINT4 uPageNo,
                char* szBillFormat, UINT4 uBillType, char* szResultPath);
    virtual ~CBillQBTask ();     //��������

    static void SetPageSize(UINT4 uPageSize);
    BOOL IsRunning();            //�߳��Ƿ���������
    const char* GetErrInfo();    //�õ����̵߳Ĵ�����Ϣ

    //��ʼ���������ú����������������
    BOOL Init(char* szPathFile, UINT4 uAPType, int nAPID);
    //��ʼ���������ú�����Բ�ѯ�������
    BOOL Init(char* szPath, char* szStartRange, char* szEndRange,
              UINT4 uFlg, char* szQueryCond, UINT4 uAPType, int nAPID);
    
    //���ⵥ SWPD05145 YanYong 2004-05-25 ��Ӹ÷���
    BOOL Init(char* szPath, LIST<STRING> & queryList,
              UINT4 uFlg, char* szQueryCond, UINT4 uAPType, int nAPID);
    
    BOOL Run();      //׼����������ҳ�߳���������
    //��û������ı�����б�
    void GetBillTitleList(LIST<STRING>& TitleList);
    //��ȡָ��ҳ�Ļ�����¼����
    int GetPageData(int nPageNo, LIST<STRING>& qResult);

    BOOL IsValidPage(int nPageNo);    //�ж�һ������ҳ���Ƿ�Ϸ�
    //�ö����Ƿ�ƥ��ָ���Ĳ�ѯ�������
    BOOL IsMatched(UINT2 uDeskNo, int nWndNo);
    int WaitThreadExit();       //�ȴ�׼�������߳̽�������

    void Stop();                //����ֹͣ��־Ϊֹͣ״̬����
    void IncTimeOutFlg();       //��ʱ��־����һ����
    void RsetTimeOutFlg();      //��ʱ��־���㺯��
    BOOL IsTimeOut(UINT4 uTimeOutLimit);  //�Ƿ�ʱ����
    void SetMustMerge(BOOL bMustMerge);
    void SetFinalSPFileFmt(BOOL bFinalSPFileFmt);
    //��ò�ѯ������Ľ���
    BOOL GetProcess(UINT4& nRecordCount, int& nPercent, int& nCurPage);
    void SetDisplayErrBill(BOOL bDisplayErrBill);
protected:
    //׼����������ҳ�߳���ں���
    static void ThreadProc(CBillQBTask *pBillQBTask);
    //׼����������ҳ�̺߳���
    void PrepareBillPages();

protected:
    UINT2               m_uDeskNo;            //����ҵ��̨��
    int                 m_nWndNo;             //��ѯ������񴰿ں�
    UINT4               m_uBillType;          //�����ļ�����
    UINT4               m_uAPType;

    CFileGroup*         m_pFileGroup;         //�����ļ������ָ��
    CIGWB*              m_pFormatObj;         //��ʽ�����ָ��
    IQueryBrowse*       m_pIQueryBrowse;      //��ʽ�����Ĳ�ѯ����ӿ�ָ��
    CFormat*            m_pFormat;            //�����ʽ����ָ��
    char                m_szFormatName[100];  //�����ʽ������
    UINT4               m_uTimeOut;           //����δ�յ���Ϣ��ʱ�䣬��ʼ��Ϊ0

    static UINT4        m_uPageSize;     //һҳ�����Ļ�����¼����������Ĭ��Ϊ50

    ACE_Thread_Manager  m_tThreadMgr;    //�̹߳����Ա����
    BOOL                m_bRunning;      //���߳��Ƿ��������б�־
    char                m_szErrInfo[2000];//���̵߳��쳣��Ϣ
                                         //���߳��쳣�˳�ʱ��Ч
    UINT4               m_uObjType;      //����������ͣ����Ͷ�����TaskType��

    CBillQBResult*      m_pResult;     //��ѯ������������ָ��

    ACE_Mutex           m_Mutex;       //��ʽ�����ָ��

    char                m_pResultPath[MAX_PATH];
    //�Ƿ���ʾ���󻰵�
    BOOL                m_bDisplayErrBill;
};

#endif
