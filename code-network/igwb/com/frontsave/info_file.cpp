#include "info_file.h"
#include "../include/toolbox.h"

/**********************************************************************
��������    Ϊָ�����ļ�����ָ����С�Ŀռ�
�������    f----ָ�����ļ���Ӧ���ļ�ָ�룬�������´������ļ�
            nFileSize----Ҫ��������ļ��ռ�Ĵ�С
�������    ��
�� �� ֵ    ����ռ��Ƿ�ɹ� 
                TRUE----����ռ�ɹ� 
                FALSE----����ռ䲻�ɹ�
�׳��쳣    ��
**********************************************************************/
BOOL AskFileSpace(FILE* f, int nFileSize)
{
	ASSERT(NULL != f);

	if (nFileSize <= 0)
	{
		return TRUE;
	}

	//����Ҫ��Ŀռ��С�ƶ��ļ�ָ�룬�������һ��λ��д��һ������0��
	//�Ӷ�Ϊ�ļ����뵽ָ����С�Ŀռ�
	int nResult = fseek(f, nFileSize - 1, SEEK_SET);
	if (0 != nResult)
	{
		return FALSE;
	}
	BYTE byFileEnd = 0;
	size_t uWritten = fwrite(&byFileEnd, 1, 1, f);
	if (1 != uWritten)
	{
		return FALSE;
	}

	//���ļ�ָ���ƻص��ļ�ͷ
	nResult = fseek(f, 0, SEEK_SET);
	
	return (0 == nResult);
}

CInfoFile::CInfoFile()
{
    m_pInfoFile = NULL;
    m_szInfoFileName = "";
    m_szInfoFilePath = "";
    m_szBackupName = "";
    m_szBackupPath = "";
    m_uHeadLen = 0;
    m_uUnitLen = 0;
    m_uUnitNum = 0;
    m_uFileLen = 0;
    m_uInfoWriteIndex = 0; 
    m_nRotateCount = 0;
}

CInfoFile::~CInfoFile()
{
    Close();
}

//��ȡ״̬�ļ���ȫ·��
STRING CInfoFile::GetFileFullPath()
{
    return (m_szInfoFilePath + "/" + m_szInfoFileName);
}

UINT4 CInfoFile::GetUnitNum()
{
    return m_uUnitNum;
}

UINT4 CInfoFile::GetWriteIndex()
{
    return m_uInfoWriteIndex;
}

/**********************************************************************
��������    ����״̬�ļ����ʹ��·��
�������    szFileName----״̬�ļ��� 
            szFilePath----״̬�ļ����·��
�������    ��
�� �� ֵ    ��
�׳��쳣    ��
**********************************************************************/
void CInfoFile::SetInfoFilePath(const STRING& szFileName, 
                                       const STRING& szFilePath)
{
    m_szInfoFileName = szFileName;
    m_szInfoFilePath = szFilePath;
}

/**********************************************************************
��������    ����״̬�����ļ����ʹ��·��
�������    szFileName----�����ļ��� 
            szFilePath----�����ļ����·��
�������    ��
�� �� ֵ    ��
�׳��쳣    ��
**********************************************************************/
void CInfoFile::SetBackupFilePath(const STRING& szFileName, 
                                  const STRING& szFilePath)
{
    m_szBackupName = szFileName;
    m_szBackupPath = szFilePath;
}

/**********************************************************************
��������    �����ļ�ͷ���ȣ���¼��Ԫ���Ⱥͼ�¼��Ԫ����
�������    uHeadLen----�ļ�ͷ���� 
            uUnitLen----��¼��Ԫ���� 
            uUnitNum----��¼��Ԫ����
�������    ��
�� �� ֵ    ��
�׳��쳣    ��
**********************************************************************/
void CInfoFile::SetFileStruct(const UINT4 uHeadLen, const UINT4 uUnitLen,
                                     const UINT4 uUnitNum)
{
    m_uHeadLen = uHeadLen;
    m_uUnitLen = uUnitLen;
    m_uUnitNum = uUnitNum;

    //�������õĲ���������ļ��ĳ���
    m_uFileLen = m_uHeadLen + m_uUnitLen * m_uUnitNum;
}

/**********************************************************************
��������    ��ָ�����ļ�ͷ����У���
�������    pHead----Ҫ����У��ͼ����״̬�ļ�ͷ
�������    pHead----����У��ͼ�����״̬�ļ�ͷ
�� �� ֵ    ��
�׳��쳣    ��
**********************************************************************/
void CInfoFile::CalcCheckSum(BYTE* pHead)
{
    ASSERT(0 != m_uHeadLen);

    UINT4 uCheckSum = 0;
    UINT4* pUint4Head = (UINT4* )pHead;
   
    //����У��ͣ�״̬ͷ�г��˵�һ��UINT4��У������������UINT4�ĺ� 
    for (int i = 1; i < m_uHeadLen / sizeof(UINT4); i++)
    {
        uCheckSum += pUint4Head[i];
    }

    pUint4Head[0] = uCheckSum;        //��pHead��У�������и�ֵ��
}

