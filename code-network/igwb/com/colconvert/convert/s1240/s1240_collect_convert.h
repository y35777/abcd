#ifndef __S1240_COLLECT_CONVERT_H__
#define __S1240_COLLECT_CONVERT_H__

/*
CCollectConvertBase�ǲɼ������Ļ���, �������˲ɼ�������صĻ������Ժͻ�������
*/
#include "../../include/collect_convert_include.h"
#include "../include/collect_convert_struct.h"

#include "../collect_convert_base.h"


//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
class  CS1240CollectConvert : public CCollectConvertBase
{

public:
    CS1240CollectConvert(int nSwitchIndex); //���캯������Э�����ָ��
    virtual ~CS1240CollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;

};

#endif //__COLLECT_CONVERT_S1240_H__

