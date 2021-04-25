#include "save.h"
#include "../include/toolbox.h"
#include <time.h>
#include <stdio.h>
#include "resource.h"

CSaveTransaction::CSaveTransaction()
{
    m_pHead = NULL;
    m_uHeadLen = 0;
    m_pLastInfoUnit = NULL;
    m_uUnitLen = 0;
    m_pBillFile = NULL;
/* Begin: 	Adder: Gan Zhiliang   Date: 2003-4-23*/
	m_uResFileProductTime = 0;
/* End  : Adder: Gan Zhiliang	   Date: 2003-4-23*/

    //��ע����Ϊ��һ�������У�ÿ��localtime���÷��ص�tm*��ָ��ͬһ���ڴ棬
    //�������������localtime���ã����һ�εĵ��ý�����ǰһ�ε�ִ�н��,
    //�������Ҫ���淵��ֵ�����뽫localtime�ķ���ֵ���⿽�����������ڴ���ȥ
    //Ϊ�˱�֤�ڳ����һ������û��״̬�ļ�������£��ܹ�������������Ŀ¼��
    //���Ա��뽫m_tmLastFileTime��m_tmCreateTime����ֱ�ֵΪ0��1��
    //�Ա�֤���ߵ�ʱ�䲻һ�£��Һ͵�ǰ��ݲ�һ�£��Ӷ��ᴴ���µ�����Ŀ¼
    m_tmToday.tm_year = 0;
    m_tmCreateTime.tm_year = 1;
    m_tmLastFileTime.tm_year = 0;

	m_sCurFileFullPath = "";
    m_bAlwaysGenFile = FALSE;  //Added by ZhengYuqun 2004-01-17 SWPD03357
}

CSaveTransaction::~CSaveTransaction()
{
    if (NULL != m_pHead)
    {
        delete[] m_pHead;
    }
    if (NULL != m_pLastInfoUnit)
    {
        delete[] m_pLastInfoUnit;
    }

	if(m_FileList.empty() == true)
    {
        return;
    }

    LISTFILELISTNODE::iterator iter = m_FileList.begin();
    while(iter != m_FileList.end())
    { 
        if(*iter != NULL)
        {
            delete *iter;
        }
        iter++;
    }
    m_FileList.clear();
}

UINT4 CSaveTransaction::GetCurCsn(BOOL bDisplay)
{
	//����������û���ʾ���������ǰ���ļ����кŲ��Ϸ��Ļ���һ����ʾΪ0
	if (bDisplay 
		  && ((m_uCurCsn > m_uFileCsnEnd) || (m_uCurCsn < m_uFileCsnStart)))
	{
		return 0;
	}

    return m_uCurCsn;
}

/**********************************************************************
��������    �����ʼ��ʱ�Ĺ���
�������    ��
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ� 
                TRUE----��ʼ���ɹ� 
                FALSE----��ʼ��ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveTransaction::Init()
{
    //��ȡ��ǰʱ�䣬���ݴ˳�ʼ��m_tmToday
    //��ע����Ϊ��һ�������У�ÿ��localtime���÷��ص�tm*��ָ��ͬһ���ڴ棬
    //�������������localtime���ã����һ�εĵ��ý�����ǰһ�ε�ִ�н��,
    //���Ա��뽫localtime�ķ���ֵ���⿽�����������ڴ���ȥ
    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCur = localtime(&tCurrent);
    m_tmToday = *ptmCur;

    //����״̬�ļ�У�鲢����״̬�ļ����б����ĳ�ʼ��
    BOOL bSucc = StartCheck();
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_CHECK_STATUS);
        return FALSE;
    }
    //����״̬�ļ��ͱ�״̬�ļ�
    bSucc = m_InfoMain.OpenInfoFile(FALSE);
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_OPEN_MAIN);
        return FALSE;
    }
    bSucc = m_InfoBak.OpenInfoFile(FALSE);
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_OPEN_BAK);
        return FALSE;
    }

    bSucc = InitByStatusFile();
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_INITBYSTATUS);
        return FALSE;
    }
/* Begin: 	Adder: Gan Zhiliang   Date: 2003-4-23*/
    if(0 == m_uResFileProductTime)
	{
		bSucc = CloseCurFile();
	}
