#include "../include/toolbox.h"
#include "../include/frame.h"
#include "cdr_stat_file.h"
#include "stat_file_group.h"
#include "cdr_stat_svr.h"

/*******************************************************************
函数名称：CCDRStatFileGroup()
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
CCDRStatFileGroup::CCDRStatFileGroup()
{
    m_nCurFileNo   = 0;
    m_pCDRStatFile = NULL;
    m_szPath[0]    = '\0';
}
/*******************************************************************
函数名称：~CCDRStatFileGroup()
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
CCDRStatFileGroup::~CCDRStatFileGroup()
{
    if (m_pCDRStatFile != NULL)
    {
        delete  m_pCDRStatFile;
        m_pCDRStatFile = NULL;
    }
}


/*******************************************************************
函数名称：Init(UINT4 uAPType, UINT4 uBillType, BOOL bMustMerge, UINT4 uBillLen)
功能描述：设置参数，进行初始化操作
输入参数：UINT4 uAPType, 接入点类型
          UINT4 uBillType, 文件类型
          BOOL bMustMerge, 是否合并标志
          UINT4 uBillLen,话单长度
输出参数：无
返回内容：成功ERR_SUCCESS,否则返回ERR_FAIL
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatFileGroup::Init(UINT4 uAPType,
                            UINT4 uBillType,
                            BOOL bMustMerge,
                            BOOL bFinalSPFileFmt,
                            UINT4 uBillLen, 
                            int nAPID)
{
    m_pCDRStatFile = new CCDRStatFile;
    m_pCDRStatFile->Init(uAPType,
                         uBillType,
                         bMustMerge,
                         bFinalSPFileFmt,
                         uBillLen,
                         nAPID);

    return ERR_SUCCESS;
}

/*******************************************************************
函数名称：PrepareFiles(LIST<char*> listPath)
功能描述：准备指定日期范围内的话单文件
输入参数：const char* const szPath, 文件路径
输出参数：无
返回内容：成功ERR_SUCCESS,否则返回ERR_FAIL
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
int CCDRStatFileGroup::PrepareFiles(const char* const szPath)
{
    m_FileNames.clear();
    strcpy(m_szPath, szPath);

    //符合条件的文件名的列表
    LIST<STRING> FileList;

    //获取目录下排过序的所有文件，然后从中得到范围内的所有文件
    CCDRStatSvr::GetBillFiles(szPath, FileList);

/*
    //如果得到的文件列表为空，说明起始文件或终止文件
    //在硬盘上已经不存在，此时返回失败
    if (FileList.empty())
    {
        return ERR_FAIL;
    }
*/

    //将得到的文件列表导入到m_qFileNames中
    LIST<STRING>::iterator i;
    for (i = FileList.begin(); i != FileList.end(); i++)
    {
        m_FileNames.push_back(*i);
    }

    return ERR_SUCCESS;
}

