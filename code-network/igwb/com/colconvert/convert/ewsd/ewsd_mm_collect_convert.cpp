#include "ewsd_mm_collect_convert.h"
#include "../comm_convert_fun.h"
#include "../../../include/collect_msg_def.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
CEWSDMMCollectConvert::CEWSDMMCollectConvert(int nSwitchIndex)
                        :CCollectConvertBase(nSwitchIndex) 
{
    //
}

//��������
CEWSDMMCollectConvert::~CEWSDMMCollectConvert()
{
    //
}


int CEWSDMMCollectConvert::Init()
{
    
    int nRet = CCollectConvertBase::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //����S1240��������Ϣ
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(%s)ת��ģ��������ļ�(%s)ʧ��", 
              m_szSwitchName, 
              m_szCfgPath);
        return ERR_FAIL;
    }

    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[CFG_BUF_SIZE - 1] = '\0';

    //ȡ���ջ���Ŀ¼
    INIFile.GetString(szSecName,
                      CFG_CO_FINAL_DIR, 
                      CCollectorAdapter::GetBackSavePath(), 
                      m_szFinalRootPath, 
                      sizeof(m_szFinalRootPath));
    int nLen = strlen(m_szFinalRootPath) - 1;
    if(nLen >= 0)
    {
        if((m_szFinalRootPath[nLen] == '\\')
           || (m_szFinalRootPath[nLen] == '/'))
        {
            m_szFinalRootPath[nLen] = '\0';
        }
        strncat(m_szFinalRootPath, "/", sizeof(m_szFinalRootPath));
        m_szFinalRootPath[sizeof(m_szFinalRootPath) - 1] = '\0';
    }
    else
    {
        COTRACE("�ɼ�����(%s)���ջ���Ŀ¼û������",
            m_szSwitchName);

        return ERR_FAIL;
    }

    //��ȡ���ջ����ڶ��ݵ�Ŀ¼
    INIFile.GetString(szSecName, 
                      CFG_CO_SECOND_FINAL_DIR, 
                      CCollectorAdapter::GetSecondBackSavePath(), 
                      m_szSecondFinalPath, 
                      sizeof(m_szSecondFinalPath));
    nLen = strlen(m_szSecondFinalPath) - 1;
    if(nLen >= 0)
    {
        if((m_szSecondFinalPath[nLen] == '\\')
           || (m_szSecondFinalPath[nLen] == '/'))
        {
            m_szSecondFinalPath[nLen] = '\0';
        }

        strncat(m_szSecondFinalPath, "/", sizeof(m_szSecondFinalPath));
        m_szSecondFinalPath[sizeof(m_szSecondFinalPath) - 1] = '\0';
    }


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

    //��ȡ������TP��Ӧ��������
    INIFile.GetString(szSecName, 
                      CFG_CO_BILL_TYPE, 
                      CFG_CO_BILL_TYPE_DEFAULT, 
                      m_szBillType, 
                      sizeof(m_szBillType));

    //���ݸ��ݲ�ͬ�Ļ�������ָ����ͬת������
    if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "�ɼ�����(%s)��⵽��Ч�Ļ�������(%s)",
             m_szSwitchName,
             m_szBillType);

        COTRACE("�ɼ�����(%s)��⵽��Ч�Ļ�������(%s)",
             m_szSwitchName,
             m_szBillType);

        return ERR_FAIL;

    }

    return ERR_SUCCESS;
}

