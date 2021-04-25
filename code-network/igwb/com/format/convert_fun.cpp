/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CConvertFun
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "format.h"
#include "format_global.h"
#include "ace/INET_Addr.h"
#include "utility_ex.h"
#include "../include/toolbox.h"
#include "convert_fun.h"

//初始化静态成员变量
int CConvertFun::m_nRefCount = 0;
ACE_Mutex CConvertFun::m_hMutex;


/************ The AddRef Defining ******************************/
// 功能描述：引用计数器加1
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CConvertFun::AddRef(void)
{
    m_hMutex.acquire();
    m_nRefCount++;
    m_hMutex.release();
}
/************ End of The AddRef Defining ***********************/


/************ The ReleaseRef Defining **************************/
// 功能描述：引用计数器减1
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CConvertFun::ReleaseRef(void)
{
    m_hMutex.acquire();
    m_nRefCount--;
    m_hMutex.release();
}
/************ End of The ReleaseRef Defining *******************/


/************ The ConvertAnalysis Defining *********************/
// 功能描述：从格式域转换函数中分离出库名、函数名、参数列表、域缺省值
//
// 输入参数：
// sConvertFun：转换函数，对应SFieldInfo中的szConvertFun
//
// 输出参数：
//        sLib：库名（如果不使用动态连接库，则返回的sLib为空字符串）
//        sFun：转换函数名
//    sDefault：域缺省值
//    ParaList：参数列表，按函数参数从左到右排列
//
//   返回值：如果由于sConvertFun配置出错，则函数返回-1，否则返回0
//
/***************************************************************/
int CConvertFun::ConvertAnalysis(const STRING& sConvertFun, STRING& sLib,
                                 STRING& sFun, STRING& sDefault,
                                 LIST<STRING>& ParaList)
{
    int     nStart = 0;               //函数字符串中第一个'('的位置
    int     nEnd = 0;                 //函数字符串中最后一个')'的位置
    int     nLib = 0;                 //函数字符串中default的位置
    STRING  sTmpStr;                  //参数字符串临时缓存变量
    char    szTmpPara[MAX_PARA_LEN];  //一个参数的临时缓冲区
    char*   pTmp = NULL;              //参数临时缓冲区的当前位置
    int     i = 0;                    //临时计数器变量


    //初始化输出参数
    sLib = "";
    sFun = "";
    sDefault = "";
    ParaList.clear();

    //如果函数参数字符串的长度等于零
    if(sConvertFun.length() == 0)
    {
        return -1;
    }

    //获取函数字符串中的左右括号位置
    nStart = sConvertFun.find_first_of('(');
    nEnd = sConvertFun.find_last_of(')');

    //如果同时存在左右括号，则表示正常的函数字符串
    if((nStart != -1) && (nEnd != -1))
    {
        //查找"DllLoad"位置
        nLib = sConvertFun.find(VAR_DLL_LOAD);

        //如果"DllLoad"存在，则表示配置有动态连接库名
        if(nLib != -1)
        {
            //提取动态连接库名
            pTmp = szTmpPara;  //初始化动态连接库名缓冲区当前位置
            i = nStart + 1;    //从第一个括号下一个位置开始

            //如果当前字符等于逗号，表示库名结束
            while((sConvertFun[i] != ',') && (sConvertFun[i] != '\0'))
            {
                //忽略空格
                if(sConvertFun[i] == ' ')
                {
                    i++;
                    continue;
                }

                *pTmp++ = sConvertFun[i++];
            }
            *pTmp = '\0';
            sLib = szTmpPara;
            if(sConvertFun[i] == '\0')
            {
                return -1;
            }

            //提取函数字符串，从逗号的下一个位置开始，到最后一个括号之前结束
            int nTmp = nEnd - i -1;
            if(nTmp < 0)
            {
                nTmp = 0;
            }
            sTmpStr = sConvertFun.substr(i+1, nTmp);

            //把函数字符串分解成函数名、缺省值、参数列表
            if(ConvertFunDiv(sTmpStr, sFun, sDefault, ParaList) != 0)
            {
                return -1;
            }
        }

        //如果"DllLoad"不存在，则表示没有配置有动态连接库名
        else
        {
            //把函数字符串分解成函数名、缺省值、参数列表
            if(ConvertFunDiv(sConvertFun, sFun, sDefault, ParaList) != 0)
            {
                return -1;
            }
        }
    }

    //如果左右括号都不存在，则整个字符串表示一个参数
    else if((nStart == -1) && (nEnd == -1))
    {
        ParaList.push_back(sConvertFun);
    }

    //如果函数参数字符串中只存在左括号或右括号
    else
    {
        return -1;
    }

    return 0;
}
/************ End of The ConvertAnalysis Defining **************/


/************ The BlockCopy Defining ***************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现数据块的拷贝
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向源数据块的内存，
//           nSrcFieldLen[0]表示源数据块的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::BlockCopy(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BLOCK_COPY, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //把原格式相应域中的数据内容拷贝到目标格式当前域中
    memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);

    return 0;
}
/************ End of The BlockCopy Defining ********************/


/************ The BlockCopy Defining ***************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现数据块的拷贝
//           具备BlockCopy的所有功能，并且支持Intel/网络字节序不同长度整数的互相拷贝
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向源数据块的内存，
//           nSrcFieldLen[0]表示源数据块的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::BlockCopyEx(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFieldInfo,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
 {
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BLOCK_COPY, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    if(pFieldInfo->nFieldLength == 1)
    {
        INT8 nTemp = 0;
        
        //将需要变换的数据转换为INT8的中间变量
        switch (nSrcFieldLen[0])
        {
        case LEN_BYTE:
            nTemp = *(BYTE*)pSrcFields[0];
            break;
            
        case LEN_INT2:
            nTemp = *(UINT2*)pSrcFields[0];
            break;
            
        case LEN_INT4:
            nTemp = *(UINT4*)pSrcFields[0];
            break;
                
        case LEN_INT8:
            nTemp = *(INT8*)pSrcFields[0];
            break;
                    
        default:
            memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);
            return 0;
        }
        
        //将INT8的中间变量再转换为需要的数据类型
        switch (pFieldInfo->nDataType)
        {
        case FT_BYTE:
            *(BYTE*)pCurField = nTemp;
            break;
            
        case FT_SHORT:
        case FT_USHORT:
            *(UINT2*)pCurField = nTemp;
            break;
            
        case FT_LONG:
        case FT_ULONG:
            *(UINT4*)pCurField = nTemp;
            break;
            
        case FT_INT8:
            *(INT8*)pCurField = nTemp;
            break;
            
        default:
            memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);
            break;
        }
        
        return 0;
    }
    
    //把原格式相应域中的数据内容拷贝到目标格式当前域中
    memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);
    
    return 0;
}
/************ End of The BlockCopy Defining ********************/

/************ The GetBinYear Defining **************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现将BYTE型或者SHORT型的两位年转换成二进制四位年
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
//           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
//           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::GetBinYear(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_BIN_YEAR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    INT2 nTmp = 0;    //存放两位年的临时变量

    //如果两为年为SHORT类型
    if(nSrcFieldLen[0] == LEN_INT2)
    {
        //nTmp = *(INT2*)pSrcFields[0];
        memcpy((void*)&nTmp, pSrcFields[0], LEN_INT2);

        //CC08交换机中对于两字节的2位年，年份保存在高字节，
        //所以需要移到低字节
        nTmp = nTmp >> 8;
    }

    //如果两为年为BYTE类型
    else if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nTmp = *(BYTE*)pSrcFields[0];
        BYTE yTmp = *(BYTE*)pSrcFields[0];
        nTmp = yTmp;
    }

    //非法参数
    else
    {
        return -1;
    }

    nTmp += 2000;
    //*(INT2*)pCurField = nTmp;
    memcpy(pCurField, &nTmp, LEN_INT2);

    return 0;
}
/************ End of The GetBinYear Defining *******************/


/************ The GetTextYear Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现将BYTE或SHORT型的两位年转换成文本格式的四位年
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
//           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
//           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::GetTextYear(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_YEAR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    INT2 nTmp = 0;    //存放两位年的临时变量

    //如果两为年为SHORT类型
    if(nSrcFieldLen[0] == LEN_INT2)
    {
        //nTmp = *(INT2*)pSrcFields[0];
        memcpy((void*)&nTmp, pSrcFields[0], LEN_INT2);

        //CC08交换机中对于两字节的2位年，年份保存在高字节，
        //所以需要移到低字节
        nTmp = nTmp >> 8;
    }

    //如果两为年为BYTE类型
    else if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nTmp = *(BYTE*)pSrcFields[0];
        BYTE yTmp = *(BYTE*)pSrcFields[0];
        nTmp = yTmp;
    }

    //非法参数
    else
    {
        return -1;
    }

    if(nTmp == 0xFF)
    {
        ((char*)pCurField)[0] = '\0';
    }
    else
    {
        //把两为年加上2000，并赋给目标格式当前域
        nTmp += 2000;

        //把四位年转换成相应的字符串
        sprintf((char*)pCurField, "%4d", nTmp);
    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The GetTextYear Defining ******************/


/************ The GetTextTime Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS.MS
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为6
//           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
//           pSrcFields[1]、pSrcFields[2]、pSrcFields[3]、pSrcFields[4]、
//           pSrcFields[5]分别指向BYTE型月、日、时、分、秒，
//           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
//           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::GetTextTime(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    INT2  nYear =  0;    //存放两位年的临时变量
    BYTE  nMon =   0;    //存放月的临时变量
    BYTE  nDate =  0;    //存放日的临时变量
    BYTE  nHour =  0;    //存放时的临时变量
    BYTE  nMin =   0;    //存放分的临时变量
    BYTE  nSec =   0;    //存放秒的临时变量
    BYTE  nMilliSec = 0; //存放毫秒的临时变量

    //确保输入参数正确
    if((pCurField == NULL) || (nSrcFields != 6 && nSrcFields != 7) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //如果两为年为SHORT类型
    if(nSrcFieldLen[0] == LEN_INT2)
    {
        //nYear = *(INT2*)pSrcFields[0];
        memcpy((void*)&nYear, pSrcFields[0], LEN_INT2);

        //CC08交换机中对于两字节的2位年，年份保存在高字节，
        //所以需要移到低字节
        nYear = nYear >> 8;
    }

    //如果两为年为BYTE类型
    else if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nYear = *(BYTE*)pSrcFields[0];
        BYTE yTmp = *(BYTE*)pSrcFields[0];
        nYear = yTmp;
    }

    //非法参数
    else
    {
        return -1;
    }

    //把两为年加上2000，并赋给目标格式当前域
    nYear += 2000;

    //获取月、日、时、分、秒的值
    nMon   =  *(BYTE*)pSrcFields[1];
    nDate  =  *(BYTE*)pSrcFields[2];
    nHour  =  *(BYTE*)pSrcFields[3];
    nMin   =  *(BYTE*)pSrcFields[4];
    nSec   =  *(BYTE*)pSrcFields[5];

    //Added by ZhengYuqun SWPD02506 2003-11-24
    if((nYear - 2000) == 0xFF || nMon == 0xFF || nDate == 0xFF || nHour == 0xFF || nMin == 0xFF || nSec == 0xFF)
    {
        ((char*)pCurField)[0] = '\0';
    } //End SWPD02506
    else if(nSrcFields == 7)
    {
        nMilliSec = *(BYTE*)pSrcFields[6];
        //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS"的字符串
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d.%d",
            nYear, nMon, nDate, nHour, nMin, nSec, nMilliSec);
    }
    else
    {
        //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS"的字符串
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDate, nHour, nMin, nSec);
    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The GetTextTime Defining ******************/


/************ The Int2Text Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现将各种整型数据转换成文本
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           nSrcFieldLen[0]为1、2、4、8时分别表示pSrcFields[0]
//           指向数据类型为BYTE、SHORT、LONG、INT8型的数据
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
//     注意：暂时不支持INT8类型转换
/***************************************************************/
int CConvertFun::Int2Text(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    INT4  nTmp = 0;    //存放BYTE、SHORT、LONG的临时变量
    //INT8  nNum8 = 0;   //存放INT8的临时变量


    //确保输入参数正确
    if(!(nSrcFields == 1 || nSrcFields == 2))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_INT_TO_TEXT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //by lx SWPD03800
    int nRadix = 10; //缺省10进制
    if(nSrcFields == 2 && pSrcFields[1] != NULL)
    {
        char cRadix;
        memcpy(&cRadix, pSrcFields[1], 1);
        if (cRadix == 'H')
        {
            nRadix = 16;
        }
    }
    //end by lx

    //获取原格式域的整型值
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nTmp = *(BYTE*)pSrcFields[0];
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        nTmp = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT2)
    {
        //nTmp = *(INT2*)pSrcFields[0];
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        nTmp = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT4)
    {
        //nTmp = *(INT4*)pSrcFields[0];
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nTmp = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_INT8)
    {
        //nTmp = *(INT4*)pSrcFields[0];
        INT8 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_INT8);

        #ifdef _PLATFORM_WIN32
        sprintf((char*)pCurField, "%I64u", uTmp);
        #else
        sprintf((char*)pCurField, "%llu", uTmp);
        #endif

        //将目标格式域增加填充符和分隔符
        if(!nIsQuery)
        {
            //如果不是为查询条件转换
            return AddFillerSeparator((char*)pCurField, pFI);
        }
        else
        {
            return 0;
        }

    }
    else
    {
        return -1;
    }
    
    //by lx SWPD03800
    if(nRadix == 16)
    {
        char szFmt[10] = "";
        sprintf(szFmt, "%%0%dx", nSrcFieldLen[0] * 2);  //转换出来的字符串必须是字段字节数的2倍，前面补0
        sprintf((char*)pCurField, szFmt, nTmp);
    }
    else
    {    
    //把整数值转换成相应的字符串
/* Begin:修改不满足无符号数的情况     Modifier: Gan Zhiliang   Date: 2003-4-22*/
    //sprintf((char*)pCurField, "%d", nTmp);
        sprintf((char*)pCurField, "%u", nTmp);
    }
    //end by lx SWPD03800
