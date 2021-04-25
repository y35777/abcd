#include "../../include/base_type.h"
#include "ftp_protocol.h"
#include "global_fun.h"
#include "../../include/toolbox.h"
#include "../include/adapter_include.h"


// 构造函数
CFtpProtocol::CFtpProtocol()
{
    m_nMode      = -1;
    m_bConnected = FALSE;
}

//析构函数
CFtpProtocol::~CFtpProtocol()
{
    //
}

//初始化本地文件协议服务, 是虚函数, 由派生类具体实现该方法
int CFtpProtocol::InitProtocol()
{
	m_FtpClient.SetPassive(CCollectorAdapter::GetFtpPasvMode());
    return  ERR_SUCCESS;
}

//连接文件服务端
int CFtpProtocol::Connect()
{
    m_Mutex.acquire();
    m_nStatus = m_FtpClient.Connect(m_szSvrName,
                                    m_szSvrUsr,
                                    m_szSvrPwd,
                                    m_szLocalName);

    if((ERR_SUCCESS == m_nStatus) && (m_nMode != -1))
    {
        m_nStatus = m_FtpClient.SetTransMode(m_nMode);
    }

    if(ERR_SUCCESS != m_nStatus)
    {
        m_FtpClient.Disconnect();
        m_bConnected = FALSE;
        m_Mutex.release();
    }
    else
    {
        m_bConnected = TRUE;
    }

    return m_nStatus;
}

//断开连接
int CFtpProtocol::Disconnect()
{
    if(m_bConnected)
    {
        m_FtpClient.Disconnect();
        m_bConnected = FALSE;

        m_Mutex.release();
    }
    m_nStatus = ERR_FAIL;

    return ERR_SUCCESS;
}

int CFtpProtocol::TryConnect()
{
    if(m_bConnected)
    {
        return ERR_SUCCESS;
    }

    int nRet = Connect();
    if(nRet == ERR_SUCCESS)
    {
        Disconnect();
    }

    return nRet;
}

//设置文件传输模式
int CFtpProtocol::SetMode(int nMode)
{
    m_nMode   = nMode;
    m_nStatus = m_FtpClient.SetTransMode(nMode);

    return m_nStatus;
}

//实现拷贝文件服务端的单个文件到本地, 虚函数, 由派生类具体实现
int CFtpProtocol::GetFile(const SGetFileReq &pReq)
{
    int nRet = ERR_FAIL;

    //设置当前目录
    if(pReq.szSrcPath[0] != '\0')
    {
        nRet = m_FtpClient.SetDirectory(pReq.szSrcPath);
        if(ERR_SUCCESS != nRet)
        {
            COTRACE("SetDirectory(%s) in GetFile failed.(ErrCode=%d)",
                  pReq.szSrcPath,
                  nRet);
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                  "SetDirectory(%s) in GetFile failed.(ErrCode=%d)",
                  pReq.szSrcPath,
                  nRet);
            return nRet;
        }
    }

    if(pReq.szDstPath[0] != '\0')
    {
        CreateDeepDir(pReq.szDstPath);
    }

    STRING strFileName = pReq.szDstPath;
    strFileName += "/";
    strFileName += pReq.szDstName;

    nRet = m_FtpClient.GetFile(strFileName.c_str(),
                               pReq.szSrcName);
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("GetFile(%s) failed.(ErrCode=%d)", pReq.szSrcName, nRet);
    }

    m_nStatus = nRet;

    return nRet;
}

//获得文件服务器当前目录下的文件列表, 虚函数, 由派生类具体实现该类
int CFtpProtocol::GetFileList(const char* szSrcDir, SListOut &pOut )
{
    int nRet = ERR_FAIL;

    if(szSrcDir[0] != '\0')
    {
        nRet = m_FtpClient.SetDirectory(szSrcDir);
        if(ERR_SUCCESS != nRet)
        {
            COTRACE("SetDirectory(%s) in GetFileList failed.(ErrCode=%d)",
                  szSrcDir,
                  nRet);
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                  "SetDirectory(%s) in GetFileList failed.(ErrCode=%d)",
                  szSrcDir,
                  nRet);
            return nRet;
        }
    }

    FTP_FILELIST file_list;
    nRet = m_FtpClient.ListDirectory(szSrcDir, file_list);
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("GetFileList(%s) failed.(ErrCode=%d)",
              szSrcDir,
              nRet);
        return nRet;
    }

    SFtpFile file_msg;
    SFileInfo file_info;
    while(file_list.size() > 0)
    {
        file_msg = file_list.front();
        file_list.pop_front();
        SNPRINTF(file_info.szFileName,
                 sizeof(file_info.szFileName),
                 "%s/%s",
                 szSrcDir,
                 file_msg.szFileName);
        file_info.szFileName[sizeof(file_info.szFileName) - 1] = '\0';
        file_info.nFileSize = file_msg.nFileSize; //added by chenliangwei
        file_info.nCollectStatus = CS_NOT_COLLECTED;
        strncpy(file_info.szModTime, file_msg.szDate, sizeof(file_info.szModTime));
        file_info.szModTime[sizeof(file_info.szModTime) - 1] = '\0';

        switch (file_msg.nAttr)
        {
        case FTP_FILE_ATTRIBUTE_DIRECTORY:
            if(strcmp(file_msg.szFileName, ".") != 0
               && strcmp(file_msg.szFileName, "..") != 0)
            {
                GetFileList(file_info.szFileName, pOut);
            }
            break;

        case FTP_FILE_ATTRIBUTE_NORMAL:
            file_info.nAttr = ATTR_FILE;
            pOut.FileList.push_back(file_info);
            break;

        default:
            break;
        }
    }

    m_nStatus = nRet;

    return nRet;
}

//删除文件服务端的指定文件
int CFtpProtocol::DelFile(const char* szFileName)
{
    int nRet = ERR_FAIL;

    STRING strPath = "";
    STRING strFile = "";
    divide_file_path_name(szFileName, strPath, strFile);
    if(strPath != "")
    {
        nRet = m_FtpClient.SetDirectory(strPath.c_str());
        if(ERR_SUCCESS != nRet)
        {
            COTRACE("SetDirectory(%s) in DelFile failed.(ErrCode=%d)",
                  strPath.c_str(),
                  nRet);
            return nRet;
        }
    }

    nRet = m_FtpClient.DeleteFile(strFile.c_str());
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("DelFile(%s) failed.(ErrCode=%d)",
              szFileName, nRet);
    }

    m_nStatus = nRet;

    return nRet;
}

//重命名指定文件
int CFtpProtocol::RenameFile( const char *szOld, const char *szNew )
{
    int nRet = ERR_FAIL;

    STRING strPath    = "";
    STRING strOldFile = "";
    STRING strNewFile = "";
    divide_file_path_name(szNew, strPath, strNewFile);
    divide_file_path_name(szOld, strPath, strOldFile);
    if(strPath != "")
    {
        nRet = m_FtpClient.SetDirectory(strPath.c_str());
        if(ERR_SUCCESS != nRet)
        {
            COTRACE("SetDirectory(%s) in RenameFile failed.(ErrCode=%d)",
                  strPath.c_str(),
                  nRet);
            return nRet;
        }
    }
    nRet = m_FtpClient.RenameFile(strOldFile.c_str(),
                                  strNewFile.c_str());
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("RenameFile(%s - %s) failed.(ErrCode=%d)", szOld, szNew, nRet);
    }

    m_nStatus = nRet;

    return nRet;
}

