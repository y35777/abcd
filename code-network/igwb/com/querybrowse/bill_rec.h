#ifndef __BILLRECORD_H__
#define __BILLRECORD_H__

#include "../include/precom.h"
#include "../include/format_out.h"

class CBillRecord
{
public:
    CBillRecord();   //ȱʡ���캯��
    virtual ~CBillRecord();  //��������

    //���û�����¼�������ݺ���
    BOOL SetContent(BYTE* pData, UINT4 uLen, CFormat* pFormat, ACE_Mutex* pMutex = NULL);
    int GetContent(LIST<STRING>& ContentList);  //��ȡ������¼����

//����Զ2002.3.30��ӣ����ⵥ��D12265
protected:
    void SetErrorContent(CFormat* pFormat);     //�ڻ������������ô�����Ϣ
//����Զ2002.3.30��ӽ���

    BOOL IsValidStr(const char* szStr, int nStrLen);

protected:
    LIST<STRING> m_ContentList; //�����е�������ݵ��б��ı����ݣ�
};

#endif
