/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CConvert
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/
#include "convert_fun.h"
#include "format.h"
#include "postfix_exp.h"
#include "../include/toolbox.h"
#include "utility_ex.h"
#include "convert.h"


/************ The CConvert Defining ****************************/
// 功能描述：构造函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CConvert::CConvert()
{
    //初始化成员变量
    m_pDestFmt = NULL;
    m_pSrcFmt = NULL;
    m_pConvertArray = NULL;
    m_pConvertMap = NULL;

    for(int i=0; i<MAX_SRC_NUM; i++)
    {
        m_pSrcValue[i] = new BYTE[FMT_FIELD_MAX_LEN];
    }
}
/************ End of The CConvert Defining *********************/


/************ The ~CConvert Defining ***************************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CConvert::~CConvert()
{
    CONVERT_MAP::iterator item;  //以域名索引域的转换关系迭代子


    //如果域的转换关系存在
    if(m_pConvertMap != NULL)
    {
        //删除转换关系中的所有SFieldConvertMap对象
        item = m_pConvertMap->begin();
        while(item != m_pConvertMap->end())
        {
            delete item->second;
            item++;
        }

        //删除以域名索引域的转换关系对象
        delete m_pConvertMap;
        m_pConvertMap = NULL;

        //删除以域序号索引域的转换关系数组对象
        delete[] m_pConvertArray;
        m_pConvertArray = NULL;
    }

    for(int i=0; i<MAX_SRC_NUM; i++)
    {
        if(m_pSrcValue[i] != NULL)
        {
            delete[] m_pSrcValue[i];
        }
    }
}
/************ End of The ~CConvert Defining ********************/


extern char szFormatCfgErr[2048];


