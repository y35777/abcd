#include "../include/toolbox.h"
#include "bill_task_struct.h"
#include "scp_file_group.h"
#include "scp_bill_file.h"
#include "scp_rvy_task.h"
#include "../querybrowse/qb_comm.h"


/********************************************************
* 名称: CScpFileGroup::CScpFileGroup
* 功能: 构造函数
* 输入: pFormat     : 格式库对象的指针
*       uBillType   : 原始话单还是最终话单
*       uAPType     : 接入点类型
*       pScpRvyTask : CScpRvyTask对象的指针
* 输出: 
* 返回: 
********************************************************/
CScpFileGroup::CScpFileGroup(CFormat *pFormat, 
                             UINT4 uBillType, 
                             UINT4 uAPType, 
                             CScpRvyTask* pScpRvyTask)
{
    m_pFormat    = pFormat;
    m_uBillType  = uBillType;
    m_uAPType    = uAPType;
    m_nCurFileNo = -1;
    m_nCurOffset = -1;

    m_pBillFile = new CScpBillFile(m_pFormat, m_uBillType, m_uAPType, pScpRvyTask);
    if(NULL == m_pBillFile) throw BR_SYSECPT;
}


/********************************************************
* 名称: CScpFileGroup::~CScpFileGroup
* 功能: 析构函数
* 输入: 
* 输出: 
* 返回: 
********************************************************/
CScpFileGroup::~CScpFileGroup()
{
    if (NULL != m_pBillFile)
    {
        delete m_pBillFile;
    }
}

