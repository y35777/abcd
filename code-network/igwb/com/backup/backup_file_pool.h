#ifndef __NET_BACKUP_FILE_POOL_H__
#define __NET_BACKUP_FILE_POOL_H__

#include "../include/base_type.h"

#define INVALID_FILE_POOL_ID -1

struct SFilePoolBackupInfo
{
    time_t      tBackupTime;
    SET<STRING> file_list;
};

//文件备份仲载器,用于备份源的可删除仲裁
class CNetBackupFilePool
{
public:
   
    //获得文件备份仲载器的全局单实例
    static CNetBackupFilePool* GetInstance();

    //更新指定备份任务的备份信息
    void UpdateFilePoolTime(int          nFilePoolID, 
                            int          nBackupTaskID,
                            time_t       tBackupTime,
                            SET<STRING>& file_list);

    //判断指定文件是否已备份
    //如果指定的文件池ID为0xFFFFFFFF，表示无效，其中的任何一个备份任务都是独立的
    BOOL IsFileBackupped(int    nFilePoolID,
                         int    nBackupTaskID,
                         time_t tBackupTime,
                         char*  szFileName);

private:
    
    CNetBackupFilePool()
    {
        //
    }

    //文件池ID到各个备份任务时间信息的MAP
    MAP<int, MAP<int, SFilePoolBackupInfo> > m_FilePoolMap;  

    //用于保护g_FilePoolMap及g_pInstance
    static ACE_Mutex            g_Mutex;

    static CNetBackupFilePool*  g_pInstance;
};

#endif //__NET_BACKUP_FILE_POOL_H__