/* End  : Modifier: Gan Zhiliang   Date: 2003-4-22*/

    //如果当前域是枚举类型
    INT4 nEnumVal = 0;
    ENUMINFO_LIST::iterator item;       //枚举链表迭代子
    ENUMINFO_LIST* pEnumInfos = NULL;   //格式域枚举链表指针变量
    if(pFI->flgEnum != 0)
    {
        StringLRTrim((char*)pCurField);
        nEnumVal = atol((char*)pCurField);

        //获取格式域枚举链表
        pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
        if(pEnumInfos == NULL)
        {
            return -1;
        }

        //顺序搜索格式域枚举链表中的每一个节点
        item = pEnumInfos->begin();
        while(item != pEnumInfos->end())
        {
            //如果当前节点的枚举值等于需要的枚举值
            if(item->nEnumValue == nEnumVal)
            {
                //获取当前枚举值对应的枚举字符串信息
                strcpy((char*)pCurField, item->szEnumValue);
                break;
            }

            item++;
        }

    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The Int2Text Defining *********************/

//add by zkq 2004-09-28
/************ The BCD2RBCD Defining **************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           1.将反向BCD码转换正向BCD码，
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码等于0xF，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，//          
//           pSrcFields[0]指向正向BCD码数组， 
//           nSrcFieldLen[0]指示该正向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun:: BCD2RBCD(void* pCurField, const UINT4 nSrcFields, 
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE*  pBcdArray = NULL;   //BCD码数组
    BYTE*  pCurStr   = NULL;   //反向BCD码字符串指针


    //确保输入参数正确
    if((pCurField == NULL) || (nSrcFields != 1 ) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    pBcdArray = (BYTE *)pSrcFields[0];
   
    //初始化BCD码数组指针
    pCurStr = (BYTE*)pCurField;   
    for(;nBcdIndex  < nSrcFieldLen[0]; nBcdIndex ++)
    {
        pCurStr[nBcdIndex] = ((pBcdArray[nBcdIndex] << 4) | (pBcdArray[nBcdIndex] >> 4));
    }
    pCurStr[nBcdIndex + 1] = 0xFF;    

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;

}
/************ End of The BCD2String Defining *******************/
//end 

/************ The BCD2String Defining **************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           1.将正向BCD码转换成字符串，
//           modify by zkq 2004-05-27
//           2.将正向BCD码中B,C转换成字符'*','#'，
//           add end
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码等于0xF，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向正向BCD码数组，
//           nSrcFieldLen[0]指示该正向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::BCD2String(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针



    //确保输入参数正确
    if(nSrcFields != 1 && nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
              FUN_BCD_TO_STR, nSrcFields);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //组装BCD码
    BYTE pNumber[40];
    int  nSrcNumLen = 0;
    if(nSrcFields == 1)
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];
    }
    else
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

        memcpy(pNumber + nSrcNumLen, pSrcFields[1], nSrcFieldLen[1]);
        nSrcNumLen += nSrcFieldLen[1];        
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pNumber;

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcNumLen)
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
				//add by zkq 2004-05-27
				if((yCurBcd & 0xF0) == 0xB0)
                {
                   pCurStr[nIndex++] = '*';
                }
				else if((yCurBcd & 0xF0) == 0xC0)
                {
                   pCurStr[nIndex++] = '#';
                }//add end 
				else
				{
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
				}
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yCurBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
				//add by zkq 2004-05-27
				if(yCurBcd == 0x0B)
                {
                   pCurStr[nIndex++] = '*';
                }
				else if(yCurBcd == 0x0C)
                {
                   pCurStr[nIndex++] = '#';
                }//add end
				else
				{
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
				}
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The BCD2String Defining *******************/

/************ The BCD2String2 Defining **************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           将两个域(第一个域为n个字节，第二个域为0.5个字节)的正向BCD码转换成字符串，
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码等于0xF，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向正向BCD码数组，
//           nSrcFieldLen[0]指示该正向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::BCD2String2(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BCD_TO_STR_2, 2);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)
       || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0))
    {
        return -1;
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yCurBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    yCurBcd = *(BYTE*)pSrcFields[0];
    //如果当前BCD码值小于10
    if(yCurBcd < 0x0A)
    {
        pCurStr[nIndex++] = yCurBcd + '0';
    }

    //如果当前BCD码值大于10
    else
    {
        pCurStr[nIndex++] = yCurBcd - 10 + 'A';
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The BCD2String2 Defining *******************/

/************ The RBCD2String Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           将反向BCD码转换成字符串，
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码等于0xF，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向反向BCD码数组，
//           nSrcFieldLen[0]指示该反向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::RBCD2String(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE   yTmpBcd   = 0;      //临时BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if(nSrcFields != 1 && nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
              FUN_RBCD_TO_STR, nSrcFields);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    //组装RBCD码
    BYTE pNumber[40];
    int  nSrcNumLen = 0;
    if(nSrcFields == 1)
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];
    }
    else
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

        memcpy(pNumber + nSrcNumLen, pSrcFields[1], nSrcFieldLen[1]);
        nSrcNumLen += nSrcFieldLen[1];        
    }



    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pNumber;

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcNumLen)
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //屏蔽当前BCD码数组值的高四位
        yTmpBcd = yCurBcd & 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yTmpBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yTmpBcd < 0x0A)
            {
                pCurStr[nIndex++] = yTmpBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = yTmpBcd - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The RBCD2String Defining ******************/

/************ The SBCD2String Defining **************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           将正向BCD码转换成字符串，
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码大于等于0xA，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向正向BCD码数组，
//           nSrcFieldLen[0]指示该正向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::SBCD2String(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_SBCD_TO_STR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //初始化SBCD码数组指针和文本格式的SBCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    bool bPrefixZero = true;
    //顺序扫描SBCD码数组的每一个元素
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //获取当前SBCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        if((yCurBcd >> 4) == 0x0 && bPrefixZero)
        {
            //不处理，即去掉前面的0
        }
        //如果高四位是正常的BCD码值，即小于10
        else if(yCurBcd < 0xA0)
        {
            //如果当前BCD码值小于10
            pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            bPrefixZero = false;
        }
        //到了SBCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前SBCD码数组值的高四位
        yCurBcd &= 0x0F;

        if(yCurBcd == 0x0 && bPrefixZero)
        {
            //不处理，即去掉前面的0
        }
        //如果低四位是正常的BCD码值，即小于16
        else if(yCurBcd < 0x0A)
        {
            //如果当前BCD码值小于10
            pCurStr[nIndex++] = yCurBcd + '0';
            bPrefixZero = false;
        }
        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    if(nIndex == 0)
    {
        pCurStr[nIndex++] = '0';
    }
    pCurStr[nIndex] = '\0';

    //如果当前域是枚举类型
    INT4 nEnumVal = 0;
    ENUMINFO_LIST::iterator item;       //枚举链表迭代子
    ENUMINFO_LIST* pEnumInfos = NULL;   //格式域枚举链表指针变量
    if(pFI->flgEnum != 0)
    {
        StringLRTrim((char*)pCurStr);
        nEnumVal = atol((char*)pCurStr);

        //获取格式域枚举链表
        pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
        if(pEnumInfos == NULL)
        {
            return -1;
        }

        //顺序搜索格式域枚举链表中的每一个节点
        item = pEnumInfos->begin();
        while(item != pEnumInfos->end())
        {
            //如果当前节点的枚举值等于需要的枚举值
            if(item->nEnumValue == nEnumVal)
            {
                //获取当前枚举值对应的枚举字符串信息
                strcpy((char*)pCurStr, item->szEnumValue);
                break;
            }

            item++;
        }

    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}

/************ The ZXRBCD2String Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           将反向BCD码转换成字符串，
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码等于0xF，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向反向BCD码数组，
//           nSrcFieldLen[0]指示该反向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::ZXRBCD2String(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE   yTmpBcd   = 0;      //临时BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_ZXRBCD_TO_STR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //屏蔽当前BCD码数组值的高四位
        yTmpBcd = yCurBcd & 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yTmpBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yTmpBcd < 0x0A)
            {
                pCurStr[nIndex++] = yTmpBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = yTmpBcd - 10 + '0';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + '0';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The ZXRBCD2String Defining ******************/

/************ The GetEnumString Defining ***********************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           根据一个格式域的枚举值获得其对应的枚举字符串
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向枚举型的格式域的取值
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::GetEnumString(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{
    INT4 nEnumVal = -1;                 //枚举值临时变量
    ENUMINFO_LIST::iterator item;       //枚举链表迭代子
    ENUMINFO_LIST* pEnumInfos = NULL;   //格式域枚举链表指针变量


    //确保输入参数正确
    if((nSrcFields != 1) && (nSrcFields != 2))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_ENUM_STR, nSrcFields);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    strcpy((char*)pCurField, " ");
    //如果当前域是枚举类型
    if(pFI->flgEnum != 0)
    {
        //获取枚举值
        if(nSrcFieldLen[0]  == LEN_BYTE)
        {
            //nEnumVal = *(BYTE*)pSrcFields[0];
            BYTE uTmp = 0;
            memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
            nEnumVal = uTmp;
        }
        else if(nSrcFieldLen[0]  == LEN_UINT2)
        {
            //nEnumVal = *(INT2*)pSrcFields[0];
            UINT2 uTmp = 0;
            memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
            nEnumVal = uTmp;
        }
        else if(nSrcFieldLen[0]  == LEN_UINT4)
        {
            //nEnumVal = *(INT4*)pSrcFields[0];
            UINT4 uTmp = 0;
            memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
            nEnumVal = uTmp;
        }
        else
        {
            return -1;
        }

        //获取格式域枚举链表
        pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
        if(pEnumInfos != NULL)
        {
        
            //顺序搜索格式域枚举链表中的每一个节点
            item = pEnumInfos->begin();
            while(item != pEnumInfos->end())
            {
                //如果当前节点的枚举值等于需要的枚举值
                if(item->nEnumValue == nEnumVal)
                {
                    //获取当前枚举值对应的枚举字符串信息
                    strcpy((char*)pCurField, item->szEnumValue);
                    break;
                }

                item++;
            }
        }

        //没有对应的枚举字符串信息 added by lucy 2004-06-28
        if(((pEnumInfos == NULL) ||(item == pEnumInfos->end())) 
           && (nSrcFields == 2))
        {
            memcpy(pCurField, pSrcFields[1], nSrcFieldLen[1]);            
        }
        //end added

        //将目标格式域增加填充符和分隔符
        if(!nIsQuery)
        {
            //如果不是为查询条件转换
            return AddFillerSeparator((char*)pCurField, pFI);
        }

    }

    return 0;
}
/************ End of The GetEnumString Defining ****************/


/************ The FindFun Defining *****************************/
// 功能描述：根据格式域名的转换函数，找到对应的转换函数指针
//
// 输入参数：
// szConvertFun：格式域中的转换函数字符串
//      pSrcFmt：源格式对象
//
// 输出参数：无
//
//   返回值：转换函数指针
//
/***************************************************************/
CONVERTFUN CConvertFun::FindFun(const char* szConvertFun, const CFormat *pSrcFmt)
{
    STRING  sLib;               //库名
    STRING  sFun;               //函数名
    STRING  sDefault;           //缺省值
    LIST<STRING>  ParaList;     //参数列表
    CONVERTFUN pRetFun = NULL;  //转换函数指针


    //确保输入参数的正确性
    if(szConvertFun == NULL)
    {
        return NULL;
    }

    //如果正确地解析出库名、函数名、缺省值、参数列表
    if(ConvertAnalysis(szConvertFun, sLib, sFun, sDefault, ParaList) == 0)
    {
        //数据块拷贝的转换函数
        if(!(sFun.compare(FUN_BLOCK_COPY)))
        {
            pRetFun = BlockCopy;
        }
		
        //块拷贝函数，支持Intel/网络字节序不同长度整数的互相拷贝 ZhengYuqun 2004-04-28
        else if(!(sFun.compare(FUN_BLOCK_COPY_EX)))
        {
            pRetFun = BlockCopyEx;
        }

        //SoftX3000对于业务属性为本地长途的特殊转换
        else if(!(sFun.compare(FUN_CHANGE_GSVN_AND_CALLTYPE)))
        {
            pRetFun = ChangeGSVNandCallType;
        }
        else if(!(sFun.compare(FUN_CHANGE_GSVN_AND_CALLTYPE_TEXT)))
        {
            pRetFun = ChangeGSVNandCallType2Text;
        }

        //实现将BYTE型或者SHORT型的两位年转换成二进制四位年的转换函数
        else if(!(sFun.compare(FUN_GET_BIN_YEAR)))
        {
            pRetFun = GetBinYear;
        }

        //实现将BYTE或SHORT型的两位年转换成文本格式的四位年的转换函数
        else if(!(sFun.compare(FUN_GET_TEXT_YEAR)))
        {
            pRetFun = GetTextYear;
        }
       //实现往格式库中添加固定长度的字符串
        else if(!(sFun.compare(FUN_FILL_WITH_STR)))
        {
            pRetFun = FillWithStr;
        }
        //实现往格式库中添加一个INI文件中的指定键值
	    else if(!(sFun.compare(FUN_FILL_BY_INI_FILE)))
        {
            pRetFun = FillByIniFile;
        }        

        //实现年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS的转换函数
        else if(!(sFun.compare(FUN_GET_TEXT_TIME)))
        {
            pRetFun = GetTextTime;
        }

        //实现将各种整型数据转换成文本的转换函数
        else if(!(sFun.compare(FUN_INT_TO_TEXT)))
        {
            pRetFun = Int2Text;
        }

        //将正向SBCD码转换成字符串的转换函数
        else if(!(sFun.compare(FUN_SBCD_TO_STR)))
        {
            pRetFun = SBCD2String;
        }

        //将正向BCD码转换成字符串的转换函数
        else if(!(sFun.compare(FUN_BCD_TO_STR)))
        {
            pRetFun = BCD2String;
        }

        //将n.5个字节的正向BCD码转换成字符串的转换函数
        else if(!(sFun.compare(FUN_BCD_TO_STR_2)))
        {
            pRetFun = BCD2String2;
        }

        //将反向BCD码转换成字符串的转换函数
        else if(!(sFun.compare(FUN_RBCD_TO_STR)))
        {
            pRetFun = RBCD2String;
        }

        //将中兴话单中的反向BCD码转换成字符串的转换函数
        else if(!(sFun.compare(FUN_ZXRBCD_TO_STR)))
        {
            pRetFun = ZXRBCD2String;
        }

        //Added by maimaoshi at 2002-05-17

        //实现将UINT4数据转换成文本IP地址的转换函数
        else if(!(sFun.compare(FUN_INT_TO_IP_TEXT)))
        {
            pRetFun = Int2IpText;
        }
        //by ldf 2003-01-08 对应问题单D05886 D05556（同D05415 D5150）
        //实现把UINT4数据转化位文本IP地址的函数，但是顺序和上方的相反，
        //如上方显示为1.2.3.4则本函数结果为 4.3.2.1
        else if(!(sFun.compare(FUN_INT_TO_IP_TEXT_DIRECT)))
        {
            pRetFun = Int2IpTextDirect;
        }
        //实现把数组的内容输出 如"00 13"输出结果为"00 19"
        else if(!(sFun.compare(FUN_ARRAY_TO_TEXT)))
        {
            pRetFun = Array2Text;
        }
        //修改完毕
        //实现把字符型的年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS的转换函数
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_IN_BCD)))
        {
            pRetFun = GetTextTimeInBCD;
        }

        //实现把BCD型的4位年的年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS的转换函数
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_IN_BCD_4Y)))
        {
            pRetFun = GetTextTimeInBCD_4Y;
        }

        //实现把字符型的6.5个字节的BCD的日期时间转换成文本格式的YYYY-MM-DD HH:NN:SS的转换函数
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_IN_BCD_T)))
        {
            pRetFun = GetTextTimeInBCD_T;
        }

        //End by maimaoshi at 2002-05-17
        //实现把字符型的年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS的转换函数
        else if(!(sFun.compare(FUN_BCD_TO_SECOND)))
        {
            pRetFun = BCDToSecond;
        }
        //实现年、月、日、时、分、秒转换成文本格式的YYYYMMDDHHNNSS的转换函数
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_NO_SP)))
        {
            pRetFun = GetTextTimeNoSp;
        }
        //把某字段全部填FF的转换函数
        else if(!(sFun.compare(FUN_GET_TEXT_FILLFF)))
        {
            pRetFun = FillFF;
        }
        //根据一个格式域的枚举值获得其对应的枚举字符串的转换函数
        else if(!(sFun.compare(FUN_GET_ENUM_STR)))
        {
            pRetFun = GetEnumString;
        }
        else if(!(sFun.compare(FUN_ARRAY_TO_HEX_STRING)))
        {
            pRetFun = Array2HexString;
        }
        else if(!(sFun.compare(FUN_FILL_INVALID_VALUE)))
        {
            pRetFun = FillInvaidValue;
        }
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_EX)))
        {
            pRetFun = GetTextTimeEx;
        }
        //实现填写"\r\n"的转换函数
        else if(!(sFun.compare(FUN_TO_LRLN)))
        {
            pRetFun = ToLRLN;
        }
        //实现填写"\r"的转换函数
        else if(!(sFun.compare(FUN_TO_LR)))
        {
            pRetFun = ToLR;
        }
        //实现填写"\n"的转换函数
        else if(!(sFun.compare(FUN_TO_LN)))
        {
            pRetFun = ToLN;
        }
        //字符串拷贝的转换函数
        else if(!(sFun.compare(FUN_STRING_COPY)))
        {
            pRetFun = StringCopy;
        }
        //byte转为文本的转换函数
        else if(!(sFun.compare(FUN_BYTE_TEXT2)))
        {
            pRetFun = ByteText2;
        }
        //by ldf 2003-04-01 , 校验函数
        else if(!(sFun.compare(FUN_CHECK_SUM)))
        {
            pRetFun = CheckSum;
        }
        //end
        //把字符串日期时间转换为字符串的转换函数
        else if(!(sFun.compare(FUN_GET_TEXT_TIME_INSTRING)))
        {
            pRetFun = GetTextTimeInString;
        }
        //获得含区号的电话号码
        else if(!(sFun.compare(FUN_PAD_AREACODE))) //Added by ZhengYuqun SWPD01198,
        {
            pRetFun = PadAreacode;
        }
        //替换电话号码中的前缀
        else if(!(sFun.compare(FUN_REPLACE_PREFIX))) //Added by ZhengYuqun SWPD01198, 
        {
            pRetFun = ReplacePrefix;
        }        
        else if(!(sFun.compare(FUN_REPLACE_NUM_BY_STR))) //added by lucy 2004-06-25
        {
            pRetFun = ReplaceNumByStr;
        }
        else if (!(sFun.compare(FUN_DURATION_TO_HHMMSS)))
        {
            pRetFun = Duration2HHmmSS;
        }
        else if (!(sFun.compare(FUN_INT_TO_TEXT_EX)))
        {
            pRetFun = Int2TextEx;
        }
        //MSoftXB03话单转换为2G话单时，需要增加试呼话单类型
        else if (!(sFun.compare(FUN_CDRTYPE_2G)))  //Added by ZhengYuqun SWPD03631 2004-02-18
        {
            pRetFun = CDRType2G;
        }
        //邱意平 2004-04-19 添加
        else if (!(sFun.compare(FUN_PAD_NUMBER)))
        {
        	pRetFun = PadNumber;
        }        
        else if (!(sFun.compare(FUN_SMS_NUM_TO_TEXT)))
        {
            pRetFun = SMSNum2Text;
        }
		//左克群 2004-05-27 添加
		else if (!(sFun.compare(FUN_EBCD_TO_STR)))
        {
            pRetFun = EBCD2String;
        }
		else if (!(sFun.compare(FUN_BCD_TIME_TO_SEC)))
        {
            pRetFun = BCDTime2Sec;
        }
		//结束
        //将MSoftX3000的Diagnostics转换为2G的Cause for temination
        else if (!(sFun.compare(FUN_TERM_CAUSE_2G)))  //Added by ZhengYuqun SWPD05305 2004-05-19
        {
            pRetFun = TermCause2G;
        }
        //将MSoftX3000的Last MCCMNC转换为2G的Peer MNC
        else if (!(sFun.compare(FUN_PEER_MNC_2G)))  //Added by ZhengYuqun SWPD05306 2004-05-19
        {
            pRetFun = PeerMnc2G;
        }
        else if (!(sFun.compare(FUN_VAR_LEN_TO_TEXT)))
        {
            pRetFun = VarLen2Text;
        }
        else if (!(sFun.compare(FUN_VAR_VALUE_TO_TEXT)))
        {
            pRetFun = VarValue2Text;
        }
        //Added by ZhengYuqun SWPD06279 2004-06-26
        else if (!(sFun.compare(FUN_GET_VAR_VALUE)))
        {
            pRetFun = GetVarValue;
        }
        else if (!(sFun.compare(FUN_GET_VAR_LEN)))
        {
            pRetFun = GetVarLen;
        }
        //Added by lucy 2005-5-25 OPT version
        else if (!(sFun.compare(FUN_LINK_TEXT)))
        {
            pRetFun = LinkText;
        }
        else if(!(sFun.compare(FUN_GET_RECORDTYPE)))
        {
            pRetFun = GetRecordType;
        }
        else if(!(sFun.compare(FUN_GET_SUBCATEGORY)))
        {
            pRetFun = GetSubCategory;
        }
        else if(!(sFun.compare(FUN_ISUP_CHARGE_NUMBER)))
        {
            pRetFun = ISUPChargeNumber;
        }
        else if(!(sFun.compare(FUN_ISUP_CHARGE_NUMBER_NOA)))
        {
            pRetFun = ISUPChargeNumberNOA;
        }
        else if (!(sFun.compare(FUN_CONVERT10MS)))
        {
            pRetFun = Convert10ms;
        }
        else if(!(sFun.compare(FUN_VIDEOTIME)))
        {
            pRetFun = VideoTime;
        }                
        else if(!(sFun.compare(FUN_VIDEODURATION)))
        {
            pRetFun = VideoDuration;
        }        
        else if(!(sFun.compare(FUN_ADD)))
        {
            pRetFun = Add;
        }                
        //根据号码的长度来转换BCD码 //SWPD06703 Zheng Yuqun 2004-06-19
        else if(!(sFun.compare(FUN_BCD_TO_STR_BY_LEN)))
        {
            pRetFun = BCD2StringByLen;
        }
        //add by zkq 2004-09-28 //SWPD09010
        else if(!(sFun.compare(FUN_BCD_TO_BCD)))
        {
            pRetFun = BCD2RBCD;
        }
        else if(!(sFun.compare(FUN_GET_ARG_SPEC_FIELD)))
        {
            pRetFun = GetArgSpecField;
        }
        else
        {
            pRetFun = NULL;
        }
    }

    //如果解析函数字符串出错
    else
    {
        return NULL;
    }

    //返回获取的转换函数指针
    return pRetFun;
}
/************ End of The FindFun Defining **********************/


