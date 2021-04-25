#include "collect_convert_subobj.h"
#include "collect_convert_base.h"
#include "comm_convert_fun.h"
#include "../../include/collect_msg_def.h"
#include "../../include/format_out.h"
#include "./com/tellin_convert_fun.h"


//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
CCollectConvertSubObj::CCollectConvertSubObj(int nSwitchIndex,
                                             int nSubObjIndex,
                                             IBillProcessor* pIBillProcessor)
{
    m_nSwitchIndex         = nSwitchIndex;
    m_nSubObjIndex         = nSubObjIndex;
    m_pIBillProcessor      = pIBillProcessor;

    m_szSwitchName[0]      = '\0';
    m_szCollectId[0]       = '\0';
    m_szCfgPath[0]         = '\0';
    m_szSubObjName[0]      = '\0';
    m_szFinalRootPath[0]   = '\0';
    m_szFinalPath[0]       = '\0';
    m_szSecondFinalPath[0] = '\0';
    m_szBillType[0]        = '\0';

    m_bFileLevelVerify     = 0;
    m_bFileLevelFailReCol  = 0;
    m_bBillLevelVerify     = 0;
    m_bBillLevelFailReCol  = 0;
    m_bCrtZeroFnlFile      = 0;
    m_bSecondCopy          = FALSE;
    m_bNeedSecondAdapter   = FALSE;

    m_bCrtZeroFnlFile      = FALSE;
    m_nOrigBillLen         = 0;
}

//��������
CCollectConvertSubObj::~CCollectConvertSubObj()
{
    //

}


