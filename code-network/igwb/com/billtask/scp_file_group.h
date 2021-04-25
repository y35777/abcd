#ifndef __SCP_FILE_GROUP_H__
#define __SCP_FILE_GROUP_H__

class CScpRvyTask;
#include "../querybrowse/bill_rec.h"
#include "scp_bill_file.h"

class CScpFileGroup
{
public:
    CScpFileGroup(CFormat* pFormat, 
                  UINT4 uBillType,
                  UINT4 uAPType,
                  CScpRvyTask* pScpRvyTask);
    virtual ~CScpFileGroup();

    BOOL Open(int nFileNo, int nOffset);  //�򿪻����ļ�����
    //��ȡ��ǰ�򿪵Ļ����ļ���ź��䵱ǰ��дλ�ú���
    void GetCurPosition(int& nFileNo, int& nOffset); 
    //��ȡ��һ�ŷ��������Ļ�����¼���ݺ���
    BOOL GetNextRecord(CBillRecord*& pBillRecord, int& nStartFileNo, 
                       int& nStartOffset, BOOL& bRemain);
    //׼�������ļ�����������������
    BOOL PrepareFile(const char* szFileFullPath);
    //׼�������ļ���������Բ�ѯ����
    BOOL PrepareFiles(const char* szPath, 
                      const char* szStartRange, const char*  szEndRange, 
                      UINT4 uFlg, char* szQueryCond);

protected:
    BOOL OpenNextFile(BOOL& bRemain);   //����һ�������ļ�

protected:
    //�����ļ������б������ļ��޸�ʱ������
    VECTOR<STRING> m_qFileNames;  
    CScpBillFile*  m_pBillFile;         //�����ļ�����ָ��
    char       m_szPath[MAX_PATH];  //�����ļ���·��
    int        m_nCurFileNo;        //��ǰ�򿪵Ļ����ļ����
    int        m_nCurOffset;        //��ǰ�򿪵Ļ����ļ��Ķ�дָ��λ��
    CFormat*   m_pFormat;           //��ʽ����
    UINT4      m_uBillType;         //��������
    UINT4      m_uAPType;           //���������
};

#endif //__SCP_FILE_GROUP_H__
