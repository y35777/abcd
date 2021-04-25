/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CCondition
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
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "condition.h"

/************ The CCondition Defining **************************/
// 功能描述：构造函数
//
// 输入参数：
// pFmtOwning：所属格式
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CCondition::CCondition(CFormat* pFmtOwning): m_pFmtOwning(pFmtOwning)
{
    //初始化成员变量
    //m_pFmtOwning = (CFormat*)pFmtOwning;
    m_bThisFmt = 0;
    m_nIsQuery = 1;
}
/************ End of The CCondition Defining *******************/


/************ The CCondition Defining **************************/
// 功能描述：构造函数
//
// 输入参数：
//    pFmtOwning：所属格式
// sDisCondition：识别条件，中缀表达式
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CCondition::CCondition(CFormat* pFmtOwning, const STRING& sDisCondition):
                       m_pFmtOwning(pFmtOwning)
{
    //初始化成员变量
    //m_pFmtOwning = (CFormat*)pFmtOwning;
    m_bThisFmt = 0;
    m_nIsQuery = 1;

    //将中缀表达式转换成后缀表达式
    if(Infix2Postfix(sDisCondition.c_str()) != 0)
    {
        assert(0);
    }
}
/************ End of The CCondition Defining *******************/


/************ The ~CCondition Defining *************************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CCondition::~CCondition()
{
}
/************ End of The ~CCondition Defining ******************/


/************ The SetFlag Defining *****************************/
// 功能描述：设置格式标志函数
//
// 输入参数：
//   bThisFmt：格式标志，为0表示本格式，为-1表示源格式
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CCondition::SetFlag(const int bThisFmt)
{
    m_bThisFmt = bThisFmt;
}
/************ End of The SetFlag Defining **********************/


/************ The SetQueryFlag Defining ************************/
// 功能描述：设置查询条件标志函数
//
// 输入参数：
//   nIsQuery：查询条件标志：为1表示当前对象是查询条件对象，
//             为0表示当前对象不是查询条件对象
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CCondition::SetQueryFlag(int nIsQuery)
{
    m_nIsQuery = nIsQuery;
}
/************ End of The SetQueryFlag Defining *****************/


