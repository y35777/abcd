/**************************************************************************
 *产品名    : iGateway Bill V200
 *模块名    : 采集转换模块(collect_convertor)
 *文件名    : colloect_convertor.cpp
 *描述信息  : 本文件是类CCollectConvertorModule的实现
 *版本说明  : V200R001i00M00B00D1130A
 *版权信息  : 版权所有（C）2001-2002 华为技术有限公司
 *作者      : 王锋
 *创建时间  : 2002-10-26
 *修改记录  :
 **************************************************************************/

#include "collect_convertor.h"
#include "../collector/resource.h"


char CCollectConvertorModule::g_szColAppName[MAX_PATH] = "";
/**************************************************************************
函数原型    public: CCollectConvertorModule (void);
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CCollectConvertorModule::CCollectConvertorModule(void)
{
    m_nStatus         = CM_DEACTIVE;
    m_nConvertStatus  = CS_ALREADY_CONVERT;
    m_nFileNum        = 0;
    m_nConvertResult  = 1;

    m_szStatusFile[0] = '\0';
    m_pStatusFile     = NULL;
    m_pConvertObjList = NULL;
}

/**************************************************************************
函数原型    public: ~CCollectConvertorModule ();
功能描述    构造函数
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
CCollectConvertorModule::~CCollectConvertorModule()
{
    //
}

/**************************************************************************
函数原型    protected: void ExitInstance (void);
功能描述    退出清理
输入参数    无
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CCollectConvertorModule::ExitInstance (void)
{
    if(NULL != m_pConvertObjList)
    {
        CCollectConvertObj* pObj = NULL;
        while(m_pConvertObjList->size() > 0)
        {
            pObj = m_pConvertObjList->front();
            delete pObj;
            m_pConvertObjList->erase(m_pConvertObjList->begin());
        }

        delete m_pConvertObjList;
        m_pConvertObjList = NULL;
    }

    CWorkModule::ExitInstance();
}
/**************************************************************************
函数原型    protected: int InitInstance (void);
功能描述    初始化
输入参数    无
输出参数    无
返回值      初始化成功时返回ERR_SUCCESS
抛出异常    无
***************************************************************************/
int CCollectConvertorModule::InitInstance (void)
{
    int nRet = ERR_FAIL;
    nRet = CWorkModule::InitInstance();
    if( nRet != ERR_SUCCESS )
    {
        return nRet;
    }

    // 打开配置文件
    CINIFile IniFile(GetCfgFilePath());
    nRet = IniFile.Open();

    char strSec[20];
    sprintf(strSec, "%s%d", CFG_SEC_AP, GetProcessType() - PT_CO_BASE + 1);
    int nCollectType = IniFile.GetInt(strSec,
                                      CFG_AP_KEY_COLTYPE,
                                      CFG_AP_KEY_COLTYPE_DEFAULT);

    if(nCollectType == 0)
    {
        TRACE(THISMODULE, S_AP_COTYPE_DISMATCHED, GetProcessType() - PT_CO_BASE + 1);
        return ERR_FAIL;
    }

    IniFile.GetString(strSec, CFG_AP_NAME, strSec,
                      g_szColAppName, sizeof(g_szColAppName));

    char szCfgPath[MAX_PATH];
    IniFile.GetString(strSec, CFG_COT_CFG_PATH, "", szCfgPath, MAX_PATH);
    szCfgPath[MAX_PATH - 1] = '\0';

    // 关闭配置文件
    IniFile.Close();

    m_pConvertObjList = CCollectConvertObjFactory::CreateCollectConvertObj(szCfgPath);

    if(NULL == m_pConvertObjList)
    {
        TRACE(THISMODULE, S_FAILED_TO_CREATE_CO_TASK_OBJ);
        return ERR_FAIL;
    }

    //获得状态文件的路径
    char szPathName[MAX_PATH];
#ifdef _PLATFORM_WIN32
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "d:/other/collect/%s",
             g_szColAppName);
#else
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "/var/other/collect/%s",
             g_szColAppName);
#endif
    CreateDeepDir(szPathName);

    //获得状态文件名
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "%s/cvt_module_status.dat",
             szPathName);

    strncpy(m_szStatusFile, szPathName, sizeof(m_szStatusFile));
    m_szStatusFile[sizeof(m_szStatusFile) - 1] = '\0';

    return LoadStatus();
}

/**************************************************************************
函数原型    protected: MODULE_TYPE GetModuleType ( void );
功能描述    获得本模块类型
输入参数    无
输出参数    无
返回值      返回本模块类型
抛出异常    无
***************************************************************************/
MODULE_TYPE CCollectConvertorModule::GetModuleType ( void )
{
    return MT_COLCONVERTOR;
}


