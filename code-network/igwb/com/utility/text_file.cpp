// text_file.cpp: implementation of the CTextFile class.
//
//////////////////////////////////////////////////////////////////////

#include "text_file.h"
#include "../include/frame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextFile::~CTextFile()
{

}

CTextFile::CTextFile(UINT4 uBillType, UINT4 uBillLen, int nAPID)
                    :CIGWBFile(uBillType,uBillLen, nAPID)
{

}

/**********************************************************************
��������    ��ȡһ���ı���ʽ��ԭʼ������¼����
�������    ��
�������    pData----������¼���ݵ�ָ��
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼

**********************************************************************/
BOOL CTextFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    ASSERT(NULL != m_pMemFile);

    BYTE* pBillRecord = NULL;
    if (m_nMemDataLen <= m_nCurOffset)
    //�ڴ�ӳ���ļ��е�ʣ�೤�Ȳ���һ�������ĳ��ȣ�������������Ч
    {
        bRemain = FALSE;
    }
    else  
    {
        pBillRecord = m_pMemFile + m_nCurOffset;
        const char* p = ACE_OS::strnchr((const char*)pBillRecord, 
                                        '\n', 
                                        m_nMemDataLen - m_nCurOffset);
        if(NULL == p)
        {
            uDataLen = m_nMemDataLen - m_nCurOffset;
            m_nCurOffset += uDataLen;
        }
        else
        {
            uDataLen = UINT4(p - (const char*)pBillRecord);
            if(uDataLen > m_nMemDataLen - m_nCurOffset)
            {
                uDataLen = m_nMemDataLen - m_nCurOffset;
            }
            m_nCurOffset += uDataLen + 1;
        }
        pData = pBillRecord;

        while (uDataLen > 0 && pBillRecord[uDataLen - 1] == '\r')
        {
            uDataLen--;
        }

        if(uDataLen == 0)
        {
            bRemain = FALSE;
        }
        else
        {
            bRemain = TRUE;
        }
    }

    return TRUE;
}