/************ The Init Defining ********************************/
// 功能描述：初始化函数
//
// 输入参数：
//   pDestFmt：转换对应的目标格式对象
//    pSrcFmt：转换对应的源格式对象
//
// 输出参数：无
//
//   返回值：如果出错，返回-1，否则返回0
//
/***************************************************************/
int CConvert::Init(CFormat* pDestFmt, CFormat* pSrcFmt)
{
    int           nFieldNum    = 0;     //目标格式中域个数
    FIELD_MAP*    pSrcFmtMap   = NULL;  //源格式域描述映射指针
    SFieldInfo*   pFI          = NULL;  //目标格式当前域描述指针
    SFieldInfo*   pScrFI       = NULL;  //原格式描述指针
    SFieldInfo**  pFieldsArray = NULL;  //目标格式域数组指针

    SFieldConvertMap*   pFieldConMap = NULL;  //目标格式当前域转换函数映射指针
    FIELD_MAP::iterator iFMPItem;   //当前域转换函数参数列表中某个参数对应的原格式映射位置

    STRING        sLib;      //当前域转换函数的库名
    STRING        sFun;      //当前域转换函数名称
    STRING        sDefault;  //当前域转换函数中的缺省值
    LIST<STRING>  ParaList;  //当前域转换函数的参数列表
    LIST<STRING>::iterator iPLItem;  //当前域转换函数参数列表迭代子


    //根据传入的参数值初始化原格式对象和目标格式对象
    m_pDestFmt = pDestFmt;
    m_pSrcFmt = pSrcFmt;
    if((m_pDestFmt == NULL) || (m_pSrcFmt == 0))
    {
        return -1;
    }

    //获取目标格式域描述数组和域个数
    pFieldsArray = (SFieldInfo**)pDestFmt->GetFieldsArray();
    nFieldNum = pDestFmt->GetFieldNum();

    //获取原格式的域名和域描述的映射
    pSrcFmtMap = (FIELD_MAP*)pSrcFmt->GetFieldsMap();

    //创建以域序号索引域的转换关系和以域名索引域的转换关系
    m_pConvertArray = new SFieldConvertMap*[nFieldNum];
    m_pConvertMap = new CONVERT_MAP;

    //初始化每一个本格式域与其转换函数指针的对应关系
    for(int i = 0; i< nFieldNum; i++)
    {
        //创建一个本格式域与其转换函数指针的对应关系
        //并加入以域序号索引域的转换关系的数组中
        pFieldConMap = new SFieldConvertMap;
        m_pConvertArray[i] = pFieldConMap;

        //初始化本格式域对应关系中的格式描述指针
        pFieldConMap->pFieldInfo = (SFieldInfo*)pFieldsArray[i];

        //获取当前域的格式描述指针
        pFI = pFieldConMap->pFieldInfo;

        //把当前目标格式关系加入到以域名索引域的转换关系中
        m_pConvertMap->insert(CONVERT_MAP::value_type(pFI->szFieldName, pFieldConMap));

        //如果当前域的转换函数字符串为空，则目标格式域只对应原格式的一个域，且域名相同
        if(strlen(pFI->szConvertFun) == 0)
        {
            pFieldConMap->ySrcNum = 1;
            pFieldConMap->ConvertFun = NULL;

            iFMPItem = pSrcFmtMap->find(pFI->szFieldName);
            if(iFMPItem != pSrcFmtMap->end())
            {
                //如果找到，则获取其原格式描述，将其序号赋给本格式关系中
                //计算本格式域需要用到的源格式相应域的索引
                pScrFI = iFMPItem->second;
                pFieldConMap->Index[0] = pScrFI->nFieldIndex;
            }
            else
            {
                SNPRINTF(szFormatCfgErr,
                         sizeof(szFormatCfgErr),
                         S_FMT_CONFUN_FIELD_PARA_NOT_FOUND,
                         pFI->szFieldName, 
                         m_pDestFmt->GetFmtName(), 
                         pFI->szConvertFun, 
                         pFI->szFieldName, 
                         m_pSrcFmt->GetFmtName());
                TRACE(MTS_FORMAT, szFormatCfgErr);

                //删除所有创建的SFieldConvertMap对象
                while(i >= 0)
                {
                    delete m_pConvertArray[i];
                    m_pConvertArray[i--] = NULL;
                }

                //删除以域序号索引域的转换关系数组对象
                delete[] m_pConvertArray;
                m_pConvertArray = NULL;

                //删除以域名索引域的转换关系对象
                delete m_pConvertMap;
                m_pConvertMap = NULL;

                return -1;
            }

            continue;
        }

        //将本格式当前域的转换函数字符串解析成库名、函数名、参数列表、域缺省值
        if(CConvertFun::ConvertAnalysis(pFI->szConvertFun, sLib, sFun,
                                        sDefault, ParaList) != 0)
        {
            SNPRINTF(szFormatCfgErr,
                     sizeof(szFormatCfgErr),
                     S_FMT_CONFUN_CAN_NOT_ALALYSIS,
                     pFI->szFieldName, 
                     m_pDestFmt->GetFmtName(), 
                     pFI->szConvertFun);
            TRACE(MTS_FORMAT, szFormatCfgErr);
                     
            //如果转换函数字符串失败，则删除所有创建的SFieldConvertMap对象
            while(i >= 0)
            {
                delete m_pConvertArray[i];
                m_pConvertArray[i--] = NULL;
            }

            //删除以域序号索引域的转换关系数组对象
            delete[] m_pConvertArray;
            m_pConvertArray = NULL;

            //删除以域名索引域的转换关系对象
            delete m_pConvertMap;
            m_pConvertMap = NULL;

            return -1;
        }

        //初始化本格式域对应关系中当前域对应的原格式域个数
        //pFieldConMap->ySrcNum = ParaList.size();
		pFieldConMap->ySrcNum = 0;
        if(GetParaValue(sDefault, pFieldConMap->sDefault,
            CPostfixExp::GetDataType(sDefault.c_str())) != 0)
        {

            SNPRINTF(szFormatCfgErr,
                     sizeof(szFormatCfgErr),
                     S_FMT_CONFUN_DEFAULT_CAN_NOT_ALALYSIS,
                     pFI->szFieldName, 
                     m_pDestFmt->GetFmtName(), 
                     pFI->szConvertFun);
            TRACE(MTS_FORMAT, szFormatCfgErr);

            return -1;
        }

        int j = 0;  //计算本格式域需要用到的源格式各个域的索引计数器

        //针对本格式域的转换函数中每一个参数
        iPLItem = ParaList.begin();
        while(iPLItem != ParaList.end())
        {
            //Modified by XDB on 2003.04.01
            //增加常量参数列表和缺省值参数
            //首先判断参数的类型，如果不是变量类型就认为是常量参数
            int nParaType = CPostfixExp::GetDataType((*iPLItem).c_str());
            if(nParaType != ET_VARIANT)
            {
                //常量参数类型
                STRING sTmp;
                if(GetParaValue(*iPLItem, sTmp, nParaType) != 0)
                {
                    SNPRINTF(szFormatCfgErr,
                             sizeof(szFormatCfgErr),
                             S_FMT_CONFUN_INVALID_PARA_TYPE,
                             pFI->szFieldName, 
                             m_pDestFmt->GetFmtName(), 
                             pFI->szConvertFun, 
                             (*iPLItem).c_str());
                    TRACE(MTS_FORMAT, szFormatCfgErr);

                    return -1;
                }

                pFieldConMap->ConstValues.push_back(sTmp);
                ++iPLItem;
                continue;
            }

            //查找当前参数对应的原格式映射关系位置
			pFieldConMap->ySrcNum++;
            iFMPItem = pSrcFmtMap->find(*iPLItem);
            if(iFMPItem != pSrcFmtMap->end())
            {
                //如果找到，则获取其原格式描述，将其序号赋给本格式关系中
                //计算本格式域需要用到的源格式相应域的索引
                pScrFI = iFMPItem->second;
                pFieldConMap->Index[j++] = pScrFI->nFieldIndex;
            }

            //如果当前参数在原格式中没有找到
            else
            {
                SNPRINTF(szFormatCfgErr,
                         sizeof(szFormatCfgErr),
                         S_FMT_CONFUN_FIELD_PARA_NOT_FOUND,
                         pFI->szFieldName, 
                         m_pDestFmt->GetFmtName(), 
                         pFI->szConvertFun, 
                         (*iPLItem).c_str(), 
                         m_pSrcFmt->GetFmtName());
                TRACE(MTS_FORMAT, szFormatCfgErr);
                
                //删除所有创建的SFieldConvertMap对象
                while(i >= 0)
                {
                    delete m_pConvertArray[i];
                    m_pConvertArray[i--] = NULL;
                }

                //删除以域序号索引域的转换关系数组对象
                delete[] m_pConvertArray;
                m_pConvertArray = NULL;

                //删除以域名索引域的转换关系对象
                delete m_pConvertMap;
                m_pConvertMap = NULL;

                return -1;
            }

            iPLItem++;
        }

        //如果本格式配有转换函数
        if(sFun.length() != 0)
        {
            //查找相应的转换函数指针，并赋给本格式对应关系中的转换函数指针
            pFieldConMap->ConvertFun = CConvertFun::FindFun(pFI->szConvertFun, m_pSrcFmt);
            if(NULL == pFieldConMap->ConvertFun)
            {
                SNPRINTF(szFormatCfgErr,
                         sizeof(szFormatCfgErr),
                         S_FMT_CONFUN_NOT_FOUND,
                         pFI->szFieldName, 
                         m_pDestFmt->GetFmtName(), 
                         pFI->szConvertFun);
                TRACE(MTS_FORMAT, szFormatCfgErr);

                return -1;
            }
        }

        //如果本格式没有配置转换函数
        else
        {
            pFieldConMap->ConvertFun = NULL;
        }
        //Modified by XDB on 2003.04.01
        //增加参数个数的判断，以便于保证缓冲区数量够大
        if((pFieldConMap->ySrcNum + pFieldConMap->ConstValues.size()) > MAX_SRC_NUM)
        {

            SNPRINTF(szFormatCfgErr,
                     sizeof(szFormatCfgErr),
                     S_FMT_CONFUN_PARA_NUM_EXCEED_LIMIT,
                     pFI->szFieldName, 
                     m_pDestFmt->GetFmtName(), 
                     pFI->szConvertFun);
            TRACE(MTS_FORMAT, szFormatCfgErr);            

            return -1;
        }
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The ConvertABill Defining ************************/
// 功能描述：将一张源格式话单转换成目标格式函数
//
// 输入参数：
//    pSrcABill：源格式话单指针
//  uSrcBillLen：pSrcABill指向的话单的长度
//
// 输出参数：
//
//   pDestABill：转换后的目标格式话单指针，内存在本函数内分配，由调用者负责释放；
// uDestBillLen：pDestABill指向的话单长度
//
//   返回值：如果出错，返回-1，否则返回0
//
/***************************************************************/
int CConvert::ConvertABill(const void*  pSrcABill, const UINT4 uSrcBillLen, 
                           void*& pDestABill, UINT4& uDestBillLen)
{
    SFieldConvertMap* pFieldConMap = NULL;  //目标格式域关系中的一个节点

    UINT4  uByteOffset  = 0;      //以字节为单位的目标格式域偏移量
    UINT4  uFieldLen    = 0;      //转换出的目标格式域长度
    UINT4  uBitLen      = 0;      //位域类型的域转换出的目标格式域长度
    UINT4  uBitOffset   = 0;      //位域类型的域转换时的位偏移量

    UINT4  uTmpVal1     = 0;      //位域类型的域转换时的临时变量1
    UINT4  uTmpVal2     = 0;      //位域类型的域转换的临时变量2
    BYTE*  pTmp         = NULL;   //目标格式的临时指针

	UINT4  pSrcLen[MAX_SRC_NUM];       //目标格式域对应的原格式各域的长度
	UINT4  uSrcNum  = 0;               //目标格式对应的原格式域个数
    BYTE*  pValue = m_szFieldBuffer;   //转换出的目标格式域值

    //初始化输出参数
    uDestBillLen = 0;
    pDestABill = NULL;

    //确保输入的话单数据不为空
    if((pSrcABill == NULL) || (uSrcBillLen == 0))
    {
        return -1;
    }

    //根据目标格式长度，创建目标格式需要的空间
    uDestBillLen = m_pDestFmt->GetFmtLen();
    pDestABill = (void*)new BYTE[uDestBillLen];
    pTmp = (BYTE*)pDestABill;

    //对目标格式的每一个域进行转换
    int i = 0;
    UINT4 uFieldNum = m_pDestFmt->GetFieldNum();
    while(i < uFieldNum)
    {
        //取出目标格式的一个域关系对象
        pFieldConMap = m_pConvertArray[i++];

        //转换当前域不需要用函数
        if(pFieldConMap->ConvertFun == NULL)
        {
            //确保目标格式的当前域只对应原格式的一个域
            if(pFieldConMap->ySrcNum != 1)
            {
                delete[] (BYTE*)pDestABill;
                pDestABill = NULL;
                uDestBillLen = 0;

                return -1;
            }

            //当前域不是位域型
            if(pFieldConMap->pFieldInfo->flgBit == 0)
            {
                //根据当前域对应原格式域的索引，转换出当前域的值
                //SWPD03631 增加最后一个参数实现二进制取枚举 2004-02-18 ZhengYuqun
                if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0 , pFieldConMap->Index[0],
                                                    pValue, uFieldLen, pFieldConMap->pFieldInfo)) == 0)
                {
                    //调整域值，主要是对各种长度的整数类型值的调整
                    uFieldLen = AdjustValue(pValue, uFieldLen, pFieldConMap->pFieldInfo);

					//需要加上对字节序的恢复，从本机字节序到intel或者到网络
					ConvertToDestOrder(pFieldConMap->pFieldInfo, pValue);

                    //将当前域转换的结果放入目标格式的输出结果中去
                    memcpy((void*)(pTmp + uByteOffset), (void*)pValue, uFieldLen);
                    uByteOffset += uFieldLen;  //设置目标格式下一个域的起始位置
                }
                //如果当前域转换出错
                else
                {
                    TRACE(MTS_FORMAT, S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL, 
                          m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                          i - 1, pFieldConMap->Index[0]);
                    delete[] (BYTE*)pDestABill;
                    pDestABill = NULL;
                    uDestBillLen = 0;

                    return -1;
                }
            }
            //当前域是位域型
            else
            {
                //设置目格式下一个位域型域的起始位置
                uBitOffset = pFieldConMap->pFieldInfo->nFieldLength;
                uFieldLen = CFormat::GetFieldTypeLen(pFieldConMap->pFieldInfo);
                if(uFieldLen <= 0)
                {
                    TRACE(MTS_FORMAT, S_FMT_CONVERT_FIELD_LEN_EMPTY, 
                          m_pDestFmt->GetFmtName(), i - 1);
                    delete[] (BYTE*)pDestABill;
                    pDestABill = NULL;
                    uDestBillLen = 0;

                    return -1;
                }

                //处理字节序问题
                int nLShift = 0;  //左移位数
                if(pFieldConMap->pFieldInfo->nOrderMode == INTEL_ORDER)
                {
                    nLShift = 0;
                }
                else
                {
                    nLShift = (uFieldLen * 8) - pFieldConMap->pFieldInfo->nFieldLength;
                }

                //获得第一个位域型域值
                //SWPD03631 增加最后一个参数实现二进制取枚举 2004-02-18 ZhengYuqun
                if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[0], 
                                                    pValue, uBitLen, pFieldConMap->pFieldInfo)) == 0)
                {
                    //获取当前位域转换出的值
                    if(uBitLen == LEN_BYTE)
                    {
                        BYTE nTmp = 0;
                        memcpy(&nTmp, pValue, LEN_BYTE);
                        uTmpVal1 = nTmp;
                        ///uTmpVal1 = *(BYTE*)pValue;
                    }
                    else if(uBitLen == LEN_UINT2)
                    {
                        UINT2 nTmp = 0;
                        memcpy(&nTmp, pValue, LEN_UINT2);
                        uTmpVal1 = nTmp;
                        //uTmpVal1 = *(UINT2*)pValue;
                    }
                    else if(uBitLen == LEN_UINT4)
                    {
                        UINT4 nTmp = 0;
                        memcpy(&nTmp, pValue, LEN_UINT4);
                        uTmpVal1 = nTmp;
                        //uTmpVal1 = *(UINT4*)pValue;
                    }
                    else
                    {
                        TRACE(MTS_FORMAT, S_FMT_CONVERT_FIELD_LEN_EXCEED_BITFLD, 
                              m_pDestFmt->GetFmtName(), i - 1);
                        delete[] (BYTE*)pDestABill;
                        pDestABill = NULL;
                        uDestBillLen = 0;

                        return -1;
                    }

                    //Added by ZhengYuqun SWPD07486 2004-08-09
                    UINT4 uTmpMask = 0xFFFFFFFF;
                    uTmpMask >>= 32 - pFieldConMap->pFieldInfo->nFieldLength;
                    uTmpVal1 &= uTmpMask;   
                    //End SWPD07486
                }

                //如果转换当前域出错
                else
                {
                    TRACE(MTS_FORMAT, S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL, 
                          m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                          i - 1, pFieldConMap->Index[0]);
                    delete[] (BYTE*)pDestABill;
                    pDestABill = NULL;
                    uDestBillLen = 0;

                    return -1;
                }

                uTmpVal1 <<= nLShift;  //Added by XDB on 2002.10.28
                //继续转换目标格式的下一个话单域，
                //直到该域不是位域型或者 (nBitOffset >= uFieldLen * 8)
                while((uBitOffset < (uFieldLen * 8)) && (i < uFieldNum))
                {
                    //取出目标格式的下一个域关系对象
                    pFieldConMap = m_pConvertArray[i++];

                    //如果当前域是位域型
                    if(pFieldConMap->pFieldInfo->flgBit != 0)
                    {
                        //Added by XDB on 2002.10.28
                        //处理字节序问题
                        if(pFieldConMap->pFieldInfo->nOrderMode == INTEL_ORDER)
                        {
                            nLShift = uBitOffset;
                        }
                        else
                        {
                            nLShift = (uFieldLen * 8) - uBitOffset
                                      - pFieldConMap->pFieldInfo->nFieldLength;
                        }
                        //End of Add by XDB
                        //获得当前位域型域值
                        //SWPD03631 增加最后一个参数实现二进制取枚举 2004-02-18 ZhengYuqun
                        if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[0], 
                                                            pValue, uBitLen, pFieldConMap->pFieldInfo)) == 0)
                        {
                            //获取当前位域转换出的值
                            if(uBitLen == LEN_BYTE)
                            {
                                BYTE nTmp = 0;
                                memcpy(&nTmp, pValue, LEN_BYTE);
                                uTmpVal2 = nTmp;
                            }
                            else if(uBitLen == LEN_UINT2)
                            {
                                UINT2 nTmp = 0;
                                memcpy(&nTmp, pValue, LEN_UINT2);
                                uTmpVal2 = nTmp;
                            }
                            else if(uBitLen == LEN_UINT4)
                            {
                                UINT4 nTmp = 0;
                                memcpy(&nTmp, pValue, LEN_UINT4);
                                uTmpVal2 = nTmp;
                            }
                            else
                            {
                                TRACE(MTS_FORMAT, S_FMT_CONVERT_FIELD_LEN_EXCEED_BITFLD, 
                                      m_pDestFmt->GetFmtName(), i - 1);
                                delete[] (BYTE*)pDestABill;
                                pDestABill = NULL;
                                uDestBillLen = 0;

                                return -1;
                            }

                            //Added by ZhengYuqun SWPD07486 2004-08-09
                            UINT4 uTmpMask = 0xFFFFFFFF;
                            uTmpMask >>= 32 - pFieldConMap->pFieldInfo->nFieldLength;
                            uTmpVal2 &= uTmpMask;                            
                            //End SWPD07486

                            //把当前位域值放入前一个位域中
                            //Modified by XDB on 2002.10.28
                            uTmpVal1 |= (uTmpVal2 << nLShift);

                            //设置目格式下一个位域型域的起始位置
                            uBitOffset += pFieldConMap->pFieldInfo->nFieldLength;
                        }

                        //如果转换当前域出错
                        else
                        {
                            TRACE(MTS_FORMAT, S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL, 
                                  m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                                  i - 1, pFieldConMap->Index[0]);
                            delete[] (BYTE*)pDestABill;
                            pDestABill = NULL;
                            uDestBillLen = 0;

                            return -1;
                        }                 
                    }

                    //如果当前域不是位域类型
                    else
                    {
                        i--;  //退回当前域
                        break;
                    }                    
                }

                //将转换出的位域值放入到目标格式的输出结果中去
                if(uFieldLen == LEN_BYTE)
                {
                    BYTE yTmp = (BYTE)uTmpVal1;
                    memcpy((pTmp + uByteOffset), &yTmp, LEN_BYTE);
                }
                else if(uFieldLen == LEN_UINT2)
                {
                    UINT2 uTmp = (UINT2)uTmpVal1;
                    ConvertToDestOrder(pFieldConMap->pFieldInfo, &uTmp);
                    memcpy((pTmp + uByteOffset), &uTmp, LEN_UINT2);
                }
                else if(uFieldLen == LEN_UINT4)
                {
                    ConvertToDestOrder(pFieldConMap->pFieldInfo, &uTmpVal1);

                    memcpy((pTmp + uByteOffset), &uTmpVal1, LEN_UINT4);
                }

                //设置目标格式下一个域的起始位置
                uByteOffset += uFieldLen;
            }
        }

        //当前域需要函数转换
        else
        {
            //获取当前域对应的原格式域个数
            uSrcNum = pFieldConMap->ySrcNum;

            //获取当前域对应的原格式各个域的值和其长度
            for(int j = 0; j < uSrcNum; j++)
            {
                if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[j], 
                                                   m_pSrcValue[j], pSrcLen[j])) != 0)
                {
                    TRACE(MTS_FORMAT, S_FMT_CONVERT_GET_VALUE_BY_IDX_FAIL, 
                          m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                          i - 1, pFieldConMap->Index[j]);

                    delete[] (BYTE*)pDestABill;
                    pDestABill = NULL;
                    uDestBillLen = 0;

                    return -1;
                }
            }

            //Modified by XDB on 2003.04.01
            //增加常量参数列表和缺省值参数
            //增加所有常量参数
            LIST<STRING>::iterator it = pFieldConMap->ConstValues.begin();
            while(it != pFieldConMap->ConstValues.end())
            {
                memcpy(m_pSrcValue[uSrcNum], it->c_str(), (it->length() + 1));
                pSrcLen[uSrcNum++] = it->length() + 1;
                ++it;
            }
            uFieldLen = CFormat::GetFieldTypeLen(pFieldConMap->pFieldInfo);
            uFieldLen *= pFieldConMap->pFieldInfo->nFieldLength;

            //调用当前域的转换函数得出当前域的值
            int nRet = pFieldConMap->ConvertFun((void*)pValue, uSrcNum, (const void **)m_pSrcValue,
                                                pSrcLen, pFieldConMap->pFieldInfo,
                                                pFieldConMap->sDefault.c_str(), 
                                                pFieldConMap->sDefault.length() + 1, 0);

            //如果转换函数计算出错
            if(nRet != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_CONVERT_FAIL, 
                      m_pSrcFmt->GetFmtName(), 
                      m_pDestFmt->GetFmtName(), 
                      i - 1);
                delete[] (BYTE*)pDestABill;
                pDestABill = NULL;
                uDestBillLen = 0;

                return -1;
            }

			//需要加上对字节序的恢复，从本机字节序到intel或者到网络
			ConvertToDestOrder(pFieldConMap->pFieldInfo, pValue);
            //将当前域转换的结果放入目标格式的输出结果中去
            memcpy((void*)(pTmp + uByteOffset), (void*)pValue, uFieldLen);
            uByteOffset += uFieldLen;
        }
    }

    return 0;
}
/************ End of The ConvertABill Defining *****************/


