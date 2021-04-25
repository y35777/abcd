// 5ess_collect.cpp: implementation of the C5ESSCollect class.
//
//////////////////////////////////////////////////////////////////////
#include "../../../include/base_type.h"
#include "../../include/i_ftam.h"
#include "../../include/i_cmise_fun.h"
#include "../vertel_ftam.h"
#include "5ess_collect.h"
#include "../bk_link_mgr.h"
#include "../global_fun.h"

#include <algorithm>
#include <math.h>
#include "../file_sn_manager.h"

#include "../../../include/toolbox.h"
#include "../../include/adapter_include.h"
#include "../../include/res_config.h"
#include "../../collector_file_pool.h"
#include "../../resource.h"
#include <assert.h>

extern CCollectorFilePool* GetFilePool();
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
C5ESSCollect::C5ESSCollect(int nSwitchIndex, CLinkGroup* pLinkGroup)
                          :CCollectBase(nSwitchIndex,pLinkGroup)
{
    m_pFileSNMgr = NULL;
}

C5ESSCollect::~C5ESSCollect()
{
    //���ֻ�����ŵ���ϸĿ¼��
    m_vBillSavePath.clear();
    //�ļ�ǰ׺������
    m_vFilePrefix.clear();

    if (m_pFileSNMgr != NULL)
    {
        delete m_pFileSNMgr;
        m_pFileSNMgr = NULL;
    }

}
//��ʼ���ɼ�����, �����������ʵ��
int  C5ESSCollect::Col_Init()
{
    int  nRet = 0;
    char szBuf[BUF_SIZE] = "";
    char szPathName[MAX_PATH];

    CINIFile ini(m_szCfgPath);
    ini.Open();
    SNPRINTF(szBuf,BUF_SIZE,"%s%d", CFG_CO_SEC_SWITCH, m_nSwitchIndex);

    //��ȡ�ɼ�������
    ini.GetString(szBuf,
                  CFG_CO_NAME,
                  CFG_CO_NAME_DEFAULT,
                  m_szSwitchName,
                  sizeof(m_szSwitchName));

    // ��ȡ�����ļ�:m_strLocalRootPath ���ػ��������Ŀ¼
    ini.GetString(szBuf,
                  CFG_CO_LOCAL_DIR,
                  CCollectorAdapter::GetFrontSavePath(),
                  szPathName,
                  MAX_PATH);
    int nLen = strlen(szPathName) - 1;
    if(nLen > 0)
    {
        if((szPathName[nLen] == '\\')
           || (szPathName[nLen] == '/'))
        {
            szPathName[nLen] = '\0';
        }
    }
    m_strLocalRootPath = szPathName;

    //�������ػ��������Ŀ¼
    if (!CreateDeepDir(m_strLocalRootPath.c_str()))
    {
        COTRACE("�ɼ�����(%s)ԭʼ����Ŀ¼(%s)�޷������ɹ�",
            m_szSwitchName,
            m_strLocalRootPath.c_str());

        return ERR_FAIL;
    }

    //�������������ʱĿ¼
    m_strTmpPath = szPathName;
    m_strTmpPath += "/temp";
    CreateDeepDir(m_strTmpPath.c_str());

    //�õ�Զ��5ESS�������ϵĲɼ�ԴĿ¼m_strRemotePath
    ini.GetString(szBuf,
                  CFG_CO_REMOTE_DIR,
                  CFG_CO_REMOTE_DIR_DEFAULT,
                  szPathName,
                  MAX_PATH);
    nLen = strlen(szPathName) - 1;
    if(nLen > 0)
    {
        if((szPathName[nLen] == '\\')
           || (szPathName[nLen] == '/'))
        {
            szPathName[nLen] = '\0';
        }
    }
    m_strRemotePath  = szPathName;

    char  szAreaCode[20];  //����
    char  szBillType[20];  //��������
    //�������ļ�, �õ�����m_szAreaCode
    ini.GetString(szBuf,
                  CFG_5ESS_AREA_CODE,
                  CFG_5ESS_AREA_CODE_DEFAULT,
                  szAreaCode,
                  sizeof(szAreaCode));

    //�������ļ�,��������m_szBillType
    ini.GetString(szBuf,
                  CFG_5ESS_BILL_TYPE,
                  CFG_5ESS_BILL_TYPE_DEFAULT,
                  szBillType,
                  sizeof(szBillType));

    //�������ļ�,���ػ����ļ���׺m_szBillType
    //�ļ���׺���ݲ�ʵ��
    //added by chenliangwei
    //�������ļ��ı�������
    m_nFileSaveDays = ini.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT);
    m_nFileSaveDays = ini.GetInt(szBuf,
                                 CFG_CO_ORIG_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);
    if(m_nFileSaveDays < 0)
    {
        m_nFileSaveDays = CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT;
    }

    m_bCheckSN = ini.GetInt(szBuf,
                            CFG_CO_CHECK_SN,
                            CFG_CO_CHECK_SN_DEFAULT);

    //�Ƿ�����ļ�����У��
    m_bFileLenVerify = ini.GetInt(CFG_CO_SEC_SYSTEM,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  CFG_CO_FILE_LEN_VERIFY_DEFAULT);
    m_bFileLenVerify = ini.GetInt(szBuf,
                                  CFG_CO_FILE_LEN_VERIFY,
                                  m_bFileLenVerify);

    int nCheckSNDepth = ini.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_CHECK_SN_DEPTH,
                                 CFG_CO_CHECK_SN_DEPTH_DEFAULT);
    nCheckSNDepth = ini.GetInt(szBuf,
                                 CFG_CO_CHECK_SN_DEPTH,
                                 nCheckSNDepth);
    if(nCheckSNDepth < 0)
    {
        nCheckSNDepth = CFG_CO_CHECK_SN_DEPTH_DEFAULT;
    }

    //���״̬�ļ���·��
