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
功能描述    读取一张文本方式的原始话单记录数据
输入参数    无
输出参数    pData----话单记录数据的指针
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
修改记录

**********************************************************************/
BOOL CTextFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    ASSERT(NULL != m_pMemFile);

    BYTE* pBillRecord = NULL;
    if (m_nMemDataLen <= m_nCurOffset)
    //内存映射文件中的剩余长度不够一个话单的长度，则后面的数据无效
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

