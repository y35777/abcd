#include "../include/type_def.h"
#include "../utility/config.h"
#include "../include/export_dll.h"
#include "bill.h"

/*
    HASH��ʹ��CKey�������������
    ���л����඼�����������̳��Լ������࣬
    ��ʵ��GetKey��operator==������
    CKey�������һ��CKey��������ָ�룬���ʵ�ʵļ��㹤����

    HASH��ʹ��CKey������Ϊ������������CKey�����ָ�롣
    
    ����ָ��Ķ�����ʹ��copy��ʽ����ֵ��
    ����붨��copy���캯���͸�ֵ�����(=)��
*/

UINT4 CKey::GetKey()
{
    return m_pDelegation->GetKey();
}

//�ж�����CKey�����Ƿ���ͬ��
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
    Copy���캯������CKey������Copy�ķ�ʽ��ֵʱ��
    ����m_pDelegationָ�롣
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

//��ֵ���������CKey���󱻸�ֵʱ���ڴ���m_pDelegationָ�롣
void CKey::operator=(CKey& key)
{
    m_pDelegation = key.m_pDelegation;
    key.m_pDelegation = NULL;
}


/*
    CBill����һ�������࣬���ṩ���������Ľӿڣ�
    ���е�ʵ�ֶ���������������ɡ�
*/
CBill::CBill()
{
    m_uMaxDuration = CFG_MAX_DURATION_DEFAULT; //Added by ZhengYuqun 2003-11-04 SWPD02238
}

CBill::~CBill()
{
    //
}

//��ȡ�����ĳ��ȡ�
INT4 CBill::GetLength()
{
    return 1;
}

//Z30971�����Ƿ�֧����ʱ�ƽӿ�
//���úϲ�Duration�Ƿ�֧����ʱ��
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
    CBillParser���������ͺͻ����ṹ��ϸ�ڡ�
    ��Բ�ͬ�Ļ������ͣ��������̳�CBillParser�����࣬
    ʵ�ֻ������͵Ĺ���ͻ����ṹ�Ľ�����
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