/************ The ConvertAField Defining ***********************/
// 功能描述：针对一张源格式话单获得目标格式中一个域的取值
//
// 输入参数：
//  pSrcABill：符合源格式的一张话单
// sFieldName：目标格式的一个格式域名
//   nIsQuery：是否查询标志，1表示该函数为查询条件转换，
//             0表示不是为查询条件转换
//
// 输出参数：
// pDestAField：szFieldName的取值；
//        uLen：pDestAField指向的数据帧长度，内存在本函数内申请，由调用者负责释放
//
//   返回值：如果出错，返回-1，否则返回0
//
/***************************************************************/
int CConvert::ConvertAField(const void* pSrcABill, const STRING& sFieldName, 
                            void*& pDestAField, UINT4& uDestFieldLen,
                            int nIsQuery)
{
    SFieldConvertMap* pFieldConMap = NULL;  //目标格式域关系中的一个节点

    //void*  pSrcValue[MAX_SRC_NUM];   //目标格式域对应的原格式域数组
	UINT4  pSrcLen[MAX_SRC_NUM];       //目标格式域对应的原格式各域的长度
	UINT4  uSrcNum  = 0;               //目标格式对应的原格式域个数
    CONVERT_MAP::iterator item;        //目标格式域转换关系迭代子
    BYTE*  pValue = m_szFieldBuffer;   //转换出的目标格式域值


    //初始化输出参数
    pDestAField = NULL;
    uDestFieldLen = 0;

    //确保输入的参数正确
    if((pSrcABill == NULL) || (sFieldName.length() == 0))
    {
        return -1;
    }

    //在目标格式中查找指定的域
    item = m_pConvertMap->find(sFieldName);
    if(item != m_pConvertMap->end())
    {
        pFieldConMap = item->second;
    }
    //如果目标格式中不存在指定的域
    else
    {
        TRACE(MTS_FORMAT, S_FMT_FLDNAME_NOT_FONND_CONVERT_A_FLD, 
              m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
              sFieldName.c_str());
        return -1;
    }

    //指定域不需要转换函数
    if(pFieldConMap->ConvertFun == NULL)
    {
        //确保指定域只对应原格式的一个域
        if(pFieldConMap->ySrcNum != 1)
        {
            return -1;
        }

        //根据指定域对应原格式域的索引，转换出指定域的值
        if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0 , pFieldConMap->Index[0],
                                            pValue, uDestFieldLen)) == 0)
        {
            //调整域值，主要是对各种长度的整数类型值的调整
            uDestFieldLen = AdjustValue(pValue, uDestFieldLen, pFieldConMap->pFieldInfo, nIsQuery);

			//需要加上对字节序的恢复，从本机字节序到intel或者到网络
			ConvertToDestOrder(pFieldConMap->pFieldInfo, pValue);

            //根据目标格式域结果长度，为其分配空间
            pDestAField = (void*)new BYTE[uDestFieldLen];
            memcpy(pDestAField, pValue, uDestFieldLen);
        }
        //如果当前域转换出错
        else
        {
            TRACE(MTS_FORMAT, S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD, 
                  m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                  pFieldConMap->pFieldInfo->nFieldIndex, pFieldConMap->Index[0]);
            return -1;
        }
    }

    //指定域需要函数转换
    else
    {
        //获取指定域对应的原格式域个数
        uSrcNum = pFieldConMap->ySrcNum;

        //获取指定域对应的原格式各个域的值和其长度
        for(int j = 0; j < uSrcNum; j++)
        {
            if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[j], 
                                                m_pSrcValue[j], pSrcLen[j])) != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD, 
                      m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                      pFieldConMap->pFieldInfo->nFieldIndex, pFieldConMap->Index[j]);

                return -1;
            }
        }

        //Modified by XDB on 2003.04.01
        //增加常量参数列表和缺省值参数
        //增加所有常量参数
        LIST<STRING>::iterator it = pFieldConMap->ConstValues.begin();
        while(it != pFieldConMap->ConstValues.end())
        {
            memcpy(m_pSrcValue[uSrcNum], it->c_str(), (it->length() + 1));
            pSrcLen[uSrcNum++] = it->length() + 1;
            ++it;
        }
        //根据目标格式域的长度，为其分配空间
        uDestFieldLen = CFormat::GetFieldTypeLen(pFieldConMap->pFieldInfo);
        uDestFieldLen *= pFieldConMap->pFieldInfo->nFieldLength;
        pDestAField = (void*)new BYTE[uDestFieldLen];

        //调用域的转换函数得出域的值
        int nRet = pFieldConMap->ConvertFun((void*)pDestAField, uSrcNum, (const void **)m_pSrcValue,
                                            pSrcLen, pFieldConMap->pFieldInfo,
                                            pFieldConMap->sDefault.c_str(), 
                                            pFieldConMap->sDefault.length() + 1, nIsQuery);

        //如果转换函数计算出错
        if(nRet != 0)
        {
            TRACE(MTS_FORMAT, S_FMT_CONVERT_FAIL_CONVERT_A_FLD, 
                  m_pSrcFmt->GetFmtName(), 
                  m_pDestFmt->GetFmtName(), 
                  pFieldConMap->pFieldInfo->nFieldIndex);
            delete[] (BYTE*)pDestAField;
            pDestAField = NULL;
            uDestFieldLen = 0;

            return -1;
        }
    }

    return 0;
}
/************ End of The ConvertAField Defining ****************/

