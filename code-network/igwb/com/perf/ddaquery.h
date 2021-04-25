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
#define opSet     7   // ��������'1&2&3' {- ('1&2&3&4' == '1&3&4&2')

#define opAnd		20	// '&'
#define opOr		21	// '|'

// define item type
#define IT_BIND		1	// Լ����
#define IT_RELATION	2	// ��ϵ��

typedef struct
{
	STRING FieldName;		// �ֶ���
	STRING BindValue;		// Լ��ֵ
	int BindOp;				// ��ϵ�����
}BIND_ITEM; // Լ����ṹ

typedef struct
{
	int ItemType;			// ������
	void* Item;				// ��ָ��
}RELATION_POINTER; // ��ϵ��Ԫ��

typedef struct
{
	RELATION_POINTER lRelation; // ���ϵ��
	RELATION_POINTER rRelation; // �ҹ�ϵ��
	int LogicOp;				// �߼������
}RELATION_ITEM; // ��ϵ��ṹ


typedef VECTOR<BIND_ITEM*> CBindArray;
typedef VECTOR<RELATION_ITEM*> CRelationArray;


class CDdaQuery  
{
// attribute
public:
	CDdaSet*	m_pSet;			//������DdaSetָ��
    UINTArray   m_ResultArray;  //�������
	VECTOR<BIND_ITEM*>	m_BindAnd;		//������
	VECTOR<BIND_ITEM*>	m_BindOr;		//������
	int Chaos(const char *pLeft, const char *pRight, const char *pSeps = "&");	

public:
	CDdaQuery();
	CDdaQuery(CDdaSet* pSet);
	virtual ~CDdaQuery();

	void        Attach(CDdaSet* pSet, BOOL bClear = TRUE);
	void        ClearBind();	//�����,������
	void        AddBindAnd(STRING FieldName, STRING BindValue, int BindOp);
	void        AddBindAnd(STRING FieldName, int BindValue, int BindOp);
	void        AddBindOr(STRING FieldName, STRING BindValue, int BindOp);
	void        AddBindOr(STRING FieldName, int BindValue, int BindOp);
	int	        DoQuery();			// ��ѯ���з���������¼
	int	        QueryFirst();		// ��ѯ��һ������������¼
	BOOL        RecordConform(int nItem);
	BOOL        RecordConform(DB_RECORD* pRecord);
	BOOL        BindConfirm(int nItem, BIND_ITEM* bind);
	BOOL        BindConfirm(DB_RECORD* pRecord, BIND_ITEM* bind);
};

#endif //_DDA_QUERY_H
