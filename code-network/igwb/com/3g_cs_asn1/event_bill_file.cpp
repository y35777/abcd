#include "event_bill_file.h"
#include "../include/toolbox.h"
#include "../frame/msg_proc.h"
#include "CMCC_CS_BS_Patch.h"

CEventBillFile::CEventBillFile()
{
    m_nBillLen = sizeof(MSC_Bill_Report);
    m_nBillCntOfBuf = 100;
}

CEventBillFile::~CEventBillFile()
{

}


/******************************************************
// ������: 	CEventBillFile::Init
// ����:    chenliangwei
// ʱ��:    02-10-30
// ����:    ��ʼ������
// ����:
//       ����1 :const char* szFileName
// ����ֵ: BOOL 
******************************************************/
BOOL CEventBillFile::Init(const char* szFileName)
{
    if((szFileName == NULL) || (szFileName[0] == '\0'))
        return FALSE;

    strncpy(m_szEventFileName, szFileName, sizeof(m_szEventFileName));
    m_szEventFileName[sizeof(m_szEventFileName) - 1] = '\0';

    return LoadFromFile();
}

//����Դ�ļ�����
void CEventBillFile::SetSrcFileName(const char* szSrcFileName)
{
    strncpy(m_szSrcFileName, szSrcFileName, sizeof(m_szSrcFileName));
    m_szSrcFileName[sizeof(m_szSrcFileName) - 1] = '\0';

}

/******************************************************
// ������: 	CEventBillFile::AddAEventBill
// ����:    chenliangwei
// ʱ��:    02-10-30
// ����:    ����һ���¼�����
// ����:
//       ����1 :MSC_Bill_Report* pEventBill
//       ����2 :int nOffsetOfSrcFile
// ����ֵ: BOOL 
******************************************************/
BOOL CEventBillFile::AddAEventBill(MSC_Bill_Report* pEventBill, int nOffsetOfSrcFile)
{
    if(pEventBill == NULL)
        return FALSE;

    SMSCBillKey sKey;
    sKey.ModuleNum = pEventBill->ModuleNum;
    sKey.FirstSequence = pEventBill->FirstSequence;

    if(m_NewEventMap.find(sKey) != m_NewEventMap.end())
    {
        m_NewEventMap[sKey].push_back(nOffsetOfSrcFile);
    }
    else
    {
        OFFSETLIST offsetlist;
        offsetlist.push_back(nOffsetOfSrcFile);
        m_NewEventMap[sKey] = offsetlist;
    }

    return TRUE;
}

/******************************************************
// ������: 	CEventBillFile::FindEventBills
// ����:    chenliangwei
// ʱ��:    02-10-30
// ����:    ͨ����ͨ���������¼�����
// ����:
//       ����1 :MSC_Bill_Report* pBill
//       ����2 :MSC_Bill_Report*& pEventBill
//       ����3 :int& nEventCnt
// ����ֵ: BOOL 
******************************************************/
BOOL CEventBillFile::FindEventBills(MSC_Bill_Report* pBill,
                        MSC_Bill_Report*& pEventBill, int& nEventCnt)
{

    if(pEventBill != NULL)
        return FALSE;

    SMSCBillKey sKey;
    sKey.ModuleNum = pBill->ModuleNum;
    sKey.FirstSequence = pBill->FirstSequence;

    if((m_EventMap.find(sKey) == m_EventMap.end())
        && (m_NewEventMap.find(sKey) == m_NewEventMap.end()))
        return FALSE;

    //��EventMap��NewEventMap�еõ��ļ���ƫ�����б�
    OFFSETLIST* eventlist = NULL;
    if(m_EventMap.find(sKey) != m_EventMap.end())
        eventlist = &m_EventMap[sKey];

    OFFSETLIST* newlist = NULL;
    if(m_NewEventMap.find(sKey) != m_NewEventMap.end())
        newlist = &m_NewEventMap[sKey];

    /*
     * ���ļ��л�ȡ�¼�������
     */
    int nCount = 0;
    if(eventlist != NULL)
    {
        nCount = eventlist->size();
    }
    if(newlist != NULL)
    {
        nCount += newlist->size();
    }
    if(nCount == 0)
        return FALSE;

    MSC_Bill_Report* pEventBuf = new MSC_Bill_Report[nCount];
    MSC_Bill_Report* pABill = pEventBuf;

    //��ȡ�¼��ļ�
    if(eventlist != NULL && eventlist->size() != 0)
    {
        FILE* pFile = fopen(m_szEventFileName, "rb");
        if(pFile == NULL)
        {
            delete []pEventBuf;
            pEventBuf = NULL;
            return FALSE;
        }

        BOOL bResult = TRUE;
        OFFSETLIST::iterator iter = eventlist->begin();
        while(iter != eventlist->end())
        {
            if(fseek(pFile, *iter, SEEK_SET) != 0)
            {
                bResult = FALSE;
                break;
            }
            else
            if(fread(pABill, m_nBillLen, 1, pFile) != 1)
            {
                bResult = FALSE;
                break;
            }
            else
            {
                pABill++;
                iter++;
            }
        }

        fclose(pFile);
        pFile = NULL;

        if(!bResult)
        {
            delete []pEventBuf;
            pEventBuf = NULL;
            return FALSE;
        }

    }

    //��ȡԴ�ļ�
    if(newlist != NULL && newlist->size() != 0)
    {
        FILE* pFile = fopen(m_szSrcFileName, "rb");
        if(pFile == NULL)
        {
            delete []pEventBuf;
            pEventBuf = NULL;
            return FALSE;
        }

        BOOL bResult = TRUE;
        OFFSETLIST::iterator iter = newlist->begin();
        while(iter != newlist->end())
        {
            if(fseek(pFile, *iter, SEEK_SET) != 0)
            {
                bResult = FALSE;
                break;
            }
            else
            if(fread(pABill, m_nBillLen, 1, pFile) != 1)
            {
                bResult = FALSE;
                break;
            }
            else
            {
                pABill++;
                iter++;
            }
        }

        fclose(pFile);
        pFile = NULL;

        if(!bResult)
        {
            delete []pEventBuf;
            pEventBuf = NULL;
            return FALSE;
        }
    }

    nEventCnt = nCount;
    pEventBill = pEventBuf;

    return TRUE;
}


