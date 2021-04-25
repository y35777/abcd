#ifndef __FILEGROUP_H__
#define __FILEGROUP_H__

#include "../include/precom.h"
#include "../include/format_out.h"
#include "qb_comm.h"
#include "bill_file.h"
#include <iterator>

class CFileGroup
{
public:
    //���캯��
    CFileGroup(CFormat* pFormat, UINT4 uBillType, UINT4 uAPType, ACE_Mutex* pMutex, int nAPID);
    virtual ~CFileGroup();  //��������

    BOOL Open(int nFileNo, int nOffset);  //�򿪻����ļ�����
    //��ȡ��ǰ�򿪵Ļ����ļ���ź��䵱ǰ��дλ�ú���
    void GetCurPosition(int& nFileNo, int& nOffset);
    //��ȡ��һ�ŷ��������Ļ�����¼���ݵ�ƫ�Ƶ�ַ
    BOOL GetNextRecordOffset(int& nStartFileNo, int& nStartOffset, BOOL& bRemain);
    //��ȡָ���ļ�ָ��λ�õĻ�����¼����
    int GetRstRecord(int nStartFileNo,
                     int nStartOffset,
                     BYTE*& pBillRecord,
                     UINT4& uBillLen);

    //׼�������ļ�����������������
    BOOL PrepareFile(const char* szFileFullPath);
    //׼�������ļ���������Բ�ѯ����
    BOOL PrepareFiles(const char* szPath,
                      const char* szStartRange, const char*  szEndRange,
                      UINT4 uFlg, char* szQueryCond);

    //<SWPD05145> add by YanYong 2004-05-25
    BOOL PrepareFiles(const char* szPath, LIST<STRING> &queryList, 
                      UINT4 uFlg, char* szQueryCond);
    //</SWPD05145>

    void SetMustMerge(BOOL bMustMerge);
    void SetFinalSPFileFmt(BOOL bFinalSPFileFmt);
    INT8 GetQueryedLength();//����Ѳ�ѯ���ļ�����


    INT8 GetTotalFileLength(); //����ļ����������ļ��ĳ���
    INT8 CacalTotalFileLength(); //����ļ����������ļ��ĳ���
    //�����Ƿ���ʾ����Ļ���
    inline void SetDisplayErrBill(BOOL bDisplayErrBill)
    {
        if(m_pBillFile)
        {
            m_pBillFile->SetDisplayErrBill(bDisplayErrBill);
        }
    }

    VECTOR<STRING> GetFileNames();
    STRING GetFilesPath();
    
protected:
    BOOL OpenNextFile(BOOL& bRemain);   //����һ�������ļ�

protected:
    //�����ļ������б������ļ��޸�ʱ������
    VECTOR<STRING> m_qFileNames;
    CBillFile* m_pBillFile;         //�����ļ�����ָ��
    char       m_szPath[MAX_PATH];  //�����ļ���·��
    int        m_nCurFileNo;        //��ǰ�򿪵Ļ����ļ����
    int        m_nCurOffset;        //��ǰ�򿪵Ļ����ļ��Ķ�дָ��λ��

    INT8       m_nTotalFileLength;  //�ļ����������ļ��ĳ���

    INT8       m_nQueryedFileLen;

    ACE_Mutex  m_Mutex;

    //���ڻ��ָ����¼����ʼ�ļ��ű���
    int        m_nStartFileNo;

    //���ڻ��ָ����¼����ʼ�ļ�������
    STRING     m_strRstFileName;
};

#endif
