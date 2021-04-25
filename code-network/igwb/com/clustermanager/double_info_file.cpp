//本文件由颜文远于2002.05.08创建
#include "double_info_file.h"
#include "../frontsave/resource.h"
#include "../include/toolbox.h"


/**********************************************************************
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
CDoubleInfoFile::CDoubleInfoFile()
{
	m_uHeadLen = 0;
}

/**********************************************************************
功能描述    析构函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
CDoubleInfoFile::~CDoubleInfoFile()
{
	Close();
}


/**********************************************************************
功能描述    初始化函数
输入参数    szMainFilePath——主状态文件的路径
			szMainFileName——主状态文件的文件名
			szBakFilePath——备状态文件的路径
			szBakFileName——备状态文件的文件名
输出参数    无
返回值      无
抛出异常    无
**********************************************************************/
void CDoubleInfoFile::Init(const char* const szMainFilePath,
						   const char* const szMainFileName,
				           const char* const szBakFilePath,
						   const char* const szBakFileName)
{
	m_InfoMain.SetInfoFilePath(szMainFileName, szMainFilePath);
	m_InfoBak.SetInfoFilePath(szBakFileName, szBakFilePath);
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
void CDoubleInfoFile::SetFileStruct(const UINT4 uHeadLen, 
									const UINT4 uUnitLen, 
									const UINT4 uUnitNum)
{
	ASSERT(uHeadLen > 0);
	m_uHeadLen = uHeadLen;

	m_InfoMain.SetFileStruct(uHeadLen, uUnitLen, uUnitNum);
	m_InfoBak.SetFileStruct(uHeadLen, uUnitLen, uUnitNum);
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
BOOL CDoubleInfoFile::OpenInfoFile(BOOL bNeedCreate, BYTE* pDefaultHead)
{
	ASSERT(m_uHeadLen > 0);

	//首先检查主备状态文件的情况

    BOOL bSucc;
    BOOL bStatusValid = FALSE;  //状态文件是否有效的标志
    BOOL bMainExist = FALSE, bBakExist = FALSE; //主备状态文件是否存在

    bMainExist = m_InfoMain.OpenInfoFile(FALSE);    //打开主状态文件
    bBakExist = m_InfoBak.OpenInfoFile(FALSE);      //打开备状态文件
    if (bMainExist && bBakExist)    //主备状态文件同时存在
    {
        //主备状态文件头都有效
        if (m_InfoMain.CheckHead() && m_InfoBak.CheckHead())
        {
            bStatusValid = TRUE;
        }
        //主状态文件头有效，但备状态文件头无效
        else if (m_InfoMain.CheckHead() && !m_InfoBak.CheckHead())
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_V_BAK_INV);

            bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            STRING szSrcFileFullPath = m_InfoMain.GetFileFullPath(); 
            STRING szDstFileFullPath = m_InfoBak.GetFileFullPath(); 
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //备状态文件头有效，但主状态文件头无效
        else if (!m_InfoMain.CheckHead() && m_InfoBak.CheckHead())
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INV_BAK_V);

            bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            STRING szSrcFileFullPath = m_InfoBak.GetFileFullPath(); 
            STRING szDstFileFullPath = m_InfoMain.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        else//主备状态文件头都无效
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INV_BAK_INV);

            bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
    //主状态文件存在但备状态文件不存在
    else if (bMainExist && !bBakExist)
    {
        //主状态文件头有效
        if (m_InfoMain.CheckHead())
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_V_BAK_INE);

            STRING szSrcFileFullPath = m_InfoMain.GetFileFullPath();
            STRING szDstFileFullPath = m_InfoBak.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //主状态文件头无效
        else
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INV_BAK_INE);

            bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
    //主状态文件不存在但备状态文件存在
    else if (!bMainExist && bBakExist)
    {
        //备状态文件头有效
        if (m_InfoBak.CheckHead())
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INE_BAK_V);

            STRING szSrcFileFullPath = m_InfoBak.GetFileFullPath();
            STRING szDstFileFullPath = m_InfoMain.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //备状态文件头无效
        else
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INE_BAK_INV);

            bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
    //主备状态文件都不存在
    else
    {
        TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INE_BAK_INE);
        bStatusValid = FALSE;
    }

    //如果状态文件无效
    if (!bStatusValid)
    {
        bSucc = m_InfoMain.OpenInfoFile(bNeedCreate, pDefaultHead);
        if (!bSucc)
        {
            return FALSE;
        }
        bSucc = m_InfoBak.OpenInfoFile(bNeedCreate, pDefaultHead);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    bSucc = m_InfoMain.OpenInfoFile(FALSE);
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoBak.OpenInfoFile(FALSE);
    if (!bSucc)
    {
        return FALSE;
    }

    return TRUE;
}

/**********************************************************************
功能描述    关闭状态文件
输入参数    无
输出参数    无
返 回 值    关闭文件是否成功
                TRUE----关闭文件成功 
                FALSE----关闭文件不成功
抛出异常    无
**********************************************************************/
BOOL CDoubleInfoFile::Close()
{
	BOOL bSucc;
	bSucc = m_InfoMain.Close();
	if (!bSucc)
	{
		return FALSE;
	}
	bSucc = m_InfoBak.Close();

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
BOOL CDoubleInfoFile::Flush()
{
	BOOL bSucc;
	bSucc = m_InfoMain.Flush();
	if (!bSucc)
	{
		return FALSE;
	}
	bSucc = m_InfoBak.Flush();
	
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
**********************************************************************/
BOOL CDoubleInfoFile::WriteHead(const BYTE* pData, const UINT4 uLen)
{
    m_InfoMain.CalcCheckSum((BYTE*)pData);

	BOOL bSucc;
	bSucc = m_InfoMain.WriteHead(pData, uLen);
	if (!bSucc)
	{
		return FALSE;
	}
	bSucc = m_InfoBak.WriteHead(pData, uLen);

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
BOOL CDoubleInfoFile::ReadHead(BYTE* pData)
{
	return m_InfoBak.ReadHead(pData);
}
