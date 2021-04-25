// igwb_file.h: interface for the CIGWBFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGWB_FILE_H__B69AD699_8396_41C0_8C24_5B37B5572B02__INCLUDED_)
#define AFX_IGWB_FILE_H__B69AD699_8396_41C0_8C24_5B37B5572B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../include/precom.h"

class CIGWBFile  
{
public:
	//���캯��
	CIGWBFile(UINT4 uBillType, UINT4 uBillLen, int nAPID);

	//��������
	virtual ~CIGWBFile();

    //��ָ���Ļ����ļ���������Ӧ�Ķ�дλ��ָ�뺯��
    BOOL Open(const char* szPathFile, int nOffset);

	//��ȡ�ļ��ĵ�ǰ��дλ�ú���
    int GetCurOffset();      

	//�����ļ��Ķ�дָ��λ�ú���
    BOOL SetOffset(int nOffset);  

	//�ر��ļ�����
    BOOL Close();   

	//�����Ƿ�ϲ���־
    void    SetMustMerge(BOOL bMustMerge);
    void    SetFinalSPFileFmt(BOOL bFinalSPFileFmt);

    //��ȡһ�Ż�����¼���ݺ���
    virtual BOOL Read(void*& pData, UINT4& uDataLen, BOOL& bRemain);  

    //��ȡһ��ԭʼ������¼����
    virtual BOOL ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain); 
	
    //��ȡһ�����ջ���������¼����
    virtual BOOL ReadFinal(void*& pData, UINT4& uDataLen, BOOL& bRemain);  	 	
   
    inline int GetFileLen()
    {
        return m_nMemDataLen; 
    }
protected:
	
	//��������
    UINT4    m_uBillType;       
    
	//�����������ʽ��ԭ��ʽ����
    UINT4    m_uBillLen;        

	//�ļ���ָ��
    FILE*    m_pFileStream;     

	//��ǰ�򿪵Ļ����ļ���ȫ·��
    char     m_szFileFullPath[MAX_PATH];  

	//�ڴ�ӳ���ļ�ָ�룬��ʼ��ΪNULL
	BYTE*    m_pMemFile;    
    
	//�ڴ�ӳ���ļ��ĳ���
	int      m_nMemFileLen; 
	
	//�ڴ�ӳ���ļ����ݵĳ���
	int      m_nMemDataLen;   
	
	//�ڴ�ӳ���ļ��е�ǰ��дλ�õ�ƫ��������ʼ��Ϊ0 
    int      m_nCurOffset;    
	
	//���ջ����Ƿ��Ѻϲ�
	BOOL     m_bMustMerge;

	//���ջ����Ƿ��������ʽ
    BOOL     m_bFinalSPFileFmt;

    //������
	int      m_nAPID;
};

#endif // !defined(AFX_IGWB_FILE_H__B69AD699_8396_41C0_8C24_5B37B5572B02__INCLUDED_)
