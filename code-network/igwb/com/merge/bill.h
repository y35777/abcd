
#ifndef BILL_H
#define BILL_H
#pragma warning(disable:4100)

#include <iostream.h>
#include "../include/type_def.h"
#include "../utility/config.h"

typedef UINT4           PackID_t;


/*
    HASH表使用CKey对象进行索引。
    所有话单类都必须重这个类继承自己的子类，
    并实现GetKey和operator==方法。
    CKey对象包含一个CKey子类对象的指针，完成实际的计算工作。

    HASH表使用CKey对象作为索引键，而非CKey对象的指针。
    
    包含指针的对象，若使用copy方式传递值，
    则必须定义copy构造函数和赋值运算符(=)。
*/

class IGWB_Svc_Export CKey
{
public:

    //实际完成计算的对象指针。
    CKey* m_pDelegation;

public:

    //返回索引值。
    virtual UINT4 GetKey();

    //判断两个CKey对象是否相同。
    virtual BOOL operator==(CKey& Key);

    virtual BOOL IsMatch(CKey* pKey);

    CKey();

    /*
        Copy构造函数，当CKey对象被以Copy的方式传值时，
        传递m_pDelegation指针。
    */
    CKey(const CKey& key);

    virtual ~CKey();

    //赋值运算符，当CKey对象被赋值时用于传递m_pDelegation指针。
    void operator=(CKey& key);
};

/*
    CBill类是一个基础类，他提供话单操作的接口，
    所有的实现都必须在子类中完成。
*/
class IGWB_Svc_Export CBill
{
public:
    CBill();

    virtual ~CBill();

    //使用话单记录初始化话单对象。
    virtual BOOL Init(char* pBuf,UINT4 uLen) = 0;

    //判断当前话单是否允许与pBill合并。
    virtual BOOL CanSafeMerge(CBill* pBill) = 0;

    //把pBill合入当前话单中。
    virtual BOOL Merge(CBill* pBill) = 0;

    //判断PDP已经结束。
    virtual BOOL IsPDPCompleted() = 0;

    //判断是否需要继续等待后续话单。
    virtual BOOL NeedWaiting() = 0;

    //返回话单的记录结构。
    virtual char* GetContents() = 0;

    //判断是否部分话单。
    virtual BOOL IsPartial() = 0;
    
    //判断是否是热计费话单
    virtual BOOL IsHotBilling()
    {
        return FALSE;
    };

    //判断是否PDP的第一张部分话单。
    virtual BOOL IsFirstPartialRec() = 0;

    //判断是否PDP的末张部分话单。
    virtual BOOL IsLastPartialRec() = 0;

    //判断是否重单
    virtual BOOL IsDuplicated(CBill* pBill) = 0;    

    //返回话单类型ID值。
    virtual UINT4 GetBillType() = 0;

    //创建索引对象。
    virtual CKey CreateKey() = 0;

    //获取话单的长度。
    virtual INT4 GetLength();
	
	//Z30971增加是否支持夏时制接口
	//设置合并Duration是否支持夏时制
	virtual void SetDSTFlag(BOOL bFlag = FALSE);
	
	//Added by ZhengYuqun 2003-11-04 SWPD02238
    virtual void SetMaxDuration(UINT4 uMaxDuration);

protected:
	BOOL m_bDSTFlg;//夏时制标志
    UINT4 m_uMaxDuration; //Added by ZhengYuqun 2003-11-04 SPWD02238
        	
};

/*
    CBillParser管理话单类型和话单结构的细节。
    针对不同的话单类型，程序必须继承CBillParser的子类，
    实现话单类型的管理和话单结构的解析。
*/
class CMerger;

class IGWB_Svc_Export CBillParser
{
protected:
    CBill* m_pBill;

public:
    CBillParser();

    virtual ~CBillParser();

    //根据pBuf生成一个话单对象。
    virtual BOOL ParseARecord(char* pBuf,UINT4 uBufLen) = 0;

    //返回话单记录对象。
    virtual CBill* GetBillRecord() = 0;

    //返回话单类型数。
    virtual UINT4 GetBillTypeCount() = 0;

    //根据索引返回话单类型ID。
    virtual UINT4 GetBillTypeID(UINT4 uIndex) = 0;

    //在CMerger对象中注册话单类型。
    virtual void RegisterBillType(CMerger* pMerger) = 0;

	//颜文远2002-03-06添加
	//获得指定的话单类型ID的话单类型描述
	virtual void GetBillTypeDesc(UINT4 uBillTypeID, char* szDesc) = 0;
	//添加结束，颜文远2002-03-06
};

#endif

