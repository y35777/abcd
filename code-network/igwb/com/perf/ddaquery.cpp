// DdaQuery.cpp: implementation of the CDdaQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "ddaquery.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDdaQuery::CDdaQuery()
{
}

CDdaQuery::~CDdaQuery()
{
	ClearBind();
}

CDdaQuery::CDdaQuery(CDdaSet* pSet)
{
	m_pSet = pSet;
}

/////////////////////////////////////////////////////////////////////
//与一个数据表建立关联
/////////////////////////////////////////////////////////////////////
void CDdaQuery::Attach(CDdaSet* pSet, BOOL bClear)
{
	m_pSet = pSet;
	if(bClear)
		ClearBind();
}

/////////////////////////////////////////////////////////////////////
//清除所有约束信息
/////////////////////////////////////////////////////////////////////
void CDdaQuery::ClearBind()
{
    int i = 0;
	for(i=0; i<m_BindAnd.size(); i++)
		delete m_BindAnd[i], m_BindAnd[i] = NULL;
	for(i=0; i<m_BindOr.size(); i++)
		delete m_BindOr[i], m_BindAnd[i] = NULL;

	m_BindAnd.clear();
	m_BindOr.clear();
}	

/////////////////////////////////////////////////////////////////////
//向与队列添加约束
/////////////////////////////////////////////////////////////////////
void CDdaQuery::AddBindAnd(STRING FieldName, STRING BindValue, int BindOp)
{
	BIND_ITEM* bind	= new BIND_ITEM;
	bind->FieldName	= FieldName;
	bind->BindValue	= BindValue;
	bind->BindOp	= BindOp;
	m_BindAnd.push_back(bind);
}

/////////////////////////////////////////////////////////////////////
//向与队列添加约束
/////////////////////////////////////////////////////////////////////
void CDdaQuery::AddBindAnd(STRING FieldName, int BindValue, int BindOp)
{
	char strVal[64];
	SNPRINTF(strVal,64,"%d",BindValue);
    strVal[63] = '\0';
	AddBindAnd(FieldName, STRING(strVal), BindOp);
}

/////////////////////////////////////////////////////////////////////
//向或队列添加约束
/////////////////////////////////////////////////////////////////////
void CDdaQuery::AddBindOr(STRING FieldName, STRING BindValue, int BindOp)
{
	BIND_ITEM* bind	= new BIND_ITEM;
	bind->FieldName	= FieldName;
	bind->BindValue	= BindValue;
	bind->BindOp	= BindOp;
	m_BindOr.push_back(bind);
}

/////////////////////////////////////////////////////////////////////
//向或队列添加约束
/////////////////////////////////////////////////////////////////////
void CDdaQuery::AddBindOr(STRING FieldName, int BindValue, int BindOp)
{
	char strVal[64];
	sprintf(strVal,"%d",BindValue);
	AddBindOr(FieldName, strVal, BindOp);
}

