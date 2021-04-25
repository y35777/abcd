#include "../include/toolbox.h"
#include "../include/frame.h"
#include "cdr_stat_tsk.h"
#include "stat_rst_mgr.h"
#include "cdr_stat_svr.h"
#include "stat_file_group.h"
#include "cdr_stat_rst.h"

#ifdef _PLATFORM_WIN32
#pragma warning(disable:4503)
#endif

/*******************************************************************
�������ƣ�CCDRStatTask()
�������������캯��
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
CCDRStatTask::CCDRStatTask()
{
    m_pCDRStatRstMgr    = NULL;
    m_bExit             = FALSE;
    m_pIGWB             = NULL;
    m_pIQueryBrowse     = NULL;
    m_szOrigBasePath[0] = '\0';
    m_szFinalBasePath[0]= '\0';
    m_szStatPath[0]     = '\0';
}
/*******************************************************************
�������ƣ�~CCDRStatTask()
������������������
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
CCDRStatTask::~CCDRStatTask()
{
    if (m_pCDRStatRstMgr != NULL)
    {
        delete m_pCDRStatRstMgr;
        m_pCDRStatRstMgr = NULL;
    }

    if (m_pIGWB != NULL)
    {
        delete m_pIGWB;
        m_pIGWB = NULL;
    }
    m_pIQueryBrowse = NULL;

    while(m_TaskInfoList.size() > 0)
    {
        delete (*m_TaskInfoList.begin());
        m_TaskInfoList.pop_front();
    }
}

/*******************************************************************
�������ƣ�Init()
�������������г�ʼ������
���������UINT4 nApCount
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatTask::Init(UINT4 nApCount)
{
    //��ʼ����ʽ����ض���
    char szIGWBPath[MAX_PATH];
    sprintf(szIGWBPath, "%s/config/format", GetAppPath());
    m_pIGWB = new CIGWB(szIGWBPath);
    m_pIQueryBrowse = (IQueryBrowse*)m_pIGWB->QueryInterface(IID_FP_IQUERYBROWSE);
    if (NULL == m_pIQueryBrowse)
    {
        return ERR_FAIL;
    }
    int nResult = m_pIQueryBrowse->Init(0);
    if (ERR_SUCCESS != nResult)
    {
        return ERR_FAIL;
    }

    //��ʼ��ͳ�ƽ���������
    m_pCDRStatRstMgr = new CCDRStatRstMgr;
    nResult = m_pCDRStatRstMgr->Init(nApCount);
    if (ERR_SUCCESS != nResult)
    {
        return ERR_FAIL;
    }

    CINIFile IniFile(GetCfgFilePath());
    IniFile.Open();

    IniFile.GetString(CFG_SEC_DISK_FILE,
                      CFG_FRONT_SAVE_ROOT_DIR,
                      CFG_FRONT_SAVE_ROOT_DIR_DEFAULT,
                      m_szOrigBasePath,
                      sizeof(m_szOrigBasePath));
    m_szOrigBasePath[sizeof(m_szOrigBasePath) - 1] = '\0';

    IniFile.GetString(CFG_SEC_DISK_FILE,
                      CFG_BACK_SAVE_ROOT_DIR,
                      CFG_BACK_SAVE_ROOT_DIR_DEFAULT,
                      m_szFinalBasePath,
                      sizeof(m_szFinalBasePath));
    m_szFinalBasePath[sizeof(m_szFinalBasePath) - 1] = '\0';

    //��ȡ���н���������
    char szAPSec[25];
    char szAPName[255];
    UINT4 uBillLen = 0;

    for (UINT4 i = 1; i <=  nApCount; i++)
    {
        //���������
        SNPRINTF(szAPSec,
                 sizeof(szAPSec),
                 "%s%d",
                 CFG_SEC_AP,
                 i);
        szAPSec[sizeof(szAPSec) - 1] = '\0';

        IniFile.GetString(szAPSec,
                          CFG_AP_NAME,
                          szAPSec,
                          szAPName,
                          sizeof(szAPName));
        szAPName[sizeof(szAPName) - 1] = '\0';

        //����������
        UINT4 uAPType = IniFile.GetInt(szAPSec,
                                       CFG_AP_KEY_APTYPE,
                                       CFG_AP_KEY_APTYPE_DEFAULT);
        
        //added by lucy 2004-06-09 SWPD05649
        //��CollectType����0,��ʾ�ǲɼ������
        int nCollectType = IniFile.GetInt(szAPSec, 
                                          CFG_AP_KEY_COLTYPE,
                                          CFG_AP_KEY_COLTYPE_DEFAULT);
        if (nCollectType)
        {
            uAPType = AP_OTHER + nCollectType;
        }
        //end SWPD05649
        
        //�Ƿ�ϲ�
        BOOL bMustMerge = IniFile.GetInt(szAPSec,
                                         CFG_AP_KEY_MUST_MERGE,
                                         CFG_AP_KEY_MUST_MERGE_DEFAULT);
		
		UINT4 uBillType = IniFile.GetInt(CFG_SEC_CDRSTAT,
										 CFG_AP_CDRSTAT_BILLTYPE,
										 CFG_AP_CDRSTAT_BILLTYPE_DEFAULT);

        //ͳ�ƵĻ�������
        uBillType = IniFile.GetInt(szAPSec,
                                   CFG_AP_CDRSTAT_BILLTYPE,
                                   uBillType);
        //�Ƿ��������ԭʼ��ʽ
        BOOL bFinalSPFileFmt = IniFile.GetInt(szAPSec,
                                              CFG_AP_KEY_FINAL_SPFILEFMT,
                                              CFG_AP_KEY_FINAL_SPFILEFMT_DEFAULT);

        char szRelease[16] = "";

        if (AP_PS == uAPType || AP_PS_RESERVED == uAPType)
        {
            IniFile.GetString(szAPSec,
                              CFG_AP_KEY_RELEASE_VERSION,
                              CFG_AP_KEY_RELEASE_R98,
                              szRelease,
                              sizeof(szRelease));
            szRelease[sizeof(szRelease) - 1] = '\0';
        }

        SCdrStatTaskInfo* pInfo = new SCdrStatTaskInfo;
        pInfo->uAPNumber        = i;
        pInfo->bMustMerge       = bMustMerge;
        pInfo->bFinalSPFileFmt  = bFinalSPFileFmt;
        pInfo->uAPType          = uAPType;
        pInfo->uBillType        = uBillType;
        strncpy(pInfo->szAPName, szAPName, sizeof(pInfo->szAPName));
        pInfo->szAPName[sizeof(pInfo->szAPName) - 1] = '\0';
        strncpy(pInfo->szRelease, szRelease, sizeof(pInfo->szRelease));
        pInfo->szRelease[sizeof(pInfo->szRelease) - 1] = '\0';

        LIST<STRING> ContentList, ParaFieldList;

        //���������ʽ���ÿ����ʽ�Ŀ�ѡ����Ϣ
        FORMAT_LIST  FormatList;

        //һ��������ÿ����ʽ��Ӧ�ı��ʽ�б�
        FORMAT_MAP_INFOLIST  FormatToParaInfo;

        //���������͸�ʽ���б�
        LIST<STRING> FormatNameList, ChlNameList, DecsNameList;

        CFormat* pFmt = NULL;
        if(uBillType == 0)
        {
            //���ݽ�������ƻ������ԭʼ�����ʽ
            m_pIQueryBrowse->GetOrigBrowseNames(szAPName, FormatNameList, DecsNameList);
        }
        else
        {
            //���ݽ�������ƻ���������ջ��������ʽ
            //�������ͨ������
            m_pIQueryBrowse->GetChlNames(szAPName, ChlNameList);
            m_mapAPToChlName[szAPName] = ChlNameList;

            LIST<STRING>::iterator it_chl;
            //����ȡ��ÿ��ͨ���������ʽ
            for(it_chl = ChlNameList.begin(); it_chl != ChlNameList.end(); it_chl++)
            {
                LIST<STRING> ChlFmtNameList;

                m_pIQueryBrowse->GetFinalBrowseNames(szAPName, *it_chl, ChlFmtNameList, DecsNameList);


                if (ChlFmtNameList.size() > 0)
                {
                    pFmt = m_pIQueryBrowse->GetBrowseFmt(ChlFmtNameList.front());
                    int nLen = pFmt->GetSrcFmtLen();
                    m_mapChlNameToLen[*it_chl] = nLen;
                }

                //��ÿ��ͨ�������������ʽ���뵽FormatNameList
                FormatNameList.merge(ChlFmtNameList);
            }
        }

        LIST<STRING>::iterator it_fn;
        for (it_fn = FormatNameList.begin(); it_fn != FormatNameList.end(); it_fn++)
        {
            pFmt = m_pIQueryBrowse->GetBrowseFmt(*it_fn);
            if (NULL == pFmt)
            {
                return ERR_FAIL;
            }

            //��ô˽���㻰���ĳ���
            uBillLen = pFmt->GetSrcFmtLen();

            //�Ѹ�ʽ���뵽�б�
            FormatList.push_back(pFmt);

            int nFieldCount = pFmt->GetFieldNum();
            const SFieldInfo** FieldArray = pFmt->GetFieldsArray();

            //�������е��򣬸����������ڲ�ѯ��������֯����
            SFieldInfo* pFieldInfo = NULL;
            BYTE OperMaskArr[6] = {0x01, 0x04, 0x02, 0x10, 0x08, 0x20};
            char AllOperArr[][6] = {"==", ">", "<", ">=", "<=", "!="};
            char szFieldType[30], szOperator[10], szEnumFlg[5], szEnumValue[2048];
            int nEnumCount;
            ENUMINFO_LIST* pEnumList;
            ENUMINFO_LIST::iterator iEnumIdx;
            for (int nFieldIdx = 0; nFieldIdx < nFieldCount; nFieldIdx++)
            {
                pFieldInfo = (SFieldInfo* )FieldArray[nFieldIdx];

                //��ParaFieldList�в���szFieldName,������ں��Ե�������Ψһ
                if (FindField(pFieldInfo->szFieldName, ParaFieldList))
                {
                    continue;
                }

                //�������ڲ�ѯ����ֱ���Թ�
                if (0 == pFieldInfo->flgQuery)
                {
                    continue;
                }

                sprintf(szFieldType, "%d", pFieldInfo->nQueryCondDataType);
                //�õ������ö�ٱ�־�Լ����е�ö��ֵ
                if (0 == pFieldInfo->flgEnum)   //������ö��ֵ
                {
                    sprintf(szEnumFlg, "%d", 0);
                    szEnumValue[0] = '\0';
                }
                else
                {
                    //��ʱszEnumFlg������ʾö��ֵ�ĸ�����ö��ֵ����^�ŷֿ�
                    szEnumValue[0] = '\0';  //��֤szEnumValueΪ���ַ���
                    pEnumList = pFieldInfo->pEnumInfos;
                    nEnumCount = pEnumList->size();
                    iEnumIdx = pEnumList->begin();
                    STRNCPY(szEnumValue, (*iEnumIdx).szEnumValue, 2048);
                    iEnumIdx++;
                    for (; iEnumIdx != pEnumList->end(); iEnumIdx++)
                    {
                        STRNCAT(szEnumValue, "^", sizeof(szEnumValue));
                        STRNCAT(szEnumValue, (*iEnumIdx).szEnumValue, sizeof(szEnumValue));
                        szEnumValue[sizeof(szEnumValue) - 1] = '\0';
                    }
                    sprintf(szEnumFlg, "%d", nEnumCount);
                }

                ParaFieldList.push_back(pFieldInfo->szFieldName);

                //��ȡ���ӣ�һ������ܶ�Ӧ�������
                for (int i = 0; i < 6; i++)
                {
                    //�����ӵ�ֵת�����ַ���
                    if (pFieldInfo->nOperator & OperMaskArr[i])
                    {
                        strcpy(szOperator, AllOperArr[i]);

                        ContentList.push_back(pFieldInfo->szFieldName);
                        ContentList.push_back(pFieldInfo->szFieldDescribe);
                        ContentList.push_back(szFieldType);
                        ContentList.push_back(szOperator);
                        ContentList.push_back(pFieldInfo->szUnit);
                        ContentList.push_back(szEnumFlg);
                        ContentList.push_back(szEnumValue);
                    }
                }

            }
            FormatToParaInfo[pFmt] = ParaFieldList;
        }


        //��CCDRStatTak����FormatList
        m_mapAPToOrigFormat[szAPName] = FormatList;

        //��CCDRStatTak����FormatToParaInfo
        m_mapAPToFormatParaInfo[szAPName] = FormatToParaInfo;

        //��CCDRStatSvr����ContentList
        CCDRStatSvr::SetParaInfoContentList(szAPName, ContentList);

        BuildMap_AP_FMT_EXP(szAPName);

        pInfo->uBillLen = uBillLen;
        m_TaskInfoList.push_back(pInfo);

    }

    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�WaitForExit(void)
���������������˳���־�����ȴ��˳�
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatTask::WaitForExit(void)
{
    m_bExit = TRUE;

    wait();

}


/*******************************************************************
�������ƣ�ActivateNextStatOper(void)
����������������һ��ͳ�Ʋ���
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatTask::ActivateNextStatOper(void)
{
    activate();
}

/*******************************************************************
�������ƣ�svc()
�����������̴߳�����
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatTask::svc()
{
    DoStatOpr();

    m_bExit = FALSE;

    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�DoStatOpr(void)
��������������ͳ�Ʋ���
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatTask::DoStatOpr(void)
{
    //��õ����ʱ��
    char szToday[9];
    UINT4 i;
    time_t curtime = ACE_OS::time(NULL);
    ACE_OS::strftime(szToday, 9, "%Y%m%d", ACE_OS::localtime(&curtime));
    szToday[8] = '\0';

    SCdrStatTaskInfo* pInfo = NULL;

    TASKINFO_LIST::iterator it;
    FORMAT_MAP_INFOLIST FormatInfo;
    FORMAT_MAP_INFOLIST::iterator it_formatInfo;

    for(it = m_TaskInfoList.begin(); it != m_TaskInfoList.end(); it++)
    {
        pInfo = *it;
        //�ɽ�������ƻ�ô˽�������еı��ʽ�б�
        ICDRStatRst*  pTmpRst;
        pTmpRst = m_pCDRStatRstMgr->GetStatRstInterface(pInfo->szAPName);
        SStatExpIndexList* pIndexList = NULL;
        pIndexList = pTmpRst->GetStatExpList();

        time_t lasttime = pTmpRst->GetLastStatTime();

        //�ϴ�ͳ������
        char szLastDate[9];
        ACE_OS::strftime(szLastDate, 9, "%Y%m%d", ACE_OS::localtime(&lasttime));
        szLastDate[8] = '\0';

        BYTE* pTmpMem = NULL;
        SAllStatExpRst*  pAllRst = NULL;

        SStatExpIndexMsg* pIndexMsg = NULL;
        int nValidStatExpCount = 0;
        for(i = 0; i < pIndexList->uStatExpCount; i++)
        {
            pIndexMsg = (SStatExpIndexMsg*)(&(pIndexList->expIndex)[i]);
            if(pIndexMsg->bInUsed)
            {
                nValidStatExpCount++;
            }
        }

        if(nValidStatExpCount == 0)
        {
            pTmpRst->SetLastStatTime(curtime);
            delete pIndexList;
            continue;
        }
        pTmpMem = new BYTE[nValidStatExpCount * ONESTATEXPRSTLENGTH + 8];
        pAllRst = (SAllStatExpRst*)pTmpMem;
        pAllRst->uStatSubItemCount = nValidStatExpCount;
        UINT4 j = 0;
        SOneStatExpRst* pOneExpRst = NULL;
        for(i = 0; i < pIndexList->uStatExpCount; i++)
        {
            pOneExpRst = (SOneStatExpRst*)(&(pAllRst->ItemRst)[j]);
            pIndexMsg = (SStatExpIndexMsg*)(&(pIndexList->expIndex)[i]);
            if(pIndexMsg->bInUsed)
            {
                pOneExpRst->nStatExpIndex = pIndexMsg->nStatExpIndex;
                j++;
            }
        }


        //ÿ��ͳ�ƵĶ���ǰһ��Ļ����������뿼�ǵ����ܴ��ڸ���Ļ���û��ͳ�ƣ�
        //1��ÿ�������ͳ�Ƶ�ʱ������Ҫ�����ϴε�ͳ��ʱ��(LastStatTime),�����ʱ��(CurrentTime)
        //2������Ҫͳ�ƵĻ�����Χ(X)��LastStatTime + 1 <= X < CurrentTime
        for(;;)
        {
            //���ʱ���ڵ���֮ǰ�ͼ���ͳ�ƣ�����ʲôҲ����
            if (strcmp(szLastDate, szToday) >= 0)
            {
                break;
            }

            //��ʼ��һ�����б��ʽ��SAllStatExpRst�ṹ
            for(UINT4 i = 0; i < nValidStatExpCount; i++)
            {
                pOneExpRst = (SOneStatExpRst*)(&(pAllRst->ItemRst)[i]);
                pOneExpRst->nCDRCount   = 0;
                pOneExpRst->nValueTotal = 0;
            }

            if((*it)->uBillType == 0)
            {
                sprintf(m_szStatPath, "%s/%s/%s", m_szOrigBasePath, (*it)->szAPName, szLastDate);

                int nReturn = StatCDR(pInfo, m_szStatPath, szLastDate, pIndexList, pAllRst);
                if (nReturn == ERR_FAIL)
                {
                    pTmpRst->SetLastStatTime(curtime);
                    delete pIndexList;
                    delete pTmpMem;
                    return ERR_FAIL;
                }
                else if (nReturn == ERR_STAT_FAIL)
                {
                    delete pIndexList;
                    delete pTmpMem;
                    return ERR_STAT_FAIL;
                }
            }
            else
            {
                LIST<STRING> listChlName;
                listChlName = m_mapAPToChlName[(*it)->szAPName];
                LIST<STRING>::iterator it_chl;

                for(it_chl = listChlName.begin(); it_chl != listChlName.end(); it_chl++)
                {
                    sprintf(m_szStatPath,
                            "%s/%s/%s/%s",
                            m_szFinalBasePath,
                            (*it)->szAPName,
                            (*it_chl).c_str(),
                            szLastDate);

                    pInfo->uBillLen = m_mapChlNameToLen[*it_chl];
                    int nReturn = StatCDR(pInfo, m_szStatPath, szLastDate, pIndexList, pAllRst);
                    if (nReturn ==  ERR_FAIL)
                    {
                        pTmpRst->SetLastStatTime(curtime);
                        delete pIndexList;
                        delete pTmpMem;
                        return ERR_FAIL;
                    }
                    else if (nReturn == ERR_STAT_FAIL)
                    {
                        delete pIndexList;
                        delete pTmpMem;
                        return ERR_STAT_FAIL;
                    }
                }
            }
            //���Ӵ˽���㵱�������ͳ�ƽ��
            pAllRst->uStatEndTime = lasttime;
            pTmpRst->AddStatRst(*pAllRst, lasttime);
            
            if (GetNextDate(szLastDate, lasttime) == -1)
            {
                break;
            }
        }

        pTmpRst->SetLastStatTime(curtime);

        delete pIndexList;
        delete pTmpMem;
    }

    return ERR_SUCCESS;
}

int CCDRStatTask::StatCDR(SCdrStatTaskInfo* pInfo,
                          char* szPath,
                          char* szLastDate,
                          SStatExpIndexList* pIndexList,
                          SAllStatExpRst*& pAllRst)
{
    UINT4 nCDRLen = 0;
    BOOL  bSucc, bRemain;
    BYTE* pCDRRecord = NULL;

    TASKINFO_LIST::iterator it;
    FORMAT_MAP_INFOLIST FormatInfo;
    FORMAT_MAP_INFOLIST::iterator it_formatInfo;

    if(m_mapAPToFormatExpress.find(pInfo->szAPName) == m_mapAPToFormatExpress.end())
    {
        return ERR_SUCCESS;
    }
    //ȡ���˽����ġ���ʽ-���ʽ�б���ӳ��
    FormatInfo = m_mapAPToFormatExpress[pInfo->szAPName];

    CCDRStatFileGroup* pCDRStatFileGroup = new CCDRStatFileGroup;
    pCDRStatFileGroup->Init(pInfo->uAPType,
                            pInfo->uBillType,
                            pInfo->bMustMerge,
                            pInfo->bFinalSPFileFmt,
                            pInfo->uBillLen,
                            pInfo->uAPNumber);

    if (pCDRStatFileGroup->PrepareFiles(szPath) != ERR_SUCCESS)
    {
        delete pCDRStatFileGroup;
        return ERR_STAT_OPEN_FILE_FAIL;
    }

    if (!pCDRStatFileGroup->Open(0))
    {
        delete pCDRStatFileGroup;
        return ERR_STAT_OPEN_FILE_FAIL;
    }

    //ȡ�������ݣ�ֱ��û�л�������Ϊֹ
    do
    {
        if(m_bExit)
        {
            delete pCDRStatFileGroup;
            return ERR_STAT_FAIL;
        }

        //ȡ��һ�Ż���
        bSucc = pCDRStatFileGroup->GetNextRecord(pCDRRecord, nCDRLen, bRemain);
        if (!bSucc)
        {
            delete pCDRStatFileGroup;
            return ERR_FAIL;
        }
        if (!bRemain)
        {
            break;
        }

        CFormat*  pTmpFormat = NULL;
        //��ÿ�Ż������д���
        for(it_formatInfo = FormatInfo.begin(); it_formatInfo!= FormatInfo.end(); it_formatInfo++)
        {
            //ȡ��һ����ʽ
            pTmpFormat = (*it_formatInfo).first;
			
			STRING SFormatName, SFormatDesc;
			pTmpFormat->GetFormatGen(SFormatDesc, SFormatName);
			if (SFormatDesc.compare("ALL") == 0)
			{
				continue;
			}

            //�ж��Ƿ����ڱ���ʽ
            BOOL bFlag = pTmpFormat->IsTheFormat(pCDRRecord, nCDRLen);

            if (bFlag == 0)
            {
                INFO_LIST  ExpressList;
                INFO_LIST::iterator it_Info;

                //������ڱ���ʽ����������
                ExpressList = (*it_formatInfo).second;

                for(it_Info = ExpressList.begin(); it_Info != ExpressList.end(); it_Info++)
                {
                    //�ж��Ƿ����ڴ˲�ѯ����
                    if (pTmpFormat->IsTheQueryCondition(pCDRRecord, *it_Info) == 0)
                    {
                        //���ݱ��ʽ���ҵ����ʽ��������
                        UINT4 nIndex = FindIndexFromName((*it_Info).c_str(), pIndexList);
                        if ( nIndex != ERR_FAIL)
                        {
                            //�����������ҵ���ͳ����������λ��
                            SOneStatExpRst* pOneExpRst = NULL;

                            for(UINT4 i = 0; i < pAllRst->uStatSubItemCount; i++)
                            {
                                pOneExpRst = (SOneStatExpRst*)(&(pAllRst->ItemRst)[i]);

                                if (pOneExpRst->nStatExpIndex == nIndex)
                                {
                                    //�޸�ͳ�ƽ��ֵ
                                    pOneExpRst->nCDRCount++;
                                    pOneExpRst->nValueTotal++;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (1);


    delete pCDRStatFileGroup;
    return ERR_SUCCESS;

}

/*******************************************************************
�������ƣ�BuildMap_AP_FMT_EXP(const char* const szAPName)
��������������������Ӧ����ʽ-���ʽ�б������ڳ�ʼ��ʱ�͵����ʽ�仯ʱ
���������const char* const szAPName,���������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatTask::BuildMap_AP_FMT_EXP(const char* const szAPName)
{
    UINT4 uBillLen = 0;

    //��ø�ʽ�б�
    FORMAT_LIST FormatList;
    FormatList = m_mapAPToOrigFormat[szAPName];

    //�ɽ�������ƻ�ô˽�������еı��ʽ�б�
    ICDRStatRst*  pTmpRst = m_pCDRStatRstMgr->GetStatRstInterface(szAPName);
    LIST<STRING>  AllExpressList;
    pTmpRst->GetAllExpress(AllExpressList);

    //��ý����ġ���ʽ-��ѡ���б�ӳ��
    FORMAT_MAP_INFOLIST  FormatToParaInfo;
    FormatToParaInfo = m_mapAPToFormatParaInfo[szAPName];

    //һ��������ÿ����ʽ��Ӧ�ı��ʽ�б�
    FORMAT_MAP_INFOLIST  FormatToExpress;

    //���������ʽ���ÿ����ʽ�Ŀ�ѡ����Ϣ
    FORMAT_LIST::iterator it_Format;
    for (it_Format = FormatList.begin(); it_Format != FormatList.end(); it_Format++)
    {
        CFormat* pFmt = *it_Format;

        //��ô˽���㻰���ĳ���
        uBillLen = pFmt->GetSrcFmtLen();

        INFO_LIST ExpressList, ParaInfoList;
        //��������ʽ-���ʽ�б�ӳ��

        //1�����ÿ����ʽ�Ŀ�ѡ��
        ParaInfoList = FormatToParaInfo[pFmt];

        //2��ѭ������{����ÿ�����ʽ�����Ƿ����ڴ˸�ʽ��������ڴ˸�ʽ�ͼ��뵽pExpressListӳ�����}
        LIST<STRING>::iterator it_express;

        for(it_express = AllExpressList.begin(); it_express != AllExpressList.end(); it_express++)
        {
            INFO_LIST FieldList;
            if (pFmt->ParseExpress((*it_express).c_str(), FieldList) != 0)
            {
                continue;
            }

            BOOL bFlag = TRUE;
            INFO_LIST::iterator it_field;

            for (it_field = FieldList.begin(); it_field != FieldList.end(); it_field++)
            {
                if (!IsExist(*it_field, ParaInfoList))
                {
                    bFlag = FALSE;
                    break;
                }
            }

            if (bFlag)
            {
                ExpressList.push_back(*it_express);
            }
        }

        //3�����롰��ʽ-���ʽ��ӳ��
        if(ExpressList.size() > 0)
        {
            FormatToExpress[pFmt] = ExpressList;
        }
    }

    //�����ԡ���ʽ-���ʽ�б�ӳ��
    if(FormatToExpress.size() > 0)
    {
        m_mapAPToFormatExpress[szAPName] = FormatToExpress;
    }
}


/*******************************************************************
�������ƣ�IsExist(STRING sField, LIST<STRING> ParaInfoList)
�����������ж�һ�����Ƿ����ڿ�ѡ���б�
���������STRING sField, ����
          LIST<STRING> ParaInfoList,��ѡ���б�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatTask::IsExist(STRING sField, LIST<STRING> ParaInfoList)
{
    INFO_LIST::iterator it_parainfo;

    for (it_parainfo = ParaInfoList.begin(); it_parainfo != ParaInfoList.end(); it_parainfo++)
    {
        if (sField.compare(*it_parainfo) == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}


/*******************************************************************
�������ƣ�FindIndexFromName(const char* const szExpress, SStatExpIndexList* pIndexList)
�������������ݱ��ʽ���Ʋ���������
���������const char* const szExpress,���ʽ
          SStatExpIndexList* pIndexList,���ʽ�б�
�����������
�������ݣ�������
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatTask::FindIndexFromName(const char* const szExpress, SStatExpIndexList* pIndexList)
{
    SStatExpIndexMsg* pIndexMsg = NULL;
    for(UINT4 i = 0; i < pIndexList->uStatExpCount; i++)
    {
        pIndexMsg = (SStatExpIndexMsg*)(&(pIndexList->expIndex)[i]);

        if  (strcmp(pIndexMsg->szStatExp, szExpress) == 0)
        {
            return pIndexMsg->nStatExpIndex;
        }
    }

    return ERR_FAIL;
}


/*******************************************************************
�������ƣ�GetNextDate(char* szDate, time_t& lastdate)
���������������һ�������
���������char* & szDate,���������
���������char* & szDate,��һ�������
          time_t& lastdate, ͳ��ʱ��
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatTask::GetNextDate(char* szDate, time_t& lastdate)
{
    int nYear = 0, nMon = 0, nDay = 0;
    char szTemp[10];
    if(NULL == szDate)
    {
        return -1;
    }

    //�ֽ�����ƥ�䣬�򷵻�ʧ��
    if(strlen(szDate) != 8)
    {
        return -1;
    }

    //ȡ��
    memcpy(szTemp, szDate, 4);
    szTemp[4] = 0;
    nYear = atoi(szTemp);

    //ȡ��
    memcpy(szTemp, szDate+4, 2);
    szTemp[2] = 0;
    nMon = atoi(szTemp);

    //ȡ��
    memcpy(szTemp, szDate+6, 2);
    szTemp[2] = 0;
    nDay = atoi(szTemp);

    switch (nMon)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
        {
            if (nDay == 31)
            {
                nMon++;
                nDay = 1;
            }
            else
            {
                nDay++;
            }
            break;
        }
    case 12:
        {
            if (nDay == 31)
            {
                nYear++;
                nMon = 1;
                nDay = 1;
            }
            else
            {
                nDay++;
            }
            break;
        }
    case 4:
    case 6:
    case 9:
    case 11:
        {
            if (nDay == 30)
            {
                nMon++;
                nDay = 1;
            }
            else
            {
                nDay++;
            }
            break;
        }
    case 2:
        {
            if ((nYear % 400 == 0) || ((nYear % 4 == 0) && (nYear % 100 != 0)))
            {
                if (nDay == 29)
                {
                    nMon++;
                    nDay = 1;
                }
                else
                {
                    nDay++;
                }

            }
            else
            {
                if (nDay == 28)
                {
                    nMon++;
                    nDay = 1;
                }
                else
                {
                    nDay++;
                }
            }
            break;
        }
    default:
        break;
    }

    tm tmTar;
    tmTar.tm_year = nYear - 1900;
    tmTar.tm_mon  = nMon - 1;
    tmTar.tm_mday = nDay;
    tmTar.tm_hour = 0;
    tmTar.tm_min  = 0;
    tmTar.tm_sec  = 0;

    lastdate = mktime(&tmTar);

    //�����һ�������
    sprintf(szDate, "%04d%02d%02d", nYear, nMon, nDay);
    return 0;
}

/*******************************************************************
�������ƣ�FindField(const char* const szFieldName, LIST<STRING> FieldList)
����������������
���������
    ����1��const char* const szFieldName,����
    ����2��LIST<STRING> FieldList,���б�
�����������
�������ݣ��ɹ�����TRUE,ʧ�ܷ���FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatTask::FindField(const char* const szFieldName, LIST<STRING> FieldList)
{
    LIST<STRING>::iterator it;
    for (it = FieldList.begin(); it != FieldList.end(); it++)
    {
        if (strcmp(szFieldName, (*it).c_str()) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
CCDRStatRstMgr* CCDRStatTask::GetCCDRStatRstMgr()
{
    return m_pCDRStatRstMgr;
}

