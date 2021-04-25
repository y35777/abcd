#ifndef __FTP_PROTOCOL_H__
#define __FTP_PROTOCOL_H__

#include "file_protocol.h"
#include "../../toolbase/ftp_client.h"

//�������ļ��������Ķ���
class  CFtpProtocol : public CFileProtocol
{

public:

    CFtpProtocol();
    virtual  ~CFtpProtocol();

    //��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
    virtual  int  InitProtocol();

    //ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual  int  GetFile(const SGetFileReq &pReq);

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual  int  GetFileList( const  char* szSrcDir, SListOut &pOut );

    //ɾ���ļ�����˵�ָ���ļ�
    virtual  int  DelFile( const char* szFileName );

    //������ָ���ļ�
    virtual  int  RenameFile( const char *szOld, const char *szNew );

    //�����ļ������
    virtual  int  Connect();

    //�Ͽ�����
    virtual  int  Disconnect();

    //�����ļ�����ģʽ
    virtual  int  SetMode(int nMode);

    //�����ܷ��������ӶԶ��ļ�������, �麯��, �����������ʵ��
    virtual  int  TryConnect();

protected:
    CFtpClient    m_FtpClient;
    char          m_szRemotePath[MAX_PATH];
    int           m_nMode;
    ACE_Mutex     m_Mutex;
    BOOL          m_bConnected;
};

#endif //__FTP_PROTOCOL_H__
