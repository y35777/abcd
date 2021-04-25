#include "info_file.h"
#include "../include/toolbox.h"

/**********************************************************************
功能描述    为指定的文件申请指定大小的空间
输入参数    f----指定的文件对应的文件指针，必须是新创建的文件
            nFileSize----要求申请的文件空间的大小
输出参数    无
返 回 值    申请空间是否成功 
                TRUE----申请空间成功 
                FALSE----申请空间不成功
抛出异常    无
**********************************************************************/
BOOL AskFileSpace(FILE* f, int nFileSize)
{
	ASSERT(NULL != f);

	if (nFileSize <= 0)
	{
		return TRUE;
	}

	//根据要求的空间大小移动文件指针，并在最后一个位置写入一个数据0，
	//从而为文件申请到指定大小的空间
	int nResult = fseek(f, nFileSize - 1, SEEK_SET);
	if (0 != nResult)
	{
		return FALSE;
	}
	BYTE byFileEnd = 0;
	size_t uWritten = fwrite(&byFileEnd, 1, 1, f);
	if (1 != uWritten)
	{
		return FALSE;
	}

	//将文件指针移回到文件头
	nResult = fseek(f, 0, SEEK_SET);
	
	return (0 == nResult);
}

CInfoFile::CInfoFile()
{
    m_pInfoFile = NULL;
    m_szInfoFileName = "";
    m_szInfoFilePath = "";
    m_szBackupName = "";
    m_szBackupPath = "";
    m_uHeadLen = 0;
    m_uUnitLen = 0;
    m_uUnitNum = 0;
    m_uFileLen = 0;
    m_uInfoWriteIndex = 0; 
    m_nRotateCount = 0;
}

CInfoFile::~CInfoFile()
{
    Close();
}

//获取状态文件的全路径
STRING CInfoFile::GetFileFullPath()
{
    return (m_szInfoFilePath + "/" + m_szInfoFileName);
}

UINT4 CInfoFile::GetUnitNum()
{
    return m_uUnitNum;
}

UINT4 CInfoFile::GetWriteIndex()
{
    return m_uInfoWriteIndex;
}

/**********************************************************************
功能描述    设置状态文件名和存放路径
输入参数    szFileName----状态文件名 
            szFilePath----状态文件存放路径
输出参数    无
返 回 值    无
抛出异常    无
**********************************************************************/
void CInfoFile::SetInfoFilePath(const STRING& szFileName, 
                                       const STRING& szFilePath)
{
    m_szInfoFileName = szFileName;
    m_szInfoFilePath = szFilePath;
}

/**********************************************************************
功能描述    设置状态备份文件名和存放路径
输入参数    szFileName----备份文件名 
            szFilePath----备份文件存放路径
输出参数    无
返 回 值    无
抛出异常    无
**********************************************************************/
void CInfoFile::SetBackupFilePath(const STRING& szFileName, 
                                  const STRING& szFilePath)
{
    m_szBackupName = szFileName;
    m_szBackupPath = szFilePath;
}

/**********************************************************************
功能描述    设置文件头长度，记录单元长度和记录单元个数
输入参数    uHeadLen----文件头长度 
            uUnitLen----记录单元长度 
            uUnitNum----记录单元个数
输出参数    无
返 回 值    无
抛出异常    无
**********************************************************************/
void CInfoFile::SetFileStruct(const UINT4 uHeadLen, const UINT4 uUnitLen,
                                     const UINT4 uUnitNum)
{
    m_uHeadLen = uHeadLen;
    m_uUnitLen = uUnitLen;
    m_uUnitNum = uUnitNum;

    //根据设置的参数计算出文件的长度
    m_uFileLen = m_uHeadLen + m_uUnitLen * m_uUnitNum;
}

/**********************************************************************
功能描述    对指定的文件头计算校验和
输入参数    pHead----要进行校验和计算的状态文件头
输出参数    pHead----进行校验和计算后的状态文件头
返 回 值    无
抛出异常    无
**********************************************************************/
void CInfoFile::CalcCheckSum(BYTE* pHead)
{
    ASSERT(0 != m_uHeadLen);

    UINT4 uCheckSum = 0;
    UINT4* pUint4Head = (UINT4* )pHead;
   
    //计算校验和：状态头中除了第一个UINT4（校验和域）外的所有UINT4的和 
    for (int i = 1; i < m_uHeadLen / sizeof(UINT4); i++)
    {
        uCheckSum += pUint4Head[i];
    }

    pUint4Head[0] = uCheckSum;        //对pHead的校验和域进行赋值；
}

