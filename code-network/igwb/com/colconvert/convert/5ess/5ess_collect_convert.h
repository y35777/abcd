#ifndef __5ESS_COLLECT_CONVERT_H__
#define __5ESS_COLLECT_CONVERT_H__

/*
CCollectConvertBase�ǲɼ������Ļ���, �������˲ɼ�������صĻ������Ժͻ�������
*/
#include "../../include/collect_convert_include.h"
#include "../include/collect_convert_struct.h"

#include "../collect_convert_base.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
class  C5essCollectConvert : public CCollectConvertBase
{

public:
    C5essCollectConvert(int nSwitchIndex); //���캯������Э�����ָ��
    virtual ~C5essCollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;
};

#endif //__5ESS_COLLECT_CONVERT_H__