#ifdef _PLATFORM_WIN32
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "d:/other/collect/%s/%s",
             CCollectorAdapter::GetAPName(),
             m_szSwitchName);
#else
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "/var/other/collect/%s/%s",
             CCollectorAdapter::GetAPName(),
             m_szSwitchName);
#endif
    CreateDeepDir(szPathName);

/*
    ����Ҫע�����״̬�ļ��������ظ��������������⡣
*/
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "%s/file_sn_status.dat",
             szPathName);

    char szMgrInfo[BUF_SIZE];
    SNPRINTF(szMgrInfo,
        sizeof(szMgrInfo),
        S_COL_FILE_SN_MGR,
        m_szSwitchName);
    szMgrInfo[BUF_SIZE - 1] = '\0';

    m_pFileSNMgr = new CFileSNMgr(szPathName,
                                  nCheckSNDepth,
                                  LEN_OF_AMA_FILE_SN,
                                  0,
                                  GetFileNameBySN);

    m_pFileSNMgr->SetMgrInfo(szMgrInfo);

    m_pFileSNMgr->SetSNLimit(pow((float)10, LEN_OF_AMA_FILE_SN), 1);

    if (ERR_SUCCESS != m_pFileSNMgr->Init())
    {
        delete m_pFileSNMgr;
        m_pFileSNMgr = NULL;
    }

    //added end

    //���ļ�ǰ׺����
    int nPrefixNum = ini.GetInt(szBuf,
                                CFG_5ESS_PREFIX_NUM,
                                CFG_5ESS_PREFIX_NUM_DEFAULT);
    //�����ļ�ǰ׺����ʼ��·������FileStore����
    char szTmpKey[BUF_SIZE];
    char szPrefixName[50];
    char szLabel[50];
    char szPriLink[50];
    char szBakLink[50];

    STRING strBillPath;
    for(int i = 1; i <= nPrefixNum; i++)
    {
        SNPRINTF(szTmpKey,
                 sizeof(szTmpKey),
                 CFG_5ESS_SEC_PREFIX,
                 m_nSwitchIndex,
                 i);
        ini.GetString(szTmpKey,
                      CFG_5ESS_PREFIX_NAME,
                      CFG_5ESS_PREFIX_NAME_DEFAULT,
                      szPrefixName,
                      sizeof(szPrefixName));

        //��ʶ
        ini.GetString(szTmpKey,
                      CFG_CO_MODULE_LABEL,
                      szTmpKey,
                      szLabel,
                      sizeof(szLabel));

        //�������ļ�, �õ�����m_szAreaCode
        char szTempAreaCode[20];
        ini.GetString(szTmpKey,
                      CFG_5ESS_AREA_CODE,
                      szAreaCode,
                      szTempAreaCode,
                      sizeof(szTempAreaCode));

        //�������ļ�,��������m_szBillType
        char szTempBillType[20];
        ini.GetString(szTmpKey,
                      CFG_5ESS_BILL_TYPE,
                      szBillType,
                      szTempBillType,
                      sizeof(szTempBillType));

        ini.GetString(szTmpKey,
                      CFG_CO_PRI_LINK,
                      CFG_CO_PRI_LINK_DEFAULT,
                      szPriLink,
                      sizeof(szPriLink));
        if((szPriLink[0] == '\0') || (NULL == m_pLinkGroup->GetLink(szPriLink)))
        {
            COTRACE(S_COL_MUST_CFG_PRI_LINK, szPrefixName);
            return ERR_FAIL;
        }

        ini.GetString(szTmpKey,
                      CFG_CO_BAK_LINK,
                      CFG_CO_BAK_LINK_DEFAULT,
                      szBakLink,
                      sizeof(szBakLink));

        m_vFilePrefix.push_back(STRING(szPrefixName));
        m_vAreaCode.push_back(szTempAreaCode);
        m_vBillType.push_back(szTempBillType);
        m_vLabel.push_back(szLabel);

        CLinkMgr* pLinkMgr = new CBKLinkMgr(m_pLinkGroup);
        pLinkMgr->RegisterLink(szPriLink);
        if((szBakLink[0] != '\0') && (NULL != m_pLinkGroup->GetLink(szBakLink)))
        {
            pLinkMgr->RegisterLink(szBakLink);
        }
        m_LinkMgrMap[STRING(szPrefixName)] = pLinkMgr;
    }

    SNPRINTF(szTmpKey,BUF_SIZE,"%s%d_5ess_fna",CFG_CO_SEC_SWITCH,m_nSwitchIndex);
    m_strFnaIndex = szTmpKey;
    ini.GetString(szTmpKey,
                  CFG_5ESS_FNA_NAME,
                  CFG_5ESS_FNA_NAME_DEFAULT,
                  szBuf,
                  MAX_PATH);
    m_strFnaFile = szBuf;

    //��������ָʾ�ļ����Ŀ¼
    ini.GetString(szTmpKey,CFG_5ESS_SAVE_FNA_DIR,
                  CFG_5ESS_SAVE_FNA_DIR_DEFAULT,
                  szPathName,
                  MAX_PATH);
    m_strFNARootPath = szPathName;
    CreateDeepDir(m_strFNARootPath.c_str());

    ini.GetString(szTmpKey,
                  CFG_CO_PRI_LINK,
                  CFG_CO_PRI_LINK_DEFAULT,
                  szPriLink,
                  sizeof(szPriLink));
    if((szPriLink[0] == '\0') || (NULL == m_pLinkGroup->GetLink(szPriLink)))
    {
        COTRACE(S_COL_MUST_CFG_PRI_LINK, "FNA");
        return ERR_FAIL;
    }

    ini.GetString(szTmpKey,
                  CFG_CO_BAK_LINK,
                  CFG_CO_BAK_LINK_DEFAULT,
                  szBakLink,
                  sizeof(szBakLink));

    CLinkMgr* pLinkMgr = new CBKLinkMgr(m_pLinkGroup);
    pLinkMgr->RegisterLink(szPriLink);
    if((szBakLink[0] != '\0') && (NULL != m_pLinkGroup->GetLink(szBakLink)))
    {
        pLinkMgr->RegisterLink(szBakLink);
    }
    m_LinkMgrMap[m_strFnaIndex] = pLinkMgr;

    return nRet;  //���򷵻�
}

