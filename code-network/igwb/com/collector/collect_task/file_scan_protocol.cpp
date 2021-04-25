#include "../../include/toolbox.h"
#include "file_scan_protocol.h"
#include "../include/adapter_include.h"


// ���캯��
CFileScanProtocol::CFileScanProtocol()
{
    //
}

//��������
CFileScanProtocol::~CFileScanProtocol()
{
    //
}

//ʵ�ֿ����ļ�����˵ĵ����ļ�������, �麯��, �����������ʵ��
int CFileScanProtocol::GetFile(const SGetFileReq &pReq)
{
    if(pReq.szDstPath[0] != '\0')
    {
        CreateDeepDir(pReq.szDstPath);
    }

    STRING strSrcFileName = pReq.szSrcPath;
    strSrcFileName += "/";
    strSrcFileName += pReq.szSrcName;

    STRING strDstFileName = pReq.szDstPath;
    strDstFileName += "/";
    strDstFileName += pReq.szDstName;

    BOOL bRet = FileCopy(strSrcFileName.c_str(), strDstFileName.c_str());
    if(!bRet)
    {
        TRACE("GetFile(%s) failed.", pReq.szSrcName);
        m_nStatus = ERR_FAIL;

        return m_nStatus;
    }

    m_nStatus = ERR_SUCCESS;

    return m_nStatus;
}

//����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
int CFileScanProtocol::GetFileList(const char* szSrcDir, SListOut &pOut )
{
    ACE_DIR *pDir = NULL;
    dirent *pDirent = NULL;
    ACE_stat sDirStat;

    //���ļ�Ŀ¼,��szPathĿ¼
    pDir = ACE_OS::opendir(szSrcDir);
    if(pDir == NULL)
    {
        TRACE("��Ŀ¼(%s)ʧ��", szSrcDir);
        m_nStatus = ERR_FAIL;
        return ERR_FAIL;
    }

    SFileInfo Node;
    char szFilePath[MAX_PATH];
    while((pDirent = ACE_OS::readdir(pDir)) != NULL)
    {
        //�γ�Դ�ļ�·��
        SNPRINTF(szFilePath,MAX_PATH,"%s/%s",szSrcDir,pDirent->d_name);
        szFilePath[MAX_PATH - 1] = 0;
        
        ACE_OS::stat(szFilePath, &sDirStat);
        if(S_IFDIR == (sDirStat.st_mode & S_IFMT))
        {
            if(strcmp(pDirent->d_name, ".") != 0
               && strcmp(pDirent->d_name, "..") != 0)
            {
                GetFileList(pDirent->d_name, pOut);
            }
            else
            {
                continue;
            }
        }
        
        //�ļ���,ʱ��
        tm tmTime = *localtime(&sDirStat.st_mtime);
        strftime(Node.szModTime,
                 sizeof(Node.szModTime),
                 "%Y%m%d",
                 &tmTime);
        Node.szModTime[sizeof(Node.szModTime) - 1] = '\0';
        Node.nFileSize = sDirStat.st_size;
        Node.nAttr     = ATTR_FILE;
        strncpy(Node.szFileName,pDirent->d_name,MAX_PATH);
        Node.szFileName[MAX_PATH-1] = 0;
        Node.nCollectStatus = CS_NOT_COLLECTED;
        pOut.FileList.push_back(Node);
    }
    ACE_OS::closedir(pDir);

    m_nStatus = ERR_SUCCESS;

    return ERR_SUCCESS;
}

//ɾ���ļ�����˵�ָ���ļ�
int CFileScanProtocol::DelFile(const char* szFileName)
{
    int nRet = ERR_FAIL;

    nRet = remove(szFileName);
    if(ERR_SUCCESS != nRet)
    {
        TRACE("DelFile(%s) failed.(ErrCode=%d)",
              szFileName, nRet);
    }

    m_nStatus = nRet;

    return nRet;
}

//������ָ���ļ�
int CFileScanProtocol::RenameFile( const char *szOld, const char *szNew )
{
    int nRet = ERR_FAIL;

    nRet = FileMove(szOld, szNew);
    if(ERR_SUCCESS != nRet)
    {
        TRACE("RenameFile(%s - %s) failed.(ErrCode=%d)", szOld, szNew, nRet);
    }

    m_nStatus = nRet;

    return nRet;
}

