#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "../frontsave/front_save.h"
#include "sync_struct.h"
#include "sync_manager.h"
#include "sync_data_creator.h"


/********************************************************
* ����: CSyncManager::CSyncManager
* ����: ���캯��
* ����:   
* ���: 
* ����: 
********************************************************/
CSyncManager::CSyncManager()
{
    m_pSyncDataMap = NULL;
}

/********************************************************
* ����: CSyncManager::~CSyncManager
* ����: ��������
* ����: 
* ���: 
* ����: 
********************************************************/
CSyncManager::~CSyncManager()
{
    ExitInstance();
}

/********************************************************
* ����: CSyncManager::InitInstance
* ����: ��ʼ������
* ����: 
* ���: 
* ����: int ��ʼ�������Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CSyncManager::InitInstance()
{
    m_pSyncDataMap = CSyncDataCreator::CreateSyncDataList();
    
    if((m_pSyncDataMap == NULL) && (ERR_INIT_SYNC_MGR == errno))
    {
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CSyncManager::ExitInstance
* ����: �˳�ǰ���������
* ����: 
* ���: 
* ����: void 
********************************************************/
void CSyncManager::ExitInstance()
{
    Close();

    if(NULL != m_pSyncDataMap)
    {
        MAP<BYTE, CSyncData*>::iterator i;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            delete (*i).second;
        }
        m_pSyncDataMap->clear();
        delete m_pSyncDataMap;
        m_pSyncDataMap = NULL;
    }
}

/********************************************************
* ����: CSyncManager::Open
* ����: ״̬�ļ��򿪲�����ֻ���ڱ�ͬ����׼������ͬ������ʱ����
* ����: 
* ���: 
* ����: int �򿪲����Ƿ�ɹ���0��ʾ�ɹ�������ֵ��ʾʧ��
********************************************************/
int CSyncManager::Open()
{
    if(NULL != m_pSyncDataMap)
    {
        MAP<BYTE, CSyncData*>::iterator i;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            if((*i).second->Open() != ERR_SUCCESS)
            {
                Close();
                return ERR_FAIL;
            }
        }
    }

    return ERR_SUCCESS;
}

/********************************************************
* ����: CSyncManager::Close
* ����: ״̬�ļ��رղ�����ֻ���ڱ�ͬ������ɽ���ͬ�����ݺ����
* ����: 
* ���: 
* ����: void 
********************************************************/
void CSyncManager::Close()
{
    if(NULL != m_pSyncDataMap)
    {
        MAP<BYTE, CSyncData*>::iterator i;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            (*i).second->Close();
        }
    }
}

/********************************************************
* ����: CSyncManager::GetSyncDataLen
* ����: �����ѷ�װ��SSyncFrame�ṹ��ͬ�����ݵĳ���
* ����: 
* ���: 
* ����: int ����ͬ�����ݵĳ���
********************************************************/
int CSyncManager::GetSyncDataLen()
{
    int nLen = 0;
    if(NULL != m_pSyncDataMap)
    {
        MAP<BYTE, CSyncData*>::iterator i;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            if((*i).second->GetSyncDataLen() > 0)
            {
                nLen += (*i).second->GetSyncDataLen();
            }
        }
    }

    if(nLen > 0)
    {
        nLen += SYNC_FRAME_HEAD_LENGTH;
    }
    return nLen;
}

/********************************************************
* ����: CSyncManager::GetSyncData
* ����: ��ȡͬ�����ݲ���
* ����: 
* ���: pData     : �����������ָ��,��װ��SSyncFrame
*                   �ṹ��ͬ������
*       nLen      : ����������ĳ���
*       bReadFile : �Ƿ���ļ��ж���
* ����: int ��ȡͬ�����ݲ����Ƿ�ɹ�,0��ʾ�ɹ�,����ֵ��ʾʧ��
********************************************************/
int CSyncManager::GetSyncData(void*& pData, int& nLen, BOOL bReadFile)
{
    int nErrNo      = ERR_BUFFER_NOT_ENOUGH;
    char* pBuffer   = NULL;
    int nBufferLen  = 0;
    int nBufferRead = 0;
    MAP<BYTE, CSyncData*>::iterator i;
    int nTimes      = 0;

    pData = 0;
    nLen  = 0;
    if((NULL == m_pSyncDataMap) || (m_pSyncDataMap->size() == 0))
    {
        return ERR_SUCCESS;
    }

    //��������Ϊ�˱�������ģ���ڵ���ǰ���һ��ͬ�����ִ������µ�
    //MSC����Э��ջ��ʹͬ�����ݳ��ȸı䣬��ִ��Open���������ļ���
    //���¶���ʱ��GetSyncDataLen���ص����ݽ��õ�����
    while((ERR_BUFFER_NOT_ENOUGH == nErrNo) && (nTimes++ < 2))
    {
        nErrNo      = 0;
        nBufferRead = SYNC_FRAME_HEAD_LENGTH;
        nBufferLen  = GetSyncDataLen();
        if (pBuffer != NULL)
        {
            delete [] pBuffer;
        }
        if(0 >= nBufferLen)
        {
            return ERR_SUCCESS;
        }
        pBuffer = new char[nBufferLen];
        int nRetLen = 0;
        for(i = m_pSyncDataMap->begin(); i != m_pSyncDataMap->end(); i++)
        {
            nRetLen = (*i).second->GetSyncData(pBuffer + nBufferRead, 
                                               nBufferLen - nBufferRead, 
                                               bReadFile);
            if ((0 == nRetLen) && 
                (ERR_BUFFER_NOT_ENOUGH == errno))
            {
                nErrNo = ERR_BUFFER_NOT_ENOUGH;
                break;
            }
            nBufferRead += nRetLen;
        }
    }
    
    if(nTimes > 2)
    {
        if (pBuffer != NULL)
        {
            delete [] pBuffer;
        }
        return ERR_FAIL;
    }

    ((SSyncFrame*)pBuffer)->uLen = nBufferRead - SYNC_FRAME_HEAD_LENGTH;
    pData = pBuffer;
    nLen = nBufferRead;
    
    return ERR_SUCCESS;
}

