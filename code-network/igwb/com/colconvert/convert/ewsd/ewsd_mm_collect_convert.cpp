#include "ewsd_mm_collect_convert.h"
#include "../comm_convert_fun.h"
#include "../../../include/collect_msg_def.h"

//以下是采集操作的基类CCollectConvertBase的定义
//构造函数
CEWSDMMCollectConvert::CEWSDMMCollectConvert(int nSwitchIndex)
                        :CCollectConvertBase(nSwitchIndex) 
{
    //
}

//析构函数
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

    //读入S1240的配置信息
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("采集任务(%s)转换模块打开配置文件(%s)失败", 
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

    //取最终话单目录
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
        COTRACE("采集对象(%s)最终话单目录没有配置",
            m_szSwitchName);

        return ERR_FAIL;
    }

    //获取最终话单第二份的目录
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


     //话单文件保存天数
    m_nFileSaveDays = INIFile.GetInt(CFG_CO_SEC_SYSTEM,
                                 CFG_CO_FINAL_FILE_SAVE_DAYS,
                                 CFG_CO_FINAL_FILE_SAVE_DAYS_DEFAULT);

    m_nFileSaveDays = INIFile.GetInt(szSecName,
                                 CFG_CO_FINAL_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);

    //是否进行文件级校验
    m_bFileLevelVerify = INIFile.GetInt(szSecName,
                                 CFG_CO_FILE_LEVEL_VERIFY,
                                 CFG_CO_FILE_LEVEL_VERIFY_DEFAULT);

    //文件级校验失败是否重采
    m_bFileLevelFailReCol = INIFile.GetInt(szSecName,
                                 CFG_CO_FILE_LEVEL_FAIL_RECOL,
                                 CFG_CO_FILE_LEVEL_FAIL_RECOL_DEFAULT);

    //是否进行话单级校验
    m_bBillLevelVerify = INIFile.GetInt(szSecName,
                                 CFG_CO_BILL_LEVEL_VERIFY,
                                 CFG_CO_BILL_LEVEL_VERIFY_DEFAULT);

    //话单级校验失败是否重采
    m_bBillLevelFailReCol = INIFile.GetInt(szSecName,
                                 CFG_CO_BILL_LEVEL_FAIL_RECOL,
                                 CFG_CO_BILL_LEVEL_FAIL_RECOL_DEFAULT);

    //获取交换机TP对应话单类型
    INIFile.GetString(szSecName, 
                      CFG_CO_BILL_TYPE, 
                      CFG_CO_BILL_TYPE_DEFAULT, 
                      m_szBillType, 
                      sizeof(m_szBillType));

    //根据根据不同的话单类型指定不同转换函数
    if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "采集对象(%s)检测到无效的话单类型(%s)",
             m_szSwitchName,
             m_szBillType);

        COTRACE("采集对象(%s)检测到无效的话单类型(%s)",
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
        //COTRACE("采集文件名不能为空：输入参数错误.");
        COTRACE("file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    //因为传入的源文件名是全路径，需要把文件名和路径分开
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

    //获得最终话单文件
    strDstName = GetDstFileName(strSrcFileName.c_str());
    strFinalFile += strDstName; 

    //获得错误话单文件
    strErrName = GetErrFileName(strSrcFileName.c_str());
    strErrorFile += strErrName;
  
    //转换时先转换为临时文件
    STRING strTmpFinalFile = strFinalFile + ".tmp";
    STRING strTmpErrorFile = strErrorFile + ".tmp";    

    //转换话单文件
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
               "采集对象(%s)转换文件(%s->%s)成功", 
               m_szSwitchName,
               stReq.szSrcName,
               strFinalFile.c_str());

        WRITELOG_C("采集对象(%s)转换文件(%s->%s)成功", 
                   m_szSwitchName,
                   stReq.szSrcName,
                   strFinalFile.c_str());
        break;

    case COL_CONVERT_PART_SUCC:
        FileMove(strTmpFinalFile.c_str(), strFinalFile.c_str());
        FileMove(strTmpErrorFile.c_str(), strErrorFile.c_str());

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)转换文件(%s->%s)部分成功, 错误话单存储为错误文件(%s)", 
               m_szSwitchName,
               stReq.szSrcName,
               strFinalFile.c_str(),
               strErrorFile.c_str());

        WRITELOG_F("采集对象(%s)转换文件(%s->%s)部分成功, 错误话单存储为错误文件(%s)", 
                   m_szSwitchName,
                   stReq.szSrcName,
                   strFinalFile.c_str(),
                   strErrorFile.c_str());

        //要求采集模块重采话单。
        if (m_bBillLevelFailReCol)
        {
            nRet = COL_RECOL_BILL;
        }
        break;


    case ERR_FAIL:
        FileMove(strTmpErrorFile.c_str(), strErrorFile.c_str());

        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)转换文件(%s)失败, 存储为错误文件(%s)", 
               m_szSwitchName,
               stReq.szSrcName,
               strErrorFile.c_str());

        WRITELOG_F("采集对象(%s)转换文件(%s)失败, 存储为错误文件(%s)", 
                   m_szSwitchName,
                   stReq.szSrcName,
                   strErrorFile.c_str());

        //要求采集模块重采话单。
        if (m_bFileLevelFailReCol)
        {
            nRet = COL_RECOL_BILL;
        }

        break;

    default:
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)转换文件(%s)失败, 错误码(%d)", 
               m_szSwitchName,
               stReq.szSrcName,
               nRet);

        WRITELOG_F("采集对象(%s)转换文件(%s)失败, 错误码(%d)", 
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


    //拷贝第二份文件
/*    if ((m_szSecondFinalPath[0] != '\0')
        && ((nRet == ERR_SUCCESS)||(nRet == COL_CONVERT_PART_SUCC)))
    {
        STRING strSecondFile = m_szSecondFinalPath;
        strSecondFile += strDstName;

        if (!FileCopy(strFinalFile.c_str(), strSecondFile.c_str()))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "采集对象(%s)拷贝最终话单文件(%s->%s)失败", 
                   m_szSwitchName,
                   strFinalFile.c_str(),
                   strSecondFile.c_str());

            WRITELOG_F("采集对象(%s)拷贝最终话单文件(%s->%s)失败", 
                       m_szSwitchName,
                       strFinalFile.c_str(),
                       strSecondFile.c_str());

            //nRet = ERR_FAIL;
        }
        else
        {
            COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                   "采集对象(%s)拷贝最终话单文件(%s->%s)成功", 
                   m_szSwitchName,
                   strFinalFile.c_str(),
                   strSecondFile.c_str());

            WRITELOG_C("采集对象(%s)拷贝最终话单文件(%s->%s)成功", 
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

    //创建最终话单目录
    SNPRINTF(m_szFinalPath, 
         sizeof(m_szFinalPath),
         "%s%s/",
         m_szFinalRootPath,
         szTmp);

    CreateDeepDir(m_szFinalPath);

    //创建错误话单目录
    //CreateDeepDir(m_szErrorFilePath);

    //创建最终话单第二份的目录
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
