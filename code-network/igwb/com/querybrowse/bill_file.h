#ifndef __BILLFILE_H__
#define __BILLFILE_H__

#include "../include/precom.h"
#include "../include/format_out.h"
#include "bill_rec.h"
#include "qb_comm.h"

class CIGWBFile;
class CBillFile
{
public:
    //���캯��
    CBillFile(CFormat *pFormat, UINT4 uBillType, UINT4 uAPType, int nAPID, ACE_Mutex* pMutex);

    //��������
    virtual ~CBillFile();

    //��ָ���Ļ����ļ���������Ӧ�Ķ�дλ��ָ�뺯��
    BOOL Open(const char* szPathFile, int nOffset);

    //��ָ���Ļ����ļ���������Ӧ�Ķ�дλ��ָ�뺯��
    BOOL OpenRstFile(const char* szPathFile, int nOffset);

    //��ȡ��һ�ŷ��������Ļ�����¼��ƫ�Ƶ�ַ
    BOOL GetNextBillOffset(int& nCurOffset, BOOL& bRemain);

    int  GetFileLen();
    //��ȡָ���ļ�ָ��λ�õĻ�����¼����
    int GetRstRecord(int nStartOffset,
                     BYTE*& pBillRecord,
                     UINT4& uBillLen);

    //��ȡ�ļ��ĵ�ǰ��дλ�ú���
    int GetCurOffset();

    //�����ļ��Ķ�дָ��λ�ú���
    BOOL SetOffset(int nOffset);

    //���ò�ѯ�����ַ�������
    BOOL SetQueryCond(char* szQueryCond);

    //�ر��ļ�����
    BOOL Close();

    //�����Ƿ���ʾ����Ļ���
    void SetDisplayErrBill(BOOL bDisplayErrBill);

    //�����Ƿ�ϲ���־
    void SetMustMerge(BOOL bMustMerge);
    void SetFinalSPFileFmt(BOOL bFinalSPFileFmt);

    //�Ի����Ķ����ж�
    virtual BOOL IsMatchedBill(void* pData, int nLen){return TRUE;};

protected:

    //IGWB�ļ�ָ��
    CIGWBFile*  m_pIGWBFile;

    //�����ʽ����ָ��
    CFormat*    m_pFormat;

    //��ѯ�����ַ���
    char        m_szQueryCond[MAX_COND_LEN];

    ACE_Mutex*  m_pMutex;

    //�Ƿ���ʾ���󻰵�
    BOOL        m_bDisplayErrBill;

    //���ڻ�ȡָ����¼��IGWB�ļ�ָ��
    CIGWBFile*  m_pRstIGWBFile;
};

#endif
