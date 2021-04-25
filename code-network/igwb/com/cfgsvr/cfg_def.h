#ifndef __CFG_DEF_H__
#define __CFG_DEF_H__

#include "../include/precom.h"
#include "../include/toolbox.h"
#include "../include/frame.h"

//MAX_LINEΪ��ȡһ�е���󳤶�
const int MAX_LINE = 1024;
//������󳤶�
const int MAX_SECTNAME_LEN = 64;
//����������󳤶�
const int MAX_ITEMNAME_LEN = 32;
//����ֵ��󳤶�
const int MAX_ITEMVALUE_LEN = 256;
//��������Ϣ��������󳤶�
const int MAX_ITEMDESC_LEN = 256;

//�����ļ����Ͷ��壺0--��������(.bas)��1--�ϴ�����(.lst), 2--��ǰ����(.ini)
#define FILE_TYPE_FAC 0
#define FILE_TYPE_SUC 1
#define FILE_TYPE_CUR 2

#define SECTLIST   "SectList"	
#define RELATION   "Relation"
#define TYPE       "Type"
#define DESC       "Desc"
#define MAX        "Max"
#define MIN        "Min"
#define DEFAULT    "Default"
#define MUSTFLAG   "MustFlag"
#define UPDATEFLAG "UpdateFlag"

#endif //__CFG_DEF_H__
