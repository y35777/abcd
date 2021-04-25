/*****************************************************
//		utility_ex.cpp
//*****************************************************
// Copyright 2002 by Huawei CO.,LTD.
// All rights reserved.
// Compiler:     Visual C++ 6.0
// Author:       maimaoshi
// Date:         2002-03-18
// Description:  ���ļ�ʵ����GPRS����3G��������Ĺ��ߺ���
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
 
static ACE_Recursive_Thread_Mutex g_global_buf_Lock;  //ʹ��ȫ�ֱ����Ļ�����
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

//�ַ������飬BoolToStr�õ�
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
// Description:   �������İ�λ�����ݰ�BCD��ĸ�ʽת�����ַ���
// Input:
//       arg1 :LPOCTECSTRING pOctecString	����İ�λ������
//       arg2 :char *pBuffer				��ŷ����ַ�����������
//       arg3 :int nLength					���������ó���
// Return value: int						ת������ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int BCDOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nDataLength = pOctecString->nLength;
	BYTE nData, nTemp;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;			//��ʼ���������

	if (nLength <= (nDataLength * 2))
		return 0;

	for (int i = 0; i < nDataLength; i++)
	{
		//ȡһ���ֽ�����
		nData = pOctecString->pData[i];
		
		//�������λ
		nTemp = nData >> 4;
		if (nTemp > 0x09)
			break;
		pBuffer[nLen++] = nTemp + '0';

		//�������λ
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
// Description:   �������İ�λ�����ݰ�����BCD��ĸ�ʽת�����ַ���
// Input:
//       arg1 :LPOCTECSTRING pOctecString	����İ�λ������
//       arg2 :char *pBuffer				��ŷ����ַ�����������
//       arg3 :int nLength					���������ó���
// Return value: int						ת������ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int CvtBCDOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nDataLength = pOctecString->nLength;
	BYTE nData, nTemp;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;			//��ʼ���������

	if (nLength <= (nDataLength * 2))
		return 0;

	for (int i = 0; i < nDataLength; i++)
	{
		//ȡһ���ֽ�����
		nData = pOctecString->pData[i];
		
		//�������λ
		nTemp = nData & 0x0f;
		if (nTemp > 0x09)
			break;
		pBuffer[nLen++] = nTemp + '0';

		//�������λ
		nTemp = nData >> 4;
		if (nTemp > 0x09)
			break;
		pBuffer[nLen++] = nTemp + '0';
	}
	
	pBuffer[nLen] = 0;

	return nLen;
}

//by ldf 2004-03-26 ��Ӧ���ⵥSWPD03933
/******************************************************
// Function name: CvtBCDOctecToStr2
// Author:        lidufang
// Date:          2003-03-03
// Description:   �����������ݰ�����BCD��ĸ�ʽת�����ַ���,����F���������
//                ����pLMNIdentifier 64F000 ��ת����ʾΪ46000
// Input:
//       arg1 :LPOCTECSTRING pOctecString	���������
//       arg2 :char *pBuffer				��ŷ����ַ�����������
//       arg3 :int nLength					���������ó���
// Return value: int						ת������ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int CvtBCDOctecToStr2(LPOCTECSTRING pOctecString, char *pBuffer, int nLength)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nDataLength = pOctecString->nLength;
	BYTE nData, nTemp;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;			//��ʼ���������

	if (nLength <= (nDataLength * 2))
		return 0;

	for (int i = 0; i < nDataLength; i++)
	{
		//ȡһ���ֽ�����
		nData = pOctecString->pData[i];
		
		//�������λ
		nTemp = nData & 0x0f;
		if (nTemp <= 0x09)
        {
            pBuffer[nLen++] = nTemp + '0';
        }

		//�������λ
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
// Description:   �������İ�λ�����ݰ�ʮ�������ĸ�ʽת�����ַ���
// Input:
//       arg1 :LPOCTECSTRING pOctecString	����İ�λ������
//       arg2 :char *pBuffer				��ŷ����ַ�����������
//       arg3 :int nLength					���������ó���
//       arg4 :int bSigned					��ת�������Ƿ��з�����
// Return value: int						ת������ַ����ĳ���
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
// Description:   �������İ�λ�����ݰ����������ĸ�ʽת�����ַ���
// Input:
//       arg1 :LPOCTECSTRING pOctecString	����İ�λ������
//       arg2 :char *pBuffer				��ŷ����ַ�����������
//       arg3 :int nLength					���������ó���
//       arg4 :char *pPrefix				ǰ׺�ַ���
//       arg5 :BOOL bReverse				ת����ʱ���Ƿ���ת��������ֽ�
// Return value: int						ת�����ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int BinOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nPrefixLen = pPrefix ? strlen(pPrefix) : 0;
	BYTE nData;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLen + (pOctecString->nLength * 8)))
		return 0;

	//����ǰ׺�ַ���
	if (pPrefix)
	{
		strcpy(pBuffer, pPrefix);
		nLen = nPrefixLen;
	}

	int nDataLength = pOctecString->nLength;
	int i, j;

	for (i = 0; i < nDataLength; i++)
	{
		// ����ת����˳��Ĳ�ͬ��ȡһ���ֽڵ�����
		if (bReverse)
			nData = pOctecString->pData[nDataLength - i - 1];
		else
			nData = pOctecString->pData[i];

		//��һ���ֽڵ����ݽ���ת��
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
// Description:   �������İ�λ�����ݰ�ʮ���������ĸ�ʽת�����ַ���
// Input:
//       arg1 :LPOCTECSTRING pOctecString	����İ�λ������
//       arg2 :char *pBuffer				��ŷ����ַ�����������
//       arg3 :int nLength					���������ó���
//       arg4 :char *pPrefix				ǰ׺�ַ���
//       arg5 :BOOL bReverse				ת����ʱ���Ƿ���ת��������ֽ�
// Return value: int						ת�����ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int HexOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nPrefixLen = pPrefix ? strlen(pPrefix) : 0;
	BYTE nData;

	ASSERT((BOOL)(pOctecString && pBuffer));

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLen + (pOctecString->nLength * 2)))
		return 0;

	//����ǰ׺�ַ���
	if (pPrefix)
	{
		nLen = nPrefixLen;
		strcpy(pBuffer, pPrefix);
	}

	int nDataLength = pOctecString->nLength;

	for (int i = 0; i < nDataLength; i++)
	{
		// ����ת����˳��Ĳ�ͬ��ȡһ���ֽڵ�����
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
// Description:   �������İ�λ�����ݰ�ʮ���������ĸ�ʽת�����ַ���
//                
// Input:
//       arg1 :int nValueLength  ��ʾpValue����Ч�ֽڵĳ���
         arg2 : const unsigned char* pValue �����Ҫת��������
//       arg3 :char *pBuffer				��ŷ����ַ�����������
//       arg4 :int nLength					���������ó���
//       arg5 :char *pPrefix				ǰ׺�ַ���
//       arg6 :BOOL bReverse				ת����ʱ���Ƿ���ת��������ֽ�
// Return value: int						ת�����ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 2004-05-27      YanYong               ���ⵥSWPD05340
******************************************************/
int IGWB_Svc_Export HexOctecToStr(int nValueLength, const unsigned char* pValue, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse)
{
    int nLen = 0;			//���ص���������д������ݳ���
    int nPrefixLen = pPrefix ? strlen(pPrefix) : 0;
    BYTE nData;
    
    ASSERT((BOOL)(pValue && pBuffer));
    
    pBuffer[0] = 0;
    
    if (nLength <= (nPrefixLen + (nValueLength * 2)))
        return 0;
    
    //����ǰ׺�ַ���
    if (pPrefix)
    {
        nLen = nPrefixLen;
        strcpy(pBuffer, pPrefix);
    }
    
    int nDataLength = nValueLength;
    
    for (int i = 0; i < nDataLength; i++)
    {
        // ����ת����˳��Ĳ�ͬ��ȡһ���ֽڵ�����
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
// Description:   �������İ�λ�����ݰ�IP��ַ�ĸ�ʽת�����ַ���
// Input:
//       arg1 :LPOCTECSTRING pOctecString	����İ�λ������
//       arg2 :char *pBuffer				��ŷ����ַ�����������
//       arg3 :int nLength					���������ó���
//       arg4 :BYTE nStyle					ת�������ͣ���IPV4����IPV6��ȡֵ����
//				#define BINIPV4_STYLE			1			IPV4������
//				#define BINIPV6_STYLE			2			IPV6������
// Return value: int						ת������ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int OctecToIPAddrStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, BYTE nStyle)
{
	int nLen = 0;			//���ص���������д������ݳ���	
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
// Description:   �������İ�λ������ת�����ַ���
// Input:
//       arg1 :LPOCTECSTRING pOctecString	����İ�λ������
//       arg2 :char *pBuffer				��ŷ����ַ�����������
//       arg3 :int nLength					���������ó���
// Return value: int						ת������ַ����ĳ���
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
// Description:   ��BOOL�͵����ݰ�����������ת�����ַ���
// Input:
//       arg1 :BOOL bValue		��ת�������ݵ�ֵ
//       arg2 :char *pBuffer	��ŷ����ַ�����������
//       arg3 :int nLength		���������ó���
//       arg4 :BYTE nStyle		ת�������ͣ�ȡֵ����
//				#define BOOLBROWSE_NUMSTYLE		0			��ʾ"0"��"1"
//				#define BOOLBROWSE_UPERSTYLE	1			��ʾ"FALSE"��"TRUE"
//				#define BOOLBROWSE_LOWERSTYLE	2			��ʾ"false"��"true"
//				#define BOOLBROWSE_YESNOSTYLE	3			��ʾ"No"��"Yes"
// Return value: int			ת������ַ����ĳ���
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
// Description:   ����������ʮ����ת�����ַ���
// Input:
//       arg1 :long nNumber		��ת������
//       arg2 :char *pBuffer	��ŷ����ַ�����������
//       arg3 :int nLength		���������ó���
//       arg4 :BOOL bSigned		��ת�����Ƿ��з�������
// Return value: int			ת������ַ����ĳ���
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
// Description:   ����������������ת�����ַ���
// Input:
//       arg1 :UINT4 nNumber	��ת������
//       arg2 :char *pBuffer	��ŷ����ַ�����������
//       arg3 :int nLength		���������ó���
//       arg4 :char *pPrefix	ǰ׺�ַ������������Ҫǰ׺����NULL
// Return value: int			ת������ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int NumToBinStr(UINT4 nNumber, char *pBuffer, int nLength, const char *pPrefix)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nPrefixLength = pPrefix ? strlen(pPrefix) : 0;

	ASSERT((BOOL)pBuffer);

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLength + 32))
		return 0;

	//����ǰ׺�ַ���
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
// Description:   ����������������ת�����ַ���
// Input:
//       arg1 :char *pData		��ת�������ݴ��λ��ָ��
//       arg2 :int nDataLength	��ת�������ݵĳ���(BYTE)
//       arg3 :char *pBuffer	��ŷ����ַ�����������
//       arg4 :int nLength		���������ó���
//       arg5 :char *pPrefix	ǰ׺�ַ���
// Return value: int			���ص��ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int DataToBinStr(char *pData, int nDataLength, char *pBuffer, int nLength, const char *pPrefix)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nPrefixLength = pPrefix ? strlen(pPrefix) : 0;
	BYTE nData;

	ASSERT((BOOL)(pData && pBuffer));

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLength + (nDataLength * 8)))
		return 0;

	//����ǰ׺�ַ���
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
// Description:   ����������ʮ������ת�����ַ���
// Input:
//       arg1 :UINT4 nNumber	��ת������
//       arg2 :char *pBuffer	��ŷ����ַ�����������
//       arg3 :int nLength		���������ó���
//       arg4 :char *pPrefix	ǰ׺�ַ���
// Return value: int			���ص��ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
//2002-03-19       maimaoshi
******************************************************/
IGWB_Svc_Export int NumToHexStr(UINT4 nNumber, char *pBuffer, int nLength, const char *pPrefix)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nPrefixLength = pPrefix ? strlen(pPrefix) : 0;
	
	ASSERT((BOOL)pBuffer);

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLength + 8))
		return 0;

	//����ǰ׺�ַ���
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
// Description:   ����������ʮ������ת�����ַ���
// Input:
//       arg1 :char *pData		��ת��������
//       arg2 :int nDataLength	��ת�������ݵĳ���
//       arg3 :char *pBuffer	��ŷ����ַ�����������
//       arg4 :int nLength		���������ó���
//       arg5 :char *pPrefix	ǰ׺�ַ���
// Return value: int			���ص��ַ����ĳ���
// Notes: 
// Update: 
// Date            Name                  Description 
   ========== ================== ======================
