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
��������    ��ȡһ��S1240��ԭʼ������¼����
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
BOOL CS1240File::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    //S1240J����ǰ����20�ֽ�ͷ��Ϣ(��14��ʾ��ʵ�ʳ���)
    //ͨ��ÿ�Ż�����1�ֽ�ǰ��4λʶ�𻰵����ͣ���û�������

	//��һ�ζ�ʱҪ���ʵ�ʳ���
	if (m_nCurOffset % 2048 == 0)
	{
		//��2�ֽڿ�ʵ�ʳ���
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
    //�ڴ�ӳ���ļ��е�ʣ�೤�Ȳ���һ���������ȵ���������������Ч
    {
        bRemain = FALSE;
        return TRUE;
    }
    else  
    {
        BYTE nBillType = 0;
        memcpy(&nBillType, (BYTE* )m_pMemFile + m_nCurOffset, 1);

		//��û�������
        nRecordLen = GetRecordLen(&nBillType);
        
        if (m_nMemDataLen - m_nCurOffset < nRecordLen)
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

	if ((m_nCurOffset + m_nBlockLeftLen) % 2048 == 0)
	{
		m_nCurOffset += m_nBlockLeftLen;
	}
  
    return TRUE;
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
BOOL CS1240File::ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    if(!m_bFinalSPFileFmt)
    {
        return CIGWBFile::ReadFinal(pData, uDataLen, bRemain);
    }
    
    return ReadOriginal(pData, uDataLen, bRemain);
}


/**********************************************************************
��������    ���ݻ������ͻ�û�������
�������    ��
�������    
����ֵ      ��¼����
�׳��쳣    ��
�޸ļ�¼
maimaoshi,2003-06-03
**********************************************************************/

INT2 CS1240File::GetRecordLen(BYTE* pData)
{
	BYTE nTemp;
	BYTE nBillType = *pData;
	//�������λ
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
