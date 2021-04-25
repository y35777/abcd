#include "../../include/base_type.h"
#include "collect_include.h"
#include "collect_define.h"
#include "collect_struct.h"
#include "file_protocol.h"
#include <assert.h>

//�������ļ��������CFileProtocol��ʵ��

// ���캯��
CFileProtocol::CFileProtocol()
{
    //��Ա������ʼ��
    m_szSvrName[0]   = '\0';
    m_szSvrUsr[0]    = '\0';
    m_szSvrPwd[0]    = '\0';

    m_szLocalName[0] = '\0';
    m_szLocalUsr[0]  = '\0';
    m_szLocalPwd[0]  = '\0';

    m_szErrInfo[0]   = '\0';
    m_nStatus        = -1;
}

//��������
CFileProtocol::~CFileProtocol()
{
    //NULL
}


//��ʼ�������ļ�Э�����, ���麯��, �����������ʵ�ָ÷���
int CFileProtocol::InitProtocol()
{
    return  ERR_SUCCESS;
}

//�����ܷ��������ӶԶ��ļ�������, �麯��, �����������ʵ��
int CFileProtocol::TryConnect()
{
    return  m_nStatus;
}

//ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
int CFileProtocol::GetFile(const SGetFileReq &pReq)
{
    return  ERR_SUCCESS;
}

//����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
int CFileProtocol::GetFileList(const  char* szSrcDir, SListOut &pOut)
{
    return  ERR_SUCCESS;
}

//ɾ���ļ�����˵�ָ���ļ�
int CFileProtocol::DelFile(const char* szFileName)
{
    return  ERR_SUCCESS;
}

//ȡ�ļ�����˵�ָ���ļ��ĳ���
int CFileProtocol::GetFileLen(const char* szFileName,
                              unsigned long& uFileLen)
{
    return  0;
}

//������ָ���ļ�
int CFileProtocol::RenameFile( const char *szOld, const char *szNew )
{
    return  ERR_SUCCESS;
}

//�����ļ������
int CFileProtocol::Connect()
{
    return  ERR_SUCCESS;
}

//�Ͽ�����
int CFileProtocol::Disconnect()
{
    return  ERR_SUCCESS;
}

//�����ļ�����ģʽ
int CFileProtocol::SetMode(int nMode)
{
    return  ERR_SUCCESS;
}


/********************************************************************
Function:        char  *  GetLastErr();
Description:     �õ����һ���ļ���������Ĵ�����Ϣ
Calls:           ��
Called By:       ���ʹ����
Table Accessed:  ��
Table Updated:   ��
Input:           ��
Output:          ��
Return:          �����ַ���ָ�룬�������һ���ļ���������Ĵ�����Ϣ
Others:          ��
*********************************************************************/
const char* CFileProtocol::GetLastErr()
{
    //�������һ���ļ���������Ĵ�����Ϣ
    return  m_szErrInfo;
}


//����Զ��������Ϣ
void CFileProtocol::SetRemoteHostMsg(const char* const szHost,
                                     const char* const szUser,
                                     const char* const szPwd)
{
    if((NULL != szHost) && (szHost[0] != '\0'))
    {
        strncpy(m_szSvrName, szHost, sizeof(m_szSvrName));
        m_szSvrName[sizeof(m_szSvrName) - 1] = '\0';
    }

    if((NULL != szUser) && (szUser[0] != '\0'))
    {
        strncpy(m_szSvrUsr, szUser, sizeof(m_szSvrUsr));
        m_szSvrUsr[sizeof(m_szSvrUsr) - 1] = '\0';
    }

    if((NULL != szPwd) && (szPwd[0] != '\0'))
    {
        strncpy(m_szSvrPwd, szPwd, sizeof(m_szSvrPwd));
        m_szSvrPwd[sizeof(m_szSvrPwd) - 1] = '\0';
    }

}


//���ñ���������Ϣ
void CFileProtocol::SetLocalHostMsg(const char* const szHost,
                                    const char* const szUser,
                                    const char* const szPwd)
{
    if((NULL != szHost) && (szHost[0] != '\0'))
    {
        strncpy(m_szLocalName, szHost, sizeof(m_szLocalName));
        m_szLocalName[sizeof(m_szLocalName) - 1] = '\0';
    }

    if((NULL != szUser) && (szUser[0] != '\0'))
    {
        strncpy(m_szLocalUsr, szUser, sizeof(m_szLocalUsr));
        m_szLocalUsr[sizeof(m_szLocalUsr) - 1] = '\0';
    }

    if((NULL != szPwd) && (szPwd[0] != '\0'))
    {
        strncpy(m_szLocalPwd, szPwd, sizeof(m_szLocalPwd));
        m_szLocalPwd[sizeof(m_szLocalPwd) - 1] = '\0';
    }

}