/**********************************************************************
功能描述    使pHead中的uStatusFileWritePos递增一个状态记录的长度
输入参数    pHead----更新前的状态头，空间在函数外申请和释放
输出参数    pHead----更新后的状态头
返 回 值    无
抛出异常    无
**********************************************************************/
void CInfoFile::IncreaseIndex(BYTE* pHead)
{
    ASSERT(NULL != pHead);

    //增加pHead中的uStatusFileWritePos的值，然后处理是否发生回绕
    SInfoHead* pInfoHead = (SInfoHead* )pHead;
    pInfoHead->uStatusFileWritePos += m_uUnitLen;
    if (pInfoHead->uStatusFileWritePos >= m_uFileLen)
    {
        pInfoHead->uStatusFileWritePos = m_uHeadLen;
		pInfoHead->uRotateCount++;
    }
}

/**********************************************************************
功能描述    根据属性中指定的路径，文件名和长度创建或打开状态文件
输入参数    bNeedCreate----当文件不存在时，是否需要创建 
            pDefaultHead----在创建新文件时要写入的缺省状态头，
                在本值为NULL时，缺省状态头 中除了uStatusFileWritePos填
                为m_uHeadLen，uCheckSum进行计算外，其它的域填为0。
                如果本值不为NULL，则必须保证所指向内存的长度为m_uHeadLen。
                内存在函数外申请和释放
输出参数    无
返 回 值    文件打开是否成功 
                TRUE----文件打开成功 
                FALSE----文件打开不成功
抛出异常    无
**********************************************************************/
BOOL CInfoFile::OpenInfoFile(BOOL bNeedCreate, BYTE* pDefaultHead)
{
    BOOL bSucc = FALSE;    //用于表示操作结果是否成功
    if (NULL != m_pInfoFile)
    {
        return TRUE;
    }

    BYTE* pInfoHead = new BYTE[m_uHeadLen];

    STRING szFullPath = m_szInfoFilePath + "/" + m_szInfoFileName;
    m_pInfoFile = fopen(szFullPath.c_str(), "rb+");

    if (NULL != m_pInfoFile)
    {
        bSucc = TRUE;
        bSucc = ReadHead(pInfoHead);
        if (!bSucc)
        {
            Close();
        }
    }
    else if (!bNeedCreate)
    {
        bSucc = FALSE;
    }
    else        //文件不存在时，创建新文件，并写文件头
    {
        //创建新文件，在创建新文件时申请m_uFileLen的文件长度
        m_pInfoFile = fopen(szFullPath.c_str(), "wb+");
        if (NULL == m_pInfoFile)
        {
            bSucc = FALSE;
        }
        else if (!AskFileSpace(m_pInfoFile, m_uFileLen))  //预先申请空间
        {
            bSucc = FALSE;
            Delete();
        }
        else //创建新文件并申请空间成功
        {
            if (NULL == pDefaultHead)
            {
                memset(pInfoHead, 0, m_uHeadLen);   //将pInfoHead的所有内存填0
                ((SInfoHead* )pInfoHead)->uStatusFileWritePos = m_uHeadLen;
                CalcCheckSum(pInfoHead);
            }
            else
            {
                memcpy(pInfoHead, pDefaultHead, m_uHeadLen);
            }

            bSucc = WriteHead(pInfoHead, m_uHeadLen);
            if (!bSucc)
            {
                Delete();
            }
			//颜文远2002.05.18修改，问题单号D13767
			else
			{
				Flush();
			}
			//颜文远2002.05.18修改结束
        }
    }

    if (bSucc)
    {
        //根据状态头的内容设置相关变量的值
        m_nRotateCount = ((SInfoHead* )pInfoHead)->uRotateCount;
        UINT4 uStatusWritePos = ((SInfoHead* )pInfoHead)->uStatusFileWritePos;
        m_uInfoWriteIndex = (uStatusWritePos - m_uHeadLen) / m_uUnitLen + 1;
    }

    delete[] pInfoHead;

    return bSucc;
}

//关闭状态文件
BOOL CInfoFile::Close()
{
    if (NULL == m_pInfoFile)
    {
        return TRUE;
    }

    int nResult = fclose(m_pInfoFile);
    if (0 != nResult)
    {
        return FALSE;
    }

    m_pInfoFile = NULL;
    
    return TRUE;
}

