/*****************************************************
//		utility_ex.cpp
//*****************************************************
// Copyright 2002 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       maimaoshi
// Date:         2002-03-18
// Description:  该文件实现了GPRS或者3G话单浏览的工具函数
// 
// Side Effects: 
// Functions:
// Notes:    
// 
// Update: 2002-03-18 AM 10:28:00
// Date              Name              Description 
============== ================ ======================

*****************************************************/


#include "../format/igwb.h"
#include "../format/ibill_processor.h"
#include "../format/iquery_browse.h"
#include "../include/export_dll.h"
#include "utility_ex.h"
 
static ACE_Recursive_Thread_Mutex g_global_buf_Lock;  //使用全局变量的互斥锁
static char* g_pBillBuffer = NULL;

char* CGlobalBuffer::GetBillBuffer()
{
    g_global_buf_Lock.acquire();
    if(g_pBillBuffer == NULL)
    {
        g_pBillBuffer = new char[_BROWSEBUFFERLENGTH];
    }
    g_global_buf_Lock.release();

    return g_pBillBuffer;
}

void CGlobalBuffer::acquire()
{
    g_global_buf_Lock.acquire();
}

void CGlobalBuffer::release()
{
    g_global_buf_Lock.release();
}

const BYTE _BitMask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

//字符串数组，BoolToStr用到
const char *_BoolString[] = 
{
	"0",
	"1",
	"FALSE",
	"TRUE",
	"false",
	"true",
	"No",
	"Yes"	
};

