#include "../include/toolbox.h"
#include "alarm_bin_util.h"
#include "mml_alarm_handler.h"

CAlarmBinUtil::CAlarmBinUtil()
{
    m_pAlarmFile  = NULL;
    m_strFileName = "";
}

CAlarmBinUtil::~CAlarmBinUtil()
{
    CloseFile();
}

//����ʷ�澯�ĵ����ļ�������������ָ���ļ�
int CAlarmBinUtil::OpenFile(const char * const szFilePath)
{
    CloseFile();
    
    if(szFilePath != NULL)
    {
        m_strFileName = szFilePath;

        //�Զ�д��ʽ�򿪸澯�ļ�
        m_pAlarmFile = fopen(szFilePath,"rb+");
    
        if(m_pAlarmFile != NULL)    //�ļ�����
        {
            return ERR_SUCCESS;
        }
        else//�ļ�������ʱ�����澯�ļ�
        {
            m_pAlarmFile = fopen(szFilePath,"wb+");          
            if(NULL != m_pAlarmFile)
            {
                fwrite("VER:", sizeof(char), 4, m_pAlarmFile);//��д"VER:"
                UINT4 uTmp = LATEST_ALARM_HIS_VERSION;
                fwrite(&uTmp, sizeof(UINT4), 1, m_pAlarmFile);//��д�汾�� 
                return ERR_SUCCESS;
            }
             
        }
    }

    return ERR_OPEN_HISTORY_ALARM;
}

//�ر��ļ�
int CAlarmBinUtil::CloseFile(void)
{
    if(NULL != m_pAlarmFile)
    {
        fclose(m_pAlarmFile);
        m_pAlarmFile = NULL;
    }
    
    return ERR_SUCCESS;
}

//���ʣ��ĸ澯��
int CAlarmBinUtil::GetCount(void)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }
    
    if(NULL == m_pAlarmFile)
    {
        return 0;
    }
    
    //���ļ��򿪳ɹ�ʱ,��õ�ǰ�澯���ݿ��ļ��и澯��¼��
    fseek(m_pAlarmFile, 0L, SEEK_END);

/*#ifdef _OM_MODIFY
    int nCount = (ftell(m_pAlarmFile) - SIZEOF_ALARMHEAD*3) / SIZEOF_HISTORYALARM;
#else
    int nCount = (ftell(m_pAlarmFile) - SIZEOF_ALARMHEAD) / SIZEOF_HISTORYALARM;
#endif
*/
    int nCount = (ftell(m_pAlarmFile) - SIZEOF_ALARMHEAD*5) / SIZEOF_HISTORYALARM;    

    return nCount;
}

//�����ļ�
int CAlarmBinUtil::Flush(void)
{
    //д�ļ�
    if(NULL == m_pAlarmFile)
    {
        return -1;
    }
    
    if (fflush(m_pAlarmFile) != 0)
    {
        return -1;
    }

    return 0;
}

//ɾ��ĳ���澯��¼
int CAlarmBinUtil::Delete(int nIndex)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }

    int nPos      = ALARM_OFFSET(nIndex);
    int nFileSize = FileSize(m_pAlarmFile);
    int nCount    = (nFileSize - nPos) / (SIZEOF_HISTORYALARM) - 1;
    char szBuf[SIZEOF_HISTORYALARM];
    int nRet = 0;

    for(int i = 0; i < nCount; i++)
    {
        nRet = fseek(m_pAlarmFile, nPos + SIZEOF_HISTORYALARM, SEEK_SET);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
        nRet = fread(szBuf, SIZEOF_HISTORYALARM, 1, m_pAlarmFile);
        if(nRet != 1)
        {
            CloseFile();
            return ERR_FAIL;
        }

        nRet = fseek(m_pAlarmFile, nPos, SEEK_SET);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
        nRet = fwrite(szBuf, SIZEOF_HISTORYALARM, 1, m_pAlarmFile);
        if(nRet != 1)
        {
            CloseFile();
            return ERR_FAIL;
        }
        
#ifdef _OM_MODIFY
        //by ldf 2004-01-31 �����Ƿ�Ӧ������һ��
		nPos +=SIZEOF_HISTORYALARM;
#endif

    }

    return ERR_SUCCESS;
}

