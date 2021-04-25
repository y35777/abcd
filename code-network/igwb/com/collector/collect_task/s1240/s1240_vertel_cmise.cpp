#include "../../../include/toolbox.h"
#include "s1240_vertel_cmise.h"

#include "../../include/adapter_include.h"
#include "../../include/cmise_cmd_code.h"

#define VERTEL_CMISE_PROC       "./vcmise"

CS1240VertelCmise::CS1240VertelCmise()
{

}

CS1240VertelCmise::~CS1240VertelCmise()
{

}

//��ʼ�����ط���, ���麯��, �����������ʵ�ָ÷���
int CS1240VertelCmise::Init()
{
    assert(m_szAgent[0] != '\0');
    assert(m_szMgr[0] != '\0');

    if(ACE_OS::access(VERTEL_CMISE_PROC, F_OK) != 0)
    {
        COTRACE("CMISE���������,�����°�װ������");
        //aTRACE("Failed to get pMakeCmise");
        return ERR_FAIL;
    }

    char szCmdLink[1024];

    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "%s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s",
             VERTEL_CMISE_PROC,
             CMISE_TEST,
             m_szMgr,
             m_szAgent,
             m_szAssPwd,
             m_szActPwd,
             m_szNEName,
             m_szTPID);
    szCmdLink[1023] = '\0';

    int nRet = system(szCmdLink);
    if(ERR_SUCCESS != nRet)
    {
        COTRACE("CMISE��ʼ��ʧ��(������:%d)", nRet);
        //aTRACE("Failed to init VertelCmise(ErrCode:%d)", nRet);
        return ERR_FAIL;
    }

    return nRet;
}

int CS1240VertelCmise::GetFilePool(SS1240FilePoolMsg& pool_msg)
{
    assert(m_szAgent[0] != '\0');
    assert(m_szMgr[0] != '\0');

    CreateDeepDir(SZ_OUTPUT_PATH);

    char szOutputFile[MAX_PATH];
    SNPRINTF(szOutputFile, 
             sizeof(szOutputFile),
             "%s/%s_%s_GetFilePool.txt",
             SZ_OUTPUT_PATH,
             m_szNEName,
             m_szTPID);
    szOutputFile[MAX_PATH - 1] = '\0';

    char szCmdLink[1024];
    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "%s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%s",
             VERTEL_CMISE_PROC,
             CMISE_GET_FILE_POOL,
             m_szMgr,
             m_szAgent,
             m_szAssPwd,
             m_szActPwd,
             m_szNEName,
             m_szTPID,
             szOutputFile);
    szCmdLink[1023] = '\0';

    int nRet = system(szCmdLink);
    if(nRet != ERR_SUCCESS)
    {
        /*
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "GetFilePool failed(ErrCode:%d)", 
               nRet);
        COTRACE("GetFilePool failed(ErrCode:%d)", 
              nRet);
        */
        remove(szOutputFile);
        return nRet;
    }

    FILE* fp = fopen(szOutputFile, "r");
    if(fp == NULL)
    {
        /*
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����GetFilePool������ļ�(%s)ʧ��(������:%d)", 
               szOutputFile,
               errno);
        */
        COTRACE("����GetFilePool������ļ�(%s)ʧ��(������:%d)", 
              szOutputFile,
              errno);
        return -1;
    }
    char szFileInfo[1024] = "";  //����һ���ļ���Ϣ���ַ���
    if(NULL == fgets(szFileInfo, sizeof(szFileInfo), fp))
    {
        /*
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����GetFilePool��ȡ����ļ�(%s)ʧ��(������:%d)", 
               szOutputFile,
               errno);
        */
        COTRACE("����GetFilePool��ȡ����ļ�(%s)ʧ��(������:%d)", 
              szOutputFile,
              errno);
        fclose( fp );
        remove(szOutputFile);
        return -1;
    }

    //����Ϣ���õ��ļ���
    StringLRTrim(szFileInfo);
	int nRead = sscanf(szFileInfo, 
                       "%s%d%d%d",
                       pool_msg.szFilePoolId,
                       &pool_msg.nFileCount,
                       &pool_msg.nFileFullCount,
                       &pool_msg.nAlarmLimit);
    fclose(fp);
    remove(szOutputFile);
    if(nRead != 4)
    {
        nRet = ERR_FAIL;
    }

    return nRet;
}

