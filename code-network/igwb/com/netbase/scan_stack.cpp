#include "../include/frame.h"
#include "net_status.h"
#include "protocol_stack.h"
#include "data_buffer.h"
#include "net_status.h"
#include "scan_stack.h"
#include "resource.h"


CScanStack::CScanStack(const char* const szPath, 
                       CNetApp* const pNetApp, 
                       UINT4 const uStackId)
                       :CProtocolStack(pNetApp, uStackId)
{
    m_nScanCounter = -1;
    m_bFailure     = FALSE;
    m_bSubmitFile  = FALSE;
    m_nSubmitLen   = 0;

    memset(&m_ScanStackInfo, 0, SCAN_STACK_INFO_LEN);
    m_ScanStackInfo.nStackID = uStackId;
    if(NULL != szPath)
    {
        strncpy(m_ScanStackInfo.szPathName, szPath, MAX_PATH);
        m_ScanStackInfo.szPathName[MAX_PATH - 1] = '\0';
    }

    //读取本进程对应的相关配置参数
    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, 
            CMsgProcessor::GetProcessType() - PT_AP_BASE + 1);
    m_nBillRecordSize = IniFile.GetInt(strSec, CFG_AP_KEY_BILL_REC_SIZE, 
                                       CFG_AP_KEY_BILL_REC_SIZE_DEFAULT);
    //获得文件扫描间隔时间
    m_nScanInv = IniFile.GetInt(strSec,
                                CFG_AP_KEY_SCAN_FILE_INV,
                                CFG_AP_KEY_SCAN_FILE_INV_DEFAULT);

    //获得低潮包大小
    m_uLowAtSize = IniFile.GetInt(strSec,
                                  CFG_AP_KEY_LOW_AT_SIZE,
                                  CFG_AP_KEY_LOW_AT_SIZE_DEFAULT);
    m_uLowAtSize *= 1024;

    m_uLowAtSize = int(m_uLowAtSize / m_nBillRecordSize) * m_nBillRecordSize;
    
    m_pSubmitBuffer = NULL;
    m_nSubmitBufLen = 0;

    m_pStatusInfo = new CStackStatusInfo(STACK_SCAN, uStackId);
}

CScanStack::~CScanStack()
{
    m_FileList.clear();

    if(m_pSubmitBuffer != NULL)
    {
        delete m_pSubmitBuffer;
        m_pSubmitBuffer = NULL;
    }
}

int CScanStack::LoadStatus(void)
{
    //调用*m_pStatusInfo对象的Get函数
    UINT4 uInputLen = m_pStatusInfo->Get((BYTE*)(&m_ScanStackInfo), 
                                         SCAN_STACK_INFO_LEN);

    //读状态文件不成功则返回失败
    if(uInputLen != SCAN_STACK_INFO_LEN) return ERR_FAIL;
    if(m_ScanStackInfo.szFileName[0] != '\0')
    {
        char szFilePath[MAX_PATH];
        SNPRINTF(szFilePath,MAX_PATH,"%s/%s",
                 m_ScanStackInfo.szPathName, 
                 m_ScanStackInfo.szFileName);
        szFilePath[MAX_PATH - 1] = 0;

        if(ACE_OS::access(szFilePath, F_OK) != 0)
        {
            m_ScanStackInfo.szFileName[0] = '\0';
            m_ScanStackInfo.nSavingOffset = 0;
            m_ScanStackInfo.nSavedOffset  = 0;
        }
        else
        {
            ACE_stat sDirStat;
            ACE_OS::stat(szFilePath, &sDirStat);
            SFileListInfo file_list;
            file_list.tFileTime   = sDirStat.st_mtime;
            file_list.strFilePath = m_ScanStackInfo.szFileName;
            m_FileList.push_back(file_list);
        }
    }

    return ERR_SUCCESS;
}

int CScanStack::SaveStatus(void)
{
    //调用*m_pStatusInfo对象的Set函数
    m_pStatusInfo->Set((BYTE*)(&m_ScanStackInfo), SCAN_STACK_INFO_LEN);
    return ERR_SUCCESS;
}

//返回本对象中当前已接收的且可以提交给前存盘模块
//保存的话单数据的字节数
int CScanStack::GetReadyDataSize(void)
{
    if(m_FileList.size() > 0)
    {
        return m_uLowAtSize;
    }

    return 0;
}

//返回本对象中当前已接收的且可以提交给前存盘模块
//保存的话单数据的记录数
int CScanStack::GetReadyRecordCount(void)
{
    if(m_FileList.size() > 0)
    {
        return 1;
    }

    return 0;
}