int CCollectConvertSubObj::Init()
{
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        return ERR_FAIL;
    }

    /*
        ����ؽ������ֶλ�ȡ��Ϣ
    */
    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName, sizeof(szSecName), "%s%d", //"switch*"
             CFG_CO_SEC_SWITCH, m_nSwitchIndex);
    szSecName[CFG_BUF_SIZE - 1] = '\0';


    //��ȡ�ɼ�����ID
    INIFile.GetString(szSecName, CFG_CO_TYPE,
                      CFG_CO_TYPE_DEFAULT,
                      m_szCollectId,
                      sizeof(m_szCollectId));
    m_szCollectId[sizeof(m_szCollectId) - 1] = '\0';

    //ȡ���ջ���Ŀ¼
    INIFile.GetString(szSecName,
                      CFG_CO_FINAL_DIR,
                      CCollectorAdapter::GetBackSavePath(),
                      m_szFinalRootPath,
                      sizeof(m_szFinalRootPath));


    //��ȡ���ջ����ڶ��ݵ�Ŀ¼
    INIFile.GetString(szSecName,
                      CFG_CO_SECOND_FINAL_DIR,
                      CCollectorAdapter::GetSecondBackSavePath(),
                      m_szSecondFinalPath,
                      sizeof(m_szSecondFinalPath));

     //�����ļ���������
    m_nFileSaveDays = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_FINAL_FILE_SAVE_DAYS,
                                 CFG_CO_FINAL_FILE_SAVE_DAYS_DEFAULT);

    m_nFileSaveDays = INIFile.GetInt(szSecName,
                                 CFG_CO_FINAL_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);

    //�Ƿ�����ļ���У��
    m_bFileLevelVerify = INIFile.GetInt(szSecName,
                                 CFG_CO_FILE_LEVEL_VERIFY,
                                 CFG_CO_FILE_LEVEL_VERIFY_DEFAULT);

    //�ļ���У��ʧ���Ƿ��ز�
    m_bFileLevelFailReCol = INIFile.GetInt(szSecName,
                                 CFG_CO_FILE_LEVEL_FAIL_RECOL,
                                 CFG_CO_FILE_LEVEL_FAIL_RECOL_DEFAULT);

    //�Ƿ���л�����У��
    m_bBillLevelVerify = INIFile.GetInt(szSecName,
                                 CFG_CO_BILL_LEVEL_VERIFY,
                                 CFG_CO_BILL_LEVEL_VERIFY_DEFAULT);

    //������У��ʧ���Ƿ��ز�
    m_bBillLevelFailReCol = INIFile.GetInt(szSecName,
                                 CFG_CO_BILL_LEVEL_FAIL_RECOL,
                                 CFG_CO_BILL_LEVEL_FAIL_RECOL_DEFAULT);

    //�Ƿ�����0�ֽڵ����ջ����ļ�
    m_bCrtZeroFnlFile = INIFile.GetInt(szSecName,
                                 CFG_CO_CREATE_ZERO_FINAL_FILE,
                                 CFG_CO_CREATE_ZERO_FINAL_FILE_DEFAULT);


    //ԭʼ�����ĳ���
    m_nOrigBillLen = INIFile.GetInt(szSecName,
                                    CFG_CO_ORIG_BILL_LEN,
                                    CFG_CO_ORIG_BILL_LEN__DEFAULT);


    /*
        �����SubObj�ֶλ�ȡ��Ϣ
    */
    SNPRINTF(szSecName, sizeof(szSecName),
             "%s%d_%s",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex,
             m_szSubObjName);
    szSecName[CFG_BUF_SIZE - 1] = '\0';


    //ȡ���ջ���Ŀ¼
    INIFile.GetString(szSecName,
                      CFG_CO_FINAL_DIR,
                      m_szFinalRootPath,
                      m_szFinalRootPath,
                      sizeof(m_szFinalRootPath));
    int nLen = strlen(m_szFinalRootPath) - 1;
    if(nLen > 0)
    {
        if((m_szFinalRootPath[nLen] == '\\')
           || (m_szFinalRootPath[nLen] == '/'))
        {
            m_szFinalRootPath[nLen] = '\0';
        }
    }
    else if(strlen(m_szFinalRootPath) == 0)
    {
        COTRACE("�ɼ�����(%s,%s)���ջ���Ŀ¼û������",
            m_szSwitchName,
            m_szSubObjName);

        return ERR_FAIL;
    }

    if (!CreateDeepDir(m_szFinalRootPath))
    {
        COTRACE("�ɼ�����(%s,%s)���ջ���Ŀ¼(%s)�޷������ɹ�",
            m_szSwitchName,
            m_szSubObjName,
            m_szFinalRootPath);

        return ERR_FAIL;
    }

    //�Ƿ���Ҫ���Ƶڶ������ջ����ļ�
    m_bSecondCopy = CCollectorAdapter::GetbAPSaveSecond();

    //�Ƿ���Ҫ�ڶ��ݻ�������    
    m_bNeedSecondAdapter = CCollectorAdapter::GetbNeedSencondAdapter();

    //��ȡ���ջ����ڶ��ݵ�Ŀ¼
    INIFile.GetString(szSecName, CFG_CO_SECOND_FINAL_DIR,
                      m_szSecondFinalPath,
                      m_szSecondFinalPath,
                      sizeof(m_szSecondFinalPath));
    nLen = strlen(m_szSecondFinalPath) - 1;
    if(nLen > 0)
    {
        if((m_szSecondFinalPath[nLen] == '\\')
           || (m_szSecondFinalPath[nLen] == '/'))
        {
            m_szSecondFinalPath[nLen] = '\0';
        }
    }

    //�����ļ���������
    m_nFileSaveDays = INIFile.GetInt(szSecName,
                                 CFG_CO_FINAL_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);

    //�Ƿ�����ļ���У��
    m_bFileLevelVerify = INIFile.GetInt(szSecName,
                                 CFG_CO_FILE_LEVEL_VERIFY,
                                 m_bFileLevelVerify);

    //�ļ���У��ʧ���Ƿ��ز�
    m_bFileLevelFailReCol = INIFile.GetInt(szSecName,
                                 CFG_CO_FILE_LEVEL_FAIL_RECOL,
                                 m_bFileLevelFailReCol);

    //�Ƿ���л�����У��
    m_bBillLevelVerify = INIFile.GetInt(szSecName,
                                 CFG_CO_BILL_LEVEL_VERIFY,
                                 m_bBillLevelVerify);

    //������У��ʧ���Ƿ��ز�
    m_bBillLevelFailReCol = INIFile.GetInt(szSecName,
                                 CFG_CO_BILL_LEVEL_FAIL_RECOL,
                                 m_bBillLevelFailReCol);

    //�Ƿ�����0�ֽڵ����ջ����ļ�
    m_bCrtZeroFnlFile = INIFile.GetInt(szSecName,
                                 CFG_CO_CREATE_ZERO_FINAL_FILE,
                                 m_bCrtZeroFnlFile);
    //ԭʼ�����ĳ���
    m_nOrigBillLen = INIFile.GetInt(szSecName,
                                    CFG_CO_ORIG_BILL_LEN,
                                    m_nOrigBillLen);


    //��ȡ������TP��Ӧ��������
    INIFile.GetString(szSecName,
                      CFG_CO_BILL_TYPE,
                      CFG_CO_BILL_TYPE_DEFAULT,
                      m_szBillType,
                      sizeof(m_szBillType));

    m_pConvertFun = ProcessOtherFile;

    if (NULL != m_pIBillProcessor)
    {
        m_pIBillProcessor->GetChannelNames(m_ChlNames);
    }

    m_ChlNames.push_front(DEFAULT_CHL);

    INIFile.Close();
    return ERR_SUCCESS;
}