/********************************************************
* ����: CSyncManager::PutSyncData
* ����: д��״̬���ݲ���
* ����: pData : ���뻺������ָ��,��װ��SSyncFrame
*               �ṹ��ͬ������
*       nLen  : ���뻺�����ĳ���
* ���: 
* ����: int ��õ�ʵ��д������ݳ���
********************************************************/
int CSyncManager::PutSyncData(void* pData, int nLen)
{
    SSyncFrame* pFrame          = (SSyncFrame*)pData;
    int nBufferWritten          = SYNC_FRAME_HEAD_LENGTH;
    SMscAPSyncFrame* pSyncFrame = NULL;

    if((NULL == m_pSyncDataMap) || (m_pSyncDataMap->size() == 0))
    {
        return 0;
    }

    while(nBufferWritten < int(nLen - MSC_AP_SYNC_FRAME_HEAD_LENGTH))
    {
        pSyncFrame = (SMscAPSyncFrame*) ((char*)pFrame + nBufferWritten);
        if((m_pSyncDataMap->find(pSyncFrame->uAPointID) 
            != m_pSyncDataMap->end()) && (pSyncFrame->uLen > 0))
        {
            (*m_pSyncDataMap)[pSyncFrame->uAPointID]->PutSyncData(
                                                      (void*)pSyncFrame, 
                                                      pSyncFrame->uLen +
                                                      MSC_AP_SYNC_FRAME_HEAD_LENGTH);
        }
        nBufferWritten += MSC_AP_SYNC_FRAME_HEAD_LENGTH + pSyncFrame->uLen;
    }

    if(SYNC_FRAME_HEAD_LENGTH == nBufferWritten)
    {
        nBufferWritten = 0;
    }
    return nBufferWritten;
}

/********************************************************
* ����: CSyncManager::DoSyncData
* ����: ʵʩ����ͬ��������ֻ��ͬ���������
* ����: pData     : ���뻺������ָ�� ����װ��SMscAPSyncFrame
*                   �ṹ��ͬ������
*       nLen      : ���뻺�����ĳ���
* ���: 
* ����: void 
********************************************************/
void CSyncManager::DoSyncData(void* pData, int nLen)
{
    if((NULL == m_pSyncDataMap) || (m_pSyncDataMap->size() == 0))
    {
        return;
    }
 
    SSyncFrame* pFrame          = (SSyncFrame*)pData;
    int nBufferWritten          = SYNC_FRAME_HEAD_LENGTH;
    SMscAPSyncFrame* pSyncFrame = NULL;
    while(nBufferWritten <= int(nLen - MSC_AP_SYNC_FRAME_HEAD_LENGTH))
    {
        pSyncFrame = (SMscAPSyncFrame*) ((char*)pFrame + nBufferWritten);
        if((m_pSyncDataMap->find(pSyncFrame->uAPointID) 
            != m_pSyncDataMap->end()) && (pSyncFrame->uLen > 0))
        {
            (*m_pSyncDataMap)[pSyncFrame->uAPointID]->DoSyncData(
                                                      (void*)pSyncFrame, 
                                                      pSyncFrame->uLen +
                                                      MSC_AP_SYNC_FRAME_HEAD_LENGTH);
        }
        nBufferWritten += MSC_AP_SYNC_FRAME_HEAD_LENGTH + pSyncFrame->uLen;
    }   
}