/*
    ���������ڷǿ�ǰ׺��ƥ�䣬������ɹ��������ڿ�ǰ׺
    ����˵����Ϊ�յ�ǰ׺�������ǿ�ǰ׺�Ĳ���
*/
int C5ESSCollect::GetPrefixIndex(const char* const szSrcFile)
{
    int nIndex = -1;
    int i;
    for(i = 0; i < m_vFilePrefix.size(); i++)
    {
        if((m_vFilePrefix[i] != "")
           && strstr(szSrcFile, m_vFilePrefix[i].c_str()) != NULL)
        {
            nIndex = i;
            break;
        }
    }

    if(nIndex == -1)
    {
        for(i = 0; i < m_vFilePrefix.size(); i++)
        {
            if(m_vFilePrefix[i] == "")
            {
                nIndex = i;
                break;
            }
        }
    }

    return nIndex;
}

int C5ESSCollect::GetFileListIndex(const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nFileListIndex = -1;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if(StringiCmp(fi.szFileName, szSrcFile))
        {
            nFileListIndex = i;
            break;
        }
    }

    m_MutexForList.release();

    return nFileListIndex;
}

STRING C5ESSCollect::GetDstFileName(const char* const szSrcFile)
{
    char szDstName[MAX_PATH];

    strncpy(szDstName, szSrcFile, sizeof(szDstName));
    szDstName[sizeof(szDstName) - 1] = '\0';

    //�����ļ�����ʽ��ԭ�ļ���_HHSS.��������
    int nIndex = GetPrefixIndex(szSrcFile);
    if(nIndex != -1)
    {
        sprintf(szDstName,
                "%s_%s.%s",
                szSrcFile,
                Get_Hour_Min().c_str(),
                m_vBillType[nIndex].c_str());
    }

    return szDstName;
}