/**********************************************************************
��������    ʹpHead�е�uStatusFileWritePos����һ��״̬��¼�ĳ���
�������    pHead----����ǰ��״̬ͷ���ռ��ں�����������ͷ�
�������    pHead----���º��״̬ͷ
�� �� ֵ    ��
�׳��쳣    ��
**********************************************************************/
void CInfoFile::IncreaseIndex(BYTE* pHead)
{
    ASSERT(NULL != pHead);

    //����pHead�е�uStatusFileWritePos��ֵ��Ȼ�����Ƿ�������
    SInfoHead* pInfoHead = (SInfoHead* )pHead;
    pInfoHead->uStatusFileWritePos += m_uUnitLen;
    if (pInfoHead->uStatusFileWritePos >= m_uFileLen)
    {
        pInfoHead->uStatusFileWritePos = m_uHeadLen;
		pInfoHead->uRotateCount++;
    }
}

/**********************************************************************
��������    ����������ָ����·�����ļ����ͳ��ȴ������״̬�ļ�
�������    bNeedCreate----���ļ�������ʱ���Ƿ���Ҫ���� 
            pDefaultHead----�ڴ������ļ�ʱҪд���ȱʡ״̬ͷ��
                �ڱ�ֵΪNULLʱ��ȱʡ״̬ͷ �г���uStatusFileWritePos��
                Ϊm_uHeadLen��uCheckSum���м����⣬����������Ϊ0��
                �����ֵ��ΪNULL������뱣֤��ָ���ڴ�ĳ���Ϊm_uHeadLen��
                �ڴ��ں�����������ͷ�
�������    ��
�� �� ֵ    �ļ����Ƿ�ɹ� 
                TRUE----�ļ��򿪳ɹ� 
                FALSE----�ļ��򿪲��ɹ�
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::OpenInfoFile(BOOL bNeedCreate, BYTE* pDefaultHead)
{
    BOOL bSucc = FALSE;    //���ڱ�ʾ��������Ƿ�ɹ�
    if (NULL != m_pInfoFile)
    {
        return TRUE;
    }

    BYTE* pInfoHead = new BYTE[m_uHeadLen];

    STRING szFullPath = m_szInfoFilePath + "/" + m_szInfoFileName;
    m_pInfoFile = fopen(szFullPath.c_str(), "rb+");

    if (NULL != m_pInfoFile)
    {
        bSucc = TRUE;
        bSucc = ReadHead(pInfoHead);
        if (!bSucc)
        {
            Close();
        }
    }
    else if (!bNeedCreate)
    {
        bSucc = FALSE;
    }
    else        //�ļ�������ʱ���������ļ�����д�ļ�ͷ
    {
        //�������ļ����ڴ������ļ�ʱ����m_uFileLen���ļ�����
        m_pInfoFile = fopen(szFullPath.c_str(), "wb+");
        if (NULL == m_pInfoFile)
        {
            bSucc = FALSE;
        }
        else if (!AskFileSpace(m_pInfoFile, m_uFileLen))  //Ԥ������ռ�
        {
            bSucc = FALSE;
            Delete();
        }
        else //�������ļ�������ռ�ɹ�
        {
            if (NULL == pDefaultHead)
            {
                memset(pInfoHead, 0, m_uHeadLen);   //��pInfoHead�������ڴ���0
                ((SInfoHead* )pInfoHead)->uStatusFileWritePos = m_uHeadLen;
                CalcCheckSum(pInfoHead);
            }
            else
            {
                memcpy(pInfoHead, pDefaultHead, m_uHeadLen);
            }

            bSucc = WriteHead(pInfoHead, m_uHeadLen);
            if (!bSucc)
            {
                Delete();
            }
			//����Զ2002.05.18�޸ģ����ⵥ��D13767
			else
			{
				Flush();
			}
			//����Զ2002.05.18�޸Ľ���
        }
    }

    if (bSucc)
    {
        //����״̬ͷ������������ر�����ֵ
        m_nRotateCount = ((SInfoHead* )pInfoHead)->uRotateCount;
        UINT4 uStatusWritePos = ((SInfoHead* )pInfoHead)->uStatusFileWritePos;
        m_uInfoWriteIndex = (uStatusWritePos - m_uHeadLen) / m_uUnitLen + 1;
    }

    delete[] pInfoHead;

    return bSucc;
}

//�ر�״̬�ļ�
BOOL CInfoFile::Close()
{
    if (NULL == m_pInfoFile)
    {
        return TRUE;
    }

    int nResult = fclose(m_pInfoFile);
    if (0 != nResult)
    {
        return FALSE;
    }

    m_pInfoFile = NULL;
    
    return TRUE;
}

/**********************************************************************
��������    ɾ��״̬�ļ�
�������    ��
�������    ��
�� �� ֵ    �����Ƿ�ɹ� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::Delete()
{
    BOOL bSucc = FALSE;

    bSucc = Close();
    if (!bSucc)
    {
        return FALSE;
    }

    STRING szFullPath = m_szInfoFilePath + "/" + m_szInfoFileName;
    bSucc = FileDel(szFullPath.c_str());

    return bSucc;
}

/**********************************************************************
��������    ��δд�뵽Ӳ�̵�״̬����д�뵽Ӳ����
�������    ��
�������    ��
�� �� ֵ    �����Ƿ�ɹ� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::Flush()
{
    ASSERT(NULL != m_pInfoFile);

    return (0 == fflush(m_pInfoFile));
}

/**********************************************************************
��������    �ú���ʵ�ֽ�״̬�ļ���������Ӧ�ı���Ŀ¼��
�������    ��
�������    ��
�� �� ֵ    �����Ƿ�ɹ� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::CopyFileForBackup()
{
    STRING szSrcFullPath = m_szInfoFilePath + "/" + m_szInfoFileName;
    STRING szDstFullPath = m_szBackupPath + "/" + m_szBackupName;

    BOOL bSucc = FileCopy(szSrcFullPath.c_str(), szDstFullPath.c_str());

    return bSucc;
}

/**********************************************************************
��������    �ú���ʵ��д�ļ�ͷ�Ĺ���
�������    pData----д������ָ�룬�����߸����ͷ� 
            uLen----д�����ݳ��ȣ��������m_uHeadLen��
�������    ��
�� �� ֵ    �����Ƿ�ɹ� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
��ע        �ڵ���ʱ�����Ҫͬʱ����WriteUnit��WriteHead��
            ��WriteUnitһ��������WriteHeadǰ����
**********************************************************************/
BOOL CInfoFile::WriteHead(const BYTE* pData, const UINT4 uLen)
{
    ASSERT(m_uHeadLen == uLen);

    BOOL bSucc = WriteInfo(0, pData, uLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //��Ϊ�ļ�ͷ�е�uStatusFileWritePos�����Ѿ����£�
    //���Ա���ݴ˸���m_uInfoWriteIndex��ֵ
    UINT4 uWritePos = ((SInfoHead* )pData)->uStatusFileWritePos;
    m_uInfoWriteIndex = (uWritePos - m_uHeadLen) / m_uUnitLen + 1;
    m_nRotateCount = ((SInfoHead* )pData)->uRotateCount;

    return bSucc;
}

/**********************************************************************
��������    д��m_uInfoWriteIndex����¼��Ԫ
�������    pData----д������ָ�룬�����߸����ͷ� 
            uLen----д�����ݳ��ȣ��������m_uUnitLen
�������    ��
�� �� ֵ    �����Ƿ�ɹ� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::WriteUnit(const BYTE* pData, const UINT4 uLen)
{
    ASSERT(uLen == m_uUnitLen);

    //����д�ļ�����ʼλ��
    UINT4 uWritePos = m_uHeadLen + m_uUnitLen * (m_uInfoWriteIndex - 1);    
    BOOL bSucc = WriteInfo(uWritePos, pData, uLen);

    return bSucc;
}

/**********************************************************************
��������    ����ʵ�ֶ�ȡ�ļ�ͷ�Ĺ���
�������    pData----��ȡ���ݻ�����ָ�룬�����߸���������ͷ�
�������    ��
�� �� ֵ    �����Ƿ�ɹ� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::ReadHead(BYTE* pData)
{
    BOOL bSucc = ReadInfo(0, pData, m_uHeadLen);
    return bSucc;
}

/**********************************************************************
��������    �ú���ʵ�ֶ�ȡָ���ļ�¼��Ԫ�Ĺ���
�������    uIndex----��ȡ��Ԫ���� 
            pData----��ȡ���ݻ�����ָ�룬�����߸���������ͷ�
�������    ��
�� �� ֵ    ����ֵ˵�����£�
            0----��ȡ�ɹ�
            1----��ȡʧ��
            <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ��
                ERR_NO_UNIT_EXIST----״̬�ļ�Ϊ�´������ļ�,��״̬��¼����
                ERR_INDEX_INVALID----Ҫ��ȡ��״̬��¼����Ų��Ϸ�
                ERR_FAIL_READ----��״̬��¼ʧ��
�׳��쳣    ��
**********************************************************************/
int CInfoFile::ReadUnit(UINT4 uIndex, BYTE* pData)
{
    //�����ж�uIndex�ĺϷ���
    if ((1 == m_uInfoWriteIndex) && (0 == m_nRotateCount))
    //�����ļ�����û��д��״̬��¼
    {
        return ERR_NO_UNIT_EXIST;
    }
    else if (((uIndex >= m_uInfoWriteIndex) && (0 == m_nRotateCount))
              || (uIndex > m_uUnitNum))
    //Ҫ��ȡ�ļ�¼����Ч�ļ�¼
    {
        return ERR_INDEX_INVALID;
    }

    //�������������ʼλ��,����ȡ��Ӧ������
    UINT4 uReadPos = m_uHeadLen + m_uUnitLen * (uIndex - 1);
    BOOL bSucc = ReadInfo(uReadPos, pData, m_uUnitLen);
    if (!bSucc)
    {
        return ERR_FAIL_READ;
    }

    return ERR_SUCCESS;
}

/**********************************************************************
��������    �ú���ʵ�ֶ�ȡ���һ����¼��Ԫ�Ĺ���
�������    pData----��ȡ���ݻ�����ָ�룬�����߸���������ͷ�
�������    ��
�� �� ֵ    ����ֵ˵�����£�
            0----��ȡ�ɹ�
            1----��ȡʧ��
            <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ��
                ERR_NO_UNIT_EXIST----״̬�ļ�Ϊ�´������ļ�,��״̬��¼����
                ERR_INDEX_INVALID----Ҫ��ȡ��״̬��¼����Ų��Ϸ�
                ERR_FAIL_READ----��״̬��¼ʧ��
�׳��쳣    ��
**********************************************************************/
int CInfoFile::ReadLastUnit(BYTE* pData)
{
    //�������һ��״̬��¼�����ֵ
    int nLastIndex;
    nLastIndex = m_uInfoWriteIndex - 1;
    if (nLastIndex <= 0)
    {
        if (m_nRotateCount > 0)
        {
            nLastIndex = m_uUnitNum;
        }
        else
        {
            nLastIndex = 0;
        }
    }

    return ReadUnit(nLastIndex, pData);
}

/**********************************************************************
��������    У���ļ�ͷ�Ƿ���Ч
�������    ��
�������    ��
�� �� ֵ    �ļ�ͷ�Ƿ���Ч�� 
                TRUE----�ļ�ͷ��Ч 
                FALSE----�ļ�ͷ��Ч
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::CheckHead()
{
    ASSERT(NULL != m_pInfoFile);

    BYTE* pHead = new BYTE[m_uHeadLen];
    BOOL bSucc = ReadHead(pHead);     //��ȡ״̬ͷ
    if (!bSucc)
    {
        delete[] pHead;
        return FALSE;
    }
    UINT4 uOldCheckSum = ((SInfoHead* )pHead)->uCheckSum;
    CalcCheckSum(pHead);
    UINT4 uNewCheckSum = ((SInfoHead* )pHead)->uCheckSum;
    delete[] pHead;
    return (uOldCheckSum == uNewCheckSum);
}

/**********************************************************************
��������    �ṩ״̬�ļ�д��Ľӿ�
�������    uOffset----����д����ʼλ�� 
            pData----д������ָ�룬�����߸���������ͷ� 
            uLen----д�����ݳ���
�������    ��
�� �� ֵ    �����Ƿ�ɹ� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::WriteInfo(const UINT4 uOffset, const BYTE* pData, 
                          const UINT4 uLen)
{
    ASSERT(NULL != m_pInfoFile);

    //��λ�ļ�дָ��
    int nResult = fseek(m_pInfoFile, uOffset, SEEK_SET);
    if (0 != nResult)
    {
        return FALSE;
    }

    //���ļ�д����
    size_t uLenWritten = fwrite((void* )pData, uLen, 1, m_pInfoFile);

    return (1 == uLenWritten);
}

/**********************************************************************
��������    �ṩ��ȡ״̬�ļ����ݵĽӿ�
�������    uOffset----��ȡ������ʼλ�� 
            pData----��ȡ���ݻ�����ָ�룬�����߸���������ͷ� 
            uLen----��ȡ���ݳ���
�������    ��
�� �� ֵ    �����Ƿ�ɹ� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CInfoFile::ReadInfo(const UINT4 uOffset, const BYTE* pData, 
                         const UINT4 uLen)
{
    ASSERT(NULL != m_pInfoFile);

    //��λ�ļ���ָ��
    int nResult = fseek(m_pInfoFile, uOffset, SEEK_SET);
    if (0 != nResult)
    {
        return FALSE;
    }

    //��״̬�ļ���ȡ����
    size_t uLenRead = fread((void* )pData, uLen, 1, m_pInfoFile); 

    return (1 == uLenRead);
}
