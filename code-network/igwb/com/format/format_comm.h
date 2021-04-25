#if !defined(_FORMAT_COMMON_H_)
#define _FORMAT_COMMON_H_
/**************** The Head Comment of This File *********************/
// Description:  The Common Head File of the Format Module
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/


#include "ace/OS.h"
#include "../include/base_type.h"
#include "format_def.h"


/*
#ifndef ASSERT
#define  ASSERT assert
#endif  //#ifndef ASSERT
*/


/****************** The Align Format Defining ***********************/
#define LEFT_ALIGN    0x00    //�����
#define MID_ALIGN     0x01    //���ж���
#define RIGHT_ALIGN   0x02    //�Ҷ���
/****************** The Align Format Defining ***********************/


/****************** The Data Type Length Defining *******************/
//�����������͵ĳ���
#define LEN_INT2      (sizeof(INT2))
#define LEN_INT4      (sizeof(INT4))
#define LEN_INT8      (sizeof(INT8))

#define LEN_BYTE      (sizeof(BYTE))
#define LEN_UINT2     (sizeof(UINT2))
#define LEN_UINT4     (sizeof(UINT4))
#define LEN_BOOL      (sizeof(BOOL))

#define LEN_CHAR      (sizeof(char))
#define LEN_SHORT      (sizeof(short))
#define LEN_INT       (sizeof(int))
#define LEN_LONG      (sizeof(long))

#define LEN_UCHAR     (sizeof(unsigned char))
#define LEN_USHORT    (sizeof(unsigned short))
#define LEN_UINT      (sizeof(unsigned int))
#define LEN_ULONG     (sizeof(unsigned long))
/****************** End of The Data Type Length Defining ************/


/****************** The Data Type Defining **************************/

//���ʽԪ�����Ͷ���
enum EPostfixEleType
{
    ET_VARIANT,                     //�������绰������ 
    ET_INT_CONST,                   //���ͳ���
    ET_STRING_CONST,                //�ַ�������
    ET_UNSIGNED_CHAR_CONST,         //�ַ�����
    ET_ARITH_OPERATOR,              //��������
    ET_LOGIC_OPERATOR,              //�߼�����
    ET_FUN,                         //����
    ET_HEX_CONST,                   //16��������
    ET_NOTDEFINE                    //δ��������
};

//�����������Ͷ���
enum EOperatorType
{
    OT_UNARY_OPERATOR,
    OT_BINARY_OPERATOR,
    OT_NOTDEFINE                    //δ��������
};

//���������ʽ�������Ͷ���
enum EFIELD_TYPE
{
    FT_CHAR   =     0,              //�ַ��ͣ�1�ֽ�
    FT_BYTE   =     1,              //�޷����ַ��ͣ�1�ֽ�
    FT_SHORT  =     2,              //�����ͣ�2�ֽ�
    FT_USHORT =     3,              //�޷��Ŷ����ͣ�2�ֽ�
    FT_LONG   =     4,              //�����ͣ�4�ֽ�
    FT_ULONG  =     5,              //�޷��ų����ͣ�4�ֽ�
    FT_INT8   =     6,              //64λ���ͣ�8�ֽ�
    FT_BCD    =     7,              //BCD�룬1�ֽ�
    FT_RBCD   =     8,              //����BCD�룬1�ֽ�
    FT_STRING =     9,              //�ַ�������
    FT_HEX    =     10              //ʮ����������
};

//��������Ϊ��ѯ��������������
enum EQUERYCOND_FIELD_TYPE
{
    QT_CHAR     =    0,             //�ַ�����
    QT_NUMBER   =    1,             //��ֵ����
    QT_STRING   =    2,             //�ַ�������
    QT_ENUM     =    3,             //ö������
    QT_TIME     =    4,             //ʱ������
    QT_DATE     =    5,             //��������
    QT_HEX      =    6              //ʮ����������
};


//�ֽ���ת����־ö�ٶ���
enum EByteOrderFlg
{
    INTEL_ORDER =    0,    //��ʾ��ʽΪINTEL�ֽ���
    NET_ORDER   =    1    //��ʾ��ʽΪ�����ֽ���
};


//���ʽԪ�ؽṹ����
typedef struct _PostfixExpEle
{
        BYTE   yEleType;            //���ʽԪ�ص����ͣ�ΪEPostfixEleTypeö����
                                    //���ΪET_NOTDEFINE�����ʾ��Ԫ����Ч
        char   szEleContent[63];    //���ʽԪ�أ�Ϊ�ַ�����
} SPostfixExpEle, *PPostfixExpEle;