int C5ESSCollect::MakeFileNameMsg(const char* const szSrcFile,
                                  SGetFileReq& req)
{
    if(szSrcFile[0] == '\0')
    {
        return ERR_FAIL;
    }

    SGetFileReq stGetReq;
    memset(&stGetReq,0, sizeof(SGetFileReq));

    //5ESS�ϵ�Դ�ļ�·��
    strncpy(stGetReq.szSrcPath,
            m_strRemotePath.c_str(),
            sizeof(stGetReq.szSrcPath));

    //Դ�ļ���
    strncpy(stGetReq.szSrcName, szSrcFile, sizeof(stGetReq.szSrcName));

    //��ǰĿ�Ļ����ļ�·��
    strcpy(stGetReq.szDstPath, m_strLocalPath.c_str());

    //szBuf����µ�Ŀ���ļ���
    strncpy(stGetReq.szDstName,
            GetDstFileName(stGetReq.szSrcName).c_str(),
            sizeof(stGetReq.szDstName));

    req = stGetReq;

    return ERR_SUCCESS;
}

//�����ļ�����˵ĵ����ļ�������, �����������ʵ��;
int C5ESSCollect::Col_GetFile(SColFileReq& stReq)
{
    SGetFileReq file_req;
    int nRet = MakeFileNameMsg(stReq.szSrcName, file_req);
    if(nRet == ERR_SUCCESS)
    {
        file_req.nFileSize = stReq.nFileSize;

        if((stReq.szDstName != NULL)
           &&(stReq.szDstName[0] != 0))
        {
            STRING strPath;
            STRING strFile;
            divide_file_path_name(stReq.szDstName,
                                  strPath,
                                  strFile);
            if(strPath == "")
            {
                strPath = m_strLocalPath;
            }
#ifdef _PLATFORM_WIN32
            else if((strPath[1] != ':')
                    && (strPath[0] != '/')
                    && (strPath[0] != '\\'))
#else
            else if(strPath[0] != '/')
#endif
            {
                strPath = m_strLocalPath + "/" + strPath;
            }

            strncpy(file_req.szDstPath,
                    strPath.c_str(),
                    sizeof(file_req.szDstPath));
            strncpy(file_req.szDstName,
                    strFile.c_str(),
                    sizeof(file_req.szDstName));
            CreateDeepDir(file_req.szDstPath);
        }

        SNPRINTF(stReq.szDstName,
                 sizeof(stReq.szDstName),
                 "%s/%s",
                 file_req.szDstPath,
                 file_req.szDstName);

        nRet = GetFile(file_req);
        stReq.nFileSize = file_req.nFileSize;
    }

    return nRet;
}

