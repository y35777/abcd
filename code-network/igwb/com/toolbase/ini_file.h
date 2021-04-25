#ifndef  __INI_FILE_H__
#define  __INI_FILE_H__

#include "../include/precom.h"


//���º�����װ�˶�INI�ļ��ṹ�ĺ���
#define  DATA_ERR               0   //���ļ�����
#define  DATA_EOF               1   //�������ļ�β��
#define  DATA_SECTION_LINE      2   //�������ǽ��� "[]"
#define  DATA_KEY_LINE          3   //�����˼�ֵ "=" 
#define  DATA_NOTE_LINE         4   //����һ��ע��"//"��";"
#define  DATA_FAULT_LINE        5   //����
#define  DATA_OVERFLOW          6   //���������Ȳ���   
#define  DATA_RET_ERR           0xffffffff

#define  DATA_NO				0				
#define  DATA_YES				1

class IGWB_Svc_Export CINIFile
{
public:
	CINIFile(const char *pszFileName);
	~CINIFile();

//���ܺ���
public:
	int GetString(const char *szSection,
                  const char *szKey, 
                  const char *szDefault,
                  char *szOut,
                  int nLen);

	int GetInt(const char *szSection,
               const char *szKey, 
               const int nDefault);

	int WriteString(const char *szSectionName,
                    const char *szKeyName, 
                    const char *szKeyValue); 

	int WriteInt(const char *szSectionName,
                 const char *szKeyName, 
                 UINT4 uKeyValue);

public:
	BOOL Open();
	void Close();
	BOOL IsOpen();

protected:
	char *GetLine(char *szOut, int nLen);

	int GetLineType(char *szLine, int nLen);

	void GetSectionName(const char *szLine, char *szSection);

	void GetKeyName(const char *szLine, char *szKey);

	int GetKeyValue(const char *szLine, char *szKey, int nBufLen);

	BOOL GetStringByNo(char *szInput,
                       char *szKeyValue, 
                       int uTotalNum,
                       int uValueNum); 

	int StringToLine(const char *szKeyValue, const char *pszLine);

	int InsTxtInFile(int dwFilePos, const char *szInsString, BYTE yFlag);
	
protected:
	char   m_szFileName[MAX_PATH];
	FILE   *m_pFile;
    long   m_lFileSize;
};

#endif
