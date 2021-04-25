#include "../include/toolbox.h"
#include "../include/frame.h"
#include "cdr_stat_file.h"
#include "stat_file_group.h"
#include "cdr_stat_svr.h"

/*******************************************************************
�������ƣ�CCDRStatFileGroup()
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
CCDRStatFileGroup::CCDRStatFileGroup()
{
    m_nCurFileNo   = 0;
    m_pCDRStatFile = NULL;
    m_szPath[0]    = '\0';
}
/*******************************************************************
�������ƣ�~CCDRStatFileGroup()
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
CCDRStatFileGroup::~CCDRStatFileGroup()
{
    if (m_pCDRStatFile != NULL)
    {
        delete  m_pCDRStatFile;
        m_pCDRStatFile = NULL;
    }
}


/*******************************************************************
�������ƣ�Init(UINT4 uAPType, UINT4 uBillType, BOOL bMustMerge, UINT4 uBillLen)
�������������ò��������г�ʼ������
���������UINT4 uAPType, ���������
          UINT4 uBillType, �ļ�����
          BOOL bMustMerge, �Ƿ�ϲ���־
          UINT4 uBillLen,��������
�����������
�������ݣ��ɹ�ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatFileGroup::Init(UINT4 uAPType,
                            UINT4 uBillType,
                            BOOL bMustMerge,
                            BOOL bFinalSPFileFmt,
                            UINT4 uBillLen, 
                            int nAPID)
{
    m_pCDRStatFile = new CCDRStatFile;
    m_pCDRStatFile->Init(uAPType,
                         uBillType,
                         bMustMerge,
                         bFinalSPFileFmt,
                         uBillLen,
                         nAPID);

    return ERR_SUCCESS;
}

/*******************************************************************
�������ƣ�PrepareFiles(LIST<char*> listPath)
����������׼��ָ�����ڷ�Χ�ڵĻ����ļ�
���������const char* const szPath, �ļ�·��
�����������
�������ݣ��ɹ�ERR_SUCCESS,���򷵻�ERR_FAIL
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
int CCDRStatFileGroup::PrepareFiles(const char* const szPath)
{
    m_FileNames.clear();
    strcpy(m_szPath, szPath);

    //�����������ļ������б�
    LIST<STRING> FileList;

    //��ȡĿ¼���Ź���������ļ���Ȼ����еõ���Χ�ڵ������ļ�
    CCDRStatSvr::GetBillFiles(szPath, FileList);

/*
    //����õ����ļ��б�Ϊ�գ�˵����ʼ�ļ�����ֹ�ļ�
    //��Ӳ�����Ѿ������ڣ���ʱ����ʧ��
    if (FileList.empty())
    {
        return ERR_FAIL;
    }
*/

    //���õ����ļ��б��뵽m_qFileNames��
    LIST<STRING>::iterator i;
    for (i = FileList.begin(); i != FileList.end(); i++)
    {
        m_FileNames.push_back(*i);
    }

    return ERR_SUCCESS;
}

