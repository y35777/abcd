#include "hot_save_chanl.h"
#include "../include/msg_def.h"

CHotSaveChannel::CHotSaveChannel(CPatchEncap* pPatchEncap) : CSaveChannel(pPatchEncap)
{
    m_pSeccondFile = NULL;
}

CHotSaveChannel::~CHotSaveChannel()
{
    if (NULL != m_pSeccondFile)
    {
        fclose(m_pSeccondFile);
        m_pSeccondFile = NULL;
    }
}

/**********************************************************************
��������    ��ʼ������
�������    uAPID----������
            uChannelID----ͨ����
            szChannelName----ͨ����
�������    ��
����ֵ      ��ʼ���Ƿ�ɹ�
            TRUE----��ʼ���ɹ�
            FALSE----��ʼ��ʧ��
�׳��쳣    ��
���¼�¼: 
����            ����                  ���� 
========== ================== ======================
04-05-14        chenliangwei     ������
**********************************************************************/
BOOL CHotSaveChannel::Init(UINT4 uAPID, UINT4 uChannelID, STRING& szChannelName, int nBillLen)
{
    //�������ΪHotSave����m_bSecondCopy��������Ч
    m_bSecondCopy = TRUE;

    return CSaveChannel::Init(uAPID, uChannelID, szChannelName, nBillLen);
    
}

