/**************************************************************************
 *��Ʒ��    : iGateway Bill V200
 *ģ����    : �ɼ�ת��ģ��(collect_convertor)
 *�ļ���    : colloect_convertor.cpp
 *������Ϣ  : ���ļ�����CCollectConvertorModule��ʵ��
 *�汾˵��  : V200R001i00M00B00D1130A
 *��Ȩ��Ϣ  : ��Ȩ���У�C��2001-2002 ��Ϊ�������޹�˾
 *����      : ����
 *����ʱ��  : 2002-10-26
 *�޸ļ�¼  :
 **************************************************************************/

#include "collect_convertor.h"
#include "../collector/resource.h"


char CCollectConvertorModule::g_szColAppName[MAX_PATH] = "";
/**************************************************************************
����ԭ��    public: CCollectConvertorModule (void);
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    public: ~CCollectConvertorModule ();
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
***************************************************************************/
CCollectConvertorModule::~CCollectConvertorModule()
{
    //
}

/**************************************************************************
����ԭ��    protected: void ExitInstance (void);
��������    �˳�����
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
����ԭ��    protected: int InitInstance (void);
��������    ��ʼ��
�������    ��
�������    ��
����ֵ      ��ʼ���ɹ�ʱ����ERR_SUCCESS
�׳��쳣    ��
***************************************************************************/
int CCollectConvertorModule::InitInstance (void)
{
    int nRet = ERR_FAIL;
    nRet = CWorkModule::InitInstance();
    if( nRet != ERR_SUCCESS )
    {
        return nRet;
    }

    // �������ļ�
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

    // �ر������ļ�
    IniFile.Close();

    m_pConvertObjList = CCollectConvertObjFactory::CreateCollectConvertObj(szCfgPath);

    if(NULL == m_pConvertObjList)
    {
        TRACE(THISMODULE, S_FAILED_TO_CREATE_CO_TASK_OBJ);
        return ERR_FAIL;
    }

    //���״̬�ļ���·��
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

    //���״̬�ļ���
    SNPRINTF(szPathName,
             sizeof(szPathName),
             "%s/cvt_module_status.dat",
             szPathName);

    strncpy(m_szStatusFile, szPathName, sizeof(m_szStatusFile));
    m_szStatusFile[sizeof(m_szStatusFile) - 1] = '\0';

    return LoadStatus();
}

/**************************************************************************
����ԭ��    protected: MODULE_TYPE GetModuleType ( void );
��������    ��ñ�ģ������
�������    ��
�������    ��
����ֵ      ���ر�ģ������
�׳��쳣    ��
***************************************************************************/
MODULE_TYPE CCollectConvertorModule::GetModuleType ( void )
{
    return MT_COLCONVERTOR;
}


/**************************************************************************
����ԭ��    protected: void ParseMsg ( MSG_INTRA* pMsgParsed )
��������    ���������ַ�
�������    ��Ϣָ��
�������    ��
����ֵ      ��
�׳��쳣    ��
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
        //��Ϊ�����̿ռ�ָ���
        {
            m_nStatus = CM_ACTIVE;
        }
        break;

    case CMD_RESUME:
        //��Ϊ�����̿ռ�ָ���
        {
            MSGOUT(MTS_COLLECTOR,
                   MSGOUT_LEVEL_IMPORTANT,
                   "�ɼ�����(%s)ת��ģ�鿪ʼ���л����ָ�",
                   g_szColAppName);

            if (m_nConvertStatus == CS_NO_CONVERT)
            {
                m_nConvertResult = ProcessBillConvert();
                m_nConvertStatus = CS_ALREADY_CONVERT;
                SaveStatus();
            }

            MSGOUT(MTS_COLLECTOR,
                   MSGOUT_LEVEL_IMPORTANT,
                   "�ɼ�����(%s)ת��ģ�黰���ָ���ɣ����ģ��",
                   g_szColAppName);

            SendConvertResult(CMD_RESUME_OVER);

            m_nStatus = CM_ACTIVE;
        }
        break;

    case CMD_CONVERT_BILLFILE:
        //��Ϊ��ת�������ļ���
        {
            if (m_nStatus == CM_ACTIVE)
            {
                if (pMsgParsed->Len < INFO_CONVERT_FILE_LENGTH)
                {
                    break;
                }


                //����ת��ǰ״̬
                SCollectConvertFileInfo *pInfo =
                    (SCollectConvertFileInfo *)pMsgParsed->Data;

                m_nFileNum = pInfo->nFileNum;
                memcpy(&m_FileInfo, &pInfo->info, sizeof(m_FileInfo));
                m_nConvertStatus = CS_NO_CONVERT;
                m_nConvertResult = 0;
                SaveStatus();

                //ת�������ļ�
                m_nConvertResult = ProcessBillConvert();

                //����ת����״̬
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
����ԭ��        protected: void  OnTimer(const long nTimerID);
��������        ��ʱ������
�������        ʱ��ID ��ֻ����
�������        �ޡ�
����ֵ          �ޡ�
�׳��쳣        �ޡ�
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
               "�ɼ�����(%s)ת��ģ��ProcessBillConvert�����յ�SwitchIndex(%d)����SwitchCount(%d)",
               g_szColAppName,
               nIndex,
               m_pConvertObjList->size());

        TRACE(THISMODULE,
              "�ɼ�����(%s)ת��ģ��ProcessBillConvert�����յ�SwitchIndex(%d)����SwitchCount(%d)",
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
                   "�ɼ�����(%s)ת��ģ��ProcessBillConvert�����յ�SwitchIndex(%d)����SwitchCount(%d)",
                   g_szColAppName,
                   nIndex,
                   m_pConvertObjList->size());

        TRACE(THISMODULE,
              "�ɼ�����(%s)ת��ģ��ProcessBillConvert�����Ҳ���SwitchIndex(%d)��ת������",
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
        //�������ļ�
        m_pStatusFile = fopen(m_szStatusFile, "wb+");

        if (NULL == m_pStatusFile)
        {

            TRACE(THISMODULE,
                  "ת��ģ���״̬�ļ�(%s)ʧ��",
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

    //����ת��״̬
    memcpy(&m_nConvertStatus, p, sizeof(int));
    p += sizeof(int);

    //����ת�����
    memcpy(&m_nConvertResult, p, sizeof(int));
    p += sizeof(int);

    //�����ļ����к�
    memcpy(&m_nFileNum, p, sizeof(int));
    p += sizeof(int);


    //����ת����Ϣ
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
              "ת��ģ���/����״̬�ļ�(%s)ʧ��",
              m_szStatusFile);


        return ERR_FAIL;
    }

    int nRet = 0;
    int nFileLength = 3 * sizeof(int)
                + sizeof(SSwitchFileInfo);

    BYTE *pWriteBuffer = new BYTE[nFileLength];
    BYTE *p = pWriteBuffer;

    //����ת��״̬
    memcpy(p, &m_nConvertStatus, sizeof(int));
    p += sizeof(int);

    //����ת�����
    memcpy(p, &m_nConvertResult, sizeof(int));
    p += sizeof(int);


    //�����ļ����к�
    memcpy(p, &m_nFileNum, sizeof(int));
    p += sizeof(int);

    //�����ļ���Ϣ
    memcpy(p, &m_FileInfo, sizeof(m_FileInfo));


    //д״̬�ļ�
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