/************ The AddFillerSeparator Defining ******************/
// 功能描述：为文本格式的目标格式域增加填充符和分隔符
//
// 输入参数：
//   pszField：未增加填充符和分隔符的目标格式域内容
//        pFI：该目标格式域的描述
//
// 输出参数：
//   pszField：已经加填充符和分隔符后的目标格式域内容
//
//   返回值：成功返回0，失败返回-1
//
/****************************************************************/
int CConvertFun::AddFillerSeparator(char* pszField,
                                    const SFieldInfo* pFI)
{
    UINT4  nScrLen = strlen(pszField);   //目标格式域的原始数据区长度
    UINT4  nFieldLen = 0;                //目标格式域的实际长度
    UINT4  nLeftFill = 0;                //数据区左边需要填充字符的长度
    UINT4  nRightFill = 0;               //数据区右边需要填充字符的长度
    char*  pszTmp = NULL;                //原始数据缓冲区
    BOOL   bIsBlankSepartor = FALSE;

    //确保输入参数的正确
    if((pszField == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if (pFI->cSeparator == ' ')
    {
        bIsBlankSepartor = TRUE;
    }

    nFieldLen = pFI->nFieldLength;  //目标格式域的实际长度

    //修改字节序问题(D12711),麦茂识，2002-04-13
    //如果目标格式域的长度不够
    //if(nFieldLen < (nScrLen + 2))
    if(nFieldLen < (nScrLen + 1))
    {
        if(!bIsBlankSepartor)
        {
            pszField[nFieldLen - 1] = pFI->cSeparator;  //补上分隔符 ZhengYuqun SWPD03695 2004-02-20
        }
        return 0;// Gan Zhiliang modified
    }
    //修改结束，麦茂识，2002-04-13

    //如果是右对齐
    if(pFI->nAlignMode == RIGHT_ALIGN)
    {
        //备份原始数据区内容
        pszTmp = new char[nScrLen + 1];
        strcpy(pszTmp, pszField);


        if (bIsBlankSepartor)
        {
            nLeftFill = nFieldLen - nScrLen;  //获取左边应该填充字符的长度
        }
        else
        {
            nLeftFill = nFieldLen - nScrLen - 1;  //获取左边应该填充字符的长度
        }

        //在数据区的左边填充字符
        memset((void*)pszField, pFI->cFillChar, nLeftFill);
        memcpy((void*)&pszField[nLeftFill], (void*)pszTmp, nScrLen);
    }

    //如果是居中对齐
    else if(pFI->nAlignMode == MID_ALIGN)
    {
        //备份原始数据区内容
        pszTmp = new char[nScrLen + 1];
        strcpy(pszTmp, pszField);

        nLeftFill = (nFieldLen - nScrLen - 1) / 2;

        if (bIsBlankSepartor)
        {
            nLeftFill = (nFieldLen - nScrLen) / 2;
            nRightFill = nFieldLen - nScrLen - nLeftFill;
        }
        else
        {
            nLeftFill = (nFieldLen - nScrLen - 1) / 2;
            nRightFill = nFieldLen - nScrLen - 1 - nLeftFill;
        }


        //在数据区的左右分别填充字符
        memset((void*)pszField, pFI->cFillChar, nLeftFill);
        memcpy((void*)&pszField[nLeftFill], (void*)pszTmp, nScrLen);
        memset((void*)&pszField[nLeftFill + nScrLen], pFI->cFillChar, nRightFill);
    }

    //默认是左对齐
    else
    {
        if (bIsBlankSepartor)
        {
            nRightFill = nFieldLen -  nScrLen;  //获取右边应该填充字符的长度
        }
        else
        {
            nRightFill = nFieldLen -  nScrLen - 1;  //获取右边应该填充字符的长度
        }


        //在数据区的右边填充字符
        memset((void*)&pszField[nScrLen], pFI->cFillChar, nRightFill);
    }

    //目标格式域的最后填充分隔符
    if (!bIsBlankSepartor)
    {
        pszField[nFieldLen - 1] = pFI->cSeparator;
    }

    if(pszTmp != NULL)
    {
        delete[] pszTmp;  //释放临时缓冲区空间
    }

    return 0;
}
/************ End of The AddFillerSeparator Defining ************/

/************ The LinkText Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//          在域ServedMSISDN后用'@'连接域Domain，结果作为Change_dn
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为2,
//           即输入的ServedMSISDNMSISDN和Domain
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
// 2005-5-25 lucy  OPT版本 域MSISDN后面用'@'连接上域Domain
/***************************************************************/
int CConvertFun::LinkText(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    char sztmp[63];
    int nLen;
    
    //验证输入参数正确
    if((pCurField == NULL) || (nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    //end of 验证
    
    //因原ServedMSISDN域是RBCD码,先将之转为字符串
    nLen = RBCDtoString((BYTE*)pSrcFields[0], sztmp, nSrcFieldLen[0],0);
    //连接Domain域
    memcpy(sztmp + nLen, pSrcFields[1], nSrcFieldLen[1]);
    //结束符'\0'
    sztmp[nLen + nSrcFieldLen[1] + 1] = '\0';
    
    memcpy(pCurField, sztmp, nLen + nSrcFieldLen[1] + 1);
    
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;
}
/************ End of The LinkText Defining ****************/

/************ The GetRecordType  Defining *********************/
// 功能描述：马来TM格式转换需求，处理流程为：
// 1.   当原始话单中的free_indicator为0是，Record Type字段填写为04
// 2.   否则，原始话单中的gsvn，通过枚举转换
// 3.   支持没有取值时，用默认值代替
//
// 输入各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           输入参数为功能描述中的参数、类型及顺序
//           free_ind     gsvn
//            FT_BYTE     FT_BYTE
// 返回值：成功返回0，失败返回-1
//
// 举例：GetRecordType(free_ind, gsvn, "0")
// Added By Lucy 2004-07-12
/***************************************************************/    
int CConvertFun::GetRecordType(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[], 
                         const UINT4 nSrcFieldLen[], 
                         const SFieldInfo* pFI, 
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery)
{
    //验证输入参数正确
    if((pCurField == NULL) || ((nSrcFields != 2) && (nSrcFields != 3))
        || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_SUBCATEGORY, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    BYTE nFreeInd = 0;
    memcpy(&nFreeInd, pSrcFields[0], LEN_BYTE);

    if(0 == nFreeInd)
    {
        sprintf((char*)pCurField, "04");
    }
    else
    {
        BYTE nGsvn = 0;
        memcpy(&nGsvn, pSrcFields[1], LEN_BYTE);

        //如果当前域是枚举类型
        if(pFI->flgEnum != 0)
        {
            ENUMINFO_LIST::iterator item;       //枚举链表迭代子
            ENUMINFO_LIST* pEnumInfos = NULL;   //格式域枚举链表指针变量    
            
            //获取格式域枚举链表
            pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
            if(pEnumInfos != NULL)
            {                
                //顺序搜索格式域枚举链表中的每一个节点
                item = pEnumInfos->begin();
                while(item != pEnumInfos->end())
                {
                    //如果当前节点的枚举值等于需要的枚举值
                    if(item->nEnumValue == nGsvn)
                    {
                        //获取当前枚举值对应的枚举字符串信息
                        strcpy((char*)pCurField, item->szEnumValue);
                        break;
                    }
                    
                    item++;
                }
            }
            
            //没有对应的枚举字符串信息 
            if(((pEnumInfos == NULL) ||(item == pEnumInfos->end())) 
                && (nSrcFields == 3))
            {
                memcpy(pCurField, pSrcFields[2], nSrcFieldLen[2]);
            }
        }           
    }    
    
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;    
}
/************ End of The GetRecordType Defining ****************/

/************ The GetSubCategory  Defining *********************/
// 功能描述：马来TM格式转换需求，处理流程为：
// 1.	如果原始话单中的caller_ctx_ number字段有效，则此字段填写为03。
// 2、	如果原始话单中的caller_did为H.323或者SIP，并且trunk_group _in
//      不等于0xFFFF，则此字段填写为05。
// 3、	其它情况，根据原始话单中的caller_category字段来转换，枚举取值        
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           输入参数为功能描述中的参数、类型及顺序
//           caller_ctx_ number，caller_did， trunk_group _in, caller_category           
//           FT_BCD	           , FT_BYTE   ,  FT_USHORT	     , FT_BYTE	
//
//           第5个参数为没有对应枚举时的默认替代参数
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
// 返回值：成功返回0，失败返回-1
//
// 配置例子：GetSubCategory(caller_ctx_number, caller_did, trunk_group_in, caller_category, "01")
//
// Added By Lucy 2004-07-09
/***************************************************************/
int CConvertFun::GetSubCategory(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[], 
                          const UINT4 nSrcFieldLen[], 
                          const SFieldInfo* pFI, 
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    //验证输入参数正确
    if((pCurField == NULL) || ((nSrcFields != 4) && (nSrcFields != 5))
       || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_SUBCATEGORY, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //1. 如果原始话单中的caller_ctx_ number字段有效，则此字段填写为03。
    char szResult[32];

    BCDtoString((BYTE*)pSrcFields[0], (char*)szResult, 
        nSrcFieldLen[0], sizeof(szResult));
    
    if(szResult[0] != 'F')
    {
        strcpy((char*)pCurField, "03");        
    }
    else
    {
        //2. 如果原始话单中的caller_did为H.323 (118) 或者SIP (119)，
        //   并且trunk_group_in不等于0xFFFF，则此字段填写为05。
        BYTE nCallerDid = 0;
        UINT2 nTrunkGroupIn = 0;

        memcpy(&nCallerDid, pSrcFields[1], LEN_BYTE);
        memcpy(&nTrunkGroupIn, pSrcFields[2], LEN_UINT2);

        if((nCallerDid == 118 || nCallerDid == 119) 
           && (nTrunkGroupIn != 0xFFFF))
        {
            strcpy((char*)pCurField, "05");
        }
        else
        {
            //3. 根据原始话单中的caller_category字段来转换，枚举取值
            BYTE nCallerCategory;
            ENUMINFO_LIST::iterator item;       //枚举链表迭代子
            ENUMINFO_LIST* pEnumInfos = NULL;   //格式域枚举链表指针变量    

            strcpy((char*)pCurField, " ");        
            memcpy(&nCallerCategory, pSrcFields[3], LEN_BYTE);

            //如果当前域是枚举类型
            if(pFI->flgEnum != 0)
            {
                //获取格式域枚举链表
                pEnumInfos = (ENUMINFO_LIST*)(pFI->pEnumInfos);
                if(pEnumInfos != NULL)
                {
            
                    //顺序搜索格式域枚举链表中的每一个节点
                    item = pEnumInfos->begin();
                    while(item != pEnumInfos->end())
                    {
                        //如果当前节点的枚举值等于需要的枚举值
                        if(item->nEnumValue == nCallerCategory)
                        {
                            //获取当前枚举值对应的枚举字符串信息
                            strcpy((char*)pCurField, item->szEnumValue);
                            break;
                        }
                
                        item++;
                    }
                }
        
                //没有对应的枚举字符串信息 
                if(((pEnumInfos == NULL) ||(item == pEnumInfos->end())) 
                    && (nSrcFields == 5))
                {
                    memcpy(pCurField, pSrcFields[4], nSrcFieldLen[4]);
                }
            }        
        }
    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;
}
/************ End of The GetSubCategory Defining ****************/


/************ The ISUPChargeNumber Defining *********************/
// 功能描述：马来TM格式转换需求，处理流程为：
//     根据原始话单中的ISUP charge number indicator字段的取值转换。
//      1、	如果取值为0，将caller_number转换到此字段；
//      2、	如果取值为1，将called_number转换到此字段；
//      3、	如果取值为2，将connected_number转换到此字段。
//      其它情况填写为空字符串。
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           输入参数为功能描述中的参数、类型及顺序
//           ISUPchargenumber   caller_number  called_number  connected_number
//           FT_BYTE           , FT_BCD  10      ,  FT_BCD	     , FT_BCD	
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
// 返回值：成功返回0，失败返回-1
// 例子： ISUPChargeNumber(isup_charge_num, caller_number, called_number, connected_number)
// Added By Lucy 2004-07-09
/***************************************************************/
int CConvertFun::ISUPChargeNumber(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[], 
                            const UINT4 nSrcFieldLen[], 
                            const SFieldInfo* pFI, 
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    //验证输入参数正确
    if((pCurField == NULL) || (nSrcFields != 4) || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_SUBCATEGORY, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //填充默认空值
    strcpy((char*)pCurField, "");        

    BYTE  nISUPNumber = 0;

    memcpy(&nISUPNumber, pSrcFields[0], LEN_BYTE);

    if((0 == nISUPNumber) || (1 == nISUPNumber) || (2 == nISUPNumber))
    {
        char szResult[32];
        
        int nResLen = BCDtoString((BYTE*)pSrcFields[nISUPNumber + 1], (char*)szResult, 
                              nSrcFieldLen[nISUPNumber + 1]);
        //去掉尾部的FFFF
        char* pF = strchr(szResult, 'F');
        if(pF != NULL)
        {
            *pF = '\0';
        }

        if (nResLen > 0)
        {            
            strcpy((char*)pCurField, szResult);
        }        
    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    return 0;
}
/************ End of The ISUPChargeNumber Defining ****************/


/************ The ISUPChargeNumberNOA Defining *********************/
// 功能描述：马来TM格式转换需求，处理流程为：
//     根据原始话单中的ISUP charge number indicator字段的取值转换。
//      1、	如果取值为0，将caller_address_nature转换到此字段；
//      2、	如果取值为1，将called_address_nature转换到此字段；
//      3、	如果取值为2，将connected_address_nature转换到此字段。
//      其它情况填写为空字符串。
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           输入参数为功能描述中的参数、类型及顺序, 类型都为：FT_BYTE	
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
// 返回值：成功返回0，失败返回-1
// 例子： ISUPChargeNumberNOA(isup_charge_num, caller_addr_nature,called_addr_nature,connected_addr_nature)
// Added By Lucy 2004-07-09
/***************************************************************/
int CConvertFun::ISUPChargeNumberNOA(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[], 
                               const UINT4 nSrcFieldLen[], 
                               const SFieldInfo* pFI, 
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{
    //验证输入参数正确
    if((pCurField == NULL) || (nSrcFields != 4) || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_SUBCATEGORY, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    
    //填充默认空值
    strcpy((char*)pCurField, "");
    
    BYTE nISUPNumber = 0;
    int nResult = 0;
    memcpy(&nISUPNumber, pSrcFields[0], LEN_BYTE);
    
    if((0 == nISUPNumber) || (1 == nISUPNumber) || (2 == nISUPNumber))
    {
        memcpy(&nResult, pSrcFields[nISUPNumber + 1], nSrcFieldLen[nISUPNumber + 1]);
        sprintf((char*)pCurField, "%d", nResult);
    }
    
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;    
}
/************ End of The ISUPChargeNumberNOA Defining ****************/    

//add by lucy 2004-08-05
/************ The VideoTime Defining *********************/
// 功能描述：联通三期格式转换需求，处理流程为：
//     根据视频编解码字段判断，如果有视频编解码，则使用"应答日期时间"
// 使用示例：
// VideoTime(video_code_type, ans_year,ans_mon,ans_day,ans_hour,ans_min,ans_sec)
/********************************************************/
int CConvertFun::VideoTime(void* pCurField, const UINT4 nSrcFields,
                     const void* pSrcFields[], 
                     const UINT4 nSrcFieldLen[], 
                     const SFieldInfo* pFI, 
                     const void* pDefault, const UINT4 nDefaultLen,
                     int nIsQuery)
{
    INT2  nYear =  0;    //存放两位年的临时变量
    BYTE  nMon =   0;    //存放月的临时变量
    BYTE  nDate =  0;    //存放日的临时变量
    BYTE  nHour =  0;    //存放时的临时变量
    BYTE  nMin =   0;    //存放分的临时变量
    BYTE  nSec =   0;    //存放秒的临时变量
    
    //验证输入参数正确
    if((pCurField == NULL) || (nSrcFields != 7) || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_VIDEOTIME, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    
    //填充默认空值
    ((char*)pCurField)[0] = '\0';
    
    BYTE VideoType = 0;
    memcpy(&VideoType, pSrcFields[0], LEN_BYTE);
    
    if(VideoType != 0)
    {
        //如果两为年为SHORT类型
        if(nSrcFieldLen[1] == LEN_INT2)
        {
            memcpy((void*)&nYear, pSrcFields[1], LEN_INT2);           
            //CC08交换机中对于两字节的2位年，年份保存在高字节，
            //所以需要移到低字节
            nYear = nYear >> 8;
        }       
        //如果两为年为BYTE类型
        else if(nSrcFieldLen[1] == LEN_BYTE)
        {
            BYTE yTmp = *(BYTE*)pSrcFields[1];
            nYear = yTmp;
        }
        
        //非法参数
        else
        {
            return -1;
        }
        
        //把两为年加上2000，并赋给目标格式当前域
        nYear += 2000;
        
        //获取月、日、时、分、秒的值
        nMon   =  *(BYTE*)pSrcFields[2];
        nDate  =  *(BYTE*)pSrcFields[3];
        nHour  =  *(BYTE*)pSrcFields[4];
        nMin   =  *(BYTE*)pSrcFields[5];
        nSec   =  *(BYTE*)pSrcFields[6];
        
        if((nYear - 2000) == 0xFF || nMon == 0xFF || nDate == 0xFF || nHour == 0xFF || nMin == 0xFF || nSec == 0xFF)
        {
            ((char*)pCurField)[0] = '\0';
        }
        else
        {
            //将整型的年、月、日、时、分、秒转换成"YYYYMMDDHHNNSS"的字符串
            sprintf((char*)pCurField, "%4d%02d%02d%02d%02d%02d",
                nYear, nMon, nDate, nHour, nMin, nSec);
        }
        
    }
    
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;        
}

//add by lucy 2004-08-05
/************ The VideoDuration Defining *********************/
// 功能描述：联通三期格式转换需求，处理流程为：
//     根据视频编解码字段判断，如果有视频编解码，则使用"通话时长"
// 使用示例：
// VideoDuration(video_code_type,conversation_time)
/********************************************************/
int CConvertFun::VideoDuration(void* pCurField, const UINT4 nSrcFields,
                         const void* pSrcFields[], 
                         const UINT4 nSrcFieldLen[], 
                         const SFieldInfo* pFI, 
                         const void* pDefault, const UINT4 nDefaultLen,
                         int nIsQuery)
{
    INT4  nTmp = 0;    //存放BYTE、SHORT、LONG的临时变量

    //验证输入参数正确
    if((pCurField == NULL) || (nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_VIDEOTIME, nSrcFields);
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    
    //填充默认空值
    ((char*)pCurField)[0] = '\0';    
    
    BYTE VideoType = 0;
    memcpy(&VideoType, pSrcFields[0], LEN_BYTE);
    
    if(VideoType != 0)
    {
        //获取原格式域的整型值
        if(nSrcFieldLen[1] == LEN_BYTE)
        {
            BYTE uTmp = 0;
            memcpy(&uTmp, pSrcFields[1], LEN_BYTE);
            nTmp = uTmp;
        }
        else if(nSrcFieldLen[1] == LEN_UINT2)
        {
            UINT2 uTmp = 0;
            memcpy(&uTmp, pSrcFields[1], LEN_UINT2);
            nTmp = uTmp;
        }
        else if(nSrcFieldLen[1] == LEN_UINT4)
        {
            UINT4 uTmp = 0;
            memcpy(&uTmp, pSrcFields[1], LEN_UINT4);
            nTmp = uTmp;
        }
        else
        {
            return -1;
        }

        sprintf((char*)pCurField, "%u", nTmp);
    }
    
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;            
}


/************ The GetParas Defining *****************************/
// 功能描述：从转换函数配置的参数字符串中拆分出单个函数参数，
//           参数之间用','分隔
//
// 输入参数：
//     sParas：参数字符串
//
// 输出参数：
//   ParaList：参数列表，按函数参数顺序排列
//
//   返回值：如果由于配置出错，则函数返回-1，否则返回0
//
/****************************************************************/
int CConvertFun::GetParas(const STRING& sParas, LIST<STRING>& ParaList)
{
    int     nStart = 0;               //函数参数字符串中第一个'('的位置
    int     nEnd = 0;                 //函数参数字符串中最后一个')'的位置
    STRING  sSubStr;                  //参数字符串临时缓存变量
    char    szTmpPara[MAX_PARA_LEN];  //一个参数的临时缓冲区
    char*   pTmp = NULL;              //参数临时缓冲区的当前位置
    const char*  pStr = NULL;         //参数字符串的当前位置


    // 初始化输出参数
    ParaList.clear();

    //如果函数参数字符串的长度等于零
    if(sParas.length() == 0)
    {
        return 0;
    }

    //获取函数参数字符串中的左右括号位置
    nStart = sParas.find_first_of('(');
    nEnd = sParas.find_last_of(')');

    //如果函数参数字符串中同时存在左右括号
    if((nStart != -1) && (nEnd != -1))
    {
        //获取左右括号中的参数字符串
        sSubStr = sParas.substr(nStart+1, nEnd - nStart - 1);
    }

    //如果函数参数字符串中不存在左右括号
    else if((nStart == -1) && (nEnd == -1))
    {
        //整个函数参数字符串都是参数
        sSubStr = sParas;
    }

    //如果函数参数字符穿中只存在左括号或右括号
    else
    {
        return -1;
    }

    //设置参数字符串的当前位置
    pStr = sSubStr.c_str();

    //初始化参数的缓冲区和其当前位置
    szTmpPara[0] = '\0';
    pTmp = szTmpPara;

    //判断参数字符串中的每一个字符
    while(*pStr != '\0')
    {
        //如果当前字符等于空格
        if(*pStr == ' ')
        {
            pStr++;
            continue;
        }

        //如果当前字符等于逗号
        else if(*pStr == ',')
        {
            //如果当前参数不为空
            if(szTmpPara[0] != '\0')
            {
                //把当前参数放入输出参数链表
                *pTmp = '\0';
                ParaList.push_back(szTmpPara);

                //重新初始化参数的缓冲区和其当前位置
                szTmpPara[0] = '\0';
                pTmp = szTmpPara;
            }

            //如果当前参数为空
            else
            {
                return -1;
            }

            pStr++;
            continue;
        }
        else
        {
            //把当前字符放入参数缓冲区当前位置
            *pTmp++ = *pStr++;
        }
    }

    //如果当前参数不为空
    if(szTmpPara[0] != '\0')
    {
        //把当前参数放入输出参数链表
        *pTmp = '\0';
        ParaList.push_back(szTmpPara);
    }

    return 0;
}
/************ End of The GetParas Defining **********************/

/************ The ConvertFunDiv Defining ************************/
// 功能描述：从格式域转换函数中分离出函数名、参数列表、域缺省值
//
// 输入参数：
//    sSubStr：转换函数字符串
//
// 输出参数：
//       sFun：转换函数名
//   sDefault：域缺省值
//   ParaList：参数列表，按函数参数从左到右排列
//
//   返回值：如果由于配置出错，则函数返回-1，否则返回0
//
/****************************************************************/
int CConvertFun::ConvertFunDiv(const STRING& sSubStr, STRING& sFun,
                               STRING& sDefault, LIST<STRING>& ParaList)
{
    int     nStart = 0;               //函数字符串中第一个'('的位置
    int     nEnd = 0;                 //函数字符串中最后一个')'的位置
    int     nDefault = 0;             //函数字符串中default的位置
    STRING  sTmpStr;                  //参数字符串临时缓存变量
    char    szTmpPara[MAX_PARA_LEN];  //一个参数的临时缓冲区
    char*   pTmp = NULL;              //参数临时缓冲区的当前位置
    int     i = 0;                    //临时计数器变量


    // 初始化输出参数
    sFun = "";
    sDefault = "";
    ParaList.clear();

    //如果函数参数字符串的长度等于零
    if(sSubStr.length() == 0)
    {
        return -1;
    }

    //获取函数字符串中的左右括号位置
    nStart = sSubStr.find_first_of('(');
    nEnd = sSubStr.find_last_of(')');

    //如果函数字符串中不存在左括号或右括号，或都不存在，
    //或者第一个字符是左括号（即函数名不存在）
    if((nStart == -1) || (nEnd == -1) || (nStart == 0))
    {
        return -1;
    }

    //获取函数名称
    //sFun = sSubStr.substr(0, nStart);
    pTmp = szTmpPara;    //初始化函数名称缓冲区当前位置
    for(i = 0; i < nStart; i++)
    {
        //忽略空格
        if(sSubStr[i] == ' ')
        {
            continue;
        }

        *pTmp++ = sSubStr[i];
    }
    *pTmp = '\0';
    sFun = szTmpPara;

    //获取缺省值位置
    nDefault = sSubStr.find(VAR_DEFAULT);

    //如果缺省值存在
    if(nDefault != -1)
    {
        pTmp = szTmpPara;   //初始化缺省值缓冲区当前位置

        //获取缺省值
        i = nDefault + strlen(VAR_DEFAULT) + 1;  //从"default"的下下个位置开始
        while((sSubStr[i] != ')') && (sSubStr[i] != '\0'))  //遇到')'表示缺省值结束
        {
            //忽略空格和等号
            if((sSubStr[i] == ' ') || (sSubStr[i] == '='))
            {
                i++;
                continue;
            }

            *pTmp++ = sSubStr[i++];
        }
        *pTmp = '\0';
        sDefault = szTmpPara;

        //获取参数字符串，从第一个括的下一个位置开始，
        //到"default"的前前个位置结束
        int nTmp = nDefault - nStart - 2;
        if(nTmp < 0)
        {
            nTmp = 0;
        }
        sTmpStr = sSubStr.substr(nStart + 1, nTmp);
    }

    //如果缺省值不存在
    else
    {
        //获取参数字符串，从第一个括号的下一个位置开始，
        //到最后一个括号之前结束
        sTmpStr = sSubStr.substr(nStart + 1, nEnd - nStart - 1);
    }

    //解析参数字符串为参数列表
    if (GetParas(sTmpStr, ParaList) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The ConvertFunDiv Defining *****************/


/****************** The Prefix Defining *************************/
// 功能描述：获取电话号码字符串的前n位，并转换成相应的字符串,
//           该函数是CONDITIONFUN类型的工具函数
//
// 输入参数：
//  nParasNum：参数个数，此函数的参数个数应该为2
//     pParas：参数数组，此函数包含两个参数，pParas[0]表示电话号码字符串（BCD码）
//             pParas[1]表示要获取电话号码字符串的前n个字符
//  nParasLen：各参数长度数组
//   pDefault：函数缺省值
//
// 输出参数：SVariant类型的结果
//
//   返回值：计算成功，则返回0，如果出错返回非零
//
/****************************************************************/
int CConvertFun::Prefix(const UINT4 nParasNum,
                        const void* pParas[],
                        const UINT4 nParasLen[],
                        const STRING& pDefault,
                        SVariant& stVar)
{
    char*      pCallNum = NULL;           //电话号码字符串
    UINT4      uPreNum  = 0;              //取电话号码字符串的前uPreNum个数字
    char szTmpStr[FMT_MAX_CALL_NUM_LEN];  //电话号码的前uPreNum个数字临时字符串

    //确保输入参数的正确性
    if((nParasNum != 2) || (pParas == NULL))
    {
        return -1;
    }
    int i = 0;
    //确保参数正确
    for(i = 0; i < nParasNum; i++)
    {
        if((pParas[i] == NULL) || nParasLen[i] == 0)
        {
            return -1;
        }
    }

    //获取电话号码字符串
    pCallNum = (char*)pParas[0];

    /*
    //获取要提取的前n个数字的个数
        if(nParasLen[1] == LEN_BYTE)
        {
            //uPreNum = *(BYTE*)pParas[1];
            BYTE uTmp = 0;
            memcpy(&uTmp, pParas[1], LEN_BYTE);
            uPreNum = uTmp;
        }
        else if(nParasLen[1] == LEN_UINT2)
        {
            //uPreNum = *(INT2*)pParas[1];
            UINT2 uTmp = 0;
            memcpy(&uTmp, pParas[1], LEN_UINT2);
            uPreNum = uTmp;
        }
        else if(nParasLen[1] == LEN_UINT4)
        {
            //uPreNum = *(INT4*)pParas[1];
            UINT4 uTmp = 0;
            memcpy(&uTmp, pParas[1], LEN_UINT4);
            uPreNum = uTmp;
        }
        else
        {
            return NULL;
        }


    //最大长度为电话号码的长度
    if(uPreNum > strlen(pCallNum))
    {
        uPreNum = strlen(pCallNum);
    }*/
    memcpy(&uPreNum, pParas[1], 4);
    if(uPreNum > nParasLen[0])
    {
        uPreNum = nParasLen[0];
    }
    int nCount = 0;

    for (i = 0; i < nParasLen[0]; i++)
    {
        unsigned char ch;

        ch = pCallNum[ i ];
        if ((ch < 0xA0) && (nCount < uPreNum))
        {
            szTmpStr[nCount] = (((ch >> 4) & 0x0F) + '0');
            nCount++;
        }
        else
            break;

        ch &= 0x0F;
        if ((ch < 0x0A) && (nCount < uPreNum))
        {
            szTmpStr[nCount] = (ch + '0');
            nCount++;
        }
        else
            break;
    }

    szTmpStr[uPreNum] = '\0';

    //把号码字符串的前uPreNum个数字转换成整数
    stVar.yVariantType = FT_STRING;
    memcpy(stVar.szValue, szTmpStr,  uPreNum + 1);

    return 0;
}
/****************** End of The Prefix Defining ******************/


/****************** The FINDCONDFUN Defining ********************/
// 功能描述：查找条件函数指针的函数类型定义
//
// 输入参数：
//       sFun：函数名称
//
// 输出参数：无
//
//   返回值：CONDITIONFUN类型的函数指针
//
//   关键点：暂时只有Prefix一个函数
//
/****************************************************************/
CONDITIONFUN CConvertFun::FindConFun(const STRING& sFun)
{
    CONDITIONFUN pRetFun = NULL;


    //确保输入参数的正确性
    if(sFun.length() == 0)
    {
        return NULL;
    }

    //获取电话号码的前n位函数
    if(!(sFun.compare(FUN_PREFIX)))
    {
        pRetFun = Prefix;
    }

    else if(!(sFun.compare(FUN_ISINNUMSEG)))
    {
        pRetFun = IsInNumSeg;
    }

    else if(!(sFun.compare(FUN_ISINENUM)))
    {
        pRetFun = IsInEnum;
    }

    //实现从主叫号码中返回区号ID
    else if(!(sFun.compare(FUN_GET_AREA_ID_INSTRING)))
    {
        pRetFun = GetAreaID;
    }

    //其它非法的函数名
    else
    {
        pRetFun = NULL;
    }

    //返回获取的转换函数指针
    return pRetFun;
}
/****************** End of The FINDCONDFUN Defining *************/


/************ The GetTextTimeInBCD Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现把BCD的年月日时分秒转换成文本格式的YYYY-MM-DD HH:NN:SS.MS
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1
//
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
// Added by maimaoshi at 2002-10-15
/***************************************************************/
int CConvertFun::GetTextTimeInBCD(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    int nYear, nMon, nDay, nHour, nMin, nSec;
    char szTemp[10];

    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_TIME_IN_BCD, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yCurBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //处理年，把两位年变成四位年
    memcpy(szTemp, pCurStr, 2);
    szTemp[2] = 0;
    nYear = 2000 + atoi(szTemp);

    //处理月
    memcpy(szTemp, pCurStr + 2, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //处理日
    memcpy(szTemp, pCurStr + 4, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    //处理时
    memcpy(szTemp, pCurStr + 6, 2);
    szTemp[2] = 0;
    nHour = atoi(szTemp);

    //处理分
    memcpy(szTemp, pCurStr + 8, 2);
    szTemp[2] = 0;
    nMin = atoi(szTemp);

    //处理秒
    memcpy(szTemp, pCurStr + 10, 2);
    szTemp[2] = 0;
    nSec = atoi(szTemp);

    //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS"的字符串
    sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDay, nHour, nMin, nSec);

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The GetTextTimeInBCD Defining ******************/

/************ The GetTextTimeInBCD_4Y Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现把BCD的（四位年）年月日时分秒转换成文本格式的YYYY-MM-DD HH:NN:SS.MS
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1
//
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
// Added by maimaoshi at 2002-10-15
/***************************************************************/
int CConvertFun::GetTextTimeInBCD_4Y(void* pCurField, const UINT4 nSrcFields,
                                     const void* pSrcFields[],
                                     const UINT4 nSrcFieldLen[],
                                     const SFieldInfo* pFI,
                                     const void* pDefault, const UINT4 nDefaultLen,
                                     int nIsQuery)
{
    int nYear, nMon, nDay, nHour, nMin, nSec;
    char szTemp[10];

    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_TIME_IN_BCD_4Y, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yCurBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //处理年
    memcpy(szTemp, pCurStr, 4);
    szTemp[4] = 0;
    nYear = atoi(szTemp);

    //处理月
    memcpy(szTemp, pCurStr + 4, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //处理日
    memcpy(szTemp, pCurStr + 6, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    //处理时
    memcpy(szTemp, pCurStr + 8, 2);
    szTemp[2] = 0;
    nHour = atoi(szTemp);

    //处理分
    memcpy(szTemp, pCurStr + 10, 2);
    szTemp[2] = 0;
    nMin = atoi(szTemp);

    //处理秒
    memcpy(szTemp, pCurStr + 12, 2);
    szTemp[2] = 0;
    nSec = atoi(szTemp);

    //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS"的字符串
    sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDay, nHour, nMin, nSec);

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The GetTextTimeInBCD_4Y Defining ******************/

/************ The GetTextTimeInBCD_T Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现把BCD的YYMMDDHHMMSST共6.5个字节转换成文本格式的
//           YYYY-MM-DD HH:NN:SS.MS
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1
//
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//   Added by 王锋 at 2003-05-22
/***************************************************************/
int CConvertFun::GetTextTimeInBCD_T(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    int nYear, nMon, nDay, nHour, nMin, nSec, nTSec;
    char szTemp[10];

    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_TIME_IN_BCD_T, 2);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)
       || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0))
    {
        return -1;
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yCurBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //处理年，把两位年变成四位年
    memcpy(szTemp, pCurStr, 2);
    szTemp[2] = 0;
    nYear = 2000 + atoi(szTemp);

    //处理月
    memcpy(szTemp, pCurStr + 2, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //处理日
    memcpy(szTemp, pCurStr + 4, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    //处理时
    memcpy(szTemp, pCurStr + 6, 2);
    szTemp[2] = 0;
    nHour = atoi(szTemp);

    //处理分
    memcpy(szTemp, pCurStr + 8, 2);
    szTemp[2] = 0;
    nMin = atoi(szTemp);

    //处理秒
    memcpy(szTemp, pCurStr + 10, 2);
    szTemp[2] = 0;
    nSec = atoi(szTemp);

    //处理十分之一秒
    BYTE yTemp = *(BYTE*)pSrcFields[1];

    //如果当前BCD码值小于10
    if(yTemp < 0x0A)
    {
        nTSec = yTemp;
    }
    //如果当前BCD码值大于10
    else
    {
        nTSec = 0;
    }

    //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS.T"的字符串
    sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d.%02d",
                nYear, nMon, nDay, nHour, nMin, nSec, nTSec);

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The GetTextTimeInBCD_T Defining ******************/

/************ The Int2IpText Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现将各种整型数据转换成文本
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           nSrcFieldLen[0]只能为5，表示pSrcFields[0]
//           指向数据类型为UINT4型的数据
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
// Added by maimaoshi at 2002-05-17
/***************************************************************/
int CConvertFun::Int2IpText(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    UINT4 nTmp = 0;

    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_INT_TO_IP_TEXT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //获取原格式域的整型值
    if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nTmp = uTmp;
    }
    else
    {
        return -1;
    }

    //把整数值转换成相应的字符串
    ACE_INET_Addr addr((u_short)0,(ACE_UINT32)nTmp);

    strcpy((char*)pCurField, addr.get_host_addr());

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The Int2IpText Defining *********************/

/************ The BCDToSecond Defining **************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           将正向BCD码的通话时长转换成秒，
//           (4个字节BCD码格式为:HHHMMSST)
//           (HHH:0~255小时;MM:0~59分钟;SS:0~59秒;T:0-9毫秒)
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码等于0xF，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向正向BCD码数组，
//           nSrcFieldLen[0]指示该正向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::BCDToSecond(void* pCurField, const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BCD_TO_SECOND, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcFieldLen[0])
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yCurBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
                pCurStr[nIndex++] = yCurBcd - 10 + 'A';
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    UINT4 uSecondCount = 0;
    UINT4 uTemp = 0;

    if (nIndex == 8)
    {
        //转换小时到秒，前面3个字符
        uTemp = uTemp + (pCurStr[0] - '0')*100;
        uTemp = uTemp + (pCurStr[1] - '0')*10;
        uTemp = uTemp + (pCurStr[2] - '0');

        uSecondCount = uSecondCount + uTemp * 3600;
        uTemp = 0;

        //转换分钟到秒，接着2个字符
        uTemp = uTemp + (pCurStr[3] - '0')*10;
        uTemp = uTemp + (pCurStr[4] - '0');

        uSecondCount = uSecondCount + uTemp * 60;
        uTemp = 0;

        //转换秒数到秒，接着2个字符
        uTemp = uTemp + (pCurStr[5] - '0')*10;
        uTemp = uTemp + (pCurStr[6] - '0');

        uSecondCount = uSecondCount + uTemp;
        uTemp = 0;

        //转换毫秒到秒，最后1个字符
        //uTemp = uTemp + (pCurStr[7] - '0')*0.1;

        //uSecondCount = uSecondCount + uTemp;
        //uTemp = 0;

    }
    else
    {
        uTemp = 0;
    }

    //再转换数字字符串
    sprintf((char*)pCurField, "%d", uSecondCount);

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The BCDToSecond Defining *********************/

/************ The GetTextTimeNoSp Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现年、月、日、时、分、秒转换成文本格式的YYYYMMDDHHNNSS
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为6
//           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
//           pSrcFields[1]、pSrcFields[2]、pSrcFields[3]、pSrcFields[4]、
//           pSrcFields[5]分别指向BYTE型月、日、时、分、秒，
//           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
//           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::GetTextTimeNoSp(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    INT2  nYear =  0;    //存放两位年的临时变量
    BYTE  nMon =   0;    //存放月的临时变量
    BYTE  nDate =  0;    //存放日的临时变量
    BYTE  nHour =  0;    //存放时的临时变量
    BYTE  nMin =   0;    //存放分的临时变量
    BYTE  nSec =   0;    //存放秒的临时变量

    //确保输入参数正确
    if(nSrcFields != 6)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_TIME_NO_SP, 6);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //如果两为年为SHORT类型
    if(nSrcFieldLen[0] == LEN_INT2)
    {
        //nYear = *(INT2*)pSrcFields[0];
        memcpy((void*)&nYear, pSrcFields[0], LEN_INT2);

        //CC08交换机中对于两字节的2位年，年份保存在高字节，
        //所以需要移到低字节
        nYear = nYear >> 8;
    }

    //如果两为年为BYTE类型
    else if(nSrcFieldLen[0] == LEN_BYTE)
    {
        //nYear = *(BYTE*)pSrcFields[0];
        BYTE yTmp = *(BYTE*)pSrcFields[0];
        nYear = yTmp;
    }

    //非法参数
    else
    {
        return -1;
    }

    //把两为年加上2000，并赋给目标格式当前域
    nYear += 2000;

    //获取月、日、时、分、秒的值
    nMon   =  *(BYTE*)pSrcFields[1];
    nDate  =  *(BYTE*)pSrcFields[2];
    nHour  =  *(BYTE*)pSrcFields[3];
    nMin   =  *(BYTE*)pSrcFields[4];
    nSec   =  *(BYTE*)pSrcFields[5];

    //Added by ZhengYuqun SWPD02506 2003-11-24
    if((nYear - 2000) == 0xFF || nMon == 0xFF || nDate == 0xFF || nHour == 0xFF || nMin == 0xFF || nSec == 0xFF)
    {
        ((char*)pCurField)[0] = '\0';
    }
    else
    { //End SWPD02506
        //将整型的年、月、日、时、分、秒转换成"YYYYMMDDHHNNSS"的字符串
        sprintf((char*)pCurField, "%4d%02d%02d%02d%02d%02d",
                    nYear, nMon, nDate, nHour, nMin, nSec);
    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The GetTextTimeNoSp Defining ******************/

/************ The FillFF Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           对某字段填"FF"
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::FillFF(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_GET_TEXT_FILLFF, 1);
        return -1;
    }

    if((pCurField == NULL) || (nSrcFields != 1) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //Modified by ZhengYuqun SWPD02477 2003-11-21

    //根据目标字段的数据类型决定是填充字符串还是填转换成十六进制的字节串
    switch(pFI->nDataType)
    {
    case FT_STRING:
        sprintf((char*)pCurField, "%s", "FF");
        //将目标格式域增加填充符和分隔符
        if(!nIsQuery)
        {
            //如果不是为查询条件转换
            return AddFillerSeparator((char*)pCurField, pFI);
        }
        break;

    //其它类型按照实际长度填充0xFF
    case FT_CHAR:
    case FT_BYTE:
    case FT_BCD:
    case FT_RBCD:
        memset(pCurField, 0xFF, pFI->nFieldLength);
        break;

    case FT_SHORT:
    case FT_USHORT:
        memset(pCurField, 0xFF, 2 * pFI->nFieldLength);
        break;

    case FT_LONG:
    case FT_ULONG:
        memset(pCurField, 0xFF, 4 * pFI->nFieldLength);
        break;

    case FT_INT8:
        memset(pCurField, 0xFF, 8 * pFI->nFieldLength);
        break;

    default:
        memset(pCurField, 0xFF, pFI->nFieldLength);
        break;
    }
    //End SWPD02477


    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The FillFF Defining ******************/

/************ The ChangeGSVNandCallType Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           对SoftX300固网详细话单的呼叫类别和业务类型进行转换
//           业务属性为本地长途的话单，业务属性都要改为本局，
//           呼叫类型原来是出局改为本局，原来是汇接的改为入局
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为2
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
#define GSVN_LOCAL_OFFICE           0   //本局
#define GSVN_LOCAL                  1   //本地
#define GSVN_LOCAL_TOLL             2   //本地长途
#define GSVN_NATIONAL_TOLL          3   //国内长途

#define CALL_TYPE_INTRA_OFFICE      1   //局内
#define CALL_TYPE_INCOMING_OFFICE   2   //入局
#define CALL_TYPE_OUTGOING_OFFICE   3   //出局
#define CALL_TYPE_TANDEM            4   //汇接

int CConvertFun::ChangeGSVNandCallType(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_CHANGE_GSVN_AND_CALLTYPE, 1);
        return -1;
    }
    if((pCurField == NULL) || (nSrcFields != 1) || (pSrcFields == NULL))
    {
        return -1;
    }

    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }


    BYTE nGSVN;          //存放业务类型
    BYTE nCallType;      //存放呼叫类别
    BYTE nTemp;          //存放业务类型和呼叫类别，业务类型和呼叫类别各占半个字节

    memcpy(&nTemp, pSrcFields[0], nSrcFieldLen[0]);
    nGSVN = nTemp >> 4;
    nCallType = nTemp & 0x0f;

    if(nGSVN == GSVN_LOCAL_TOLL)  //业务类型为本地长途
    {
        nGSVN = GSVN_LOCAL_OFFICE;
        if(nCallType == CALL_TYPE_OUTGOING_OFFICE)
        {
            nCallType = CALL_TYPE_INTRA_OFFICE;
        }

        if(nCallType == CALL_TYPE_TANDEM)
        {
            nCallType = CALL_TYPE_INCOMING_OFFICE;
        }

        nTemp = nGSVN << 4;
        nTemp += nCallType;
    }

    memcpy(pCurField, &nTemp, nSrcFieldLen[0]);

    return 0;
}

int CConvertFun::ChangeGSVNandCallType2Text(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_CHANGE_GSVN_AND_CALLTYPE_TEXT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }

    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }


    BYTE nGSVN;          //存放业务类型
    BYTE nCallType;      //存放呼叫类别
    BYTE nTemp;          //存放业务类型和呼叫类别，业务类型和呼叫类别各占半个字节

    memcpy(&nTemp, pSrcFields[0], nSrcFieldLen[0]);
    nGSVN = nTemp >> 4;
    nCallType = nTemp & 0x0f;

    if(nGSVN == GSVN_LOCAL_TOLL)  //业务类型为本地长途
    {
        nGSVN = GSVN_LOCAL_OFFICE;
        if(nCallType == CALL_TYPE_OUTGOING_OFFICE)
        {
            nCallType = CALL_TYPE_INTRA_OFFICE;
        }

        if(nCallType == CALL_TYPE_TANDEM)
        {
            nCallType = CALL_TYPE_INCOMING_OFFICE;
        }
    }

    sprintf((char*)pCurField, "%-24d;%-24d", nCallType, nGSVN);

    //将目标格式域增加填充符和分隔符
    return AddFillerSeparator((char*)pCurField, pFieldInfo);
}

/************ End of The ChangeGSVNandCallType Defining ******************/

//by ldf 2003-01-08 对应问题单D05886 D05556（同D05415 D5150）
/************ The Int2IpTextDirect Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现将各种整型数据转换成文本,但是由于字节序的问题，需要重新生成一个函数，
//           本函数将 (10)(11)(243)(197) 的数组 输出为 197.243.11.10
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           nSrcFieldLen[0]只能为5，表示pSrcFields[0]
//           指向数据类型为UINT4型的数据
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
/***************************************************************/
int CConvertFun::Int2IpTextDirect(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    UINT4 nTmp = 0;
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_INT_TO_IP_TEXT_DIRECT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //获取原格式域的整型值
    if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nTmp = uTmp;
    }
    else
    {
        return -1;
    }

    //把整数值转换成相应的字符串
    BYTE pTmpBuf[4];

    //先把UINT4的4字节分拆到缓冲数组中
    memset((void*)pTmpBuf, 0, 4);
    memcpy((void*)pTmpBuf, &nTmp, 4);

    char szIpAddr[256];

    sprintf(szIpAddr, "%d.%d.%d.%d\0", pTmpBuf[3],pTmpBuf[2],pTmpBuf[1],pTmpBuf[0]);

    strcpy((char*)pCurField, szIpAddr);

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The Int2IpTextDirect Defining *********************/

/************ The Array2Text Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现将各种整型数据转换成文本
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           指向数据类型为BYTE型的数据
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::Array2Text(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_ARRAY_TO_TEXT, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    int nBufLen = nSrcFieldLen[0];

    char*  pCurStr   = NULL;
    pCurStr = (char*)pCurField;

    BYTE*  pArray = NULL;
    pArray = (BYTE*)pSrcFields[0];

    for(int nIndex=0; nIndex < nBufLen; nIndex++)
    {
        int yCurNum = (int)pArray[nIndex];
        sprintf ( (char *)&pCurStr[nIndex*2], "%02d", yCurNum );
    }

    pCurStr[nBufLen*2] ='\0';


    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}

/************ End of The Array2Text Defining *********************/
//修改完毕


/************ The Array2HexString Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现将各种整型数据转换成文本
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           指向数据类型为BYTE型的数据
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::Array2HexString(void* pCurField, const UINT4 nSrcFields,
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFI,
                                 const void* pDefault, const UINT4 nDefaultLen,
                                 int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_ARRAY_TO_HEX_STRING, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    UINT4 nByteCount = nSrcFieldLen[0];
    for(UINT4 nIndex=0; nIndex < nByteCount; nIndex++)
    {
        BYTE yCurNum = ((BYTE*)pSrcFields[0])[nIndex];
        sprintf (&((char *)pCurField)[nIndex*3], "%02x ", yCurNum );
    }

    ((char *)pCurField)[nByteCount*3] ='\0';


    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The Array2HexString Defining *********************/

/************ The FillInvaidValue Defining ****************************/
// 功能描述：对字节填充FF,为转换所使用
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           指向数据类型为BYTE型的数据
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::FillInvaidValue(void* pCurField, const UINT4 nSrcFields,
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFI,
                                 const void* pDefault, const UINT4 nDefaultLen,
                                 int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_FILL_INVALID_VALUE, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //目标格式该域的长度
    UINT4 nByteCount = pFI->nFieldLength;

    for(UINT4 nIndex=0; nIndex < nByteCount; nIndex++)
    {
        ((BYTE *)pCurField)[nIndex] = 0XFF;
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The FillInvaidValue Defining *********************/


/************ The GetTextTimeEx Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS.MS
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为6或者7
//           pSrcFields[0]指向数据类型为SHORT型的4位“年”，
//           pSrcFields[1]、pSrcFields[2]、pSrcFields[3]、pSrcFields[4]、
//           pSrcFields[5]分别指向BYTE型月、日、时、分、秒
//           pSrcFields[6]如果存在,则表示毫秒
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::GetTextTimeEx(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    INT2  nYear =  0;    //存放两位年的临时变量
    BYTE  nMon =   0;    //存放月的临时变量
    BYTE  nDate =  0;    //存放日的临时变量
    BYTE  nHour =  0;    //存放时的临时变量
    BYTE  nMin =   0;    //存放分的临时变量
    BYTE  nSec =   0;    //存放秒的临时变量
    BYTE  nMilliSec = 0; //存放毫秒的临时变量

    //确保输入参数正确
    if((pCurField == NULL) || (nSrcFields != 6 && nSrcFields != 7) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //获取年、月、日、时、分、秒的值
    nYear  =  *(INT2*)pSrcFields[0];
    nMon   =  *(BYTE*)pSrcFields[1];
    nDate  =  *(BYTE*)pSrcFields[2];
    nHour  =  *(BYTE*)pSrcFields[3];
    nMin   =  *(BYTE*)pSrcFields[4];
    nSec   =  *(BYTE*)pSrcFields[5];

    //Added by ZhengYuqun SWPD02506 2003-11-24
    if(nYear == 0xFFFF || nMon == 0xFF || nDate == 0xFF || nHour == 0xFF || nMin == 0xFF || nSec == 0xFF)
    {
        ((char*)pCurField)[0] = '\0';
    }//End SWPD02506
    else if(nSrcFields == 7)
    {
        nMilliSec = *(BYTE*)pSrcFields[6];
        //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS"的字符串
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d.%d",
            nYear, nMon, nDate, nHour, nMin, nSec, nMilliSec);
    }
    else
    {
        //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS"的字符串
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDate, nHour, nMin, nSec);
    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The GetTextTime Defining ******************/

//added by maimaoshi ,2003-03-10
/************ The ToLRLN Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现填写"\r\n"
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
//     注意：暂时不支持INT8类型转换
/***************************************************************/
int CConvertFun::ToLRLN(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_TO_LRLN, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //将此字段填"\r\n"
    sprintf((char*)pCurField, "%s", "\r\n");

    return 0;
}
/************ End of The ToLRLN Defining *********************/

/************ The ToLR Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现填写"\r"
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
//     注意：暂时不支持INT8类型转换
/***************************************************************/
int CConvertFun::ToLR(void* pCurField, const UINT4 nSrcFields,
                      const void* pSrcFields[],
                      const UINT4 nSrcFieldLen[],
                      const SFieldInfo* pFI,
                      const void* pDefault, const UINT4 nDefaultLen,
                      int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_TO_LR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //将此字段填"\r"
    sprintf((char*)pCurField, "%s", "\r");

    return 0;
}
/************ End of The ToLR Defining *********************/


/************ The ToLN Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现填写"\n"
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
//     注意：暂时不支持INT8类型转换
/***************************************************************/
int CConvertFun::ToLN(void* pCurField, const UINT4 nSrcFields,
                      const void* pSrcFields[],
                      const UINT4 nSrcFieldLen[],
                      const SFieldInfo* pFI,
                      const void* pDefault, const UINT4 nDefaultLen,
                      int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_TO_LN, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //将此字段填"\r"
    sprintf((char*)pCurField, "%s", "\n");

    return 0;
}
/************ End of The ToLN Defining *********************/


/************ The StringCopy Defining ***************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现字符串的拷贝
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向源数据块的内存，
//           nSrcFieldLen[0]表示源数据块的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::StringCopy(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    INT4 nEnumVal = 0;
    ENUMINFO_LIST::iterator item;       //枚举链表迭代子
    ENUMINFO_LIST* pEnumInfos = NULL;   //格式域枚举链表指针变量

    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_STRING_COPY, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //把原格式相应域中的数据内容拷贝到目标格式当前域中
    strncpy((char*)pCurField, (const char*)pSrcFields[0], nSrcFieldLen[0]);
    ((char*)pCurField)[nSrcFieldLen[0]] = '\0';

    //如果当前域是枚举类型
    if(pFieldInfo->flgEnum != 0)
    {
        StringLRTrim((char*)pCurField);
        nEnumVal = atol((char*)pCurField);

        //获取格式域枚举链表
        pEnumInfos = (ENUMINFO_LIST*)(pFieldInfo->pEnumInfos);
        if(pEnumInfos == NULL)
        {
            return -1;
        }

        //顺序搜索格式域枚举链表中的每一个节点
        item = pEnumInfos->begin();
        while(item != pEnumInfos->end())
        {
            //如果当前节点的枚举值等于需要的枚举值
            if(item->nEnumValue == nEnumVal)
            {
                //获取当前枚举值对应的枚举字符串信息
                strcpy((char*)pCurField, item->szEnumValue);
                break;
            }

            item++;
        }

    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFieldInfo);
    }


    return 0;
}
/************ End of The StringCopy Defining ********************/
/************ The ByteText2 Defining ****************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现将各种整型数据转换成文本
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           nSrcFieldLen[0]为1、2、4、8时分别表示pSrcFields[0]
//           指向数据类型为BYTE、SHORT、LONG、INT8型的数据
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
//     注意：暂时不支持INT8类型转换
/***************************************************************/
int CConvertFun::ByteText2(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    INT4  nTmp = 0;    //存放BYTE、SHORT、LONG的临时变量



    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_BYTE_TEXT2, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //获取原格式域的整型值
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        nTmp = uTmp;
    }
    else
    {
        return -1;
    }

    //把整数值转换成相应的字符串
    sprintf((char*)pCurField, "%02d", nTmp);

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The ByteText2 Defining *********************/


/************ The GetTextTimeInString Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现年、月、日、时、分、秒转换成文本格式的YYYY-MM-DD HH:NN:SS.MS
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为6
//           pSrcFields[0]指向数据类型为BYTE或SHORT型的2位“年”，
//           pSrcFields[1]、pSrcFields[2]、pSrcFields[3]、pSrcFields[4]、
//           pSrcFields[5]分别指向BYTE型月、日、时、分、秒，
//           如果nSrcFieldLen[0]为1，表示pSrcFields[0]为BYTE型的年，
//           如果nSrcFieldLen[0]为2，表示pSrcFields[0]为SHORT型的年
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::GetTextTimeInString(void* pCurField, const UINT4 nSrcFields,
                             const void* pSrcFields[],
                             const UINT4 nSrcFieldLen[],
                             const SFieldInfo* pFI,
                             const void* pDefault, const UINT4 nDefaultLen,
                             int nIsQuery)
{
    INT2  nYear =  0;    //存放两位年的临时变量
    BYTE  nMon =   0;    //存放月的临时变量
    BYTE  nDate =  0;    //存放日的临时变量
    BYTE  nHour =  0;    //存放时的临时变量
    BYTE  nMin =   0;    //存放分的临时变量
    BYTE  nSec =   0;    //存放秒的临时变量
    char  szTmp[5];

    //确保输入参数正确
    if((pCurField == NULL) || (nSrcFields != 3 && nSrcFields != 6) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }

    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    if (nSrcFields == 6)
    {
        //获取年、月、日、时、分、秒的值
        strncpy(szTmp, (const char*)pSrcFields[0], nSrcFieldLen[0]);
        szTmp[nSrcFieldLen[0]] = '\0';
        nYear = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[1], nSrcFieldLen[1]);
        szTmp[nSrcFieldLen[1]] = '\0';
        nMon = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[2], nSrcFieldLen[2]);
        szTmp[nSrcFieldLen[2]] = '\0';
        nDate = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[3], nSrcFieldLen[3]);
        szTmp[nSrcFieldLen[3]] = '\0';
        nHour = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[4], nSrcFieldLen[4]);
        szTmp[nSrcFieldLen[4]] = '\0';
        nMin = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[5], nSrcFieldLen[5]);
        szTmp[nSrcFieldLen[5]] = '\0';
        nSec = atoi(szTmp);


        //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS"的字符串
        sprintf((char*)pCurField, "%4d-%02d-%02d %02d:%02d:%02d",
                nYear, nMon, nDate, nHour, nMin, nSec);
    }
    else if (nSrcFields == 3)
    {
        strncpy(szTmp, (const char*)pSrcFields[0], nSrcFieldLen[0]);
        szTmp[nSrcFieldLen[0]] = '\0';
        nHour = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[1], nSrcFieldLen[1]);
        szTmp[nSrcFieldLen[1]] = '\0';
        nMin = atoi(szTmp);

        strncpy(szTmp, (const char*)pSrcFields[2], nSrcFieldLen[2]);
        szTmp[nSrcFieldLen[2]] = '\0';
        nSec = atoi(szTmp);


        //将整型的年、月、日、时、分、秒转换成"YYYY-MM-DD HH:NN:SS"的字符串
        sprintf((char*)pCurField, "%02d:%02d:%02d", nHour, nMin, nSec);
    }
    else
    {
        return -1;
    }


    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The GetTextTimeInString Defining ******************/
//end by maimashi 2003-03-10
//by ldf 2003-04-01
int CConvertFun::CheckSum(void* pCurField, const UINT4 nSrcFields,
                          const void* pSrcFields[],
                          const UINT4 nSrcFieldLen[],
                          const SFieldInfo* pFI,
                          const void* pDefault, const UINT4 nDefaultLen,
                          int nIsQuery)
{
    if (!pCurField || nSrcFieldLen <= 0 || !pSrcFields)
    {
        return -1;
    }

    UINT4 nFieldLen = nSrcFieldLen[0];
    unsigned char* pData = (unsigned char*)pSrcFields[0];
    unsigned char uCheckSum = 0;

    UINT4 nIndex = 0;
    //计算校验和，按128计算校验和的方法，只进行逐字节累加
    for(UINT4 i = 0; i < nSrcFields; i++)
    {
        pData = (unsigned char*)pSrcFields[i];
        nIndex = 0;
        nFieldLen = nSrcFieldLen[i];
        while (nIndex < nFieldLen)
        {
            uCheckSum += pData[nIndex];
            nIndex ++;
        }
    }

    memcpy(pCurField, &uCheckSum, 1);

    return 0;
}
//end

/************ The FillWithStr Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           对某字段填写某一个常量的字符串
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//修改记录：
//2003-11-20 郑育群  使其支持二进制也支持文本
/***************************************************************/
int CConvertFun::FillWithStr(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_FILL_WITH_STR, 1);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //Modified by ZhengYuqun SWPD02477 2003-11-21

    //根据目标字段的数据类型决定是填充字符串还是填转换成十六进制的字节串
    if(pFI->nDataType == FT_STRING) //只有STRING类型按字符串转
    {
        int nLen = min(nSrcFieldLen[0], pFI->nFieldLength);
        memcpy(pCurField, pSrcFields[0], nLen);
        //将目标格式域增加填充符和分隔符
        if(!nIsQuery)
        {
            //如果不是为查询条件转换
            return AddFillerSeparator((char*)pCurField, pFI);
        }
    }
    else
    {
        int nLen = pFI->nFieldLength;
        switch(pFI->nDataType)
        {
            case FT_SHORT:
            case FT_USHORT:
                nLen *= 2;
                break;

            case FT_LONG:
            case FT_ULONG:
                nLen *= 4;
                break;

            case FT_INT8:
                nLen *= 8;
                break;

            default:
                break;
                //其它都按1字节长度处理
        }

        //先组合出字符串然后再转换为十六进制串
        char szTemp[FMT_FIELD_MAX_LEN * 2 + 1];
        char cFillChar = pFI->cFillChar;

        memcpy(szTemp, pSrcFields[0], nSrcFieldLen[0]);
        if(cFillChar == ' ')  //如果填充符是空格，那么改为0x00
        {
            cFillChar = '0';
        }

        //根据目标字段长度的2倍先用填充符填充完szTemp
        if(nSrcFieldLen[0] - 1 < nLen * 2)
        {
            memset(szTemp + nSrcFieldLen[0] - 1, cFillChar, nLen * 2 - nSrcFieldLen[0] + 1);
            szTemp[nLen * 2] = '\0';
        }

        StringtoBCD(szTemp, (BYTE*)pCurField, nLen);
    }

    //End SWPD02477

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;

}
/************ End of The FillWithStr Defining ******************/
/************ The FillByIniFile Defining *************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           实现往格式库中添加一个INI文件中的指定键值
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为2，
//           pSrcFields[0]指向数据类型为char，
//           pSrcFields[1]指向数据类型为char
//           
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::FillByIniFile(void* pCurField, const UINT4 nSrcFields, 
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{
	//确保输入参数正确
    if((pCurField == NULL) || (nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    //根据目标字段的数据类型决定是否填充INI文件中的值
    if((pFI->nDataType == FT_STRING)  || (pFI->nDataType == FT_CHAR))
    {
        static MAP<STRING, STRING> iniFileMap;
        static ACE_Mutex   mutex;  

        STRING strKeyName = (char*)pSrcFields[0];
        strKeyName += (char*)pSrcFields[1];
        
        mutex.acquire();

        MAP<STRING, STRING>::iterator iter;
        iter = iniFileMap.find(strKeyName);
        if(iter == iniFileMap.end())
        {
            //缓冲区Map中没有数据，从文件中加载
            CINIFile iniFile("./config/ini/igwb.ini");
            if(!iniFile.Open())
            {
                mutex.release();
                return -1;
            }

            char szValue[64] = "";
            iniFile.GetString((char*)pSrcFields[0],              
                              (char*)pSrcFields[1],                      //APName
                              "",
                              szValue,
                              sizeof(szValue));
            szValue[sizeof(szValue) - 1] = '\0';

            iniFileMap[strKeyName] = szValue;
        }

	    int nLen = min(iniFileMap[strKeyName].length() + 1, pFI->nFieldLength);
        memcpy(pCurField, iniFileMap[strKeyName].c_str(), nLen);
        
        mutex.release();
	    //将目标格式域增加填充符和分隔符
        if(!nIsQuery)
        {
            //如果不是为查询条件转换
            return AddFillerSeparator((char*)pCurField, pFI);
        }
    }
    else
    {
        return -1;
    }


    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;

}
/************ End of The FillByIniFile Defining ******************/


#define AREA_CODE_COUNT 108
const char* g_szAreaCode[AREA_CODE_COUNT] = {
        "022  1 天津.天津",
        "024  5 辽宁.沈阳",
        "0310 2 河北.邯郸",
        "0311 2 河北.石家庄",
        "0312 2 河北.保定",
        "0313 2 河北.张家口",
        "0314 2 河北.承德",
        "0315 2 河北.唐山",
        "0316 2 河北.廊坊",
        "0317 2 河北.沧州",
        "0318 2 河北.衡水",
        "0319 2 河北.邢台",
        "0335 2 河北.秦皇岛",
        "0349 8 山西.朔州",
        "0350 8 山西.忻州",
        "0351 8 山西.太原",
        "0352 8 山西.大同",
        "0353 8 山西.阳泉",
        "0354 8 山西.榆次",
        "0355 8 山西.长治",
        "0356 8 山西.晋城",
        "0357 8 山西.临汾",
        "0358 8 山西.离石",
        "0359 8 山西.运城",
        "0370 9 河南.商丘",
        "0371 9 河南.郑州",
        "0372 9 河南.安阳",
        "0373 9 河南.新乡",
        "0374 9 河南.许昌",
        "0375 9 河南.平顶山",
        "0376 9 河南.信阳",
        "0377 9 河南.南阳",
        "0378 9 河南.开封",
        "0379 9 河南.洛阳",
        "0391 9 河南.焦作",
        "0392 9 河南.鹤壁",
        "0393 9 河南.濮阳",
        "0394 9 河南.周口",
        "0395 9 河南.漯河",
        "0396 9 河南.驻马店",
        "0397 9 河南.潢川",
        "0398 9 河南.三门峡",
        "0410 5 辽宁.铁岭",
        "0411 5 辽宁.大连",
        "0412 5 辽宁.鞍山",
        "0413 5 辽宁.抚顺",
        "0414 5 辽宁.本溪",
        "0415 5 辽宁.丹东",
        "0416 5 辽宁.锦州",
        "0417 5 辽宁.营口",
        "0418 5 辽宁.阜新",
        "0419 5 辽宁.辽阳",
        "0421 5 辽宁.朝阳",
        "0427 5 辽宁.盘锦",
        "0429 5 辽宁.葫芦岛",
        "0431 6 吉林.长春",
        "0432 6 吉林.吉林",
        "0433 6 吉林.延边",
        "0434 6 吉林.四平",
        "0435 6 吉林.通化",
        "0436 6 吉林.白城",
        "0437 6 吉林.辽源",
        "0438 6 吉林.松原",
        "0439 6 吉林.白山",
        "0440 6 吉林.珲春",
        "0448 6 吉林.梅河口",
        "0451 7 黑龙江.哈尔滨",
        "0452 7 黑龙江.齐齐哈尔",
        "0453 7 黑龙江.牡丹江",
        "0454 7 黑龙江.佳木斯",
        "0455 7 黑龙江.绥化",
        "0456 7 黑龙江.黑河",
        "0457 7 黑龙江.大兴安岭",
        "0458 7 黑龙江.伊春",
        "0459 7 黑龙江.大庆",
        "0464 7 黑龙江-七台河",
        "0467 7 黑龙江-鸡西",
        "0468 7 黑龙江-鹤岗",
        "0469 7 黑龙江-双鸭山",
        "0470 4 内蒙古.呼伦贝尔盟",
        "0471 4 内蒙古.呼和浩特",
        "0472 4 内蒙古.包头",
        "0473 4 内蒙古.乌海",
        "0474 4 内蒙古.乌兰查布盟",
        "0475 4 内蒙古.通辽",
        "0476 4 内蒙古.赤峰",
        "0477 4 内蒙古.伊克昭盟",
        "0478 4 内蒙古.巴彦卓尔盟",
        "0479 4 内蒙古.锡林浩特",
        "0482 4 内蒙古-兴安盟",
        "0483 4 内蒙古-阿拉善盟",
        "0530 3 山东.菏泽",
        "0531 3 济南",
        "0532 3 山东.青岛",
        "0533 3 山东.淄博",
        "0534 3 山东.德州",
        "0535 3 山东.烟台",
        "0536 3 山东.潍坊",
        "0537 3 山东.济宁",
        "0538 3 山东.泰安",
        "0539 3 山东.临沂",
        "0543 3 山东.滨州",
        "0546 3 山东.东营",
        "0631 3 山东.威海",
        "0632 3 山东.枣庄",
        "0633 3 山东.日照",
        "0634 3 山东.莱芜",
        "0635 3 山东.聊城"
    };

/****************** The GetAreaID Defining *************************/
// 功能描述：获取电话号码的前n位，并转换成相应的整数,
//           实现从主叫号码中返回区号ID
//
// 输入参数：
//  nParasNum：参数个数，此函数的参数个数应该为2
//     pParas：参数数组，此函数包含两个参数，pParas[0]表示电话号码字符串
//             pParas[1]表示要获取电话号码的前n个数字
//  nParasLen：各参数长度数组
//   pDefault：函数缺省值
//
// 输出参数：SVariant类型的结果
//
//   返回值：计算成功，则返回0，如果出错返回非零
//
/****************************************************************/
int CConvertFun::GetAreaID(const UINT4 nParasNum,
                           const void* pParas[],
                           const UINT4 nParasLen[],
                           const STRING& pDefault,
                           SVariant& stVar)
{
    //确保输入参数的正确性
    if((nParasNum != 1) || (pParas == NULL))
    {
        return -1;
    }

    //确保参数正确
    for(int i = 0; i < nParasNum; i++)
    {
        if((pParas[i] == NULL) || nParasLen[i] == 0)
        {
            return -1;
        }
    }

    static MAP<STRING, int> map_code_to_id;
    static ACE_Mutex mutex;

    mutex.acquire();
    if(map_code_to_id.size() == 0)
    {
        char szAreaCode[10];
        char szTemp[100];
        int  nAreaID;

        FILE* f = fopen("./config/area/area.map", "rt");
        if(f != NULL)
        {
            char szLine[1024];
            while(!feof(f))
            {
                char* p = fgets(szLine, sizeof(szLine), f);
                if(p != NULL)
                {
                    int nRead = sscanf(szLine, "%s%d%s", szAreaCode, &nAreaID, szTemp);
                    if(nRead >= 2)
                    {
                        szAreaCode[sizeof(szAreaCode) - 1] = '\0';
                        map_code_to_id[szAreaCode] = nAreaID;
                    }
                }
            }
            fclose(f);
        }

        if(map_code_to_id.size() == 0)
        {
            for(int i = 0; i < AREA_CODE_COUNT; i++)
            {
                int nRead = sscanf(g_szAreaCode[i], "%s%d%s", szAreaCode, &nAreaID, szTemp);
                if(nRead >= 2)
                {
                    szAreaCode[sizeof(szAreaCode) - 1] = '\0';
                    map_code_to_id[szAreaCode] = nAreaID;
                }
            }
        }
    }

    char szTemp[10];
    memcpy(szTemp, (char*)pParas[0], 4);
    szTemp[4] = '\0';
    int nAreaID = 0;
    if(map_code_to_id.find(szTemp) != map_code_to_id.end())
    {
        nAreaID = map_code_to_id[szTemp];
    }
    else
    {
        memcpy(szTemp, (char*)pParas[0], 3);
        szTemp[3] = '\0';
        if(map_code_to_id.find(szTemp) != map_code_to_id.end())
        {
            nAreaID = map_code_to_id[szTemp];
        }
    }
    mutex.release();

    //把号码字符串的前uPreNum个数字转换成整数
    stVar.yVariantType = FT_ULONG;
    stVar.u4Value = nAreaID;

    return 0;
}
/************ End of The GetAreaID Defining ******************/

/************ The PadAreacode Defining ***************************/
// 功能描述：给用户号码前面添加区号，如果区号已经存在，则不添加。
//            添加原则:
//            1. 对于话单中的主叫号码，如果地址属性为本地号码，就根据主叫本地号首集给
//               主叫加上区号，如果地址属性为其他值，就不做任何处理；
//            2. 对于话单中的被叫号码，如果地址属性为本地号码，就根据主叫本地号首集给
//               主叫加上区号，如果地址属性为其他值，就不做任何处理；此外，对于13移动
//               号码，不要加区号;           
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为4，
//           pSrcFields[0]/nSrcFieldLen[0] : 用户号码/用户号码字段的长度(主叫或者被叫)
//           pSrcFields[1]/nSrcFieldLen[1] : 号码属性/号码属性对应的字段长度
//           pSrcFields[2]/nSrcFieldLen[2] : 主叫用户号码对应的号首集/号首集字段长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
// 返回值：成功返回0，失败返回-1
// 
// Added by ZhengYuqun , SWPD01198, 为SoftX3000固网的话单中的号码增加区号, 2003-08-27 
//修改记录：
//2004-02-23 郑育群 修改本函数支持目标字段为文本
/***************************************************************/
int CConvertFun::PadAreacode(void* pCurField, const UINT4 nSrcFields,
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFieldInfo,
                           const void* pDefault, const UINT4 nDefaultLen,
                           int nIsQuery)
{
#define ADDRESS_NATURE_SUBSCRIBER         0      //用户号码
#define ADDRESS_NATURE_RESERVED           1      //备用
#define ADDRESS_NATURE_DOMESTIC           2      //国内号码
#define ADDRESS_NATURE_INTERNATIONAL      3      //国际号码

    static MAP<UINT4, STRING> mapDNSet2AreaCode;
	static BOOL bIsFirstRun = TRUE;

    char* pszCurField = (char *)pCurField;

    if (bIsFirstRun && mapDNSet2AreaCode.size() == 0)
    {
        //读取并保存号首集和区号的对照表
		FILE* f = fopen("./config/area/area.map", "rt");
        if(f != NULL)
        {
            char szLine[1024];
            while(!feof(f))
            {
                char szAreaCode[128];
                UINT4 nDNSet;
                char* p = fgets(szLine, sizeof(szLine), f);
                if(p != NULL)
                {
                    int nRead = sscanf(szLine, "%d%s", &nDNSet, szAreaCode);
                    if(nRead >= 2)
                    {
                        szAreaCode[sizeof(szAreaCode) - 1] = '\0';
                        mapDNSet2AreaCode[nDNSet] = szAreaCode;
                    }
                }
            }
            fclose(f);
        }

		bIsFirstRun = FALSE; //防止mapDNSet2AreaCode为空的时候多次试图打开文件
    }

    //确保输入参数正确
    if ((pCurField == NULL) || (nSrcFields != 3) || (pSrcFields == NULL))
    {
        return -1;
    }

    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)
     || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0)
     || (pSrcFields[2] == NULL) || (nSrcFieldLen[2] == 0))
    {
        return -1;
    }

    //电话号码必须是BCD码或者文本
    if (pFieldInfo[0].nDataType != FT_BCD && pFieldInfo[0].nDataType != FT_STRING) 
    {
        return -1;
    }

    //获得号码属性
    BYTE nAddrNature;
    if (nSrcFieldLen[1] == LEN_BYTE)
    {
        memcpy(&nAddrNature, pSrcFields[1], LEN_BYTE);
    }
    else
    {
        return -1;
    }

    //获得号首集
    UINT2 nDNSet = 0;
    if (nSrcFieldLen[2] == LEN_BYTE)
    {
        nDNSet = (UINT2)*(BYTE*)pSrcFields[2];
    }
    else if(nSrcFieldLen[2] == LEN_INT2) //兼容将来号首集为2个字节
    {
        memcpy((void*)&nDNSet, pSrcFields[2], LEN_INT2);
        //nDNSet  = (UINT2)ITOHS(nDNSet);
    }
    else
    {
        return -1;
    }

    char szSubscriber[128];
    BCDtoString((BYTE*)pSrcFields[0], szSubscriber, nSrcFieldLen[0]);
    char* pF = strchr(szSubscriber, 'F');
    if(pF != NULL)
    {
        *pF = '\0';
    }

    //国内号码、以0或者13开头的号码，不处理
    if ((nAddrNature != ADDRESS_NATURE_SUBSCRIBER) 
     || (strncmp(szSubscriber, "13", 2) == 0)
     || (szSubscriber[0] == '0')) 
    {
        if (pFieldInfo[0].nDataType == FT_STRING)
        {
            if (pFieldInfo->nFieldLength < strlen(szSubscriber))
            {
                strncpy(pszCurField, szSubscriber, pFieldInfo->nFieldLength);                
                pszCurField[pFieldInfo->nFieldLength - 1] = '\0';
            }
            else
            {
                strcpy(pszCurField, szSubscriber);
            }           
 
            if (nIsQuery == 0)
            {
                return AddFillerSeparator(pszCurField, pFieldInfo);        
            }
        }
        else
        {
            memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);            
        }

        return 0;
    }

    //先根据号首集查找区号
	if (mapDNSet2AreaCode.find(nDNSet) == mapDNSet2AreaCode.end())
    {
        //如果找不到对应的号首集，直接拷贝电话号码
        if (pFieldInfo[0].nDataType == FT_STRING)
        {
            if (pFieldInfo->nFieldLength < strlen(szSubscriber))
            {
                strncpy(pszCurField, szSubscriber, pFieldInfo->nFieldLength);
                pszCurField[pFieldInfo->nFieldLength - 1] = '\0';
            }
            else
            {
                strcpy(pszCurField, szSubscriber);
            }    
            
            if (nIsQuery == 0)
            {
                return AddFillerSeparator(pszCurField, pFieldInfo);        
            }
        }
        else
        {
            memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);            
        }
    }
    else
    {
        int i = 0;
        char szAreaCode[128];
        szAreaCode[0] = '\0';

        //从MAP中获得本号首集所对应的区号
        strncpy(szAreaCode, mapDNSet2AreaCode[nDNSet].c_str(), sizeof(szAreaCode));
        szAreaCode[sizeof(szAreaCode) - 1] = '\0';


        //判断号码的前几位是否等于区号
        for (i = 0; ((i < strlen(szAreaCode)) && (i < strlen(szSubscriber))); i++)
        {
            if (szAreaCode[i] != szSubscriber[i])
            {
                break;
            }
        }

        if (i == strlen(szAreaCode))
        {
            //号码前部已经包含区号，直接拷贝就可以了
            if (pFieldInfo[0].nDataType == FT_STRING)
            {
                if (pFieldInfo->nFieldLength < strlen(szSubscriber))
                {
                    strncpy(pszCurField, szSubscriber, pFieldInfo->nFieldLength);
                    pszCurField[pFieldInfo->nFieldLength - 1] = '\0';
                }
                else
                {
                    strcpy(pszCurField, szSubscriber);
                }    

                if (nIsQuery == 0)
                {
                    AddFillerSeparator(pszCurField, pFieldInfo);        
                }
            }
            else
            {
                memcpy(pCurField, pSrcFields[0], nSrcFieldLen[0]);            
            }
        }
        else
        {
            //号码没有包含区号，先将区号和电话号码进行字符串拼接
            strncat(szAreaCode, szSubscriber, sizeof(szAreaCode) - strlen(szAreaCode));
            
            if (pFieldInfo[0].nDataType == FT_STRING)
            {
                if (pFieldInfo->nFieldLength < strlen(szAreaCode))
                {
                    strncpy(pszCurField, szAreaCode, pFieldInfo->nFieldLength);
                    pszCurField[pFieldInfo->nFieldLength - 1] = '\0';
                }
                else
                {
                    strcpy(pszCurField, szAreaCode);
                }    

                if (nIsQuery == 0)
                {
                    AddFillerSeparator(pszCurField, pFieldInfo);        
                }
            }
            else
            {
                //将拼接后的带区号的电话号码转换为BCD码直接写到结果，长度不超过源字段的长度
                //StringtoBCD参数表修改，所以将第三个参数的&去掉 ZhengYuqun 2003-11-21
                StringtoBCD(szAreaCode, (BYTE*)pCurField, nSrcFieldLen[0]);  
            }
        }
    }

    return 0;
}

/************ End of The PadAreacode Defining ******************/


/************ The ReplacePrefix Defining ***************************/
// 功能描述：用新的前缀替换电话号码中旧的前缀，并根据配置决定是否增加号首集。
//           替换原则:
//           电话号码前缀按最大匹配原则与前缀表中的前缀进行匹配。                
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1或者2，
// 
//           pSrcFields[0]/nSrcFieldLen[0] : 用户号码前一部分/用户号码前一部分的长度/必选参数
//           pSrcFields[1]/nSrcFieldLen[1] : 号首集字段/号首集字段的长度/可选参数
//           pSrcFields[2]/nSrcFieldLen[2] : 缺省是否增加区号/可选参数
//           pSrcFields[3]/nSrcFieldLen[3] : 用户号码后一部分/用户号码后一部分的长度/可选参数
//           因为常数是排在最后的，因此有四个参数时，用户号码后一部分在pSrcFields[2]，
//           而缺省是否增加区号在pSrcFields[3]
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
// 返回值：成功返回0，失败返回-1
// 
// Added by ZhengYuqun , SWPD01198, 为SoftX3000固网的话单中的号码增加区号, 2003-08-27 
// Modify by chenliangwei, 2003-03-20
//修改记录：
/***************************************************************/
typedef struct _SPrefixCvtInfo
{
    STRING  strNewPrefix;
    int     bPadAreaCode;    
} SPrefixCvtInfo;

typedef MAP<STRING, SPrefixCvtInfo> PREFIX_MAP;

int CConvertFun::ReplacePrefix(void* pCurField, const UINT4 nSrcFields,
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFieldInfo,
                               const void* pDefault, const UINT4 nDefaultLen,
                               int nIsQuery)
{

    //确保输入参数正确
    if(nSrcFields < 1 || nSrcFields > 4)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
              FUN_BCD_TO_STR, nSrcFields);
        return -1;
    }
    
    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }

    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //电话号码必须是BCD码或者文本
    if (pFieldInfo[0].nDataType != FT_BCD && pFieldInfo[0].nDataType != FT_STRING) 
    {
        return -1;
    }

    //获得号首集
    UINT2 nDNSet = 0;
    if(nSrcFields > 1)
    {
        if (nSrcFieldLen[1] == LEN_BYTE)
        {
            nDNSet = (UINT2)*(BYTE*)pSrcFields[1];
        }
        else if(nSrcFieldLen[1] == LEN_INT2) //兼容将来号首集为2个字节
        {
            memcpy((void*)&nDNSet, pSrcFields[1], LEN_INT2);
            //nDNSet  = (UINT2)ITOHS(nDNSet);
        }
        else
        {
            return -1;
        }
    }

    //缺省是否增加区号
    BOOL bDefPadAreaCode = 0; 
    if((nSrcFields == 3) || (nSrcFields == 4))
    {
        bDefPadAreaCode = atoi((char*)pSrcFields[nSrcFields - 1]);
    }

    //组装电话号码
    BYTE pNumber[40];
    int  nSrcNumLen = 0;
    if(nSrcFields != 4)
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

    }
    else
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

        memcpy(pNumber + nSrcNumLen, pSrcFields[2], nSrcFieldLen[2]);
        nSrcNumLen += nSrcFieldLen[1];
        
    }




    static PREFIX_MAP mapPrefix;
    static MAP<UINT4, STRING> mapDNSet2AreaCode;
	static BOOL bIsFirstRun = TRUE;
    static ACE_Mutex mutex;    

    /*
     * 加载前缀表的配置文件。
     */
    mutex.acquire();
    if(bIsFirstRun)
    {
        //读取并保存号首集和区号的对照表
		FILE* f = fopen("./config/area/prefix.map", "rt");
        if(f != NULL)
        {
            char szLine[1024];
            while(!feof(f))
            {
                char szOldPrefix[128] = "";
                char szNewPrefix[128] = "";
                char szbAddAreaCode[3] = "";

                char* p = fgets(szLine, sizeof(szLine), f);
                if((p != NULL) && (szLine[0] != '\0'))
                {
                    //对"旧前缀,新前缀,是否加区号"进行解析
                    char* pDiv = strchr(szLine, ',');
                    if(pDiv == NULL)
                    {
                        //没有逗号进行分隔，说明只有一个旧前缀，并获得它
                        strncpy(szOldPrefix, szLine, sizeof(szOldPrefix));
                        szOldPrefix[sizeof(szOldPrefix) - 1] = '\0';
                    }
                    else
                    {
                        //获得旧前缀
                        *pDiv = '\0';
                        strncpy(szOldPrefix, szLine, sizeof(szOldPrefix));
                        szOldPrefix[sizeof(szOldPrefix) - 1] = '\0';

                        char* pNewPrefix = pDiv + 1;
                        pDiv = strchr(pNewPrefix, ',');
                        if(pDiv == NULL)
                        {
                            //只有一个逗号分隔，说明只有旧前缀和新前缀，获得新前缀
                            strncpy(szNewPrefix, pNewPrefix, sizeof(szNewPrefix));
                            szNewPrefix[sizeof(szNewPrefix) - 1] = '\0';
                        }
                        else
                        {
                            //两个逗号分隔，说明旧前缀\新前缀\是否加区号都有，
                            //获得是否加区号标示
                            *pDiv = '\0';
                            strncpy(szNewPrefix, pNewPrefix, sizeof(szNewPrefix));
                            szNewPrefix[sizeof(szNewPrefix) - 1] = '\0';

                            char* pbAddAreaCode = pDiv + 1;
                            strncpy(szbAddAreaCode, pbAddAreaCode, sizeof(szbAddAreaCode));
                            szbAddAreaCode[sizeof(szbAddAreaCode) - 1] = '\0';
                        }
                    }

                    //把旧前缀作为关键字加入Map
                    StringLRTrim(szOldPrefix);
                    StringLRTrim(szNewPrefix);
                    StringLRTrim(szbAddAreaCode);

                    SPrefixCvtInfo stInfo;
                    stInfo.strNewPrefix = szNewPrefix;
                    if(szbAddAreaCode[0] == '\0')
                    {
                        stInfo.bPadAreaCode = bDefPadAreaCode;
                    }
                    else
                    {
                        stInfo.bPadAreaCode = atoi(szbAddAreaCode);
                    }

                    mapPrefix[szOldPrefix] = stInfo;
                }
            }
            fclose(f);
        }

        //读取并保存号首集和区号的对照表
        if(nSrcFields > 1)
        {
		    f = fopen("./config/area/area.map", "rt");
            if(f != NULL)
            {
                char szLine[1024];
                while(!feof(f))
                {
                    char szAreaCode[128];
                    UINT4 nDNSet;
                    char* p = fgets(szLine, sizeof(szLine), f);
                    if(p != NULL)
                    {
                        int nRead = sscanf(szLine, "%d%s", &nDNSet, szAreaCode);
                        if(nRead >= 2)
                        {
                            szAreaCode[sizeof(szAreaCode) - 1] = '\0';
                            mapDNSet2AreaCode[nDNSet] = szAreaCode;
                        }
                    }
                }
                fclose(f);
            }
        }
        
        //防止mapPrefix为空的时候多次试图打开文件
		bIsFirstRun = FALSE; 
    }
    mutex.release();


    //将电话号码从BCD码转换为文本
    char szNumber[128] = "";
    BCDtoString(pNumber, szNumber, nSrcNumLen);
    char* pF = strchr((char*)szNumber, 'F');
    if(pF != NULL)
    {
        *pF = '\0';
    }


    /*
     * 根据前缀表中的旧前缀对电话号码进行匹配，按最大化匹配原则；
     * 因为Map中的数据都按键值进行了排序，因此从后往前查，就符合最大化原则。
     */

    mutex.acquire();
    PREFIX_MAP::reverse_iterator rit= mapPrefix.rbegin();
    while(rit != mapPrefix.rend())
    {
        STRING sOldPrefix = rit->first;
        if(strncmp(sOldPrefix.c_str(), szNumber, sOldPrefix.length()) == 0)
        {
            break;
        }            
        rit++;
    }


    char szNewNumber[256] = "";

    //如果找不到对应的前缀
    if(rit == mapPrefix.rend())
    {
        if((nSrcFields > 1) && bDefPadAreaCode)
        {
            //根据号首集查找区号
            MAP<UINT4, STRING>::iterator itDNSet = 
                                mapDNSet2AreaCode.find(nDNSet);
	        if ( itDNSet != mapDNSet2AreaCode.end())
            {
                strncpy(szNewNumber, itDNSet->second.c_str(), sizeof(szNewNumber));
                szNewNumber[sizeof(szNewNumber) - 1] = '\0';
            }
        }
        strcat(szNewNumber, szNumber);
    }
    else
    {
        //先加区号
        if((nSrcFields > 1) && rit->second.bPadAreaCode)
        {
            //根据号首集查找区号
            MAP<UINT4, STRING>::iterator itDNSet = 
                                mapDNSet2AreaCode.find(nDNSet);
	        if ( itDNSet != mapDNSet2AreaCode.end())
            {
                strncpy(szNewNumber, itDNSet->second.c_str(), sizeof(szNewNumber));
                szNewNumber[sizeof(szNewNumber) - 1] = '\0';
            }
        }

        //再替换前缀
        strcat(szNewNumber, rit->second.strNewPrefix.c_str());
        strcat(szNewNumber, &szNumber[rit->first.length()]);
    }

    if(pFieldInfo[0].nDataType == FT_STRING)
    {
        strcpy((char*)pCurField, szNewNumber);
        if (nIsQuery == 0)
        {
            AddFillerSeparator((char*)pCurField, pFieldInfo);        
        }
    }
    else
    {
        //将替换前缀后的电话号码转换为BCD码直接写到结果
        StringtoBCD(szNewNumber, (BYTE*)pCurField, pFieldInfo->nFieldLength);
    }

    mutex.release();

    return 0;
}

/************ End of The ReplacePrefix Defining ******************/

/************ The ReplaceNumByStr Defining ***************************/
// 功能描述：将某个字段从数字替换成字符串,数字和字符串对应关系在文件中配置
//           原始字段的值		替换后的值
//           1        			abc
//           2      			def
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1或者2，
// 
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
// 返回值：成功返回0，失败返回-1
// 
// Added By Lucy 2004-06-25
/***************************************************************/
typedef MAP<int, STRING>         NUMSTRMAP;
typedef MAP<STRING, NUMSTRMAP>   NUMSTRFILE;

//加载配置文件
BOOL LoadNumStr(NUMSTRMAP& NumStrMap, const char* szFileName)
{
    STRING sFileName = "./config/area/" + STRING(szFileName);
    //文件是否存在，不存在则创建
    FILE* pFile = fopen(sFileName.c_str(), "r");
    if( pFile == NULL)
    {
        CreateDeepDir("./config/area");
        pFile = fopen(sFileName.c_str(), "w");
        if(pFile == NULL)
        {
            return FALSE;
        }
    }
    else
    //读取配置信息
    {
        char szLine[1024];
        int  nNum;        //数字
        char szStr[1024]; //字符串
        while(!feof(pFile))
        {           
            char* p = fgets(szLine, sizeof(szLine), pFile);
            if((p != NULL) && (szLine[0] != '\0'))
            {
                sscanf(szLine,"%d %s", &nNum, szStr);
                //插入MAP中
                NumStrMap[nNum] = STRING(szStr);
            }
        }
    }
    fclose(pFile);
    
    return TRUE;
}

int CConvertFun::ReplaceNumByStr(void* pCurField, const UINT4 nSrcFields,
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFieldInfo,
                                 const void* pDefault, const UINT4 nDefaultLen,
                                 int nIsQuery)
{
    INT4 uSrcNum;

    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
            FUN_REPLACE_NUM_BY_STR, nSrcFields);
        return -1;
    }    
    if((pCurField == NULL) || (pSrcFields == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)||(pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0))
    {
        return -1;
    }
    
    //获取原格式域的整型值
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        uSrcNum = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        uSrcNum = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        uSrcNum = uTmp;
    }    
    else if(nSrcFieldLen[0] == LEN_INT8)
    {
        TRACE(MTS_FORMAT, S_CFG_SET_ITEMVALUE_TOOLONG,
            FUN_REPLACE_NUM_BY_STR, nSrcFieldLen[0]);
        return -1;
    }    
    else
    {
        return -1;
    }

    //读取配置中的段名称SegName
    const char* szFileName = (const char*)pSrcFields[1];

    static ACE_Mutex mutex;
    mutex.acquire();

    static NUMSTRFILE  SegFile;
    NUMSTRFILE::iterator iterFile = SegFile.find(szFileName);

    //没有加载过配置数据，从文件中加载
    if(iterFile == SegFile.end())
    {
        NUMSTRMAP NumStrMap;
        if(!LoadNumStr(NumStrMap, szFileName))
        {
            mutex.release();
            return -1;
        }

        SegFile[szFileName] = NumStrMap;        
    }

    //取得对应段的列表
    NUMSTRMAP& NumStrMap = SegFile[szFileName];

    //找到对应的数字 和 替代字符串
    NUMSTRMAP::iterator iterMap = NumStrMap.find(uSrcNum);
    if(iterMap != NumStrMap.end())
    {
        strncpy((char*)pCurField, (NumStrMap[uSrcNum]).c_str(), (NumStrMap[uSrcNum]).length() + 1); 
    }
    else
    {
        sprintf((char*)pCurField, "%u", uSrcNum);
    }
    
    mutex.release();

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFieldInfo);
    }
    
    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;    
}
//end


// 功能描述：将原始话单中的“通话时长”字段转换成HHmmSS格式，例如，通话时长为128秒，则转换后的格式
//           为000208。
// 输入参数：
//           pCurField      : 转换后的存放HHmmSS格式的字符串指针
//           nSrcFields     : 在本函数中应该等于1
//           pSrcFields[0]  : 通话时长(UINT4)
//           nSrcFieldLen[0]: 通话时长字段的长度
int CConvertFun::Duration2HHmmSS(void* pCurField,
                                 const UINT4 nSrcFields,
                                 const void* pSrcFields[],
                                 const UINT4 nSrcFieldLen[],
                                 const SFieldInfo* pFI,
                                 const void* pDefault,
                                 const UINT4 nDefaultLen,
                                 int nIsQuery)
{
    if ((pCurField == NULL) || (nSrcFieldLen[0] <= 0) || (pSrcFields == NULL))
    {
        return -1;
    }

    //获取输入参数中的通话时长
    UINT4 nDuration;
    memcpy(&nDuration, pSrcFields[0], LEN_UINT4);
    //nDuration = ITOHL(nDuration);

    UINT4 nHours    = 0;
    UINT4 nMinutes  = 0;
    UINT4 nSeconds  = 0;

    nHours    = nDuration/3600;
    nMinutes  = (nDuration - (nHours * 3600))/60;
    nSeconds  = (nDuration % 60);

    sprintf((char*)pCurField, "%02d%02d%02d", nHours, nMinutes, nSeconds);
    if (nIsQuery == 0)
    {
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    return 0;
}

/***************  The Define of  Convert10ms   ************************/
// 功能：将10ms为单位的时间转换成S, 或HHMMSS，转换成HHMMSS时需要带上格式
//
// 例如：12800, "HH:MM:SS" -> 02:08:00
//
// 输入参数：
//           pCurField      : 转换后的存放HHmmSS格式的字符串指针
//           nSrcFields     : 在本函数中应该等于1
//           pSrcFields[0]  : 通话时长(UINT4)
//           nSrcFieldLen[0]: 通话时长字段的长度
// 输出： -1：失败, 0: 成功
//
// Added By Lucy 2004-07-16 
/**********************************************************************/
int CConvertFun::Convert10ms(void* pCurField, 
                       const UINT4 nSrcFields,
                       const void* pSrcFields[],
                       const UINT4 nSrcFieldLen[],
                       const SFieldInfo* pFI,
                       const void* pDefault, 
                       const UINT4 nDefaultLen,
                       int nIsQuery)
{
    //验证输入参数正确
    if((pCurField == NULL) || ((nSrcFields != 1) && (nSrcFields != 2))|| 
       (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_CONVERT10MS, nSrcFields);
        
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    //end of 验证
    
    //获取输入参数: 时长
    UINT4 nDuration = 0;
    
    //获取原格式域的整型值
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        nDuration = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        nDuration = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nDuration = uTmp;
    }    
    else 
    {
        return -1;
    }
    
    //将10ms单位转换成s为单位,有余则进1
    nDuration = (nDuration + 99) / 100;
    
    //按目标类型(二进制/文本)转换成秒
    if(1 == nSrcFields)
    {
        if((FT_CHAR == pFI->nDataType) || (FT_STRING == pFI->nDataType))
        {
            sprintf((char*)pCurField, "%u", nDuration);
        }
        else
        {
            switch(pFI->nDataType) 
            {
            case FT_BYTE:
                {                
                    BYTE uTmp = nDuration;
                    memcpy(pCurField, &uTmp, LEN_BYTE);
            	    break;
                }
            case FT_SHORT:
            case FT_USHORT:
                {
                    UINT2 uTmp = nDuration;
                    memcpy(pCurField, &uTmp, LEN_INT2);
            	    break;
                }
            case FT_LONG:
            case FT_ULONG:   
                {
                    UINT4 uTmp = nDuration;
                    memcpy(pCurField, &uTmp, LEN_INT4);
                    break;
                }
            case FT_INT8:
                {
                    INT8 uTmp = nDuration;
                    memcpy(pCurField, &uTmp, LEN_INT8);
                    break;
                }
            default:
                return -1;
            }
        }
    }
    //按格式转换成HHMMSS
    else
    {               
        UINT4 nHours    = nDuration/3600;
        UINT4 nMinutes  = (nDuration - (nHours * 3600))/60;
        UINT4 nSeconds  = (nDuration % 60);

        char szTmp[32] = "";
        char szSep1[8] = "";
        char szSep2[8] = "";
        
        memcpy(szTmp, pSrcFields[1], nSrcFieldLen[1]);
        //存在分隔符HH:MM:SS
        if(strlen(szTmp) > 6)
        {
            sscanf(szTmp, "HH%cMM%cSS", szSep1, szSep2);
        }

        STRING sFormat = "%02d" + STRING(szSep1) + "%02d" + STRING(szSep2) + "%02d";

        sprintf((char*)pCurField, sFormat.c_str(), nHours, nMinutes, nSeconds);
    }

    if (nIsQuery == 0)
    {
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;    
}


//本函数是Int2Text(...)函数的增强版，如果传入的数字全为0xFF，则转换成空字符串
int CConvertFun::Int2TextEx(void* pCurField,
                            const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault,
                            const UINT4 nDefaultLen,
                            int nIsQuery)
{
    INT4  nTmp = 0;    //存放BYTE、SHORT、LONG的临时变量
    BOOL bIsAllF = TRUE;  //是否全为0xF

    //确保输入参数正确
    if((pCurField == NULL) || !(nSrcFields == 1 || nSrcFields == 2)  || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }

    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    for(int i = 0; i < nSrcFieldLen[0]; i++)
    {
        if(*((BYTE*)(pSrcFields[0]) + i) != 0xFF)
        {
            bIsAllF = FALSE;
            break;
        }
    }

    if(bIsAllF)
    {
        *(char*)pCurField = '\0';
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    }
    else
    {
        return Int2Text(pCurField, nSrcFields, pSrcFields, nSrcFieldLen,
                        pFI, pDefault, nDefaultLen, nIsQuery);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}

int CConvertFun::TrkGrp2CallNo(void* pCurField,
                               const UINT4 nSrcFields, 
                               const void* pSrcFields[],
                               const UINT4 nSrcFieldLen[],
                               const SFieldInfo* pFI,
                               const void* pDefault,
                               const UINT4 nDefaultLen,
                               int nIsQuery)
{
    static MAP<UINT2, STRING> mapTrunkGroup2CallNo;
    static BOOL bIsFirstRun = TRUE;
    
    //确保输入参数正确
    if((pCurField == NULL) || (nSrcFields != 1) || (pSrcFields == NULL))
    {
        return -1;
    }
    
    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }    
    
    static ACE_Mutex mutex;
    
    mutex.acquire();
    if(bIsFirstRun)
    {
        //读取并保存号首集和区号的对照表
        FILE* f = fopen("./config/distinction/trk2callno.map", "rt");
        if(f != NULL)
        {
            char szLine[1024];
            char szCallNo[40];
            UINT2 uTrunkGroup = 0;
            while(!feof(f))
            {
                if( fgets(szLine, sizeof(szLine), f) != NULL)
                {
                    StringLRTrim(szLine);
                    int nRead = sscanf(szLine, "%d%s", &uTrunkGroup, szCallNo);
                    if(nRead == 2)
                    {
                        mapTrunkGroup2CallNo[uTrunkGroup] = szCallNo;
                    }
                }
            }
            fclose(f);
        }
        
        bIsFirstRun = FALSE; //防止多次试图打开文件
    }  

    if(nSrcFieldLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        MAP<UINT2, STRING>::iterator pMap = mapTrunkGroup2CallNo.find(uTmp);
        if (pMap != mapTrunkGroup2CallNo.end())
        {
            strncpy((char*)pCurField,
                     mapTrunkGroup2CallNo[uTmp].c_str(),
                     pFI->nFieldLength - 1);
            memset((char*)pCurField + pFI->nFieldLength - 1, '\0', 1);
        }
        else
        {
            sprintf((char*)pCurField,"%d\0",uTmp);
        }
    }
    else
    {
        memset(pCurField, 0, 1);
    }
    mutex.release();

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    return 0;
}

//Added by ZhengYuqun SWPD03631 2004-02-18
//MSoftX3000的话单类型转换为2G的话单类型时需要根据话单类型和中间话单原因值生成试呼话单类型
int CConvertFun::CDRType2G(void* pCurField,
                            const UINT4 nSrcFields,
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault,
                            const UINT4 nDefaultLen,
                            int nIsQuery)
{
    INT4  nTmp = 0;    //存放BYTE、SHORT、LONG的临时变量
    BOOL bIsAllF = TRUE;  //是否全为0xF
    int i;

    //确保输入参数正确
    if((pCurField == NULL) || (nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }

    for(i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }

    if(nSrcFieldLen[0] != LEN_BYTE || nSrcFieldLen[1] != LEN_BYTE)
    {
        return -1;
    }

    BYTE byCDRType = *(BYTE*)pSrcFields[0];
    BYTE byCause = *(BYTE*)pSrcFields[1];

    //中间话单原因值为3(失败)并且话单类型为2(入关口局/漫游)、4(汇接)、10(出关口局)时将话单类型修改为9(试呼)
    if(byCause == 3 && (byCDRType == 2 || byCDRType == 4 || byCDRType == 10))
    {
        byCDRType = 9;
    }

    memcpy(pCurField, &byCDRType, LEN_BYTE);

    return 0;
}


/****************** The IsInNumSeg Defining *************************/
// 功能描述：判断一个号码是否在指定的号段中
//
// 输入参数：
//  nParasNum：参数个数，此函数的参数个数应该为2
//     pParas：参数数组，此函数包含两个参数，
//             pParas[0]表示TBCD编码的电话号码
//             pParas[1]表示号段类的名称
//  nParasLen：各参数长度数组
//   pDefault：函数缺省值
//
// 输出参数：无
//
//   返回值：计算成功，则返回SVariant*类型的结果，如果出错返回NULL
//
/****************************************************************/
typedef struct _SNUMSEG
{
    UINT2  nNumberCount;        //pNumSeg有多少个号码是有效的
    BYTE   pNumSeg[20];         //号段的存放区域
} SNUMSEG;

#define   NUM_SEG_SINGLE    0
#define   NUM_SEG_SCOPE     1
typedef struct _SNUMSEGSCOPE
{
    BYTE      byScopeTag;          //标示是单个号段还是一个号段范围
    SNUMSEG   stMinSeg;            //号段范围中最小的段，单个号段时就是此号段
    SNUMSEG   stMaxSeg;            //号段范围中最大的段
} SNUMSEGSCOPE;


typedef LIST<SNUMSEGSCOPE>          NUMSEGTABLE;
typedef MAP<STRING, NUMSEGTABLE>    NUMSEGFILE;

#define NUM_SEG_COUNT               "NumSegCount"
#define NUM_SEG_COUNT_DEFAULT       0

#define NUM_SEG_KEY                 "NumSeg"
#define NUM_SEG_VALUE_DEFAULT       ""
//加载号段表
BOOL LoadNumSegTabel(NUMSEGTABLE& SegTable, const char* szNumSegName)
{
    //测试文件是否存在，不存在则创建
    FILE* pFile = fopen("./config/distination/number_seg.ini", "r");
    if( pFile == NULL)
    {
        CreateDeepDir("./config/distination");
        pFile = fopen("./config/distination/number_seg.ini", "w");
        if(pFile == NULL)
        {
            return FALSE;
        }
    }
    fclose(pFile);
    pFile = NULL;

    char szNumSegKey[15] = "";
    char szNumSeg[40]    = "";
    BYTE pBCD[64]        = "";
    BYTE pRBCD[64]       = "";

    //读入配置的号段
    CINIFile iniFile("./config/distination/number_seg.ini");
    iniFile.Open();
    UINT4 nNumSegCnt = iniFile.GetInt(szNumSegName,
                                      NUM_SEG_COUNT,
                                      NUM_SEG_COUNT_DEFAULT);
    for(int i = 1; i <= nNumSegCnt; i++)
    {
        //读入一个号段
        SNPRINTF(szNumSegKey,
                 sizeof(szNumSegKey),
                 "%s%d",
                 NUM_SEG_KEY,
                 i);
        szNumSegKey[sizeof(szNumSegKey) - 1] = '\0';

        iniFile.GetString(szNumSegName,
                          szNumSegKey,
                          NUM_SEG_VALUE_DEFAULT,
                          szNumSeg,
                          sizeof(szNumSeg));
        szNumSeg[sizeof(szNumSeg) - 1] = '\0';

        //没有配置的话，继续找下一个
        StringLRTrim(szNumSeg);
        if(szNumSeg[0] == '\0')
        {
            continue;
        }

        SNUMSEGSCOPE stNumSegScope;
        char* pDiv = strchr(szNumSeg, '-');

        if (pDiv == NULL)
        {
            //单个号段的情况
            stNumSegScope.byScopeTag = NUM_SEG_SINGLE;
            int nByteLen = StringtoBCD(szNumSeg, pBCD);
            int nRBCDLen = BCDtoRBCD(stNumSegScope.stMinSeg.pNumSeg, pBCD, nByteLen);
            if(nRBCDLen == 0)
            {
                continue;
            }
            if(pRBCD[nRBCDLen - 1] & 0x0F == 0x0F)
            {
                stNumSegScope.stMinSeg.nNumberCount = 2 * nRBCDLen - 1;
            }
            else
            {
                stNumSegScope.stMinSeg.nNumberCount = 2 * nRBCDLen;
            }
        }
        else
        {
            //号段范围的情况
            stNumSegScope.byScopeTag = NUM_SEG_SCOPE;

            pDiv[0] = '\0';
            char* szMinSeg = szNumSeg;
            char* szMaxSeg = pDiv + 1;
            StringLRTrim(szMinSeg);
            StringLRTrim(szMaxSeg);

            //转换最小号段为反向BCD码
            int nByteLen = StringtoBCD(szMinSeg, pBCD);
            int nRBCDLen = BCDtoRBCD(stNumSegScope.stMinSeg.pNumSeg, pBCD, nByteLen);
            if(nRBCDLen == 0)
            {
                continue;
            }
            if(pRBCD[nRBCDLen - 1] & 0x0F == 0x0F)
            {
                stNumSegScope.stMinSeg.nNumberCount = 2 * nRBCDLen - 1;
            }
            else
            {
                stNumSegScope.stMinSeg.nNumberCount = 2 * nRBCDLen;
            }

            //转换最大号段为反向BCD码
            nByteLen = StringtoBCD(szMaxSeg, pBCD);
            nRBCDLen = BCDtoRBCD(stNumSegScope.stMaxSeg.pNumSeg, pBCD, nByteLen);
            if(nRBCDLen == 0)
            {
                continue;
            }
            if(pRBCD[nRBCDLen - 1] & 0x0F == 0x0F)
            {
                stNumSegScope.stMaxSeg.nNumberCount = 2 * nRBCDLen - 1;
            }
            else
            {
                stNumSegScope.stMaxSeg.nNumberCount = 2 * nRBCDLen;
            }
        }

        //构建一种产品的号段列表
        SegTable.push_back(stNumSegScope);
    }

    return TRUE;
}

//比较指定位数的反向BCD码
int RBCDCmp(BYTE* pRBCD1, BYTE* pRBCD2, int nCmpCodeLen)
{
    BOOL bIsOdd = (nCmpCodeLen % 2 != 0);
    int  nByteLen = nCmpCodeLen / 2;
    int  i;

    for(i = 0; i < nByteLen; i++)
    {
        BYTE byBCD1 = pRBCD1[i] << 4 || pRBCD1[i] >> 4;
        BYTE byBCD2 = pRBCD2[i] << 4 || pRBCD2[i] >> 4;

        if(byBCD1 > byBCD2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    if(bIsOdd)
    {
        BYTE byBCD1 = pRBCD1[i] & 0xF;
        BYTE byBCD2 = pRBCD2[i] & 0xF;
        if(byBCD1 > byBCD2)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}

//判断一个号码是否在指定的号段中
int CConvertFun::IsInNumSeg(const UINT4 nParasNum,
                            const void* pParas[],
                            const UINT4 nParasLen[],
                            const STRING& pDefault,
                            SVariant& stVar)
{
    if((nParasNum != 2) || (pParas == NULL))
    {
        return -1;
    }

    //确保参数正确
    for(int i = 0; i < nParasNum; i++)
    {
        if((pParas[i] == NULL) || nParasLen[i] == 0)
        {
            return -1;
        }
    }

    const char* szNumSegName = (const char*)pParas[1];
    static ACE_Mutex   mutex;
    mutex.acquire();

    static NUMSEGFILE  SegFile;
    NUMSEGFILE::iterator iterFile = SegFile.find(szNumSegName);

    //没有加载过配置数据，从文件中加载
    if(iterFile == SegFile.end())
    {
        NUMSEGTABLE SegTable;
        if(!LoadNumSegTabel(SegTable, szNumSegName))
        {
            mutex.release();
            return -1;
        }

        SegFile[szNumSegName] = SegTable;
    }

    /*
     *  以下为判断号码是否在号段表内。
     */
    int nRet = 0;
    NUMSEGTABLE& SegTable = SegFile[szNumSegName];
    BYTE* pByNumber = (BYTE*)pParas[0];

    NUMSEGTABLE::iterator iterTab = SegTable.begin();
    while(iterTab != SegTable.end())
    {
        SNUMSEGSCOPE& NumScope = (*iterTab);

        //判断是否符合单个号段
        if(NumScope.byScopeTag == NUM_SEG_SINGLE)
        {
            if(RBCDCmp(pByNumber, NumScope.stMinSeg.pNumSeg,
                       NumScope.stMinSeg.nNumberCount) == 0)
            {
                nRet = 1;
                break;
            }
        }
        else
        {
            if(( RBCDCmp(pByNumber, NumScope.stMinSeg.pNumSeg, NumScope.stMinSeg.nNumberCount) >= 0)
              &&(RBCDCmp(pByNumber, NumScope.stMaxSeg.pNumSeg, NumScope.stMaxSeg.nNumberCount) <= 0))
            {
                nRet = 1;
                break;
            }
        }
    }
    mutex.release();

    stVar.yVariantType = FT_ULONG;
    stVar.u4Value = nRet;

    return 0;

}

//判断一个号码是否在指定的号段中
int CConvertFun::IsInEnum(const UINT4 nParasNum,
                          const void* pParas[],
                          const UINT4 nParasLen[],
                          const STRING& pDefault,
                          SVariant& stVar)
{
    if((nParasNum != 2) || (pParas == NULL))
    {
        return -1;
    }

    //确保参数正确
    for(int i = 0; i < nParasNum; i++)
    {
        if((pParas[i] == NULL) || nParasLen[i] == 0)
        {
            return -1;
        }
    }

    //获取原格式域的整型值
    UINT4 uSrcNum = 0;
    if(nParasLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pParas[0], LEN_BYTE);
        uSrcNum = uTmp;
    }
    else if(nParasLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pParas[0], LEN_UINT2);
        uSrcNum = uTmp;
    }
    else if(nParasLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pParas[0], LEN_UINT4);
        uSrcNum = uTmp;
    }    
    else
    {
        return -1;
    }
    
    const char* szFileName = (const char*)pParas[1];
    static ACE_Mutex   mutex;
    mutex.acquire();

    static MAP<STRING, MAP<UINT4, UINT4> >  EnumFile;
    MAP<STRING, MAP<UINT4, UINT4> >::iterator iterFile = EnumFile.find(szFileName);

    //没有加载过配置数据，从文件中加载
    if(iterFile == EnumFile.end())
    {
        MAP<UINT4, UINT4> EnumTable;
        char  szTemp[100];
        UINT4 uOrgNum;
        UINT4 uDstNum;

        STRING strFileName = "./config/distinction/";
        strFileName += szFileName;
        FILE* f = fopen(strFileName.c_str(), "rt");
        if(f != NULL)
        {
            char szLine[1024];
            while(!feof(f))
            {
                char* p = fgets(szLine, sizeof(szLine), f);
                if(p != NULL)
                {
                    int nRead = sscanf(szLine, "%d%d%s", &uOrgNum, &uDstNum, szTemp);
                    if(nRead >= 2)
                    {
                        EnumTable[uOrgNum] = uDstNum;
                    }
                }
            }
            fclose(f);
        }

        if(EnumTable.size() > 0)
        {
            EnumFile[szFileName] = EnumTable;
        }
    }

    /*
     *  以下为判断号码是否在号段表内。
     */
    UINT4 uRet = 0;
    MAP<UINT4, UINT4>& EnumTable = EnumFile[szFileName];
    if (EnumTable.find(uSrcNum) != EnumTable.end())
    {
        uRet = EnumTable[uSrcNum];
    }

    mutex.release();

    stVar.yVariantType = FT_ULONG;
    stVar.u4Value = uRet;

    return 0;

}

/*
  函数功能：将长度不足的号码填补指定的字冠。
  输入参数：
          pSrcFields[0]/nSrcFieldLen[0] : 待处理的号码（BCD码）
          pSrcFields[1]/nSrcFieldLen[1] : 号码长度要求
          pSrcFields[2]/nSrcFieldLen[2] : 需要添加的字冠
      
*/
int CConvertFun::PadNumber(void* pCurField, 
                           const UINT4 nSrcFields, 
                           const void* pSrcFields[],
                           const UINT4 nSrcFieldLen[],
                           const SFieldInfo* pFI,
                           const void* pDefault, 
                           const UINT4 nDefaultLen,
                           int nIsQuery)
{
    //确保输入参数正确
    if ((pCurField == NULL) || (nSrcFields != 3) || (pSrcFields == NULL))
    {
        return -1;
    }

    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0)
     || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0)
     || (pSrcFields[2] == NULL) || (nSrcFieldLen[2] == 0))
    {
        return -1;
    }

    if ((pFI->nDataType != FT_BCD) && (pFI->nDataType != FT_STRING))
    {
        return -1;
    }
	
	char szNumber[32];
	char szLen[16];
	
	strncpy(szLen, (char *)pSrcFields[1], 16);
	szLen[nSrcFieldLen[1]] = '\0';
	
	int  nLen = atoi(szLen);
	
	strncpy(szNumber, (char *)pSrcFields[2], nSrcFieldLen[2]);
	szNumber[nSrcFieldLen[2]] = '\0';
	
	char szTemp[32];
    BCDtoString((BYTE*)pSrcFields[0], szTemp, nSrcFieldLen[0]);
    char* pF = strchr(szTemp, 'F');
    if (pF != NULL)
    {
        *pF = '\0';
    }
    //改为长度为nLen才在前面加相应的字符 20004-06-02 by zkq
    if ((strlen(szTemp) == nLen) && (strlen(szTemp) != 0))
    {
    	strncat(szNumber, szTemp, 32 - strlen(szNumber));
    } 
    else
    {
    	strcpy(szNumber, szTemp); 
    }
    //end 
    char* pszCurField = (char *)pCurField;
   
    if (pFI->nDataType == FT_STRING)
    {
        if (pFI->nFieldLength < strlen(szNumber))
        {
            strncpy(pszCurField, szNumber, pFI->nFieldLength);                
            pszCurField[pFI->nFieldLength - 1] = '\0';
        }
        else
        {
            strcpy(pszCurField, szNumber);
        }           
 
        if (nIsQuery == 0)
        {
            return AddFillerSeparator(pszCurField, pFI);        
        }
    }
    else if (strlen(szTemp) != 0)
    {
        StringtoBCD(szNumber, (BYTE*)pCurField, nSrcFieldLen[0]); 
    }
    else
    {
    	memset(pCurField, 0xFF, pFI->nFieldLength);
    }      	
    
    return 0;
}

/*
  函数功能：将软交换移动的SMS话单中的主被叫号码按号码类型转换为文本
            如果号码类型为5，则是文本类型，否则为BCD码类型
  输入参数：
          pSrcFields[0]/nSrcFieldLen[0] : 待处理的号码
          pSrcFields[1]/nSrcFieldLen[1] : 号码类型字段
          pSrcFields[2]/nSrcFieldLen[2] : 号码长度字段

  Added by ZhengYuqun      
*/
int CConvertFun::SMSNum2Text(void* pCurField, const UINT4 nSrcFields, 
                                    const void* pSrcFields[],
                                    const UINT4 nSrcFieldLen[],
                                    const SFieldInfo* pFI,
                                    const void* pDefault, const UINT4 nDefaultLen,
                                    int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针
    

    if(nSrcFields != 3)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_SMS_NUM_TO_TEXT, 1);
        return -1;
    }
    
    //确保输入参数正确
    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    BYTE byNAI = *(BYTE*)pSrcFields[1];
    if(byNAI == 5) //号码类型为5的时候就是ASCII码
    {
        pCurStr = (char*)pCurField;
        BYTE byNumLen = *(BYTE*)pSrcFields[2];
        memcpy(pCurStr, (BYTE*)pSrcFields[0], byNumLen / 2); //与主机的约定就是要长度除以2
        pCurStr[byNumLen / 2] = '\0';
    }
    else
    {
        //初始化BCD码数组指针和文本格式的BCD码字符串指针
        pCurStr = (char*)pCurField;
        pBcdArray = (BYTE*)pSrcFields[0];
    
        //顺序扫描BCD码数组的每一个元素
        while(nBcdIndex < nSrcFieldLen[0])
        {
            //获取当前BCD码的值
            yCurBcd = pBcdArray[nBcdIndex++];
        
            //如果高四位是正常的BCD码值，即小于16
            if(yCurBcd < 0xF0)
            {
                //如果当前BCD码值小于10
                if(yCurBcd < 0xA0)
                {
                    pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
                }
            
                //如果当前BCD码值大于10
                else
                {
                    pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
                }
            }
        
            //到了BCD码数组结束位置
            else
            {
                break;
            }
        
            //屏蔽当前BCD码数组值的高四位
            yCurBcd &= 0x0F;
        
            //如果低四位是正常的BCD码值，即小于16
            if(yCurBcd < 0x0F)
            {
                //如果当前BCD码值小于10
                if(yCurBcd < 0x0A)
                {
                    pCurStr[nIndex++] = yCurBcd + '0';
                }
            
                //如果当前BCD码值大于10
                else
                {
                    pCurStr[nIndex++] = yCurBcd - 10 + 'A';
                }
            }
        
            //到了BCD码数组结束位置
            else
            {
                break;
            }
        }
    
        //设置字符串的结束符
        pCurStr[nIndex] = '\0';
    }
    
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}

/*
  函数功能：如果是短消息话单并且diagnostics的类型为gsm0902MapError就直接填normal release的值
            否则直接拷贝diagnostics的值
  输入参数：
          pSrcFields[0]/nSrcFieldLen[0] : 话单类型
          pSrcFields[1]/nSrcFieldLen[1] : Diagnostics

  Added by ZhengYuqun SWPD05305 2004-05-19     
*/
int CConvertFun::TermCause2G(void* pCurField, const UINT4 nSrcFields, 
                       const void* pSrcFields[],
                       const UINT4 nSrcFieldLen[],
                       const SFieldInfo* pFI,
                       const void* pDefault, const UINT4 nDefaultLen,
                       int nIsQuery)
{
/*
SDiagnostics的结构如下：
    typedef struct
    {
        BYTE   CauseType;
        union
        {
            UINT4     gsm0408Cause;                // CauseType == 0
            UINT4     gsm0902MapErrorValue;        // CauseType == 1
            UINT4     ccittQ767Cause;              // CauseType == 2
        } Cause;
    } SDiagnostics;
*/
#define CDRTYPE_MT_SMS	6
#define CDRTYPE_MO_SMS	7
#define NORMAL_CALL_RELEASE	144

    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_TERM_CAUSE_2G, 1);
        return -1;
    }    
    
    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    
    if(nSrcFieldLen[0] != LEN_BYTE || nSrcFieldLen[1] != 5)
    {
        return -1;
    }
    
    BYTE byTermCause = NORMAL_CALL_RELEASE;  //最终的话终原因值

    BYTE byCDRType = *(BYTE*)pSrcFields[0];
    BYTE byCauseType = *(BYTE*)pSrcFields[1];
    UINT4 uCause = 0;

    memcpy(&uCause, (BYTE*)pSrcFields[1] + 1, LEN_UINT4);
    //认定主机发送话单的字节序为Intel字节序
    //uCause = ITOHL(uCause);
    
    //Diagnostics的类型为gsm0902MapError并且话单类型为短消息则直接填NORMAL_CALL_RELEASE
    if(byCauseType == 1 && (byCDRType == CDRTYPE_MT_SMS || byCDRType == CDRTYPE_MO_SMS))
    {
        byTermCause = NORMAL_CALL_RELEASE;
    }
    else
    {
        byTermCause = uCause;
    }
    
    memcpy(pCurField, &byTermCause, LEN_BYTE);
    
    return 0;
}

