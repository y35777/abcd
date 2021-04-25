#ifndef __SCP_RVY_TASK_H__
#define __SCP_RVY_TASK_H__

#include "bill_task.h"
#include "../toolbase/ftp_client.h"

class CIGWB;
class IQueryBrowse;
class CFormat;
class CScpRvyTask : public CBillTask
{
public:
    CScpRvyTask(int nAP, 
                const SSCPBillRcvrApply* pApply, 
                const SRcvrCmd* pCmd, 
                int nTaskID);
    virtual ~CScpRvyTask();

    //��ʼ��
    virtual int Init();

    //ͨ����Ϣ֡�ж��Ƿ���ͬһ����
    virtual BOOL IsMatchedTask(MSG_INTRA* pMsg);

    //��������ַ���ת��Ϊtm�ṹ
    int StrToTm(const char* szSrc, tm &tmTar);

    //�жϻ����Ƿ���������뼰ҵ��������Ľӿ�
    BOOL IsMatchedScpBill(BYTE* pData, int nLen);

protected:

    //ʵʩ�ض�����
    virtual void ProcBillTask();

    //��һ���쳣�����ָ�������лָ�
    void ProcOneQueryTask(FILE* fDstFile) throw(int);

    //��ǰ�����ָ�����
    SSCPBillRcvrApply   m_ScpApply;
		
    //���滰���ָ�������й���Ϣ�����ر���ʱ��
    SRcvrCmd            m_RcvrCmd;

    //�쳣��������
    SEcptBillRec        m_EcptBillRec;

    //FTP�ͻ��˶���
    CFtpClient          m_FtpClient;	

    //�����롢ҵ����б�
    LIST<SCodeKey>      m_CodeKeyList;

    //��ʽ�����ָ��
    CIGWB*              m_pIGWB;

    //��ʽ�����Ĳ�ѯ����ӿ�ָ��
    IQueryBrowse*       m_pIQueryBrowse;

    //��ѯ��ʽ����ָ��
    CFormat*            m_pFormat;

    //������
    int                 m_nAP;

    //m_nAP������º���������ܻ���ͨ����·��
    STRING              m_strBackChlPath;

    //���õĸ�ʽ������
    STRING              m_strFormatName;

    //����SCP�����ָ������ڱ��صĹ���Ŀ¼ȫ·����
    STRING              m_strSCPRvyRoot;

    //SCP�쳣�������ڱ��ص�ȫ·����
    STRING              m_strSCPErrBill;

    //SCP�ָ��������ڱ��ص�ȫ·�������ļ���Լ����ʽ��OMCSSPNoyyyymmdd.seqNo
    STRING              m_strSCPRvyBill;

	STRING              m_strLocalIp;
};

#endif //__SCP_RVY_TASK_H__
