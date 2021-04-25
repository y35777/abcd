#include "s1240_tp_collect_convert.h"
#include "../comm_convert_fun.h"
#include "s1240_convert_fun.h"
#include "s1240_gb_convert_fun.h"
#include "../../../include/format_out.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
CS1240TPCollectConvert::CS1240TPCollectConvert(int nSwitchIndex, int nTPIndex, IBillProcessor* pIBillProcessor)
                       : CCollectConvertSubObj(nSwitchIndex, nTPIndex, pIBillProcessor)
{
    sprintf(m_szSubObjName, "%s%d", CFG_CO_SEC_TP, nTPIndex); 
}

//��������
CS1240TPCollectConvert::~CS1240TPCollectConvert()
{
    //
}


int CS1240TPCollectConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //���ݸ��ݲ�ͬ�Ļ�������ָ����ͬת������
    if (StringiCmp(m_szBillType, CO_BILL_TYPE_GB))
    {
        m_pConvertFun = ProcessS1240GBFile;
    }
/*    else if (StringiCmp(m_szBillType, CO_BILL_TYPE_DETAIL))
    {
        m_pConvertFun = ProcessS1240DetailFile;
    }
    else if (StringiCmp(m_szBillType, CO_BILL_TYPE_METER))
    {
        m_pConvertFun = ProcessS1240MeterFile;
    }
*/
    else if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "�ɼ�����(%s, TP=%d)��⵽��Ч�Ļ�������(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        COTRACE("�ɼ�����(%s, TP=%d)��⵽��Ч�Ļ�������(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        return ERR_FAIL;

    }

    return ERR_SUCCESS;
}