/**************************************************************************
函数原型    protected: void ParseMsg ( MSG_INTRA* pMsgParsed )
功能描述    命令解析与分发
输入参数    消息指针
输出参数    无
返回值      无
抛出异常    无
***************************************************************************/
void CCollectConvertorModule::ParseMsg ( MSG_INTRA* pMsgParsed )
{
    switch(pMsgParsed->CmdCode)
    {
    case CMD_STOP_NETWORK:
        {
            m_nStatus = CM_DEACTIVE;
        }
        break;

    case CMD_RESUME_NETWORK:
        //若为“磁盘空间恢复”
        {
            m_nStatus = CM_ACTIVE;
        }
        break;

    case CMD_RESUME:
        //若为“磁盘空间恢复”
        {
            MSGOUT(MTS_COLLECTOR,
                   MSGOUT_LEVEL_IMPORTANT,
                   "采集进程(%s)转换模块开始进行话单恢复",
                   g_szColAppName);

            if (m_nConvertStatus == CS_NO_CONVERT)
            {
                m_nConvertResult = ProcessBillConvert();
                m_nConvertStatus = CS_ALREADY_CONVERT;
                SaveStatus();
            }

            MSGOUT(MTS_COLLECTOR,
                   MSGOUT_LEVEL_IMPORTANT,
                   "采集进程(%s)转换模块话单恢复完成，激活本模块",
                   g_szColAppName);

            SendConvertResult(CMD_RESUME_OVER);

            m_nStatus = CM_ACTIVE;
        }
        break;

    case CMD_CONVERT_BILLFILE:
        //若为“转换话单文件”
        {
            if (m_nStatus == CM_ACTIVE)
            {
                if (pMsgParsed->Len < INFO_CONVERT_FILE_LENGTH)
                {
                    break;
                }


                //保存转换前状态
                SCollectConvertFileInfo *pInfo =
                    (SCollectConvertFileInfo *)pMsgParsed->Data;

                m_nFileNum = pInfo->nFileNum;
                memcpy(&m_FileInfo, &pInfo->info, sizeof(m_FileInfo));
                m_nConvertStatus = CS_NO_CONVERT;
                m_nConvertResult = 0;
                SaveStatus();

                //转换话单文件
                m_nConvertResult = ProcessBillConvert();

                //保存转换后状态
                m_nConvertStatus = CS_ALREADY_CONVERT;
                SaveStatus();


                SendConvertResult(CMD_ANS_CONVERT_BILLFILE);

            }
        }
        break;

    default:
        CWorkModule::ParseMsg(pMsgParsed);
        break;
    }
}

/*********************************************************************************************
函数原型        protected: void  OnTimer(const long nTimerID);
功能描述        定时器处理
输入参数        时钟ID ，只读。
输出参数        无。
返回值          无。
抛出异常        无。
**********************************************************************************************/
void CCollectConvertorModule::OnTimer(const long nTimerID)
{
    CWorkModule::OnTimer(nTimerID);
}

int CCollectConvertorModule::ProcessBillConvert()
{
    int nIndex = m_FileInfo.nSwitchIndex;
    if (nIndex > m_pConvertObjList->size())
    {
        MSGOUT(MTS_COLLECTOR,
               MSGOUT_LEVEL_URGENT,
               "采集进程(%s)转换模块ProcessBillConvert函数收到SwitchIndex(%d)大于SwitchCount(%d)",
               g_szColAppName,
               nIndex,
               m_pConvertObjList->size());

        TRACE(THISMODULE,
              "采集进程(%s)转换模块ProcessBillConvert函数收到SwitchIndex(%d)大于SwitchCount(%d)",
              g_szColAppName,
              nIndex,
              m_pConvertObjList->size());

        return ERR_FAIL;
    }

    CCollectConvertObj* pObj = (*m_pConvertObjList)[nIndex - 1];

    int nRet = ERR_FAIL;
    if (pObj != NULL)
    {
        nRet = pObj->Process(m_FileInfo.szLocalFile,
                             m_FileInfo.nConvertInfo);
    }
    else
    {
        MessageOut(THISMODULE,
                   MSGOUT_LEVEL_URGENT,
                   "采集进程(%s)转换模块ProcessBillConvert函数收到SwitchIndex(%d)大于SwitchCount(%d)",
                   g_szColAppName,
                   nIndex,
                   m_pConvertObjList->size());

        TRACE(THISMODULE,
              "采集进程(%s)转换模块ProcessBillConvert函数找不到SwitchIndex(%d)的转换对象",
              g_szColAppName,
              nIndex);
    }

    return nRet;

}

