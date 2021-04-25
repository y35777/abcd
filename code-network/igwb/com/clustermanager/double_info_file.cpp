//���ļ�������Զ��2002.05.08����
#include "double_info_file.h"
#include "../frontsave/resource.h"
#include "../include/toolbox.h"


/**********************************************************************
��������    ���캯��
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
CDoubleInfoFile::CDoubleInfoFile()
{
	m_uHeadLen = 0;
}

/**********************************************************************
��������    ��������
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
CDoubleInfoFile::~CDoubleInfoFile()
{
	Close();
}


/**********************************************************************
��������    ��ʼ������
�������    szMainFilePath������״̬�ļ���·��
			szMainFileName������״̬�ļ����ļ���
			szBakFilePath������״̬�ļ���·��
			szBakFileName������״̬�ļ����ļ���
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CDoubleInfoFile::Init(const char* const szMainFilePath,
						   const char* const szMainFileName,
				           const char* const szBakFilePath,
						   const char* const szBakFileName)
{
	m_InfoMain.SetInfoFilePath(szMainFileName, szMainFilePath);
	m_InfoBak.SetInfoFilePath(szBakFileName, szBakFilePath);
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
void CDoubleInfoFile::SetFileStruct(const UINT4 uHeadLen, 
									const UINT4 uUnitLen, 
									const UINT4 uUnitNum)
{
	ASSERT(uHeadLen > 0);
	m_uHeadLen = uHeadLen;

	m_InfoMain.SetFileStruct(uHeadLen, uUnitLen, uUnitNum);
	m_InfoBak.SetFileStruct(uHeadLen, uUnitLen, uUnitNum);
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
BOOL CDoubleInfoFile::OpenInfoFile(BOOL bNeedCreate, BYTE* pDefaultHead)
{
	ASSERT(m_uHeadLen > 0);

	//���ȼ������״̬�ļ������

    BOOL bSucc;
    BOOL bStatusValid = FALSE;  //״̬�ļ��Ƿ���Ч�ı�־
    BOOL bMainExist = FALSE, bBakExist = FALSE; //����״̬�ļ��Ƿ����

    bMainExist = m_InfoMain.OpenInfoFile(FALSE);    //����״̬�ļ�
    bBakExist = m_InfoBak.OpenInfoFile(FALSE);      //�򿪱�״̬�ļ�
    if (bMainExist && bBakExist)    //����״̬�ļ�ͬʱ����
    {
        //����״̬�ļ�ͷ����Ч
        if (m_InfoMain.CheckHead() && m_InfoBak.CheckHead())
        {
            bStatusValid = TRUE;
        }
        //��״̬�ļ�ͷ��Ч������״̬�ļ�ͷ��Ч
        else if (m_InfoMain.CheckHead() && !m_InfoBak.CheckHead())
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_V_BAK_INV);

            bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            STRING szSrcFileFullPath = m_InfoMain.GetFileFullPath(); 
            STRING szDstFileFullPath = m_InfoBak.GetFileFullPath(); 
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //��״̬�ļ�ͷ��Ч������״̬�ļ�ͷ��Ч
        else if (!m_InfoMain.CheckHead() && m_InfoBak.CheckHead())
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INV_BAK_V);

            bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            STRING szSrcFileFullPath = m_InfoBak.GetFileFullPath(); 
            STRING szDstFileFullPath = m_InfoMain.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        else//����״̬�ļ�ͷ����Ч
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INV_BAK_INV);

            bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
    //��״̬�ļ����ڵ���״̬�ļ�������
    else if (bMainExist && !bBakExist)
    {
        //��״̬�ļ�ͷ��Ч
        if (m_InfoMain.CheckHead())
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_V_BAK_INE);

            STRING szSrcFileFullPath = m_InfoMain.GetFileFullPath();
            STRING szDstFileFullPath = m_InfoBak.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //��״̬�ļ�ͷ��Ч
        else
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INV_BAK_INE);

            bSucc = m_InfoMain.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
    //��״̬�ļ������ڵ���״̬�ļ�����
    else if (!bMainExist && bBakExist)
    {
        //��״̬�ļ�ͷ��Ч
        if (m_InfoBak.CheckHead())
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INE_BAK_V);

            STRING szSrcFileFullPath = m_InfoBak.GetFileFullPath();
            STRING szDstFileFullPath = m_InfoMain.GetFileFullPath();
            bSucc = 
               FileCopy(szSrcFileFullPath.c_str(), szDstFileFullPath.c_str());
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = TRUE;
        }
        //��״̬�ļ�ͷ��Ч
        else
        {
            TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INE_BAK_INV);

            bSucc = m_InfoBak.Delete();
            if (!bSucc)
            {
                return FALSE;
            }
            bStatusValid = FALSE;
        }
    }
    //����״̬�ļ���������
    else
    {
        TRACE(MTS_CLSTR, S_FS_TRC_MAIN_INE_BAK_INE);
        bStatusValid = FALSE;
    }

    //���״̬�ļ���Ч
    if (!bStatusValid)
    {
        bSucc = m_InfoMain.OpenInfoFile(bNeedCreate, pDefaultHead);
        if (!bSucc)
        {
            return FALSE;
        }
        bSucc = m_InfoBak.OpenInfoFile(bNeedCreate, pDefaultHead);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    bSucc = m_InfoMain.OpenInfoFile(FALSE);
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoBak.OpenInfoFile(FALSE);
    if (!bSucc)
    {
        return FALSE;
    }

    return TRUE;
}

/**********************************************************************
��������    �ر�״̬�ļ�
�������    ��
�������    ��
�� �� ֵ    �ر��ļ��Ƿ�ɹ�
                TRUE----�ر��ļ��ɹ� 
                FALSE----�ر��ļ����ɹ�
�׳��쳣    ��
**********************************************************************/
BOOL CDoubleInfoFile::Close()
{
	BOOL bSucc;
	bSucc = m_InfoMain.Close();
	if (!bSucc)
	{
		return FALSE;
	}
	bSucc = m_InfoBak.Close();

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
BOOL CDoubleInfoFile::Flush()
{
	BOOL bSucc;
	bSucc = m_InfoMain.Flush();
	if (!bSucc)
	{
		return FALSE;
	}
	bSucc = m_InfoBak.Flush();
	
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
**********************************************************************/
BOOL CDoubleInfoFile::WriteHead(const BYTE* pData, const UINT4 uLen)
{
    m_InfoMain.CalcCheckSum((BYTE*)pData);

	BOOL bSucc;
	bSucc = m_InfoMain.WriteHead(pData, uLen);
	if (!bSucc)
	{
		return FALSE;
	}
	bSucc = m_InfoBak.WriteHead(pData, uLen);

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
BOOL CDoubleInfoFile::ReadHead(BYTE* pData)
{
	return m_InfoBak.ReadHead(pData);
}