/*
  函数功能：将软交换移动的HO话单中的PeerMnc填为原始话单中last mccmnc的高字节并且反向，
            其余话单的PeerMnc填0xFF
  输入参数：
          pSrcFields[0]/nSrcFieldLen[0] : 话单类型
          pSrcFields[1]/nSrcFieldLen[1] : last mccmnc

  Added by ZhengYuqun SWPD05306 2004-05-19     
*/
int CConvertFun::PeerMnc2G(void* pCurField, const UINT4 nSrcFields, 
                     const void* pSrcFields[],
                     const UINT4 nSrcFieldLen[],
                     const SFieldInfo* pFI,
                     const void* pDefault, const UINT4 nDefaultLen,
                     int nIsQuery)
{
#define CDRTYPE_MO_HO	20
#define CDRTYPE_MT_HO	21
    
    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
              FUN_PEER_MNC_2G, 1);
        return -1;
    }    
    
    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    
    if(nSrcFieldLen[0] != LEN_BYTE && nSrcFieldLen[1] != 3)
    {
        return -1;
    }
    
    BYTE byCDRType = *(BYTE*)pSrcFields[0];
    BYTE byPeerMNC = *((BYTE*)pSrcFields[1] + 2);  //，mnc取最高的一个字节
    byPeerMNC = byPeerMNC >> 4 | byPeerMNC << 4; //而且还要反一下
    
    //如果话单类型不是2PLMN切换话单，那么PeerMNC就填0xFF
    if(byCDRType != CDRTYPE_MO_HO && byCDRType != CDRTYPE_MT_HO)
    {
        byPeerMNC = 0xFF;
    }
    
    memcpy(pCurField, &byPeerMNC, LEN_BYTE);
    
    return 0;
}