/************ The ConvertAFieldByIndex Defining ***********************/
// 功能描述：针对一张源格式话单获得目标格式中一个域的取值
//
// 输入参数：
//  pSrcABill：符合源格式的一张话单
// nFieldIndex：目标格式的一个域序号
//   nIsQuery：是否查询标志，1表示该函数为查询条件转换，
//             0表示不是为查询条件转换
//
// 输出参数：
// pDestAField：szFieldName的取值；
//        uLen：pDestAField指向的数据帧长度，内存在本函数外申请，由调用者负责释放
//
//   返回值：如果出错，返回-1，否则返回0
//
/***************************************************************/
int CConvert::ConvertAFieldByIndex(const void* pSrcABill, UINT2 nFieldIndex, 
                                   void* pDestAField, UINT4& uDestFieldLen,
                                   int nIsQuery)
{
    //确保输入的参数正确
    if(pSrcABill == NULL)
    {
        return -1;
    }

	SFieldConvertMap* pFieldConMap = NULL;  //目标格式域关系中的一个节点

	UINT4  pSrcLen[MAX_SRC_NUM];       //目标格式域对应的原格式各域的长度
	UINT4  uSrcNum  = 0;               //目标格式对应的原格式域个数
    CONVERT_MAP::iterator item;        //目标格式域转换关系迭代子

    //在目标格式中查找指定的域
	pFieldConMap = m_pConvertArray[nFieldIndex];
    if(NULL == pFieldConMap)
    {
        return -1;
    }

    
    //指定域不需要转换函数
    if(pFieldConMap->ConvertFun == NULL)
    {
        //确保指定域只对应原格式的一个域
        if(pFieldConMap->ySrcNum != 1)
        {
            return -1;
        }

        //根据指定域对应原格式域的索引，转换出指定域的值
        if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[0], 
                                            pDestAField, uDestFieldLen)) == 0)
        {
            //调整域值，主要是对各种长度的整数类型值的调整
            uDestFieldLen = AdjustValue((BYTE*)pDestAField, uDestFieldLen, 
                                        pFieldConMap->pFieldInfo, nIsQuery);

			//需要加上对字节序的恢复，从本机字节序到intel或者到网络
			ConvertToDestOrder(pFieldConMap->pFieldInfo, pDestAField);
        }
        //如果当前域转换出错
        else
        {
            TRACE(MTS_FORMAT, S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD, 
                  m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                  nFieldIndex, pFieldConMap->Index[0]);
            return -1;
        }
    }

    //指定域需要函数转换
    else
    {
        //获取指定域对应的原格式域个数
        uSrcNum = pFieldConMap->ySrcNum;

        //获取指定域对应的原格式各个域的值和其长度
        for(int j = 0; j < uSrcNum; j++)
        {
            if((m_pSrcFmt->GetFieldValueByIndex(pSrcABill, 0, pFieldConMap->Index[j], 
                                                m_pSrcValue[j], pSrcLen[j])) != 0)
            {
                TRACE(MTS_FORMAT, S_FMT_GET_VAL_BY_IDX_FAIL_CONVERT_A_FLD, 
                      m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                      nFieldIndex, pFieldConMap->Index[j]);
                return -1;
            }
        }

        //Modified by XDB on 2003.04.01
        //增加常量参数列表和缺省值参数
        //增加所有常量参数
        LIST<STRING>::iterator it = pFieldConMap->ConstValues.begin();
        while(it != pFieldConMap->ConstValues.end())
        {
            memcpy(m_pSrcValue[uSrcNum], it->c_str(), (it->length() + 1));
            pSrcLen[uSrcNum++] = it->length() + 1;
            ++it;
        }
        //根据目标格式域的长度，为其分配空间
        uDestFieldLen = CFormat::GetFieldTypeLen(pFieldConMap->pFieldInfo);
        uDestFieldLen *= pFieldConMap->pFieldInfo->nFieldLength;

		
        //调用域的转换函数得出域的值
        int nRet = pFieldConMap->ConvertFun((void*)pDestAField, uSrcNum, (const void **)m_pSrcValue,
                                            pSrcLen, pFieldConMap->pFieldInfo,
                                            pFieldConMap->sDefault.c_str(), 
                                            pFieldConMap->sDefault.length() + 1, nIsQuery);

        //如果转换函数计算出错
        if(nRet != 0)
        {
            TRACE(MTS_FORMAT, S_FMT_CONVERT_FAIL_CONVERT_A_FLD, 
                  m_pSrcFmt->GetFmtName(), m_pDestFmt->GetFmtName(), 
                  nFieldIndex);
            uDestFieldLen = 0;
            return -1;
        }
        
    }

    return 0;
}
/************ End of The ConvertAField Defining ****************/
//Added by XDB on 2003.04.01
//获取参数字符串值
int CConvert::GetParaValue(const STRING& sParaStr, STRING& sParaValue, int nParaType)
{
	//首先判断输入的字符串是否为空
	if(sParaStr.size() == 0)
	{
		sParaValue = "";
		return 0;
	}

    if(nParaType == ET_INT_CONST)
    {
        //整数常量
        sParaValue = sParaStr;
    }
    else if((nParaType == ET_STRING_CONST) || (nParaType == ET_UNSIGNED_CHAR_CONST))
    {
        //字符常量或者字符串常量
        sParaValue = sParaStr.substr(1, sParaStr.length() - 2);
    }
    else //其它都看为非法
    {
        //暂时不考虑其它类型
        return -1;
    }

    return 0;
}

