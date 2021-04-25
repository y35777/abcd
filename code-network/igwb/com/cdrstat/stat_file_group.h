#ifndef __STAT_FILE_GROUP_H__
#define __STAT_FILE_GROUP_H__

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

#include "../include/toolbox.h"

class CCDRStatFile;

class CCDRStatFileGroup
{
public:
    CCDRStatFileGroup();
    virtual ~CCDRStatFileGroup();
public:

    //���ò��������г�ʼ������
    int Init(UINT4 uAPType,
             UINT4 uBillType,
             BOOL  bMustMerge,
             BOOL  bFinalSPFileFmt,
             UINT4 uBillLen,
             int nAPID);

    //׼��ָ�����ڷ�Χ�ڵĻ����ļ�
    int PrepareFiles(const char* const szPath);

    //��ȡ��һ�Ż�����¼����
    BOOL GetNextRecord(BYTE*& pCDRRecord,  UINT4& nCDRLen, BOOL&  bRemain);

    //�����ļ��Ŵ��ļ�
    BOOL Open(int nFileNo);

    //����һ���ļ�
    BOOL OpenNextFile(BOOL& bRemain);

    //ת����ѯ����
    void TransQueryCond(const char* szCond,
                        const char* szBeginDate,
                        const char* szEndDate,
                        char* szExCond);

protected:

    //�����ļ���·��
    char            m_szPath[MAX_PATH];
    //�����ļ�����
    CCDRStatFile*   m_pCDRStatFile;
    //�����ļ������б�
    VECTOR<STRING>  m_FileNames;
    //��ǰ�򿪵Ļ����ļ����
    int             m_nCurFileNo;
};
#endif //__STAT_FILE_GROUP_H__

