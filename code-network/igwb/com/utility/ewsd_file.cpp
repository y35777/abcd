// ewsd_file.cpp: implementation of the CEwsdFile class.
//
//////////////////////////////////////////////////////////////////////

#include "ewsd_file.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEwsdFile::CEwsdFile(UINT4 uBillType, UINT4 uBillLen, int nAPID)
                    :CIGWBFile(uBillType,uBillLen,nAPID)
{
	//
}

CEwsdFile::~CEwsdFile()
{

}
/**********************************************************************
��������    ��ȡһ��EWSD��ԭʼ������¼����
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
BOOL CEwsdFile::ReadOriginal(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    
    bRemain = FALSE;

    return TRUE;
}