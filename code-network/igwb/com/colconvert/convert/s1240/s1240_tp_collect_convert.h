#ifndef __S1240_TP_COLLECT_CONVERT_H__
#define __S1240_TP_COLLECT_CONVERT_H__

/*
CS1240TPCollectConvert������S1240�ɼ�������صĻ������Ժͻ�������
*/
#include "../collect_convert_subobj.h"

//�����ǲɼ������Ļ���CS1240TPCollectConvert�Ķ���
class  CS1240TPCollectConvert : public CCollectConvertSubObj 
{

public:
    CS1240TPCollectConvert(int nSwitchIndex, 
                           int nTPIndex, 
                           IBillProcessor* pIBillProcessor = NULL); //���캯������Э�����ָ��
    virtual ~CS1240TPCollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;

};

#endif //__S1240_TP_COLLECT_CONVERT_H__