void CCollectConvertorModule::SendConvertResult(int nCmdCode)
{
    SCollectConvertResult result;
    result.nFileNum = m_nFileNum;
    result.nConvertResult = m_nConvertResult;

    MSG_INTRA *pMsg = new(sizeof(SCollectConvertResult)) MSG_INTRA;
    pMsg->SenderPid = PT_SELF;
    pMsg->SenderMid = MT_COLCONVERTOR;
    pMsg->ReceiverPid = PT_SELF;
    pMsg->ReceiverMid = MT_COLLECTOR;
    pMsg->CmdCode = nCmdCode;
    memcpy(pMsg->Data, &result, sizeof(SCollectConvertResult));

    SendMsg(pMsg);

}

int CCollectConvertorModule::LoadStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "rb+");
    if(NULL == m_pStatusFile)
    {
        //创建新文件
        m_pStatusFile = fopen(m_szStatusFile, "wb+");

        if (NULL == m_pStatusFile)
        {

            TRACE(THISMODULE,
                  "转换模块打开状态文件(%s)失败",
                  m_szStatusFile);

            return ERR_FAIL;
        }
        else
        {
            m_nConvertStatus = CS_ALREADY_CONVERT;
            m_nFileNum = 0;
            m_nConvertResult = 1;

            fclose(m_pStatusFile);
            m_pStatusFile = NULL;

            return ERR_SUCCESS;
        }
    }

    int nFileLength = FileSize(m_pStatusFile);
    if(nFileLength < 3 * sizeof(int) + sizeof(SSwitchFileInfo))
    {
        m_nConvertStatus = CS_ALREADY_CONVERT;
        m_nFileNum = 0;
        m_nConvertResult = 1;

        fclose(m_pStatusFile);
        m_pStatusFile = NULL;

        return ERR_SUCCESS;
    }

    int nReadLen = 3 * sizeof(int) + sizeof(SSwitchFileInfo);
    BYTE *pReadBuffer = new BYTE[nReadLen];
    int nRet = fread(pReadBuffer,
                     nReadLen,
                     1,
                     m_pStatusFile);

    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
        delete []pReadBuffer;

        return ERR_FAIL;
    }

    BYTE* p = pReadBuffer;

    //读入转换状态
    memcpy(&m_nConvertStatus, p, sizeof(int));
    p += sizeof(int);

    //读入转换结果
    memcpy(&m_nConvertResult, p, sizeof(int));
    p += sizeof(int);

    //读入文件序列号
    memcpy(&m_nFileNum, p, sizeof(int));
    p += sizeof(int);


    //读入转换信息
    memcpy(&m_FileInfo, p, sizeof(m_FileInfo));

    fclose(m_pStatusFile);
    m_pStatusFile = NULL;
    delete []pReadBuffer;

    return ERR_SUCCESS;

}

int CCollectConvertorModule::SaveStatus()
{
    m_pStatusFile = fopen(m_szStatusFile, "wb");
    if(NULL == m_pStatusFile)
    {

        TRACE(THISMODULE,
              "转换模块打开/创建状态文件(%s)失败",
              m_szStatusFile);


        return ERR_FAIL;
    }

    int nRet = 0;
    int nFileLength = 3 * sizeof(int)
                + sizeof(SSwitchFileInfo);

    BYTE *pWriteBuffer = new BYTE[nFileLength];
    BYTE *p = pWriteBuffer;

    //拷贝转换状态
    memcpy(p, &m_nConvertStatus, sizeof(int));
    p += sizeof(int);

    //拷贝转换结果
    memcpy(p, &m_nConvertResult, sizeof(int));
    p += sizeof(int);


    //拷贝文件序列号
    memcpy(p, &m_nFileNum, sizeof(int));
    p += sizeof(int);

    //拷贝文件信息
    memcpy(p, &m_FileInfo, sizeof(m_FileInfo));


    //写状态文件
    nRet = fwrite(pWriteBuffer,
                  nFileLength,
                  1,
                  m_pStatusFile);

    if(1 != nRet)
    {
        fclose(m_pStatusFile);
        m_pStatusFile = NULL;
        delete []pWriteBuffer;

        return ERR_FAIL;
    }



    nRet = fflush(m_pStatusFile);
    fclose(m_pStatusFile);
    m_pStatusFile = NULL;
    delete []pWriteBuffer;


    ACE_OS::truncate(m_szStatusFile, nFileLength);

    return nRet;

}