/************ The HomeToIntelOrNet Defining ***********************/
// 功能描述：把本机字节序转换为INTEL和Net字节序
//
// 输入参数：
//  SFieldInfo* pFI：当前格式的结构
//  void* pFieldValue：转换前的值
//
// 输出参数：
// void* pFieldValue：转换后的值；
// 
//added by maimaoshi,2002-10-24
//Modified by XDB on 2002.10.28
/***************************************************************/
int CConvert::ConvertToDestOrder(const SFieldInfo* pFI, void* pFieldValue)
{
    //INTEL_ORDER	0;表示格式为INTEL字节序
    //NET_ORDER 	1;表示格式为网络字节序
    if((pFI == NULL) || (pFieldValue == NULL))
    {
        return -1;
    }  
    
    INT2  nShortValue = 0;
	INT4  nIntValue = 0;
    if ((pFI->nDataType == FT_SHORT) || (pFI->nDataType == FT_USHORT))
    {
        //对于2字节的类型处理
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //原格式为本机字节序，需要把本机字节许转为INTEL字节序
            memcpy(&nShortValue, pFieldValue, LEN_INT2);
            nShortValue = HTOIS(nShortValue);
            memcpy(pFieldValue, &nShortValue, LEN_INT2);
        }
        else
        {
            //原格式为本机字节序，需要把本机字节许转为网络字节序
            memcpy(&nShortValue, pFieldValue, LEN_INT2);
            nShortValue = HTONS(nShortValue);
            memcpy(pFieldValue, &nShortValue, LEN_INT2);			
        }
        
    }
    else if ((pFI->nDataType == FT_LONG) || (pFI->nDataType == FT_ULONG))
    {
        //对于4字节的类型处理
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //原格式为本机字节序，需要把本机字节许转为INTEL字节序
            memcpy(&nIntValue, pFieldValue, LEN_INT4);
            nIntValue = HTOIL(nIntValue);
            memcpy(pFieldValue, &nIntValue, LEN_INT4);
        }
        else
        {
            //原格式为本机字节序，需要把本机字节许转为网络字节序
            memcpy(&nIntValue, pFieldValue, LEN_INT4);
            nIntValue = HTONL(nIntValue);
            memcpy(pFieldValue, &nIntValue, LEN_INT4);
        }        
    }

    return 0;   
}
/************ End of The HomeToIntelOrNet Defining ****************/

