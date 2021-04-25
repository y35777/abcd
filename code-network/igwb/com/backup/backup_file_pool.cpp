#include "backup_file_pool.h"

ACE_Mutex CNetBackupFilePool::g_Mutex;
CNetBackupFilePool* CNetBackupFilePool::g_pInstance = NULL;

//����ļ�������������ȫ�ֵ�ʵ��
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

//����ָ����������ı�����Ϣ
void CNetBackupFilePool::UpdateFilePoolTime(int          nFilePoolID, 
                                            int          nBackupTaskID,
                                            time_t       tBackupTime,
                                            SET<STRING>& file_list)
{
    g_Mutex.acquire();

    //����Ҳ�����Ӧ���ļ��أ��򴴽�һ��
    if(m_FilePoolMap.find(nFilePoolID) == m_FilePoolMap.end())
    {
        MAP<int, SFilePoolBackupInfo> file_pool;
        m_FilePoolMap[nFilePoolID] = file_pool;
    }

    //���¶�Ӧ��������ĵ�ǰ������Ϣ
    MAP<int, SFilePoolBackupInfo> &file_pool = m_FilePoolMap[nFilePoolID];
    SFilePoolBackupInfo pool_info;
    pool_info.tBackupTime = tBackupTime;
    pool_info.file_list = file_list;
    file_pool[nBackupTaskID] = pool_info;

    g_Mutex.release();
}

//�ж�ָ���ļ��Ƿ��ѱ���
//���ָ�����ļ���IDΪ0xFFFFFFFF����ʾ��Ч�����е��κ�һ�����������Ƕ�����
BOOL CNetBackupFilePool::IsFileBackupped(int    nFilePoolID,
                                         int    nBackupTaskID,
                                         time_t tFileTime,
                                         char*  szFileName)
{
    g_Mutex.acquire();

    //����ҵ���Ӧ���ļ���
    if(m_FilePoolMap.find(nFilePoolID) != m_FilePoolMap.end())
    {
        MAP<int, SFilePoolBackupInfo> &file_pool = m_FilePoolMap[nFilePoolID];
        if(nFilePoolID == INVALID_FILE_POOL_ID)
        {
            if(file_pool.find(nBackupTaskID) != file_pool.end())
            {
                SFilePoolBackupInfo& pool_info = file_pool[nBackupTaskID];
                //����ļ�ʱ��С����󱸷ݵ��ļ�ʱ�䣬��˵���ļ��ѱ���
                if(tFileTime < pool_info.tBackupTime)
                {
                    g_Mutex.release();
                    return TRUE;
                }
                //����ļ�ʱ�������󱸷ݵ��ļ�ʱ�䣬�����ѱ��ݼ�¼�����Ƿ���ڶ�Ӧ��
                else if(tFileTime == pool_info.tBackupTime)
                {
                    //����ҵ���˵������ȫ����
                    if(pool_info.file_list.find(szFileName) != pool_info.file_list.end())
                    {
                        g_Mutex.release();
                        return TRUE;
                    }
                }

                //���µ����˵�������ļ�δ������������ȫ����
                g_Mutex.release();
                return FALSE;
            }
        }
        else if(file_pool.size() > 0)
        {
            MAP<int, SFilePoolBackupInfo>::iterator i = file_pool.begin();
            for(; i != file_pool.end(); i++)
            {
                //ֻҪ��һ�����������У������ļ�ʱ����ڸñ����������󱸷ݵ��ļ�ʱ�䣬
                //��˵���ļ�δ��ȫ����
                if(tFileTime > i->second.tBackupTime)
                {
                    g_Mutex.release();
                    return FALSE;
                }
                //����ļ�ʱ�������󱸷ݵ��ļ�ʱ�䣬�����ѱ��ݼ�¼�����Ƿ���ڶ�Ӧ��
                else if(tFileTime == i->second.tBackupTime)
                {
                    //ֻҪ��һ�������������Ҳ�����˵��δ��ȫ����
                    if(i->second.file_list.find(szFileName) == i->second.file_list.end())
                    {
                        g_Mutex.release();
                        return FALSE;
                    }
                }
            }

            //���µ����˵�������ļ��ѱ�����ͬID�ı���������ȫ����
            g_Mutex.release();
            return TRUE;
        }
    }

    g_Mutex.release();

    return FALSE;
}
