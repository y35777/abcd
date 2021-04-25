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
// 函数名: 	CEventBillFile::Init
// 作者:    chenliangwei
// 时期:    02-10-30
// 描述:    初始化函数
// 输入:
//       参数1 :const char* szFileName
// 返回值: BOOL 
******************************************************/
BOOL CEventBillFile::Init(const char* szFileName)
{
    if((szFileName == NULL) || (szFileName[0] == '\0'))
        return FALSE;

    strncpy(m_szEventFileName, szFileName, sizeof(m_szEventFileName));
    m_szEventFileName[sizeof(m_szEventFileName) - 1] = '\0';

    return LoadFromFile();
}

//设置源文件名称
void CEventBillFile::SetSrcFileName(const char* szSrcFileName)
{
    strncpy(m_szSrcFileName, szSrcFileName, sizeof(m_szSrcFileName));
    m_szSrcFileName[sizeof(m_szSrcFileName) - 1] = '\0';

}

/******************************************************
// 函数名: 	CEventBillFile::AddAEventBill
// 作者:    chenliangwei
// 时期:    02-10-30
// 描述:    增加一条事件话单
// 输入:
//       参数1 :MSC_Bill_Report* pEventBill
//       参数2 :int nOffsetOfSrcFile
// 返回值: BOOL 
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
// 函数名: 	CEventBillFile::FindEventBills
// 作者:    chenliangwei
// 时期:    02-10-30
// 描述:    通过普通话单查找事件话单
// 输入:
//       参数1 :MSC_Bill_Report* pBill
//       参数2 :MSC_Bill_Report*& pEventBill
//       参数3 :int& nEventCnt
// 返回值: BOOL 
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

    //从EventMap和NewEventMap中得到文件的偏移量列表
    OFFSETLIST* eventlist = NULL;
    if(m_EventMap.find(sKey) != m_EventMap.end())
        eventlist = &m_EventMap[sKey];

    OFFSETLIST* newlist = NULL;
    if(m_NewEventMap.find(sKey) != m_NewEventMap.end())
        newlist = &m_NewEventMap[sKey];

    /*
     * 从文件中获取事件话单。
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

    //读取事件文件
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

    //读取源文件
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
// 函数名: 	CEventBillFile::RemoveEventBills
// 作者:    chenliangwei
// 时期:    02-10-30
// 描述:    通过普通话单删除事件话单
// 输入:
//       参数1 :MSC_Bill_Report* pBill
// 返回值: BOOL 
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
// 函数名: 	CEventBillFile::SaveToFile
// 作者:    chenliangwei
// 时期:    02-10-30
// 描述:    保存EventMap到事件文件中
// 输入:
// 返回值: BOOL 
******************************************************/
BOOL CEventBillFile::SaveToFile()
{
    //新的事件文件
    char szNewEventFile[MAX_PATH];
    strcpy(szNewEventFile, m_szEventFileName);
    strcat(szNewEventFile, ".new");
    FILE*  pNewEventFile = fopen(szNewEventFile, "wb");
    if (NULL == pNewEventFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Create new Event File %s fail", szNewEventFile);
        return FALSE;
    }

    //事件文件
    FILE*  pEventFile = fopen(m_szEventFileName, "rb");
    if (NULL == pEventFile)
    {
        TRACE(MTS_BACKSAVE_PATCH, "Open Event File %s fail", m_szEventFileName);
        fclose(pNewEventFile);
        pNewEventFile = NULL;
        return FALSE;
    }

    //源话单文件
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

    //保存事件文件中的事件话单到新的事件文件中
    MAP<SMSCBillKey, OFFSETLIST>::iterator mapiter = m_EventMap.begin();
    while (mapiter != m_EventMap.end())
    {
        OFFSETLIST& offsetlist = mapiter->second;

        OFFSETLIST::iterator listiter = offsetlist.begin();
        while (listiter != offsetlist.end())
        {
            fseek(pEventFile, *listiter, SEEK_SET);
            fread(pBillBuf + nOffset, m_nBillLen, 1, pEventFile);

            //更改EventMap中的偏移量为新文件中的偏移量
            *listiter = nFileOffset + nOffset;

            nOffset += m_nBillLen;
            listiter++;

            //缓冲区满，将之写入文件
            if ((nOffset + m_nBillLen) > nBillBufSize)
            {
                fwrite(pBillBuf, nOffset, 1, pNewEventFile);
                nFileOffset += nOffset;
                nOffset = 0;
            }
        }
        mapiter++;
    }

    //把缓冲区中残留的数据写入文件
    if (nOffset > 0)
    {
        fwrite(pBillBuf, nOffset, 1, pNewEventFile);
        nFileOffset += nOffset;
        nOffset = 0;
    }

    //保存源文件中的事件话单到新的事件文件中
    mapiter = m_NewEventMap.begin();
    while (mapiter != m_NewEventMap.end())
    {
        OFFSETLIST& offsetlist = mapiter->second;

        OFFSETLIST::iterator listiter = offsetlist.begin();
        while (listiter != offsetlist.end())
        {
            fseek(pSrcFile, *listiter, SEEK_SET);
            fread(pBillBuf + nOffset, m_nBillLen, 1, pSrcFile);

            //更改NewEventMap中的偏移量为新文件中的偏移量
            *listiter = nFileOffset + nOffset;

            nOffset += m_nBillLen;
            listiter++;

            //缓冲区满，将之写入文件
            if ((nOffset + m_nBillLen) > nBillBufSize)
            {
                fwrite(pBillBuf, nOffset, 1, pNewEventFile);
                nFileOffset += nOffset;
                nOffset = 0;
            }
        }
        
        //将NewEventMap中的偏移量列表串到EventMap中
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
    
    //把缓冲区中残留的数据写入文件
    if (nOffset > 0)
    {
        fwrite(pBillBuf, nOffset, 1, pNewEventFile);
        nFileOffset += nOffset;
        nOffset = 0;
    }

    //清空NewEventMap
    m_NewEventMap.clear();

    delete []pBillBuf;
    pBillBuf = NULL;

    fclose(pNewEventFile);
    pNewEventFile = NULL;
    fclose(pEventFile);
    pEventFile = NULL;
    fclose(pSrcFile);
    pSrcFile = NULL;
    
    //移动新的事件文件为事件文件
    FileDel(m_szEventFileName);
    FileMove(szNewEventFile, m_szEventFileName);
    
    return TRUE;
}


/******************************************************
// 函数名: 	CEventBillFile::LoadFromFile
// 作者:    chenliangwei
// 时期:    02-10-30
// 描述:    加载事件文件
// 输入:
// 返回值: BOOL 
******************************************************/
BOOL CEventBillFile::LoadFromFile()
{
    FILE*  pEventFile = fopen(m_szEventFileName, "rb");
    if (NULL == pEventFile)
    {
        //创建新文件
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
	 * 从文件中依次读出缓冲区大小的话单。
	 */
	for (; nFileOffset < nFileLen; nFileOffset += nBytesRead)
	{
		//把话单读进缓冲区
		nBytesRead = fread(pBillBuf, 1, nBillBufSize, pEventFile);
	
		/*
		 * 处理每条话单。
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
