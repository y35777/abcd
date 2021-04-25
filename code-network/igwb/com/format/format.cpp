/**************** The Head Comment of This File *********************/
// Description:  The Implementing File of the CFormat
//      Author:  XieDaibin
//        Date:  2001.11.10
/**************** End of This File's Head Comment *******************/
/****** For PC-LINT **********
#define _DLL
#define _MT
#define _PLATFORM_WIN32
/****** End For PC-LINT ******/

#include "convert.h"
#include "condition.h"
#include "format_global.h"
#include "../include/toolbox.h"
#include "../include/export_dll.h"
#include "format.h"

extern char szFormatCfgErr[2048];

/************ The CFormat Defining *****************************/
// 功能描述：构造函数
//
// 输入参数：
//   sRootDir：格式配置库根路径
//   sFmtName：格式名
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CFormat::CFormat(const STRING& sRootDir, const STRING& sFmtName):
                 m_sFmtRootDir(sRootDir), m_sFmtName(sFmtName)
{
    //确保参数正确
    assert(m_sFmtRootDir.length() != 0);
    assert(m_sFmtName.length() != 0);


    //根据参数初始化相应成员变量
    //m_sFmtRootDir = sRootDir;
    //m_sFmtName = sFmtName;

    //初始化其它指针成员变量和整数成员变量
    m_pFieldsMap = NULL;
    m_pDistingCondition = NULL;
    m_pSrcFmt = NULL;
    m_pConvert = NULL;
    m_uFmtLen = 0;
    m_uFieldNum = 0;
    m_pFieldsArray = NULL;
    m_pQueryCondition = new CCondition(this);

    //设置条件对象的成员标志为非零，表示条件中的域需要转换成目标格式
    m_pQueryCondition->SetFlag(-1);
    m_pQueryCondition->SetQueryFlag(1);  //设置查询标志

    m_pStatCondition = new CCondition(this);

    //设置条件对象的成员标志为非零，表示条件中的域需要转换成目标格式
    m_pStatCondition->SetFlag(-1);
    m_pStatCondition->SetQueryFlag(1);  //设置查询标志
}
/************ End of The CFormat Defining **********************/


/************ The ~CFormat Defining ****************************/
// 功能描述：析构函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
CFormat::~CFormat()
{
    SFieldInfo*  pFI = NULL;     //话单域描述指针


    //删除作为识别条件的后缀表达式对象
    if(m_pDistingCondition != NULL)
    {
        delete m_pDistingCondition;
        m_pDistingCondition = NULL;
    }

    //删除作为查询条件的后缀表达式对象
    if(m_pQueryCondition != NULL)
    {
        delete m_pQueryCondition;
        m_pQueryCondition = NULL;
    }

    //删除格式转换对象
    if(m_pConvert != NULL)
    {
        delete m_pConvert;
        m_pConvert = NULL;
    }

    //删除话单域描述信息
    if(m_pFieldsArray != NULL)
    {
        for(int i = 0; i < m_uFieldNum; i++)
        {
            pFI = m_pFieldsArray[i];
            if(pFI != NULL)
            {
                if(pFI->pEnumInfos != NULL)
                {
                    delete pFI->pEnumInfos;
                }
                delete pFI;
            }
        }

        //删除话单域描述数组
        delete[] m_pFieldsArray;
        m_pFieldsArray = NULL;
    }

    //删除以话单域名称为索引MAP
    if(m_pFieldsMap != NULL)
    {
        delete m_pFieldsMap;
        m_pFieldsMap = NULL;
    }

    if(m_pStatCondition != NULL)
    {
        delete m_pStatCondition;
        m_pStatCondition = NULL;
    }
}
/************ End of The ~CFormat Defining *********************/

void CFormat::SetMustMerge(BOOL bMustMerge)
{
     m_bIsMustMerge = bMustMerge;	
}

/************ The Init Defining ********************************/
// 功能描述：初始化函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::Init(void)
{
    //载入格式的配置信息
    if(LoadConfig() != 0)
    {
        SNPRINTF(szFormatCfgErr,
                 sizeof(szFormatCfgErr),
                 S_FMT_LOAD_FMT_CONFIG_FAIL,
                 m_sFmtName.c_str());
        TRACE(MTS_FORMAT, szFormatCfgErr);

        return -1;
    }

    return 0;
}
/************ End of The Init Defining *************************/