// 
******************************************************/
IGWB_Svc_Export int DataToHexStr(char *pData, int nDataLength, char *pBuffer, int nLength, const char *pPrefix)
{
	int nLen = 0;			//���ص���������д������ݳ���
	int nPrefixLength = pPrefix ? strlen(pPrefix) : 0;
	BYTE nData;

	ASSERT((BOOL)(pBuffer && pData));

	pBuffer[0] = 0;

	if (nLength <= (nPrefixLength + (nDataLength * 2)))
		return 0;

	//����ǰ׺�ַ���
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
// Description: ��TimeStamp���͵�����ת�����ַ���  
// Input:
//       arg1 :LPTIMESTAMP pTimeStamp	��ת����TimeStamp�������ݵ�ָ��
//       arg2 :char *pBuffer			��ŷ����ַ�����������
//       arg3 :int nLength				���������ó���
//       arg4 :BOOL bFormat				��ʱ���ʽ��Ϊ��YYYY-MM-DD HH:MM:SS+HH:MM���ĸ�ʽ
//                                      ��"YYYY/MM/DD"��Ϊ"YYYY-MM-DD" By XDB on 2001.11.09
// Return value: int					ת������ַ����ĳ���
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
// Description:   ��Enumֵ����������Ϣת���ɶ�Ӧ���ַ���
// Input:
//       arg1 :LPENUMINFO pEnumInfo	ö����Ϣ�ṹ��ָ��
//       arg2 :int nEnumValue		��ת����ö��ֵ
//       arg3 :char *pBuffer		��ŷ����ַ�����������
//       arg4 :int nLength			���������ó���
// Return value: int				ת������ַ����ĳ���
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

    //��ʼ����ʽ��ӿ�
    STRING strIGWBPath = szPath;
    CIGWB* pIGWB = new CIGWB(strIGWBPath);
    if(NULL == pIGWB)
    {
        strncpy(szFormatCfgErr, "����CIGWB����ʧ��", sizeof(szFormatCfgErr));
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
    //��ø�ʽ���ѯ�ӿ�
    IQueryBrowse* pIQueryBrowse = (IQueryBrowse*)pIGWB->QueryInterface(IID_FP_IQUERYBROWSE);
	if (NULL == pIQueryBrowse)
    {
        strncpy(szOutMsg, "��ø�ʽ���ѯ�ӿ�ʧ��", nOutMsgLen);
        szOutMsg[nOutMsgLen - 1] = '\0';
        delete pIGWB;
		return -1;
    }
    nResult = pIQueryBrowse->Init(0);
    if (0 != nResult)
    {
        strncpy(szOutMsg, "��ʽ���ѯ�ӿڳ�ʼ��ʧ��", nOutMsgLen);
        szOutMsg[nOutMsgLen - 1] = '\0';
        delete pIGWB;
		return -1;
    }
    
    IBillProcessor* pIBillProcessor 
        = (IBillProcessor* )pIGWB->QueryInterface(IID_FP_IBILLPROCESSOR);
	if (NULL == pIBillProcessor)
    {
        strncpy(szOutMsg, "��ø�ʽ���ѯ�ӿ�ʧ��", nOutMsgLen);
        szOutMsg[nOutMsgLen - 1] = '\0';
        delete pIGWB;
        return -1;
    }
    nResult = pIBillProcessor->Init(1);     
    if (0 != nResult)
    {
        strncpy(szOutMsg, "��ʽ��ת���ӿڳ�ʼ��ʧ��", nOutMsgLen);
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

	//������ʵ�ĳ���
    for (; i < nLen; i++)
	{
        if ((pBCD[i] & 0x0F) == 0x0F || (pBCD[i] & 0xF0) == 0xF0)
        {
            break;
        }    
    }
    
    //�������һ���ֽ�Ϊ 0xFF �����
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