/************ The GetVarValue Defining *************************/
// 功能描述：获得一个变量的实际取值，重载基类的虚函数
//
// 输入参数：
//    szInput：输入数据，例如一张话单
//  szVarName：变量名
//
// 输出参数：无
//
//   返回值：返回计算出的变量值，内存在本函数内分配，由本函数的调用者负责释放
//
/***************************************************************/
//对应问题单D17165 2002.9.26
#define STARTTIME_LENGTH  19
//修改
int CCondition::GetVarValue(const void* szInput, const char* szVarName, SVariant& stVar)
{
    //确保输入的参数正确
    if(szInput == NULL)
    {
        return -1;
    }
	
    if(szVarName == NULL)
	{
		return -1;
	}
	
    UINT2 nFieldIndex = 0;
    memcpy(&nFieldIndex, szVarName, sizeof(UINT2));
    UINT4        uLen = 0;      //计算域值时的结果长度
    SFieldInfo*  pFIF = NULL;   //当前变量对应的域描述指针
	SFieldInfo**   pFieldsArray;

    //获得话单域的值、长度
    m_pFmtOwning->GetFieldContentByIndex(szInput, m_bThisFmt, nFieldIndex, 
                                         (void *)(stVar.szValue), uLen, m_nIsQuery);
    if(uLen == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_GET_VALUE_BY_IDX_FAIL, 
              nFieldIndex, m_pFmtOwning->GetFmtName());
        return -1;
    }

    if (uLen > 63)
    {
        TRACE(MTS_FORMAT, S_FMT_VAL_LEN_EXCEED_LIMIT_GET_BY_IDX, 
              nFieldIndex, m_pFmtOwning->GetFmtName(), 
              uLen, 63);
        return -1;  //如果获取域值失败
    }

    //by lx SWPD04662
    //2002.9.26 by ldf 对应问题单D17165
    ////2002.11.19 by ldf 对应问题单 D18928 
    /*if(uLen > STARTTIME_LENGTH)
    {
        (stVar.szValue)[STARTTIME_LENGTH] = 0;
    }*/
    //change end

    //获得对格式描述的引用，并获取当前变量对应的格式描述映射中的节点    
	pFieldsArray = (SFieldInfo**)m_pFmtOwning->GetFieldsArray();
	pFIF = pFieldsArray[nFieldIndex];

    //如果当前变量是话单格式中的一个域
    if(NULL != pFIF)
    {
        //如果当前域类型是字符型
        if(pFIF->nDataType == FT_CHAR)
        {
            stVar.yVariantType = FT_CHAR;
			BYTE yValue = stVar.szValue[0];
            memcpy(&stVar.yValue, (void *)&yValue, 1);
        }

        //如果当前域是BCD码、反向BCD码、字符串类型，
        //为了表达式计算方便，BCD码、反向BCD码、字符串类型都以字符串类型表示
        else if((pFIF->nDataType == FT_BCD) || (pFIF->nDataType == FT_RBCD)
                || (pFIF->nDataType == FT_STRING)) 
        {
            stVar.yVariantType = FT_STRING;
            stVar.szValue[uLen] = '\0';
        }

        //如果当前域是整型（暂时不考虑INT8类型），
        //为了表达式计算方便，所有整型都以INT4型表示
        else if((pFIF->nDataType == FT_BYTE) || (pFIF->nDataType == FT_SHORT)
                || (pFIF->nDataType == FT_USHORT) || (pFIF->nDataType == FT_LONG)
                || (pFIF->nDataType == FT_ULONG))
        {
            
            stVar.yVariantType = FT_ULONG;        
            if(uLen == LEN_BYTE)
            {
                BYTE nTmp = 0;
                memcpy(&nTmp, stVar.szValue, LEN_BYTE);
                stVar.u4Value = nTmp;
            }
            else if(uLen == LEN_INT2)
            {
                INT2 nTmp = 0;
                memcpy(&nTmp, stVar.szValue, LEN_INT2);
                stVar.u4Value = nTmp;
            }
            else if(uLen == LEN_INT4)
            {
                INT4 nTmp = 0;
                memcpy(&nTmp, stVar.szValue, LEN_INT4);
                stVar.u4Value = nTmp;
            }            
            else
            {
                TRACE(MTS_FORMAT, S_FMT_VAL_LEN_EXCEED_INT_GET_BY_IDX, 
                      nFieldIndex, m_pFmtOwning->GetFmtName(), uLen);
                return -1;
            }
        }

        //非法类型
        else
        {
            TRACE(MTS_FORMAT, S_FMT_UNKONWN_FIELD_TYPE, 
                  m_pFmtOwning->GetFmtName(), nFieldIndex);
            return -1;
        }
    }

    //如果话单格式中不存在当前变量域
    else
    {
        return -1;
    }

    //返回计算结果
    return 0;
}

/************ End of The GetVarValue Defining ******************/

