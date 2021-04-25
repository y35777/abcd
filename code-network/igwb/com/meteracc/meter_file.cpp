#include "../../com/include/toolbox.h"
#include "../include/format_out.h"
#include "meter_acc.h"
#include "meter_file.h"
#include "meter_index.h"
#include "resource.h"

/****************************************************************************
����ԭ��	public:CMeterFile::CMeterFile(const char* pszFileName,
                                          CMeterAccIndex* pMeterIndex,
                                          CFormat* pSrcFmt,
                                          CFormat* pDestFmt,
                                          int bForTTAT,
                                          int nTables)
��������	���캯��
�������	pszFileName  ---ָ��ƴ��ۼӱ��ļ���ָ��  
            pMeterIndex  ---ָ����������(CMeterAccIndex)ָ��   
			pSrcFmt      ---ָ���ʽ����(CFormat)ָ��   
			pDestFmt     ---ָ���ʽ����(CFormat)ָ��
			bForTTAT     ---̩��TT&T�ֵ�
			nTables      ---�û��ƴα�����
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
CMeterFile::CMeterFile(const char* pszFileName,
                       CMeterAccIndex* pMeterIndex,
                       CFormat* pSrcFmt,
                       CFormat* pDestFmt,
                       int bForTTAT,
                       int nTables)
{
    m_pFile          = NULL;
    m_pMeterIndex    = pMeterIndex;
    m_pSrcFmt        = pSrcFmt;
    m_pDestFmt       = pDestFmt;

    m_pWriteEntry    = NULL;
    m_bForTTAT       = bForTTAT;
    m_nMeterTables   = nTables;

    if (pszFileName)
    {
        strncpy(m_szFileName, pszFileName, MAX_PATH);
        m_szFileName[MAX_PATH - 1] = '\0';
    }
    else
    {
        m_szFileName[0]  = '\0';
    }
}

/****************************************************************************
����ԭ��	public:CMeterFile::~CMeterFile()
��������	��������
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
CMeterFile::~CMeterFile()
{
    Close();

    if (m_pWriteEntry)
    {
		delete[] m_pWriteEntry;
        m_pWriteEntry = NULL;
    }
}

/****************************************************************************
����ԭ��	public:int CMeterFile::Init()
��������	�ƴ��ۼӱ��ļ�
�������	��
�������	��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterFile::Init()
{
    if (!m_pMeterIndex || !m_pSrcFmt || !m_pDestFmt)
    {
        return -1;
    }

    Close();

    //����Ϊ�ļ��Ѿ�����
    if (NULL == (m_pFile = fopen(m_szFileName, "rb+")))
    {
        //����ļ������ڣ��򴴽�һ�����ļ�
        if (NULL == (m_pFile = fopen(m_szFileName, "wb+")))
        {
            TRACE(MTS_METERACC, S_BS_FAIL_OPEN_FILE, m_szFileName);
            return -1;
        }
    }

    if (!m_pWriteEntry)
    {
        m_pWriteEntry = new unsigned char[m_pSrcFmt->GetFmtLen()];
    }

    return 0;
}
/****************************************************************************
����ԭ��	protected:int CMeterFile:CMeterFile::Close(void)
��������	�رմ򿪵��ļ�
�������	��
�������	��
����ֵ	    ��
�׳��쳣	��
*****************************************************************************/
void CMeterFile::Close(void)
{
    if (m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}
/****************************************************************************
����ԭ��	protected:int CMeterFile::GetIndex(const unsigned char* pSubscriber, 
                                               const int nLength,
                                               long& nLocation)
��������	����û���¼�ڱ���ļ��е�λ��
�������	pSubscribler  ---  ָ���û���¼����ָ��
            nLength       ---  �û����볤��
�������	nLocation     ---  �û���¼�ڱ���ļ��е�λ��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/

int CMeterFile::GetIndex(const unsigned char* pSubscriber, 
                         const int nLength,
                         long& nLocation)
{
    if (NULL == m_pFile)
    {
        return -1;
    }

	unsigned long nEntrySize = m_pSrcFmt->GetFmtLen();

    unsigned char* pEntry = new unsigned char[nEntrySize];

    BOOL bFound  = FALSE;
    fseek(m_pFile, 0, SEEK_SET);
    while (!feof(m_pFile))
    {
        nLocation = ftell(m_pFile);
        if (1 != fread(pEntry, nEntrySize, 1, m_pFile))
        {
            break;
        }

        if (0 == memcmp(pSubscriber, pEntry, nLength)) //�ȽϺ����Ƿ�һ��
        {
            bFound = TRUE;
        }
    }

	if(pEntry)
	{
		delete[] pEntry;
	}
    if (!bFound)
    {
        return -1;
    }

    return 0;
}

/****************************************************************************
����ԭ��	protected:int CMeterFile::ReadEntry(unsigned char* pEntry, 
                                                const int nLength,
                                                const long nLocation)
��������	����û��ڼƴ��ۼӱ���ļ��еļ�¼
�������	nLength   ---  �û���¼����
	        nLocation ---  �û���¼�ڱ���ļ��е�λ��           
�������    pEntry    ---  ָ���û���¼����ָ��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterFile::ReadEntry(unsigned char* pEntry, 
                          const int nLength,
                          const long nLocation)
{
    if ((NULL == m_pFile) || (NULL == pEntry))
    {
        return -1;
    }

    if (0 != fseek(m_pFile, nLocation, SEEK_SET))
    {
        return -1;
    }
    
    if (1 != fread(pEntry, nLength, 1, m_pFile))
    {
	    return -1;
    }

    return 0;
}

/****************************************************************************
����ԭ��	protected:int CMeterFile::WriteEntry(unsigned char* pEntry, 
                                                const int nLength,
                                                const long nLocation)
��������	��д�û��ڼƴ��ۼӱ���ļ��еļ�¼
�������	pEntry    ---  ָ���û���¼����ָ��
            nLength   ---  �û���¼����
	        nLocation ---  �û���¼�ڱ���ļ��е�λ��           
�������    ��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterFile::WriteEntry(const unsigned char* pEntry, 
                           const int nLength, 
                           const long nLocation)
{
    if ((NULL == m_pFile) || (NULL == pEntry))
    {
        return -1;
    }

    if (0 != fseek(m_pFile, nLocation, SEEK_SET))
    {
        return -1;
    }
    
    if (1 != fwrite(pEntry, nLength, 1, m_pFile))
    {
        return -1;
    }
    
	return 0;
}
/****************************************************************************
����ԭ��	protected:int CMeterFile::AddEntry(const unsigned char* pEntry, 
                                               const int nLength,
                                               long& nLocation)
��������	��ƴ��ۼӱ�����û���¼
�������	pEntry  ---  ָ�û���¼�����ݵ�ָ��
            nLength ---  �û���¼�ĳ��� 
�������	nLocation -  �û���¼�ڼƴ��ۼӱ��е�λ��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterFile::AddEntry(const unsigned char* pEntry, 
                         const int nLength,
                         long& nLocation)
{
    if ((NULL == m_pFile) || (NULL == pEntry))
    {
        return -1;
    }

    if (0 != fseek(m_pFile, 0, SEEK_END))
    {
        return -1;
    }
    
    nLocation = ftell(m_pFile);

    if (1 != fwrite(pEntry, nLength, 1, m_pFile))
    {
        return -1;
    }
    
	return 0;
}
/****************************************************************************
����ԭ��	public:int CMeterFile::ProcessARecord(const unsigned char* pEntry, 
                                                  const int nLength)
��������	�����û��ƴα��ļ��еı��д��ƴ��ۼӱ��ļ�
�������	pEntry  ---  ָ�û���¼�����ݵ�ָ��
            nLength ---  �û���¼�ĳ��� 
�������	��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterFile::ProcessARecord(const unsigned char* pEntry, 
                               const int nLength)
{
    long nLocation;
    unsigned long nMeter1 = 0;
    unsigned long nMeter2 = 0;
    unsigned long nMeter3 = 0;
    
    unsigned long nMeterShift = SUBSCRIBER_NUM_LEN + 1;
    unsigned char nCatetory   = pEntry[SUBSCRIBER_NUM_LEN];

    //�����û��ƴα��¼λ�� //��һλtime_id ��ֹһ���û��ı��ύʱ��
    if (0 != m_pMeterIndex->GetIndex(pEntry, SUBSCRIBER_NUM_LEN + 1, nLocation))
    {
        if (0 != GetIndex(pEntry, SUBSCRIBER_NUM_LEN, nLocation))
        {
            memcpy(m_pWriteEntry, pEntry, nLength);

            if (m_bForTTAT)
            {
                memcpy(&nMeter1, &m_pWriteEntry[nMeterShift], sizeof(unsigned long));
                memcpy(&nMeter2, &m_pWriteEntry[nMeterShift + sizeof(unsigned long)], 
                       sizeof(unsigned long));
                nMeter1 += nMeter2;
                memcpy(&m_pWriteEntry[nMeterShift], &nMeter1, sizeof(unsigned long));
            }
            
            //д��ƴα��ۼ��ļ�β
            if (0 != AddEntry(m_pWriteEntry, nLength, nLocation))
            {
                fclose(m_pFile);
                TRACE(MTS_METERACC, S_BS_FAIL_WRITE_FILE, m_szFileName);
                return -1;
            }
            
            //δ�ҵ����������Ǹ��û��ƴα��һ���ۼ� //��һλtime_id ��ֹһ���û��ı��ύʱ��
            if (0 != m_pMeterIndex->AddIndex(m_pWriteEntry, SUBSCRIBER_NUM_LEN + 1, nLocation))
            {
                return -1;        
            }
        }                
        
        return 0;
    }
    
    if (0 != ReadEntry(m_pWriteEntry, nLength, nLocation))
    {
		//�����쳣(���������)
		unsigned long nFileLen =0;
		unsigned long nZero = 0;
		fseek(m_pFile, 0, SEEK_END);
		nFileLen = ftell(m_pFile);

		if ((nLocation - nFileLen) >= 0)  //��������ƴ��ۼӱ��ͻ
		{
			memcpy(m_pWriteEntry, pEntry, m_pSrcFmt->GetFmtLen());
			for (int j = m_nMeterTables - 1; j >= 0; j--)
			{   			
				memcpy(&m_pWriteEntry[nMeterShift + j * sizeof(unsigned long)], 
					&nZero, sizeof(unsigned long));
			}
		}
		else
		{
			fclose(m_pFile);
            TRACE(MTS_METERACC, S_BS_FAIL_READ_FILE, m_szFileName);        
            return -1;
		}
		//end
    }
    
    //�ƴα������ۼ�            
    for (int i = m_nMeterTables - 1; i >= 0; i--)
    {
        memcpy(&nMeter1, &m_pWriteEntry[nMeterShift + i * sizeof(unsigned long)], 
               sizeof(unsigned long));
        memcpy(&nMeter2, &pEntry[nMeterShift + i * sizeof(unsigned long)], 
               sizeof(unsigned long));
        
        if (m_bForTTAT && (i == 1))
        {
            nMeter3 = nMeter2;
        }
        
        if (m_bForTTAT && (i == 0))
        {
            nMeter1 += (nMeter2 + nMeter3);     //��׵ı�1��ԭ��2��ԭ��1�ĺ�
        }
        else
        {
            nMeter1 += nMeter2;
        }
        
        memcpy(&m_pWriteEntry[nMeterShift + i * sizeof(unsigned long)], 
               &nMeter1, sizeof(unsigned long));
    }
    
    //д�ر���ļ�
    if (0 != WriteEntry(m_pWriteEntry, nLength, nLocation))
    {
        fclose(m_pFile);
        TRACE(MTS_METERACC, S_BS_FAIL_WRITE_FILE, m_szFileName);
        
        return -1;
    }

    return 0;
}
/****************************************************************************
����ԭ��	public:int CMeterFile::Submit(const char* pszTmpFile,
                                          const char* pszBSFile,
                                          const char* pszBackupFile)
��������	���ɼƷ����ĸ�ʽ�ļ�������ÿһת�����
�������	pszTmpFile    ---  ָ����ʱ�����ļ���ָ��
            pszBSFile     ---  ָ��Ʒ����ı����ļ���ָ��
			pszBackupFile ---  ָ�򱸷ݱ����ļ���ָ��
�������	��
����ֵ	    0     ---�ɹ�
            ����ֵ---ʧ��
�׳��쳣	��
*****************************************************************************/
int CMeterFile::Submit(const char* pszTmpFile,
                       const char* pszBSFile,
                       const char* pszBackupFile)
{
    if (!pszTmpFile || !pszBSFile || !pszBackupFile)
    {
        return -1;
    }

    if (NULL == m_pFile)
    {
        return -1;
    }

    FILE* pTmpFile  = NULL;
	
	unsigned long nCopyNeed =0;
    unsigned long nFileLen = 0;
	unsigned long nCopiedBytes = 0;
	const static unsigned long FILE_BUFFER_SIZE = 500;  //���ٸ��û���¼

    fseek(m_pFile, 0, SEEK_END);
    if (0 ==(nFileLen = ftell(m_pFile)))
    {
        return 0;       //���ļ�
    }

    if (NULL == (pTmpFile = fopen(pszTmpFile, "wb")))
    {
        TRACE(MTS_METERACC, S_BS_FAIL_OPEN_FILE, pszTmpFile);
        return -1;
    }

    unsigned long nSrcBillLen  = m_pSrcFmt->GetFmtLen();
    unsigned long nDestBillLen = m_pDestFmt->GetFmtLen(); 
    void* pAccBill = NULL;    
	
    fflush(m_pFile);      //��ֹ��Щ�ڴ��е�������û��д���ļ�

    //ָ���ļ�ͷ�� 
    fseek(m_pFile, 0, SEEK_SET);

	//һ������ת��
    
	unsigned char *pWriteEntry = NULL;
    unsigned long nFileBufferSize = 0;
    unsigned char *pWriteEntryBlock = new unsigned char[FILE_BUFFER_SIZE * nSrcBillLen];
    
    while(nFileLen > nCopiedBytes)
    {
        nCopyNeed = min(nFileLen - nCopiedBytes, FILE_BUFFER_SIZE * nSrcBillLen);
		

        if ((1 != fread(pWriteEntryBlock, nCopyNeed, 1, m_pFile)))
        {
            fclose(m_pFile);           
            return -1;
        }     
		
        pWriteEntry = pWriteEntryBlock;
		nCopiedBytes += nCopyNeed;           
		nFileBufferSize = min( nCopyNeed / nSrcBillLen, FILE_BUFFER_SIZE);

		for (int i = 0; i < nFileBufferSize; i++)
		{
			memcpy(m_pWriteEntry, pWriteEntry, nSrcBillLen); //����һ����¼						
            pWriteEntry += nSrcBillLen;

			if (0 != m_pDestFmt->ConvertABill(m_pWriteEntry, 
                                                nSrcBillLen, 
                                                pAccBill, 
                                                nDestBillLen))
                {
                    fclose(pTmpFile);
                    pTmpFile = NULL;
        
                    if (pAccBill)
                    {
                        delete pAccBill;
                    }
        
                    return -1;
                }
           
                if (1 != fwrite(pAccBill, nDestBillLen, 1, pTmpFile))
                {
                    fclose(pTmpFile);
                    pTmpFile = NULL;
        
                    if (pAccBill)
                    {
                        delete pAccBill;
                    }
        
                    return -1;
                }						
		}            

        if (pAccBill)
        {
            delete pAccBill;
        }

        pAccBill = NULL;
		
    }

    if (pTmpFile)
    {
        fclose(pTmpFile);
        pTmpFile = NULL;
    }

    if (!FileCopy(pszTmpFile, pszBSFile))
    {
        TRACE(MTS_METERACC, S_COPY_FILE_FAILED, pszTmpFile, pszBSFile);
        return -1;
    }
    
    if (!FileCopy(pszTmpFile, pszBackupFile))
    {
        TRACE(MTS_METERACC, S_COPY_FILE_FAILED, pszTmpFile, pszBackupFile);
        return -1;
    }

    if (!FileDel(pszTmpFile))
    {
        TRACE(MTS_METERACC, S_BACKUP_DEL_FILE_FAILURE, pszTmpFile);
        return -1;
    }
    
	if(pWriteEntryBlock)
	{
		delete [] pWriteEntryBlock;
		pWriteEntry = NULL;
	}

    return 0;	
	
}

void CMeterFile::Flush()
{
	fflush(m_pFile);
}