/* End  : Adder: Gan Zhiliang	   Date: 2003-4-23*/
    if (!bSucc)
    {
		TRACE(m_szModuName, S_FS_TRC_FAIL_CLOSE_BILL);
        return FALSE;
    }
    
    m_uLastFlushPID = m_uPID;

    return TRUE;
}

/**********************************************************************
��������    ���¼���������
�������    ��
�������    ��
����ֵ      �����Ƿ�ɹ�
�׳��쳣    ��
**********************************************************************/
BOOL CSaveTransaction::Timer1Sec()
{
    //��ȡ��ǰʱ��
    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCurTime = localtime(&tCurrent);
    
    //��������˿��죬��رյ�ǰ�Ļ����ļ���������״̬�ļ�
    if ((ptmCurTime->tm_year != m_tmToday.tm_year)
        || (ptmCurTime->tm_mon != m_tmToday.tm_mon)
		|| (ptmCurTime->tm_mday != m_tmToday.tm_mday))
    {
        /* Begin: 	Adder: Gan Zhiliang   Date: 2003-4-23*/
		if(0 == m_uResFileProductTime)
		{
			BOOL bSucc = CloseCurFile();
			if (!bSucc)
			{				
				return FALSE;
			}
            //Added by ZhengYuqun ���������ļ���ʱ��Ҳ��Ҫ����ر� SWPD04389 2004-03-26
            else if(m_bAlwaysGenFile)
            {
                CreateNewFile();
            }			
		}
       /* End  : Adder: Gan Zhiliang	   Date: 2003-4-23*/
		

        m_InfoMain.CopyFileForBackup();
        m_tmToday = *ptmCurTime;
    }

	return TRUE;
}

/**********************************************************************
��������    ʵ�ֻ���������Ĵ����߼��� 
            �ô�������Ϊ�� 
                1����״̬��Ϣд����״̬�ļ��� 
                2�� ��������д�뻰���ļ��У� 
                3����״̬��Ϣд�뱸״̬�ļ��� 
            ������������һ��������������м�ĳһ��������ô����Ϊʧ��.
            Ϊ�˰�ȫ�Կ��ǣ�������д״̬�ļ�������һ��Ҫ����״̬�ļ���
            Flush������ͬʱ�����ļ�Ҳ�������Flush����������Ϊ��Ч�ʿ��ǣ�
            ����ÿ����һ��������Ҫ����״̬�ļ���ˢ�£������ڱ�����ָ����
            ���Ļ��������һ��״̬���£�ǰ���̱���ÿ����һ����������һ��
            ״̬�ļ����£�����ָ���ĸ��¼����������Ϊ1��
�������    pPacketInfo----�������������Ϣ��������������������Խ��ͣ�
                �ں����⸺��������ͷ�
�������    uWriteTime----д�����ļ�ʱ�䣬��λms�����ڼƷ����ص�ͳ�ƹ���
����ֵ      ����ֵ˵�����£� 
            0----��ȡ�ɹ� 
            1----��ȡʧ�� 
            <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ�� 
                ERR_WRITE_MAIN_STATUS_FAIL----д��״̬�ļ�ʧ�� 
                ERR_WRITE_BAK_STATUS_FAIL----д��״̬�ļ�ʧ�� 
                ERR_WRITE_BILLFILE_FAIL----д�����ļ�ʧ�� 
                ERR_CREATE_BILLFILE_FAIL----�����»����ļ�ʧ��
                ERR_CLOSE_BILLFILE_FAIL----�رյ�ǰ�����ļ�ʧ��
�׳��쳣    ��
**********************************************************************/
int CSaveTransaction::WritePacket(BYTE* pPacketInfo, UINT4& uWriteTime)
{
    ACE_Profile_Timer timer;   //���ڽ�������ͳ�ƣ���ȷ������
    timer.start();

    BOOL bSucc = FALSE;
	BOOL bCreateNewFile = FALSE;  //�Ƿ��´����˻����ļ�

    //�����ǰû�л����ļ����򿪣��һ������ǿգ��򴴽��µĻ����ļ�
    if ((NULL == m_pBillFile) && !IsEmptyPacket(pPacketInfo))
    {
		bCreateNewFile = TRUE;
        bSucc = CreateNewFile(); //�����µĻ����ļ�
        if (!bSucc)
        {			
			m_pBillFile = NULL;
            return ERR_CREATE_BILLFILE_FAIL;
        }
    }

    RefreshStatus(pPacketInfo);

    //�ж��Ƿ���Ҫ����Flush��������д��ָ����Ŀ�Ļ������󣬱������Flush������
	//���⣬�ڴ������ļ���ĵ�һ��Ҳ�������Flush������
	//�������˳�ʱ�����γɿջ����ļ�
    BOOL bFlush = FALSE;
    if (bCreateNewFile || ((m_uPID - m_uLastFlushPID) >= m_uMaxFlushSpan))
    {
        bFlush = TRUE;
        m_uLastFlushPID = m_uPID;
    }
    else
    {
        bFlush = FALSE;
    }


    //д��״̬�ļ�
    bSucc = WriteStatusFile(&m_InfoMain, bFlush);
    if (!bSucc)
    {
		return ERR_WRITE_MAIN_STATUS_FAIL;
    }

    //д�����ļ�
    int nResult = WriteBillFile(pPacketInfo, bFlush);
    if (0 != nResult)
    {		
		return nResult;
    }

    //д��״̬�ļ�
    bSucc = WriteStatusFile(&m_InfoBak, bFlush);
    if (!bSucc)
    {
		return ERR_WRITE_BAK_STATUS_FAIL;
    }

    //�����ǰ�����ļ��Ѿ������������رյ�ǰ�����ļ�
/* Begin: 	Modifier: Gan Zhiliang   Date: 2003-4-23*/
    //if ((NULL != m_pBillFile) && (ftell(m_pBillFile) > m_uFileLen))
	if ((NULL != m_pBillFile) && (ftell(m_pBillFile) > m_uFileLen) 
        && (0 == m_uResFileProductTime) && !m_bAlwaysGenFile)  //�������һ������ ZhengYuqun 2004-01-17 SWPD03357
/* End  : Modifier: Gan Zhiliang		   Date: 2003-4-23*/
    {
        bSucc = CloseCurFile();
        if (!bSucc)
        {			
			return ERR_CLOSE_BILLFILE_FAIL;
        }
    }

    //���㱣�滰���������ѵ�ʱ��
    timer.stop();
    ACE_Profile_Timer::ACE_Elapsed_Time et;
    timer.elapsed_time(et);
    uWriteTime = et.real_time;
    
    return ERR_SUCCESS;
}