int CS1240VertelCmise::SetFilePool(int nAlarmLimit)
{
    assert(m_szAgent[0] != '\0');
    assert(m_szMgr[0] != '\0');

    char szCmdLink[1024];

    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "%s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%d",
             VERTEL_CMISE_PROC,
             CMISE_SET_FILE_POOL,
             m_szMgr,
             m_szAgent,
             m_szAssPwd,
             m_szActPwd,
             m_szNEName,
             m_szTPID,
             nAlarmLimit);
    szCmdLink[1023] = '\0';

    int nRet = system(szCmdLink);
    if(nRet != ERR_SUCCESS)
    {
        /*
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "SetFilePool failed(ErrCode:%d, nAlarmLimit:%d)", 
               nRet, nAlarmLimit);
        */
        COTRACE("SetFilePool failed(ErrCode:%d, nAlarmLimit:%d)", 
              nRet, nAlarmLimit);
    }

    return nRet;
}

int CS1240VertelCmise::GetFile(const char* const szFileId,
                               SS1240FileMsg&    file_msg)
{
    assert(m_szAgent[0] != '\0');
    assert(m_szMgr[0] != '\0');
    if((szFileId == NULL) || (szFileId[0] == '\0'))
    {
        COTRACE("szFileId == 0 || szFileId[0] == '\0' in CS1240VertelCmise::GetFile");
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "szFileId == 0 || szFileId[0] == '\0' in CS1240VertelCmise::GetFile");
        return ERR_FAIL;
    }

    CreateDeepDir(SZ_OUTPUT_PATH);

    char szOutputFile[MAX_PATH];
    SNPRINTF(szOutputFile, 
             sizeof(szOutputFile),
             "%s/%s_%s_GetFile.txt",
             SZ_OUTPUT_PATH,
             m_szNEName,
             m_szTPID);

    char szCmdLink[1024];
    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "%s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%s 09=%s",
             VERTEL_CMISE_PROC,
             CMISE_GET_FILE,
             m_szMgr,
             m_szAgent,
             m_szAssPwd,
             m_szActPwd,
             m_szNEName,
             m_szTPID,
             szFileId,
             szOutputFile);
    szCmdLink[1023] = '\0';

    int nRet = system(szCmdLink);
    if(ERR_SUCCESS != nRet)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "GetFile failed(ErrCode:%d, szFileId:%s)", 
               nRet, szFileId);
        COTRACE("GetFile failed(ErrCode:%d, szFileId:%s)", 
              nRet, szFileId);
    }

    FILE* fp = fopen(szOutputFile, "r");
    if(fp == NULL)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����GetFile������ļ�(%s)ʧ��(������:%d)", 
               szOutputFile,
               errno);
        COTRACE("����GetFile������ļ�(%s)ʧ��(������:%d)", 
              szOutputFile,
              errno);
        return -1;
    }

    char szFileInfo[1024] = "";  //����һ���ļ���Ϣ���ַ���
    if(NULL == fgets(szFileInfo, sizeof(szFileInfo), fp))
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����GetFile��ȡ����ļ�(%s)ʧ��(������:%d)", 
               szOutputFile,
               errno);
        COTRACE("����GetFile��ȡ����ļ�(%s)ʧ��(������:%d)", 
              szOutputFile,
              errno);
        fclose( fp );
        remove(szOutputFile);
        return -1;
    }

    //����Ϣ���õ��ļ���
    char* szFileIdDup = NULL;
    strncpy(file_msg.szFileId, szFileId, sizeof(file_msg.szFileId));
    file_msg.szFileId[sizeof(file_msg.szFileId) - 1] = '\0';

    StringLRTrim(szFileInfo);
	int nRead = sscanf(szFileInfo, 
                       "%s%d%d%s%d%d%u",
                       file_msg.szFileId,
                       &file_msg.nFileStatus,
                       &file_msg.nFileMode,
                       file_msg.szFileIdDup,
                       &file_msg.nFileCounter,
                       &file_msg.nFileLocalDumped,
                       &file_msg.uFileSize);
    fclose(fp);
    remove(szOutputFile);
    if(nRead != 1)
    {
        nRet = ERR_FAIL;
    }

    return nRet;
}
                            