/*
  函数功能：将变长字段中某个字段的长度转为文本
  输入参数：
          pSrcFields[0]/nSrcFieldLen[0] : 变长字段
          pSrcFields[1]/nSrcFieldLen[1] : "m/n"，m表示第几个字段，n表示变长字段中共含几个字段

    Added by ZhengYuqun
*/
int CConvertFun::VarLen2Text(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[], 
                             const UINT4 nSrcFieldLen[], 
                             const SFieldInfo* pFI, 
                             const void* pDefault, const UINT4 nDefaultLen, 
                             int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_VAR_LEN_TO_TEXT, 2);
        return -1;
    }  

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (pSrcFields[1] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    int nLenIndOffset = 0;  //第几个存放长度指示的字节
    UINT4 nLenInd = 0;      //存放长度指示的字节的内容
    
    char szPara2[128];
    char* pPos;
    strncpy(szPara2, (char*)pSrcFields[1], 128);
    pPos = strchr(szPara2, '/');
    if(pPos != NULL)
    {
        *pPos = '\0';
        nLenIndOffset = atoi(szPara2); 
        nLenIndOffset--;
    }
    else
    {
        return -1;
    }
    nLenInd = ((BYTE*)pSrcFields[0])[nLenIndOffset];

    ACE_OS_String::itoa(nLenInd, (char *)pCurField, 10);
    
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}


