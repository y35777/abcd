#include "data_buffer.h"
#include "resource.h"

/******************************************************
// ������:  CDataBuffer::CDataBuffer
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ���캯��
// ����:
//       ����1 :UINT4 uBufLen
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CDataBuffer::CDataBuffer(UINT4 uBufLen)
{
    m_pBuffer = 0;
    m_uBufLen = uBufLen;
    m_uWritten = 0;
    if (m_uBufLen > 0)  //���仺����
    {
        m_pBuffer = new BYTE[m_uBufLen];
    }
}


/******************************************************
// ������:  CDataBuffer::~CDataBuffer
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ��������
// ����:
//       ����1 :void
// ����ֵ: 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
CDataBuffer::~CDataBuffer(void)
{
    //�ͷŻ�����m_pBuffer
    if(m_pBuffer != 0)
    {
        delete [] m_pBuffer;
        m_pBuffer = 0;
    }
}


/******************************************************
// ������:  CDataBuffer::Alloc
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ���·��仺����
// ����:
//       ����1 :UINT4 uBufLen
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
void CDataBuffer::Alloc(UINT4 uBufLen)
{
    ASSERT(uBufLen > 0);

    //�ͷ�ԭ���Ļ�����
    if (m_pBuffer != 0)
    {
        delete [] m_pBuffer;
        m_pBuffer = 0;
    }

    m_uBufLen = uBufLen;
    m_uWritten = 0;

    //�����µĻ�����
    m_pBuffer = new BYTE[m_uBufLen];
}


/******************************************************
// ������:  CDataBuffer::Put
// ����:    Wangfeng
// ʱ��:    01-10-18
// ����:    ����pBuf��ʼ��uBufLen���ֽڿ������ڲ���������
// ����:
//       ����1 :const char* const pBuf
//       ����2 :UINT4 const uBufLen
// ����ֵ: int 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-18      Wangfeng
******************************************************/
int CDataBuffer::Put(const char* const pBuf, UINT4 const uBufLen)
{
    if ((m_uBufLen - m_uWritten >= uBufLen) && m_pBuffer)
    //�ڲ���������ʣ�೤���㹻
    {
        //����pBuf��ʼ��uBufLen���ֽڿ�����m_pBuffer+m_uWritten
        //��ʼ��������
        memcpy(m_pBuffer + m_uWritten, pBuf, uBufLen);
        m_uWritten += uBufLen;
        return ERR_SUCCESS;
    }
    else
    {
        //���ÿ���ṩ��MSGOUT������MML���е���̨��������ԭʼ��
        //��ʱ���ڲ����������㡱�Ĵ�����Ϣ
        MSGOUT(MTS_NETWORK, MSGOUT_LEVEL_IMPORTANT,
               S_NET_MAKE_BILL_PACKAGE);
        return ERR_FAIL;
    }
}



/******************************************************
// ������:  CDataBuffer::Get
// ����:    Wangfeng
// ʱ��:    01-10-19
// ����:    ����ڲ���������ָ��
// ����:
//       ����1 :char*& pBuf--(���)�ڲ���������ָ��
//       ����2 :UINT4& uBufLen--(���)�ڲ����������Ѿ�д
//              ������ݵ��ֽ���
//       ����3 :BOOL const bReset--���ò�������TRUE����
//              ��ʾ�����߸����ͷ�����õĻ�����������
//              �������ͷ��ڲ�������
// ����ֵ: void 
// ע��: 
// ����: 
// ����            ����                  ����     
   ========== ================== ======================
//01-10-19      Wangfeng
******************************************************/
void CDataBuffer::Get(char*& pBuf, UINT4& uBufLen, BOOL const bReset)
{
    pBuf = (char*)m_pBuffer;
    uBufLen = m_uWritten;
    if (bReset)
    {
        m_pBuffer = 0;
        m_uBufLen = 0;
        m_uWritten = 0;
    }
}