//���ʽ����ṹ���Ͷ���
typedef  LIST<SPostfixExpEle> PostfixExpEleList;

//��ʾ���㵥Ԫ���Ͷ���
typedef struct _Variant
{
    BYTE yVariantType;              //������ȡֵ���͵ı�ʶ
    union 
    {
        BYTE     yValue;            //�޷����ַ��ͣ�FT_BYTE��FT_CHAR                
        INT2     n2Value;           //���ͣ�FT_SHORT
        INT4     n4Value;           //���ͣ�FT_LONG
        INT8     n8Value;           //���ͣ�FT_INT8
        UINT2    u2Value;           //���ͣ�FT_USHORT
        UINT4    u4Value;           //���ͣ�FT_ULONG
        char     szValue[63];       //�ַ�����FT_BCD��FT_RBCD
    };
} SVariant, *PVariant;

//���������ȼ��ṹ����
typedef struct _OPERATOR_PRIORITY
{
    unsigned char  Left;            //��ߵĲ�����
    unsigned char  Right;           //�ұߵĲ�����
    int            nPriority;       //���������ȼ�
} SOperatorPriority, *POperatorPriority;

//�������ṹ����
typedef struct _OPERATOR_SYMBOL
{
    unsigned char Left;             //��ߵĲ�����
    unsigned char Right;            //�ұߵĲ�����
} SOperatorSymbol, *POperatorSymbol;

//������ö�����Ͷ���
typedef struct _EnumInfo
{
	UINT2  nEnumValue;			  //ö��ֵ
	char   szEnumValue[30];		  //��ö��ֵ��Ӧ���ַ��� 
} SEnumInfo, *PEnumInfo;

//��1�ֽڶ����SEnumInfo�ṹ�ĳ���
//����ƽ 2002��4��4������
#define SIZEOF_ENUMINFO  32
//������

//��1�ֽڶ���SFieldInfo�ṹ�ĳ���
//����ƽ 2002��4��4������
#define SIZEOF_FIELDINFO    287
//������

//������ö��ֵ�������Ͷ���
typedef LIST<SEnumInfo> ENUMINFO_LIST;

//��ʽ����Ϣ����
typedef struct   _FieldInfo		
{
	UINT2           nFieldIndex;		  //����ţ����㿪ʼ���ε������
	char            szFieldName[64];	  //����
	char            szFieldDescribe[64];  //����������������ϸ����
	BYTE            nDataType;		      //���������ͣ�ΪEFIELD_TYPEö����
    BYTE            nQueryCondDataType;   //��ѯ�����������ͣ�ΪEQUERYCOND_FIELD_TYPEö����
	BYTE            flgBit;			      //�Ƿ���λ���ͣ������ʾ������λ����
	UINT2           nFieldLength;		  //�򳤶ȣ���λ���ͣ���ָʾλ���λ�����Է�λ���ͣ�
                                          //��ָʾ�ֶε�ά��������int a������Ϊ1��int a[4]����Ϊ4��                                                                       //int C: 3���򳤶�Ϊ3��
	BYTE            flgEnum;		      //ö�ٱ�־�������ʾö����
	ENUMINFO_LIST*  pEnumInfos;           //ö�ٽṹ������һ����Ϊö����ʱ��������ö����Ϣ
	char            szConvertFun[128];	  //ת������������Դ��ʽ��Ŀ���ʽ�辭���ı任
	BYTE            nAlignMode;		      //���뷽ʽ�����ı���ʽ������Ŀ���ʽ�������壬
                                          //0x01Ϊ����룬0x02Ϊ���ж��룬0x03Ϊ�Ҷ���
	char            cFillChar;			  //���������ı���ʽ������Ŀ���ʽ��������
	char            cSeparator; 		  //��������һ����֮��ķָ�������ı���ʽ������Ŀ���ʽ��������
    INT4            nBitOffset;		      //������������ʽ�а�λ�����ƫ��(�����ü���ó�)
	UINT4           nByteOffset;		  //������������ʽ�а��ֽڼ����ƫ��(�����ü���ó�)
	BYTE            flgQuery;			  //�����Ƿ����ڲ�ѯ
	BYTE            nOperator;		      //���ڲ�ѯ���߼����ӣ�0x01��ʾ��==����0x02��ʾ��<����
                                          //0x04��ʾ��>����0x08��ʾ��<=����0x10��ʾ��>=����0x20��ʾ��!=��
	char            szUnit[5]; 			  //��ѯʱ�������ȡֵ��λ�����硰�롱�������� 
	BYTE            nOrderMode;           //�ֽ���ת��ģʽ
										  //INTEL_ORDER: INTEL�ֽ���
										  //  NET_ORDER: �����ֽ���
} SFieldInfo, *PFieldInfo;