int C5ESSCollect::GetFile(SGetFileReq& stReq)
{
    if(stReq.szSrcName[0] == '\0')
    {
        COTRACE("Remote file name is not specified: failed to get file");
        return ERR_FAIL;
    }


    //����Ƿ����ڱ��ɼ�����
    int nFileListIndex = GetFileListIndex(stReq.szSrcName);
    if(nFileListIndex == -1)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FILE_NOT_FOUND,
               m_szSwitchName,
               stReq.szSrcName);
        return ERR_FAIL;
    }

    //ȡ��·
    int  nRet = 0;
    m_pLinkMgr = NULL;
    int nIndex = GetPrefixIndex(stReq.szSrcName);
    if(nIndex == -1)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FILE_NOT_FOUND,
               m_szSwitchName,
               stReq.szSrcName);
        return ERR_FAIL;
    }
    m_pLinkMgr = m_LinkMgrMap[m_vFilePrefix[nIndex]];

    //������ʱ�ļ���
    string  strSrcFile = m_strTmpPath;
    strSrcFile += "/";
    strSrcFile += stReq.szDstName;

    //������ʽ�ļ���
    string  strDstFile = stReq.szDstPath;
    if((stReq.szDstPath[0] != 0) &&
       ((stReq.szDstPath[strlen(stReq.szDstPath) - 1] != '\\') &&
        (stReq.szDstPath[strlen(stReq.szDstPath) - 1] != '/')))
    {
        strDstFile += "/";
    }
    strDstFile += stReq.szDstName;

    m_MutexForList.acquire();
    m_FileList[nFileListIndex].nCollectStatus = CS_COLLECTING;
    m_MutexForList.release();

    strncpy(stReq.szDstPath, m_strTmpPath.c_str(), sizeof(stReq.szDstPath));
    stReq.szDstPath[sizeof(stReq.szDstPath) - 1] = '\0';

    //ȡ�ļ�
    nRet = m_pLinkMgr->GetFile(stReq);//���Ż�Ϊ����ȫ·������

    //��¼������־
    char szTraceMsg[1024];
    if( nRet == ERR_SUCCESS )
    {
        //�����ļ�����
        nRet = ERR_FAIL;
        FILE* f = fopen(strSrcFile.c_str(), "rb");
        if(f != NULL)
        {
            int nLength = FileSize(f);
            fclose(f);

            if(m_bFileLenVerify && stReq.nFileSize != nLength)
            {
                SNPRINTF(szTraceMsg,
                         sizeof(szTraceMsg),
                         S_COL_FILE_DISMATCHED_LENGTH,
                         m_szSwitchName,
                         stReq.szSrcName,
                         stReq.nFileSize,
                         nLength);
                szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
                COTRACE(szTraceMsg);
                COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);

                //�����ļ����ɼ������󣬽��������Զ�ɾ�����ļ����������ʧ�ܣ�
                //���ļ�����Ҳ�Ҳ������˴����سɹ����ٿɼ���һЩ��ʧ
                nRet = ERR_SUCCESS;
            }
            else
            {
                nRet = ERR_SUCCESS;
                stReq.nFileSize = nLength;
            }
        }

        if(nRet == ERR_SUCCESS)
        {
            //�ƶ���ʱ�ļ�Ϊ��ʽ�ļ�
            BOOL bRet = FileCopy(strSrcFile.c_str(), strDstFile.c_str() );
            if(!bRet)
            {
                SNPRINTF(szTraceMsg,
                         sizeof(szTraceMsg),
                         S_COL_FAILED_TO_MOVE_FILE,
                         m_szSwitchName,
                         strSrcFile.c_str(),
                         strDstFile.c_str());
                szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
                COTRACE(szTraceMsg);
                COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
                nRet = ERR_FAIL;
            }
            else
            {
                FileDel(strSrcFile.c_str());
            }
        }
    }
    else
    {
        SNPRINTF(szTraceMsg,
                 sizeof(szTraceMsg),
                 S_COL_FAILED_TO_COLLECT_FILE,
                 m_szSwitchName,
                 stReq.szSrcName,
                 strSrcFile.c_str());
        szTraceMsg[sizeof(szTraceMsg) - 1] = '\0';
        COTRACE(szTraceMsg);
        COMSGOUT(MSGOUT_LEVEL_URGENT, szTraceMsg);
    }

    if(nRet != ERR_SUCCESS)
    {
        remove(strSrcFile.c_str());
        remove(strDstFile.c_str());

        m_MutexForList.acquire();
        m_FileList[nFileListIndex].nCollectStatus = CS_NOT_COLLECTED;
        m_MutexForList.release();
    }
    else
    {
        m_MutexForList.acquire();
        m_FileList[nFileListIndex].nCollectStatus = CS_COLLECTED;
        m_MutexForList.release();


        //���ļ���Ϣ�ӵ�ת���ļ�����
        SSwitchFileInfo stFileInfo;
        stFileInfo.nSwitchIndex = m_nSwitchIndex;
        strncpy(stFileInfo.szModuleLabel,
                m_vLabel[nIndex].c_str(),
                sizeof(stFileInfo.szModuleLabel));
        strncpy(stFileInfo.szRemoteFile,
                stReq.szSrcName,
                sizeof(stFileInfo.szRemoteFile));
        strncpy(stFileInfo.szLocalFile,
                strDstFile.c_str(),
                sizeof(stFileInfo.szLocalFile));
        stFileInfo.nConvertInfo = nIndex + 1;

        GetFilePool()->PushBack(stFileInfo);
    }

    return  nRet;
}

