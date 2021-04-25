//////////////////////////////////////////////////////////////////////
// DdaSet.h: interface for the CDdaSet class.
// DdaSet : Direct Data Access Object
//////////////////////////////////////////////////////////////////////

#ifndef _DBF_DDASET_H_
#define _DBF_DDASET_H_

#include "../include/base_type.h"
#include <fstream>

#define MAX_RECORD_LEN	10240	// ��¼��󳤶�
#define MAX_FIELD_LEN	256		// �ֶ���󳤶�

// define index
#define dbNoneIndex		-1

// define database field type
#define dbNULL			0		// ��
#define dbLogic			1		// �߼���
#define dbChar			2		// �ַ���
#define dbNumber		3		// ������
#define dbData			4		// ������
#define dbMemo			5		// ��ע��
#define dbFloat			6		// ������ ?? 3?
#define RecUsed          ' '     //��¼�б�־��

typedef struct
{
	char Sign;		// �汾��,һ��Ϊ03
	char Year;		// ������,ȡ��ݺ���λ
	char Month;		// ������
	char Day;		// ������
	int  RecordNum;	// ���м�¼����
	UINT2 FirstOfs;	// ��һ����¼�ڿ��е�λ��(ͷ��Ϣ+���ֶ���Ϣ+�ָ����)
	UINT2 RecordLen;	// ÿ����¼����
	char Reserved[20];
}DBF_HEAD; // DBF�ļ�ͷ�ṹ

typedef struct
{
	char FieldName[10+1];	// �ֶ���,���10�ַ�,��'\0'����
	char FieldType;			// �ֶ�����,ASCII��
	int  FieldOfs;			// ���ֶ��ڼ�¼�е�ƫ��
	char FieldLen;			// �ֶγ���
	char FieldDot;			// С����λ��
	UINT2 Reserved;
	char NewSign;			// ������µĹ�������ʶ��
	char Reserved2[11];
}DBF_FIELD; // DBF�ֶ������ṹ

typedef struct
{
	STRING Name;
	char	Type;
	int		Ofs;
	int		Len;
	int		Dot;
}DB_FIELD; // �ֶνṹ

typedef struct _DB_RECORD
{
	char sign;		// ɾ�����,'*'��ʾ��ɾ��,' '��ʾδɾ��
	char data[1];	// ��������,���Ȳ���
	void* operator new(size_t classSize, size_t DataLength = 2)
    {
        void *ptr = NULL;
        int uLen = (DataLength > 2) ? (DataLength+1):2;
		uLen += sizeof(char);
        if (uLen > 1024 * 1024)
        {
            return NULL;
        }
        
        ptr = ::new char[uLen];
        if(ptr)
        {
            memset(ptr, 0, uLen);
           char *p = (char *)ptr;
           p[0] = ' ';
        }

        return ptr;
    };
}DB_RECORD;

typedef VECTOR<unsigned int> UINTArray;
class CDdaQuery;
class CDBFFile  
{
public:
	long tell();
	void flush();
	bool write(void *Buffer,int nSize,int nCount = 1);
	int seek(long Offset, int nPos = SEEK_SET);
	bool read(void *Buffer, int nSize, int nCount =1);
	bool open(const char *szFileName, const char *szFlag);
	void close();
	bool is_open();
	CDBFFile();
	virtual ~CDBFFile();
protected:
    FILE *m_pFile;
};

