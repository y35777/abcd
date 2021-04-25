#ifndef __LINK_BASE_H__
#define __LINK_BASE_H__

#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "file_protocol.h"
#include <assert.h>

class CLinkBase
{
public:
    CLinkBase(CFileProtocol*    fp,
              const char* const szHost,
              const char* const szUser,
              const char* const szPwd,
              int nAlarmPara)
    {
        assert((fp != NULL) && (szHost != NULL) && (szUser != NULL) && (szPwd != NULL));

        m_fp = fp;

        SetRemoteHostMsg(szHost, szUser, szPwd);

        m_nAlarmPara = nAlarmPara;

        m_szLocalUser[0] = '\0';           
        m_szLocalPwd[0] = '\0';            
        m_szLocalHostName[0] = '\0';
    }

    virtual ~CLinkBase()
    {
        if(m_fp != NULL)
        {
            delete m_fp;
            m_fp = NULL;
        }
    }

    //��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
    virtual int  Init()
    {
        return m_fp->InitProtocol();
    }

    //�����ļ������
    virtual int  Connect()
    {
        return m_fp->Connect();
    }

    //�Ͽ�����
    virtual int  Disconnect()
    {
        return m_fp->Disconnect();
    }

    //�����ļ�����ģʽ
    virtual int  SetMode(int nMode)
    {
        return m_fp->SetMode(nMode);
    }

    //�����ܷ��������ӶԶ��ļ�������, �麯��, �����������ʵ��
    virtual int  TestLink()
    {
        int nRet = ERR_FAIL;
        if(m_fp != NULL)
        {
            nRet = m_fp->TryConnect();
        }

        return nRet;
    }

    //ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual int  GetFile(const SGetFileReq &pReq)
    {
        return m_fp->GetFile(pReq);
    }

    //ȡ�ļ�����˵�ָ���ļ��ĳ���
    virtual int  GetFileLen(const char* szFileName,
                            unsigned long& uFileLen)
    {
        return m_fp->GetFileLen(szFileName, uFileLen);
    }

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual int  GetFileList(const char* const szSrcDir, SListOut &pOut)
    {
        return m_fp->GetFileList(szSrcDir, pOut);
    }

    //ɾ���ļ�����˵�ָ���ļ�
    virtual int  DelFile(const char* const szFileName)
    {
        return m_fp->DelFile(szFileName);
    }

    //������ָ���ļ�
    virtual int  RenameFile(const char *szOld, const char *szNew)
    {
        return m_fp->RenameFile(szOld, szNew);
    }

    //�õ����һ���ļ���������Ĵ�����Ϣ
    const char*  GetLastErr()
    {
        return m_szErrInfo;
    }

    BOOL GetbFailure()
    {
        return m_bFailure;
    }

    int GetAlarmPara()
    {
        return m_nAlarmPara;
    }

    const char* GetLocalHost()
    {
        return m_szLocalHostName;
    }

    const char* GetRemoteHost()
    {
        return m_szHostName;
    }
    

    //����Զ��������Ϣ
    void SetRemoteHostMsg(const char* const szHost,
                          const char* const szUser,
                          const char* const szPwd)
    {
        strncpy(m_szHostName, szHost, sizeof(m_szHostName));
        m_szHostName[sizeof(m_szHostName) - 1] = '\0';

        strncpy(m_szUser, szUser, sizeof(m_szUser));
        m_szUser[sizeof(m_szUser) - 1] = '\0';

        strncpy(m_szPwd, szPwd, sizeof(m_szPwd));
        m_szPwd[sizeof(m_szPwd) - 1] = '\0';

        m_fp->SetRemoteHostMsg(szHost,szUser,szPwd);
    }

    //���ñ���������Ϣ
    void SetLocalHostMsg(const char* const szHost,
                         const char* const szUser,
                         const char* const szPwd)
    {
        strncpy(m_szLocalHostName, szHost, sizeof(m_szLocalHostName));
        m_szHostName[sizeof(m_szLocalHostName) - 1] = '\0';

        strncpy(m_szLocalUser, szUser, sizeof(m_szLocalUser));
        m_szUser[sizeof(m_szLocalUser) - 1] = '\0';

        strncpy(m_szLocalPwd, szPwd, sizeof(m_szLocalPwd));
        m_szPwd[sizeof(m_szLocalPwd) - 1] = '\0';

        m_fp->SetLocalHostMsg(szHost,szUser,szPwd);
    }

protected:

    BOOL  m_bFailure;                   //�Ƿ����
    CFileProtocol*  m_fp;
    char  m_szErrInfo[MAX_ERROR_LEN];   //�������һ���ļ���������Ĵ�����Ϣ

    char  m_szLocalHostName[50];        //���汾������
    char  m_szLocalUser[50];            //�����û���
    char  m_szLocalPwd[50];             //�����û�����

    char  m_szHostName[50];             //����Զ���ļ�������������
    char  m_szUser[50];                 //Զ���ļ����������û���
    char  m_szPwd[50];                  //Զ���ļ����������û�����

    int   m_nAlarmPara;                 //�澯������
};

#endif //__LINK_BASE_H__