//Added by XDB on 2003.04.04 and 2003.04.07
//调整域值函数，主要是将原值与目标值的长度不一致的情况
//返回调整后的域值长度
UINT4 CConvert::AdjustValue(BYTE* pValue, UINT4 nOrigLen, const SFieldInfo* pFI, int nIsQuery)
{
    const FIELD_MAP* pFieldMap = m_pSrcFmt->GetFieldsMap();
    FIELD_MAP::const_iterator iter = pFieldMap->find(pFI->szFieldName);
    if(iter == pFieldMap->end())
    {
        return 0;
    }

    //如果原值的长度等于目标值的长度，则不用调整
    UINT4 nDestLen = CFormat::GetFieldTypeLen(pFI) * pFI->nFieldLength;
    const SFieldInfo* pSrcFI = iter->second;

    if(pFI->nFieldLength == pSrcFI->nFieldLength && pSrcFI->nDataType == pFI->nDataType)
    {
        //memmove(pValue, pValue, nDestLen);
        return nDestLen;
    }
    //注意，只有对于某种整数的值才能调整，其它类型则直接拷贝目标的值长度
    switch(pFI->nDataType)
    {
    case FT_BYTE:
    case FT_CHAR:
        //如果目标值是一字节整数类型
        if(nDestLen == LEN_BYTE)
        {
            if(nOrigLen == LEN_UINT2)
            {
                UINT2 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT2);
                BYTE nValue = nTmp;
                memcpy(pValue, &nValue, LEN_BYTE);
                return LEN_BYTE;
            }
            else if(nOrigLen == LEN_UINT4)
            {
                UINT4 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT4);
                BYTE nValue = nTmp;
                memcpy(pValue, &nValue, LEN_BYTE);
                return LEN_BYTE;
            }
        }
        break;

    case FT_SHORT:
    case FT_USHORT:
        //如果目标值是二字节整数类型
        if(nDestLen == LEN_UINT2)
        {
            if(nOrigLen == LEN_BYTE)
            {
                BYTE nTmp = 0;
                memcpy(&nTmp, pValue, LEN_BYTE);
                UINT2 nValue = nTmp;
                memcpy(pValue, &nValue, LEN_UINT2);
                return LEN_UINT2;
            }
            else if(nOrigLen == LEN_UINT4)
            {
                UINT4 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT4);
                UINT2 nValue = nTmp;
                memcpy(pValue, &nValue, LEN_UINT2);
                return LEN_UINT2;
            }
        }
        break;

    case FT_LONG:
    case FT_ULONG:
        //如果目标值是四字节整数类型
        if(nDestLen == LEN_UINT4)
        {
            if(nOrigLen == LEN_BYTE)
            {
                BYTE nTmp = 0;
                memcpy(&nTmp, pValue, LEN_BYTE);
                UINT4 nValue = nTmp;
                memcpy(pValue, &nValue, LEN_UINT4);
                return LEN_UINT4;
            }
            else if(nOrigLen == LEN_UINT2)
            {
                UINT2 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT2);
                UINT4 nValue = nTmp;
                memcpy(pValue, &nValue, LEN_UINT4);
                return LEN_UINT4;
            }
        }
        break;

    case FT_STRING:
        switch(pSrcFI->nDataType) {
        case FT_STRING:
            pValue[nOrigLen] = '\0';
        	break;

        case FT_CHAR:
            {
                char nTmp = 0;
                memcpy(&nTmp, pValue, LEN_CHAR);
                SNPRINTF((char*)pValue, nDestLen,
                         "%d",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_BYTE:
            {
                BYTE nTmp = 0;
                memcpy(&nTmp, pValue, LEN_BYTE);
                SNPRINTF((char*)pValue, nDestLen,
                         "%u",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_SHORT:
            {
                INT2 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_INT2);
                SNPRINTF((char*)pValue, nDestLen,
                         "%d",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_USHORT:
            {
                UINT2 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_INT2);
                SNPRINTF((char*)pValue, nDestLen,
                         "%u",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_LONG:
            {
                INT4 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT4);
                SNPRINTF((char*)pValue, nDestLen,
                         "%d",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_ULONG:
            {
                UINT4 nTmp = 0;
                memcpy(&nTmp, pValue, LEN_UINT4);
                SNPRINTF((char*)pValue, nDestLen,
                         "%u",
                         nTmp);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        case FT_BCD:
            {
                char szTemp[FMT_FIELD_MAX_LEN] = "";
                UINT4 nLen = FMT_FIELD_MAX_LEN;
                BCDtoString(pValue, szTemp,
                            nOrigLen,
                            nLen);

                //去掉终结符之后的字符串
                char* p1 = szTemp;
                while((*p1 >= '0')
                      && (*p1 <= '9'))
                {
                    p1++;
                }
                *p1 = '\0';

                strncpy((char*)pValue, szTemp, nDestLen);
                pValue[nDestLen - 1] = '\0';
            }
        	break;

        default:
            break;
        }

        if(!nIsQuery)
        {
            CConvertFun::AddFillerSeparator((char*)pValue, pFI);
        }
        break;

    default:
        break;
    }
    
    return nDestLen;
}