/******************************************************
// ������: 	CEventBillFile::RemoveEventBills
// ����:    chenliangwei
// ʱ��:    02-10-30
// ����:    ͨ����ͨ����ɾ���¼�����
// ����:
//       ����1 :MSC_Bill_Report* pBill
// ����ֵ: BOOL 
******************************************************/
BOOL CEventBillFile::RemoveEventBills(MSC_Bill_Report* pEventBill)
{
    if(pEventBill == NULL)
        return FALSE;

    SMSCBillKey sKey;
    sKey.ModuleNum = pEventBill->ModuleNum;
    sKey.FirstSequence = pEventBill->FirstSequence;

    if(m_EventMap.find(sKey) != m_EventMap.end())
        m_EventMap.erase(sKey);

    if(m_NewEventMap.find(sKey) != m_NewEventMap.end())
        m_NewEventMap.erase(sKey);

    return TRUE;
}

/******************************************************
// ������: 	CEventBillFile::SaveToFile
// ����:    chenliangwei
// ʱ��:    02-10-30
// ����:    ����EventMap���¼��ļ���
// ����:
// ����ֵ: BOOL 
******************************************************/
BOOL CEventBillFile::SaveToFile()
{
    //�µ��¼��ļ�
    char szNewEventFile[MAX_PATH];
    strcpy(szNewEventFile, m_szEventFileName);
    strcat(szNewEventFile, ".new");
    FILE*  pNewEventFile = fopen(szNewEventFile, "wb");
    if (NULL == pNewEventFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Create new Event File %s fail", szNewEventFile);
        return FALSE;
    }

    //�¼��ļ�
    FILE*  pEventFile = fopen(m_szEventFileName, "rb");
    if (NULL == pEventFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Open Event File %s fail", m_szEventFileName);
        fclose(pNewEventFile);
        pNewEventFile = NULL;
        return FALSE;
    }

    //Դ�����ļ�
    FILE*  pSrcFile = fopen(m_szSrcFileName, "rb");
    if (NULL == pSrcFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Open Source File %s fail", m_szSrcFileName);
        fclose(pNewEventFile);
        pNewEventFile = NULL;
        fclose(pEventFile);
        pEventFile = NULL;
        return FALSE;
    }


    int nBillBufSize = m_nBillCntOfBuf * m_nBillLen;
    char* pBillBuf = new char[nBillBufSize];
    UINT4 nFileOffset = 0;
    UINT4 nOffset = 0;

    //�����¼��ļ��е��¼��������µ��¼��ļ���
    MAP<SMSCBillKey, OFFSETLIST>::iterator mapiter = m_EventMap.begin();
    while (mapiter != m_EventMap.end())
    {
        OFFSETLIST& offsetlist = mapiter->second;

        OFFSETLIST::iterator listiter = offsetlist.begin();
        while (listiter != offsetlist.end())
        {
            fseek(pEventFile, *listiter, SEEK_SET);
            fread(pBillBuf + nOffset, m_nBillLen, 1, pEventFile);

            //����EventMap�е�ƫ����Ϊ���ļ��е�ƫ����
            *listiter = nFileOffset + nOffset;

            nOffset += m_nBillLen;
            listiter++;

            //������������֮д���ļ�
            if ((nOffset + m_nBillLen) > nBillBufSize)
            {
                fwrite(pBillBuf, nOffset, 1, pNewEventFile);
                nFileOffset += nOffset;
                nOffset = 0;
            }
        }
        mapiter++;
    }

    //�ѻ������в���������д���ļ�
    if (nOffset > 0)
    {
        fwrite(pBillBuf, nOffset, 1, pNewEventFile);
        nFileOffset += nOffset;
        nOffset = 0;
    }

    //����Դ�ļ��е��¼��������µ��¼��ļ���
    mapiter = m_NewEventMap.begin();
    while (mapiter != m_NewEventMap.end())
    {
        OFFSETLIST& offsetlist = mapiter->second;

        OFFSETLIST::iterator listiter = offsetlist.begin();
        while (listiter != offsetlist.end())
        {
            fseek(pSrcFile, *listiter, SEEK_SET);
            fread(pBillBuf + nOffset, m_nBillLen, 1, pSrcFile);

            //����NewEventMap�е�ƫ����Ϊ���ļ��е�ƫ����
            *listiter = nFileOffset + nOffset;

            nOffset += m_nBillLen;
            listiter++;

            //������������֮д���ļ�
            if ((nOffset + m_nBillLen) > nBillBufSize)
            {
                fwrite(pBillBuf, nOffset, 1, pNewEventFile);
                nFileOffset += nOffset;
                nOffset = 0;
            }
        }
        
        //��NewEventMap�е�ƫ�����б���EventMap��
        SMSCBillKey sKey = mapiter->first;
        if (m_EventMap.find(sKey) != m_EventMap.end())
        {
            OFFSETLIST& eventlist = m_EventMap[sKey];
            eventlist.insert(eventlist.end(), offsetlist.begin(), offsetlist.end());
        }
        else
        {
            m_EventMap[sKey] = offsetlist;
        }

        mapiter++;
    }
    
    //�ѻ������в���������д���ļ�
    if (nOffset > 0)
    {
        fwrite(pBillBuf, nOffset, 1, pNewEventFile);
        nFileOffset += nOffset;
        nOffset = 0;
    }

    //���NewEventMap
    m_NewEventMap.clear();

    delete []pBillBuf;
    pBillBuf = NULL;

    fclose(pNewEventFile);
    pNewEventFile = NULL;
    fclose(pEventFile);
    pEventFile = NULL;
    fclose(pSrcFile);
    pSrcFile = NULL;
    
    //�ƶ��µ��¼��ļ�Ϊ�¼��ļ�
    FileDel(m_szEventFileName);
    FileMove(szNewEventFile, m_szEventFileName);
    
    return TRUE;
}


