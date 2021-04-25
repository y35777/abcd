//////////////////////////////////////////////////////////////////////
// DdaQuery.h: interface for the CDdaQuery class.
// DdaQuery : Direct Data Access Query Object
//////////////////////////////////////////////////////////////////////

#ifndef _DDA_QUERY_H_
#define _DDA_QUERY_H_

#include "../include/base_type.h"
#include "ddaset.h"
#include <algorithm>

// define operator
#define opEqual		1	// '='
#define opUnEqual	2	// '!='
#define opGreater	3	// '>'
#define opLess		4	// '<'
#define opEqGtr		5	// '>='
#define opEqLess	6	// '<='
#define opSet     7   // 混沌运算'1&2&3' {- ('1&2&3&4' == '1&3&4&2')

#define opAnd		20	// '&'
#define opOr		21	// '|'

// define item type
#define IT_BIND		1	// 约束项
#define IT_RELATION	2	// 关系项

typedef struct
{
	STRING FieldName;		// 字段名
	STRING BindValue;		// 约束值
	int BindOp;				// 关系运算符
}BIND_ITEM; // 约束项结构

typedef struct
{
	int ItemType;			// 项类型
	void* Item;				// 项指针
}RELATION_POINTER; // 关系项元素

typedef struct
{
	RELATION_POINTER lRelation; // 左关系项
	RELATION_POINTER rRelation; // 右关系项
	int LogicOp;				// 逻辑运算符
}RELATION_ITEM; // 关系项结构


typedef VECTOR<BIND_ITEM*> CBindArray;
typedef VECTOR<RELATION_ITEM*> CRelationArray;


class CDdaQuery  
{
// attribute
public:
	CDdaSet*	m_pSet;			//关联的DdaSet指针
    UINTArray   m_ResultArray;  //结果数组
	VECTOR<BIND_ITEM*>	m_BindAnd;		//与数组
	VECTOR<BIND_ITEM*>	m_BindOr;		//或数组
	int Chaos(const char *pLeft, const char *pRight, const char *pSeps = "&");	

public:
	CDdaQuery();
	CDdaQuery(CDdaSet* pSet);
	virtual ~CDdaQuery();

	void        Attach(CDdaSet* pSet, BOOL bClear = TRUE);
	void        ClearBind();	//清除与,或数组
	void        AddBindAnd(STRING FieldName, STRING BindValue, int BindOp);
	void        AddBindAnd(STRING FieldName, int BindValue, int BindOp);
	void        AddBindOr(STRING FieldName, STRING BindValue, int BindOp);
	void        AddBindOr(STRING FieldName, int BindValue, int BindOp);
	int	        DoQuery();			// 查询所有符合条件记录
	int	        QueryFirst();		// 查询第一条符合条件记录
	BOOL        RecordConform(int nItem);
	BOOL        RecordConform(DB_RECORD* pRecord);
	BOOL        BindConfirm(int nItem, BIND_ITEM* bind);
	BOOL        BindConfirm(DB_RECORD* pRecord, BIND_ITEM* bind);
};

#endif //_DDA_QUERY_H
