#ifndef __COLLECT_CONVERT_STRUCT_H__
#define __COLLECT_CONVERT_STRUCT_H__


#include "../../../include/base_type.h"
//�������ݽṹ����

//���ݽṹ����
//�ṹ����SColFileReq���ڶ���ɼ������ļ�ʱ������������������£�
struct SConvertFileReq
{
    char  szSrcName[MAX_PATH];//Դ�ļ���
    char  szFinalName[MAX_PATH];//Ŀ���ļ���
    char  szSecondFinalName[MAX_PATH];
    char  szErrName[MAX_PATH];
    int   nConvertInfo;//�ļ�����
};


#endif //__COLLECT_CONVERT_STRUCT_H__
