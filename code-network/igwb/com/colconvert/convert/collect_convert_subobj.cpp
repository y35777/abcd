#include "collect_convert_subobj.h"
#include "collect_convert_base.h"
#include "comm_convert_fun.h"
#include "../../include/collect_msg_def.h"
#include "../../include/format_out.h"
#include "./com/tellin_convert_fun.h"


//以下是采集操作的基类CCollectConvertBase的定义
//构造函数
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

//析构函数
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
        在相关交换机字段获取信息
    */
    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName, sizeof(szSecName), "%s%d", //"switch*"
             CFG_CO_SEC_SWITCH, m_nSwitchIndex);
    szSecName[CFG_BUF_SIZE - 1] = '\0';


    //获取采集对象ID
    INIFile.GetString(szSecName, CFG_CO_TYPE,
                      CFG_CO_TYPE_DEFAULT,
                      m_szCollectId,
                      sizeof(m_szCollectId));
    m_szCollectId[sizeof(m_szCollectId) - 1] = '\0';

    //取最终话单目录
    INIFile.GetString(szSecName,
                      CFG_CO_FINAL_DIR,
                      CCollectorAdapter::GetBackSavePath(),
                      m_szFinalRootPath,
                      sizeof(m_szFinalRootPath));


    //获取最终话单第二份的目录
    INIFile.GetString(szSecName,
                      CFG_CO_SECOND_FINAL_DIR,
                      CCollectorAdapter::GetSecondBackSavePath(),
                      m_szSecondFinalPath,
                      sizeof(m_szSecondFinalPath));

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

    //是否生成0字节的最终话单文件
    m_bCrtZeroFnlFile = INIFile.GetInt(szSecName,
                                 CFG_CO_CREATE_ZERO_FINAL_FILE,
                                 CFG_CO_CREATE_ZERO_FINAL_FILE_DEFAULT);


    //原始话单的长度
    m_nOrigBillLen = INIFile.GetInt(szSecName,
                                    CFG_CO_ORIG_BILL_LEN,
                                    CFG_CO_ORIG_BILL_LEN__DEFAULT);


    /*
        在相关SubObj字段获取信息
    */
    SNPRINTF(szSecName, sizeof(szSecName),
             "%s%d_%s",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex,
             m_szSubObjName);
    szSecName[CFG_BUF_SIZE - 1] = '\0';


    //取最终话单目录
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
        COTRACE("采集对象(%s,%s)最终话单目录没有配置",
            m_szSwitchName,
            m_szSubObjName);

        return ERR_FAIL;
    }

    if (!CreateDeepDir(m_szFinalRootPath))
    {
        COTRACE("采集对象(%s,%s)最终话单目录(%s)无法创建成功",
            m_szSwitchName,
            m_szSubObjName,
            m_szFinalRootPath);

        return ERR_FAIL;
    }

    //是否需要复制第二份最终话单文件
    m_bSecondCopy = CCollectorAdapter::GetbAPSaveSecond();

    //是否需要第二份话单适配    
    m_bNeedSecondAdapter = CCollectorAdapter::GetbNeedSencondAdapter();

    //获取最终话单第二份的目录
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

    //话单文件保存天数
    m_nFileSaveDays = INIFile.GetInt(szSecName,
                                 CFG_CO_FINAL_FILE_SAVE_DAYS,
                                 m_nFileSaveDays);

    //是否进行文件级校验
    m_bFileLevelVerify = INIFile.GetInt(szSecName,
                                 CFG_CO_FILE_LEVEL_VERIFY,
                                 m_bFileLevelVerify);

    //文件级校验失败是否重采
    m_bFileLevelFailReCol = INIFile.GetInt(szSecName,
                                 CFG_CO_FILE_LEVEL_FAIL_RECOL,
                                 m_bFileLevelFailReCol);

    //是否进行话单级校验
    m_bBillLevelVerify = INIFile.GetInt(szSecName,
                                 CFG_CO_BILL_LEVEL_VERIFY,
                                 m_bBillLevelVerify);

    //话单级校验失败是否重采
    m_bBillLevelFailReCol = INIFile.GetInt(szSecName,
                                 CFG_CO_BILL_LEVEL_FAIL_RECOL,
                                 m_bBillLevelFailReCol);

    //是否生成0字节的最终话单文件
    m_bCrtZeroFnlFile = INIFile.GetInt(szSecName,
                                 CFG_CO_CREATE_ZERO_FINAL_FILE,
                                 m_bCrtZeroFnlFile);
    //原始话单的长度
    m_nOrigBillLen = INIFile.GetInt(szSecName,
                                    CFG_CO_ORIG_BILL_LEN,
                                    m_nOrigBillLen);


    //获取交换机TP对应话单类型
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
        COTRACE("采集文件名不能为空：输入参数错误.");
        //COTRACE("file name is not specified: failed to get file");
        return ERR_FAIL;
    }

    //构建目标文件的路径
    ConstuctDestFileLst(stReq);


    //转换话单文件
    int nRet =   ConvertFile(stReq);

