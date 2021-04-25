#include "file_group.h"
#include <algorithm>
#include "qb_comm.h"
#include "../include/toolbox.h"
#include "../include/frame.h"
#include "bill_qb.h"

//构造函数
CFileGroup::CFileGroup(CFormat *pFormat, UINT4 uBillType, UINT4 uAPType, ACE_Mutex* pMutex, int nAPID)
{
    m_nCurFileNo       = -1;
    m_nCurOffset       = -1;
    m_nQueryedFileLen  = 0;
    m_nTotalFileLength = 0;
    m_nStartFileNo     = -1;

    m_pBillFile        = new CBillFile(pFormat, uBillType, uAPType, nAPID, pMutex);
}

CFileGroup::~CFileGroup()
{
    if (NULL != m_pBillFile)
    {
        delete m_pBillFile;
    }
}

void CFileGroup::SetMustMerge(BOOL bMustMerge)
{
    if(NULL != m_pBillFile)
    {
        m_pBillFile->SetMustMerge(bMustMerge);
    }
}

void CFileGroup::SetFinalSPFileFmt(BOOL bFinalSPFileFmt)
{
    if(NULL != m_pBillFile)
    {
        m_pBillFile->SetFinalSPFileFmt(bFinalSPFileFmt);
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
BOOL CFileGroup::Open(int nFileNo, int nOffset)
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
void CFileGroup::GetCurPosition(int& nFileNo, int& nOffset)
{
    nFileNo = m_nCurFileNo;
    nOffset = m_nCurOffset;
}

/**********************************************************************
功能描述    获取下一张符合条件的话单记录数据的偏移地址
输入参数    无
输出参数    nStartFileNo----本张话单记录数据所在话单文件序号
            nStartOffset----本张话单记录数据在话单文件中的位置
            nBillLen---话单记录数据的长度
            bRemain----是否还有后续的数据，为FALSE时则pBillRecord无效
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CFileGroup::GetNextRecordOffset(int& nStartFileNo, int& nStartOffset,
                                     BOOL& bRemain)
{
    BOOL bRemainInFile = TRUE;
    while (1)
    {
        BOOL bSucc = m_pBillFile->GetNextBillOffset(m_nCurOffset, bRemainInFile);
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
            
            //by lx SWPD04804
            if (!bSucc && bRemainInGroup)
            {
                continue;
            }
            //end SWPD04804

            if (!bRemainInGroup)
            {
                bRemain = FALSE;
                return TRUE;
            }
        }
    }
}

/**********************************************************************
功能描述    获取指定文件指定位置的话单记录数据
输入参数    nStartFileNo----本张话单记录数据所在话单文件序号
            nStartOffset----本张话单记录数据在话单文件中的位置
输出参数    pBillRecord ----话单记录对象指针
            nBillLen    ----输出的话单长度
返回值      ERR_SUCCESS        ---成功
抛出异常    无
**********************************************************************/
int CFileGroup::GetRstRecord(int nStartFileNo,
                             int nStartOffset,
                             BYTE*& pBillRecord,
                             UINT4& uBillLen)
{
    if(m_nStartFileNo != nStartFileNo)
    {
        char szFileName[MAX_PATH];
        SNPRINTF(szFileName,
                 sizeof(szFileName),
                 "%s/%s",
                 m_szPath,
                 m_qFileNames[nStartFileNo].c_str());
        BOOL bRet = m_pBillFile->OpenRstFile(szFileName, nStartOffset);
        if(!bRet)
        {
            return ERR_FAIL;
        }
        m_nStartFileNo = nStartFileNo;
    }

    int nRet = m_pBillFile->GetRstRecord(nStartOffset,
                                         pBillRecord,
                                         uBillLen);

    return nRet;
}

/**********************************************************************
功能描述    准备话单文件，针对浏览任务
输入参数    szFileFullPath----包括全路径的话单文件名称
输出参数    无
返回值      成功与否
抛出异常    无
**********************************************************************/
BOOL CFileGroup::PrepareFile(const char* szFileFullPath)
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

    CacalTotalFileLength();

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
BOOL CFileGroup::PrepareFiles(const char* szPath, const char* szStartRange,
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

    CacalTotalFileLength();

    return TRUE;
}


/**********************************************************************
功能描述    准备话单文件，针对查询任务
输入参数    szPath----目录路径
            fileList----日期或者的带查询的多个文件列表
            uFlg----查询范围标志，用于区分查询范围是日期目录还是话单文件名称，
                    取值在QueryRangeType里定义
            szQueryCond----查询条件字符串
输出参数    无
返回值      成功与否
抛出异常    无
修改记录:
------------------------------------------------------------------------
问题单  SWPD05145       add by YanYong 2004-05-25
**********************************************************************/
BOOL CFileGroup::PrepareFiles(const char* szPath, LIST<STRING> &queryList, 
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
        LIST<STRING>& DateList = queryList;
        STRING szDateName;

        //如果得到的日期目录列表为空，说明目录在硬盘上已经不存在，此时返回失败
        if (DateList.empty())
        {
            return FALSE;
        }

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
        //获取目录下所有文文件
        FileList = queryList;
        STRING szFileName;
    
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

    CacalTotalFileLength();
    queryList.clear(); //清空列表.

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
BOOL CFileGroup::OpenNextFile(BOOL& bRemain)
{
    //更新已查询文件的总长度
    m_Mutex.acquire();
    m_nQueryedFileLen = m_nQueryedFileLen + m_pBillFile->GetFileLen();
    m_Mutex.release();
    m_pBillFile->SetOffset(0);

    //首先关闭当前的文件
    BOOL bSucc = m_pBillFile->Close();
    if (!bSucc)
    {
        return FALSE;
    }

    if (m_nCurFileNo == m_qFileNames.size() - 1)
    //如果当前已经是文件组中的最后一个文件，则直接返回
    {
        bRemain = FALSE;
        return TRUE;
    }
    else
    //否则打开下一个文件，并刷新当前位置
    {
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

INT8 CFileGroup::GetQueryedLength()
{

    INT8 nQueryFileLen = m_pBillFile->GetCurOffset();

    m_Mutex.acquire();
    nQueryFileLen += m_nQueryedFileLen;
    m_Mutex.release();

    return nQueryFileLen;
}

INT8 CFileGroup::CacalTotalFileLength()
{
    FILE* pFile = NULL;
    INT8  nTotalLen = 0;
    VECTOR<STRING>::iterator it;

    char szFilePath[MAX_PATH];
    it = m_qFileNames.begin();

    while (it != m_qFileNames.end())
    {
        sprintf(szFilePath, "%s/%s", m_szPath, (*it).c_str());
        nTotalLen += FileSize(szFilePath);
        it++;
    }

    m_nTotalFileLength = nTotalLen;

    return nTotalLen;
}

INT8 CFileGroup::GetTotalFileLength()
{
    return m_nTotalFileLength;
}

VECTOR<STRING> CFileGroup::GetFileNames()
{
    return m_qFileNames;
}

STRING CFileGroup::GetFilesPath()
{
    return m_szPath;
}