/**********************************************************************
功能描述    打开指定的话单文件，并设置其读写位置为指定的位置
输入参数    nFileNo----指定的话单文件序号 
            nOffset----指定的读写位置
输出参数    无
返回值      打开成功与否
抛出异常    无
**********************************************************************/
BOOL CScpFileGroup::Open(int nFileNo, int nOffset)
{
    ASSERT(NULL != m_pBillFile);

    BOOL bSucc = FALSE;

    if (m_nCurFileNo == nFileNo)
    {
        bSucc = m_pBillFile->SetOffset(nOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }
    else
    {
        //关闭当前文件，打开指定文件，并设置写位置
        bSucc = m_pBillFile->Close();
        if (!bSucc)
        {
            return FALSE;
        }

        STRING szFileName = m_qFileNames[nFileNo];

        char szFileFullPath[MAX_PATH];
        sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName.c_str());
        bSucc = m_pBillFile->Open(szFileFullPath, nOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    m_nCurFileNo = nFileNo;
    m_nCurOffset = nOffset;

    return TRUE;
}

//获取当前打开的话单文件序号和其当前读写位置
void CScpFileGroup::GetCurPosition(int& nFileNo, int& nOffset)
{
    nFileNo = m_nCurFileNo;
    nOffset = m_nCurOffset;
}

/**********************************************************************
功能描述    获取下一张符合条件的话单记录数据
输入参数    无
输出参数    pBillRecord---- 话单记录对象指针
            nStartFileNo----本张话单记录数据所在话单文件序号
            nStartOffset----本张话单记录数据在话单文件中的位置
            bRemain----是否还有后续的数据，为FALSE时则pBillRecord无效
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CScpFileGroup::GetNextRecord(CBillRecord*& pBillRecord, 
                                 int& nStartFileNo, 
                                 int& nStartOffset,
                                 BOOL& bRemain)
{
    BOOL bRemainInFile;
    while (1)
    {
        BOOL bSucc = m_pBillFile->GetNextBill(pBillRecord, m_nCurOffset, 
                                              bRemainInFile);
        if (!bSucc)
        {
            return FALSE;
        }

        if (bRemainInFile)  //在当前的文件中找到了话单记录
        {
            nStartFileNo = m_nCurFileNo;
            nStartOffset = m_nCurOffset;
            bRemain = TRUE;
            return TRUE;
        }
        else    //否则，打开下一个文件，继续获得下一张话单记录
        {
            BOOL bRemainInGroup;
            bSucc = OpenNextFile(bRemainInGroup);
            if (!bSucc)
            {
                return FALSE;
            }
            if (!bRemainInGroup)
            {
                bRemain = FALSE;
                return TRUE;
            }
        }
    }
}

/**********************************************************************
功能描述    准备话单文件，针对浏览任务
输入参数    szFileFullPath----包括全路径的话单文件名称
输出参数    无
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CScpFileGroup::PrepareFile(const char* szFileFullPath)
{
	m_pBillFile->SetQueryCond((char* )(""));

    //从全路径中获得文件路径和文件名
    char* pPos = (char* )strrchr(szFileFullPath, '/');
    if (NULL == pPos)   //如果没有找到'/'，则找'\'试试
    {
        pPos = (char* )strrchr(szFileFullPath, '\\');
        if (NULL == pPos)
        {
            return FALSE;
        }
    }
    memcpy(m_szPath, szFileFullPath, pPos - szFileFullPath);
    m_szPath[pPos - szFileFullPath] = '\0';
    STRING szFileName = (pPos + 1);

    m_qFileNames.push_back(szFileName);

    return TRUE;
}


/**********************************************************************
功能描述    准备话单文件，针对查询任务
输入参数    szPath----目录路径
            szStartRange----查询的开始范围，取值可能是日期、或话单文件名
            szEndRange----查询结束范围，取值可能是日期、或话单文件名
            uFlg----查询范围标志，用于区分查询范围是日期目录还是话单文件名称，
                    取值在QueryRangeType里定义
            szQueryCond----查询条件字符串
输出参数    无
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CScpFileGroup::PrepareFiles(const char* szPath, const char* szStartRange, 
                              const char* szEndRange, 
                              UINT4 uFlg, char* szQueryCond)
{
    strcpy(m_szPath, szPath);
    BOOL bSucc = m_pBillFile->SetQueryCond(szQueryCond);
    if (!bSucc)
    {
        return FALSE;
    }
    
    LIST<STRING> FileList;    //符合条件的文件名的列表
    if (DATE_RANGE == uFlg) 
    {
        //获取排过序的所有日期目录，然后从中得到范围内的所有日期目录
        LIST<STRING> DateList;
        STRING szDateName;
        GetDateDir(m_szPath, DateList);
        while (1)     //截去日期目录列表中头部不符合条件的项
        {
            if (DateList.empty())
            {
               break;
            }

            szDateName = DateList.front();
            if (0 == strcmp(szDateName.c_str(), szStartRange))
            {
                break;
            }
            else
            {
                DateList.pop_front();
            }
        }
        while (1)     //截去日期目录列表中尾部不符合条件的项
        {
            if (DateList.empty())
            {
                break;
            }

            szDateName = DateList.back();
            if (0 == strcmp(szDateName.c_str(), szEndRange))
            {
                break;
            }
            else
            {
                DateList.pop_back();
            }
        }

        //如果得到的日期目录列表为空，说明起始日期目录或终止日期
        //目录在硬盘上已经不存在，此时返回失败
        if (DateList.empty())
        {
            return FALSE;
        }

        //获取条件范围内的日期目录下的所有文件，
        //文件名格式为日期目录/文件名的方式
        char szFilePath[MAX_PATH];
        LIST<STRING> FileSubList;     //存放一个日期目录下的所有文件的列表
        LIST<STRING>::iterator i, j;
        for (i = DateList.begin(); i != DateList.end(); i++)
        {
            szDateName = *i;
            sprintf(szFilePath, "%s/%s", m_szPath, szDateName.c_str());
            GetFileList(szFilePath, FileSubList);

            //GetFileList返回的列表中只包含文件名，所以必须将日期目录信息加上去
            for (j = FileSubList.begin(); j != FileSubList.end(); j++)
            {
                *j = szDateName + "/" + *j;
            }

            //将FileSubList中的内容加入到FileList的末尾
            FileList.splice(FileList.end(), FileSubList);
            FileSubList.clear();
        }
		if (FileList.empty())
		{
			return FALSE;
		}
    }
    else
    {
        //获取目录下排过序的所有文件，然后从中得到范围内的所有文件
        GetFileList(m_szPath, FileList);
        STRING szFileName;
        while (1)     //截去文件列表中头部不符合条件的项
        {
            if (FileList.empty())
            {
                break;
            }

            szFileName = FileList.front();
            if (0 == strcmp(szFileName.c_str(), szStartRange))
            {
                break;
            }
            else
            {
                FileList.pop_front();
            }
        }
        while (1)     //截去文件列表中尾部不符合条件的项
        {
            if (FileList.empty())
            {
                break;
            }

            szFileName = FileList.back();
            if (0 == strcmp(szFileName.c_str(), szEndRange))
            {
                break;
            }
            else
            {
                FileList.pop_back();
            }
        }

        //如果得到的文件列表为空，说明起始文件或终止文件 
        //在硬盘上已经不存在，此时返回失败
        if (FileList.empty())
        {
            return FALSE;
        }
    }

    //将得到的文件列表导入到m_qFileNames中
    LIST<STRING>::iterator i;
    for (i = FileList.begin(); i != FileList.end(); i++)
    {
        m_qFileNames.push_back(*i);
    }

    return TRUE;
}

/**********************************************************************
功能描述    打开下一个话单文件
输入参数    无
输出参数    bRemain----表示文件组中是否已经没有文件可以打开了，当被
                       设置为FALSE时，m_pBillFile的值无效
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CScpFileGroup::OpenNextFile(BOOL& bRemain)
{
    if (m_nCurFileNo == m_qFileNames.size() - 1)
    //如果当前已经是文件组中的最后一个文件，则直接返回
    {
        bRemain = FALSE;
        return TRUE;
    }
    else
    //否则打开下一个文件，并刷新当前位置
    {
		//首先关闭当前的文件
		BOOL bSucc = m_pBillFile->Close();
		if (!bSucc)
		{
			return FALSE;
		}

        bRemain = TRUE;
        m_nCurFileNo++;
        m_nCurOffset = 0;

        char szFileName[MAX_PATH], szFileFullPath[MAX_PATH];
        strcpy(szFileName, m_qFileNames[m_nCurFileNo].c_str());
        sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName);

        bSucc = m_pBillFile->Open(szFileFullPath, m_nCurOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }
    
    return TRUE;
}
