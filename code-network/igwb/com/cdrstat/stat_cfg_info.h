#ifndef __STAT_CFG_INFO_H__
#define __STAT_CFG_INFO_H__

#include "../include/toolbox.h"

#pragma warning(disable :66)
#pragma warning(disable :21)
#pragma warning(disable :46)

#define MSG_BUFFER_MAXSIZE   500 * 1024

//ͳ�������ļ�ͷ��Ϣ�ĳ���
#define STAT_CFG_HEAD_LENGTH    20

#define INCMEMLENGTH         1024 * 400

//ͳ�������ļ�ͷ��Ϣ
struct SStatCfgHead
{
    UINT4 uVersion;       //�ļ��İ汾��
    UINT4 uAccessPoint;   //������
    UINT4 uStatItemCount; //ͳ����Ŀ����
    BYTE  yReserved[8];   //�����ֽ�
};


//ͳ������������Ϣ�ĳ���
#define SUBITEMCFGLENGTH     576

//ͳ������������Ϣ
struct SStatSubItemCfg
{
    char  szStatSubItemName[64];  //ͳ��������
    char  szStatSubItemCond[512]; //ͳ���������ʽ
};

//ͳ����Ŀͷ��Ϣ��ͷ����
#define STATITEMCFGLENGTH    68

//ͳ����Ŀͷ��Ϣ
struct SStatItemCfg
{
    char  szStatItemName[64];     //ͳ������
    UINT4 uStatItemCount;         //��ʾ����ͳ��������Ϣ�ĸ���
    SStatSubItemCfg subItemCfg[1];//ͳ������������Ϣ����

    void* operator new(size_t classSize, size_t nCount = 1)
    {
        void *ptr = NULL;

        if( nCount < 1)
        {
            nCount = 1;
        }

        UINT4 length = nCount * sizeof(SStatSubItemCfg) + 68;

        if (length > MSG_BUFFER_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new BYTE[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SStatItemCfg*)ptr)->uStatItemCount = nCount;
        }

        return ptr;
    };
};

//һ��ͳ�������������Ϣ
struct SOneStatSubItemCfg
{
    char szStatItemName[64];    //ͳ����Ŀ��
    char szStatSubItemName[64]; //ͳ��������
    char szStatSubItemCond[512]; //ͳ���������ʽ
};



//ͳ�ƽ���ļ�ͷ��Ϣ�ĳ���
#define STAT_RST_HEAD_LENGTH  20

//ͳ�ƽ���ļ�ͷ��Ϣ
struct SStatRstHead
{
    UINT4  uVersion;        //�ļ��İ汾��
    UINT4  uAccessPoint;    //������
    time_t tStatEndTime;    //ͳ�ƽ�ֹʱ��
    BYTE   yReserved[8];    //�����ֽ�
};


#define STATEXPINDEXMSGLENGTH 520

struct SStatExpIndexMsg
{
    char  szStatExp[512]; //ͳ�Ʊ��ʽ
    BOOL  bInUsed;        //�Ƿ���ʹ����
    UINT4 nStatExpIndex;  //ͳ�Ʊ��ʽ������
};


//ͳ�Ʊ��ʽ��������Ϣ
struct SStatExpIndexList
{
    UINT4 uStatExpCount;           //ͳ�Ʊ��ʽ����
    SStatExpIndexMsg expIndex[1];  //ͳ�Ʊ��ʽ��Ϣ

    void* operator new(size_t classSize, size_t nCount = 1)
    {
        void *ptr = NULL;

        if( nCount < 1)
        {
            nCount = 1;
        }

        UINT4 length = nCount * sizeof(SStatExpIndexMsg) + sizeof(UINT4);

        if (length > MSG_BUFFER_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new BYTE[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SStatExpIndexList*)ptr)->uStatExpCount = nCount;
        }

        return ptr;
    };
};

//һ��ͳ������һ��Ľ����Ϣ�ĳ���
#define ONESTATEXPRSTLENGTH 12

//һ��ͳ������һ��Ľ����Ϣ
struct SOneStatExpRst
{
    INT4  nStatExpIndex; //ͳ�Ʊ��ʽ������
    INT4  nCDRCount;     //��������
    INT4  nValueTotal;   //����ۼ�ֵ
};


//����ͳ������һ��Ľ����Ϣ
struct SAllStatExpRst
{
    UINT4 uStatEndTime;         //ͳ�ƽ�ֹʱ��
    UINT4 uStatSubItemCount;    //����ͳ��������
    SOneStatExpRst ItemRst[1];  //����������ͳ��������һ��

    void* operator new(size_t classSize, size_t nCount = 1)
    {
        void *ptr = NULL;

        if( nCount < 1)
        {
            nCount = 1;
        }

        UINT4 length = nCount * sizeof(SOneStatExpRst) + 2 * sizeof(UINT4);

        if (length > MSG_BUFFER_MAXSIZE)
        {
            return NULL;
        }

        ptr = ::new BYTE[length];
        if(ptr)
        {
            memset(ptr, 0, length);
            ((SAllStatExpRst*)ptr)->uStatSubItemCount = nCount;
        }

        return ptr;
    };
};

typedef LIST<SOneStatSubItemCfg*> ONESTATSUBITEMCFG_LIST;
typedef LIST<STRING>     INFO_LIST;

struct SCdrStatTaskInfo
{
    char  szAPName[16];
    char  szRelease[16];
    UINT4 uAPNumber;
    UINT4 uAPType;
    UINT4 uBillType;
    UINT4 uBillLen;
    BOOL  bMustMerge;
    BOOL  bFinalSPFileFmt;
};

typedef LIST<SCdrStatTaskInfo*> TASKINFO_LIST;

#define MAX_COND_LEN 2048             //��ѯ��������󳤶�

#endif //__STAT_CFG_INFO_H__
