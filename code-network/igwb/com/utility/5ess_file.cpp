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
��������    ��ȡһ��5ESS��ԭʼ������¼����
�������    ��
�������    pData----������¼���ݵ�ָ�룬ʹ�ù̶��ڴ�
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼
maimaoshi,2003-06-03
**********************************************************************/
BOOL C5essFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    BOOL nBillClass = 0;

	//�ֱ�����������ͣ��굥0���Ǽƴα�1��
	if (m_nCurOffset == 0)
	{
		char* pStart = NULL;
		char* pSubrgd = NULL;

		pStart  = strstr((char*)m_pMemFile, "START");		
		pSubrgd = strstr((char*)m_pMemFile, "SUBRGD");

		if ((pStart != NULL) && (pSubrgd != NULL))
		{
			nBillClass = 1;

			//�Ƶ���Ч���ݶ�
			m_nCurOffset =  (BYTE*)pSubrgd - m_pMemFile;
		}
		else
		{
			nBillClass = 0;

			//5ess����ǰ����40�ֽ�ͷ��Ϣ,��һ�ζ�ʱҪ�Ƶ�40�ֽڴ�
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
��������    ��ȡһ�����ջ���������¼����
�������    ��
�������    pData---- ������¼���ݵ�ָ�룬ʹ�ù̶����ڴ�
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
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
��������    ��ȡһ��5ESS��ԭʼ��ϸ������¼����
�������    ��
�������    pData----������¼���ݵ�ָ�룬ʹ�ù̶��ڴ�
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼
maimaoshi,2003-06-03
**********************************************************************/
BOOL C5essFile::ReadOriginal_Detail(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{   
	//ÿ�Ż���ǰ��4���ֽڱ�ʾ�˻�������(Ҳ��ʶ��������)

	INT4 nRecordLen = 0;
    if (m_nMemDataLen - m_nCurOffset < 4)
    //�ڴ�ӳ���ļ��е�ʣ�೤�Ȳ���һ���������ȵ���������������Ч
    {
        bRemain = FALSE;
        return TRUE;
    }
    else  
    {
        //��ȡ�ļ��еĻ����ĳ��ȣ������������������ֽ�����,
        //���ڿ��ǵ�UNIX�µ��ֽڶ������⣬������ֱ�ӿ���������

		BYTE  pTmp[4];
        memcpy(&pTmp, (BYTE* )m_pMemFile + m_nCurOffset, 4);
        nRecordLen = GetRecordLen(pTmp);
        
        //��������Ļ�������Ϊ0����˵�����ļ���ǰ���̵����һ���ļ���
        //����Ļ�����Ч
        if (0 == nRecordLen)
        {
            bRemain = FALSE;
            return TRUE;
        }
        else if (m_nMemDataLen - m_nCurOffset < nRecordLen)
        //ʣ������ݲ���һ�Ż�����������������Ч
        {
            bRemain = FALSE;
            return TRUE;
        }
    }

    BYTE* pBillRecord = NULL;
    pBillRecord = m_pMemFile + m_nCurOffset;
    //����������Ļ�������ȫ��Ϊ0����˵�����ļ���ǰ���̵����һ��������
    //�����Ļ��������Լ����ļ��ĺ������ݱ��붪������bRemain����ΪFALSE

    BOOL bValid = FALSE;
    //�ж��Ƿ�����Ļ������ݶ���0
    for (int i = 0; i < nRecordLen; i++)
    {
        if (0 != pBillRecord[i])
        {
            bValid = TRUE;
            break;
        }
	}
      
    //����������ݶ���0����˵���û�������Ч�ģ����붪��
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
��������    ��ȡһ��5ESS��ԭʼ�ƴα�����¼����
�������    ��
�������    pData----������¼���ݵ�ָ�룬ʹ�ù̶��ڴ�
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼
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

	//�ڴ�ӳ���ļ��е�ʣ�೤��С�ڵ���0����ʾ�����ļ�β
	if (m_nMemDataLen - m_nCurOffset <= 0)
    {
        bRemain = FALSE;
        return TRUE;
    }

	//˵�������ơ�SUBRGD 50332000,������,������m_MetertValue
	if ((pSubrgd != NULL) && (pLRLN != NULL) && (pLRLN - pSubrgd > 0))
	{
		m_nCurOffset = m_nCurOffset + (pLRLN - pTemp + 1);

		//����"SUBRGD "
		pSubrgd = pSubrgd + 7;
		//���˵���,��
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
    int    nIndex    = 0;      //�ı���ʽ��BCD���ַ�������
    int    nBcdIndex = 0;      //BCD����������
    BYTE   yCurBcd   = 0;      //��ǰBCD��ֵ
    BYTE*  pBcdArray = NULL;   //BCD������ָ��
    char   pCurStr[9];   //�ı���ʽ��BCD���ַ���ָ��

	//��ʼ��BCD������ָ����ı���ʽ��BCD���ַ���ָ��
    pBcdArray = pData;

    //˳��ɨ��BCD�������ÿһ��Ԫ��
    while(nBcdIndex < 4)
    {
        //��ȡ��ǰBCD���ֵ
        yCurBcd = pBcdArray[nBcdIndex++];

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0xF0)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }

        //���ε�ǰBCD������ֵ�ĸ���λ
        yCurBcd &= 0x0F;

        //�������λ��������BCD��ֵ����С��16
        if(yCurBcd < 0x0F)
        {
            //�����ǰBCD��ֵС��10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //�����ǰBCD��ֵ����10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //����BCD���������λ��
        else
        {
            break;
        }
    }

    //�����ַ����Ľ�����
    pCurStr[nIndex] = '\0';

	return atoi(pCurStr);
}