/*
  函数功能：将变长字段中某个字段的内容按十六进制串转为文本
  输入参数：
          pSrcFields[0]/nSrcFieldLen[0] : 变长字段
          pSrcFields[1]/nSrcFieldLen[1] : "m/n"，m表示第几个字段，n表示变长字段中共含几个字段

    Added by ZhengYuqun
*/
int CConvertFun::VarValue2Text(void* pCurField, const UINT4 nSrcFields, 
                               const void* pSrcFields[], 
                               const UINT4 nSrcFieldLen[], 
                               const SFieldInfo* pFI, 
                               const void* pDefault, const UINT4 nDefaultLen, 
                               int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_VAR_VALUE_TO_TEXT, 2);
        return -1;
    }  

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (pSrcFields[1] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    UINT4 nLenIndOffset = 0;  //第几个存放长度指示的字节
    UINT4 nIndex = 0;         //要转换的字节串的起始偏移量

    //分别获得上面两个变量的值
    char szPara2[128];
    char* pPos;
    strncpy(szPara2, (char*)pSrcFields[1], 128);
    pPos = strchr(szPara2, '/');
    if(pPos != NULL)
    {
        *pPos = '\0';
        nLenIndOffset = atoi(szPara2); 
        nIndex = atoi(pPos + 1);
        nLenIndOffset--;
    }
    else
    {
        return -1;
    }

    for(int i = 0; i < nLenIndOffset; i++)
    {
        nIndex += ((BYTE*)pSrcFields[0])[i];
    }
        
    //取出正确的部分转换为文本
    UINT4 nTotalByteCount = nSrcFieldLen[0];
    UINT4 nCopyByteCount = ((BYTE*)pSrcFields[0])[nLenIndOffset];
    UINT4 nCount = 0;
    for(nCount = 0; nIndex < nTotalByteCount && nCount < nCopyByteCount; nIndex++, nCount++)
    {
        BYTE yCurNum = ((BYTE*)pSrcFields[0])[nIndex];
        if(nCount * 3 + 4 >= FMT_FIELD_MAX_LEN)
        {
            break;
        }
        sprintf (&((char *)pCurField)[nCount * 3], "%02x ", yCurNum );
    }
    ((char *)pCurField)[nCount*3] ='\0';
    
    
    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}

