#ifndef __METER_ACCUMULATION_FILE__
#define __METER_ACCUMULATION_FILE__

#include "../include/base_type.h"
#include "meter_file.h"

class CMeterAccIndex;
class CFormat;

class CMeterFile
{
public:
    CMeterFile(const char* pszFileName,
               CMeterAccIndex* pMeterIndex,
               CFormat* pSrcFmt,
               CFormat* pDestFmt,
               int bForTTAT,
               int nTables);

    ~CMeterFile();

public:
	void Flush( void );
    
	int    Init();
    
	void   Close(void);

    int    ProcessARecord(const unsigned char* pEntry, 
                          const int nLength);

    int    Submit(const char* pszTmpFile,
                  const char* pszBSFile,
                  const char* pszBackupFile);

protected:
    
    int    GetIndex(const unsigned char* pSubscriber, 
                    const int nLength,
                    long& nLocation);
    int    ReadEntry(unsigned char* pEntry, 
                     const int nLength,
                     const long nLocation);
    int    WriteEntry(const unsigned char* pEntry, 
                      const int nLength, 
                      const long nLocation);
    int    AddEntry(const unsigned char* pEntry, 
                    const int nLength,
                    long& nLocation);

private:
    FILE*       m_pFile;
    char        m_szFileName[MAX_PATH];
    CFormat*    m_pSrcFmt;          //计次表转换格式的源格式
    CFormat*    m_pDestFmt;         //计次表转换格式的目标格式
    int         m_bForTTAT;
    int         m_nMeterTables;     
    
    unsigned char*  m_pWriteEntry;
    CMeterAccIndex* m_pMeterIndex;
};

#endif //__METER_ACCUMULATION_FILE__