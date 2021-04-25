#include "../../com/include/toolbox.h"
#include "../include/format_out.h"
#include "meter_acc.h"
#include "meter_file.h"
#include "meter_index.h"
#include "resource.h"

/****************************************************************************
函数原型	public:CMeterFile::CMeterFile(const char* pszFileName,
                                          CMeterAccIndex* pMeterIndex,
                                          CFormat* pSrcFmt,
                                          CFormat* pDestFmt,
                                          int bForTTAT,
                                          int nTables)
功能描述	构造函数
输入参数	pszFileName  ---指向计次累加表文件名指针  
            pMeterIndex  ---指向索引表类(CMeterAccIndex)指针   
			pSrcFmt      ---指向格式库类(CFormat)指针   
			pDestFmt     ---指向格式库类(CFormat)指针
			bForTTAT     ---泰国TT&T局点
			nTables      ---用户计次表张数
输出参数	无
返回值	    无
抛出异常	无
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
函数原型	public:CMeterFile::~CMeterFile()
功能描述	析构函数
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
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
函数原型	public:int CMeterFile::Init()
功能描述	计次累加表文件
输入参数	无
输出参数	无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
*****************************************************************************/
int CMeterFile::Init()
{
    if (!m_pMeterIndex || !m_pSrcFmt || !m_pDestFmt)
    {
        return -1;
    }

    Close();

    //先认为文件已经存在
    if (NULL == (m_pFile = fopen(m_szFileName, "rb+")))
    {
        //如果文件不存在，则创建一个新文件
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
函数原型	protected:int CMeterFile:CMeterFile::Close(void)
功能描述	关闭打开的文件
输入参数	无
输出参数	无
返回值	    无
抛出异常	无
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
函数原型	protected:int CMeterFile::GetIndex(const unsigned char* pSubscriber, 
                                               const int nLength,
                                               long& nLocation)
功能描述	获得用户记录在表底文件中的位置
输入参数	pSubscribler  ---  指向用户记录内容指针
            nLength       ---  用户号码长度
输出参数	nLocation     ---  用户记录在表底文件中的位置
返回值	    0     ---成功
            非零值---失败
抛出异常	无
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

        if (0 == memcmp(pSubscriber, pEntry, nLength)) //比较号码是否一致
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
函数原型	protected:int CMeterFile::ReadEntry(unsigned char* pEntry, 
                                                const int nLength,
                                                const long nLocation)
功能描述	获得用户在计次累加表底文件中的记录
输入参数	nLength   ---  用户记录长度
	        nLocation ---  用户记录在表底文件中的位置           
输出参数    pEntry    ---  指向用户记录内容指针
返回值	    0     ---成功
            非零值---失败
抛出异常	无
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
函数原型	protected:int CMeterFile::WriteEntry(unsigned char* pEntry, 
                                                const int nLength,
                                                const long nLocation)
功能描述	改写用户在计次累加表底文件中的记录
输入参数	pEntry    ---  指向用户记录内容指针
            nLength   ---  用户记录长度
	        nLocation ---  用户记录在表底文件中的位置           
输出参数    无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
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
函数原型	protected:int CMeterFile::AddEntry(const unsigned char* pEntry, 
                                               const int nLength,
                                               long& nLocation)
功能描述	向计次累加表添加用户记录
输入参数	pEntry  ---  指用户记录的内容的指针
            nLength ---  用户记录的长度 
输出参数	nLocation -  用户记录在计次累加表中的位置
返回值	    0     ---成功
            非零值---失败
抛出异常	无
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
函数原型	public:int CMeterFile::ProcessARecord(const unsigned char* pEntry, 
                                                  const int nLength)
功能描述	处理用户计次表文件中的表段写入计次累加表文件
输入参数	pEntry  ---  指用户记录的内容的指针
            nLength ---  用户记录的长度 
输出参数	无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
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

    //查找用户计次表记录位置 //加一位time_id 防止一个用户改变提交时间
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
            
            //写入计次表累加文件尾
            if (0 != AddEntry(m_pWriteEntry, nLength, nLocation))
            {
                fclose(m_pFile);
                TRACE(MTS_METERACC, S_BS_FAIL_WRITE_FILE, m_szFileName);
                return -1;
            }
            
            //未找到，表明这是该用户计次表第一次累加 //加一位time_id 防止一个用户改变提交时间
            if (0 != m_pMeterIndex->AddIndex(m_pWriteEntry, SUBSCRIBER_NUM_LEN + 1, nLocation))
            {
                return -1;        
            }
        }                
        
        return 0;
    }
    
    if (0 != ReadEntry(m_pWriteEntry, nLength, nLocation))
    {
		//处理异常(计算机重启)
		unsigned long nFileLen =0;
		unsigned long nZero = 0;
		fseek(m_pFile, 0, SEEK_END);
		nFileLen = ftell(m_pFile);

		if ((nLocation - nFileLen) >= 0)  //索引表与计次累加表冲突
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
    
    //计次表逐张累加            
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
            nMeter1 += (nMeter2 + nMeter3);     //表底的表1是原表2与原表1的和
        }
        else
        {
            nMeter1 += nMeter2;
        }
        
        memcpy(&m_pWriteEntry[nMeterShift + i * sizeof(unsigned long)], 
               &nMeter1, sizeof(unsigned long));
    }
    
    //写回表底文件
    if (0 != WriteEntry(m_pWriteEntry, nLength, nLocation))
    {
        fclose(m_pFile);
        TRACE(MTS_METERACC, S_BS_FAIL_WRITE_FILE, m_szFileName);
        
        return -1;
    }

    return 0;
}
/****************************************************************************
函数原型	public:int CMeterFile::Submit(const char* pszTmpFile,
                                          const char* pszBSFile,
                                          const char* pszBackupFile)
功能描述	生成计费中心格式文件并备份每一转换结果
输入参数	pszTmpFile    ---  指向临时保存文件名指针
            pszBSFile     ---  指向计费中心保存文件名指针
			pszBackupFile ---  指向备份保存文件名指针
输出参数	无
返回值	    0     ---成功
            非零值---失败
抛出异常	无
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
	const static unsigned long FILE_BUFFER_SIZE = 500;  //多少个用户记录

    fseek(m_pFile, 0, SEEK_END);
    if (0 ==(nFileLen = ftell(m_pFile)))
    {
        return 0;       //空文件
    }

    if (NULL == (pTmpFile = fopen(pszTmpFile, "wb")))
    {
        TRACE(MTS_METERACC, S_BS_FAIL_OPEN_FILE, pszTmpFile);
        return -1;
    }

    unsigned long nSrcBillLen  = m_pSrcFmt->GetFmtLen();
    unsigned long nDestBillLen = m_pDestFmt->GetFmtLen(); 
    void* pAccBill = NULL;    
	
    fflush(m_pFile);      //防止有些内存中的内容中没有写入文件

    //指向文件头部 
    fseek(m_pFile, 0, SEEK_SET);

	//一批批的转换
    
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
			memcpy(m_pWriteEntry, pWriteEntry, nSrcBillLen); //处理一个记录						
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
