#include "../include/frame.h"
#include "scp_bill_file.h"
#include "scp_rvy_task.h"
#include "../querybrowse/qb_comm.h"

/********************************************************
* 名称: CScpBillFile::CScpBillFile
* 功能: 构造函数
* 输入: pFormat     : 格式库对象的指针
*       uBillType   : 原始话单还是最终话单
*       uAPType     : 接入点类型
*       pScpRvyTask : CScpRvyTask对象的指针
* 输出: 
* 返回: 
********************************************************/
CScpBillFile::CScpBillFile(CFormat *pFormat, 
                           UINT4 uBillType, 
                           UINT4 uAPType, 
                           CScpRvyTask* pScpRvyTask)
{
    m_pFormat       = pFormat;
    m_uBillType     = uBillType;
    m_uAPType       = uAPType;
    m_uBillLen      = pFormat->GetSrcFmtLen();

    m_nMemFileLen   = 4*1024*1024;
    m_pMemFile      = new BYTE[m_nMemFileLen];
    m_nMemDataLen   = 0;
    m_pFileStream   = NULL;
    ASSERT(pScpRvyTask != NULL);

    m_pScpRvyTask = pScpRvyTask;
}


/********************************************************
* 名称: CScpBillFile::~CScpBillFile
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CScpBillFile::~CScpBillFile()
{
    m_pScpRvyTask = NULL;
    if (NULL != m_pFileStream)
    {
        fclose(m_pFileStream);
    }

    //进行与ASN1解码相关的成员变量的释放
    if (NULL != m_pMemFile)
    {
        delete[] m_pMemFile;
    }
}

/**********************************************************************
功能描述    打开指定的话单文件和设置相应的读写位置指针函数
输入参数    szPathFile----话单文件名称（包含全路径）
            nOffset----读写位置偏移量
输出参数    无
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CScpBillFile::Open(const char* szPathFile, int nOffset)
{
    //如果当前有文件打开，则关闭之
    if (NULL != m_pFileStream)
    {
        Close();
    }

    strcpy(m_szFileFullPath, szPathFile);
    m_nCurOffset = nOffset;

    //打开文件，并设置偏移量

    m_pFileStream = fopen(szPathFile, "rb");
    if (NULL == m_pFileStream)
    {
        return FALSE;
    }
    else
    {
        m_nMemDataLen = FileSize(m_pFileStream);
        if(m_nMemFileLen < m_nMemDataLen)
        {
            m_nMemFileLen = m_nMemDataLen;

            ASSERT(m_pMemFile != NULL);
            delete [] m_pMemFile;
            m_pMemFile = new BYTE[m_nMemFileLen];
        }
        size_t nBytesRead = fread(m_pMemFile, m_nMemDataLen,
                                  1, m_pFileStream);
        if (1 != nBytesRead)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/********************************************************
* 名称: CScpBillFile::IsMatchedBill
* 功能: 对一张话单进行特殊判断
* 输入: pData : 话单数据指针
*       nLen  : 话单长度
* 输出: 
* 返回: BOOL 是否符合条件
********************************************************/
BOOL CScpBillFile::IsMatchedBill(void* pData, int nLen)
{
    return m_pScpRvyTask->IsMatchedScpBill((BYTE*)pData, nLen);
}


