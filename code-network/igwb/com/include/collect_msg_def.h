#ifndef __COLLECT_MESSAGEDEF_H__
#define __COLLECT_MESSAGEDEF_H__

#include "../include/precom.h"

struct SSwitchFileInfo
{
    int   nSwitchIndex;               //��������������
    char  szModuleLabel[50];          //ģ����Ϣ
    char  szRemoteFile[MAX_PATH];     //�������ϵ�ԭʼ�ļ���
    char  szLocalFile[MAX_PATH];      //Ҫת���Ļ����ļ�·��
    int   nConvertInfo;               //�����ļ�������
};

//�ɼ�ģ�鷢���ɼ�ת��ģ��"ת�������ļ�"��Ϣ����������
struct SCollectConvertFileInfo
{
    int   nFileNum;             
    SSwitchFileInfo info;
};
  
#define INFO_CONVERT_FILE_LENGTH  (sizeof(SCollectConvertFileInfo))

//�ɼ�ת��ģ�鷢���ɼ�ģ��"ת�������ļ�Ӧ��"��Ϣ����������
struct SCollectConvertResult
{
    int   nFileNum;
    int   nConvertResult;                    //ת�����
};

#define COL_RECOL_BILL      40001
#define COL_WRITE_BILL_FAIL 40002



#endif //__COLLECT_MESSAGEDEF_H__
