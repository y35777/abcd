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
* ����: CMscSyncData::CMscSyncData
* ����: ���캯��
* ����: nAccessPoint : ������
* ���:
* ����:
********************************************************/
CMscSyncData::CMscSyncData(int nAccessPoint)
:CSyncData(nAccessPoint)
{
    m_uFrontSavePID = 0;
    m_uFrontFileCsn = 0;
    m_uNetworkPID   = 0;
}

/********************************************************
* ����: CMscSyncData::~CMscSyncData
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CMscSyncData::~CMscSyncData()
{
    //
}

/********************************************************
* ����: CMscSyncData::InitInstance
* ����: ��ʼ������
* ����:
* ���:
* ����: int ��ʼ�������Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CMscSyncData::InitInstance()
{
    //���ݱ������ţ���ȡͨ������
    STRING strIGWBPath = STRING(GetAppPath()) + "/"
                         + "config" + "/" + "format";
    CIGWB IGWB(strIGWBPath);
    IBillProcessor* pIBillProcessor =
        (IBillProcessor* )IGWB.QueryInterface(IID_FP_IBILLPROCESSOR);
    if ((NULL == pIBillProcessor) || (0 != pIBillProcessor->Init(m_nAccessPoint)))
    {
        return ERR_GET_CHL_NUM;
    }
    //���ͨ�������������ȱʡͨ��0
    LIST<STRING> ChannelNameList;
    int uChannelCount  = pIBillProcessor->GetChannelNames(ChannelNameList) + 1;

    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();
    char szAPSec[25];
    sprintf(szAPSec, "%s%d", CFG_SEC_AP, m_nAccessPoint);

    //�Ӷ�Ӧ�����ڵ�CFG_AP_NAME�л�ȡ�������
    char szAPName[MAX_PATH];
    IniFile.GetString(szAPSec, CFG_AP_NAME, szAPSec,
                        szAPName, MAX_PATH);
    szAPName[MAX_PATH - 1] = '\0';

    //�õ���������״̬�ļ��ĸ�Ŀ¼
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

    //�õ���������״̬�ļ��ĸ�Ŀ¼
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

    //�õ�����ģ��״̬�ļ��Ĵ��Ŀ¼Ϊ����ģ��״̬�ļ��ĸ�Ŀ¼+"network"
    char szNetStatusPath[MAX_PATH];
    SNPRINTF(szNetStatusPath, MAX_PATH, "%s/network", szMainStatusRootDir);
    szNetStatusPath[MAX_PATH - 1] = '\0';
    CreateDeepDir(szNetStatusPath);

    //�õ�����ģ����״̬�ļ��Ĵ��Ŀ¼Ϊ��״̬�ļ��ĸ�Ŀ¼+"save"��
    char szMainSaveStatusPath[MAX_PATH];
    SNPRINTF(szMainSaveStatusPath, MAX_PATH, "%s/save", szMainStatusRootDir);
    szMainSaveStatusPath[MAX_PATH - 1] = '\0';
    CreateDeepDir(szMainSaveStatusPath);

    //�õ�����ģ�鱸״̬�ļ��Ĵ��Ŀ¼Ϊ��״̬�ļ��ĸ�Ŀ¼+"save"��
    char szBakSaveStatusPath[MAX_PATH];
    SNPRINTF(szBakSaveStatusPath, MAX_PATH, "%s/save", szBakStatusRootDir);
    szBakSaveStatusPath[MAX_PATH - 1] = '\0';
    CreateDeepDir(szBakSaveStatusPath);

    //�ֱ𴴽�����ģ�顢ǰ���̼�����̸�ͨ����ͬ��״̬�ļ���д����
    //������ָ�����m_SyncInfoFileList����
    m_SyncInfoFileList.push_back(new CNetInfoFile(szNetStatusPath,
                                                  "status.dat",
                                                  m_nAccessPoint,
                                                  MT_NETWORK,
                                                  NETWORK_CHANNEL_ID));

    //��������״̬�ļ�����ز���
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
    //��״̬ͷ�������������ȱʡֵ�����״̬�ļ������ڣ���ȱʡֵд��״̬�ļ�
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
        //��״̬ͷ�������������ȱʡֵ�����״̬�ļ������ڣ���ȱʡֵд��״̬�ļ�
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

    //����Open��������״̬�ļ�
    if(Open() != ERR_SUCCESS)
    {
        return ERR_FAIL;
    }

    //����һ��64KB��С����ʱ�ռ䣬��ָ�븳��pBuffer;
    char* pBuffer = new char[64 * 1024];

    //����״̬�ļ�����
    GetSyncData(pBuffer, 64 * 1024, FALSE);

    //����ģ���ΪMT_FRONTSAVE��ͬ�����ݣ�����m_uFrontSavePID
    SMscSyncData* pMscSyncData = NULL;
    pMscSyncData = FindSyncData((SMscAPSyncFrame*)pBuffer, 
                                MT_FRONTSAVE, 
                                FRONTSAVE_CHANNEL_ID);
    if(NULL != pMscSyncData)
    {
        m_uFrontSavePID = ((SSaveSyncData*)pMscSyncData->Data)->uPID;
        m_uFrontFileCsn = ((SSaveSyncData*)pMscSyncData->Data)->uFileCsn;
    }

    //����ģ���ΪMT_NETWORK��ͬ�����ݣ�����m_uNetworkPID
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
* ����: CMscSyncData::GetLastWriteTime
* ����: ��ȡ�ϴ�д״̬�ļ���ʱ��
* ����: 
* ���: 
* ����: time_t �����ϴμ����ʱ��
********************************************************/
time_t CMscSyncData::GetLastWriteTime()
{
    //����m_tLastActiveTime
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
* ����: CMscSyncData::GetSyncDataLen
* ����: �����ѷ�װ��SMscAPSyncFrame�ṹ��ͬ�����ݵĳ���
* ����:
* ���:
* ����: int ����ͬ�����ݵĳ���
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
* ����: CMscSyncData::GetSyncData
* ����: ��ȡͬ�����ݲ���
* ����: pData     : �����������ָ�룬��װ��SMscAPSyncFrame
*                   �ṹ��ͬ������
*       nLen      : ����������ĳ���
*       bReadFile : �Ƿ���ļ��ж����µ����ݣ�TRUE��ʾҪ��
*                   �ļ��ж�����FALSE��ʾ�ӻ������ж���
* ���:
* ����: int ����ʵ�ֵõ����ֽ���
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
* ����: CMscSyncData::PutSyncData
* ����: д��״̬���ݲ���
* ����: pData     : ���뻺������ָ�� ����װ��SMscAPSyncFrame
*                   �ṹ��ͬ������
*       nLen      : ���뻺�����ĳ���
* ���:
* ����: int ��õ�ʵ��д������ݳ���
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

        //��m_SyncInfoFileList�в���ģ���ΪpSyncData->uMID��CSyncInfoFile����;
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
* ����: CMscSyncData::DoSyncData
* ����: ʵʩ����ͬ��������ֻ��ͬ���������
*       ǰ�����ģ��״̬���ݵ�ͬ������: 
*       ͬ�����ݷ��ͷ���ǰ�����ļ����кŴ��ڵ��ڻ�ԶС�ڽ��շ�
*       ����ģ��״̬���ݵ�ͬ������:
*       ͬ�����ݷ��ͷ���ǰ�����ļ����кŴ��ڵ��ڻ�ԶС�ڽ��շ���
*       ��ͬ�����ݷ��ͷ����ϴμ���ʱ�����ڽ��շ�
* ����: pData     : ���뻺������ָ�� ����װ��SMscAPSyncFrame
*                   �ṹ��ͬ������
*       nLen      : ���뻺�����ĳ���
* ���:
* ����: void
********************************************************/
void CMscSyncData::DoSyncData(void* pData, int nLen)
{
    SMscAPSyncFrame* pFrame = (SMscAPSyncFrame*)pData;
    int nBufferWritten      = MSC_AP_SYNC_FRAME_HEAD_LENGTH;
    LIST<CSyncInfoFile*>::iterator i;
    CSyncInfoFile* pInfoFile;
    BOOL bDoSyncSaveData = FALSE;

    //��������������ǰ����ģ���ͬ��ָ֡��
    SMscSyncData* pSyncData = NULL;
    pSyncData = FindSyncData((SMscAPSyncFrame*)pData, 
                              MT_FRONTSAVE, 
                              FRONTSAVE_CHANNEL_ID);
    if(NULL != pSyncData)
    {
        //ȡ������������ǰ����ģ����ļ����к�
        UINT4 uFrontFileCsn = ((SSaveSyncData*)pSyncData->Data)->uFileCsn;

        //����������ݵ�ǰ�����ļ����кŴ��ڵ��ڱ��˵�ǰ�����ļ����кţ�
        //�����������ݵ�ǰ�����ļ����к�ԶԶС�ڱ��˵�ǰ�����ļ����к�
        //���ô���ģ���ͬ����־Ϊ��
        if((uFrontFileCsn >= m_uFrontFileCsn) ||
           (uFrontFileCsn + MID_OF_FILE_NUMBER < m_uFrontFileCsn))
        {
            bDoSyncSaveData = TRUE;
        }
    }

    //���ǰ�����ļ����кŲ�����ͬ��Ҫ�����˳�
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
                            ��bWaitForResponseΪFALSEʱ������ʱ����ģ����ǰ���̵İ���
                            Ӧ����ȣ������ʱ��bWaitForResponse��ΪTRUE������ģ���
                            ʼ��ʱֻ�Ჹ��Ӧ��֡����������̲���Ӱ�죬��˴˴���
                            bWaitForResponse����ΪTRUE
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

    //��������ͬ��״̬�ļ���д�����Flush������ˢ��״̬�ļ�
    for(i = m_SyncInfoFileList.begin(); i != m_SyncInfoFileList.end(); i++)
    {
        (*i)->Flush();
    }
}

/********************************************************
* ����: CMscSyncData::FindSyncData
* ����: ����ָ��ģ��ż�ͨ���ŵ�ͬ�����ݿ�ָ��
* ����: pAPFrame : Դ������ָ��
*       uMID     : ģ���
*       uChannel : ͨ����
* ���: 
* ����: SMscSyncData* nChl��Ӧͨ���ŵ�ͬ�����ݿ�ָ��
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
