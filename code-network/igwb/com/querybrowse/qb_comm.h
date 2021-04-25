#ifndef __QB_COMMON_H__
#define __QB_COMMON_H__

#include "../include/precom.h"
#include "resource.h"

#define MAX_COND_LEN 2048             //��ѯ��������󳤶�

enum STYLE
{
    STYLE_HORI, //����
    STYLE_VERI  //����
};

//�������͵�ö�ٶ���
enum TaskType
{
    BROWSE_TASK = 0,    //�������
    QUERY_TASK = 1      //��ѯ����
};

//��ѯ���ļ���Χ���͵�ö�ٶ���
enum QueryRangeType
{
    DATE_RANGE = 0,     //��ѯĳ���ڷ�Χ�ڵĻ���
    FILE_RANGE = 1      //��ѯĳ�ļ���Χ�ڵĻ���
};

//����ҳ״̬��ö��
enum PageState
{
    PREPARING,         //����׼��
    READY,             //�Ѿ�׼����
    DELETED            //�Ѿ�ɾ��
};

//��ѯ���ƫ��������¼���ϲ�ѯ�����Ļ�����¼�ڲ�ѯ�ļ��е�λ��
struct ResultOffset
{
    INT2 nFileNo; //��ѯ�ļ��ڲ�ѯ�ļ��б�ı��
    //INT2 nBillLen;//��������
    INT4 nOffset; //��ѯ����ڲ�ѯ�ļ��е�ƫ����
};

#endif