/**********************************************************************
��������    �����ڶ��ݻ����ļ��������ø����ͬ��������
�������    ��
�������    ��
����ֵ      �������ļ��Ƿ�ɹ���
                TRUE----�����ɹ�
                FALSE----����ʧ��
�׳��쳣    ��
��ע        �ȴ����ڶ��ݻ����ļ����ٵ��ø����ͬ������������ʱ�ļ��Ĵ���
            ��״̬�ļ��ĸ��¡�
���¼�¼: 
����            ����                  ���� 
========== ================== ======================
04-05-14        chenliangwei     ������
**********************************************************************/
BOOL CHotSaveChannel::CreateNewFile()
{
    //����ǰ׺
    BOOL bSucc = CSaveTransaction::CreateNewFile();
    if (!bSucc)
    {
        return FALSE;
    }

    time_t tCurrent;
    time(&tCurrent);
    tm* ptmCurTime = localtime(&tCurrent);
    m_tmCreateTime = *ptmCurTime;

    STRING szPrefix = FormatCfgStr(m_szFormalPrefix, 
                                   m_tmCreateTime, 
                                   m_uChannelID);

    //�������к�
    STRING szFileCsn = GetFileCsnStr(m_uCurCsn);

    STRING szMidfix = FormatCfgStr(m_szFormalMidfix, 
                                   m_tmCreateTime, 
                                   m_uChannelID);

	//by	����	2004-5-29	��Ӧ���ⵥSWPD05385	
    //����ڶ��ݻ����ļ��ļ���
    if (0 == strlen(m_szFormalPostfix))
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        m_szSecondFileName = szPrefix + szFileCsn + szMidfix;
    }
    else
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        m_szSecondFileName
             = szPrefix + szFileCsn + szMidfix + "." + m_szFormalPostfix;
    }
	//the end	2004-5-29		��Ӧ���ⵥSWPD05385
	

    STRING szSecondFullPath = m_szSecondPath + "/" + m_szSecondFileName;

    //�����ڶ��ݻ����ļ�
    m_pSeccondFile = fopen(szSecondFullPath.c_str(), "wb+");
    if (NULL == m_pSeccondFile)
    {
        FileDel(m_sCurFileFullPath.c_str());
        return FALSE;
    }

    //ˢ��״̬�ļ�

    //����Զ2002.05.18�޸ģ����ⵥ��D13767
    //ˢ��״̬��¼����д��״̬��¼
    ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFirstPID
                = ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;
    ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFileCsn = m_uCurCsn;
    ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uBillCsn
                = ((BS_STATUS_FILE_HEAD* )m_pHead)->uBillCsn;
    ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uReserved
                = tCurrent;

    bSucc = m_InfoMain.WriteUnit(m_pLastInfoUnit, m_uUnitLen);
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoBak.WriteUnit(m_pLastInfoUnit, m_uUnitLen);
    if (!bSucc)
    {
        return FALSE;
    }

    //ˢ��״̬ͷ����д״̬ͷ
    m_InfoMain.IncreaseIndex(m_pHead);

    //����һ������״̬�ļ�ͷ���е��ļ�CSNʼ�ն���Ϊ��ʽ�ļ���CSN
    m_uCurCsn = m_uCurCsn - m_uFileCsnStep;
    if ((m_uCurCsn > m_uFileCsnEnd) || (m_uCurCsn < m_uFileCsnStart - m_uFileCsnStep))
    {
        m_uCurCsn = m_uFileCsnStart - m_uFileCsnStep;
    }
    
    ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn = m_uCurCsn;
    ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos = 0;
    m_InfoMain.CalcCheckSum(m_pHead);
    bSucc = m_InfoMain.WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoMain.Flush();
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoBak.WriteHead(m_pHead, m_uHeadLen);
    if (!bSucc)
    {
        return FALSE;
    }
    bSucc = m_InfoBak.Flush();
    if (!bSucc)
    {
        return FALSE;
    }
    //����Զ2002.05.18�޸Ľ���

    m_uFileExistTime = 0;

    return TRUE;

    return TRUE;

}
/**********************************************************************
��������    �رջ����ļ��ĺ��������رղ��ضϵڶ��ݻ����ļ���
�������    ��
�������    ��
����ֵ      �����Ƿ�ɹ�
                TRUE----����ɹ�
                FALSE----����ʧ��
�׳��쳣    ��
���¼�¼: 
����            ����                  ���� 
========== ================== ======================
04-05-14        chenliangwei     ������
**********************************************************************/
BOOL CHotSaveChannel::DoWhenFileClose()
{
    //�رյڶ��ݻ����ļ�
    if (NULL != m_pSeccondFile)
    {
        int nResult = fclose(m_pSeccondFile);
        if (0 != nResult)
        {
            return FALSE;
        }
        
        m_pSeccondFile = NULL;
        
        //�ضϵڶ��ݻ����ļ�
        UINT4 uWriteFilePos = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileWritePos;
        STRING szFileFullPath = m_szSecondPath + "/" + m_szSecondFileName;
        nResult = ACE_OS::truncate(szFileFullPath.c_str(), uWriteFilePos);
        if (0 != nResult)
        {
            return FALSE;
        }
    }
    
    //Ϊ��ֹ�ظ������Ƶڶ����ļ��Ĳ�����
    //���ｫm_bSecondCopy�Ƚ��ã�������
    m_bSecondCopy = FALSE;
    BOOL bSucc = CSaveChannel::DoWhenFileClose();
    m_bSecondCopy = TRUE;

    return bSucc;
}