/*******************************************************************
�������ƣ�GetNextRecord(BYTE*& pCDRRecord,  UINT4& nCDRLen, BOOL&  bRemain)
������������ȡ��һ�Ż�����¼����
�����������
���������BYTE*& pCDRRecord, ��û�������
          UINT4& nCDRLen, ���Ż����ĳ���
          BOOL&  bRemain, �Ƿ��л���
�������ݣ��ɹ�TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatFileGroup::GetNextRecord(BYTE*& pCDRRecord,
                                      UINT4& nCDRLen,
                                      BOOL&  bRemain)
{
    BOOL bRemainInFile;
    while (1)
    {
        BOOL bSucc = m_pCDRStatFile->GetNextBill(pCDRRecord,
                                                 nCDRLen,
                                                 bRemainInFile);
        if (!bSucc)
        {
            return FALSE;
        }

        if (bRemainInFile)  //�ڵ�ǰ���ļ����ҵ��˻�����¼
        {
            bRemain = TRUE;
            return TRUE;
        }
        else    //���򣬴���һ���ļ������������һ�Ż�����¼
        {
            BOOL bRemainInGroup;
            bSucc = OpenNextFile(bRemainInGroup);
            if (!bSucc)
            {
                return FALSE;
            }
            if (!bRemainInGroup)
            {
                bRemain = FALSE;
                return TRUE;
            }
        }
    }

    return TRUE;
}

/*******************************************************************
�������ƣ�Open(int nFileNo)
������������ָ���Ļ����ļ�
���������int nFileNo,�ļ���
�����������
�������ݣ��򿪳ɹ�����TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatFileGroup::Open(int nFileNo)
{
    ASSERT(NULL != m_pCDRStatFile);

    BOOL bSucc = FALSE;

    if(m_FileNames.size() == 0)
    {
        return FALSE;
    }

    //�رյ�ǰ�ļ�����ָ���ļ���������дλ��
    bSucc = m_pCDRStatFile->Close();
    if (!bSucc)
    {
        return FALSE;
    }

    char szFileName[MAX_PATH], szFileFullPath[MAX_PATH];
    strcpy(szFileName, m_FileNames[m_nCurFileNo].c_str());
    sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName);
    bSucc = m_pCDRStatFile->Open(szFileFullPath);
    if (!bSucc)
    {
         return FALSE;
    }
    m_nCurFileNo = nFileNo;

    return TRUE;
}

/*******************************************************************
�������ƣ�OpenNextFile(BOOL& bRemain)
��������������һ�������ļ�
�����������
���������BOOL& bRemain,�Ƿ����ļ�
�������ݣ��򿪳ɹ�����TRUE,���򷵻�FALSE
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
BOOL CCDRStatFileGroup::OpenNextFile(BOOL& bRemain)
{
    if (m_nCurFileNo >= m_FileNames.size() - 1)
    //�����ǰ�Ѿ����ļ����е����һ���ļ�����ֱ�ӷ���
    {
        bRemain = FALSE;
        return TRUE;
    }
    else
    //�������һ���ļ�����ˢ�µ�ǰλ��
    {
        //���ȹرյ�ǰ���ļ�
        BOOL bSucc = m_pCDRStatFile->Close();
        if (!bSucc)
        {
            return FALSE;
        }

        bRemain = TRUE;
        m_nCurFileNo++;

        char szFileName[MAX_PATH], szFileFullPath[MAX_PATH];
        strcpy(szFileName, m_FileNames[m_nCurFileNo].c_str());
        sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName);
        bSucc = m_pCDRStatFile->Open(szFileFullPath);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*******************************************************************
�������ƣ�TransQueryCond(const char* szCond,
                                const char* szBeginDate,
                                const char* szEndDate, char* szExCond)
�����������Ի�����ѯ�����������⴦��
          ���ڻ���̨�������Ĳ�ѯ�����У���ʼʱ��Ĳ�ѯ����ֻ����ʱ����
            �����ڸ�ʽ����ж��У���ʼʱ��ȴ����������ں�ʱ�������ԣ�
            ���뽫����̨���͵���Ϣ�еĿ�ʼ���ںͽ������ڷֱ���뵽��ʼʱ��
            �Ĳ�ѯ������ȥ����ʼʱ�����������Ϊ"StartTime"��ʱ�����ʽ��
            2001-11-05 21:00:03��ʾ���ӻ���̨���͵���Ϣ�е�������20011105
            ��ʾ���������ڵ���ʾ��ʽ���������Ӧ��ת�������磬�ӻ���̨���ص�
            ��ѯ�����У���ʼʱ��Ĳ�ѯ����Ϊ"StartTime >= 08:05:00"��
            "StartTime <= 18:05:00"����ʼ���ںͽ������ڷֱ�Ϊ"20011101"��
            "20011103"�����޸ĺ�Ŀ�ʼʱ���ѯ����Ϊ
            "StartTime >= 2001-11-01 08:05:00"��
            "StartTime <= 2001-11-03 18:05:00"

���������const char* szCond,�ӻ���̨�͹����Ĳ�ѯ����
          const char* szBeginDate,�ӻ���̨�͹�������ʼ����
          const char* szEndDate,�ӻ���̨�͹����Ľ�������
���������char* szExCond,�����Ĳ�ѯ�������ռ��ں�����������ͷ�
�������ݣ���
���ú�����
����������
����ʱ�䣺maimaoshi,2003/02/13
------------------------------�޸ļ�¼------------------------------
�޸���          �޸�ʱ��           �޸�����



********************************************************************/
void CCDRStatFileGroup::TransQueryCond(const char* szCond,
                                       const char* szBeginDate,
                                       const char* szEndDate,
                                       char* szExCond)
{
    //�޸Ŀ�ʼ���ںͽ������ڵĸ�ʽ
    char szExBeginDate[40], szExEndDate[40], szYear[10], szMon[10], szDay[10];
    memcpy(szYear, szBeginDate, 4);
    szYear[4] = '\0';
    memcpy(szMon, szBeginDate + 4, 2);
    szMon[2] = '\0';
    memcpy(szDay, szBeginDate + 6, 2);
    szDay[2] = '\0';
    sprintf(szExBeginDate, "%s-%s-%s ", szYear, szMon, szDay);
    memcpy(szYear, szEndDate, 4);
    szYear[4] = '\0';
    memcpy(szMon, szEndDate + 4, 2);
    szMon[2] = '\0';
    memcpy(szDay, szEndDate + 6, 2);
    szDay[2] = '\0';
    sprintf(szExEndDate, "%s-%s-%s ", szYear, szMon, szDay);

    //�޸Ĳ�ѯ����������ʼ���ںͽ������ڲ���

    char* pReadPos = (char* )szCond;    //�����ַ�������ʱ�Ķ�ָ��
    szExCond[0] = '\0';
    char* pWritePos = (char* )szExCond; //�����ַ�������ʱ��дָ��
    char* pSchPos = (char* )szCond;     //�����ַ����в��ҵ�ָ��

/*
    char cOperator;                     //StartTime�����������ӵĵ�һ���ַ�
    //��szCond�в������е�StartTime�����StartTime�������Ϊ>=��>��
    //�����������м���szExBeginDate���������szEndDate
    while (1)
    {
       pSchPos = (char* )strstr(pSchPos, "StartTime");
       if (NULL == pSchPos)
       {
           break;
       }
       else
       {
           pSchPos += 9;

           //����StartTime���������ӵĵ�һ���ַ�
           while (1)
           {
               if (' ' != *pSchPos)
               {
                   cOperator = *pSchPos;
                   break;
               }
               pSchPos++;
           }

           //����ʼ���ڻ�������ڵ���Ϣ���뵽��ѯ������
           pSchPos = strchr(pSchPos, '@');
           ASSERT(NULL != pSchPos);

           memcpy(pWritePos, pReadPos, pSchPos - pReadPos + 1);
           pWritePos += pSchPos - pReadPos + 1;
           pReadPos += pSchPos - pReadPos + 1;
           if ('>' == cOperator)
           {
               memcpy(pWritePos, szExBeginDate, strlen(szExBeginDate));
               pWritePos += strlen(szExBeginDate);
           }
           else
           {
               ASSERT('<' == cOperator);
               memcpy(pWritePos, szExEndDate, strlen(szExEndDate));
               pWritePos += strlen(szExEndDate);
           }
       }
    }
*/

    *pWritePos = '\0';
    STRNCAT(szExCond, pReadPos, MAX_COND_LEN);
}

