#ifndef __CDR_STAT_FILE_H__
#define __CDR_STAT_FILE_H__

#include "../include/toolbox.h"

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

class CIGWBFile;
class CCDRStatFile
{
public:
	CCDRStatFile();
	virtual ~CCDRStatFile();

public:

	//���ò��������г�ʼ������
    int Init(UINT4 uAPType,
             UINT4 uBillType,
             BOOL  bMustMerge, 
             BOOL  bFinalSPFileFmt, 
		     UINT4 uBillLen,
             int nAPID);

    //��ָ���Ļ����ļ�
    BOOL Open(const char* const szPathFile);    

	//�ر��ļ�
	BOOL Close();

	//��ȡ��һ�Ż�����¼����
    int  GetNextBill(BYTE*& pCDRRecord, UINT4& nCDRLen, BOOL&  bRemain);
	
protected:
	
	//�����ļ����͵�ö�ٶ���
	enum BillType
	{
		ORIG_BILL = 0,      //ԭʼ����
		FINAL_BILL = 1      //���ջ���
	};

    //IGWB�ļ�ָ��
    CIGWBFile*   m_pIGWBFile;
};
#endif //__CDR_STAT_FILE_H__

