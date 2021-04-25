#include "../include/toolbox.h"
#include "../include/frame.h"
#include "../utility/igwb_file_creator.h"
#include "cdr_stat_file.h"


/*******************************************************************
函数名称：CCDRStatFile()
功能描述：构造函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
CCDRStatFile::CCDRStatFile()
{
    m_pIGWBFile = NULL;
}
/*******************************************************************
函数名称：~CCDRStatFile()
功能描述：析构函数
输入参数：无
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
CCDRStatFile::~CCDRStatFile()
{
    if (NULL != m_pIGWBFile)
    {
        delete m_pIGWBFile;
        m_pIGWBFile = NULL;
    }
}

/*******************************************************************
函数名称：Init(UINT4 uAPType,  UINT4 uBillType,  BOOL  bMustMerge,
               UINT4 uBillLen, int nAPID)
功能描述：设置参数，进行初始化操作
输入参数：
输出参数：无
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatFile::Init(UINT4 uAPType,
                       UINT4 uBillType,
                       BOOL  bMustMerge,
                       BOOL  bFinalSPFileFmt,
                       UINT4 uBillLen,
                       int   nAPID)
{
    if(NULL == m_pIGWBFile)
    {
        m_pIGWBFile = CIGWBFileCreator::CreateIGWBFile(uBillType,
                                                       uAPType,
                                                       uBillLen,
                                                       nAPID);
    }

    m_pIGWBFile->SetMustMerge(bMustMerge);
    m_pIGWBFile->SetFinalSPFileFmt(bFinalSPFileFmt);

    return ERR_SUCCESS;
}


/*******************************************************************
函数名称：Open(const char* const szPathFile)
功能描述：打开指定的话单文件
输入参数：const char* const szPathFile,文件路径
输出参数：无
返回内容：读成功返回TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatFile::Open(const char* const szPathFile)
{
    return m_pIGWBFile->Open(szPathFile, 0);
}

/*******************************************************************
函数名称：Close()
功能描述：关闭文件函数
输入参数：无
输出参数：无
返回内容：读成功返回TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatFile::Close()
{
    if (NULL != m_pIGWBFile)
    {
        m_pIGWBFile->Close();
        return TRUE;
    }

    return FALSE;
}

/*******************************************************************
函数名称：获取下一张话单记录数据
功能描述：GetNextBill(BYTE*& pCDRRecord, UINT4& nCDRLen, BOOL&  bRemain)
输入参数：无
输出参数：BYTE*& pCDRRecord, 话单数据
          UINT4& nCDRLen, 数据长度
          BOOL&  bRemain,是否还有话单
返回内容：读成功返回TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatFile::GetNextBill(BYTE*& pCDRRecord, UINT4& nCDRLen, BOOL&  bRemain)
{
    return m_pIGWBFile->Read((void*&)pCDRRecord, nCDRLen, bRemain);
}

