#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "../include/msg_def.h"
#include "../frontsave/front_save.h"
#include "../backsave/save_chanl.h"
#include "net_info_file.h"
#include "save_info_file.h"
#include "back_save_info_file.h"
#include "msc_sync_data.h"


/********************************************************
* 名称: CMscSyncData::CMscSyncData
* 功能: 构造函数
* 输入: nAccessPoint : 接入点号
* 输出:
* 返回:
********************************************************/
CMscSyncData::CMscSyncData(int nAccessPoint)
:CSyncData(nAccessPoint)
{
    m_uFrontSavePID = 0;
    m_uFrontFileCsn = 0;
    m_uNetworkPID   = 0;
}

/********************************************************
* 名称: CMscSyncData::~CMscSyncData
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CMscSyncData::~CMscSyncData()
{
    //
}

/********************************************************
* 名称: CMscSyncData::InitInstance
* 功能: 初始化操作
* 输入:
* 输出:
* 返回: int 初始化操作是否成功，0表示成功、非零值表示失败
********************************************************/
int CMscSyncData::InitInstance()
{
    //根据本接入点号，获取通道数。
    STRING strIGWBPath = STRING(GetAppPath()) + "/"
                         + "config" + "/" + "format";
    CIGWB IGWB(strIGWBPath);
    IBillProcessor* pIBillProcessor =
        (IBillProcessor* )IGWB.QueryInterface(IID_FP_IBILLPROCESSOR);
    if ((NULL == pIBillProcessor) || (0 != pIBillProcessor->Init(m_nAccessPoint)))
    {
        return ERR_GET_CHL_NUM;
    }
    //获得通道数，必须包括缺省通道0
    LIST<STRING> ChannelNameList;
    int uChannelCount  = pIBillProcessor->GetChannelNames(ChannelNameList) + 1;

    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();
    char szAPSec[25];
    sprintf(szAPSec, "%s%d", CFG_SEC_AP, m_nAccessPoint);

    //从对应接入点节的CFG_AP_NAME中获取接入点名
    char szAPName[MAX_PATH];
    IniFile.GetString(szAPSec, CFG_AP_NAME, szAPSec,
                        szAPName, MAX_PATH);
    szAPName[MAX_PATH - 1] = '\0';

    //得到并创建主状态文件的根目录
    char szMainStatusRootDir[MAX_PATH];
    IniFile.GetString(CFG_SEC_DISK_FILE, CFG_MAIN_STATUS_ROOT_DIR,
                        CFG_MAIN_STATUS_ROOT_DIR_DEFAULT,
                        szMainStatusRootDir, MAX_PATH);
    szMainStatusRootDir[MAX_PATH - 1] = '\0';
    if((szMainStatusRootDir[strlen(szMainStatusRootDir) - 1] == '\\') ||
       (szMainStatusRootDir[strlen(szMainStatusRootDir) - 1] == '/'))
    {
        szMainStatusRootDir[strlen(szMainStatusRootDir) - 1] = '\0';
    }
    strncat(szMainStatusRootDir, "/", MAX_PATH);
    strncat(szMainStatusRootDir, szAPName, MAX_PATH);
    szMainStatusRootDir[MAX_PATH - 1] = '\0';

    //得到并创建备状态文件的根目录
    char szBakStatusRootDir[MAX_PATH];
    IniFile.GetString(CFG_SEC_DISK_FILE, CFG_BAK_STATUS_ROOT_DIR,
                        CFG_BAK_STATUS_ROOT_DIR_DEFAULT,
                        szBakStatusRootDir, MAX_PATH);
    szBakStatusRootDir[MAX_PATH - 1] = '\0';

    if((szBakStatusRootDir[strlen(szBakStatusRootDir) - 1] == '\\') ||
       (szBakStatusRootDir[strlen(szBakStatusRootDir) - 1] == '/'))
    {
        szBakStatusRootDir[strlen(szBakStatusRootDir) - 1] = '\0';
    }
    strncat(szBakStatusRootDir, "/", MAX_PATH);
    strncat(szBakStatusRootDir, szAPName, MAX_PATH);
    szBakStatusRootDir[MAX_PATH - 1] = '\0';

    //得到网络模块状态文件的存放目录为网络模块状态文件的根目录+"network"
    char szNetStatusPath[MAX_PATH];
    SNPRINTF(szNetStatusPath, MAX_PATH, "%s/network", szMainStatusRootDir);
    szNetStatusPath[MAX_PATH - 1] = '\0';
    CreateDeepDir(szNetStatusPath);

    //得到存盘模块主状态文件的存放目录为主状态文件的根目录+"save"；
    char szMainSaveStatusPath[MAX_PATH];
    SNPRINTF(szMainSaveStatusPath, MAX_PATH, "%s/save", szMainStatusRootDir);
    szMainSaveStatusPath[MAX_PATH - 1] = '\0';
    CreateDeepDir(szMainSaveStatusPath);

    //得到存盘模块备状态文件的存放目录为备状态文件的根目录+"save"；
    char szBakSaveStatusPath[MAX_PATH];
    SNPRINTF(szBakSaveStatusPath, MAX_PATH, "%s/save", szBakStatusRootDir);
    szBakSaveStatusPath[MAX_PATH - 1] = '\0';
    CreateDeepDir(szBakSaveStatusPath);

    //分别创建网络模块、前存盘及后存盘各通道的同步状态文件读写对象，
    //并将其指针加入m_SyncInfoFileList队列
    m_SyncInfoFileList.push_back(new CNetInfoFile(szNetStatusPath,
                                                  "status.dat",
                                                  m_nAccessPoint,
                                                  MT_NETWORK,
                                                  NETWORK_CHANNEL_ID));

    //设置主备状态文件的相关参数
    char szMainStatusName[MAX_PATH], szBakStatusName[MAX_PATH];
    sprintf(szMainStatusName, "%s_FS_MAIN.stf", szAPName);
    sprintf(szBakStatusName, "%s_FS_BAK.stf", szAPName);

    CSaveInfoFile* pInfoFile = NULL;
    pInfoFile = new CSaveInfoFile(szMainSaveStatusPath,
                                  szMainStatusName,
                                  szBakSaveStatusPath,
                                  szBakStatusName,
                                  m_nAccessPoint,
                                  MT_FRONTSAVE,
                                  FRONTSAVE_CHANNEL_ID);
    //在状态头中填入各变量的缺省值，如果状态文件不存在，则将缺省值写入状态文件
    FS_STATUS_FILE_HEAD* pFSHead = new FS_STATUS_FILE_HEAD;
    memset(pFSHead, 0, sizeof(FS_STATUS_FILE_HEAD));
    pFSHead->uFileCsn = 0;
    pFSHead->uLastPID = 1;
    pFSHead->uStatusFileWritePos = sizeof(FS_STATUS_FILE_HEAD);
    pInfoFile->SetFileStruct(pFSHead, 
                             sizeof(FS_STATUS_FILE_HEAD), 
                             sizeof(FS_STATUS_FILE_ELEMENT),
                             MAX_UNIT_NUM);
    m_SyncInfoFileList.push_back(pInfoFile);

    for (int i = 0; i < uChannelCount; i++)
    {
        sprintf(szMainStatusName, "%s_BS_%02d_MAIN.stf", szAPName, i);
        sprintf(szBakStatusName, "%s_BS_%02d_BAK.stf", szAPName, i);
        pInfoFile = new CBackSaveInfoFile(szMainSaveStatusPath,
                                          szMainStatusName,
                                          szBakSaveStatusPath,
                                          szBakStatusName,
                                          m_nAccessPoint,
                                          MT_BACKSAVE,
                                          i);
        //在状态头中填入各变量的缺省值，如果状态文件不存在，则将缺省值写入状态文件
        BS_STATUS_FILE_HEAD* pBSHead = new BS_STATUS_FILE_HEAD;
        memset(pBSHead, 0, sizeof(BS_STATUS_FILE_HEAD));

		char szChlSection[40];
	    sprintf(szChlSection, CFG_SEC_CHANNEL, m_nAccessPoint, i);
		UINT4 uFileCsnStart, uFileCsnStep;
		uFileCsnStart = IniFile.GetInt(CFG_SEC_DISK_FILE, 
										 CFG_START_CSN, CFG_START_CSN_DEFAULT);
		uFileCsnStart = IniFile.GetInt(szAPSec,
										 CFG_START_CSN, uFileCsnStart);
		uFileCsnStart = IniFile.GetInt(szChlSection,
										 CFG_START_CSN, uFileCsnStart);
		uFileCsnStep = IniFile.GetInt(CFG_SEC_DISK_FILE, 
										   CFG_CSN_STEP, CFG_CSN_STEP_DEFAULT);
		uFileCsnStep = IniFile.GetInt(szAPSec,
										   CFG_CSN_STEP, uFileCsnStep);
		uFileCsnStep = IniFile.GetInt(szChlSection,
										   CFG_CSN_STEP, uFileCsnStep);
        pBSHead->uBillCsn = 1;
        pBSHead->uFileCsn = uFileCsnStart - uFileCsnStep;
        pBSHead->uLastPID = 1;
        pBSHead->uStatusFileWritePos = sizeof(BS_STATUS_FILE_HEAD);
        pInfoFile->SetFileStruct(pBSHead, 
                                 sizeof(BS_STATUS_FILE_HEAD),
                                 sizeof(BS_STATUS_FILE_ELEMENT),
                                 MAX_UNIT_NUM);
        m_SyncInfoFileList.push_back(pInfoFile);
    }

    m_tLastActiveTime = GetLastWriteTime();

    //调用Open函数，打开状态文件
    if(Open() != ERR_SUCCESS)
    {
        return ERR_FAIL;
    }

    //分配一个64KB大小的临时空间，将指针赋给pBuffer;
    char* pBuffer = new char[64 * 1024];

    //读出状态文件数据
    GetSyncData(pBuffer, 64 * 1024, FALSE);

    //查找模块号为MT_FRONTSAVE的同步数据，更新m_uFrontSavePID
    SMscSyncData* pMscSyncData = NULL;
    pMscSyncData = FindSyncData((SMscAPSyncFrame*)pBuffer, 
                                MT_FRONTSAVE, 
                                FRONTSAVE_CHANNEL_ID);
    if(NULL != pMscSyncData)
    {
        m_uFrontSavePID = ((SSaveSyncData*)pMscSyncData->Data)->uPID;
        m_uFrontFileCsn = ((SSaveSyncData*)pMscSyncData->Data)->uFileCsn;
    }

    //查找模块号为MT_NETWORK的同步数据，更新m_uNetworkPID
    pMscSyncData = FindSyncData((SMscAPSyncFrame*)pBuffer, 
                                MT_NETWORK, 
                                NETWORK_CHANNEL_ID);
    if(NULL != pMscSyncData)
    {
        m_uNetworkPID = ((SMscNetSyncData*)pMscSyncData->Data)->uPackId;
    }

    delete [] pBuffer;

    return ERR_SUCCESS;
}

