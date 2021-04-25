#ifndef __5ESS_PREFIX_COLLECT_CONVERT_H__
#define __5ESS_PREFIX_COLLECT_CONVERT_H__

/*
C5essPrefixCollectConvert������5ess�ɼ�������صĻ������Ժͻ�������
*/
#include "../collect_convert_subobj.h"

//�����ǲɼ���������C5essPrefixCollectConvert�Ķ���
class  C5essPrefixCollectConvert : public CCollectConvertSubObj
{

public:
    C5essPrefixCollectConvert(int nSwitchIndex, 
                              int nPrefixIndex,
                              IBillProcessor* pIBillProcessor = NULL); //���캯������Э�����ָ��
    virtual ~C5essPrefixCollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;

};

#endif //__5ESS_PREFIX_COLLECT_CONVERT_H__

