#include "perf_bin_util.h"
#include "../include/toolbox.h"


CPerfBinUtil::CPerfBinUtil()
{
    m_pPerfFile = NULL;
    m_strFileName = "";
}

CPerfBinUtil::~CPerfBinUtil()
{
    CloseFile();
}

//剖析并生成DOM文档树，文件不存在则生成指定文件
int CPerfBinUtil::OpenFile(const char * const szFilePath)
{
    CloseFile();
    
    if(szFilePath != NULL)
    {
        m_strFileName = szFilePath;
        m_pPerfFile = fopen(szFilePath, "rb+");
        if ( m_pPerfFile != NULL)
        {
            int nRet = fseek(m_pPerfFile, 0, SEEK_END);
            if ( nRet != 0) 
            {
                CloseFile();
                return -1;
            }
        }
        else 
        {
            m_pPerfFile = fopen(szFilePath,"wb+");
            if(NULL == m_pPerfFile)
            {
                return -1;
            }
        }

        return 0;
    }

    return -1;
}

//关闭文件
int CPerfBinUtil::CloseFile(void)
{
    if(NULL != m_pPerfFile)
    {
        fclose(m_pPerfFile);
        m_pPerfFile = NULL;
    }

    return 0;
}

//保存文件
int CPerfBinUtil::Flush(void)
{
    //写文件
    if(NULL == m_pPerfFile)
    {
        return -1;
    }
    
    if (fflush(m_pPerfFile) != 0)
    {
        return -1;
    }

    return 0;
}

//追加一条性能信息
int CPerfBinUtil::AppendPerfItem(SPerfItem& perfItem)
{
    if(NULL == m_pPerfFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pPerfFile)
    {
        return ERR_FAIL;
    }
    
    int nRet = fwrite(&perfItem, sizeof(perfItem), 1, m_pPerfFile);
    if(nRet != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
