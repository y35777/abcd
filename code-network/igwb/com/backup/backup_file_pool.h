#ifndef __NET_BACKUP_FILE_POOL_H__
#define __NET_BACKUP_FILE_POOL_H__

#include "../include/base_type.h"

#define INVALID_FILE_POOL_ID -1

struct SFilePoolBackupInfo
{
    time_t      tBackupTime;
    SET<STRING> file_list;
};

//�ļ�����������,���ڱ���Դ�Ŀ�ɾ���ٲ�
class CNetBackupFilePool
{
public:
   
    //����ļ�������������ȫ�ֵ�ʵ��
    static CNetBackupFilePool* GetInstance();

    //����ָ����������ı�����Ϣ
    void UpdateFilePoolTime(int          nFilePoolID, 
                            int          nBackupTaskID,
                            time_t       tBackupTime,
                            SET<STRING>& file_list);

    //�ж�ָ���ļ��Ƿ��ѱ���
    //���ָ�����ļ���IDΪ0xFFFFFFFF����ʾ��Ч�����е��κ�һ�����������Ƕ�����
    BOOL IsFileBackupped(int    nFilePoolID,
                         int    nBackupTaskID,
                         time_t tBackupTime,
                         char*  szFileName);

private:
    
    CNetBackupFilePool()
    {
        //
    }

    //�ļ���ID��������������ʱ����Ϣ��MAP
    MAP<int, MAP<int, SFilePoolBackupInfo> > m_FilePoolMap;  

    //���ڱ���g_FilePoolMap��g_pInstance
    static ACE_Mutex            g_Mutex;

    static CNetBackupFilePool*  g_pInstance;
};

#endif //__NET_BACKUP_FILE_POOL_H__