//�Ի�������Ϊ��ֵ�������û�����Ľṹ���Ͷ���
typedef MAP<STRING, SFieldInfo*> FIELD_MAP;

/****************** The CONVERTFUN Defining *************************/
// ����������ת���������Ͷ���
//
// ���������
//   nSrcFields�����㵱ǰ�������Դ��ʽ�����������ĸ���
//   pSrcFields�����㵱ǰ�������Դ��ʽ�����е������ֵ��ָ�����飬
//               ���鳤����nSrcFieldsָʾ
// nSrcFieldLen�����㵱ǰ�������Դ��ʽ�����е������ĳ��ȵ����飬
//               ���鳤����nSrcFieldsָʾ����������pSrcFields����һһ��Ӧ��ϵ
//   pFieldInfo����ǰ�����������Ŀ���ʽΪ�ı���ʽ��ʱ��ÿת��һ����
//               ��ҪΪĿ���ʽ�����������ͷָ�����Ϊ���ڸ�����ת�������ṩ���㣬
//               �ڵ���ʵ�ʵ�ת������ʱ��pSrcFields�����һ��Ԫ�ع̶���дĿ���ʽ��
//               ������SFieldInfo
//     pDefault����ǰ���ȱʡȡֵ
//  nDefaultLen����ǰ���ȱʡȡֵ�ĳ���
//
// ���������
//   pCurtField����ǰ��ֵ��ָ�룬ת�������ĵ����߸����ʼ����ָ�룻
//               ת���������𽫵�ǰ���ȡֵд���ָ��ָ����ڴ�����
//               ת�����������޸ĸ�ָ���ȡֵ
//     nIsQuery���Ƿ��ѯ��־��1��ʾ�ú���Ϊ��ѯ����ת����
//               0��ʾ����Ϊ��ѯ����ת��
//
//   ����ֵ����
//
//   �ؼ��㣺
//         
/********************************************************************/
typedef int (*CONVERTFUN)(void* pCurField,
                          const UINT4 nSrcFields,
                          const void* pSrcFields[],
			              const UINT4 nSrcFieldLen[],
			              const SFieldInfo* pFieldInfo,
			              const void* pDefault,
			              const UINT4 nDefaultLen,
                          int nIsQuery);
/****************** End of The CONVERTFUN Defining ******************/

/****************** The FINDFUN Defining ****************************/
// �������������Һ���ָ��ĺ������Ͷ���
//
// ���������
//   nSrcFields�����㵱ǰ�������Դ��ʽ�����������ĸ���
//nSrcFieldType�����㵱ǰ�������Դ��ʽ�����е����������ͣ�
//               ��������CONVERTFUN���������pSrcFields����һһ��Ӧ��ϵ��
//               �������͵�ȡֵ�ɲμ�FIELD_TYPE�Ķ���
//
// �����������
//
//   ����ֵ��CONVERTFUN���͵ĺ���ָ��
//
//   �ؼ��㣺
//         
/********************************************************************/
typedef CONVERTFUN (* FINDFUN)(const UINT4 nSrcFields, 
                               const BYTE nSrcFieldType[]);
/****************** End of The FINDFUN Defining *********************/

#define  MAX_SRC_NUM  8	        //����Ŀ���ʽ�����Ҫ�õ���Դ��ʽ�����


/****************** The CONDITIONFUN Defining ***********************/
// ���������������������Ͷ���
//
// ���������
//    nParasNum����������
//       pParas������ֵָ�����飬���鳤����nParasNumָʾ
//    nParasLen���������ĳ��ȵ����飬���鳤����nParasNumָʾ����������pParas����һһ��Ӧ��ϵ
//     pDefault��������ȱʡȡֵ
//  nDefaultLen��������ȱʡȡֵ�ĳ���
//
// �����������
//
//   ����ֵ�����������ȷ���򷵻�SVariant*���͵ļ����������������NULL
//
//   �ؼ��㣺
//         
/********************************************************************/
typedef int (*CONDITIONFUN)(const UINT4 nParasNum,
							const void* pParas[],
							const UINT4 nParasLen[],
							const STRING& pDefault,
							SVariant& stVar);
/****************** End of The CONDITIONFUN Defining ****************/

#define  MAX_PARA_NUM  8	        //��������������������


