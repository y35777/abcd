#include "../include/toolbox.h"
#include "bill_task_struct.h"
#include "scp_file_group.h"
#include "scp_bill_file.h"
#include "scp_rvy_task.h"
#include "../querybrowse/qb_comm.h"


/********************************************************
* ����: CScpFileGroup::CScpFileGroup
* ����: ���캯��
* ����: pFormat     : ��ʽ������ָ��
*       uBillType   : ԭʼ�����������ջ���
*       uAPType     : ���������
*       pScpRvyTask : CScpRvyTask�����ָ��
* ���: 
* ����: 
********************************************************/
CScpFileGroup::CScpFileGroup(CFormat *pFormat, 
                             UINT4 uBillType, 
                             UINT4 uAPType, 
                             CScpRvyTask* pScpRvyTask)
{
    m_pFormat    = pFormat;
    m_uBillType  = uBillType;
    m_uAPType    = uAPType;
    m_nCurFileNo = -1;
    m_nCurOffset = -1;

    m_pBillFile = new CScpBillFile(m_pFormat, m_uBillType, m_uAPType, pScpRvyTask);
    if(NULL == m_pBillFile) throw BR_SYSECPT;
}


/********************************************************
* ����: CScpFileGroup::~CScpFileGroup
* ����: ��������
* ����: 
* ���: 
* ����: 
********************************************************/
CScpFileGroup::~CScpFileGroup()
{
    if (NULL != m_pBillFile)
    {
        delete m_pBillFile;
    }
}