/*
  函数功能：得到变长字段中某个字段的二进制内容
  输入参数：
          pSrcFields[0]/nSrcFieldLen[0] : 变长字段
          pSrcFields[1]/nSrcFieldLen[1] : "m/n"，m表示第几个字段，n表示变长字段中共含几个字段

    Added by ZhengYuqun
*/
int CConvertFun::GetVarValue(void* pCurField, const UINT4 nSrcFields, 
                               const void* pSrcFields[], 
                               const UINT4 nSrcFieldLen[], 
                               const SFieldInfo* pFI, 
                               const void* pDefault, const UINT4 nDefaultLen, 
                               int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_VAR_VALUE, 2);
        return -1;
    }  

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (pSrcFields[1] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    UINT4 nLenIndOffset = 0;  //第几个存放长度指示的字节
    UINT4 nIndex = 0;         //要转换的字节串的起始偏移量

    //分别获得上面两个变量的值
    char szPara2[128];
    char* pPos;
    strncpy(szPara2, (char*)pSrcFields[1], 128);
    pPos = strchr(szPara2, '/');
    if(pPos != NULL)
    {
        *pPos = '\0';
        nLenIndOffset = atoi(szPara2); 
        nIndex = atoi(pPos + 1);
        nLenIndOffset--;
    }
    else
    {
        return -1;
    }

    for(int i = 0; i < nLenIndOffset; i++)
    {
        nIndex += ((BYTE*)pSrcFields[0])[i];
    }
        
    //取出正确的部分
    UINT4 nTotalByteCount = nSrcFieldLen[0];
    UINT4 nCopyByteCount = min(((BYTE*)pSrcFields[0])[nLenIndOffset], nTotalByteCount - nIndex);
    memset((BYTE *)pCurField, 0xFF, FMT_FIELD_MAX_LEN);
    if(nCopyByteCount > 0)
    {
        memcpy((BYTE *)pCurField, (BYTE*)pSrcFields[0] + nIndex, min(nCopyByteCount, FMT_FIELD_MAX_LEN));
    }
   
    return 0;
}