class CDdaSet
{
// Attribute
public:
	STRING			    m_strSetName;		// ���ݿ��ļ���
    CDBFFile            m_File;             // ���ݿ��ļ�
	int				    m_nOpenStyle;		// �򿪷�ʽ
	VECTOR<DB_FIELD*>	m_Field;			// �ṹ��������
	VECTOR<DB_RECORD*>	m_Record;			// ��¼����
	VECTOR<UINTArray*>	m_Index;			// ��������
	int				m_nRecordCount;		// �ܼ�¼����(������ɾ����)
	int				m_nUseRecordCount;	// ���õļ�¼����
	int				m_nRecordLength;	// ��¼����
	int				m_nFirstOfs;		// ��һ����¼�ڿ��е�λ��(ͷ��Ϣ+���ֶ���Ϣ+�ָ����)
	int				m_nFieldCount;		// �ֶθ���
	int				m_nCurIndex;		// ��ǰ������
	int				m_nSortField;		// ��ǰ�����ֶ�
	BOOL			m_bModifyFlag;		// �޸ı�־
	CDdaQuery*		m_pQuery;			// Loadʱ����������


public:
	int UpdateRecord(int nItem);
	void Truncate(int nPos);
	int DeleteRecord(unsigned int nItem);
	int AppendRecordToDisk(DB_RECORD *pRecord);
	void Clear();
	CDdaSet();
	virtual ~CDdaSet();

	CDdaSet(int iOpenStyle);

    BOOL    Open(const char *strFileName, const char *szFlag = "rb");
	void    Close(int nClear = 1);

	BOOL    IsOpen() { return m_File.is_open(); }
	void	SetQuery(CDdaQuery* pQuery);

	// function for load data from bam dataman
	void   InitSet(STRING &strFileName);
	void   AddField(DB_FIELD* pField);
	void   AddRecord(DB_RECORD* pRecord);

    BOOL   LoadFromFile(STRING &strFileName);
	BOOL   SyncToFile();

	int    GetRecordCount();		//ȡ��¼��
	int    GetFieldCount();		//ȡ�ֶ���

	int    GetFieldIndex(const STRING &strFieldName);
	STRING GetFieldName(int nField);
	STRING GetFieldName(const char * lpszName);
	int    ConvFieldType(char cType);
	int    GetFieldType(int nField);
	int    GetFieldType(const char *lpszName);

	// read function
	STRING GetField(int nItem, int nField);
	STRING GetFieldValue(int nItem,int nField);
	STRING GetFieldValue(DB_RECORD* pRecord, int nField);
	STRING GetFieldValue(DB_RECORD* pRecord, const char * lpszFieldName);
	BOOL   GetFieldBoolValue(int nItem, int nField);
	BOOL   GetFieldBoolValue(int nItem, const char * lpszFieldName);
	int    GetFieldIntValue(int nItem, int nField);
	int    GetFieldIntValue(int nItem, const char * lpszFieldName);
	STRING GetFieldStringValue(int nItem, int nField);
	STRING GetFieldStringValue(int nItem, const char * lpszFieldName);

	// write function
	void    SetField(int nItem, int nField, STRING &strField);
	void    SetFieldValue(int nItem, int nField, const STRING &strField);
	void    SetFieldBoolValue(int nItem, int nField, BOOL bField);
	void    SetFieldBoolValue(int nItem, const char * lpszFieldName, BOOL bField);
	void    SetFieldIntValue(int nItem, int nField, int iField);
	void    SetFieldIntValue(int nItem, const char * lpszFieldName, int iField);
	void    SetFieldStringValue(int nItem, int nField, STRING &strField);
	void    SetFieldStringValue(int nItem, const char *lpszFieldName, STRING &strField);

	// index and sort function
	void    UseIndex(int nIndex = dbNoneIndex);
	void    QuickSort(int L, int R);
	int     CreateIndex(int nField);
	int     CreateIndex(const char * lpszFieldName);

};
//һ�鳣�õ��ַ���������

//���ַ���ת���ɴ�д
void string_to_upper(char *pszBuffer);

//���ַ���ת����Сд
void string_to_lower(char *pszBuffer);

//ȥ���ַ�������ߵĿո�
void string_left_trim(char *szStr);

//ȥ���ַ����ұߵĿո�
void string_right_trim(char *szStr);

//ȥ���ַ������ҵĿո�
void string_lr_trim(char *szStr);

//�ַ�����Сд���бȽ�
int string_compare(const char *szS1, const char *szS2);

//�ַ�����Сд�����бȽ�
int string_nocase_compare(const char *szStr1, const char * szStr2);

//�ж�һ���ַ����Ƿ�����
bool is_digit_value(const char *szValue);

#endif //_DBF_DDASET_H
