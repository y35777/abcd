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
��������    ��ȡһ��3G��ԭʼ������¼����
�������    ��
�������    pData----������¼���ݵ�ָ�룬ʹ�ù̶��ڴ�m_pASN1RecContent
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼

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

    //3Gԭʼ�����Ĵ�ŷ�ʽΪ��2λ���ȣ���������
    int nSizeFieldLen;
    nSizeFieldLen = 2;  
   
    INT4 nRecordLen = 0;
    if (m_nMemDataLen - m_nCurOffset < nSizeFieldLen)
    //�ڴ�ӳ���ļ��е�ʣ�೤�Ȳ���һ���������ȵ���������������Ч
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

        //��������Ļ�������Ϊ0����˵�����ļ���ǰ���̵����һ���ļ���
        //����Ļ�����Ч
        if (0 == nRecordLen)
        {
            bRemain = FALSE;
            return TRUE;
        }
        else if (m_nMemDataLen - m_nCurOffset < nSizeFieldLen + nRecordLen)
        //ʣ������ݲ���һ�Ż�����������������Ч
        {
            bRemain = FALSE;
            return TRUE;
        }
    }

    m_nCurOffset += nSizeFieldLen;
    uDataLen = nRecordLen;

    //��3Gԭʼ��������ASN.1���н��룬��uDataLen�з��ؽ����ĳ���
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
��������    ��ȡһ��3G��ԭʼ������¼����
�������    ��
�������    pData----������¼���ݵ�ָ�룬ʹ�ù̶��ڴ�m_pASN1RecContent
            uDataLen----������¼�ĳ���
            bRemain----��ʾ�ļ����Ƿ��Ѿ�û�м�¼�ˣ�����
                        ����ΪFALSEʱ��pData��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
�޸ļ�¼

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

    //3G���ջ����Ĵ�ŷ�ʽΪ��4λ���ȣ���������
    int nSizeFieldLen;
    nSizeFieldLen = 4;  
    
    INT4 nRecordLen = 0;
    if (m_nMemDataLen - m_nCurOffset < nSizeFieldLen)
    //�ڴ�ӳ���ļ��е�ʣ�೤�Ȳ���һ���������ȵ���������������Ч
    {
        bRemain = FALSE;
        return TRUE;
    }
    else  
    {
        //��ȡ�ļ��еĻ����ĳ��ȣ������������������ֽ�����,
        //���ڿ��ǵ�UNIX�µ��ֽڶ������⣬������ֱ�ӿ���������
        memcpy(&nRecordLen, (BYTE* )m_pMemFile + m_nCurOffset, 4);
        nRecordLen = NTOHL(nRecordLen);
        
        //��������Ļ�������Ϊ0����˵�����ļ���ǰ���̵����һ���ļ���
        //����Ļ�����Ч
        if (0 == nRecordLen)
        {
            bRemain = FALSE;
            return TRUE;
        }
        else if (m_nMemDataLen - m_nCurOffset < nSizeFieldLen + nRecordLen)
        //ʣ������ݲ���һ�Ż�����������������Ч
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
        //��3Gԭʼ��������ASN.1���н��룬��uDataLen�з��ؽ����ĳ���
	    nRet = m_PSAsnClosor.DeCodeBill_ini_mt(m_pASN1IniGlobal, 
                                               (char* )m_pMemFile + m_nCurOffset, 
                                               (int* )&uDataLen,
                                               (char* )m_pASN1RecContent,
                                               ASN1_BUF_SIZE);
    }
    else
    {
        //��3G���ջ�������ASN.1���н��룬��uDataLen�з��ؽ����ĳ���
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