//����ļ��б�, �����������ʵ��
int  C5ESSCollect::Col_GetList(SListOut& pOut)
{
    //�������Ϸ���
    int  nRet ;     //���Ӻ�������ֵ

    //���ָʾ�ļ�fna.txt
    SGetFileReq   stGetReq; // ��ȡ�ļ���������Ϣ
    memset(&stGetReq, 0, sizeof(stGetReq));

    stGetReq.nFileSize = 50; //���ᳬ��50K
    strcpy(stGetReq.szSrcName, m_strFnaFile.c_str()); //Դ�ļ���
    strcpy(stGetReq.szSrcPath, m_strRemotePath.c_str()); //Դ�ļ�·��

    //������ָʾ�ļ�����Ϊ"fna"+"��ǰ����".txt����ʽ
    //FNA_FILE_NAME : "fna_%s.txt"
    sprintf(stGetReq.szDstName, FNA_FILE_NAME, get_date_time());
    strcpy( stGetReq.szDstPath, m_strFNAPath.c_str());

    m_pLinkMgr = NULL;
    if (m_LinkMgrMap.find(m_strFnaIndex) != m_LinkMgrMap.end())
    {
        m_pLinkMgr = m_LinkMgrMap[m_strFnaIndex];
    }
    if(m_pLinkMgr == NULL)
    {
        COTRACE(S_COL_FAILED_TO_GET_FILE_LIST, m_szSwitchName);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FAILED_TO_GET_FILE_LIST,
               m_szSwitchName);
        return ERR_FAIL;
    }

    //�����ļ�����������ȡָʾ�ļ�
    nRet  = m_pLinkMgr->GetFile( stGetReq );
    if ( nRet != ERR_SUCCESS )
    {
        //������Ϣ�Ѽ�¼
        COTRACE(S_COL_FAILED_TO_GET_FNA,
              m_szSwitchName,
              nRet);
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FAILED_TO_GET_FNA,
               m_szSwitchName,
               nRet);
        return nRet;  //������󷵻�
    }

    //�����ļ�����·����Ϣ
    string  strFnaName = m_strFNAPath + DIR_CHAR;
    strFnaName += stGetReq.szDstName;

    //��ȡָʾ�ļ��е����ݣ����ļ����б�ŵ��ļ���Ϣ����������
    FILE*   fp = fopen( strFnaName.c_str(), "r");
    if( NULL == fp )
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FAILED_TO_OPEN_FNA,
               m_szSwitchName,
               strFnaName.c_str(),
               errno);
        COTRACE(S_COL_FAILED_TO_OPEN_FNA,
              m_szSwitchName,
              strFnaName.c_str(),
              errno);

        return COL_LISTFAIL;  //������󷵻�
    }

    char      szFileInfo[256];  //����һ���ļ���Ϣ���ַ���
    SFileInfo stFile;           //�ļ���Ϣ�ṹ

    /*���ж�ȡָʾ�ļ��е���Ϣ��ÿ�е���Ϣ���£�
      �ļ���    �ļ���С\r\n*/
    int  nCount = 0;
    while( !feof( fp ) )
    {
        memset( szFileInfo, 0, sizeof(szFileInfo) );
        if( NULL == fgets( szFileInfo, 255, fp ) )
        {
            continue;
        }

        //����Ϣ���õ��ļ��������浽stFile
        memset( &stFile, 0, sizeof(stFile) );
        if ( !GetSubName(szFileInfo, stFile) )
        {
            continue;
        }

        int nIndex = GetPrefixIndex(stFile.szFileName);
        if(nIndex != -1)
        {
            char szRemoteFile[MAX_PATH] = "";
            if(stGetReq.szSrcPath[0] != '\0')
            {
                strncpy(szRemoteFile, stGetReq.szSrcPath, sizeof(szRemoteFile));
                if((szRemoteFile[strlen(szRemoteFile) - 1] != '\\')
                   && (szRemoteFile[strlen(szRemoteFile) - 1] != '/'))
                {
                    strncat(szRemoteFile, "/", sizeof(szRemoteFile));
                }
            }
            strncat(szRemoteFile, stFile.szFileName, sizeof(szRemoteFile));
            unsigned long uFileLen = 0;
            if(m_pLinkMgr->GetFileLen(szRemoteFile, uFileLen) == 0)
            {
                stFile.nFileSize = uFileLen;
            }
            pOut.FileList.push_back(stFile);
        }

    }
    fclose( fp );

    //added by chenliangwei
    std::sort(pOut.FileList.begin(), pOut.FileList.end(), Compare());
    //added end

    m_MutexForList.acquire();

    m_FileList.clear();
    for(int i = 0; i < pOut.FileList.size(); i++)
    {
        SFileInfo& fi = pOut.FileList[i];
        fi.nCollectStatus = CS_NOT_COLLECTED;
        m_FileList.push_back(fi);
    }

    m_MutexForList.release();

    return  ERR_SUCCESS;  //��������, pOut�а����ļ��б���Ϣ
}

