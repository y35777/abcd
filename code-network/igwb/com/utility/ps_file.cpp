// 3g_file.cpp: implementation of the CPSFile class.
//
//////////////////////////////////////////////////////////////////////

#include "ps_file.h"
#include "../include/toolbox.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPSFile::CPSFile(UINT4 uBillType, UINT4 uBillLen, int nAPID)
                    :CIGWBFile(uBillType,uBillLen,nAPID)
{
	m_pASN1RecContent  = NULL;
    m_pASN1IniGlobal   = NULL;
    m_pASN1FinalGlobal = NULL;
}

CPSFile::~CPSFile()
{
    if (NULL != m_pASN1RecContent)
    {
        delete[] m_pASN1RecContent;
        m_pASN1RecContent = NULL;
    }
    if (NULL != m_pASN1IniGlobal)
    {
        m_PSAsnClosor.Release_OSS_Lib_Ini(m_pASN1IniGlobal);
        m_pASN1IniGlobal = NULL;
    }
    if (NULL != m_pASN1FinalGlobal)
    {
        m_PSAsnClosor.Release_OSS_Lib_Final(m_pASN1FinalGlobal);
        m_pASN1FinalGlobal = NULL;
    }
}

/**********************************************************************
功能描述    读取一张3G的原始话单记录数据
输入参数    无
输出参数    pData----话单记录数据的指针，使用固定内存m_pASN1RecContent
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
修改记录

**********************************************************************/
BOOL CPSFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    if (NULL == m_pASN1RecContent)
    {
        if(!m_PSAsnClosor.GetbAviable())
        {
            m_PSAsnClosor.Init(m_nAPID);
            if(!m_PSAsnClosor.GetbAviable())
            {
                bRemain = FALSE;
                return FALSE;
            }
        }

        m_pASN1RecContent = new BYTE[ASN1_BUF_SIZE];
        m_pASN1IniGlobal = m_PSAsnClosor.Init_OSS_Lib_Ini();
    }

    //3G原始话单的存放方式为：2位长度＋话单内容
    int nSizeFieldLen;
    nSizeFieldLen = 2;  
   
    INT4 nRecordLen = 0;
    if (m_nMemDataLen - m_nCurOffset < nSizeFieldLen)
    //内存映射文件中的剩余长度不够一个话单长度的域，则后面的数据无效
    {
        bRemain = FALSE;
        return TRUE;
    }
    else  
    {
        INT2 nTemp;
        memcpy(&nTemp, (BYTE* )m_pMemFile + m_nCurOffset, 2);
        nTemp = NTOHS(nTemp);
        nRecordLen = nTemp;

        //如果读出的话单长度为0，则说明本文件是前存盘的最后一个文件，
        //后面的话单无效
        if (0 == nRecordLen)
        {
            bRemain = FALSE;
            return TRUE;
        }
        else if (m_nMemDataLen - m_nCurOffset < nSizeFieldLen + nRecordLen)
        //剩余的数据不够一张话单，则后面的数据无效
        {
            bRemain = FALSE;
            return TRUE;
        }
    }

    m_nCurOffset += nSizeFieldLen;
    uDataLen = nRecordLen;

    //对3G原始话单进行ASN.1进行解码，在uDataLen中返回解码后的长度
	int nRet = m_PSAsnClosor.DeCodeBill_ini_mt(m_pASN1IniGlobal, 
                                               (char* )m_pMemFile + m_nCurOffset, 
                                               (int* )&uDataLen,
                                               (char* )m_pASN1RecContent,
                                               ASN1_BUF_SIZE);
    
    m_nCurOffset += nRecordLen;

    if (ERR_SUCCESS != nRet)
    {
        pData    = NULL;
        uDataLen = 0;
        bRemain  = TRUE;
        return TRUE;
    }
    else
    {
        pData = m_pASN1RecContent;
    }

    bRemain = TRUE;

    return TRUE;
}

/**********************************************************************
功能描述    读取一张3G的原始话单记录数据
输入参数    无
输出参数    pData----话单记录数据的指针，使用固定内存m_pASN1RecContent
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
修改记录

**********************************************************************/
BOOL CPSFile::ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    if (NULL == m_pASN1RecContent)
    {
        if(!m_PSAsnClosor.GetbAviable())
        {
            m_PSAsnClosor.Init(m_nAPID);
            if(!m_PSAsnClosor.GetbAviable())
            {
                bRemain = FALSE;
                return FALSE;
            }
        }

        m_pASN1RecContent = new BYTE[ASN1_BUF_SIZE];
        if (!m_bMustMerge)
        {
            m_pASN1IniGlobal = m_PSAsnClosor.Init_OSS_Lib_Ini();
        }
        else
        {
            m_pASN1FinalGlobal = m_PSAsnClosor.Init_OSS_Lib_Final();
        }
    }

    //3G最终话单的存放方式为：4位长度＋话单内容
    int nSizeFieldLen;
    nSizeFieldLen = 4;  
    
    INT4 nRecordLen = 0;
    if (m_nMemDataLen - m_nCurOffset < nSizeFieldLen)
    //内存映射文件中的剩余长度不够一个话单长度的域，则后面的数据无效
    {
        bRemain = FALSE;
        return TRUE;
    }
    else  
    {
        //获取文件中的话单的长度，话单长度域以网络字节序存放,
        //由于考虑到UNIX下的字节对齐问题，所以先直接拷贝长度域
        memcpy(&nRecordLen, (BYTE* )m_pMemFile + m_nCurOffset, 4);
        nRecordLen = NTOHL(nRecordLen);
        
        //如果读出的话单长度为0，则说明本文件是前存盘的最后一个文件，
        //后面的话单无效
        if (0 == nRecordLen)
        {
            bRemain = FALSE;
            return TRUE;
        }
        else if (m_nMemDataLen - m_nCurOffset < nSizeFieldLen + nRecordLen)
        //剩余的数据不够一张话单，则后面的数据无效
        {
            bRemain = FALSE;
            return TRUE;
        }
    }

    m_nCurOffset += nSizeFieldLen;
    uDataLen = nRecordLen;
    int nRet = ERR_FAIL;
    if (!m_bMustMerge)
    {
        //对3G原始话单进行ASN.1进行解码，在uDataLen中返回解码后的长度
	    nRet = m_PSAsnClosor.DeCodeBill_ini_mt(m_pASN1IniGlobal, 
                                               (char* )m_pMemFile + m_nCurOffset, 
                                               (int* )&uDataLen,
                                               (char* )m_pASN1RecContent,
                                               ASN1_BUF_SIZE);
    }
    else
    {
        //对3G最终话单进行ASN.1进行解码，在uDataLen中返回解码后的长度
	    nRet = m_PSAsnClosor.DeCodeBill_final_mt(m_pASN1FinalGlobal, 
                                                 (char* )m_pMemFile + m_nCurOffset, 
                                                 (int* )&uDataLen,
                                                 (char* )m_pASN1RecContent,
                                                 ASN1_BUF_SIZE);
    }
    m_nCurOffset += nRecordLen;


    if (ERR_SUCCESS != nRet)
    {
        pData = NULL;
        uDataLen = 0;
        return TRUE;
    }
    else
    {
        pData = m_pASN1RecContent;
    }

    bRemain = TRUE;

    return TRUE;
}
