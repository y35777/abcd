#ifndef __SCP_BILL_FILE_H__
#define __SCP_BILL_FILE_H__

class CScpRvyTask;
#include "../querybrowse/bill_rec.h"
#include "../format/format.h"

class CScpBillFile
{
public:
    CScpBillFile(CFormat *pFormat, 
                 UINT4 uBillType, 
                 UINT4 uAPType, 
                 CScpRvyTask* pScpRvyTask);

    virtual ~CScpBillFile();  //��������

    //��ָ���Ļ����ļ���������Ӧ�Ķ�дλ��ָ�뺯��
    BOOL Open(const char* szPathFile, int nOffset);
    //��ȡ��һ�ŷ��������Ļ�����¼���ݺ���
    BOOL GetNextBill(CBillRecord*& pBillRecord, 
                    int& nCurOffset, BOOL& bRemain);
    int GetCurOffset();                            //��ȡ�ļ��ĵ�ǰ��дλ�ú���
    BOOL SetOffset(int nOffset);                   //�����ļ��Ķ�дָ��λ�ú���
    BOOL SetQueryCond(char* szQueryCond);          //���ò�ѯ�����ַ�������
    BOOL Close();                                  //�ر��ļ�����

protected:
    //��ȡһ�Ż�����¼���ݺ���
    BOOL Read(void*& pData, UINT4& uDataLen, BOOL& bRemain);  
    //�Ի����Ķ����ж�
    virtual BOOL IsMatchedBill(void* pData, int nLen);

protected:
    //CScpRvyTask�����ָ��
    CScpRvyTask* m_pScpRvyTask;

    UINT4    m_uAPType;         //���������
    UINT4    m_uBillType;       //��������
    CFormat* m_pFormat;         //�����ʽ����ָ��
    UINT4    m_uBillLen;        //�����������ʽ��ԭ��ʽ����
    FILE*    m_pFileStream;     //�ļ���ָ��
    char     m_szFileFullPath[MAX_PATH];  //��ǰ�򿪵Ļ����ļ���ȫ·��

    //���³�Ա���������ҪASN.1���������
    BYTE*    m_pMemFile;        //�ڴ�ӳ���ļ�ָ�룬��ʼ��ΪNULL
	int      m_nMemFileLen;     //�ڴ�ӳ���ļ��ĳ���
	int      m_nMemDataLen;     //�ڴ�ӳ���ļ����ݵĳ���
    int      m_nCurOffset;      //�ڴ�ӳ���ļ��е�ǰ��дλ�õ�ƫ��������ʼ��Ϊ0};
};

#endif //__SCP_BILL_FILE_H__