//��̬������Ŀ¼, �����������ʵ��
BOOL C5ESSCollect::CreateLocSubDir()
{
    char szTmp[24];
    time_t tNow = time(NULL);
    tm tmNow = *localtime(&tNow);
    strftime(szTmp,24,"%Y%m%d",&tmNow);
    m_strCurDate = szTmp;
    m_strLocalPath = m_strLocalRootPath + "/" + szTmp;
    CreateDeepDir(m_strLocalPath.c_str());

    m_strFNAPath = m_strFNARootPath + "/" + szTmp;
    CreateDeepDir(m_strFNAPath.c_str());

    /*STRING strSavePath;
    for(int i = 0; i <= m_vBillSavePath; i++)
    {
        strSavePath = m_vBillSavePath[i] + DIR_CHAR + m_strCurDate;
        if(!CreateDeepDir(strSavePath.c_str()))
        {
            m_strError = strerror(errno);
            return FALSE;
        }
    }*/
    return TRUE;
}

//��������ļ�, �����������ʵ��
BOOL C5ESSCollect::ClearOldFile()
{
    //added by chenliangwei
    int nRet1 = ClearOldDateSubDir(m_strLocalRootPath.c_str(),
                                   m_nFileSaveDays);
    int nRet2 = ClearOldDateSubDir(m_strFNARootPath.c_str(),
                                   m_nFileSaveDays);
    //added end
    return nRet1 == ERR_SUCCESS && nRet2 == ERR_SUCCESS;
}

//����ļ����к��Ƿ�������������©���ļ�
BOOL C5ESSCollect::CheckFileSN(const char* szFileName, char* szPreviousFile)
{
    if ((m_pFileSNMgr ==NULL)||(m_bCheckSN == 0))
    {
        return TRUE;
    }

    if (strstr(szFileName, PREFIX_AMA_FILE) == NULL)
    {
        return TRUE;
    }


    return m_pFileSNMgr->CheckFileSN(szFileName, szPreviousFile);

}

//�ļ��������Ļص�������ͨ���ļ������кż���©���ļ���
STRING C5ESSCollect::GetFileNameBySN(const int   nFileSN,
                                     const char* szReferName,
                                     const int   nSNLen,
                                     const int   nPostfixLen)
{

    //�������к�
    char szFormat[20];
    sprintf(szFormat, "%%0%dd", nSNLen);
    char* szFileSN = new char[nSNLen + 1];
    SNPRINTF(szFileSN, nSNLen + 1, szFormat, nFileSN);
    szFileSN[nSNLen] = '\0';

    STRING strFileName = "";
    strFileName = strFileName + PREFIX_AMA_FILE
                 + "*." + szFileSN;

    return strFileName;

}


BOOL C5ESSCollect::GetSubName(const char *sInfoLine,
                              SFileInfo& szFileInfo )
{

    char   buf[256];
    float  fFileSize = 0;

    if( NULL == sInfoLine)      //ΪNULL
    {
        return  FALSE;
    }

    if( strlen(sInfoLine) == 0 ) //Ϊ""
    {
        return  FALSE;
    }

    if( strlen(sInfoLine) > sizeof(buf)-1 )
    {
        return FALSE;
    }

    strcpy( buf, sInfoLine );
    buf[strlen(buf)-1] ='\0';   //ȥ��'\n'
    StringLRTrim(buf);
    if( strlen(buf) == 0 ) //Ϊ""
    {
        return  FALSE;
    }

    //������ �ļ��� �� �ļ���С;��ʽΪ��stream001.1234     1002.4       2002-10-21
    int nRet = sscanf(buf,"%s%f", szFileInfo.szFileName, &fFileSize );
    szFileInfo.nFileSize      = fFileSize * 1024;
    szFileInfo.nCollectStatus = CS_NOT_COLLECTED;

    return nRet == 2;

} //end of GetSubName

//added by chenliangwei
BOOL C5ESSCollect::Compare::operator ()(const SFileInfo &lfileinfo, const SFileInfo &rfileinfo)
{
    //�������AMA�ļ�����ŵ��б��β��
    if (strstr(rfileinfo.szFileName, PREFIX_AMA_FILE) == NULL)
    {
        return TRUE;
    }

    if (strstr(lfileinfo.szFileName, PREFIX_AMA_FILE) == NULL)
    {
        return FALSE;
    }

    char szlFileSN[10], szrFileSN[10];
    int  nlLen = 0, nrLen = 0;
    UINT4 nlFileSN = 0, nrFileSN = 0;

    nlLen = strlen(lfileinfo.szFileName);
    strcpy(szlFileSN, lfileinfo.szFileName + nlLen - LEN_OF_AMA_FILE_SN);
    nlFileSN = (UINT4)atoi(szlFileSN);

    nrLen = strlen(rfileinfo.szFileName);
    strcpy(szrFileSN, rfileinfo.szFileName + nlLen - LEN_OF_AMA_FILE_SN);
    nrFileSN = (UINT4)atoi(szrFileSN);

    return (nlFileSN < nrFileSN);
}
//added end