/******************************************************
// Function name: BCDOctecToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将给出的八位组数据按BCD码的格式转换成字符串
// Input:
//       arg1 :LPOCTECSTRING pOctecString	输入的八位组数据
//       arg2 :char *pBuffer				存放返回字符串的数据区
//       arg3 :int nLength					数据区可用长度
// Return value: int						转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int BCDOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nDataLength = pOctecString->nLength;
	BYTE nData, nTemp;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;			//初始化输出数组

	if (nLength <= (nDataLength * 2))
		return 0;

	for (int i = 0; i < nDataLength; i++)
	{
		//取一个字节数据
		nData = pOctecString->pData[i];
		
		//处理高四位
		nTemp = nData >> 4;
		if (nTemp > 0x09)
			break;
		pBuffer[nLen++] = nTemp + '0';

		//处理低四位
		nTemp = nData & 0x0f;
		if (nTemp > 0x09)
			break;
		pBuffer[nLen++] = nTemp + '0';
	}

	pBuffer[nLen] = 0;

	return nLen;
}

/******************************************************
// Function name: CvtBCDOctecToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将给出的八位组数据按反向BCD码的格式转换成字符串
// Input:
//       arg1 :LPOCTECSTRING pOctecString	输入的八位组数据
//       arg2 :char *pBuffer				存放返回字符串的数据区
//       arg3 :int nLength					数据区可用长度
// Return value: int						转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int CvtBCDOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nDataLength = pOctecString->nLength;
	BYTE nData, nTemp;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;			//初始化输出数组

	if (nLength <= (nDataLength * 2))
		return 0;

	for (int i = 0; i < nDataLength; i++)
	{
		//取一个字节数据
		nData = pOctecString->pData[i];
		
		//处理低四位
		nTemp = nData & 0x0f;
		if (nTemp > 0x09)
			break;
		pBuffer[nLen++] = nTemp + '0';

		//处理高四位
		nTemp = nData >> 4;
		if (nTemp > 0x09)
			break;
		pBuffer[nLen++] = nTemp + '0';
	}
	
	pBuffer[nLen] = 0;

	return nLen;
}

//by ldf 2004-03-26 对应问题单SWPD03933
/******************************************************
// Function name: CvtBCDOctecToStr2
// Author:        lidufang
// Date:          2003-03-03
// Description:   将给出的数据按反向BCD码的格式转换成字符串,遇到F不代表结束
//                比如pLMNIdentifier 64F000 则转换显示为46000
// Input:
//       arg1 :LPOCTECSTRING pOctecString	输入的数据
//       arg2 :char *pBuffer				存放返回字符串的数据区
//       arg3 :int nLength					数据区可用长度
// Return value: int						转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int CvtBCDOctecToStr2(LPOCTECSTRING pOctecString, char *pBuffer, int nLength)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nDataLength = pOctecString->nLength;
	BYTE nData, nTemp;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;			//初始化输出数组

	if (nLength <= (nDataLength * 2))
		return 0;

	for (int i = 0; i < nDataLength; i++)
	{
		//取一个字节数据
		nData = pOctecString->pData[i];
		
		//处理低四位
		nTemp = nData & 0x0f;
		if (nTemp <= 0x09)
        {
            pBuffer[nLen++] = nTemp + '0';
        }

		//处理高四位
		nTemp = nData >> 4;
		if (nTemp <= 0x09)
        {
            pBuffer[nLen++] = nTemp + '0';
        }
	}
	
	pBuffer[nLen] = 0;

	return nLen;
}
//end
/******************************************************
// Function name: DecOctecToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将给出的八位组数据按十进制数的格式转换成字符串
// Input:
//       arg1 :LPOCTECSTRING pOctecString	输入的八位组数据
//       arg2 :char *pBuffer				存放返回字符串的数据区
//       arg3 :int nLength					数据区可用长度
//       arg4 :int bSigned					被转换数据是否有符号数
// Return value: int						转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int DecOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, BOOL bSigned, BOOL bReversed)
{
	ASSERT((BOOL)(pOctecString && pBuffer));

	if ((pOctecString->nLength > 4) 
		|| (pOctecString->nLength = 0))
		return 0;
	
	UINT4 nData = 0;

	if (bReversed)
		memcpy(&nData, pOctecString->pData, pOctecString->nLength);
	else
	{
		BYTE *pTemp = (BYTE*)&nData;
		for (int i = 0; i < pOctecString->nLength; i++)
		{
			memcpy(pTemp+i, &pOctecString->pData[pOctecString->nLength-i-1], 1);
		}
	}

	return NumToDecStr((long)nData, pBuffer, nLength, bSigned);
}

/******************************************************
// Function name: BinOctecToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将给出的八位组数据按二进制数的格式转换成字符串
// Input:
//       arg1 :LPOCTECSTRING pOctecString	输入的八位组数据
//       arg2 :char *pBuffer				存放返回字符串的数据区
//       arg3 :int nLength					数据区可用长度
//       arg4 :char *pPrefix				前缀字符串
//       arg5 :BOOL bReverse				转换的时候是否先转换后面的字节
// Return value: int						转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int BinOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nPrefixLen = pPrefix ? strlen(pPrefix) : 0;
	BYTE nData;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLen + (pOctecString->nLength * 8)))
		return 0;

	//处理前缀字符串
	if (pPrefix)
	{
		strcpy(pBuffer, pPrefix);
		nLen = nPrefixLen;
	}

	int nDataLength = pOctecString->nLength;
	int i, j;

	for (i = 0; i < nDataLength; i++)
	{
		// 根据转换的顺序的不同，取一个字节的数据
		if (bReverse)
			nData = pOctecString->pData[nDataLength - i - 1];
		else
			nData = pOctecString->pData[i];

		//对一个字节的数据进行转换
		for (j = 0; j < 8; j++)
		{
			if (nData & _BitMask[j])
				pBuffer[nLen++] = '1';
			else
				pBuffer[nLen++] = '0';
		}
	}

	pBuffer[nLen] = 0;

	return nLen;
}


/******************************************************
// Function name: HexOctecToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将给出的八位组数据按十六进制数的格式转换成字符串
// Input:
//       arg1 :LPOCTECSTRING pOctecString	输入的八位组数据
//       arg2 :char *pBuffer				存放返回字符串的数据区
//       arg3 :int nLength					数据区可用长度
//       arg4 :char *pPrefix				前缀字符串
//       arg5 :BOOL bReverse				转换的时候是否先转换后面的字节
// Return value: int						转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int HexOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nPrefixLen = pPrefix ? strlen(pPrefix) : 0;
	BYTE nData;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLen + (pOctecString->nLength * 2)))
		return 0;

	//处理前缀字符串
	if (pPrefix)
	{
		nLen = nPrefixLen;
		strcpy(pBuffer, pPrefix);
	}

	int nDataLength = pOctecString->nLength;

	for (int i = 0; i < nDataLength; i++)
	{
		// 根据转换的顺序的不同，取一个字节的数据
		if (bReverse)
			nData = pOctecString->pData[nDataLength - i - 1];
		else
			nData = pOctecString->pData[i];

		nLen += sprintf(&pBuffer[nLen], "%02X", nData);
	}

	pBuffer[nLen] = 0;

	return nLen;
}

/******************************************************
// Function name: HexOctecToStr
// Author:        YanYong
// Date:          2004-05-27
// Description:   将给出的八位组数据按十六进制数的格式转换成字符串
//                
// Input:
//       arg1 :int nValueLength  表示pValue中有效字节的长度
         arg2 : const unsigned char* pValue 存放需要转换的数据
//       arg3 :char *pBuffer				存放返回字符串的数据区
//       arg4 :int nLength					数据区可用长度
//       arg5 :char *pPrefix				前缀字符串
//       arg6 :BOOL bReverse				转换的时候是否先转换后面的字节
// Return value: int						转换后字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 2004-05-27      YanYong               问题单SWPD05340
******************************************************/
int IGWB_Svc_Export HexOctecToStr(int nValueLength, const unsigned char* pValue, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse)
{
    int nLen = 0;			//返回的数据区内写入的数据长度
    int nPrefixLen = pPrefix ? strlen(pPrefix) : 0;
    BYTE nData;
    
    ASSERT((BOOL)(pValue && pBuffer));
    
    pBuffer[0] = 0;
    
    if (nLength <= (nPrefixLen + (nValueLength * 2)))
        return 0;
    
    //处理前缀字符串
    if (pPrefix)
    {
        nLen = nPrefixLen;
        strcpy(pBuffer, pPrefix);
    }
    
    int nDataLength = nValueLength;
    
    for (int i = 0; i < nDataLength; i++)
    {
        // 根据转换的顺序的不同，取一个字节的数据
        if (bReverse)
            nData = pValue[nDataLength - i - 1];
        else
            nData = pValue[i];
        
        nLen += sprintf(&pBuffer[nLen], "%02X", nData);
    }
    
    pBuffer[nLen] = 0;
    
    return nLen;
}


