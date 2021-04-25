#include "../include/toolbox.h"
#include "../include/frame.h"
#include "cdr_stat_rst.h"


/*******************************************************************
�������ƣ�CCDRStatRst()
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
CCDRStatRst::CCDRStatRst()
{
    m_uAccessPoint  = 0;
    m_nMemDataLen   = 0;
    m_pMemData      = NULL;
    m_szFileName[0] = '\0';
}

/*******************************************************************
�������ƣ�~CCDRStatRst()
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
CCDRStatRst::~CCDRStatRst()
{
     if (m_pMemData != NULL)
    {
        delete[] m_pMemData;
        m_pMemData = NULL;
    }
    if (m_pHead != NULL)
    {
        delete[] m_pHead;
        m_pHead = NULL;
    }

}

/*******************************************************************
�������ƣ�Init(UINT4 uAccessPoint)
�������������ò��������г�ʼ������
���������UINT4 uAccessPoint,������
�����������
�������ݣ��ɹ�����ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatRst::Init(UINT4 uAccessPoint)
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
    sprintf(m_szFileName, "%s/stat_rst_%d.dat", szPath, m_uAccessPoint);

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

        //�ѽ��ͷ��Ϣд���ļ���
        time_t curtime = ACE_OS::time(NULL);
        SStatRstHead head;
        head.uAccessPoint   = m_uAccessPoint;
        head.uVersion       = 0x00000001;
        head.tStatEndTime   = curtime;
        memset(head.yReserved, 0, 8);

        if (fwrite(&head, STAT_RST_HEAD_LENGTH, 1, pFile) != 1)
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

    m_pHead = new BYTE[STAT_RST_HEAD_LENGTH];
    memset(m_pHead, 0, STAT_RST_HEAD_LENGTH);

    //������ͷ��Ϣ�����ڴ�
    if (fread(m_pHead, STAT_RST_HEAD_LENGTH, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fseek(pFile, 0, SEEK_SET);

    //��������Ϣ�����ڴ�
    if ( fread(m_pMemData, m_nMemDataLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    fclose(pFile);
    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�GetStatRstHead(SStatRstHead& statRstHead)
�������������ͳ�ƽ���ļ�ͷ��Ϣ
�����������
���������SStatRstHead& statRstHead,ͳ�ƽ���ṹͷ��Ϣ
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatRst::GetStatRstHead(SStatRstHead& statRstHead)
{
    memcpy(&statRstHead, m_pHead, STAT_RST_HEAD_LENGTH);
}


/*******************************************************************
�������ƣ�GetStatExpList(SStatExpIndexList& statExpIndexList)
�����������������ͳ���������ʽ
�����������
���������SStatExpIndexList& statExpIndexList,����ͳ���������ʽ
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
SStatExpIndexList* CCDRStatRst::GetStatExpList(void)
{
    SStatExpIndexList* pTemp;
    SStatExpIndexList* pStatExpIndexList;

    pTemp = (SStatExpIndexList*)(m_pMemData + STAT_RST_HEAD_LENGTH);

    if (m_nMemDataLen <= STAT_RST_HEAD_LENGTH)
    {
        pStatExpIndexList = new SStatExpIndexList;
        pStatExpIndexList->uStatExpCount = 0;
        return pStatExpIndexList;
    }
    else
    {
        pStatExpIndexList = new (pTemp->uStatExpCount)SStatExpIndexList;
    }

    pStatExpIndexList->uStatExpCount = pTemp->uStatExpCount ;

    memcpy(pStatExpIndexList->expIndex,
           pTemp->expIndex,
           pTemp->uStatExpCount * sizeof(SStatExpIndexMsg));

    return pStatExpIndexList;
}


/*******************************************************************
�������ƣ�GetStatExpRst(const char* const szStatExpress, time_t tBeginTime,
                        time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
�������������ָ��ʱ�䷶Χ��ͳ���������ʽ�Ľ��
���������const char* const szStatExpress,���ʽ
          time_t tBeginTime, ��ʼʱ��
          time_t tEndTime, ����ʱ��
���������UINT4& nCDRCount, ��������
          UINT4& nValueTotal,�ۼ�ֵ
�������ݣ��ɹ�����ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatRst::GetStatExpRst(const char* const szStatExpress, time_t tBeginTime,
                        time_t tEndTime, UINT4& nCDRCount, UINT4& nValueTotal)
{
    UINT4 nOffset = 0;
    UINT4 nCurrentIndex = 0;
    SStatExpIndexMsg*  pExpMsg  = NULL;
    if (FindStatExpress((char*)szStatExpress, nOffset))
    {
        pExpMsg = (SStatExpIndexMsg*)(m_pMemData + nOffset);
        nCurrentIndex = pExpMsg->nStatExpIndex;
    }
    else
    {
        return ERR_FAIL;
    }

    nOffset = STAT_RST_HEAD_LENGTH + 4;
    SStatExpIndexList* pExpList = (SStatExpIndexList*)(m_pMemData + STAT_RST_HEAD_LENGTH);

    nOffset = nOffset + pExpList->uStatExpCount * STATEXPINDEXMSGLENGTH;

    SAllStatExpRst*  pAllRst = NULL;
    SOneStatExpRst*  pOneRst = NULL;

    while (nOffset < m_nMemDataLen)
    {
        pAllRst = (SAllStatExpRst*)(m_pMemData + nOffset);

        if ((pAllRst->uStatEndTime >= tBeginTime) && (pAllRst->uStatEndTime <= tEndTime))
        {
            FindStatRst(pAllRst, nCurrentIndex, nCDRCount, nValueTotal);
        }
        nOffset = nOffset + pAllRst->uStatSubItemCount * ONESTATEXPRSTLENGTH + 8;
    }

    return ERR_SUCCESS;
}


/*******************************************************************
�������ƣ�AddStatExpress(LIST<char* >& StatExpList)
�������������ͳ���������ʽ
���������LIST<char* >& StatExpList,���ʽ�б�
�����������
�������ݣ��ɹ�����ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatRst::AddStatExpress(LIST<char* >& StatExpList)
{
    FILE* pFile = NULL;
    if ((pFile = fopen(m_szFileName, "rb+")) == NULL)
    {
        return ERR_FAIL;
    }

    //����дͷ
    fseek(pFile, 0, SEEK_SET);

    if (fwrite(m_pHead, 20, 1, pFile) != 1)
    {
        fclose(pFile);
    }

    SStatExpIndexMsg  ExpMsg;
    UINT4 nStatExpCount = 0;

    //��һ��д��ͷû��д���ݵ�ʱ��
    if (m_nMemDataLen <= STAT_RST_HEAD_LENGTH)
    {
        ExpMsg.nStatExpIndex  = 0;
    }
    else
    {
        if (m_nMemDataLen <= STAT_RST_HEAD_LENGTH)
        {
            nStatExpCount = 0;
        }
        else
        {
            SStatExpIndexList* pExpList;
            pExpList = (SStatExpIndexList*)(m_pMemData + STAT_RST_HEAD_LENGTH);

            nStatExpCount = pExpList->uStatExpCount;
        }
    }

    UINT4 nOffset = STAT_RST_HEAD_LENGTH + 4;
    //ȫ����ԭ���ı��ʽ��Ϊ������
    for (UINT4 i = 1; i <= nStatExpCount; i++)
    {
        memcpy(&ExpMsg, m_pMemData + nOffset, STATEXPINDEXMSGLENGTH);
        ExpMsg.bInUsed = FALSE;

        fseek(pFile, nOffset, SEEK_SET);

        //д���ļ���
        if (fwrite(&ExpMsg, STATEXPINDEXMSGLENGTH, 1, pFile) != 1)
        {
            fclose(pFile);
            RollBack();
            return ERR_FAIL;
        }
        nOffset = nOffset + STATEXPINDEXMSGLENGTH;
    }

    //���һ��һ�����ʽ�������ţ�������������
    UINT4 nLastExpMsgIndex = ExpMsg.nStatExpIndex;

    LIST<char* >::iterator it = StatExpList.begin();

    UINT4 nPosition = 0, nAddExpLen = nOffset;
    while (it != StatExpList.end())
    {
        if (FindStatExpress(*it, nPosition))
        {
            //����ԭ�������ҵ��ģ��޸�bInUsedΪTRUEȻ��д�ļ���
            memcpy(&ExpMsg, m_pMemData + nPosition, STATEXPINDEXMSGLENGTH);
            ExpMsg.bInUsed = TRUE;

            fseek(pFile, nPosition, SEEK_SET);

            //д���ļ���
            if (fwrite(&ExpMsg, STATEXPINDEXMSGLENGTH, 1, pFile) != 1)
            {
                fclose(pFile);
                RollBack();
                return ERR_FAIL;
            }
        }
        else
        {
            //����ԭ�������ڵģ�����һ����nOffset�������д��
            ExpMsg.bInUsed = TRUE;
            nLastExpMsgIndex++;
            ExpMsg.nStatExpIndex = nLastExpMsgIndex;
            strcpy(ExpMsg.szStatExp, *it);

            fseek(pFile, nAddExpLen, SEEK_SET);

            //д���ļ���
            if (fwrite(&ExpMsg, STATEXPINDEXMSGLENGTH, 1, pFile) != 1)
            {
                fclose(pFile);
                RollBack();
                return ERR_FAIL;
            }

            //�ļ�λ�ú���
            nAddExpLen = nAddExpLen + STATEXPINDEXMSGLENGTH;
        }
        it++;
    }

    //��д���ʽ������,����д���ļ�
    UINT4 nCount = (nAddExpLen - STAT_RST_HEAD_LENGTH - 4) / STATEXPINDEXMSGLENGTH;

    fseek(pFile, STAT_RST_HEAD_LENGTH, SEEK_SET);

    if (fwrite(&nCount, 4, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }


    if (m_nMemDataLen > nOffset)
    {
        fseek(pFile, nAddExpLen, SEEK_SET);
        //����ͳ�ƽ��������дһ��,д���ļ���
        if (fwrite(m_pMemData + nOffset, m_nMemDataLen - nOffset, 1, pFile) != 1)
        {
            fclose(pFile);
            RollBack();
            return ERR_FAIL;
        }
    }

    fclose(pFile);
    delete[] m_pMemData;
    m_pMemData = NULL;

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
�������ƣ�AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime)
�������������һ�������ͳ�Ʊ��ʽ��ͳ�ƽ��
���������const SAllStatExpRst& statExpRst,һ���ͳ�ƽ��
�����������
�������ݣ��ɹ�����ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatRst::AddStatRst(const SAllStatExpRst& statExpRst, time_t statTime)
{
    FILE* pFile = NULL;

    if ((pFile = fopen(m_szFileName, "rb+")) == NULL)
    {
        return ERR_FAIL;
    }

    SStatRstHead* pStatRstHead = (SStatRstHead*)m_pHead;
    pStatRstHead->tStatEndTime = statTime;

    //�޸�ͳ��ʱ��
    fseek(pFile, 0, SEEK_SET);
    int nWrite = fwrite(m_pHead, sizeof(SStatRstHead), 1, pFile);
    if(nWrite != 1)
    {
        return ERR_FAIL;
    }

    //����ͳ������
    UINT4 nAllRstLen = statExpRst.uStatSubItemCount * ONESTATEXPRSTLENGTH + 8;

    fseek(pFile, m_nMemDataLen, SEEK_SET);
    if (fwrite(&statExpRst, nAllRstLen, 1, pFile) != 1)
    {
        fclose(pFile);
        return ERR_FAIL;
    }

    //����ļ���С
    UINT4 nFileLen = FileSize(pFile);
    m_nMemDataLen  = nFileLen;
    delete m_pMemData;
    m_pMemData     = new BYTE[m_nMemDataLen];
    memset(m_pMemData, 0, m_nMemDataLen);

    fseek(pFile, 0, SEEK_SET);
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
�������ƣ�CleanStatExp(void)
��������������ͳ�ƽ���ļ��в���ʹ�õ��������ʽ
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatRst::CleanStatExp(void)
{
    UINT4 nOffset = STAT_RST_HEAD_LENGTH + 4;
    SStatExpIndexMsg*  pExpMsg  = NULL;
    SStatExpIndexList* pExpList = NULL;
    pExpList = GetStatExpList();

    //�����������ʽ�������bIsUsedΪFALSE�ı��ʽ
    for (UINT4 i = 0; i < pExpList->uStatExpCount; i++)
    {
        pExpMsg = (SStatExpIndexMsg*)(m_pMemData + nOffset);

        if (!pExpMsg->bInUsed)
        {
            //�ҵ�bInUsedΪFALSE�ı��ʽ,�����������ҵ���ͳ�ƽ����λ��
            //�����
        }
        nOffset = nOffset +  STATEXPINDEXMSGLENGTH;
    }
    delete pExpList;

    return ERR_SUCCESS;
}

/*******************************************************************
�������ƣ�FindStatExpress(char* szExpress, UINT4& nPosition)
�������������ұ��ʽ��λ��
���������char* szExpress, ���ҵı��ʽ
���������UINT4& nPosition,���ʽλ��
�������ݣ��ҵ�����TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatRst::FindStatExpress(char* szExpress, UINT4& nPosition)
{
    BOOL bIsFinded = FALSE;

    //��һ��д��ͷû��д���ݵ�ʱ��
    if (m_nMemDataLen <= STAT_RST_HEAD_LENGTH)
    {
        return bIsFinded;
    }

    UINT4 nOffset = STAT_RST_HEAD_LENGTH + 4;

    SStatExpIndexMsg*  pExpMsg  = NULL;
    SStatExpIndexList* pExpList = NULL;

    pExpList = GetStatExpList();
    //�����������ʽ�������szExpress
    for (UINT4 i = 0; i < pExpList->uStatExpCount; i++)
    {
        if (strcmp((pExpList->expIndex[i]).szStatExp, szExpress) == 0)
        {
            bIsFinded = TRUE;
            break;
        }
        nOffset = nOffset +  STATEXPINDEXMSGLENGTH;
    }

    nPosition = nOffset;
    delete pExpList;
    return bIsFinded;
}


/*******************************************************************
�������ƣ�RollBack(void)
�������������дʧ�ܣ���m_pMemData���ݻ�д
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatRst::RollBack(void)
{
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
�������ƣ�FindStatRst(SAllStatExpRst* pAllRst, UINT4 nIndex, UINT4& nCDRCount, UINT4& nValueTotal)
���������������е�ͳ�ƽ���и��������Ų�����ͳ�ƽ��ֵ
���������SAllStatExpRst* pAllRst, ����ͳ�ƽ���ṹ
          UINT4 nIndex, ������
���������UINT4& nCDRCount, ��������
          UINT4& nValueTotal,�ۼ�ֵ
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatRst::FindStatRst(SAllStatExpRst* pAllRst,
                              UINT4 nIndex,
                              UINT4& nCDRCount,
                              UINT4& nValueTotal)
{
    SOneStatExpRst* pOneRst = NULL;

    //�����������ʽ�������szExpress
    for (UINT4 i = 0; i < pAllRst->uStatSubItemCount; i++)
    {
        pOneRst = (SOneStatExpRst*)(&(pAllRst->ItemRst[i]));

        if (pOneRst->nStatExpIndex == nIndex)
        {
            nCDRCount   += pOneRst->nCDRCount;
            nValueTotal += pOneRst->nValueTotal;
            break;
        }
    }

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
UINT4 CCDRStatRst::GetAccessPoint()
{
    return m_uAccessPoint;
}


/*******************************************************************
�������ƣ�GetAccessPointName()
�������������ؽ��������
�����������
�����������
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
const char* CCDRStatRst::GetAccessPointName()
{
    return m_sAPName.c_str();
}

/*******************************************************************
�������ƣ�GetAllExpress(LIST<STRING>& ExpressList)
�������������ݽ����ŷ������б��ʽ
�����������
���������LIST<STRING>& ExpressList,������б��ʽ�б�
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void  CCDRStatRst::GetAllExpress(LIST<STRING>& ExpressList)
{
    SStatExpIndexList* pExpList = NULL;
    pExpList = GetStatExpList();

    for( UINT4 i = 0; i < pExpList->uStatExpCount; i++)
    {
        if((pExpList->expIndex[i]).bInUsed)
        {
            ExpressList.push_back((pExpList->expIndex[i]).szStatExp);
        }
    }
    delete pExpList;
}

/*******************************************************************
�������ƣ�GetLastStatTime()
��������������ϴ�ͳ��ʱ��
�����������
�����������
�������ݣ��ϴ�ͳ��ʱ��
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
time_t CCDRStatRst::GetLastStatTime()
{
    SStatRstHead* pStatRstHead = (SStatRstHead*)m_pHead;

    return pStatRstHead->tStatEndTime;
}

//ˢ����ִ��ͳ�Ʋ�����ʱ��
int CCDRStatRst::SetLastStatTime(time_t curtime)
{
    SStatRstHead* pStatRstHead = (SStatRstHead*)m_pHead;

    pStatRstHead->tStatEndTime = curtime;

    FILE* pFile = NULL;

    if ((pFile = fopen(m_szFileName, "rb+")) != NULL)
    {
        //�޸�ͳ��ʱ��
        fseek(pFile, 0, SEEK_SET);
        int nWrite = fwrite(m_pHead, sizeof(SStatRstHead), 1, pFile);
        fclose(pFile);

        if(nWrite != 1)
        {
            return ERR_FAIL;
        }
         
        return ERR_SUCCESS;
    }

    return ERR_FAIL;
}