//����ļ�ͷ��¼
int CAlarmBinUtil::ReadHeadElement(UINT4& uCSN)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*2, SEEK_SET);    
    //��ȡ�澯��ˮ����Ϣ,����ȡ�����򷵻�ʧ��
    int nRes = fread(&uCSN,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if( nRes != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

int CAlarmBinUtil::ReadHeadElementSyn(UINT4& uSYN)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*3, SEEK_SET);    
    //��ȡ�澯ͬ������Ϣ,����ȡ�����򷵻�ʧ��
    int nRes = fread(&uSYN,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if( nRes != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
int CAlarmBinUtil::ReadHeadElementMMLBaseID(UINT4& uMMLBaseID)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*4, SEEK_SET);    
    //��ȡ�澯��ˮ����Ϣ,����ȡ�����򷵻�ʧ��
    int nRes = fread(&uMMLBaseID,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if( nRes != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

//���/д�ļ�ͷ��¼
int CAlarmBinUtil::WriteHeadElement(UINT4 uCSN)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*2, SEEK_SET);    
    //��ȡ�澯��ˮ����Ϣ,����ȡ�����򷵻�ʧ��
    int nRes = fwrite(&uCSN,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if(nRes != 1 || Flush() != 0)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

int CAlarmBinUtil::WriteHeadElementSyn(UINT4 uSYN)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*3, SEEK_SET);    
    //��ȡ�澯��ˮ����Ϣ,����ȡ�����򷵻�ʧ��
    int nRes = fwrite(&uSYN,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if(nRes != 1 || Flush() != 0)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
int CAlarmBinUtil::WriteHeadElementMMLBaseID(UINT4 uBaseID)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }
    
    fseek(m_pAlarmFile, SIZEOF_ALARMHEAD*4, SEEK_SET);    
    //��ȡ�澯��ˮ����Ϣ,����ȡ�����򷵻�ʧ��
    int nRes = fwrite(&uBaseID,SIZEOF_ALARMHEAD,1,m_pAlarmFile);
    if(nRes != 1 || Flush() != 0)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}
//дһ����־��Ϣ
int CAlarmBinUtil::SaveHisAlarm(int            nIndex,
                                SHistoryAlarm& hisAlarm)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }

    int nPos      = ALARM_OFFSET(nIndex);
    int nFileSize = FileSize(m_pAlarmFile);
    int nRet      = 0;
    
#ifdef _OM_MODIFY
    if(nPos  > nFileSize)
    {
            CloseFile();
            return ERR_FAIL;
    }
#else
    if(nPos >= nFileSize)
    {
        nRet = fseek(m_pAlarmFile, 0, SEEK_END);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
    }
#endif
    else
    {
        nRet = fseek(m_pAlarmFile, nPos, SEEK_SET);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
    }

    nRet = fwrite(&hisAlarm, SIZEOF_HISTORYALARM, 1, m_pAlarmFile);
    if(nRet != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

//��һ���澯��Ϣ
int CAlarmBinUtil::ReadOneAlarm(int            nIndex,
                                SHistoryAlarm& hisAlarm)
{
    if(NULL == m_pAlarmFile && m_strFileName != "")
    {
        OpenFile(m_strFileName.c_str());
    }

    if(NULL == m_pAlarmFile)
    {
        return ERR_FAIL;
    }

    int nPos      = ALARM_OFFSET(nIndex);
    int nFileSize = FileSize(m_pAlarmFile);
    int nRet      = 0;
    if(nPos >= nFileSize)
    {
        return ERR_FAIL;
    }
    else
    {
        nRet = fseek(m_pAlarmFile, nPos, SEEK_SET);
        if(nRet != 0)
        {
            CloseFile();
            return ERR_FAIL;
        }
    }

    nRet = fread(&hisAlarm, SIZEOF_HISTORYALARM, 1, m_pAlarmFile);
    if(nRet != 1)
    {
        CloseFile();
        return ERR_FAIL;
    }

    return ERR_SUCCESS;
}

