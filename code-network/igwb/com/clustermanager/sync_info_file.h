#ifndef __SYNC_INFO_FILE_H__
#define __SYNC_INFO_FILE_H__

class CSyncInfoFile
{
public:
    CSyncInfoFile(const char* const szFilePath,
                  const char* const szFileName,
                  int nAPNum,
                  int nModule,
                  int nChl);
    virtual ~CSyncInfoFile();

    int GetAccessPoint();           //��ý�����
    int GetModule();                //���ģ���
    int GetChannel();               //���ͨ����
    int GetSyncDataLen();           //���ͬ�����ݳ���
    time_t GetLastWriteTime();      //ȡ״̬�ļ��������޸�ʱ��

    virtual int Open() = 0;         //״̬�ļ��򿪲�����ֻ�б�ͬ���������
    virtual void Close() = 0;       //״̬�ļ��رղ�����ֻ�б�ͬ���������
    //��ȡͬ�����ݲ���
    virtual int GetSyncData(void* pData, int nLen, BOOL bReadFile = TRUE) = 0;
    //д��״̬���ݲ���
    virtual int PutSyncData(void* pData, int nLen) = 0;
    //���������е�����д��״̬�ļ���ֻ�б�ͬ���������
    virtual int Flush() = 0;

protected:

    //����������
    int      m_nBufferLen;

    //������ָ��
    char*    m_pBuffer;

    //״̬�ļ����ڵ�Ŀ¼·����
    char     m_szFilePath[MAX_PATH];

    //״̬�ļ���
    char     m_szFileName[MAX_PATH];

    //״̬�ļ�ȫ·����
    char     m_szFullFileName[MAX_PATH];

    //ͬ�����ݳ���
    int      m_nSyncDataLen;

    //������
    int      m_nAccessPoint;

    //ģ���
    int      m_nModule;

    //ͨ����
    int      m_nChannel;

};

#endif //__SYNC_INFO_FILE_H__