/************ The GetFunReturn Defining ************************/
// 功能描述：计算一个函数的输出值，重载基类的虚函数
//
// 输入参数：
//    szInput：输入数据，例如一张话单
//      szFun：函数字符串
//
// 输出参数：无
//
//   返回值：返回计算出的变量值，内存在本函数内分配，由本函数的调用者负责释放
//
/***************************************************************/
int CCondition::GetFunReturn(const void* szInput, const char* szFun, SVariant& stVar)
{
    STRING  sLib;                 //库
    STRING  sFun;                 //函数名
    STRING  sDefault;             //缺省值
    LIST<STRING>  ParaList;       //参数列表
    LIST<STRING>::iterator item;  //参数列表迭代子

    void*  pParas[MAX_PARA_NUM];      //条件函数参数数组
	UINT4  nParasLen[MAX_PARA_NUM];   //条件函数各参数的长度
    EPostfixEleType eParaType;        //条件函数各参数类型的临时变量

	UINT4  uParasNum  = 0;            //条件函数参数的个数
    char*  pTmpStr   = NULL;          //字符串临时变量 

    CONDITIONFUN pCondFun = NULL;     //条件函数指针
    

    //确保输入的参数正确
    if((szInput == NULL) || (szFun == NULL))
    {
        return -1;
    }
    
    //条件函数参数数组
    for(int n = 0; n < MAX_PARA_NUM; n++)
    {
        pParas[n] = NULL;
    }

    //解析函数字符串为库名、函数名、缺省值、参数列表
    if((CConvertFun::ConvertAnalysis(szFun, sLib, sFun, sDefault, ParaList)) != 0)
    {
        TRACE(MTS_FORMAT, S_FMT_FUN_CAN_NOT_ALALYSIS, 
              szFun, m_pFmtOwning->GetFmtName());
        return -1;    //解析函数字符串出错
    }

    //如果函数名为空
    else if(sFun.length() == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_FUN_CAN_NOT_ALALYSIS, 
              szFun, m_pFmtOwning->GetFmtName());
        return -1;
    }
    uParasNum = ParaList.size();
    
    //获取条件函数处理指针
    pCondFun = CConvertFun::FindConFun(sFun);
    if(pCondFun == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_FUN_NOT_FOUND, 
              szFun, m_pFmtOwning->GetFmtName());
        return -1;
    }

    //分别获取每一个条件函数参数的值
    int i = 0;
    item = ParaList.begin();
    while(item != ParaList.end())
    {
        eParaType = GetDataType((*item).c_str());

        //如果当前参数是变量类型
        if(eParaType == ET_VARIANT)
        {
            //获取当前参数变量对应域的值和长度
            if((m_pFmtOwning->GetFieldValueByName(szInput, m_bThisFmt, *item,
               pParas[i], nParasLen[i], m_nIsQuery)) != 0)
            {
                //释放已经分配的参数的空间
                while(i >= 0)
                {
                    if(pParas[i] != NULL)
                    {
                        delete[] (BYTE*)pParas[i];
                        pParas[i] = NULL;
                    }

                    --i;
                }

                TRACE(MTS_FORMAT, S_FMT_FUN_GET_VALUE_BY_NAME_FAIL, 
                      szFun, (*item).c_str(), m_pFmtOwning->GetFmtName());
                return -1;  //如果获取域值出错
            }
        }

        //如果当前参数是整形常量
        else if(eParaType == ET_INT_CONST)
        {
            pParas[i] = new BYTE[LEN_INT4];            
            //*(UINT4*)pParas[i] = atol(item->c_str());
            INT4 nTmp = atol(item->c_str());
            memcpy(pParas[i], (void*)&nTmp, LEN_INT4);
            nParasLen[i] = LEN_INT4;
        }

        //如果当前参数是字符型常量
        else if(eParaType == ET_UNSIGNED_CHAR_CONST)
        {
            pParas[i] = new BYTE[LEN_BYTE];

            //如果字符型常量中包含单引号，如'a'
            if(item->length() == 3)
            {
                *(char*)pParas[i] = (*item)[1];
            }

            //如果字符型常量中不包含单引号
            else
            {
                *(char*)pParas[i] = (*item)[0];
            }            
            nParasLen[i] = LEN_BYTE;
        }

        //如果当前参数是字符串常量
        else if(eParaType == ET_STRING_CONST)
        {
            //如果字符串常量中包含'@'或双引号，'@'代表字符串两边的双引号
            if(((*item)[0] == '@') || ((*item)[0] == '\"'))
            {
                pParas[i] = new BYTE[item->length() - 1];
                nParasLen[i] = item->length() - 1;

                //pTmpStr指到参数字符串中'@'或双引号的下一个位置
                pTmpStr = (char*)item->c_str();
                pTmpStr++;
                memcpy(pParas[i], (void*)pTmpStr, item->length() - 2);

                //给参数字符串最后添加结束符
                pTmpStr = (char*)pParas[i];
                pTmpStr += item->length() - 2;
                *pTmpStr = '\0';
            }

            //如果字符串常量中不包含'@'或双引号
            else
            {
                pParas[i] = new BYTE[item->length() + 1];
                strcpy((char*)pParas[i], item->c_str());
                nParasLen[i] = item->length() + 1;
            }            
        }

        //参数是非法类型
        else
        {
            //释放已经分配的参数的空间
            while(i >= 0)
            {
                if(pParas[i] != NULL)
                {
                    delete[] (BYTE*)pParas[i];
                    pParas[i] = NULL;
                }

                --i;
            }

            TRACE(MTS_FORMAT, S_FMT_INVALID_PARA_TYPE, 
                  szFun, (*item).c_str(), m_pFmtOwning->GetFmtName());
            return NULL;
        }

        i++;
        item++;
    }

    //根据参数获取条件函数的处理结果
    int nRet = pCondFun(uParasNum, (const void**)pParas, nParasLen, sDefault, stVar);

    //释放条件函数各个参数的空间
    for(int k = 0; k < MAX_PARA_NUM; k++)
    {
        if(pParas[k] != NULL)
        {
            delete[] (BYTE*)pParas[k];
            pParas[k] = NULL;
        }
    }

    return nRet;
}
/************ End of The GetFunReturn Defining *****************/