//ָ�����ļ��Ƿ�ɲ�
int C5ESSCollect::CanGetSpecFile(const char* const szModule,
                                 const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nRet = ERR_FILE_CANNOT_BE_FOUND;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if(strcmp(fi.szFileName, szSrcFile) == 0)
        {
            if(CS_NOT_COLLECTED == fi.nCollectStatus)
            {
                nRet = ERR_SUCCESS;
            }
            else if(CS_COLLECTING == fi.nCollectStatus)
            {
                nRet = ERR_FILE_BEING_COLLECTED;
            }
            else
            {
                nRet = ERR_NEW_FILE_CANNOT_BE_CO;
            }
            break;
        }
    }

    m_MutexForList.release();

    return nRet;
}

int C5ESSCollect::GetSpecFile(const char* const szModule,
                              const char* const szSrcFile,
                              const char* const szDstFile)
{
    int nFileListIndex = -1;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if(strcmp(szSrcFile, fi.szFileName) == 0)
        {
            nFileListIndex = i;
            break;
        }
    }

    if(nFileListIndex == -1)
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               S_COL_FAILED_TO_MAN_COL_FILE,
               m_szSwitchName,
               szSrcFile);
        return ERR_FAIL;
    }

    SGetFileReq file_req;
    int nRet = MakeFileNameMsg(szSrcFile, file_req);
    if(nRet == ERR_SUCCESS)
    {
        file_req.nFileSize = m_FileList[nFileListIndex].nFileSize;
        if((szDstFile != NULL) && (szDstFile[0] != 0))
        {
            STRING strPath;
            STRING strFile;
            divide_file_path_name(szDstFile, strPath, strFile);
            strncpy(file_req.szDstPath,
                    strPath.c_str(),
                    sizeof(file_req.szDstPath));
            strncpy(file_req.szDstName,
                    strPath.c_str(),
                    sizeof(file_req.szDstName));
        }
        nRet = GetFile(file_req);
    }

    return nRet;
}

//��ȡ��������Ϣ
int C5ESSCollect::GetSwitchInfo(LIST<STRING>& label,
                                LIST<STRING>& content)
{
    label.push_back("module");

    VECTOR<STRING>::iterator i = m_vLabel.begin();
    for(; i != m_vLabel.end(); i++)
    {
        content.push_back(*i);
    }

    return ERR_SUCCESS;
}

//��ȡģ����Ϣ
int C5ESSCollect::GetModuleInfo(const char* const szModule,
                                LIST<STRING>& label,
                                LIST<STRING>& content)
{
    label.push_back("�ļ���");
    label.push_back("�ļ�����");
    label.push_back("�ɼ�״̬");

    int nLabelIndex = -1;
    for(int i = 0; i < m_vLabel.size(); i++)
    {
        if(m_vLabel[i] == szModule)
        {
            nLabelIndex = i;
            break;
        }
    }

    if(nLabelIndex >= 0)
    {
        m_MutexForList.acquire();

        for(int i = 0; i < m_FileList.size(); i++)
        {
            SFileInfo& fi = m_FileList[i];
            if(GetPrefixIndex(fi.szFileName) != nLabelIndex)
            {
                continue;
            }

            content.push_back(fi.szFileName);

            char szBuf[20];
            SNPRINTF(szBuf, sizeof(szBuf), "%d", fi.nFileSize);
            content.push_back(szBuf);
            switch (fi.nCollectStatus)
            {
            case CS_COLLECTING:
                content.push_back(S_CS_COLLECTING);
                break;

            case CS_COLLECTED:
                content.push_back(S_CS_COLLECTED);
                break;

            case CS_NOT_COLLECTED:
            default:
                content.push_back(S_CS_NOT_COLLECTED);
                break;
            }
        }

        m_MutexForList.release();
    }

    if(content.size() == 0)
    {
        content.push_back("-");
        content.push_back("-");
        content.push_back("-");
    }

    return ERR_SUCCESS;
}

//��ȡ�ļ�����
int C5ESSCollect::GetFileLen(const char* const szModule,
                             const char* const szSrcFile)
{
    m_MutexForList.acquire();

    int nRet = 0;
    for(int i = 0; i < m_FileList.size(); i++)
    {
        SFileInfo& fi = m_FileList[i];
        if((strcmp(fi.szFileName, szSrcFile) == 0)
           && (CS_NOT_COLLECTED == fi.nCollectStatus))
        {
            nRet = fi.nFileSize;
            break;
        }
    }

    m_MutexForList.release();

    return nRet;
}

