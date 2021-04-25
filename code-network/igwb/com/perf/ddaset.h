//////////////////////////////////////////////////////////////////////
// DdaSet.h: interface for the CDdaSet class.
// DdaSet : Direct Data Access Object
//////////////////////////////////////////////////////////////////////

#ifndef _DBF_DDASET_H_
#define _DBF_DDASET_H_

#include "../include/base_type.h"
#include <fstream>

#define MAX_RECORD_LEN	10240	// 记录最大长度
#define MAX_FIELD_LEN	256		// 字段最大长度

// define index
#define dbNoneIndex		-1

// define database field type
#define dbNULL			0		// 空
#define dbLogic			1		// 逻辑型
#define dbChar			2		// 字符型
#define dbNumber		3		// 数字型
#define dbData			4		// 日期型
#define dbMemo			5		// 备注型
#define dbFloat			6		// 浮点型 ?? 3?
#define RecUsed          ' '     //记录中标志域

typedef struct
{
	char Sign;		// 版本号,一般为03
	char Year;		// 建库年,取年份后两位
	char Month;		// 建库月
	char Day;		// 建库日
	int  RecordNum;	// 库中记录个数
	UINT2 FirstOfs;	// 第一条记录在库中的位置(头信息+各字段信息+分隔标记)
	UINT2 RecordLen;	// 每条记录长度
	char Reserved[20];
}DBF_HEAD; // DBF文件头结构

typedef struct
{
	char FieldName[10+1];	// 字段名,最多10字符,以'\0'结束
	char FieldType;			// 字段类型,ASCII码
	int  FieldOfs;			// 本字段在记录中的偏移
	char FieldLen;			// 字段长度
	char FieldDot;			// 小数点位数
	UINT2 Reserved;
	char NewSign;			// 最近更新的工作区标识符
	char Reserved2[11];
}DBF_FIELD; // DBF字段描述结构

typedef struct
{
	STRING Name;
	char	Type;
	int		Ofs;
	int		Len;
	int		Dot;
}DB_FIELD; // 字段结构

typedef struct _DB_RECORD
{
	char sign;		// 删除标记,'*'表示已删除,' '表示未删除
	char data[1];	// 具体数据,长度不定
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
	STRING			    m_strSetName;		// 数据库文件名
    CDBFFile            m_File;             // 数据库文件
	int				    m_nOpenStyle;		// 打开方式
	VECTOR<DB_FIELD*>	m_Field;			// 结构描述数组
	VECTOR<DB_RECORD*>	m_Record;			// 记录数组
	VECTOR<UINTArray*>	m_Index;			// 索引数组
	int				m_nRecordCount;		// 总记录个数(包括已删除的)
	int				m_nUseRecordCount;	// 可用的记录个数
	int				m_nRecordLength;	// 记录长度
	int				m_nFirstOfs;		// 第一条记录在库中的位置(头信息+各字段信息+分隔标记)
	int				m_nFieldCount;		// 字段个数
	int				m_nCurIndex;		// 当前索引号
	int				m_nSortField;		// 当前索引字段
	BOOL			m_bModifyFlag;		// 修改标志
	CDdaQuery*		m_pQuery;			// Load时的限制条件


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

	int    GetRecordCount();		//取记录数
	int    GetFieldCount();		//取字段数

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
//一组常用的字符串处理函数

//将字符串转换成大写
void string_to_upper(char *pszBuffer);

//将字符串转换成小写
void string_to_lower(char *pszBuffer);

//去掉字符串中左边的空格
void string_left_trim(char *szStr);

//去掉字符串右边的空格
void string_right_trim(char *szStr);

//去掉字符串左右的空格
void string_lr_trim(char *szStr);

//字符串大小写敏感比较
int string_compare(const char *szS1, const char *szS2);

//字符串大小写不敏感比较
int string_nocase_compare(const char *szStr1, const char * szStr2);

//判断一个字符串是否数字
bool is_digit_value(const char *szValue);

#endif //_DBF_DDASET_H
