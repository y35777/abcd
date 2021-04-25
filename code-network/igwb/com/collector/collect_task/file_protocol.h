#ifndef __FILE_PROTOCOL_H__
#define __FILE_PROTOCOL_H__

#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"

//��������
static  const int  ATTR_FILE = 0;  //�ļ����ԣ������ļ�
static  const int  ATTR_DIR = 10;  //�ļ����ԣ�����Ŀ¼

//�������ļ��������Ķ���
class  CFileProtocol
{

public:

    CFileProtocol();
    virtual  ~CFileProtocol();

    //��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
    virtual  int  InitProtocol();

    //�����ܷ��������ӶԶ��ļ�������, �麯��, �����������ʵ��
    virtual  int  TryConnect();

    //ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
    virtual  int  GetFile(const SGetFileReq &pReq);

    //����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
    virtual  int  GetFileList( const  char* szSrcDir, SListOut &pOut );

    //ɾ���ļ�����˵�ָ���ļ�
    virtual  int  DelFile( const char* szFileName );

    //ȡ�ļ�����˵�ָ���ļ��ĳ���
    virtual  int  GetFileLen(const char* szFileName,
                             unsigned long& uFileLen);

    //������ָ���ļ�
    virtual  int  RenameFile( const char *szOld, const char *szNew );

    //�����ļ������
    virtual  int  Connect();

    //�Ͽ�����
    virtual  int  Disconnect();

    //�����ļ�����ģʽ
    virtual  int  SetMode(int nMode);

    //�õ����һ���ļ���������Ĵ�����Ϣ
    const char*   GetLastErr();

    //����Զ��������Ϣ
    void SetRemoteHostMsg(const char* const szHost,
                          const char* const szUser,
                          const char* const szPwd);

    //���ñ���������Ϣ
    void SetLocalHostMsg(const char* const szHost,
                         const char* const szUser,
                         const char* const szPwd);

public:
    char  m_szErrInfo[MAX_ERROR_LEN]; //�������һ���ļ���������Ĵ�����Ϣ


protected:
    char  m_szSvrName[50];      //����Զ���ļ�������������
    char  m_szSvrUsr[50];       //Զ���ļ����������û���
    char  m_szSvrPwd[50];       //Զ���ļ����������û�����

    char  m_szLocalName[50];    //���汾���ļ�������������
    char  m_szLocalUsr[50];     //�����ļ����������û���
    char  m_szLocalPwd[50];     //�����ļ����������û�����

    int   m_nStatus;            //״̬��0��ʾ����
};

#endif //__FILE_PROTOCOL_H__
