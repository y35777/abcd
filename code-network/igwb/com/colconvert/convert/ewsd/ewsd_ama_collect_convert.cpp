#include "ewsd_ama_collect_convert.h"
#include "../comm_convert_fun.h"
#include "ewsd_convert_fun.h"
#include "../../../include/format_out.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
CEWSDAMACollectConvert::CEWSDAMACollectConvert(int nSwitchIndex, int nAMAIndex, IBillProcessor* pIBillProcessor)
                       : CCollectConvertSubObj(nSwitchIndex, nAMAIndex, pIBillProcessor)
{
    sprintf(m_szSubObjName, "%s%d", CFG_CO_SEC_AMA, nAMAIndex); 
}

//��������
CEWSDAMACollectConvert::~CEWSDAMACollectConvert()
{
    //
}


int CEWSDAMACollectConvert::Init()
{
    int nRet = CCollectConvertSubObj::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //���ݸ��ݲ�ͬ�Ļ�������ָ����ͬת������
/*    if (StringiCmp(m_szBillType, CO_BILL_TYPE_AMA))
    {
        m_pConvertFun = ProcessEWSDAMAFile;
    }
    else if (m_szBillType[0] == '\0')
    {
        m_pConvertFun = ProcessOtherFile;
    }
    else
    {
        COMSGOUT(MSGOUT_LEVEL_URGENT,
             "�ɼ�����(%s, AMA=%d)��⵽��Ч�Ļ�������(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        COTRACE("�ɼ�����(%s, AMA=%d)��⵽��Ч�Ļ�������(%s)",
             m_szSwitchName,
             m_nSubObjIndex,
             m_szBillType);

        return ERR_FAIL;

    }
*/
    return ERR_SUCCESS;
}