/********************************************************
* 名称: CMscSyncData::GetLastWriteTime
* 功能: 获取上次写状态文件的时间
* 输入: 
* 输出: 
* 返回: time_t 返回上次激活的时间
********************************************************/
time_t CMscSyncData::GetLastWriteTime()
{
    //更新m_tLastActiveTime
    LIST<CSyncInfoFile*>::iterator iter;
    for (iter = m_SyncInfoFileList.begin();
         iter != m_SyncInfoFileList.end();
         iter++)
    {
        if((*iter)->GetModule() == MT_NETWORK)
        {
            return (*iter)->GetLastWriteTime();
        }
    }

    return 0;
}

/********************************************************
* 名称: CMscSyncData::GetSyncDataLen
* 功能: 返回已封装成SMscAPSyncFrame结构的同步数据的长度
* 输入:
* 输出:
* 返回: int 返回同步数据的长度
********************************************************/
int CMscSyncData::GetSyncDataLen()
{
    int nLen = 0;
    LIST<CSyncInfoFile*>::iterator i;

    for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
    {
        if((*i)->GetSyncDataLen() > 0)
        {
            nLen += (*i)->GetSyncDataLen() + MSC_SYNC_DATA_HEAD_LENGTH;
        }
    }
    
    if(nLen > 0)
    {
        nLen += MSC_AP_SYNC_FRAME_HEAD_LENGTH;
    }

    return nLen;
}

