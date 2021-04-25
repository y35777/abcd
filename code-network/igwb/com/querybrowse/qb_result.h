#ifndef __BILL_QB_RESULT_H__
#define __BILL_QB_RESULT_H__

#include "../include/precom.h"
#include "../include/format_out.h"
#include "../include/frame.h"
#include "qb_comm.h"
#include "file_group.h"
#include "bill_rec.h"
#include <iterator>

class CBillQBResult
{
public:
    //���캯��
    CBillQBResult(UINT2 uDeskNo, int nWndNo, ACE_Mutex* pMutex, CFormat *pFormat,
                  UINT4 uPageSize, CFileGroup* pFileGroup, char* pResultPath);
    ~CBillQBResult();  //��������

    void Init();//��ʼ������

    void AddARecordOffset(int nStartFileNo, int nStartOffset);//����һ��������ƫ����

    BOOL SaveToFile();//����¼�µĻ���ƫ����ȫ�����浽�ļ���

    int GetPageCount();//�õ�����ҳ��

    UINT4 GetRecordCount();//�õ�������¼����

    void GetProcess(UINT4& nRecordCount, int& nPercent, int& nCurPage);//��ò�ѯ������Ľ���

    int GetData(int nPageNo, BOOL bRuning, LIST<STRING>& qResult, BOOL bDisplayErrBill);//��ȡָ��ҳ������¼���ݺ���

protected:

    //���ļ��л��ƫ����
    BOOL GetDataFromFile(int nStartRecord, int nRecordCount, ResultOffset*& pResultOffset);

    //���ڴ��л��ƫ����
    BOOL GetDataFromMem(int nStartRecord, int nRecordCount, ResultOffset*& pResultOffset);

    //���㻰��ҳ��
    int ComputePageCount();                     //�����ܵĻ���ҳ��

protected:

    CFormat*            m_pFormat;              //��ʽ�����

    STRING              m_strTmpFileName;       //��ʱ�ļ���
    FILE*               m_pTmpFile;             //��ʱ�ļ�ָ��
    VECTOR<ResultOffset> m_ResultOffsetList;    //ƫ�����б�


    int                 m_nPageCount;           //������ҳ��
    int                 m_nFileNo;              //��ǰ��������ļ���
    UINT4               m_uPageSize;            //ҳ��С
    UINT4               m_uRecordCount;         //���������Ļ�������
    LIST<CBillRecord*>  m_pRecordList;          //������¼�����б�

    CFileGroup*         m_pFileGroup;           //�ļ������
    VECTOR<STRING>      m_qFileNames;
    STRING              m_szPath;    
    INT8                m_nTotalFileLength;     //�ļ��������ļ��ĳ���

    ACE_Mutex*          m_pFormatMutex;
    ACE_Mutex           m_Mutex;
};

#endif //__BILL_QB_RESULT_H__





















