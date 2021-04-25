#include "../include/toolbox.h"
#include "../include/frame.h"
#include "cdr_stat_cfg.h"

/*******************************************************************
�������ƣ�CCDRStatCfg()
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
CCDRStatCfg::CCDRStatCfg()
{
    m_uAccessPoint  = 0;
    m_nMemDataLen   = 0;
    m_pMemData      = NULL;
    m_szFileName[0] = '\0';

}
/*******************************************************************
�������ƣ�~CCDRStatCfg()
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
CCDRStatCfg::~CCDRStatCfg()
{
    if (m_pMemData != NULL)
    {
        delete m_pMemData;
        m_pMemData = NULL;
    }
}


/*******************************************************************
�������ƣ�Init(UINT4 uAccessPoint)
�������������ò��������г�ʼ������
���������UINT4 uAccessPoint,������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatCfg::Init(UINT4 uAccessPoint)
{
    m_uAccessPoint = uAccessPoint;

    CINIFile iniFile(GetCfgFilePath());
    char szPath[MAX_PATH];
    iniFile.Open();
    iniFile.GetString(CFG_SEC_CDRSTAT,
                      CFG_CDRSTAT_ROOT_DIR,
                      CFG_CDRSTAT_ROOT_DIR_DEFAULT,
                      szPath,
                      sizeof(szPath));
    szPath[sizeof(szPath) - 1] = '\0';

    //�������ļ��ж�ȡ��ǰ���������
    char szAPSec[25];
    SNPRINTF(szAPSec,
             sizeof(szAPSec),
             "%s%d",
             CFG_SEC_AP,
             m_uAccessPoint);
    szAPSec[sizeof(szAPSec) - 1] = '\0';

    char szAPName[25];
    iniFile.GetString(szAPSec,
                      CFG_AP_NAME,
                      szAPSec,
                      szAPName,
                      sizeof(szAPName));
    szAPName[sizeof(szAPName) - 1] = '\0';
    m_sAPName = szAPName;

    if (!CreateDeepDir(szPath))
    {
        return ERR_FAIL;
    }

    //�����ļ����ļ���
    sprintf(m_szFileName, "%s/cdr_stat_%d.dat", szPath, m_uAccessPoint);

    FILE* pFile = NULL;

    //����ļ������ڣ�Ҫ�����ļ�
    if (access(m_szFileName, 0) == -1)
    {
        pFile = fopen(m_szFileName, "wb+");
        //�����ļ�ʧ��
        if (pFile == NULL)
        {
            return ERR_FAIL;
        }

        //������ͷ��Ϣд���ļ���
        SStatCfgHead head;
        head.uAccessPoint   = m_uAccessPoint;
        head.uStatItemCount = 0;
        head.uVersion       = 0x00000001;
        memset(head.yReserved, 0, 8);

        if (fwrite(&head, STAT_CFG_HEAD_LENGTH, 1, pFile) != 1)
        {
            fclose(pFile);
            return ERR_FAIL;
        }

        fclose(pFile);
    }

    //���ļ�
    if ((pFile = fopen(m_szFileName, "rb")) == NULL)
    {
        return ERR_FAIL;
    }

    //����ļ���С
    UINT4 nFileLen = FileSize(pFile);
    m_nMemDataLen  = nFileLen;
    m_pMemData     = new BYTE[m_nMemDataLen];
    memset(m_pMemData, 0, m_nMemDataLen);

    //��������Ϣ�����ڴ�
    if (fread(m_pMemData, m_nMemDataLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fclose(pFile);
    return ERR_SUCCESS;
}



/*******************************************************************
�������ƣ�GetStatItemNameList(LIST<STRING>& listStatItemName)
�����������������ͳ�������Ŀ
���������LIST<char* >& listStatItemName,ͳ���������б�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatCfg::GetStatItemNameList(LIST<STRING>& listStatItemName)
{
    //��û��д���κ�ͳ����Ŀ,���ؿ�
    if (m_nMemDataLen <= STAT_CFG_HEAD_LENGTH)
    {
        return;
    }

    UINT4 nOffset = STAT_CFG_HEAD_LENGTH;
    SStatCfgHead* pHead = (SStatCfgHead*)m_pMemData;

    for (UINT4 i = 0; i < pHead->uStatItemCount; i++)
    {
        SStatItemCfg* pStatItemCfg = (SStatItemCfg*)(m_pMemData + nOffset);
        nOffset = nOffset + pStatItemCfg->uStatItemCount * SUBITEMCFGLENGTH + STATITEMCFGLENGTH;
        listStatItemName.push_back(pStatItemCfg->szStatItemName);
    }

}

/*******************************************************************
�������ƣ�GetStatItem(const char* const szStatItemName)
�������������ָ��ͳ����Ŀ����ͳ��������Ϣ
���������const char* const szStatItemName,ͳ��������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
SStatItemCfg* CCDRStatCfg::GetStatItem(const char* const szStatItemName)
{
    SStatItemCfg* pStatItemCfg = NULL;

    UINT4 nOffset = STAT_CFG_HEAD_LENGTH;
    SStatCfgHead* pHead = (SStatCfgHead*)m_pMemData;

    for (UINT4 i = 0; i < pHead->uStatItemCount; i++)
    {
        SStatItemCfg* pCfg = NULL;
        pCfg = (SStatItemCfg*)(m_pMemData + nOffset);
        nOffset = nOffset + pCfg->uStatItemCount * SUBITEMCFGLENGTH + STATITEMCFGLENGTH;

        if (strcmp(pCfg->szStatItemName, szStatItemName) == 0)
        {
            pStatItemCfg = new (pCfg->uStatItemCount)SStatItemCfg;
            sprintf((char*)pStatItemCfg->szStatItemName, "%s", pCfg->szStatItemName);
            pStatItemCfg->uStatItemCount = pCfg->uStatItemCount;

            memcpy(pStatItemCfg->subItemCfg,
                   pCfg->subItemCfg,
                   pStatItemCfg->uStatItemCount * sizeof(SStatSubItemCfg));

            break;
        }
    }

    return pStatItemCfg;

}

/*******************************************************************
�������ƣ�GetStatItem(const char* const szStatItemName, LIST<SOneStatSubItemCfg*>& subItemList)
�������������ָ��ͳ����Ŀ����ͳ��������Ϣ
���������const char* const szStatItemName,ͳ��������
          LIST<SOneStatSubItemCfg*>& subItemList,ͳ�����б�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatCfg::GetStatItem(const char* const szStatItemName,
                              LIST<SOneStatSubItemCfg*>& subItemList)
{
    const SStatItemCfg* pStatItemCfg = NULL;

    pStatItemCfg = GetStatItem(szStatItemName);

    if (pStatItemCfg == NULL)
    {
        return;
    }

    UINT4 nCount = pStatItemCfg->uStatItemCount;

    //��䷵���б�subItemList
    const SStatSubItemCfg* pTmp;
    for( UINT4 i = 0; i < nCount; i++)
    {
        pTmp = &(pStatItemCfg->subItemCfg[i]);

        SOneStatSubItemCfg* pOneCfg = new SOneStatSubItemCfg;

        memcpy(pOneCfg->szStatItemName,
               pStatItemCfg->szStatItemName,
               sizeof(pOneCfg->szStatItemName));

        memcpy(pOneCfg->szStatSubItemName,
               pTmp->szStatSubItemName,
               sizeof(pOneCfg->szStatSubItemName));

        memcpy(pOneCfg->szStatSubItemCond,
               pTmp->szStatSubItemCond,
               sizeof(pOneCfg->szStatSubItemCond));

        subItemList.push_back(pOneCfg);
    }
}


/*******************************************************************
�������ƣ�SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
������������������ͳ�������������Ϣ
���������LIST<SOneStatSubItemCfg*> subItemList,ͳ�����б�
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatCfg::SetStatCfg(LIST<SOneStatSubItemCfg*> subItemList)
{
    SOneStatSubItemCfg* pTemp = NULL;
    char  szItemName[64];
    szItemName[63] = '\0';

    UINT4 nItemCount  = 0, nSubItemCount = 0;
    UINT4 nItemOffSet = 0, nSubItemOffSet= 0;
    SStatItemCfg    StatItemCfg;
    SStatSubItemCfg StatSubItemCfg;

    LIST<SOneStatSubItemCfg*>::iterator it = subItemList.begin();

    BOOL bFlag = FALSE;
    BOOL bIsTheSameStatItem = TRUE;

    //��һ��ͳ�����λ����20
    nItemOffSet = STAT_CFG_HEAD_LENGTH;
    //��һ�������λ����88(20+68)
    nSubItemOffSet = STAT_CFG_HEAD_LENGTH + STATITEMCFGLENGTH;

    while (it != subItemList.end())
    {
        pTemp = (SOneStatSubItemCfg*)(*it);

        if (bIsTheSameStatItem)
        {
            SNPRINTF(szItemName, sizeof(szItemName), "%s", pTemp->szStatItemName);
            szItemName[sizeof(szItemName) - 1] = '\0';
            bIsTheSameStatItem = FALSE;
        }

        //���ͳ�������Ʋ���ȣ�ͳ����ĸ���Ҫ����
        if ((strcmp(pTemp->szStatItemName, szItemName) != 0) && bFlag)
        {
            nItemCount++;

            //��һ��ͳ����д����Ϣ��������
            memcpy(StatItemCfg.szStatItemName,
                   szItemName,
                   sizeof(StatItemCfg.szStatItemName));
            StatItemCfg.uStatItemCount = nSubItemCount;

            memcpy(m_pMemData + nItemOffSet, &StatItemCfg, STATITEMCFGLENGTH);

            nSubItemCount = 0;

            SNPRINTF(szItemName, sizeof(szItemName), "%s", pTemp->szStatItemName);
            szItemName[sizeof(szItemName) - 1] = '\0';
            

            //������һ��ͳ�������һ��ͳ�������λ��
            nItemOffSet = nSubItemOffSet;
            nSubItemOffSet = nSubItemOffSet + STATITEMCFGLENGTH;
        }


        //��ÿһ������д���ڴ�
        memcpy(StatSubItemCfg.szStatSubItemName,
               pTemp->szStatSubItemName,
               sizeof(StatSubItemCfg.szStatSubItemName));
        memcpy(StatSubItemCfg.szStatSubItemCond,
               pTemp->szStatSubItemCond,
               sizeof(StatSubItemCfg.szStatSubItemCond));

        //���绺��������
        if (nSubItemOffSet + SUBITEMCFGLENGTH > m_nMemDataLen)
        {
            BYTE* pMemData = m_pMemData;
            m_nMemDataLen  = m_nMemDataLen + INCMEMLENGTH;
            m_pMemData     = new BYTE[m_nMemDataLen];
            memcpy(m_pMemData, pMemData, nSubItemOffSet);
            memset(&m_pMemData[nSubItemOffSet], 0, m_nMemDataLen - nSubItemOffSet);
            delete pMemData, pMemData = NULL;
        }

        memcpy(m_pMemData + nSubItemOffSet, &StatSubItemCfg, SUBITEMCFGLENGTH);

        nSubItemOffSet = nSubItemOffSet + SUBITEMCFGLENGTH;

        nSubItemCount++;
        bFlag = TRUE;
        it++;
    }

    //�޸����һ��ͳ�����ͷ��Ϣ
    if (bFlag)
    {
        nItemCount++;
        //��һ��ͳ����д����Ϣ��������
        memcpy(StatItemCfg.szStatItemName,
               pTemp->szStatItemName,
               sizeof(StatItemCfg.szStatItemName));
        StatItemCfg.uStatItemCount = nSubItemCount;
        memcpy(m_pMemData + nItemOffSet, &StatItemCfg, STATITEMCFGLENGTH);
    }

    //�޸�������Ϣͷ������ͳ������Ŀ
    SStatCfgHead head;
    memcpy(&head, m_pMemData, STAT_CFG_HEAD_LENGTH);
    head.uStatItemCount = nItemCount;
    memcpy(m_pMemData, &head, STAT_CFG_HEAD_LENGTH);

    //��m_pMemData������д���ļ���
    FILE* pFile = NULL;
    if ((pFile = fopen(m_szFileName, "wb")) == NULL)
    {
        return ERR_FAIL;
    }

    if (fwrite(m_pMemData, m_nMemDataLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fclose(pFile);
    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�GetAccessPoint()
�������������ؽ�����
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
UINT4 CCDRStatCfg::GetAccessPoint()
{
    return m_uAccessPoint;
}


/*******************************************************************
�������ƣ���ý��������
����������GetAccessPointName()
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
const char* CCDRStatCfg::GetAccessPointName()
{
    return m_sAPName.c_str();
}