/********************************************************
* 名称: CMscSyncData::GetSyncData
* 功能: 获取同步数据操作
* 输入: pData     : 输出缓冲区的指针，封装成SMscAPSyncFrame
*                   结构的同步数据
*       nLen      : 输出缓冲区的长度
*       bReadFile : 是否从文件中读出新的数据，TRUE表示要从
*                   文件中读出，FALSE表示从缓冲区中读出
* 输出:
* 返回: int 返回实现得到的字节数
********************************************************/
int CMscSyncData::GetSyncData(void* pData, int nLen, BOOL bReadFile)
{
    errno = 0;

    if (nLen < GetSyncDataLen())
    {
        errno = ERR_BUFFER_NOT_ENOUGH;
        return 0;
    }

    int nBufferRead = MSC_AP_SYNC_FRAME_HEAD_LENGTH;
    int nRetLen     = 0;
    SMscSyncData* pSyncData;
    LIST<CSyncInfoFile*>::iterator i;
    for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
    {
        pSyncData = (SMscSyncData*)((char*)pData + nBufferRead);
        nRetLen = (*i)->GetSyncData((char*)pSyncData + 
                                    MSC_SYNC_DATA_HEAD_LENGTH, 
                                    nLen - nBufferRead - 
                                    MSC_SYNC_DATA_HEAD_LENGTH, 
                                    bReadFile);
        if(0 >= nRetLen)
        {
            if(ERR_BUFFER_NOT_ENOUGH == errno)
            {
                return 0;
            }
        }
        else
        {
            pSyncData->uMID     = (*i)->GetModule();
            pSyncData->uChannel = (*i)->GetChannel();
            pSyncData->uLength  = nRetLen;
            nBufferRead += MSC_SYNC_DATA_HEAD_LENGTH + nRetLen;
        }
    }
    nBufferRead -= MSC_AP_SYNC_FRAME_HEAD_LENGTH;

    if(nBufferRead > 0)
    {
        SMscAPSyncFrame* pSyncFrame = (SMscAPSyncFrame*) pData;

        if(bReadFile)
        {
            m_tLastActiveTime = GetLastWriteTime();
        }
        time_t tNow = time(NULL);
        if (tNow - m_tLastActiveTime < 0)
        {
            m_tLastActiveTime = tNow;
        }
        pSyncFrame->uAPointID   = m_nAccessPoint;
        pSyncFrame->uDiffTime   = tNow - m_tLastActiveTime;
        pSyncFrame->uDiffPackId = m_uNetworkPID - m_uFrontSavePID;
        pSyncFrame->uLen        = nBufferRead;
        nBufferRead            += MSC_AP_SYNC_FRAME_HEAD_LENGTH;
    }

    return nBufferRead;
}