/******************************************************
// Function name: OctecToIPAddrStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将给出的八位组数据按IP地址的格式转换成字符串
// Input:
//       arg1 :LPOCTECSTRING pOctecString	输入的八位组数据
//       arg2 :char *pBuffer				存放返回字符串的数据区
//       arg3 :int nLength					数据区可用长度
//       arg4 :BYTE nStyle					转换的类型，是IPV4还是IPV6，取值见下
//				#define BINIPV4_STYLE			1			IPV4的类型
//				#define BINIPV6_STYLE			2			IPV6的类型
// Return value: int						转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int OctecToIPAddrStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, BYTE nStyle)
{
	int nLen = 0;			//返回的数据区内写入的数据长度	
	BYTE nData;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;

	int nDataLength = pOctecString->nLength;
	char pTempBuffer[64];

	switch (nStyle)
	{
	case BINIPV4_STYLE:
		{
			if (nDataLength != 4)
			{
				pBuffer[0] = 0;
				return 0;
			}
			for (int i = 0; i < nDataLength; i++)
			{
				nData = pOctecString->pData[i];
				nLen += sprintf(&pTempBuffer[nLen], "%d", nData);

				if (i < 3)
					pTempBuffer[nLen++] = '.';
			}
		}
		break;
	case BINIPV6_STYLE:
		{
			UINT2 wData;

			if (nDataLength != 16)
			{
				pBuffer[0] = 0;
				return 0;
			}
			for (int i = 0; i < nDataLength; i += 2)
			{				
				wData = pOctecString->pData[i] << 8;
				wData += pOctecString->pData[i+1];
				nLen += sprintf(&pTempBuffer[nLen], "%X", wData);

				if (i < 13)
					pTempBuffer[nLen++] = ':';
			}
		}
		break;
	default:
		{
			pBuffer[0] = 0;
			return 0;
		}
	}

	pTempBuffer[nLen] = 0;

	if (nLength > nLen)
		strcpy(pBuffer, pTempBuffer);
	else
		return 0;

	return nLen;
}

/******************************************************
// Function name: OctecToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将给出的八位组数据转换成字符串
// Input:
//       arg1 :LPOCTECSTRING pOctecString	输入的八位组数据
//       arg2 :char *pBuffer				存放返回字符串的数据区
//       arg3 :int nLength					数据区可用长度
// Return value: int						转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int OctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength)
{
	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;

	if (nLength > pOctecString->nLength)
	{
		memcpy(pBuffer, pOctecString->pData, pOctecString->nLength);
		pBuffer[pOctecString->nLength] = 0;
		return pOctecString->nLength;
	}
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////////////

/******************************************************
// Function name: BoolToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将BOOL型的数据按给定的类型转换成字符串
// Input:
//       arg1 :BOOL bValue		待转换的数据的值
//       arg2 :char *pBuffer	存放返回字符串的数据区
//       arg3 :int nLength		数据区可用长度
//       arg4 :BYTE nStyle		转换的类型，取值见下
//				#define BOOLBROWSE_NUMSTYLE		0			显示"0"或"1"
//				#define BOOLBROWSE_UPERSTYLE	1			显示"FALSE"或"TRUE"
//				#define BOOLBROWSE_LOWERSTYLE	2			显示"false"或"true"
//				#define BOOLBROWSE_YESNOSTYLE	3			显示"No"或"Yes"
// Return value: int			转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int BoolToStr(BOOL bValue, char *pBuffer, int nLength, BYTE nStyle)
{
	char buffer[6];
	int nLen;

	ASSERT((BOOL)pBuffer);

	pBuffer[0] = 0;

	int nPos = nStyle * 2;
	
	if (bValue)
		nPos++;

	strcpy(buffer, _BoolString[nPos]);

	nLen = strlen(buffer);

	if (nLength > nLen)
		strcpy(pBuffer, buffer);
	else
		return 0;

	return nLen;
}


/******************************************************
// Function name: NumToDecStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将整形数按十进制转换成字符串
// Input:
//       arg1 :long nNumber		待转换的数
//       arg2 :char *pBuffer	存放返回字符串的数据区
//       arg3 :int nLength		数据区可用长度
//       arg4 :BOOL bSigned		待转换数是否有符号类型
// Return value: int			转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int NumToDecStr(long nNumber, char *pBuffer, int nLength, BOOL bSigned)
{
	char buffer[32];
	int nLen;

	ASSERT((BOOL)pBuffer);

	pBuffer[0] = 0;

	if (bSigned)
        sprintf(buffer, "%d", nNumber);
	else
        sprintf(buffer, "%u", nNumber);

	nLen = strlen(buffer);
	if (nLength > nLen)
		strcpy(pBuffer, buffer);
	else
		return 0;

	return nLen;
}


/******************************************************
// Function name: NumToBinStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将整形数按二进制转换成字符串
// Input:
//       arg1 :UINT4 nNumber	待转换的数
//       arg2 :char *pBuffer	存放返回字符串的数据区
//       arg3 :int nLength		数据区可用长度
//       arg4 :char *pPrefix	前缀字符串，如果不需要前缀，用NULL
// Return value: int			转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int NumToBinStr(UINT4 nNumber, char *pBuffer, int nLength, const char *pPrefix)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nPrefixLength = pPrefix ? strlen(pPrefix) : 0;

	ASSERT((BOOL)pBuffer);

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLength + 32))
		return 0;

	//处理前缀字符串
	if (pPrefix)
	{
		nLen = nPrefixLength;
		strcpy(pBuffer, pPrefix);
	}

	BOOL bStart = FALSE;
	UINT4 nMask;
	for (int i = 0; i < 32; i++)
	{
		nMask = 0x80000000 >> i;
		if (nMask & nNumber)
		{
			bStart = TRUE;
			pBuffer[nLen++] = '1';
		}
		else if (bStart)
		{
			pBuffer[nLen++] = '0';
		}
	}
	pBuffer[nLen] = 0;

	return nLen;
}

/******************************************************
// Function name: DataToBinStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将数据流按二进制转换成字符串
// Input:
//       arg1 :char *pData		待转换的数据存放位置指针
//       arg2 :int nDataLength	待转换的数据的长度(BYTE)
//       arg3 :char *pBuffer	存放返回字符串的数据区
//       arg4 :int nLength		数据区可用长度
//       arg5 :char *pPrefix	前缀字符串
// Return value: int			返回的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int DataToBinStr(char *pData, int nDataLength, char *pBuffer, int nLength, const char *pPrefix)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nPrefixLength = pPrefix ? strlen(pPrefix) : 0;
	BYTE nData;

	ASSERT((BOOL)(pData && pBuffer));

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLength + (nDataLength * 8)))
		return 0;

	//处理前缀字符串
	if (pPrefix)
	{
		nLen = nPrefixLength;
		strcpy(pBuffer, pPrefix);
	}
	
	int i, j;
	for (i = 0; i < nDataLength; i++)
	{
		nData = pData[i];
		for (j = 0; j < 8; j++)
		{
			if (nData & _BitMask[j])
				pBuffer[nLen++] = '1';
			else
				pBuffer[nLen++] = '0';
		}
	}

	pBuffer[nLen] = 0;

	return nLen;
}


/******************************************************
// Function name: NumToHexStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将整形数按十六进制转换成字符串
// Input:
//       arg1 :UINT4 nNumber	待转换的数
//       arg2 :char *pBuffer	存放返回字符串的数据区
//       arg3 :int nLength		数据区可用长度
//       arg4 :char *pPrefix	前缀字符串
// Return value: int			返回的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//2002-03-19       maimaoshi
******************************************************/
IGWB_Svc_Export int NumToHexStr(UINT4 nNumber, char *pBuffer, int nLength, const char *pPrefix)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nPrefixLength = pPrefix ? strlen(pPrefix) : 0;
	
	ASSERT((BOOL)pBuffer);

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLength + 8))
		return 0;

	//处理前缀字符串
	if (pPrefix)
	{
		nLen = nPrefixLength;
		strcpy(pBuffer, pPrefix);
	}

	nLen += sprintf(&pBuffer[nLen], "%X", nNumber);

	pBuffer[nLen] = 0;

	return nLen;
}