/*
  函数功能：获取变长字段中某个字段的长度
  输入参数：
          pSrcFields[0]/nSrcFieldLen[0] : 变长字段
          pSrcFields[1]/nSrcFieldLen[1] : "m/n"，m表示第几个字段，n表示变长字段中共含几个字段

    Added by ZhengYuqun
*/
int CConvertFun::GetVarLen(void* pCurField, const UINT4 nSrcFields, 
                             const void* pSrcFields[], 
                             const UINT4 nSrcFieldLen[], 
                             const SFieldInfo* pFI, 
                             const void* pDefault, const UINT4 nDefaultLen, 
                             int nIsQuery)
{
    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_GET_VAR_LEN, 2);
        return -1;
    } 

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (pSrcFields[1] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }
    
    int nLenIndOffset = 0;  //第几个存放长度指示的字节
    INT8 nLenInd = 0;      //存放长度指示的字节的内容
    
    char szPara2[128];
    char* pPos;
    strncpy(szPara2, (char*)pSrcFields[1], 128);
    pPos = strchr(szPara2, '/');
    if(pPos != NULL)
    {
        *pPos = '\0';
        nLenIndOffset = atoi(szPara2); 
        nLenIndOffset--;
    }
    else
    {
        return -1;
    }
    nLenInd = ((BYTE*)pSrcFields[0])[nLenIndOffset];
    
    //将INT8的中间变量再转换为需要的数据类型
    switch (pFI->nDataType)
    {
    case FT_BYTE:
        *(BYTE*)pCurField = nLenInd;
        break;
        
    case FT_SHORT:
    case FT_USHORT:
        *(UINT2*)pCurField = nLenInd;
        break;
        
    case FT_LONG:
    case FT_ULONG:
        *(UINT4*)pCurField = nLenInd;
        break;
        
    case FT_INT8:
        *(INT8*)pCurField = nLenInd;
        break;
        
    default:
        memcpy(pCurField, ((BYTE*)pSrcFields[0]) + nLenIndOffset, sizeof(BYTE));
        break;
    }
    
    return 0;
}

/************ The EBCD2String Defining **************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           1、将正向BCD码转换成字符串，
//           2、将正向BCD码中B,C转换成字符'*','#'，
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码等于0xE，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向正向BCD码数组， 
//           nSrcFieldLen[0]指示该正向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::EBCD2String(void* pCurField, const UINT4 nSrcFields, 
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针


    //确保输入参数正确
    if((pCurField == NULL) || (nSrcFields != 1 && nSrcFields != 2) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }

    //组装BCD码
    BYTE pNumber[40];
    int  nSrcNumLen = 0;
    if(nSrcFields == 1)
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];
    }
    else
    {
        memcpy(pNumber, pSrcFields[0], nSrcFieldLen[0]);
        nSrcNumLen = nSrcFieldLen[0];

        memcpy(pNumber + nSrcNumLen, pSrcFields[1], nSrcFieldLen[1]);
        nSrcNumLen += nSrcFieldLen[1];        
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pNumber;

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcNumLen)
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于15
        if((yCurBcd & 0xF0)< 0xE0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
				if((yCurBcd & 0xF0) == 0xB0)
                {
					pCurStr[nIndex++] = '*';
                }
				else if((yCurBcd & 0xF0) == 0xC0)
                {
					pCurStr[nIndex++] = '#';
                }
				else
				{
					pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
				}
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于15
        //在sofx3000中designated charging_num 显示以E为结束       
        if(yCurBcd < 0x0E)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
				if(yCurBcd == 0x0B)
                {
					pCurStr[nIndex++] = '*';
                }
				else if(yCurBcd == 0x0C)
                {
					pCurStr[nIndex++] = '#';
                }
                else
				{
					pCurStr[nIndex++] = yCurBcd - 10 + 'A';
				}
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换        
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The EBCD2String Defining *******************/

/************ The BCDTime2Sec Defining **************************/
// 功能描述：本函数就是由CConvert类提供的CONVERTFUN类型的工具函数，
//           将正向BCD码转换成字符串，并根据时分秒的BCD码转换成以秒显示的字符串
//           转换时在两种情况下认为BCD数组结束：
//           1、BCD码等于0xF，2、达到BCD码数组的最大长度
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为1，
//           pSrcFields[0]指向正向BCD码数组， 
//           nSrcFieldLen[0]指示该正向BCD码数组的长度
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CConvertFun::BCDTime2Sec(void* pCurField, const UINT4 nSrcFields, 
                            const void* pSrcFields[],
                            const UINT4 nSrcFieldLen[],
                            const SFieldInfo* pFI,
                            const void* pDefault, const UINT4 nDefaultLen,
                            int nIsQuery)
{
    
    if(0 != BCD2String(pCurField,  nSrcFields, pSrcFields, nSrcFieldLen, 
                       pFI, pDefault,  nDefaultLen,nIsQuery))
    {
        return -1;
    }
    
    //并根据时分秒的BCD码转换成以秒显示的字符串
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针   
	char   szTmp[4];
    pCurStr = (char*)pCurField;        
    UINT4 uSec = 0;
    UINT2 uTmp = 0;
    int  yMsec =0;  
    int   nIndex = 0;   
	
    memcpy(szTmp, pCurStr, 3 * sizeof(char)); //小时
	szTmp[3] = '\0';
	uTmp = atoi(szTmp);
    uSec += uTmp * 3600;    
    pCurStr += 3; //后移三个字符    
    
    memcpy(szTmp, pCurStr, 2 * sizeof(char)); //分钟
	szTmp[2] = '\0';
	uTmp = atoi(szTmp);
    uSec += uTmp * 60;    
    pCurStr += 2; //后移两个字符    
   
    memcpy(szTmp, pCurStr, 2 * sizeof(char)); //秒
    szTmp[2] = '\0';
	uTmp = atoi(szTmp);
    uSec += uTmp;
    pCurStr += 2; //后移两个字符     
    
    memcpy(szTmp, pCurStr, sizeof(char)); //100毫秒    
    szTmp[1] = '\0';
	yMsec = atoi(szTmp);
    pCurStr[0] = '\0';
    sprintf((char*)pCurField, "%u.%u", uSec,yMsec); //转换到字符串中
    
    
    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}
/************ End of The BCDTime2Sec Defining *******************/

//End  by zkq for TT&T 2004.5.27


//SWPD06703 Zheng Yuqun 2004-06-19
//根据号码的实际长度来转换BCD
//第一个参数为BCD码字段
//第二个参数为长度字段，长度为转换为字符之后的长度，非BCD码字节数
int CConvertFun::BCD2StringByLen(void* pCurField, const UINT4 nSrcFields,
                                const void* pSrcFields[],
                                const UINT4 nSrcFieldLen[],
                                const SFieldInfo* pFI,
                                const void* pDefault, const UINT4 nDefaultLen,
                                int nIsQuery)
{
    int    nIndex    = 0;      //文本格式的BCD码字符串索引
    int    nBcdIndex = 0;      //BCD码数组索引
    BYTE   yCurBcd   = 0;      //当前BCD码值
    BYTE*  pBcdArray = NULL;   //BCD码数组指针
    char*  pCurStr   = NULL;   //文本格式的BCD码字符串指针



    //确保输入参数正确
    if(nSrcFields != 2)
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_INVALID_PARA_NUM,
              FUN_BCD_TO_STR_BY_LEN, nSrcFields);
        return -1;
    }

    if((pCurField == NULL) || (pSrcFields == NULL) || (pFI == NULL))
    {
        return -1;
    }
    if((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0) 
        || (pSrcFields[1] == NULL) || (nSrcFieldLen[1] == 0))
    {
        return -1;
    }

    int  nSrcNumLen = 0;
    nSrcNumLen = nSrcFieldLen[0];

    //获取实际长度
    INT4 nActulLen = 0;
    if(nSrcFieldLen[1]  == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[1], LEN_BYTE);
        nActulLen = uTmp;
    }
    else if(nSrcFieldLen[1]  == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[1], LEN_UINT2);
        //nActulLen = ITOHS(uTmp);
        nActulLen = uTmp;
    }
    else if(nSrcFieldLen[1]  == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[1], LEN_UINT4);
        //nActulLen = ITOHL(uTmp);
        nActulLen = uTmp;
    }
    else
    {
        return -1;
    }

    //初始化BCD码数组指针和文本格式的BCD码字符串指针
    pCurStr = (char*)pCurField;
    pBcdArray = (BYTE*)pSrcFields[0];

    //顺序扫描BCD码数组的每一个元素
    while(nBcdIndex < nSrcNumLen && nActulLen > 0)
    {
        //获取当前BCD码的值
        yCurBcd = pBcdArray[nBcdIndex++];

        //如果高四位是正常的BCD码值，即小于16
        if(yCurBcd < 0xF0)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0xA0)
            {
                pCurStr[nIndex++] = (yCurBcd >> 4) + '0';
            }

            //如果当前BCD码值大于10
            else
            {
				//add by zkq 2004-05-27
				if((yCurBcd & 0xF0) == 0xB0)
                {
                   pCurStr[nIndex++] = '*';
                }
				else if((yCurBcd & 0xF0) == 0xC0)
                {
                   pCurStr[nIndex++] = '#';
                }//add end 
				else
				{
                pCurStr[nIndex++] = (yCurBcd >> 4) - 10 + 'A';
				}
            }

            nActulLen--;

            //Added by ZhengYuqun SWPD06312 2004-06-28
            if(nActulLen == 0)
            {
                break;
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }

        //屏蔽当前BCD码数组值的高四位
        yCurBcd &= 0x0F;

        //如果低四位是正常的BCD码值，即小于16
        if(yCurBcd < 0x0F)
        {
            //如果当前BCD码值小于10
            if(yCurBcd < 0x0A)
            {
                pCurStr[nIndex++] = yCurBcd + '0';
            }

            //如果当前BCD码值大于10
            else
            {
				//add by zkq 2004-05-27
				if(yCurBcd == 0x0B)
                {
                   pCurStr[nIndex++] = '*';
                }
				else if(yCurBcd == 0x0C)
                {
                   pCurStr[nIndex++] = '#';
                }//add end
				else
				{
                    pCurStr[nIndex++] = yCurBcd - 10 + 'A';
				}
            }

            nActulLen--;

            //Added by ZhengYuqun SWPD06312 2004-06-28
            if(nActulLen == 0)
            {
                break;
            }
        }

        //到了BCD码数组结束位置
        else
        {
            break;
        }
    }

    //设置字符串的结束符
    pCurStr[nIndex] = '\0';

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    //查询条件转换时不需要增加任何填充符和分隔符
    return 0;
}


/************ The GetArgSpecField Defining ***************************/
// 功能描述：用新的前缀替换电话号码中旧的前缀，并根据配置决定是否增加号首集。
//           替换原则:
//           电话号码前缀按最大匹配原则与前缀表中的前缀进行匹配。                
//
// 输入参数：各个域的说明请参见CONVERTFUN函数类型的参数说明，
//           需要补充的是调用此函数时nSrcFields必须为2或者3，
// 
//           pSrcFields[0]/nSrcFieldLen[0] : 中继群号/中继群号的长度/必选参数
//           因为常数是排在最后的，因此有最后一个参数总是最终的字段名称，
//           所在位置pSrcFields[1]或pSrcFields[2]。
//           在2的情况时，pSrcFields[1]的字段为被叫号码
//
// 输出参数：各个域的说明请参见CONVERTFUN函数类型的参数说明
//
// 返回值：成功返回0，失败返回-1
// 
// Added by chenliangwei , 2004-8-3
//修改记录：
/***************************************************************/
typedef struct _STrunkGroupAttr
{
    UINT2     nTrunkGroupID;
    UINT4     nCarrierID;
    char      szCountry[10];
    UINT4     nCountryID;
} STrunkGroupAttr;

typedef MAP<UINT2, STrunkGroupAttr> TRUNK_TABLE;


int CConvertFun::GetArgSpecField(void* pCurField, const UINT4 nSrcFields,
                              const void* pSrcFields[],
                              const UINT4 nSrcFieldLen[],
                              const SFieldInfo* pFI,
                              const void* pDefault, const UINT4 nDefaultLen,
                              int nIsQuery)
{
    //确保输入参数正确
    if((pCurField == NULL) || nSrcFields < 2 || nSrcFields > 3 || (pSrcFields == NULL))
    {
        return -1;
    }

    if ((pSrcFields[0] == NULL) || (nSrcFieldLen[0] == 0))
    {
        return -1;
    }


    static TRUNK_TABLE trunkTable;
	static BOOL bIsFirstRun = TRUE;
    static ACE_Mutex mutex;    

    /*
     * 加载前缀表的配置文件。
     */
    mutex.acquire();
    if(bIsFirstRun)
    {
        //读取并保存中继群属性表
		FILE* f = fopen("./config/area/trunktable.cfg", "rt");
        if( f == NULL)
        {
            CreateDeepDir("./config/area");
            f = fopen("./config/area/trunktable.cfg", "w");
            if(f == NULL)
            {
                mutex.release();
                return -1;
            }
            fclose(f);
        }
        else
        {
            char szLine[1024];
            while(!feof(f))
            {
                char* p = fgets(szLine, sizeof(szLine), f);
                if((p != NULL) && (szLine[0] != '\0'))
                {
                    STrunkGroupAttr stTrunkAttr;
                    memset(&stTrunkAttr, 0, sizeof(STrunkGroupAttr));

                    int nRead = sscanf(szLine, "%d%d%s%d", 
                                       &stTrunkAttr.nTrunkGroupID,
                                       &stTrunkAttr.nCarrierID, 
                                       stTrunkAttr.szCountry,
                                       &stTrunkAttr.nCountryID);
                    if(nRead != 4)
                    {
                        continue;
                    }

                    trunkTable[stTrunkAttr.nTrunkGroupID] = stTrunkAttr;
                }
            }
            fclose(f);
        }
		bIsFirstRun = FALSE; 
    }
    mutex.release();

    /*
     * 根据不同的目标字段名，区取不同的值。
     */
    mutex.acquire();
    char* szDstField = (char*)pSrcFields[nSrcFields - 1];
    UINT2 nTrunkGroupID = *(UINT2*)pSrcFields[0];

    BOOL bFind = (trunkTable.find(nTrunkGroupID) != trunkTable.end());
    if(StringiCmp(szDstField, "SubscriberB"))
    {
        //将电话号码从BCD码转换为文本
        char szNumber[31] = "";
        BCDtoString((BYTE*)pSrcFields[1], szNumber, nSrcFieldLen[1], sizeof(szNumber) - 1);
        szNumber[sizeof(szNumber) - 1] = '\0';
        char* pF = strchr((char*)szNumber, 'F');
        if(pF != NULL)
        {
            *pF = '\0';
        }                

        if(bFind)
        {
            STrunkGroupAttr& stTrunkAttr = trunkTable[nTrunkGroupID];
            if(StringiCmp(stTrunkAttr.szCountry, "USA"))
            {
                char szTemp[31];
                SNPRINTF(szTemp, sizeof(szTemp), 
                        "1%s", szNumber);
                szTemp[sizeof(szTemp) - 1] = '\0';
                strcpy(szNumber, szTemp);
            }
        }
        strcpy((char*)pCurField, szNumber);
    }
    else if(StringiCmp(szDstField, "DestType"))
    {
        UINT4 nDestType = 0;
        if(bFind)
        {
            STrunkGroupAttr& stTrunkAttr = trunkTable[nTrunkGroupID];
            nDestType = stTrunkAttr.nCountryID;
            sprintf((char*)pCurField, "%d", nDestType);
        }
        else
        {
            sprintf((char*)pCurField, "FFF");
        }
    }
    else if(StringiCmp(szDstField, "CarrierID"))
    {
        UINT4 nCarrierID = 0;
        if(bFind)
        {
            STrunkGroupAttr& stTrunkAttr = trunkTable[nTrunkGroupID];
            nCarrierID = stTrunkAttr.nCarrierID;
            sprintf((char*)pCurField, "%d", nCarrierID);
        }
        else
        {
            sprintf((char*)pCurField, "FFF");
        }
    }
    else
    {
        mutex.release();
        return -1;
    }
    mutex.release();

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }

    return 0;
}



/***************      The Define of  IntPlus   ************************/
// 功能：整型域和常量做加法运算
//
// 输入参数：
//           pCurField      : 存放结果
//           nSrcFields     : 在本函数中应该等于2
//           pSrcFields     : 做加法运算的域，或常量
//           nSrcFieldLen   : 对应pSrcFields的长度
//
// 输出： -1：失败, 0: 成功
//
// Added By Lucy 2004-08-16
/**********************************************************************/
int CConvertFun::Add(void* pCurField, const UINT4 nSrcFields,
                   const void* pSrcFields[], 
                   const UINT4 nSrcFieldLen[], 
                   const SFieldInfo* pFI, 
                   const void* pDefault, const UINT4 nDefaultLen,
                   int nIsQuery)
{
    //输入参数
    UINT4 nSrc[2] = {0, 0};

    //验证输入参数正确
    if((pCurField == NULL) || (nSrcFields != 2)|| 
        (pSrcFields == NULL) || (pFI == NULL))
    {
        TRACE(MTS_FORMAT, S_FMT_CONFUN_ERR_PARA_NUM,
            FUN_ADD, nSrcFields);        
        return -1;
    }
    for(int i = 0; i < nSrcFields; i++)
    {
        if((pSrcFields[i] == NULL) || (nSrcFieldLen[i] == 0))
        {
            return -1;
        }
    }
    //end of 验证
    
    //获取原格式域的整型值
    if(nSrcFieldLen[0] == LEN_BYTE)
    {
        BYTE uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_BYTE);
        nSrc[0] = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT2)
    {
        UINT2 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT2);
        nSrc[0] = uTmp;
    }
    else if(nSrcFieldLen[0] == LEN_UINT4)
    {
        UINT4 uTmp = 0;
        memcpy(&uTmp, pSrcFields[0], LEN_UINT4);
        nSrc[0] = uTmp;
    }    
    else
    {
        return -1;
    }

    BOOL bIsAllF = TRUE;  //是否全为0xF
    
    for(int j = 0; j < nSrcFieldLen[0]; j++)
    {
        if(*((BYTE*)(pSrcFields[0]) + j) != 0xFF)
        {
            bIsAllF = FALSE;
            break;
        }
    }
    
    //如果全部为F
    if(bIsAllF)
    {
        sprintf((char*)pCurField, "%u", nSrc[0]);
    }
    else
    {    
        const char* szSrc = (const char*)pSrcFields[1];
        nSrc[1] = atoi(szSrc);
    
        sprintf((char*)pCurField, "%u", nSrc[0] + nSrc[1]);
    }

    //将目标格式域增加填充符和分隔符
    if(!nIsQuery)
    {
        //如果不是为查询条件转换
        return AddFillerSeparator((char*)pCurField, pFI);
    }
    
    return 0;
}


















