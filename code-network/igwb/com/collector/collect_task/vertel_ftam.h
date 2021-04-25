#ifndef __VERTEL_FTAM_FILE_PROTOCOL_H__
#define __VERTEL_FTAM_FILE_PROTOCOL_H__

#include "file_protocol.h"
#include <assert.h>

//�������ļ��������Ķ���
class CVertelFtam : public CFileProtocol
{
public:

    CVertelFtam(int nTransSpeed, const char* szDevName);
    virtual  ~CVertelFtam();

    //��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
    virtual  int  InitProtocol();

    //�����ܷ��������ӶԶ��ļ�������, �麯��, �����������ʵ��
    virtual  int  TryConnect();

    //ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual int   GetFile(const SGetFileReq &pReq);

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual int   GetFileList(const  char* szSrcDir, SListOut &pOut);

    //ɾ���ļ�����˵�ָ���ļ�
    virtual  int  DelFile(const char* szFileName );

    //ȡ�ļ�����˵�ָ���ļ��ĳ���
    virtual  int  GetFileLen(const char* szFileName,
                             unsigned long& uFileLen);

    //������ָ���ļ�
    virtual  int  RenameFile(const char *szOld, const char *szNew);

    //�����ļ������
    virtual  int  Connect()
    {
        return ERR_SUCCESS;
    }

    //�Ͽ�����
    virtual  int  Disconnect()
    {
        return ERR_SUCCESS;
    }

protected:
    static ACE_Recursive_Thread_Mutex g_VertelMutex;
    int                               m_nTransSpeed;
    int                               m_bOperate;
    LIST<int>                         m_LastStatusList;
    char                              m_szDevName[50];
};

#endif //__VERTEL_FTAM_FILE_PROTOCOL_H__
