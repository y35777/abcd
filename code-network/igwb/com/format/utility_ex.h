#ifndef __UTILITY_EX_H__
#define __UTILITY_EX_H__

#include "../include/type_def.h"
#include "format_comm.h"

#ifndef ASSERT
#define  ASSERT assert
#endif 

#pragma warning(disable : 4200)

//����IP��ַ�ı�ʾ���ͣ�OctecToIPAddrStr�õ�
#define BINIPV4_STYLE			1			//IPV4������
#define BINIPV6_STYLE			2			//IPV6������

#ifndef _BROWSETEMPBUFFERLENGTH
#define _BROWSETEMPBUFFERLENGTH	4194304	//���һ���ֶεĻ�������4M
#endif

#ifndef _BROWSEBUFFERLENGTH
#define _BROWSEBUFFERLENGTH	8388608		//���һ�������Ļ�������8M
#endif

class IGWB_Svc_Export CGlobalBuffer
{
public:
    static char* GetBillBuffer();

    static void acquire();

    static void release();
};

//TimeStamp���õ��ļ�����
#define _TIME_LEN			9			//�ṹ�����ݵĳ���
#define _TIMESTAMPSTRLENGTH	18			//ת�����ַ����ĳ���

#define _EMPTYSTRING				"Empty value"
#define _EMPTYSTRINGLENGTH		11


//BoolToStr�õ��ļ����꣬��ʾת��������
#define BOOLBROWSE_NUMSTYLE		0			//��ʾ"0"��"1"
#define BOOLBROWSE_UPERSTYLE	1			//��ʾ"FALSE"��"TRUE"
#define BOOLBROWSE_LOWERSTYLE	2			//��ʾ"false"��"true"
#define BOOLBROWSE_YESNOSTYLE	3			//��ʾ"No"��"Yes"


typedef struct tagOCTECSTRING
{
    unsigned short nLength;				//pData����Ч���ݵĳ���
    unsigned char  pData[1];			//��������ָ��
}OCTECSTRING, *LPOCTECSTRING;


typedef struct tagENUMITEMINFO
{
    long nValue;						//��ö����Ϣ��Ӧ��ö��ֵ
    const char *pString;				//��ö��ֵ��ʾʱҪ��ʾ���ַ���
}ENUMITEMINFO, *LPENUMITEMINFO;

typedef struct tagENUMINFO
{
    unsigned short nCount;				//��ö�������ж�����ö��ֵ
    ENUMITEMINFO *pInfoTable;			//ö��ֵ��Ϣ���б�
}ENUMINFO, *LPENUMINFO;

typedef struct tagBITFLAGITEMINFO
{
    long nBitPos;						//Bit��λ�ã���0��ʼ
    char *pSetString;					//��λ������ʱҪ��ʾ���ַ���
    char *pNotSetString;				//��λû������ʱ��ʾ���ַ���
}BITFLAGITEMINFO, *LPBITFLAGITEMINFO;

typedef struct tagBITFLAGINFO
{
    unsigned short nCount;				//�ж��ٸ�λ��ʶ��Ϣ
    BITFLAGITEMINFO *pInfoTable;		//λ��ʶ��Ϣ���б�
}BITFLAGINFO, *LPBITFLAGINFO;

typedef struct tagTIMESTAMP
{
    unsigned short nLength;				//����Ϊ9
    unsigned char pData[1];				//TimeStamp���͵����ݣ��������ݵ���ϸ�ṹ������
}TIMESTAMP, *LPTIMESTAMP;


//�����ǹ��õĹ��ߺ���
int IGWB_Svc_Export BCDOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength);
int IGWB_Svc_Export CvtBCDOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength);
//by ldf 2004-03-26 ��Ӧ���ⵥSWPD03933
int IGWB_Svc_Export CvtBCDOctecToStr2(LPOCTECSTRING pOctecString, char *pBuffer, int nLength);
//end
int IGWB_Svc_Export DecOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, BOOL bSigned = TRUE, BOOL bReserved = FALSE);
int IGWB_Svc_Export BinOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse = TRUE);
int IGWB_Svc_Export HexOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse = TRUE);
//add by YanYong 2004-05-27  ���ⵥSWPD05340 
int IGWB_Svc_Export HexOctecToStr(int nValueLength, const unsigned char* pValue, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse = TRUE);
//</SWPD05340>
int IGWB_Svc_Export OctecToIPAddrStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, BYTE nStyle);
int IGWB_Svc_Export OctecToStr(LPOCTECSTRING pOctecString,  char *pBuffer, int nLength);

int IGWB_Svc_Export BoolToStr(BOOL bValue, char *pBuffer, int nLength, BYTE nStyle);
int IGWB_Svc_Export NumToDecStr(long nNumber, char *pBuffer, int nLength, BOOL bSigned = TRUE);
int IGWB_Svc_Export NumToBinStr(UINT4 nNumber, char *pBuffer, int nLength, const char *pPrefix);
int IGWB_Svc_Export DataToBinStr(char *pData, int nDataLength, char *pBuffer, int nLength, const char *pPrefix);
int IGWB_Svc_Export NumToHexStr(UINT4 nNumber, char *pBuffer, int nLength, const char *pPrefix);
int IGWB_Svc_Export DataToHexStr(char *pData, int nDataLength, char *pBuffer, int nLength, const char *pPrefix);


int IGWB_Svc_Export TimeStampToStr(LPTIMESTAMP pTimeStamp, char *pBuffer, int nLength, BOOL bFormat = TRUE);

int IGWB_Svc_Export EnumToStr(LPENUMINFO pEnumInfo, int nEnumValue, char *pBuffer, int nLength);

//��BCD��ת��Ϊ����BCD��
int IGWB_Svc_Export BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen);

#endif //__UTILITY_EX_H__