/**********************************************************************
��������    �ڱ��滰���������е�д�����ļ�����
            ��д���һ����ʱ�ļ�����д�ڶ��ݻ����ļ�
�������    pPacketInfo----�������������Ϣ���ں����⸺��������ͷ�
            bFlush----�Ƿ����Flush����
�������    ��
����ֵ      ����ֵ˵�����£�
                0----��ȡ�ɹ�
                1----��ȡʧ��
                <0----��ȡʧ�ܣ�����ֵ�ľ���ֵΪ�����룬�������������ȡֵ��
                    ERR_WRITE_BILLFILE_FAIL----д�����ļ�ʧ��
�׳��쳣    ��
���¼�¼: 
����            ����                  ���� 
========== ================== ======================
04-05-14        chenliangwei     ������
**********************************************************************/
int CHotSaveChannel::WriteBillFile(BYTE* pPacketInfo, BOOL bFlush)
{
    int nRet = CSaveChannel::WriteBillFile(pPacketInfo, bFlush);

    if ( ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //�Դ������Ϣ��������ת��
    SBackPacketInfo* pBackPacketInfo = (SBackPacketInfo* )pPacketInfo;

    //д��ڶ��ݻ����ļ�
    //ֻ���ڻ������ǿյ�����£�����ʵ�ʵĴ������ļ���д�̵Ȳ�����
    //�������κβ���
    if (pBackPacketInfo->uLen >0)
    {
        if(m_pSeccondFile != NULL)
        {
            //д����������
            size_t uLenWritten = fwrite(pBackPacketInfo->pPacket, 
                                        pBackPacketInfo->uLen, 1, m_pSeccondFile);

            if (1 != uLenWritten)
            {
                return ERR_WRITE_BILLFILE_FAIL;
            }

            if (bFlush)
            {
                //����Flush����
                int nResult = fflush(m_pSeccondFile);
                if (0 != nResult)
                {
                    return ERR_WRITE_BILLFILE_FAIL;
                }
            }
        }
    }

    return ERR_SUCCESS;
}

BOOL CHotSaveChannel::DeleteTempFile()
{
    BS_STATUS_FILE_ELEMENT LastUnitInfo;

    //��ȡ��״̬�ļ������һ��״̬��¼
    int nResult = m_InfoMain.ReadLastUnit((BYTE* )&LastUnitInfo);
    if ((ERR_INDEX_INVALID == nResult) || (ERR_FAIL_READ == nResult))
    {
        return FALSE;
    }
    else if (ERR_NO_UNIT_EXIST == nResult)
    {
        return TRUE;
    }
    
    time_t tFileCreateTime = LastUnitInfo.uReserved;
    tm* ptmCreateTime = localtime(&tFileCreateTime);

    //�õ��ڶ����ļ���·�����ļ���
    STRING szPrefix = FormatCfgStr(m_szFormalPrefix, 
                                   *ptmCreateTime, 
                                   m_uChannelID);
    
    //�������к�
    STRING szFileCsn = GetFileCsnStr(LastUnitInfo.uFileCsn);

    STRING szMidfix = FormatCfgStr(m_szFormalMidfix, 
                                   m_tmCreateTime, 
                                   m_uChannelID);

	//by	����	2004-5-29	��Ӧ���ⵥSWPD05385
    //����ڶ��ݻ����ļ��ļ���
    STRING szSecondFileName;
    if (0 == strlen(m_szFormalPostfix))
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        szSecondFileName = szPrefix + szFileCsn + szMidfix;
    }
    else
    {
		if (0 == m_nCsnLen)
		{
			szFileCsn = "";
		}
        szSecondFileName
             = szPrefix + szFileCsn + szMidfix + "." + m_szFormalPostfix;
    }
	//the end	2004-5-29		��Ӧ���ⵥSWPD05385

    STRING szSecondFullPath = m_szSecondPath + "/" + szSecondFileName;

    BOOL bSucc = FileDel(szSecondFullPath.c_str());
    if (bSucc)
    {
        TRACE(m_szModuName, S_BS_TRC_SUCC_DEL_FILE, LastUnitInfo.uFileCsn);
    }
    else
    {
        TRACE(m_szModuName, S_BS_TRC_FAIL_DEL_FILE, LastUnitInfo.uFileCsn);
    }    

    return CSaveChannel::DeleteTempFile();    
    
}