/////////////////////////////////////////////////////////////////////
//执行查询,返回所有符合条件的记录
/////////////////////////////////////////////////////////////////////
int CDdaQuery::DoQuery()
{
    m_ResultArray.clear();

	if(m_pSet == NULL)
		return -1;

	int m_size = m_pSet->GetRecordCount();
	for(int i=0; i < m_pSet->GetRecordCount(); i++)
	{
		if(RecordConform(i))
			m_ResultArray.push_back(i);
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////
//执行查询,返回第一条符合条件的记录
/////////////////////////////////////////////////////////////////////
int CDdaQuery::QueryFirst()
{
    if(m_pSet == NULL)
        return -1;
    m_ResultArray.clear();
    int  m_size = m_pSet->GetRecordCount();
    for(int i=0; i<m_pSet->GetRecordCount(); i++)
    {
        if(RecordConform(i))
        {
            m_ResultArray.push_back(i);
            return i;
        }
    }
    return -1;
}

/////////////////////////////////////////////////////////////////////
//判定一条记录是否符合条件
/////////////////////////////////////////////////////////////////////
BOOL CDdaQuery::RecordConform(int nItem)
{
    int i = 0;
	for(; i<m_BindAnd.size(); i++)
	{
		BIND_ITEM* bind = m_BindAnd[i];
		if(!BindConfirm(nItem, bind))
			return FALSE;
	}

	if(m_BindOr.size() == 0)
		return TRUE;

	BOOL bRes = FALSE;
	for(i=0; i<m_BindOr.size(); i++)
	{
		BIND_ITEM* bind = m_BindOr[i];
		bRes |= BindConfirm(nItem, bind);
	}

	return bRes;
}

/////////////////////////////////////////////////////////////////////
//判断一条记录是否符合一个约束
/////////////////////////////////////////////////////////////////////		
BOOL CDdaQuery::BindConfirm(int nItem, BIND_ITEM* bind)
{
	if(m_pSet == NULL)
		return FALSE;

	STRING strVal = m_pSet->GetFieldStringValue(nItem,(bind->FieldName).c_str());
	char *pTmp = const_cast<char *>(strVal.c_str());
	string_lr_trim(pTmp);

	BOOL bRes = FALSE; 
	switch(bind->BindOp)
	{
		case opEqual:	
			bRes = (string_nocase_compare(strVal.c_str(),bind->BindValue.c_str()) == 0);	
			break;
		case opUnEqual:	
			bRes = (string_nocase_compare(strVal.c_str(),bind->BindValue.c_str()) != 0);	
			break;
		case opGreater:	
			bRes = (string_nocase_compare(strVal.c_str(),bind->BindValue.c_str()) > 0);	
			break;
		case opLess:	
			bRes = (string_nocase_compare(strVal.c_str(),bind->BindValue.c_str()) < 0);	
			break;
		case opEqGtr:	
			bRes = (string_nocase_compare(strVal.c_str(),bind->BindValue.c_str()) >= 0);	
			break;
		case opEqLess:	
			bRes = (string_nocase_compare(strVal.c_str(),bind->BindValue.c_str()) <= 0);	
			break;
        case opSet:
            bRes = Chaos(bind->BindValue.c_str(),strVal.c_str());
            break;

	}

	return (BOOL)bRes;
}

/////////////////////////////////////////////////////////////////////
//判定一条记录是否符合条件(输入的记录)
/////////////////////////////////////////////////////////////////////
BOOL CDdaQuery::RecordConform(DB_RECORD* pRecord)
{
    int i = 0;
	for(; i<m_BindAnd.size(); i++)
	{
		BIND_ITEM* bind = m_BindAnd[i];
		if(!BindConfirm(pRecord, bind))
			return FALSE;
	}

	if(m_BindOr.size() == 0)
		return TRUE;

	BOOL bRes = FALSE;
	for(i = 0; i<m_BindOr.size(); i++)
	{
		BIND_ITEM* bind = m_BindOr[i];
		bRes |= BindConfirm(pRecord, bind);
	}

	return bRes;
}

/////////////////////////////////////////////////////////////////////
//判断一条记录是否符合一个约束(输入的记录)
/////////////////////////////////////////////////////////////////////		
BOOL CDdaQuery::BindConfirm(DB_RECORD* pRecord, BIND_ITEM* bind)
{
    if(m_pSet == NULL)
        return FALSE;
    BOOL bRes = FALSE;
    STRING strVal = m_pSet->GetFieldValue(pRecord, (bind->FieldName).c_str());

    switch(bind->BindOp)
	{
    case opEqual:	
        return (strVal == bind->BindValue);
    case opUnEqual:	
        return (strVal != bind->BindValue);	
    case opGreater:	
        return (strVal > bind->BindValue);	
    case opLess:	
        return (strVal < bind->BindValue);	
    case opEqGtr:	
        return (strVal >= bind->BindValue);	
    case opEqLess:	
        return (strVal <= bind->BindValue);	
    case opSet:
        bRes = Chaos(bind->BindValue.c_str(),strVal.c_str());
        return bRes;
	}
	return FALSE;
}

int CDdaQuery::Chaos(const char *pLeft,const char *pRight, const char *pSeps)
{
    if(strlen(pLeft) > strlen(pRight))
        return FALSE;
    VECTOR<int> v_chaos_left;
    VECTOR<int> v_chaos_right;
    int nLeft = strlen(pLeft) + 1;
    int nRight = strlen(pRight) +1 ;
    char *szLeft = new char[nLeft];
    char *szRight = new char[nRight];
    strncpy(szLeft,pLeft,nLeft);
    strncpy(szRight,pRight,nRight);
    int nValue = 0; 
    //将左值分解成数组
    char *pResult = strtok(szLeft,pSeps);
    while(pResult != NULL)
    {
        nValue = atoi(pResult);
        v_chaos_left.push_back(nValue);
        pResult = strtok(NULL,pSeps);
    }
    //将右值分解成数组
    pResult = strtok(szRight,pSeps);
    while(pResult != NULL)
    {
        nValue = atoi(pResult);
        v_chaos_right.push_back(nValue);
        pResult = strtok(NULL,pSeps);
    }
    VECTOR<int>::iterator it;
    for(int i = 0; i < v_chaos_left.size(); i++)
    {
        it = std::find(v_chaos_right.begin(),v_chaos_right.end(),v_chaos_left[i]);
        if(it == v_chaos_right.end())
        {
            delete[] szRight,szRight = NULL;
            delete[] szLeft, szLeft = NULL;
            return FALSE;
        }

    }
    delete[] szRight,szRight = NULL;
    delete[] szLeft, szLeft = NULL;
    return TRUE;
}
