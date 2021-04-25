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

    //初始化
    virtual int Init();

    //通过消息帧判断是否是同一任务
    virtual BOOL IsMatchedTask(MSG_INTRA* pMsg);

    //将输入的字符串转换为tm结构
    int StrToTm(const char* szSrc, tm &tmTar);

    //判断话单是否满足接入码及业务键条件的接口
    BOOL IsMatchedScpBill(BYTE* pData, int nLen);

protected:

    //实施特定操作
    virtual void ProcBillTask();

    //对一条异常话单恢复请求进行恢复
    void ProcOneQueryTask(FILE* fDstFile) throw(int);

    //当前话单恢复请求
    SSCPBillRcvrApply   m_ScpApply;
		
    //保存话单恢复命令的有关信息，返回报告时用
    SRcvrCmd            m_RcvrCmd;

    //异常话单内容
    SEcptBillRec        m_EcptBillRec;

    //FTP客户端对象
    CFtpClient          m_FtpClient;	

    //接入码、业务键列表
    LIST<SCodeKey>      m_CodeKeyList;

    //格式库对象指针
    CIGWB*              m_pIGWB;

    //格式库对象的查询浏览接口指针
    IQueryBrowse*       m_pIQueryBrowse;

    //查询格式对象指针
    CFormat*            m_pFormat;

    //接入点号
    int                 m_nAP;

    //m_nAP接入点下后存盘中智能话单通道的路径
    STRING              m_strBackChlPath;

    //所用的格式库名称
    STRING              m_strFormatName;

    //本次SCP话单恢复任务在本地的工作目录全路径名
    STRING              m_strSCPRvyRoot;

    //SCP异常话单放在本地的全路径名
    STRING              m_strSCPErrBill;

    //SCP恢复话单放在本地的全路径名，文件名约定格式：OMCSSPNoyyyymmdd.seqNo
    STRING              m_strSCPRvyBill;

	STRING              m_strLocalIp;
};

#endif //__SCP_RVY_TASK_H__
