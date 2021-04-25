#ifndef __UTILITY_EX_H__
#define __UTILITY_EX_H__

#include "../include/type_def.h"
#include "format_comm.h"

#ifndef ASSERT
#define  ASSERT assert
#endif 

#pragma warning(disable : 4200)

//定义IP地址的表示类型，OctecToIPAddrStr用到
#define BINIPV4_STYLE			1			//IPV4的类型
#define BINIPV6_STYLE			2			//IPV6的类型

#ifndef _BROWSETEMPBUFFERLENGTH
#define _BROWSETEMPBUFFERLENGTH	4194304	//存放一个字段的缓冲区，4M
#endif

#ifndef _BROWSEBUFFERLENGTH
#define _BROWSEBUFFERLENGTH	8388608		//存放一条话单的缓冲区，8M
#endif

class IGWB_Svc_Export CGlobalBuffer
{
public:
    static char* GetBillBuffer();

    static void acquire();

    static void release();
};

//TimeStamp中用到的几个数
#define _TIME_LEN			9			//结构中数据的长度
#define _TIMESTAMPSTRLENGTH	18			//转换后字符串的长度

#define _EMPTYSTRING				"Empty value"
#define _EMPTYSTRINGLENGTH		11


//BoolToStr用到的几个宏，表示转换的类型
#define BOOLBROWSE_NUMSTYLE		0			//显示"0"或"1"
#define BOOLBROWSE_UPERSTYLE	1			//显示"FALSE"或"TRUE"
#define BOOLBROWSE_LOWERSTYLE	2			//显示"false"或"true"
#define BOOLBROWSE_YESNOSTYLE	3			//显示"No"或"Yes"


typedef struct tagOCTECSTRING
{
    unsigned short nLength;				//pData中有效数据的长度
    unsigned char  pData[1];			//数据区的指针
}OCTECSTRING, *LPOCTECSTRING;


typedef struct tagENUMITEMINFO
{
    long nValue;						//该枚举信息对应的枚举值
    const char *pString;				//该枚举值显示时要显示的字符串
}ENUMITEMINFO, *LPENUMITEMINFO;

typedef struct tagENUMINFO
{
    unsigned short nCount;				//该枚举类型有多少种枚举值
    ENUMITEMINFO *pInfoTable;			//枚举值信息的列表
}ENUMINFO, *LPENUMINFO;

typedef struct tagBITFLAGITEMINFO
{
    long nBitPos;						//Bit的位置，从0开始
    char *pSetString;					//该位被设置时要显示的字符串
    char *pNotSetString;				//该位没有设置时显示的字符串
}BITFLAGITEMINFO, *LPBITFLAGITEMINFO;

typedef struct tagBITFLAGINFO
{
    unsigned short nCount;				//有多少个位标识信息
    BITFLAGITEMINFO *pInfoTable;		//位标识信息的列表
}BITFLAGINFO, *LPBITFLAGINFO;

typedef struct tagTIMESTAMP
{
    unsigned short nLength;				//必须为9
    unsigned char pData[1];				//TimeStamp类型的数据，关于数据的详细结构见附档
}TIMESTAMP, *LPTIMESTAMP;


//下面是公用的工具函数
int IGWB_Svc_Export BCDOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength);
int IGWB_Svc_Export CvtBCDOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength);
//by ldf 2004-03-26 对应问题单SWPD03933
int IGWB_Svc_Export CvtBCDOctecToStr2(LPOCTECSTRING pOctecString, char *pBuffer, int nLength);
//end
int IGWB_Svc_Export DecOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, BOOL bSigned = TRUE, BOOL bReserved = FALSE);
int IGWB_Svc_Export BinOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse = TRUE);
int IGWB_Svc_Export HexOctecToStr(LPOCTECSTRING pOctecString, char *pBuffer, int nLength, const char *pPrefix, BOOL bReverse = TRUE);
//add by YanYong 2004-05-27  问题单SWPD05340 
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

//将BCD码转换为反向BCD码
int IGWB_Svc_Export BCDtoRBCD(BYTE* pRBCD, BYTE* pBCD, const int nLen);

#endif //__UTILITY_EX_H__
