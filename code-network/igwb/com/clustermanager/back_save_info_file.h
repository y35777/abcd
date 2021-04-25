#ifndef __BACK_SAVE_SYNC_INFO_FILE_H__
#define __BACK_SAVE_SYNC_INFO_FILE_H__

#include "save_info_file.h"

class CBackSaveInfoFile : public CSaveInfoFile
{
public:
    CBackSaveInfoFile(const char* const szFilePath,
                      const char* const szFileName,
                      const char* const szBakFilePath,
                      const char* const szBakFileName,
                      int nAPNum,
                      int nModule,
                      int nChl);
    virtual ~CBackSaveInfoFile();

    //��ȡͬ�����ݲ���
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE);
    //д��״̬���ݲ���
    virtual int PutSyncData(void* pData, int nLen);

};

#endif //__BACK_SAVE_SYNC_INFO_FILE_H__
