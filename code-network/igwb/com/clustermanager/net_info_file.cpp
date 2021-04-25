#include "../include/base_type.h"
#include "net_info_file.h"


/********************************************************
* ����: CNetInfoFile::CNetInfoFile
* ����: ���캯��
* ����: const char* const szFilePath : �ļ�����·��
*       const char* const szFileName : �ļ���
*       int nAPNum                   : ������
*       int nModule                  : ģ���
*       int nChl                     : ͨ����
* ���:
* ����:
********************************************************/
CNetInfoFile::CNetInfoFile(const char* const szFilePath,
                             const char* const szFileName,
                             int nAPNum,
                             int nModule,
                             int nChl)
                             :CSyncInfoFile(szFilePath,
                                            szFileName,
                                            nAPNum,
                                            nModule,
                                            nChl)
{
    m_pFile = NULL;
}

/********************************************************
* ����: CNetInfoFile::~CNetInfoFile
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CNetInfoFile::~CNetInfoFile()
{
    if(NULL != m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

/********************************************************
* ����: CNetInfoFile::Open
* ����: ״̬�ļ��򿪲�����ֻ�б�ͬ���������
* ����:
* ���:
* ����: int �򿪲����Ƿ�ɹ�
********************************************************/
int CNetInfoFile::Open()
{
    m_nBufferLen = 0;
    m_nSyncDataLen = 0;
    if (NULL != m_pBuffer)
    {
        delete [] m_pBuffer;
        m_pBuffer = NULL;
    }

    if(NULL == m_pFile)
    {
        //��fopen�����Զ����ƶ�д��ʽ��m_szFileNameָ����ļ�
        //Opens for both reading and writing. (The file must exist.)
        m_pFile = fopen(m_szFullFileName, "rb+");
        if(NULL == m_pFile)
        {
            return ERR_SUCCESS;
        }
    }

    //ȡ�ļ�����
    if(fseek(m_pFile, 0, SEEK_END) != 0)
    {
        return errno;
    }
    int nFileLen = ftell(m_pFile);
    if(fseek(m_pFile, 0, SEEK_SET) != 0)
    {
        return errno;
    }

    //��������
    if (nFileLen > 0)
    {
        //����һ���СΪm_nBufferLen���ڴ棬��ָ�븳��m_pBuffer
        m_nBufferLen   = nFileLen;
        m_nSyncDataLen = m_nBufferLen;
        m_pBuffer      = new char[m_nBufferLen];

        //���ļ��ж����������ݣ�����m_pBuffer��������
        if(fread(m_pBuffer, m_nBufferLen, 1, m_pFile) != 1)
        {
            return errno;
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CNetInfoFile::Close
* ����: ״̬�ļ��رղ�����ֻ�б�ͬ���������
* ����:
* ���:
* ����: void
********************************************************/
void CNetInfoFile::Close()
{
    if(NULL != m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

/********************************************************
* ����: CNetInfoFile::GetSyncData
* ����: ��ȡͬ�����ݲ���
* ����: nData     : �����������ָ��
*       nLen      : ����������ĳ���
*       bReadFile : �Ƿ���ļ��ж����µ����ݣ�TRUE��ʾҪ
*                   ���ļ��ж�����FALSE��ʾ�ӻ������ж���
* ���:
* ����: int ��õ�ʵ�ʶ��������ݳ���
********************************************************/
int CNetInfoFile::GetSyncData(void* pData, int nLen, BOOL bReadFile)
{
    errno = 0;

    if(bReadFile)
    {
        //��״̬�ļ�;
        if (Open() != ERR_SUCCESS)
        {
            return 0;
        }
    }

    if (nLen < m_nBufferLen)
    {
        errno = ERR_BUFFER_NOT_ENOUGH;
        return 0;
    }
    else if((NULL != m_pBuffer) && (m_nBufferLen > 0))
    {
        //��m_pBufferָ��Ļ��������ݿ�����pDataָ��Ļ�������
        memcpy((BYTE*)pData, m_pBuffer, m_nBufferLen);
        return m_nBufferLen;
    }

    return 0;
}

/********************************************************
* ����: CNetInfoFile::PutSyncData
* ����: д��״̬���ݲ���
* ����: pData : ���뻺������ָ��
*       nLen  : ���뻺�����ĳ���
* ���:
* ����: int ��õ�ʵ��д������ݳ���
********************************************************/
int CNetInfoFile::PutSyncData(void* pData, int nLen)
{
    errno = 0;

    //���û��������ֱ���˳�
    if(nLen <= 0)
    {
        return 0;
    }

    //���nLen>m_nBufferLen�������·���һ������ΪnLen�Ļ�������
    //����ָ�븳��m_pBuffer
    if(nLen > m_nBufferLen)
    {
        delete [] m_pBuffer;
        m_pBuffer = new char[nLen];
    }

    m_nBufferLen   = nLen;
    m_nSyncDataLen = nLen;

    //��pDataָ��Ļ���������������m_pBufferָ��Ļ�����
    memcpy(m_pBuffer, (BYTE*)pData, m_nBufferLen);

    return m_nBufferLen;
}

/********************************************************
* ����: CNetInfoFile::Flush
* ����: ���������е�����д��״̬�ļ���ֻ�б�ͬ���������
* ����:
* ���:
* ����: int д״̬�ļ��Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CNetInfoFile::Flush()
{
    if(NULL == m_pFile)
    {
        //��fopen�����Զ����ƶ�д��ʽ��m_szFileNameָ����ļ�
        //Opens for both reading and writing. (The file must exist.)
        m_pFile = fopen(m_szFullFileName, "rb+");
        if(NULL == m_pFile)
        {
            //���������µ��ļ������ļ�ָ�븳��m_pFile;
            //Opens an empty file for writing. If the given file exists,
            //its contents are destroyed.
            m_pFile = fopen(m_szFullFileName, "wb+");
            if(NULL == m_pFile)
            {
                return errno;
            }
        }
    }

    //��m_pBuffer�������е�����д��m_pFileָ����ļ�
    if(fseek(m_pFile, 0, SEEK_SET) != 0)
    {
        return ERR_FAIL;
    }

    //���ļ��ж����������ݣ�����m_pBuffer��������
    if((fwrite(m_pBuffer, m_nBufferLen, 1, m_pFile) != 1) ||
       (fflush(m_pFile) != 0))
    {
        return ERR_FAIL;
    }

    ACE_OS::truncate(m_szFullFileName, m_nBufferLen);

    return ERR_SUCCESS;
}