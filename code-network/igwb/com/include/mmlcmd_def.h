#ifndef _MML_COMMAND_DEFINE_
#define _MML_COMMAND_DEFINE_

#include "precom.h"

#define MAX_COMMAND_CODE      30000                  //MML����������ֵ 
#define MAX_CMD_NAME_LEN      32                     //MML����������󳤶�
#define MAX_MACRO_NAME_LEN    64                     //�궨����󳤶�
#define MAX_CMD_DES_LEN	      64                     //������������󳤶�
#define MAX_BUFFER_SIZE       (16 * 1024)            //��������󳤶� 


//����MML SERVER�����

#define USER_NAME_LEN   48      //�û�������󳤶�
#define PARA_NAME_LEN   32      //MML����������󳤶�
#define CMD_NAME_LEN    32      //MML����������󳤶�
//by ldf 2004-04-07 ��Ӧ���ⵥ SWPD04414
#define MAX_CMD_DSP_LEN 128      //������ʾ���Ƶ���󳤶�
//end
#define VALUE_LEN       64      //MML����ֵ����󳤶�
#define ENUM_LEN        20      //ö��ֵ���Ƴ���

#define MAX_UINT2       0xFFFF  //2λ�޷������������ֵ

#define SZ_MACRO_CODE        "macro_code"          //����ID��MACRO.INI�еùؼ�����
#define SZ_PARA_NUM          "para_num"	           //�����в���������MACRO.INI�еĹؼ�����
#define SZ_MUST_GIVE         "must_give"           //�������б�������Ĳ���
#define SZ_MUST_CONFIRM      "MUST BE CONFIRM"	   //����ȷ��
#define SZ_ALL_MACROS        "all_macros"          //����MML�������ƶ���	
#define SZ_NODE_NUM          "node_num"            //MML����ڵ���
#define SZ_MACRO_NAME        "macro_name"          //MML������ʾ����
#define SZ_COMPOUND_STRING   "fieldarray~1"
#define	SZ_COMPLEX_STRING    "complex~1"
#define SZ_CHILDPART	     "childpart"
#define SZ_BYTENUM           "bytenumber"
#define SZ_DEFAULT           "default("
#define SZ_TABLE             "table("
#define SZ_FIELD             "field("
#define SZ_QUERY_CMD	     "query_cmd"
#define SZ_PARA_MAP	         "para_map"
#define SZ_PARA_VALUE        "para_value"
#define SZ_CMD_LEVEL	     "cmd_level"
#define SZ_QUERY_CMD	     "query_cmd"
#define SZ_PARA_MAP	         "para_map"
#define SZ_CONFIRM_ASK	     "�Ƿ�ȷ��"
#define SZ_EMPTY_STR	     ""			//���ַ���ָ��

//MML����弶��
//(MML SERVERʹ��ö��)

#define CMD_LEVEL_ADMIN       0			//ֻ�й���Ա��Ȩʹ��
#define CMD_LEVEL_OPER	      1			//��ͨ����Ա����ʹ��


//������������
typedef enum       
{
    TYPE_CHAR          = 0,        /* ���ֽ� */
    TYPE_SHORT         = 1,        /* ˫�ֽ� */
    TYPE_LONG          = 2,        /* ˫�� */
    TYPE_TELE          = 3,        /* �绰�������� */
    TYPE_STRING        = 4,        /* �ַ��� */
    TYPE_DATE          = 5,        /* ������ */
    TYPE_TIME          = 6,        /* ʱ������ */
    TYPE_IP            = 7,        /* IP��ַ */
    TYPE_SPC           = 8,        /* �������� */
    TYPE_CELLID        = 9,        /* С������ */
    TYPE_RAI           = 10,       /* ·�������� */
    TYPE_BYTE          = 11,       /* λ������ */
    TYPE_COMPLEX       = 12,       /* �������� */
    TYPE_BCD           = 13,       /* BCD������ */
    TYPE_IMSI          = 14,       /* IMSI������ */
    TYPE_DATETIME      = 15,       /* ����ʱ���� */
    TYPE_HEXADECIMAL   = 16,       /* ʮ�������� */
    TYPE_SUBCOMPLEX    = 17,       /* �Ӳ����� */
    TYPE_BUTT
}DATA_TYPE;

#define  PARA_ENABLE        1     //��������������
#define  PARA_DISABLE       0     //�������������


//������ѡ����
typedef enum  
{
   OPT_ENABLE       = 0,        /* ��ѡ */        
   OPT_MUST         = 1         /* ��ѡ */
}MML_PARA_OPTIONAL;


/* ����������¼�ṹ (MML SERVER)*/
typedef struct _CmdDef
{
    UINT2   uCmdCode;                       //������
    char    szCmdName[CMD_NAME_LEN];        //��������
    UINT2   uParaNum;                       //�����������
    BYTE    yLevel;                         //�����
    char    szDspName[MAX_CMD_DSP_LEN];     //�������ʾ����

}SCmdDef,*PCmdDef;


//MML�����������

/* ������������������� (MML SERVER)*/
enum OPERATOR_TYPE
{
    OT_ASSIGN           = 0,    //��ֵ
    OT_ASCEND           = 1,    //��������
    OT_DESCEND          = 2,    //��������

    OT_EQUAL            = 3,    //��ȹ�ϵ
    OT_LARGER           = 4,    //���ڹ�ϵ
    OT_SMALLER          = 5,    //С�ڹ�ϵ
    OT_LARGER_EQUAL     = 6,    //���ڵ��ڹ�ϵ
    OT_SMALLER_EQUAL    = 7,    //С�ڵ��ڹ�ϵ
    OT_NOT_EQUAL        = 8     //�����ڹ�ϵ
};

/* �������Ͷ���  (MML SERVER)*/
enum PARA_TYPE
{
    PT_DIGIT            = 0,    //��ֵ��
    PT_BCD              = 1,    //BCD��
    PT_STRING           = 2,    //��ͨ�ַ���
    PT_BITFIELD         = 3,    //λ��/ö�ٲ���
    PT_COMPLEX          = 4,    //���ϲ���
    PT_ANYTYPE          = 5     //�������,�����ַ�������
};

/* ֵ���Ͷ���  (MML SERVER)*/
enum VALUE_FORMAT
{
    VF_SINGLE           = 0,    //ֵ�ǵ�ֵ
    VF_REGION           = 1,    //ֵ������
    VF_ERROR            = 0xFF  //��Ч,��Ϊ�ǵ�ֵ
};

/* �û�����  (MML SERVER)*/
enum USER_LEVEL
{
    UL_ADMIN            = 0,    //����Ա�û�
    UL_OPER             = 1     //��ͨ�����û�
};

/* �����  (MML SERVER)*/
enum CMD_LEVEL
{
    CL_ADMIN            = 0,    //����Ա����
    CL_OPER             = 1     //��ͨ����Ա����
};

/* ����̨����  (MML SERVER)*/
enum  STATION_TYPE
{
    ST_COMMON           = 0,    //��ͨά����
    ST_ALAEM            = 1,    //�澯��
    ST_DEBUG            = 2     //������Ϣ��
};

/*
    MML����ֵ�����Ͷ���  (MML SERVER).
    ֻ�е�����������PT_DIGITʱ,ֵ�����Ͳ�����ENUM_ZONE
    ��ENUM_ARATE.
*/
typedef enum
{
    ENUM_NONE   = 0,            /* ��ֵ����Ҫ��     */
    ENUM_ZONE   = 1,            /* ֵ��Ϊ��������   */
    ENUM_ARATE  = 2             /* ֵ��Ϊö������   */

}RangeType;

/*
    MML����������¼�ṹ.  (MML SERVER)
	��Ӧ�����ļ�:paradef.dat
*/
typedef struct _ParaDef
{
    UINT2 uCmdCode;                  /* MML������        */
    UINT2 uParaCode;                 /* MML���������    */
    char  szParaName[PARA_NAME_LEN]; /* MML�����������  */
    char  szDspName[PARA_NAME_LEN];  /* ��������ʾ���ƣ����������"������"��"������" */
    BYTE  yDataType;                 /* ������������     */
    char  szDefault[VALUE_LEN];      /* ����ȱʡֵ       */
    BYTE  yMustGive;                 /* �������������� */
    BYTE  yRangeType;                /* �ò���ֵ������   */
    BYTE  yByteNum;                  /* �ֽ���/�Ӳθ���  */

}SParaDef,*PParaDef;


/*
    MML�����Ӳ������¼�ṹ.  (MML SERVER)
	��Ӧ�����ļ�:childparadef.dat
*/
typedef struct _ChildParaDef
{
    UINT2 uCmdCode;                  /* MML������        */
    UINT2 uParaCode;                 /* MML���������    */
    UINT2 uChildParaCode;            /* �Ӳ�����         */
    char  szParaName[PARA_NAME_LEN]; /* ������           */
    char  szDefValue[VALUE_LEN];     /* ȱʡֵ           */
    char  szDspName[PARA_NAME_LEN];	 /* �Ӳ�����ʾ����    */

    /*
        �Ӳ�����ֵ������,�Ӳ����Ĳ������Ͷ�����ֵ.
    */
    BYTE  yRangeType;

}SChildParaDef,*PChildParaDef;


/*
    MML�������ö��ֵ���¼�ṹ.  (MML SERVER)
	��Ӧ�����ļ�:paraenumdef.dat
*/
typedef struct _ParaEnumDef
{
    UINT2 uCmdCode;                     /* MML������     */
    UINT2 uParaCode;                    /* ������        */
    UINT2 uChildParaCode;               /* �Ӳ�����      */
    UINT4 uValue;                       /* ö��ֵ/λ��� */
    char  szInput[ENUM_LEN];            /* ö����/λ���� */
    char  szDspName[MAX_CMD_DSP_LEN];   /* ö�ٵ���ʾֵ  */

}SParaEnumDef,*PParaEnumDef;


/*
    MML�����������ֵ���¼�ṹ.  (MML SERVER)
	��Ӧ�����ļ�:pararangedef.dat
*/
typedef  struct  _ParaRangeDef
{
    UINT2 uCmdCode;             /* MML����    */
    UINT2 uParaCode;            /* ������     */
    UINT2 uChildParaCode;       /* �Ӳ�����   */
    INT4  nMinValue;            /* ��Сֵ     */
    INT4  nMaxValue;            /* ���ֵ     */

}SParaRangeDef,*PParaRangeDef;


BOOL IsParaCode(UINT2 nCmdCode, UINT2 nParaCode, char *pszParaName);
BYTE GetParaDataType(UINT2 nCmdCode, UINT2 nParaNo);

#endif