int CS1240VertelCmise::SetFile(const char* const szFileId,
                               int               nFilestatus)
{
    assert(m_szAgent[0] != '\0');
    assert(m_szMgr[0] != '\0');
    if((szFileId == NULL) || (szFileId[0] == '\0'))
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "szFileId == 0 || szFileId[0] == '\0' in CS1240VertelCmise::SetFile");
        COTRACE("szFileId == 0 || szFileId[0] == '\0' in CS1240VertelCmise::SetFile");
        return ERR_FAIL;
    }

    char szCmdLink[1024];

    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "%s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%s 09=%d",
             VERTEL_CMISE_PROC,
             CMISE_SET_FILE,
             m_szMgr,
             m_szAgent,
             m_szAssPwd,
             m_szActPwd,
             m_szNEName,
             m_szTPID,
             szFileId,
             nFilestatus);
    szCmdLink[1023] = '\0';

    int nRet = system(szCmdLink);
    if(nRet != ERR_SUCCESS)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "SetFile failed(ErrCode:%d, szFileId:%s, nFileStatus:%d)", 
               nRet, szFileId, nFilestatus);
        COTRACE("SetFile failed(ErrCode:%d, szFileId:%s, nFileStatus:%d)", 
              nRet, szFileId, nFilestatus);
    }

    return nRet;
}

//����ļ���������ǰĿ¼�µ��ļ��б�, �麯��, �����������ʵ�ָ���
int CS1240VertelCmise::GetFileList(int nFileStatus,
                                   VECTOR<SS1240FileMsg> &Out)
{
    assert(m_szAgent[0] != '\0');
    assert(m_szMgr[0] != '\0');

    int nRet;
    char szOutputFile[MAX_PATH];

    CreateDeepDir(SZ_OUTPUT_PATH);

#ifdef _SIMU_DEBUG
    SNPRINTF(szOutputFile, 
             sizeof(szOutputFile),
             "%s/%s_%s_GetFileList_%d.txt",
             SZ_OUTPUT_PATH,
             m_szNEName,
             m_szTPID,
             nFileStatus);
    szOutputFile[MAX_PATH - 1] = '\0';

#else
    SNPRINTF(szOutputFile, 
             sizeof(szOutputFile),
             "%s/%s_%s_GetFileList.txt",
             SZ_OUTPUT_PATH,
             m_szNEName,
             m_szTPID);
    szOutputFile[MAX_PATH - 1] = '\0';

    char szCmdLink[1024];
    SNPRINTF(szCmdLink, 
             sizeof(szCmdLink),
             "%s 01=%s 02=%s 03=%s 04=%s 05=%s 06=%s 07=%s 08=%d 09=%s",
             VERTEL_CMISE_PROC,
             CMISE_GET_FILE_LIST,
             m_szMgr,
             m_szAgent,
             m_szAssPwd,
             m_szActPwd,
             m_szNEName,
             m_szTPID,
             nFileStatus,
             szOutputFile);
    szCmdLink[1023] = '\0';

    nRet = system(szCmdLink);
    if(nRet != ERR_SUCCESS)
    {
        /*
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "GetFileList failed(ErrCode:%d, nFileStatus:%d)", 
               nRet, nFileStatus);
        COTRACE("GetFileList failed(ErrCode:%d, nFileStatus:%d)", 
              nRet, nFileStatus);
        */
        return nRet;
    }
#endif

    FILE* fp = fopen(szOutputFile, "r");
    if(fp == NULL)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "����GetFileList������ļ�(%s)ʧ��(������:%d)", 
               szOutputFile,
               errno);
        COTRACE("����GetFileList������ļ�(%s)ʧ��(������:%d)", 
              szOutputFile,
              errno);
        return -1;
    }

    SS1240FileMsg file_msg;
    char szFileInfo[1024];  //����һ���ļ���Ϣ���ַ���
    int  nCount = 0;
    while( !feof( fp ) )
    {
        szFileInfo[0] = '\0';
        if(NULL == fgets(szFileInfo, sizeof(szFileInfo), fp))
        {
            continue;
        }

        //����Ϣ���õ��ļ���
        StringLRTrim(szFileInfo);
        nRet = sscanf(szFileInfo, "%s  %d  %d  %s  %d  %d  %u\n",
                      file_msg.szFileId,
                      &file_msg.nFileStatus,
                      &file_msg.nFileMode,
                      file_msg.szFileIdDup,
                      &file_msg.nFileCounter,
                      &file_msg.nFileLocalDumped,
                      &file_msg.uFileSize);
        if(nRet == 7)
        {
            Out.push_back(file_msg);
        }
    }
    fclose( fp );
#ifndef _SIMU_DEBUG
    remove(szOutputFile);
#endif
    return ERR_SUCCESS;
}