/**********************************************************************
��������    ����״̬�ļ������Ϣ����صĳ�Ա�������г�ʼ��
            �����˶Եڶ����ļ��Ĵ򿪲�����
�������    ��
�������    ��
����ֵ      �����Ƿ�ɹ�
                TRUE----�����ɹ�
                FALSE----����ʧ��
�׳��쳣    ��
���¼�¼: 
����            ����                  ���� 
========== ================== ======================
04-05-14        chenliangwei     ������
**********************************************************************/
BOOL CHotSaveChannel::InitByStatusFile()
{
    BOOL bSucc = m_InfoMain.ReadHead(m_pHead);
    if (!bSucc)
    {
        return FALSE;
    }

    m_uCurCsn = ((BS_STATUS_FILE_HEAD* )m_pHead)->uFileCsn;
    m_uPID = ((BS_STATUS_FILE_HEAD* )m_pHead)->uLastPID;

    int nResult = m_InfoMain.ReadLastUnit(m_pLastInfoUnit);
    if ((ERR_FAIL_READ == nResult) || (ERR_INDEX_INVALID == nResult))
    {
        TraceModuleErr(MTS_BACKSAVE, nResult);
        return FALSE;
    }
    else if (ERR_SUCCESS == nResult)
    {
        //��ȡ״̬�ļ��ɹ��������״̬�ļ����ݳ�ʼ����س�Ա����

        //�õ����һ���ļ���ȫ·�������ݴ˳�ʼ��m_pBillFile
        //��˫����״̬�ļ�����ͬ��������£�m_pHead�Լ�m_pLastInfoUnit
        //�е��ļ����кſ��ܲ�һ�£�������m_pLastInfoUnit�е�Ϊ׼��
        //�ϴ�����ʱ����������ļ�
        m_szFileName = m_szPrefix
            + GetFileCsnStr(((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFileCsn)
            + "." + m_szPostfix;

        STRING szFileFullPath = m_szFilePath + "/" + m_szFileName;
        m_pBillFile = fopen(szFileFullPath.c_str(), "rb+");

        //��ʼ��m_tmCreateTime
        if (NULL != m_pBillFile)
        {
            time_t tFileCreateTime = ((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uReserved;
            
            //״̬�ļ��е�ʱ����Ч�Ļ������ļ������ж�ȡ
            if(tFileCreateTime == 0)
            {
                ACE_stat FileStat;
                ACE_OS::stat(szFileFullPath.c_str(), &FileStat);
                tFileCreateTime = FileStat.st_ctime;
            }

            tm* ptmCreateTime = localtime(&tFileCreateTime);
            m_tmCreateTime = *ptmCreateTime;

            //Ϊ�˷�ֹ���һ������Ŀ¼��ɾ�������Գ�ʼ��ʱ����һ�¸�����Ŀ¼
            char szLastDate[20];
            strftime(szLastDate, 20, "%Y%m%d", &m_tmCreateTime);
            m_szFirstPath = m_szDatePath + "/" + szLastDate;
            ACE_OS::mkdir(m_szFirstPath.c_str());

            //�õ��ڶ����ļ���·�����ļ���
            STRING szPrefix = FormatCfgStr(m_szFormalPrefix, 
                                           m_tmCreateTime, 
                                           m_uChannelID);

            //�������к�
            STRING szFileCsn = 
                GetFileCsnStr(((BS_STATUS_FILE_ELEMENT* )m_pLastInfoUnit)->uFileCsn);

            STRING szMidfix = FormatCfgStr(m_szFormalMidfix, 
                                           m_tmCreateTime, 
                                           m_uChannelID);
			
			//by	����	2004-5-29	��Ӧ���ⵥSWPD05385
            //����ڶ��ݻ����ļ��ļ���
            if (0 == strlen(m_szFormalPostfix))
            {
				if (0 == m_nCsnLen)
				{
					szFileCsn = "";
				}
                m_szSecondFileName = szPrefix + szFileCsn + szMidfix;
            }
            else
            {
				if (0 == m_nCsnLen)
				{
					szFileCsn = "";
				}
                m_szSecondFileName
                     = szPrefix + szFileCsn + szMidfix + "." + m_szFormalPostfix;
            }
			//the end	2004-5-29		��Ӧ���ⵥSWPD05385
			

            STRING szSecondFullPath = m_szSecondPath + "/" + m_szSecondFileName;
            m_pSeccondFile = fopen(szSecondFullPath.c_str(), "rb+");          
       }
    }

    return TRUE;    
}