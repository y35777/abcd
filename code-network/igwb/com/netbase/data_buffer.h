#ifndef  __DATA_BUFFER_H__
#define  __DATA_BUFFER_H__

#include "../include/frame.h"

class CDataBuffer
{
public:
    CDataBuffer(UINT4 uBufLen = 0);
    virtual ~CDataBuffer(void);

    //���·��仺����
    void Alloc(UINT4 uBufLen);

    //����pBuf��ʼ��uBufLen���ֽڿ������ڲ���������
    int Put(const char* const pBuf, UINT4 const uBufLen);

    //����ڲ���������ָ��
    void Get(char*& pBuf, UINT4& uBufLen, BOOL const bReset = TRUE);

protected:

    //��ָ��ָ��������ʱ��Ž�Ҫ������͸�ǰ����ģ������ݵĻ�����
    //�û������ڹ��캯����Alloc�����д������������������ͷ�
    BYTE* m_pBuffer;

    //�ó�Ա������ʾm_pBuffer�������ĳ���
    //�ó�Ա�����ڹ��캯����Alloc����������
    UINT4 m_uBufLen;

    //�ó�Ա������ʾ������m_pBuffer���Ѿ�д������ݵ��ֽ�����
    UINT4 m_uWritten;

};

#endif  //__DATA_BUFFER_H__