/************ The Infix2Postfix Defining ************************/
// 功能描述：将中缀表达式转化为后缀表达式，重载基类的函数
//
// 输入参数：
//   无
//
// 输出参数：无
//
//   返回值：如果出错返回-1，否则返回0
//
/***************************************************************/
int CCondition::Infix2Postfix(const char* szInfix)
{
	//生成后缀表达式
    int nResult = CPostfixExp::Infix2Postfix(szInfix);
    if (ERR_SUCCESS != nResult)
    {
        return -1;
    }
    
    
    PostfixExpEleList::iterator itemPostExp;  
    FIELD_MAP::iterator itemFieldMap;
    SFieldInfo*  pFI   = NULL;
    FIELD_MAP* fieldmap = (FIELD_MAP*)m_pFmtOwning->GetFieldsMap();
    
    //将变量类型的表达式元素转换成域序号存放
    itemPostExp = m_PostfixExp.begin();
    while(itemPostExp != m_PostfixExp.end())
    {
        if(itemPostExp->yEleType == ET_VARIANT)
        {
            itemFieldMap = fieldmap->find(itemPostExp->szEleContent);
            if(itemFieldMap == fieldmap->end())
            {
                TRACE(MTS_FORMAT, S_FMT_EXPRESS_FIELD_PARA_NOT_FOUND, 
                    szInfix, itemPostExp->szEleContent, m_pFmtOwning->GetFmtName());
                return -1;
            }
            
            pFI = itemFieldMap->second;
            itemPostExp->yEleType = 0;
            memcpy(itemPostExp->szEleContent, &(pFI->nFieldIndex), sizeof(UINT2));
        }		
        
        itemPostExp++;
    }
    
    return 0;
}
/************ End of The Infix2Postfix Defining *****************/


//获得可以用于统计的域列表
int CCondition::ParseExpress(const char* const  szExpress, LIST<STRING>& FieldList)
{
	if (Infix2Postfix(szExpress) != 0)
	{
		return -1;
	}

	PostfixExpEleList::iterator item;   //后缀表达式迭代子
	//顺序获取后缀表达式的每一个元素
    item = m_PostfixExp.begin();
    while(item != m_PostfixExp.end())
    {
        //如果当前元素是变量类型
        if(item->yEleType == ET_VARIANT)
        {
            UINT2 nFieldIndex = 0;
            memcpy(&nFieldIndex, item->szEleContent,sizeof(UINT2));
	        const SFieldInfo** fields_array = m_pFmtOwning->GetFieldsArray();
            if(NULL != fields_array && nFieldIndex < m_pFmtOwning->GetFieldNum())
            {
	            const SFieldInfo* pFI = fields_array[nFieldIndex];
			    FieldList.push_back(pFI->szFieldName);
            }
		}
		item++;
	}
	return 0;
}
