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
��������    ��ȡһ��ZTE�Ļ�����¼����
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
BOOL CZxjFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
	//ZTE��������ͨ���ж��ļ�ǰ2���ֽ����жϰ汾�ţ��жϻ�������
	//1��c8 00(200)��ʾV300�굥������Ϊ93�ֽ�
	//2��2c 01(300)��ʾV302�굥������Ϊ123�ֽ�
	//3��c8(200)��ʾV303�ƴα�����Ϊ98�ֽ�
	//4��c9(201)��ʾV304�ƴα�����Ϊ170�ֽ�

	//��һ�ζ�ʱҪ��������
	if (m_nCurOffset == 0)
	{		
		//�ȷ���1�ֽ�
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
			//�ٷ���2�ֽ�
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


	//ʣ������ݲ���һ�Ż�����������������Ч
	if (m_nMemDataLen - m_nCurOffset < m_uBillLen)    
    {
        bRemain = FALSE;
        return TRUE;
    }
	
	BYTE* pBillRecord = NULL;
	pBillRecord = m_pMemFile + m_nCurOffset;
    //����������Ļ�������ȫ��Ϊ0����˵�����ļ���ǰ���̵����һ��������
    //�����Ļ��������Լ����ļ��ĺ������ݱ��붪������bRemain����ΪFALSE

    BOOL bValid = FALSE;
    //�ж��Ƿ�����Ļ������ݶ���0
    for (int i = 0; i < m_uBillLen; i++)
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
        m_nCurOffset += m_uBillLen;
	pData = pBillRecord ;
        uDataLen = m_uBillLen;
        bRemain = TRUE;
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
BOOL CZxjFile::ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    if(!m_bFinalSPFileFmt)
    {
        return CIGWBFile::ReadFinal(pData, uDataLen, bRemain);
    }
    
    return ReadOriginal(pData, uDataLen, bRemain);
}
