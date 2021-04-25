#include "ewsd_amas_collect_convert.h"
#include "ewsd_ama_collect_convert.h"
#include "../../../include/format_out.h"

//�����ǲɼ������Ļ���CCollectConvertBase�Ķ���
//���캯��
CEWSDAMASCollectConvert::CEWSDAMASCollectConvert(int nSwitchIndex)
                        :CCollectConvertBase(nSwitchIndex) 
{
    //
}

//��������
CEWSDAMASCollectConvert::~CEWSDAMASCollectConvert()
{
    //
}


int CEWSDAMASCollectConvert::Init()
{
    
    int nRet = CCollectConvertBase::Init();

    if (ERR_SUCCESS != nRet)
    {
        return nRet;
    }

    //����S1240��������Ϣ
    CINIFile INIFile(m_szCfgPath);
    if (!INIFile.Open())
    {
        COTRACE("�ɼ�����(%s)ת��ģ��������ļ�(%s)ʧ��", 
              m_szSwitchName, 
              m_szCfgPath);
        return ERR_FAIL;
    }

    char szSecName[CFG_BUF_SIZE];
    SNPRINTF(szSecName,
             sizeof(szSecName),
             "%s%d",
             CFG_CO_SEC_SWITCH,
             m_nSwitchIndex);
    szSecName[CFG_BUF_SIZE - 1] = '\0';

    CEWSDAMACollectConvert* pConvert = NULL;

	int nAMACount = INIFile.GetInt(szSecName, 
                                  CFG_CO_COL_AMA_COUNT,
                                  CFG_CO_COL_AMA_COUNT_DEFAULT);
    for(int i = 1; i <= nAMACount; i++)
    {
        /*
            ȡ��AMA����·��Ϣ
        */
        pConvert = new CEWSDAMACollectConvert(m_nSwitchIndex,
                                                i,
                                                m_pIBillProcessor);
        pConvert->SetCfgPath(m_szCfgPath);
        pConvert->SetSwitchName(m_szSwitchName);

        if((NULL == pConvert) || (pConvert->Init() != ERR_SUCCESS))
        {
            COTRACE("�ɼ�����(%s)����AMAת���Ӷ���(ID=%d)ʧ��", 
                  m_szSwitchName,
                  i);
            delete pConvert;
            return ERR_FAIL;
        }
        m_SubObjMap[i] = pConvert;
    }

    return ERR_SUCCESS;
}