/**********************************************************************
��������    ������һ���»����ļ����ļ����к�
�������    ��
�������    ��
����ֵ      ��
�׳��쳣    ��
**********************************************************************/
void CSaveTransaction::CalcNextCsn()
{
    m_uCurCsn += m_uFileCsnStep;
    if ((m_uCurCsn > m_uFileCsnEnd) || (m_uCurCsn < m_uFileCsnStart))
    {
        m_uCurCsn = m_uFileCsnStart;
    }
}

/**********************************************************************
��������    �õ���һ�������ļ����ļ���
�������    ��
�������    ��
����ֵ      �� 
�׳��쳣    ��
**********************************************************************/
void CSaveTransaction::GetNextFileName()
{
    //�����һ�������ļ����ļ����кŵ��ַ���
    CalcNextCsn();
    STRING szFileCsn = GetFileCsnStr(m_uCurCsn);

    if (strlen(m_szPostfix) > 0)
    {
        m_szFileName = m_szPrefix + szFileCsn + "." + m_szPostfix;
    }
    else
    {
        m_szFileName = m_szPrefix + szFileCsn;
    }
}

/**********************************************************************
��������    �����µĻ����ļ��Լ���ص�״̬�ļ�ˢ�²�����
            �ڻ�����ʵ�ִ������ļ��Ĳ������������и���ʵ��״̬�ļ���ˢ��
�������    ��
�������    ��
����ֵ      �������ļ��Ƿ�ɹ��� 
                TRUE----�����ɹ� 
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveTransaction::CreateNewFile()
{
    //�������ļ�������ʱ��������ָ���Ŀռ�
    GetNextFileName();
    GetNextFilePath();
    m_sCurFileFullPath = m_szFilePath + "/" + m_szFileName;
    m_pBillFile = fopen(m_sCurFileFullPath.c_str(), "wb+");
    if (NULL == m_pBillFile)
    {
        return FALSE;
    }
    BOOL bSucc = AskFileSpace(m_pBillFile, m_uFileLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //����m_tmCreateTime��m_tmLastFileTime��ֵ
    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCurTime = localtime(&tCurrent);
    m_tmLastFileTime = m_tmCreateTime;
    m_tmCreateTime = *ptmCurTime;

    return TRUE;
}

/**********************************************************************
��������    �رյ�ǰ���ڲ����Ļ����ļ������ļ�������ʱ�䳬ʱ�����졢
            �����ָ�������±�����
�������    ��
�������    ��
����ֵ      �رջ����ļ��Ƿ�ɹ�
                TRUE----д�ɹ�
                FALSE----дʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveTransaction::CloseCurFile()
{
    if (NULL != m_pBillFile)
    {
        ASSERT(FileSize(m_pBillFile) >= 0);

        m_uLastFlushPID = m_uPID;
        int nResult = fclose(m_pBillFile);
        if (0 != nResult)
        {
            return FALSE;
        }
        m_pBillFile = NULL;

        BOOL bSucc = DoWhenFileClose();
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/**********************************************************************
��������    ��һ������д�뵽��ǰ�Ļ����ļ���
�������    pPacket----������ָ�룬�ں�����������ͷ� 
            uPacketLen----����������
�������    ��
����ֵ      д�����ļ��Ƿ�ɹ� 
                TRUE----д�ɹ� 
                FALSE----дʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveTransaction::WritePacketData(BYTE* pPacket, UINT4 uPacketLen)
{
    if (0 != uPacketLen)  //���ǿհ�
    {
        //д����������
        size_t uLenWritten = fwrite(pPacket, uPacketLen, 1, m_pBillFile);
        return (1 == uLenWritten);
    }

    return TRUE;
}

/**********************************************************************
��������    ������״̬�ļ�����У�鹤������Ӧ������������������״̬����
            �����ļ�����ģ������ʱ���á�
�������    ��
�������    ��
����ֵ      �����Ƿ�ɹ�
                TRUE----�����ɹ�
                FALSE----����ʧ��
�׳��쳣    ��
**********************************************************************/
BOOL CSaveTransaction::StartCheck()
{
    BOOL bSucc;
    BOOL bStatusValid = FALSE;  //״̬�ļ��Ƿ���Ч�ı�־
    BOOL bMainExist = FALSE, bBakExist = FALSE; //����״̬�ļ��Ƿ����

    bMainExist = m_InfoMain.OpenInfoFile(FALSE);    //����״̬�ļ�
    bBakExist = m_InfoBak.OpenInfoFile(FALSE);      //�򿪱�״̬�ļ�

	//����Զ2002.05.18�޸ģ����ⵥ��D13767
    if (bMainExist && bBakExist)    //����״̬�ļ�ͬʱ����
    {
        //����״̬�ļ�ͷ����Ч
        if (m_InfoMain.CheckHead() && m_InfoBak.CheckHead())
        {
            TRACE(m_szModuName, S_FS_TRC_BOTH_VALID);

			int nResult;
            BYTE* pMainHead = new BYTE[m_uHeadLen];
			memset(pMainHead, 0, m_uHeadLen);
            BYTE* pBakHead = new BYTE[m_uHeadLen];
			memset(pBakHead, 0, m_uHeadLen);
			BYTE* pMainLastUnit = new BYTE[m_uUnitLen];
			memset(pMainLastUnit, 0, m_uUnitLen);
			BYTE* pBakLastUnit = new BYTE[m_uUnitLen];
			memset(pBakLastUnit, 0, m_uUnitLen);
            bSucc = m_InfoMain.ReadHead(pMainHead);
            if (!bSucc)
            {
                delete[] pMainHead;
                delete[] pBakHead;
				delete[] pMainLastUnit;
				delete[] pBakLastUnit;
                return FALSE;
            }
            bSucc = m_InfoBak.ReadHead(pBakHead);
            if (!bSucc)
            {
                delete[] pMainHead;
                delete[] pBakHead;
				delete[] pMainLastUnit;
				delete[] pBakLastUnit;
                return FALSE;
            }
            nResult = m_InfoMain.ReadLastUnit(pMainLastUnit);
            if ((ERR_INDEX_INVALID == nResult) || (ERR_FAIL_READ == nResult))
            {
                delete[] pMainHead;
                delete[] pBakHead;
				delete[] pMainLastUnit;
				delete[] pBakLastUnit;
                return FALSE;
            }
            nResult = m_InfoBak.ReadLastUnit(pBakLastUnit);
            if ((ERR_INDEX_INVALID == nResult) || (ERR_FAIL_READ == nResult))
            {
                delete[] pMainHead;
                delete[] pBakHead;
				delete[] pMainLastUnit;
				delete[] pBakLastUnit;
                return FALSE;
            }

            //����״̬�ļ�ͷ����Ч�����໥�䲻һ��
            if ((0 != memcmp(pMainLastUnit, pBakLastUnit, m_uUnitLen)) 
				|| (0 != memcmp(pMainHead, pBakHead, m_uHeadLen)))
            {
                TRACE(m_szModuName, S_FS_TRC_MAIN_DIFF_BAK);

                bSucc = DoWithMainDiffBak(pMainHead, pBakHead, 
										  pMainLastUnit, pBakLastUnit);
                if (!bSucc)
                {
                    delete[] pMainHead;
                    delete[] pBakHead;
					delete[] pMainLastUnit;
					delete[] pBakLastUnit;
                    return FALSE;
                }
            }
            else
            {
                TRACE(m_szModuName, S_FS_TRC_MAIN_SAME_BAK);
            }

            delete[] pMainHead;
            delete[] pBakHead;
			delete[] pMainLastUnit;
			delete[] pBakLastUnit;
            bStatusValid = TRUE;
        }
        //��״̬�ļ�ͷ��Ч������״̬�ļ�ͷ��Ч
        else if (m_InfoMain.CheckHead() && !m_InfoBak.CheckHead())
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_V_BAK_INV);

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
            TRACE(m_szModuName, S_FS_TRC_MAIN_INV_BAK_V);

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
            TRACE(m_szModuName, S_FS_TRC_MAIN_INV_BAK_INV);

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
	//����Զ2002.05.18�޸Ľ���

    //��״̬�ļ����ڵ���״̬�ļ�������
    else if (bMainExist && !bBakExist)
    {
        //��״̬�ļ�ͷ��Ч
        if (m_InfoMain.CheckHead())
        {
            TRACE(m_szModuName, S_FS_TRC_MAIN_V_BAK_INE);

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
            TRACE(m_szModuName, S_FS_TRC_MAIN_INV_BAK_INE);

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
            TRACE(m_szModuName, S_FS_TRC_MAIN_INE_BAK_V);

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
            TRACE(m_szModuName, S_FS_TRC_MAIN_INE_BAK_INV);

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
        TRACE(m_szModuName, S_FS_TRC_MAIN_INE_BAK_INE);
        bStatusValid = FALSE;
    }

    //���״̬�ļ���Ч���򴴽��µ�����״̬�ļ���ȱʡ���ļ�ͷ�����m_pHead��
    if (!bStatusValid)
    {
        bSucc = m_InfoMain.OpenInfoFile(TRUE, m_pHead);
        if (!bSucc)
        {
            return FALSE;
        }
        bSucc = m_InfoBak.OpenInfoFile(TRUE, m_pHead);
        if (!bSucc)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//�������кŵõ��ļ����кŵ��ַ���
STRING CSaveTransaction::GetFileCsnStr(UINT4 uCsn)
{
    char szFormat[100], szFileCsn[100];

    SNPRINTF(szFormat, 100, "%%0%dd", m_nCsnLen);
    szFormat[99] = 0;
    SNPRINTF(szFileCsn, 100, szFormat, uCsn);
    szFileCsn[99] = 0;

    return szFileCsn;
}

/**************************************************************************
����ԭ��    protected: int DelOneDirBillFiles(char* szPath);
��������    ɾ��ָ��Ŀ¼�¹��ڻ����ļ�������
�������    szPath: ��ɾ���Ļ����ļ�Ŀ¼
�������    ��
����ֵ      ɾ�����ڻ����ļ��Ƿ�ɹ�������ɹ�����ERR_SUCCESS�����򷵻ط���
�׳��쳣    ��
**************************************************************************/
int CSaveTransaction::DelOneDirBillFiles(char* szPath)
{
    STRING sFileNameDeleted = "";
    STRING szLog;
    
    //ת����ʱ��ṹ
    //��ȡ��ǰʱ��
    time_t tCurDate,tDirDate;
    time(&tCurDate);
    
    //�õ�Ŀ¼ʱ��
    tm tmDirDate;
    memset(&tmDirDate,0,sizeof(tm));
    STRING szDirDate(szPath);
    int nLen = szDirDate.size();
	
	STRING sYear = szDirDate.substr(nLen-8,4);
    STRING sMon = szDirDate.substr(nLen-4,2);
    STRING sDay = szDirDate.substr(nLen-2,2);	

    tmDirDate.tm_year = atoi(sYear.c_str()) - 1900;
    tmDirDate.tm_mon = atoi(sMon.c_str()) - 1;
    tmDirDate.tm_mday = atoi(sDay.c_str());

    tDirDate = mktime(&tmDirDate);

    // ��ǰ����������Ŀ¼������֮��С�ڻ����ļ���������
    if(difftime(tCurDate,tDirDate) < (m_uBillSavedDays * 24 * 60 * 60))
    {
        return ERR_FAIL;
    }
    
    PFilesListNode pCurFile;
    STRING szCurFile;
    LISTFILELISTNODE::iterator iter = m_FileList.begin();
    while(iter != m_FileList.end())
    {
        pCurFile = *iter;
        if(pCurFile == NULL) 
        {
            iter = m_FileList.erase(iter);
            continue;
        }
        
        szCurFile = szPath;
        szCurFile = MakePath(szCurFile, pCurFile->szFileName);
        
        //ɾ����ǰ�ļ�
        if(FileDel(szCurFile.c_str()) != TRUE)
        {
            if(sFileNameDeleted.empty() != true)
            {
                szLog += sFileNameDeleted;
                CWorkModule::WriteLog(m_szModuNameEx, szLog.c_str());
                sFileNameDeleted = "";
            }

            char szTmpBuf[500];
            sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
            //by ldf 2004-01-29 �����Mesgoutû��ʲô���壬��Դ�ļ�Ҳδ���壬ȥ��֮
            //STRING sTmpInfo = S_DELETE_BILL_FILE_FAILED;
            //sTmpInfo += szCurFile;
            //CWorkModule::MessageOut(m_szModuNameEx, MSGOUT_LEVEL_SIDLY, sTmpInfo.c_str());
            //by ldf 2004-01-28 ��core�Ķ�λ�޸�
            STRING sTmpInfo =szCurFile;
            sTmpInfo += ',';
            //end
            sTmpInfo += szTmpBuf;
            TRACE(m_szModuNameEx, sTmpInfo.c_str());

            iter++;
            continue;
        }
        
        if(sFileNameDeleted.empty() == true)
        {
            szLog = m_szModuNameEx;
			szLog += " Deleted ";
            szLog += szCurFile;
            szLog += " ~ ";
        }
        else 
        {
            int nPos1;
            int nPos2;
            STRING sTmpCurFile = pCurFile->szFileName;
            nPos2 = sFileNameDeleted.find_last_of(CHAR_DIR);
            STRING sTmpFileDel = sFileNameDeleted.substr(nPos2+1);
            nPos1 = sTmpCurFile.find(".");
            if(nPos1 == -1)
            {
                nPos1 = sTmpCurFile.length();
            }
            nPos2 = sTmpFileDel.find(".");
            if(nPos2 == -1)
            {
                nPos2 = sTmpFileDel.length();
            }
            STRING sCurFileBack4Int = (sTmpCurFile.substr(nPos1 - 4, 4)).c_str();
            STRING sFileDeletedBack4Int = (sTmpFileDel.substr(nPos2-4, 4)).c_str();

            if(atoi(sCurFileBack4Int.c_str()) - atoi(sFileDeletedBack4Int.c_str()) != 1)
            {
                szLog += sFileNameDeleted;
                CWorkModule::WriteLog(m_szModuNameEx, szLog.c_str());

                szLog = m_szModuNameEx;
				szLog += " Deleted ";
                szLog += szCurFile;
                szLog += " ~ ";
            }
        }

        sFileNameDeleted = pCurFile->szFileName;
        iter++;
    }
    
    if(sFileNameDeleted.empty() != true)
    {
        szLog += sFileNameDeleted;
        CWorkModule::WriteLog(m_szModuNameEx, szLog.c_str());
        sFileNameDeleted = "";
    }
    
    SweepDirectory(szPath);    
    return ERR_SUCCESS;
}

STRING CSaveTransaction::MakePath(const STRING &path, const char *filename)
{
    if(filename == NULL)
    {
        return STRING("");
    }

    char szTmp[MAX_PATH];
    if( *((path).end() - 1) != CHAR_DIR )
    {
        SNPRINTF(szTmp,MAX_PATH,"%s/%s", path.c_str(),filename);
        szTmp[MAX_PATH - 1] = 0;
    }

    else
    {
        SNPRINTF(szTmp,MAX_PATH,"%s%s", path.c_str(),filename);
        szTmp[MAX_PATH - 1] = 0;
    }
    return STRING(szTmp);
}
/**************************************************************************
����ԭ��    protected: virtual int PrepareFiles(char* szPath, 
                                                time_t tFileTime);
��������    ׼���ļ��б�
�������    szPath: �����ļ���·��
            tFileTime: �ϴ��ѳɹ����ݵĻ����ļ����޸�ʱ��
�������    ��
����ֵ      ׼�������ļ��Ƿ�ɹ�������ɹ�����ERR_SUCCESS�����򷵻ط���
�׳��쳣    ��
**************************************************************************/
int CSaveTransaction::PrepareFiles(const char *szPath, time_t tFileTime)
{
    // ��Ŀ¼
    ACE_DIR *pBaseDIR = ACE_OS_Dirent::opendir(szPath);
    if( pBaseDIR == NULL)
    {
        char szTmpBuf[500];
        sprintf(szTmpBuf, "Errno: %d (%s)", errno, strerror(errno));
        STRING sTmpInfo = S_OPEN_DIR_FAILED;
        sTmpInfo += szPath;
        MSGOUT(m_szModuName, MSGOUT_LEVEL_IMPORTANT, sTmpInfo.c_str());
        //by ldf 2004-01-28 ��core�Ķ�λ���ִ˴���������
        sTmpInfo =szPath;
        sTmpInfo += ',';
        //end
        sTmpInfo += szTmpBuf;
        TRACE(m_szModuName, sTmpInfo.c_str());

        return ERR_FAIL;
    }

    int nRet = 0;
    dirent *pDirent;
    ACE_stat astat;
    STRING szCurFile;
    PFilesListNode pFileNode;
    
    // ����Ŀ¼�µ������ļ�
    while((pDirent = ACE_OS_Dirent::readdir(pBaseDIR)) != NULL)
    {
        // ��������Ŀ¼
        szCurFile = szPath;
        szCurFile = MakePath(szCurFile, pDirent->d_name);
        nRet = ACE_OS::stat(szCurFile.c_str(), &astat);
        if(nRet != 0)
        {
            continue;
        }
        
        #ifdef _PLATFORM_WIN32
        #define S_ISDIR(mode)   (((mode) & S_IFMT) == S_IFDIR)
        #endif
        nRet = S_ISDIR(astat.st_mode);
        if(nRet != 0 )
        {
            continue;
        }

        // ����������tFileTime֮��û���޸Ĺ����ļ�
        if(astat.st_mtime < tFileTime)
        {
            continue;
        }
        
        pFileNode = new SFilesListNode;
        pFileNode->tFileTime = astat.st_mtime;
        ACE_OS::strcpy(pFileNode->szFileName, pDirent->d_name);
        m_FileList.push_back(pFileNode);
    }
    
    ACE_OS_Dirent::closedir(pBaseDIR);
    CSaveTransaction::Compare comp;
    std::sort(m_FileList.begin(), m_FileList.end(), comp);

    return ERR_SUCCESS;
}


bool CSaveTransaction::Compare::operator ()(SFilesListNode* x, SFilesListNode *y)
{
    if(x->tFileTime < y->tFileTime)
    {
        return true;
    }

    if(x->tFileTime > y->tFileTime)
    {
        return false;
    }

    //if(x->tFileTime == y->tFileTime)
    if(strcmp(x->szFileName, y->szFileName) < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CSaveTransaction::GetCurrentFileName(STRING& sFileName)
{
	sFileName = m_sCurFileFullPath;
}