//Modified by XDB on 2003.04.01
//���ӳ��������б��ȱʡֵ����
//Ŀ���ʽ������ת������ָ��Ķ�Ӧ��ϵ�ṹ����
typedef struct FIELD_CONVERT_MAP
{
	SFieldInfo*    pFieldInfo;			 //Ŀ���ʽ������
	BYTE	       ySrcNum;			     //���㱾��ʽ����Ҫ�õ���Դ��ʽ��ĸ���
	UINT2	       Index[MAX_SRC_NUM];   //���㱾��ʽ����Ҫ�õ���Դ��ʽ�����������  ��BYTE��ΪUINT2 ZhengYuqun SWPD03695 2004-02-20
	CONVERTFUN     ConvertFun;		     //��Ӧ��ת������ָ��
    LIST<STRING>   ConstValues;          //���������б�
    STRING         sDefault;             //ȱʡֵ
} SFieldConvertMap, *PFieldConvertMap;

//�Ը�ʽ����Ϊ�����ĸ�ʽ��ת��������ӳ�����Ͷ���
typedef MAP<STRING, SFieldConvertMap*> CONVERT_MAP;

//��ʽ���ýӿڱ�ʶö�ٶ���
enum IID
{
	IID_FP_IFORMATCFG,        //��ʽ���ýӿڱ�ʶ
	IID_FP_IBILLPROCESSOR,    //��������ӿڱ�ʶ
	IID_FP_IQUERYBROWSE       //��ѯ����ӿڱ�ʶ
};


//3G������ѯ������������ֵ��Ӧ��ϵ���Ͷ���
typedef struct _QUERY_COND_FIELDS_VALUE
{
    UINT2  nFieldIndex;		  //�����
    STRING sFieldName;
    STRING sFieldValue;
} SQUERY_COND_FIELDS_VALUE, *PQUERY_COND_FIELDS_VALUE;
typedef VECTOR<SQUERY_COND_FIELDS_VALUE> QUERY_COND_FIELDS_VALUE;

/****************** End of The Data Type Defining *******************/

//����ƽ 2002��4��4���������´����ֽ���ĺ궨��
# define SWAP_LONG(L) ((SWAP_WORD ((L) & 0xFFFF) << 16) \
        | SWAP_WORD(((L) >> 16) & 0xFFFF))
# define SWAP_WORD(L) ((((L) & 0x00FF) << 8) | (((L) & 0xFF00) >> 8))

# define SWAP_LLONG(L)((SWAP_LONG ((L) & 0xFFFFFFFF) << 32) \
        | SWAP_LONG(((L) >> 32) & 0xFFFFFFFF))
#if defined (_PLATFORM_WIN32) || defined (_PLATFORM_LINUX) 

    # define ITOHS(S) (S)
    # define ITOHL(L) (L)
    #define  ITOHLL(L) (L)

    # define HTOIS(S) (S)
    # define HTOIL(L) (L)

    # define NTOHS(S) SWAP_WORD(S)
    # define NTOHL(L) SWAP_LONG(L)

    # define HTONS(S) SWAP_WORD(S)
    # define HTONL(L) SWAP_LONG(L)
	//added by Gan Zhiliang �����ֽ��������INT8
	#define HTONLL(L)  SWAP_LLONG(L)
	#define NTOHLL(L)  SWAP_LLONG(L)

#else

    # define ITOHS(S) SWAP_WORD(S)
    # define ITOHL(L) SWAP_LONG(L)
    #define  ITOHLL(L) SWAP_LLONG(L)

    # define HTOIS(S) SWAP_WORD(S)
    # define HTOIL(L) SWAP_LONG(L)

    # define NTOHS(S) (S)
    # define NTOHL(L) (L)

    # define HTONS(S) (S)
    # define HTONL(L) (L)

	#define HTONLL(L)  (L)
	#define NTOHLL(L)  (L)
#endif //_PLATFORM_WIN32
//������ ����ƽ

//����ƽ 2004-06-24 ������ݼƷѸ�ʽ��ʶ
//��Ϊ���ݼƷѵĿͻ����̶Ⱥܸߣ���ͬ��Ӫ�̶�������ݼƷ��ֶεĸ�ʽ������ȫ��ͬ
//CBB - Content Based Billing
enum CBB_OPERATOR_ID
{
	CBB_OPERATOR_ID_DEFAULT	= 0,
	CBB_OPERATOR_ID_CMCC	= 1	
};


enum FILE_FORMAT
{
    FILE_FMT_UNKNOWN = 0,
    FILE_FMT_BINARY  = 1,
    FILE_FMT_ASCII	 = 2
};

#endif  //!defined(_FORMAT_COMMON_H_)

