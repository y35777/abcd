#ifndef __COM_COLLECT_CONVERT_H__
#define __COM_COLLECT_CONVERT_H__

/*
CCollectConvertBase�ǲɼ������Ļ���, �������˲ɼ�������صĻ������Ժͻ�������
*/
#include "../../include/collect_convert_include.h"
#include "../include/collect_convert_struct.h"

#include "../collect_convert_base.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
class  CComCollectConvert : public CCollectConvertBase
{

public:
    CComCollectConvert(int nSwitchIndex); //���캯������Э�����ָ��
    virtual ~CComCollectConvert();

    //��ʼ���ɼ�����, ���麯��, �����������ʵ��
    virtual  int  Init() ;

};

#endif //__COM_COLLECT_CONVERT_H__

