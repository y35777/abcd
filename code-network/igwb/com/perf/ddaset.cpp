// DdaSet.cpp: implementation of the CDdaSet class.
//
//////////////////////////////////////////////////////////////////////

#include "ddaset.h"
#include "ddaquery.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDdaSet::CDdaSet()
{
	m_bModifyFlag		= FALSE;
	m_nRecordCount		= 0;
	m_nUseRecordCount	= 0;
	m_nFieldCount		= 0;
	m_nCurIndex			= dbNoneIndex;
	m_pQuery			= NULL;
	m_nRecordLength     = 1; //任何记录都至少有一个标记
	m_nFirstOfs			= 0;

}

CDdaSet::CDdaSet(int iOpenStyle)
{
	m_nOpenStyle		= iOpenStyle;
	m_bModifyFlag		= FALSE;
	m_nRecordCount		= 0;
	m_nUseRecordCount	= 0;
	m_nFieldCount		= 0;
	m_nRecordLength     = 1;
	m_nFirstOfs			= 0;
	m_nCurIndex			= dbNoneIndex;
	m_pQuery			= NULL;
	
}

CDdaSet::~CDdaSet()
{
	// release dynamic memory
	Clear();
}

/////////////////////////////////////////////////////////////////////
//打开文件(采用直接读取方式)
/////////////////////////////////////////////////////////////////////
BOOL CDdaSet::Open(const char *strFileName, const char *szFlag)
{
    if(m_File.is_open())
    {
        Close();
    }
    
	//m_strSetName = strFileName;
	bool bRet = m_File.open(strFileName,szFlag);
	if(!bRet)
	{
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//关闭文件
/////////////////////////////////////////////////////////////////////
void CDdaSet::Close(int nClear)
{
    m_File.close();
    if(nClear) 
        Clear();
}

void CDdaSet::Truncate(int nPos)
{
    ACE_OS::truncate(m_strSetName.c_str(),nPos);
}


/////////////////////////////////////////////////////////////////////
//设置查询对象
/////////////////////////////////////////////////////////////////////
void CDdaSet::SetQuery(CDdaQuery* pQuery)
{
	m_pQuery = pQuery;
	m_pQuery->Attach(this, FALSE);
}

/////////////////////////////////////////////////////////////////////
//初始化表
/////////////////////////////////////////////////////////////////////
void CDdaSet::InitSet(STRING &strFileName)
{
	m_strSetName	= strFileName;
	m_nRecordCount	= 0;
	m_nRecordLength	= 1;
	m_nFirstOfs		= 0;
	m_nFieldCount	= 0;
}

/////////////////////////////////////////////////////////////////////
//添加字段，并自动计算偏移
/////////////////////////////////////////////////////////////////////
void CDdaSet::AddField(DB_FIELD* pField)
{
	int ofs = 0;
	for(int i=0; i<m_nFieldCount; i++)
		ofs += m_Field[i]->Len;
	pField->Ofs = ofs;
	m_Field.push_back(pField);
	m_nFieldCount++;
	m_nRecordLength += pField->Len;
}

/////////////////////////////////////////////////////////////////////
//添加记录
/////////////////////////////////////////////////////////////////////
void CDdaSet::AddRecord(DB_RECORD* pRecord)
{
	if(m_nFieldCount <=0)
	{
		return;
	}
	if(pRecord->sign != '*')
	{
        m_Record.push_back(pRecord);
		m_nUseRecordCount++;
        m_nRecordCount++;
	}
	return;
}

/////////////////////////////////////////////////////////////////////
//读取文件
/////////////////////////////////////////////////////////////////////
BOOL CDdaSet::LoadFromFile(STRING &strFileName)
{
    Clear();
    //by ldf 2003-12
    BOOL bRetOpen = Open(strFileName.c_str());
    m_strSetName = strFileName;
	
    if(!bRetOpen)
        return FALSE;

	DBF_HEAD head;
	bool bRet = m_File.read((void *)&head, sizeof(DBF_HEAD));
    if(!bRet)
        return FALSE;
	m_nRecordCount	= head.RecordNum;
	m_nRecordLength	= head.RecordLen;
	m_nFirstOfs		= head.FirstOfs;
	m_nFieldCount	= (head.FirstOfs - 33) / 32;

	int tempOfs		= 0;
	int i = 0;
	for(; i<m_nFieldCount; i++)
	{
		DBF_FIELD dbf_field;
		bRet = m_File.read((char*)&dbf_field, sizeof(DBF_FIELD));
		if(!bRet)
            return false;
        DB_FIELD* field	= new DB_FIELD;
		field->Name		= dbf_field.FieldName;
		field->Type		= dbf_field.FieldType;
		field->Ofs		= tempOfs;
		field->Len		= dbf_field.FieldLen;
		field->Dot		= dbf_field.FieldDot;
		tempOfs			+= field->Len;
		m_Field.push_back(field);
	}

	m_File.seek(m_nFirstOfs);
	int nDelCount = 0;
	for(i=0; i<m_nRecordCount; i++)
	{
		DB_RECORD* record = new(m_nRecordLength) DB_RECORD;
        bRet = m_File.read((void *)record, m_nRecordLength);
        if(!bRet)
            return FALSE;
        //注意即是有'*'的代删除记录，记录数还是不变的
		if(record->sign == ' ')
		{
			if(m_pQuery)
			{
				if(m_pQuery->RecordConform(record))
					m_Record.push_back(record);
				else
				{
					nDelCount++;
					delete record, record = NULL;
				}
			}else
				m_Record.push_back(record);
		}
		else if(record->sign == '*')
		{
			nDelCount++;
            delete record , record = NULL;
		}
	}
	m_nUseRecordCount = m_nRecordCount - nDelCount;
    m_File.close();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//保存文件
/////////////////////////////////////////////////////////////////////
BOOL CDdaSet::SyncToFile()
{

	if(!m_File.is_open())
    {
        int nRet = Open(m_strSetName.c_str(),"wb+");
        if(!nRet)
          return nRet;
    }
	
	//m_nFirstOfs = m_nFirstOfs + sizeof(DBF_HEAD) 
	//			  + m_Field.size()*sizeof(DBF_FIELD) + 1;
    m_nFirstOfs = sizeof(DBF_HEAD) 
				  + m_Field.size()*sizeof(DBF_FIELD) + 1;
	DBF_HEAD head;
	memset(&head, 0, sizeof(DBF_HEAD));
	head.Sign		= 0x03;

	time_t ltime = 0;
	time(&ltime);
	tm sTime = *localtime(&ltime);
	head.Year		= sTime.tm_year - 100;
	head.Month		= sTime.tm_mon + 1;
	head.Day		= sTime.tm_mday;
	head.RecordNum	= m_Record.size();
	head.FirstOfs	= m_nFirstOfs;
	head.RecordLen	= m_nRecordLength;
	m_File.seek(0); //设置到文件的开始
	if(!m_File.write((void *)&head, sizeof(DBF_HEAD)))
	{
		printf("%s\n",strerror(errno));
		return FALSE;
	}
	m_File.flush();

	int nTempOfs = 1;
	int i = 0;
    for(; i<m_nFieldCount; i++)
	{
		DBF_FIELD dbf_field;
		memset(&dbf_field, 0, sizeof(DBF_FIELD));
		memcpy(dbf_field.FieldName, (m_Field[i]->Name).c_str(), (m_Field[i]->Name).size());
        switch(m_Field[i]->Type)
		{
		case dbLogic:
			dbf_field.FieldType	= 'L';
			break;
		case dbChar:
			dbf_field.FieldType	= 'C';
			break;
		case dbNumber:
			dbf_field.FieldType	= 'N';
			break;
		case dbData:
			dbf_field.FieldType	= 'D';
			break;
		case dbMemo:
			dbf_field.FieldType	= 'M';
			break;
		case dbFloat: 
			dbf_field.FieldType	= 'F';
			break;
        default:
            //如果大于6，可能已经转换好了，就不必转换了
            dbf_field.FieldType = m_Field[i]->Type;
            break;
		}
		dbf_field.FieldOfs		= nTempOfs;
		dbf_field.FieldLen		= m_Field[i]->Len;
		dbf_field.FieldDot		= m_Field[i]->Dot;
		if(!m_File.write((char *)&dbf_field, sizeof(DBF_FIELD)))
		{
			printf("%s\n",strerror(errno));
		}
		nTempOfs += m_Field[i]->Len;
	}
	char Reserved = (char)0xD;
	m_File.write((void *)&Reserved,1);
	m_File.seek(m_nFirstOfs);

	int nDelCount =0;
    m_nRecordCount = m_Record.size();
	for(i=0; i<m_Record.size(); i++)
	{
		DB_RECORD* record = m_Record[i];
		if(record->sign == '*')
			nDelCount++;
        else if(!m_File.write((void *)record, m_nRecordLength))
            printf("%s\n",strerror(errno));
	}
    m_File.flush();
    long nPos = m_File.tell();
    Close(0);
    Truncate(nPos);
    m_nUseRecordCount = m_nRecordCount - nDelCount;
    return TRUE;
}

/////////////////////////////////////////////////////////////////////
//取使用的记录数
/////////////////////////////////////////////////////////////////////
int CDdaSet::GetRecordCount()
{
	return m_nUseRecordCount;
}

/////////////////////////////////////////////////////////////////////
//取字段数
/////////////////////////////////////////////////////////////////////
int CDdaSet::GetFieldCount()
{
	return m_nFieldCount;
}

/////////////////////////////////////////////////////////////////////
//取字段序号
/////////////////////////////////////////////////////////////////////
int CDdaSet::GetFieldIndex(const STRING &strFieldName)
{
	for(int i=0; i<m_nFieldCount; i++)
	{
		if(strFieldName.compare(m_Field[i]->Name) == 0)
			return i;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////
//取字段名字
/////////////////////////////////////////////////////////////////////
STRING CDdaSet::GetFieldName(int nField)
{
	if(nField<0 || nField>=m_nFieldCount)
		return STRING("");

	return m_Field[nField]->Name;
}

/////////////////////////////////////////////////////////////////////
//取字段名字
/////////////////////////////////////////////////////////////////////
STRING CDdaSet::GetFieldName(const char* lpszName)
{
	int index = GetFieldIndex(STRING(lpszName));
	return GetFieldName(index);
}

/////////////////////////////////////////////////////////////////////
//转换字段类型
/////////////////////////////////////////////////////////////////////
int CDdaSet::ConvFieldType(char cType)
{
	switch(cType)
	{
		case 'L': return dbLogic;
		case 'C': return dbChar;
		case 'N': return dbNumber;
		case 'D': return dbData;
		case 'M': return dbMemo;
		case 'F': return dbFloat;
		default:  return dbNULL;
	}
}

/////////////////////////////////////////////////////////////////////
//取字段类型
/////////////////////////////////////////////////////////////////////
int CDdaSet::GetFieldType(int nField)
{
	if(nField<0 || nField>=m_nFieldCount)
		return dbNULL;

	return ConvFieldType(m_Field[nField]->Type);
}

/////////////////////////////////////////////////////////////////////
//取字段类型
/////////////////////////////////////////////////////////////////////
int CDdaSet::GetFieldType(const char* lpszName)
{
	int index = GetFieldIndex(STRING(lpszName));
	return GetFieldType(index);
}

///////////////////////////////////////////////////////////////////
// read function
///////////////////////////////////////////////////////////////////
STRING CDdaSet::GetField(int nItem, int nField)
{
	if(nItem<0 || nItem>=m_nUseRecordCount ||
	   nField<0 || nField>=m_nFieldCount)
		return STRING("");

    char buffer[MAX_FIELD_LEN] = {0};
	int FieldOfs = m_Field[nField]->Ofs;
	int FieldLen = m_Field[nField]->Len;
    memset(buffer,0,MAX_FIELD_LEN);
	memcpy(buffer, m_Record[nItem]->data + FieldOfs, FieldLen);
/*	STRING strField("");
	int pos = 0;
	while(pos<FieldLen && buffer[pos] != 0)
	{
		strField += buffer[pos];
		pos++;
	}
    */
    string_lr_trim(buffer);

	return STRING(buffer);
}

STRING CDdaSet::GetFieldValue(int nItem, int nField)
{
	if(nItem<0 || nItem>=m_nUseRecordCount ||
	   nField<0 || nField>=m_nFieldCount)
		return STRING("");

	int item;
	if(m_nCurIndex == dbNoneIndex)
		item = nItem;
	else
		item = (*m_Index[m_nCurIndex])[nItem];

	char buffer[MAX_FIELD_LEN];
	int FieldOfs = m_Field[nField]->Ofs;
	int FieldLen = m_Field[nField]->Len;
    memset(buffer,0,MAX_FIELD_LEN);
	memcpy(buffer, m_Record[item]->data + FieldOfs, FieldLen);
	string_lr_trim(buffer);
	return STRING(buffer);
}

STRING CDdaSet::GetFieldValue(DB_RECORD* pRecord, int nField)
{
	if(nField<0 || nField>=m_nFieldCount)
		return STRING("");

	char buffer[MAX_FIELD_LEN];
	int FieldOfs = m_Field[nField]->Ofs;
	int FieldLen = m_Field[nField]->Len;
    memset(buffer,0,MAX_FIELD_LEN);
	memcpy(buffer, pRecord->data + FieldOfs, FieldLen);
	string_lr_trim(buffer);
	return STRING(buffer);
}

STRING CDdaSet::GetFieldValue(DB_RECORD* pRecord, const char* lpszFieldName)
{
	int index = GetFieldIndex(STRING(lpszFieldName));
	return GetFieldValue(pRecord, index);
}

BOOL CDdaSet::GetFieldBoolValue(int nItem, int nField)
{
	return (GetFieldValue(nItem, nField).compare("T") == 0);
}

BOOL CDdaSet::GetFieldBoolValue(int nItem, const char* lpszFieldName)
{
	int index = GetFieldIndex(STRING(lpszFieldName));
	return (GetFieldValue(nItem, index).compare("T") == 0);
}

int CDdaSet::GetFieldIntValue(int nItem, int nField)
{
	return atoi(GetFieldValue(nItem, nField).c_str());
}

int CDdaSet::GetFieldIntValue(int nItem, const char* lpszFieldName)
{
	int index = GetFieldIndex(STRING(lpszFieldName));
	return atoi(GetFieldValue(nItem, index).c_str());
}

STRING CDdaSet::GetFieldStringValue(int nItem, int nField)
{
	return GetFieldValue(nItem, nField);
}

STRING CDdaSet::GetFieldStringValue(int nItem, const char* lpszFieldName)
{
	int index = GetFieldIndex(STRING(lpszFieldName));
	return GetFieldValue(nItem, index);
}

///////////////////////////////////////////////////////////////////
// write function
///////////////////////////////////////////////////////////////////
void CDdaSet::SetField(int nItem, int nField, STRING &strField)
{
	if(nItem<0 || nItem>=m_nUseRecordCount ||
	   nField<0 || nField>=m_nFieldCount)
		return;

	int FieldOfs	= m_Field[nField]->Ofs;
	int FieldLen	= m_Field[nField]->Len;
	int Len			= strField.size();
	
	if(Len > m_Field[nField]->Len)
		Len	= m_Field[nField]->Len;

	memset(m_Record[nItem]->data + FieldOfs, 0, FieldLen);
	memcpy(m_Record[nItem]->data + FieldOfs, strField.c_str(), Len);
}

void CDdaSet::SetFieldValue(int nItem, int nField, const STRING &strField)
{
	if(nItem<0 || nItem>=m_nUseRecordCount ||
	   nField<0 || nField>=m_nFieldCount)
		return;

	int item;
	if(m_nCurIndex == dbNoneIndex)
		item = nItem;
	else
		item = (*m_Index[m_nCurIndex])[nItem];

	int FieldOfs	= m_Field[nField]->Ofs;
	int FieldLen	= m_Field[nField]->Len;
	int Len			= strField.size();
	if(Len > m_Field[nField]->Len)
		Len	= m_Field[nField]->Len;

	memset(m_Record[item]->data + FieldOfs, 0, FieldLen);
	memcpy(m_Record[item]->data + FieldOfs, strField.c_str(), Len);
}

void CDdaSet::SetFieldBoolValue(int nItem, int nField, BOOL bField)
{
	SetFieldValue(nItem, nField, bField ? STRING("T") : STRING("F"));
}

void CDdaSet::SetFieldBoolValue(int nItem, const char *lpszFieldName, BOOL bField)
{
	int index = GetFieldIndex(STRING(lpszFieldName));
	SetFieldValue(nItem, index, bField ? STRING("T") : STRING("F"));
}

void CDdaSet::SetFieldIntValue(int nItem, int nField, int iField)
{
	char szTemp[64];
	sprintf(szTemp,"%d",iField);
	STRING strField(szTemp);
	int FieldLen = m_Field[nField]->Len;

	while(strField.size() < FieldLen)
	{
		strField = STRING(" ") + strField;

	}
	
	
	SetFieldValue(nItem, nField, strField);
}

void CDdaSet::SetFieldIntValue(int nItem, const char* lpszFieldName, int iField)
{
	int index = GetFieldIndex(STRING(lpszFieldName));
	char szTemp[64];
	sprintf(szTemp,"%d",iField);
	STRING strField(szTemp);

	int FieldLen = m_Field[index]->Len;
	
	while(strField.size() < FieldLen)
	{
		strField = STRING(" ") + strField;
	}

	SetFieldValue(nItem, index, strField);
}

void CDdaSet::SetFieldStringValue(int nItem, int nField, STRING &strField)
{
	SetFieldValue(nItem, nField, strField);
}

void CDdaSet::SetFieldStringValue(int nItem, const char *lpszFieldName, STRING &strField)
{
	int index = GetFieldIndex(STRING(lpszFieldName));
	SetFieldValue(nItem, index, strField);
}

///////////////////////////////////////////////////////////////////
// index function
///////////////////////////////////////////////////////////////////
void CDdaSet::UseIndex(int nIndex)
{
	if(nIndex < -1 || nIndex >= m_Index.size())
		return;
	m_nCurIndex = nIndex;
}

void CDdaSet::QuickSort(int L, int R)
{
	unsigned int I, J, Temp;
	STRING  X;

	I = L;
	J = R;
	X = GetField(m_nSortField, (L + R) / 2);

	do
	{
		while(GetField(m_nSortField, I) < X) { I++; }
		while(X < GetField(m_nSortField, J)) { J--; }
		if(I <= J)
		{
			int index = m_Index.size() - 1;
			UINTArray *pUA = m_Index[index];
			Temp = (*pUA)[I];
			(*pUA)[I] = (*pUA)[J];
			(*pUA)[J] = Temp;
			I++;
			J--;
		}
	}while(I <= J);

	if(L < J)
		QuickSort(L, J);
	if(I < R)
		QuickSort(I, R);
}

int CDdaSet::CreateIndex(int nField)
{
	if(nField < 0 || nField >= m_nFieldCount)
		return -1;

	m_nSortField		= nField;
	UINTArray* pIndex	= new UINTArray();
	
	for(int i=0; i<m_nUseRecordCount; i++)
		pIndex->push_back(i);
	m_Index.push_back(pIndex);

	QuickSort(0, m_nUseRecordCount - 1);

	return (m_Index.size() - 1);
}

int CDdaSet::CreateIndex(const char* lpszFieldName)
{
	int index = GetFieldIndex(STRING(lpszFieldName));
	return CreateIndex(index);
}

void CDdaSet::Clear()
{
	int i=0;
	for(i=0; i<m_Field.size(); i++)
		delete m_Field[i], m_Field[i] = NULL;
	for(i=0; i<m_Record.size(); i++)
		delete m_Record[i], m_Record[i] = NULL;
	for(i=0; i<m_Index.size(); i++)
		delete m_Index[i], m_Index[i] = NULL;

	m_Field.clear();
	m_Record.clear();
	m_Index.clear();
    m_bModifyFlag		= FALSE;
	m_nRecordCount		= 0;
	m_nUseRecordCount	= 0;
	m_nFieldCount		= 0;
	m_nCurIndex			= dbNoneIndex;
	m_pQuery			= NULL;
	m_nRecordLength     = 1; //任何记录都至少有一个标记
	m_nFirstOfs			= 0;
}

int CDdaSet::AppendRecordToDisk(DB_RECORD *pRecord)
{

	if(pRecord->sign == '*')
		return TRUE;
    if(!m_File.is_open())
    {
        if(!Open(m_strSetName.c_str(),"rb+"))
            return FALSE;
    }
    DBF_HEAD head; 
    m_File.seek(0);
    bool bRet = m_File.read((void *)&head,sizeof(DBF_HEAD));
    if(!bRet)
        return FALSE;
    head.RecordNum += 1;
    m_File.seek(0);
    if(!m_File.write((void *)&head,sizeof(DBF_HEAD)))
        return FALSE;
	m_File.seek(0,SEEK_END);
	if(!m_File.write((void *)pRecord, head.RecordLen))
	{
		return FALSE;
	}
    m_File.flush();
	m_Record.push_back(pRecord);
	m_nUseRecordCount++;
	m_nRecordCount++;
    Close(0);
	return TRUE;
}

int CDdaSet::DeleteRecord(unsigned int nItem)
{
    if(nItem < m_Record.size())
    {
        VECTOR<DB_RECORD*>::iterator it = m_Record.begin();
        it += nItem;
        delete (*it), (*it) = NULL;
        m_Record.erase(it);
        m_nUseRecordCount --;
        m_nRecordCount --;
    }
    return TRUE;
}
/****************************************************************************
 *函数功能：字符串大小写不敏感比较
 *输入：
 *     szStr1：需要比较得字符串1
 *     szStr2：需要比较得字符串2
 *输出：
 *     字符串是否相等, 0－相等
 ****************************************************************************/
int string_nocase_compare(const char *szStr1, const char *szStr2)
{
    const char *szTemp1 = szStr1;
    const char *szTemp2 = szStr2;
    char cChar1;
    char cChar2;
    char cDiff = 'a' - 'A';
	if(strlen(szStr1) != strlen(szStr2))
	{
		return (strlen(szStr1) - strlen(szStr2));
	}

    while(*szTemp1 != '\0')
    {
        cChar1 = *szTemp1;
        cChar2 = *szTemp2;

        szTemp1++;
        szTemp2++;

        if (cChar1 >= 'a' && cChar1 <= 'z')
        {
              cChar1 -= cDiff;  //转换成大写
        }

        if (cChar2 >= 'a' && cChar2 <= 'z')
        {
              cChar2 -= cDiff;  //转换成大写
        }

        if (cChar1 != cChar2)
        {
            return (cChar1 - cChar2);
        }
    }

    return 0;
}

/***************************************************************************
 *函数功能：字符串大小写敏感比较
 *输入：
 *     szS1：需要比较得字符串1
 *     szS2：需要比较得字符串2
 *输出：
 *     字符串是否相等, 0－相等
 ***************************************************************************/
int string_compare(const char *szS1,  const char *szS2)
{
    const char *szTmp1 = szS1;
    const char *szTmp2 = szS2;
  
	if(strlen(szS1) != strlen(szS2))
	{
		return (strlen(szS1) - strlen(szS2));
	}

    while (*szTmp1 != '\0')
    {
        if (*szTmp1 == *szTmp2)
        {
            szTmp1++;
            szTmp2++;
        }
        else
        {
            return (*szTmp1 - *szTmp2);
        }
    }
    return 0;
}

/**************************************************************************
 *函数功能：将一个字符串全部转换成大写
 *输入：
 *     szStr：待转换的字符串
 *输出：
 *     szStr：转换完成的字符串
 **************************************************************************/
void string_to_upper(char *szStr)
{
    char cDiff = 'a' - 'A';

    while (*szStr)
    {
        if (*szStr >= 'a' && *szStr <= 'z')
        {
              *szStr -= cDiff;
        }

        szStr++;
    }
}

void string_to_lower(char *szStr)
{
    char cDiff = 'a' - 'A';

    while (*szStr)
    {
        if (*szStr >= 'A' && *szStr <= 'Z')
        {
              *szStr += cDiff;
        }

        szStr++;
    }   
}

/***************************************************************************
 *函数功能：去掉字符串左边的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 **************************************************************************/
void string_left_trim(char *szStr)
{
    if(strcmp(szStr,"")==0)
        return;
    char* szTemp = NULL;
    szTemp = szStr;

    while ((*szTemp == ' ')  || (*szTemp == '\t') 
        || (*szTemp == '\r') || (*szTemp == '\n'))
    {
        szTemp++;
    }

    strcpy(szStr, szTemp);
}

/************************************************************************
 *函数功能：去掉字符串右边的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 ************************************************************************/
void string_right_trim(char *szStr)
{
    if(strcmp(szStr,"")==0)
        return;
    char *szTemp = NULL;
    char ch;
    int  nLen;

    nLen = strlen(szStr);

    szTemp = szStr + nLen;

    while (szTemp != szStr)
    {
        szTemp--;
        ch = *szTemp;
        if ((ch == ' ') || (ch == '\n') 
         || (ch == '\t') || (ch == '\r'))
        {
            *szTemp = '\0';
        }
        else
        {
            break;
        }
    }
}

/*************************************************************************
 *函数功能：去掉字符串左右的空格
 *输入：
 *     szStr：待处理的字符串
 *输出：
 *     szStr：处理完成的字符串
 *************************************************************************/
void string_lr_trim(char *szStr)
{
    string_left_trim(szStr);
    string_right_trim(szStr);
}

bool is_digit_value(const char *szValue)
{
    char ch;

    ch = *szValue;

    //字符串为空
    if (ch == '\0') return false;

    while (ch != '\0')
    {
    if (ch < '0' || ch > '9')
        {
            return (false);
        }

    ch = *(++szValue);
    }

    return (true);
}
//////////////////////////////////////////////////////////////////////
// CDBFFile Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBFFile::CDBFFile()
{
    m_pFile = NULL;
}

CDBFFile::~CDBFFile()
{
    if(m_pFile)
        fclose(m_pFile);
}

bool CDBFFile::is_open()
{
    return (m_pFile != NULL);
}


void CDBFFile::close()
{
    if(m_pFile)
    {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

bool CDBFFile::open(const char *szFileName, const char *szFlag)
{
    if(m_pFile)
        return false;
    m_pFile = fopen(szFileName,szFlag);
    if(m_pFile == NULL)
        return false;
    return true;
}

bool CDBFFile::read(void *Buffer, int nSize, int nCount)
{
    if(!m_pFile)
        return false;
    int nRet = fread(Buffer,nSize,nCount,m_pFile);
    return nRet == nCount;
}

int CDBFFile::seek(long Offset, int nPos)
{
    if(!m_pFile)
        return -1;
    return fseek(m_pFile,Offset,nPos);
}

bool CDBFFile::write(void *Buffer, int nSize, int nCount)
{
    if(!m_pFile)
        return false;
    int nRet = fwrite(Buffer,nSize,nCount,m_pFile);
    return nRet == nCount;
}

void CDBFFile::flush()
{
    fflush(m_pFile);
}

long CDBFFile::tell()
{
    fseek(m_pFile,0,SEEK_END);
    return ftell(m_pFile);
}

int CDdaSet::UpdateRecord(int nItem)
{
    Close(0);
    if(Open(m_strSetName.c_str(),"rb+"))
    {
        long Offset = m_nFirstOfs + m_nRecordLength * nItem;
        m_File.seek(Offset);
        bool bRet = m_File.write(m_Record[nItem],m_nRecordLength);
        m_File.close();
        if(!bRet)
            return FALSE;
        return TRUE;
    }
    return FALSE;
}