/*******************************************************************
函数名称：GetNextRecord(BYTE*& pCDRRecord,  UINT4& nCDRLen, BOOL&  bRemain)
功能描述：获取下一张话单记录数据
输入参数：无
输出参数：BYTE*& pCDRRecord, 获得话单内容
          UINT4& nCDRLen, 此张话单的长度
          BOOL&  bRemain, 是否还有话单
返回内容：成功TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatFileGroup::GetNextRecord(BYTE*& pCDRRecord,
                                      UINT4& nCDRLen,
                                      BOOL&  bRemain)
{
    BOOL bRemainInFile;
    while (1)
    {
        BOOL bSucc = m_pCDRStatFile->GetNextBill(pCDRRecord,
                                                 nCDRLen,
                                                 bRemainInFile);
        if (!bSucc)
        {
            return FALSE;
        }

        if (bRemainInFile)  //在当前的文件中找到了话单记录
        {
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

    return TRUE;
}

/*******************************************************************
函数名称：Open(int nFileNo)
功能描述：打开指定的话单文件
输入参数：int nFileNo,文件号
输出参数：无
返回内容：打开成功返回TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatFileGroup::Open(int nFileNo)
{
    ASSERT(NULL != m_pCDRStatFile);

    BOOL bSucc = FALSE;

    if(m_FileNames.size() == 0)
    {
        return FALSE;
    }

    //关闭当前文件，打开指定文件，并设置写位置
    bSucc = m_pCDRStatFile->Close();
    if (!bSucc)
    {
        return FALSE;
    }

    char szFileName[MAX_PATH], szFileFullPath[MAX_PATH];
    strcpy(szFileName, m_FileNames[m_nCurFileNo].c_str());
    sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName);
    bSucc = m_pCDRStatFile->Open(szFileFullPath);
    if (!bSucc)
    {
         return FALSE;
    }
    m_nCurFileNo = nFileNo;

    return TRUE;
}

/*******************************************************************
函数名称：OpenNextFile(BOOL& bRemain)
功能描述：打开下一个话单文件
输入参数：无
输出参数：BOOL& bRemain,是否还有文件
返回内容：打开成功返回TRUE,否则返回FALSE
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
BOOL CCDRStatFileGroup::OpenNextFile(BOOL& bRemain)
{
    if (m_nCurFileNo >= m_FileNames.size() - 1)
    //如果当前已经是文件组中的最后一个文件，则直接返回
    {
        bRemain = FALSE;
        return TRUE;
    }
    else
    //否则打开下一个文件，并刷新当前位置
    {
        //首先关闭当前的文件
        BOOL bSucc = m_pCDRStatFile->Close();
        if (!bSucc)
        {
            return FALSE;
        }

        bRemain = TRUE;
        m_nCurFileNo++;

        char szFileName[MAX_PATH], szFileFullPath[MAX_PATH];
        strcpy(szFileName, m_FileNames[m_nCurFileNo].c_str());
        sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName);
        bSucc = m_pCDRStatFile->Open(szFileFullPath);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*******************************************************************
函数名称：TransQueryCond(const char* szCond,
                                const char* szBeginDate,
                                const char* szEndDate, char* szExCond)
功能描述：对话单查询条件进行特殊处理
          由于话单台超过来的查询条件中，开始时间的查询条件只包含时间域，
            但是在格式库的判断中，开始时间却必须包含日期和时间域。所以，
            必须将话单台发送的消息中的开始日期和结束日期分别加入到开始时间
            的查询条件中去。开始时间的域名定死为"StartTime"，时间的形式如
            2001-11-05 21:00:03所示，从话单台发送的消息中的日期如20011105
            所示，所以日期的显示方式必须进行相应的转换。例如，从话单台返回的
            查询条件中，开始时间的查询条件为"StartTime >= 08:05:00"、
            "StartTime <= 18:05:00"，开始日期和结束日期分别为"20011101"、
            "20011103"，则修改后的开始时间查询条件为
            "StartTime >= 2001-11-01 08:05:00"和
            "StartTime <= 2001-11-03 18:05:00"

输入参数：const char* szCond,从话单台送过来的查询条件
          const char* szBeginDate,从话单台送过来的起始日期
          const char* szEndDate,从话单台送过来的结束日期
输出参数：char* szExCond,处理后的查询条件，空间在函数外申请和释放
返回内容：无
调用函数：
被调函数：
作者时间：maimaoshi,2003/02/13
------------------------------修改记录------------------------------
修改人          修改时间           修改内容



********************************************************************/
void CCDRStatFileGroup::TransQueryCond(const char* szCond,
                                       const char* szBeginDate,
                                       const char* szEndDate,
                                       char* szExCond)
{
    //修改开始日期和结束日期的格式
    char szExBeginDate[40], szExEndDate[40], szYear[10], szMon[10], szDay[10];
    memcpy(szYear, szBeginDate, 4);
    szYear[4] = '\0';
    memcpy(szMon, szBeginDate + 4, 2);
    szMon[2] = '\0';
    memcpy(szDay, szBeginDate + 6, 2);
    szDay[2] = '\0';
    sprintf(szExBeginDate, "%s-%s-%s ", szYear, szMon, szDay);
    memcpy(szYear, szEndDate, 4);
    szYear[4] = '\0';
    memcpy(szMon, szEndDate + 4, 2);
    szMon[2] = '\0';
    memcpy(szDay, szEndDate + 6, 2);
    szDay[2] = '\0';
    sprintf(szExEndDate, "%s-%s-%s ", szYear, szMon, szDay);

    //修改查询条件，将开始日期和结束日期插入

    char* pReadPos = (char* )szCond;    //进行字符串拷贝时的读指针
    szExCond[0] = '\0';
    char* pWritePos = (char* )szExCond; //进行字符串拷贝时的写指针
    char* pSchPos = (char* )szCond;     //用于字符串中查找的指针

/*
    char cOperator;                     //StartTime后所带的算子的第一个字符
    //在szCond中查找所有的StartTime，如果StartTime后的算子为>=或>，
    //则在其条件中加入szExBeginDate，否则加入szEndDate
    while (1)
    {
       pSchPos = (char* )strstr(pSchPos, "StartTime");
       if (NULL == pSchPos)
       {
           break;
       }
       else
       {
           pSchPos += 9;

           //查找StartTime所带的算子的第一个字符
           while (1)
           {
               if (' ' != *pSchPos)
               {
                   cOperator = *pSchPos;
                   break;
               }
               pSchPos++;
           }

           //将开始日期或结束日期的信息加入到查询条件中
           pSchPos = strchr(pSchPos, '@');
           ASSERT(NULL != pSchPos);

           memcpy(pWritePos, pReadPos, pSchPos - pReadPos + 1);
           pWritePos += pSchPos - pReadPos + 1;
           pReadPos += pSchPos - pReadPos + 1;
           if ('>' == cOperator)
           {
               memcpy(pWritePos, szExBeginDate, strlen(szExBeginDate));
               pWritePos += strlen(szExBeginDate);
           }
           else
           {
               ASSERT('<' == cOperator);
               memcpy(pWritePos, szExEndDate, strlen(szExEndDate));
               pWritePos += strlen(szExEndDate);
           }
       }
    }
*/

    *pWritePos = '\0';
    STRNCAT(szExCond, pReadPos, MAX_COND_LEN);
}

