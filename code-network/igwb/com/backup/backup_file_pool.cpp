#include "backup_file_pool.h"

ACE_Mutex CNetBackupFilePool::g_Mutex;
CNetBackupFilePool* CNetBackupFilePool::g_pInstance = NULL;

//获得文件备份仲载器的全局单实例
CNetBackupFilePool* CNetBackupFilePool::GetInstance()
{
    g_Mutex.acquire();

    if(g_pInstance == NULL)
    {
        g_pInstance = new CNetBackupFilePool;
    }

    g_Mutex.release();

    return g_pInstance;
}

//更新指定备份任务的备份信息
void CNetBackupFilePool::UpdateFilePoolTime(int          nFilePoolID, 
                                            int          nBackupTaskID,
                                            time_t       tBackupTime,
                                            SET<STRING>& file_list)
{
    g_Mutex.acquire();

    //如果找不到对应的文件池，则创建一个
    if(m_FilePoolMap.find(nFilePoolID) == m_FilePoolMap.end())
    {
        MAP<int, SFilePoolBackupInfo> file_pool;
        m_FilePoolMap[nFilePoolID] = file_pool;
    }

    //更新对应备份任务的当前备份信息
    MAP<int, SFilePoolBackupInfo> &file_pool = m_FilePoolMap[nFilePoolID];
    SFilePoolBackupInfo pool_info;
    pool_info.tBackupTime = tBackupTime;
    pool_info.file_list = file_list;
    file_pool[nBackupTaskID] = pool_info;

    g_Mutex.release();
}

//判断指定文件是否已备份
//如果指定的文件池ID为0xFFFFFFFF，表示无效，其中的任何一个备份任务都是独立的
BOOL CNetBackupFilePool::IsFileBackupped(int    nFilePoolID,
                                         int    nBackupTaskID,
                                         time_t tFileTime,
                                         char*  szFileName)
{
    g_Mutex.acquire();

    //如果找到对应的文件池
    if(m_FilePoolMap.find(nFilePoolID) != m_FilePoolMap.end())
    {
        MAP<int, SFilePoolBackupInfo> &file_pool = m_FilePoolMap[nFilePoolID];
        if(nFilePoolID == INVALID_FILE_POOL_ID)
        {
            if(file_pool.find(nBackupTaskID) != file_pool.end())
            {
                SFilePoolBackupInfo& pool_info = file_pool[nBackupTaskID];
                //如果文件时间小于最后备份的文件时间，则说明文件已备份
                if(tFileTime < pool_info.tBackupTime)
                {
                    g_Mutex.release();
                    return TRUE;
                }
                //如果文件时间等于最后备份的文件时间，且在已备份记录中找是否存在对应项
                else if(tFileTime == pool_info.tBackupTime)
                {
                    //如果找到，说明已完全备份
                    if(pool_info.file_list.find(szFileName) != pool_info.file_list.end())
                    {
                        g_Mutex.release();
                        return TRUE;
                    }
                }

                //余下的情况说明话单文件未被备份任务完全备份
                g_Mutex.release();
                return FALSE;
            }
        }
        else if(file_pool.size() > 0)
        {
            MAP<int, SFilePoolBackupInfo>::iterator i = file_pool.begin();
            for(; i != file_pool.end(); i++)
            {
                //只要在一个备份任务中，发现文件时间大于该备份任务的最后备份的文件时间，
                //则说明文件未完全备份
                if(tFileTime > i->second.tBackupTime)
                {
                    g_Mutex.release();
                    return FALSE;
                }
                //如果文件时间等于最后备份的文件时间，且在已备份记录中找是否存在对应项
                else if(tFileTime == i->second.tBackupTime)
                {
                    //只要在一个备份任务中找不到，说明未完全备份
                    if(i->second.file_list.find(szFileName) == i->second.file_list.end())
                    {
                        g_Mutex.release();
                        return FALSE;
                    }
                }
            }

            //余下的情况说明话单文件已被所有同ID的备份任务完全备份
            g_Mutex.release();
            return TRUE;
        }
    }

    g_Mutex.release();

    return FALSE;
}
