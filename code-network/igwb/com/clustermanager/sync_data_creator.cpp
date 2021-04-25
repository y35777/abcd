#include "../include/base_type.h"
#include "../include/toolbox.h"
#include "../include/format_out.h"
#include "../frontsave/front_save.h"
#include "msc_sync_data.h"
#include "sync_data_creator.h"


/********************************************************
* 名称: CSyncDataCreator::CSyncDataCreator
* 功能: 构造函数
* 输入:
* 输出:
* 返回:
********************************************************/
CSyncDataCreator::CSyncDataCreator()
{
    //
}

/********************************************************
* 名称: CSyncDataCreator::~CSyncDataCreator
* 功能: 析构函数
* 输入:
* 输出:
* 返回:
********************************************************/
CSyncDataCreator::~CSyncDataCreator()
{
    //
}

/********************************************************
* 名称: CSyncData*>* CSyncDataCreator::CreateSyncDataList
* 功能: 创建接入点号到同步数据对象的MAP
* 输入:
* 输出:
* 返回: MAP <BYTE, CSyncData*>* 接入点号到同步数据对象的MAP指针
********************************************************/
MAP<BYTE, CSyncData*>* CSyncDataCreator::CreateSyncDataList()
{
    errno = 0;

    MAP<BYTE, CSyncData*>* pMap = new MAP<BYTE, CSyncData*>;
    MAP<BYTE, CSyncData*>::iterator iter;
    CSyncData* pSyncData = NULL;

    //从配置文件中读出各个接入点的配置信息
    CINIFile *pIniFile;
    pIniFile = new CINIFile(GetCfgFilePath());
    pIniFile->Open();

    //读接入点数
    int nAPCount = pIniFile->GetInt(CFG_SEC_COMMON,
                                    CFG_COMMON_KEY_APCOUNT,
                                    CFG_COMMON_KEY_APCOUNT_DEFAULT);
    char szAPSec[25];
    int nAPType;
    for(int i = 1; i <= nAPCount; i++)
    {
        //读接入点类型
        sprintf(szAPSec, "%s%d", CFG_SEC_AP, i);
        nAPType = pIniFile->GetInt(szAPSec,
                                   CFG_AP_KEY_APTYPE,
                                   CFG_AP_KEY_APTYPE_DEFAULT);

        switch(nAPType)
        {
        //当前接入点类型是MSC、CDMA或128)
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
