#ifndef __EWSD_AMAS_COLLECT_CONVERT_H__
#define __EWSD_AMAS_COLLECT_CONVERT_H__

/*
CEWSDAMASCollectConvert��ewsdama�ɼ�������, �������˲ɼ�������صĻ������Ժͻ�������
*/
#include "../../include/collect_convert_include.h"
#include "../include/collect_convert_struct.h"

#include "../collect_convert_base.h"

//�����ǲɼ���������CEWSDAMASCollectConvert�Ķ���
class  CEWSDAMASCollectConvert : public CCollectConvertBase
{

public:
    CEWSDAMASCollectConvert(int nSwitchIndex); //���캯������Э�����ָ��
    virtual ~CEWSDAMASCollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;

};

#endif //__EWSD_AMAS_COLLECT_CONVERT_H__

