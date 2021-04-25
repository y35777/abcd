// s1240_file.cpp: implementation of the CS1240File class.
//
//////////////////////////////////////////////////////////////////////

#include "s1240_file.h"
#include "../include/toolbox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CS1240File::CS1240File(UINT4 uBillType, UINT4 uBillLen, int nAPID)
                        :CIGWBFile(uBillType,uBillLen,nAPID)
{
	//
}

CS1240File::~CS1240File()
{

}


/**********************************************************************
功能描述    读取一张S1240的原始话单记录数据
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
BOOL CS1240File::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    //S1240J话单前面有20字节头信息(第14表示块实际长度)
    //通过每张话单第1字节前面4位识别话单类型，获得话单长度

	//第一次读时要获得实际长度
	if (m_nCurOffset % 2048 == 0)
	{
		//读2字节块实际长度
		UINT2 nBlockLen = 0;
        memcpy(&nBlockLen, (BYTE* )m_pMemFile + m_nCurOffset + 13, 2);
        nBlockLen = NTOHS(nBlockLen);

        if (nBlockLen > 2 * 1024)
		{
			bRemain = FALSE;
            return TRUE;
		}
		else
		{
			m_nBlockLeftLen = 2 * 1024 - nBlockLen;
			m_nCurOffset += 20;
		}
	}
	
	INT2 nRecordLen = 0;
    if (m_nMemDataLen - m_nCurOffset < 1)
    //内存映射文件中的剩余长度不够一个话单长度的域，则后面的数据无效
    {
        bRemain = FALSE;
        return TRUE;
    }
    else  
    {
        BYTE nBillType = 0;
        memcpy(&nBillType, (BYTE* )m_pMemFile + m_nCurOffset, 1);

		//获得话单长度
        nRecordLen = GetRecordLen(&nBillType);
        
        if (m_nMemDataLen - m_nCurOffset < nRecordLen)
        //剩余的数据不够一张话单，则后面的数据无效
        {
            bRemain = FALSE;
            return TRUE;
        }
    }

	BYTE* pBillRecord = NULL;
	pBillRecord = m_pMemFile + m_nCurOffset;
    //如果读出来的话单内容全部为0，则说明本文件是前存盘的最后一个话单，
    //读出的话单内容以及本文件的后续内容必须丢弃，将bRemain设置为FALSE

    BOOL bValid = FALSE;
    //判断是否读出的话单内容都是0
    for (int i = 0; i < nRecordLen; i++)
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
        m_nCurOffset += nRecordLen;
        pData = pBillRecord;
        uDataLen = nRecordLen;
        bRemain = TRUE;
    }

	if ((m_nCurOffset + m_nBlockLeftLen) % 2048 == 0)
	{
		m_nCurOffset += m_nBlockLeftLen;
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
BOOL CS1240File::ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    if(!m_bFinalSPFileFmt)
    {
        return CIGWBFile::ReadFinal(pData, uDataLen, bRemain);
    }
    
    return ReadOriginal(pData, uDataLen, bRemain);
}


/**********************************************************************
功能描述    根据话单类型获得话单长度
输入参数    无
输出参数    
返回值      记录长度
抛出异常    无
修改记录
maimaoshi,2003-06-03
**********************************************************************/

INT2 CS1240File::GetRecordLen(BYTE* pData)
{
	BYTE nTemp;
	BYTE nBillType = *pData;
	//处理高四位
	nTemp = nBillType >> 4;
	if (nTemp > 0x09)
	{
		return -1;
	}

	switch (nTemp)
	{
	case 0:
		return 60;
	case 1:
		return 78;
	case 3:
		return 160;
	case 4: 
		return 106;
	default:
		return -1;
	}
}
