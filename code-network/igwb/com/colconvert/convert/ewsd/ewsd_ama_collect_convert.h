#ifndef __EWSD_AMA_COLLECT_CONVERT_H__
#define __EWSD_AMA_COLLECT_CONVERT_H__

/*
CEWSDAMACollectConvert������EWSDAMA�ɼ�������صĻ������Ժͻ�������
*/
#include "../collect_convert_subobj.h"

//�����ǲɼ������Ļ���CEWSDAMACollectConvert�Ķ���
class  CEWSDAMACollectConvert : public CCollectConvertSubObj 
{

public:
    CEWSDAMACollectConvert(int nSwitchIndex, 
                           int nAMAIndex,
                           IBillProcessor* pIBillProcessor = NULL); //���캯������Э�����ָ��
    virtual ~CEWSDAMACollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;
};

#endif //__EWSD_AMA_COLLECT_CONVERT_H__