int  CCollectConvertSubObj::Convert(SConvertFileReq& stReq)
{
    if(stReq.szSrcName[0] == '\0')
    {
        COTRACE("�ɼ��ļ�������Ϊ�գ������������.");
        //COTRACE("file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    //����Ŀ���ļ���·��
    ConstuctDestFileLst(stReq);


    //ת�������ļ�
    int nRet =   ConvertFile(stReq);

/*
    if (ERR_SUCCESS != nRet)
    {
        TRACE("�����ļ�ת��ʧ��.");
        return ERR_FAIL;
    }
*/
    //����0�ֽڵ����ջ����ļ�
    //��ÿ��û�л�����ͨ����������0�ֽڻ���
    if ((m_bCrtZeroFnlFile) && (nRet == ERR_SUCCESS))
    {
	    for(int i = 1; i < m_CvtResultList.size(); i++)
        {
            if(m_CvtResultList[i] == 0)
            {
                FILE* pFile = fopen(m_FinalFileList[i].c_str(), "wb");
                if(pFile == NULL)
                {
                    COMSGOUT(MSGOUT_LEVEL_URGENT,
                           "����0�ֽ����ջ����ļ�(%s)ʧ��",            
                           m_FinalFileList[i].c_str());
                        
                    WRITELOG_F("����0�ֽ����ջ����ļ�(%s)ʧ��",    
                               m_FinalFileList[i].c_str());
                }
                else
                {
                    fclose(pFile);
                    pFile = NULL;
                    m_CvtResultList[i] = 1;
                }
            }
        }
    }

    //����ʱĿ¼�µ��ļ��ƶ�����ʽĿ¼��
    RenameDestFile();


    //����ļ����л���ת���ɹ�������ֵΪERR_SUCCESS
    if (ERR_SUCCESS == nRet)
    {
        int nChlCount = m_CvtResultList.size();
        for (int i = 1; i < nChlCount; i++)
        {
            if (i%m_ChlNames.size() == 0) continue;
            if (m_CvtResultList[i] > 0)
            {
				OnFileConvertSucc(i);
                COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                       "�ɼ�����(%s)ת���ļ�(%s->%s)�ɹ�",
                       m_szSwitchName,
                       stReq.szSrcName,
                       m_FinalFileList[i].c_str());

                WRITELOG_C("�ɼ�����(%s)ת���ļ�(%s->%s)�ɹ�",
                           m_szSwitchName,
                           stReq.szSrcName,
                           m_FinalFileList[i].c_str());
            }
        }

        if ((m_CvtResultList[0] > 0) && (m_bBillLevelVerify))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "�ɼ�����(%s)���ļ�(%s)�з��ִ��󻰵������洢Ϊ�����ļ�(%s)",
                   m_szSwitchName,
                   stReq.szSrcName,
                   m_FinalFileList[0].c_str());

            WRITELOG_F("�ɼ�����(%s)���ļ�(%s)�з��ִ��󻰵������洢Ϊ�����ļ�(%s)",
                       m_szSwitchName,
                       stReq.szSrcName,
                       m_FinalFileList[0].c_str());

            nRet = ERR_FAIL;

            //modifyed by chenliangwei, 2003-08-05
            //Ҫ��ɼ�ģ���زɻ�����
            if (m_bBillLevelFailReCol)
            {
                nRet = COL_RECOL_BILL;
            }
            //modifyed end
        }
    }
    //�������ΪERR_FAIL�������д����ļ����ɣ���ʾ���ļ���У��ʧ��
    else if (m_CvtResultList[0] > 0)
    {
        if (m_bFileLevelVerify)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "�ɼ�����(%s)ת���ļ�(%s)ʧ��, �洢Ϊ�����ļ�(%s)",
                   m_szSwitchName,
                   stReq.szSrcName,
                   m_FinalFileList[0].c_str());

            WRITELOG_F("�ɼ�����(%s)ת���ļ�(%s)ʧ��, �洢Ϊ�����ļ�(%s)",
                       m_szSwitchName,
                       stReq.szSrcName,
                       m_FinalFileList[0].c_str());

            //modifyed by chenliangwei, 2003-08-05
            //Ҫ��ɼ�ģ���زɻ�����
            if (m_bFileLevelFailReCol)
            {
                nRet = COL_RECOL_BILL;
            }
            //modifyed end
        }
        else
        {
            nRet = ERR_SUCCESS;
        }

    }
    //�����쳣���󣬲���û�д����ļ�����
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s)ת���ļ�(%s)ʧ��",
               m_szSwitchName,
               stReq.szSrcName);

        WRITELOG_F("�ɼ�����(%s)ת���ļ�(%s)ʧ��",
                   m_szSwitchName,
                   stReq.szSrcName);

        nRet = COL_WRITE_BILL_FAIL;

        return nRet;
    }

    /*if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }
    */


    //�����ڶ����ļ�
    if (m_bSecondCopy
        && (m_szSecondFinalPath[0] != '\0'))
    {
       // nRet = CopySecondFile();
        CopySecondFile();
    }

    return nRet;

}