/********************************************************
* 名称: CMscSyncData::PutSyncData
* 功能: 写入状态数据操作
* 输入: pData     : 输入缓冲区的指针 ，封装成SMscAPSyncFrame
*                   结构的同步数据
*       nLen      : 输入缓冲区的长度
* 输出:
* 返回: int 获得的实际写入的数据长度
********************************************************/
int CMscSyncData::PutSyncData(void* pData, int nLen)
{
    SMscAPSyncFrame* pFrame = (SMscAPSyncFrame*)pData;
    int nBufferWritten      = MSC_AP_SYNC_FRAME_HEAD_LENGTH;
    SMscSyncData* pSyncData = NULL;
    m_tLastActiveTime       = time(NULL);
    CSyncInfoFile* pInfoFile;

    while(nBufferWritten <= int(nLen - MSC_SYNC_DATA_HEAD_LENGTH))
    {
        pSyncData = (SMscSyncData*) ((char*)pFrame + nBufferWritten);

        if(0 == pSyncData->uLength)
        {
            nBufferWritten += MSC_SYNC_DATA_HEAD_LENGTH;
            continue;
        }

        //在m_SyncInfoFileList中查找模块号为pSyncData->uMID的CSyncInfoFile对象;
        LIST<CSyncInfoFile*>::iterator i;
        for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
        {
            pInfoFile = *i;
            if(pInfoFile->GetChannel() == pSyncData->uChannel)
            {
                switch(pSyncData->uMID)
                {
                case MT_NETWORK:
                    m_uNetworkPID = 
                        ((SMscNetSyncData*)pSyncData->Data)->uPackId;
                    break;

                case MT_FRONTSAVE:
                    m_uFrontSavePID = 
                        ((SSaveSyncData*)pSyncData->Data)->uPID;
                    m_uFrontFileCsn = 
                        ((SSaveSyncData*)pSyncData->Data)->uFileCsn;
                    break;

                default:
                    break;
                }
                pInfoFile->PutSyncData(pSyncData->Data, pSyncData->uLength);
            }
        }
        nBufferWritten += MSC_SYNC_DATA_HEAD_LENGTH + pSyncData->uLength;
    }

    if(MSC_AP_SYNC_FRAME_HEAD_LENGTH == nBufferWritten)
    {
        nBufferWritten = 0;
    }

    return nBufferWritten;
}