/************ The LoadConfig Defining **************************/
// 功能描述：将格式配置信息读入内存函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::LoadConfig(void)
{
    STRING  sFmtFile;            //存放配置文件全路径
    char    szBuff[255];         //读取配置文件信息的缓冲区
    char*   pTmpStr  = szBuff;   //指向缓冲区的指针
    FILE*   pFmtFile = NULL;     //配置文件指针
    SFieldInfo*  pFI = NULL;     //话单域描述指针
    UINT2   uTmpNum = 0;         //读取两个字节的整数临时变量
    SEnumInfo  EIF;              //话单域枚举值结构变量


    //获取配置文件的全路径
    sFmtFile = m_sFmtRootDir + "/" + m_sFmtName + ".fmt";

    //以只读方式打开文件名为sFmtFile的格式配置文件
    pFmtFile = fopen(sFmtFile.c_str(), "rb");
    if(pFmtFile == NULL)
    {
        TRACE(MTS_FORMAT, S_FMT_OPEN_FILE_FAIL, 
              sFmtFile.c_str());
        return -1;
    }

    //读取格式描述
    if((fread((void*)pTmpStr, LEN_FMT_DESC, 1, pFmtFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sFmtFile.c_str());
        fclose(pFmtFile);
        return -1;
    }
    else
    {
        *(pTmpStr + LEN_FMT_DESC) = '\0';
        m_sFmtDesc = pTmpStr;
    }

    //读取源格式名
    if((fread((void*)pTmpStr, LEN_FMT_SRC_FMT_NAME, 1, pFmtFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
            sFmtFile.c_str());
        fclose(pFmtFile);
        return -1;
    }
    else
    {
        *(pTmpStr + LEN_FMT_SRC_FMT_NAME) = '\0';
        m_sSrcFmtName = pTmpStr;
    }

    //读取格式识别条件
    if((fread((void*)pTmpStr, LEN_FMT_DIS_CONDITION, 1, pFmtFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sFmtFile.c_str());
        fclose(pFmtFile);
        return -1;
    }
    else
    {
        *(pTmpStr + LEN_FMT_DIS_CONDITION) = '\0';
        if(strlen(pTmpStr) != 0)
        {
            m_sDistingCondition = pTmpStr;
        }
    }

    //邱意平 2002年4月4日修改如下代码为了处理格式配置文件中的字节序和字节对齐问题
    //读出格式配置文件中话单域个数指示
    if((fread((void*)&uTmpNum, 2, 1, pFmtFile)) != 1)
    {
        TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
              sFmtFile.c_str());
        fclose(pFmtFile);
        return -1;
    }
    uTmpNum = NTOHS(uTmpNum);   //处理字节序问题

    //初始化相应成员变量
    m_uFieldNum = uTmpNum;
    m_pFieldsArray = new SFieldInfo*[m_uFieldNum];
    m_pFieldsMap = new FIELD_MAP;

    //初始化话单域描述数组的每一项为空，以便于析构函数里能够正确删除
    for(int k = 0; k < m_uFieldNum; k++)
    {
        m_pFieldsArray[k] = NULL;
    }

    BYTE pFIBuf[SIZEOF_FIELDINFO];
    BYTE pEIBuf[SIZEOF_ENUMINFO];
    BYTE* pIn = NULL;

    //读取话单格式的每一个域描述
    for(int i = 0; i < m_uFieldNum; i++)
    {
        //创建一个话单域描述节点
        pFI = new SFieldInfo;
        pFI->pEnumInfos = NULL;
        m_pFieldsArray[i] = pFI;

        //读出一张话单域描述SFieldInfo到pFI
        if ((fread(pFIBuf, SIZEOF_FIELDINFO, 1, pFmtFile)) != 1)
        {
            TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                  sFmtFile.c_str());
            fclose(pFmtFile);
            return -1;
        }

        pIn = pFIBuf;
        SerialFieldInfo(pFI, pIn);

        pFI->pEnumInfos = NULL;

        //如果该域是枚举型
        if(pFI->flgEnum != 0)
        {
            //创建枚举信息结构链表
            pFI->pEnumInfos = new ENUMINFO_LIST;

            //读出当前域的枚举信息个数
            if ((fread((void*)&uTmpNum, 2, 1, pFmtFile)) != 1)
            {
                TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                      sFmtFile.c_str());
                fclose(pFmtFile);
                return -1;
            }

            uTmpNum = NTOHS(uTmpNum);  //处理字节序问题

            //读取当前域的所有枚举信息
            for(int j = 0; j < uTmpNum; j++)
            {
                //读出一个枚举信息描述
                if ((fread(pEIBuf, SIZEOF_ENUMINFO, 1, pFmtFile)) != 1)
                {
                    TRACE(MTS_FORMAT, S_FMT_READ_FILE_FAIL, 
                          sFmtFile.c_str());
                    fclose(pFmtFile);
                    return -1;
                }

                pIn = pEIBuf;
                SerialEnumInfo(&EIF, pIn);

                //将读取的枚举信息加入到当前域的枚举链表中
                pFI->pEnumInfos->push_back(EIF);
            }
        }

        //把当前域描述加入到以话单域名称为索引MAP
        m_pFieldsMap->insert(FIELD_MAP::value_type(pFI->szFieldName, pFI));
    }
    //修改完毕 邱意平2002年4月4日

	if(!m_sDistingCondition.empty())
	{
		m_pDistingCondition = new CCondition(this, m_sDistingCondition);
	}

    //关闭打开的格式配置文件
    fclose(pFmtFile);

    //初始化话单域，计算话单域位偏移、字节偏移、话单长度
    if((InitFormatInfo()) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The LoadConfig Defining *******************/


/************ The InitFormatInfo Defining **********************/
// 功能描述：计算一个格式中所有域的字节偏移、位偏移以及字节总长度函数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::InitFormatInfo(void)
{
    UINT4  nByteOffset = 0;      //本话单格式以字节计算的长度计数器
    UINT4  nBitOffset  = 0;      //本话单格式以位计算的长度计数器
    SFieldInfo*  pFI   = NULL;   //当前域描述指针

    UINT4  nBitNum     = NULL;   //相连位域以位计算的长度计数器
    UINT4  nTotalBit   = NULL;   //相连位域以位计算的的总长度


    //确保本格式域个数大于零
    if(m_uFieldNum == 0)
    {
        TRACE(MTS_FORMAT, S_FMT_FIELD_NUM_EMPTY, 
              m_sFmtName.c_str());
        return -1;
    }

    m_uFmtLen = 0;

    //初始化本格式中的每一个域的字节偏移量和位偏移量
    for(int i = 0; i < m_uFieldNum; i++)
    {
        //设置当前域的字节偏移量和位偏移量
        pFI = m_pFieldsArray[i];
        pFI->nByteOffset = nByteOffset;
        pFI->nBitOffset = nBitOffset;

        //如果当前话单域不是位域型
        if(pFI->flgBit == 0)
        {
            //计算下一个域的字节偏移量和位偏移量
            nByteOffset += GetFieldTypeLen(pFI) * pFI->nFieldLength;
            nBitOffset += GetFieldTypeLen(pFI) * pFI->nFieldLength * 8;
        }

        //如果该话单域是位域型
        else
        {
            //设置下一个域的位偏移量
            nBitOffset  += pFI->nFieldLength;  //对于位域型，直接是该位域的长度

            //计算下一个位域的位偏移量和相连位域的总位长度
            nBitNum = pFI->nFieldLength;
            nTotalBit = GetFieldTypeLen(pFI) * 8;

            //计算相连位域的位偏移量
            while((nBitNum < nTotalBit) && i < m_uFieldNum)
            {
                pFI = m_pFieldsArray[++i];

                //如果当前域不是位域
                if(pFI->flgBit == 0)
                {
                    TRACE(MTS_FORMAT, S_FMT_NOT_SEQUENTIAL_BITFLD, 
                          pFI->szFieldName, m_sFmtName.c_str());
                    return -1;
                }
                nBitNum += pFI->nFieldLength;

                //如果位域长度计数器值大于相连位域的总位长度
                if(nBitNum > nTotalBit)
                {
                    TRACE(MTS_FORMAT, S_FMT_BITFLD_LEN_OVERFLOW, 
                          pFI->szFieldName, m_sFmtName.c_str());
                    return -1;
                }

                //设置当前域的字节偏移量和位偏移量
                pFI->nByteOffset = nByteOffset;
                pFI->nBitOffset = nBitOffset;

                //设置下一个域的位偏移量
                nBitOffset += pFI->nFieldLength;
            }

            //设置下一个域的字节偏移量
            nByteOffset += GetFieldTypeLen(pFI);
        }
    }

    //设置本格式的总字节长度
    m_uFmtLen = nByteOffset;

    return 0;
}
/************ End of The InitFormatInfo Defining ***************/


/************ The CaculateFmtLen Defining **********************/
// 功能描述：计算本格式字节的长度（该函数目前用不到）
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回本格式的字节总长度，失败返回-1
//
/***************************************************************/
UINT4 CFormat::CaculateFmtLen(void)
{
    SFieldInfo*  pFI   = NULL;   //当前域描述指针
    UINT4  uFmtLen = 0;          //本格式长度计数器


    //遍历本格式中的所有域，计算以位表示的本格式总长度
    for(int i = 0; i < m_uFieldNum; i++)
    {
        pFI = m_pFieldsArray[i];

        //如果该话单域不是位域型
        if(pFI->flgBit == 0)
        {
            uFmtLen += GetFieldTypeLen(pFI) * pFI->nFieldLength * 8;
        }

        //如果该话单域是位域型
        else
        {
            uFmtLen  += pFI->nFieldLength;  //对于位域型，直接是该位域的长度
        }
    }

    //确保计算出的总位长度是8的整数倍
    if((uFmtLen % 8) != 0)
    {
        TRACE(MTS_FORMAT, S_FMT_INVALID_FMT_LEN, 
              m_sFmtName.c_str());
         return -1;
    }

    //将总长度从BIT单位转换为BYTE单位
    uFmtLen /= 8;
    m_uFmtLen = uFmtLen;

    return m_uFmtLen;
}
/************ End of The CaculateFmtLen Defining ***************/


/************ The Create Defining ******************************/
// 功能描述：创建一种话单格式
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::Create(void)
{
    STRING  sFmtFile;           //存放配置文件全路径
    FILE*   pFmtFile   = NULL;  //配置文件指针
    int     nWriteLen  = 0;     //写文件数据的长度
    BYTE    pBuff[50];          //在写文件数据时长度不足的地方填写0的缓冲区
    char    szPath[MAX_PATH];   //配置文件全路径（不包括文件名）


    //获取配置文件的全路径
    sFmtFile = m_sFmtRootDir + "/" + m_sFmtName + ".fmt";
    strcpy(szPath, m_sFmtRootDir.c_str());

    //创建指定格式的配置文件
    pFmtFile = fopen(sFmtFile.c_str(), "wb");
    if(pFmtFile == NULL)
    {
        //如果第一次打开文件失败，则创建该文件的目录
        if(!CreateDeepDir(szPath))
        {
            return -1;  //创建文件目录失败
        }

        //再打开一次文件
        pFmtFile = fopen(sFmtFile.c_str(), "wb");
        if(pFmtFile == NULL)
        {
            return -1;  //第二次打开文件失败
        }
    }

    //将本格式描述写入配置文件
    memset((void*)pBuff, 0, 50);
    nWriteLen = m_sFmtDesc.length() + 1;
    if(nWriteLen > 48)
    {
        nWriteLen = 48;  //格式描述最大长度是48个字节
    }
    memcpy((void*)pBuff, (void*)m_sFmtDesc.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, 48, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //将原格式名写入配置文件
    memset((void*)pBuff, 0, 50);
    nWriteLen = m_sSrcFmtName.length() + 1;
    if(nWriteLen > 16)
    {
        nWriteLen = 16;  //原格式名最大长度是16个字节
    }
    memcpy((void*)pBuff, (void*)m_sSrcFmtName.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, 16, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //关闭格式配置文件
    fclose(pFmtFile);

    return 0;
}
/************ End of The Create Defining ***********************/


/************ The Delete Defining ******************************/
// 功能描述：删除一种话单格式
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::Delete(void)
{
    STRING  sFmtFile;    //存放配置文件全路径


    //获取配置文件的全路径
    sFmtFile = m_sFmtRootDir + "/" + m_sFmtName + ".fmt";

    //删除指定的配置文件
    if(remove(sFmtFile.c_str()) != 0)
    {
        return -1;
    }

    return 0;
}
/************ End of The Delete Defining ***********************/


/************ The GetFmtName Defining **************************/
// 功能描述：获得本格式名
//
// 输入参数：无
//
// 输出参数：
//   sFmtName：本格式的名称
//
//   返回值：无
//
/***************************************************************/
void CFormat::GetFmtName(STRING& sFmtName) const
{
    sFmtName = m_sFmtName;
}
/************ End of The GetFmtName Defining *******************/

const char* CFormat::GetFmtName() const
{
    return (m_sFmtName.c_str());
}


/************ The SetSrcFmt Defining ***************************/
// 功能描述：设置本格式对应的源格式对象
//
// 输入参数：
//    pSrcFmt：本格式对应的源格式对象，在CFormat内部不得释放该指针
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::SetSrcFmt(CFormat* pSrcFmt)
{
    m_pSrcFmt = pSrcFmt;

    //如果本格式不等于本格式对应的原格式
    if(m_sFmtName != m_sSrcFmtName)
    {
        //只有目标格式名不等于源格式名时，才需要格式转换对象
        if(m_pConvert != NULL)
        {
            delete m_pConvert;
            m_pConvert = NULL;
        }
        m_pConvert = new CConvert;
        int nRet = m_pConvert->Init(this, m_pSrcFmt);
        if(nRet != 0)
        {
            delete m_pConvert;
            m_pConvert = NULL;

            return -1;
        }
    }

    return 0;
}
/************ End of The SetSrcFmt Defining ********************/


/************ The GetSrcFmt Defining ***************************/
// 功能描述：获得本格式对应的源格式对象
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：本格式对应的源格式对象，本函数的调用者不得释放
//
/***************************************************************/
CFormat* CFormat::GetSrcFmt(void) const
{
    return m_pSrcFmt;
}
/************ End of The GetSrcFmt Defining ********************/


/************ The GetSrcFmtName Defining ***********************/
// 功能描述：获得本格式的源格式名
//
// 输入参数：无
//
// 输出参数：
//  sSrcFmtName：本格式的源格式名
//
//   返回值：无
//
/***************************************************************/
void CFormat::GetSrcFmtName(STRING& sSrcFmtName) const
{
    sSrcFmtName = m_sSrcFmtName;
}
/************ End of The GetSrcFmtName Defining ****************/

const char* CFormat::GetSrcFmtName() const
{
    return (m_sSrcFmtName.c_str());
}
/************ The GetFmtLen Defining ***************************/
// 功能描述：获得本格式对应的单张话单长度（字节数）
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：本格式对应的单张话单长度（字节数）
//
/***************************************************************/
UINT4 CFormat::GetFmtLen(void) const
{
    return m_uFmtLen;
}

UINT4 CFormat::GetSrcFmtLen(void) const
{
    if (m_pSrcFmt != NULL)
    {
        return m_pSrcFmt->GetFmtLen();
    }

    return 0;
}

/************ End of The GetFmtLen Defining ********************/


/************ The GetFieldNum Defining *************************/
// 功能描述：获得本格式域个数
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：本格式域个数
//
/***************************************************************/
UINT4 CFormat::GetFieldNum(void) const
{
    return m_uFieldNum;
}
/************ End of The GetFieldNum Defining ******************/


/************ The GetFieldsMap Defining ************************/
// 功能描述：获得本格式所有话单域MAP指针
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：本格式话单域MAP指针，调用者不得释放获得的FIELD_MAP*指针
//
/***************************************************************/
const FIELD_MAP* CFormat::GetFieldsMap(void) const
{
    return m_pFieldsMap;
}
/************ End of The GetFieldsMap Defining *****************/


/************ The GetFieldsArray Defining **********************/
// 功能描述：获得本格式所有话单域数组指针
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：本格式话单域数组指针，调用者不得释放获得的SFieldInfo*指针
//
/***************************************************************/
const SFieldInfo** CFormat::GetFieldsArray(void) const
{
    return (const SFieldInfo**)m_pFieldsArray;
}
/************ End of The GetFieldsArray Defining ***************/


/************ The GetFormatGen Defining ************************/
// 功能描述：获得本格式的格式描述和源格式名
//
// 输入参数：无
//
// 输出参数：
//    sFmtDesc：本格式的格式描述
// sSrcFmtName：本格式对应的源格式名
//
//   返回值：无
//
/***************************************************************/
void CFormat::GetFormatGen(STRING& sFmtDesc, STRING& sSrcFmtName) const
{
    sFmtDesc = m_sFmtDesc;
    sSrcFmtName = m_sSrcFmtName;
}
/************ End of The GetFormatGen Defining *****************/


/************ The SetFormatGen Defining ************************/
// 功能描述：设置本格式的格式描述和源格式名
//
// 输入参数：
//    sFmtDesc：本格式的格式描述
// sSrcFmtName：本格式对应的源格式名
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CFormat::SetFormatGen(const STRING& sFmtDesc,  const STRING& sSrcFmtName)
{
    m_sFmtDesc = sFmtDesc;
    m_sSrcFmtName = sSrcFmtName;
}
/************ End of The SetFormatGen Defining *****************/


/************ The SetDistingCondition Defining *****************/
// 功能描述：设置本格式的识别条件
//
// 输入参数：
//  sDistingCondition：格式识别条件
//
// 输出参数：无
//
//   返回值：无
//
/***************************************************************/
void CFormat::SetDistingCondition(const STRING& sDistingCondition)
{
    m_sDistingCondition = sDistingCondition;
}
/************ End of The SetDistingCondition Defining **********/


/************ The GetDistingCondition Defining *****************/
// 功能描述：获得本格式的识别条件
//
// 输入参数：无
//
// 输出参数：
//  sDistingCondition：格式识别条件
//
//   返回值：无
//
/***************************************************************/
void CFormat::GetDistingCondition(STRING&  sDistingCondition) const
{
    sDistingCondition = m_sDistingCondition;
}
/************ End of The GetDistingCondition Defining **********/


/************ The IsTheFormat Defining *************************/
// 功能描述：判断传入的一张话单是否符合本格式
//
// 输入参数：
//     pABill：传入的单张话单
//       uLen：一张话单的长度，即pABill所占内存大小
//
// 输出参数：无
//
//   返回值：符合本格式返回0，不符合本格式返回1，出错返回-1
//
/***************************************************************/
int CFormat::IsTheFormat(const void* pABill,  const UINT4 uLen)
{
    SVariant  Var;   //条件表达式计算结果变量

    if (m_pSrcFmt != this)
    {
        return m_pSrcFmt->IsTheFormat(pABill, uLen);
    }

    //不符合本格式
    if((pABill == NULL) || (m_uFmtLen != uLen))
    {
        return 1;
    }

    //计算识别条件表达式
    if(m_pDistingCondition->Caculate(pABill, uLen, Var) != 0)
    {
        //assert(0);
        return -1;
    }

    //不符合本格式
    if(Var.n4Value == 0)
    {
        return 1;
    }

    //符合本格式
    return 0;
}
/************ End of The IsTheFormat Defining ******************/


/************ The ConvertABill Defining ************************/
// 功能描述：将传入的一张话单转换成目标格式
//
// 输入参数：
//   pASrcBill：传入的单张话单
// uSrcBillLen：一张话单的长度，即pABill所占内存大小
//
// 输出参数：
//   pADestBill：转换后的话单，指向的内存由本函数分配，由本函数的调用者负责释放
// uDestBillLen：转换后的话单长度
//
//   返回值：正确转换返回0，失败返回-1
//
/***************************************************************/
int CFormat::ConvertABill(const void* pASrcBill, const UINT4 uSrcBillLen,
                          void*& pADestBill, UINT4& uDestBillLen)
{
    //初始化输出参数
    pADestBill = NULL;
    uDestBillLen  = 0;

    //确保输入参数的合法性
    if((pASrcBill == NULL) || (uSrcBillLen == 0))
    {
        return -1;
    }

    //如果目标格式等于原格式
    if(m_sFmtName == m_sSrcFmtName)
    {
        pADestBill = new BYTE[uSrcBillLen];
        uDestBillLen = uSrcBillLen;
        memcpy(pADestBill, pASrcBill, uSrcBillLen);
    }

    //目标格式不等于原格式
    else
    {
        //调用Convert对象把原格式转换成目标格式
        int nRet = m_pConvert->ConvertABill(pASrcBill, uSrcBillLen, pADestBill, uDestBillLen);
        if(nRet != 0)
        {
            return -1;
        }
    }

    return 0;
}
/************ End of The ConvertABill Defining *****************/


/************ The GetFieldValueByIndex Defining ****************/
// 功能描述：根据格式域索引从一张话单中获得该域的值
//
// 输入参数：
//     pABill：传入的单张话单
//   bThisFmt：传入的话单是否符合本格式，0表示符合本格式，
//             非零表示符合本格式的原格式
//     uIndex：格式域索引，与SFieldInfo结构中的nFieldIndex对应
//     pDestFI：目标格式的字段信息，为了能够实现二进制取枚举值的功能，缺省值为NULL
//
// 输出参数：
// pFieldValue：获得的域值
//        uLen：该域长度
//
//   返回值：成功返回0，失败返回-1
//
//修改记录：
// 2004-02-18  SWPD03631 增加参数pDestFI实现二进制取枚举值 ZhengYuqun
/***************************************************************/
int CFormat::GetFieldValueByIndex(const void* pABill, const int bThisFmt,
                                  const UINT4 uIndex,
                                  void* pFieldValue, UINT4& uLen, SFieldInfo* pDestFI)
{
    SFieldInfo*  pFI   = NULL;   //域描述指针
    BYTE* pTmpStr = NULL;        //指向话单内容的临时指针


    //初始化输出参数和变量
    //pFieldValue = NULL;
    uLen = 0;
    pTmpStr = (BYTE*)pABill;

    //不是本格式
    if(bThisFmt != 0)
    {
        //调用源格式
        int retVal = m_pSrcFmt->GetFieldValueByIndex(pABill, 0, uIndex, pFieldValue, uLen);
        return retVal;
    }

    //如果指定的域索引超过范围，或输入话单数据为空
    if((uIndex >= m_uFieldNum) || (pABill == NULL))
    {
        return -1;
    }

    pFI = m_pFieldsArray[uIndex];

    //该域不是位域型
    if(pFI->flgBit == 0)
    {
        uLen = GetFieldTypeLen(pFI) * pFI->nFieldLength;
        //pFieldValue = new BYTE[uLen];
        memcpy(pFieldValue, (void*)(pTmpStr + pFI->nByteOffset), uLen);
        //added by maimaoshi,2002-10-26
        ConvertToHostOrder(pFI, pFieldValue);
        //end
    }

    //该域是位域型
    else
    {
        uLen = GetFieldTypeLen(pFI);
        //pFieldValue = new BYTE[uLen];
        memcpy(pFieldValue, (void*)(pTmpStr + pFI->nByteOffset), uLen);

        //added by maimaoshi,2002-10-26
        ConvertToHostOrder(pFI, pFieldValue);
        //end
        //计算该位域应该左移位、右移位的位数
        //Modified by XDB on 2002.10.28
        //int nRShift = pFI->nBitOffset - (pFI->nByteOffset * 8);  //右移位数
        int nRShift = 0;  //右移位数
        if(pFI->nOrderMode == INTEL_ORDER)
        {
            nRShift = pFI->nBitOffset - (pFI->nByteOffset * 8);
        }
        else
        {
            nRShift = (uLen * 8) - (pFI->nBitOffset - (pFI->nByteOffset * 8))
                      - pFI->nFieldLength;
        }
        //End of Modify by XDB

        //该域是BYTE类型
        if(uLen == LEN_BYTE)
        {
            //右移nRShift位
            //BYTE nTmpNum = *(BYTE*)pFieldValue;
            BYTE nTmpNum = 0;
            memcpy(&nTmpNum, pFieldValue, LEN_BYTE);
            nTmpNum = nTmpNum >> nRShift;

            //只取低pFI->nFieldLength位
            nTmpNum &= ((1 << pFI->nFieldLength) -1);
            memcpy(pFieldValue, &nTmpNum, LEN_BYTE);
            //*(BYTE*)pFieldValue = nTmpNum;
        }

        //该域是UINT2或INT2类型
        else if(uLen == LEN_UINT2)
        {
            //右移nRShift位
            //UINT2 nTmpNum = *(UINT2*)pFieldValue;
            UINT2 nTmpNum = 0;
            memcpy(&nTmpNum, pFieldValue, LEN_UINT2);
            nTmpNum = nTmpNum >> nRShift;

            //只取低pFI->nFieldLength位
            nTmpNum &= ((1 << pFI->nFieldLength) -1);
            memcpy(pFieldValue, &nTmpNum, LEN_UINT2);
            //*(UINT2*)pFieldValue = nTmpNum;
        }

        //该域是UINT4或INT4类型
        else if(uLen == LEN_UINT4)
        {
            //右移nRShift位
            //UINT4 nTmpNum = *(UINT4*)pFieldValue;
            UINT4 nTmpNum = 0;
            memcpy(&nTmpNum, pFieldValue, LEN_UINT4);
            nTmpNum = nTmpNum >> nRShift;

            //只取低pFI->nFieldLength位
            nTmpNum &= ((1 << pFI->nFieldLength) -1);
            memcpy(pFieldValue, &nTmpNum, LEN_UINT4);
            //*(UINT4*)pFieldValue = nTmpNum;
        }

        //该域是非法枚举类型
        else
        {
            //delete[] (BYTE*)pFieldValue;
            //pFieldValue = NULL;
            uLen = 0;

            return -1;
        }
    }

    //Added by ZhengYuqun 2004-02-18 SWPD03631 二进制取枚举
    if(pDestFI)
    {
        ENUMINFO_LIST* pEnumInfos = pDestFI->pEnumInfos;
        if(pEnumInfos && pFI->nDataType != FT_CHAR && pFI->nDataType != FT_STRING)
        {
            if(uLen <= sizeof(INT4))
            {
                INT4 nEnumVal = 0;
                memcpy((BYTE*)&nEnumVal, pFieldValue, uLen);
                ENUMINFO_LIST::iterator item;
                //顺序搜索格式域枚举链表中的每一个节点
                item = pEnumInfos->begin();
                while(item != pEnumInfos->end())
                {
                    //如果当前节点的枚举值等于需要的枚举值
                    if(item->nEnumValue == nEnumVal)
                    {
                        //将当前枚举值对应的枚举字符串转换为数字
                        nEnumVal = atoi(item->szEnumValue);
                        memcpy(pFieldValue, (BYTE*)&nEnumVal, uLen);
                        break;
                    }
                    item++;
                }         
            }    
        }
    }
    //End SWPD03631

    return 0;
}
/************ End of The GetFieldValueByIndex Defining *********/


/************ The GetFieldValueByName Defining *****************/
// 功能描述：根据话单域名从一张话单中获得该域的值
//
// 输入参数：
//     pABill：传入的单张话单
//   bThisFmt：传入的话单是否符合本格式，0表示符合本格式，
//             非零则需要转换成目标格式
//     uIndex：格式域名，与SFieldInfo结构中的nFieldIndex对应
//   nIsQuery：是否查询标志，1表示该函数为查询条件转换，
//             0表示不是为查询条件转换
//
// 输出参数：
// pFieldValue：获得的域值
//        uLen：该域长度
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::GetFieldValueByName(const void* pABill, const int bThisFmt,
                                 const STRING& sFieldName,
                                 void*& pFieldValue, UINT4& uLen,
                                 int nIsQuery)
{
    SFieldInfo*  pFI   = NULL;   //域描述指针
    BYTE*  pTmpStr     = NULL;   //指向话单内容的临时指针
    FIELD_MAP::iterator item;    //指定域名在域描述映射中的节点


    //初始化输出参数和变量
    pFieldValue = NULL;
    uLen = 0;
    pTmpStr = (BYTE*)pABill;

    //确保输入的话单数据不为空
    if((pABill == NULL) || (sFieldName.length() == 0))
    {
        return -1;
    }

    //不是本格式,但如果格式转换对象为空，表示原格式等于目标格式
    if((bThisFmt != 0) && (m_pConvert != NULL))
    {
        //转换成目标格式
        int nRet = m_pConvert->ConvertAField(pABill, sFieldName, pFieldValue, uLen, nIsQuery);
        if(nRet != 0)
        {
            return -1;
        }

        return 0;
    }

    //在域描述映射关系根据域名查找指定的域节点
    item = m_pFieldsMap->find(sFieldName);
    if(item == m_pFieldsMap->end())
    {
        //本格式的域中不存在指定的域名
        return -1;
    }

    //根据该域对应的域索引获取其值
    pFI = item->second;
    uLen = GetFieldTypeLen(pFI) * pFI->nFieldLength;
    pFieldValue = new BYTE[uLen];
    if(GetFieldValueByIndex(pABill, 0, pFI->nFieldIndex, pFieldValue, uLen) != 0)
    {
        delete[] (BYTE*)pFieldValue;
        pFieldValue = NULL;
        uLen = 0;
        return -1;
    }

    return 0;
}
/************ End of The GetFieldValueByName Defining **********/

/***************************************************************/
// 功能描述：根据话单域索引从一张话单中获得该域的值
//
// 输入参数：
//     pABill：传入的单张话单
//   bThisFmt：传入的话单是否符合本格式，0表示符合本格式，
//             非零则需要转换成目标格式
//     uIndex：格式域名，与SFieldInfo结构中的nFieldIndex对应
//   nIsQuery：是否查询标志，1表示该函数为查询条件转换，
//             0表示不是为查询条件转换
//
// 输出参数：
// pFieldValue：获得的域值，由外部分配内存
//        uLen：该域长度
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::GetFieldContentByIndex(const void* pABill, const int bThisFmt,
					   			    UINT2 nFieldIndex,  
								    void* pFieldValue, UINT4& uLen,
								    int nIsQuery)
{
    SFieldInfo*  pFI   = NULL;   //域描述指针
    BYTE*  pTmpStr     = NULL;   //指向话单内容的临时指针	
	
    //初始化输出参数和变量
    uLen = 0;
    pTmpStr = (BYTE*)pABill;
	
    //确保输入的话单数据不为空
    if(pABill == NULL)
    {
        return -1;
    }
	
    //不是本格式,但如果格式转换对象为空，表示原格式等于目标格式
    if((bThisFmt != 0) && (m_pConvert != NULL))
    {
        //转换成目标格式
        int nRet = m_pConvert->ConvertAFieldByIndex(pABill, nFieldIndex,
                                                    pFieldValue, uLen, nIsQuery);
        if(nRet != 0)
        {
            return -1;
        }
		
        return 0;
    }
	
    //在域描述映射关系根据域序号查找指定的域节点
    pFI = m_pFieldsArray[nFieldIndex];
    if(NULL == pFI)
    {
        //本格式的域中不存在指定的域名
        return -1;
    }
	
    //根据该域对应的域索引获取其值 
    uLen = GetFieldTypeLen(pFI) * pFI->nFieldLength;
    if(GetFieldValueByIndex(pABill, 0, nFieldIndex, pFieldValue, uLen) != 0)
    {
        uLen = 0;
        return -1;
    }
	
    return 0;
}
/***************************************************************/


/************ The IsTheQueryCondition Defining *****************/
// 功能描述：判断一张话单是否符合指定的查询条件
//
// 输入参数：
//   pASrcBill：传入的单张话单
//  sQueryCond：查询条件，中缀表达式
//
// 输出参数：无
//
//   返回值：符合格式返回0，不符合返回1，出错返回-1
//
/***************************************************************/
int CFormat::IsTheQueryCondition(const void* pASrcBill,const STRING& sQueryCond)
{
    SVariant RetVar;    //条件表达式计算结果变量

    //确保输入参数的正确性
    if((pASrcBill == NULL)|| ((m_sQueryCond.length() == 0)
        && (sQueryCond.length() == 0)))
    {
        return -1;
    }

    //如果条件表达式参数不为空，将中缀表达式转换为后缀表达式
    if(sQueryCond.length() != 0)
    {
        if(m_pQueryCondition->Infix2Postfix(sQueryCond.c_str()) != 0)
        {
            return -1;
        }
        m_sQueryCond = sQueryCond;
    }

    //计算条件表达式
    if(m_pQueryCondition->Caculate(pASrcBill, m_uFmtLen, RetVar) != 0)
    {
        //assert(0);
        return -1;
    }

    //不符合条件
    if(RetVar.n4Value == 0)
    {
        return 1;
    }

    //符合条件
    else
    {
        return 0;
    }
}
/************ End of The IsTheQueryCondition Defining **********/


/************ The GetFieldTypeLen Defining **********************/
// 功能描述：获取话单域类型的长度
//
// 输入参数：
//   pFI：话单格式域描述
//
// 输出参数：无
//
//   返回值：成功返回话单域类型的长度，失败返回-1
//
/***************************************************************/
int CFormat::GetFieldTypeLen(const SFieldInfo* pFI)
{
    int nFieldTypeLen = 0;   //域类型长度变量

    //获取当前域类型的长度
    if(pFI->nDataType == FT_CHAR)
    {
        nFieldTypeLen = sizeof(char);
    }
    else if(pFI->nDataType == FT_BYTE)
    {
        nFieldTypeLen = sizeof(BYTE);
    }
    else if(pFI->nDataType == FT_SHORT)
    {
        nFieldTypeLen = sizeof(INT2);
    }
    else if(pFI->nDataType == FT_USHORT)
    {
        nFieldTypeLen = sizeof(UINT2);
    }
    else if(pFI->nDataType == FT_LONG)
    {
        nFieldTypeLen = sizeof(INT4);
    }
    else if(pFI->nDataType == FT_ULONG)
    {
        nFieldTypeLen = sizeof(UINT4);
    }
    else if(pFI->nDataType == FT_INT8)
    {
        nFieldTypeLen = sizeof(INT8);
    }
    else if((pFI->nDataType == FT_STRING) || (pFI->nDataType == FT_BCD)
            || (pFI->nDataType == FT_RBCD))
    {
        nFieldTypeLen = sizeof(BYTE);
    }

    //如果不是合法的域类型
    else
    {
        return -1;
    }

    return nFieldTypeLen;
}
/************ End of The GetFieldTypeLen Defining ***********/


/************ The Commit Defining ******************************/
// 功能描述：使上一次Commit到本次Commit调用之间所修改的配置信息
//           保存到磁盘中
//
// 输入参数：无
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::Commit(void)
{
    STRING  sFmtFile;           //存放配置文件全路径
    FILE*   pFmtFile   = NULL;  //配置文件指针
    int     nWriteLen  = 0;     //写文件数据的长度
    BYTE    pBuff[100];         //在写文件数据时长度不足的地方填写0的缓冲区
    UINT2   uTmpNum = 0;        //写两个字节长度的整数变量
    char    szPath[MAX_PATH];   //配置文件全路径（不包括文件名）


    //获取配置文件的全路径
    sFmtFile = m_sFmtRootDir + "/" + m_sFmtName + ".fmt";
    strcpy(szPath, m_sFmtRootDir.c_str());

    //打开指定格式的配置文件
    pFmtFile = fopen(sFmtFile.c_str(), "wb");
    if(pFmtFile == NULL)
    {
        //如果第一次打开文件失败，则创建该文件的目录
        if(!CreateDeepDir(szPath))
        {
            return -1;  //创建文件目录失败
        }

        //再打开一次文件
        pFmtFile = fopen(sFmtFile.c_str(), "wb");
        if(pFmtFile == NULL)
        {
            return -1;  //第二次打开文件失败
        }
    }

    //将本格式描述写入配置文件
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sFmtDesc.length() + 1;
    if(nWriteLen > LEN_FMT_DESC)
    {
        nWriteLen = LEN_FMT_DESC;  //格式描述最大长度是48个字节
    }
    memcpy((void*)pBuff, (void*)m_sFmtDesc.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_FMT_DESC, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //将原格式名写入配置文件
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sSrcFmtName.length() + 1;
    if(nWriteLen > LEN_FMT_SRC_FMT_NAME)
    {
        nWriteLen = LEN_FMT_SRC_FMT_NAME;  //原格式名最大长度是16个字节
    }
    memcpy((void*)pBuff, (void*)m_sSrcFmtName.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_FMT_SRC_FMT_NAME, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //将识别条件写入配置文件
    memset((void*)pBuff, 0, 100);
    nWriteLen = m_sDistingCondition.length() + 1;
    if(nWriteLen > LEN_FMT_DIS_CONDITION)
    {
        nWriteLen = LEN_FMT_DIS_CONDITION;  //识别条件最大长度是64个字节
    }
    memcpy((void*)pBuff, (void*)m_sDistingCondition.c_str(), nWriteLen);
    if(fwrite((void*)pBuff, LEN_FMT_DIS_CONDITION, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //将话单域总数写入配置文件
    uTmpNum = m_uFieldNum;  //话单域总数占用两个字节
    if(fwrite((void*)&uTmpNum, LEN_UINT2, 1, pFmtFile) != 1)
    {
        fclose(pFmtFile);
        return -1;
    }

    //将每一个话单域描述信息写入配置文件
    SFieldInfo* pFI = NULL;
    ENUMINFO_LIST::iterator it;
    SEnumInfo SNIF;
    for(int i = 0; i < m_uFieldNum; i++)
    {
        pFI = m_pFieldsArray[i];

        //保存当前域SFieldInfo结构信息
        if(fwrite((void*)pFI, sizeof(SFieldInfo), 1, pFmtFile) != 1)
        {
            fclose(pFmtFile);
            return -1;
        }

        //如果当前域是枚举类型，则保存该域的所有枚举值
        if((pFI->flgEnum != 0) && (pFI->pEnumInfos != NULL))
        {
            //将当前域的枚举个数写入配置文件
            uTmpNum = pFI->pEnumInfos->size();  //枚举个数占用两个字节
            if(fwrite((void*)&uTmpNum, LEN_UINT2, 1, pFmtFile) != 1)
            {
                fclose(pFmtFile);
                return -1;
            }

            it = pFI->pEnumInfos->begin();
            while(it != pFI->pEnumInfos->end())
            {
                //保存当前枚举值
                SNIF = *it;
                if(fwrite((void*)&SNIF, sizeof(SEnumInfo), 1, pFmtFile) != 1)
                {
                    fclose(pFmtFile);
                    return -1;
                }

                it++;
            }
        }
    }

    //关闭格式配置文件
    fclose(pFmtFile);

    return 0;
}
/************ End of The Commit Defining ***********************/


/************ The SetQueryCond Defining ************************/
// 功能描述：设置查询条件
//
// 输入参数：
//  sQueryCond：查询条件
//
// 输出参数：无
//
//   返回值：成功返回0，失败返回-1
//
/***************************************************************/
int CFormat::SetQueryCond(const STRING& sQueryCond)
{
    //确保输入参数的正确性
    if(sQueryCond.length() == 0)
    {
        return -1;
    }

    //将中缀表达式转换为后缀表达式
    if(m_pQueryCondition->Infix2Postfix(sQueryCond.c_str()) != 0)
    {
        //assert(0);
        return -1;
    }

    m_sQueryCond = sQueryCond;
    return 0;
}
/************ End of The SetQueryCond Defining *****************/

/***************************************************************************************************
 *函数功能：将网络字节序、1字节对齐的枚举信息pIn转换成SEnumInfo结构。
 *邱意平 2002年4月4日增加此函数
 *输入：
 *    pIn  ：从配置文件中读取的按网络字节序、1字节对齐保存的SEnumInfo信息。
 *
 *输出：
 *    pOut ： 转换后的与平台相关的SEnumInfo信息。
 *返回：
 *    无。
 ***************************************************************************************************/
void CFormat::SerialEnumInfo(SEnumInfo* pOut, BYTE* pIn)
{
    if ((pOut == NULL) || (pIn == NULL))
    {
        return;
    }

    memcpy(&(pOut->nEnumValue), pIn, 2);
    pIn += 2;

    memcpy(pOut->szEnumValue, pIn, 30);

    pOut->nEnumValue = NTOHS(pOut->nEnumValue);
}

/***************************************************************************************************
 *函数功能：将网络字节序、1字节对齐的枚举信息pIn转换成SFieldInfo结构。
 *邱意平 2002年4月4日增加此函数
 *输入：
 *    pIn  ：从配置文件中读取的按网络字节序、1字节对齐保存的SFieldInfo信息。
 *
 *输出：
 *    pOut ： 转换后的与平台相关的SFieldInfo信息。
 *返回：
 *    无。
 ***************************************************************************************************/
void CFormat::SerialFieldInfo(SFieldInfo* pOut, BYTE* pIn)
{
    if ((pOut == NULL) || (pIn == NULL))
    {
        return;
    }

    memcpy(&(pOut->nFieldIndex), pIn, 2);
    pIn += 2;

    memcpy(pOut->szFieldName, pIn, 64);
    pIn += 64;

    memcpy(pOut->szFieldDescribe, pIn, 64);
    pIn += 64;

    memcpy(&(pOut->nDataType), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->nQueryCondDataType), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->flgBit), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->nFieldLength), pIn, 2);
    pIn += 2;

    memcpy(&(pOut->flgEnum), pIn, 1);

    pIn += (1+4);

    memcpy(pOut->szConvertFun, pIn, 128);
    pIn += 128;

    memcpy(&(pOut->nAlignMode), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->cFillChar), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->cSeparator), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->nBitOffset), pIn, 4);
    pIn += 4;

    memcpy(&(pOut->nByteOffset), pIn, 4);
    pIn += 4;

    memcpy(&(pOut->flgQuery), pIn, 1);
    pIn += 1;

    memcpy(&(pOut->nOperator), pIn, 1);
    pIn += 1;

    memcpy(pOut->szUnit, pIn, 5);
    pIn += 5;

    memcpy(&(pOut->nOrderMode), pIn, 1);

    pOut->nFieldIndex   = NTOHS(pOut->nFieldIndex);
    pOut->nFieldLength  = NTOHS(pOut->nFieldLength);

    pOut->nBitOffset    = NTOHL(pOut->nBitOffset);
    pOut->nByteOffset   = NTOHL(pOut->nByteOffset);
}

//分析表达式，提取域名
int CFormat::ParseExpress(const char* const  szExpress, LIST<STRING>& FieldList)
{
    return m_pStatCondition->ParseExpress(szExpress, FieldList);
}

/******************************************************************************
 *函数功能：对字节序的修改，从INTEL或者到网络本机字节序
 *
 *输入：
 *    SFieldInfo *pFI  :SFieldInfo信息
 *    void *pFieldValue:转换前的值
 *
 *输出：
 *    void *pFieldValue:转换前的值
 *返回：
 *    无。
 *added by maimaoshi,2002-10-24
 *Modified by XDB on 2002.10.28
 ******************************************************************************/
int CFormat::ConvertToHostOrder(const SFieldInfo* pFI, void* pFieldValue)
{
    //需要加上对字节序的修改，从INTEL或者到网络本机字节序
    //INTEL_ORDER   0;表示格式为INTEL字节序
    //NET_ORDER     1;表示格式为网络字节序
    INT2  nShortValue = 0;
    INT4  nIntValue = 0;

    if((pFI == NULL) || (pFieldValue == NULL))
    {
        return -1;
    }

    if ((pFI->nDataType == FT_SHORT) || (pFI->nDataType == FT_USHORT))
    {
        //对于2字节的类型处理
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //原格式为INTEL字节序，需要把INTEL字节许转为本机字节序
            memcpy(&nShortValue, pFieldValue, LEN_INT2);
            nShortValue = ITOHS(nShortValue);
            memcpy(pFieldValue, &nShortValue, LEN_INT2);
        }
        else
        {
            //原格式为网络字节序，需要把网络字节许转为本机字节序
            memcpy(&nShortValue, pFieldValue, LEN_INT2);
            nShortValue = NTOHS(nShortValue);
            memcpy(pFieldValue, &nShortValue, LEN_INT2);
        }
    }

    else if ((pFI->nDataType == FT_LONG) || (pFI->nDataType == FT_ULONG))
    {
        //对于4字节的类型处理
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //原格式为INTEL字节序，需要把INTEL字节许转为本机字节序
            memcpy(&nIntValue, pFieldValue, LEN_INT4);
            nIntValue = ITOHL(nIntValue);
            memcpy(pFieldValue, &nIntValue, LEN_INT4);
        }
        else
        {
            //原格式为网络字节序，需要把网络字节许转为本机字节序
            memcpy(&nIntValue, pFieldValue, LEN_INT4);
            nIntValue = NTOHL(nIntValue);
            memcpy(pFieldValue, &nIntValue, LEN_INT4);
        }
    }

    else if (pFI->nDataType == FT_INT8)
    {
        INT8 nIntValue8;
        //对于8字节的类型处理
        if (pFI->nOrderMode == INTEL_ORDER)
        {
            //原格式为INTEL字节序，需要把INTEL字节许转为本机字节序
            memcpy(&nIntValue8, pFieldValue, LEN_INT8);
            nIntValue8 = ITOHLL(nIntValue8);
            memcpy(pFieldValue, &nIntValue8, LEN_INT8);
        }
        else
        {
            //原格式为网络字节序，需要把网络字节许转为本机字节序
            memcpy(&nIntValue8, pFieldValue, LEN_INT8);
            nIntValue8 = NTOHLL(nIntValue8);
            memcpy(pFieldValue, &nIntValue8, LEN_INT8);
        }
    }

    return 0;
}