int CCollectConvertSubObj::CreateLocSubDir()
{
    time_t tNow = time(NULL);
    tm tmNow = *localtime(&tNow);
    strftime(m_szCurDate, sizeof(m_szCurDate), "%Y%m%d", &tmNow);

    char szFinalPath[MAX_PATH];
    LIST<STRING>::iterator i = m_ChlNames.begin();

    //�������ջ���Ŀ¼
    m_FinalDirList.clear();
    for (; i != m_ChlNames.end(); i++)
    {
        //��ʱĿ¼
        SNPRINTF(szFinalPath,
             sizeof(szFinalPath),
             "%s/%s/temp",
             m_szFinalRootPath,
             (*i).c_str());
        szFinalPath[sizeof(szFinalPath) - 1] = '\0';

        CreateDeepDir(szFinalPath);

        //��ʽĿ¼
        SNPRINTF(szFinalPath,
             sizeof(szFinalPath),
             "%s/%s/%s",
             m_szFinalRootPath,
             (*i).c_str(),
             m_szCurDate);
        szFinalPath[sizeof(szFinalPath) - 1] = '\0';

        CreateDeepDir(szFinalPath);

        m_FinalDirList.push_back(szFinalPath);
    }


    //�������ջ����ڶ��ݵ�Ŀ¼
    if (m_bSecondCopy && m_szSecondFinalPath[0] != '\0')
    {
        i = m_ChlNames.begin();
        for (; i != m_ChlNames.end(); i++)
        {
            SNPRINTF(szFinalPath,
                 sizeof(szFinalPath),
                 "%s/%s",
                 m_szSecondFinalPath,
                 (*i).c_str());

            CreateDeepDir(szFinalPath);

            m_SecondDirList.push_back(szFinalPath);
        }

    }

    return ERR_SUCCESS;
}

int CCollectConvertSubObj::ClearOldFile()
{
    char szFinalPath[MAX_PATH];
    LIST<STRING>::iterator i = m_ChlNames.begin();

    //ɾ�����ջ���Ŀ¼
    for (; i != m_ChlNames.end(); i++)
    {
        SNPRINTF(szFinalPath,
             sizeof(szFinalPath),
             "%s/%s",
             m_szFinalRootPath,
             (*i).c_str());

       CCollectConvertBase::ClearOldDateSubDir(szFinalPath,
                                  m_nFileSaveDays);
    }

    return ERR_SUCCESS;
}