int  CEWSDMMCollectConvert::Convert(SConvertFileReq& stReq)
{
/*    if(stReq.szSrcName[0] == '\0')
    {
        //COTRACE("�ɼ��ļ�������Ϊ�գ������������.");
        COTRACE("file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    //��Ϊ�����Դ�ļ�����ȫ·������Ҫ���ļ�����·���ֿ�
    STRING strSrcFileName = "";
    STRING strSrcPath = "";

    divide_file_path_name(stReq.szSrcName, 
                          strSrcPath,
                          strSrcFileName);

    int  nRet = ERR_SUCCESS;
    STRING strDstName = "";
    STRING strErrName = "";
    STRING strFinalFile = m_szFinalPath;
    STRING strErrorFile = m_szErrorFilePath;

    //������ջ����ļ�
    strDstName = GetDstFileName(strSrcFileName.c_str());
    strFinalFile += strDstName; 

    //��ô��󻰵��ļ�
    strErrName = GetErrFileName(strSrcFileName.c_str());
    strErrorFile += strErrName;
  
    //ת��ʱ��ת��Ϊ��ʱ�ļ�
    STRING strTmpFinalFile = strFinalFile + ".tmp";
    STRING strTmpErrorFile = strErrorFile + ".tmp";    

    //ת�������ļ�
    nRet = m_pConvertFun(stReq.szSrcName,
                         strFinalFile.c_str(),
                         strErrorFile.c_str(),
                         m_bFileLevelVerify,
                         m_bBillLevelVerify);

    switch (nRet)
    {
    case ERR_SUCCESS:
        FileMove(strTmpFinalFile.c_str(), strFinalFile.c_str());

        COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
               "�ɼ�����(%s)ת���ļ�(%s->%s)�ɹ�", 
               m_szSwitchName,
               stReq.szSrcName,
               strFinalFile.c_str());

        WRITELOG_C("�ɼ�����(%s)ת���ļ�(%s->%s)�ɹ�", 
                   m_szSwitchName,
                   stReq.szSrcName,
                   strFinalFile.c_str());
        break;

    case COL_CONVERT_PART_SUCC:
        FileMove(strTmpFinalFile.c_str(), strFinalFile.c_str());
        FileMove(strTmpErrorFile.c_str(), strErrorFile.c_str());

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s)ת���ļ�(%s->%s)���ֳɹ�, ���󻰵��洢Ϊ�����ļ�(%s)", 
               m_szSwitchName,
               stReq.szSrcName,
               strFinalFile.c_str(),
               strErrorFile.c_str());

        WRITELOG_F("�ɼ�����(%s)ת���ļ�(%s->%s)���ֳɹ�, ���󻰵��洢Ϊ�����ļ�(%s)", 
                   m_szSwitchName,
                   stReq.szSrcName,
                   strFinalFile.c_str(),
                   strErrorFile.c_str());

        //Ҫ��ɼ�ģ���زɻ�����
        if (m_bBillLevelFailReCol)
        {
            nRet = COL_RECOL_BILL;
        }
        break;


    case ERR_FAIL:
        FileMove(strTmpErrorFile.c_str(), strErrorFile.c_str());

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s)ת���ļ�(%s)ʧ��, �洢Ϊ�����ļ�(%s)", 
               m_szSwitchName,
               stReq.szSrcName,
               strErrorFile.c_str());

        WRITELOG_F("�ɼ�����(%s)ת���ļ�(%s)ʧ��, �洢Ϊ�����ļ�(%s)", 
                   m_szSwitchName,
                   stReq.szSrcName,
                   strErrorFile.c_str());

        //Ҫ��ɼ�ģ���زɻ�����
        if (m_bFileLevelFailReCol)
        {
            nRet = COL_RECOL_BILL;
        }

        break;

    default:
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "�ɼ�����(%s)ת���ļ�(%s)ʧ��, ������(%d)", 
               m_szSwitchName,
               stReq.szSrcName,
               nRet);

        WRITELOG_F("�ɼ�����(%s)ת���ļ�(%s)ʧ��, ������(%d)", 
                   m_szSwitchName,
                   stReq.szSrcName,
                   nRet);
    }
*/
    /*if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }
    */


    //�����ڶ����ļ�
/*    if ((m_szSecondFinalPath[0] != '\0')
        && ((nRet == ERR_SUCCESS)||(nRet == COL_CONVERT_PART_SUCC)))
    {
        STRING strSecondFile = m_szSecondFinalPath;
        strSecondFile += strDstName;

        if (!FileCopy(strFinalFile.c_str(), strSecondFile.c_str()))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "�ɼ�����(%s)�������ջ����ļ�(%s->%s)ʧ��", 
                   m_szSwitchName,
                   strFinalFile.c_str(),
                   strSecondFile.c_str());

            WRITELOG_F("�ɼ�����(%s)�������ջ����ļ�(%s->%s)ʧ��", 
                       m_szSwitchName,
                       strFinalFile.c_str(),
                       strSecondFile.c_str());

            //nRet = ERR_FAIL;
        }
        else
        {
            COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                   "�ɼ�����(%s)�������ջ����ļ�(%s->%s)�ɹ�", 
                   m_szSwitchName,
                   strFinalFile.c_str(),
                   strSecondFile.c_str());

            WRITELOG_C("�ɼ�����(%s)�������ջ����ļ�(%s->%s)�ɹ�", 
                       m_szSwitchName,
                       strFinalFile.c_str(),
                       strSecondFile.c_str());
        }
    }

    return nRet;
*/
    return ERR_SUCCESS;
}

int CEWSDMMCollectConvert::CreateLocSubDir()
{
    char szTmp[24];
    time_t tNow = time(NULL);
    tm tmNow = *localtime(&tNow);
    strftime(szTmp,24,"%Y%m%d",&tmNow);

    //�������ջ���Ŀ¼
    SNPRINTF(m_szFinalPath, 
         sizeof(m_szFinalPath),
         "%s%s/",
         m_szFinalRootPath,
         szTmp);

    CreateDeepDir(m_szFinalPath);

    //�������󻰵�Ŀ¼
    //CreateDeepDir(m_szErrorFilePath);

    //�������ջ����ڶ��ݵ�Ŀ¼
    if (m_szSecondFinalPath[0] != '\0')
    {
        CreateDeepDir(m_szSecondFinalPath);
    }

    return ERR_SUCCESS;
}

int CEWSDMMCollectConvert::ClearOldFile()
{
    return  ClearOldDateSubDir(m_szFinalRootPath, 
                                  m_nFileSaveDays);
 
}

STRING CEWSDMMCollectConvert::GetDstFileName(const char* szSrcName)
{
    STRING strDstName = szSrcName;
    return strDstName;
}

STRING CEWSDMMCollectConvert::GetErrFileName(const char* szSrcName)
{
    STRING strErrName = szSrcName;
    return strErrName;
}
