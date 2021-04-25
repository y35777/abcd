// 5ess_file.cpp: implementation of the C5essFile class.
//
//////////////////////////////////////////////////////////////////////

#include "5ess_file.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C5essFile::C5essFile(UINT4 uBillType, UINT4 uBillLen, int nAPID)
                    :CIGWBFile(uBillType,uBillLen,nAPID)
{
	m_MetertValue= 0;

	m_szLine     = new char[512];
	m_szLine[0]  = 0;
}

C5essFile::~C5essFile()
{
	if (m_szLine != NULL)
	{
		delete[] m_szLine;
		m_szLine = NULL;
	}

}


/**********************************************************************
功能描述    读取一张5ESS的原始话单记录数据
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
BOOL C5essFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    BOOL nBillClass = 0;

	//分辨出话单的类型（详单0还是计次表1）
	if (m_nCurOffset == 0)
	{
		char* pStart = NULL;
		char* pSubrgd = NULL;

		pStart  = strstr((char*)m_pMemFile, "START");		
		pSubrgd = strstr((char*)m_pMemFile, "SUBRGD");

		if ((pStart != NULL) && (pSubrgd != NULL))
		{
			nBillClass = 1;

			//移到有效数据段
			m_nCurOffset =  (BYTE*)pSubrgd - m_pMemFile;
		}
		else
		{
			nBillClass = 0;

			//5ess话单前面有40字节头信息,第一次读时要移到40字节处
			m_nCurOffset = 40;
		}
	}


	if (nBillClass == 0)
	{
		return ReadOriginal_Detail(pData, uDataLen, bRemain);
	}
	else 
	{
		return ReadOriginal_Meter(pData, uDataLen, bRemain);
	}
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
BOOL C5essFile::ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    if(!m_bFinalSPFileFmt)
    {
        return CIGWBFile::ReadFinal(pData, uDataLen, bRemain);
    }
    
    return ReadOriginal(pData, uDataLen, bRemain);
}

/**********************************************************************
功能描述    读取一张5ESS的原始详细话单记录数据
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
BOOL C5essFile::ReadOriginal_Detail(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{   
	//每张话单前面4个字节表示此话单长度(也标识话单类型)

	INT4 nRecordLen = 0;
    if (m_nMemDataLen - m_nCurOffset < 4)
    //内存映射文件中的剩余长度不够一个话单长度的域，则后面的数据无效
    {
        bRemain = FALSE;
        return TRUE;
    }
    else  
    {
        //获取文件中的话单的长度，话单长度域以网络字节序存放,
        //由于考虑到UNIX下的字节对齐问题，所以先直接拷贝长度域

		BYTE  pTmp[4];
        memcpy(&pTmp, (BYTE* )m_pMemFile + m_nCurOffset, 4);
        nRecordLen = GetRecordLen(pTmp);
        
        //如果读出的话单长度为0，则说明本文件是前存盘的最后一个文件，
        //后面的话单无效
        if (0 == nRecordLen)
        {
            bRemain = FALSE;
            return TRUE;
        }
        else if (m_nMemDataLen - m_nCurOffset < nRecordLen)
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

    return TRUE;
}

/**********************************************************************
功能描述    读取一张5ESS的原始计次表话单记录数据
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
BOOL C5essFile::ReadOriginal_Meter(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{   
	char  pValue[20];
	char* pTemp   = NULL;
	char* pSubrgd = NULL;
	char* pLRLN   = NULL;

	pTemp = (char*)(m_pMemFile + m_nCurOffset);

	pSubrgd  = strstr((char*)pTemp, "SUBRGD");		
	pLRLN    = strstr((char*)pTemp, "\n");

	//内存映射文件中的剩余长度小于等于0，表示到达文件尾
	if (m_nMemDataLen - m_nCurOffset <= 0)
    {
        bRemain = FALSE;
        return TRUE;
    }

	//说明是类似“SUBRGD 50332000,”的行,分析出m_MetertValue
	if ((pSubrgd != NULL) && (pLRLN != NULL) && (pLRLN - pSubrgd > 0))
	{
		m_nCurOffset = m_nCurOffset + (pLRLN - pTemp + 1);

		//跳过"SUBRGD "
		pSubrgd = pSubrgd + 7;
		//回退到“,”
		pLRLN   = pLRLN - 1;

		strncpy(pValue, pSubrgd, pLRLN - pSubrgd);
		pValue[pLRLN - pSubrgd] = '\0';
		m_MetertValue = atoi(pValue);	
		
		pTemp = (char*)(m_pMemFile + m_nCurOffset);
		pLRLN    = strstr((char*)pTemp, "\n");
	}


	if (pLRLN != NULL)
	{		
		int   nLen;
		
		sprintf(pValue, "%d", m_MetertValue);
		nLen = strlen(pValue);
		strcpy(m_szLine, pValue);
			
		strncpy(m_szLine + nLen, pTemp + 1, pLRLN - pTemp - 1);
		nLen = nLen + pLRLN - pTemp - 1;
		m_szLine[nLen] = '\0';
		
		pData    = m_szLine;
		uDataLen = nLen;
		m_nCurOffset += pLRLN - pTemp + 1;

		m_MetertValue++;
	}
	else
	{
		if (m_nCurOffset + 37 == m_nMemDataLen)
		{
			int   nLen;
			
			sprintf(pValue, "%d", m_MetertValue);
			nLen = strlen(pValue);
			strcpy(m_szLine, pValue);
				
			strncpy(m_szLine + nLen, pTemp + 1, 36);
			nLen = nLen + 36;
			m_szLine[nLen] = '\0';
			
			pData    = m_szLine;
			uDataLen = nLen;
			m_nCurOffset += 37;				
		}
	}

	bRemain = TRUE;
	return TRUE;

}


INT2 C5essFile::GetRecordLen(BYTE* pData)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char   pCurStr[9];   //文本格式的BCD码字符串指针

	//初始化BCD码数组指针和文本格式的BCD码字符串指针
    pBcdArray = pData;

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < 4)
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yCurBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

	return atoi(pCurStr);
}