//将本协议栈可提交给前存盘模块保存的话单数据放入*pDataBuf对象中
void CScanStack::GetReadyData(CDataBuffer* const pDataBuf)
{
    if(m_FileList.size() > 0)
    {
        char szFilePath[MAX_PATH];
        SNPRINTF(szFilePath,MAX_PATH,"%s/%s",
                 m_ScanStackInfo.szPathName, 
                 m_FileList.begin()->strFilePath.c_str());
        szFilePath[MAX_PATH - 1] = 0;

        //打开文件
        FILE* f = fopen(szFilePath, "rb");
        if(f == NULL)
        {
            m_FileList.pop_back();
            m_ScanStackInfo.szFileName[0] = '\0';
            m_ScanStackInfo.nSavingOffset = 0;
            m_ScanStackInfo.nSavedOffset  = 0;
            return;
        }

        //确认文件读取长度
        int nFileSize = FileSize(f);
        int nReadBytes = min(nFileSize - m_ScanStackInfo.nSavingOffset, m_uLowAtSize);
        if(nReadBytes > m_nSubmitBufLen)
        {
            m_nSubmitBufLen = nReadBytes;
            if(NULL != m_pSubmitBuffer)
            {
                delete m_pSubmitBuffer;
            }
            m_pSubmitBuffer = new char[m_nSubmitBufLen];
        }

        int nRet = fseek(f, m_ScanStackInfo.nSavingOffset, SEEK_SET);
        if(nRet != 0)
        {
            m_FileList.pop_back();
            m_ScanStackInfo.szFileName[0] = '\0';
            m_ScanStackInfo.nSavingOffset = 0;
            m_ScanStackInfo.nSavedOffset  = 0;
            fclose(f);
            return;
        }

        //读取数据
        nRet = fread(m_pSubmitBuffer, nReadBytes, 1, f);
        fclose(f);
        if(nRet != 1)
        {
            m_FileList.pop_back();
            m_ScanStackInfo.szFileName[0] = '\0';
            m_ScanStackInfo.nSavingOffset = 0;
            m_ScanStackInfo.nSavedOffset  = 0;
            return;
        }

        //写缓冲区
        nRet = pDataBuf->Put(m_pSubmitBuffer, nReadBytes);
        if(nRet == ERR_SUCCESS)
        {
            strcpy(m_ScanStackInfo.szFileName, m_FileList.begin()->strFilePath.c_str());
            m_ScanStackInfo.nSavingOffset += nReadBytes;
        }
    }
}

void CScanStack::OnResponse(BOOL const bSaved)
{
    if(bSaved)
    {
        //未保存过话单，不用处理
        if(m_ScanStackInfo.szFileName[0] == '\0')
        {
            return;
        }

        //取当前话单文件长度
        char szFilePath[MAX_PATH];
        SNPRINTF(szFilePath,MAX_PATH,"%s/%s",
                 m_ScanStackInfo.szPathName, 
                 m_ScanStackInfo.szFileName);
        szFilePath[MAX_PATH - 1] = 0;

        ACE_stat sDirStat;
        ACE_OS::stat(szFilePath, &sDirStat);
        int nFileSize = sDirStat.st_size;

        //判断该文件是否已读完或不存在了
        if((ACE_OS::access(szFilePath, F_OK) != 0)
           || ((m_ScanStackInfo.szFileName[0] != '\0') 
               && (nFileSize == m_ScanStackInfo.nSavingOffset)))
        {
            remove(szFilePath);

            if(m_FileList.size() > 0)
            {
                m_FileList.pop_front();
            }

            //保证下一次秒事件能立即进行文件搜索
            if(m_FileList.size() == 0)
            {
                m_nScanCounter = m_nScanInv;
            }

            m_ScanStackInfo.szFileName[0] = '\0';
            m_ScanStackInfo.nSavingOffset = 0;
            m_ScanStackInfo.nSavedOffset  = 0;
        }
        else if(nFileSize < m_ScanStackInfo.nSavingOffset)
        {
            ASSERT(0);
        }
        m_ScanStackInfo.nSavedOffset = m_ScanStackInfo.nSavingOffset;
    }
    else
    {
        //当保证话单包失败时，要回退文件读取指针
        m_ScanStackInfo.nSavingOffset = m_ScanStackInfo.nSavedOffset;
    }
}


//秒事件处理函数
void CScanStack::Timer1Sec(void)
{
    if(m_nScanCounter++ >= m_nScanInv)
    {
        if(m_FileList.size() == 0) 
        {
            PrepareFiles(m_ScanStackInfo.szPathName);
        }

        m_nScanCounter = 0;
    }
}

//向对应的主机模块或GSN发送协议帧
int CScanStack::SendFrame2Ex(void* pFrame)
{
    return 0;
}

//搜索szPath目录中所有文件
int CScanStack::PrepareFiles(const char* const szPath)
{
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    //打开文件目录,打开szPath目录
    pDir = ACE_OS::opendir(szPath);
    if(pDir == NULL)
    {
        m_bFailure = FALSE;
        return ERR_FAIL;
    }

    m_FileList.clear();
    char szFilePath[MAX_PATH];
	while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        SNPRINTF(szFilePath,MAX_PATH,"%s/%s",
                szPath, pDirent->d_name);
        szFilePath[MAX_PATH - 1] = 0;

        ACE_OS::stat(szFilePath, &sDirStat);
        if(S_IFDIR == (sDirStat.st_mode & S_IFMT))
        {
            continue;
        }
/*
        if(S_IFDIR == (sDirStat.st_mode & S_IFMT))
        {
            if(strcmp(pDirent->d_name, ".") != 0
               && strcmp(pDirent->d_name, "..") != 0)
            {
                PrepareFiles(pDirent->d_name);
            }
            else
            {
                continue;
            }
        }
*/

        SFileListInfo file_list;
        file_list.tFileTime   = sDirStat.st_mtime;
        file_list.strFilePath = pDirent->d_name;
        m_FileList.push_back(file_list);
    }
	ACE_OS::closedir(pDir);

	m_FileList.sort();

	return ERR_SUCCESS;
}

//获取故障状态
BOOL CScanStack::GetFailureStatus()
{
    return m_bFailure;
}