/******************************************************
// ������: 	CEventBillFile::LoadFromFile
// ����:    chenliangwei
// ʱ��:    02-10-30
// ����:    �����¼��ļ�
// ����:
// ����ֵ: BOOL 
******************************************************/
BOOL CEventBillFile::LoadFromFile()
{
    FILE*  pEventFile = fopen(m_szEventFileName, "rb");
    if (NULL == pEventFile)
    {
        //�������ļ�
        pEventFile = fopen(m_szEventFileName, "wb");
        if (NULL == pEventFile)
        {
            TRACE(MTS_BACKSAVE_PATCH, "Open Event File %s fail", m_szEventFileName);
            return FALSE;
        }
        else
        {
            fclose(pEventFile);
            pEventFile = NULL;
            return TRUE;
        }
    }

    int nBillBufSize = m_nBillCntOfBuf * m_nBillLen;
    char* pBillBuf = new char[nBillBufSize];
    UINT4 nFileLen = FileSize(pEventFile);
    size_t nBytesRead = 0;
    UINT4 nFileOffset = 0;

    MSC_Bill_Report* pCurrentBill = NULL;
    SMSCBillKey sKey;
        
 	/*
	 * ���ļ������ζ�����������С�Ļ�����
	 */
	for (; nFileOffset < nFileLen; nFileOffset += nBytesRead)
	{
		//�ѻ�������������
		nBytesRead = fread(pBillBuf, 1, nBillBufSize, pEventFile);
	
		/*
		 * ����ÿ��������
		 */
		INT4 nOffset = 0;
		for (; (nOffset + m_nBillLen) <= nBytesRead; nOffset += m_nBillLen)
		{
			pCurrentBill = (MSC_Bill_Report*)(pBillBuf + nOffset);
            sKey.ModuleNum = pCurrentBill->ModuleNum;
            sKey.FirstSequence = pCurrentBill->FirstSequence;

            if (m_EventMap.find(sKey) != m_EventMap.end())
            {
                m_EventMap[sKey].push_back(nFileOffset + nOffset);
            }
            else
            {
                OFFSETLIST offsetlist;
                offsetlist.push_back(nFileOffset + nOffset);
                m_EventMap[sKey] = offsetlist;
            }
		}
    }

    fclose(pEventFile);
    pEventFile = NULL;

    delete []pBillBuf;
    pBillBuf = NULL;
   
    return TRUE;    

}