/**********************************************************************
��������    ��ָ���Ļ����ļ������������дλ��Ϊָ����λ��
�������    nFileNo----ָ���Ļ����ļ���� 
            nOffset----ָ���Ķ�дλ��
�������    ��
����ֵ      �򿪳ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CScpFileGroup::Open(int nFileNo, int nOffset)
{
    ASSERT(NULL != m_pBillFile);

    BOOL bSucc = FALSE;

    if (m_nCurFileNo == nFileNo)
    {
        bSucc = m_pBillFile->SetOffset(nOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }
    else
    {
        //�رյ�ǰ�ļ�����ָ���ļ���������дλ��
        bSucc = m_pBillFile->Close();
        if (!bSucc)
        {
            return FALSE;
        }

        STRING szFileName = m_qFileNames[nFileNo];

        char szFileFullPath[MAX_PATH];
        sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName.c_str());
        bSucc = m_pBillFile->Open(szFileFullPath, nOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    m_nCurFileNo = nFileNo;
    m_nCurOffset = nOffset;

    return TRUE;
}

//��ȡ��ǰ�򿪵Ļ����ļ���ź��䵱ǰ��дλ��
void CScpFileGroup::GetCurPosition(int& nFileNo, int& nOffset)
{
    nFileNo = m_nCurFileNo;
    nOffset = m_nCurOffset;
}

/**********************************************************************
��������    ��ȡ��һ�ŷ��������Ļ�����¼����
�������    ��
�������    pBillRecord---- ������¼����ָ��
            nStartFileNo----���Ż�����¼�������ڻ����ļ����
            nStartOffset----���Ż�����¼�����ڻ����ļ��е�λ��
            bRemain----�Ƿ��к��������ݣ�ΪFALSEʱ��pBillRecord��Ч
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CScpFileGroup::GetNextRecord(CBillRecord*& pBillRecord, 
                                 int& nStartFileNo, 
                                 int& nStartOffset,
                                 BOOL& bRemain)
{
    BOOL bRemainInFile;
    while (1)
    {
        BOOL bSucc = m_pBillFile->GetNextBill(pBillRecord, m_nCurOffset, 
                                              bRemainInFile);
        if (!bSucc)
        {
            return FALSE;
        }

        if (bRemainInFile)  //�ڵ�ǰ���ļ����ҵ��˻�����¼
        {
            nStartFileNo = m_nCurFileNo;
            nStartOffset = m_nCurOffset;
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
}

/**********************************************************************
��������    ׼�������ļ�������������
�������    szFileFullPath----����ȫ·���Ļ����ļ�����
�������    ��
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CScpFileGroup::PrepareFile(const char* szFileFullPath)
{
	m_pBillFile->SetQueryCond((char* )(""));

    //��ȫ·���л���ļ�·�����ļ���
    char* pPos = (char* )strrchr(szFileFullPath, '/');
    if (NULL == pPos)   //���û���ҵ�'/'������'\'����
    {
        pPos = (char* )strrchr(szFileFullPath, '\\');
        if (NULL == pPos)
        {
            return FALSE;
        }
    }
    memcpy(m_szPath, szFileFullPath, pPos - szFileFullPath);
    m_szPath[pPos - szFileFullPath] = '\0';
    STRING szFileName = (pPos + 1);

    m_qFileNames.push_back(szFileName);

    return TRUE;
}


/**********************************************************************
��������    ׼�������ļ�����Բ�ѯ����
�������    szPath----Ŀ¼·��
            szStartRange----��ѯ�Ŀ�ʼ��Χ��ȡֵ���������ڡ��򻰵��ļ���
            szEndRange----��ѯ������Χ��ȡֵ���������ڡ��򻰵��ļ���
            uFlg----��ѯ��Χ��־���������ֲ�ѯ��Χ������Ŀ¼���ǻ����ļ����ƣ�
                    ȡֵ��QueryRangeType�ﶨ��
            szQueryCond----��ѯ�����ַ���
�������    ��
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CScpFileGroup::PrepareFiles(const char* szPath, const char* szStartRange, 
                              const char* szEndRange, 
                              UINT4 uFlg, char* szQueryCond)
{
    strcpy(m_szPath, szPath);
    BOOL bSucc = m_pBillFile->SetQueryCond(szQueryCond);
    if (!bSucc)
    {
        return FALSE;
    }
    
    LIST<STRING> FileList;    //�����������ļ������б�
    if (DATE_RANGE == uFlg) 
    {
        //��ȡ�Ź������������Ŀ¼��Ȼ����еõ���Χ�ڵ���������Ŀ¼
        LIST<STRING> DateList;
        STRING szDateName;
        GetDateDir(m_szPath, DateList);
        while (1)     //��ȥ����Ŀ¼�б���ͷ����������������
        {
            if (DateList.empty())
            {
               break;
            }

            szDateName = DateList.front();
            if (0 == strcmp(szDateName.c_str(), szStartRange))
            {
                break;
            }
            else
            {
                DateList.pop_front();
            }
        }
        while (1)     //��ȥ����Ŀ¼�б���β����������������
        {
            if (DateList.empty())
            {
                break;
            }

            szDateName = DateList.back();
            if (0 == strcmp(szDateName.c_str(), szEndRange))
            {
                break;
            }
            else
            {
                DateList.pop_back();
            }
        }

        //����õ�������Ŀ¼�б�Ϊ�գ�˵����ʼ����Ŀ¼����ֹ����
        //Ŀ¼��Ӳ�����Ѿ������ڣ���ʱ����ʧ��
        if (DateList.empty())
        {
            return FALSE;
        }

        //��ȡ������Χ�ڵ�����Ŀ¼�µ������ļ���
        //�ļ�����ʽΪ����Ŀ¼/�ļ����ķ�ʽ
        char szFilePath[MAX_PATH];
        LIST<STRING> FileSubList;     //���һ������Ŀ¼�µ������ļ����б�
        LIST<STRING>::iterator i, j;
        for (i = DateList.begin(); i != DateList.end(); i++)
        {
            szDateName = *i;
            sprintf(szFilePath, "%s/%s", m_szPath, szDateName.c_str());
            GetFileList(szFilePath, FileSubList);

            //GetFileList���ص��б���ֻ�����ļ��������Ա��뽫����Ŀ¼��Ϣ����ȥ
            for (j = FileSubList.begin(); j != FileSubList.end(); j++)
            {
                *j = szDateName + "/" + *j;
            }

            //��FileSubList�е����ݼ��뵽FileList��ĩβ
            FileList.splice(FileList.end(), FileSubList);
            FileSubList.clear();
        }
		if (FileList.empty())
		{
			return FALSE;
		}
    }
    else
    {
        //��ȡĿ¼���Ź���������ļ���Ȼ����еõ���Χ�ڵ������ļ�
        GetFileList(m_szPath, FileList);
        STRING szFileName;
        while (1)     //��ȥ�ļ��б���ͷ����������������
        {
            if (FileList.empty())
            {
                break;
            }

            szFileName = FileList.front();
            if (0 == strcmp(szFileName.c_str(), szStartRange))
            {
                break;
            }
            else
            {
                FileList.pop_front();
            }
        }
        while (1)     //��ȥ�ļ��б���β����������������
        {
            if (FileList.empty())
            {
                break;
            }

            szFileName = FileList.back();
            if (0 == strcmp(szFileName.c_str(), szEndRange))
            {
                break;
            }
            else
            {
                FileList.pop_back();
            }
        }

        //����õ����ļ��б�Ϊ�գ�˵����ʼ�ļ�����ֹ�ļ� 
        //��Ӳ�����Ѿ������ڣ���ʱ����ʧ��
        if (FileList.empty())
        {
            return FALSE;
        }
    }

    //���õ����ļ��б��뵽m_qFileNames��
    LIST<STRING>::iterator i;
    for (i = FileList.begin(); i != FileList.end(); i++)
    {
        m_qFileNames.push_back(*i);
    }

    return TRUE;
}

/**********************************************************************
��������    ����һ�������ļ�
�������    ��
�������    bRemain----��ʾ�ļ������Ƿ��Ѿ�û���ļ����Դ��ˣ�����
                       ����ΪFALSEʱ��m_pBillFile��ֵ��Ч
����ֵ      �ɹ����
�׳��쳣    ��
**********************************************************************/
BOOL CScpFileGroup::OpenNextFile(BOOL& bRemain)
{
    if (m_nCurFileNo == m_qFileNames.size() - 1)
    //�����ǰ�Ѿ����ļ����е����һ���ļ�����ֱ�ӷ���
    {
        bRemain = FALSE;
        return TRUE;
    }
    else
    //�������һ���ļ�����ˢ�µ�ǰλ��
    {
		//���ȹرյ�ǰ���ļ�
		BOOL bSucc = m_pBillFile->Close();
		if (!bSucc)
		{
			return FALSE;
		}

        bRemain = TRUE;
        m_nCurFileNo++;
        m_nCurOffset = 0;

        char szFileName[MAX_PATH], szFileFullPath[MAX_PATH];
        strcpy(szFileName, m_qFileNames[m_nCurFileNo].c_str());
        sprintf(szFileFullPath, "%s/%s", m_szPath, szFileName);

        bSucc = m_pBillFile->Open(szFileFullPath, m_nCurOffset);
        if (!bSucc)
        {
            return FALSE;
        }
    }
    
    return TRUE;
}