/**********************************************************************
功能描述    获取下一张符合条件的话单记录数据函数
输入参数    无
输出参数    pBillRecord---- 话单记录对象指针
            nCurOffset----找到一张话单记录后的当前在话单文件中的位置
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pBillRecord的值无效
返回值      成功与否
抛出异常    无
修改记录
    2002.3.30   颜文远      修改代码，在话单内容错误时设置错误信息
**********************************************************************/
BOOL CScpBillFile::GetNextBill(CBillRecord*& pBillRecord,
                            int& nCurOffset, BOOL& bRemain)
{
    void* pBillData = NULL;
    UINT4 uBillLen;

    //如果当前没有打开文件，则直接返回
    if (NULL == m_pFileStream)
    {
        bRemain = FALSE;
        return TRUE;
    }

    BOOL bSucc = TRUE, bRemainInFile = TRUE;
    CBillRecord* pRecord = NULL;
    nCurOffset = m_nCurOffset;
    //如果读出的一条话单记录满足要求，则返回该话单记录，
    //否则继续读取下一条话单，直到文件结束或找到满足条件的话单
    while (1)
    {
        bSucc = Read(pBillData, uBillLen, bRemainInFile);
        if (!bSucc)
        {
            return FALSE;
        }
        else if (!bRemainInFile)
        {
            bRemain = FALSE;
            return TRUE;
        }

        //颜文远2002.3.30添加，问题单号D12265
        if (NULL == pBillData)  //话单内容错误
        {
            continue;
        }
        //颜文远2002.3.30添加结束

        //判断话单记录是否满足查询条件，以决定返回该记录还是继续判断下一条话单
        int nResult = m_pFormat->IsTheFormat(pBillData, uBillLen);
        if (0 == nResult)   //话单记录符合当前浏览格式
        {
            if ((0 == m_pFormat->IsTheQueryCondition(pBillData)) &&
                IsMatchedBill(pBillData, uBillLen))
            //该话单记录满足查询条件，则返回本话单记录
            {
                pRecord = new CBillRecord();
                bSucc = pRecord->SetContent((BYTE* )pBillData,
                                            uBillLen, m_pFormat);
                if (!bSucc)
                {
                    delete pRecord;
                    return FALSE;
                }
                else
                {
                    pBillRecord = pRecord;
                    bRemain = TRUE;
                    nCurOffset = m_nCurOffset;
                    return TRUE;
                }
            }
        }
    }
}

//获取文件的当前读写位置函数
int CScpBillFile::GetCurOffset()
{
    return m_nCurOffset;
}

//设置文件的读写指针位置函数
BOOL CScpBillFile::SetOffset(int nOffset)
{
    m_nCurOffset = nOffset;

    return TRUE;
}

//设置查询条件字符串函数
BOOL CScpBillFile::SetQueryCond(char* szQueryCond)
{
    int nResult = 0;
    if (strlen(szQueryCond) > 0)
    {
        nResult = m_pFormat->SetQueryCond(szQueryCond);
    }

    return (0 == nResult);
}

//关闭文件函数
BOOL CScpBillFile::Close()
{
    if (NULL != m_pFileStream)
    {
        fclose(m_pFileStream);
        m_pFileStream = NULL;
    }

    return TRUE;
}

/**********************************************************************
功能描述    读取一张话单记录数据函数
输入参数    无
输出参数    pData---- 话单记录数据的指针，使用固定的内存
            uDataLen----话单记录的长度
            bRemain----表示文件中是否已经没有记录了，当被
                        设置为FALSE时，pData的值无效
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CScpBillFile::Read(void*& pData, UINT4& uDataLen, BOOL& bRemain)
{
    ASSERT(NULL != m_pMemFile);

    BYTE* pBillRecord = NULL;
    if (m_nMemDataLen < m_uBillLen + m_nCurOffset)
    //内存映射文件中的剩余长度不够一个话单的长度，则后面的数据无效
    {
        bRemain = FALSE;
    }
    else
    {
        pBillRecord = m_pMemFile + m_nCurOffset;
        //如果读出来的话单内容全部为0，则说明本文件是前存盘的最后一个话单，
        //读出的话单内容以及本文件的后续内容必须丢弃，将bRemain设置为FALSE

        BOOL bValid = TRUE;
        if (ORIG_BILL == m_uBillType)
        {
            bValid = FALSE;

            //判断是否读出的话单内容都是0
            for (int i = 0; i < m_uBillLen; i++)
            {
                if (0 != pBillRecord[i])
                {
                    bValid = TRUE;
                    break;
                }
            }
        }

        //如果话单内容都是0，则说明该话单是无效的，必须丢弃
        if (!bValid)
        {
            bRemain = FALSE;
        }
        else
        {
            m_nCurOffset += m_uBillLen;
            pData = pBillRecord;
            uDataLen = m_uBillLen;
            bRemain = TRUE;
        }
    }

    return TRUE;
}