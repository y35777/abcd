// zxj_file.cpp: implementation of the CZxjFile class.
//
//////////////////////////////////////////////////////////////////////

#include "zxj_file.h"
#include "../include/toolbox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZxjFile::CZxjFile(UINT4 uBillType, UINT4 uBillLen, int nAPID)
                    :CIGWBFile(uBillType,uBillLen,nAPID)
{
	m_nVersion   = 0;
}

CZxjFile::~CZxjFile()
{

}

/**********************************************************************
功能描述    读取一张ZTE的话单记录数据
输入参数    无
输出参数    pData----话单记录数据的指针，使用固定内存
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
修改记录
maimaoshi,2003-06-03
**********************************************************************/
BOOL CZxjFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
	//ZTE话单必须通过判断文件前2个字节来判断版本号，判断话单类型
	//1、c8 00(200)表示V300详单，长度为93字节
	//2、2c 01(300)表示V302详单，长度为123字节
	//3、c8(200)表示V303计次表，长度为98字节
	//4、c9(201)表示V304计次表，长度为170字节

	//第一次读时要话单长度
	if (m_nCurOffset == 0)
	{		
		//先分析1字节
		BYTE nByteVersion = 0;
		memcpy(&nByteVersion, (BYTE* )m_pMemFile + m_nCurOffset, 1);

		m_nVersion = nByteVersion;

		if (m_nVersion == 200)
		{
			m_uBillLen = 98;			
		}
		else if (m_nVersion == 201)
		{
			m_uBillLen = 170;
		}
		else
		{
			//再分析2字节
			memcpy(&m_nVersion, (BYTE* )m_pMemFile + m_nCurOffset, 2);
            m_nVersion = ITOHS(m_nVersion);

			if (m_nVersion == 200)
			{
				m_uBillLen = 93;
			}
			else if (m_nVersion == 300)
			{
				m_uBillLen = 123;
			}
			else
			{
				bRemain = FALSE;
				return TRUE;
			}
		}
		m_nCurOffset = m_uBillLen;
	}


	//剩余的数据不够一张话单，则后面的数据无效
	if (m_nMemDataLen - m_nCurOffset < m_uBillLen)    
    {
        bRemain = FALSE;
        return TRUE;
    }
	
	BYTE* pBillRecord = NULL;
	pBillRecord = m_pMemFile + m_nCurOffset;
    //如果读出来的话单内容全部为0，则说明本文件是前存盘的最后一个话单，
    //读出的话单内容以及本文件的后续内容必须丢弃，将bRemain设置为FALSE

    BOOL bValid = FALSE;
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
	pData = pBillRecord ;
        uDataLen = m_uBillLen;
        bRemain = TRUE;
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
BOOL CZxjFile::ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    if(!m_bFinalSPFileFmt)
    {
        return CIGWBFile::ReadFinal(pData, uDataLen, bRemain);
    }
    
    return ReadOriginal(pData, uDataLen, bRemain);
}
