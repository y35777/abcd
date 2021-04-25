#include "5ess_prefix_collect_convert.h"
#include "../comm_convert_fun.h"
#include "5ess_convert_fun.h"
#include "../../../include/format_out.h"
#include "../collect_convert_base.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
C5essPrefixCollectConvert::C5essPrefixCollectConvert(int nSwitchIndex, int nPrefixIndex, IBillProcessor* pIBillProcessor)
                       : CCollectConvertSubObj(nSwitchIndex, nPrefixIndex, pIBillProcessor)
{
    sprintf(m_szSubObjName, "%s%d", CFG_CO_SEC_PREFIX, nPrefixIndex); 
}

//��������
C5essPrefixCollectConvert::~C5essPrefixCollectConvert()
{
    //
}


int C5essPrefixCollectConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //���ݸ��ݲ�ͬ�Ļ�������ָ����ͬת������
    if (StringiCmp(m_szBillType, CO_BILL_TYPE_AMA))
    {
        m_pConvertFun = Process5essAMAFile;
    }
    else if (StringiCmp(m_szBillType, CO_BILL_TYPE_METER))
    {
        m_pConvertFun = Process5essMMFile;
    }
    else if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "�ɼ�����(%s, Prefix=%d)��⵽��Ч�Ļ�������(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        COTRACE("�ɼ�����(%s, Prefix=%d)��⵽��Ч�Ļ�������(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        return ERR_FAIL;

    }

    return ERR_SUCCESS;
}