/**********************************************************************
功能描述    删除状态文件
输入参数    无
输出参数    无
返 回 值    操作是否成功 
                TRUE----操作成功 
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CInfoFile::Delete()
{
    BOOL bSucc = FALSE;

    bSucc = Close();
    if (!bSucc)
    {
        return FALSE;
    }

    STRING szFullPath = m_szInfoFilePath + "/" + m_szInfoFileName;
    bSucc = FileDel(szFullPath.c_str());

    return bSucc;
}

/**********************************************************************
功能描述    将未写入到硬盘的状态数据写入到硬盘中
输入参数    无
输出参数    无
返 回 值    操作是否成功 
                TRUE----操作成功 
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CInfoFile::Flush()
{
    ASSERT(NULL != m_pInfoFile);

    return (0 == fflush(m_pInfoFile));
}

/**********************************************************************
功能描述    该函数实现将状态文件拷贝到相应的备份目录下
输入参数    无
输出参数    无
返 回 值    操作是否成功 
                TRUE----操作成功 
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CInfoFile::CopyFileForBackup()
{
    STRING szSrcFullPath = m_szInfoFilePath + "/" + m_szInfoFileName;
    STRING szDstFullPath = m_szBackupPath + "/" + m_szBackupName;

    BOOL bSucc = FileCopy(szSrcFullPath.c_str(), szDstFullPath.c_str());

    return bSucc;
}

/**********************************************************************
功能描述    该函数实现写文件头的功能
输入参数    pData----写入数据指针，调用者负责释放 
            uLen----写入数据长度，必须等于m_uHeadLen，
输出参数    无
返 回 值    操作是否成功 
                TRUE----操作成功 
                FALSE----操作失败
抛出异常    无
备注        在调用时，如果要同时调用WriteUnit和WriteHead，
            则WriteUnit一定必须在WriteHead前调用
**********************************************************************/
BOOL CInfoFile::WriteHead(const BYTE* pData, const UINT4 uLen)
{
    ASSERT(m_uHeadLen == uLen);

    BOOL bSucc = WriteInfo(0, pData, uLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //因为文件头中的uStatusFileWritePos可能已经更新，
    //所以必须据此更新m_uInfoWriteIndex的值
    UINT4 uWritePos = ((SInfoHead* )pData)->uStatusFileWritePos;
    m_uInfoWriteIndex = (uWritePos - m_uHeadLen) / m_uUnitLen + 1;
    m_nRotateCount = ((SInfoHead* )pData)->uRotateCount;

    return bSucc;
}

/**********************************************************************
功能描述    写第m_uInfoWriteIndex个记录单元
输入参数    pData----写入数据指针，调用者负责释放 
            uLen----写入数据长度，必须等于m_uUnitLen
输出参数    无
返 回 值    操作是否成功 
                TRUE----操作成功 
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CInfoFile::WriteUnit(const BYTE* pData, const UINT4 uLen)
{
    ASSERT(uLen == m_uUnitLen);

    //计算写文件的起始位置
    UINT4 uWritePos = m_uHeadLen + m_uUnitLen * (m_uInfoWriteIndex - 1);    
    BOOL bSucc = WriteInfo(uWritePos, pData, uLen);

    return bSucc;
}

/**********************************************************************
功能描述    函数实现读取文件头的功能
输入参数    pData----读取数据缓冲区指针，调用者负责申请和释放
输出参数    无
返 回 值    操作是否成功 
                TRUE----操作成功 
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CInfoFile::ReadHead(BYTE* pData)
{
    BOOL bSucc = ReadInfo(0, pData, m_uHeadLen);
    return bSucc;
}

/**********************************************************************
功能描述    该函数实现读取指定的记录单元的功能
输入参数    uIndex----读取单元索引 
            pData----读取数据缓冲区指针，调用者负责申请和释放
输出参数    无
返 回 值    返回值说明如下：
            0----读取成功
            1----读取失败
            <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值：
                ERR_NO_UNIT_EXIST----状态文件为新创建的文件,无状态记录存在
                ERR_INDEX_INVALID----要读取的状态记录的序号不合法
                ERR_FAIL_READ----读状态记录失败
抛出异常    无
**********************************************************************/
int CInfoFile::ReadUnit(UINT4 uIndex, BYTE* pData)
{
    //首先判断uIndex的合法性
    if ((1 == m_uInfoWriteIndex) && (0 == m_nRotateCount))
    //是新文件，还没有写入状态记录
    {
        return ERR_NO_UNIT_EXIST;
    }
    else if (((uIndex >= m_uInfoWriteIndex) && (0 == m_nRotateCount))
              || (uIndex > m_uUnitNum))
    //要读取的记录是无效的记录
    {
        return ERR_INDEX_INVALID;
    }

    //计算读操作的起始位置,并读取相应的数据
    UINT4 uReadPos = m_uHeadLen + m_uUnitLen * (uIndex - 1);
    BOOL bSucc = ReadInfo(uReadPos, pData, m_uUnitLen);
    if (!bSucc)
    {
        return ERR_FAIL_READ;
    }

    return ERR_SUCCESS;
}

/**********************************************************************
功能描述    该函数实现读取最后一个记录单元的功能
输入参数    pData----读取数据缓冲区指针，调用者负责申请和释放
输出参数    无
返 回 值    返回值说明如下：
            0----读取成功
            1----读取失败
            <0----读取失败，返回值的绝对值为错误码，错误码包括如下取值：
                ERR_NO_UNIT_EXIST----状态文件为新创建的文件,无状态记录存在
                ERR_INDEX_INVALID----要读取得状态记录的序号不合法
                ERR_FAIL_READ----读状态记录失败
抛出异常    无
**********************************************************************/
int CInfoFile::ReadLastUnit(BYTE* pData)
{
    //计算最后一个状态记录的序号值
    int nLastIndex;
    nLastIndex = m_uInfoWriteIndex - 1;
    if (nLastIndex <= 0)
    {
        if (m_nRotateCount > 0)
        {
            nLastIndex = m_uUnitNum;
        }
        else
        {
            nLastIndex = 0;
        }
    }

    return ReadUnit(nLastIndex, pData);
}

/**********************************************************************
功能描述    校验文件头是否有效
输入参数    无
输出参数    无
返 回 值    文件头是否有效： 
                TRUE----文件头有效 
                FALSE----文件头无效
抛出异常    无
**********************************************************************/
BOOL CInfoFile::CheckHead()
{
    ASSERT(NULL != m_pInfoFile);

    BYTE* pHead = new BYTE[m_uHeadLen];
    BOOL bSucc = ReadHead(pHead);     //读取状态头
    if (!bSucc)
    {
        delete[] pHead;
        return FALSE;
    }
    UINT4 uOldCheckSum = ((SInfoHead* )pHead)->uCheckSum;
    CalcCheckSum(pHead);
    UINT4 uNewCheckSum = ((SInfoHead* )pHead)->uCheckSum;
    delete[] pHead;
    return (uOldCheckSum == uNewCheckSum);
}

/**********************************************************************
功能描述    提供状态文件写入的接口
输入参数    uOffset----数据写入起始位置 
            pData----写入数据指针，调用者负责申请和释放 
            uLen----写入数据长度
输出参数    无
返 回 值    操作是否成功 
                TRUE----操作成功 
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CInfoFile::WriteInfo(const UINT4 uOffset, const BYTE* pData, 
                          const UINT4 uLen)
{
    ASSERT(NULL != m_pInfoFile);

    //定位文件写指针
    int nResult = fseek(m_pInfoFile, uOffset, SEEK_SET);
    if (0 != nResult)
    {
        return FALSE;
    }

    //向文件写数据
    size_t uLenWritten = fwrite((void* )pData, uLen, 1, m_pInfoFile);

    return (1 == uLenWritten);
}

/**********************************************************************
功能描述    提供读取状态文件数据的接口
输入参数    uOffset----读取数据起始位置 
            pData----读取数据缓冲区指针，调用者负责申请和释放 
            uLen----读取数据长度
输出参数    无
返 回 值    操作是否成功 
                TRUE----操作成功 
                FALSE----操作失败
抛出异常    无
**********************************************************************/
BOOL CInfoFile::ReadInfo(const UINT4 uOffset, const BYTE* pData, 
                         const UINT4 uLen)
{
    ASSERT(NULL != m_pInfoFile);

    //定位文件读指针
    int nResult = fseek(m_pInfoFile, uOffset, SEEK_SET);
    if (0 != nResult)
    {
        return FALSE;
    }

    //从状态文件读取数据
    size_t uLenRead = fread((void* )pData, uLen, 1, m_pInfoFile); 

    return (1 == uLenRead);
}
