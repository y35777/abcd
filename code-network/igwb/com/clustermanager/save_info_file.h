#ifndef __SAVE_SYNC_INFO_FILE_H__
#define __SAVE_SYNC_INFO_FILE_H__

#include "sync_info_file.h"
#include "double_info_file.h"

#define FRONTSAVE_CHANNEL_ID  255

class CSaveInfoFile : public CSyncInfoFile
{
public:
    CSaveInfoFile(const char* const szFilePath,
                  const char* const szFileName,
                  const char* const szBakFilePath,
                  const char* const szBakFileName,
                  int nAPNum,
                  int nModule,
                  int nChl);
    virtual ~CSaveInfoFile();

    //�����ļ�ͷ���ȣ���¼��Ԫ���Ⱥͼ�¼��Ԫ������������
    void SetFileStruct(void* pData,
                       const UINT4 uHeadLen, 
                       const UINT4 uUnitLen, 
                       const UINT4 uUnitNum);
    //״̬�ļ��򿪲�����ֻ�б�ͬ���������
    virtual int Open();
    //״̬�ļ��رղ�����ֻ�б�ͬ���������
    virtual void Close();
    //��ȡͬ�����ݲ���
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE);
    //д��״̬���ݲ���
    virtual int PutSyncData(void* pData, int nLen);
    //���������е�����д��״̬�ļ���ֻ�б�ͬ���������
    virtual int Flush();

protected:

    //��״̬�ļ����ڵ�Ŀ¼·����
    char                m_szBakFilePath[MAX_PATH];

    //��״̬�ļ���
    char                m_szBakFileName[MAX_PATH];

    //�ļ�ͷ����
    UINT4               m_uHeadLen;

    //��¼��Ԫ����
    UINT4               m_uUnitLen;

    //��¼��Ԫ����
    UINT4               m_uUnitNum;

    //ǰ�����״̬�ļ���д����
    CDoubleInfoFile     m_InfoFile;
};

#endif //__SAVE_SYNC_INFO_FILE_H__
