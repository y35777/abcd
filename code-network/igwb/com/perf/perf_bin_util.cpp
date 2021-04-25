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

//����������DOM�ĵ������ļ�������������ָ���ļ�
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

//�ر��ļ�
int CPerfBinUtil::CloseFile(void)
{
    if(NULL != m_pPerfFile)
    {
        fclose(m_pPerfFile);
        m_pPerfFile = NULL;
    }

    return 0;
}

//�����ļ�
int CPerfBinUtil::Flush(void)
{
    //д�ļ�
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

//׷��һ��������Ϣ
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
