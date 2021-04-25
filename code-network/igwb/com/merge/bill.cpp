#include "../include/type_def.h"
#include "../utility/config.h"
#include "../include/export_dll.h"
#include "bill.h"

/*
    HASH表使用CKey对象进行索引。
    所有话单类都必须重这个类继承自己的子类，
    并实现GetKey和operator==方法。
    CKey对象包含一个CKey子类对象的指针，完成实际的计算工作。

    HASH表使用CKey对象作为索引键，而非CKey对象的指针。
    
    包含指针的对象，若使用copy方式传递值，
    则必须定义copy构造函数和赋值运算符(=)。
*/

UINT4 CKey::GetKey()
{
    return m_pDelegation->GetKey();
}

//判断两个CKey对象是否相同。
BOOL CKey::operator==(CKey& Key)
{
    return m_pDelegation->IsMatch(Key.m_pDelegation);
}

BOOL CKey::IsMatch(CKey* pKey)
{
    return FALSE;
}

CKey::CKey()
{
    m_pDelegation = NULL;
}

/*
    Copy构造函数，当CKey对象被以Copy的方式传值时，
    传递m_pDelegation指针。
*/
CKey::CKey(const CKey& key)
{
    m_pDelegation = key.m_pDelegation;
    ((CKey&)key).m_pDelegation = NULL;
}

CKey::~CKey()
{
    if(m_pDelegation != NULL)
    {
        delete m_pDelegation;
        m_pDelegation = NULL;
    }
}

//赋值运算符，当CKey对象被赋值时用于传递m_pDelegation指针。
void CKey::operator=(CKey& key)
{
    m_pDelegation = key.m_pDelegation;
    key.m_pDelegation = NULL;
}


/*
    CBill类是一个基础类，他提供话单操作的接口，
    所有的实现都必须在子类中完成。
*/
CBill::CBill()
{
    m_uMaxDuration = CFG_MAX_DURATION_DEFAULT; //Added by ZhengYuqun 2003-11-04 SWPD02238
}

CBill::~CBill()
{
    //
}

//获取话单的长度。
INT4 CBill::GetLength()
{
    return 1;
}

//Z30971增加是否支持夏时制接口
//设置合并Duration是否支持夏时制
void CBill::SetDSTFlag(BOOL bFlag)
{
	m_bDSTFlg = bFlag;
}

//Added by ZhengYuqun 2003-11-04 SWPD02238
void CBill::SetMaxDuration(UINT4 uMaxDuration)
{
	m_uMaxDuration = uMaxDuration;
}



/*
    CBillParser管理话单类型和话单结构的细节。
    针对不同的话单类型，程序必须继承CBillParser的子类，
    实现话单类型的管理和话单结构的解析。
*/

CBillParser::CBillParser()
{
    m_pBill = NULL;
}

CBillParser::~CBillParser()
{
    delete m_pBill;
    m_pBill = NULL;
}


