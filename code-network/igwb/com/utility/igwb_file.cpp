// igwb_file.cpp: implementation of the CIGWBFile class.
//
//////////////////////////////////////////////////////////////////////
#include "igwb_file.h"
#include "../include/toolbox.h"
#include "../include/frame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIGWBFile::	CIGWBFile(UINT4 uBillType, UINT4 uBillLen, int nAPID)
{
	m_uBillType       = uBillType;
    m_uBillLen        = uBillLen;
	m_nMemFileLen     = 4*1024*1024;
    m_pMemFile        = new BYTE[m_nMemFileLen];
    m_nMemDataLen     = 0;
    m_pFileStream     = NULL;
    m_bFinalSPFileFmt = CFG_AP_KEY_FINAL_SPFILEFMT_DEFAULT;
    m_nAPID           = nAPID;
    m_nCurOffset      = 0;
    m_bMustMerge      = FALSE;
}

CIGWBFile::~CIGWBFile()
{
	if (NULL != m_pFileStream)
    {
        fclose(m_pFileStream);
    }  

	if (NULL != m_pMemFile)
    {
        delete[] m_pMemFile;
    }
}

/**********************************************************************
功能描述    打开指定的话单文件和设置相应的读写位置指针函数
输入参数    szPathFile----话单文件名称（包含全路径）
            nOffset----读写位置偏移量
输出参数    无
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CIGWBFile::Open(const char* szPathFile, int nOffset)
{
    //如果当前有文件打开，则关闭之
    if (NULL != m_pFileStream)    
    {
        Close();
    }

    strcpy(m_szFileFullPath, szPathFile);
    m_nCurOffset = nOffset;

    //打开文件，并设置偏移量
    
    m_pFileStream = fopen(szPathFile, "rb");
    if (NULL == m_pFileStream)
    {
        return FALSE;
    }
    else
    {
        m_nMemDataLen = FileSize(m_pFileStream);
        if(m_nMemFileLen < m_nMemDataLen)
        {
            m_nMemFileLen = m_nMemDataLen;

            ASSERT(m_pMemFile != NULL);
            delete [] m_pMemFile;
            m_pMemFile = new BYTE[m_nMemFileLen];
        }

        if(m_nMemDataLen > 0)
        {
            size_t nBytesRead = fread(m_pMemFile, m_nMemDataLen, 
                                      1, m_pFileStream);
            if (1 != nBytesRead)
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}
            
//获取文件的当前读写位置函数
int CIGWBFile::GetCurOffset()
{
    return m_nCurOffset;
}

//设置文件的读写指针位置函数
BOOL CIGWBFile::SetOffset(int nOffset)
{
    m_nCurOffset = nOffset;

    return TRUE;
}

//关闭文件函数
BOOL CIGWBFile::Close()
{
    if (NULL != m_pFileStream)
    {
        fclose(m_pFileStream);
        m_pFileStream = NULL;
    }

    return TRUE;
}

//设置是否合并标志
void CIGWBFile::SetMustMerge(BOOL bMustMerge)
{
    m_bMustMerge = bMustMerge;
}

void CIGWBFile::SetFinalSPFileFmt(BOOL bFinalSPFileFmt)
{
    m_bFinalSPFileFmt = bFinalSPFileFmt;
}

/**********************************************************************
功能描述    读取一张话单记录数据函数
输入参数    无
输出参数    pData----话单记录数据的指针，使用固定内存
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CIGWBFile::Read(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
	BOOL bSucc;

    if (ORIG_BILL == m_uBillType)
	{
		bSucc = ReadOriginal(pData, uDataLen, bRemain);
	}
	else
	{
		bSucc = ReadFinal(pData, uDataLen, bRemain);
	}

    return bSucc;
}


/**********************************************************************
功能描述    读取一张原始话单记录数据
输入参数    无
输出参数    pData---- 话单记录数据的指针，使用固定的内存
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CIGWBFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    ASSERT(NULL != m_pMemFile);

    BYTE* pBillRecord = NULL;
    if (m_nMemDataLen < m_uBillLen + m_nCurOffset)
    //内存映射文件中的剩余长度不够一个话单的长度，则后面的数据无效
    {
        bRemain = FALSE;
    }
    else  
    {
        pBillRecord = m_pMemFile + m_nCurOffset;
        //如果读出来的话单内容全部为0，则说明本文件是前存盘的最后一个话单，
        //读出的话单内容以及本文件的后续内容必须丢弃，将bRemain设置为FALSE

        BOOL   bValid = FALSE;
        //判断是否读出的话单内容都是0
        for (int i = 0; i < m_uBillLen; i++)
        {
			if (0 != pBillRecord[i])
            {
				bValid = TRUE;
                break;
             }
        }

        //如果话单内容都是0，则说明该话单是无效的，必须丢弃
        if (!bValid)
        {
            bRemain = FALSE;
        }
        else
        {
            m_nCurOffset += m_uBillLen;
            pData = pBillRecord;
            uDataLen = m_uBillLen;
            bRemain = TRUE;
        }
    }

    return TRUE;
}


/**********************************************************************
功能描述    读取一张最终话单话单记录数据
输入参数    无
输出参数    pData---- 话单记录数据的指针，使用固定的内存
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CIGWBFile::ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
	ASSERT(NULL != m_pMemFile);

    BYTE* pBillRecord = NULL;
    if (m_nMemDataLen < m_uBillLen + m_nCurOffset)
    //内存映射文件中的剩余长度不够一个话单的长度，则后面的数据无效
    {
        bRemain = FALSE;
    }
    else  
    {
        pBillRecord = m_pMemFile + m_nCurOffset;
        m_nCurOffset += m_uBillLen;
        pData = pBillRecord;
        uDataLen = m_uBillLen;
        bRemain = TRUE;
    }

    return TRUE;   
}