#define MID_OF_FILE_NUMBER 50000000 
/********************************************************
* 名称: CMscSyncData::DoSyncData
* 功能: 实施数据同步操作，只被同步方会调用
*       前后存盘模块状态数据的同步条件: 
*       同步数据发送方的前存盘文件序列号大于等于或远小于接收方
*       网络模块状态数据的同步条件:
*       同步数据发送方的前存盘文件序列号大于等于或远小于接收方，
*       且同步数据发送方的上次激活时间晚于接收方
* 输入: pData     : 输入缓冲区的指针 ，封装成SMscAPSyncFrame
*                   结构的同步数据
*       nLen      : 输入缓冲区的长度
* 输出:
* 返回: void
********************************************************/
void CMscSyncData::DoSyncData(void* pData, int nLen)
{
    SMscAPSyncFrame* pFrame = (SMscAPSyncFrame*)pData;
    int nBufferWritten      = MSC_AP_SYNC_FRAME_HEAD_LENGTH;
    LIST<CSyncInfoFile*>::iterator i;
    CSyncInfoFile* pInfoFile;
    BOOL bDoSyncSaveData = FALSE;

    //查找输入数据中前存盘模块的同步帧指针
    SMscSyncData* pSyncData = NULL;
    pSyncData = FindSyncData((SMscAPSyncFrame*)pData, 
                              MT_FRONTSAVE, 
                              FRONTSAVE_CHANNEL_ID);
    if(NULL != pSyncData)
    {
        //取出输入数据中前存盘模块的文件序列号
        UINT4 uFrontFileCsn = ((SSaveSyncData*)pSyncData->Data)->uFileCsn;

        //如果输入数据的前存盘文件序列号大于等于本端的前存盘文件序列号，
        //或者输入数据的前存盘文件序列号远远小于本端的前存盘文件序列号
        //则置存盘模块可同步标志为真
        if((uFrontFileCsn >= m_uFrontFileCsn) ||
           (uFrontFileCsn + MID_OF_FILE_NUMBER < m_uFrontFileCsn))
        {
            bDoSyncSaveData = TRUE;
        }
    }

    //如果前存盘文件序列号不满足同步要求，则退出
    if(!bDoSyncSaveData)
    {
        return;
    }

    time_t tNow = time(NULL);
    if (tNow < m_tLastActiveTime)
    {
        m_tLastActiveTime = tNow;
    }

    pSyncData = NULL;
    while(nBufferWritten <= int(nLen - MSC_SYNC_DATA_HEAD_LENGTH))
    {
        pSyncData = (SMscSyncData*) ((char*)pFrame + nBufferWritten);
        if(0 == pSyncData->uLength)
        {
            nBufferWritten += MSC_SYNC_DATA_HEAD_LENGTH;
            continue;
        }

        for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
        {
            pInfoFile = *i;
            if(pInfoFile->GetChannel() == pSyncData->uChannel)
            {
                switch (pSyncData->uMID)
                {
                case MT_NETWORK:
                    if((pFrame->uDiffTime == UINT4(-1)) ||
                       (long(pFrame->uDiffTime) + m_tLastActiveTime < tNow))
                    {
                        m_uNetworkPID = pFrame->uDiffPackId + m_uFrontSavePID;
                        ((SMscNetSyncData*)pSyncData->Data)->uPackId = m_uNetworkPID;
                        /*
                            当bWaitForResponse为FALSE时，正常时网络模块与前存盘的包号
                            应该相等，如果此时将bWaitForResponse置为TRUE，网络模块初
                            始化时只会补发应答帧，不会对流程产生影响，因此此处将
                            bWaitForResponse恒置为TRUE
                        */
                        ((SMscNetSyncData*)pSyncData->Data)->bWaitForResponse = TRUE;
                        pInfoFile->PutSyncData(pSyncData->Data, pSyncData->uLength);
                    }
                    break;

                case MT_FRONTSAVE:
                case MT_BACKSAVE:
                    pInfoFile->PutSyncData(pSyncData->Data, pSyncData->uLength);
                    break;

                default:
                    break;
                }
            }
        }
        nBufferWritten += MSC_SYNC_DATA_HEAD_LENGTH + pSyncData->uLength;
    }

    //调用所有同步状态文件读写对象的Flush函数，刷新状态文件
    for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
    {
        (*i)->Flush();
    }
}