/*
    if (ERR_SUCCESS != nRet)
    {
        TRACE("话单文件转换失败.");
        return ERR_FAIL;
    }
*/
    //生成0字节的最终话单文件
    //对每个没有话单的通道补充生成0字节话单
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
                           "创建0字节最终话单文件(%s)失败",            
                           m_FinalFileList[i].c_str());
                        
                    WRITELOG_F("创建0字节最终话单文件(%s)失败",    
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

    //把临时目录下的文件移动到正式目录下
    RenameDestFile();


    //如果文件中有话单转换成功，返回值为ERR_SUCCESS
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
                       "采集对象(%s)转换文件(%s->%s)成功",
                       m_szSwitchName,
                       stReq.szSrcName,
                       m_FinalFileList[i].c_str());

                WRITELOG_C("采集对象(%s)转换文件(%s->%s)成功",
                           m_szSwitchName,
                           stReq.szSrcName,
                           m_FinalFileList[i].c_str());
            }
        }

        if ((m_CvtResultList[0] > 0) && (m_bBillLevelVerify))
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "采集对象(%s)在文件(%s)中发现错误话单，并存储为错误文件(%s)",
                   m_szSwitchName,
                   stReq.szSrcName,
                   m_FinalFileList[0].c_str());

            WRITELOG_F("采集对象(%s)在文件(%s)中发现错误话单，并存储为错误文件(%s)",
                       m_szSwitchName,
                       stReq.szSrcName,
                       m_FinalFileList[0].c_str());

            nRet = ERR_FAIL;

            //modifyed by chenliangwei, 2003-08-05
            //要求采集模块重采话单。
            if (m_bBillLevelFailReCol)
            {
                nRet = COL_RECOL_BILL;
            }
            //modifyed end
        }
    }
    //如果返回为ERR_FAIL，并且有错误文件生成，表示是文件级校验失败
    else if (m_CvtResultList[0] > 0)
    {
        if (m_bFileLevelVerify)
        {
            COMSGOUT(MSGOUT_LEVEL_URGENT,
                   "采集对象(%s)转换文件(%s)失败, 存储为错误文件(%s)",
                   m_szSwitchName,
                   stReq.szSrcName,
                   m_FinalFileList[0].c_str());

            WRITELOG_F("采集对象(%s)转换文件(%s)失败, 存储为错误文件(%s)",
                       m_szSwitchName,
                       stReq.szSrcName,
                       m_FinalFileList[0].c_str());

            //modifyed by chenliangwei, 2003-08-05
            //要求采集模块重采话单。
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
    //其它异常错误，并且没有错误文件生成
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
               "采集对象(%s)转换文件(%s)失败",
               m_szSwitchName,
               stReq.szSrcName);

        WRITELOG_F("采集对象(%s)转换文件(%s)失败",
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


    //拷贝第二份文件
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

    //创建最终话单目录
    m_FinalDirList.clear();
    for (; i != m_ChlNames.end(); i++)
    {
        //临时目录
        SNPRINTF(szFinalPath,
             sizeof(szFinalPath),
             "%s/%s/temp",
             m_szFinalRootPath,
             (*i).c_str());
        szFinalPath[sizeof(szFinalPath) - 1] = '\0';

        CreateDeepDir(szFinalPath);

        //正式目录
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


    //创建最终话单第二份的目录
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

    //删除最终话单目录
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
    //因为传入的源文件名是全路径，需要把文件名和路径分开
    STRING strSrcFileName = "";
    STRING strSrcPath = "";

    m_FinalFileList.clear();
    m_CvtResultList.clear();

    divide_file_path_name(stReq.szSrcName,
                          strSrcPath,
                          strSrcFileName);

    //获得最终话单文件
    STRING strDstName = "";
    strDstName = GetDstFileName(strSrcFileName.c_str());

    //转换
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

//把临时目录下的文件移动到正式目录下
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

//拷贝第二份文件
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
                    "采集对象(%s)拷贝最终话单文件(%s->%s)失败",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                
                WRITELOG_F("采集对象(%s)拷贝最终话单文件(%s->%s)失败",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                nRet = ERR_FAIL;
                break;
            }
            else
            {
                COMSGOUT(MSGOUT_LEVEL_IMPORTANT,
                    "采集对象(%s)拷贝最终话单文件(%s->%s)成功",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
                
                WRITELOG_C("采集对象(%s)拷贝最终话单文件(%s->%s)成功",
                    m_szSwitchName,
                    m_FinalFileList[i].c_str(),
                    strSecondFile.c_str());
            }
        }
    }
    
    return nRet;
}

//转换话单文件
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