void CCollectConvertSubObj::ConstuctDestFileLst(SConvertFileReq& stReq)
{
    //��Ϊ�����Դ�ļ�����ȫ·������Ҫ���ļ�����·���ֿ�
    STRING strSrcFileName = "";
    STRING strSrcPath = "";

    m_FinalFileList.clear();
    m_CvtResultList.clear();

    divide_file_path_name(stReq.szSrcName,
                          strSrcPath,
                          strSrcFileName);

    //������ջ����ļ�
    STRING strDstName = "";
    strDstName = GetDstFileName(strSrcFileName.c_str());

    //ת��
    char szFinalFile[MAX_PATH] = "";

    LIST<STRING>::iterator iter = m_ChlNames.begin();

    for (; iter != m_ChlNames.end(); iter++)
    {
        SNPRINTF(szFinalFile,
             sizeof(szFinalFile),
             "%s/%s/temp/%s",
             m_szFinalRootPath,
             (*iter).c_str(),
             strDstName.c_str());
        szFinalFile[sizeof(szFinalFile) - 1] = '\0';

        m_FinalFileList.push_back(szFinalFile);
        m_CvtResultList.push_back(0);
    }
}

//����ʱĿ¼�µ��ļ��ƶ�����ʽĿ¼��
int CCollectConvertSubObj::RenameDestFile()
{
    char szFinalFile[MAX_PATH] = "";
    STRING strSrcFileName = "";
    STRING strSrcPath = "";

    int nChlCount = m_ChlNames.size();
    for (int i = 0; i < nChlCount; i++)
    {
        divide_file_path_name(m_FinalFileList[i].c_str(),
            strSrcPath,
            strSrcFileName);
        
        SNPRINTF(szFinalFile,
            sizeof(szFinalFile),
            "%s/%s",
            m_FinalDirList[i].c_str(),
            strSrcFileName.c_str());
        szFinalFile[sizeof(szFinalFile) - 1] = '\0';
        
        FileMove(m_FinalFileList[i].c_str(), szFinalFile);
        m_FinalFileList[i] = szFinalFile;
    }

    return ERR_SUCCESS;
}

//�����ڶ����ļ�
int CCollectConvertSubObj::CopySecondFile()
{
    int nRet = ERR_SUCCESS;
    STRING strSecondFile = "";
    STRING strSrcFileName = "";
    STRING strSrcPath = "";
    
    int nChlCount = m_ChlNames.size();
    for (int i = 0; i < nChlCount; i++)
    {
        if (m_CvtResultList[i] > 0)
        {
            divide_file_path_name(m_FinalFileList[i].c_str(),
                                  strSrcPath,
                                  strSrcFileName);
            
            strSecondFile = m_SecondDirList[i];
            strSecondFile += "/" + strSrcFileName;
            
            BOOL bSuccess = TRUE;
            bSuccess = FileCopy(m_FinalFileList[i].c_str(), 
                strSecondFile.c_str());
            
            
            if (!bSuccess)
            {
                COMSGOUT(MSGOUT_LEVEL_URGENT,
                    "�ɼ�����(%s)�������ջ����ļ�(%s->%s)ʧ��",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                
                WRITELOG_F("�ɼ�����(%s)�������ջ����ļ�(%s->%s)ʧ��",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                nRet = ERR_FAIL;
                break;
            }
            else
            {
                COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                    "�ɼ�����(%s)�������ջ����ļ�(%s->%s)�ɹ�",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                
                WRITELOG_C("�ɼ�����(%s)�������ջ����ļ�(%s->%s)�ɹ�",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
            }
        }
    }
    
    return nRet;
}

//ת�������ļ�
int CCollectConvertSubObj::ConvertFile(SConvertFileReq& stReq)
{
    
    int nRet = m_pConvertFun(stReq.szSrcName,
                             m_FinalFileList,
                             m_CvtResultList,
                             m_bFileLevelVerify,
                             m_bBillLevelVerify,
                             m_pIBillProcessor,
                             m_nOrigBillLen);
    return nRet;
}

STRING CCollectConvertSubObj::GetDstFileName(const char* szSrcName)
{
    STRING strDstName = szSrcName;
    return strDstName;
}

STRING CCollectConvertSubObj::GetErrFileName(const char* szSrcName)
{
    STRING strErrName = szSrcName;
    return strErrName;
}

void CCollectConvertSubObj::OnFileConvertSucc(int nChannel)
{
    return;
}