/******************************************************
// Function name: DataToHexStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将数据流按十六进制转换成字符串
// Input:
//       arg1 :char *pData		待转换的数据
//       arg2 :int nDataLength	待转换的数据的长度
//       arg3 :char *pBuffer	存放返回字符串的数据区
//       arg4 :int nLength		数据区可用长度
//       arg5 :char *pPrefix	前缀字符串
// Return value: int			返回的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int DataToHexStr(char *pData, int nDataLength, char *pBuffer, int nLength, const char *pPrefix)
{
	int nLen = 0;			//返回的数据区内写入的数据长度
	int nPrefixLength = pPrefix ? strlen(pPrefix) : 0;
	BYTE nData;

	ASSERT((BOOL)(pBuffer && pData));

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLength + (nDataLength * 2)))
		return 0;

	//处理前缀字符串
	if (pPrefix)
	{
		nLen = nPrefixLength;
		strcpy(pBuffer, pPrefix);
	}

	for (int i = 0; i < nDataLength; i++)
	{
		nData = pData[i];
		nLen += sprintf(&pBuffer[nLen], "%02X", nData);
	}

	pBuffer[nLen] = 0;

	return nLen;
}

//////////////////////////////////////////////////////////////////////////////


/******************************************************
// Function name: TimeStampToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description: 将TimeStamp类型的数据转换成字符创  
// Input:
//       arg1 :LPTIMESTAMP pTimeStamp	待转换的TimeStamp类型数据的指针
//       arg2 :char *pBuffer			存放返回字符串的数据区
//       arg3 :int nLength				数据区可用长度
//       arg4 :BOOL bFormat				将时间格式化为“YYYY-MM-DD HH:MM:SS+HH:MM”的格式
//                                      将"YYYY/MM/DD"改为"YYYY-MM-DD" By XDB on 2001.11.09
// Return value: int					转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int TimeStampToStr(LPTIMESTAMP pTimeStamp, char *pBuffer, int nLength, BOOL bFormat)
{
	int nLen = 0;

	ASSERT((BOOL)pBuffer);

	if ((pTimeStamp->nLength != _TIME_LEN) || (nLength < _TIMESTAMPSTRLENGTH))
	{
		pBuffer[0] = 0;
		return 0;
	}

	BYTE nData;
	BOOL bSuccess = TRUE;
	for (int i = 0; i < 6; i++)
	{
		nData = pTimeStamp->pData[i];
		if (nData < 0xa0)
			pBuffer[nLen++] = (nData >> 4) + '0';
		else
		{
			bSuccess = FALSE;
			break;
		}

		nData &= 0x0f;
		if (nData < 0x0a)
			pBuffer[nLen++] = nData + '0';
		else 
		{
			bSuccess = FALSE;
			break;
		}
	}

	if (!bSuccess)
	{
		pBuffer[0] = 0;
		return 0;
	}

	pBuffer[nLen++] = pTimeStamp->pData[6];
	
	nLen += sprintf(&pBuffer[nLen], "%02x", pTimeStamp->pData[7]);
	nLen += sprintf(&pBuffer[nLen], "%02x", pTimeStamp->pData[8]);	

    pBuffer[nLen] = 0;

	if (bFormat)
	{
		char szTemp[26];
		memset(szTemp, 0, 26);
		memcpy(szTemp, "20", 2);
		memcpy(szTemp + 2, pBuffer, 2);
		memset(szTemp + 4, '-', 1);            
		memcpy(szTemp + 5, pBuffer + 2, 2);
		memset(szTemp + 7, '-', 1);           
		memcpy(szTemp + 8, pBuffer + 4, 2);
		memset(szTemp + 10, ' ', 1);
		memcpy(szTemp + 11, pBuffer + 6, 2);
		memset(szTemp + 13, ':', 1);
		memcpy(szTemp + 14, pBuffer + 8, 2);
		memset(szTemp + 16, ':', 1);
		memcpy(szTemp + 17, pBuffer + 10, 2);
		memcpy(szTemp + 19, pBuffer + 12, 3);
		memset(szTemp + 22, ':', 1);
		memcpy(szTemp + 23, pBuffer + 15, 2);
		strcpy(pBuffer, szTemp);
		nLen = strlen(pBuffer);
	}


	return nLen;
}

/******************************************************
// Function name: EnumToStr
// Author:        maimaoshi
// Date:          2002-03-19
// Description:   将Enum值按给出的信息转换成对应的字符串
// Input:
//       arg1 :LPENUMINFO pEnumInfo	枚举信息结构的指针
//       arg2 :int nEnumValue		待转换的枚举值
//       arg3 :char *pBuffer		存放返回字符串的数据区
//       arg4 :int nLength			数据区可用长度
// Return value: int				转换后的字符串的长度
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int EnumToStr(LPENUMINFO pEnumInfo, int nEnumValue, char *pBuffer, int nLength)
{
	ASSERT((BOOL)(pBuffer && pEnumInfo));

	pBuffer[0] = 0;

	int nCount = pEnumInfo->nCount;
	ENUMITEMINFO *pItem;
	BOOL bSuccess = FALSE;

	for (int i = 0; i < nCount; i++)
	{
		pItem = &pEnumInfo->pInfoTable[i];
		if (pItem->nValue == nEnumValue)
		{
			bSuccess = TRUE;
			break;
		}
	}

	if (!bSuccess)
		return 0;

	int nLen = strlen(pItem->pString);
	if (nLength < (nLen + 1))
		return 0;

	strcpy(pBuffer, pItem->pString);

	return nLen;
}


char szFormatCfgErr[2048];

extern "C"
{
IGWB_Svc_Export const char* GetFormatErr(void)
{
    return szFormatCfgErr;
}

IGWB_Svc_Export int CheckFormat(const char* szPath)
{
    szFormatCfgErr[0] = '\0';

    //初始化格式库接口
    STRING strIGWBPath = szPath;
    CIGWB* pIGWB = new CIGWB(strIGWBPath);
    if(NULL == pIGWB)
    {
        strncpy(szFormatCfgErr, "创建CIGWB对象失败", sizeof(szFormatCfgErr));
        szFormatCfgErr[sizeof(szFormatCfgErr) - 1] = '\0';
		return -1;
    }
    int nResult = pIGWB->Init(0);
    if (0 != nResult)
    {
        delete pIGWB;
		return -1;
    }

    /*
    //获得格式库查询接口
    IQueryBrowse* pIQueryBrowse = (IQueryBrowse*)pIGWB->QueryInterface(IID_FP_IQUERYBROWSE);
	if (NULL == pIQueryBrowse)
    {
        strncpy(szOutMsg, "获得格式库查询接口失败", nOutMsgLen);
        szOutMsg[nOutMsgLen - 1] = '\0';
        delete pIGWB;
		return -1;
    }
    nResult = pIQueryBrowse->Init(0);
    if (0 != nResult)
    {
        strncpy(szOutMsg, "格式库查询接口初始化失败", nOutMsgLen);
        szOutMsg[nOutMsgLen - 1] = '\0';
        delete pIGWB;
		return -1;
    }
    
    IBillProcessor* pIBillProcessor 
        = (IBillProcessor* )pIGWB->QueryInterface(IID_FP_IBILLPROCESSOR);
	if (NULL == pIBillProcessor)
    {
        strncpy(szOutMsg, "获得格式库查询接口失败", nOutMsgLen);
        szOutMsg[nOutMsgLen - 1] = '\0';
        delete pIGWB;
        return -1;
    }
    nResult = pIBillProcessor->Init(1);     
    if (0 != nResult)
    {
        strncpy(szOutMsg, "格式库转换接口初始化失败", nOutMsgLen);
        szOutMsg[nOutMsgLen - 1] = '\0';
        delete pIGWB;
        return -1;
    }*/

    delete pIGWB;

    return 0;
}
}

IGWB_Svc_Export int BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen)
{
    int i = 0;
    int nActLen = 0;

	//计算真实的长度
    for (; i < nLen; i++)
	{
        if ((pBCD[i] & 0x0F) == 0x0F || (pBCD[i] & 0xF0) == 0xF0)
        {
            break;
        }    
    }
    
    //考虑最后一个字节为 0xFF 的情况
    if ((pBCD[i] & 0xF0) == 0xF0)
    {
        nActLen = i;
    }
    else
    {
        nActLen = i + 1;
    }

    //Added by ZhengYuqun SWPD04257 2004-03-25
    if(nActLen > nLen)
    {
        nActLen = nLen;
    }
    //End

    for (i = 0; i < nActLen; i++)
	{
        pRBCD[i] = pBCD[i] << 4 | pBCD[i] >> 4;
	}

    return nActLen;
}