/********************************************************
* 名称: CMscSyncData::FindSyncData
* 功能: 查找指定模块号及通道号的同步数据块指针
* 输入: pAPFrame : 源数据区指针
*       uMID     : 模块号
*       uChannel : 通道号
* 输出: 
* 返回: SMscSyncData* nChl对应通道号的同步数据块指针
********************************************************/
SMscSyncData* CMscSyncData::FindSyncData(SMscAPSyncFrame* pAPFrame, 
                                         UINT4 uMID, UINT4 uChannel)
{
    SMscSyncData* pMscSyncData = (SMscSyncData*)pAPFrame->Data;
    int nLen = pAPFrame->uLen;

    while(nLen >= int(MSC_SYNC_DATA_HEAD_LENGTH))
    {
        if((uMID == pMscSyncData->uMID) && 
           (uChannel == pMscSyncData->uChannel) &&
           (pMscSyncData->uLength > 0))
        {
            return pMscSyncData;
        }
        nLen -= MSC_SYNC_DATA_HEAD_LENGTH + pMscSyncData->uLength;
        pMscSyncData = (SMscSyncData *)((char*)pMscSyncData + 
                       MSC_SYNC_DATA_HEAD_LENGTH + 
                       pMscSyncData->uLength);
    }

    return NULL;
}
