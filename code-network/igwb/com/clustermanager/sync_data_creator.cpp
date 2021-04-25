#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "../frontsave/front_save.h"
#include "msc_sync_data.h"
#include "sync_data_creator.h"


/********************************************************
* ����: CSyncDataCreator::CSyncDataCreator
* ����: ���캯��
* ����:
* ���:
* ����:
********************************************************/
CSyncDataCreator::CSyncDataCreator()
{
    //
}

/********************************************************
* ����: CSyncDataCreator::~CSyncDataCreator
* ����: ��������
* ����:
* ���:
* ����:
********************************************************/
CSyncDataCreator::~CSyncDataCreator()
{
    //
}

/********************************************************
* ����: CSyncData*>* CSyncDataCreator::CreateSyncDataList
* ����: ���������ŵ�ͬ�����ݶ����MAP
* ����:
* ���:
* ����: MAP <BYTE, CSyncData*>* �����ŵ�ͬ�����ݶ����MAPָ��
********************************************************/
MAP<BYTE, CSyncData*>* CSyncDataCreator::CreateSyncDataList()
{
    errno = 0;

    MAP<BYTE, CSyncData*>* pMap = new MAP<BYTE, CSyncData*>;
    MAP<BYTE, CSyncData*>::iterator iter;
    CSyncData* pSyncData = NULL;

    //�������ļ��ж�������������������Ϣ
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    //���������
    int nAPCount = pIniFile->GetInt(CFG_SEC_COMMON,
                                    CFG_COMMON_KEY_APCOUNT,
                                    CFG_COMMON_KEY_APCOUNT_DEFAULT);
    char szAPSec[25];
    int nAPType;
    for(int i = 1; i <= nAPCount; i++)
    {
        //�����������
        sprintf(szAPSec, "%s%d", CFG_SEC_AP, i);
        nAPType = pIniFile->GetInt(szAPSec,
                                   CFG_AP_KEY_APTYPE,
                                   CFG_AP_KEY_APTYPE_DEFAULT);

        switch(nAPType)
        {
        //��ǰ�����������MSC��CDMA��128)
        case AP_GSM:
        case AP_128:
        case AP_SOFTX:
            pSyncData = new CMscSyncData(i);
            if((NULL == pSyncData) ||
               (pSyncData->InitInstance() != ERR_SUCCESS))
            {
                delete pSyncData;
                for(iter = pMap->begin(); iter != pMap->end(); iter++)
                {
                    delete (*iter).second;
                }
                pMap->clear();
                delete pMap;
                pIniFile->Close();
                delete pIniFile;
                errno = ERR_INIT_SYNC_MGR;
                return NULL;
            }
            (*pMap)[i] = pSyncData;
            break;

        default:
            break;
        }
    }

    pIniFile->Close();
    delete pIniFile;

    if(0 == pMap->size())
    {
        delete pMap;
        return NULL;
    }

    return pMap;
}
